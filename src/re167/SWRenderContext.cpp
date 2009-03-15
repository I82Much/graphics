#include "SWRenderContext.h"
#include "SWWidget.h"
#include <algorithm>
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
    
    
    
}

void SWRenderContext::endFrame()
{
	mswWidget->setPixmap(QPixmap::fromImage(*image));
	mswWidget->repaint();
	
	buffer->reset();
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
* @param p		Triangle vertex positions (x,y,z,w)
* @param n		Triangle vertex normals (x,y,z)
* @param c		Triangle vertex colors (r,g,b,a)
*/
void SWRenderContext::rasterizeTriangle(float p[3][4], float n[3][3], float c[3][4])
{
    // Do the complete transformation on object coordinates 
    Vector4 w1 = total * Vector4(p[0][0], p[0][1], p[0][2], p[0][3]);
	Vector4 w2 = total * Vector4(p[1][0], p[1][1], p[1][2], p[1][3]);
	Vector4 w3 = total * Vector4(p[2][0], p[2][1], p[2][2], p[2][3]);

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
    
        
    float x1 = w1.getX();
    float y1 = w1.getY();
   
    float x2 = w2.getX();
    float y2 = w2.getY();
   
    float x3 = w3.getX();
    float y3 = w3.getY();
    
    // Calculate a bounding box around the vertex
    float xMin = std::min(x1, std::min(x2, x3));
    float xMax = std::max(x1, std::max(x2, x3));
    float yMin = std::min(y1, std::min(y2, y3));
    float yMax = std::max(y1, std::max(y2, y3));
    
    // Clamp the mins and maxes to fit within the screen
    xMin = std::max(0.0f, xMin);
    xMax = std::min((float)width, xMax);
    
    yMin = std::max(0.0f, yMin);
    yMax = std::min((float)height, yMax);
    
    // Group all of our vertices in screen coordinates together
    float vertices[3][2] = { 	{ x1, y1 },
								{ x2, y2 },
								{ x3, y3 }
							};
	
	// Calculate barycentric coordinates for each pixel in bounding box.
	// Barycentric coordinates are a coordinate system in which a plane is
	// defined by the three sides of a triangle.  The parametrization values
	// are called alpha, beta, gamma.  If all are between 0 and 1, the point is
	// within the triangle. 
	// See http://en.wikipedia.org/wiki/Barycentric_coordinates_(mathematics)
	for (int x = xMin; x < xMax; x++) {
		for (int y = yMin; y < yMax; y++) {
			
			float point[] = {x, y};
			
			Vector3 baryCoords = barycentric(point, vertices);
			float alpha = baryCoords.getX();
			float beta = baryCoords.getY();
			float gamma = baryCoords.getZ();
			
			// Pixel is inside of the triangle.
			if ( (0 < alpha && alpha < 1) &&
				 (0 < beta && beta < 1) &&
				 (0 < gamma && gamma < 1) ) {
                    
                    
                // Linearly interpolate the z value so we can check if the point
                // is visible    
                // di is the z distance to vertex i
                float depth = (alpha * d1) + (beta * d2) + (gamma * d3);
                    
				if (buffer->isCloser(x, y, depth)) {
				    
				    
                    buffer->setPixel(x, y, depth);
                    // Set pixel to linearly interpolated color
				
			    	// c(p) = alpha(p)c_a + beta(p)c_b + gamma(p)c_c
                    float r = (alpha * c[0][0]) + (beta * c[1][0]) + (gamma * c[2][0]);
                    float g = (alpha * c[0][1]) + (beta * c[1][1]) + (gamma * c[2][1]);
                    float b = (alpha * c[0][2]) + (beta * c[1][2]) + (gamma * c[2][2]);
                    float a = (alpha * c[0][3]) + (beta * c[1][3]) + (gamma * c[2][3]);
				
    				// We have our rgba on a scale of [0...1].  Scale to be in range [0...255].
                    r *= 255;
                    g *= 255;
                    b *= 255;
                    a *= 255;
				
                    image->setPixel(x, y, qRgba(r,g,b,a) );
                
				} // pixel visible
			} // pixel in triangle
			
		}
	}
	
	
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





void SWRenderContext::setWidget(SWWidget *swWidget)
{
	mswWidget = swWidget;
	image = mswWidget->getImage();
}