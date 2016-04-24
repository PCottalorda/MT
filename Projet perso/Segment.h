#pragma once

template <typename T>
class Segment
{
public:
	Segment(const T& t1, const T& t2) {};
	~Segment() {};

	static T interCoeffs(const T& t1, const T& t2) {
		T C0 = t1.c0 - t1.c1;
		T C1 = t2.c1 - t2.c0;
		auto det = C0.x*C1.y - C0.y*C1.x;
		T X = t2.c1 - t1.c1;
		T res;
		res.x = C1.c1*X.c0 - C1.c0*X.c1;
		res.y = C0.c0*X.c1 - C0.c1*X.c0;

		return res;
	}

	T c0;
	T c1;
};

