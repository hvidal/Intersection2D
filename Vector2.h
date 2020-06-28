#ifndef VECTOR2_H_
#define VECTOR2_H_

#include <math.h>
#include "Log.h"

class Vector2 {
private:
	float mX, mY;
	static constexpr float tolerance = 0.000001f;

public:

	Vector2(float x, float y):
		mX(x),
		mY(y)
	{}

	~Vector2() {}

	float x() const { return mX; }
	float y() const { return mY; }

	float length() const { return sqrt(mX * mX + mY * mY); }
	float dot(Vector2 v) const { return mX * v.mX + mY * v.mY; }
	void print() const { Log::info("(%f, %f)", mX, mY); }

	bool operator==(const Vector2& v) { return fabs(mX - v.mX) < tolerance && fabs(mY - v.mY) < tolerance; }
	Vector2 operator-(const Vector2& v) const { return Vector2(mX-v.mX, mY-v.mY); }
	Vector2 operator+(const Vector2& v) const { return Vector2(mX+v.mX, mY+v.mY); }
	Vector2 operator/(float s) const { return Vector2(mX/s, mY/s); }
	void operator+=(const Vector2& v) { mX += v.mX; mY += v.mY; }
	void operator/=(float s) { mX /= s; mY /= s; }

	void normalize();
	bool isParallelTo(const Vector2& v) const;
	Vector2 getNormalCCW() const;
	Vector2 getNormalCW() const;
	bool pointsToTheLeft(const Vector2& v) const;
	bool pointsToTheRight(const Vector2& v) const;
};

void Vector2::normalize() {
	float length = this->length();
	mX /= length;
	mY /= length;
}

/* Returns a normal vector that is a counterclockwise rotation of this vector (consider the Y-axis pointing up) */
Vector2 Vector2::getNormalCCW() const {
	return Vector2(-mY, mX);
}

/* Returns a normal vector that is a clockwise rotation of this vector (consider the Y-axis pointing up) */
Vector2 Vector2::getNormalCW() const {
	return Vector2(mY, -mX);
}

bool Vector2::isParallelTo(const Vector2& v) const {
	// cross product using z=0
	float cross = mX * v.mY - mY * v.mX;
	return cross == 0;
}

bool Vector2::pointsToTheLeft(const Vector2& v) const {
	return getNormalCCW().dot(v) > 0;
}

bool Vector2::pointsToTheRight(const Vector2& v) const {
	return getNormalCW().dot(v) > 0;
}

#endif
