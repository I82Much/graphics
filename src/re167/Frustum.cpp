#include "Frustum.h"
#include <math.h>

using namespace RE167;

/*
void Frustum::setNearPlane(	const float _near) {
	near = _near;
	updateProjectionMatrix();
}
void Frustum::setFarPlane(	const float _far) {
	far = _far;
	updateProjectionMatrix();
}
void Frustum::setAspectRatio(const float ratio) {
	aspectRatio = ratio;
	updateProjectionMatrix();
}
void Frustum::setVerticalFOV(const float fov) {
	vertFOV = fov;
	updateProjectionMatrix();
}
*/

/**
* Updates the projection matrix to account for
* one or more changes to the near clip plane,
* the far clip plane, the aspect ratio, or
* the vertical field of view (FOV).
* @param near
* @param far
* @param aspect
* @param vertFOV	angular extent vertically (radians)
*/
void Frustum::setFrustum(const float near, const float far, 
    const float aspect, const float vertFOV)
 {
     
	this->near = near;
    this->far = far;
    this->aspectRatio = aspect;
    this->vertFOV = vertFOV;
	
	float p00 = 1.0f / (aspect * tan(vertFOV / 2.0f));
	float p11 = 1.0f / (tan(vertFOV / 2));
	// We have to negate the z row due to the difference in convention with
	// OpenGL
	float p22 = -(far + near) / (far - near);
	float p23 = -(2 * far * near)/(far - near);
	float p32 = -1.0f;

	this->p = Matrix4(	p00,	0,		0,	0,
						0,		p11,	0,	0,
						0,		0,		p22,p23,
						0,		0,		p32,	0);
}

