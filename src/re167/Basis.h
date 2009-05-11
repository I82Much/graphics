#ifndef __Basis_h__
#define __Basis_h__

#include "RE167_global.h"
#include "Vector3.h"
#include "Matrix4.h"

namespace RE167 {

	class RE167_EXPORT Basis 
	{
	public:
        Basis(const Vector3 &u_, const Vector3 &v_, const Vector3 &w_, const Vector3 &o_);
        
        ~Basis();
        
        inline const Vector3 getOrigin() const { return origin; }
        inline const Vector3 getU() const { return u; }
        inline const Vector3 getV() const { return v; }
        inline const Vector3 getW() const { return w; }
        inline const Matrix4 getTransformation() const { return transformation; }

	private:
        Matrix4 transformation;
        Vector3 u;
        Vector3 v;
        Vector3 w;
        Vector3 origin;
	};
}
#endif /* end of include guard: Basis_CPP_G5MC9S8S */
