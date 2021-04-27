#include "FloodFill.h"

#include <cmath>
#include <queue>
#include <algorithm>
#include <optional>
#include <utility>

struct Point {
	int x;
	int y;
};

// Draws a line on screen and returns two visible ends of the line it they exist
std::optional<std::pair<Point, Point>> drawLine(int* screen, int width, int height, int x0, int y0, int x1, int y1, int color);
bool isPointInsideTriandle(const ML::mat4<int>& vertices, Point p);

static std::queue<Point> queue{};

// Draws trianle in vertices on the screen with color by 4-connected flood fill algorithm
void RNDR::algorithms::floodFill(int width, int height, int* screen, const ML::mat4<int>& vertices, int color) {
	std::fill(screen, screen + (static_cast<size_t>(width) * static_cast<size_t>(height)), color - 1);

	double x = 0, y = 0; // coordinates of the first point
	int count = 0; // count of the vertices added to x,y
	auto tmp = drawLine(screen, width, height, vertices[1][0], vertices[1][1], vertices[2][0], vertices[2][1], color);
	if (tmp.has_value()) {
		count += 2;
		x += static_cast<double>(tmp.value().first.x) + static_cast<double>(tmp.value().second.x);
		y += static_cast<double>(tmp.value().first.y) + static_cast<double>(tmp.value().second.y);
	}
	tmp = drawLine(screen, width, height, vertices[0][0], vertices[0][1], vertices[2][0], vertices[2][1], color);
	if (tmp.has_value()) {
		count++;
		x += static_cast<double>(tmp.value().first.x);
		y += static_cast<double>(tmp.value().first.y);
		if ((tmp.value().second.x != vertices[2][0]) || (tmp.value().second.y != vertices[2][1])) { // do not add point if it is original vertex cuz it has been added previously
			count++;
			x += static_cast<double>(tmp.value().second.x);
			y += static_cast<double>(tmp.value().second.y);
		}
	}

	tmp = drawLine(screen, width, height, vertices[0][0], vertices[0][1], vertices[1][0], vertices[1][1], color);
	if (tmp.has_value()) {
		if ((tmp.value().first.x != vertices[0][0]) || (tmp.value().first.y != vertices[0][1])) { // do not add point if it is original vertex cuz it has been added previously
			count++;
			x += static_cast<double>(tmp.value().first.x);
			y += static_cast<double>(tmp.value().first.y);
		}
		if ((tmp.value().second.x != vertices[1][0]) || (tmp.value().second.y != vertices[1][1])) { // do not add point if it is original vertex cuz it has been added previously
			count++;
			x += static_cast<double>(tmp.value().second.x);
			y += static_cast<double>(tmp.value().second.y);
		}
	}

	if (count == 0) return; // triangle is completely clipped

	// add corner of the screen if it is inside triangle
	if (isPointInsideTriandle(vertices, { 0, 0 })) {
		count++;
	}
	if (isPointInsideTriandle(vertices, { width - 1, 0 })) {
		count++;
		x += width - 1;
	}
	if (isPointInsideTriandle(vertices, { 0, height - 1 })) {
		count++;
		y += height - 1;
	}
	if (isPointInsideTriandle(vertices, { width - 1, height - 1 })) {
		count++;
		x += width - 1;
		y += height - 1;
	}

	x /= count;
	y /= count;

	queue.push(Point{ (int)std::round(x),(int)std::round(y) });

	while (!queue.empty()) {
		Point p = queue.front();
		queue.pop();
		if (screen[width * p.y + p.x] == (color - 1)) {
			screen[width * p.y + p.x] = color;
			if (p.y + 1 < height) queue.push({ p.x,p.y + 1 });
			if (p.y - 1 >= 0) queue.push({ p.x,p.y - 1 });
			if (p.x + 1 < width) queue.push({ p.x + 1,p.y });
			if (p.x - 1 >= 0) queue.push({ p.x - 1,p.y });
		}
	}
}

std::optional<std::pair<Point, Point>> drawLine(int* screen, int width, int height, int x0, int y0, int x1, int y1, int color) {
	int x = 0, y = 0, prevX = 0, prevY = 0;
	bool is_started = false;
	int dx = std::abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = std::abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2;
	int e2 = err;
	while (true) {
		if (y0 >= 0 && x0 >= 0 && y0 < height && x0 < width) {
			if (!is_started) {
				x += x0;
				y += y0;
				is_started = true;
			}
			prevX = x0;
			prevY = y0;
			screen[width * y0 + x0] = color;
		} else {
			if (is_started) break;
		}
		if (x0 == x1 && y0 == y1) break;
		e2 = err;
		if (e2 > -dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}
	if (!is_started) return std::nullopt;
	return std::pair<Point, Point>{ { x, y }, { prevX,prevY } };
}

inline bool isNegative(int n) {
	return n < 0;
}

bool isPointInsideTriandle(const ML::mat4<int>& vertices, Point p) {
	ML::vec4<int> P{ p.x,p.y,0,0 };
	ML::vec4<int> ABAP = ML::crossProduct(vertices[1] - vertices[0], P - vertices[0]);
	ML::vec4<int> BCBP = ML::crossProduct(vertices[2] - vertices[1], P - vertices[1]);
	ML::vec4<int> CACP = ML::crossProduct(vertices[0] - vertices[2], P - vertices[2]);
	return (isNegative(ABAP[2]) == isNegative(BCBP[2]) && isNegative(BCBP[2]) == isNegative(CACP[2]));
}
