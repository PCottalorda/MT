#include "SettingWindow.h"

#include <iostream>
#include <limits>
#include <string>


int main(int argc, char* argv[]) {

	sf::Font font;
	if (!font.loadFromFile("arial.ttf")) {
		return EXIT_FAILURE;
	}

	bool loop = true;

	while (loop) {

		// Get the genus.
		std::string buff;
		std::cout << "Enter the genus (or quit with <q>) or <quit>:";
		std::getline(std::cin, buff);
		std::istringstream iss(buff);
		int genus = -1;
		if (buff == std::string("q") || buff == std::string("quit")) {
			// TODO: G�n�rer le .tex

			std::cout << "\tQuitting." << std::endl;
			loop = false;
		}
		else if ((iss >> genus) && (iss.eof())) {
			if (genus <= 0) {
				std::cerr << "\tERROR: The genus must be a strictly positive number! (" << genus << " entered)" << std::endl;
			}
			else {
				SettingWindow window(800, genus, font);
				window.setFramerateLimit(60);
				while (window.isOpen()) {
					window.clear(sf::Color::Black);
					window.updateLoop();
					window.display();
				}
			}
		} else {
			std::cerr << "\tERROR: The string entered is incorrect. <q>, <quit> or a strictly positive number is expected." << std::endl;
		}

	}



	return EXIT_SUCCESS;

}
