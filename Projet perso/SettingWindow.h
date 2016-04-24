#pragma once

#include <SFML/Graphics.hpp>

#include "SegmentDrawable.h"
#include "PointOnBoundiaryWrapper.h"
#include "InternalPositionSystem.h"

using Vector2fWrapper = PointOnBoundiaryWrapper<sf::Vector2f>;

class SettingWindow :
	public sf::RenderWindow {

	friend class InternalPositionSystem;

public:

	SettingWindow(unsigned int size, unsigned int genus);;
	~SettingWindow();
	void updateLoop();
	sf::Vector2f convertInternalToWindow(const sf::Vector2f& v) const;
	sf::Vector2f convertWindowToInternal(const sf::Vector2f& v) const;
	void addPoint(const sf::Vector2f& p, bool onBoudiary, bool addSegment = true);
	void drawPointSegsAndPos(const sf::Vector2f& p);
	void cancelMove();
	bool actionConsistent() const;
	void invertBinding();
	void setBinding(bool b);

	// InternalPositionSystem related functions
	bool MouseOnClosure() const;
	bool MouseOnBoundiary() const;
	int getSideIndex() const;
	sf::Vector2f getMousePosition() const;
	void invert();

private:
	int size;
	//std::vector<Rational2DForm> ratForms;
	std::vector<sf::Vector2f> ratFormsF;

	std::vector<sf::Vector2f> points;
	bool binded;
	sf::ConvexShape interiorShape;
	sf::Vector2f center;
	float amplitude;


	std::vector<SegmentDrawable> edgeSegments;


	bool complete;

	sf::CircleShape cursorStandard;
	sf::CircleShape cursorBoundiary;

	std::vector<Vector2fWrapper> segmentPoints;
	std::vector<SegmentDrawable> segments;

	InternalPositionSystem InternalSys;
};
