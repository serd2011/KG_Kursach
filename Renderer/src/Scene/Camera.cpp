#include "Camera.h"

RNDR::Camera::Camera() {
	this->recalculateMatrix();
}

RNDR::Camera::~Camera() {}

const ML::mat4<double>& RNDR::Camera::getProjectionMatrix() const {
	return this->matrix;
}

void RNDR::Camera::setPosition(ML::vec4<double> position, ML::vec4<double>target) {
	this->position = position;
	this->target = target;
	this->recalculateMatrix();
}

void RNDR::Camera::recalculateMatrix() {

	ML::vec4<double> up = ML::vec4<double>(0.0, 1.0, 0.0);
	ML::vec4<double> f = ML::normalize(this->target - this->position);
	ML::vec4<double> u = ML::normalize(up);
	ML::vec4<double> s = ML::normalize(ML::crossProduct(f, u));
	u = ML::crossProduct(s, f);

	this->matrix = ML::mat4<double>(
		{ s[0], u[0], -f[0], 0 },
		{ s[1], u[1], -f[1], 0 },
		{ s[2], u[2], -f[2], 0 },
		{ -ML::dotProduct(s, this->position), -ML::dotProduct(u, this->position) ,ML::dotProduct(f, this->position), 1 }
	);
	
}
