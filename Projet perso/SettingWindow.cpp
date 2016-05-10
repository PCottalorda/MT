#include "SettingWindow.h"


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

}

SettingWindow::SettingWindow(unsigned size, unsigned genus) :
	sf::RenderWindow(sf::VideoMode(size, size), "SettingWindows"),
	size(size),
	binded(true),
	complete(false),
	amplitude(size / 2.0f),
	cursorStandard(10.0f),
	cursorBoundiary(15.0f),
	InternalSys(this) {
	// Exceptions
	if (size <= 0)
		throw std::exception();
	if (genus <= 0)
		throw std::exception();


	setVerticalSyncEnabled(true);
	setMouseCursorVisible(false);


	// Compute rationalForms
	float rotAngle = PI / genus;
	sf::Vector2f axis(1.0f, 0.0f);
	for (size_t i = 0; i < 2 * genus; i++) {
		ratFormsF.push_back(axis);
		__rotate(axis, rotAngle);
	}

	std::vector<sf::Vector2f> interPoints;
	for (size_t i = 0; i < ratFormsF.size(); i++) {
		interPoints.push_back(__intersectionPoint(ratFormsF[i], ratFormsF[(i + 1) % ratFormsF.size()]));
	}

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


	if (actionConsistent())
		if (segments.size() > 0) {
			sf::Mouse::setPosition(sf::Vector2i(MousePos), *this);
			sf::CircleShape circ(20);
			circ.setOrigin(20, 20);
			circ.setFillColor(sf::Color::Cyan);
			circ.setPosition(segmentPoints.front().point);
			draw(circ);
		}

	sf::Event event;
	while (this->pollEvent(event)) {
		switch (event.type) {
			case sf::Event::KeyPressed:
				switch (event.key.code) {
					case sf::Keyboard::B:
						invertBinding();
						break;
					case sf::Keyboard::C:
						cancelMove();
						break;
					case sf::Keyboard::Escape:
						this->close();
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
		segmentPoints.push_back(Vector2fWrapper(_p, onBoudiary, -1));
	} else {
		if (p != segmentPoints.back().point) {
			sf::Vector2f p_prec = segmentPoints.back().point;
			segmentPoints.push_back(Vector2fWrapper(_p, onBoudiary, -1));
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
		if (segmentPoints.size() != 0) {
			this->draw(SegmentDrawable(p, segmentPoints.back().point, 3, sf::Color::Magenta));
		}
	}
}

void SettingWindow::cancelMove() {
	if (segmentPoints.size() == 0) {
		// Nothing to do!
	} else if (segmentPoints.size() == 1) {
		segmentPoints.pop_back();
	} else {
		segments.pop_back();
		Vector2fWrapper last = segmentPoints.back();
		if (last.onBoundiary) {
			segmentPoints.pop_back();
			segmentPoints.pop_back();
		} else {
			segmentPoints.pop_back();
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
