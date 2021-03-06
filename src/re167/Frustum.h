#ifndef __Frustum_h__
#define __Frustum_h__

#include "RE167_global.h"
#include "Matrix4.h"

namespace RE167 {

	/** This class defines a view frustum.
	@remarks
		The default view frustum has an aspect ratio of one and a field
		of view of about 45 degrees. The near and far clip planes are
		at 1 and 101 respectively.
	*/
	class RE167_EXPORT Frustum
	{
		friend class Camera;

	public:
		Frustum() :	p(2, 0, 0, 0, 0, 2, 0, 0, 0, 0, -1.02, -2.02, 0, 0, -1.f, 0.f) {}
						
		const Matrix4 &getProjectionMatrix() const { return p; }

		void setFrustum(const float near, const float far, 
							const float aspectRatio, const float vertFOV);

		/*const float getNearPlane() 		const { return near; 	}
		const float getFarPlane() 		const { return far; 	}
		const float getAspectRatio() 	const { return aspectRatio; }
		const float getVerticalFOV() 	const { return vertFOV; 	}
		
		void setNearPlane(	const float _near);
		void setFarPlane(	const float _far);
		void setAspectRatio(const float _ratio);
		void setVerticalFOV(const float _fov);
	*/
		
	private:
		Matrix4 p;

		/*float near;
		float far;
		float aspectRatio;
		float vertFOV;*/
	};

}

#endif