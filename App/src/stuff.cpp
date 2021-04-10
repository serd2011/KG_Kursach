#include "pch.h"
#include "stuff.h"

#include "Renderer/Renderer.h"
#include "MathLib/literals.h"
#include "Log/Log.h"

#include <chrono>

static RNDR::Renderer renderer{};
static RNDR::Scene scene{};
static RNDR::Camera camera{};

using namespace RNDR::Literals;

void stuff::init() {
	//scene.addMesh(RNDR::components::Mesh({ {-50,-50,-50},{0,50,0},{50,-50,50} }, { {0,1,2} }, 0x000FF00FF), RNDR::components::Transform{ {300,300,100} });
	scene.addMesh(RNDR::components::Mesh({
											{-1,-1,1},
											{1,-1,1},
											{1,1,1},
											{-1,1,1},
											{-1,-1,-1},
											{1,-1,-1},
											{1,1,-1},
											{-1,1,-1}
										 }, {
											 {0,2,1},
											 {0,3,2},
											 {1,6,5},
											 {1,2,6},
											 {5,7,4},
											 {5,6,7},
											 {4,3,0},
											 {4,7,3},
											 {4,1,5},
											 {4,0,1},
											 {3,6,7},
											 {3,2,6},
										 }, 0x0055FF), RNDR::components::Transform{ {500,500,0},{60_deg,0,-45_deg},{200,200,200} });

	scene.addMesh(RNDR::components::Mesh({
										{-1,-1,1},
										{1,-1,1},
										{1,1,1},
										{-1,1,1},
										{-1,-1,-1},
										{1,-1,-1},
										{1,1,-1},
										{-1,1,-1}
										 }, {
											 {0,2,1},
											 {0,3,2},
											 {1,6,5},
											 {1,2,6},
											 {5,7,4},
											 {5,6,7},
											 {4,3,0},
											 {4,7,3},
											 {4,1,5},
											 {4,0,1},
											 {3,6,7},
											 {3,2,6},
										 }, 0xFF5500), RNDR::components::Transform{ {1000,500,0},{25_deg,0,50_deg},{280,200,200} });

	scene.addLight(RNDR::components::Light({ 1000,500,200 }));
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
void stuff::resetAll() {
	scene.addLight(RNDR::components::Light({ 0,0,0 }));
};
