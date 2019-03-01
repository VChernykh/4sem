#pragma once
#ifndef VECTOR2_H_
#include <iostream>

class Vector2{

public:
	float x, y;
	Vector2 operator+(const Vector2 other) const;
	Vector2& operator+=(const Vector2& other);
	Vector2 operator-(const Vector2 other) const;
	Vector2& operator-=(const Vector2& other);
	float operator*(const Vector2& other) const;
	float operator^(const Vector2& other) const;
	Vector2 operator*(float num);
	Vector2 operator/(float num);
	Vector2 norm();
	Vector2 perpendicular();
	float len();
	float squareLen();
	Vector2 operator-() const;
	Vector2();
	Vector2(float xx, float yy);
	Vector2& rotate(float angle);
	Vector2 getRotated(float angle) const;
};

std::ostream& operator<<(std::ostream& stream, const Vector2& vector);
std::istream& operator>>(std::istream& stream, Vector2& vector);
Vector2 operator*(float num, const Vector2& vector);

#endif