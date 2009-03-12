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
}

void SWRenderContext::setViewport(int width, int height)
{
	// Compute viewport matrix based on width, height.

	viewport = Matrix4(
			width/2,	0,			0,	width/2,
			// We need to take the - of the height due to the flipped y
			// axis, but only for the entry on diagonal
			0,			-height/2,	0,	height/2,
			0,			0,			.5,	.5,
			0,			0,			0,	1
		);

	total = viewport * projection * modelview;

	// The width and height have changed, need to resize our z-buffer

}

void SWRenderContext::beginFrame()
{
	image->fill(qRgb(0,0,0));
}

void SWRenderContext::endFrame()
{
	mswWidget->setPixmap(QPixmap::fromImage(*image));
	mswWidget->repaint();
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
	// Implement triangle rasterization here.
	// Use viewport*projection*modelview matrix to project vertices to screen.
	// You can draw pixels in the output image using image->setPixel(...);
	
 	
	// Project vertices to screen

	Vector4 v1 = total * Vector4(p[0][0], p[0][1], p[0][2], p[0][3]);
	Vector4 v2 = total * Vector4(p[1][0], p[1][1], p[1][2], p[1][3]);
	Vector4 v3 = total * Vector4(p[2][0], p[2][1], p[2][2], p[2][3]);
	
	// Need to homogenize the coordinates
	float x1, y1;
	float x2, y2;
	float x3, y3;
	
    x1 = v1.getX() / v1.getW();
    y1 = v1.getY() / v1.getW();
    
    x2 = v2.getX() / v2.getW();
    y2 = v2.getY() / v2.getW();
    
	x2 = v2.getX() / v2.getW();
    y2 = v2.getY() / v2.getW();
    
    
    image->setPixel(static_cast<int>(x1), static_cast<int>(y1), QColor::fromRgb(1,0,0).value());
    image->setPixel(static_cast<int>(x2), static_cast<int>(y2), QColor::fromRgb(1,0,0).value());
    image->setPixel(static_cast<int>(x3), static_cast<int>(y3), QColor::fromRgb(1,0,0).value());
	
	
	// Calculate a bounding box around the vertex
	float xMin = std::min(x1, std::min(x2, x3));
	float xMax = std::max(x1, std::max(x2, x3));
	float yMin = std::min(y1, std::min(y2, y3));
	float yMax = std::max(y1, std::max(y2, y3));
	
	float vertices[3][2] = { 	{ x1, y1 },
								{ x2, y2 },
								{ x3, y3 }
							};
	const int ALPHA = 0;
	const int BETA = 1;
	const int GAMMA = 2;
							
	
	// Calculate barycentric coordinates for each pixel in bounding box.
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
				// Set pixel to triangle color	
			}
			
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