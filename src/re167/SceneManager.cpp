#include "SceneManager.h"
#include "RenderContext.h"


using namespace RE167;


const unsigned int SceneManager::MAX_NUM_LIGHTS;

SceneManager::SceneManager()
	: mCamera(0)
{
}

SceneManager::~SceneManager()
{
	if(mCamera)
	{
		delete mCamera;
	}
	// Free space of all objects
	while(mObjectList.size() > 0)
	{
		Object *o = mObjectList.front();
	    mObjectList.pop_front();
		delete o;
	}
	// Free space of all lights
	while (mLightList.size() > 0) 
	{
        Light *l = mLightList.front();
        mLightList.pop_front();
        delete l;
	} 
}

Object* SceneManager::createObject()
{	
	Object *o = new Object();
	mObjectList.push_back(o);

	return o;
}

/**
* Returns whether or not an additional light can be added.
*/
bool SceneManager::canCreateLight() 
{
    return mLightList.size() <= MAX_NUM_LIGHTS;
}

/**
* Creates a light, adds it to internal data structure, and returns it.  If there
* are too many lights (at least MAX_NUM_LIGHTS), prints error message and returns
* NULL.
*/
Light* SceneManager::createLight()
{
    if (!canCreateLight()) {
        std::cerr << "Error: Attempted to assign too many lights. " <<
                    "Ignoring request." << std::endl;
        return NULL;
    }
    
    Light *l = new Light();
    mLightList.push_back(l);
    return l;
}

Camera* SceneManager::createCamera()
{
    mCamera = new Camera();
	return mCamera;
}




void SceneManager::renderScene()
{
	RenderContext* renderContext = RenderContext::getSingletonPtr();

	if(mCamera!=0) 
	{
	    // Set up the lights
        renderContext->setLights(mLightList);
	    
		renderContext->beginFrame();

		renderContext->setProjectionMatrix(mCamera->getProjectionMatrix());
		Matrix4 v = mCamera->getViewMatrix();

		// Iterate through list of objects
		std::list<Object *>::const_iterator iter;
		for (iter=mObjectList.begin(); iter!=mObjectList.end(); iter++)
		{
			Object *o = (*iter);
			Matrix4 m = o->getTransformation();

			renderContext->setModelViewMatrix(v*m);
			renderContext->render(o);
		}

		renderContext->endFrame();
	}
}