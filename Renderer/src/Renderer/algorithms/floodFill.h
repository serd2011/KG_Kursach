#pragma once

#include "MathLib/mat4.h"

#include <vector>

namespace RNDR {

	namespace algorithms {

		bool floodFill(int width, int height,std::vector<bool>& screen,const ML::mat4<int>& vertices);

	}
}