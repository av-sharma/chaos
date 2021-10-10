#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

class Engine {
private:
	int window_width = 1920;
	int window_height = 1080;
	int window_bits = 128;
	bool fullscreen = true;

public:
	float plot_scale = 0.8f;
	float plot_x = 0.0f;
	float plot_y = 0.0f;
	sf::ContextSettings settings;

	Engine(int width, int height, int bits, bool fullscreen);
	void createRenderWindow(sf::RenderWindow& window, const char* title);
	sf::Vector2f toScreen(double x, double y);
	virtual void render(Engine& renderer, sf::RenderWindow& window);
};