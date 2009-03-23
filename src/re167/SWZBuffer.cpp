#include "SWZBuffer.h"
#include <iostream>

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
    std::cout << "setting size " << std::endl;

	deleteBuffer(width, height);
	createBuffer(newWidth, newHeight);
	this->width = newWidth;
	this->height = newHeight;
}


/**
* Determines if this z coordinate is closer than the one previously stored in
* the zbuffer data structure.  If so, overwrites the value in the zbuffer and
* returns true; else false.
* @param x x-coordinate in image (column)
* @param y y-coordinate in image (row)
* @param 
*/
bool SWZBuffer::isCloser(int x, int y, float depth) {
    assert(zbuffer);
    assert(zbuffer[y]);

    assert(y <= (height - 1));
    assert(x <= (width - 1));

    return depth > zbuffer[y][x];    
}

void SWZBuffer::setPixel(int x, int y, float depth) {
    assert(zbuffer != NULL);
    assert(zbuffer[y] != NULL);

    zbuffer[y][x] = depth;
}


void SWZBuffer::reset() {
    if (zbuffer != NULL) {
        fill();
    }
    
}


// PRIVATE METHODS

/**
 * See http://www.velocityreviews.com/forums/t286693-how-to-quotnewquot-a-twodimension-array-in-c.html

 */
void SWZBuffer::createBuffer(int width, int height) {
	// Initialize the zbuffer
	zbuffer = new float*[height];
	assert(zbuffer != NULL);

    zbuffer[0] = new float[height * width];
    assert (zbuffer[0] != NULL);
    // Rather than manually calling "new" each time, we just take
    // a pointer to an element within our zbuffer[0] that we allocated.
	for (int i = 1; i < height; i++) {
		zbuffer[i] = zbuffer[i-1] + width;
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
    assert(zbuffer);
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
        delete[] zbuffer[0];
        delete[] zbuffer;
    }
}
