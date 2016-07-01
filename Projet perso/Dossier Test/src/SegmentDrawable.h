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


#include <SFML/Graphics.hpp>

class SegmentDrawable :
	public sf::Drawable {
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
