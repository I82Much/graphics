#ifndef SPLINE_H_4K2S3LB4
#define SPLINE_H_4K2S3LB4

#include "RE167_global.h"
#include <vector>
#include "Matrix4.h"


namespace RE167 {
    class Vector3;
    class Basis;
    
    class RE167_EXPORT Spline
    {
    public:
        Spline();
        virtual ~Spline() {}
        
        virtual std::vector <Vector3> uniformPointSample(int numPoints) const;
        virtual std::vector <Vector3> uniformTangentSample(int numPoints) const;
        virtual std::vector <Vector3> uniformAccelerationSample(int numPoints) const;        
        
        virtual Vector3 position(float t) const = 0;
        virtual Vector3 tangent(float t) const = 0;
        virtual Vector3 acceleration(float t) const = 0;

        virtual inline Matrix4 getTransformation() { return transformation; }
        virtual void setTransformation(const Matrix4 &t) { transformation = t; }
 
        std::vector <Basis> getReferenceFrames(int numSamples, bool useAdaptive) const;
        
		
		// implements adaptive sampling of the Spline to get position, tangent, and acceleration
		// note: couldn't figure out how to do it except in the BezierCurve case so that is the only subclass where
		// these virtual functions are implemented
		virtual void adaptiveSample(int numSamples, std::vector<Vector3>& position,
									std::vector<Vector3>& tangent, std::vector<Vector3>& acceleration) const = 0;
		virtual std::vector<float> getTValues(int numSamples) const = 0;
		virtual void recursivelyDivide (float maxStepSize, Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3,
										float scaleTValuesBy, float startingTValue, int level,
										std::vector<float>& tValues) const = 0;
        virtual bool flatEnough(Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3, float flatness) const = 0;
		
        
    protected:
        Matrix4 transformation;
    private:
       const Basis createInitialReferenceFrame(const Vector3 &origin, const Vector3 &tangent, const Vector3 &acceleration) const; 
       
    };
}
#endif /* end of include guard: SPLINE_H_4K2S3LB4 */
