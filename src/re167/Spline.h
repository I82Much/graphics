#ifndef SPLINE_H_4K2S3LB4
#define SPLINE_H_4K2S3LB4

#include "RE167_global.h"
#include <vector>

namespace RE167 {
    class Vector3;
    
    class RE167_EXPORT Spline
    {
    public:
        
        virtual std::vector <Vector3> uniformPointSample(int numStraightSegments) const = 0;
        virtual std::vector <Vector3> uniformTangentSample(int numStraightSegments) const = 0;
        virtual std::vector <Vector3> uniformAccelerationSample(int numStraightSegments) const = 0;
        
        virtual Vector3 position(float t) const = 0;
        virtual Vector3 tangent(float t) const = 0;
        virtual Vector3 acceleration(float t) const = 0;

        //inline Matrix4 getTransformation() { return transformation; }
    };
}



#endif /* end of include guard: SPLINE_H_4K2S3LB4 */
