#pragma once

namespace ML {

	template<typename T>
	class vec4;

	template<typename T>
	vec4<T> operator*(const vec4<T>&, const vec4<T>&);

	template<typename T>
	vec4<T> operator+(const vec4<T>&, const vec4<T>&);

	template<typename T>
	vec4<T> operator-(const vec4<T>&, const vec4<T>&);

	template<typename T>
	double getAngle(const vec4<T>&, const vec4<T>&);

	template<typename T>
	class vec4 {
	public:
		vec4(
			T p0 = T(),
			T p1 = T(),
			T p2 = T(),
			T p3 = 1
		);
		~vec4() {};

		vec4(const vec4&);
		template<typename U>
		vec4(const vec4<U>&);

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

		friend vec4<T> operator*(const vec4<T>&, const vec4<T>&);

		friend vec4<T> operator+(const vec4<T>&, const vec4<T>&);

		friend vec4<T> operator-(const vec4<T>&, const vec4<T>&);

		friend double getAngle(const vec4<T>&, const vec4<T>&);
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
inline ML::vec4<T>::vec4(const vec4<U>& in) {
	data[0] = static_cast<U>(in.data[0]);
	data[1] = static_cast<U>(in.data[1]);
	data[2] = static_cast<U>(in.data[2]);
	data[3] = static_cast<U>(in.data[3]);
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
	data[0] = static_cast<U>(in.data[0]);
	data[1] = static_cast<U>(in.data[1]);
	data[2] = static_cast<U>(in.data[2]);
	data[3] = static_cast<U>(in.data[3]);
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
ML::vec4<T> ML::operator*(const vec4<T>& first, const vec4<T>& second) {
	return vec4<T>(
		(first.data[2] * second.data[3] - first.data[3] * second.data[2]),
		(first.data[3] * second.data[1] - first.data[1] * second.data[3]),
		(first.data[1] * second.data[2] - first.data[2] * second.data[1])
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
