#ifndef __SWZBuffer_h_defined__
#define __SWZBuffer_h_defined__

#include "RE167_global.h"
#include "QImage.h"

namespace RE167
{
	/** This is a software implemented z-buffer, used to
	* determine depth ordering of geometry in a scene.
	* Note that the zbuffer depends on the frustum in the scene,
	* since only those objects that are within the near/far range
	* are rendered.
	*/
	class RE167_EXPORT SWZBuffer
	{
	public:
		SWZBuffer(int width, int height);
		~SWZBuffer();

		void setSize(int width, int height);
        
        void reset();
        
        bool isCloser(int x, int y, float z);
        void setPixel(int x, int y, float z);
        
        
	private:
        int width;
        int height;


		float ** zbuffer;
		float near;
		float far;

		void createBuffer(int width, int height);
		void deleteBuffer(int width, int height);
        
        void fill();

		static const int NUM_BITS_PRECISION;
		static const int NUMBER_OF_BINS;
	};
}


#endif
