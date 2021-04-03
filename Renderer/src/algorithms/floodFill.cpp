#include "FloodFill.h"

#include <cmath>
#include <queue>
#include <algorithm>

void drawLine(int* screen, int width, int x0, int y0, int x1, int y1, short color) {
	int dx = std::abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = std::abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2;
	int e2 = err;
	while (true) {
		if (y0 >= 0 && x0 >= 0) screen[width * y0 + x0] = color;
		if (x0 == x1 && y0 == y1) break;
		e2 = err;
		if (e2 > -dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}
}

struct Point {
	int x;
	int y;
};

std::queue<Point> queue{};

void RNDR::algorithms::floodFill(int width, int height, int* screen, const ML::mat4<int>& vertices, int color) {
	std::fill(screen, screen + (static_cast<size_t>(width) * static_cast<size_t>(height)), color - 1);

	drawLine(screen, width, vertices[0][0], vertices[0][1], vertices[1][0], vertices[1][1], color + 1);
	drawLine(screen, width, vertices[0][0], vertices[0][1], vertices[2][0], vertices[2][1], color + 1);
	drawLine(screen, width, vertices[1][0], vertices[1][1], vertices[2][0], vertices[2][1], color + 1);

	queue.push({ std::max((vertices[0][0] + vertices[1][0] + vertices[2][0]) / 3,0), std::max((vertices[0][1] + vertices[1][1] + vertices[2][1]) / 3,0) });
	//queue.push({10,10});
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

	drawLine(screen, width, vertices[0][0], vertices[0][1], vertices[1][0], vertices[1][1], color);
	drawLine(screen, width, vertices[0][0], vertices[0][1], vertices[2][0], vertices[2][1], color);
	drawLine(screen, width, vertices[1][0], vertices[1][1], vertices[2][0], vertices[2][1], color);
}
