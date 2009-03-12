#ifndef __GLRenderContext_h__
#define __GLRenderContext_h__

#include "glew.h"
#include <QtOpenGL>

#include "RE167_global.h"
#include "RenderContext.h"
#include "VertexDeclaration.h"

namespace RE167 {

	/** This class implements the abstract base class RenderContext and
		it is the main interface to the low-level graphics OpenGL API.
	@remarks
		This class also instantiates the Singleton class to make sure there is
		only one instance at any time.
	*/
		
	class RE167_EXPORT GLRenderContext : public RenderContext
	{
	public:
		void init();
		void toggleWireframe();
		void setViewport(int width, int height);
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
		bool wireframe;
	};

}

#endif