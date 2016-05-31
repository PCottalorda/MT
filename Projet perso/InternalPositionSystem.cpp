#include "InternalPositionSystem.h"

#include "SettingWindow.h"
#include "IntersectionManager.h"

namespace {
	float project(const sf::Vector2f& v, const sf::Vector2f& form) {
		return v.x * form.x + v.y * form.y;
	}

	float norm2(const sf::Vector2f& v) {
		return v.x * v.x + v.y * v.y;
	}

	float dist2(const sf::Vector2f& v1, const sf::Vector2f& v2) {
		return norm2(v2 - v1);
	}

}

InternalPositionSystem::InternalPositionSystem(SettingWindow* w) :
window(w),
onClosure(false),
authModif(true),
onBoundiary(false),
index(0) {
	// ratFormF initialized in SettingWindow::SettingWindow()
	// internalShape initialized in SettingWindow::SettingWindow()
}


void InternalPositionSystem::update() {

	readMousePosition();


	onBoundiary = false;
	onClosure = false;


	if (!window->actionConsistent()) return;

	//==== Determining the position with regards to the 4n-gon CW-complex of the n-genus surface ====
	sf::Vector2f form = ratFormsF[0];
	float proj = project(MouseInternal, form);
	index = 0;

	for (size_t i = 1; i < ratFormsF.size(); i++) {
		float p = project(MouseInternal, ratFormsF[i]);
		if (p > proj) {
			proj = p;
			form = ratFormsF[i];
			index = i;
		}
	}

	if (proj > 0.97) {
		onBoundiary = true;
	}

	// We recompute the position to "stick" to the boundiary
	if (onBoundiary) {
		sf::Vector2f p1 = internalShape[index];
		sf::Vector2f p2 = internalShape[(index - 1 + internalShape.size()) % internalShape.size()];

		sf::Vector2f X = p2 - p1;

		sf::Vector2f nP = MouseInternal - p1;
		float coeff = project(nP, X) / norm2(X);
		if (coeff < 0.01f) {
			coeff = 0.01f;
		}
		else if (coeff > 0.99f) {
			coeff = 0.99f;
		}
		sf::Vector2f newPos = coeff * p2 + (1 - coeff) * p1;
		setMouseInternal(newPos);
		writeMousePosition();
	}
	//=================================================


	//==== Determining the position with regards to the first point (if it exists) ====
	if (!internalPoints.empty()) {
		float d2 = dist2(MouseInternal, internalPoints.front().point);
		if (d2 < 0.005) {
			onClosure = true;
			if (authModif) {
				setMouseInternal(internalPoints.front().point);
				writeMousePosition();
			}
		}
	}
	//=================================================
}

void InternalPositionSystem::readMousePosition() {
	MouseInternal = window->convertWindowToInternal(sf::Vector2f(sf::Mouse::getPosition(*window)));
}

void InternalPositionSystem::writeMousePosition() const {
	sf::Mouse::setPosition(sf::Vector2i(window->convertInternalToWindow(MouseInternal)), *window);
}

void InternalPositionSystem::setMouseInternal(const sf::Vector2f& v) {
	MouseInternal = v;
}

void InternalPositionSystem::addPoint() {
	/* We check the conditions to see if we can pose our point
	 * The conditions are:
	 *		- The first point of a multi-loop cannot be placed on a boundiary.
	 *		- One complete segment cannot be placed on a boundiary.
	 *		- A point placed on a boundiary is automatically teleported on a segment.
	 *		- Points cannot intersect on a boundiary.
	 */

	if (!authModif) return; // NEED TO HAVE MODIFICATION AUTHORIZATION

	if (onBoundiary) {
		if (internalPoints.empty()) { // The first point is located on the border
			return;
		}
		else {
			// We check if the last point was onBoundiary
			// TODO: Restrict to same boundiary!
			if (internalPoints.back().index == index) {
				// We do nothing!
			}
			else {
				// We add the new boundiary point
				// TODO: Add the new points
				internalPoints.push_back(Point(MouseInternal, true, index));
				window->addPoint(MouseInternal, true, true);
				invert();
				internalPoints.push_back(Point(MouseInternal, true, index));
				window->addPoint(MouseInternal, true, false);
			}
		}
	}
	else {
		// WE REMIND THAT THE FIRST POINT IS NEVER ON BOUNDIARY 
		// (onClosure and onBoundiary are never both true at the same time)

		if (internalPoints.empty()) {
			internalPoints.push_back(Point(MouseInternal, false, -1));
			window->addPoint(MouseInternal, true, false);
			window->resetIndiceFirstPoint();
		}
		else {
			if (onClosure) {
				// We do not add any point!
				window->complete = true;
				window->setBinding(false);
				window->addPoint(MouseInternal, true, true);
				exportAndReinitialize();
				window->complete = false;
				window->setBinding(true);
			}
			else {
				internalPoints.push_back(Point(MouseInternal, false, -1));
				window->addPoint(MouseInternal, true, true);
			}
		}

	}
}

void InternalPositionSystem::invert() {
	assert(internalShape.size() % 2 == 0);
	int ind1 = index;
	int ind2 = (index - 1 + internalShape.size()) % internalShape.size();
	sf::Vector2f p1 = internalShape[ind1];
	sf::Vector2f p2 = internalShape[ind2];
	sf::Vector2f middle = 0.5f * (p1 + p2);
	sf::Vector2f diff_midlle = MouseInternal - middle;
	middle = -middle;
	MouseInternal = middle + diff_midlle;
	writeMousePosition();
	int new_index = (index + (internalRationalShape.size() / 2)) % internalRationalShape.size();
	index = new_index;
}

RationalPoint InternalPositionSystem::invert(const RationalPoint& p) const {
	assert(p.onBoundiary);
	assert(internalShape.size() % 2 == 0);
	assert(internalShape.size() == 4 * window->genus);
	int ind1 = p.index;
	int ind2 = (ind1 - 1 + internalShape.size()) % internalShape.size();
	Rational2DPoint p1 = internalRationalShape[ind1];
	Rational2DPoint p2 = internalRationalShape[ind2];
	/*
	p1.prettyPrint();
	p2.prettyPrint();
	p.point.prettyPrint();
	if (p.onBoundiary) {
		std::cout << "OnBound, index: " << p.index << std::endl;
	} else {
		std::cout << "NotOnBoundiary" << std::endl;
	}
	std::cout << p.onBoundiary << std::endl;
	std::cout << "P1: " << p1 << " | "; p1.prettyPrint();
	std::cout << "P2: " << p2 << " | "; p2.prettyPrint();
	std::cout << "P : " << p.point << " | "; p.point.prettyPrint();
	*/
	Rational det = Rational2DPoint::det(p1 - p.point, p2 - p.point);
	//std::cerr << det << " | " << static_cast<float>(det) << std::endl;
	assert(Rational2DPoint::det(p1 - p.point, p2 - p.point) == 0);
	Rational2DPoint middle = Rational(1, 2) * (p1 + p2);
	Rational2DPoint diff_midlle = p.point - middle;
	middle = Rational(-1) * middle; // We go to the opposite!
	Rational2DPoint res = middle + diff_midlle;
	int new_index = (p.index + (internalRationalShape.size() / 2)) % internalRationalShape.size();
	RationalPoint result(res, true, new_index);
	return result;
}

std::vector<RationalPoint> InternalPositionSystem::exportPoints() {
	auto boundiaryfind = [&](const Point& p)
	{
		auto findLambda = [](const sf::Vector2f& v1, const sf::Vector2f& v2)
		{
			return (v1.x*v2.x + v1.y*v2.y) / (v2.x*v2.x + v2.y*v2.y);
		};
		assert(p.onBoundiary);
		int ind1 = p.index;
		int ind2 = (ind1 - 1 + internalShape.size()) % internalShape.size();
		Rational2DPoint rp1 = internalRationalShape[ind1];
		Rational2DPoint rp2 = internalRationalShape[ind2];
		sf::Vector2f p1 = internalShape[ind1];
		sf::Vector2f p2 = internalShape[ind2];
		float lambda = findLambda(p.point - p1, p2 - p1);
		//std::cout << lambda << std::endl;
		assert(lambda >= 0 && lambda <= 1);
		Rational rLambda = floatToRational(lambda);
		Rational2DPoint newP = rp1 + rLambda*(rp2 - rp1);
		return newP;
	};
	std::vector<RationalPoint> res;
	auto PointToRationalPoint = [&res,&boundiaryfind](const Point& p) {
		if (p.onBoundiary) {
			Rational2DPoint rPBase(boundiaryfind(p));
			RationalPoint rP(rPBase, p.onBoundiary, p.index);
			return rP;
		}
		else {
			assert(!p.onBoundiary);
			Rational2DPoint rPBase(floatToRational(p.point.x), floatToRational(p.point.y));
			RationalPoint rP(rPBase, p.onBoundiary, p.index);
			return rP;
		}
	};


	int genus = window->genus;
	size_t i = 0;
	while (i < internalPoints.size()) {
		Point& p = internalPoints[i];
		if (p.onBoundiary) {
			auto printRatPoint = [](const RationalPoint& rP) {
				std::cerr << "====== " << &rP << " ======" << std::endl;
				std::cerr << rP.point << " | "; rP.point.prettyPrint();
				std::cout.flush();
				if (rP.onBoundiary) {
					std::cerr << "onBoundary: true\nindex: " << rP.index << std::endl;
				} else {
					std::cerr << "onboundary: false" << std::endl;
				}
				std::cerr << "======================" << std::endl;
			};
			auto printPoint = [](const Point& rP) {
				std::cerr << "====== " << &rP << " ======" << std::endl;
				std::cerr << "[ " << rP.point.x << " , " << rP.point.y << " ]" << std::endl;
				std::cout.flush();
				if (rP.onBoundiary) {
					std::cerr << "onBoundary: true\nindex: " << rP.index << std::endl;
				}
				else {
					std::cerr << "onboundary: false" << std::endl;
				}
				std::cerr << "======================" << std::endl;
			};
			RationalPoint rP = PointToRationalPoint(p);
			//printRatPoint(rP);
			res.push_back(rP);
			//printPoint(internalPoints[i]);
			assert(res[i].index == internalPoints[i].index);
			++i;
			RationalPoint rP2 = invert(rP);
			//std::cerr << "Insertion!" << std::endl;
			//printRatPoint(rP2);
			res.push_back(rP2);
			//printPoint(internalPoints[i]);
			assert(res[i].index == internalPoints[i].index);
		} else {
			RationalPoint rP = PointToRationalPoint(p);
			res.push_back(rP);
			assert(res[i].index == internalPoints[i].index);
		}
		++i;
	}
	assert(res.size() == internalPoints.size());
	
	/*
	assert(res.size() == internalPoints.size());
	for (size_t i = 0; i < res.size(); ++i) {
		auto dist = [](const RationalPoint& rP, const Point& p)
		{
			sf::Vector2f vec = p.point - rP.point.toSFMLVector2f();
			return sqrtf(vec.x*vec.x + vec.y*vec.y);
		};
		float d = dist(res[i], internalPoints[i]);
		//std::cout << d << std::endl;
		assert(d < 1.0e-4f);
		assert(res[i].onBoundiary == internalPoints[i].onBoundiary);
		std::cerr << "ResIndex: " << res[i].index << "   |   " << "InternalPointIndex: " << internalPoints[i].index << std::endl;
		assert(res[i].index == internalPoints[i].index);

	}
	*/


	return res;
}

Rational InternalPositionSystem::floatToRational(float f) {
	auto pow = [](Rational num, int puiss) -> Rational {
		Rational res = 1;
		Rational b = num;
		if (puiss < 0) {
			puiss = -puiss;
			b = Rational(1) / b;
		}

		for (int i = 0; i < puiss; ++i) {
			res *= b;
		}
		return res;
	};
	using float_cast = union {
		float f;

		struct {
			unsigned int mantissa : 23;
			unsigned int exponent : 8;
			unsigned int sign : 1;
		} parts;
	};
	float_cast f_transf;
	f_transf.f = f;
	const unsigned int coeff_conv = 8388608; // 2^23;
	Rational base = Rational(1) + Rational(f_transf.parts.mantissa) / Rational(coeff_conv);
	int exp = f_transf.parts.exponent - 127;
	Rational expo = pow(2, exp);
	return base * expo * (f_transf.parts.sign ? -1 : 1);
}

void InternalPositionSystem::exportAndReinitialize() {
	std::vector<RationalPoint> res = exportPoints();
	assert(!res.empty());
	std::vector<Segment> segs;
	RationalPoint prev = res.front();
	for (unsigned int i = 1; i < res.size() +1; ++i) {
		RationalPoint current = res[i%res.size()];
		Segment seg(prev, current);
		segs.push_back(seg);
		if (current.onBoundiary) {
			++i;
			prev = res[i%res.size()];
		}
		else {
			prev = current;
		}
	}
	window->lineCurvesSet.push_back(PolyLineCurve(segs));	
	reset();
}

void InternalPositionSystem::reset() {
	internalPoints.clear();
	update();
}

InternalPositionSystem::~InternalPositionSystem() {
}

sf::Vector2f InternalPositionSystem::getMouseWindowPosition() const {
	return window->convertInternalToWindow(MouseInternal);
}
