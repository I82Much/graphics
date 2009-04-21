#include "Vector4.h"
#include "Vector3.h"
#include <assert.h>
#include <stdlib.h>
#include "BasicMath.h"
#include <cmath>

using namespace RE167;

// Default constructor{} creates 0 vector
Vector4::Vector4(void) : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}

/* Creates a vector with given x, y, z magnitudes.s
*/
Vector4::Vector4(float x_, float y_, float z_, float w_) :
	x(x_), y(y_), z(z_), w(w_) {}

// We have no allocated memory for the object, so nothing to free or
// delete
Vector4::~Vector4() {}

const Vector4 Vector4::ZERO_VECTOR(0, 0, 0, 0);
const Vector4 Vector4::ORIGIN(0, 0, 0, 1);


/**
* Constructor that makes a Vector4 from a Vector3
*/
Vector4::Vector4(const Vector3 &val) : x(val.getX()), y(val.getY()), z(val.getZ()), w(0) {}



// Addition operator for vectors
Vector4 & Vector4::operator+=(const Vector4 &rhs) {
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	w += rhs.w;
	return *this;
}

/**
* Calculates the angle in radians between a and b.
*/
float Vector4::angleBetween(const Vector4 &a, const Vector4 &b) {
	float dotProd = a.dotProduct(b);
	float aLength = a.magnitude();
	float bLength = b.magnitude();

	// They've passed in the zero vector.  
	if (BasicMath::approxEqual(aLength, 0.0f) ||
		BasicMath::approxEqual(bLength, 0.0f)) {
		return 0.0f;
	}
	else {
		return acos( dotProd / (aLength * bLength) );
	}
}

/**
* @see http://www.cs.caltech.edu/courses/cs11/material/cpp/donnie/cpp-ops.html
* Modified from the + example there
*/
const Vector4 Vector4::operator+(const Vector4 &rhs) const{
	// Make a copy
	Vector4 result = *this;
	result += rhs;
	return result;
}

// Subtraction operator for vectors that 
Vector4 & Vector4::operator-=(const Vector4 &rhs) {
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	w -= rhs.w;
	return *this;
}

/**
* 
* @see http://www.cs.caltech.edu/courses/cs11/material/cpp/donnie/cpp-ops.html
* Modified from the + example there
*/ 
const Vector4 Vector4::operator-(const Vector4 &rhs) const {
	// Make a copy
	Vector4 result = *this;
	result -= rhs;
	return result;
}
	


// Multiplication operator for scalar and vector
Vector4 & Vector4::operator*=(const float scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;
	return *this;
}


/**
* Two vectors are the same if they have the same x, y, z, and w magnitude.
* Note that since we store the values as floats, this allows for some
* error in rounding.
*/
const bool Vector4::operator==(const Vector4& other) const {
	return	BasicMath::approxEqual( x, other.x ) &&
			BasicMath::approxEqual( y, other.y ) &&
			BasicMath::approxEqual( z, other.z ) &&
			BasicMath::approxEqual( w, other.w );
}


const bool Vector4::operator!=(const Vector4& other) const {
	return !((*this) == other);
}


const bool Vector4::operator<(const Vector4& other) const {

    // Order by x, then by y, then by z, then w
    if (x != other.x) {
      return (x < other.x);
    }
    else if (y != other.y) {
      return (y < other.y);
    }
    else if (z != other.z) {
      return (z < other.z);
    }
    else {
      return w < other.w;
    }
}



/**
* @return the dot product between two vectors.  The dot product between
* vectors a and b is defined as ||a|| ||b|| cos theta, where ||a|| is
* the magnitude of a, and theta is the angle between the two vectors.
*/
const float Vector4::dotProduct(const Vector4 &rhs) const {
	return (x * rhs.x) + (y * rhs.y) + (z * rhs.z) + (w * rhs.w);
}


/**
* @return the length (magnitude) of the vector.
*/
const float Vector4::magnitude() const{ 
	return sqrt( (x * x) + (y * y) + (z * z) + (w * w) );
}



/* @return a vector of unit length that points in the same
 * direction as this vector.
 * @precondition the vector is not the zero vector
 */
const Vector4 Vector4::normalize() {
	float length = magnitude();
	assert(length != 0.0f);
	return Vector4( x / length,
					y / length,
					z / length,
					w / length );
}

/**
* Runs a test suite to ensure that all methods are working properly.
* If they are, then this method returns EXIT_SUCCESS as defined in
* stdlib.h.  Else if one fails, returns EXIT_FAILURE.
*/
int Vector4::runTestSuite() {
	std::cout<< "Testing Vector4 functionality." <<std::endl;

	const float X = 1.0f;
	const float Y = 2.0f;
	const float Z = 3.0f;
	const float W = 1.0f;

	std::cout<< "Testing constructor: " << std::endl;
	// Test that the constructor works as desired
	Vector4 vec(X, Y, Z, W);
	bool valuesMatch =	vec.x == X &&
						vec.y == Y &&
						vec.z == Z &&
						vec.w == W;
	assert(valuesMatch);

	std::cout<< "Testing default constructor: " << std::endl;
	Vector4 zeroVector;
	bool allZeroes =	zeroVector.getX() == 0 &&
						zeroVector.getY() == 0 &&
						zeroVector.getZ() == 0 &&
						zeroVector.getW() == 0;
	assert(allZeroes);

	const float X2 = 2.0f;
	const float Y2 = 3.0f;
	const float Z2 = 4.0f;
	const float W2 = 0.0f;

	Vector4 addend(X2, Y2, Z2, W2);

	Vector4 sum = addend + vec;

	Vector4 correctSum(X + X2, Y + Y2, Z + Z2, W + W2);
	
	std::cout<< "Testing vector addition: " << std::endl;
	assert(sum == correctSum);
	assert(correctSum == sum);

	std::cout<< "Testing vector subtraction: " << std::endl;
	// a+(-b) = a - b
	Vector4 a(5,10,15,0);
	Vector4 b(2,-10,-234,0);
	Vector4 aPlusMinusB = a + (-b);
	Vector4 aMinusB = a - b;
	assert(aPlusMinusB == aMinusB);
	
	// Test scalar multiplication
	std::cout<< "Testing scalar vector multiplication: " << std::endl;
	
	Vector4 c(5,-10,0,0);
	Vector4 d = c * -5;
	Vector4 e = c * 14.3;
	Vector4 f = c * 0;
	assert (d.magnitude() == c.magnitude() * 5);
	assert (e.magnitude() == c.magnitude() * 14.3f);
	assert (f.magnitude() == 0.0);
	
	

	std::cout<< "Testing dot product for orthogonal vectors: " << std::endl;
	// Check dot product for two known orthogonal vectors; should be zero
	Vector4 xVector(1,0,0,0);
	Vector4 yVector(0,1,0,0);
	float dotProduct = xVector.dotProduct(yVector);
	assert(dotProduct == 0);

	std::cout<< "Testing magnitude: " << std::endl;
	// Make sure magnitude code works
	float zeroLength = zeroVector.magnitude();
	assert(zeroLength == 0.0f);

	float xVectorLength = xVector.magnitude();
	assert(xVectorLength == 1.0f);
	
	Vector4 toMeasure(sqrt(5.0f), -sqrt(3.0f), 2, 0);
	assert(toMeasure.magnitude() == sqrt(12.0f));
	
	return EXIT_SUCCESS;
}


