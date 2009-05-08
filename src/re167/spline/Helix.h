#ifndef HELIX_H_QYIH3BHR
#define HELIX_H_QYIH3BHR

#include "../RE167_global.h"
#include "Spline.h"

namespace RE167 {
    class Vector3;

    class RE167_EXPORT Helix : public Spline
    {
    public:
        Helix(float numTurns_ = 1.0); 
        ~Helix();
        
        Vector3 position(float t) const;
        Vector3 tangent(float t) const;
        Vector3 acceleration(float t) const;
        
//        inline void setScaleCurve(const )
        
    private:
        float numTurns;
        
        Spline * scaleCurve;
    };
}

#endif /* end of include guard: HELIX_H_QYIH3BHR */
