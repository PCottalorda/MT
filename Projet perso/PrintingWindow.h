#pragma once


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


#include <vector>
#include <SFML\Graphics.hpp>
#include <cmath>
#include <functional>


#include "Rational.h"
#include "Rational2DPoint.h"


sf::Color genColor(double x);

using Boundiaries = struct {
	float x_min;
	float x_max;
	float y_min;
	float y_max;
};


Boundiaries generateBoundiaries(const std::vector<Rational2DPoint>& vertices);


class PrintingWindow :
	public sf::RenderWindow {
public:

	sf::Vector2f windowCoor(const Rational2DPoint& r);

	void drawPoly(const std::vector<Rational2DPoint>& v);

	PrintingWindow(unsigned int X, unsigned int Y, const std::vector<Rational2DPoint>& vertices);
	PrintingWindow(unsigned int X, unsigned int Y, const Boundiaries& bound);

	void drawNorm(std::function<float(float x, float y)> norm);
	bool generateImage(const std::string& filename);

	void resetBoundiaires(const Boundiaries& bounds);
	void computeUpperLayout();

	void display();

private:

	PrintingWindow(unsigned int X, unsigned int Y, float x_min, float x_max, float y_min, float y_max);

	sf::RenderTexture UpperLayout;
	sf::Sprite UpperLayoutSprite;

	float x_min;
	float x_max;
	float y_min;
	float y_max;

	float x_amp;
	float y_amp;

	float x_coeff;
	float y_coeff;

	float x_center;
	float y_center;

	unsigned int X;
	unsigned int Y;
};
