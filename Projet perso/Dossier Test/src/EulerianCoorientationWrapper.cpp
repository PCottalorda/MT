#include "EulerianCoorientationWrapper.h"

EulerianCoorientationWrapper::EulerianCoorientationWrapper(const HomologieValue& hom, bool on_boundiary): hom(hom),
                                                                                                          onBoundiary(on_boundiary) {
}

EulerianCoorientationWrapper::EulerianCoorientationWrapper(const EulerianCoorientationWrapper& other): hom(other.hom),
                                                                                                       onBoundiary(other.onBoundiary) {
}

EulerianCoorientationWrapper::EulerianCoorientationWrapper(EulerianCoorientationWrapper&& other): hom(std::move(other.hom)),
                                                                                                  onBoundiary(other.onBoundiary) {
}

EulerianCoorientationWrapper& EulerianCoorientationWrapper::operator=(const EulerianCoorientationWrapper& other) {
	if (this == &other)
		return *this;
	hom = other.hom;
	onBoundiary = other.onBoundiary;
	return *this;
}

EulerianCoorientationWrapper& EulerianCoorientationWrapper::operator=(EulerianCoorientationWrapper&& other) {
	if (this == &other)
		return *this;
	hom = std::move(other.hom);
	onBoundiary = other.onBoundiary;
	return *this;
}

EulerianCoorientationWrapper::~EulerianCoorientationWrapper() {
}

std::vector<EulerianCoorientationWrapper> EulerianCoorientationWrapper::ConvexHullMarking2D(const std::set<HomologieValue>& s) {
	
	if (std::any_of(s.begin(), s.end(), [](const HomologieValue& hval) {
		                return hval.dimension() != 2;
	                })) {
		throw std::out_of_range("ConvexHullMarking2D must be called with HomologieValues of dimension 2!");
	}

	std::vector<EulerianCoorientationWrapper> convHullS;

	for each (const auto& hVal in s) {
		convHullS.push_back(EulerianCoorientationWrapper(hVal));
	}
	assert(std::none_of(convHullS.begin(), convHullS.end(), [](const EulerianCoorientationWrapper& ECW) {return ECW.onBoundiary;}));

	int n = convHullS.size(), k = 0;
	std::vector<EulerianCoorientationWrapper> H(2 * n);

	// Build lower hull
	for (int i = 0; i < n; ++i) {
		while (k >= 2 && cross(H[k - 2].hom, H[k - 1].hom, convHullS[i].hom) <= 0) k--;
		H[k++] = convHullS[i];
		convHullS[i].onBoundiary = true;
	}

	// Build upper hull
	for (int i = n - 2, t = k + 1; i >= 0; i--) {
		while (k >= t && cross(H[k - 2].hom, H[k - 1].hom, convHullS[i].hom) <= 0) k--;
		H[k++] = convHullS[i];
		convHullS[i].onBoundiary = true;
	}

	return convHullS;
}

std::vector<Rational2DPoint> EulerianCoorientationWrapper::planeCorrespondance(const HomologieValue& A, const HomologieValue& B, int i, int j) {
	assert(A.dimension() == B.dimension());
	assert(0 < i && i < A.dimension() && 0 < j && j < B.dimension());
	assert(i < j);
	auto inPlane = [&i, &j](const HomologieValue& hval) {
		for (int h = 0; h < hval.dimension(); ++h) {
			if (h != i && h != j) {
				if (hval[h] == 0) {
					return false;
				}
			}
		}
		return true;
	};

	std::vector<Rational2DPoint> res;
	Rational2DPoint nA = Rational2DPoint(A[i], A[j]);
	Rational2DPoint nB = Rational2DPoint(B[i], B[j]);

	if (inPlane(A)) {
		res.push_back(nA);
	}
	if (inPlane(B)) {
		res.push_back(nB);
	}
	if (res.empty()) {
		std::set<Rational> lambdas;
		for (int k = 0; k < A.dimension(); ++k) {
			if (k != i && k != j) {
				Rational num = -B[k];
				Rational den = A[k] - B[k];
				if (den == 0) {
					if (num == 0) {
						// Nothing to do
					} else {
						// No intersection;
						res.clear();
						return res;
					}
				} else {
					Rational lambda = num / den;
					lambdas.insert(lambda);
				}
			}
		}
		assert(lambdas.size() == 1);
		Rational lambda = *lambdas.begin();
		if (0 <= lambda && lambda <= 1) {
			Rational2DPoint inter = lambda * nA + (1 - lambda) * nB;
			res.push_back(inter);
		}
	}

	return res;

}

long long EulerianCoorientationWrapper::cross(const HomologieValue& O, const HomologieValue& A, const HomologieValue& B) {
	return (A[0] - O[0]) * (B[1] - O[1]) - (A[1] - O[1]) * (B[0] - O[0]);
}

Rational EulerianCoorientationWrapper::cross(const Rational2DPoint& O, const Rational2DPoint& A, const Rational2DPoint& B) {
	return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

Rational EulerianCoorientationWrapper::volume(const std::vector<EulerianCoorientationWrapper>& v) {
	unsigned int i = std::count_if(v.begin(), v.end(), [](const EulerianCoorientationWrapper& ECW) {
		                               return !ECW.onBoundiary;
	                               });
	unsigned int b = std::count_if(v.begin(), v.end(), [](const EulerianCoorientationWrapper& ECW) {
		                               return ECW.onBoundiary;
	                               });
	assert(i+b == v.size());
	return 2*(i + Rational(1, 2) * b - 1);
}

Rational EulerianCoorientationWrapper::symplecticArea(const std::set<HomologieValue>& s) {
	if (s.size() == 0) {
		return 0;
	}

	size_t dim = s.begin()->dimension();

	// check
	assert(std::all_of(s.begin(), s.end(), [&s](const HomologieValue& hval){return hval.dimension() == s.begin()->dimension(); }));
	assert(dim % 2 == 0);

	if (dim == 2) {
		std::vector<EulerianCoorientationWrapper> vec;
		for each(const auto& hval in s) {
			vec.push_back(EulerianCoorientationWrapper(hval));
		}
		return volume(vec);
	}

	std::vector<HomologieValue> buff;
	for each (const auto& hval in s) {
		buff.push_back(hval);
	}

	size_t half_dimension = dim / 2;
	std::vector<std::vector<Rational2DPoint>> projs(half_dimension);

	for (int i = 0; i < buff.size(); ++i) {
		for (int j = i + 1; j < buff.size(); ++j) {
			const HomologieValue& A = buff[i];
			const HomologieValue& B = buff[j];
			for (int k = 0; k < half_dimension; ++k) {
				int k0 = 2 * k;
				int k1 = 2 * k + 1;
				std::vector<Rational2DPoint> corr = planeCorrespondance(A, B, k0, k1);
				for each (const auto& rP in corr) {
					projs[k].push_back(rP);
				}
			}
		}
	}

	Rational res(0);
	for (int k = 0; k < half_dimension; ++k) {
		res += computeArea(computeConvexHull2D(projs[k]));
	}

	return res;
}

Rational EulerianCoorientationWrapper::area(const Rational2DPoint& A, const Rational2DPoint& B, const Rational2DPoint& C) {
	return Rational(1, 2) * Rational2DPoint::det(B - A, C - A);
}

Rational EulerianCoorientationWrapper::computeArea(const std::vector<Rational2DPoint>& v) {
	if (v.size() <= 3) {
		return 0;
	}
	Rational res(0);
	for (int i = 1; i < v.size(); ++i) {
		const Rational2DPoint& p1 = v[i];
		const Rational2DPoint& p2 = v[i + 1];
		res += area(v[0], p1, p2);
	}
	return res;
}

std::vector<Rational2DPoint> EulerianCoorientationWrapper::computeConvexHull2D(const std::set<Rational2DPoint>& s) {

	std::vector<Rational2DPoint> convHullS;
	for each (const auto& hVal in s) {
		convHullS.push_back(hVal);
	}
	//assert(std::is_sorted(convHullS.begin(),convHullS.end()));

	int n = convHullS.size(), k = 0;
	std::vector<Rational2DPoint> H(2 * n);

	// Build lower hull
	for (int i = 0; i < n; ++i) {
		while (k >= 2 && cross(H[k - 2], H[k - 1], convHullS[i]) <= 0) k--;
		H[k++] = convHullS[i];
	}

	// Build upper hull
	for (int i = n - 2, t = k + 1; i >= 0; i--) {
		while (k >= t && cross(H[k - 2], H[k - 1], convHullS[i]) <= 0) k--;
		H[k++] = convHullS[i];
	}

	H.resize(k - 2);
	return H;
}

std::vector<Rational2DPoint> EulerianCoorientationWrapper::computeConvexHull2D(const std::vector<Rational2DPoint>& v) {
	struct comp {
		bool operator() (const Rational2DPoint& lhs, const Rational2DPoint& rhs) const {
			return (lhs.x < rhs.x) || ((lhs.x == rhs.x) && (lhs.y < rhs.y));
		}
	};
	
	std::vector<Rational2DPoint> convHullS;
	for each (const auto& hVal in v) {
		convHullS.push_back(hVal);
	}
	std::sort(convHullS.begin(), convHullS.end(), comp());
	assert(std::is_sorted(convHullS.begin(), convHullS.end(),comp()));

	int n = convHullS.size(), k = 0;
	std::vector<Rational2DPoint> H(2 * n);

	// Build lower hull
	for (int i = 0; i < n; ++i) {
		while (k >= 2 && cross(H[k - 2], H[k - 1], convHullS[i]) <= 0) k--;
		H[k++] = convHullS[i];
	}

	// Build upper hull
	for (int i = n - 2, t = k + 1; i >= 0; i--) {
		while (k >= t && cross(H[k - 2], H[k - 1], convHullS[i]) <= 0) k--;
		H[k++] = convHullS[i];
	}

	H.resize(k - 2);
	return H;
}
