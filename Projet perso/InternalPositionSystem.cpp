#include "InternalPositionSystem.h"

#include "SettingWindow.h"
#include <iostream>

namespace {
	float project(const sf::Vector2f& v, const sf::Vector2f& form) {
		return v.x * form.x + v.y * form.y;
	}

	float norm2(const sf::Vector2f& v) {
		return v.x * v.x + v.y * v.y;
	}

	float dist2(const sf::Vector2f& v1, const sf::Vector2f& v2) {
		return norm2(v2 - v1);
	}

}

InternalPositionSystem::InternalPositionSystem(SettingWindow* w) :
	window(w),
	onClosure(false),
	authModif(true),
	onBoundiary(false),
	index(0) {
	// ratFormF initialized in SettingWindow::SettingWindow()
	// internalShape initialized in SettingWindow::SettingWindow()
}


void InternalPositionSystem::update() {

	readMousePosition();

	onBoundiary = false;
	onClosure = false;

	//==== Determining the position with regards to the 4n-gon CW-complex of the n-genus surface ====
	sf::Vector2f form = ratFormsF[0];
	float proj = project(MouseInternal, form);
	index = 0;

	for (size_t i = 1; i < ratFormsF.size(); i++) {
		float p = project(MouseInternal, ratFormsF[i]);
		if (p > proj) {
			proj = p;
			form = ratFormsF[i];
			index = i;
		}
	}

	if (proj > 0.97) {
		onBoundiary = true;
	}

	// We recompute the position to "stick" to the boundiary
	if (onBoundiary) {
		sf::Vector2f p1 = internalShape[index];
		sf::Vector2f p2 = internalShape[(index - 1 + internalShape.size()) % internalShape.size()];

		sf::Vector2f X = p2 - p1;

		sf::Vector2f nP = MouseInternal - p1;
		float coeff = project(nP, X) / norm2(X);
		if (coeff < 0.01f) {
			coeff = 0.01f;
		} else if (coeff > 0.99f) {
			coeff = 0.99f;
		}
		sf::Vector2f newPos = coeff * p2 + (1 - coeff) * p1;
		setMouseInternal(newPos);
		writeMousePosition();
	}
	//=================================================


	//==== Determining the position with regards to the first point (if it exists) ====
	if (!internalPoints.empty()) {
		float d2 = dist2(MouseInternal, internalPoints.front().point);
		std::cerr << d2 << std::endl;
		if (d2 < 0.005) {
			onClosure = true;
			if (authModif) {
				setMouseInternal(internalPoints.front().point);
				writeMousePosition();
			}
		}
	}
	//=================================================
}

void InternalPositionSystem::readMousePosition() {
	MouseInternal = window->convertWindowToInternal(sf::Vector2f(sf::Mouse::getPosition(*window)));
}

void InternalPositionSystem::writeMousePosition() const {
	sf::Mouse::setPosition(sf::Vector2i(window->convertInternalToWindow(MouseInternal)), *window);
}

void InternalPositionSystem::setMouseInternal(const sf::Vector2f& v) {
	MouseInternal = v;
}

void InternalPositionSystem::addPoint() {
	/* We check the conditions to see if we can pose our point
	 * The conditions are:
	 *		- The first point of a multi-loop cannot be placed on a boundiary.
	 *		- One complete segment cannot be placed on a boundiary.
	 *		- A point placed on a boundiary is automatically teleported on a segment.
	 *		- Points cannot intersect on a boundiary.
	 */

	if (!authModif) return; // NEED TO HAVE MODIFICATION AUTHORIZATION

	if (onBoundiary) {
		if (internalPoints.empty()) { // The first point is located on the border
			return;
		} else {
			// We check if the last point was onBoundiary
			// TODO: Restrict to same boundiary!
			if (internalPoints.back().index == index) {
				// We do nothing!
			} else {
				// We add the new boundiary point
				// TODO: Add the new points
				internalPoints.push_back(Point(MouseInternal, true, index));
				window->addPoint(MouseInternal, true, true);
				invert();
				internalPoints.push_back(Point(MouseInternal, true, index));
				window->addPoint(MouseInternal, true, false);
			}
		}
	} else {
		// WE REMIND THAT THE FIRST POINT IS NEVER ON BOUNDIARY 
		// (onClosure and onBoundiary are never both true at the same time)

		if (internalPoints.empty()) {
			internalPoints.push_back(Point(MouseInternal, false, -1));
			window->addPoint(MouseInternal, true, false);
		} else {
			if (onClosure) {
				// We do not add any point!
				window->complete = true;
				window->setBinding(false);
				window->addPoint(MouseInternal, true, true);
			} else {
				internalPoints.push_back(Point(MouseInternal, false, -1));
				window->addPoint(MouseInternal, true, true);
			}
		}

	}
}

void InternalPositionSystem::invert() {
	sf::Vector2f middSeg = 0.5f * (window->convertWindowToInternal(window->edgeSegments[index].getP1()) + window->convertWindowToInternal(window->edgeSegments[index].getP2()));
	MouseInternal = MouseInternal - 2.0f * middSeg;
	writeMousePosition();
}


InternalPositionSystem::~InternalPositionSystem() {
}

sf::Vector2f InternalPositionSystem::getMouseWindowPosition() const {
	return window->convertInternalToWindow(MouseInternal);
}
