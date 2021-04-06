#pragma once

#include "MathLib/mat4.h"

namespace RNDR {

	class Camera {
	public:
		Camera();
		~Camera();
		Camera(Camera&) = delete;
		Camera& operator=(Camera&) = delete;

		ML::mat4<double>& getProjectionMetrix();

		void setFov(int fovx, int fovy);
		void setPosition(ML::vec4<double> position, ML::vec4<double> direction);

	private:
		int fovx = 60;
		int fovy = 40;
		ML::vec4<double> position;
		ML::vec4<double> direction;
		ML::mat4<double> matrix;

		void recalculateMatrix();
	};

}
