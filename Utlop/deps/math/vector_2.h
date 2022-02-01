#ifndef __VECTOR2_H__
#define __VECTOR2_H__ 1

#include <math.h>

class Vector2 {
 public:

  Vector2();
  Vector2(float x, float y);
  Vector2(const Vector2& copy);
  ~Vector2();

  Vector2 operator+(const Vector2& other) const;
  Vector2 operator+(float value);
  Vector2& operator+=(const Vector2& other);
  Vector2& operator+=(float value);
  Vector2 operator-(const Vector2& other) const;
  Vector2 operator-(float value) const;
  Vector2& operator-();
  Vector2& operator-=(const Vector2& other);
  Vector2& operator-=(float value);
  bool operator==(const Vector2& other) const;
  bool operator!=(const Vector2& other) const;
  void operator=(const Vector2& other);
  void operator=(float value);
  Vector2 operator*(float value) const;
  Vector2& operator*=(float value);
  Vector2 operator/(float value) const;
  Vector2& operator/=(float value);

  float Magnitude() const;
  void Normalize();
  Vector2 Normalized() const;

  void Scale(const Vector2 scale);

  float SqrMagnitude() const;
  static float Distance(const Vector2 a, const Vector2 b);

  static float DotProduct(Vector2 a, Vector2 b);

  static Vector2 Lerp(const Vector2 a, const Vector2 b, float t);
  static Vector2 LerpUnclamped(const Vector2 a, const Vector2 b, float t);

  static const Vector2 up;
  static const Vector2 down;
  static const Vector2 right;
  static const Vector2 left;
  static const Vector2 zero;
  static const Vector2 one;

  float x;
  float y;
};

inline Vector2::Vector2() {}

inline Vector2::Vector2(float x, float y) {
	this->x = x;
	this->y = y;
}

inline Vector2::Vector2(const Vector2& other) {
	this->x = other.x;
	this->y = other.y;
}

inline Vector2::~Vector2() {}

inline Vector2 Vector2::operator+(const Vector2& other) const {
	Vector2 addResult;
	addResult.x = x + other.x;
	addResult.y = y + other.y;
  return addResult;
}

inline Vector2 Vector2::operator+(float value) {
	Vector2 addResult;
	addResult.x = x + value;
	addResult.y = y + value;
	return addResult;
}

inline Vector2& Vector2::operator+=(const Vector2& other){
	x = x + other.x;
	y = y + other.y;
  return *this;
}

inline Vector2& Vector2::operator+=(float value){
	x = x + value;
	y = y + value;
  return *this;
}

inline Vector2 Vector2::operator-(const Vector2& other) const {
	Vector2 subResult;
	subResult.x = x - other.x;
	subResult.y = y - other.y;
	return subResult;
}

inline Vector2 Vector2::operator-(float value) const {
	Vector2 subResult;
	subResult.x = x - value;
	subResult.y = y - value;
	return subResult;
}

inline Vector2& Vector2::operator-() {
	return *this;
}

inline Vector2& Vector2::operator-=(const Vector2& other) {
	x -= other.x;
	y -= other.y;
  return *this;
}

inline Vector2& Vector2::operator-=(float value){
	x -= value;
	y -= value;
  return *this;
}

inline bool Vector2::operator==(const Vector2& value) const {

  return x == value.x && y == value.y;
}

inline bool Vector2::operator!=(const Vector2& value) const {
  return !(x == value.x && y == value.y);
}


inline void Vector2::operator=(const Vector2& other) {
	x = other.x;
	y = other.y;
}

inline void Vector2::operator=(float value) {
}

inline Vector2 Vector2::operator*(float value) const {

	Vector2 vecMult;
	vecMult.x = x * value;
	vecMult.y = y * value;

	return vecMult;
}

inline Vector2& Vector2::operator*=(float value) {  
  return *this;
}

inline Vector2 Vector2::operator/(float value) const {

	Vector2 vecDiv;
	vecDiv.x = x / value;
	vecDiv.y = y / value;

	return vecDiv;
}

inline Vector2& Vector2::operator/=(float value) {
	return Vector2();
}

inline float Vector2::Magnitude() const {

	float magnitude = sqrt(x * x + y * y);

  return magnitude;
}

inline void Vector2::Normalize() { 

	float magnitude = Magnitude();

	this->x = this->x / magnitude;
	this->y = this->y / magnitude;

}

inline Vector2 Vector2::Normalized() const {

	float magnitude = Magnitude();

	Vector2 vectorNormalize;

	vectorNormalize.x = x / magnitude;
	vectorNormalize.y = y / magnitude;

	return vectorNormalize;
}

inline void Vector2::Scale(const Vector2 scale){
		
	x *= scale.x;
	y *= scale.y;

}

inline float Vector2::SqrMagnitude() const {
  return x * x + y * y;
}


inline float Vector2::Distance(const Vector2 a, const Vector2 b) {

  return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

inline Vector2 Vector2::Lerp(const Vector2 a, const Vector2 b, float t) {
	if (t < 0.0f) { t = 0.0f; }
	if (t > 1.0f) { t = 1.0f; }
	return Vector2(a + ((b - a) * t));
}
inline Vector2 Vector2::LerpUnclamped(const Vector2 a, const Vector2 b, float t) {
	return Vector2(a + ((b - a) * t));
}


inline float Vector2::DotProduct(Vector2 a, Vector2 b) {
	float dotP;

	dotP = a.x * b.x + a.y * b.y;


	return dotP;
}

#endif 
