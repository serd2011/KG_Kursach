#include "Mesh.h"

using namespace RNDR::components;

Mesh::Mesh() {};

Mesh::Mesh(const Mesh& other) :
	vertices(other.vertices),
	faces(other.faces),
	normals(other.normals),
	color(other.color) {};

Mesh::~Mesh() {}

Mesh::Mesh(const std::vector<ML::vec4<double>>& vertices, const std::vector<ML::vec4<size_t>>& faces, const std::vector<ML::vec4<double>>& normals, int color) :
	vertices(vertices),
	faces(faces),
	normals(normals),
	color(color) {}

Mesh& Mesh::operator=(const Mesh& other) {
	this->vertices = other.vertices;
	this->faces = other.faces;
	return *this;
}