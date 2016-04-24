#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>

#include "PointOnBoundiaryWrapper.h"

class SettingWindow;

using Point = PointOnBoundiaryWrapper < sf::Vector2f > ;

class InternalPositionSystem
{
public:
	explicit InternalPositionSystem(SettingWindow* w);
	~InternalPositionSystem();


	bool onBoundiary;
	bool onClosure;
	std::vector<sf::Vector2f> ratFormsF;
	std::vector<sf::Vector2f> internalShape;
	sf::Vector2f MouseInternal;

	std::vector<Point> internalPoints;

	bool authModif;


	int index;
	SettingWindow *window;

	sf::Vector2f getMouseWindowPosition() const;

	void update();
	void readMousePosition();
	void writeMousePosition() const;
	void setMouseInternal(const sf::Vector2f &v);

	void addPoint();

	void invert();

};

