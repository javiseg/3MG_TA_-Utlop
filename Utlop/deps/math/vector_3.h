#ifndef __VECTOR_3_H__
#define __VECTOR_3_H__ 1

#include "math_utils.h"
#include <math.h>

class Vector3 {

public:
	Vector3();
	Vector3(float value);
	Vector3(float x, float y, float z);
	Vector3(float* values_array);
	Vector3(const float* values_array);
	Vector3(const Vector3& other);
	~Vector3();

	Vector3 operator+(const Vector3& other) const;
	Vector3 operator+(float value) const;
	Vector3& operator+=(const Vector3& other);
	Vector3& operator+=(float value);
	Vector3 operator-(const Vector3& other) const;
	Vector3 operator-(float value) const;
	Vector3& operator-=(const Vector3& other);
	Vector3& operator-=(float value);
	bool operator==(const Vector3& other) const;
	bool operator!=(const Vector3& other) const;
	void operator=(const Vector3& other);
	void operator=(float value);
	Vector3 operator*(float value) const;
	Vector3& operator*=(float value);
	Vector3 operator/(float value) const;
	Vector3& operator/=(float value);

	float Magnitude() const;
	Vector3 Normalized() const;
	void Normalize();
	float SqrMagnitude() const;
	void Scale(const Vector3& other);

	static Vector3 Lerp(const Vector3& a, const Vector3& b, float t);
	static Vector3 LerpUnclamped(const Vector3& a, const Vector3& b, float t);
	static float DotProduct(const Vector3& a, const Vector3& b);
	static float Angle(const Vector3& a, const Vector3& b);
	static Vector3 CrossProduct(const Vector3& a,const Vector3& b);	
	static float Distance(const Vector3& a, const Vector3& b);
	static Vector3 Reflect(const Vector3& direction, const Vector3& normal);
	//inline Vector3 Vector3::operator+(const Vector3& other) const;


	static const Vector3 up;
	static const Vector3 down;
	static const Vector3 right;
	static const Vector3 left;
	static const Vector3 forward;
	static const Vector3 back;
	static const Vector3 zero;
	static const Vector3 unit;

	float x;
	float y;
	float z;
};
inline Vector3::Vector3() {}

inline Vector3::Vector3(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

inline Vector3::Vector3(float * values_array) {

	this->x = values_array[0];
	this->y = values_array[1];
	this->z = values_array[2];
}
inline Vector3::Vector3(const float * values_array) {

	this->x = values_array[0];
	this->y = values_array[1];
	this->z = values_array[2];
}

inline Vector3::Vector3(float value) {
	this->x = value;
	this->y = value;
	this->z = value;
}

inline Vector3::Vector3(const Vector3& other) {
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
}

inline Vector3::~Vector3() {}

inline float Vector3::Magnitude() const {
	
	return sqrt(SqrMagnitude());
}

inline void Vector3::Normalize() {	

	float magnitude = Magnitude();

	this->x = this->x / magnitude;
	this->y = this->y / magnitude;
	this->z = this->z / magnitude;
}

inline Vector3 Vector3::Normalized() const {

	float length = Magnitude();

	Vector3 norVec;

	norVec.x = x / length;
	norVec.y = y / length;
	norVec.z = z / length;

	return norVec;
}

inline float Vector3::DotProduct(const Vector3& a, const Vector3& other)  {

	float dotP;

	dotP = a.x * other.x + a.y * other.y + a.z * other.z;


	return dotP;
}

inline float Vector3::Angle(const Vector3& a, const Vector3& other)  {
	float cos = DotProduct(a, other) / (a.Magnitude()*other.Magnitude());

	return acos(cos);
}

inline Vector3 Vector3::CrossProduct(const Vector3& a, const Vector3& other)  {

	Vector3 crossP;

	crossP.x = a.y * other.z - a.z * other.y;
	crossP.y = a.z * other.x - a.x * other.z;
	crossP.z = a.x * other.y - a.y * other.x;

	return crossP;
}

inline float Vector3::SqrMagnitude() const {
	
	return x*x+y*y+z*z;
}

inline void Vector3::Scale(const Vector3& other) {
	x *= other.x;
	y *= other.y;
	z *= other.z;
}

inline Vector3 Vector3::Lerp(const Vector3& a, const Vector3& b, float t) {
	if (t < 0.0f) { t = 0.0f; }
	if (t > 1.0f) { t = 1.0f; }
	return Vector3(a + ((b - a) * t));
}
inline Vector3 Vector3::LerpUnclamped(const Vector3& a, const Vector3& b, float t) {
	return Vector3(a + ((b - a) * t));
}

inline float Vector3::Distance(const Vector3& a, const Vector3& b) {
	
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y)* (a.y - b.y)+
	(a.z - b.z)* (a.z - b.z));
}

inline Vector3 Vector3::Reflect(const Vector3& direction, const Vector3& normal) {

	return direction - normal * (2 * (DotProduct(direction,normal))) ;
}

inline Vector3 Vector3::operator+(const Vector3& other) const {
	
	Vector3 addVector;
	addVector.x = x + other.x;
	addVector.y = y + other.y;
	addVector.z = z + other.z;
	
	return addVector;
}

inline Vector3 Vector3::operator+(float value) const {
	Vector3 addVector;
	addVector.x = x + value;
	addVector.y = y + value;
	addVector.z = z + value;

	return addVector;
}

inline Vector3& Vector3::operator+=(const Vector3& other) {
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

inline Vector3& Vector3::operator+=(float value) {
	x += value;
	y += value;
	z += value;
	return *this;
}

inline Vector3 Vector3::operator-(const Vector3& other) const {
	Vector3 subVector;
	subVector.x = x - other.x;
	subVector.y = y - other.y;
	subVector.z = z - other.z;

	return subVector;
}

inline Vector3 Vector3::operator-(float value) const {
	Vector3 subVector;
	subVector.x = x - value;
	subVector.y = y - value;
	subVector.z = z - value;

	return subVector;
}

inline Vector3& Vector3::operator-=(const Vector3& other) {
	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;
	return *this;
}

inline Vector3& Vector3::operator-=(float value) {
	this->x -= value;
	this->y -= value;
	this->z -= value;
	return *this;
}

inline bool Vector3::operator==(const Vector3& other) const {
	return x==other.x && y==other.y && z==other.z;
}

inline bool Vector3::operator!=(const Vector3& other) const {
	return x!=other.x || y!=other.y || z!=other.z;
}

inline void Vector3::operator=(const Vector3& other) {	
	x = other.x;
	y = other.y;
	z = other.z;
}

inline void Vector3::operator=(float value) {
	x = value;
	y = value;
	z = value;
}

inline Vector3 Vector3::operator*(float value) const {
	Vector3 mulVector;
	mulVector.x = x * value;
	mulVector.y = y * value;
	mulVector.z = z * value;

	return mulVector;
}

inline Vector3& Vector3::operator*=(float value) {
	x *= value;
	y *= value;
	z *= value;
	return *this;
}

inline Vector3 Vector3::operator/(float value) const {
	Vector3 divVector;
	divVector.x = x / value;
	divVector.y = y / value;
	divVector.z = z / value;

	return divVector;
}

inline Vector3& Vector3::operator/=(float value) {	
	x /= value;
	y /= value;
	z /= value;
	return *this;
}

#endif 
