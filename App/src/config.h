#pragma once
#include "pch.h"

namespace config {

	constexpr auto IDS_APP_TITLE = L"Курсач";

	constexpr LONG mainMinWidth = 1200;
	constexpr LONG mainMinHeight = 600;
	constexpr LONG asideWidth = 400;

	namespace aside {
		constexpr int elementHeight = 25;

		namespace fontSize {
			constexpr int small = 12;
			constexpr int normal = 14;
		}

		//colors
	#ifndef APP_DARK_THEME
		constexpr COLORREF background = 0xF1F1F1;
		constexpr COLORREF foreground = 0x000000;
		constexpr COLORREF xLabelBackground = 0x5A5AF1;
		constexpr COLORREF xLabelForeground = 0xFFFFFF;
		constexpr COLORREF yLabelBackground = 0x6FBA4E;
		constexpr COLORREF yLabelForeground = 0xFFFFFF;
		constexpr COLORREF zLabelBackground = 0xBF952D;
		constexpr COLORREF zLabelForeground = 0xFFFFFF;
		constexpr COLORREF editBackground = 0xE5E5E5;
		constexpr COLORREF editForeground = 0x000000;
	#else
		constexpr COLORREF background = 0x181818;
		constexpr COLORREF foreground = 0xE5E5E5;
		constexpr COLORREF xLabelBackground = 0x5A5AF1;
		constexpr COLORREF xLabelForeground = 0xFFFFFF;
		constexpr COLORREF yLabelBackground = 0x6FBA4E;
		constexpr COLORREF yLabelForeground = 0xFFFFFF;
		constexpr COLORREF zLabelBackground = 0xBF952D;
		constexpr COLORREF zLabelForeground = 0xFFFFFF;
		constexpr COLORREF editBackground = 0x212121;
		constexpr COLORREF editForeground = 0xE5E5E5;
	#endif
	}

	namespace content {
	#ifndef APP_DARK_THEME
		constexpr COLORREF background = 0xE1E1E1;
	#else
		constexpr COLORREF background = 0x1E1E1E;
	#endif
	}

	namespace objects {
		namespace figure1 {
			constexpr int positionX = -100;
			constexpr int positionY = 250;
			constexpr int positionZ = 0;
			constexpr int rotationX = 0;
			constexpr int rotationY = 0;
			constexpr int rotationZ = 0;
			constexpr int scaleX = 150;
			constexpr int scaleY = 150;
			constexpr int scaleZ = 150;
			constexpr COLORREF color = 0x0055FF;
		}
		namespace figure2 {
			constexpr int positionX = 40;
			constexpr int positionY = 330;
			constexpr int positionZ = 50;
			constexpr int rotationX = 0;
			constexpr int rotationY = 20;
			constexpr int rotationZ = 0;
			constexpr int scaleX = 180;
			constexpr int scaleY = 200;
			constexpr int scaleZ = 200;
			constexpr COLORREF color = 0xFF5500;
		}
		namespace light {
			constexpr int positionX = 500;
			constexpr int positionY = 1500;
			constexpr int positionZ = 1000;
		}
	}

	namespace camera {
		constexpr double yawAngle = 105.0;
		constexpr double pitchAngle = 10.0;
		constexpr double cameraX = 0;
		constexpr double cameraY = 250;
		constexpr double angleMultiplier = 200;
		constexpr double positionMultiplier = 500;
	}
}
