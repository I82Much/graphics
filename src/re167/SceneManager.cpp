#include "SceneManager.h"
#include "RenderContext.h"
#include "scenegraph/TransformGroup.h"
#include "scenegraph/LightNode.h"
#include "scenegraph/CameraNode.h"


using namespace RE167;


const unsigned int SceneManager::MAX_NUM_LIGHTS;

SceneManager::SceneManager()
	: mCamera(0), root(new TransformGroup()), objectLevelCulling(true)
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


/**
* A recursive method that finds the camera within the scene graph, keeps
* track of the transformations above the camera in the tree, and returns
* the transformed CameraNode.
* @return a pointer to the CameraNode, or NULL if no camera exists in the
* subtree rooted at root.
**/
CameraNode * SceneManager::findCamera(Node * root, const Matrix4 &transform) {
    // Defensively check against null pointer
    if (root == NULL) {
        return NULL;
    }
    
    CameraNode * camera = dynamic_cast<CameraNode*>(root);
    if (camera != NULL) {
        camera->setTransformation(transform);
        return camera;
    }
    
    // The current node is not a camera.  If it's a leaf of any other type,
    // return NULL; we didn't find it.
    if (dynamic_cast<Leaf*>(root) != NULL) {
        return NULL;
    }
    
    TransformGroup * group = dynamic_cast<TransformGroup*>(root);
    // We have a transform group; recursively call this method on each of
    // its children
    if (group != NULL) {
        const Matrix4 updatedTransform = group->getTransformation() * transform;

        std::list<Node *>::iterator iter;
        for (iter=group->children.begin(); iter!=group->children.end(); iter++){
            Node * n = findCamera(*iter, updatedTransform);
            if (n != NULL) {
                // We only ever return anything that's NOT null from this method
                // if it's a CameraNode, so this is guaranteed to work.
                return dynamic_cast<CameraNode*>(n);
            }
        }
        // None of the children were CameraNodes
        return NULL;
    }
    
    // Should never get here
    assert(false && "Reached unreachable portion of findCamera()");
    return NULL;
}


void SceneManager::renderScene()
{
	RenderContext* renderContext = RenderContext::getSingletonPtr();


    // Find the camera node in the scene graph; if there are more than one
    // just pick the first one
    CameraNode * cameraNode = findCamera(root, Matrix4::IDENTITY);
    
    assert (cameraNode != NULL);
    
    Camera * camera = cameraNode->getCamera();

//    if (mCamera !=0)
    if(camera!=0) 
	{
	    // Set up the lights
        std::list<LightNode *> lightNodes;
        addLightNodes(root, lightNodes, Matrix4::IDENTITY);
        
        //renderContext->setLights(mLightList);
        renderContext->setLightNodes(lightNodes);


        renderContext->beginFrame();

        renderContext->setProjectionMatrix(mCamera->getProjectionMatrix());
        //Matrix4 v = mCamera->getViewMatrix();

        Matrix4 v = cameraNode->getTransformation() * camera->getViewMatrix();



        // Traverse the scene graph
        // TODO: the mCamera thing could be replaced later on with the CameraNode
        root->draw(v, renderContext, mCamera, objectLevelCulling);
        
		renderContext->endFrame();
	}
}