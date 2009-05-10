#ifndef BEZIERCURVE_H_C38IIXGN
#define BEZIERCURVE_H_C38IIXGN
#include "RE167_global.h"
#include <vector>
#include "../Matrix4.h"
#include "Spline.h"

namespace RE167
{
    class Vector3;
    /**
    * Class represents a two-dimensional piecewise Bezier curve in the xy plane
    **/
	class RE167_EXPORT BezierCurve : public Spline
	{
	public:
        BezierCurve(const Vector3 &p0, 
                    const Vector3 &p1,
                    const Vector3 &p2,
                    const Vector3 &p3);

        BezierCurve(Vector3 points[], int numPoints);


        ~BezierCurve();
        
        std::vector <Vector3> uniformPointSample(int numStraightSegments) const;
        std::vector <Vector3> uniformTangentSample(int numStraightSegments) const;
        std::vector <Vector3> uniformAccelerationSample(int numStraightSegments) const;
        
        Vector3 position(float t) const;
        
        Vector3 tangent(float t) const;
        
        Vector3 acceleration(float t) const;

        void setTransformation(const Matrix4 &t) { transformation = t; createMatrices(); }

        static void test();
        
	private:

        int numCubicSegments;
        std::vector <Vector3> controlPoints;
        
        static const Matrix4 WEIGHTS;
        
        
        void createMatrices();
        std::vector <Matrix4> matrices;
        
        
	};


}



#endif /* end of include guard: BEZIERCURVE_H_C38IIXGN */
