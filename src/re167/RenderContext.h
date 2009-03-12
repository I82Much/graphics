#ifndef __RenderContext_h__
#define __RenderContext_h__

#include "RE167_global.h"
#include "Singleton.h"
#include "Camera.h"
#include "Matrix4.h"
#include "Object.h"
#include <qwidget>

namespace RE167 {

	/** This is an abstract class serving as the interface to the low-level
		API. Currently, only an OpenGL subclass is implemented. You could 
		also add a DirectX class. You will add a software renderer in a 
		later project assignment.
	*/
	class RE167_EXPORT RenderContext : public Singleton<RenderContext>
	{
	public:
		virtual void init() = 0;
		virtual void setViewport(int width, int height) = 0;
		virtual void beginFrame() = 0;
		virtual void endFrame() = 0;
		virtual void setModelViewMatrix(const Matrix4 &m) = 0;
		virtual void setProjectionMatrix(const Matrix4 &m) = 0;
		virtual void render(Object *object) = 0;
	};

}

#endif