
#include <string>
#include <vector>
#include "Vector2.h"
#include "LineSegment.h"
#include "Shape.h"


void printPoints(const std::vector<Vector2>& points) {
	std::for_each(points.begin(), points.end(), [](Vector2 v) {
		v.print();
	});
}

void check(const std::string& name, const std::vector<Vector2>& t1, const std::vector<Vector2>& t2, const std::vector<Vector2>& expected) {
	const Shape shape1(t1);
	const Shape shape2(t2);
	const Shape out = shape1 - shape2;

	std::vector<Vector2> points;
	out.getPoints(points);

	if (points.size() != expected.size())
		Log::error("[%s] size should be %d, but size = %d", name.c_str(), expected.size(), points.size());
	else {
		for (int i = 0; i < points.size(); i++) {
			bool isEqual = points[i] == expected[i];
			if (!isEqual) {
				Log::error("[%s] points don't match", name.c_str());
				printPoints(points);
				return;
			}
		}
		Log::info("[%s] Success", name.c_str());
	}
}

void test1() {
	// two triangles, small intersection, no points inside
	std::vector<Vector2> t1 = { {0,0}, {1,0}, {0,1} };
	std::vector<Vector2> t2 = { {0,0}, {1,0}, {1,1} };
	std::vector<Vector2> expected = {{.5f,.5f},{0,0},{1,0}};
	check("test1", t1, t2, expected);
}

void test2() {
	// two triangles, lines overlap, but no common region
	std::vector<Vector2> t1 = { {0,0}, {1,0}, {0,1} };
	std::vector<Vector2> t2 = { {0,0}, {1,0}, {0,-1} };
	std::vector<Vector2> expected; // empty
	check("test2", t1, t2, expected);
}

void test3() {
	// two triangles, square region overlap, no points inside
	std::vector<Vector2> t1 = { {0,0}, {1,0}, {0,1} };
	std::vector<Vector2> t2 = { {0.5f, -0.5f}, {0.5f, 0.5f}, {-0.5f, 0.5f} };
	std::vector<Vector2> expected = {{0,.5f},{0,0},{.5f,0},{.5f,.5f}};
	check("test3", t1, t2, expected);
}

void test4() {
	// two triangles, one is upside down (star with six points), no points inside
	std::vector<Vector2> t1 = { {-2,-2}, {2,-2}, {0,2} };
	std::vector<Vector2> t2 = { {-2,1}, {0,-3}, {2,1} };
	std::vector<Vector2> expected = {{1.25f,-.5f},{.5f,1},{-.5f,1},{-1.25f,-.5f},{-.5f,-2},{.5f,-2}};
	check("test4", t1, t2, expected);
}

void test5() {
	// second triangle is inside the first one
	std::vector<Vector2> t1 = { {0,0}, {10,0}, {0,10} };
	std::vector<Vector2> t2 = { {1,1}, {2,1}, {1,5} };
	std::vector<Vector2> expected = {{1,1},{2,1},{1,5}};
	check("test5", t1, t2, expected);
}

void test6() {
	// first triangle is inside the second one
	std::vector<Vector2> t1 = { {0,0}, {0,1}, {1,0} };
	std::vector<Vector2> t2 = { {-1,-1}, {2,-1}, {0,2} };
	std::vector<Vector2> expected = {{0,0},{0,1},{1,0}};
	check("test6", t1, t2, expected);
}

void test7() {
	// two rectangles, square intersection, no points inside
	std::vector<Vector2> t1 = { {0,2}, {5,2}, {5,3}, {0,3} }; // horizontal rect
	std::vector<Vector2> t2 = { {3,0}, {4,0}, {4,4}, {3,4} }; // vertical rect
	std::vector<Vector2> expected = {{4,2},{4,3},{3,3},{3,2}};
	check("test7", t1, t2, expected);
}

void test8() {
	// one rect, one triangle, no points inside
	std::vector<Vector2> t1 = { {-1,0}, {1,0}, {0,3} };
	std::vector<Vector2> t2 = { {-2,1}, {2,1}, {2,2}, {-2,2} };
	std::vector<Vector2> expected = {{-.333333f,2},{-.666667f,1},{.666667f,1},{.333333f,2}};
	check("test8", t1, t2, expected);
}

void test9() {
	// two rects, intersection is a triangle
	std::vector<Vector2> t1 = { {0,2}, {1,1}, {2,2}, {1,3} }; // horizontal rect
	std::vector<Vector2> t2 = { {1,2}, {3,2}, {3,4}, {1,4} }; // vertical rect
	std::vector<Vector2> expected = {{1,2},{2,2},{1,3}};
	check("test9", t1, t2, expected);
}

void test10() {
	// two triangles that share a small segment, but no intersection
	std::vector<Vector2> t1 = { {0,2}, {2,0}, {2,4} };
	std::vector<Vector2> t2 = { {2,1}, {3,2}, {2,3} };
	std::vector<Vector2> expected;
	check("test10", t1, t2, expected);
}

void test11() {
	// two triangles that share a small segment, second one is inside the first one
	std::vector<Vector2> t1 = { {0,2}, {2,0}, {2,4} };
	std::vector<Vector2> t2 = { {2,1}, {2,3}, {1,2} };
	std::vector<Vector2> expected = { {2,1}, {2,3}, {1,2} };
	check("test11", t1, t2, expected);
}

int main() {
	Log::info("started");
	test1();
	test2();
	test3();
	test4();
	test5();
	test6();
	test7();
	test8();
	test9();
	test10();
	test11();
	Log::info("finished");

	return 0;
}
