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
        
        ~BezierCurve();
        
        
        Vector3 position(float t);
        Vector3 tangent(float t);
        
	private:

        int numCubicSegments;
        std::vector <Vector3> controlPoints;
        
        static const Matrix4 WEIGHTS;
        
        static void test();
        
        void createMatrices();
        int calculateIndex(float t);
        std::vector <Matrix4> matrices;
	};


}



#endif /* end of include guard: BEZIERCURVE_H_C38IIXGN */
