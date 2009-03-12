#include "Camera.h"

using namespace RE167;

/**
* This class represents a camera, through which the
* scene is viewed.
* @modified Nicholas Dunn
* @date   February 21, 2009
*/



/*
void Camera::setCenterOfProjection(const Vector3 &center) {
	centerOfProjection = center;
	updateViewMatrix();
}

void Camera::setLookAtPoint(const Vector3 &point) {
	lookAtPoint = point;
	updateViewMatrix();
}

void Camera::setUpVector(const Vector3 &up) {
	upVector = up;
	updateViewMatrix();
}*/


/**
* Modifies the view matrix to match what the 3 defining vectors
* are.  Called by member functions that change one of the 3
* defining vectors (centerOfProjection, lookAtPoint, upVector)
*/
void Camera::updateViewMatrix() {
	// Create an orthonormal basis such that w points opposite to the
	// gaze direction, v is in same plane as gaze direction and view-up
	// See p. 164, Ch. 7.2.1 in Fundamental of Computer Graphics,
	// Second Edition, by Peter Shirley
	Vector3 w = (centerOfProjection - lookAtPoint).normalize();
	Vector3 u = (upVector.crossProduct(w)).normalize();
	Vector3 v = w.crossProduct(u);
		
	// Create a rotation matrix out of w,u,v 
	Matrix4 rotate = Matrix4( 	u.getX(), u.getY(), u.getZ(), 0,
								v.getX(), v.getY(), v.getZ(), 0,
								w.getX(), w.getY(), w.getZ(), 0,
								0,		  0,		0,		  1    );
	
	// Create the translation matrix to move to origin
	Matrix4 translate = Matrix4::translate(	-centerOfProjection.getX(), 
											-centerOfProjection.getY(),
											-centerOfProjection.getZ() );

	this->v = rotate * translate;
}


void Camera::changeSettings(const Vector3 &center, const Vector3 &point, const Vector3 &up) {
	centerOfProjection = center;
	lookAtPoint = point;
	upVector = up;
	updateViewMatrix();
}


/**
* If a Camera is created without having the 3 vectors specified,
* then this method will update the 3 vectors to match what the
* view matrix calls for
*/
void Camera::updateVectors() {
	
}

void Camera::setViewMatrix(const Matrix4 &matrix) {
	v = matrix;
	//updateVectors();
}

void Camera::resetViewMatrix() {
	updateViewMatrix();
}


