#pragma once

#include <cmath>

#include "mat4.h"
#include "vec4.h"

namespace ML {

	template<typename T>
	mat4<T> translate(const mat4<T>&, const vec4<T>&);

	template<typename T>
	vec4<T> translate(const vec4<T>& m, const vec4<T>& v);

	template<typename T>
	mat4<T> rotate(const mat4<T>&, const vec4<T>&);

	template<typename T>
	vec4<T> rotate(const vec4<T>&, const vec4<T>&);

	template<typename T>
	mat4<T> scale(const mat4<T>&, const vec4<T>&);

	template<typename T>
	mat4<T> getShadow(const mat4<T>&, const vec4<T>& light);

}

template<typename T>
ML::mat4<T> ML::translate(const mat4<T>& m, const vec4<T>& v) {
	return  m * mat4<T>({ 1,0,0,1 },
						{ 0,1,0,1 },
						{ 0,0,1,1 },
						{ v[0],v[1],v[2],1 });
}


template<typename T>
ML::vec4<T> ML::translate(const vec4<T>& m, const vec4<T>& v) {
	return  ML::vec4(m[0] + v[0], m[1] + v[1], m[2] + v[2]);
}

template<typename T>
inline ML::mat4<T> getRotationMatrix(const ML::vec4<T>& v) {
	double sinX = std::sin(v[0]);
	double cosX = std::cos(v[0]);
	double sinY = std::sin(v[1]);
	double cosY = std::cos(v[1]);
	double sinZ = std::sin(v[2]);
	double cosZ = std::cos(v[2]);
	return ML::mat4<T>(
		{ cosZ * cosY,cosZ * sinY * sinX - sinZ * cosX,cosZ * sinY * cosX + sinZ * sinX,0 },
		{ sinZ * cosY,sinZ * sinY * sinX + cosZ * cosX,sinZ * sinY * cosX - cosZ * sinX,0 },
		{ -sinY,cosY * sinX,cosY * cosX,0 },
		{ 0,0,0,1 });
}

template<typename T>
ML::mat4<T> ML::rotate(const mat4<T>& m, const vec4<T>& v) {
	return m * getRotationMatrix(v);
}

template<typename T>
ML::vec4<T> ML::rotate(const vec4<T>& m, const vec4<T>& v) {
	return m * getRotationMatrix(v);
}

template<typename T>
ML::mat4<T> ML::scale(const mat4<T>& m, const vec4<T>& v) {
	return m * mat4<T>({ v[0],0,0,0 },
					   { 0,v[1],0,0 },
					   { 0,0,v[2],0 },
					   { 0,0,0,1 });
}

template<typename T>
ML::mat4<T> ML::getShadow(const mat4<T>& m, const vec4<T>& light) {
	ML::vec4<double> p0 = m[0] + ((m[0] - light) * ((-m[0][1]) / (m[0][1] - light[1])));
	ML::vec4<double> p1 = m[1] + ((m[1] - light) * ((-m[1][1]) / (m[1][1] - light[1])));
	ML::vec4<double> p2 = m[2] + ((m[2] - light) * ((-m[2][1]) / (m[2][1] - light[1])));
	return ML::mat4<double>(p0, p1, p2);
}
