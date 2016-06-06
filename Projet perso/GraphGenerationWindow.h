#pragma once

#include "SFML\Graphics.hpp"
#include <cassert>
#include <set>
#include "HomologieValue.h"
#include "SegmentDrawable.h"
#include <iostream>
#include <fstream>

class GraphGenerationWindow :
	public sf::RenderWindow
{
public:
	GraphGenerationWindow(unsigned int size, std::string filename);;
	~GraphGenerationWindow() {};

	void setToPrint(const std::set<HomologieValue>& val);
	void screenShot();

	void prettyPrint(std::ostream& os = std::cout) const;

	void generateStreamLateX2dim(std::ostream& os, std::string& namePoly, std::string& nameEul) const {
		os << "\\documentclass[10pt, a4paper]{article}" << std::endl;
		os << "\\usepackage[utf8]{inputenc}" << std::endl;
		os << "\\usepackage{graphicx}" << std::endl;
		os << "\\usepackage[left=3cm,top=3cm,bottom=3cm,right=3cm]{geometry}" << std::endl;
		os << "\\begin{document}" << std::endl;
		os << "\\begin{figure}" << std::endl;
		os << "\\centering" << std::endl;
		os << "\\includegraphics[width=7cm]{" << namePoly << "}\\hspace{0.5cm}" << std::endl;
		os << "\\includegraphics[width = 7cm]{" << nameEul << "}" << std::endl;
		os << "\\end{figure}" << std::endl;
		os << "\\paragraph{Eulerian points : }";
		if (hVal.size() != 0) {
			std::vector<HomologieValue> vecVal;
			for (const HomologieValue& val : hVal) {
				vecVal.push_back(val);
			}
			os << vecVal[0];
			for (size_t i = 1; i < hVal.size(); ++i) {
				os << ", " << vecVal[i];
			}
		}
		os << std::endl << "\\end{document}" << std::endl;
	}
	void generateStreamLateXAllDim(std::ostream& os, std::string& namePoly, std::string& nameEul) const {
		os << "\\documentclass[10pt, a4paper]{article}" << std::endl;
		os << "\\usepackage[utf8]{inputenc}" << std::endl;
		os << "\\usepackage{graphicx}" << std::endl;
		os << "\\usepackage[left=3cm,top=3cm,bottom=3cm,right=3cm]{geometry}" << std::endl;
		os << "\\begin{document}" << std::endl;
		os << "\\begin{figure}" << std::endl;
		os << "\\centering" << std::endl;
		os << "\\includegraphics[width=7cm]{" << namePoly << "}" << std::endl;
		os << "\\end{figure}" << std::endl;
		os << "\\paragraph{Eulerian points : }";
		if (hVal.size() != 0) {
			std::vector<HomologieValue> vecVal;
			for (const HomologieValue& val : hVal) {
				vecVal.push_back(val);
			}
			os << vecVal[0];
			for (size_t i = 1; i < hVal.size(); ++i) {
				os << ", " << vecVal[i];
			}
		}
		os << std::endl << "\\end{document}" << std::endl;
	}
	void generateLatex(std::string filename, std::string namePoly,std::string nameEul) {
		std::ofstream file;
		file.open(filename);
		if (file.is_open()) {
			if (dimension() == 2) {
				screenShot();
				generateStreamLateX2dim(file, namePoly, nameEul);
			} else {
				generateStreamLateXAllDim(file, namePoly, nameEul);
			}
			file.close();
		}
	}

	int dimension() const {
		if (hVal.empty()) {
			return 0;
		} else {
			int dim = hVal.begin()->dimension();
			for (const HomologieValue& val : hVal) {
				assert(val.dimension() == dim);
			}
			return dim;
		}
	}

private:
	void drawAxis();
	sf::Vector2f convert(const sf::Vector2f& v) const;
	void GraphGenerationWindow::drawPoint(const sf::Vector2f& v, float radius = 5.0f, sf::Color color = sf::Color::Black, float thicknessRapport = 1.0f / 5.0f) {
		sf::CircleShape P(radius);
		P.setOutlineThickness(radius * thicknessRapport);
		P.setOrigin(P.getRadius(), P.getRadius());
		P.setFillColor(sf::Color::White);
		P.setOutlineColor(color);
		P.setPosition(convert(v));
		draw(P);
	}
	
	void drawLatice();
	void drawHomoVal();
	void update();

	int x_max;
	int y_max;
	unsigned int size;
	std::set<HomologieValue> hVal;
	std::string filename;
};

