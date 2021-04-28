#pragma once
#include "pch.h"

namespace stuff{

	void init();
	void draw(HDC);
	void changeDimensions(int width, int height);
	void changeLight(int, int, int);
	void enableHiQualityLight(bool enable);

	enum class TransfromType : int {
		Translation,
		Rotation,
		Scale
	};

	void changeFigure(int num, TransfromType, int, int, int);

	void resetAll();

}
