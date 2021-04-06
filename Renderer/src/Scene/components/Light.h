#pragma once

#include "MathLib/vec4.h"

namespace RNDR {

	namespace components {

		class Light {
		public:
			Light();
			Light(const Light&);
			~Light();
			Light(const ML::vec4<double>& position);

			Light& operator=(const Light&);

			ML::vec4<double> position;
		};

	}

}