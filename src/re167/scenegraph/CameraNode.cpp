#include "CameraNode.h"

using namespace RE167;



CameraNode::CameraNode (Camera* aCamera) : camera(aCamera) {
	centerOfProj = camera->getCenterOfProjection();
	lookAtPoint = camera->getLookAtPoint();
	lookUpVector = camera->getUpVector();
	rotation = 0;
	valueWasChanged = false;
	transform = Matrix4::IDENTITY;
}

// Do nothing.
void CameraNode::draw(const Matrix4 &t, RenderContext * context, Camera * Camera, bool cull) {}

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
	rotation = rotate;
	valueWasChanged = true;
	updateProjection();
}

void CameraNode::updateProjection () {
	if (valueWasChanged) {
		
		Vector3 newCenter = Vector3(transform*Vector4(centerOfProj));
		
		Matrix4 rotationMatrix = Matrix4::rotate(Vector4(lookUpVector), rotation);
		Vector3 newLookAt = Vector3(transform*rotationMatrix*Vector4(lookAtPoint));
		
		Vector3 newLookUp = Vector3(transform*Vector4(lookUpVector));
		
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