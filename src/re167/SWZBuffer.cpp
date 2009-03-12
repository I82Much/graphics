#include "SWZBuffer.h"


using namespace RE167;

/**
* Some implementation details from 
* http://www.sjbaker.org/steve/omniv/love_your_z_buffer.html
* This class represents a z-buffer that gives more resolution near
* the eye and less in the distance.
*/

// CONSTANTS
// 8 bits per byte; sizeof returns number of bytes
const int SWZBuffer::NUM_BITS_PRECISION = sizeof(int) * 8;

// TODO : WTF?
const int SWZBuffer::NUMBER_OF_BINS = 4;



// PUBLIC METHODS

SWZBuffer::SWZBuffer(int width, int height, QImage * image) {
	this->width = width;
	this->height = height;
	// Store the image pointer for later
	this->image = image;

	createBuffer(width, height);
}

SWZBuffer::~SWZBuffer() {
	deleteBuffer(width, height);
}

/**
* Changes the underlying zbuffer data structure to have newWidth
* newHeight.
*/
void SWZBuffer::setSize(int newWidth, int newHeight) {
	deleteBuffer(width, height);
	createBuffer(newWidth, newHeight);
	this->width = newWidth;
	this->height = newHeight;
}


/**
* @param i 		which row in image?
* @param j 		which column in image?
* @param z  	at what depth is the pixel?
* @param color	what color to set (3 element array, (r,g,b)
*/
void SWZBuffer::setPixel(int i, int j, float z, float* color) {
	// Note: since these values only change when changing camera 
	// settings, these should really be constants...
	float a = far / ( far - near );
	float b = far * near / ( near - far );

	int depth = NUMBER_OF_BINS * ( a + b / z );
	if (zbuffer[i][j] < depth) {
		zbuffer[i][j] = depth;
		//image->setPixel(i, j, color[0], color[1], color[2]);
	}
}








// PRIVATE METHODS


void SWZBuffer::createBuffer(int width, int height) {
	// Initialize the zbuffer
	zbuffer = new int*[height];
	assert(zbuffer != NULL);
	for (int i = 0; i < height; i++) {
		zbuffer[i] = new int[width];
		assert(zbuffer[i] != NULL);
	}
	
	// Initialize the buffer to hold the farthest value
	// that can be represented.
	int maxDist = 0;
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			zbuffer[row][col] = maxDist;
		}
	}
}

/**
* Fills the buffer with the farthest value that can be
* represented.
*/
void SWZBuffer::fillZBuffer(int width, int height) {
	
}

/**
*
*/
void SWZBuffer::deleteBuffer(int width, int height) {
	if (zbuffer != NULL) {
		for (int i = 0; i < height; i++) {
			delete[] zbuffer[i];
		}
		delete[] zbuffer;
	}
}
