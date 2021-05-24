#pragma once

#include <cmath>

namespace ML {

	template<typename T>
	class vec4;

	template<typename T>
	vec4<T> operator+(const vec4<T>&, const vec4<T>&);

	template<typename T>
	vec4<T> operator-(const vec4<T>&, const vec4<T>&);

	template<typename T>
	double getAngle(const vec4<T>&, const vec4<T>&);

	// Векторное произведение
	template<typename T>
	vec4<T> crossProduct(const vec4<T>&, const vec4<T>&);

	// Скалярное произведение
	template<typename T>
	T dotProduct(const vec4<T>&, const vec4<T>&);

	// Смешанное произведение
	template<typename T>
	T tripleProduct(const vec4<T>&, const vec4<T>&, const vec4<T>&);

	template<typename T>
	vec4<T> normalize(const vec4<T>&);

	template<typename T>
	class vec4 {
	public:
		vec4(
			T p0 = T(),
			T p1 = T(),
			T p2 = T(),
			T p3 = 1
		);
		template<typename U>
		vec4(
			U p0 = U(),
			U p1 = U(),
			U p2 = U(),
			U p3 = 1
		);
		~vec4() {};

		vec4(const vec4&);
		template<typename U>
		vec4(const vec4<U>&);

		vec4& operator*(T);

		vec4& operator=(const vec4&);
		template<typename U>
		vec4<T>& operator=(const vec4<U>&);

		T& operator[](size_t);
		const T& operator[](size_t) const;

		template<typename U>
		explicit operator vec4<U>() const;

		template<typename U>
		friend class vec4;

	private:
		T data[4];

		friend vec4<T> operator+<T>(const vec4<T>&, const vec4<T>&);

		friend vec4<T> operator-<T>(const vec4<T>&, const vec4<T>&);

		friend double getAngle<T>(const vec4<T>&, const vec4<T>&);

		friend vec4<T> crossProduct<T>(const vec4<T>&, const vec4<T>&);

		friend T dotProduct<T>(const vec4<T>&, const vec4<T>&);

		friend T tripleProduct<T>(const vec4<T>&, const vec4<T>&, const vec4<T>&);
	};

}

template<typename T>
inline ML::vec4<T>::vec4(T p0, T p1, T p2, T p3) {
	data[0] = p0;
	data[1] = p1;
	data[2] = p2;
	data[3] = p3;
}

template<typename T>
inline ML::vec4<T>::vec4(const vec4<T>& in) {
	data[0] = in.data[0];
	data[1] = in.data[1];
	data[2] = in.data[2];
	data[3] = in.data[3];
}

template<typename T>
template<typename U>
inline ML::vec4<T>::vec4(U p0, U p1, U p2, U p3) {
	data[0] = static_cast<T>(p0);
	data[1] = static_cast<T>(p1);
	data[2] = static_cast<T>(p2);
	data[3] = static_cast<T>(p3);
}

template<typename T>
template<typename U>
inline ML::vec4<T>::vec4(const vec4<U>& in) {
	data[0] = static_cast<T>(in.data[0]);
	data[1] = static_cast<T>(in.data[1]);
	data[2] = static_cast<T>(in.data[2]);
	data[3] = static_cast<T>(in.data[3]);
}

template<typename T>
inline ML::vec4<T>& ML::vec4<T>::operator*(T a) {
	data[0] *= a;
	data[1] *= a;
	data[2] *= a;
	data[3] *= a;
	return *this;
}

template<typename T>
inline ML::vec4<T>& ML::vec4<T>::operator=(const vec4<T>& in) {
	data[0] = in.data[0];
	data[1] = in.data[1];
	data[2] = in.data[2];
	data[3] = in.data[3];
	return *this;
}

template<typename T>
template<typename U>
inline ML::vec4<T>& ML::vec4<T>::operator=(const vec4<U>& in) {
	data[0] = static_cast<T>(in.data[0]);
	data[1] = static_cast<T>(in.data[1]);
	data[2] = static_cast<T>(in.data[2]);
	data[3] = static_cast<T>(in.data[3]);
	return *this;
}

template<typename T>
inline T& ML::vec4<T>::operator[](size_t index) {
	return data[index];
}

template<typename T>
inline const T& ML::vec4<T>::operator[](size_t index) const {
	return data[index];
}

template<typename T>
template<typename U>
inline ML::vec4<T>::operator vec4<U>() const {
	return vec4<U>(
		static_cast<U>(this->data[0]),
		static_cast<U>(this->data[1]),
		static_cast<U>(this->data[1]),
		static_cast<U>(this->data[2]),
		);

}


template<typename T>
ML::vec4<T> ML::operator+(const vec4<T>& first, const vec4<T>& second) {
	return vec4<T>(
		first.data[0] + second.data[0],
		first.data[1] + second.data[1],
		first.data[2] + second.data[2],
		first.data[3] + second.data[3]
		);
}

template<typename T>
ML::vec4<T> ML::operator-(const vec4<T>& first, const vec4<T>& second) {
	return vec4<T>(
		first.data[0] - second.data[0],
		first.data[1] - second.data[1],
		first.data[2] - second.data[2],
		first.data[3] - second.data[3]
		);
}

template<typename T>
double ML::getAngle(const vec4<T>& first, const vec4<T>& second) {
	return (first.data[0] * second.data[0] + first.data[1] * second.data[1] + first.data[2] * second.data[2] + first.data[3] * second.data[3]) / (
		std::sqrt(std::pow(first.data[0], 2) + std::pow(first.data[1], 2) + std::pow(first.data[2], 2) + std::pow(first.data[3], 2)) *
		std::sqrt(std::pow(second.data[0], 2) + std::pow(second.data[1], 2) + std::pow(second.data[2], 2) + std::pow(second.data[3], 2))
		);
}

template<typename T>
ML::vec4<T> ML::crossProduct(const vec4<T>& first, const vec4<T>& second) {
	return vec4<T>(
		(first.data[1] * second.data[2] - first.data[2] * second.data[1]),
		(first.data[2] * second.data[0] - first.data[0] * second.data[2]),
		(first.data[0] * second.data[1] - first.data[1] * second.data[0])
		);
}

template<typename T>
T ML::dotProduct(const vec4<T>& first, const vec4<T>& second) {
	return first[0] * second[0] + first[1] * second[1] + first[2] * second[2];
}

template<typename T>
T ML::tripleProduct(const vec4<T>& first, const vec4<T>& second, const vec4<T>& third) {
	return dotProduct(first, crossProduct(second, third));
}

template<typename T>
ML::vec4<T> ML::normalize(const vec4<T>& v) {
	T length = std::sqrt(std::pow(v[0], 2) + std::pow(v[1], 2) + std::pow(v[2], 2));
	if (length == 0) return v;
	return vec4<T>(v[0] / length, v[1] / length, v[2] / length);
}
