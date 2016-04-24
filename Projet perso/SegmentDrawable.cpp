#include "SegmentDrawable.h"

#include <SFML/System.hpp>
#include <cmath>

static const float PI = 3.14159265358979323846f;


SegmentDrawable::SegmentDrawable(const sf::Vector2f& p1, const sf::Vector2f& p2, float thickness):
	p1(p1),
	p2(p2),
	thk(thickness) {
	computeShape();
}

SegmentDrawable::SegmentDrawable(const sf::Vector2f& p1, const sf::Vector2f& p2, float thickness, const sf::Color& color):
	SegmentDrawable(p1,p2,thickness) {
	shape.setFillColor(color);
}

SegmentDrawable::~SegmentDrawable()
{
}

void SegmentDrawable::computeShape() {
	sf::Vector2f df = p2 - p1;
	float rot = atan2f(df.y, df.x) * 180 / PI;
	shape.setSize(sf::Vector2f(sqrtf(df.x*df.x + df.y*df.y), thk));
	shape.setOrigin(0, thk / 2.0f);
	shape.setPosition(p1);
	shape.setRotation(rot);
}

void SegmentDrawable::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(shape, states);
}

void SegmentDrawable::setFillColor(const sf::Color& color) {
	shape.setFillColor(color);
}

sf::Vector2f SegmentDrawable::getP1() const {
	return p1;
}

sf::Vector2f SegmentDrawable::getP2() const {
	return p2;
}
