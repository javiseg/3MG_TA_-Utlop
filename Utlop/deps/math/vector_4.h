#ifndef __Vector4_H__
#define __Vector4_H__ 1

#include "vector_3.h"
#include "matrix_3.h"

class Vector4 {
public:

	Vector4();
	Vector4(float x, float y, float z, float w);
	Vector4(Vector3 a, float w);
	Vector4(float a);
	Vector4(float* values_array);
	Vector4(const Vector4& other);
	~Vector4();
	
	Vector4 operator+(const Vector4& other) const;
	Vector4 operator+(float value) const;
	void operator+=(const Vector4& other);
	void operator+=(float value);
	Vector4 operator-(const Vector4& other) const;
	Vector4 operator-(float value) const;
	void operator -=(const Vector4& other);
	void operator -=(float value);

	Vector4 operator*(float value) const;
	void operator*=(float value);
	Vector4 operator/(float value) const;
	void operator/=(float value);
	bool operator==(const Vector4& other);
	bool operator!=(const Vector4& other);
	void operator=(const Vector4& other);

	float Magnitude() const;
	void Normalize();
	Vector4 Normalized() const;
	void Scale(Vector4 scale);
	float SqrMagnitude() const;

	static float Distance(const Vector4& a, const Vector4& b);
	static float DotProduct(Vector4 a, Vector4 b);
	static Vector4 Lerp(const Vector4& a, const Vector4& b, float index);	

	static const Vector4 one;
	static const Vector4 zero;

	float x;
	float y;
	float z;
	float w;

};

inline Vector4::Vector4() { }

inline Vector4::Vector4(float x, float y, float z, float w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

inline Vector4::Vector4(Vector3 a, float w) {
	this->x = a.x;
	this->y = a.y;
	this->z = a.z;
	this->w = w;
}

inline Vector4::Vector4(float a) {
	this->x = a;
	this->y = a;
	this->z = a;
	this->w = a;
}

inline Vector4::Vector4(float* values_array) {
	this->x = values_array[0];
	this->y = values_array[1];
	this->z = values_array[2];
	this->w = values_array[3];
}

inline Vector4::Vector4(const Vector4& other) {
	*this = other;
}

inline Vector4::~Vector4() { }

inline float Vector4::Magnitude() const{
	return sqrt(x * x + y * y + z * z + w*w);
}

inline void Vector4::Normalize() {	
	float magnitude = Magnitude();

	this->x = this->x / magnitude;
	this->y = this->y / magnitude;
	this->z = this->z / magnitude;
	this->w = this->w / magnitude;
}

inline Vector4 Vector4::Normalized() const {
	float length = Magnitude();

	Vector4 norVec;

	norVec.x = x / length;
	norVec.y = y / length;
	norVec.z = z / length;
	norVec.w = w / length;

	return norVec;
}

inline void Vector4::Scale(Vector4 scale) {
	x *= scale.x;
	y *= scale.y;
	z *= scale.z;
	w *= scale.w;
}

inline float Vector4::SqrMagnitude() const {
	return x * x + y * y + z * z + w * w;
}

inline float Vector4::Distance(const Vector4& a, const Vector4& b) {
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) +
		(a.z - b.z) * (a.z - b.z) + (a.w - b.w) * (a.w - b.w));
}

inline float Vector4::DotProduct(Vector4 a, Vector4 b) {
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

inline Vector4 Vector4::Lerp(const Vector4& a, const Vector4& b, float index) {	
	if (index < 0.0f) { index = 0.0f; }
	if (index > 1.0f) { index = 1.0f; }
	return Vector4(a + ((b - a) * index));
}

inline Vector4 Vector4::operator+(const Vector4& other) const{
	Vector4 addVector;
	addVector.x = x + other.x;
	addVector.y = y + other.y;
	addVector.z = z + other.z;
	addVector.w = w + other.w;

	return addVector;
}

inline Vector4 Vector4::operator+(float value) const{
	Vector4 addVector;
	addVector.x = x + value;
	addVector.y = y + value;
	addVector.z = z + value;
	addVector.w = w + value;

	return addVector;
}

inline void Vector4::operator+=(const Vector4& other) {
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
	this->w += other.w;
}

inline void Vector4::operator+=(float value) {
	this->x += value;
	this->y += value;
	this->z += value;
	this->w += value;
}

inline Vector4 Vector4::operator-(const Vector4& other) const{
	Vector4 subVector;
	subVector.x = x - other.x;
	subVector.y = y - other.y;
	subVector.z = z - other.z;
	subVector.w = w - other.w;

	return subVector;
}

inline Vector4 Vector4::operator-(float value) const{
	Vector4 subVector;
	subVector.x = x - value;
	subVector.y = y - value;
	subVector.z = z - value;
	subVector.w = w - value;

	return subVector;
}

inline void Vector4::operator -=(const Vector4& other) {	
	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;
	this->w -= other.w;
}

inline void Vector4::operator -=(float value) {
	this->x -= value;
	this->y -= value;
	this->z -= value;
	this->w -= value;
}

inline Vector4 Vector4::operator*(float value) const{
	Vector4 mulVector;
	mulVector.x = x * value;
	mulVector.y = y * value;
	mulVector.z = z * value;
	mulVector.w = w * value;

	return mulVector;
}

inline void Vector4::operator*=(float value) {
	x *= value;
	y *= value;
	z *= value;
	w *= value;
}

inline Vector4 Vector4::operator/(float value) const{
	Vector4 divVector;
	divVector.x = x / value;
	divVector.y = y / value;
	divVector.z = z / value;
	divVector.w = w / value;

	return divVector;
}

inline void Vector4::operator/=(float value) {
	x /= value;
	y /= value;
	z /= value;
	w /= value;
}

inline bool Vector4::operator==(const Vector4& other) {
	return x == other.x && y == other.y && z == other.z && w == other.w;
}
inline bool Vector4::operator!=(const Vector4& other) {
	return x != other.x || y != other.y || z != other.z || w != other.w;
}
inline void Vector4::operator=(const Vector4& other) {
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	this->w = other.w;
}

#endif 
