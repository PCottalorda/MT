


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



/*
sf::Joystick::update();

int indiceManette = 0;
bool noConnection = true;

// Détection des manettes
for (int i = 0; i < 8; i++) {
if (sf::Joystick::isConnected(i)) {
std::cout << "Manette connectée sur le port " << i << std::endl;
noConnection = false;
indiceManette = i;
break;
}
}


std::cout << sf::Joystick::Count << std::endl;
sf::Vector2i buff;

bool keepLoop = true;

std::string button[10];

button[0] = "A";
button[1] = "B";
button[2] = "X";
button[3] = "Y";
button[4] = "LB";
button[5] = "RB";
button[6] = "SELECT";
button[7] = "START";
button[8] = "LEFT";
button[9] = "RIGHT";

sf::Joystick::Identification id = sf::Joystick::getIdentification(indiceManette);
std::cout << "Name:\t" << id.name.toAnsiString() << std::endl;
std::cout << "ProductId:\t" << id.productId << std::endl;
std::cout << "VendorId:\t" << id.vendorId << std::endl;

std::cout << "Ten " << XBoxJoystick::A << std::endl;



while (keepLoop) {

sf::Joystick::update();


float Xposition = sf::Joystick::getAxisPosition(indiceManette, sf::Joystick::X);
float Yposition = sf::Joystick::getAxisPosition(indiceManette, sf::Joystick::Y);
float Uposition = sf::Joystick::getAxisPosition(indiceManette, sf::Joystick::U);
float Vposition = sf::Joystick::getAxisPosition(indiceManette, sf::Joystick::V);
float PovXposition = sf::Joystick::getAxisPosition(indiceManette, sf::Joystick::PovX);
float PovYposition = sf::Joystick::getAxisPosition(indiceManette, sf::Joystick::PovY);
float Rposition = sf::Joystick::getAxisPosition(indiceManette, sf::Joystick::R);
float Zposition = sf::Joystick::getAxisPosition(indiceManette, sf::Joystick::Z);
/*
system("CLS");	// Vide l'écran
std::cout << "Xposition: " << (int)Xposition << "\t\t\t" << "Rposition: " << (int)Rposition << std::endl;
std::cout << "Yposition: " << (int)Yposition << "\t\t\t" << "Uposition: " << (int)Uposition << std::endl;
std::cout  << std::endl;
std::cout << "PovXposition: " << (int)PovXposition << "\t\t\t" << "Vposition: " << (int)Vposition << std::endl;
std::cout << "PovYposition: " << (int)PovYposition << "\t\t\t" << "Zposition: " << (int)Zposition << std::endl;

//X,Y: Stick haut droite
//
*/

/*
float dist = sqrtf(Xposition*Xposition + Yposition*Yposition);
if (dist > 15) {
buff.x = (int)Xposition/3;
buff.y = (int)Yposition/3;

sf::Mouse::setPosition(sf::Mouse::getPosition() + buff);
}

for (int i = 0; i < 10; i++) {
if (sf::Joystick::isButtonPressed(indiceManette, i))
std::cout << button[i] << std::endl;
//std::cout << i << std::endl;
}

if (sf::Joystick::isButtonPressed(indiceManette, 0) && sf::Joystick::isButtonPressed(indiceManette, 7))
break;


sf::sleep(sf::milliseconds(100));

//std::cout << "[" << (int)Xposition << "," << (int)Yposition << "]" << std::endl;
}


return EXIT_SUCCESS;

*/


/*


template<typename T>
T normalize(const T& x, std::function<Rational(const T&)> norm) {
	return T / norm(T);
}

//Précondition: 0.5*(r1 + r2 \not = 0)
template<typename T>
void eqRational(const T &t1, const T &t2) {
	Rational half = Rational(1, 2);
	T middle = half * (t1 + t2);
	T r_middle normalize(middle);

}

// Prerequisite: mat_size = 2^n
template<typename T>
T __internalMatDet(const size_t mat_size, const size_t sub_mat_size, const T*mat, const size_t x, const size_t y) {
	// Stopping condition!
	if (sub_mat_size == 1)
		return mat[mat_size*x + y];
	else {		// We reduce the problem
		T a0 == __internalMatDet(mat_size, sub_mat_size / 2, mat, x, y);
		T a1 == __internalMatDet(mat_size, sub_mat_size / 2, mat, x, y + sub_mat_size / 2);
		T a2 == __internalMatDet(mat_size, sub_mat_size / 2, mat, x + sub_mat_size / 2, y);
		T a3 == __internalMatDet(mat_size, sub_mat_size / 2, mat, x + sub_mat_size / 2, y + sub_mat_size / 2);

		return a0*a3 - a1*a2;
	}
}

// Return (with exp TODO: DOC) the uint64_t the closest to u such that it is a power of 2.
uint64_t __internalUplog(uint64_t u) {
	uint64_t mask = 0x8000000000000000;
	int last_bit_non_null = 0;
	for (int i = 0; i < 64; i++) {
		if ((u & mask) != 0) {
			last_bit_non_null = 64 - i;
			break;
		}
		mask = mask >> 1;
	}
	uint64_t strongest_power_of_two = 0x000000000000001 << last_bit_non_null;
	if (u != strongest_power_of_two) {
		uint64_t buff = strongest_power_of_two << 1;
		// TODO gestion errors;
		return buff;
	}
	else {
		return strongest_power_of_two;
	}
}

template<typename T>
T determinant(size_t mat_size, const T* mat) {
	uint64_t newsize = __internalUplog(mat_size);
	// Enhanceable with memcopy!!!
	// Alloc
	T* new_mat = new T[newsize*newsize];
	for (int i = 0; i < newsize*newsize; i++)
		new_mat[i] = T::zero();

	for (int i = 0; i < mat_size; i++)
		for (int j = 0; j < mat_size; j++)
			new_mat[i*newsize + j] = mat[i*mat_size + j];

	for (int i = mat_size, i < newsize; i++) {
		new_mat[i*newsize + i] = T::one();
	}

	T det = __internalMatDet(newsize, newsize, new_mat, 0, 0);

	// Unalloc
	delete[] new_mat;
}


*/





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

/*
std::vector<Rational2DPoint> points;
points.push_back(Rational2DPoint(Rational(3), Rational(0)));
points.push_back(Rational2DPoint(Rational(3), Rational(2)));
points.push_back(Rational2DPoint(Rational(6), Rational(-1)));
points.push_back(Rational2DPoint(Rational(8), Rational(3)));
points.push_back(Rational2DPoint(Rational(5), Rational(2)));

std::vector<Segment> segments;
for (size_t i = 0; i < points.size(); ++i) {
int index1 = i;
int index2 = (i + 1) % points.size();
std::cerr << "[" << index1 << ";" << index2 << "]\n";
segments.push_back(Segment(points[index1], points[index2]));
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
std::cerr << "[" << ++i << "/" << polyline.size() << "]\n";
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
} else {
std::cerr << "false";
}
std::cerr << std::endl;
std::cerr << "\t" << InterSol.inter << "\n\n";

}

i = 0;
for (auto it = polyline.begin(); it != polyline.end(); ++it) {
PolyLineCurve::iterator base(it);
++base;
for (auto itt = base; itt != polyline.end(); ++itt) {
auto InterSol = (*itt).intersectionWith((*it));
if (InterSol.exists) {
std::cerr << "[" << ++i << "]\n";
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
} else {
std::cerr << "false";
}
std::cerr << std::endl;
std::cerr << "\t" << InterSol.inter << "\n\n";


}
}
}
*/




/*

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
} catch (std::exception& e) {
std::cerr << "ERROR CATCHED!!!!! : " << e.what() << std::endl;
}

// We clear the data (not necessary in absolute but just to be shure)
intersectionPoints.clear();
forms.clear();

// We collect and merge the data from the two edges
try {
t1.collectData(forms, intersectionPoints);
t2.collectData(forms, intersectionPoints); // The data are naturally merged by collectData
} catch (std::exception& e) {
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
*/



/*

#include "PrintingWindow.h"


sf::Color genColor(double x) {

	x = std::max(0.0, std::min(x, 1.0));

	sf::Color out_color;

	if (x < 1.0 / 3.0) {
		out_color.r = 0;
		out_color.g = (uint8_t)(255 * 3 * x);
		out_color.b = 255;
	}
	else if (x < 2.0 / 3.0) {
		out_color.r = (uint8_t)(255 * 3 * (x - 1.0 / 3.0));
		out_color.g = 255;
		out_color.b = (uint8_t)(255 * 3 * (2.0 / 3.0 - x));
	}
	else {
		out_color.r = 255;
		out_color.g = (uint8_t)(255 * 3 * (1.0 - x));
		out_color.b = 0;
	}

	return out_color;
}


Boundiaries generateBoundiaries(const std::vector<Rational2DPoint>& vertices) {
	Rational __x_max(vertices[0].x);
	Rational __x_min(vertices[0].x);
	Rational __y_max(vertices[0].y);
	Rational __y_min(vertices[0].y);

	for (unsigned int i = 0; i < vertices.size(); i++) {
		Rational x = vertices[i].x;
		Rational y = vertices[i].y;

		if (x > __x_max) {
			__x_max = x;
		}
		else if (x < __x_min) {
			__x_min = x;
		}
		if (y > __y_max) {
			__y_max = y;
		}
		else if (y < __y_min) {
			__y_min = y;
		}
	}

	float a = static_cast<float>(Rational(1, 1));

	Rational x_rat_amp = __x_max - __x_min;
	Rational y_rat_amp = __y_max - __y_min;
	Rational x_rat_sum = __x_max + __x_min;
	Rational y_rat_sum = __y_max + __y_min;

	float x_amp = 1.2f * static_cast<float>(x_rat_amp);
	float y_amp = 1.2f * static_cast<float>(y_rat_amp);

	float x_center = 0.5f * static_cast<float>(x_rat_sum);
	float y_center = 0.5f * static_cast<float>(x_rat_sum);

	float half_x_amp = x_amp * 0.5f;
	float half_y_amp = y_amp * 0.5f;

	float x_min = x_center - half_x_amp;
	float x_max = x_center + half_x_amp;
	float y_min = y_center - half_y_amp;
	float y_max = y_center + half_y_amp;

	Boundiaries bound;

	bound.x_max = x_max;
	bound.x_min = x_min;
	bound.y_min = y_min;
	bound.y_max = y_max;

	return bound;
};


sf::Vector2f PrintingWindow::windowCoor(const Rational2DPoint& r) {
	float __x = static_cast<float>(r.x) - x_min;
	float __y = static_cast<float>(r.y) - y_min;

	float x_coeff = x_amp / X;
	float y_coeff = y_amp / Y;

	return sf::Vector2f(__x / x_coeff, __y / y_coeff);
}

void PrintingWindow::drawPoly(const std::vector<Rational2DPoint>& v) {
	float size_dot = std::max((float)(std::min(X, Y)) / 100.0f, 1.0f);
	sf::CircleShape dot(size_dot);
	dot.setOrigin(sf::Vector2f(dot.getRadius(), dot.getRadius()));
	dot.setFillColor(sf::Color(148, 0, 211));

	sf::VertexArray arr(sf::LinesStrip, v.size() + 1);
	for (unsigned int i = 0; i < v.size() + 1; i++) {
		arr[i] = windowCoor(v[i % v.size()]);
		arr[i].color = sf::Color::White;
		arr[i].color = sf::Color(148, 0, 211);
	}

	this->draw(arr);


	for (unsigned int i = 0; i < v.size(); i++) {
		sf::Vector2f pos = windowCoor(v[i]);
		dot.setPosition(pos);
		this->draw(dot);
	}


};

PrintingWindow::PrintingWindow(unsigned int X, unsigned int Y, float x_min, float x_max, float y_min, float y_max) :
RenderWindow(sf::VideoMode(X, Y), "Visualization window"),
x_min(x_min),
x_max(x_max),
y_min(y_min),
y_max(y_max),
X(X),
Y(Y) {
	if (X == 0 || Y == 0) {
		// TODO: Generate exception;
		throw std::exception();
	}

	if (x_min >= x_max || y_min >= y_max) {
		// TODO: Generate exception;
		throw std::exception();
	}

	x_amp = x_max - x_min;
	y_amp = y_max - y_min;

	x_coeff = X / x_amp;
	y_coeff = Y / y_amp;

	x_center = 0.5f * (x_max + x_min);
	y_center = 0.5f * (y_max + y_min);

	computeUpperLayout();
};

void PrintingWindow::drawNorm(std::function<float(float x, float y)> norm) {
	float x_amp = x_max - x_min;
	float y_amp = y_max - y_min;

	float x_coeff = x_amp / X;
	float y_coeff = y_amp / Y;

	float x = X * x_coeff;
	float y = Y * y_coeff;

	float x_offset = x / 2.0f;
	float y_offset = y / 2.0f;

	float upperBound = 0.0f;
	upperBound = std::max(upperBound, norm(x_min, y_min));
	upperBound = std::max(upperBound, norm(x_min, y_max));
	upperBound = std::max(upperBound, norm(x_max, y_min));
	upperBound = std::max(upperBound, norm(x_max, y_max));

	sf::Image im;
	im.create(X, Y);

	for (unsigned int i = 0; i < X; i++)
		for (unsigned int j = 0; j < Y; j++) {
			float x = x_min + i * x_coeff;
			float y = y_min + j * y_coeff;

			float __eval_norm = norm(x, y);

			sf::Color cc = genColor(__eval_norm / upperBound);
			if (abs(__eval_norm - 1.0f) <= 0.01f) {
				cc = sf::Color::Black;
			}
			im.setPixel(i, j, cc);
		}


	sf::Sprite sprite;
	sf::Texture text;
	text.loadFromImage(im);
	sprite.setTexture(text);

	this->draw(sprite);
}

void PrintingWindow::display() {
	this->draw(UpperLayoutSprite);
	RenderWindow::display();
};

bool PrintingWindow::generateImage(const std::string& filename) {
	sf::Image screenshot = this->capture();
	return screenshot.saveToFile(filename);
};


PrintingWindow::PrintingWindow(unsigned int X, unsigned int Y, const Boundiaries& bound) :
PrintingWindow(X, Y, bound.x_min, bound.x_max, bound.y_min, bound.y_max) {
};


PrintingWindow::PrintingWindow(unsigned int X, unsigned int Y, const std::vector<Rational2DPoint>& vertices) :
PrintingWindow(X, Y, generateBoundiaries(vertices)) {
};

void PrintingWindow::resetBoundiaires(const Boundiaries& bounds) {

	x_min = bounds.x_min;
	x_max = bounds.x_max;
	y_min = bounds.y_min;
	y_max = bounds.y_max;


	if (x_min >= x_max || y_min >= y_max) {
		// TODO: Generate exception;
		throw std::exception();
	}

	x_amp = x_max - x_min;
	y_amp = y_max - y_min;

	x_coeff = X / x_amp;
	y_coeff = Y / y_amp;

	x_center = 0.5f * (x_max + x_min);
	y_center = 0.5f * (y_max + y_min);

	sf::Vector2f center(x_center, y_center);
	sf::Vector2f size(x_amp, y_amp);

	computeUpperLayout();
}

void PrintingWindow::computeUpperLayout() {
	sf::Vector2f center(x_center, y_center);
	sf::Vector2f size(x_amp, y_amp);


	//=========== Creating and setting the upper layout =============
	// UpperLayout is a member.
	UpperLayout.create(X, Y);
	UpperLayout.clear(sf::Color(0, 0, 0, 0)); // Transparent layout


	// Compute a "good size" for the integral latice.
	float X_nb_int_pt = ((float)X) / (20.0f * x_amp);
	float Y_nb_int_pt = ((float)Y) / (20.0f * x_amp);

	float int_point_rad = std::max(1.0f, std::min(X_nb_int_pt, Y_nb_int_pt));
	int_point_rad = 4.0f;
	float origin_point_rad = 2 * int_point_rad;

	sf::CircleShape int_point(int_point_rad);
	int_point.setFillColor(sf::Color::Black);
	int_point.setOrigin(int_point.getRadius(), int_point.getRadius());

	sf::CircleShape origin_point(origin_point_rad);
	origin_point.setOrigin(origin_point.getRadius(), origin_point.getRadius());
	sf::Vector2f origin(-x_min * x_coeff, -y_min * y_coeff);
	origin_point.setPosition(origin);
	origin_point.setFillColor(sf::Color::Black);

	//=========== Drawing the integral lattice =====================
	int int_x_min = (int)x_min - 1;
	int int_x_max = (int)x_max + 1;

	int int_y_min = (int)y_min - 1;
	int int_y_max = (int)y_max + 1;

	int numberOfPoints = (int_x_max - int_x_min + 1) * (int_y_max - int_y_min + 1);
	int counter = 1;

	//std::cerr << "In..." << std::endl;

	for (int i = int_x_min; i <= int_x_max; i++) {
		for (int j = int_y_min; j <= int_y_max; j++) {
			float __x = (float)i;
			float __y = (float)j;

			float x = (__x - x_min) * x_coeff;
			float y = (__y - y_min) * y_coeff;
			// Convert to frame coordinates


			int_point.setPosition(x, y);
			UpperLayout.draw(int_point);

			counter++;
		}
	}

	UpperLayout.draw(origin_point); // Draw the origin point

	sf::RectangleShape X_axis(sf::Vector2f((float)X, 4.0f));
	X_axis.setOrigin(X_axis.getSize() / 2.0f);
	X_axis.setPosition(origin);
	X_axis.setFillColor(sf::Color::Black);

	UpperLayout.draw(X_axis);

	sf::RectangleShape Y_axis(sf::Vector2f(4.0f, (float)Y));
	Y_axis.setOrigin(Y_axis.getSize() / 2.0f);
	Y_axis.setPosition(origin);
	Y_axis.setFillColor(sf::Color::Black);

	UpperLayout.draw(Y_axis);


	//=========== Set the sprite  for the upper layout ============
	UpperLayoutSprite.setTexture(UpperLayout.getTexture());
}
#pragma once


#include <vector>
#include <SFML\Graphics.hpp>
#include <cmath>
#include <functional>


#include "Rational.h"
#include "Rational2DPoint.h"


sf::Color genColor(double x);

using Boundiaries = struct {
	float x_min;
	float x_max;
	float y_min;
	float y_max;
};


Boundiaries generateBoundiaries(const std::vector<Rational2DPoint>& vertices);


class PrintingWindow :
	public sf::RenderWindow {
public:

	sf::Vector2f windowCoor(const Rational2DPoint& r);

	void drawPoly(const std::vector<Rational2DPoint>& v);

	PrintingWindow(unsigned int X, unsigned int Y, const std::vector<Rational2DPoint>& vertices);
	PrintingWindow(unsigned int X, unsigned int Y, const Boundiaries& bound);

	void drawNorm(std::function<float(float x, float y)> norm);
	bool generateImage(const std::string& filename);

	void resetBoundiaires(const Boundiaries& bounds);
	void computeUpperLayout();

	void display();

private:

	PrintingWindow(unsigned int X, unsigned int Y, float x_min, float x_max, float y_min, float y_max);

	sf::RenderTexture UpperLayout;
	sf::Sprite UpperLayoutSprite;

	float x_min;
	float x_max;
	float y_min;
	float y_max;

	float x_amp;
	float y_amp;

	float x_coeff;
	float y_coeff;

	float x_center;
	float y_center;

	unsigned int X;
	unsigned int Y;
};


#include "NormObject.h"


#include <chrono>
#include <cstdint>

std::default_random_engine NormObject::RationalSlopeRep::generator((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());
std::uniform_int_distribution<int> NormObject::RationalSlopeRep::distribution(-100, 100);
std::function<int()> NormObject::RationalSlopeRep::RNG(std::bind(distribution, generator));

void NormObject::RationalSlopeRep::randomize() {
	do {
		a = RNG();
		b = RNG();
	} while (a == 0 && b == 0);
}

NormObject::NormObject() {
	int nb_set;

	do {
		nb_set = abs(RationalSlopeRep::RNG()) % 20;
	} while (nb_set <= 1);

	auto check = [](const std::vector<RationalSlopeRep>& v) {
		const RationalSlopeRep& refSlope = v[0];
		for (unsigned int i = 1; i < v.size(); i++) {
			if (!(refSlope.isProportional(v[i])))
				return true;
		}

		return false;
	};

	do {
		v.clear();

		for (int i = 0; i < nb_set; i++) {
			v.push_back(RationalSlopeRep()); // Random slope rep
		}
	} while (!check(v));
}

std::ostream& operator<<(std::ostream& os, const NormObject::RationalSlopeRep& slopeRep) {
	os << "[" << slopeRep.a << "," << slopeRep.b << "]";
	return os;
}

NormObject::RationalSlopeRep::RationalSlopeRep(int a, int b) :
a(a),
b(b) {
	if ((a == 0) && (b == 0)) {
		throw NormObject::NormObjectException();
	}
}

NormObject::RationalSlopeRep::RationalSlopeRep() {
	randomize();
}

NormObject::RationalSlopeRep::~RationalSlopeRep() {
}

bool NormObject::RationalSlopeRep::isProportional(const NormObject::RationalSlopeRep& slopeRep) const {
	int64_t __a = slopeRep.a;
	int64_t __b = slopeRep.b;
	return (a * __b + b * __a) == 0;
}


NormObject::NormObject(std::string& str) {

	enum __AUTOM_STATE {
		START,
		GEN_OPEN,
		GEN_CLOSE,
		LOC_OPEN,
		LOC_CLOSE,
		READ_NUMBER_1,
		READ_NUMBER_2,
		COMMA_LOC,
		COMMA_GEN,
		MINUS_1,
		MINUS_2
	};


	int64_t number_read_1 = 0;
	int64_t number_read_2 = 0;
	bool negative_read = false;

	__AUTOM_STATE state = START;

	if (str.size() == 0) {
		throw NormObject::NormObjectException();
	}


	for (size_t index = 0; index < str.size(); index++) {

		char curr = str[index];

		switch (state) {
		case START:
			if (curr == '[') {
				state = GEN_OPEN;
			}
			else {
				throw NormObject::NormObjectException();
			}
			break;
		case GEN_OPEN:
			if (curr == '[') {
				state = LOC_OPEN;
			}
			else {
				throw NormObject::NormObjectException();
			}
			break;
		case GEN_CLOSE:
			break;
		case LOC_OPEN:
			number_read_1 = 0;
			number_read_2 = 0;
			if (curr == '-') {
				negative_read = true;
				state = MINUS_1;
			}
			else if (isDigit(curr)) {
				number_read_1 = number_read_1 * 10 + getDigit(curr);
				state = READ_NUMBER_1;
			}
			else {
				throw NormObject::NormObjectException();
			}
			break;
		case LOC_CLOSE:
			if (curr == ',') {
				state = COMMA_GEN;
			}
			else if (curr == ']') {
				state = GEN_CLOSE;
			}
			else {
				throw NormObject::NormObjectException();
			}
			negative_read = false;
			v.push_back(RationalSlopeRep((int)number_read_1, (int)number_read_2));
			break;
		case READ_NUMBER_1:
			if (curr == ',') {
				if (negative_read) {
					number_read_1 *= -1;
				}
				negative_read = false;
				state = COMMA_LOC;
			}
			else if (isDigit(curr)) {
				number_read_1 = number_read_1 * 10 + getDigit(curr);
				state = READ_NUMBER_1;
			}
			else {
				throw NormObject::NormObjectException();
			}
			break;
		case MINUS_1:
			if (isDigit(curr)) {
				number_read_1 = number_read_1 * 10 + getDigit(curr);
				state = READ_NUMBER_1;
			}
			else {
				throw NormObject::NormObjectException();
			}
			break;
		case READ_NUMBER_2:
			if (curr == ']') {
				if (negative_read) {
					number_read_2 *= -1;
				}
				negative_read = false;
				state = LOC_CLOSE;
			}
			else if (isDigit(curr)) {
				number_read_2 = number_read_2 * 10 + getDigit(curr);
				state = READ_NUMBER_2;
			}
			else {
				throw NormObject::NormObjectException();
			}
			break;
		case MINUS_2:
			if (isDigit(curr)) {
				number_read_2 = number_read_2 * 10 + getDigit(curr);
				state = READ_NUMBER_2;
			}
			else {
				throw NormObject::NormObjectException();
			}
			break;
		case COMMA_LOC:
			if (curr == '-') {
				negative_read = true;
				state = MINUS_2;
			}
			else if (isDigit(curr)) {
				number_read_2 = number_read_2 * 10 + getDigit(curr);
				state = READ_NUMBER_2;
			}
			else {
				throw NormObject::NormObjectException();
			}
			break;
		case COMMA_GEN:
			if (curr == '[') {
				state = LOC_OPEN;
			}
			else {
				throw NormObject::NormObjectException();
			}
			break;
		default:
			// Unreachable code
			throw NormObject::NormObjectException();
		}

	}

	if (v.size() < 2) {
		throw NormObject::NormObjectException();
	}

	bool isNorm = false;

	RationalSlopeRep& slopeRepRef = v[0];

	for (size_t i = 1; i < v.size(); i++) {
		if (!slopeRepRef.isProportional(v[i])) {
			isNorm = true;
			break;
		}
	}

	if (!isNorm) {
		throw NormObject::NormObjectException();
	}
}

NormObject::~NormObject() {

}

float NormObject::operator()(float x, float y) const {
	float res = 0;
	for (size_t i = 0; i < v.size(); i++) {
		float a = (float)(v[i].a);
		float b = (float)(v[i].b);
		res += abs(b * x - a * y);
	}
	return res;
}

Rational NormObject::operator()(const Rational& x, const Rational& y) const {
	Rational res(0, 1);
	for (size_t i = 0; i < v.size(); i++) {
		int __a = v[i].a;
		int __b = v[i].b;
		Rational a(__a, 1);
		Rational b(__b, 1);
		res += abs(b * x - a * y);
	}
	return res;
}

Rational NormObject::operator()(const Rational2DPoint& p) const {
	return operator()(p.x, p.y);
}

bool NormObject::isDigit(const char c) {
	return (c - '0') >= 0 && (c - '0') <= 9;
}

int NormObject::getDigit(const char c) {
	return c - '0';
}

std::ostream& operator<<(std::ostream& os, const NormObject& nO) {
	os << "[";
	for (size_t i = 0; i < (nO.v.size() - 1); i++) {
		os << nO.v[i] << ",";
	}
	os << nO.v.back() << "]";
	return os;
}


#pragma once


#include <vector>

#include <ostream>
#include <random>

#include "Rational.h"
#include "Rational2DPoint.h"

class NormObject {

	using NormObjectException = std::exception;

	// Public methods
public:
	NormObject();
	NormObject(std::string& str);
	virtual ~NormObject();

	float operator()(float x, float y) const;
	Rational operator()(const Rational& x, const Rational& y) const;
	Rational operator()(const Rational2DPoint& p) const;

	// Private intern class
private:
	class RationalSlopeRep final {

		friend class NormObject;

	public:
		RationalSlopeRep();
		RationalSlopeRep(int a, int b);
		~RationalSlopeRep();

		bool isProportional(const RationalSlopeRep& slopeRep) const;

		friend std::ostream& operator<<(std::ostream& os, const RationalSlopeRep& slopeRep);

	public:
		int a;
		int b;

		void randomize();

	private:
		static std::default_random_engine generator;
		static std::uniform_int_distribution<int> distribution;
		static std::function<int()> RNG;

	};

	friend std::ostream& operator<<(std::ostream& os, const RationalSlopeRep& slopeRep);
	friend std::ostream& operator<<(std::ostream& os, const NormObject& slopeRep);

	// Private static methods
	static bool isDigit(const char c);
	static int getDigit(const char c);

	// Private members
	std::vector<RationalSlopeRep> v;


};


#include "__internalTree.h"


#include "Rational.h"

__internalTree::__internalTree(Rational2DPoint __x1, Rational2DPoint __x2, std::function<Rational(const Rational2DPoint&)> norm) :
x1(__x1),
x2(__x2),
norm(norm),
child_1(NULL),
child_2(NULL),
__isFlat(false),
__isCone(false) {
	normalize(x1);
	normalize(x2);
};

__internalTree::~__internalTree() {
	delete child_1;
	delete child_2;
};

void __internalTree::normalize(Rational2DPoint& x) {
	x = x / norm(x);
};

Rational2DPoint __internalTree::normed(const Rational2DPoint& x) {
	return x / norm(x);
};

bool __internalTree::isFlat() {
	return __isFlat;
};

bool __internalTree::isCone() {
	return __isCone;
}

bool __internalTree::isLeaf() {
	return __isCone || __isFlat;
};

bool __internalTree::isFlatSegment(const Rational2DPoint& x1, const Rational2DPoint& x2, std::function<Rational(const Rational2DPoint&)> norm) {
	//std::cerr << "isFlat called [" << this << "]" << std::endl;
	Rational2DPoint mid(Rational2DPoint::middle(x1, x2));
	return (norm(mid) == Rational(1, 1));
};

void __internalTree::prettyPrint() {
	if (this == NULL)
		return;

	if (isLeaf()) {
		if (isCone()) {
			std::cerr << "|| CONE : " << std::endl;
			std::cerr << "|| x1 : " << x1 << std::endl;
			std::cerr << "|| x2 : " << x2 << std::endl;
			std::cerr << "||\t ConeNorm1 : " << ConeNorm1 << std::endl;
			std::cerr << "||\t ConeNorm2 : " << ConeNorm2 << std::endl;
			std::cerr << "||\t Intersect : " << ConeIntersec << std::endl;
		}
		else {
			std::cerr << "|| FLAT : " << std::endl;
			std::cerr << "|| x1 : " << x1 << std::endl;
			std::cerr << "|| x2 : " << x2 << std::endl;
			std::cerr << "||\t FlatNorm : " << FlatNorm << std::endl;
		}
		std::cerr << std::endl;
	}
	else {
		child_1->prettyPrint();
		child_2->prettyPrint();
	}
};

void __internalTree::__internal_computeUnitaryBall() {

	if (x1 == x2) {
		// Dead case: should never happened!
		// TODO: gestion erreur;
		throw std::exception();
	}
	else {
		Rational2DPoint mid(Rational2DPoint::middle(x1, x2));
		mid = Rational(1, 2) * (x1 + x2);
		if (norm(mid) == Rational(1, 1)) { // Flat leaf
			__isFlat = true;
			FlatNorm = compute_rat_form(x1, x2);
		}
		else {
			Rational2DPoint __xx(mid);
			Rational2DPoint __yy(mid);

			// We search the extremal pent of the unitary ball on the segment.
			// /!\ CAUTION: THIS OPERATION IS NOT BOUNDED IN TIME BUT CONVERGES RAPIDELY IN
			// REASONABLE CASES /!\.


			while (!isFlatSegment(x1, normed(__xx), norm)) {
				__xx = Rational2DPoint::middle(x1, __xx);
				normalize(__xx);
			}
			while (!isFlatSegment(x2, normed(__yy), norm)) {
				__yy = Rational2DPoint::middle(x2, __yy);
				normalize(__yy);
			}

			normalize(__xx);
			normalize(__yy);

			// The two Rational2DForm c represent the affine rational form x -> <x|c> -1;
			Rational2DForm f1 = compute_rat_form(x1, __xx);
			Rational2DForm f2 = compute_rat_form(__yy, x2);

			// By duality the same algorithm allows to compute the intersection point of the two forms.
			Rational2DPoint __internal_intersectionPoint = compute_rat_form(f1, f2);

			if (norm(__internal_intersectionPoint) == Rational(1, 1)) { // Cone leaf

				ConeIntersec = __internal_intersectionPoint;
				__isCone = true;
				ConeNorm1 = f1;
				ConeNorm2 = f2;
			}
			else { // Not a leaf

				normalize(mid);

				child_1 = new __internalTree(x1, mid, norm);
				child_2 = new __internalTree(mid, x2, norm);

				child_1->__internal_computeUnitaryBall();
				child_2->__internal_computeUnitaryBall();
			}
		}
	}
}

void __internalTree::collectData(std::vector<Rational2DForm>& forms, std::vector<Rational2DPoint>& v) {
	if (isFlat()) {
		// We add the FlatNorm to the list 
		if (forms.size() == 0) {
			forms.push_back(FlatNorm);
		}
		else {
			Rational2DForm f = forms.back();
			if (FlatNorm != f) // We check if the norm matches
				forms.push_back(FlatNorm);
		}
	}
	else if (isCone()) {
		// We add the ConeForms to the list
		if (forms.size() == 0) {
			forms.push_back(ConeNorm1);
			forms.push_back(ConeNorm2);
		}
		else {
			Rational2DForm f = forms.back();
			if (ConeNorm1 != f) { // We check if the norm matches
				forms.push_back(ConeNorm1);
			}
			forms.push_back(ConeNorm2);
		}
		v.push_back(ConeIntersec);
	}
	else {
		child_1->collectData(forms, v);
		child_2->collectData(forms, v);
	}
}

#pragma once



#include <vector>
#include <functional>

#include "Rational2DPoint.h"


class __internalTree {

public:
	Rational2DPoint x1;
	Rational2DPoint x2;

	bool __isCone;
	bool __isFlat;

	Rational2DPoint ConeIntersec;

	Rational2DForm FlatNorm; // Has sense only is isFlat
	Rational2DForm ConeNorm1; // Has sense only if ifCone
	Rational2DForm ConeNorm2; // Has sense only if ifCone

	std::function<Rational(const Rational2DPoint&)> norm;

	__internalTree* child_1 = nullptr;
	__internalTree* child_2 = nullptr;


public:
	__internalTree(Rational2DPoint __x1, Rational2DPoint __x2, std::function<Rational(const Rational2DPoint&)> norm);
	~__internalTree();

	void normalize(Rational2DPoint& x);

	Rational2DPoint normed(const Rational2DPoint& x);

	bool isFlat();
	bool isCone();

	bool isLeaf();
	bool isFlatSegment(const Rational2DPoint& x1, const Rational2DPoint& x2, std::function<Rational(const Rational2DPoint&)> norm);

	void prettyPrint();

	void __internal_computeUnitaryBall();

	void collectData(std::vector<Rational2DForm>& forms, std::vector<Rational2DPoint>& v);
};

*/