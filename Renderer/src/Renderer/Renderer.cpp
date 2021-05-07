#include "Renderer.h"

#include <limits>
#include <algorithm>

#include "MathLib/mat4.h"
#include "MathLib/vec4.h"

#include <cmath>

using namespace RNDR;

Renderer::Renderer(int backgroundColor) :backgroundColor(backgroundColor) {}

Renderer::~Renderer() {
	if (this->zBuffer != nullptr) delete[] this->zBuffer;
	if (this->screen != nullptr) delete[] this->screen;
}

void RNDR::Renderer::setDimensions(int width, int height) {
	this->width = width;
	this->height = height;
	if (this->zBuffer != nullptr) delete[] this->zBuffer;
	if (this->screen != nullptr) delete[] this->screen;
	size_t pixelCount = (static_cast<size_t>(width) * static_cast<size_t>(height));
	this->zBuffer = new double[pixelCount];
	this->screen = new int[pixelCount];
	this->tmpScreen.assign(pixelCount, false);
}

RNDR::Dimensions RNDR::Renderer::getDimensions() {
	return Dimensions{ this->width,this->height };
}

void RNDR::Renderer::enableHiQualityLight(bool enabled) {
	this->isHiQualityLight = enabled;
}

void RNDR::Renderer::render(const Scene& scene, const Camera& camera) {

	size_t pixelCount = (static_cast<size_t>(this->width) * static_cast<size_t>(this->height));
	std::fill(this->zBuffer, this->zBuffer + pixelCount, std::numeric_limits<int>::min());
	std::fill(this->screen, this->screen + pixelCount, this->backgroundColor);

	ML::vec4<double> viewportTransformVec = ML::vec4<double>((this->width / 2.0), (this->height / 2.0), 0.0);
	auto cameraProjectionMetrix = camera.getProjectionMetrix();
	auto a = (scene.light.position * cameraProjectionMetrix);
	ML::vec4<double> lightPosition = ML::translate(a, viewportTransformVec);
	auto modelTransformComponent = scene.transforms.begin();
	for (auto mesh = scene.meshes.begin(); mesh != scene.meshes.end(); mesh++, modelTransformComponent++) {
		size_t index = 0;
		auto normalv = mesh->normals.begin();
		ML::mat4<double> modelTransform = modelTransformComponent->getTransform();
		for (auto vertexIndices = mesh->faces.begin(); vertexIndices != mesh->faces.end(); vertexIndices++, normalv++) {
			ML::mat4<double> face = ML::mat4<double>(
				mesh->vertices[vertexIndices->operator[](0)],
				mesh->vertices[vertexIndices->operator[](1)],
				mesh->vertices[vertexIndices->operator[](2)]
				) * modelTransform;
			face.setColumn(3, 1);
			face *= cameraProjectionMetrix;
			face.setColumn(3, 1);
			face = ML::translate(face, viewportTransformVec);

			ML::vec4<double> lightNormal = ML::rotate(*normalv, modelTransformComponent->rotation);
			ML::vec4<double> center{ (face[0][0] + face[1][0] + face[2][0]) / 3.0,(face[0][1] + face[1][1] + face[2][1]) / 3.0,(face[0][2] + face[1][2] + face[2][2]) / 3.0,0 };
			ML::vec4<double> zBufferNormal = ML::crossProduct((face[1] - face[0]), (face[2] - face[0]));

			algorithms::floodFill(this->width, this->height, this->tmpScreen, face);

			int color;
			bool isFirstTime = true;
			for (int i = 0; i < pixelCount; i++) {
				if (this->tmpScreen[i] == true) {
					int y = (i / this->width);
					int x = i - (y * this->width);
					double z = (ML::tripleProduct(face[0], face[1], face[2]) - x * zBufferNormal[0] - y * zBufferNormal[1]) / zBufferNormal[2];

					if (this->isHiQualityLight || isFirstTime) {
						ML::vec4<double> lightDirection = lightPosition - (this->isHiQualityLight ? ML::vec4<double>{static_cast<double>(x), static_cast<double>(y), z} : center);
						double illumination = ML::getAngle(lightNormal, lightDirection);
						if (illumination < 0) illumination = 0;
						illumination += 0.3;
						int b = static_cast<int>(std::round(static_cast<unsigned char>(mesh->color) * illumination));
						int g = static_cast<int>(std::round(static_cast<unsigned char>(mesh->color >> 8) * illumination));
						int r = static_cast<int>(std::round(static_cast<unsigned char>(mesh->color >> 16) * illumination));
						b = std::max(std::min(b, 255), 0);
						g = std::max(std::min(g, 255), 0);
						r = std::max(std::min(r, 255), 0);
						color = b + (g << 8) + (r << 16);
						isFirstTime = false;
					}

					if (z >= this->zBuffer[i]) {
						this->zBuffer[i] = z;
						this->screen[i] = color;
					}
				}
			}
		}
	}
}

int* RNDR::Renderer::getScreen() {
	return this->screen;
}
