#ifndef __MATRIX_4_H__
#define __MATRIX_4_H__ 1

#include "vector_3.h"
#include "vector_4.h"
#include "matrix_3.h"

class Matix4x4{
 public:

  Matix4x4();
  Matix4x4(const float a[16]);
  Matix4x4(float value);
  Matix4x4(const Matix4x4& copy);
  ~Matix4x4();

  Matix4x4 Identity() const;
  Matix4x4 Multiply(const Matix4x4& other) const;

  float Determinant() const;
  Matix4x4 Adjoint() const;
  bool GetInverse(Matix4x4* out) const;
  bool Inverse();

  Matix4x4 Transpose() const;


  static Matix4x4 Translate(const Vector3& distance);
  static Matix4x4 Translate(float x, float y, float z);

  static Matix4x4 Scale(const Vector3& scale);
  static Matix4x4 Scale(float x, float y, float z);

  static Matix4x4 RotateX(float radians);
  static Matix4x4 RotateY(float radians);
  static Matix4x4 RotateZ(float radians);

  static Matix4x4 GetTransform(const Vector3& translate, const Vector3& scale,
                      float rotateX, float rotateY, float rotateZ);

  static Matix4x4 GetTransform(float trans_x, float trans_y, float trans_z,
                      float scale_x, float scale_y, float scale_Z,
                      float rotateX, float rotateY, float rotateZ);

  Matix4x4 PerspectiveMatrix(float fov, float aspect,
	  float near, float far) const;

  Matix4x4 OrthoMatrix(float right, float left, float top, float valueottom,
	  float near, float far) const;

  Vector4 GetColum(int colum) const;
  Vector4 GetLine(int line) const;

  Matix4x4 operator+(const Matix4x4& other) const;
  Matix4x4& operator+=(const Matix4x4& other);
  Matix4x4 operator+(float value) const;
  Matix4x4& operator+=(float value);
  Matix4x4 operator-(const Matix4x4& other) const;
  Matix4x4& operator-=(const Matix4x4& other);
  Matix4x4 operator-(float value) const;
  Matix4x4& operator-=(float value);
  Matix4x4& operator*=(float value);
  Matix4x4 operator*(float value) const;
  Matix4x4& operator/=(float value);
  Matix4x4 operator/(float value) const;
  bool operator==(const Matix4x4& other);
  bool operator!=(const Matix4x4& other);
  void operator=(const Matix4x4& other);

  float m[16];
};


inline Matix4x4::Matix4x4() {

}

inline Matix4x4::Matix4x4(const float array[16]) {
	for (int i = 0; i < 16; i++) {
		m[i] = array[i];
	}
}

inline Matix4x4::Matix4x4(float value) {
	for (int i = 0; i < 16; i++) {
		m[i] = value;
	}
}

inline Matix4x4::Matix4x4(const Matix4x4& copy) {
	for (int i = 0; i < 16; i++) {
		m[i] = copy.m[i];
	}
}

inline Matix4x4::~Matix4x4() {

}

inline Matix4x4 Matix4x4::Identity() const {
	float identity[] = { 1.0f , 0.0f , 0.0f, 0.0f,
												0.0f , 1.0f , 0.0f, 0.0f,
												0.0f , 0.0f , 1.0f, 0.0f, 
												0.0f , 0.0f , 0.0f, 1.0f};
	Matix4x4 mI(identity);

	return mI;
}

inline Matix4x4 Matix4x4::Multiply(const Matix4x4& other)const  {
	
	

	Matix4x4 mulMatrix;

	mulMatrix.m[0] = m[0] * other.m[0] + m[1] * other.m[4] + m[2] * other.m[8] + m[3] * other.m[12];
	mulMatrix.m[1] = m[0] * other.m[1] + m[1] * other.m[5] + m[2] * other.m[9] + m[3] * other.m[13];
	mulMatrix.m[2] = m[0] * other.m[2] + m[1] * other.m[6] + m[2] * other.m[10] + m[3] * other.m[14];
	mulMatrix.m[3] = m[0] * other.m[3] + m[1] * other.m[7] + m[2] * other.m[11] + m[3] * other.m[15];

	mulMatrix.m[4] = m[4] * other.m[0] + m[5] * other.m[4] + m[6] * other.m[8] + m[7] * other.m[12];
	mulMatrix.m[5] = m[4] * other.m[1] + m[5] * other.m[5] + m[6] * other.m[9] + m[7] * other.m[13];
	mulMatrix.m[6] = m[4] * other.m[2] + m[5] * other.m[6] + m[6] * other.m[10] + m[7] * other.m[14];
	mulMatrix.m[7] = m[4] * other.m[3] + m[5] * other.m[7] + m[6] * other.m[11] + m[7] * other.m[15];

	mulMatrix.m[8] = m[8] * other.m[0] + m[9] * other.m[4] + m[10] * other.m[8] + m[11] * other.m[12];
	mulMatrix.m[9] = m[8] * other.m[1] + m[9] * other.m[5] + m[10] * other.m[9] + m[11] * other.m[13];
	mulMatrix.m[10] = m[8] * other.m[2] + m[9] * other.m[6] + m[10] * other.m[10] + m[11] * other.m[14];
	mulMatrix.m[11] = m[8] * other.m[3] + m[9] * other.m[7] + m[10] * other.m[11] + m[11] * other.m[15];
	
	mulMatrix.m[12] = m[12] * other.m[0] + m[13] * other.m[4] + m[14] * other.m[8] + m[15] * other.m[12];
	mulMatrix.m[13] = m[12] * other.m[1] + m[13] * other.m[5] + m[14] * other.m[9] + m[15] * other.m[13];
	mulMatrix.m[14] = m[12] * other.m[2] + m[13] * other.m[6] + m[14] * other.m[10] + m[15] * other.m[14];
	mulMatrix.m[15] = m[12] * other.m[3] + m[13] * other.m[7] + m[14] * other.m[11] + m[15] * other.m[15];


	return mulMatrix;
}

inline float Matix4x4::Determinant() const {

	

	return m[0] * (m[5] * (m[10] * m[15] - m[11] * m[14]) - m[6] * (m[9] * m[15] - m[11] * m[13]) +
		m[7] * (m[9] * m[14] - m[10] * m[13])) -
		m[1] * (m[4] * (m[10] * m[15] - m[11] * m[14]) - m[6] * (m[8] * m[15] - m[11] * m[12]) +
			m[7] * (m[8] * m[14] - m[10] * m[12])) +
		m[2] * (m[4] * (m[9] * m[15] - m[11] * m[13]) - m[5] * (m[8] * m[15] - m[11] * m[12]) +
			m[7] * (m[8] * m[13] - m[9] * m[12])) -
		m[3] * (m[4] * (m[9] * m[14] - m[10] * m[13]) - m[5] * (m[8] * m[14] - m[10] * m[12]) +
			m[6] * (m[8] * m[13] - m[9] * m[12]));
}



inline Matix4x4 Matix4x4::Adjoint() const {
	
	Matix4x4 result;

	result.m[0] = (m[5] * m[10] * m[15]) + (m[6] * m[11] * m[13]) + (m[7] * m[9] * m[14])
		- (m[7] * m[10] * m[13]) - (m[6] * m[9] * m[15]) - (m[5] * m[11] * m[14]); 
	result.m[4] = -(m[1] * m[10] * m[15]) - (m[2] * m[11] * m[13]) - (m[3] * m[9] * m[14])
		+ (m[3] * m[10] * m[13]) + (m[2] * m[9] * m[15]) + (m[1] * m[11] * m[14]); 
	result.m[8] = (m[1] * m[6] * m[15]) + (m[2] * m[7] * m[13]) + (m[3] * m[5] * m[14])
		- (m[3] * m[6] * m[13]) - (m[2] * m[5] * m[15]) - (m[1] * m[7] * m[14]); 
	result.m[12] = - (m[1] * m[6] * m[11]) - (m[2] * m[7] * m[9]) - (m[3] * m[5] * m[10])
		+ (m[3] * m[6] * m[9]) + (m[2] * m[5] * m[11]) + (m[1] * m[7] * m[10]); 

	result.m[1] = - (m[4] * m[10] * m[15]) - (m[6] * m[11] * m[12]) - (m[7] * m[8] * m[14])
		+ (m[7] * m[10] * m[12]) + (m[6] * m[8] * m[15]) + (m[4] * m[11] * m[14]); 
	result.m[5] = (m[0] * m[10] * m[15]) + (m[2] * m[11] * m[12]) + (m[3] * m[8] * m[14])
		- (m[3] * m[10] * m[12]) - (m[2] * m[8] * m[15]) - (m[0] * m[11] * m[14]); 
	result.m[9] = - (m[0] * m[6] * m[15]) - (m[2] * m[7] * m[12]) - (m[3] * m[4] * m[14])
		+ (m[3] * m[6] * m[12]) + (m[2] * m[4] * m[15]) + (m[0] * m[7] * m[14]);
	result.m[13] = (m[0] * m[6] * m[11]) + (m[2] * m[7] * m[8]) + (m[3] * m[4] * m[10])
		- (m[3] * m[6] * m[8]) - (m[2] * m[4] * m[11]) - (m[0] * m[7] * m[10]); 

	result.m[2] = (m[4] * m[9] * m[15]) + (m[5] * m[11] * m[12]) + (m[7] * m[8] * m[13])
		- (m[7] * m[9] * m[12]) - (m[5] * m[8] * m[15]) - (m[4] * m[11] * m[13]);
	result.m[6] = - (m[0] * m[9] * m[15]) - (m[1] * m[11] * m[12]) - (m[3] * m[8] * m[13])
		+ (m[3] * m[9] * m[12]) + (m[1] * m[8] * m[15]) + (m[0] * m[11] * m[13]); 
	result.m[10] = (m[0] * m[5] * m[15]) + (m[1] * m[7] * m[12]) + (m[3] * m[4] * m[13])
		- (m[3] * m[5] * m[12]) - (m[1] * m[4] * m[15]) - (m[0] * m[7] * m[13]);
	result.m[14] = - (m[0] * m[5] * m[11]) - (m[1] * m[7] * m[8]) - (m[3] * m[4] * m[9])
		+ (m[3] * m[5] * m[8]) + (m[1] * m[4] * m[11]) + (m[0] * m[7] * m[9]);


	result.m[3] = -(m[4] * m[9] * m[14]) - (m[5] * m[10] * m[12]) - (m[6] * m[8] * m[13])
		+ (m[6] * m[9] * m[12]) + (m[5] * m[8] * m[14]) + (m[4] * m[10] * m[13]);

	result.m[7] = (m[0] * m[9] * m[14]) + (m[1] * m[10] * m[12]) + (m[2] * m[8] * m[13])
		- (m[2] * m[9] * m[12]) - (m[1] * m[8] * m[14]) - (m[0] * m[10] * m[13]);

	result.m[11] = -(m[0] * m[5] * m[14]) - (m[1] * m[6] * m[12]) - (m[2] * m[4] * m[13])
		+ (m[2] * m[5] * m[12]) + (m[1] * m[4] * m[14]) + (m[0] * m[6] * m[13]); 

	result.m[15] = (m[0] * m[5] * m[10]) + (m[1] * m[6] * m[8]) + (m[2] * m[4] * m[9])
		- (m[2] * m[5] * m[8]) - (m[1] * m[4] * m[10]) - (m[0] * m[6] * m[9]);
	

	return result;
}

inline bool Matix4x4::Inverse() {
	if (Determinant() == 0) {
		return false;
	}
	else
	{
		Matix4x4 adjMatrix = Adjoint().Transpose();
		*this = adjMatrix / Determinant();
		return true;
	}

}

inline bool Matix4x4::GetInverse(Matix4x4* out) const {
	if (Determinant() == 0) {
		return false;
	}
	else
		return true;

}

inline Matix4x4 Matix4x4::Transpose() const {
	
	Matix4x4 transMatrix;
	transMatrix.m[0] = m[0];
	transMatrix.m[1] = m[4];
	transMatrix.m[2] = m[8];
	transMatrix.m[3] = m[12];
	transMatrix.m[4] = m[1];
	transMatrix.m[5] = m[5];
	transMatrix.m[6] = m[9];
	transMatrix.m[7] = m[13];
	transMatrix.m[8] = m[2];
	transMatrix.m[9] = m[6];
	transMatrix.m[10] = m[10];
	transMatrix.m[11] = m[14];
	transMatrix.m[12] = m[3];
	transMatrix.m[13] = m[7];
	transMatrix.m[14] = m[11];
	transMatrix.m[15] = m[15];

	return transMatrix;
}

inline Matix4x4 Matix4x4::Translate(const Vector3& distance){
	
	return Translate(distance.x, distance.y, distance.z);
}

inline Matix4x4 Matix4x4::Translate(float x, float y, float z){
	
	Matix4x4 translateMatrix;

	translateMatrix.m[0] = 1.0f;
	translateMatrix.m[1] = 0.0f;
	translateMatrix.m[2] = 0.0f;
	translateMatrix.m[3] = 0.0f;

	translateMatrix.m[4] = 0.0f;
	translateMatrix.m[5] = 1.0f;
	translateMatrix.m[6] = 0.0f;
	translateMatrix.m[7] = 0.0f;

	translateMatrix.m[8] = 0.0f;
	translateMatrix.m[9] = 0.0f;
	translateMatrix.m[10] = 1.0f;
	translateMatrix.m[11] = 0.0f;

	translateMatrix.m[12] = x;
	translateMatrix.m[13] = y;
	translateMatrix.m[14] = z;
	translateMatrix.m[15] = 1.0f;

	return translateMatrix;
}

inline Matix4x4 Matix4x4::Scale(const Vector3& scale){
	Matix4x4 scaleMatrix;

	scaleMatrix.m[0] = scale.x;
	scaleMatrix.m[1] = 0.0f;
	scaleMatrix.m[2] = 0.0f;
	scaleMatrix.m[3] = 0.0f;

	scaleMatrix.m[4] = 0.0f;
	scaleMatrix.m[5] = scale.y;
	scaleMatrix.m[6] = 0.0f;
	scaleMatrix.m[7] = 0.0f;

	scaleMatrix.m[8] = 0.0f;
	scaleMatrix.m[9] = 0.0f;
	scaleMatrix.m[10] = scale.z;
	scaleMatrix.m[11] = 0.0f;

	scaleMatrix.m[12] = 0.0f;
	scaleMatrix.m[13] = 0.0f;
	scaleMatrix.m[14] = 0.0f;
	scaleMatrix.m[15] = 1.0f;

	return scaleMatrix;
}

inline Matix4x4 Matix4x4::Scale(float x, float y, float z){
	Matix4x4 scaleMatrix;

	scaleMatrix.m[0] = x;
	scaleMatrix.m[1] = 0.0f;
	scaleMatrix.m[2] = 0.0f;
	scaleMatrix.m[3] = 0.0f;

	scaleMatrix.m[4] = 0.0f;
	scaleMatrix.m[5] = y;
	scaleMatrix.m[6] = 0.0f;
	scaleMatrix.m[7] = 0.0f;

	scaleMatrix.m[8] = 0.0f;
	scaleMatrix.m[9] = 0.0f;
	scaleMatrix.m[10] = z;
	scaleMatrix.m[11] = 0.0f;

	scaleMatrix.m[12] = 0.0f;
	scaleMatrix.m[13] = 0.0f;
	scaleMatrix.m[14] = 0.0f;
	scaleMatrix.m[15] = 1.0f;

	return scaleMatrix;
}

inline Matix4x4 Matix4x4::RotateX(float radians){
	
	Matix4x4 rotateMatrix;

	rotateMatrix.m[0] = 1.0f;
	rotateMatrix.m[1] = 0.0f;
	rotateMatrix.m[2] = 0.0f;
	rotateMatrix.m[3] = 0.0f;

	rotateMatrix.m[4] = 0.0f;
	rotateMatrix.m[5] = cos(radians);
	rotateMatrix.m[6] = -sin(radians);
	rotateMatrix.m[7] = 0.0f;

	rotateMatrix.m[8] = 0.0f;
	rotateMatrix.m[9] = sin(radians);
	rotateMatrix.m[10] = cos(radians);
	rotateMatrix.m[11] = 0.0f;

	rotateMatrix.m[12] = 0.0f;
	rotateMatrix.m[13] = 0.0f;
	rotateMatrix.m[14] = 0.0f;
	rotateMatrix.m[15] = 1.0f;

	return rotateMatrix;
}

inline Matix4x4 Matix4x4::RotateY(float radians){
	
	Matix4x4 rotateMatrix;

	rotateMatrix.m[0] = cos(radians);
	rotateMatrix.m[1] = 0.0f;
	rotateMatrix.m[2] = sin(radians);
	rotateMatrix.m[3] = 0.0f;

	rotateMatrix.m[4] = 0.0f;
	rotateMatrix.m[5] = 1.0f;
	rotateMatrix.m[6] = 0.0f;
	rotateMatrix.m[7] = 0.0f;

	rotateMatrix.m[8] = -sin(radians);
	rotateMatrix.m[9] = 0.0f;
	rotateMatrix.m[10] = cos(radians);
	rotateMatrix.m[11] = 0.0f;

	rotateMatrix.m[12] = 0.0f;
	rotateMatrix.m[13] = 0.0f;
	rotateMatrix.m[14] = 0.0f;
	rotateMatrix.m[15] = 1.0f;

	return rotateMatrix;
}

inline Matix4x4 Matix4x4::RotateZ(float radians) {
	
	Matix4x4 rotateMatrix;

	rotateMatrix.m[0] = cos(radians);
	rotateMatrix.m[1] = -sin(radians);
	rotateMatrix.m[2] = 0.0f;
	rotateMatrix.m[3] = 0.0f;

	rotateMatrix.m[4] = sin(radians);
	rotateMatrix.m[5] = cos(radians);
	rotateMatrix.m[6] = 0.0f;
	rotateMatrix.m[7] = 0.0f;

	rotateMatrix.m[8] = 0.0f;
	rotateMatrix.m[9] = 0.0f;
	rotateMatrix.m[10] = 1.0f;
	rotateMatrix.m[11] = 0.0f;

	rotateMatrix.m[12] = 0.0f;
	rotateMatrix.m[13] = 0.0f;
	rotateMatrix.m[14] = 0.0f;
	rotateMatrix.m[15] = 1.0f;

	return rotateMatrix;
}

inline Matix4x4 Matix4x4::GetTransform(const Vector3& translate,
								const Vector3& scale,
								float rotateX, float rotateY,
								float rotateZ)   {
	Matix4x4 transformMatrix = Translate(translate);
	transformMatrix = transformMatrix.Multiply(Scale(scale));
	transformMatrix = transformMatrix.Multiply(RotateX(rotateX));
	transformMatrix = transformMatrix.Multiply(RotateY(rotateY));
	transformMatrix = transformMatrix.Multiply(RotateZ(rotateZ));

	return transformMatrix;
}

inline Matix4x4 Matix4x4::GetTransform(float trans_x, float trans_y, float trans_z,
	float scale_x, float scale_y, float scale_Z,
	float rotateX, float rotateY, float rotateZ)  {
	
	Matix4x4 transformMatrix = Translate(trans_x, trans_y, trans_z);
	transformMatrix = transformMatrix.Multiply(Scale(scale_x, scale_y, scale_Z));
	transformMatrix = transformMatrix.Multiply(RotateX(rotateX));
	transformMatrix = transformMatrix.Multiply(RotateY(rotateY));
	transformMatrix = transformMatrix.Multiply(RotateZ(rotateZ));

	return transformMatrix;
}

inline Vector4 Matix4x4::GetColum(int colum) const {
	return Vector4(m[colum], m[colum + 4], m[colum + 8], m[colum + 12]);
}

inline Vector4 Matix4x4::GetLine(int line) const {
	return Vector4(m[line * 4], m[line * 4 + 1], m[line * 4 + 2], m[line * 4 + 3]);
}

inline Matix4x4 Matix4x4::PerspectiveMatrix(float fov, float aspect,
	float near, float far) const
{
	float upper = near * tan((3.1415 / 180.0) * (fov * 0.5));
	float down = -1 * down;
	float right = upper * aspect;
	float left = -1 * right;

	Matix4x4 pers_matrix;

	pers_matrix.m[0] = (2 * near) / (right - left);
	pers_matrix.m[1] = 0.0f;
	pers_matrix.m[2] = (right + left) / (right - left);
	pers_matrix.m[3] = 0.0f;

	pers_matrix.m[4] = 0.0f;
	pers_matrix.m[5] = (2 * near) / (upper - down);
	pers_matrix.m[6] = (upper + down) / (upper - down);
	pers_matrix.m[7] = 0.0f;

	pers_matrix.m[8] = 0.0f;
	pers_matrix.m[9] = 0.0f;
	pers_matrix.m[10] = -((far + near) / (far - near));
	pers_matrix.m[11] = -((2 * near * far) / (far - near));

	pers_matrix.m[12] = 0.0f;
	pers_matrix.m[13] = 0.0f;
	pers_matrix.m[14] = -1.0f;
	pers_matrix.m[15] = 0.0f;

	return pers_matrix;
}

inline Matix4x4 Matix4x4::OrthoMatrix(float right, float left, float top, float bottom,
	float near, float far) const
{
	Matix4x4 or_matrix;

	or_matrix.m[0] = 2.0f / (right - left);
	or_matrix.m[1] = 0.0f;
	or_matrix.m[2] = 0.0f;
	or_matrix.m[3] = -((right + left) / (right - left));

	or_matrix.m[4] = 0.0f;
	or_matrix.m[5] = 2.0f / (top - bottom);
	or_matrix.m[6] = -((top + bottom) / (top - bottom));
	or_matrix.m[7] = 0.0f;

	or_matrix.m[8] = 0.0f;
	or_matrix.m[9] = 0.0f;
	or_matrix.m[10] = -2.0f / (far - near);
	or_matrix.m[11] = -((far + near) / (far - near));

	or_matrix.m[12] = 0.0f;
	or_matrix.m[13] = 0.0f;
	or_matrix.m[14] = 0.0f;
	or_matrix.m[15] = 1.0f;

	return or_matrix;
}



inline Matix4x4 Matix4x4::operator+(const Matix4x4& other) const {
	
	Matix4x4 addMatrix;

	for (int i = 0; i < 16; i++) {
		addMatrix.m[i] = m[i] + other.m[i];
	}
	
	return addMatrix;
}

inline Matix4x4& Matix4x4::operator+=(const Matix4x4& other) {
	for (int i = 0; i < 16; i++) {
		m[i] += other.m[i];
	}
	return *this;
}

inline Matix4x4 Matix4x4::operator+(float value) const {
	Matix4x4 addMatrix;

	for (int i = 0; i < 16; i++) {
		addMatrix.m[i] = m[i] + value;
	}

	return addMatrix;
}

inline Matix4x4& Matix4x4::operator+=(float value) {
	for (int i = 0; i < 16; i++) {
		m[i] += value;
	}
	return *this;
}


inline Matix4x4 Matix4x4::operator-(const Matix4x4& other) const  {
	Matix4x4 subMatrix;

	for (int i = 0; i < 16; i++) {
		subMatrix.m[i] = m[i] - other.m[i];
	}

	return subMatrix;
}

inline Matix4x4& Matix4x4::operator-=(const Matix4x4& other) {
	for (int i = 0; i < 16; i++) {
		m[i] -= other.m[i];
	}
	return *this;
}

inline Matix4x4 Matix4x4::operator-(float value) const  {
	Matix4x4 subMatrix;

	for (int i = 0; i < 16; i++) {
		subMatrix.m[i] = m[i] - value;
	}

	return subMatrix;
}

inline Matix4x4& Matix4x4::operator-=(float value) {
	for (int i = 0; i < 16; i++) {
		m[i] -= value;
	}
	return *this;
}

inline Matix4x4& Matix4x4::operator*=(float value) {
	for (int i = 0; i < 16; i++) {
		m[i] *= value;
	}
	return *this;
}

inline Matix4x4 Matix4x4::operator*(float value) const  {
	Matix4x4 mulMatrix;

	for (int i = 0; i < 16; i++) {
		mulMatrix.m[i] = m[i] * value;
	}

	return mulMatrix;
}

inline Matix4x4& Matix4x4::operator/=(float value) {
	for (int i = 0; i < 16; i++) {
		m[i] /= value;
	}
	return *this;
}

inline Matix4x4 Matix4x4::operator/(float value) const {
	Matix4x4 divMatrix;

	for (int i = 0; i < 16; i++) {
		divMatrix.m[i] = m[i] / value;
	}

	return divMatrix;
}

inline bool Matix4x4::operator==(const Matix4x4& other) {
	for (int i = 0; i < 16; i++) {
		if (m[i] != other.m[i]) {
			return false;
		}
	}
	return true;
}

inline bool Matix4x4::operator!=(const Matix4x4& other) {
	int coincidence = 0;
	for (int i = 0; i < 16; i++) {
		if (m[i] == other.m[i]) {
			coincidence++;
		}
	}
	if (coincidence == 16) {
		return false;
	}
	else
		return true;
}

inline void Matix4x4::operator=(const Matix4x4& other) {
	for (int i = 0; i < 16; i++) {
		m[i] = other.m[i];
	}
}

#endif
