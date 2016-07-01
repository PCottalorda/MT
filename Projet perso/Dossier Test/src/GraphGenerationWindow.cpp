

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


#include "GraphGenerationWindow.h"
#include <iostream>

GraphGenerationWindow::GraphGenerationWindow(unsigned size, std::string filename): sf::RenderWindow(sf::VideoMode(size, size), ""), size(size), filename(filename), x_max(0), y_max(0) {
	setVisible(false);
}

void GraphGenerationWindow::setToPrint(const std::set<HomologieValue>& val) {
	hVal = val;
	x_max = 0;
	y_max = 0;
	//assert(std::is_sorted(hVal.begin(), hVal.end(), [](const HomologieValue& v1, const HomologieValue& v2) {return v1[0] < v2[0]; }));
	//assert(std::all_of(hVal.begin(), hVal.end(), [](const HomologieValue& val){return val.dimension() == 2; }));
	std::for_each(hVal.begin(), hVal.end(), [&](const HomologieValue& val) {
		              int x = val[0];
		              int y = val[1];
		              x_max = std::max(x_max, x);
		              y_max = std::max(y_max, y);
	              });
	x_max += 2;
	y_max += 2;
}

void GraphGenerationWindow::drawAxis() {
	SegmentDrawable Y_axis(sf::Vector2f(size / 2.0f, 0.0f), sf::Vector2f(size / 2.0f, size), 2.5, sf::Color::Black);
	SegmentDrawable X_axis(sf::Vector2f(0.0f, size / 2.0f), sf::Vector2f(size, size / 2.0f), 2.5, sf::Color::Black);
	draw(X_axis);
	draw(Y_axis);
}

sf::Vector2f GraphGenerationWindow::convert(const sf::Vector2f& v) const {
	float half_size = size / 2.0f;
	sf::Vector2f center(half_size, half_size);
	float x_coeff = half_size / static_cast<float>(x_max) ;
	float y_coeff = half_size / static_cast<float>(y_max) ;
	sf::Vector2f dilatedVec(v);
	dilatedVec.x = dilatedVec.x * x_coeff;
	dilatedVec.y = - dilatedVec.y * y_coeff;
	dilatedVec += center;
	return dilatedVec;
}



void GraphGenerationWindow::drawLatice() {
	for (int i = -x_max; i <= x_max; ++i)
		for (int j = -y_max; j <= y_max; ++j)
			drawPoint(sf::Vector2f(i, j));
}

void GraphGenerationWindow::drawHomoVal() {
	for (const HomologieValue& val : hVal) {
		sf::Vector2f pos(val[0], val[1]);
		drawPoint(pos, 10, sf::Color::Blue, 0.5f);
	}
}

void GraphGenerationWindow::update() {
	clear(sf::Color::White);
	drawAxis();
	drawLatice();
	drawHomoVal();
	display();
}

void GraphGenerationWindow::screenShot() {
	update();
	update();
	sf::Image screen = capture();
	screen.saveToFile(filename);
}

void GraphGenerationWindow::prettyPrint(std::ostream& os) const {
	os << "x_max: " << x_max << std::endl;
	os << "y_max: " << y_max << std::endl;
	os << "size : " << size << std::endl;
}
