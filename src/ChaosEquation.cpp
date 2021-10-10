#include "Engine.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <iostream>
#include <string>
#include <stdlib.h>

static GLubyte* pixels = NULL;
static float pointSize = 1.0f;

void screenshot_ppm(const char* filename, unsigned int width, unsigned int height, GLubyte** pixels);
float randNo();
sf::Color getColor();

int main() {
	Engine engine(1920, 1080, 24, true);

	srand(time(NULL));

	sf::RenderWindow window;
	engine.createRenderWindow(window, "Window");
	engine.render(engine, window);

	return 0;
}

// Equation
// x' = -y^2 - t^2 - xy - x - y
// y' = xy - xt - y
void Engine::render(Engine& engine, sf::RenderWindow& window) {
	double t = -3.0,
		rolling_delta = 1e-5,
		speed_mult = 1.0;
	int iterations = 800,
		steps = 500,
		frameSteps = 500;

	std::vector<sf::Vertex> va(iterations * frameSteps);
	std::vector<sf::Vector2f> history(iterations);

	for (int i = 0; i < va.size(); i++) {
		// va[i].color = getRandomColor();
		va[i].color = getColor();
	}

	sf::BlendMode fade(sf::BlendMode::One, sf::BlendMode::One, sf::BlendMode::ReverseSubtract);
	sf::RenderStates renderBlur(fade);
	sf::RectangleShape rect;
	rect.setPosition(0.0f, 0.0f);
	rect.setSize(sf::Vector2f(engine.window_width, engine.window_height));
	sf::Uint8 fadeSpeed = 2;

	glEnable(GL_POINT_SMOOTH);
	glPointSize(pointSize);

	bool paused = false;
	int img = 0;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
				break;
			}
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					window.close();
				}
				if (event.key.code == sf::Keyboard::Insert) {
					/*std::string base = "ss-";
					std::string num = std::to_string(img++);
					std::string ext = ".ppm";*/
					char base[] = "ss-";
					char num[5];
					_itoa(img++, num, 5);
					char ext[] = ".ppm";
					screenshot_ppm(strcat(base, strcat(num, ext)), 1920, 1080, &pixels);
					std::cout << "time: " << t << std::endl;
				}
			}
		}

		if (t > 3.0f) {
			std::cout << "PAUSED!" << std::endl;
			paused = true;
		}

		if (paused) {
			window.display();
			continue;
		}

		// TODO change speed maybe
		// TODO add pause functionality
		// TODO add auto restart functionality
		rect.setFillColor(sf::Color(fadeSpeed, fadeSpeed, fadeSpeed, 0));
		window.draw(rect, renderBlur);

		rolling_delta = rolling_delta * 0.99 + 1e-7;
		for (int step = 0; step < steps; ++step) {
			bool isOffScreen = true;
			double x = t;
			double y = t;
			for (int iteration = 0; iteration < iterations; ++iteration) {
				double xNew = -(x + y + x * y + y * y + t * t);
				double yNew = -(y - x * y + x * t);
				x = xNew;
				y = yNew;
				sf::Vector2f screenPt = engine.toScreen(x, y);
				va[step * iterations + iteration].position = screenPt;

				if (screenPt.x > 0.0f && screenPt.y > 0.0f && screenPt.x < engine.window_width && screenPt.y < engine.window_height) {
					float dx = history[iteration].x - float(x);
					float dy = history[iteration].y - float(y);
					double dist = std::sqrt(dx * dx + dy * dy);
					rolling_delta = std::min(rolling_delta, std::max((1e-5 * speed_mult) / (dist * 5 * 1e2 + 1e-5), 1e-7 * speed_mult));
					isOffScreen = false;
					history[iteration].x = float(x);
					history[iteration].y = float(y);
				}
			}
			if (isOffScreen) {
				t += 0.01;
			} else {
				t += rolling_delta;
			}
		}

		window.draw(va.data(), va.size(), sf::PrimitiveType::Points);
		window.display();
	}
}

void screenshot_ppm(const char* filename, unsigned int width,
	unsigned int height, GLubyte** pixels) {
	size_t i, j, cur;
	const size_t format_nchannels = 3;
	FILE* f = fopen(filename, "w");
	fprintf(f, "P3\n%d %d\n%d\n", width, height, 255);
	*pixels = (GLubyte*)realloc(*pixels, format_nchannels * sizeof(GLubyte) * width * height);

	int c = 0;
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, *pixels);
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			cur = format_nchannels * ((height - i - 1) * width + j);
			fprintf(f, "%3d %3d %3d ", (*pixels)[cur], (*pixels)[cur + 1], (*pixels)[cur + 2]);
		}
		fprintf(f, "\n");
	}

	fclose(f);
}

float randNo() {
	return (float)rand() / RAND_MAX;
}

sf::Color getColor() {
	return sf::Color(154, 203, 245, 11);
}