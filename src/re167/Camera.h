#ifndef __Camera_h__
#define __Camera_h__

#include "RE167_global.h"
#include "Frustum.h"
#include "Matrix4.h"
#include "Vector3.h"

namespace RE167 {

	/** Camera specification.
	@remarks
		This class specifies a camera. The default camera is located at
		(0,0,10) in world coordinates and looks towards (0,0,0).
	*/
	class RE167_EXPORT Camera : public Frustum
	{
	public:
	    enum ClipStatus {
	        COMPLETELY_INSIDE,
	        COMPLETELY_OUTSIDE,
	        OVERLAPPING
        };
	    
	    
		Camera() : v(1,0,0,0, 0,1,0,0, 0,0,1,-10, 0,0,0,1) {};

		Camera(const Vector3 &center, 
		        const Vector3 &lookAt, 
		        const Vector3 &up) : 
			centerOfProjection(center), lookAtPoint(lookAt), upVector(up) {
			updateViewMatrix();
		};
		const Matrix4 &getViewMatrix() const { return v; }

		const Vector3 &getCenterOfProjection() const { return centerOfProjection; }
		const Vector3 &getLookAtPoint() const { return lookAtPoint; }
		const Vector3 &getUpVector() const { return upVector; }
		
		void changeSettings(const Vector3 &center, const Vector3 &point, const Vector3 &up);

		/*
		void setCenterOfProjection(const Vector3 &center);
		void setLookAtPoint(const Vector3 &point);
		void setUpVector(const Vector3 &up);
		*/
		void setViewMatrix(const Matrix4 &matrix);
		void resetViewMatrix();

		void updateVectors();

        ClipStatus getSphereClipStatus(const Vector4 &center, const float radius); 

	private:
		Matrix4 v;
		Vector3 centerOfProjection;
		Vector3 lookAtPoint;
		Vector3 upVector;
		
		void updateViewMatrix();
	};

}

#endif