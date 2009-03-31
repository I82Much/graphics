#ifndef __FirstPersonCamera_h__
#define __FirstPersonCamera_h__

#include "Camera.h"

namespace RE167 {

	/** Camera specification.
	@remarks
		This class specifies a camera. The default camera is located at
		(0,0,10) in world coordinates and looks towards (0,0,0).
	*/
	class RE167_EXPORT FirstPersonCamera : public Camera
	{
    public:
        
        FirstPersonCamera(const Vector3 &position = Vector3::ZERO_VECTOR,
                            const Vector3 &lookAt = Vector3(0,0,-1), 
                            const Vector3 &up = Vector3(0,1,0) );
        ~FirstPersonCamera();
        
        void setYaw(float yaw);
        void setPitch(float pitch);
        
        inline const int getYaw() const { return yaw; }
        inline const int getPitch() const { return pitch; }
	    
    private:
        float yaw;
        float pitch;
        
        static const float MAX_PITCH;
        static const float MIN_PITCH;
        
        void updatePosition();
    };
}
#endif