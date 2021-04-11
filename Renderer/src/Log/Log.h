#pragma once

namespace LOG {
	void init();
	void hide();
	void show();

	void info(char*, ...);
	void error(char*, ...);
	void success(char*, ...);
}

#define LOG_INIT() ::LOG::init()
#define LOG_HIDE() ::LOG::hide()
#define LOG_SHOW() ::LOG::show()

#define LOG_INFO(...)		::LOG::info(__VA_ARGS__)
#define LOG_SUCCESS(...)	::LOG::success(__VA_ARGS__)
#define LOG_ERROR(...)		::LOG::error(__VA_ARGS__)
