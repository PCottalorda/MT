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
	friend class IntersectionManager;

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
	RationalPoint invert(const RationalPoint& p) {
		assert(p.onBoundiary);
		assert(internalShape.size() % 2 == 0);
		int ind1 = p.index;
		int ind2 = (ind1 - 1 + internalShape.size()) % internalShape.size();
		Rational2DPoint p1 = internalRationalShape[ind1];
		Rational2DPoint p2 = internalRationalShape[ind2];
		assert(Rational2DPoint::det(p1-p.point,p2-p.point) == 0);
		Rational2DPoint middle = Rational(1,2) * (p1 + p2);
		Rational2DPoint diff_midlle = p.point - middle;
		middle = Rational(-1) * middle; // We go to the opposite!
		Rational2DPoint res = middle + diff_midlle;
		int new_index = (p.index + internalRationalShape.size()/2) % internalRationalShape.size();
		RationalPoint result(res, true, new_index);
		return result;
	}
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
