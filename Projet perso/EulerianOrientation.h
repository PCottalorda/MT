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

#include "Edge.h"

class Graph;

class EulerianOrientation {
	friend Graph;

public:
	EulerianOrientation(Graph* G, int tag);
	~EulerianOrientation();

	std::string generateGraphVizString() const;
	HomologieValue cycleValue() const;

private:


	Graph* BaseGraph;
	std::vector<Edge> edges;

	int internalNumber;
};
