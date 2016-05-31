#pragma once

#include <utility>

template <typename T>
class PointOnBoundiaryWrapper {
public:
	~PointOnBoundiaryWrapper() {
	};


	PointOnBoundiaryWrapper(const T& point, bool on_boundiary, int index);
	PointOnBoundiaryWrapper(const PointOnBoundiaryWrapper& other);
	PointOnBoundiaryWrapper(PointOnBoundiaryWrapper&& other);
	PointOnBoundiaryWrapper& operator=(const PointOnBoundiaryWrapper& other);
	PointOnBoundiaryWrapper& operator=(PointOnBoundiaryWrapper&& other);

	friend bool operator==(const PointOnBoundiaryWrapper& lhs, const PointOnBoundiaryWrapper& rhs) {
		return lhs.point == rhs.point
			&& lhs.onBoundiary == rhs.onBoundiary
			&& lhs.index == rhs.index;
	}

	friend bool operator!=(const PointOnBoundiaryWrapper& lhs, const PointOnBoundiaryWrapper& rhs) {
		return !(lhs == rhs);
	}

	T point;
	bool onBoundiary;
	int index;
};


template <typename T>
PointOnBoundiaryWrapper<T>::PointOnBoundiaryWrapper(const T& point, bool on_boundiary, int index): point(point),
                                                                                                   onBoundiary(on_boundiary),
                                                                                                   index(index) {
}

template <typename T>
PointOnBoundiaryWrapper<T>::PointOnBoundiaryWrapper(const PointOnBoundiaryWrapper& other): point(other.point),
                                                                                           onBoundiary(other.onBoundiary),
                                                                                           index(other.index) {
}

template <typename T>
PointOnBoundiaryWrapper<T>::PointOnBoundiaryWrapper(PointOnBoundiaryWrapper&& other): point(std::move(other.point)),
                                                                                      onBoundiary(other.onBoundiary),
                                                                                      index(other.index) {
}

template <typename T>
PointOnBoundiaryWrapper<T>& PointOnBoundiaryWrapper<T>::operator=(const PointOnBoundiaryWrapper& other) {
	if (this == &other)
		return *this;
	point = other.point;
	onBoundiary = other.onBoundiary;
	index = other.index;
	return *this;
}

template <typename T>
PointOnBoundiaryWrapper<T>& PointOnBoundiaryWrapper<T>::operator=(PointOnBoundiaryWrapper&& other) {
	if (this == &other)
		return *this;
	point = std::move(other.point);
	onBoundiary = other.onBoundiary;
	index = other.index;
	return *this;
}

#include "PointOnBoundiaryWrapper.cpp"
