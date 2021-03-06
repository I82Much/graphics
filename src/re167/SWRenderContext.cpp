#include "SWRenderContext.h"
#include "SWWidget.h"
#include <algorithm>
#include <qdatetime.h>
#include <cmath>
#include "BasicMath.h"
using namespace RE167;

SWRenderContext* SWRenderContext::getSingletonPtr(void)
{
	return static_cast<SWRenderContext *>(ms_Singleton);
}

SWRenderContext& SWRenderContext::getSingleton(void)
{
	assert( ms_Singleton );
	return ( *(static_cast<SWRenderContext *>(ms_Singleton)) );
}

void SWRenderContext::init()
{
	modelview = Matrix4::IDENTITY;
	projection = Matrix4::IDENTITY;
	viewport = Matrix4::IDENTITY;
	total = Matrix4::IDENTITY;

	// Create the z-buffer that will be used for depth ordering
    buffer = new SWZBuffer(this->width, this->height);
    numFramesRendered = 0;

    clock = new QTime();
    clock->start();
    lastFrameTime = 0;
}

void SWRenderContext::setViewport(int width, int height)
{
	// Compute viewport matrix based on width, height.

	viewport = Matrix4(
			width/2.0f,	0,			0,	width/2.0f,
			// We need to take the - of the height due to the flipped y
			// axis, but only for the entry on diagonal
			0,			-height/2.0f,	0,	height/2.0f,
			0,			0,			.5,	.5,
			0,			0,			0,	1
		);

	total = viewport * projection * modelview;

    // Keep track of width and height of image for when we compute bounding boxes
    this->width = width;
    this->height = height;

    // The width and height have changed, need to resize our z-buffer
    buffer->setSize(width, height);
}

void SWRenderContext::beginFrame()
{
	image->fill(qRgb(0,0,0));
    //clock->start();
}

void SWRenderContext::endFrame()
{
	mswWidget->setPixmap(QPixmap::fromImage(*image));
	mswWidget->repaint();

    buffer->reset();

    
    /*curFrameTime = clock->elapsed();

    // http://stackoverflow.com/questions/87304/calculating-frames-per-second-in-a-game
    // TODO: Magic number removal
    float avgTime = (curFrameTime * 0.9) + (lastFrameTime * 0.1);

    float avgSecondsPerFrame = avgTime / 1000.0f;
    float fps = 1.0f / avgSecondsPerFrame;
    std::cout << fps << std::endl;
    lastFrameTime = curFrameTime;*/
    numFramesRendered++;

    if (numFramesRendered == 1000) {
        // Number of milliseconds
        int elapsedTime = clock->elapsed();
        float numSeconds = elapsedTime / 1000.0f;
        std::cout << numSeconds << " num seconds" << std::endl;
        std::cout << "FPS: " << numFramesRendered / numSeconds << std::endl;
    }

}

void SWRenderContext::setModelViewMatrix(const Matrix4 &m)
{
	// Set modelview matrix.
	modelview = m;
	total = viewport * projection * modelview;
}

void SWRenderContext::setProjectionMatrix(const Matrix4 &m)
{
	// Set projection matrix.
	projection = m;
	total = viewport * projection * modelview;
}

void SWRenderContext::render(Object *object)
{
	VertexData& vertexData = object->vertexData;
	VertexDeclaration& vertexDeclaration = vertexData.vertexDeclaration;
	VertexBufferBinding& vertexBufferBinding = vertexData.vertexBufferBinding;

	unsigned char *pPtr;	// Pointer to vertex position
	int pStr;				// Stride, i.e., number of bytes between vertex entries
	int pSze;				// Size, i.e., number of vector elements per vertex position (typically 3)

	unsigned char *nPtr;
	int nStr;
	int nSze;

	unsigned char *cPtr;
	int cStr;
	int cSze;

	pPtr = 0;	// Disable all arrays per default
	nPtr = 0;
	cPtr = 0;

	// Set up arrays
	for(int j=0; j<vertexDeclaration.getElementCount(); j++)
	{
		const VertexElement *element = vertexDeclaration.getElement(j);

		const VertexBuffer& vertexBuffer = vertexBufferBinding.getBuffer(element->getBufferIndex());
		unsigned char* buf = vertexBuffer.getBuffer();

		int vertexStride = static_cast<int>(element->getStride());
		int vertexSize = static_cast<int>(element->getSize());
		int offset = element->getOffset();

		switch(element->getSemantic())
		{
			case VES_POSITION :
				pPtr = buf+offset;
				pStr = vertexStride;
				pSze = vertexSize;
				break;
			case VES_NORMAL :
				nPtr = buf+offset;
				nStr = vertexStride;
				nSze = 3;	// always
				break;
			case VES_DIFFUSE :
				cPtr = buf+offset;
				cStr = vertexStride;
				cSze = vertexSize;
				break;
		}
	}

	// Draw
	float p[3][4];	// Triangle vertex positions
	float c[3][4];	// Triangle vertex colors
	float n[3][3];	// Triangle vertex normals

	int *iPtr = vertexData.getIndexBuffer();

	for(int i=0; i<vertexData.getIndexCount(); i++)
	{
		// Local index of current triangle vertex
		int k = i%3;

		// Set default values
		p[k][0] = p[k][1] = p[k][2] = 0;
		p[k][3] = 1.f;
		c[k][0] = c[k][1] = c[k][2] = 1.f;
		c[k][3] = 0;
		n[k][0] = n[k][1] = 0;
		n[k][2] = 1;

		// Vertex position
		if(pPtr)
		{
			float *cur = (float*)(pPtr + pStr*iPtr[i]);
			for(int s=0; s<pSze; s++)
			{
				p[k][s] = *cur;
				cur++;
			}
		}

		// Vertex normal
		if(nPtr)
		{
			float *cur = (float*)(nPtr + nStr*iPtr[i]);
			for(int s=0; s<nSze; s++)
			{
				n[k][s] = *cur;
				cur++;
			}
		}

		// Vertex color
		if(cPtr)
		{
			float *cur = (float*)(cPtr + cStr*iPtr[i]);
			for(int s=0; s<cSze; s++)
			{
				c[k][s] = *cur;
				cur++;
			}
		}

		// Draw triangle
		if(i%3 == 2)
		{
			rasterizeTriangle(p, n, c);
		}
	}
}

/**
 * Given a triangle in 4-space, its normals, and the colors at each
 * vertex, draws it to the screen.
 * @param p		Triangle vertex positions (x,y,z,w)
 * @param n		Triangle vertex normals (x,y,z)
 * @param c		Triangle vertex colors (r,g,b,a)
*/
void SWRenderContext::rasterizeTriangle(float p[3][4], float n[3][3], float c[3][4])
{
    // This brings the vertices into clip space
    Vector4 c1 = (projection * modelview) * Vector4(p[0][0], p[0][1], p[0][2], p[0][3]);
	Vector4 c2 = (projection * modelview) * Vector4(p[1][0], p[1][1], p[1][2], p[1][3]);
	Vector4 c3 = (projection * modelview) * Vector4(p[2][0], p[2][1], p[2][2], p[2][3]);

    // Do the complete transformation on object coordinates
    Vector4 w1 = total * Vector4(p[0][0], p[0][1], p[0][2], p[0][3]);
	Vector4 w2 = total * Vector4(p[1][0], p[1][1], p[1][2], p[1][3]);
	Vector4 w3 = total * Vector4(p[2][0], p[2][1], p[2][2], p[2][3]);

    // triangle is behind the camera
    if (w1.getW() < 0 && w2.getW() < 0 && w3.getW() < 0) {
        return;
    }

    // We will linearly interpolate between these values for the depths; note that
    // w is not necessarily equal to 1 due to perspective transformation.
    float d1 = 1.0f / w1.getW();
    float d2 = 1.0f / w2.getW();
    float d3 = 1.0f / w3.getW();

    // Bring to screen coordinates by homogenizing.  This is the same as dividing by the w coordinate,
    // but we must do it this way since vectors do not have division defined.
    w1 *= (1.0f/w1.getW());
    w2 *= (1.0f/w2.getW());
    w3 *= (1.0f/w3.getW());

    // After homogenous division, the vector is basically a 3d vector so we can
    // treat it as such (necessary for what we need to do with cross products)
    Vector3 v1(w1.getX(), w1.getY(), w1.getZ());
    Vector3 v2(w2.getX(), w2.getY(), w2.getZ());
    Vector3 v3(w3.getX(), w3.getY(), w3.getZ());

    // Attempt to cull this triangle
    Vector3 normal = (v2 - v1).crossProduct((v3 - v1));
    // Triangle is backfacing.  Ignore it
    if (normal.getZ() > 0) {
        return;
    }
    // Degenerate culling
    else if (normal == Vector3::ZERO_VECTOR) {
        return;
    }

    // Isolate the x, y coordinates of each vertex so we avoid calling the
    // getter for each calculation
    float x1 = (w1.getX());
    float y1 = (w1.getY());

    float x2 = (w2.getX());
    float y2 = (w2.getY());

    float x3 = (w3.getX());
    float y3 = (w3.getY());


    // Calculate a bounding box around the vertex
    float xMin = std::min(x1, std::min(x2, x3));
    float xMax = std::max(x1, std::max(x2, x3));
    float yMin = std::min(y1, std::min(y2, y3));
    float yMax = std::max(y1, std::max(y2, y3));

    // If the triangle is not at least partially onscreen, we can quit now
    if (offscreen(xMin, xMax, yMin, yMax)) {
        return;
    }

    // Clamp the mins and maxes to fit within the screen
    xMin = std::max(0.0f, xMin);
    // the right most pixel on screen is at index width - 1
    xMax = std::min(width - 1.0f, xMax);

    yMin = std::max(0.0f, yMin);
    yMax = std::min(height - 1.0f, yMax);

    assert(xMin <= xMax);
    assert(yMin <= yMax);

    // Group all of our vertices in screen coordinates together
    float vertices[3][2] = { 	{ x1, y1 },
                                { x2, y2 },
                                { x3, y3 }
    };


    // The idea is to split the bounding box into smaller tiles, each of which
    // contains multiple pixels.  If a tile is completely outside of the triangle,
    // then it can be ignored

    // Calculate the size of the tiles
    float MAX_TILE_SIZE = 16;

    float triangleWidth =  (xMax - xMin) + 1.0f;
    float triangleHeight = (yMax - yMin) + 1.0f;


    assert (triangleHeight > 0);
    assert (triangleWidth > 0);

    // Calculate how many tiles / partial tiles fit within the bounding box
    int numTileRows = static_cast<int>( std::ceil( triangleHeight/MAX_TILE_SIZE ) );
    int numTileCols = static_cast<int>( std::ceil( triangleWidth/MAX_TILE_SIZE ) );

    assert (numTileRows >= 1);
    assert (numTileCols >= 1);

    float depths[] = {d1, d2, d3};

    for (int row = 0; row < numTileRows; row++) {
        for (int col = 0; col < numTileCols; col++) {
            // Determine the bounds of tile, ensuring that the tile does not
            // extend off the screen or outside of the triangle limits
            int left = xMin + col * MAX_TILE_SIZE;
            int right = std::min(left + MAX_TILE_SIZE, xMax);

            int top = yMin + row * MAX_TILE_SIZE;
            int bottom = std::min(top + MAX_TILE_SIZE, yMax);

            // At least part of the tile overlaps; go through and determine
            // which pixels need drawing
            if (tileOverlapsTriangle(left, right, top, bottom, vertices) ) {
                drawTile(left, right, top, bottom, vertices, c, depths);
            }
            // tile is completely outside of the triangle; no need to do anything
            else {
                continue;
            }
        }
    }
}


/**
 * Determines if the bounding box defined by left, right,
 * top, and bottom is entirely offscreen.
 */
bool SWRenderContext::offscreen(float left, float right, float top, float bottom) {
    float minX = 0.0f;
    float minY = 0.0f;
    float maxX = static_cast<float>(width - 1.0f);
    float maxY = static_cast<float>(height - 1.0f);

    // Check if left and right are off the left side of screen
    return ((left < minX && right < minX) ||
            // off the right side of screen
            (left >= maxX && right >= maxX) ||
            // off top of screen
            (top < minY && bottom < minY) ||
            // off bottom of screen
            (top >= maxY && bottom >= maxY));
}


/**
 * Determines if the pixel corresponding to the coordinate in 
 * barycentric coordinates should be drawn.  All pixels in the triangle
 * should be drawn (with the caveat that they pass the depth buffer test)
 * but none outside should be drawn.
 */
bool SWRenderContext::shouldDraw(float alpha, float beta, float gamma, float vertices[3][2]) {
    SWRenderContext::TriangleLocation location = fromBarycentric(alpha, beta, gamma);

    if (location == OUTSIDE) {
        return false;
    }

    else if (location == INSIDE) {
       return true;
    }
    // We will adopt an upper left fill convention, so only those edges that are
    // horizontal top edges or edges on left of triangle are drawn
    else if (location == ON_EDGE) {
        // This is not quite working yet; for now don't draw edges
        return false;
        /*
        SWRenderContext::TriangleEdge edge = findEdge(alpha, beta, gamma);
        return isLeftEdge(edge, vertices) || isTopHorizontalEdge(edge, vertices);*/
    }
    else {
        assert(false && "Fell through cases in shouldDraw");
    }

    return false;
}


/**
 * Determines if the given edge is on the left in this triangle.
 */
bool SWRenderContext::isLeftEdge(SWRenderContext::TriangleEdge edge, float vertices[3][2]) {
    const int Y_INDEX = 1;

    //If you traverse the triangle edges in counter-clockwise order,
    //the top-left ones will be the ones pointing downward.
    float dy = 0;

    // Calculate the vector defining the directed edge
    switch (edge) {

    // AB is between vertices [0] and [1]
    case AB:
        dy = vertices[1][Y_INDEX] - vertices[0][Y_INDEX];
        break;
         // BC is between vertices [1] and [2]
    case BC:
        dy = vertices[2][Y_INDEX] - vertices[1][Y_INDEX];
        break;
        // CA is between vertices [2] and [0]
    case CA:
        dy = vertices[0][Y_INDEX] - vertices[2][Y_INDEX];
        break;
    default:
        std::cerr << "Fell through switch in isLeftEdge" << std::endl;
        assert(false);
        break;
    }
    // Since we are dealing with screen coordinates and our convention is that
    // lower on the screen has higher y value, a positive dy indicates
    // that the directed edge points downward.
    return dy > 0.0f;
}



/**
 * Determines if the edge (AB, BC, or CA) is a top horizontal edge in our
 * triangle.
 */
bool SWRenderContext::isTopHorizontalEdge(SWRenderContext::TriangleEdge edge, float vertices[3][2]) {
    const int Y_INDEX = 1;
    float y1 = vertices[0][Y_INDEX];
    float y2 = vertices[1][Y_INDEX];
    float y3 = vertices[2][Y_INDEX];

    float dy = 0;
    bool oppositeVertexBelow = false;
    // Calculate the vector defining the directed edge
    switch (edge) {
    // AB is between vertices [0] and [1]
    case AB:
        dy = y2 - y1;
        oppositeVertexBelow = y3 > y1 && y3 > y2;
        break;
         // BC is between vertices [1] and [2]
    case BC:
        dy = y3 - y2;
        oppositeVertexBelow = y1 > y3 && y1 > y2;
        break;
        // CA is between vertices [2] and [0]
    case CA:
        dy = y1 - y3;
        oppositeVertexBelow = y2 > y1 && y2 > y1;
        break;
    default:
        std::cerr << "Fell through switch in isLeftEdge" << std::endl;
        assert(false);
        break;
    }

    return dy == 0.0f && oppositeVertexBelow; //BasicMath::approxEqual(dy, 0.0f) && oppositeVertexBelow;



}

/**
 * Given coordinates of a point on an edge of a triangle, determine which
 * edge it is
 */
SWRenderContext::TriangleEdge SWRenderContext::findEdge(float alpha, float beta, float gamma) {

    if (BasicMath::approxEqual(0.0f, alpha)) {
        return BC;
    }
    else if (BasicMath::approxEqual(0.0f, beta)) {
        return CA;
    }
    else {
        return AB;
    }

}

/**
 * Given a pair of barycentric coordinates, determines if the point 
 * represented is inside, outside, or on an edge within the triangle.
 */
SWRenderContext::TriangleLocation SWRenderContext::fromBarycentric(float alpha, float beta, float gamma) {

    if ( (0 < alpha && alpha < 1) &&
         (0 < beta && beta < 1) &&
         (0 < gamma && gamma < 1) ) {
        return INSIDE;
    }
    else if (BasicMath::approxEqual(0.0f, alpha) ||
             BasicMath::approxEqual(0.0f, beta) ||
             BasicMath::approxEqual(0.0f, gamma) ) {
        return ON_EDGE;
    }
    else {
        return OUTSIDE;
    }

}


/**
 * Given a rectangular region of the screen, determines for each pixel whether
 * it needs to be drawn, and if so in which color.
 */
void SWRenderContext::drawTile(int left, int right, int top, int bottom, float vertices[3][2], float c[3][4], float depths[3]) {

    assert(right <= width - 1);
    assert(left >= 0);
    assert(top <= height - 1);
    assert(bottom >= 0);

    // Calculate barycentric coordinates for each pixel in bounding box.
	// Barycentric coordinates are a coordinate system in which a plane is
	// defined by the three sides of a triangle.  The parametrization values
	// are called alpha, beta, gamma.  If all are between 0 and 1, the point is
	// within the triangle.
	// See http://en.wikipedia.org/wiki/Barycentric_coordinates_(mathematics)
	for (int x = left; x <= right; x++) {
		for (int y = top; y <= bottom; y++) {

			float point[] = {x, y};

			Vector3 baryCoords = barycentric(point, vertices);
			float alpha = baryCoords.getX();
			float beta = baryCoords.getY();
			float gamma = baryCoords.getZ();

            // Those points outside of triangle should not be drawn
            if (shouldDraw(alpha, beta, gamma, vertices)) {

                // Linearly interpolate the z value so we can check if the point
                // is visible
                float depth = (alpha * depths[0]) +
                    (beta * depths[1]) +
                    (gamma * depths[2]);


                if (buffer->isCloser(x, y, depth)) {
                    buffer->setPixel(x, y, depth);
                    // QRgb color = linearInterpolation(alpha, beta, gamma, c);
                    QRgb color = perspectiveCorrectInterpolation(alpha, beta, gamma,  depths, c);
                    image->setPixel(x, y, color );

				} // pixel visible
			} // pixel in triangle

		} // for y
    } // for x
}



/**
* @param point 		a float array, [x, y] representing position of point to calculate
* 					barycentric coordinates for
* @param vertices 	a 3 element array, one per vertex of triangle, representing the
* 					2d location of each vertex
*/
Vector3 SWRenderContext::barycentric(float point[2], float vertices[3][2]){
	const int x = 0;
	const int y = 1;

	// Triangle is defined by points (a,b,c).
	float *a = vertices[0];
	float *b = vertices[1];
	float *c = vertices[2];
	float gammaNumerator = 		((a[y] - b[y]) * point[x]) +
								((b[x] - a[x]) * point[y]) +
								(a[x] * b[y]) - (b[x] * a[y]);

	float gammaDenominator = 	((a[y] - b[y]) * c[x]) +
								((b[x] - a[x]) * c[y]) +
								(a[x] * b[y]) - (b[x] * a[y]);

	float gamma = gammaNumerator / gammaDenominator;

	float betaNumerator =		((a[y] - c[y]) * point[x]) +
								((c[x] - a[x]) * point[y]) +
								(a[x] * c[y]) - (c[x] * a[y]);

	float betaDenominator =		((a[y] - c[y]) * b[x]) +
								((c[x] - a[x]) * b[y]) +
								(a[x] * c[y]) - (c[x] * a[y]);

	float beta = betaNumerator / betaDenominator;

	float alpha = 1.0f - beta - gamma;
	return Vector3(alpha, beta, gamma);
}


/**
 * Given a bounding box, determine if it overlaps the triangle defined
 * by the vertices float array.  
 */
bool SWRenderContext::tileOverlapsTriangle(int left,
                                           int right,
                                           int top,
                                           int bottom,
                                           float vertices[3][2]) {

    float upperLeft[] = {left, top};
    float upperRight[] = {right, top};
    float lowerLeft[] = {left, bottom};
    float lowerRight[] = {right, bottom};


    // Calculate the barycentric coordinates of the four corners
    Vector3 upperLeftBary = barycentric(upperLeft, vertices);
    Vector3 upperRightBary = barycentric(upperRight, vertices);
    Vector3 lowerLeftBary = barycentric(lowerLeft, vertices);
    Vector3 lowerRightBary = barycentric(lowerRight, vertices);

    float upperLeftAlpha = upperLeftBary.getX();
    float upperLeftBeta = upperLeftBary.getY();
    float upperLeftGamma = upperLeftBary.getZ();

    float upperRightAlpha = upperRightBary.getX();
    float upperRightBeta = upperRightBary.getY();
    float upperRightGamma = upperRightBary.getZ();

    float lowerLeftAlpha = lowerLeftBary.getX();
    float lowerLeftBeta = lowerLeftBary.getY();
    float lowerLeftGamma = lowerLeftBary.getZ();

    float lowerRightAlpha = lowerRightBary.getX();
    float lowerRightBeta = lowerRightBary.getY();
    float lowerRightGamma = lowerRightBary.getZ();

    // If all are < 0 on one parameter, the tile is completely outside
    // of the triangle.
    if ((upperLeftAlpha < 0 && upperRightAlpha < 0 && lowerLeftAlpha < 0 && lowerRightAlpha < 0) ||
        (upperLeftBeta < 0 && upperRightBeta < 0 && lowerLeftBeta < 0 && lowerRightBeta < 0) ||
        (upperLeftGamma < 0 && upperRightGamma < 0 && lowerLeftGamma < 0 && lowerRightGamma < 0)) {
        return false;
    }
    else {
        return true;
    }

}

/**
 * Given coordinates in barycentric coordinate system and the colors of the 
 * vertices of the triangle, calculates linear interpolation of the colors.
 * Used to create a smooth shaded appearance. Note that this does not
 * handle perspective correctly - must use perspectiveCorrectInterpolation
 * for that
 */
QRgb SWRenderContext::linearInterpolation(float alpha, float beta, float gamma, float colors[3][4]) {
     // c(p) = alpha(p)c_a + beta(p)c_b + gamma(p)c_c
    float r = (alpha * colors[0][0]) + (beta * colors[1][0]) + (gamma * colors[2][0]);
    float g = (alpha * colors[0][1]) + (beta * colors[1][1]) + (gamma * colors[2][1]);
    float b = (alpha * colors[0][2]) + (beta * colors[1][2]) + (gamma * colors[2][2]);
    float a = (alpha * colors[0][3]) + (beta * colors[1][3]) + (gamma * colors[2][3]);

    // We have our rgba on a scale of [0...1].  Scale to be in range [0...255].
    r *= 255;
    g *= 255;
    b *= 255;
    a *= 255;

    return qRgba(r,g,b,a);
}

/**
 * Given coordinates in barycentric coordinate system, the colors of the 
 * vertices of the triangle, and the depths of the same vertices, calculates
 * the correct color for that coordinate given the perspective of the triangle.
 */
QRgb SWRenderContext::perspectiveCorrectInterpolation(float alpha, float beta, float gamma, float depths[3], float colors[3][4]) {
    float d1 = depths[0];
    float d2 = depths[1];
    float d3 = depths[2];

    float oneOverW = (alpha * d1) + (beta * d2) + (gamma * d3);

    float rOverW = (alpha * d1 * colors[0][0]) + (beta * d2 * colors[1][0]) + (gamma * d3 * colors[2][0]);
    float gOverW = (alpha * d1 * colors[0][1]) + (beta * d2 * colors[1][1]) + (gamma * d3 * colors[2][1]);
    float bOverW = (alpha * d1 * colors[0][2]) + (beta * d2 * colors[1][2]) + (gamma * d3 * colors[2][2]);
    float aOverW = (alpha * d1 * colors[0][3]) + (beta * d2 * colors[1][3]) + (gamma * d3 * colors[2][3]);
    float r = rOverW / oneOverW;
    float g = gOverW / oneOverW;
    float b = bOverW / oneOverW;
    float a = aOverW / oneOverW;

    // We have our rgba on a scale of [0...1].  Scale to be in range [0...255].
    r *= 255;
    g *= 255;
    b *= 255;
    a *= 255;

    return qRgba(r,g,b,a);


}

void SWRenderContext::setWidget(SWWidget *swWidget)
{
	mswWidget = swWidget;
	image = mswWidget->getImage();
}
