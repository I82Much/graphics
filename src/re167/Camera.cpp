#include "Camera.h"
#include <cmath>
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



/**
* Given the definition of a sphere, determines if the sphere is completely
* outside of the clipping planes
* @param center the center of the sphere, in *MODEL SPACE* coordinates
* @param radius the radius of the sphere
**/
Camera::ClipStatus Camera::getSphereClipStatus(const Vector4 &center, const float radius) 
{
    

    // For each plane defining view volume of camera
        // Determine if sphere is completely above 
    return COMPLETELY_INSIDE;
    
    
}

/**
* Calculates the 6 planes defining our view volume.  
**/

void Camera::calculatePlanes() 
{
    float near = getNearPlane();
    float far = getFarPlane();
    float aspectRatio = getAspectRatio();
    float vertFOV = getVerticalFOV();
    
    // tan (fov/2) = top/near
    float top = near * tan(vertFOV/2);
    
    // aspect ratio = (right-left) / (top - bottom) = right / top 
    // (since right = -left, top = -bottom)
    float right = aspectRatio * top;
    
    float bottom = -top;
    float left = -right;
    
    // Now we have the coordinates of our frustum.  
    
    // Note that the center of projection of our camera, in camera coordinates,
    // is (0,0,0).  All the sides of the frustum other than the near plane
    // and far plane converge to this point.
    

    // Near and far planes are easy; we know how far they are from origin 
    // (near and far, respectively) and we know their normals - they're orthogonal
    // to camera
    this->near = Plane();
    this->far = Plane();
    
    // We want all the normals to point towards the outside of the frustum
    // (meaning we need to give the points in counter clockwise order)


    this->left = Plane();
    this->right = Plane();
    this->top = Plane();
    this->bottom = Plane();
    
    
    
  
}

