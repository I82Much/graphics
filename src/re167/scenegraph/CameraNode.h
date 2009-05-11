#ifndef CAMERANODE_H_M9Z4FZOF
#define CAMERANODE_H_M9Z4FZOF

#include "Leaf.h"
#include "Matrix4.h"

namespace RE167
{
    class Matrix4;
    class RenderContext;
    class Camera;
	class RE167_EXPORT CameraNode : public Leaf
	{
    public:
        
        CameraNode(Camera * aCamera);
        // Nothing allocated.
        ~CameraNode() {}
        
        inline void setCamera(Camera * aCamera) { camera = aCamera; }
        inline Camera * getCamera() { return camera; }
        
        virtual void draw(const Matrix4 &t, RenderContext * context, Camera * Camera, bool cull);
        
        inline void resetTransformation() { transform = Matrix4::IDENTITY; }
        inline void setTransformation(const Matrix4 &t) { transform = t; valueWasChanged = true; updateProjection();}
        inline Matrix4 getTransformation() const { return transform; }
        
        static void test();
		
		const Vector3 &getCenterOfProjection() const { return centerOfProj; }
		const Vector3 &getLookAtPoint() const { return lookAtPoint; }
		const Vector3 &getLookUpVector() const { return lookUpVector; }
		
		
		void setCenter (Vector3 center);
		void setLookAt (Vector3 lookAt);
		void setLookUp (Vector3 lookUp);
		
		void setRotation (float rotate);
		
		void updateProjection();
		void updateProjection(Vector3 center, Vector3 lookAt, Vector3 lookUp);
		
    protected:
        Camera * camera;
        Matrix4 transform;
		
		// stored to create world to camera matrix - assumed that rotation and transform have not been applied
		Vector3 centerOfProj;
		Vector3 lookAtPoint;
		Vector3 lookUpVector;
		
		// the amount that lookAtPoint should be rotated by around lookUpVector
		float rotation;
		
		// if true, then the projection matrix for the camera must be recalculated
		bool valueWasChanged;
		
		
    };
}

#endif /* end of include guard: CAMERANODE_H_M9Z4FZOF */
