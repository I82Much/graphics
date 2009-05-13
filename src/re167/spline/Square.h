#ifndef SQUARE_H_UMAJ89SH
#define SQUARE_H_UMAJ89SH

#include "../RE167_global.h"
#include "Spline.h"
#include "../BasicMath.h"

namespace RE167 {
    class Vector3;

    class RE167_EXPORT Square : public Spline
    {
    public:
        
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
		
    private:
        static const Vector3 UPPER_LEFT;
        static const Vector3 UPPER_RIGHT;
        static const Vector3 LOWER_RIGHT;
        static const Vector3 LOWER_LEFT;
        
        static const Vector3 UPPER_TANGENT;
        static const Vector3 RIGHT_TANGENT;
        static const Vector3 LOWER_TANGENT;
        static const Vector3 LEFT_TANGENT;
        
        static const int TOP;
        static const int RIGHT;
        static const int BOTTOM;
        static const int LEFT;
        
        void translateT(float t, float &tPrime, int &index) const;
    };
}



#endif /* end of include guard: SQUARE_H_UMAJ89SH */
