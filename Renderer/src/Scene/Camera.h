#pragma once

#include "MathLib/mat4.h"

namespace RNDR {

	class Camera {
	public:
		Camera();
		~Camera();
		Camera(Camera&) = delete;
		Camera& operator=(Camera&) = delete;

		const ML::mat4<double>& getProjectionMatrix() const;

		void RNDR::Camera::setPosition(ML::vec4<double> position, ML::vec4<double>target);


	private:
		ML::vec4<double> position;
		ML::vec4<double> target;
		ML::mat4<double> matrix;

		void recalculateMatrix();
	};

}
