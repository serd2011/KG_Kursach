#include "Light.h"

using namespace RNDR::components;

Light::Light() {};

Light::Light(const Light& other) {
	this->position = other.position;
};

Light::~Light() {};

Light::Light(const ML::vec4<double>& position) {
	this->position = position;
}
Light& Light::operator=(const Light& other) {
	this->position = other.position;
	return *this;
};
