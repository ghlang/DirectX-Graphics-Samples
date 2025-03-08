#ifndef GEOM_OBJECTS
#define GEOM_OBJECTS
#include "DXSample.h"
#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <DirectXMath.h> // Include DirectXMath for XMFLOAT4
#include <cmath>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>

namespace bg = boost::geometry;
using Point2D = bg::model::point<float, 2, bg::cs::cartesian>;
using Segment = bg::model::segment<Point2D>;

struct Vertex2D {
	double x, y;

	Vertex2D() : x(0), y(0) {}
	Vertex2D(float x, float y) : x(x), y(y) {}

	void set(float newX, float newY) {
		x = newX;
		y = newY;
	}

	void get(float& outX, float& outY) const {
		outX = x;
		outY = y;
	}

	// add an operator to return the values as an XMFLOAT2 object
	operator DirectX::XMFLOAT2() const {
		return DirectX::XMFLOAT2(static_cast<float>(x), static_cast<float>(y));
	}
	// add an operator to return the values as a Point2D object	
	operator Point2D() const {
		return Point2D(static_cast<float>(x), static_cast<float>(y));
	}



};




// define a strucuture called Vertex3D to hold 3d coordinates and methods to get and set them
struct Vertex3D {
	double x, y, z;

	Vertex3D() : x(0), y(0), z(0) {}
	Vertex3D(float x, float y, float z) : x(x), y(y), z(z) {}

	void set(float newX, float newY, float newZ) {
		x = newX;
		y = newY;
		z = newZ;
	}

	void get(float& outX, float& outY, float& outZ) const {
		outX = x;
		outY = y;
		outZ = z;
	}

	// add an operator to return the values as an XMFLOAT3 object
	operator DirectX::XMFLOAT3() const {
		return DirectX::XMFLOAT3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
	}

	// add an operator to return the values as a Vertex2D object
	operator Vertex2D() const {
		return Vertex2D(static_cast<float>(x), static_cast<float>(y));
	}

	// add an operator to return the values as a Point2D object
	operator Point2D() const {
		return Point2D(static_cast<float>(x), static_cast<float>(y));
	}
	// add an = operator
	Vertex3D& operator=(const Vertex3D& other) {
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}
	// add a + operator
	Vertex3D operator+(const Vertex3D& other) const {
		return Vertex3D(x + other.x, y + other.y, z + other.z);
	}
	// add a - operator
	Vertex3D operator-(const Vertex3D& other) const {
		return Vertex3D(x - other.x, y - other.y, z - other.z);
	}
	// add a * operator for a scalar
	Vertex3D operator*(double scalar) const {
		return Vertex3D(x * scalar, y * scalar, z * scalar);
	}
	// add a * operator for another Vertex3D object
	Vertex3D operator*(const Vertex3D& other) const {
		return Vertex3D(x * other.x, y * other.y, z * other.z);
	}
	
	// add a / operator for a scalar
	Vertex3D operator/(double scalar) const {
		return Vertex3D(x / scalar, y / scalar, z / scalar);
	}
	// add a / operator for another Vertex3D object
	Vertex3D operator/(const Vertex3D& other) const {
		return Vertex3D(x / other.x, y / other.y, z / other.z);
	}
	// add a += operator
	Vertex3D& operator+=(const Vertex3D& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}
	// add a -= operator
	Vertex3D& operator-=(const Vertex3D& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}
	// add a *= operator for a scalar
	Vertex3D& operator*=(double scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}
	// add a *= operator for another Vertex3D object
	Vertex3D& operator*=(const Vertex3D& other) {
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}
	// add a /= operator for a scalar
	Vertex3D& operator/=(double scalar) {
		// return if scalar is zero
		if (std::fabs(scalar) < std::numeric_limits<double>::epsilon()) return *this;
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}
	// add a /= operator for another Vertex3D object
	Vertex3D& operator/=(const Vertex3D& other) {
		// only divide if the other object is not zero, check for each component
		x = (std::fabs(other.x) > std::numeric_limits<double>::epsilon()) ? x / other.x : x;
		y = (std::fabs(other.y) > std::numeric_limits<double>::epsilon()) ? y / other.y : y;
		z = (std::fabs(other.z) > std::numeric_limits<double>::epsilon()) ? z / other.z : z;
		return *this;
	}


};

#endif // !GEOM_OBJECTS

