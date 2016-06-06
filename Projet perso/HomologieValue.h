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


#include <vector>
#include <ostream>

class HomologieValue
{
public:

	class DimensionException : public std::exception {};

	HomologieValue(unsigned int dimension);;
	HomologieValue(const HomologieValue& other);
	HomologieValue(HomologieValue&& other);
	~HomologieValue();

	HomologieValue& operator=(const HomologieValue& other);
	HomologieValue& operator=(HomologieValue&& other);

	friend bool operator==(const HomologieValue& lhs, const HomologieValue& rhs);
	friend bool operator!=(const HomologieValue& lhs, const HomologieValue& rhs);
	friend bool operator<(const HomologieValue& lhs, const HomologieValue& rhs) {
		if (lhs.dimension() != rhs.dimension()) {
			throw DimensionException();
		}
		for (unsigned int i = 0; i < lhs.dimension(); ++i) {
			if (lhs[i] == rhs[i]) {
				continue;
			}
			else if (lhs[i] < rhs[i]) {
				return true;
			} else {
				return false;
			}
		}
		return false;
	}

	int operator[](int i) const;
	int& operator[](int i);

	HomologieValue operator-() const;

	HomologieValue& operator+=(const HomologieValue& rhs);
	HomologieValue& operator-=(const HomologieValue& rhs);
	friend HomologieValue operator+(HomologieValue lhs, const HomologieValue& rhs);
	friend HomologieValue operator-(HomologieValue lhs, const HomologieValue& rhs);
	unsigned int dimension() const;

	friend std::ostream& operator<<(std::ostream& os, const HomologieValue& hVal);

private:
	unsigned int dim;
	std::vector<int> base;
};

