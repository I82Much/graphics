#ifndef __Object_h__
#define __Object_h__

#include "RE167_global.h"

#include "Matrix4.h"
#include "Vector4.h"
#include "VertexData.h"

namespace RE167 {

    class Material;
    

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

        void setVertices(float * vertices, int numVertices);
        void setColors(float * colors, int numVertices);
        void setNormals(float * normals, int numNormals);
        void setIndices(int * indices, int numIndices);

        inline void setMaterial(Material * m) { material = m; }
        inline Material * getMaterial() { return material; }
                
        inline void setSphereCenter(const Vector4 &center) { sphereCenter = center; }
        inline void setSphereRadius(const float radius) { sphereRadius = radius; }
        
        inline Vector4 getSphereCenter() { return sphereCenter; }
        inline float getSphereRadius() { return sphereRadius; }
        
    	
		Object() : mTransformation(Matrix4::IDENTITY), material(NULL) {} 
    protected:
		Matrix4 mTransformation;
        
		friend class SceneManager;

	private:
	    Material * material;
        
        // Hold bounding sphere information
        Vector4 sphereCenter;
        float sphereRadius;
	};

}

#endif