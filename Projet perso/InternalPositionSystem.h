#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>

#include "PointOnBoundiaryWrapper.h"
#include "Rational2DPoint.h"

class SettingWindow;


using Point			= PointOnBoundiaryWrapper < sf::Vector2f >;
using RationalPoint = PointOnBoundiaryWrapper < Rational2DPoint > ;

class InternalPositionSystem {

	friend class SettingWindow;

public:
	explicit InternalPositionSystem(SettingWindow* w);
	~InternalPositionSystem();

	sf::Vector2f getMouseWindowPosition() const;
	void update();
	void readMousePosition();
	void writeMousePosition() const;
	void setMouseInternal(const sf::Vector2f& v);
	void addPoint();
	void invert();
	std::vector<RationalPoint> exportPoints();
	static Rational floatToRational(float f);;

private:
	bool onBoundiary;
	bool onClosure;
	std::vector<sf::Vector2f> ratFormsF;
	std::vector<sf::Vector2f> internalShape;
	std::vector<Rational2DPoint> internalRationalShape;
	sf::Vector2f MouseInternal;
	std::vector<Point> internalPoints;
	bool authModif;
	int index;
	SettingWindow* window;

};
