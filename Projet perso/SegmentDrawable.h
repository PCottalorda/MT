#pragma once

#include <SFML/Graphics.hpp>

class SegmentDrawable : 
	public sf::Drawable
{
public:
	SegmentDrawable(const sf::Vector2f& p1, const sf::Vector2f& p2, float thickness);
	SegmentDrawable(const sf::Vector2f& p1, const sf::Vector2f& p2, float thickness, const sf::Color& color);;
	~SegmentDrawable();

	void computeShape();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setFillColor(const sf::Color& color);

	sf::Vector2f getP1() const;

	sf::Vector2f getP2() const;

private:
	sf::Vector2f p1;
	sf::Vector2f p2;
	float thk;
	sf::RectangleShape shape;
};

