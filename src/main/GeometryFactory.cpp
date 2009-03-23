/**
* This class creates 3d objects which OpenGL can then render.
* In general there will be two forms for each method: one that
* takes in an Object pointer and optional complexity parameters,
* and another that takes in many more arguments, such as a pointer
* to a vertices array, etc.  The reason for the duplication is that
* the method taking the Object is a nice wrapper to calling classes
* who do not need to worry about the underlying method call that
* creates the geometry (the overloaded method with many more
* parameters).  A calling class can still access the more complicated
* method if they wish; for instance they might wish to get the
* vertices arrays and define their own color arrays.
* @author Nicholas Dunn
* @date   February 6, 2009
*/
#include "GeometryFactory.h"
#include <cmath>             // for trig functions
#include <assert.h>          // for assertions
#include <iostream>
#include "Vector3.h"
#include "ColorFactory.h"
#include "ObjReader.h"
#include "PGMReader.h"

using namespace RE167;
 
//#include "BasicMath.h"

// HACK: CANNOT GET BasicMath to work right
#ifndef PI
#define PI 3.14159265f
#endif

#ifndef TWO_PI
#define TWO_PI (PI * 2.0f)
#endif



const int GeometryFactory::NUM_TRIANGLES_PER_RECTANGULAR_FACE = 2;
const int GeometryFactory::NUM_VERTICES_PER_TRIANGLE = 3;
const int GeometryFactory::NUM_COMPONENTS_PER_VERTEX = 3;
const int GeometryFactory::NUM_COMPONENTS_PER_RECTANGULAR_FACE =
	GeometryFactory::NUM_TRIANGLES_PER_RECTANGULAR_FACE * 
	GeometryFactory::NUM_VERTICES_PER_TRIANGLE * 
	GeometryFactory::NUM_COMPONENTS_PER_VERTEX;



/**
* Creates a terrain from a PGM file and stores it in this 
*/
void GeometryFactory::createTerrainFromPGM(RE167::Object *o, char * filepath, bool normalize) {
	
	assert(o != NULL);
	int imageWidth = 0;
	int imageHeight = 0;

	unsigned char * image = PGMReader::loadPGM(filepath, imageWidth, imageHeight);

	assert(image != NULL);
	assert(imageWidth > 1);
	assert(imageHeight > 1);

	// The value at image[i * imageHeight + j] is the height of the pixel at row i, column j in image.
	// Each value is in the range 0-255.  Let's just use the raw values and go from there.
	
	
	// If you imagine the image viewed from the top, each group of 4 adjacent pixels form a square.
	int numSquareFaces = (imageWidth - 1) * (imageHeight - 1);
	int numTriangles = numSquareFaces * NUM_TRIANGLES_PER_RECTANGULAR_FACE;
	int numVertices = numTriangles * NUM_VERTICES_PER_TRIANGLE;

	float * vertices = new float[numVertices * NUM_COMPONENTS_PER_VERTEX];
	float * colors = new float[numVertices * ColorFactory::NUM_COMPONENTS_PER_COLOR];
	int * indices = new int[numVertices];

	assert(vertices);
	assert(colors);
	assert(indices);

	
	// Go through and create the triangles.  Note that the height (the values stored
	// in image) goes in the y axis, the x axis is for the columns of image,
	// and z axis is for rows

	int counter = 0;
	for (int i = 0; i < imageHeight - 1; i++) {
		for (int j = 0; j < imageWidth - 1; j++) {
			// Create the two triangles tesselating the region formed from
			// pixel[i][j], pixel[i][j+1], pixel[i+1][j], and pixel[i+1][j+1]

			// Triangle 1:  Formed from pixel[i][j], pixel[i+1][j+1], pixel[i][j+1] 
			// (upper right triangle when viewed from above)
			float x1 = static_cast<float> (j);
			float y1 = static_cast<float> (image[(i * imageHeight) + j]);
			float z1 = static_cast<float> (i);

			float x2 = static_cast<float> (j+1);
			float y2 = static_cast<float> (image[((i + 1)* imageHeight) + j + 1]);
			float z2 = static_cast<float> (i+1);

			float x3 = static_cast<float> (j+1);
			float y3 = static_cast<float> (image[(i * imageHeight) + j + 1]);
			float z3 = static_cast<float> (i);

			// Triangle 2: Formed from pixel[i][j], pixel[i+1][j], pixel[i+1][j+1]
			// (lower left triangle when viewed from above)

			float x4 = x1;
			float y4 = y1;
			float z4 = z1;

			float x5 = static_cast<float> (j);
			float y5 = static_cast<float> (image[((i + 1) * imageHeight) + j]);
			float z5 = static_cast<float> (i + 1);

			float x6 = x2;
			float y6 = y2;
			float z6 = z2;

			vertices[counter++] = x1;
			vertices[counter++] = y1;
			vertices[counter++] = z1;

			vertices[counter++] = x2;
			vertices[counter++] = y2;
			vertices[counter++] = z2;

			vertices[counter++] = x3;
			vertices[counter++] = y3;
			vertices[counter++] = z3;

			vertices[counter++] = x4;
			vertices[counter++] = y4;
			vertices[counter++] = z4;

			vertices[counter++] = x5;
			vertices[counter++] = y5;
			vertices[counter++] = z5;

			vertices[counter++] = x6;
			vertices[counter++] = y6;
			vertices[counter++] = z6;
		}
	}

	for (int i = 0; i < numVertices; i++) {
		indices[i] = i;
	}

	if (normalize) {
		ObjReader::normalize(vertices, numVertices);
	}

	ColorFactory::colorTerrain(colors, vertices, numVertices);
	ColorFactory::perturb(colors, numVertices * 3);

	GeometryFactory::fillInObject(o, vertices, colors, indices,
									numVertices * NUM_COMPONENTS_PER_VERTEX,
									numVertices * ColorFactory::NUM_COMPONENTS_PER_COLOR,
									numVertices);
	
}


/**
* Given an Object and a filepath, load the triangle mesh given
* in the file and save the vertex data to the Object.
* Based on sample code provided by Prof. Palacio.
* @param o			the Object to fill with vertex data
* @param filePath	the path to the .obj file
* @param normalize	whether or not to make the object fit into unit cube centered at origin
*/
void GeometryFactory::createObject(RE167::Object *o, char * filepath, bool normalize) {
	int nVerts;
	float *vertices;
	float *normals;
	float *texcoords;
	int nIndices;
	int *indices;

	ObjReader::readObj(filepath, nVerts, &vertices, &normals, &texcoords, nIndices, &indices);

	if (normalize) {
		// Scale all the vertices to fit within the space.
		ObjReader::normalize(vertices, nVerts);
	}

	float *colors = new float[3 * nVerts];
	ColorFactory::matchNormalizedVertices(colors, vertices, 3 * nVerts);
	
	

	VertexData& vertexData = o->vertexData;
	vertexData.vertexDeclaration.addElement(0, 0, 3, 3*sizeof(float), RE167::VES_POSITION);
	vertexData.createVertexBuffer(0, nVerts*3*sizeof(float), (unsigned char*)vertices);

	vertexData.vertexDeclaration.addElement(1, 0, 3, 3*sizeof(float), RE167::VES_DIFFUSE);
	vertexData.createVertexBuffer(1, nVerts*3*sizeof(float), (unsigned char*)colors);

	if(normals)
	{
		vertexData.vertexDeclaration.addElement(2, 0, 3, 3*sizeof(float), RE167::VES_NORMAL);		
		vertexData.createVertexBuffer(2, nVerts*3*sizeof(float), (unsigned char*)normals);
	}

	vertexData.createIndexBuffer(nIndices, indices);

	if(normals) delete[] normals;
	if(texcoords) delete[] texcoords;
	delete[] vertices;
	delete[] indices;
	//delete[] colors;
}

/*
* Creates a cube of side length = 2 centered at (0,0,0)
* Modified by Nick Dunn, provided by Prof. Palacio.
* @param o the Object with which to fill with vertex and color data
*/
void GeometryFactory::createCube(Object *o) {
	// 3 per 
	const int NUM_VERTICES = 24;
	const int SIZE_OF_VERTICES_ARRAY = NUM_VERTICES * NUM_COMPONENTS_PER_VERTEX; 
	float vertices[] = {-1,-1,1, 1,-1,1, 1,1,1, -1,1,1,		// front face
						-1,-1,-1, -1,-1,1, -1,1,1, -1,1,-1, // left face
						1,-1,-1,-1,-1,-1, -1,1,-1, 1,1,-1,  // back face
						1,-1,1, 1,-1,-1, 1,1,-1, 1,1,1,		// right face
						1,1,1, 1,1,-1, -1,1,-1, -1,1,1,		// top face
						-1,-1,1, -1,-1,-1, 1,-1,-1, 1,-1,1};// bottom face

	
	float colors[]= {	1,0,0, 1,0,0, 1,0,0, 1,0,0,
						//0,1,0, 0,1,0, 0,1,0, 0,1,0,
                        0,0,1, 1,0,0, 1,0,0, 0,0,1,
						1,0,0, 1,0,0, 1,0,0, 1,0,0,
						0,1,0, 0,1,0, 0,1,0, 0,1,0,
						0,0,1, 0,0,1, 0,0,1, 0,0,1,
						0,0,1, 0,0,1, 0,0,1, 0,0,1};
	// 6 per face
	const int NUM_INDICES = 36;
	// The index data that stores the connectivity of the triangles
	int indices[] = {0,2,3, 0,1,2,			// front face
		             4,6,7, 4,5,6,			// left face
					 8,10,11, 8,9,10,		// back face
					 12,14,15, 12,13,14,	// right face
					 16,18,19, 16,17,18,	// top face
					 20,22,23, 20,21,22};	// bottom face

	GeometryFactory::fillInObject(o, vertices, colors, indices,
									SIZE_OF_VERTICES_ARRAY, SIZE_OF_VERTICES_ARRAY, NUM_INDICES);
								

}









/**
* Snipped provided by Prof. Palacio
*/
void GeometryFactory::createHouses(Object *object) {
	// quad houses
int nVerts = 42 + (24 + 14) * 3;
float vertices[] = {-4+6,-4, 4+6,   4+6,-4, 4+6,   4+6, 4, 4+6,  -4+6, 4, 4+6,  // front face
                    -4+6,-4,-4+6,  -4+6,-4, 4+6,  -4+6, 4, 4+6,  -4+6, 4,-4+6,  // left face
                     4+6,-4,-4+6,  -4+6,-4,-4+6,  -4+6, 4,-4+6,   4+6, 4,-4+6,  // back face
                     4+6,-4, 4+6,   4+6,-4,-4+6,   4+6, 4,-4+6,   4+6, 4, 4+6,  // right face
                     4+6, 4, 4+6,   4+6, 4,-4+6,  -4+6, 4,-4+6,  -4+6, 4, 4+6,  // top face
                    -4+6,-4, 4+6,  -4+6,-4,-4+6,   4+6,-4,-4+6,   4+6,-4, 4+6,  // bottom face

                    -20,-4,20,  20,-4,20,  20,-4,-20,  -20,-4,-20,              // ground floor

                    -4+6,4, 4+6,   4+6,4, 4+6,  0+6,8, 4+6,                     // the roof
                     4+6,4, 4+6,   4+6,4,-4+6,  0+6,8,-4+6,   0+6,8, 4+6,
                    -4+6,4, 4+6,   0+6,8, 4+6,  0+6,8,-4+6,  -4+6,4,-4+6,
                     4+6,4,-4+6,  -4+6,4,-4+6,  0+6,8,-4+6, 
	
                    -4+6,-4, 4-6,   4+6,-4, 4-6,   4+6, 4, 4-6,  -4+6, 4, 4-6,  // front face
                    -4+6,-4,-4-6,  -4+6,-4, 4-6,  -4+6, 4, 4-6,  -4+6, 4,-4-6,  // left face
                     4+6,-4,-4-6,  -4+6,-4,-4-6,  -4+6, 4,-4-6,   4+6, 4,-4-6,  // back face
                     4+6,-4, 4-6,   4+6,-4,-4-6,   4+6, 4,-4-6,   4+6, 4, 4-6,  // right face
                     4+6, 4, 4-6,   4+6, 4,-4-6,  -4+6, 4,-4-6,  -4+6, 4, 4-6,  // top face
                    -4+6,-4, 4-6,  -4+6,-4,-4-6,   4+6,-4,-4-6,   4+6,-4, 4-6,  // bottom face
					
                    -4+6,4, 4-6,   4+6,4, 4-6,  0+6,8, 4-6,                     // the roof
                     4+6,4, 4-6,   4+6,4,-4-6,  0+6,8,-4-6,  0+6,8, 4-6,
                    -4+6,4, 4-6,   0+6,8, 4-6,  0+6,8,-4-6, -4+6,4,-4-6,
                     4+6,4,-4-6,  -4+6,4,-4-6,  0+6,8,-4-6, 
					
                    -4-6,-4, 4-6,   4-6,-4, 4-6,   4-6, 4, 4-6,  -4-6, 4, 4-6,  // front face
                    -4-6,-4,-4-6,  -4-6,-4, 4-6,  -4-6, 4, 4-6,  -4-6, 4,-4-6,  // left face
                     4-6,-4,-4-6,  -4-6,-4,-4-6,  -4-6, 4,-4-6,   4-6, 4,-4-6,  // back face
                     4-6,-4, 4-6,   4-6,-4,-4-6,   4-6, 4,-4-6,   4-6, 4, 4-6,  // right face
                     4-6, 4, 4-6,   4-6, 4,-4-6,  -4-6, 4,-4-6,  -4-6, 4, 4-6,  // top face
                    -4-6,-4, 4-6,  -4-6,-4,-4-6,   4-6,-4,-4-6,   4-6,-4, 4-6,  // bottom face
					
                    -4-6,4, 4-6,   4-6,4, 4-6,  0-6,8, 4-6,                     // the roof
                     4-6,4, 4-6,   4-6,4,-4-6,  0-6,8,-4-6,  0-6,8, 4-6,
                    -4-6,4, 4-6,   0-6,8, 4-6,  0-6,8,-4-6, -4-6,4,-4-6,
                     4-6,4,-4-6,  -4-6,4,-4-6,  0-6,8,-4-6, 
					
					
                    -4-6,-4, 4+6,   4-6,-4, 4+6,   4-6, 4, 4+6,  -4-6, 4, 4+6,  // front face
                    -4-6,-4,-4+6,  -4-6,-4, 4+6,  -4-6, 4, 4+6,  -4-6, 4,-4+6,  // left face
                     4-6,-4,-4+6,  -4-6,-4,-4+6,  -4-6, 4,-4+6,   4-6, 4,-4+6,  // back face
                     4-6,-4, 4+6,   4-6,-4,-4+6,   4-6, 4,-4+6,   4-6, 4, 4+6,  // right face
                     4-6, 4, 4+6,   4-6, 4,-4+6,  -4-6, 4,-4+6,  -4-6, 4, 4+6,  // top face
                    -4-6,-4, 4+6,  -4-6,-4,-4+6,   4-6,-4,-4+6,   4-6,-4, 4+6,  // bottom face
					
                    -4-6,4, 4+6,   4-6,4, 4+6,  0-6,8, 4+6,                     // the roof
                     4-6,4, 4+6,   4-6,4,-4+6,  0-6,8,-4+6,  0-6,8, 4+6,
                    -4-6,4, 4+6,   0-6,8, 4+6,  0-6,8,-4+6, -4-6,4,-4+6,
                     4-6,4,-4+6,  -4-6,4,-4+6,  0-6,8,-4+6, 
	
                   };

	float colors[] = {1,0,0, 1,1,0, 1,0,0, 1,0,1,
                      0,1,0, 0,1,0, 0,1,1, 0,1,0,
                      1,0,1, 1,0,0, 1,0,1, 1,0,0,
                      1,1,0, 0,1,0, 0,1,1, 0,1,0,
                      0,0,1, 1,0,1, 0,0,1, 0,1,1,
                      1,0,1, 0,1,1, 0,0,1, 0,0,1,
	
                      0,0,0, 0,1,0, 1,1,0, 1,0,0,  // ground floor

                      0,1,1, 0,0,1, 1,0,1,         // roof
                      1,0,0, 1,0,1, 1,0,0, 1,1,0,
                      1,1,0, 0,1,0, 1,1,0, 0,1,0,
                      0,1,1, 0,0,1, 1,0,1, 

                      1,0,0, 1,1,0, 1,0,0, 1,0,1,
                      0,1,0, 0,1,0, 0,1,1, 0,1,0,
                      1,0,1, 1,0,0, 1,0,1, 1,0,0,
                      1,1,0, 0,1,0, 0,1,1, 0,1,0,
                      0,0,1, 1,0,1, 0,0,1, 0,1,1,
                      1,0,1, 0,1,1, 0,0,1, 0,0,1,
	
                      0,1,1, 0,0,1, 1,0,1,         // roof
                      1,0,0, 1,0,1, 1,0,0, 1,1,0,
                      1,1,0, 0,1,0, 1,1,0, 0,1,0,
                      0,1,1, 0,0,1, 1,0,1, 
	
                      1,0,0, 1,1,0, 1,0,0, 1,0,1,
                      0,1,0, 0,1,0, 0,1,1, 0,1,0,
                      1,0,1, 1,0,0, 1,0,1, 1,0,0,
                      1,1,0, 0,1,0, 0,1,1, 0,1,0,
                      0,0,1, 1,0,1, 0,0,1, 0,1,1,
                      1,0,1, 0,1,1, 0,0,1, 0,0,1,
	
                      0,1,1, 0,0,1, 1,0,1,         // roof
                      1,0,0, 1,0,1, 1,0,0, 1,1,0,
                      1,1,0, 0,1,0, 1,1,0, 0,1,0,
                      0,1,1, 0,0,1, 1,0,1,

                      1,0,0, 1,1,0, 1,0,0, 1,0,1,
                      0,1,0, 0,1,0, 0,1,1, 0,1,0,
                      1,0,1, 1,0,0, 1,0,1, 1,0,0,
                      1,1,0, 0,1,0, 0,1,1, 0,1,0,
                      0,0,1, 1,0,1, 0,0,1, 0,1,1,
                      1,0,1, 0,1,1, 0,0,1, 0,0,1,

                      0,1,1, 0,0,1, 1,0,1,         // roof
                      1,0,0, 1,0,1, 1,0,0, 1,1,0,
                      1,1,0, 0,1,0, 1,1,0, 0,1,0,
                      0,1,1, 0,0,1, 1,0,1,
                     };


	// Set up the vertex data
	VertexData& vertexData = object->vertexData;

	// Specify the elements of the vertex data:
	// - one element for vertex positions
	vertexData.vertexDeclaration.addElement(0, 0, 3, 3*sizeof(float), RE167::VES_POSITION);
	// - one element for vertex colors
	vertexData.vertexDeclaration.addElement(1, 0, 3, 3*sizeof(float), RE167::VES_DIFFUSE);
	
	// Create the buffers and load the data
	vertexData.createVertexBuffer(0, nVerts*3*sizeof(float), (unsigned char*)vertices);
	vertexData.createVertexBuffer(1, nVerts*3*sizeof(float), (unsigned char*)colors);

	// The index data that stores the connectivity of the triangles
	int indices[] = {0,2,3, 0,1,2,         // front face
	                 4,6,7, 4,5,6,         // left face
	                 8,10,11, 8,9,10,      // back face
	                 12,14,15, 12,13,14,   // right face
	                 16,18,19, 16,17,18,   // top face
	                 20,22,23, 20,21,22,   // bottom face
					
	                 24,26,27, 24,25,26,   // ground floor
					
	                 28,29,30,             // roof
	                 31,33,34, 31,32,33,
	                 35,37,38, 35,36,37,
	                 39,40,41, 
					
	                 0+42, 2+42, 3+42,  0+42, 1+42, 2+42,   // front face
	                 4+42, 6+42, 7+42,  4+42, 5+42, 6+42,   // left face
	                 8+42,10+42,11+42,  8+42, 9+42,10+42,   // back face
	                 12+42,14+42,15+42, 12+42,13+42,14+42,  // right face
	                 16+42,18+42,19+42, 16+42,17+42,18+42,  // top face
	                 20+42,22+42,23+42, 20+42,21+42,22+42,  // bottom face
					
	                 28+42-28+24,29+42-28+24,30+42-28+24,   // roof
	                 31+42-28+24,33+42-28+24,34+42-28+24, 31+42-28+24,32+42-28+24,33+42-28+24,
	                 35+42-28+24,37+42-28+24,38+42-28+24, 35+42-28+24,36+42-28+24,37+42-28+24,
	                 39+42-28+24,40+42-28+24,41+42-28+24, 
					
	                 0+80,2+80,3+80, 0+80,1+80,2+80,        // front face
	                 4+80,6+80,7+80, 4+80,5+80,6+80,        // left face
	                 8+80,10+80,11+80, 8+80,9+80,10+80,     // back face
	                 12+80,14+80,15+80, 12+80,13+80,14+80,  // right face
	                 16+80,18+80,19+80, 16+80,17+80,18+80,  // top face
	                 20+80,22+80,23+80, 20+80,21+80,22+80,  // bottom face
					
	                 28+80-4,29+80-4,30+80-4,               // roof
	                 31+80-4,33+80-4,34+80-4, 31+80-4,32+80-4,33+80-4,
	                 35+80-4,37+80-4,38+80-4, 35+80-4,36+80-4,37+80-4,
	                 39+80-4,40+80-4,41+80-4, 
					
	                 0+118,2+118,3+118, 0+118,1+118,2+118,        // front face
	                 4+118,6+118,7+118, 4+118,5+118,6+118,        // left face
	                 8+118,10+118,11+118, 8+118,9+118,10+118,     // back face
	                 12+118,14+118,15+118, 12+118,13+118,14+118,  // right face
	                 16+118,18+118,19+118, 16+118,17+118,18+118,  // top face
	                 20+118,22+118,23+118, 20+118,21+118,22+118,  // bottom face
					
	                 28+118-4,29+118-4,30+118-4,                  // roof
	                 31+118-4,33+118-4,34+118-4, 31+118-4,32+118-4,33+118-4,
	                 35+118-4,37+118-4,38+118-4, 35+118-4,36+118-4,37+118-4,
	                 39+118-4,40+118-4,41+118-4, 
					};	

	vertexData.createIndexBuffer(60 + (36 + 18) * 3, indices);


}



/**
* Changes the Object passed in to be a sphere.  Changes underlying
* vertex buffers etc. of the Object.
* @param o
* @param numRows			how many rows of faces to make
* @param numFacesPerRow		the number of faces per row 
*/
void GeometryFactory::createSphere(RE167::Object *o, int numRows, int numFacesPerRow) {
	int *indices = NULL;
	float *vertices= NULL;
	float *colors = NULL;
	
	int numIndexElements = 0;
	int numVertexElements = 0;
	int numColorElements = 0;
	// Do the heavy lifting with a helper method
	GeometryFactory::createSphere(numRows, numFacesPerRow, vertices, colors, indices, 
								numVertexElements, numColorElements, numIndexElements);

	GeometryFactory::fillInObject(o, vertices, colors, indices, numVertexElements,
									numColorElements, numIndexElements);
	
	delete[] indices;
	delete[] vertices;
	delete[] colors;
}

/**
* Creates a sphere of radius 1 centered at (0,0,0).
* @param numFaceRows		how many rows of faces will appear in the sphere
* @param numFacesPerRow		how many faces will appear per row
* @param vertices			the float pointer that will be allocated
*							and filled in with the vertices of the faces 
*							in this method
* @param colors				the float pointer that will be allocated and
*							filled in with the colors of the vertices
* @param indices			will be created within this method and filled
*							with the connectivity information needed to render 
*							the triangles
* @param sizeOfVerticesArray	will be changed to reflect how many elements
*							are stored within the vertices array
* @param sizeOfColorsArray	will be changed to reflect how many elements
*							are stored within the colors array
* @param sizeOfIndicesArray will be changed to reflect how many elements
*							are stored within the indices array
*/
void GeometryFactory::createSphere(int numFaceRows,
								   int numFacesPerRow,
                                   float *&vertices,
                                   float *&colors,
                                   int *&indices,
                                   int &sizeOfVerticesArray,
                                   int &sizeOfColorsArray,
                                   int &sizeOfIndicesArray) {

    
    assert(numFaceRows >= 1);
	assert(numFacesPerRow >= 3);

    // Even with just one row of faces, you need two rows of vertices
    const int NUM_VERTICES_ROWS = numFaceRows + 1;
    const int NUM_VERTICES_PER_ROW = numFacesPerRow;

    // Make a temporary array of Vectors to store the vertices
    Vector3 * tempVertices = new Vector3[(NUM_VERTICES_ROWS + 1) *
                                         NUM_VERTICES_PER_ROW];

   
    // Here is the idea:  We will calculate where all the vertices in the
    // sphere are by "cutting" the sphere parallel to the xy plane and looking
    // at the level curves (circles) at each point.
    for (int row = 0, count = 0; row < NUM_VERTICES_ROWS; row++) {
        // We do not want to equally space our rows of vertices because we need more
		// detail at the top and bottom of sphere and less in the middle.  Space
		// the rows based off of a circle.  Imagine drawing a ray from the center of
		// the circle to the top most point, and then rotating the ray along the circle.
		// The height of the ray at each time step will be the height of our row.
		float percent = static_cast<float>(row) / (NUM_VERTICES_ROWS - 1);
		// Start ray at top, trace down
		float alpha = PI/2 + PI * percent;
		float y = sin(alpha);

		
        // The radius of the circle formed by cutting the sphere at this
        // height is also constant
        float radius = static_cast<float>( cos( asin(y) ) );

        // Each level is a circle sampled at distinct points
        for (int j = 0; j < NUM_VERTICES_PER_ROW; j++) {
            // How far around the circle are we?
            float theta = static_cast<float>( j * ( TWO_PI / NUM_VERTICES_PER_ROW) );
            float x = radius * sin(theta);
            float z = radius * cos(theta);

            // Store the calculate vertex within the temp array
            tempVertices[count] = Vector3(x, y, z);
            count++;
        }
    }

    // Our temporary array holds every point on the sphere.  Because we want
    // every face to be a different color, we need to duplicate some of the
    // vertices.

    const int NUM_FACES = numFaceRows * numFacesPerRow;
    const int NUM_VERTICES =
            NUM_FACES * NUM_TRIANGLES_PER_RECTANGULAR_FACE * NUM_VERTICES_PER_TRIANGLE;

    // Caller needs to be able to assertain size of array we will create
    sizeOfVerticesArray = NUM_VERTICES * NUM_COMPONENTS_PER_VERTEX;
    vertices = new float[sizeOfVerticesArray];

    const int NUM_COMPONENTS_PER_ROW =
        NUM_COMPONENTS_PER_RECTANGULAR_FACE * numFacesPerRow;

    // The basic strategy will be to go row by row and match
    // the vertices on one layer with those on the next layer, creating
    // faces.  Since we need to deal with triangular faces, each rectangular
    // face will be subdivided into two triangles.  Note that since each
    // pass deals with the current row of vertices and one below it, we stop
    // one row from the end 
    for (int row = 0; row < numFaceRows; row++) {

        for (int face = 0; face < numFacesPerRow; face++) {
            // The rectangular face we are dealing with is formed from the
            // "face"th and ("face"+1)th vertex on level "row",
            // and the "face"th and ("face"+1)th
            // vertex on level "row" + 1.
            // We need to be careful to declare our vertices in counter
            // clockwise order
			
			// Face 1: The upper right triangle of the rectangular face
            // Vertex 1: Upper left corner
            Vector3 f1_1 = tempVertices[row * NUM_VERTICES_PER_ROW + face];
            // Vertex 2: Lower right corner.  
            Vector3 f1_2 = tempVertices[(row + 1) * NUM_VERTICES_PER_ROW + ((face + 1) % NUM_VERTICES_PER_ROW)];
            // Vertex 3: Upper right corner
			Vector3 f1_3 = tempVertices[(row * NUM_VERTICES_PER_ROW) + ((face + 1) % NUM_VERTICES_PER_ROW)];

            // Face 2: The lower left triangle of the rectangular face
            // Vertex 1: The upper left corner (same as f1_1)
            Vector3 f2_1 = f1_1;
            // Vertex 2: Lower left corner
            Vector3 f2_2 = tempVertices[(row + 1) * NUM_VERTICES_PER_ROW + face];
            // Vertex 3: Lower right corner: same as f1_2
            Vector3 f2_3 = f1_2;

            // Now we need to actually fill in the vertices array with the
            // raw float values from our vertices.  We are filling in
            // 6 vertices, for a total of 18 floats added to the array per
            // iteration of this inner loop
            int startIndex = (row * NUM_COMPONENTS_PER_ROW) + (NUM_COMPONENTS_PER_RECTANGULAR_FACE * face);
   
            vertices[startIndex]     = f1_1.getX();
            vertices[startIndex + 1] = f1_1.getY();
            vertices[startIndex + 2] = f1_1.getZ();

            vertices[startIndex + 3] = f1_2.getX();
            vertices[startIndex + 4] = f1_2.getY();
            vertices[startIndex + 5] = f1_2.getZ();

            vertices[startIndex + 6] = f1_3.getX();
            vertices[startIndex + 7] = f1_3.getY();
            vertices[startIndex + 8] = f1_3.getZ();

            vertices[startIndex + 9]  = f2_1.getX();
            vertices[startIndex + 10] = f2_1.getY();
            vertices[startIndex + 11] = f2_1.getZ();

            vertices[startIndex + 12] = f2_2.getX();
            vertices[startIndex + 13] = f2_2.getY();
            vertices[startIndex + 14] = f2_2.getZ();

            vertices[startIndex + 15] = f2_3.getX();
            vertices[startIndex + 16] = f2_3.getY();
            vertices[startIndex + 17] = f2_3.getZ();
		}
    }
   
    // Our vertices array is fully filled in; no need for the temporary one
    delete[] tempVertices;

    // Create the color array.  Each triangle section of each face will be
    // a separate color
	sizeOfColorsArray = NUM_VERTICES * ColorFactory::NUM_COMPONENTS_PER_COLOR;
    colors = new float[sizeOfColorsArray];
    ColorFactory::randomlyColorize(colors, sizeOfColorsArray);


    // Create the connectivity array, which says how the vertices are
    // joined together to create faces.  This array is set up so that
    // if the first three elements of it are i1, i2, and i3, OpenGL will
    // create a triangular face from the i1th vertex, i2th vertex, and i3th
    // vertex as defined in the vertices array.
    sizeOfIndicesArray = NUM_VERTICES;
    indices = new int[sizeOfIndicesArray];
    for (int i = 0; i < sizeOfIndicesArray; i++) {
        indices[i] = i;
    }
}







void GeometryFactory::createCylinder(RE167::Object *o, int numRows,
									 int numFacesPerRow, float radius) {
	GeometryFactory::createTaperedCylinder(o, numRows, numFacesPerRow, radius, radius);
}





void GeometryFactory::createCylinder(int numRows,
									int numFacesPerRow,
									float radius,
									float *&vertices,
									float *&colors,
									int *&indices,
									int &sizeOfVerticesArray,
									int &sizeOfColorsArray,
									int &sizeOfIndicesArray) {

	GeometryFactory::createTaperedCylinder(numRows,
											numFacesPerRow,
											radius,
											radius,
											vertices,
											colors,
											indices,
											sizeOfVerticesArray,
											sizeOfColorsArray,
											sizeOfIndicesArray);
}



void GeometryFactory::createCone(RE167::Object *o, int numRows,
								 int numFacesPerRow, float bottomRadius) {
	
	GeometryFactory::createTaperedCylinder(o, numRows, numFacesPerRow, 0, bottomRadius);
}

void GeometryFactory::createCone(int numRows,
									int numFacesPerRow,
									float bottomRadius,
									float *&vertices,
									float *&colors,
									int *&indices,
									int &sizeOfVerticesArray,
									int &sizeOfColorsArray,
									int &sizeOfIndicesArray) {

	GeometryFactory::createTaperedCylinder(numRows,
											numFacesPerRow,
											0,
											bottomRadius,
											vertices,
											colors,
											indices,
											sizeOfVerticesArray,
											sizeOfColorsArray,
											sizeOfIndicesArray);
}


/**
* Wrapper method for underlying createTaperedCylinder method.
* Fills in Object o with the geometry and colors of the tapered cylinder
* created from its arguments.
* @param o				the object to fill in
* @param numRows		the number of rows 
* @param numFacesPerRow	number of faces per row
* @param topRadius		radius of top of cylinder
* @param bottomRadius	radius of bottom of cylinder
*/
void GeometryFactory::createTaperedCylinder(RE167::Object *o, int numRows,
									int numFacesPerRow, float topRadius,
									float bottomRadius) {

	int *indices = NULL;
	float *vertices= NULL;
	float *colors = NULL;
	
	int numIndexElements = 0;
	int numVertexElements = 0;
	int numColorElements = 0;
	// Do the heavy lifting with a helper method
	GeometryFactory::createTaperedCylinder(numRows, 
									numFacesPerRow, 
									topRadius, 
									bottomRadius, 
									vertices, 
									colors, 
									indices, 
									numVertexElements, 
									numColorElements, 
									numIndexElements);

	GeometryFactory::fillInObject(o, vertices, colors, indices, numVertexElements,
									numColorElements, numIndexElements);
	
	delete[] indices;
	delete[] vertices;
	delete[] colors;

}


/**
* Creates a tapered cylinder whose base has radius bottomRadius
* and top has radius topRadius.  Height of 1, centered at
* (0,0,0).
* @param numRows	how many rows of faces to have
* @param numFacesPerRow	how many rectangular faces per row
* @param topRadius	the radius of the top of the cylinder
* @param bottomRadius	the radius of the bottom of the cylinder
* @param vertices	the array of vertices that will be allocated and
*					filled in
* @param colors		the array of colors that will be allocated and filled
*					in
* @param indices	the array of indices that will be allocated and 
*					filled in; represents the connectivity of the
*					vertices
* @param sizeOfVerticesArray	will be changed to reflect the length
*					of the vertices array
* @param sizeOfColorsArray	will be changed to reflect the length
*					of the colors array
* @param sizeOfIndicesArray	will be changed to reflect the length
*					of the indices array
*/
void GeometryFactory::createTaperedCylinder(int numRows,
									int numFacesPerRow,
									float topRadius,
									float bottomRadius,
									float *&vertices,
									float *&colors,
									int *&indices,
									int &sizeOfVerticesArray,
									int &sizeOfColorsArray,
									int &sizeOfIndicesArray) {

	// We need at least one row
	assert(numRows >= 1);
	// We can't really approximate a circle with fewer than 3 vertices
	assert(numFacesPerRow >= NUM_VERTICES_PER_TRIANGLE);

	assert(bottomRadius >= 0);
	assert(topRadius >= 0);

	const float HEIGHT = 1.0f;
	// Since we are centering the object at (0,0,0)
	const float HIGHEST_Y_VALUE = HEIGHT/2;
	const float LOWEST_Y_VALUE = -HIGHEST_Y_VALUE;

	const int NUM_ROWS_VERTICES = numRows + 1;

	// The center of the circle must be included numFacesPerRow times.
	const int NUM_CENTER_POINTS = numFacesPerRow;
	// Each vertex along the top must be included twice - once
	// for each triangular face that shares it
	const int NUM_TOP_VERTICES = NUM_CENTER_POINTS + 2 * numFacesPerRow;
	
	// The bottom has exactly as many vertices as does the top
	const int NUM_BOTTOM_VERTICES = NUM_TOP_VERTICES;

	const int NUM_RECTANGULAR_FACES = numFacesPerRow * NUM_ROWS_VERTICES;
	
	const int NUM_MID_VERTICES =	numFacesPerRow * 
									numRows * 
									NUM_TRIANGLES_PER_RECTANGULAR_FACE * 
									NUM_VERTICES_PER_TRIANGLE;
	
	const int TOTAL_NUM_VERTICES =	NUM_TOP_VERTICES + 
									NUM_MID_VERTICES + 
									NUM_BOTTOM_VERTICES;

	
	const int NUM_COMPONENTS_PER_ROW =
        NUM_COMPONENTS_PER_RECTANGULAR_FACE * numFacesPerRow;


	sizeOfVerticesArray = TOTAL_NUM_VERTICES * NUM_COMPONENTS_PER_VERTEX;
	sizeOfColorsArray = sizeOfVerticesArray;
	sizeOfIndicesArray = TOTAL_NUM_VERTICES;

	vertices = new float[sizeOfVerticesArray];
	colors = new float[sizeOfColorsArray];
	indices = new int[sizeOfIndicesArray];

	assert(vertices != NULL);
	assert(colors != NULL);
	assert(indices != NULL);

	// Calculate all of the vertices once, and then from this temporary
	// array, copy them multiple times as necessary so that each face
	// can have its own color.
	Vector3 * tempVertices = new Vector3[NUM_ROWS_VERTICES * numFacesPerRow];
	assert(tempVertices != NULL);

	// How much does the radius change between top and bottom?
	const float RADIUS_CHANGE = bottomRadius - topRadius;

	// Divide the conical shape into NUM_ROWS_VERTICES slices
	// Draw from top to bottom
	for (int row = 0, count = 0; row < NUM_ROWS_VERTICES; row++) {
		// Calculate the height of the row of vertices.
		float proportionDown = static_cast<float>(row) / (NUM_ROWS_VERTICES - 1);
		float y = HIGHEST_Y_VALUE - (HEIGHT * proportionDown);

		// Calculate the radius of the circle at this height.  We linearly
		// interpolate between the two radii
		float radius = (RADIUS_CHANGE * proportionDown) + topRadius;

		// For each vertex along the circumference of the circle
		for (int j = 0; j < numFacesPerRow; j++) {
			// What angle must we rotate by to reach this vertex?
			float proportionAround = static_cast<float> (j) / (numFacesPerRow - 1);
			float theta = proportionAround * TWO_PI;

			// Calculate x and z coordinate.
			float x = radius * sin(theta);
			float z = radius * cos(theta);

			// Assign the (x,y,z) triple
			tempVertices[count] = Vector3(x,y,z);
			count++;
		}
	}

	// From the vertices we have calculated, we need to duplicate some since 
	// multiple faces have vertices in common and we want to be able to 
	// set each face's color individually
	
	// The top of the cylindrical column
	for (int i = 0; i < numFacesPerRow; i++) {
		// Need to make sure we go counterclockwise.
		int tempVertex1Index = i;
		int tempVertex2Index = (i + 1) % numFacesPerRow;
		
		Vector3 vertex1 = tempVertices[tempVertex1Index];
		Vector3 vertex2 = tempVertices[tempVertex2Index];
		Vector3 center  = Vector3(0, HIGHEST_Y_VALUE, 0);


		// Assign the three vertices to their position in the array
		int vertex1Index = (NUM_COMPONENTS_PER_VERTEX * NUM_VERTICES_PER_TRIANGLE) * i;
		int vertex2Index = vertex1Index + NUM_COMPONENTS_PER_VERTEX;
		int vertex3Index = vertex2Index + NUM_COMPONENTS_PER_VERTEX;

		// Assign vertex 1
		vertices[vertex1Index    ] = vertex1.getX();
		vertices[vertex1Index + 1] = vertex1.getY();
		vertices[vertex1Index + 2] = vertex1.getZ();

		// Assign vertex 2
		vertices[vertex2Index    ] = vertex2.getX();
		vertices[vertex2Index + 1] = vertex2.getY();
		vertices[vertex2Index + 2] = vertex2.getZ();

		// Assign vertex 3
		vertices[vertex3Index    ] = center.getX();
		vertices[vertex3Index + 1] = center.getY();
		vertices[vertex3Index + 2] = center.getZ();
	}

	const int topOffset = NUM_TOP_VERTICES * NUM_COMPONENTS_PER_VERTEX;

	// The middle faces.  For each interation we connect
	// a single band of faces (rectangular faces that have
	// been triangulated)
	for (int row = 0; row < numRows; row++) {
		

		// The "face"th and ("face"+1)%numFacesPerRow vertices in row "row"
		// form a rectangular face with the "face"th and ("face"+1)%numFacesPerRow
		// vertices in row "row" + 1.
		for (int face = 0; face < numFacesPerRow; face++) {

			// Face 1: The upper right triangle of the rectangular face
            // Vertex 1: Upper left corner
            Vector3 f1_1 = tempVertices[(row * numFacesPerRow) + face];
            // Vertex 2: Lower right corner.  
            Vector3 f1_2 = tempVertices[((row + 1) * numFacesPerRow) + ((face + 1) % numFacesPerRow)];
            // Vertex 3: Upper right corner
			Vector3 f1_3 = tempVertices[(row * numFacesPerRow) + ((face + 1) % numFacesPerRow)];


            // Face 2: The lower left triangle of the rectangular face
            // Vertex 1: The upper left corner (same as f1_1)
            Vector3 f2_1 = f1_1;
            // Vertex 2: Lower left corner
            Vector3 f2_2 = tempVertices[((row + 1) * numFacesPerRow) + face];
            // Vertex 3: Lower right corner: same as f1_2
            Vector3 f2_3 = f1_2;

			// Now we need to actually fill in the vertices array with the
            // raw float values from our vertices.  We are filling in
            // 6 vertices, for a total of 18 floats added to the array per
            // iteration of this inner loop.  We also need to note that we
			// have added a bunch of elements already due to the top of the cylinder
            int startIndex = (row * NUM_COMPONENTS_PER_ROW) + (NUM_COMPONENTS_PER_RECTANGULAR_FACE * face)
							+ topOffset; 

            vertices[startIndex]     = f1_1.getX();
            vertices[startIndex + 1] = f1_1.getY();
            vertices[startIndex + 2] = f1_1.getZ();

            vertices[startIndex + 3] = f1_2.getX();
            vertices[startIndex + 4] = f1_2.getY();
            vertices[startIndex + 5] = f1_2.getZ();

            vertices[startIndex + 6] = f1_3.getX();
            vertices[startIndex + 7] = f1_3.getY();
            vertices[startIndex + 8] = f1_3.getZ();

            vertices[startIndex + 9]  = f2_1.getX();
            vertices[startIndex + 10] = f2_1.getY();
            vertices[startIndex + 11] = f2_1.getZ();

            vertices[startIndex + 12] = f2_2.getX();
            vertices[startIndex + 13] = f2_2.getY();
            vertices[startIndex + 14] = f2_2.getZ();

            vertices[startIndex + 15] = f2_3.getX();
            vertices[startIndex + 16] = f2_3.getY();
            vertices[startIndex + 17] = f2_3.getZ();
		}
	}


	const int midOffset = topOffset + NUM_MID_VERTICES * NUM_COMPONENTS_PER_VERTEX;


	// The bottom of the cylindrical column
	// Exactly the same as the top part, except that we need to give the
	// vertices in a different order so that the counterclockwise order is
	// maintained, given that it needs to be visible from the bottom
	for (int i = 0; i < numFacesPerRow; i++) {
		// Need to make sure we go counterclockwise.  But we're viewing it
		// from the bottom this time
		int tempVertex1Index = (numFacesPerRow * numRows) + (i + 1) % numFacesPerRow;
		int tempVertex2Index = (numFacesPerRow * numRows) + i;

		Vector3 vertex1 = tempVertices[tempVertex1Index];
		Vector3 vertex2 = tempVertices[tempVertex2Index];
		Vector3 center  = Vector3(0, LOWEST_Y_VALUE, 0);


		// Assign the three vertices to their position in the array
		int vertex1Index = midOffset + (NUM_COMPONENTS_PER_VERTEX * NUM_VERTICES_PER_TRIANGLE) * i;
		int vertex2Index = vertex1Index + NUM_COMPONENTS_PER_VERTEX;
		int vertex3Index = vertex2Index + NUM_COMPONENTS_PER_VERTEX;

		// Assign vertex 1
		vertices[vertex1Index    ] = vertex1.getX();
		vertices[vertex1Index + 1] = vertex1.getY();
		vertices[vertex1Index + 2] = vertex1.getZ();

		// Assign vertex 2
		vertices[vertex2Index    ] = vertex2.getX();
		vertices[vertex2Index + 1] = vertex2.getY();
		vertices[vertex2Index + 2] = vertex2.getZ();

		// Assign vertex 3
		vertices[vertex3Index    ] = center.getX();
		vertices[vertex3Index + 1] = center.getY();
		vertices[vertex3Index + 2] = center.getZ();
	}

	// Fill in the color arrays
	ColorFactory::colorize(colors, sizeOfColorsArray);
	
	// Fill in the connectivity arrays
	for (int i = 0; i < sizeOfIndicesArray; i++) {
		indices[i] = i;
	}

	// We no longer need the temparray
	delete[] tempVertices;
}



// HELPER METHODS


/* Helper method to get vertexData, create buffers for the vertices, indices, colors, and then
 * fill them in
 * @param o
 * @param vertices			the (x,y,z) locations of all the vertices
 * @param colors			the (r,g,b) values at each vertex
 * @param indices			the indices into the vertices array that make up each triangular face
 * @param numVertexElements the size of the vertices array
 * @param numColorElements	the size of the colors array
 * @param numIndexElements	the size of the indices array
 */
void GeometryFactory::fillInObject(RE167::Object *o, float *vertices, float *colors, int *indices,
								   int numVertexElements, int numColorElements, int numIndexElements) {
	VertexData& vertexData = o->vertexData;
	// one element for vertices
	vertexData.vertexDeclaration.addElement(0, 0, 3, 3*sizeof(float), RE167::VES_POSITION);
	// - one element for vertex colors
	vertexData.vertexDeclaration.addElement(1, 0, 3, 3*sizeof(int), RE167::VES_DIFFUSE);

	vertexData.createVertexBuffer(0, numVertexElements * sizeof(float), (unsigned char*) vertices);
	vertexData.createVertexBuffer(1, numColorElements * sizeof(float), (unsigned char*) colors);

	vertexData.createIndexBuffer(numIndexElements, indices);
}

/** Debugging method that prints all the vertices out neatly
* @param verticesArray	the vertices to print
* @param numElements	the length of the verticesArray
* @param entriesPerLine	how many entries to print per line
*/
void GeometryFactory::printVerticesArray(float *verticesArray, const int numElements, const int entriesPerLine) {
	
	for (int i = 0, count = 0; i < numElements/3; i++) {
		if (i % entriesPerLine == 0) {
			std::cout<<std::endl;
		}
		std::cout<< "(" << verticesArray[3 * i] << ", " << verticesArray[(3 * i) + 1] << ", " << verticesArray[(3 * i) + 2] << ")";
		count++;
	}

}

/** Debugging method that prints all the vertices saved as Vector3 objects neatly
* @param vectorArray	the vertices to print
* @param numElements	the length of the verticesArray
* @param entriesPerLine	how many entries to print per line
*/
void GeometryFactory::printVectorArray(Vector3 *vectorArray, const int numElements, int entriesPerLine) {
	for (int i = 0, count = 0; i < numElements; i++) {
		if (i % entriesPerLine == 0) {
			std::cout<<std::endl;
		}
		std::cout<< vectorArray[i];
		count++;
	}
}
