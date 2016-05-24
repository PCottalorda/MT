#include "SettingWindow.h"



int main(int argc, char* argv[]) {


	SettingWindow window(800, 1);
	window.setFramerateLimit(60);
	while (window.isOpen()) {
		window.clear(sf::Color::Black);
		window.updateLoop();
		window.display();
	}

	return EXIT_SUCCESS;

}
