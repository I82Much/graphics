#include "Matrix4.h"
#include "BasicMath.h"
//#include <math.h>
#include <assert.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <cmath>

/**
* This class represents a 4x4 matrix, which are used to implement
* affine transformations in graphics.
* @modified Nicholas Dunn
* @date   February 21, 2009
*/

using namespace RE167;

const Matrix4 Matrix4::IDENTITY(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1 );

// Matrix multiplied by vector of correct size yields a new vector.
// The result is found by taking the dot products of each row vector
// with the vector being multiplied by.
Vector4 Matrix4::operator*(const Vector4 &vec) const {
	
	Vector4 row1( m[0][0], m[0][1], m[0][2], m[0][3] );
	Vector4 row2( m[1][0], m[1][1], m[1][2], m[1][3] );
	Vector4 row3( m[2][0], m[2][1], m[2][2], m[2][3] );
	Vector4 row4( m[3][0], m[3][1], m[3][2], m[3][3] );

	return Vector4 (	row1.dotProduct(vec),
						row2.dotProduct(vec),
						row3.dotProduct(vec),
						row4.dotProduct(vec) );
}

/**
* Creates a scaling matrix that brings point (1,1,1,1)^T to
* (xScale, yScale, zScale, wScale)^T
*/
Matrix4 Matrix4::scale(float xScale, float yScale, float zScale, 
						float wScale) {
	return Matrix4(	xScale,	0,		0,		0,
					0,		yScale,	0,		0,
					0,		0,		zScale,	0,
					0,		0,		0,		wScale );
					
}

/*
* Creates a 4x4 matrix encompassing a rotation of
* theta radians about the x axis.  Positive values of
* theta rotate counterclockwise
*/
Matrix4 Matrix4::rotateX(float theta) {
	// Store trig functions so you don't recompute twice
	float sinTheta = sin(theta);
	float cosTheta = cos(theta);
	return Matrix4( 
		1,	0,			0,			0,
		0,	cosTheta,	-sinTheta,0,
		0,	sinTheta,	cosTheta,	0,
		0,	0,			0,			1 );
}

/*
* Creates a 4x4 matrix encompassing a rotation of
* theta radians about the y axis.  Positive values of
* theta rotate counterclockwise
*/
Matrix4 Matrix4::rotateY(float theta) {
	// Store trig functions so you don't recompute twice
	float sinTheta = sin(theta);
	float cosTheta = cos(theta);
	return Matrix4(
		cosTheta,	0,	sinTheta,	0,
		0,			1,	0,			0,
		-sinTheta,	0,	cosTheta,	0,
		0,			0,	0,			1 );	
	
}

/*
* Creates a 4x4 matrix encompassing a rotation of
* theta radians about the z axis.  Positive values of
* theta rotate counterclockwise.  This is commented out
* for now because it's not working right.  It is less
* error prone to show matrices in terms of compositions
* of matrices, which is what this version attempts to do.
*/
Matrix4 Matrix4::rotateZ(float theta) {
	// Store trig functions so you don't recompute twice
	float sinTheta = sin(theta);
	float cosTheta = cos(theta);
	return Matrix4(
		cosTheta,	-sinTheta,		0,	0,
		sinTheta,	cosTheta,		0,	0,
		0,			0,				1,	0,
		0,			0,				0,	1 );
}

/*
* Creates a 4x4 matrix encompassing a rotation of
* theta radians about the arbitrary axis provided.
* @see p.148 of Fundamentals of Computer Graphics 2nd edition,
* by Peter Shirley.
* @prerequisite axis is not the zero vector
*/
/*
Matrix4 Matrix4::rotate(const Vector4 &axis, float theta) {
	// "If we wish to rotate about an arbitrary vector a, we can form
	// an orthonormal basis with w = a, rotate that basis to the canonical 
	// basis xyz, rotate about the z-axis, and then rotate the canonical
	// basis back to the uvw basis." p. 148

	// Check for zero vector
	assert(!BasicMath::approxEqual(axis.magnitude(), 0));

	Vector4 w = axis.normalize();

	// To get u and v we need to find a vector t that is not
	// collinear with w.  By changing just one coordinate, we
	// guarantee this to be the case
	Vector4 t = Vector4(w.getX() + 1.0f, w.getY(), w.getZ(), w.getW());
	
	Vector4 u = (t.crossProduct(w)).normalize();

	// Since w and u are already unit length, this vector is too
	Vector4 v = w.crossProduct(u); 

	// u, v, w are now an orthonormal basis
	Matrix4 rotateToXYZ(
		u.getX(),	v.getX(),	w.getX(),	0,
		u.getY(),	v.getY(),	w.getY(),	0,
		u.getZ(),	v.getZ(),	w.getZ(),	0,
		0,			0,			0,			1);

	Matrix4 rotateByZ = Matrix4::rotateX(theta);
	Matrix4 rotateBackToUVW = rotateToXYZ.transpose();

	// Remember: read from right to left: first apply rotation to XYZ, then rotate by z, 
	// then get back to original coordinates
	return rotateBackToUVW * (rotateByZ * rotateToXYZ);
}*/




/*
* Creates a 4x4 matrix encompassing a rotation of
* theta radians about the arbitrary axis provided.
* @prerequisite axis is of unit length
*/

Matrix4 Matrix4::rotate(const Vector4 &axis, float theta) {

	float ax = axis.getX();
	float ay = axis.getY();
	float az = axis.getZ();

	// Store trig functions so you don't recompute multiple times
	float cosTheta = cos(theta);
	float sinTheta = sin(theta);

	// Store all of the squared terms
	float ax2 = ax * ax;
	float ay2 = ay * ay;
	float az2 = az * az;

	// Calculate each element using formula for rotation about
	// arbitrary axis

	// Row 1
	float m00 = ax2 + (cosTheta * (1 - ax2));
	float m01 = (ax * ay * (1 - cosTheta)) - (az * sinTheta);
	float m02 = (ax * az * (1 - cosTheta)) + (ay * sinTheta);
	float m03 = 0;

	// Row 2
	float m10 = (ax * ay * (1 - cosTheta)) + (az * sinTheta);
	float m11 = ay2 + cosTheta * (1 - ay2);
	float m12 = (ay * az * (1 - cosTheta)) - (ax * sinTheta);
	float m13 = 0;

	// Row 3
	float m20 = (ax * az * (1 - cosTheta)) - (ay * sinTheta);
	float m21 = (ay * az * (1 - cosTheta)) + (ax * sinTheta);
	float m22 = az2 + (cosTheta * (1 - az2));
	float m23 = 0;
	
	// Row 4
	float m30 = 0;
	float m31 = 0;
	float m32 = 0;
	float m33 = 1;


	return Matrix4( m00, m01, m02, m03,
					m10, m11, m12, m13,
					m20, m21, m22, m23,
					m30, m31, m32, m33 );
}

/**
* Creates a matrix that will take the point (0,0,0,1)^T to
* (dx, dy, dz, 1)^T.  In other words, translates a point by
* dx units in the x direction, dy units in the y direction,
* and dz units in the z direction
* @param dx the number of units to shift in the x direction
* @param dy the number of units to shift in the y direction
* @param dz the number of units to shift in the z direction
*/
Matrix4 Matrix4::translate(float dx, float dy, float dz) {
	return Matrix4( 1, 0, 0, dx,
					0, 1, 0, dy,
					0, 0, 1, dz,
					0, 0, 0, 1 );
}		



// Equality operator
const bool Matrix4::operator==(const Matrix4 &other) const {
	// Equal if all corresponding elements are equal (or close enough
	// given possible rounding errors)
	for (int i = 0; i < 16; i++) {
		if (!BasicMath::approxEqual(_m[i], other._m[i])) {
			return false;
		}
	}
	return true;
}
			
// Inequality operator
const bool Matrix4::operator!=(const Matrix4 &other) const {
	return !((*this) == other);
}


int Matrix4::runTestSuite() {
	testTranslate();
	testRotateX();
	testRotateY();
	testRotateZ();
	testRotateArbitrary();
	testScale();

	return EXIT_SUCCESS;
}

int Matrix4::testTranslate() {
	// Translating by 0 should give original vector
	return EXIT_SUCCESS;
}

int Matrix4::testRotateX() {
	// Rotating by 0 radians should give identity
	assert(rotateX(0.0f) == Matrix4::IDENTITY);


	std::cout << rotateX(BasicMath::TWO_PI) << std::endl;
	std::cout << Matrix4::IDENTITY << std::endl;
	// Rotating by 2 PI radians should give identity
	assert(rotateX(BasicMath::TWO_PI) == Matrix4::IDENTITY);

	return EXIT_SUCCESS;
}

int Matrix4::testRotateY() {
	// Rotating by 0 radians should give identity
	assert(rotateY(0.0f) == Matrix4::IDENTITY);

	// Rotating by 2 PI radians should give identity
	assert(rotateY(BasicMath::TWO_PI) == Matrix4::IDENTITY);

	return EXIT_SUCCESS;
}

int Matrix4::testRotateZ() {
	// Rotating by 0 radians should give identity
	assert(rotateZ(0.0f) == Matrix4::IDENTITY);

	// Rotating by 2 PI radians should give identity
	assert(rotateZ(BasicMath::TWO_PI) == Matrix4::IDENTITY);

	// Rotating by 90 degrees should take the point (1,0,0,1) to (0,1,0,1)
	Matrix4 rotateNinety = rotateZ(BasicMath::radian(90));
	Vector4 xPoint(1,0,0,1);
	Vector4 expectedResult(0,1,0,1);
	assert(rotateNinety * xPoint == expectedResult);

	// Rotating by 45 should equal rotating by -315
	assert(rotateZ(BasicMath::radian(45.0f)) == rotateZ(BasicMath::radian(-315.0f)));


	return EXIT_SUCCESS;
}

int Matrix4::testRotateArbitrary() {
	Vector4 xAxis = Vector4(1,0,0,0);
	Vector4 yAxis = Vector4(0,1,0,0);
	Vector4 zAxis = Vector4(0,0,1,0);


	float radians[] = {235.11f, -124.f, 0.0f,
					0.5f, 14452.0f, 23.0f, 124.0f};
	int NUM_TESTS = 7;


	// Test at each amount of radians

	for (int i = 0; i < NUM_TESTS; i++) {
		// Test the x axis
		assert(rotate(xAxis, radians[i]) == rotateX(radians[i]));

		// Test the y axis
		assert(rotate(yAxis, radians[i]) == rotateY(radians[i]));

		// Test the z axis
		assert(rotate(zAxis, radians[i]) == rotateZ(radians[i]));
	}

	// Test an arbitrary vector

	
	return EXIT_SUCCESS;

}

int Matrix4::testScale() {
	// 

	return EXIT_SUCCESS;
}



