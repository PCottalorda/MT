#include <iostream>
#include <cstdlib>
#include <cmath>
#include <string>
#include <functional>
#include <exception>
#include <fstream>

#include <cmath>


#include <cfloat>
#include <random>
#include <string>


#include <SFML\Window.hpp>
#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\Graphics\Sprite.hpp>


#include "IntersectionManager.h"

#include <stdint.h>

#include "Rational.h"
#include "Rational2DPoint.h"
#include "__internalTree.h"
#include "PrintingWindow.h"
#include "Graph.h"

#include "NormObject.h"
#include "SettingWindow.h"

std::string str("[[5,6],[1,3],[-1,6],[-2,6]]");

NormObject normO(str);


void computeUnitaryBall(std::vector<Rational2DPoint>& intersectionPoints, std::vector<Rational2DForm>& forms, std::function<Rational(const Rational2DPoint&)> norm) {

	Rational2DPoint x1(Rational(1, 1), Rational(0, 1)); // x1 = (1,0)
	Rational2DPoint x2(Rational(0, 1), Rational(1, 1)); // x2 = (0,1)
	Rational2DPoint x3(Rational(-1, 1), Rational(0, 1)); // x3 = (-1,0);

	__internalTree t1(x1, x2, norm);
	__internalTree t2(x2, x3, norm);

	try {
		t1.__internal_computeUnitaryBall();
		t2.__internal_computeUnitaryBall();
	}
	catch (std::exception& e) {
		std::cerr << "ERROR CATCHED!!!!! : " << e.what() << std::endl;
	}

	// We clear the data (not necessary in absolute but just to be shure)
	intersectionPoints.clear();
	forms.clear();

	// We collect and merge the data from the two edges
	try {
		t1.collectData(forms, intersectionPoints);
		t2.collectData(forms, intersectionPoints); // The data are naturally merged by collectData
	}
	catch (std::exception& e) {
		std::cerr << "ERROR CATCHED!!!!! : " << e.what() << std::endl;
	}


	// We add the other points by symetry
	size_t RP2_number_of_points = intersectionPoints.size();
	for (unsigned int i = 0; i < RP2_number_of_points; i++) {
		Rational2DPoint newPoint(Rational(-1, 1) * intersectionPoints[i]); // newPoint = -intersectionPoints[i]
		intersectionPoints.push_back(newPoint);
	}

	// We add the other 1-forms by symetry
	size_t RP2_number_of_forms = forms.size();
	unsigned int i = 0;
	if (forms.front() == (Rational(-1, 1)) * forms.back()) {
		RP2_number_of_forms--;
		i++;
	}
	for (; i < RP2_number_of_forms; i++) { // i was already initialized
		Rational2DForm newForm(Rational(-1, 1) * forms[i]); // newForm = -forms[i]
		forms.push_back(newForm);
	}

};

bool checkUnitaryBall(const std::vector<Rational2DPoint>& vertices, std::function<Rational(const Rational2DPoint&)> norm) {

	if (vertices.size() < 4) {
		return false;
	}

	// We check the vertices
	bool correctResults = true;
	for (auto it = vertices.begin(); it != vertices.end(); ++it) {
		if (norm(*it) != Rational(1, 1)) {
			correctResults = false;
			break;
		}
	}

	if (!correctResults)
		return false;

	// We check the simplicies
	Rational2DPoint prec = vertices[0]; // At this point, vertices.size() >= 4
	for (size_t i = 1; i < vertices.size() + 1; i++) {
		size_t new_index = i % vertices.size();
		Rational2DPoint current = vertices[new_index];

		Rational2DPoint middle = Rational(1, 2) * (prec + current);
		if (norm(middle) != Rational(1, 1)) {
			correctResults = false;
			break;
		}

		// Update prec
		prec = current;
	}


	return correctResults;

}


void generateLaTeX(std::ostream& os, unsigned int nbData = 10) {

	std::string str("[[5,6],[1,3],[-1,6],[-2,6]]");

	NormObject normO(str);

	std::vector<Rational2DPoint> vertices;
	std::vector<Rational2DForm> forms;

	computeUnitaryBall(vertices, forms, normO);
	checkUnitaryBall(vertices, normO);

	int X = 800;
	int Y = 600;
	PrintingWindow window(X, Y, vertices);
	window.setFramerateLimit(5);

	window.clear();
	window.drawNorm(normO);
	window.drawPoly(vertices);
	window.display();
	window.clear(); // Double loop due to double buffering
	window.drawNorm(normO);
	window.drawPoly(vertices);
	window.display();

	//window.generateImage("data.jpg");


	os << "\\documentclass[10pt, a4paper]{article}" << std::endl;
	os << "\\usepackage[latin1]{inputenc}" << std::endl;
	os << "\\usepackage[english]{babel}" << std::endl;
	os << "\\usepackage{amsmath}" << std::endl;
	os << "\\usepackage{amsfonts}" << std::endl;
	os << "\\usepackage{amssymb}" << std::endl;
	os << "\\usepackage{amsthm}" << std::endl;
	os << "\\usepackage{graphicx}" << std::endl;
	os << "\\usepackage{float}" << std::endl;
	os << "\\usepackage[left = 2cm, right = 2cm, top = 2cm, bottom = 2cm]{geometry}" << std::endl;
	os << "\\author{Paul Cottalorda}" << std::endl;
	os << "\\title{Exemple de normes d\'intersections sur le tore [Généré automatiquement]}" << std::endl;
	os << "\\begin{document}" << std::endl;
	os << "\\maketitle" << std::endl;


	for (unsigned int i = 0; i < nbData; i++) {
		int index = i + 1;
		NormObject norm; // The default constructor generates a random NormObject

		vertices.clear();
		forms.clear();

		computeUnitaryBall(vertices, forms, norm);
		checkUnitaryBall(vertices, norm);

		window.resetBoundiaires(generateBoundiaries(vertices));

		window.clear();
		window.drawNorm(norm);
		window.drawPoly(vertices);
		window.display();
		window.clear(); // Double loop due to double buffering
		window.drawNorm(norm);
		window.drawPoly(vertices);
		window.display();

		window.generateImage("data" + std::to_string(index) + ".png");

		os << "\\section{Données " << index << "}" << std::endl;
		os << "\\paragraph{Norme}" << norm << std::endl << std::endl;

		os << "\\paragraph{Points}" << std::endl;
		os << "\\begin{itemize}" << std::endl;
		for (auto it = vertices.begin(); it != vertices.end(); ++it) {
			os << "\t\\item " << *it << std::endl;
		}
		os << "\\end{itemize}" << std::endl;

		os << "\\paragraph{1-formes}" << std::endl;
		os << "\\begin{itemize}" << std::endl;
		for (auto it = forms.begin(); it != forms.end(); ++it) {
			os << "\t\\item " << *it << std::endl;
		}
		os << "\\end{itemize}" << std::endl;

		os << "\\begin{figure}[H]\n\t\\centering\n\t\\includegraphics[width = 15cm]{data" + std::to_string(index) + ".png}\n\\end{figure}" << std::endl << std::endl;
	}

	os << "\\end{document}" << std::endl;

	window.close();
}


int main(int argc, char* argv[]) {


	/*
	// === Initialization
	std::cerr << "Launching programm..." << std::endl;
	std::cerr << "Norm tested : " << normO << std::endl;

	// === Unitary ball
	std::cerr << "ComputeUnitaryBall...";
	std::vector<Rational2DPoint> vertices;
	std::vector<Rational2DForm> forms;

	computeUnitaryBall(vertices, forms, normO);
	std::cerr << "\t\t[Done]" << std::endl;

	// === Test
	std::cerr << "TestingResults...";
	if (checkUnitaryBall(vertices, normO)) {
		std::cerr << "\t\t[Done]" << std::endl;
	}
	else {
		std::cerr << "\t\t[Failure]" << std::endl;
	}

	// === Window creation
	std::cerr << "Creating window...";
	int X = 800;
	int Y = 600;
	PrintingWindow window(X, Y, vertices);
	window.setFramerateLimit(5);
	std::cerr << "\t\t[Done]" << std::endl;


	// === Drawing data


	// === Printing the forms
	for (auto it = forms.begin(); it != forms.end(); ++it)
		std::cerr << *it << std::endl;



	// === Display loop
	window.clear();
	window.drawNorm(normO);
	window.drawPoly(vertices);
	window.display();
	window.clear();		// Double loop due to double buffering
	window.drawNorm(normO);
	window.drawPoly(vertices);
	window.display();

	std::cerr << "Generating image...";
	if (window.generateImage("data.jpg")) {
		std::cerr << "\t\t[Done]" << std::endl;
	}
	else {
		std::cerr << "\t\t[Failure]" << std::endl;
	}

	system("pause");

	*/

	/*
	std::ofstream outFile("gen.tex", std::ios::out);
	if (!outFile.is_open()) {
		return EXIT_FAILURE;
	}
	*/


	//generateLaTeX(outFile);

	//system("pause");

	//outFile.close();


	
	//std::ofstream outFile("C:\\Users\\Paul Cottalorda\\Desktop\\GraphVizOut.txt", std::ios::out);
	//if (!outFile.is_open()) {
	//	return EXIT_FAILURE;
	//}

	/*

	Graph G(4);
	G.addEdge(0, 1);
	G.addEdge(1, 2);
	G.addEdge(3, 3);
	G.addEdge(2, 3);
	G.addEdge(0, 3);
	G.addEdge(0, 2);
	G.addEdge(0, 2);

	std::cerr << G.generateGraphVizString() << std::endl;
	//outFile << G.generateGraphVizString();

	auto printBool = [](bool b) -> std::string { if (b) { return "true"; } else { return "false"; }};

	for (size_t i = 0; i < G.getEdges().size(); i++)
	{
		std::cerr << "Noeud " << i << " : " << *(G.getEdges()[i]) << " : isLoop : " << printBool(G.getEdges()[i]->isLoop()) << std::endl;
	}

	for (size_t i = 0; i < G.getNodes().size(); i++)
	{
		std::cerr << "Noeud " << i << " : " << G.getNodes()[i]->degree() << std::endl;
	}

	G.generateAllEulerianOrientations();

	*/

	//outFile.close();
	//system("pause");

	std::vector<Rational2DPoint> points;
	points.push_back(Rational2DPoint(Rational(3),Rational(0)));
	points.push_back(Rational2DPoint(Rational(3),Rational(2)));
	points.push_back(Rational2DPoint(Rational(6),Rational(-1)));
	points.push_back(Rational2DPoint(Rational(8),Rational(3)));
	points.push_back(Rational2DPoint(Rational(5),Rational(2)));

	std::vector<Segment> segments;
	for (size_t i = 0; i < points.size(); ++i) {
		int index1 = i;
		int index2 = (i + 1) % points.size();
		std::cerr << "[" << index1 << ";" << index2 << "]\n";
		segments.push_back(Segment(points[index1],points[index2]));
	}
	std::cerr << "\n\n";

	
	PolyLineCurve polyline(segments);
	int i = 0;
	for (auto it = polyline.begin(); it != polyline.end(); ++it) {
		std::cerr << segments[i] << std::endl;
		std::cerr << ++i << "/" << polyline.size() << " : " << *it << std::endl;
	}
	std::cerr << "\n\n";
	
	i = 1;
	auto itt = polyline.begin();
	for (auto it = polyline.begin(); it != polyline.end(); ++it) {
		++itt;
		if (itt == polyline.end()) {
			std::cerr << "return\n";
			itt = polyline.begin();
		} else {
			std::cerr << "normal\n";
		}
		auto InterSol = (*itt).intersectionWith((*it));
		std::cerr << "[" << i << "/" << polyline.size() <<"]\n";
		std::cerr << "\t exists: ";
		if (InterSol.exists) {
			std::cerr << "true";
		} else {
			std::cerr << "false";
		}
		std::cerr << std::endl;
		std::cerr << "\t unique: ";
		if (InterSol.isUnique) {
			std::cerr << "true";
		}
		else {
			std::cerr << "false";
		}
		std::cerr << std::endl;
		std::cerr << "\t" << InterSol.inter << "\n\n";

		++i;
	}
	

	system("pause");
	return EXIT_SUCCESS;



	SettingWindow window(800, 4);
	//sf::RenderWindow window(sf::VideoMode(800, 800), "SFML Window");
	window.setFramerateLimit(60);
	while (window.isOpen()) {
		window.clear(sf::Color::Black);
		window.updateLoop();
		window.display();
	}

	return EXIT_SUCCESS;

}
