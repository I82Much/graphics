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

		void adaptiveSample(int numSamples, std::vector<Vector3>& position,
							std::vector<Vector3>& tangent, std::vector<Vector3>& acceleration) const {}
		std::vector<float> getTValues(int numSamples) const {}
		void recursivelyDivide (float maxStepSize, Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3,
								float scaleTValuesBy, float startingTValue, int level,
								std::vector<float>& tValues) const {}
		bool flatEnough(Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3, float flatness) const {}
		
		
    private:
        float numTurns;
        
        Spline * scaleCurve;
    };
}

#endif /* end of include guard: HELIX_H_QYIH3BHR */
