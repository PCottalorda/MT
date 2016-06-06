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
	void __validity_check() const;

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
