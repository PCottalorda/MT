#pragma once
#include <SFML/Graphics.hpp>
#include "InternalPositionSystem.h"
#include "IntersectionManager.h"
#include "SegmentDrawable.h"
#include "HomologieValue.h"
#include "EulerianOrientation.h"
#include "GraphGenerationWindow.h"

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
	bool actionConsistent() const;
	void invertBinding();
	void setBinding(bool b);
	void resetIndiceFirstPoint() {
		indiceFirstPoint = segmentPoints.size() - 1;
	}
	void screenCapture(const std::string& filename) {
		auto minimalDraw = [&]() {
			clear();
			draw(interiorShape);
			for (auto it = segments.begin(); it != segments.end(); ++it) {
				draw(*it);
			}

			sf::CircleShape circ(7.0f);
			circ.setFillColor(sf::Color::Magenta);
			circ.setOrigin(7.0f, 7.0f);

			for (auto it = segmentPoints.begin(); it != segmentPoints.end(); ++it) {
				circ.setPosition(it->point);
				draw(circ);
			}
			display();
		};
		// 2 calls of minimal draw for the double buffering;
		minimalDraw();
		minimalDraw();
		sf::Image screenShot = capture();
		screenShot.saveToFile(filename);
	}

	std::set<HomologieValue> computeDualUnitaryBall() const;

	void computeAndGenerateLateX() {
		std::cout << "Save image of the multi-lacet..." << std::endl;
		unsigned int indice = rand();	std::string namePoly = "GenData\\Screenshot_" + std::to_string(indice) + ".png";
		std::string intNamePoly = "Screenshot_" + std::to_string(indice) + ".png";
		std::string nameBall = "GenData\\Lattice_" + std::to_string(indice) + ".png";
		std::string intNameBall = "Lattice_" + std::to_string(indice) + ".png";
		std::string nameTex = "GenData\\output_" + std::to_string(indice) + ".tex";
		screenCapture(namePoly);
		std::cout << "Done." << std::endl;
		std::cout << "Compute Unitary Ball (this can take a while)..." << std::endl;
		std::set<HomologieValue> vals = computeDualUnitaryBall();
		GraphGenerationWindow graphWin(800, nameBall);
		graphWin.setToPrint(vals);
		graphWin.generateLatex(nameTex, intNamePoly, intNameBall);
		graphWin.close();
		std::cout << "Done." << std::endl;
	}

	// InternalPositionSystem related functions
	bool MouseOnClosure() const;
	bool MouseOnBoundiary() const;
	int getSideIndex() const;
	sf::Vector2f getMousePosition() const;
	void invert();

	std::string stateString() const;
	HomologieValue evaluate(const Segment& seg) const;
	void displayState();

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
	std::vector<Point> segmentPoints;
	std::vector<SegmentDrawable> segments;
	InternalPositionSystem InternalSys;

	std::vector<PolyLineCurve> lineCurvesSet;
	int indiceFirstPoint;
	const sf::Font& font;
};
