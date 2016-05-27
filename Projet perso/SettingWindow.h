#pragma once

#include <SFML/Graphics.hpp>

#include "SegmentDrawable.h"
#include "PointOnBoundiaryWrapper.h"
#include "InternalPositionSystem.h"

using Vector2fWrapper = PointOnBoundiaryWrapper<sf::Vector2f>;

class SettingWindow :
	public sf::RenderWindow {

	friend class InternalPositionSystem;
	friend class IntersectionManager;

public:

	SettingWindow(unsigned int size, unsigned int genus, const sf::Font& font);
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

	std::string stateString() const {
		auto bool_to_string = [](bool b) -> std::string {if (b) return "true"; else return "false"; };
		std::string str("-- Internal State: --\n");
		str += "\tGenus: " + std::to_string(genus) + "\n";
		str += "\tX position: " + std::to_string(InternalSys.MouseInternal.x) + "\n";
		str += "\tY position: " + std::to_string(InternalSys.MouseInternal.y) + "\n";
		str += "\tBinded: " + bool_to_string(binded) + "\n";
		str += "\tonClosure: " + bool_to_string(MouseOnClosure()) + "\n";
		str += "\tonBoundiary: " + bool_to_string(MouseOnBoundiary());
		if (MouseOnBoundiary()) {
			str += "\n\tindex: " + std::to_string(InternalSys.index);
		}
		return str;
	}

	void displayState() {
		stateText.setString(stateString());
		draw(stateText);
	}

private:
	int size;
	int genus;
	//std::vector<Rational2DForm> ratForms;
	std::vector<sf::Vector2f> ratFormsF;
	std::vector<sf::Vector2f> points;
	bool binded;
	sf::ConvexShape interiorShape;
	sf::Vector2f center;
	float amplitude;
	std::vector<SegmentDrawable> edgeSegments;
	bool complete;
	sf::Text stateText;
	sf::Text instructionText;
	sf::CircleShape cursorStandard;
	sf::CircleShape cursorBoundiary;
	std::vector<Vector2fWrapper> segmentPoints;
	std::vector<SegmentDrawable> segments;
	InternalPositionSystem InternalSys;

	const sf::Font& font;
};
