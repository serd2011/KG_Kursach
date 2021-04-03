#include "pch.h"
#include "stuff.h"

#include "Renderer.h"

static RNDR::Renderer renderer{};
static RNDR::Scene scene{};
static RNDR::Camera camera{};

void stuff::init() {
	scene.addMesh(RNDR::components::Mesh({ {-50,-50,0},{0,50,0},{50,-50,0} }, { {0,1,2} }, 0x000000FF), RNDR::components::Transform{ {300,300,0} });
}

void stuff::draw(HDC hdc) {
	renderer.render(scene, camera);
	auto screen = renderer.getScreen();
	auto dimensions = renderer.getDimensions();
	HBITMAP map = CreateBitmap(dimensions.width, dimensions.height, 1, 8 * 4, (void*)screen);
	HDC src = CreateCompatibleDC(hdc);
	SelectObject(src, map);
	BitBlt(hdc, 0, 0, dimensions.width, dimensions.height, src, 0, 0, SRCCOPY);
	DeleteDC(src);
}
void stuff::changeDimensions(size_t width, size_t height) {
	renderer.setDimensions(width, height);
};
