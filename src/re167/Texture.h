#ifndef __Texture_h__
#define __Texture_h__

#include "RE167_global.h"
#include <qimage>

namespace RE167 {

	class RE167_EXPORT Texture
	{
	public:
		/** Initialize a texture with an image stored in a QImage object.
			You can load an image from a file into a QImage object using 
			code like 
			QImage *texImg = new QImage("your_texture.PNG", "PNG");
		*/
		Texture(QImage *texImg);
		/** Get a handle to the texture. This is used by GLRenderContext
			to pass the texture to OpenGL. See GLRenderContext::setMaterial.
		*/
		int getId() const;

	private:
		int mId;

		int* getData(QImage *tex);
	};
}

#endif