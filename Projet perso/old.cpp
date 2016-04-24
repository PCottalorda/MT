
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