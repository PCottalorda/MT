#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>

#include "PointOnBoundiaryWrapper.h"
#include "Rational2DPoint.h"
#include "IntersectionManager.h"

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
	RationalPoint invert(const RationalPoint& p);
	std::vector<RationalPoint> exportPoints();
	static Rational floatToRational(float f);

	 PolyLineCurve exportAndReinitialize() {
		std::vector<RationalPoint> res = exportPoints();
		assert(!res.empty());
		internalPoints.clear();
		std::vector<Segment> segs;
		RationalPoint prev = res.front();
		for (unsigned int i = 1; i < res.size(); ++i) {
			RationalPoint current = res[i];
			Segment seg(prev, current);
			segs.push_back(seg);
			if (current.onBoundiary) {
				++i;
				prev = res[i];
			} else {
				prev = current;
			}
		}
		return PolyLineCurve(segs);
	}

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
