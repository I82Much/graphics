#ifndef __SceneManager_h__
#define __SceneManager_h__

#include "RE167_global.h"
#include "Camera.h"
#include "Object.h"
#include "RenderContext.h"
#include <list>
#include "Light.h"
#include "FirstPersonCamera.h"

namespace RE167 {

	/** This class provides an abstraction of a scene. It manages a camera,
		objects in the scene, etc. It is the main interface for applications
		to the rendering engine.
	*/
	class RE167_EXPORT SceneManager
	{
	public:
		SceneManager();
		~SceneManager();

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

	private:
		Camera *mCamera;
		std::list<Object*> mObjectList;
        std::list<Light*> mLightList;
                
        static const unsigned int MAX_NUM_LIGHTS = 8;
	};

}

#endifß