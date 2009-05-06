#ifndef __SceneManager_h__
#define __SceneManager_h__

#include "RE167_global.h"
#include "Camera.h"
#include "Object.h"
#include "RenderContext.h"
#include <list>
#include "Light.h"
#include "FirstPersonCamera.h"
#include "scenegraph/Node.h"


namespace RE167 {

    class TransformGroup;
    class LightNode;
    class CameraNode;

	/** This class provides an abstraction of a scene. It manages a camera,
		objects in the scene, etc. It is the main interface for applications
		to the rendering engine.
	*/
	class RE167_EXPORT SceneManager
	{
	public:
		SceneManager();
		~SceneManager();

        TransformGroup * getRoot() { return root; }
        void setRoot(TransformGroup * root) { this->root = root; }
        
		/** This method creates a default camera.
		*/
		Camera *createCamera();

		/** This method creates an object and adds it to the list of object
			stored in the scene manager.
		*/
		Object *createObject();

        bool canCreateLight();
		
		/**
		* This method creates a light and adds it to the list of light objects
		* stored in the scene manager
		*/
        Light *createLight();

		/** This method needs to be called in the renderSceneEvent
			event handler of the RenderWidget. 
		@remarks
			You should *not* call this anywhere else in your code except in 
			the renderScene event handler of your RenderWidget. To trigger 
			rendering, call updateScene of your RenderWidget, which will then 
			trigger the renderScene event handler to be called. 
		*/
		void renderScene();
		
        inline void setObjectLevelCulling(bool b) { objectLevelCulling = b; }
        inline bool getObjectLevelCulling() { return objectLevelCulling; }
        

	private:
		Camera *mCamera;
		std::list<Object*> mObjectList;
        std::list<Light*> mLightList;
        
        TransformGroup * root;
        bool objectLevelCulling;
        
        CameraNode * findCamera(Node * root, const Matrix4 &transform);
        
        void addLightNodes(Node * node, 
            std::list<LightNode *> &lightNodes, 
            const Matrix4 &transform);
        
                
        static const unsigned int MAX_NUM_LIGHTS = 8;
	};

}

#endifß