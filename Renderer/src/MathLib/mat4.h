#pragma once
#include "vec4.h"

namespace ML {

	template<typename T>
	class mat4 {
	public:
		mat4(
			const vec4<T>& v0 = vec4<T>(),
			const vec4<T>& v1 = vec4<T>(),
			const vec4<T>& v2 = vec4<T>(),
			const vec4<T>& v3 = vec4<T>()
		);
		mat4(T fill);
		~mat4() {};

		mat4(const mat4&);
		template<typename U>
		mat4(const mat4<U>&);

		mat4& operator=(const mat4&);
		template<typename U>
		mat4<T>& operator=(const mat4<U>&);

		mat4 operator*(const mat4&) const;
		mat4& operator*=(const mat4&) const;

		vec4<T>& operator[](size_t);
		const vec4<T>& operator[](size_t) const;

		template<typename U>
		explicit operator mat4<U>() const;

		template<typename U>
		friend class mat4;

	private:
		vec4<T> data[4];
	};

	template<typename T>
	vec4<T> operator*(const vec4<T>&, const mat4<T>&);

}

template<typename T>
inline ML::mat4<T>::mat4(const ML::vec4<T>& v0, const ML::vec4<T>& v1, const ML::vec4<T>& v2, const ML::vec4<T>& v3) : data{ v0,v1,v2,v3 } {}

template<typename T>
inline ML::mat4<T>::mat4(T s) : mat4(
	{ s,0,0,0 },
	{ 0,s,0,0 },
	{ 0,0,s,0 },
	{ 0,0,0,s }
) {}

template<typename T>
inline ML::mat4<T>::mat4(const ML::mat4<T>& in) : data{ in.data[0] ,in.data[1],in.data[2] ,in.data[3] } {}

template<typename T>
template<typename U>
inline ML::mat4<T>::mat4(const ML::mat4<U>& in) : data{
	static_cast<ML::vec4<T>>(in.data[0]),
	static_cast<ML::vec4<T>>(in.data[1]),
	static_cast<ML::vec4<T>>(in.data[2]),
	static_cast<ML::vec4<T>>(in.data[3])
} {}

template<typename T>
inline ML::mat4<T>& ML::mat4<T>::operator=(const ML::mat4<T>& in) {
	data[0] = in.data[0];
	data[1] = in.data[1];
	data[2] = in.data[2];
	data[3] = in.data[3];
	return *this;
}

template<typename T>
template<typename U>
inline ML::mat4<T>& ML::mat4<T>::operator=(const ML::mat4<U>& in) {
	data[0] = static_cast<ML::vec4<T>>(in.data[0]);
	data[1] = static_cast<ML::vec4<T>>(in.data[1]);
	data[2] = static_cast<ML::vec4<T>>(in.data[2]);
	data[3] = static_cast<ML::vec4<T>>(in.data[3]);
	return *this;
}

template<typename T>
inline ML::mat4<T> ML::mat4<T>::operator*(const ML::mat4<T>& in) const{
	mat4<T> result{};
	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			result[i][j] = this->data[i][0] * in.data[0][j] +
				this->data[i][1] * in.data[1][j] +
				this->data[i][2] * in.data[2][j] +
				this->data[i][3] * in.data[3][j];
		}
	}
	return result;
}

template<typename T>
inline ML::mat4<T>& ML::mat4<T>::operator*=(const ML::mat4<T>& in) const{
	mat4<T> result{};
	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			result[i][j] = this->data[i][0] * in->data[0][j] +
				this->data[i][1] * in->data[1][j] +
				this->data[i][2] * in->data[2][j] +
				this->data[i][3] * in->data[3][j];
		}
	}
	data[0] = result.data[0];
	data[1] = result.data[1];
	data[2] = result.data[2];
	data[3] = result.data[3];
	return *this;
}

template<typename T>
inline ML::vec4<T>& ML::mat4<T>::operator[](size_t index) {
	return data[index];
}

template<typename T>
inline const ML::vec4<T>& ML::mat4<T>::operator[](size_t index) const {
	return data[index];
}


template<typename T>
ML::vec4<T> ML::operator*(const ML::vec4<T>& v, const ML::mat4<T>& m) {
	vec4<T> result{};
	for (size_t j = 0; j < 4; j++) {
		result[j] = v->data[0] * m->data[0][j] +
			v->data[1] * m->data[1][j] +
			v->data[2] * m->data[2][j] +
			v->data[3] * m->data[3][j];
	}
	return result;
}
