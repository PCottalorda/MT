

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


#include "PrintingWindow.h"


sf::Color genColor(double x) {

	x = std::max(0.0, std::min(x, 1.0));

	sf::Color out_color;

	if (x < 1.0 / 3.0) {
		out_color.r = 0;
		out_color.g = (uint8_t)(255 * 3 * x);
		out_color.b = 255;
	} else if (x < 2.0 / 3.0) {
		out_color.r = (uint8_t)(255 * 3 * (x - 1.0 / 3.0));
		out_color.g = 255;
		out_color.b = (uint8_t)(255 * 3 * (2.0 / 3.0 - x));
	} else {
		out_color.r = 255;
		out_color.g = (uint8_t)(255 * 3 * (1.0 - x));
		out_color.b = 0;
	}

	return out_color;
}


Boundiaries generateBoundiaries(const std::vector<Rational2DPoint>& vertices) {
	Rational __x_max(vertices[0].x);
	Rational __x_min(vertices[0].x);
	Rational __y_max(vertices[0].y);
	Rational __y_min(vertices[0].y);

	for (unsigned int i = 0; i < vertices.size(); i++) {
		Rational x = vertices[i].x;
		Rational y = vertices[i].y;

		if (x > __x_max) {
			__x_max = x;
		} else if (x < __x_min) {
			__x_min = x;
		}
		if (y > __y_max) {
			__y_max = y;
		} else if (y < __y_min) {
			__y_min = y;
		}
	}

	float a = static_cast<float>(Rational(1, 1));

	Rational x_rat_amp = __x_max - __x_min;
	Rational y_rat_amp = __y_max - __y_min;
	Rational x_rat_sum = __x_max + __x_min;
	Rational y_rat_sum = __y_max + __y_min;

	float x_amp = 1.2f * static_cast<float>(x_rat_amp);
	float y_amp = 1.2f * static_cast<float>(y_rat_amp);

	float x_center = 0.5f * static_cast<float>(x_rat_sum);
	float y_center = 0.5f * static_cast<float>(x_rat_sum);

	float half_x_amp = x_amp * 0.5f;
	float half_y_amp = y_amp * 0.5f;

	float x_min = x_center - half_x_amp;
	float x_max = x_center + half_x_amp;
	float y_min = y_center - half_y_amp;
	float y_max = y_center + half_y_amp;

	Boundiaries bound;

	bound.x_max = x_max;
	bound.x_min = x_min;
	bound.y_min = y_min;
	bound.y_max = y_max;

	return bound;
};


sf::Vector2f PrintingWindow::windowCoor(const Rational2DPoint& r) {
	float __x = static_cast<float>(r.x) - x_min;
	float __y = static_cast<float>(r.y) - y_min;

	float x_coeff = x_amp / X;
	float y_coeff = y_amp / Y;

	return sf::Vector2f(__x / x_coeff, __y / y_coeff);
}

void PrintingWindow::drawPoly(const std::vector<Rational2DPoint>& v) {
	float size_dot = std::max((float)(std::min(X, Y)) / 100.0f, 1.0f);
	sf::CircleShape dot(size_dot);
	dot.setOrigin(sf::Vector2f(dot.getRadius(), dot.getRadius()));
	dot.setFillColor(sf::Color(148, 0, 211));

	sf::VertexArray arr(sf::LinesStrip, v.size() + 1);
	for (unsigned int i = 0; i < v.size() + 1; i++) {
		arr[i] = windowCoor(v[i % v.size()]);
		arr[i].color = sf::Color::White;
		arr[i].color = sf::Color(148, 0, 211);
	}

	this->draw(arr);


	for (unsigned int i = 0; i < v.size(); i++) {
		sf::Vector2f pos = windowCoor(v[i]);
		dot.setPosition(pos);
		this->draw(dot);
	}


};

PrintingWindow::PrintingWindow(unsigned int X, unsigned int Y, float x_min, float x_max, float y_min, float y_max) :
	RenderWindow(sf::VideoMode(X, Y), "Visualization window"),
	x_min(x_min),
	x_max(x_max),
	y_min(y_min),
	y_max(y_max),
	X(X),
	Y(Y) {
	if (X == 0 || Y == 0) {
		// TODO: Generate exception;
		throw std::exception();
	}

	if (x_min >= x_max || y_min >= y_max) {
		// TODO: Generate exception;
		throw std::exception();
	}

	x_amp = x_max - x_min;
	y_amp = y_max - y_min;

	x_coeff = X / x_amp;
	y_coeff = Y / y_amp;

	x_center = 0.5f * (x_max + x_min);
	y_center = 0.5f * (y_max + y_min);

	computeUpperLayout();
};

void PrintingWindow::drawNorm(std::function<float(float x, float y)> norm) {
	float x_amp = x_max - x_min;
	float y_amp = y_max - y_min;

	float x_coeff = x_amp / X;
	float y_coeff = y_amp / Y;

	float x = X * x_coeff;
	float y = Y * y_coeff;

	float x_offset = x / 2.0f;
	float y_offset = y / 2.0f;

	float upperBound = 0.0f;
	upperBound = std::max(upperBound, norm(x_min, y_min));
	upperBound = std::max(upperBound, norm(x_min, y_max));
	upperBound = std::max(upperBound, norm(x_max, y_min));
	upperBound = std::max(upperBound, norm(x_max, y_max));

	sf::Image im;
	im.create(X, Y);

	for (unsigned int i = 0; i < X; i++)
		for (unsigned int j = 0; j < Y; j++) {
			float x = x_min + i * x_coeff;
			float y = y_min + j * y_coeff;

			float __eval_norm = norm(x, y);

			sf::Color cc = genColor(__eval_norm / upperBound);
			if (abs(__eval_norm - 1.0f) <= 0.01f) {
				cc = sf::Color::Black;
			}
			im.setPixel(i, j, cc);
		}


	sf::Sprite sprite;
	sf::Texture text;
	text.loadFromImage(im);
	sprite.setTexture(text);

	this->draw(sprite);
}

void PrintingWindow::display() {
	this->draw(UpperLayoutSprite);
	RenderWindow::display();
};

bool PrintingWindow::generateImage(const std::string& filename) {
	sf::Image screenshot = this->capture();
	return screenshot.saveToFile(filename);
};


PrintingWindow::PrintingWindow(unsigned int X, unsigned int Y, const Boundiaries& bound) :
	PrintingWindow(X, Y, bound.x_min, bound.x_max, bound.y_min, bound.y_max) {
};


PrintingWindow::PrintingWindow(unsigned int X, unsigned int Y, const std::vector<Rational2DPoint>& vertices) :
	PrintingWindow(X, Y, generateBoundiaries(vertices)) {
};

void PrintingWindow::resetBoundiaires(const Boundiaries& bounds) {

	x_min = bounds.x_min;
	x_max = bounds.x_max;
	y_min = bounds.y_min;
	y_max = bounds.y_max;


	if (x_min >= x_max || y_min >= y_max) {
		// TODO: Generate exception;
		throw std::exception();
	}

	x_amp = x_max - x_min;
	y_amp = y_max - y_min;

	x_coeff = X / x_amp;
	y_coeff = Y / y_amp;

	x_center = 0.5f * (x_max + x_min);
	y_center = 0.5f * (y_max + y_min);

	sf::Vector2f center(x_center, y_center);
	sf::Vector2f size(x_amp, y_amp);

	computeUpperLayout();
}

void PrintingWindow::computeUpperLayout() {
	sf::Vector2f center(x_center, y_center);
	sf::Vector2f size(x_amp, y_amp);


	//=========== Creating and setting the upper layout =============
	// UpperLayout is a member.
	UpperLayout.create(X, Y);
	UpperLayout.clear(sf::Color(0, 0, 0, 0)); // Transparent layout


	// Compute a "good size" for the integral latice.
	float X_nb_int_pt = ((float)X) / (20.0f * x_amp);
	float Y_nb_int_pt = ((float)Y) / (20.0f * x_amp);

	float int_point_rad = std::max(1.0f, std::min(X_nb_int_pt, Y_nb_int_pt));
	int_point_rad = 4.0f;
	float origin_point_rad = 2 * int_point_rad;

	sf::CircleShape int_point(int_point_rad);
	int_point.setFillColor(sf::Color::Black);
	int_point.setOrigin(int_point.getRadius(), int_point.getRadius());

	sf::CircleShape origin_point(origin_point_rad);
	origin_point.setOrigin(origin_point.getRadius(), origin_point.getRadius());
	sf::Vector2f origin(-x_min * x_coeff, -y_min * y_coeff);
	origin_point.setPosition(origin);
	origin_point.setFillColor(sf::Color::Black);

	//=========== Drawing the integral lattice =====================
	int int_x_min = (int)x_min - 1;
	int int_x_max = (int)x_max + 1;

	int int_y_min = (int)y_min - 1;
	int int_y_max = (int)y_max + 1;

	int numberOfPoints = (int_x_max - int_x_min + 1) * (int_y_max - int_y_min + 1);
	int counter = 1;

	//std::cerr << "In..." << std::endl;

	for (int i = int_x_min; i <= int_x_max; i++) {
		for (int j = int_y_min; j <= int_y_max; j++) {
			float __x = (float)i;
			float __y = (float)j;

			float x = (__x - x_min) * x_coeff;
			float y = (__y - y_min) * y_coeff;
			// Convert to frame coordinates


			int_point.setPosition(x, y);
			UpperLayout.draw(int_point);

			counter++;
		}
	}

	UpperLayout.draw(origin_point); // Draw the origin point

	sf::RectangleShape X_axis(sf::Vector2f((float)X, 4.0f));
	X_axis.setOrigin(X_axis.getSize() / 2.0f);
	X_axis.setPosition(origin);
	X_axis.setFillColor(sf::Color::Black);

	UpperLayout.draw(X_axis);

	sf::RectangleShape Y_axis(sf::Vector2f(4.0f, (float)Y));
	Y_axis.setOrigin(Y_axis.getSize() / 2.0f);
	Y_axis.setPosition(origin);
	Y_axis.setFillColor(sf::Color::Black);

	UpperLayout.draw(Y_axis);


	//=========== Set the sprite  for the upper layout ============
	UpperLayoutSprite.setTexture(UpperLayout.getTexture());
}
