#ifndef MORPHER_H_E52RF6A8
#define MORPHER_H_E52RF6A8

#include "RE167_global.h"
#include "Matrix4.h"
#include "Spline.h"

namespace RE167 {
    class Vector3;
    
    class RE167_EXPORT Morpher : public Spline
    {
    public:
        Morpher(Spline * first, Spline * second);
        ~Morpher();

        
        Vector3 position(float t) const;
        Vector3 tangent(float t) const;
        Vector3 acceleration(float t) const;

        
    protected:
        Spline * first;
        Spline * second;
    };
}


#endif /* end of include guard: MORPHER_H_E52RF6A8 */
