#ifndef BEZIERCURVE_H_C38IIXGN
#define BEZIERCURVE_H_C38IIXGN
#include "RE167_global.h"
#include <vector>

namespace RE167
{
    class Vector3;
    class Matrix4;
    /**
    * Class represents a two-dimensional piecewise Bezier curve in the xy plane
    **/
	class RE167_EXPORT BezierCurve
	{
	public:
        BezierCurve();
        BezierCurve(const Vector3 &p0, 
                    const Vector3 &p1,
                    const Vector3 &p2,
                    const Vector3 &p3);

        BezierCurve(Vector3 points[], int numPoints);


        ~BezierCurve();
        
        // TODO: there is no symmetry between point and position.
        std::vector <Vector3> uniformPointSample(int numStraightSegments) const;
        std::vector <Vector3> uniformTangentSample(int numStraightSegments) const;
        
        
        Vector3 position(float t) const;
        
        Vector3 tangent(float t) const;

        static void test();
        
	private:

        int numCubicSegments;
        std::vector <Vector3> controlPoints;
        
        static const Matrix4 WEIGHTS;
        
        
        void createMatrices();
        static int calculateIndex(float t);
        std::vector <Matrix4> matrices;
	};


}



#endif /* end of include guard: BEZIERCURVE_H_C38IIXGN */
