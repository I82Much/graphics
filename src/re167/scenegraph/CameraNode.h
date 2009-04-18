#ifndef CAMERANODE_H_M9Z4FZOF
#define CAMERANODE_H_M9Z4FZOF

#include "Leaf.h"


namespace RE167
{
    class Matrix4;
    class RenderContext;
    class Camera;
	class RE167_EXPORT CameraNode : public Leaf
	{
    public:
        
        CameraNode(Camera * aCamera) : camera(aCamera) {}
        // Nothing allocated.
        ~CameraNode() {}
        
        inline void setCamera(Camera * aCamera) { camera = aCamera; }
        inline Camera * getCamera() { return camera; }
        
        virtual void draw(const Matrix4 &t, RenderContext * context, Camera * Camera);
        
        static void test();
        
    protected:
        Camera * camera;
    };
}

#endif /* end of include guard: CAMERANODE_H_M9Z4FZOF */
