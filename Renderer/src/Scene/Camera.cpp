#include "Camera.h"

RNDR::Camera::Camera() {}

RNDR::Camera::~Camera() {}

const ML::mat4<double>& RNDR::Camera::getProjectionMetrix() const{
	return this->matrix;
}

void RNDR::Camera::setFov(int fovx, int fovy) {
	this->fovx = fovx;
	this->fovy = fovy;
	this->recalculateMatrix();
}

void RNDR::Camera::setPosition(ML::vec4<double> position, ML::vec4<double> direction) {
	this->position = position;
	this->direction = direction;
	this->recalculateMatrix();
}

void RNDR::Camera::recalculateMatrix() {}
