#include "Spline.h"
#include "../Vector3.h"
#include "../Basis.h"

/**
* A spline represents any ``curve'' in 3-space; in other words a function
* of some parameter t that returns a position in space.
* The curves also have support for calculating the tangent vector to the
* curve, as well as the acceleration vector (the first and second derivative of
* the position function, respectively)
* @author Nicholas Dunn
**/

using namespace RE167;
using std::vector;


Spline::Spline() : transformation(Matrix4::IDENTITY) {}

vector <Vector3> Spline::uniformPointSample(int numPoints) const
{
    assert (numPoints >= 2);
    vector<Vector3> points;
    for (int i = 0; i < numPoints; i++) {
        // 0 <= t <= numCubicSegments
        float t =   static_cast<float>(i) / 
                    static_cast<float>(numPoints - 1);
        Vector3 point = position(t);
        points.push_back(point);
    }
    return points;
}
vector <Vector3> Spline::uniformTangentSample(int numPoints) const
{
    assert (numPoints >= 2);
    vector<Vector3> points;
    for (int i = 0; i < numPoints; i++) {
        // 0 <= t <= numCubicSegments
        float t =   static_cast<float>(i) / 
                    static_cast<float>(numPoints - 1);
        Vector3 point = tangent(t);
        points.push_back(point);
    }
    return points;
}
vector <Vector3> Spline::uniformAccelerationSample(int numPoints) const
{
    assert (numPoints >= 2);
    vector<Vector3> points;
    for (int i = 0; i < numPoints; i++) {
        // 0 <= t <= numCubicSegments
        float t =   static_cast<float>(i) / 
                    static_cast<float>(numPoints - 1);
        Vector3 point = acceleration(t);
        points.push_back(point);
    }
    return points;
}

/**
* Calculates <code>numSamples</code> reference frames along the curve. In 
* other words, a local coordinate system at uniformly sampled points along
* the spline, with associated tangent vector, normal vector, and binormal
* vector.
* 
* Each Basis will be set up in the following way:
* Origin    point on path
* u         principalNormal (unit length)
* v         tangent (unit length)
* w         binormal (unit length)
*
*
* Note that there is not a unique local coordinate system at each point along
* the curve; rather this is chosen to minimize rotation/torsion in adjacent
* reference frames.
*
* Makes heavy use of the paper "Calculation of Reference Frames along a Space
* Curve" by Jules Bloomenthal, published in Graphics Gems, vol.1.
*
* These reference frames are used in two different ways.  
* Firstly, they can be used to extract one spline along another spline's path;
* see GeometryFactory::createLoft for this usage.
*
* The second usage is to allow a camera to follow a path; the camera can simply
* use the tangent vector as a lookat vector, and then use the normal vector as
* the Up vector.
*
* @param numSamples how many different places along the spline to sample (and hence 
* how many reference frames to calculate) 
**/
vector <Basis> Spline::getReferenceFrames(int numSamples) const
{
    assert(numSamples >= 1);
    
    vector <Basis> referenceFrames;
    
    vector<Vector3> points =        uniformPointSample(numSamples);
    vector<Vector3> tangents =      uniformTangentSample(numSamples);
    vector<Vector3> accelerations = uniformAccelerationSample(numSamples);
    
    
    const Basis initialBasis = 
        createInitialReferenceFrame(points[0], tangents[0], accelerations[0]);
    referenceFrames.push_back(initialBasis);    
        
    // While there is a method of analytically calculating a reference frame
    // at each point along a curve, (Frenet frame), as Bloomenthal writes
    // "it is undefined wherever the curvature vanishes, such as at points of 
    // inflection or along straight sections of curve.  Worse the curvature
    // vector can reverse direciton on either side of an inflection point, 
    // inflicting a violent twist in ap rogression of Frenet frames."
    //    
    // As a result, it is better to calculate the local rotations and 
    // translations needed to move an initial reference frame along the
    // curve
    
    Basis lastReferenceFrame = initialBasis;
    
    // We already have our initial frame
    for (int i = 1; i < numSamples; i++) 
    {
        Vector3 newTangent = tangents[i].normalize();
        // We created the coordinate system such that the V vector points
        // in direction of tangent to curve
        Vector3 oldNormal = lastReferenceFrame.getU();
        Vector3 oldTangent = lastReferenceFrame.getV();
        Vector3 oldBinormal = lastReferenceFrame.getW();
        
        Matrix4 rotationMatrix;
        // Both tangents are in the same direction; no rotation necessary
        if (newTangent == oldTangent) {
            rotationMatrix = Matrix4::IDENTITY;
        }
        // We need to do some rotation in order to line the old axes up with
        // the new axes
        else {
            Vector3 axis = oldTangent.crossProduct(newTangent).normalize();
            Vector4 axisOfRotationVec4 = Vector4::homogeneousVector(axis);

            float angle = Vector3::angleBetween(oldTangent, newTangent);
            rotationMatrix = Matrix4::rotate(axisOfRotationVec4, angle);
        }
        
        // Rotate the old reference frame such that the old tangent vector
        // is aligned with the new tangent vector
        // Multiply the old tangent vector and old bitangent vectors by rotation
        // matrix to calculate the transformed ones.

        Vector4 newNormalVec4 = rotationMatrix * Vector4(oldNormal.getX(), oldNormal.getY(), oldNormal.getZ(), 0);
        Vector4 newBinormalVec4 = rotationMatrix * Vector4(oldBinormal.getX(), oldBinormal.getY(), oldBinormal.getZ(), 0);

        Vector3 newNormal = Vector3(newNormalVec4);
        Vector3 newBinormal = Vector3(newBinormalVec4);
        Vector3 newOrigin = points[i];

        // Create a new coordinate system out of the new tangent, new normal, 
        // and new binormal
        const Basis newReferenceFrame(newNormal, newTangent, newBinormal, newOrigin);
        
        referenceFrames.push_back(newReferenceFrame);
        
        lastReferenceFrame = newReferenceFrame;
    }
    
    assert(referenceFrames.size() == numSamples);
    
    return referenceFrames;
        
}


/**
* @param 
**/
const Basis Spline::createInitialReferenceFrame(const Vector3 &origin, const Vector3 &tangent, const Vector3 &acceleration) const
{
    
    Vector3 unitTangent = tangent.normalize();

    Vector3 principalNormal;
    
    // Straight line segment or acceleration in same direction as tangent
    if (acceleration == Vector3::ZERO_VECTOR || 
        acceleration.crossProduct(unitTangent) == Vector3::ZERO_VECTOR) {
        
        // Pick any unit length vector normal to the tangent vector
        
        // TODO: remove magic number and dehack
        
        Vector3 linearlyIndependentVector = Vector3(unitTangent.getX(), unitTangent.getY(), unitTangent.getZ() + 10).normalize();
        principalNormal = linearlyIndependentVector.crossProduct(unitTangent).normalize();
        
    }
    // The principal normal we can calculate by using the acceleration vector.
    // It's going to be linearly independent, and according to James Bloomenthal
    // in an e-mail correspondence:
    // V and Q are linearly independent, but I'm not sure that fully explains K=VxQxV. From p. 37, "An Introduction to Splines for  use in Computer Graphics and Geometric Modeling", by Bartels, Beatty and Barsky:
    //"
    //  With the use of a bit of differential geometry, it is possible to show (see
    //  Barsky81a, Barsky87a or pp.99-101 of [Faux/Pratt79]) that the vector
    //  VxQ/|V|^3 has magnitude equal to the curvature. However, this vector is
    //  perpendiular to the plane containing the osculating circle (the osculating
    //  plane). An additional cross-product with K/|K| results in a vector of the
    //  same length lying in the osculating plane, which is the curvature vector.
    // "   
    else {
        principalNormal = 
            unitTangent.crossProduct(acceleration).crossProduct(unitTangent).normalize();
    }
    
    Vector3 binormal = unitTangent.crossProduct(principalNormal).normalize();
    
    // We now have a reference frame defined by unitTangent, principalNormal, and biNormal
    Vector3 u = principalNormal;
    Vector3 v = unitTangent;
    Vector3 w = binormal;

    Basis basis(u,v,w,origin);

    return basis;
}