#ifndef __MATRIX3_H__
#define __MATRIX3_H__ 1

#include "vector_2.h"
#include "vector_3.h"

class Matrix3x3 {
public:

	Matrix3x3();
	Matrix3x3(float *values_array);
	Matrix3x3(float value);
	Matrix3x3(Vector3 a, Vector3 b, Vector3 c);

	Matrix3x3(const Matrix3x3& copy);
	~Matrix3x3();

	static Matrix3x3 Identity();

	Matrix3x3 Multiply(const Matrix3x3& other) const;

	float Determinant() const;

	Matrix3x3 Adjoint() const;
	bool GetInverse(Matrix3x3& out) const;
	bool Inverse();

	Matrix3x3 Transpose() const;

	static Matrix3x3 Translate(const Vector2& position);
	static Matrix3x3 Translate(float x, float y);

	Vector3 GetColum(int colum) const;
	Vector3 GetLine(int line) const;

	inline Matrix3x3 operator+(const Matrix3x3& other) const;
	inline Matrix3x3& operator+=(const Matrix3x3& other);
	inline Matrix3x3 operator+(float value) const;
	inline Matrix3x3& operator+=(float value);
	inline Matrix3x3 operator-(const Matrix3x3& other) const;
	inline Matrix3x3& operator-=(const Matrix3x3& other);
	inline Matrix3x3 operator-(float value) const;
	inline Matrix3x3& operator-=(float value);
	inline Matrix3x3 operator*(float value) const;
	inline Matrix3x3& operator*=(float value);
	inline Matrix3x3 operator/(float value) const;
	inline Matrix3x3& operator/=(float value);
	bool operator==(const Matrix3x3& other) const;
	bool operator!=(const Matrix3x3& other) const;
	inline void operator=(const Matrix3x3& other);

	float m[9];
};


inline Matrix3x3::Matrix3x3() {
}

inline Matrix3x3::Matrix3x3(float value) {
	for (int i = 0; i < 9; i++) {
		m[i] = value;
	}
}

inline Matrix3x3::Matrix3x3(float *values_array) {
	for (int i = 0; i < 9; i++) {
		m[i] = values_array[i];
	}	
}

inline Matrix3x3::Matrix3x3(Vector3 a, Vector3 b, Vector3 c) {
	m[0] = a.x;
	m[1] = a.y;
	m[2] = a.z;
	m[3] = b.x;
	m[4] = b.y;
	m[5] = b.z;
	m[6] = c.x;
	m[7] = c.y;
	m[8] = c.z;
}

inline Matrix3x3::Matrix3x3(const Matrix3x3& copy) {
	for (int i = 0; i < 9; i++) {
		m[i] = copy.m[i];
	}
}

inline Matrix3x3::~Matrix3x3() {
}

inline Matrix3x3 Matrix3x3::operator+(const Matrix3x3& other) const {
	Matrix3x3 sumMatrix;
	for (int i = 0; i < 9; i++) {
		sumMatrix.m[i] = m[i] + other.m[i];
	}
	return sumMatrix;
}

inline Matrix3x3& Matrix3x3::operator+=(const Matrix3x3& other) {
	for (int i = 0; i < 9; i++) {
		m[i] += other.m[i];
	}
	return *this;
}

inline Matrix3x3 Matrix3x3::operator+(float value) const {
	Matrix3x3 sumMatrix;
	for (int i = 0; i < 9; i++) {
		sumMatrix.m[i] = m[i] + value;
	}
	return sumMatrix;
}

inline Matrix3x3& Matrix3x3::operator+=(float value) {
	for (int i = 0; i < 9; i++) {
		m[i] += value;
	}
	return *this;
}

inline Matrix3x3 Matrix3x3::operator-(const Matrix3x3& other) const {
	Matrix3x3 subMatrix;
	for (int i = 0; i < 9; i++) {
		subMatrix.m[i] = m[i] - other.m[i];
	}
	return subMatrix;
}

inline Matrix3x3& Matrix3x3::operator-=(const Matrix3x3& other) {
	for (int i = 0; i < 9; i++) {
		m[i] -= other.m[i];
	}
	return *this;
}

inline Matrix3x3 Matrix3x3::operator-(float value) const {
	Matrix3x3 subMatrix;
	for (int i = 0; i < 9; i++) {
		subMatrix.m[i] = m[i] - value;
	}
	return subMatrix;
}

inline Matrix3x3& Matrix3x3::operator-=(float value) {
	for (int i = 0; i < 9; i++) {
		m[i] -= value;
	}
	return *this;
}

inline Matrix3x3 Matrix3x3::operator*(float value) const {
	Matrix3x3 mulMatrix;
	for (int i = 0; i < 9; i++) {
		mulMatrix.m[i] = m[i] * value;
	}
	return mulMatrix;
}

inline Matrix3x3& Matrix3x3::operator*=(float value) {
	for (int i = 0; i < 9; i++) {
		m[i] *= value;
	}
	return *this;
}

inline Matrix3x3 Matrix3x3::operator/(float value) const {
	Matrix3x3 divMatrix;
	for (int i = 0; i < 9; i++) {
		divMatrix.m[i] = m[i] / value;
	}
	return divMatrix;
}

inline Matrix3x3& Matrix3x3::operator/=(float value) {
	for (int i = 0; i < 9; i++) {
		m[i] /= value;
	}
	return *this;
}

inline bool Matrix3x3::operator==(const Matrix3x3& other) const {
	for (int i = 0; i < 9; i++) {
		if (m[i] != other.m[i]) {
			return false;
		}
	}
	return true;
}

inline bool Matrix3x3::operator!=(const Matrix3x3& other) const {
	
	for (int i = 0; i < 9; i++) {
		if (m[i] != other.m[i]) {
			return true;
		}
	}
	return false;
}

inline void Matrix3x3::operator=(const Matrix3x3& other) {
	for (int i = 0; i < 9; i++) {
		m[i] = other.m[i];
	}
}

inline Matrix3x3 Matrix3x3::Identity(){

	float identity[] = {	1.0f , 0.0f , 0.0f, 
												0.0f , 1.0f , 0.0f,
												0.0f , 0.0f , 1.0f};
	Matrix3x3 mI(identity);


	return mI;
}

inline float Matrix3x3::Determinant() const {

	return m[0] * (m[4] * m[8] - m[5] * m[7]) - m[1] * (m[3] * m[8] - m[5] * m[6]) +
		m[2] * (m[3] * m[7] - m[4] * m[6]);
}

inline bool Matrix3x3::GetInverse(Matrix3x3& out) const {

	float det = Determinant();
	if (det == 0) {
		return false;
	}

	out = Adjoint().Transpose() / det;

	return true;
}

inline bool Matrix3x3::Inverse() {

	return GetInverse(*this);

}

inline Matrix3x3 Matrix3x3::Translate(const Vector2& mov_vector) {

	return Translate(mov_vector.x, mov_vector.y);
}

inline Matrix3x3 Matrix3x3::Translate(float x, float y) {
	
	Matrix3x3 transMatrix;
	transMatrix.m[0] = 1;
	transMatrix.m[1] = 0;
	transMatrix.m[2] = x;
	transMatrix.m[3] = 0;
	transMatrix.m[4] = 1;
	transMatrix.m[5] = y;
	transMatrix.m[6] = 0;
	transMatrix.m[7] = 0;
	transMatrix.m[8] = 1;


	return transMatrix;
}

inline Matrix3x3 Matrix3x3::Multiply(const Matrix3x3& other) const {
	
	Matrix3x3 productM;

	productM.m[0] = m[0] * other.m[0] + m[1] * other.m[3] + m[2] * other.m[6];
	productM.m[1] = m[0] * other.m[1] + m[1] * other.m[4] + m[2] * other.m[7];
	productM.m[2] = m[0] * other.m[2] + m[1] * other.m[5] + m[2] * other.m[8];

	productM.m[3] = m[3] * other.m[0] + m[4] * other.m[3] + m[5] * other.m[6];
	productM.m[4] = m[3] * other.m[1] + m[4] * other.m[4] + m[5] * other.m[7];
	productM.m[5] = m[3] * other.m[2] + m[4] * other.m[5] + m[5] * other.m[8];

	productM.m[6] = m[6] * other.m[0] + m[7] * other.m[3] + m[8] * other.m[6];
	productM.m[7] = m[6] * other.m[1] + m[7] * other.m[4] + m[8] * other.m[7];
	productM.m[8] = m[6] * other.m[2] + m[7] * other.m[5] + m[8] * other.m[8];

	return productM;
}

inline Matrix3x3 Matrix3x3::Adjoint() const {

	
	Matrix3x3 adjM;



	adjM.m[0] = m[4] * m[8] - m[5] * m[7];
	adjM.m[1] = -1 * (m[3] * m[8] - m[5] * m[6]);
	adjM.m[2] = m[3] * m[7] - m[4] * m[6];
	adjM.m[3] = -1 * (m[1] * m[8] - m[2] * m[7]);
	adjM.m[4] = m[0] * m[8] - m[2] * m[6];
	adjM.m[5] = -1 * (m[0] * m[7] - m[1] * m[6]);
	adjM.m[6] = m[1] * m[5] - m[2] * m[4];
	adjM.m[7] = -1 * (m[0] * m[5] - m[2] * m[3]);
	adjM.m[8] = m[0] * m[4] - m[1] * m[3];

	return adjM;
}

inline Matrix3x3 Matrix3x3::Transpose() const {

	Matrix3x3 transposeM;

	transposeM.m[0] = m[0];
	transposeM.m[1] = m[3];
	transposeM.m[2] = m[6];
	transposeM.m[3] = m[1];
	transposeM.m[4] = m[4];
	transposeM.m[5] = m[7];
	transposeM.m[6] = m[2];
	transposeM.m[7] = m[5];
	transposeM.m[8] = m[8];


	return transposeM;
}

inline Vector3 Matrix3x3::GetColum(int colum) const {

	return Vector3(m[colum], m[colum + 3], m[colum + 6]);
}


inline Vector3 Matrix3x3::GetLine(int line) const {


	return Vector3 (m[line * 3], m[line * 3 + 1], m[line * 3 + 2]);

}

#endif 