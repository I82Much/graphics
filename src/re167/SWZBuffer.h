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
	* This class holds a reference to the QImage as it serves as
	* an intermediary between the SWRenderContext and the QImage.
	* This class delegates.
	*/
	class RE167_EXPORT SWZBuffer
	{
	public:
		SWZBuffer(int width, int height, QImage * image);
		~SWZBuffer();

		void setSize(int width, int height);

		void setPixel(int i, int j, float z, float* color);


	private:
        int width;
        int height;
        QImage *image;


		int ** zbuffer;
		float near;
		float far;

		/*float farOverDiff;
		float farTimesNearOverDiff;*/

		void createBuffer(int width, int height);
		void deleteBuffer(int width, int height);
        void fillZBuffer(int width, int height);

		static const int NUM_BITS_PRECISION;
		static const int NUMBER_OF_BINS;
	};
}


#endif
