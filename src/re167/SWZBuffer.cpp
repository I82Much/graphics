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

SWZBuffer::SWZBuffer(int width, int height) {
	this->width = width;
	this->height = height;

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
* Determines if this z coordinate is closer than the one previously stored in
* the zbuffer data structure.  If so, overwrites the value in the zbuffer and
* returns true; else false.
* @param i which row in the image
* @param j which column
* @param 
*/
bool SWZBuffer::isCloser(int i, int j, float depth) {
    assert(zbuffer);
    assert(zbuffer[i]);
    
    return depth > zbuffer[i][j];    
}

void SWZBuffer::setPixel(int i, int j, float depth) {
    assert(zbuffer);
    assert(zbuffer[i]);
    
    zbuffer[i][j] = depth;
}







void SWZBuffer::reset() {
    if (zbuffer != NULL) {
        fill();
    }
    
}


// PRIVATE METHODS


void SWZBuffer::createBuffer(int width, int height) {
	// Initialize the zbuffer
	zbuffer = new float*[height];
	assert(zbuffer != NULL);
	for (int i = 0; i < height; i++) {
		zbuffer[i] = new float[width];
		assert(zbuffer[i] != NULL);
	}

	// Initialize the buffer to hold the farthest value
	// that can be represented.
    fill();
}

/**
* Fills the buffer with the farthest value that can be
* represented.
*/
void SWZBuffer::fill() {
    
	float maxDist = 0;
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			zbuffer[row][col] = maxDist;
		}
	}
    
    
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
