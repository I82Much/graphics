#ifndef SPLINE_H_4K2S3LB4
#define SPLINE_H_4K2S3LB4

#include "RE167_global.h"
#include <vector>
#include "Matrix4.h"

namespace RE167 {
    class Vector3;
    
    class RE167_EXPORT Spline
    {
    public:
        Spline();
        virtual ~Spline() {}
        
        virtual std::vector <Vector3> uniformPointSample(int numPoints) const;
        virtual std::vector <Vector3> uniformTangentSample(int numPoints) const;
        virtual std::vector <Vector3> uniformAccelerationSample(int numPoints) const;        
        
        virtual Vector3 position(float t) const = 0;
        virtual Vector3 tangent(float t) const = 0;
        virtual Vector3 acceleration(float t) const = 0;

        virtual inline Matrix4 getTransformation() { return transformation; }
        virtual void setTransformation(const Matrix4 &t) { transformation = t; }
        
        
    protected:
        Matrix4 transformation;
    };
}



#endif /* end of include guard: SPLINE_H_4K2S3LB4 */
