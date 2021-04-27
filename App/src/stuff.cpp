#include "pch.h"
#include "stuff.h"

#include "config.h"

#include "Renderer/Renderer.h"
#include "MathLib/literals.h"
#include "Log/Log.h"

#include <chrono>

static RNDR::Renderer renderer{};
static RNDR::Scene scene{};
static RNDR::Camera camera{};

using namespace RNDR::Literals;

void stuff::init() {
	/*scene.addMesh(RNDR::components::Mesh({ 
											{-1, 1, 0},
											{ 1, -1, 0},
											{ 1, 1, 0}
										 }, { {0,1,2} }, 0x000FF00FF), RNDR::components::Transform{ {300,300,300} });*/
	scene.addMesh(RNDR::components::Mesh({
											 {-1,-1, 1},
											 { 1,-1, 1},
											 { 1, 1, 1},
											 {-1, 1, 1},
											 {-1,-1,-1},
											 { 1,-1,-1},
											 { 1, 1,-1},
											 {-1, 1,-1}
										 }, {
											 {0, 2, 1},
											 {0, 3, 2},
											 {1, 6, 5},
											 {1, 2, 6},
											 {5, 7, 4},
											 {5, 6, 7},
											 {4, 3, 0},
											 {4, 7, 3},
											 {4, 1, 5},
											 {4, 0, 1},
											 {3, 6, 7},
											 {3, 2, 6},
										 }, 0x0055FF));

	scene.addMesh(RNDR::components::Mesh({
											 {-1, 1, 1},
											 { 0, 1,-1},
											 { 1, 1, 1},
											 { 0,-2, 0},
										 }, {
											 {3, 0, 2},
											 {3, 2, 1},
											 {3, 1, 0},
											 {0, 2, 1},
										 }, 0xFF5500));
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
}

void stuff::changeLight(int x, int y, int z) {
	scene.addLight(RNDR::components::Light({ static_cast<double>(x), static_cast<double>(y), static_cast<double>(z) }));
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
						{config::transforms::figure1::positionX, config::transforms::figure1::positionY, config::transforms::figure1::positionZ},
						{toRad(config::transforms::figure1::rotationX), toRad(config::transforms::figure1::rotationY), toRad(config::transforms::figure1::rotationZ)},
						{config::transforms::figure1::scaleX, config::transforms::figure1::scaleY, config::transforms::figure1::scaleZ}
					   });
	scene.setTransform(1, {
						{config::transforms::figure2::positionX, config::transforms::figure2::positionY, config::transforms::figure2::positionZ},
						{toRad(config::transforms::figure2::rotationX), toRad(config::transforms::figure2::rotationY), toRad(config::transforms::figure2::rotationZ)},
						{config::transforms::figure2::scaleX, config::transforms::figure2::scaleY, config::transforms::figure2::scaleZ}
					   });
	scene.addLight(RNDR::components::Light({ config::transforms::light::positionX, config::transforms::light::positionY,  config::transforms::light::positionZ }));
	LOG_INFO("Transform: Full Reset");
};
