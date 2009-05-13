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

		void adaptiveSample(int numSamples, std::vector<Vector3>& position,
							std::vector<Vector3>& tangent, std::vector<Vector3>& acceleration) const {}
		std::vector<float> getTValues(int numSamples) const {}
		void recursivelyDivide (float maxStepSize, Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3,
								float scaleTValuesBy, float startingTValue, int level,
								std::vector<float>& tValues) const {}
		bool flatEnough(Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3, float flatness) const {}
        
    protected:
        Spline * first;
        Spline * second;
    };
}


#endif /* end of include guard: MORPHER_H_E52RF6A8 */
