

/*****************************************************************************
*                                                                            *
*  Copyright 2016 Paul Cottalorda                                            *
*                                                                            *
*  Licensed under the Apache License, Version 2.0 (the "License");           *
*  you may not use this file except in compliance with the License.          *
*  You may obtain a copy of the License at                                   *
*                                                                            *
*      http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                            *
*  Unless required by applicable law or agreed to in writing, software       *
*  distributed under the License is distributed on an "AS IS" BASIS,         *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
*  See the License for the specific language governing permissions and       *
*  limitations under the License.                                            *
*                                                                            *
******************************************************************************/


#include "SettingWindow.h"

#include <iostream>
#include <limits>
#include <string>
#include "GraphGenerationWindow.h"


int main(int argc, char* argv[]) {

	sf::Font font;
	if (!font.loadFromFile("arial.ttf")) {
		return EXIT_FAILURE;
	}

	/*
	std::set<HomologieValue> homoVal;
	for (int i = 0; i < 3; ++i) {
		HomologieValue pos(2);
		pos[0] = i;
		pos[1] = i;
		homoVal.insert(pos);
		homoVal.insert(-pos);
	}

	GraphGenerationWindow win(800, "GenData/test.png");
	win.setToPrint(homoVal);
	win.screenShot();
	win.generateStreamLateX(std::cout, std::string("bite"), std::string("ouille"));

	system("pause");
	*/
	
	bool loop = true;
	while (loop) {

		// Get the genus.
		std::string buff;
		std::cout << "Enter the genus (or quit with <q> or <quit>):";
		std::getline(std::cin, buff);
		std::istringstream iss(buff);
		int genus = -1;
		if (buff == std::string("q") || buff == std::string("quit")) {
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
