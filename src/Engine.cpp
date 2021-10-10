#include "Engine.h"

Engine::Engine(int window_width, int window_height, int window_bits, bool fullscreen) {
	this->window_width = window_width;
	this->window_height = window_height;
	this->window_bits = window_bits;
	this->fullscreen = fullscreen;
}

void Engine::createRenderWindow(sf::RenderWindow& window, const char* title) {
    sf::VideoMode screenSize = sf::VideoMode::getDesktopMode();
    this->window_bits = screenSize.bitsPerPixel;
    if (this->fullscreen) {
        this->window_width = screenSize.width;
        this->window_height = screenSize.height;
    }

    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 8;
    settings.majorVersion = 3;
    settings.minorVersion = 3;

    //sf::VideoMode screenSize(window_w, window_h, window_bits);
    window.create(screenSize, title, (fullscreen ? sf::Style::Fullscreen : sf::Style::Close), settings);
    window.setPosition(sf::Vector2i(0, 0));
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    window.setActive(false);
    window.requestFocus();
}

sf::Vector2f Engine::toScreen(double x, double y) {
    const float s = plot_scale * float(this->window_height / 2);
    const float nx = float(this->window_width) * 0.5f + (float(x) - plot_x) * s;
    const float ny = float(this->window_height) * 0.5f + (float(y) - plot_y) * s;

    return sf::Vector2f(nx, ny);
}