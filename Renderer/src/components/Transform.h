#pragma once

#include "../MathLib/vec4.h"
#include "../MathLib/matrixTransform.h"

namespace RNDR {

	namespace components {

		struct Transform {

			ML::vec4<double> translation = ML::vec4<double>(0, 0, 0);
			ML::vec4<double> rotation = ML::vec4<double>(0, 0, 0);
			ML::vec4<double> scale = ML::vec4<double>(1, 1, 1);

			ML::mat4<double> getTransform() const {
				return ML::scale(ML::mat4(1.0), scale) *
					ML::rotate(ML::mat4(1.0), rotation) *
					ML::translate(ML::mat4(1.0), translation);
			}

		};

	}
}