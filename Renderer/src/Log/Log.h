#pragma once

namespace LOG {
	void init();
	void hide();
	void show();

	void info(char*, ...);
}

#define LOG_INIT() ::LOG::init()
#define LOG_HIDE() ::LOG::hide()
#define LOG_SHOW() ::LOG::show()

#define LOG_INFO(...)	::LOG::info(__VA_ARGS__)
