#include "SceneManager.h"
#include "RenderContext.h"
#include "scenegraph/TransformGroup.h"
#include "scenegraph/LightNode.h"


using namespace RE167;


const unsigned int SceneManager::MAX_NUM_LIGHTS;

SceneManager::SceneManager()
	: mCamera(0), root(new TransformGroup())
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
	if (root) 
	{
        delete root; 
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



/**
* @param node           the node to be added to light nodes, or if a group,
*                       the nodes whose children will be recursively added
* @param lightNodes     the list holding all of the LightNodes found in
*                       graph
* @param transform      
**/
void SceneManager::addLightNodes(Node * node, 
                                std::list<LightNode *> &lightNodes, 
                                const Matrix4 &transform) 
{
    LightNode * ln = dynamic_cast<LightNode*>(node);
    // Current node is a light node
    if ( ln != NULL ) {
        // Update the transformation
        ln->setTransformation(transform);
        lightNodes.push_back( ln );
        return;
    }

    // Check to see if this node has children; if so recursively call this
    // method on each child
    TransformGroup * g = dynamic_cast<TransformGroup *>(node);
    if ( g != NULL ) {
        // TODO: is it left or right multiply?
        const Matrix4 &updatedTransform = g->getTransformation() * transform;
        std::list<Node *>::iterator iter;
        for (iter=g->children.begin(); iter!=g->children.end(); iter++)
        	addLightNodes((*iter), lightNodes, updatedTransform);
    }
    // All other cases (Shape3D, CameraNode) we do nothing.
}



void SceneManager::renderScene()
{
	RenderContext* renderContext = RenderContext::getSingletonPtr();

	if(mCamera!=0) 
	{
	    // Set up the lights
        //renderContext->setLights(getLightsFromGraph(root));
        std::list<LightNode *> lights;
        addLightNodes(dynamic_cast<Node *>(root), lights, root->getTransformation());
        
        // TODO: Need to convert the LightNodes into Lights.
        
        renderContext->setLights(mLightList);

        renderContext->beginFrame();

        renderContext->setProjectionMatrix(mCamera->getProjectionMatrix());
        Matrix4 v = mCamera->getViewMatrix();

        // Traverse the scene graph
        root->draw(v, renderContext);
        
		renderContext->endFrame();
	}
}