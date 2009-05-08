#ifndef CIRCLE_H_A4MI90N6
#define CIRCLE_H_A4MI90N6

#include "../RE167_global.h"
#include "Spline.h"

namespace RE167 {
    class Vector3;

    class RE167_EXPORT Circle : public Spline
    {
    public:
        
        Vector3 position(float t) const;
        Vector3 tangent(float t) const;
        Vector3 acceleration(float t) const;
    };
}

#endif /* end of include guard: CIRCLE_H_A4MI90N6 */

