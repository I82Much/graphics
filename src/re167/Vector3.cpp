#include "Vector3.h"
#include <assert.h>
#include <stdlib.h>
#include "BasicMath.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <map>
#include "Vector4.h"

using namespace RE167;

// Default constructor{} creates 0 vector
Vector3::Vector3(void) : x(0), y(0), z(0) {}

/* Creates a vector with given x, y, z magnitudes.s
*/
Vector3::Vector3(float x_, float y_, float z_) :
	x(x_), y(y_), z(z_) {}

/**
* Convenience method to convert a Vec4 into Vec3 by dropping w coordinate
**/
Vector3::Vector3(const Vector4 &vec4) :
    x(vec4.getX()), y(vec4.getY()), z(vec4.getZ()) {}


// We have no allocated memory for the object, so nothing to free or
// delete
Vector3::~Vector3() {}

const Vector3 Vector3::ZERO_VECTOR(0,0,0);

// Addition operator for vectors
Vector3 & Vector3::operator+=(const Vector3 &rhs) {
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}




/**
* @see http://www.cs.caltech.edu/courses/cs11/material/cpp/donnie/cpp-ops.html
* Modified from the + example there
*/
const Vector3 Vector3::operator+(const Vector3 &rhs) const{
	// Make a copy
	Vector3 result = *this;
	result += rhs;
	return result;
}

// Subtraction operator for vectors that modifies the vector
Vector3 & Vector3::operator-=(const Vector3 &rhs) {
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}

// Multiplication operator for scalar and vector
Vector3 & Vector3::operator*=(const float scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}




/**
* Two vectors are the same if they have the same x, y, and z magnitude.
* Note that since we store the values as floats, this allows for some
* error in rounding.
*/
const bool Vector3::operator==(const Vector3& other) const {
	return	BasicMath::approxEqual(x, other.getX()) &&
			BasicMath::approxEqual(y, other.getY()) &&
			BasicMath::approxEqual(z, other.getZ());
}

const bool Vector3::operator!=(const Vector3& other) const {
	return !((*this) == other);
}

const bool Vector3::operator<(const Vector3& other) const {

//    std::cout << *this << ": " << other << std::endl;


    /*
    if (this->operator==(other)) {
        return false;
    }*/

    // Order by x, then by y, then by z
    if (x < other.x) {
        return true;
    }
    else if (y < other.y) {
        return true;
    }
    else {
        return z < other.z;
    }
}


/**
*
* @see http://www.cs.caltech.edu/courses/cs11/material/cpp/donnie/cpp-ops.html
* Modified from the + example there
*/
const Vector3 Vector3::operator-(const Vector3 &rhs) const {
	// Make a copy
	Vector3 result = *this;
	result -= rhs;
	return result;
}

/**
* @return the dot product between two vectors.  The dot product between
* vectors a and b is defined as ||a|| ||b|| cos theta, where ||a|| is
* the magnitude of a, and theta is the angle between the two vectors.
*/
const float Vector3::dotProduct(const Vector3 &rhs) const {
	return (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
}


/**
* Calculates the angle in radians between a and b.
*/
const float Vector3::angleBetween(const Vector3 &a, const Vector3 &b) {
	float dotProd = a.dotProduct(b);
	float aLength = a.magnitude();
	float bLength = b.magnitude();

	// They've passed in the zero vector.
	if (BasicMath::approxEqual(aLength, 0.0f) ||
		BasicMath::approxEqual(bLength, 0.0f)) {
		return 0.0f;
	}

	// Ensure that we don't take the acos of something we can't handle.
	float quotient = BasicMath::clamp(dotProd / (aLength * bLength), -1.0f, 1.0f);
	return acos( quotient );

}



/**
* @return the vector perpendicular to both vectors, according to the
* right hand rule.  The length of the resulting vector is equal to the
* area of the parallelogram formed by the two vectors in question.
*/
const Vector3 Vector3::crossProduct(const Vector3 &rhs) const {
	float xComponent = (y * rhs.z) - (z * rhs.y);
	float yComponent = (z * rhs.x) - (x * rhs.z);
	float zComponent = (x * rhs.y) - (y * rhs.x);

	return Vector3(xComponent, yComponent, zComponent);
}

/**
* @return the length (magnitude) of the vector.
*/
const float Vector3::magnitude() const{
	return sqrt( (x * x) + (y * y) + (z * z) );
}



/* @return a vector of unit length that points in the same
 * direction as this vector.
 * @precondition the vector is not the zero vector
 */
const Vector3 Vector3::normalize() const {
	float length = magnitude();
	assert(length != 0.0f);
	return Vector3(	x / length,
					y / length,
					z / length);
}


/**
* Runs a test suite to ensure that all methods are working properly.
* If they are, then this method returns EXIT_SUCCESS as defined in
* stdlib.h.  Else if one fails, returns EXIT_FAILURE.
*/
int Vector3::runTestSuite() {


	std::cout<< "Testing Vector3 functionality." <<std::endl;

	const float X = 1.0f;
	const float Y = 2.0f;
	const float Z = 3.0f;

	std::cout<< "Testing constructor: " << std::endl;
	// Test that the constructor works as desired
	Vector3 vec(X, Y, Z);
	bool valuesMatch =	vec.getX() == X &&
						vec.getY() == Y &&
						vec.getZ() == Z;
	assert(valuesMatch);

	std::cout<< "Testing default constructor: " << std::endl;
	Vector3 zeroVector;
	bool allZeroes =	zeroVector.getX() == 0 &&
						zeroVector.getY() == 0 &&
						zeroVector.getZ() == 0;
	assert(allZeroes);

	const float X2 = 2.0f;
	const float Y2 = 3.0f;
	const float Z2 = 4.0f;

	Vector3 addend(X2, Y2, Z2);

	Vector3 sum = addend + vec;

	Vector3 correctSum(X + X2, Y + Y2, Z + Z2);

	std::cout<< "Testing vector addition: " << std::endl;
	assert(sum == correctSum);
	assert(correctSum == sum);

	std::cout<< "Testing vector subtraction: " << std::endl;
	// a+(-b) = a - b
	Vector3 a(5,10,15);
	Vector3 b(2,-10,-234);
	Vector3 aPlusMinusB = a + (-b);
	Vector3 aMinusB = a - b;
	assert(aPlusMinusB == aMinusB);

	// Test scalar multiplication
	std::cout<< "Testing scalar vector multiplication: " << std::endl;

	Vector3 c(5,-10,0);
	assert(BasicMath::approxEqual(c.magnitude(), sqrt(125.0f)));

	Vector3 d = c * -5;
	// Make sure the multiplication didn't change the magnitude
	assert(BasicMath::approxEqual(c.magnitude(), sqrt(125.0f)));

	Vector3 e = c * 14.3f;

	Vector3 realE(5 * 14.3f, -10 * 14.3f, 0 * 14.3f);
	assert(e == realE);


	Vector3 f = c * 0;
	assert (BasicMath::approxEqual(d.magnitude(), c.magnitude() * 5));
	assert (BasicMath::approxEqual(e.magnitude(), c.magnitude() * 14.3f));
	assert (BasicMath::approxEqual(f.magnitude(), 0.0));



	std::cout<< "Testing dot product for orthogonal vectors: " << std::endl;
	// Check dot product for two known orthogonal vectors; should be zero
	Vector3 xVector(1,0,0);
	Vector3 yVector(0,1,0);
	float dotProduct = xVector.dotProduct(yVector);
	assert(dotProduct == 0);

	std::cout<< "Testing magnitude: " << std::endl;
	// Make sure magnitude code works
	float zeroLength = zeroVector.magnitude();
	assert(zeroLength == 0.0f);

	float xVectorLength = xVector.magnitude();
	assert(xVectorLength == 1.0f);

	Vector3 toMeasure(sqrt(5.0f), -sqrt(3.0f), 2);
	assert(BasicMath::approxEqual(toMeasure.magnitude(), sqrt(12.0f)));


	std::cout<< "Testing cross product: " << std::endl;
	// Check cross product!
	// Simple case: x, y vectors.  The cross product should be
	// the z axis (since the cross product of two orthogonal vectors
	// forms the third part of an orthogonal basis)
	Vector3 result = xVector.crossProduct(yVector);
	bool zVector =	result.getX() == 0 &&
					result.getY() == 0 &&
					result.getZ() == 1.0f;
	assert(zVector);

	// Make sure the properties of cross product hold true
	Vector3 aCrossB = vec.crossProduct(addend);
	Vector3 bCrossA = addend.crossProduct(vec);
	assert(aCrossB == -bCrossA);




    Vector3 y(1.54f, 12.42f, 1240.1f);
    Vector3 z(1.540001f, 12.42f, 1240.10002f);


	// Test the map functionality
    std::map<Vector3, int> m;

    std::cout<<m.size() << std::endl;

    /*
    // aCrossB and -bCrossA are equal; the value that should be stored
    // in map is 1923 and map should have only one entry
    m[y] = 1923;
    std::cout << "After adding a x b"<<m.size() << std::endl;
    m[z] = 3105;
    std::cout<< "after adding - b x a" <<m.size() << std::endl;

    assert(y == z);

    
    m[Vector3()] = 4105;
    std::cout<< "after adding Vector3()" <<m.size() << std::endl;

    m[Vector3::ZERO_VECTOR] = 19512;
    std::cout<< "after adding ZERO_VECTOR" <<m.size() << std::endl;

    assert(Vector3() == Vector3::ZERO_VECTOR);


    m[Vector3(-0.25, -0.866026, -0.433012)] = 19523;

    m[Vector3(-0.866026, 5.10342e-12, -0.5)] = 152-14;

    m[Vector3(0, -1, -0)] = 101;

    m[Vector3(-0.251, -0.8660263, -0.4330121)] = 19522;

    m[Vector3(0, -1, -0.00001)] = 102;



//    assert(m.size() == 5);

    std::cout<<m.size() << std::endl;
    
    for( std::map<Vector3, int>::iterator ii=m.begin(); ii!=m.end(); ii++)
    {
           std::cout << (*ii).first << ": " << (*ii).second << std::endl;
    }


    std::map<Vector3,int>::iterator it;
    it=m.find(Vector3::ZERO_VECTOR);
    assert(it != m.end());

    it = m.find(y);
    assert(it != m.end());
*/

	return EXIT_SUCCESS;
}
