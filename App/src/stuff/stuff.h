#pragma once
#include "pch.h"

#include "Renderer/Renderer.h"

namespace stuff {

	enum class TransfromType : int {
		Translation,
		Rotation,
		Scale
	};

	enum class FigureType : int {
		Cube,
		TriangularPyramid
	};

	class Scene;
	class Viewport;

	class Scene {
	public:
		void addFigure(FigureType, int color);

		void changeFigure(int figureNum, TransfromType, int x, int y, int z);
		void changeLight(int x, int y, int z);

	private:
		RNDR::Scene scene;

		friend Viewport;
	};

	class Viewport {
	public:
		Viewport() = delete;
		Viewport(stuff::Scene* stuffScene, int backgroundColor) :scene(&(stuffScene->scene)), renderer(backgroundColor) {};
		void changeDimensions(int width, int height);
		void changeCamera(double dx, double dy, bool isAngle);

		void draw(HDC);

		void reset();

	private:
		RNDR::Renderer renderer;
		RNDR::Camera camera;
		RNDR::Scene* scene;

		double yawCameraAngle = 0;
		double pitchCameraAngle = 0;
		double xCameraPosition = 0;
		double yCameraPosition = 0;

	};

}
