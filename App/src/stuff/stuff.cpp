#include "pch.h"
#include "stuff.h"

#include <cmath>
#include <chrono>

#include "MathLib/literals.h"

#include "Scene/components/sampleMeshes/pack1.h"

using namespace RNDR::Literals;

void stuff::Scene::addFigure(FigureType type, int color) {
	switch (type) {
	case stuff::FigureType::Cube:
		this->scene.addMesh(RNDR::Samples::Mesh::createCubeMesh(color));
		break;
	case stuff::FigureType::TriangularPyramid:
		this->scene.addMesh(RNDR::Samples::Mesh::createTriangularPyramidMesh(color));
		break;
	default:
		LOG_ERROR("Unsupported Figure Type: %d", type);
		break;
	}

}

void stuff::Scene::changeLight(int x, int y, int z) {
	this->scene.addLight(RNDR::components::Light({ static_cast<double>(x), static_cast<double>(y), static_cast<double>(z) }));
}

void stuff::Scene::changeFigure(int num, TransfromType type, int x, int y, int z) {
	auto& transform = this->scene.getTransform(num);
	switch (type) {
	case stuff::TransfromType::Translation:
		transform.translation = { x, y, z };
		break;
	case stuff::TransfromType::Rotation:
		transform.rotation = { toRad(x), toRad(y), toRad(z) };
		break;
	case stuff::TransfromType::Scale:
		transform.scale = { x, y, z };
		break;
	default:
		LOG_ERROR("Unsupported Transfrom Type: %d", type);
		return;
	}
	LOG_INFO("Transform figure %d: changed %d to {%d, %d, %d}", num, type, x, y, z);
}

void stuff::Viewport::reset() {
	yawCameraAngle = config::camera::yawAngle;
	pitchCameraAngle = config::camera::pitchAngle;
	xCameraPosition = config::camera::cameraX;
	yCameraPosition = config::camera::cameraY;
	this->changeCamera(0.0, 0.0, true);
	LOG_INFO("Viewport: Reset");
};


void stuff::Viewport::draw(HDC hdc) {
	auto t1 = std::chrono::high_resolution_clock::now();

	this->renderer.render(*(this->scene), this->camera);
	auto screen = this->renderer.getScreen();
	auto dimensions = this->renderer.getDimensions();
	HBITMAP map = CreateBitmap(dimensions.width, dimensions.height, 1, sizeof(int) * 8, (void*)screen);
	HDC src = CreateCompatibleDC(hdc);
	SelectObject(src, map);
	BitBlt(hdc, 0, 0, dimensions.width, dimensions.height, src, 0, 0, SRCCOPY);
	DeleteDC(src);

	auto t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::ratio<1>> duration = t2 - t1;
	LOG_INFO("Render Time: %fs", duration);
}

void stuff::Viewport::changeDimensions(int width, int height) {
	this->renderer.setDimensions(width, height);
	LOG_INFO("Dimensions set to {%d, %d}", width, height);
}

void stuff::Viewport::changeCamera(double dx, double dy, bool isAngle) {
	if (isAngle) {
		this->yawCameraAngle += dx * config::camera::angleMultiplier;
		this->pitchCameraAngle += dy * config::camera::angleMultiplier;
	} else {
		this->xCameraPosition -= dx * config::camera::positionMultiplier;
		this->yCameraPosition += dy * config::camera::positionMultiplier;
	}
	if (this->pitchCameraAngle > 89) this->pitchCameraAngle = 89;
	if (this->pitchCameraAngle < -89) this->pitchCameraAngle = -89;
	double x = std::cos(toRad(this->yawCameraAngle)) * std::cos(toRad(this->pitchCameraAngle));
	double y = std::sin(toRad(this->pitchCameraAngle));
	double z = std::sin(toRad(this->yawCameraAngle)) * std::cos(toRad(this->pitchCameraAngle));
	camera.setPosition({ x + this->xCameraPosition,y + this->yCameraPosition,z }, { this->xCameraPosition,this->yCameraPosition,0 });
	LOG_INFO("Yaw: %fdeg Pitch: %fdeg", this->yawCameraAngle, this->pitchCameraAngle);
}
