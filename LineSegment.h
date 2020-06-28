#ifndef LINESEGMENT_H_
#define LINESEGMENT_H_

#include <iostream>
#include <vector>
#include <cmath>
#include "Vector2.h"
#include "Log.h"

class LineSegment {
private:
	const Vector2 mP0;
	const Vector2 mP1;
	const Vector2 mV;
	const Vector2 mNormal;
	float mA;
	float mB;

	bool intersectsSegment(const LineSegment& s) const;
	Vector2 intersectionPoint(const LineSegment& s) const;

public:
	LineSegment(const Vector2& p0, const Vector2& p1) :
		mP0(p0),
		mP1(p1),
		mV(p1-p0),
		mNormal(mV.getNormalCW())
	{
		// considering the equation y = ax + b, let's calculate 'a' (the slope)
		mA = (mP1.y() - mP0.y()) / (mP1.x() - mP0.x());
		// considering the equation y = ax + b, let's calculate 'b' (we need 'a')
		mB = mP0.y() - mA * mP0.x();
	}

	~LineSegment() {}

	const Vector2& p0() const { return mP0; }
	const Vector2& p1() const { return mP1; }
	const Vector2& v() const { return mV; }

	bool isOnTheLine(const Vector2& point) const;
	void intersectionWith(const LineSegment& s, std::vector<Vector2>& points) const;
	bool isOnTheSameSide(const Vector2& p1, const Vector2& p2) const;
};

bool LineSegment::isOnTheLine(const Vector2& point) const {
	// using y = ax + b
	float y = mA * point.x() + mB;
	return y == point.y();
}

bool LineSegment::intersectsSegment(const LineSegment& s) const {
	// one must be positive and the other negative
	const static auto alternate = [] (float a, float b) -> bool { return (a >= 0.f && b < 0.f) || (a < 0.f && b >= 0.f); };

	// each point has to be on different sides of the line
	float dotA1 = mNormal.dot(s.mP0 - mP0);
	float dotA2 = mNormal.dot(s.mP1 - mP0);
	bool alternateA = alternate(dotA1, dotA2);

	float dotB1 = s.mNormal.dot(mP0 - s.mP0);
	float dotB2 = s.mNormal.dot(mP1 - s.mP0);
	bool alternateB = alternate(dotB1, dotB2);

	return alternateA && alternateB;
}

Vector2 LineSegment::intersectionPoint(const LineSegment& s) const {
	float a1 = mA;
	float b1 = mB;

	float a2 = s.mA;
	float b2 = s.mB;

	// A vertical line cannot be converted to y = ax + b
	// Vertical lines have a = +/-infinite and b is either NaN or +/-infinite
	float x, y;
	if (std::isinf(a1)) {
		// this segment is vertical, so we get the X from the first point
		x = mP0.x();
		// calculate y using the equation of the other segment
		y = a2 * x + b2;
	} else if (std::isinf(a2)) {
		x = s.mP0.x();
		y = a1 * x + b1;
	} else {
		// Lines are not vertical
		// Now both line equations should be equal
		// a1*x + b1 = a2*x + b2
		x = (b2 - b1) / (a1 - a2);
		y = a1 * x + b1;
	}
	return Vector2(x, y);
}

void LineSegment::intersectionWith(const LineSegment& s, std::vector<Vector2>& points) const {
	bool isParallel = mV.isParallelTo(s.mV);
	if (isParallel) {
		if (isOnTheLine(s.mP0)) {
			points.push_back(s.mP0);
		}
		if (isOnTheLine(s.mP1)) {
			points.push_back(s.mP1);
		}
	} else {
		// check if segments intersect
		if (intersectsSegment(s)) {
			Vector2 v = intersectionPoint(s);
			points.push_back(v);
		}
	}
}

/* Checks if two points are on the same side of the line created with this segment */
bool LineSegment::isOnTheSameSide(const Vector2& p1, const Vector2& p2) const {
	Vector2 v1 = p1 - mP0;
	Vector2 v2 = p2 - mP0;
	float dot1 = mNormal.dot(v1);
	float dot2 = mNormal.dot(v2);
	return (dot1 >= 0.f && dot2 >= 0.f) || (dot1 < 0.f && dot2 < 0.f);
}

#endif
