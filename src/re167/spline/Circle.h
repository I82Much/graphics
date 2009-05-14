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
		
		void adaptiveSample(int numSamples, std::vector<Vector3>& position,
							std::vector<Vector3>& tangent, std::vector<Vector3>& acceleration) const;
		std::vector<float> getTValues(int numSamples) const { std::vector<float> toReturn; return toReturn;}
		void recursivelyDivide (float maxStepSize, Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3,
								float scaleTValuesBy, float startingTValue, int level,
								std::vector<float>& tValues) const {}
		bool flatEnough(Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3, float flatness) const {return true;}
		
    };
}

#endif /* end of include guard: CIRCLE_H_A4MI90N6 */

