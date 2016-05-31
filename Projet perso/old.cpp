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