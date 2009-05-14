#include "CameraNode.h"

using namespace RE167;



CameraNode::CameraNode (Camera* aCamera) : camera(aCamera) {
	centerOfProj = camera->getCenterOfProjection();
	lookAtPoint = camera->getLookAtPoint();
	lookUpVector = camera->getUpVector();
	rotation = 0;
	valueWasChanged = false;
	isInUse = true;
	transform = Matrix4::IDENTITY;
	isUnaffected = false;
}

// Do nothing.
void CameraNode::draw(const Matrix4 &t, RenderContext * context, Camera * Camera, bool cull) {}


// set the transformation matrix of the camera node
void CameraNode::setTransformation(const Matrix4 &t) {
	transform = t;
	valueWasChanged = true;
	updateProjection();
}

void CameraNode::translateCamera(float dx, float dy, float dz) {
	Matrix4 t = Matrix4::translate(dx, dy, dz);
	Vector3 newCenter = Vector3(t*Vector4(centerOfProj));
	Matrix4 rotationMatrix = Matrix4::rotate(Vector4(lookUpVector), rotation);
	Vector3 newLookAt = Vector3(rotationMatrix*t*Vector4(lookAtPoint));
	rotation = 0;
	updateProjection(newCenter, newLookAt, lookUpVector);
}

// set the values needed for the projection matrix and indicate that they have been changed
void CameraNode::setCenter (Vector3 center) {
	centerOfProj = center;
	valueWasChanged = true;
	updateProjection();
}

void CameraNode::setLookAt (Vector3 lookAt) {
	lookAtPoint = lookAt;
	valueWasChanged = true;
	updateProjection();
}

void CameraNode::setLookUp (Vector3 lookUp) {
	lookUpVector = lookUp;
	valueWasChanged = true;
	updateProjection();
}

void CameraNode::setRotation (float rotate) {
	std::cout << "Rotate = " << rotate << std::endl;
	rotation = BasicMath::clamp(rotate, -BasicMath::PI / 2.0, BasicMath::PI / 2.0);
	std::cout << "      Rotation = " << rotation << std::endl;
	valueWasChanged = true;
	updateProjection();
}

void CameraNode::updateProjection () {
	if (valueWasChanged && !isUnaffected) {
		
		Vector3 newCenter = Vector3(transform*Vector4(centerOfProj));
		
		Matrix4 rotationMatrix = Matrix4::rotate(Vector4(lookUpVector), rotation);
		Vector3 newLookAt = Vector3(rotationMatrix*transform*Vector4(lookAtPoint));
		
		Vector3 newLookUp = Vector3(transform*Vector4(lookUpVector));
		
		camera->changeSettings(newCenter, newLookAt, newLookUp);
		
		valueWasChanged = false;
		
	}
	else if (valueWasChanged && isUnaffected) {
		
		Vector3 newCenter = Vector4(centerOfProj);
		
		Matrix4 rotationMatrix = Matrix4::rotate(Vector4(lookUpVector), rotation);
		Vector3 newLookAt = Vector3(rotationMatrix*Vector4(lookAtPoint));
		
		Vector3 newLookUp = lookUpVector;
		
		camera->changeSettings(newCenter, newLookAt, newLookUp);
		
		valueWasChanged = false;
		
	}
}

void CameraNode::updateProjection (Vector3 center, Vector3 lookAt, Vector3 lookUp) {
	centerOfProj = center;
	lookAtPoint = lookAt;
	lookUpVector = lookUp;
	valueWasChanged = true;
	updateProjection();
}