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

	size_t pixelCount = (static_cast<size_t>(this->width) * static_cast<size_t>(this->height));
	std::fill(this->zBuffer, this->zBuffer + pixelCount, std::numeric_limits<int>::min());
	std::fill(this->screen, this->screen + pixelCount, 0xE1E1E1);

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

			ML::vec4<double> normal = ML::crossProduct((face[1] - face[0]), (face[2] - face[0]));
			ML::vec4<double> lightDirection = face[0]-scene.light.position;
			double illumination = ML::getAngle(normal, lightDirection);
			int b = static_cast<int>(std::round(static_cast<unsigned char>(mesh->color) * illumination));
			int g = static_cast<int>(std::round(static_cast<unsigned char>(mesh->color >> 8) * illumination));
			int r = static_cast<int>(std::round(static_cast<unsigned char>(mesh->color >> 16) * illumination));
			b = std::max(std::min(r, 255), 0);
			g = std::max(std::min(g, 255), 0);
			r = std::max(std::min(b, 255), 0);
			int color = b + (static_cast<int>(g) << 8) + (static_cast<int>(r) << 16);

			color = mesh->color;

			algorithms::floodFill(this->width, this->height, this->tmpScreen, face, color);
			for (int i = 0; i < pixelCount; i++) {
				if (this->tmpScreen[i] != color - 1) {
					int y = (i / this->width);
					int x = i - (y * this->width);
					int z = static_cast<int>((ML::tripleProduct(face[0], face[1], face[2]) - x * normal[0] - y * normal[1]) / (normal[2]));
					if (z >= this->zBuffer[i]) {
						this->zBuffer[i] = z;
						this->screen[i] = this->tmpScreen[i];
					}
				}				
			}
		}
	}

	// Создание тени
}

int* RNDR::Renderer::getScreen() {
	return this->screen;
}
