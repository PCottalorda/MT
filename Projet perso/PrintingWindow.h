#pragma once

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


Boundiaries generateBoundiaries(const std::vector<Rational2DPoint> &vertices);


class PrintingWindow :
	public sf::RenderWindow
{
public:

	sf::Vector2f windowCoor(const Rational2DPoint& r);

	void drawPoly(const std::vector<Rational2DPoint>& v);

	PrintingWindow(unsigned int X, unsigned int Y, const std::vector<Rational2DPoint> &vertices);
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

