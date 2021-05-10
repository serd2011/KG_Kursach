#include "pch.h"
#include "stuff.h"

#include "config.h"

#include "Renderer/Renderer.h"
#include "MathLib/literals.h"
#include "Log/Log.h"

#include "Scene/components/sampleMeshes/pack1.h"

#include <chrono>
#include <cmath>

static RNDR::Renderer renderer(static_cast<int>(config::content::background));
static RNDR::Scene scene;
static RNDR::Camera camera;

using namespace RNDR::Literals;

static double yawCameraAngle;
static double pitchCameraAngle;
static double xCameraPosition = 0;
static double yCameraPosition = 0;

void stuff::init() {
	scene.addMesh(RNDR::Samples::Mesh::createCubeMesh(config::objects::figure1::color));
	scene.addMesh(RNDR::Samples::Mesh::createTriangularPyramidMesh(config::objects::figure2::color));
}

void stuff::draw(HDC hdc) {
	auto t1 = std::chrono::high_resolution_clock::now();

	renderer.render(scene, camera);
	auto screen = renderer.getScreen();
	auto dimensions = renderer.getDimensions();
	HBITMAP map = CreateBitmap(dimensions.width, dimensions.height, 1, sizeof(int) * 8, (void*)screen);
	HDC src = CreateCompatibleDC(hdc);
	SelectObject(src, map);
	BitBlt(hdc, 0, 0, dimensions.width, dimensions.height, src, 0, 0, SRCCOPY);
	DeleteDC(src);

	auto t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::ratio<1>> duration = t2 - t1;
	LOG_INFO("Render Time: %fs", duration);
}

void stuff::changeDimensions(int width, int height) {
	renderer.setDimensions(width, height);
	LOG_INFO("Dimensions set to {%d, %d}", width, height);
}

void stuff::changeLight(int x, int y, int z) {
	scene.addLight(RNDR::components::Light({ static_cast<double>(x), static_cast<double>(y), static_cast<double>(z) }));
}

void stuff::enableHiQualityLight(bool enable) {
	renderer.enableHiQualityLight(enable);
}

void stuff::changeCamera(double dx, double dy, bool isAngle){
	if (isAngle) {
		yawCameraAngle += dx * 200;
		pitchCameraAngle += dy * 200;
	} else {
		xCameraPosition -= dx * 500;
		yCameraPosition += dy * 500;
	}
	if (pitchCameraAngle > 90) pitchCameraAngle = 90;
	if (pitchCameraAngle < -90) pitchCameraAngle = -90;
	double x = std::cos(toRad(yawCameraAngle)) * std::cos(toRad(pitchCameraAngle));
	double y = std::sin(toRad(pitchCameraAngle));
	double z = std::sin(toRad(yawCameraAngle)) * std::cos(toRad(pitchCameraAngle));
	camera.setPosition({ x + xCameraPosition,y+ yCameraPosition,z }, { xCameraPosition,yCameraPosition,0 });
	LOG_INFO("Yaw: %fdeg Pitch: %fdeg", yawCameraAngle, pitchCameraAngle);
}

void stuff::changeFigure(int num, TransfromType type, int x, int y, int z) {
	auto& transform = scene.getTransform(num);
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

void stuff::resetAll() {
	scene.setTransform(0, {
						{config::objects::figure1::positionX, config::objects::figure1::positionY, config::objects::figure1::positionZ},
						{toRad(config::objects::figure1::rotationX), toRad(config::objects::figure1::rotationY), toRad(config::objects::figure1::rotationZ)},
						{config::objects::figure1::scaleX, config::objects::figure1::scaleY, config::objects::figure1::scaleZ}
					   });
	scene.setTransform(1, {
						{config::objects::figure2::positionX, config::objects::figure2::positionY, config::objects::figure2::positionZ},
						{toRad(config::objects::figure2::rotationX), toRad(config::objects::figure2::rotationY), toRad(config::objects::figure2::rotationZ)},
						{config::objects::figure2::scaleX, config::objects::figure2::scaleY, config::objects::figure2::scaleZ}
					   });
	scene.addLight(RNDR::components::Light({ config::objects::light::positionX, config::objects::light::positionY,  config::objects::light::positionZ }));
	yawCameraAngle = config::camera::yawAngle;
	pitchCameraAngle = config::camera::pitchAngle;
	xCameraPosition = config::camera::cameraX;
	yCameraPosition = config::camera::cameraY;
	changeCamera(0.0, 0.0,true);
	LOG_INFO("Transform: Full Reset");
};
