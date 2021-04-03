#include "Renderer.h"

#include <limits>
#include <algorithm>

#include "MathLib/mat4.h"
#include "MathLib/vec4.h"

using namespace RNDR;

Renderer::Renderer() {}

Renderer::~Renderer() {
	if (this->zBuffer != nullptr) delete[] this->zBuffer;
	if (this->screen != nullptr) delete[] this->screen;
	if (this->tmpScreen != nullptr) delete[] this->tmpScreen;
}

void RNDR::Renderer::setDimensions(int width, int height) {
	this->width = width;
	this->height = height;
	if (this->zBuffer != nullptr) delete[] this->zBuffer;
	if (this->screen != nullptr) delete[] this->screen;
	if (this->tmpScreen != nullptr) delete[] this->tmpScreen;
	size_t pixelCount = (static_cast<size_t>(width) * static_cast<size_t>(height));
	this->zBuffer = new int[pixelCount];
	this->screen = new int[pixelCount];
	this->tmpScreen = new int[pixelCount];
}

RNDR::Dimensions RNDR::Renderer::getDimensions() {
	return Dimensions{ this->width,this->height };
}

void RNDR::Renderer::render(const Scene& scene, const Camera& camera) {

	size_t pixelCount = (static_cast<size_t>(width) * static_cast<size_t>(height));
	std::fill(this->zBuffer, this->zBuffer + pixelCount, std::numeric_limits<int>::min());
	std::fill(this->screen, this->screen + pixelCount, 0x00FFFFFF);

	auto transfrom = scene.transforms.begin();
	for (auto mesh = scene.meshes.begin(); mesh != scene.meshes.end(); mesh++, transfrom++) {
		size_t index = 0;
		ML::mat4 transfromMatrix = transfrom->getTransform();
		for (auto vertexIndices = mesh->faces.begin(); vertexIndices != mesh->faces.end(); vertexIndices++, index++) {
			ML::mat4<double> face = ML::mat4<double>(
				mesh->vertices[vertexIndices->operator[](0)],
				mesh->vertices[vertexIndices->operator[](1)],
				mesh->vertices[vertexIndices->operator[](2)]
				) * transfromMatrix;
			/*ML::vec4<double> normal = (face[0] - face[1]) * (face[0] - face[2]);
			ML::vec4<double> lightDirection = ML::vec4<double>((face[0][0] + face[1][0] + face[2][0]) / 3.0, (face[0][1] + face[1][1] + face[2][1]) / 3.0, (face[0][2] + face[1][2] + face[2][2]) / 3.0) - scene.light.position;
			double illumination = ML::getAngle(normal, lightDirection);
			*/
			int color = mesh->color + (index * 10);
			algorithms::floodFill(this->width, this->height, this->tmpScreen, face, color);
			for (int i = 0; i < pixelCount; i++) {
				if (this->tmpScreen[i] == color) {
					// Проверить Z координату
					this->screen[i] = color;
				}
			}
		}
	}

	// Создание тени
}

int* RNDR::Renderer::getScreen() {
	return this->screen;
}
