#pragma once

#include <vector>

#include "../MathLib/vec4.h"
#include "../MathLib/vec3.h"

namespace RNDR {

	namespace components {

		class Mesh {
		public:
			Mesh();
			Mesh(const Mesh&);
			~Mesh();
			Mesh(const std::vector<ML::vec4<double>>& vertices, const std::vector<ML::vec3<size_t>>& faces, int color = 0);

			Mesh& operator=(const Mesh&);

			std::vector<ML::vec4<double>> vertices;
			std::vector<ML::vec3<size_t>> faces;
			int color = 0;
		};

	}

}