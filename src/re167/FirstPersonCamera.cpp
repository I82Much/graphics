#include "FirstPersonCamera.h"
#include "BasicMath.h"

using namespace RE167;

const float FirstPersonCamera::MAX_PITCH(179.0f);
const float FirstPersonCamera::MIN_PITCH(-179.0f);


FirstPersonCamera::FirstPersonCamera(const Vector3 &p, 
                                    const Vector3 &lookAt, 
                                    const Vector3 &up) : 
                                    Camera(p, lookAt, up), yaw(0), pitch(0) 
{
    updatePosition();
}

    
        
FirstPersonCamera::~FirstPersonCamera() 
{
    
}

/**
* @param yaw - the number of degrees around the y axis to rotate
*/
void FirstPersonCamera::setYaw(float yaw) 
{

    this->yaw = yaw;
    updatePosition();
}

/**
*@param pitch - the number of degrees around the x axis to rotate.
* Capped to be between -179 and 179 so that we can't look all the way up
* or down
*/
void FirstPersonCamera::setPitch(float pitch) 
{
    this->pitch = BasicMath::clamp(pitch, MIN_PITCH, MAX_PITCH);
    updatePosition();
}


void FirstPersonCamera::updatePosition()
{
    // Without having moved, we look down z axis
    Vector3 lookAtPoint(getLookAtPoint());
    
    Vector4 p(lookAtPoint.getX(), lookAtPoint.getY(), lookAtPoint.getZ(), 1);
    
    Matrix4 pitchRot = Matrix4::rotateX(pitch);
    Matrix4 yawRot = Matrix4::rotateY(yaw);
    
    std::cout << "lookAtPoint: " << lookAtPoint << std::endl;
    
    std::cout<< "pitch: " << pitch << " yaw: " << yaw << std::endl;

    std::cout << pitchRot * (yawRot * p) << std::endl;;
    
    
    p = pitchRot * (yawRot * p);
    
    
    std::cout << "lookAtPoint after translation: " << p << std::endl;
    
    
    // Strip off the w coordinate
    Vector3 point(p.getX(), p.getY(), p.getZ());
    
    
    
    changeSettings(getCenterOfProjection(), point, getUpVector());
}
