#include "__internalTree.h"


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
