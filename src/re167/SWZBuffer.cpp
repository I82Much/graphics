#include "SWZBuffer.h"
#include <iostream>
#include "MainWindow.h"
using namespace RE167;



// PUBLIC METHODS

SWZBuffer::SWZBuffer(int width, int height) {
	this->width = width;
	this->height = height;

	createBuffer(MainWindow::MAX_WIDTH, MainWindow::MAX_HEIGHT);
}

SWZBuffer::~SWZBuffer() {
	deleteBuffer(MainWindow::MAX_WIDTH, MainWindow::MAX_HEIGHT);
}

/**
* Changes the underlying zbuffer data structure to have newWidth
* newHeight.
*/
void SWZBuffer::setSize(int newWidth, int newHeight) {
    
	this->width = newWidth;
	this->height = newHeight;
}


/**
* Determines if this z coordinate is closer than the one previously stored in
* the zbuffer data structure.  
* @param x x-coordinate in image (column)
* @param y y-coordinate in image (row)
* @param depth the depth of the pixel
*/
bool SWZBuffer::isCloser(int x, int y, float depth) {
    assert(zbuffer != NULL);
    assert(zbuffer[y] != NULL);

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

    for (int i = 0; i < height; i++) {
        zbuffer[i] = new float[width];
        assert(zbuffer[i]);
    }
    
/*
    zbuffer[0] = new float[height * width];
    assert (zbuffer[0] != NULL);
    // Rather than manually calling "new" each time, we just take
    // a pointer to an element within our zbuffer[0] that we allocated.
	for (int i = 1; i < height; i++) {
		zbuffer[i] = zbuffer[i-1] + width;
		assert(zbuffer[i] != NULL);
        }*/

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
    //std::fill(zbuffer[0], zbuffer[0] + width * height, maxDist);

	for (int row = 0; row < height; row++) {
        if (!zbuffer[row]) {
            return;
        }

        assert(zbuffer[row]);

		for (int col = 0; col < width; col++) {
			zbuffer[row][col] = maxDist;
		}
    }
}

/**
*
*/
void SWZBuffer::deleteBuffer(int width, int height) {
    std::cout << "Deleting buffer " << std:: endl;

	if (zbuffer != NULL) {
        for (int i = 0; i < height; i++) {
            delete[] zbuffer[i];
        }
        delete [] zbuffer;
        /*
        delete[] zbuffer[0];
        delete[] zbuffer;*/
    }
}
