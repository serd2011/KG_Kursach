#pragma once

#include "MathLib/mat4.h"

namespace RNDR {

	namespace algorithms {

		void floodFill(int width, int height, int* screen,const ML::mat4<int>& vertices, int color);

	}
}