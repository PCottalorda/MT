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

