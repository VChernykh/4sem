#include <iostream>
#include "Vector2.h"
#include <cmath>

using namespace std;

ostream& operator<<(ostream& stream, const Vector2& vector) {
	stream << "[" << vector.x << "; " << vector.y << "]";
	return stream;
}

istream& operator>>(istream& stream, Vector2& vector) {
	stream >> vector.x >> vector.y;
	return stream;
}

Vector2 Vector2::operator+(const Vector2 other) const {
	Vector2 result;
	result.x = x + other.x;
	result.y = y + other.y;
	return result;
}

Vector2& Vector2::operator+=(const Vector2& other) {
	x += other.x;
	y += other.y;
	return *this;
}

Vector2 Vector2::operator-(const Vector2 other) const {
	Vector2 result;
	result.x = x - other.x;
	result.y = y - other.y;
	return result;
}

Vector2& Vector2::operator-=(const Vector2& other) {
	x -= other.x;
	y -= other.y;
	return *this;
}

float Vector2::operator*(const Vector2& other) const {
	int result = x * other.x + y * other.y;
	return result;
}

float Vector2::operator^(const Vector2& other) const {
	int result = x * other.y - y * other.x;
	return result;
}

Vector2 operator*(float num, const Vector2& vector) {
	return Vector2(vector.x * num, vector.y * num);
}

Vector2 Vector2::operator*(float num) {
	return Vector2(x * num, y * num);
}

Vector2 Vector2::operator/(float num) {
	return Vector2(x / num, y / num);
}

Vector2 Vector2::norm() {
	return Vector2(x / abs(x), y / abs(y));
}

Vector2 Vector2::perpendicular() {
	return Vector2(y, -x);
}

float Vector2::len() {
	return (sqrt(x * x + y * y));
}

float Vector2::squareLen() {
	return (x * x + y * y);
}

Vector2 Vector2::operator-() const {
	return Vector2(-x, -y);
}

Vector2::Vector2() {
	x = 0;
	y = 0;
}

Vector2::Vector2(float xx, float yy) {
	x = xx;
	y = yy;
}

Vector2& Vector2::rotate(float a) {
	int x1 = x;
	int y1 = y;
	x = x1 * cos(a) - y1 * sin(a);
	y = x1 * sin(a) + y1 * cos(a);
	return *this;
}

Vector2 Vector2::getRotated(float a) const {
	Vector2 answer;
	int x1 = x;
	int y1 = y;
	answer.x = x1 * cos(a) - y1 * sin(a);
	answer.y = y1 * cos(a) + x1 * sin(a);
	return answer;
}