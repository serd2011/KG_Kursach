#pragma once
#include "pch.h"

namespace config {

	constexpr auto IDS_APP_TITLE = L"Курсач";

	constexpr LONG mainMinWidth = 1200;
	constexpr LONG mainMinHeight = 600;
	constexpr LONG asideWidth = 400;

	namespace aside {
		constexpr int elementHeight = 25;

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

	namespace transforms {
		namespace figure1 {
			constexpr int positionX = 500;
			constexpr int positionY = 500;
			constexpr int positionZ = 0;
			constexpr int rotationX = 100;
			constexpr int rotationY = 0;
			constexpr int rotationZ = 70;
			constexpr int scaleX = 200;
			constexpr int scaleY = 200;
			constexpr int scaleZ = 200;
		}
		namespace figure2 {
			constexpr int positionX = 550;
			constexpr int positionY = 470;
			constexpr int positionZ = 60;
			constexpr int rotationX = -40;
			constexpr int rotationY = 170;
			constexpr int rotationZ = 0;
			constexpr int scaleX = 280;
			constexpr int scaleY = 210;
			constexpr int scaleZ = 210;
		}
		namespace light {
			constexpr int positionX = 1000;
			constexpr int positionY = 500;
			constexpr int positionZ = 300;
		}
	}
}
