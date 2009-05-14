#ifndef PIECEWISESPLINE_H_U1BNHRH4
#define PIECEWISESPLINE_H_U1BNHRH4

#include "../RE167_global.h"
#include "Spline.h"
#include <vector>

/**
* This class represents a spline that is itself made up of other splines.
* For instance, we can combine a helix with a bezier curve in one seamless
* curve, assuming that the curves are specified correctly.
**/

namespace RE167 {
    class Vector3;

    class RE167_EXPORT PiecewiseSpline : public Spline
    {
    public:
        PiecewiseSpline(Spline * first);
        ~PiecewiseSpline();
        
        Vector3 position(float t) const;
        Vector3 tangent(float t) const;
        Vector3 acceleration(float t) const;
        
        bool addSpline(Spline * n);
        bool removeSpline(Spline * n); 
        inline int numSplines() { return pieces.size(); }
        
        static void test();
		
		void adaptiveSample(int numSamples, std::vector<Vector3>& position,
							std::vector<Vector3>& tangent, std::vector<Vector3>& acceleration) const;
		std::vector<float> getTValues(int numSamples) const { std::vector<float> toReturn; return toReturn;}
		void recursivelyDivide (float maxStepSize, Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3,
								float scaleTValuesBy, float startingTValue, int level,
								std::vector<float>& tValues) const {}
		bool flatEnough(Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3, float flatness) const {return true;}
		
        
    private:
        std::vector<Spline *> pieces;
        
        void calculateCurve(const float t, int &curveIndex, float &tPrime) const;
    };
}

#endif /* end of include guard: PIECEWISESPLINE_H_U1BNHRH4 */
