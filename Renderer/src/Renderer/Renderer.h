#pragma once

#include <functional>
#include <vector>

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
		Renderer(int backgroundColor = 0);
		~Renderer();
		Renderer(Renderer&) = delete;
		Renderer& operator=(Renderer&) = delete;

		void setDimensions(int width, int height);
		Dimensions getDimensions();

		void enableHiQualityLight(bool enabled);

		void render(const Scene&, const Camera&);
		int* getScreen();

	private:
		int width = 0;
		int height = 0;
		double* zBuffer = nullptr;
		int* screen = nullptr;
		std::vector<bool> tmpScreen{};

		int backgroundColor = 0;

		bool isHiQualityLight = false;
	};

}
