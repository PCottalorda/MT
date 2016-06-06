

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


#include "SettingWindow.h"

#include <SFML/Graphics.hpp>
#include "SegmentDrawable.h"
#include "HomologieValue.h"
#include "InternalPositionSystem.h"
#include "IntersectionManager.h"
#include "GraphGenerationWindow.h"

static const float PI = 3.14159265358979323846f;

namespace {

	sf::Vector2f __intersectionPoint(const sf::Vector2f& p1, const sf::Vector2f& p2) {
		sf::Vector2f f(p2.y - p1.y, p1.x - p2.x);
		float r = p1.x * p2.y - p2.x * p1.y;
		if (r == 0) {
			return sf::Vector2f(0, 0);
		} else {
			return f /= r;
		}
	}

	void __rotate(sf::Vector2f& v, float angle) {
		float co = cosf(angle);
		float si = sinf(angle);

		float x = co * v.x - si * v.y;
		float y = si * v.x + co * v.y;

		v.x = x;
		v.y = y;
	}

	float project(const sf::Vector2f& v, const sf::Vector2f& form) {
		return v.x * form.x + v.y * form.y;
	}

	float norm(const sf::Vector2f& v) {
		return sqrtf(v.x*v.x + v.y*v.y);
	}

}

SettingWindow::SettingWindow(unsigned int size, unsigned int genus, const sf::Font& font) :
sf::RenderWindow(sf::VideoMode(size, size), "SettingWindows"),
size(size),
genus(genus),
	binded(true),
	complete(false),
	amplitude(size / 2.0f),
	cursorStandard(10.0f),
	cursorBoundiary(15.0f),
	InternalSys(this),
	font(font),
	indiceFirstPoint(0)
{
	// Exceptions
	if (size <= 0)
		throw std::exception();
	if (genus <= 0)
		throw std::exception();

	srand(time(NULL));

	setVerticalSyncEnabled(true);
	setMouseCursorVisible(false);


	// Compute rationalForms
	float rotAngle = PI / (2*genus);
	sf::Vector2f axis(1.0f, 0.0f);
	for (size_t i = 0; i < 4 * genus; i++) {
		ratFormsF.push_back(axis);
		__rotate(axis, rotAngle);
	}

	std::vector<sf::Vector2f> interPoints;
	for (size_t i = 0; i < ratFormsF.size(); i++) {
		interPoints.push_back(__intersectionPoint(ratFormsF[i], ratFormsF[(i + 1) % ratFormsF.size()]));
	}

	auto sfVector2f_to_Rational2DPoint = [&](const sf::Vector2f& vec)
	{
		Rational x = InternalPositionSystem::floatToRational(vec.x);
		Rational y = InternalPositionSystem::floatToRational(vec.y);
		return Rational2DPoint(x, y);
	};

	std::vector<Rational2DPoint> ratInterPoints;
	for (size_t i = 0; i < interPoints.size() / 2; ++i) {
		ratInterPoints.push_back(sfVector2f_to_Rational2DPoint(interPoints[i]));
	}
	assert(ratInterPoints.size() == interPoints.size() / 2);
	for (size_t i = 0; i < interPoints.size() / 2; ++i) {
		ratInterPoints.push_back(-ratInterPoints[i]);
	}
	assert(ratInterPoints.size() == interPoints.size());

	center = sf::Vector2f(amplitude, amplitude);

	interiorShape.setPointCount(interPoints.size());
	for (size_t i = 0; i < interPoints.size(); i++) {
		interiorShape.setPoint(i, convertInternalToWindow(interPoints[i]));
	}
	interiorShape.setFillColor(sf::Color::Blue);

	for (size_t i = 0; i < interiorShape.getPointCount(); i++) {
		sf::Vector2f p1 = interiorShape.getPoint(i);
		sf::Vector2f p2 = interiorShape.getPoint((i - 1 + interiorShape.getPointCount()) % interiorShape.getPointCount());
		edgeSegments.push_back(SegmentDrawable(p1, p2, 3));
		edgeSegments.back().setFillColor(sf::Color::Yellow);
	}

	// Setting of the parameters for the cursor in normal state
	cursorStandard.setFillColor(sf::Color::Green);
	cursorStandard.setOrigin(10.0f, 10.0f);

	// Setting of the parameters for the cursor on a boundiary
	cursorBoundiary.setFillColor(sf::Color::Red);
	cursorBoundiary.setOrigin(15.0f, 15.0f);

	// Complete the initialization of the related InternalPositionSystem.
	InternalSys.ratFormsF = ratFormsF;
	InternalSys.internalShape = interPoints;
	InternalSys.internalRationalShape = ratInterPoints;

	assert(InternalSys.internalShape.size() == InternalSys.internalRationalShape.size());
	/*
	for (size_t i = 0; i < interPoints.size(); ++i) {
		std::cout << "[" << InternalSys.internalShape[i].x << "," << InternalSys.internalShape[i].y << "]" << std::endl;
	}
	for (size_t i = 0; i < ratInterPoints.size(); ++i) {
		std::cout << InternalSys.internalRationalShape[i] << " | "; InternalSys.internalRationalShape[i].prettyPrint();
	}	
	for (size_t i = 0; i < InternalSys.internalShape.size(); ++i) {
		//std::cout << norm(interPoints[i] - ratInterPoints[i].toSFMLVector2f()) << std::endl;
	}
	*/

	// Initialize the state;
	stateText.setString(stateString());
	stateText.setPosition(10.0f, 10.0f);
	stateText.setColor(sf::Color::White);
	stateText.setStyle(sf::Text::Bold);
	stateText.setFont(font);
	stateText.setCharacterSize(15);

	instructionText.setString("-- Instructions: --\n\t<b>: Bind cursor\n\t<ESC>: Quit the window");
	instructionText.setCharacterSize(15);
	instructionText.setFont(font);
	instructionText.setColor(sf::Color::Red);
	instructionText.setStyle(sf::Text::Bold);
	instructionText.setOrigin(instructionText.getLocalBounds().width, 0.0f);
	instructionText.setPosition(sf::RenderWindow::getSize().x - 20.0f, 10.0f);

	InternalSys.__validity_check();
}

SettingWindow::~SettingWindow() {
}

void SettingWindow::updateLoop() {

	this->draw(interiorShape);

	InternalSys.update();
	sf::Vector2f MousePos(getMousePosition());

	// Enlight the segment to which the mouse is (eventually) linked.
	if (binded && MouseOnBoundiary()) {
		this->draw(edgeSegments[getSideIndex()]);
		this->draw(edgeSegments[(getSideIndex() + (edgeSegments.size() / 2)) % edgeSegments.size()]);
	}

	// Draw the segments of the current poly line.
	drawPointSegsAndPos(MousePos);

	// Draw the cursor accordingly to its position with regard with the boundiaries.
	if (binded) {
		if (MouseOnBoundiary()) {
			cursorBoundiary.setPosition(MousePos);
			draw(cursorBoundiary);
		} else {
			cursorStandard.setPosition(MousePos);
			draw(cursorStandard);
		}
	}


	if (actionConsistent()) {
		if (!InternalSys.internalPoints.empty()) {
			float rad = 12.5f;
			sf::Mouse::setPosition(sf::Vector2i(MousePos), *this);
			sf::CircleShape circ(rad);
			circ.setOrigin(rad, rad);
			circ.setFillColor(sf::Color::Red);
			circ.setPosition(segmentPoints[indiceFirstPoint].point);
			draw(circ);
		}
	}

	draw(instructionText);
	displayState();

	sf::Event event;
	while (pollEvent(event)) {
		switch (event.type) {
			case sf::Event::KeyPressed:
				switch (event.key.code) {
					case sf::Keyboard::B:
						invertBinding();
						break;
					case sf::Keyboard::Escape:
						computeAndGenerateLateX();
						close();
						break;
					default:
						break;
				}
				break;
			case sf::Event::MouseButtonPressed:
				switch (event.mouseButton.button) {
					case sf::Mouse::Left:
						if (actionConsistent()) {
							InternalSys.addPoint();
						}
						break;
					default:
						break;
				}
			default:
				break;
		}
	}

}

sf::Vector2f SettingWindow::convertInternalToWindow(const sf::Vector2f& v) const {
	return 0.9f * amplitude * v + center;
}

sf::Vector2f SettingWindow::convertWindowToInternal(const sf::Vector2f& v) const {
	return (v - center) / (0.9f * amplitude);
}


void SettingWindow::addPoint(const sf::Vector2f& p, bool onBoudiary, bool addSegment) {
	sf::Vector2f _p = convertInternalToWindow(p);
	if (segmentPoints.size() == 0) {
		segmentPoints.push_back(Point(_p, onBoudiary, -1));
	} else {
		if (p != segmentPoints.back().point) {
			sf::Vector2f p_prec = segmentPoints.back().point;
			segmentPoints.push_back(Point(_p, onBoudiary, -1));
			if (addSegment) {
				segments.push_back(SegmentDrawable(p_prec, _p, 3, sf::Color::Magenta));
			}
		}
	}
}

void SettingWindow::drawPointSegsAndPos(const sf::Vector2f& p) {
	for (auto it = segments.begin(); it != segments.end(); ++it) {
		this->draw(*it);
	}

	sf::CircleShape circ(7.0f);
	circ.setFillColor(sf::Color::Magenta);
	circ.setOrigin(7.0f, 7.0f);

	for (auto it = segmentPoints.begin(); it != segmentPoints.end(); ++it) {
		circ.setPosition(it->point);
		this->draw(circ);
	}

	if (actionConsistent()) {
		if (InternalSys.internalPoints.size() != 0) {
			this->draw(SegmentDrawable(p, segmentPoints.back().point, 3, sf::Color::Magenta));
		}
	}
}


bool SettingWindow::actionConsistent() const {
	return binded && !complete && hasFocus();
}

void SettingWindow::invertBinding() {
	setBinding(!binded);
}

void SettingWindow::setBinding(bool b) {
	binded = b;
	setMouseCursorVisible(!binded);
}

std::set<HomologieValue> SettingWindow::computeDualUnitaryBall() const {
	IntersectionManager intersection_manager(this);
	std::set<HomologieValue> vals = intersection_manager.generateValues();
	return vals;
}

bool SettingWindow::MouseOnClosure() const {
	return InternalSys.onClosure;
}

bool SettingWindow::MouseOnBoundiary() const {
	return InternalSys.onBoundiary;
}

int SettingWindow::getSideIndex() const {
	return InternalSys.index;
}

sf::Vector2f SettingWindow::getMousePosition() const {
	return InternalSys.getMouseWindowPosition();
}

void SettingWindow::invert() {
	InternalSys.invert();
}

std::string SettingWindow::stateString() const {
	auto bool_to_string = [](bool b) -> std::string {
		if (b) return "true";
		else return "false";
	};
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

HomologieValue SettingWindow::evaluate(const Segment& seg) const {
	HomologieValue res(2 * genus);
	if (seg.p2.onBoundiary) {
		int homoInd = seg.p2.index % (2 * genus);
		if (seg.p2.index / (2 * genus)) {
			res[homoInd] = 1;
		} else {
			res[homoInd] = -1;
		}
	}
	return res;
}

void SettingWindow::displayState() {
	stateText.setString(stateString());
	draw(stateText);
}
