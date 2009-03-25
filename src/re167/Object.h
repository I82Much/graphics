#ifndef __Object_h__
#define __Object_h__

#include "RE167_global.h"
#include "VertexData.h"
#include "Matrix4.h"
#include "Material.h"

namespace RE167 {

	/** This class provides an abstraction of scene objects.
	@remarks
		The SceneManager creates and deletes objects. Vertex data is stored
		in the VertexData member.
	*/
	class RE167_EXPORT Object
	{
	public:
		inline void resetTransformation() { mTransformation = Matrix4::IDENTITY; }
		inline void setTransformation(const Matrix4 &t) { mTransformation = t; }
		inline Matrix4 getTransformation() const { return mTransformation; }
		VertexData vertexData;

        inline void setMaterial(Material * m) { material = m; }
        inline Material * getMaterial() { return material; }
        
	protected:
		Object() { mTransformation = Matrix4::IDENTITY; }
		Matrix4 mTransformation;
        
		friend class SceneManager;

	private:
	    Material * material;
        
	};

}

#endif