#include "SettingWindow.h"



int main(int argc, char* argv[]) {

	sf::Font font;
	if (!font.loadFromFile("arial.ttf")) {
		return EXIT_FAILURE;
	}

	SettingWindow window(800, 1, font);
	window.setFramerateLimit(60);
	while (window.isOpen()) {
		window.clear(sf::Color::Black);
		window.updateLoop();
		window.display();
	}

	return EXIT_SUCCESS;

}
