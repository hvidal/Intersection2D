compile() {
	g++ -std=c++1y -pedantic -Wall -stdlib=libc++ $1.cpp -o $1
}
compile Intersection2D

