#pragma once


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


/**

template <typename T>
class Segment {
public:
	Segment(const T& t1, const T& t2) {
	};

	~Segment() {
	};

	static T interCoeffs(const T& t1, const T& t2) {
		T C0 = t1.c0 - t1.c1;
		T C1 = t2.c1 - t2.c0;
		auto det = C0.x * C1.y - C0.y * C1.x;
		T X = t2.c1 - t1.c1;
		T res;
		res.x = C1.c1 * X.c0 - C1.c0 * X.c1;
		res.y = C0.c0 * X.c1 - C0.c1 * X.c0;

		return res;
	}

	T c0;
	T c1;
};

**/