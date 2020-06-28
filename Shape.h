#ifndef SHAPE_H_
#define SHAPE_H_

#include <vector>
#include <list>
#include "Vector2.h"
#include "LineSegment.h"
#include "Log.h"

class Shape {
private:
	Vector2 mCenter;
	std::vector<LineSegment> mSegments;

	/* List of points that describe a region in counterclockwise direction */
	class Region {
	private:
		/* use a std::list because we will insert points in random positions */
		std::list<Vector2> mList;
	public:
		void add(const Vector2& p) {
			// ignore duplicates
			if (std::find(mList.begin(), mList.end(), p) == mList.end()) {
				// the point must follow the counterclockwise flow
				// so we have to navigate through the list and find the right spot for this point
				if (mList.size() < 2)
					mList.push_back(p);
				else {
					// we have at least two points,
					// so let's make a vector V with every two consecutive points (vA and vB)
					// V = vB - vA
					// and check if point P is on the left side of this vector (counterclockwise direction)
					// Let's make vP = P - vA
					// and let N = CCW normal of V
					// The dot product of vP with N has to be positive (less than 90 degrees)
					bool inserted = false;
					for (auto pos = mList.cbegin(); pos != mList.cend(); ++pos) {
						auto nextPos = std::next(pos, 1);
						nextPos = nextPos == mList.cend()? mList.begin() : nextPos;
						const Vector2& vA = *pos;
						const Vector2& vB = *nextPos;
						Vector2 V = vB - vA;
						Vector2 vP = p - vA;
						bool isOnTheLeft = V.pointsToTheLeft(vP);
						// if the point is NOT on the left side of V,
						// then we have to add it to the nextIndex position
						if (!isOnTheLeft) {
							mList.insert(nextPos, p);
							inserted = true;
							break;
						}
					}
					// If the point was not inserted yet, then it belongs to the end of the list
					if (!inserted) {
						mList.push_back(p);
					}
				}
			}
		}

		void addAll(std::vector<Vector2>& points) {
			for (const Vector2& p : points)
				add(p);
		}

		void getPoints(std::vector<Vector2>& v) const {
			v.reserve(mList.size());
			for (const Vector2& p : mList)
				v.push_back(p);
		}
	};


public:
	explicit Shape(const std::vector<Vector2>& points):
		mCenter(0.f, 0.f)
	{
		unsigned long size = points.size();
		for (int i = 0; i < size; i++) {
			unsigned long nextIndex = (i + 1) % size;
			Vector2 p1 = points[i];
			Vector2 p2 = points[nextIndex];
			LineSegment s(p1, p2);
			mSegments.push_back(s);
			mCenter += p1;
		}
		mCenter /= mSegments.size();
	}

	~Shape() {
		mSegments.clear();
	}

	bool isInside(const Vector2& point) const;
	Shape operator-(const Shape& shape) const;
	void getPoints(std::vector<Vector2>& points) const;
};

void Shape::getPoints(std::vector<Vector2>& points) const {
	points.reserve(mSegments.size());
	for (const LineSegment& s : mSegments) {
		points.push_back(s.p0());
	}
}

bool Shape::isInside(const Vector2& point) const {
	// check that 'point' is on the same side as the center for all segments
	for (const LineSegment& s : mSegments) {
		if (!s.isOnTheLine(point) && !s.isOnTheSameSide(point, mCenter)) {
			return false;
		}
	}
	return true;
}

Shape Shape::operator-(const Shape& shape) const {
	Region region;
	for (const LineSegment& s1 : mSegments) {
		for (const LineSegment& s2 : shape.mSegments) {
			std::vector<Vector2> localPoints;
			localPoints.reserve(5);
			s1.intersectionWith(s2, localPoints);
			if (localPoints.size() == 1) {
				const Vector2& s1p1 = s1.p1();
				const Vector2& s2p1 = s2.p1();
				if (shape.isInside(s1p1)) {
					localPoints.push_back(s1p1);
				} else if (isInside(s2p1)) {
					localPoints.push_back(s2p1);
				}
			}
			region.addAll(localPoints);
		}
	}
	std::vector<Vector2> points;
	region.getPoints(points);

	// if the intersection is empty, then one shape could be inside the other
	if (points.size() == 0) {
		// check if this shape is inside the 'shape'(param)
		if (shape.isInside(mSegments[0].p0())) {
			getPoints(points);
		} else if (isInside(shape.mSegments[0].p0())) {
			shape.getPoints(points);
		}
	}

	// we need at least three points to make a region.
	// if this is not the case, then there is no intersection
	if (points.size() < 3) {
		points.clear();
	}
	return Shape(points);
}

#endif /* SHAPE_H_ */
