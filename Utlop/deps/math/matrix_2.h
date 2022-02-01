#ifndef __MATRIX2_H__
#define __MATRIX2_H__ 1

#include "vector_2.h"

class Matrix2x2 {
public:

	Matrix2x2();
	Matrix2x2(float a[4]);
	Matrix2x2(float value);
	Matrix2x2(const Vector2& a, const Vector2& b); 
	Matrix2x2(const Matrix2x2& copy);
	~Matrix2x2();
	Matrix2x2 Identity() const;
	Matrix2x2 Multiply(const Matrix2x2& other) const;
	float Determinant() const;
	Matrix2x2 Adjoint() const;
	Vector2 GetLine(int line) const;
	Vector2 GetColum(int line) const;

	Matrix2x2 Inverse() const;
	Matrix2x2 Transpose() const;

	inline Matrix2x2 operator+(const Matrix2x2& other) const;
	inline void operator+=(const Matrix2x2& other);
	inline Matrix2x2 operator+(float value) const;
	inline void operator+=(float value);
	inline Matrix2x2 operator-(const Matrix2x2& other) const;
	inline void operator-=(const Matrix2x2& other);
	inline Matrix2x2 operator-(float value) const;
	inline void operator-=(float value);

	inline Matrix2x2 operator*(float value) const;
	inline void operator*=(float value);
	inline Matrix2x2 operator/(float value) const;
	inline void operator/=(float value);

	bool operator==(const Matrix2x2& other) const;
	bool operator!=(const Matrix2x2& other) const;
	inline void operator=(const Matrix2x2& other);

	float m[4];
};


inline Matrix2x2::Matrix2x2() {
}

inline Matrix2x2::Matrix2x2(float a[4]) {
	for (int i = 0; i < 4; i++) {
		m[i] = a[i];
	}
}

inline Matrix2x2::Matrix2x2(float a) {
	for (int i = 0; i < 4; i++) {
		m[i] = a;
	}
}

inline Matrix2x2::Matrix2x2(const Vector2& a, const Vector2& b) {
	m[0] = a.x;
	m[1] = a.y;
	m[2] = b.x;
	m[3] = b.y;
}

inline Matrix2x2::Matrix2x2(const Matrix2x2& copy) {
	for (int i = 0; i < 4; i++) {
		m[i] = copy.m[i];
	}
}

inline Matrix2x2::~Matrix2x2() {

}

inline Matrix2x2 Matrix2x2::operator+(const Matrix2x2& other) const {
	
	Matrix2x2 addingMatrix;

	for (int i = 0; i < 4; i++) {
		addingMatrix.m[i] = m[i] + other.m[i];
	}

	return addingMatrix;
}

inline void Matrix2x2::operator+=(const Matrix2x2& other) {
	for (int i = 0; i < 4; i++) {
		m[i] += other.m[i];
	}
}

inline Matrix2x2 Matrix2x2::operator+(float value) const {
	
	Matrix2x2 addingMatrix;
	
	for (int i = 0; i < 4; i++) {
		addingMatrix.m[i] = m[i] + value;
	}

	return addingMatrix;
}

inline void Matrix2x2::operator+=(float value) {
	for (int i = 0; i < 4; i++) {
		m[i] += value;
	}
}

inline Matrix2x2 Matrix2x2::operator-(const Matrix2x2& other) const {
	Matrix2x2 subMatrix;

	for (int i = 0; i < 4; i++) {
		subMatrix.m[i] = m[i] - other.m[i];
	}

	return subMatrix;
}

inline void Matrix2x2::operator-=(const Matrix2x2& other) {
	for (int i = 0; i < 4; i++) {
		m[i] -= other.m[i];
	}
}

inline Matrix2x2 Matrix2x2::operator-(float value) const {	
	Matrix2x2 subMatrix;

	for (int i = 0; i < 4; i++) {
		subMatrix.m[i] = m[i] - value;
	}

	return subMatrix;
}

inline void Matrix2x2::operator-=(float value) {
	for (int i = 0; i < 4; i++) {
		m[i] -= value;
	}
}


inline Matrix2x2 Matrix2x2::operator*(float value) const {
	
	Matrix2x2 mulMatrix;
	
	for (int i = 0; i < 4; i++) {
		mulMatrix.m[i] = m[i] * value;
	}

	return mulMatrix;
}

inline void Matrix2x2::operator*=(float value) {
	for (int i = 0; i < 4; i++) {
		m[i] *= value;
	}
}

inline Matrix2x2 Matrix2x2::operator/(float value) const {
	Matrix2x2 divMatrix;

	for (int i = 0; i < 4; i++) {
		divMatrix.m[i] = m[i] / value;
	}

	return divMatrix;
}

inline void Matrix2x2::operator/=(float value) {
	for (int i = 0; i < 4; i++) {
		m[i] /= value;
	}
}


inline bool Matrix2x2::operator==(const Matrix2x2& other) const {
	for (int i = 0; i < 4; i++) {
		if (m[i] != other.m[i]) {
			return false;
		}
	}
	return true;
}

inline bool Matrix2x2::operator!=(const Matrix2x2& other) const {
	for (int i = 0; i < 4; i++) {
		if (m[i] != other.m[i]) {
			return true;
		}
	}
	return false;
}

inline void Matrix2x2::operator=(const Matrix2x2& other) {
	for (int i = 0; i < 4; i++) {
		m[i] = other.m[i];
	}
}

inline Matrix2x2 Matrix2x2::Identity() const {

	float identity[4];
	identity[0] = 1;
	identity[1] = 0;
	identity[2] = 0;
	identity[3] = 1;

	Matrix2x2 mI(identity);

	return mI;
}

inline float Matrix2x2::Determinant() const {

	float det = m[0] * m[3] - m[1] * m[2];

	return det;
}

inline Matrix2x2 Matrix2x2::Inverse() const {

	if (Determinant() != 0) {
		return Matrix2x2();
	}
	float inv[4];
	inv[0] = m[3];
	inv[1] = m[2];
	inv[2] = m[1];
	inv[3] = m[0];

	Matrix2x2 mI(inv);

	return mI;
		
}

inline Matrix2x2 Matrix2x2::Multiply(const Matrix2x2& other) const {

	Matrix2x2 mM;

	mM.m[0] = m[0] * other.m[0] + m[1] * other.m[2];
	mM.m[1] = m[0] * other.m[1] + m[1] * other.m[3];
	mM.m[2] = m[2] * other.m[0] + m[3] * other.m[2];
	mM.m[3] = m[0] * other.m[1] + m[3] * other.m[3];

	return mM;
}

inline Matrix2x2 Matrix2x2::Adjoint() const {

	float aM[4];
	aM[0] = m[3];
	aM[1] = -m[1];
	aM[2] = -m[2];
	aM[3] = m[0];

	Matrix2x2 adjM(aM);

	return adjM;
}

inline Matrix2x2 Matrix2x2::Transpose() const {

	float tM[4];
	tM[0] = m[3];
	tM[1] = m[2];
	tM[2] = m[1];
	tM[3] = m[0];

	Matrix2x2 traM(tM);

	return traM;

}

inline Vector2 Matrix2x2::GetLine(int line) const {

	float v1 = m[line*2];
	float v2 = m[line * 2 + 1];
	Vector2 vec(v1, v2);

	return vec;
}

inline Vector2 Matrix2x2::GetColum(int line) const {	
	float v1 = m[line];
	float v2 = m[line + 2];
	Vector2 vec(v1, v2);

	return vec;
}


#endif
