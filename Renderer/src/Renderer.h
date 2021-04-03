#pragma once
#include "all.h"

#include <functional>

#include "Camera.h"
#include "Scene.h"
#include "algorithms/FloodFill.h"
#include "algorithms/ZBuffer.h"

namespace RNDR {

	using putPuxelFunctionType = std::function<void(int x, int y, int color)>;

	struct Dimensions {
		int width;
		int height;
	};

	class Renderer {
	public:
		Renderer();
		~Renderer();
		Renderer(Renderer&) = delete;
		Renderer& operator=(Renderer&) = delete;

		void setDimensions(int width, int height);
		Dimensions getDimensions();

		void render(const Scene&, const Camera&);
		int* getScreen();

	private:
		int width;
		int height;
		int* zBuffer;
		int* screen;
		int* tmpScreen;
	};

}
