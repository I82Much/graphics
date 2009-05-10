#ifndef __GLRenderContext_h__
#define __GLRenderContext_h__

#include "glew.h"
#include <QtOpenGL>
#include "RE167_global.h"
#include "RenderContext.h"



// TODO: Fix the wireframe thing.  Would be cool to add :)
namespace RE167 {
    // Forward declarations
    class Light;
    class Material;
    class LightNode;
    
	/** This class implements the abstract base class RenderContext and
		it is the main interface to the low-level graphics OpenGL API.
	@remarks
		This class also instantiates the Singleton class to make sure there is
		only one instance at any time.
	*/
		
	class RE167_EXPORT GLRenderContext : public RenderContext
	{
	public:
	    
	    virtual ~GLRenderContext() {}
		void init();
		void toggleWireframe();
		void setViewport(int width, int height);
		/** This function needs to be called by the SceneManager once per 
    		frame, before all objects are rendered. The function assumes 
    		your light sources are stored in a std::list.

    		NOTE: You also need to add this declaration to the base class 
    		RenderContext.h so that the SceneManager can call it.
    	*/
    	void setLights(const std::list<Light*> &lightList);
    	
    	void setLightNodes(const std::list<LightNode*> &lightList);
        
    	
		void beginFrame();
		void endFrame();
		void setModelViewMatrix(const Matrix4 &m);
		void setProjectionMatrix(const Matrix4 &m);
		/** This is the main method for rendering objects. Note that it uses
			OpenGL vertex arrays. See the OpenGL 2.0 book for details.
		*/
		void render(Object *object);
		

		static GLRenderContext& getSingleton(void);
        static GLRenderContext* getSingletonPtr(void);

	private:
	    /** This function needs to be called before rendering each object.
    	*/
    	void setMaterial(const Material *material);
		bool wireframe;
		QTime *clock;
        QTime *globalClock;
	};

}

#endif