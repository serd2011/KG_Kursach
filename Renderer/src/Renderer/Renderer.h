#pragma once

#include <functional>

#include "Scene/Camera.h"
#include "Scene/Scene.h"
#include "Renderer/algorithms/FloodFill.h"

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
		int width = 0;
		int height = 0;
		int* zBuffer = nullptr;
		int* screen = nullptr;
		int* tmpScreen = nullptr;
	};

}
