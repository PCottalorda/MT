#pragma once
#include "PointOnBoundiaryWrapper.h"
#include <SFML/System/Vector2.hpp>
#include "Rational2DPoint.h"


class PolyLineCurve;
class SettingWindow;


using Point			= PointOnBoundiaryWrapper< sf::Vector2f >;
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
	RationalPoint invert(const RationalPoint& p) const ;
	std::vector<RationalPoint> exportPoints();
	static Rational floatToRational(float f);

	void exportAndReinitialize();
	void reset();
	void __validity_check() const {
		assert(internalShape.size() == internalRationalShape.size());
		assert(internalShape.size() == ratFormsF.size());
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
