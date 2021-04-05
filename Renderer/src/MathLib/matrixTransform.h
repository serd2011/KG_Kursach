#pragma once

#include <cmath>

#include "mat4.h"
#include "vec4.h"

namespace ML {

	template<typename T>
	mat4<T> translate(const mat4<T>&, const vec4<T>&);

	template<typename T>
	mat4<T> rotate(const mat4<T>&, const vec4<T>&);

	template<typename T>
	mat4<T> scale(const mat4<T>&, const vec4<T>&);

}

template<typename T>
ML::mat4<T> ML::translate(const mat4<T>& m, const vec4<T>& v) {
	return  m * mat4<T>({ 1,0,0,1 },
						{ 0,1,0,1 },
						{ 0,0,1,1 },
						{ v[0],v[1],v[2],1 });
}

template<typename T>
ML::mat4<T> ML::rotate(const mat4<T>& m, const vec4<T>& v) {
	double sinX = std::sin(v[0]);
	double cosX = std::cos(v[0]);
	double sinY = std::sin(v[1]);
	double cosY = std::cos(v[1]);
	double sinZ = std::sin(v[2]);
	double cosZ = std::cos(v[2]);
	return m * mat4<T>(
		{ cosZ * cosY,cosZ * sinY * sinX - sinZ * cosX,cosZ * sinY * cosX + sinZ * sinX,0 },
		{ sinZ * cosY,sinZ * sinY * sinX + cosZ * cosX,sinZ * sinY * cosX - cosZ * sinX,0 },
		{ -sinY,cosY * sinX,cosY * cosX,0 },
		{ 0,0,0,1 });
}

template<typename T>
ML::mat4<T> ML::scale(const mat4<T>& m, const vec4<T>& v) {
	return m * mat4<T>({ v[0],0,0,0 },
					   { 0,v[1],0,0 },
					   { 0,0,v[2],0 },
					   { 0,0,0,1 });
}