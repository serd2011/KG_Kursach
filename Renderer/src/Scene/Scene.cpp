#include "Scene.h"

#include <cmath>

using namespace RNDR;

size_t Scene::addMesh(const components::Mesh& mesh, const components::Transform& transform) {
	this->meshes.push_back(mesh);
	this->transforms.push_back(transform);
	return this->meshes.size();
}

void Scene::removeMesh(size_t index) {
	this->meshes.erase(this->meshes.begin() + index);
	this->transforms.erase(this->transforms.begin() + index);
}

void Scene::setTransform(size_t index, const components::Transform& transform) {
	this->transforms[index] = transform;
}

components::Transform& RNDR::Scene::getTransform(size_t index) {
	return this->transforms[index];
}

void Scene::addLight(const components::Light& light) {
	this->light = light;
}
