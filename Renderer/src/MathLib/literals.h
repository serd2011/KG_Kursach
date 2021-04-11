#pragma once

constexpr double PI = 3.14159265358979323846;

namespace RNDR {

	namespace Literals {

		inline double operator "" _deg(long double angle) {
			return angle * PI / 180;
		}

		inline double operator "" _deg(unsigned long long angle) {
			return (double)(angle * PI / 180);
		}

		inline double toRad(long double angle) {
			return angle * PI / 180;
		}

	}

}
