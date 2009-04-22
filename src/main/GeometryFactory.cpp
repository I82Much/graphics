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
#include "Vector4.h"
#include "ColorFactory.h"
#include "ObjReader.h"
#include "PGMReader.h"
#include <map>
#include <vector>
#include "VertexData.h"
#include "Object.h"
#include "BezierCurve.h"

using std::cout;
using std::endl;
using namespace RE167;

#include "BasicMath.h"

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
* object
**/
void GeometryFactory::createTerrainFromPGM(Object *o, char * filepath, bool normalize) 
{
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

    // we have not calculated normals for the terrain; ignore.
	GeometryFactory::fillInObject(o, vertices, NULL, colors, indices,
									numVertices,
									numVertices);

}


/**
* Given an Object and a filepath, load the triangle mesh given
* in the file and save the vertex data to the Object.
* Based on sample code provided by Prof. Palacio.
* @param o			the Object to fill with vertex data
* @param filePath	the path to the .obj file
* @param normalize	whether or not to make the object fit into unit cube centered at origin
**/
void GeometryFactory::createObject(Object *o, char * filepath, bool normalize) {
	int numVertices;
	float *vertices;
	float *normals;
	float *texcoords;
	int numIndices;
	int *indices;

	ObjReader::readObj(filepath, numVertices, &vertices, &normals, &texcoords, numIndices, &indices);

	if (normalize) {
		// Scale all the vertices to fit within the space.
		ObjReader::normalize(vertices, numVertices);
	}

	float *colors = new float[3 * numVertices];
	//ColorFactory::matchNormalizedVertices(colors, vertices, 3 * numVertices);
	//ColorFactory::randomlyColorize(colors, 3 * numVertices);
    //std::fill(&colors[0], &colors[3 * numVertices], 1.0f);


    // Calculate the bounding sphere and save it to the object
     float radius;
     Vector4 center;
     calculateBoundingSphere(vertices, numVertices, center, radius);
     o->setSphereCenter(center);
     o->setSphereRadius(radius);


	VertexData& vertexData = o->vertexData;
	vertexData.vertexDeclaration.addElement(0, 0, 3, 3*sizeof(float), RE167::VES_POSITION);
	vertexData.createVertexBuffer(0, numVertices*3*sizeof(float), (unsigned char*)vertices);

	/*vertexData.vertexDeclaration.addElement(1, 0, 3, 3*sizeof(float), RE167::VES_DIFFUSE);
	vertexData.createVertexBuffer(1, numVertices*3*sizeof(float), (unsigned char*)colors);
    */

	if(normals)
	{
		vertexData.vertexDeclaration.addElement(2, 0, 3, 3*sizeof(float), RE167::VES_NORMAL);
		vertexData.createVertexBuffer(2, numVertices *3*sizeof(float), (unsigned char*)normals);
	}
	// Calculate the normals since they weren't included on object
	else {
	    
        calculateSphericalNormals(vertices, normals, indices, numVertices, numIndices);
	    
        //        calculateNormals(vertices, indices, normals, numVertices, numIndices);
        assert(normals);

        vertexData.vertexDeclaration.addElement(2, 0, 3, 3*sizeof(float), RE167::VES_NORMAL);
        vertexData.createVertexBuffer(2, numVertices * 3 * sizeof(float), (unsigned char*)normals);

        std::cout << "non normals num vertices: " << numVertices << " num indices:" << numIndices << std::endl;
	}
    
    float * texCoords = NULL;
    //createSphericalCoordinates(vertices, normals, indices, texCoords, numVertices, numIndices);
    createPositionalSphericalCoordinates(vertices, indices, texCoords, numVertices, numIndices);
    assert(texCoords != NULL);

    vertexData.vertexDeclaration.addElement(3, 0, 2, 2*sizeof(float),
        RE167::VES_TEXTURE_COORDINATES);
    vertexData.createVertexBuffer(3, numVertices * 2*sizeof(float), (unsigned char*) texCoords);



	vertexData.createIndexBuffer(numIndices, indices);

	if(normals) {
	    delete[] normals;
        normals = NULL;
    }
	if(texcoords) {
	    delete[] texcoords;
        texcoords = NULL;
    }
	delete[] vertices;
    vertices = NULL;
	delete[] indices;
    indices = NULL;
	delete[] colors;
    colors = NULL;
}



/**
*
*/
void GeometryFactory::calculateSphericalNormals(float *vertices, 
                                            float *&normals, 
                                            int *indices, 
                                            int numVertices, 
                                            int numIndices) {
    // Calculate the bounding box around the object
    Vector3 vMin;
    Vector3 vMax;
    calculateBoundingBox(vertices, numVertices, vMin, vMax);
    
    normals = new float[numVertices * 3];
    
    // Calculate the center; just the average of the bounding box
    // coordinates
    Vector3 center = 0.5f * (vMax + vMin);

    for (int i = 0; i < numVertices; i++) {
        int index = 3 * i;
        float x = vertices[index    ];
        float y = vertices[index + 1];
        float z = vertices[index + 2];
        
        Vector3 position(x, y, z);
        Vector3 vertexRay = (position - center).normalize();
       
        fillInVertex(normals, index, vertexRay);
    }
}


/**
* Given the geometry of an object, calculate the texture coordinates for
* each vertex in the mesh.  Calling class is responsible for delete[]ing
* the texCoords array that is allocated within this method.
*
*
* {@link http://www.mvps.org/directx/articles/spheremap.htm}
* @param vertices       an array representing the vertices of the mesh.
*                       Must not be NULL.
* @param normals        an array representing the unit normal vectors at
*                       each vertex.  Must not be NULL.
* @param indices        an array mapping the faces of mesh to the vertices.
*                       Must not be NULL.
* @param texCoords      the array that will be allocated within this method
*                       to hold all of the (u,v) texture coordinates for
*                       each vertex in the mesh
* @param numVertices    how many vertices are in the mesh (vertices array will
*                       have 3 times this number worth of elements)
* @param numIndices     the size of the indices array; there will be exactly
*                       numIndices / 3 faces on the mesh
**/
void GeometryFactory::createSphericalCoordinates(float *vertices,
                                                float *normals,
                                                int *indices,
                                                float *&texCoords,
                                                int numVertices,
                                                int numIndices)
{
    assert(vertices != NULL);
    assert(indices != NULL);
    assert(normals != NULL);

    // Each texture coordinate is two dimensional
    texCoords = new float[numIndices * 2];

    // For each vertex in the mesh, calculate
    for (int i = 0; i < numIndices; i++) {
        int index = indices[i];

        float x = normals[3 * index    ];
        float y = normals[3 * index + 1];

        float tu = asin(x)/PI + 0.5f;
        float tv = asin(y)/PI + 0.5f;

        //std::cout << "(" << x << "," << y << "): " << tu << " , " << tv << std::endl;

        int textureIndex = 2 * i;
        texCoords[textureIndex] = tu;
        texCoords[textureIndex + 1] = tv;
    }
}

/**
* Given the geometry of an object, calculate the texture coordinates for
* each vertex in the mesh.  Calling class is responsible for delete[]ing
* the texCoords array that is allocated within this method.
* Used when the the object in question has large flat faces.
* 
* Modified version of algorithm described here
* {@link http://blogs.msdn.com/coding4fun/archive/2006/10/31/912562.aspx}
* {@link http://www.cs.unc.edu/~rademach/xroads-RT/RTarticle.html#texturemap}
* {@link http://www.mvps.org/directx/articles/spheremap.htm}
* @param vertices       an array representing the vertices of the mesh.
*                       Must not be NULL.
* @param indices        an array mapping the faces of mesh to the vertices.
*                       Must not be NULL.
* @param texCoords      the array that will be allocated within this method
*                       to hold all of the (u,v) texture coordinates for
*                       each vertex in the mesh
* @param numVertices    how many vertices are in the mesh (vertices array will
*                       have 3 times this number worth of elements)
* @param numIndices     the size of the indices array; there will be exactly
*                       numIndices / 3 faces on the mesh
**/
void GeometryFactory::createPositionalSphericalCoordinates(float *vertices,
                                                    int *indices,
                                                    float *&texCoords,
                                                    int numVertices,
                                                    int numIndices)
{
    assert(vertices != NULL);
    assert(indices != NULL);


    // Calculate the bounding box around the object
    Vector3 vMin;
    Vector3 vMax;

    calculateBoundingBox(vertices, numVertices, vMin, vMax);

    // Calculate the center; just the average of the bounding box
    // coordinates
    Vector3 center = 0.5f * (vMax + vMin);

    // The north vector is a vector from center of object to north pole
    Vector3 north(0, 1, 0);
    // A point on the equator of the sphere
    Vector3 equator(1, 0, 0);
    Vector3 northCrossEquator = north.crossProduct(equator);
    
    // Each texture coordinate is two dimensional
    texCoords = new float[numIndices * 2];

    // For each vertex in the mesh, calculate the unit vector from
    // center of object to vertex. Use this to calculate the longitude
    // and latitude of point, and convert it into range [-1,1] in u coord,
    // [0,1] in v coord
    for (int i = 0, counter = 0; i < numIndices; i++) {
        int index = indices[i];
        
        float x = vertices[3 * index    ];
        float y = vertices[3 * index + 1];
        float z = vertices[3 * index + 2];

        Vector3 position(x,y,z);
        
        // Calculate the vector from the center of the sphere to the
        // position of vertex in question
        Vector3 vertexRay = (position - center).normalize();

        // Calculate the latitude - simply the angle between the equator
        // and our vertexRay
        double phi = acos(-north.dotProduct(vertexRay));
        // Make sure v is in range [0,1]
        float tv = phi / PI;
        
        float tu;
        
        // Calculate the longitude
        float theta = ( acos( vertexRay.dotProduct(equator) / sin( phi )) ) / ( 2 * PI);
        // Ensure that the both sides of the sphere have different textures.
        // Maps the u coordinate to be in range [-1, 1]
        if ( vertexRay.dotProduct(northCrossEquator) > 0) {
            tu = theta;
         }
        else {
            tu = 1 - theta;
        }
        
        
        texCoords[counter++] = tu;
        texCoords[counter++] = tv;
    }

}

/**
* Given an array of vertices defining an object, calculates the smallest
* axis-aligned bounding box that fits all the points.
* Has the side effect of changing vMin to contain the smallest x, y, and z
* value in the point set, and vMax to contain the largest x, y, and z values.
**/
void GeometryFactory::calculateBoundingBox(float *vertices, int numVertices,
                                            Vector3 &vMin,
                                            Vector3 &vMax)
{
    
    float xMin, yMin, zMin;
	float xMax, yMax, zMax;

	// Initialize the mins/maxes
	xMin = xMax = vertices[0];
	yMin = yMax = vertices[1];
	zMin = zMax = vertices[2];


	// Each vertex consists of 3 floats, (x,y,z)
	for (int i = 0; i < numVertices; i++) {
		int startIndex = 3 * i;

		float x = vertices[startIndex];
		float y = vertices[startIndex + 1];
		float z = vertices[startIndex + 2];

		if (x < xMin) { xMin = x; }
		if (y < yMin) { yMin = y; }
		if (z < zMin) { zMin = z; }

		if (x > xMax) { xMax = x; }
		if (y > yMax) { yMax = y; }
		if (z > zMax) { zMax = z; }
		
		//std::cout << "Vertex " << i << ": " << Vector3(x,y,z) << std::endl;
		//std::cout << "xMin: " << xMin << " yMin: " << yMin << " zMin: " << zMin << std::endl;

	}
    vMin.setX(xMin);
    vMin.setY(yMin);
    vMin.setZ(zMin);

    vMax.setX(xMax);
    vMax.setY(yMax);
    vMax.setZ(zMax);
}

/**
* Given an array of vertices defining and object, calculates a bounding sphere
* containing all of the vertices within it.  Note that this is not guaranteed
* to find the absolute minimal bounding sphere, but it will be close in most
* cases.
* @param vertices       the vertices making up the object
* @param numVertices    how many vertices there are
* @param center         will be changed to reflect the center of the bounding
*                       sphere
* @param radius         will be changed to reflect the radius of the bounding
*                       sphere
**/
void GeometryFactory::calculateBoundingSphere(float * vertices, int numVertices, 
    Vector4 &center, float & radius)
{

    Vector3 vMin;
    Vector3 vMax;
    
    calculateBoundingBox(vertices, numVertices, vMin, vMax);
    // The average of the corners of box gives center
    Vector3 center3d = 0.5f * (vMin + vMax);
    // Homogenize
    center = Vector4(center3d.getX(), center3d.getY(), center3d.getZ(), 1);
    // Both vMax and vMin will be equally close to the center of the bounding
    // box, but they are also the farthest possible points from the center of
    // the box.  Thus the bounding sphere can safely hold all of the points
    // if we set its radius to be equal to the distance between the center
    // and the farthest corner.
    radius = (vMax - center3d).magnitude();
}

/**
* Given a triangular mesh defined by vertices and indices where there
* is a one to one correspondence between the vertices and the indices
* (vertices that are shared by different faces are duplicated rather
* than reused) this method determines which vertices are duplicates,
* eliminates them, and updates the indices array such that the
* connectivity of the mesh is maintained.  Also assumes that the
* indices array is created such that it's full of consecutive integers
* (so face one is comprised of vertices 0, 1, 2, face 2 by 3, 4, 5, ...)
*
*
* CALLER IS RESPONSIBLE FOR FREEING MEMORY ALLOCATED IN THIS METHOD
* FOR outVertices and outIndices
*/
void GeometryFactory::eliminateDuplicateVertices(float *vertices,
                                                int *indices,
                                                float *&outVertices,
                                                int *&outIndices,
                                                int &numVertices,
                                                int &numIndices)
{

    assert(vertices);
    assert(indices);
    // We only want to eliminate duplicate vertices in the case that
    // the mesh was defined such that each vertex has one entry in index
    // table
    assert(numVertices == numIndices);

    // Create space for the new indices table
    outIndices = new int[numIndices];


    // Create a vector of Vector3s; will hold all of the unique vertices
    // in the order in which we add them
    std::vector<Vector3> uniqueSortedVertices;

    // Create a map of Vector3s; will hold all of the unique vertices but
    // with no particular useful order.  Holds the index where we first
    // found the unique vertex
    std::map<Vector3, int> uniqueVerticesMap;

    // Convert our float array into a Vector3 vector.
    std::vector<Vector3> verticesVector;
    for (int i = 0; i < numVertices; i++) {
        int index = 3*i;
        float x = vertices[index    ];
        float y = vertices[index + 1];
        float z = vertices[index + 2];
        verticesVector.push_back(Vector3(x,y,z));

        std::cout << Vector3(x,y,z) << std::endl;

    }




    // Go through and determine the unique vertices.  At the same time
    // keep track of the indices into our unique vertices vector.
    for (unsigned int i = 0; i < verticesVector.size(); i++) {
        Vector3 vertex = verticesVector[i];
        // Search our map for this vertex
        std::map<Vector3, int>::iterator v = uniqueVerticesMap.find(vertex);

        // The vertex has not yet been put in our map; add it to both
        // the map and the uniqueSortedVertices vector
        if (v == uniqueVerticesMap.end()) {


            int indexInUniqueVertices = uniqueSortedVertices.size();
            // Store the index in the map for later retrieval
            uniqueVerticesMap[vertex] = indexInUniqueVertices;
            uniqueSortedVertices.push_back(vertex);
            // Keep track in our new indices array where in our unique
            outIndices[i] = indexInUniqueVertices;
        }
        // The vertex has already been placed into both our map and
        // our uniqueSortedVertices vector; therefore we just need to
        // retrieve the correct index and place it in the outIndices
        else {
            std::cout << "vertex " << vertex << "is a duplicate." << std::endl;


            // The map stores (vertex, index)
            outIndices[i] = v->second;
        }
    }

    std::cout << "Size of map: " << uniqueVerticesMap.size() << std::endl;
    std::cout << "Size of vector: " << uniqueSortedVertices.size() << std::endl;


    for( std::map<Vector3, int>::iterator ii=uniqueVerticesMap.begin(); ii!=uniqueVerticesMap.end(); ii++)
    {
           std::cout << (*ii).first << ": " << (*ii).second << std::endl;
    }

    assert(uniqueSortedVertices.size() <= static_cast<unsigned int>(numVertices));

    // Change the parameter to reflect the new number of vertices;
    // note that the number of indices does not change
    numVertices = uniqueSortedVertices.size();

    // At this point we have our unique vertices as well as our
    // indices.  Now all we need to do is convert from our Vector3 vector
    // into a float array.
    outVertices = new float[3 * numVertices];
    for (unsigned int i = 0; i < uniqueSortedVertices.size(); i++) {
        unsigned int index = 3 * i;
        outVertices[index    ] = uniqueSortedVertices[i].getX();
        outVertices[index + 1] = uniqueSortedVertices[i].getY();
        outVertices[index + 2] = uniqueSortedVertices[i].getZ();
    }


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






     float * normals = NULL;
     calculateNormals(vertices, indices, normals, NUM_VERTICES, NUM_INDICES);
     assert(normals);

	 GeometryFactory::fillInObject(o, vertices, normals, colors, indices,
									NUM_VERTICES, NUM_INDICES);

                                


}

/**
* Return vector that is the normal to the
* triangle defined by v1, v2, v3
*/
Vector3 GeometryFactory::calculateTriangleNormal(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3) {
    Vector3 normal = (v2 - v1).crossProduct(v3 - v1);
    //return normal.normalize();
    return normal;
}

/**
* Based on pseudocode from http://www.devmaster.net/forums/showthread.php?t=414
**/
void GeometryFactory::calculateNormals(float *vertices, int *indices, float *&normals,
								        int numVertices, int numIndexElements)
{

    // Create a normal array, all starting at (0,0,0)
    Vector3 * vNormals = new Vector3[numVertices];

    // for each face in our object, calculate the normal and add that vector
    // to the corresponding normals of the three vertices
    for (int i = 0; i < numIndexElements; i += 3) {
        int index1 = indices[i];
        int index2 = indices[i+1];
        int index3 = indices[i+2];

        // Since our index refers to the nth triplet, we need to multiply
        // by 3 to find corresponding place in array
        int floatIndex1 = 3 * index1;
        int floatIndex2 = 3 * index2;
        int floatIndex3 = 3 * index3;

        // The current triangular face is defined by these three
        // vertices.
        const Vector3 v1( vertices[floatIndex1    ],
                          vertices[floatIndex1 + 1],
                          vertices[floatIndex1 + 2]);

        const Vector3 v2( vertices[floatIndex2    ],
                          vertices[floatIndex2 + 1],
                          vertices[floatIndex2 + 2]);

        const Vector3 v3( vertices[floatIndex3    ],
                          vertices[floatIndex3 + 1],
                          vertices[floatIndex3 + 2]);

        Vector3 normal = calculateTriangleNormal(v1,v2,v3);

        // Find the current normals associated with these vertices and
        // add the new normal to them
        vNormals[index1] += normal;
        vNormals[index2] += normal;
        vNormals[index3] += normal;

        /*std::cout<< "i1: " << index1 << " i2: " << index2 << " i3: " << index3 << std::endl;
        std::cout<< "v1:" << v1 << " v2: " << v2 << " v3: " << v3 << " normal: " << normal << std::endl;*/


    }
    // Normalize the array of normals (make them unit length)
    for (int i = 0; i < numVertices; i++) {
        // For some reason some of the vectors are zero.  Ensure we don't
        // try to normalize them.
        if (vNormals[i] == Vector3::ZERO_VECTOR) {
            //std::cout<< "Error: the normal at index " << i <<  " was of zero length" << std::endl;
        }
        else {
            vNormals[i] = vNormals[i].normalize();
        }
    }

    // Allocate enough space for the float array
    normals = new float[numVertices * 3];

    // Switch from using our Vector3s to raw floats; that's what OpenGL needs
    for (int i = 0; i < numVertices; i++) {
        int index = 3 * i;
        normals[index] = vNormals[i].getX();
        normals[index+1] = vNormals[i].getY();
        normals[index+2] = vNormals[i].getZ();
    }

    delete[] vNormals;
    vNormals = NULL;

}






/**
* Snipped provided by Prof. Palacio
*/
void GeometryFactory::createHouses(Object *object)
{
	// quad houses
int numVertices = 42 + (24 + 14) * 3;
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


    // Calculate the bounding sphere and save it to the object
     float radius;
     Vector4 center;
     calculateBoundingSphere(vertices, numVertices, center, radius);
     object->setSphereCenter(center);
     object->setSphereRadius(radius);

	// Set up the vertex data
	VertexData& vertexData = object->vertexData;

	// Specify the elements of the vertex data:
	// - one element for vertex positions
	vertexData.vertexDeclaration.addElement(0, 0, 3, 3*sizeof(float), RE167::VES_POSITION);
	// - one element for vertex colors
	vertexData.vertexDeclaration.addElement(1, 0, 3, 3*sizeof(float), RE167::VES_DIFFUSE);

	// Create the buffers and load the data
	vertexData.createVertexBuffer(0, numVertices*3*sizeof(float), (unsigned char*)vertices);
	vertexData.createVertexBuffer(1, numVertices*3*sizeof(float), (unsigned char*)colors);

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
**/
void GeometryFactory::createSphere(Object *o, int numRows, int numFacesPerRow) {
	int *indices = NULL;
	float *vertices= NULL;
	float *colors = NULL;
    float *normals = NULL;
	int numVertices = 0;
	int numIndices = 0;
	// Do the heavy lifting with a helper method
	GeometryFactory::createSphere(numRows, numFacesPerRow, vertices, normals, colors, indices,
        numVertices, numIndices);
    
	GeometryFactory::fillInObject(o, vertices, normals, colors, indices, 
        numVertices, numIndices);
    

	delete[] indices;
	delete[] vertices;
    delete[] normals;
	delete[] colors;
    //delete[] normals;
    indices = NULL;
    vertices = NULL;
    normals = NULL;
    colors = NULL;
}

/**
* Creates a sphere of radius 1 centered at (0,0,0).
* @param numFaceRows		how many rows of faces will appear in the sphere
* @param numFacesPerRow		how many faces will appear per row
* @param vertices			the float pointer that will be allocated
*							and filled in with the vertices of the faces
*							in this method
* @param normals            the float pointer that will be allocated
*							and filled in with the normals of each vertex
* @param colors				the float pointer that will be allocated and
*							filled in with the colors of the vertices
* @param indices			will be created within this method and filled
*							with the connectivity information needed to render
*							the triangles
* @param numVertices    	will be changed to reflect how many elements
*							are stored within the vertices array
* @param numIndices         will be changed to reflect how many elements
*							are stored within the indices array
**/
void GeometryFactory::createSphere(int numFaceRows,
								   int numFacesPerRow,
                                   float *&vertices,
                                   float *&normals,
                                   float *&colors,
                                   int *&indices,
                                   int &numVertices,
                                   int &numIndices) {

                                       
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
    numVertices = NUM_VERTICES;
    vertices = new float[NUM_VERTICES * NUM_COMPONENTS_PER_VERTEX];
    // One normal per vertex
    normals = new float[NUM_VERTICES * NUM_COMPONENTS_PER_VERTEX];


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

            fillInVertex(vertices, startIndex, f1_1);
            fillInVertex(vertices, startIndex+3, f1_2);
            fillInVertex(vertices, startIndex+6, f1_3);
            fillInVertex(vertices, startIndex+9, f2_1);
            fillInVertex(vertices, startIndex+12, f2_2);
            fillInVertex(vertices, startIndex+15, f2_3);
            
            int normalStartIndex = startIndex;
            
            // Fill in normal information for each of the 6 vertices;
            // simply the normalized (x,y,z)
            fillInVertex(normals, normalStartIndex, f1_1.normalize());
            fillInVertex(normals, normalStartIndex+3, f1_2.normalize());
            fillInVertex(normals, normalStartIndex+6, f1_3.normalize());
            fillInVertex(normals, normalStartIndex+9, f2_1.normalize());
            fillInVertex(normals, normalStartIndex+12, f2_2.normalize());
            fillInVertex(normals, normalStartIndex+15, f2_3.normalize());
   		}
    }

    // Our vertices array is fully filled in; no need for the temporary one
    delete[] tempVertices;
    tempVertices = NULL;
    
    // Create the color array.  Each triangle section of each face will be
    // a separate color
    //colors = new float[numVertices * 3];
    //ColorFactory::randomlyColorize(colors, numVertices);
    //std::fill(&colors[0], &colors[numVertices * 3], 1.0f);

    // Create the connectivity array, which says how the vertices are
    // joined together to create faces.  This array is set up so that
    // if the first three elements of it are i1, i2, and i3, OpenGL will
    // create a triangular face from the i1th vertex, i2th vertex, and i3th
    // vertex as defined in the vertices array.
    numIndices = NUM_VERTICES;
    indices = new int[numIndices];
    for (int i = 0; i < numIndices; i++) {
        indices[i] = i;
    }
}





void GeometryFactory::createCylinder(Object *o, int numRows,
									 int numFacesPerRow, float radius) {
	GeometryFactory::createTaperedCylinder(o, numRows, numFacesPerRow, radius, radius);
}





void GeometryFactory::createCylinder(int numRows,
									int numFacesPerRow,
									float radius,
									float *&vertices,
									float *&normals,
									float *&colors,
									int *&indices,
									int &numVertices,
									int &numIndices) {

	GeometryFactory::createTaperedCylinder(numRows,
											numFacesPerRow,
											radius,
											radius,
											vertices,
											normals,
											colors,
											indices,
											numVertices,
											numIndices);
}



void GeometryFactory::createCone(Object *o, int numRows,
								 int numFacesPerRow, float bottomRadius) {

	GeometryFactory::createTaperedCylinder(o, numRows, numFacesPerRow, 0, bottomRadius);
}

void GeometryFactory::createCone(int numRows,
									int numFacesPerRow,
									float bottomRadius,
									float *&vertices,
									float *&normals,
									float *&colors,
									int *&indices,
									int &numVertices,
									int &numIndices) {

	GeometryFactory::createTaperedCylinder(numRows,
											numFacesPerRow,
											0,
											bottomRadius,
											vertices,
											normals,
											colors,
											indices,
											numVertices,
                                            numIndices);
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
void GeometryFactory::createTaperedCylinder(Object *o, int numRows,
									int numFacesPerRow, float topRadius,
									float bottomRadius) 
{							

	int *indices = NULL;
	float *vertices= NULL;
	float *colors = NULL;
    float *normals = NULL;

	int numIndices = 0;
	int numVertices = 0;
	// Do the heavy lifting with a helper method
	GeometryFactory::createTaperedCylinder(numRows,
									numFacesPerRow,
									topRadius,
									bottomRadius,
									vertices,
									normals,
									colors,
									indices,
									numVertices,
									numIndices);
									
                                
//    calculateNormals(vertices, indices, normals, numVertices, numIndices);								
    assert(normals);
    
	GeometryFactory::fillInObject(o, vertices, normals, colors, indices, numVertices,
								numIndices);

	delete[] indices;
	delete[] vertices;
    delete[] normals;
	delete[] colors;
    indices = NULL;
    vertices = NULL;
    normals = NULL;
    colors = NULL;

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
* @param normals    will be filled in with a normal for each vertex in object
* @param colors		the array of colors that will be allocated and filled
*					in
* @param indices	the array of indices that will be allocated and
*					filled in; represents the connectivity of the
*					vertices
* @param numVertices	will be changed to reflect the length
*					of the vertices array
* @param numIndices	will be changed to reflect the length
*					of the indices array
**/
void GeometryFactory::createTaperedCylinder(int numRows,
									int numFacesPerRow,
									float topRadius,
									float bottomRadius,
									float *&vertices,
									float *&normals,
									float *&colors,
									int *&indices,
									int &numVertices,
									int &numIndices)
{

	// We need at least one row
	assert(numRows >= 1);
	// We can't really approximate a circle with fewer than 3 vertices
	assert(numFacesPerRow >= NUM_VERTICES_PER_TRIANGLE);

	assert(bottomRadius >= 0);
	assert(topRadius >= 0);

	const float HEIGHT = 2.0f;
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
	const int NUM_MID_VERTICES =	numFacesPerRow *
									numRows *
									NUM_TRIANGLES_PER_RECTANGULAR_FACE *
									NUM_VERTICES_PER_TRIANGLE;

	const int TOTAL_NUM_VERTICES =	NUM_TOP_VERTICES +
									NUM_MID_VERTICES +
									NUM_BOTTOM_VERTICES;


	const int NUM_COMPONENTS_PER_ROW =
        NUM_COMPONENTS_PER_RECTANGULAR_FACE * numFacesPerRow;


    numVertices = TOTAL_NUM_VERTICES;
    numIndices = numVertices;


	vertices = new float[numVertices * 3];
	colors = new float[numVertices * 3];
    normals = new float[numVertices * 3];
	indices = new int[numIndices];

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

    // The normals are constant at given proportion around circular
    // cross sections (the sloping side of the cylinder is straight).
    // Calculate the normal at theta = 0 around the circle, e.g. when
    // x = 1, z = 0
    /*
    Vector3 tangent(0,0,1);
    Vector3 lowestPoint(bottomRadius, LOWEST_Y_VALUE, 0);
    Vector3 highestPoint(topRadius, HIGHEST_Y_VALUE, 0);
    Vector3 side = lowestPoint - highestPoint;
    Vector3 normal(tangent.crossProduct(side).normalize());
    Vector4 normal4v(normal.getX(), normal.getY(), normal.getZ(), 0);*/
    Vector3 * tempNormals = new Vector3[NUM_ROWS_VERTICES * numFacesPerRow];
    /*
    std::cout << "Tangent: " << tangent << std::endl;
    std::cout << "Lowest point: " << lowestPoint << std::endl;
    std::cout << "Highest point: " << highestPoint << std::endl;
    std::cout << "Normal: " << normal << std::endl;*/
    
	// Divide the conical shape into NUM_ROWS_VERTICES slices
	// Draw from top to bottom.  
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
			
            // TODO: This only works for a cylinder; otherwise the y value
            // should change
            tempNormals[count] = Vector3(x, 0, z);
			
			count++;
		}
	}
    /*for (int i = 0; i < numFacesPerRow; i++) {
        std::cout << "Normal[" << i << "]: " << tempNormals[i] << std::endl;
    }*/


    
	// From the vertices we have calculated, we need to duplicate some since
	// multiple faces have vertices in common and we want to be able to
	// set each face's color individually


	// The top of the cylindrical column.  The normals here are easy: just the
	// up vector
	const Vector3 UP_VECTOR(0,1,0);
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


        fillInVertex(vertices, vertex1Index, vertex1);
        fillInVertex(vertices, vertex2Index, vertex2);
        fillInVertex(vertices, vertex3Index, center);
        
        // The top of the cylinder has normals facing up
        fillInVertex(normals, vertex1Index, UP_VECTOR);
        fillInVertex(normals, vertex2Index, UP_VECTOR);
        fillInVertex(normals, vertex3Index, UP_VECTOR);
        
	}

	const int topOffset = NUM_TOP_VERTICES * NUM_COMPONENTS_PER_VERTEX;

	// The middle faces.  For each interation we connect
	// a single band of faces (rectangular faces that have
	// been triangulated)
	//
	// The normals here are slightly more difficult to calculate.  
	
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

            Vector3 f1_1n = tempNormals[(row * numFacesPerRow) + face];
            // Vertex 2: Lower right corner.
            Vector3 f1_2n = tempNormals[((row + 1) * numFacesPerRow) + ((face + 1) % numFacesPerRow)];
            // Vertex 3: Upper right corner
			Vector3 f1_3n = tempNormals[(row * numFacesPerRow) + ((face + 1) % numFacesPerRow)];

            // Face 2: The lower left triangle of the rectangular face
            // Vertex 1: The upper left corner (same as f1_1)
            Vector3 f2_1n = f1_1n;
            // Vertex 2: Lower left corner
            Vector3 f2_2n = tempNormals[((row + 1) * numFacesPerRow) + face];
            // Vertex 3: Lower right corner: same as f1_2
            Vector3 f2_3n = f1_2n;



			// Now we need to actually fill in the vertices array with the
            // raw float values from our vertices.  We are filling in
            // 6 vertices, for a total of 18 floats added to the array per
            // iteration of this inner loop.  We also need to note that we
			// have added a bunch of elements already due to the top of the cylinder
            int startIndex = (row * NUM_COMPONENTS_PER_ROW) + (NUM_COMPONENTS_PER_RECTANGULAR_FACE * face)
                + topOffset;

            fillInVertex(vertices, startIndex,    f1_1);
            fillInVertex(vertices, startIndex+3,  f1_2);
            fillInVertex(vertices, startIndex+6,  f1_3);
            fillInVertex(vertices, startIndex+9,  f2_1);
            fillInVertex(vertices, startIndex+12, f2_2);
            fillInVertex(vertices, startIndex+15, f2_3);

            fillInVertex(normals, startIndex,    f1_1n);
            fillInVertex(normals, startIndex+3,  f1_2n);
            fillInVertex(normals, startIndex+6,  f1_3n);
            fillInVertex(normals, startIndex+9,  f2_1n);
            fillInVertex(normals, startIndex+12, f2_2n);
            fillInVertex(normals, startIndex+15, f2_3n);
		}
	}


	const int midOffset = topOffset + NUM_MID_VERTICES * NUM_COMPONENTS_PER_VERTEX;

    const Vector3 DOWN_VECTOR(0,-1,0);

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

        fillInVertex(vertices, vertex1Index, vertex1);
        fillInVertex(vertices, vertex2Index, vertex2);
        fillInVertex(vertices, vertex3Index, center);
        
        fillInVertex(normals, vertex1Index, DOWN_VECTOR);
        fillInVertex(normals, vertex2Index, DOWN_VECTOR);
        fillInVertex(normals, vertex3Index, DOWN_VECTOR);
	}

	// Fill in the color arrays
	//ColorFactory::colorize(colors, numVertices);
    std::fill(&colors[0], &colors[numVertices * 3], 1.0f);


	// Fill in the connectivity arrays
	for (int i = 0; i < numIndices; i++) {
		indices[i] = i;
	}

	// We no longer need the temparray
	delete[] tempVertices;
    delete[] tempNormals;
    tempVertices = NULL;
    tempNormals = NULL;
}

void GeometryFactory::createSurfaceOfRevolution(Object *o, 
    const BezierCurve &generatrix,
    int numPointsToEvaluateAlongCurve,
    int numAnglesToRotate)
{
    assert (o != NULL);
    int *indices = NULL;
	float *vertices= NULL;
	float *colors = NULL;
    float *normals = NULL;
    float *textureCoords = NULL;
	int numVertices = 0;
	int numIndices = 0;
	// Do the heavy lifting with a helper method
	createSurfaceOfRevolution(generatrix, 
	    numPointsToEvaluateAlongCurve, 
	    numAnglesToRotate, 
	    vertices,  
	    normals, 
	    textureCoords,
	    colors, 
	    indices,
	    numVertices,
        numIndices);
        
    assert (indices != NULL);
    assert (vertices != NULL);
        
       
    // TODO: Make this method take in the texture coordinates too
	fillInObject(o, vertices, normals, colors, indices, 
        numVertices, numIndices);
    
    
    delete[] indices;
    indices = NULL;
    delete[] vertices;
    vertices = NULL;
    if (normals != NULL) {
        delete[] normals;
        normals = NULL;
    }
    if (colors != NULL) {
	    delete[] colors;
        colors = NULL;
    }
    if (normals != NULL) {
        delete[] normals;
        normals = NULL;
    }
    if (textureCoords != NULL) {
        delete[] textureCoords;
        textureCoords = NULL;
    }
    
    
}

/**
* Given a 2D bezier curve in the xy plane, rotate about the y-axis to produce
* a 3d surface.  
**/
void GeometryFactory::createSurfaceOfRevolution(
    const BezierCurve &generatrix,
    int numPointsToEvaluateAlongCurve,
    int numAnglesToRotate,
    // outputs
    float *&vertices,
    float *&normals,
    float *&textureCoords,
    float *&colors,
    int *&indices,
    int &numVertices,
    int &numIndices)
{
    assert(numPointsToEvaluateAlongCurve >= 1);
    assert(numAnglesToRotate >= 1);
    
    // Uniformly sample both the points and the tangents
    std::vector <Vector3> pointsOnCurve = 
        generatrix.uniformPointSample(numPointsToEvaluateAlongCurve);
    std::vector <Vector3> tangentVectorsOnCurve = 
        generatrix.uniformTangentSample(numPointsToEvaluateAlongCurve);

    std::cout << "\n\n\n" << std::endl;

    std::vector<Vector3>::iterator i;
    for(i=pointsOnCurve.begin(); i != pointsOnCurve.end(); ++i) std::cout << (*i);
    std::cout << "Size of point array: " << pointsOnCurve.size() << std::endl;
    std::cout << "numPointsToEvaluateAlongCurve: " << numPointsToEvaluateAlongCurve << std::endl;
    

    assert (pointsOnCurve.size() == tangentVectorsOnCurve.size());
    assert (pointsOnCurve.size() == numPointsToEvaluateAlongCurve);    
        
    std::vector <std::vector<Vector3> > rotatedPoints;
    std::vector <std::vector<Vector3> > rotatedTangentVectors;
    
    
    // Sample at numAnglesToRotate positions around the y axis
    for (int i = 0; i < numAnglesToRotate; i++) {
        // TODO: Look into this.
        // Need to subtract 1 in order to ever have 100% (0 based indexing)
        float proportionAround =   static_cast<float>(i) / 
                                    static_cast<float>(numAnglesToRotate);
        float theta = TWO_PI * proportionAround;
        
        std::cout << "theta: " << BasicMath::degrees(theta) << std::endl;
        
        Matrix4 rotationMatrix = Matrix4::rotateY(theta);

        // Create the std::vector we need to hold all the points at this
        // angle of rotation
        rotatedPoints.push_back(std::vector<Vector3>());
        rotatedTangentVectors.push_back(std::vector<Vector3>());
        

        // Need to rotate each point along the curve
        for (int j = 0; j < pointsOnCurve.size(); j++) {
            Vector3 point = pointsOnCurve[j];
            Vector3 tangent = tangentVectorsOnCurve[j].normalize();
            
            // We need to transform the 3d points to 4d for purposes of 
            // matrix multiplication
            Vector4 point4(point);
            Vector4 tangent4(tangent);
            
            Vector4 transformedPoint4 = rotationMatrix * point4;
            Vector4 transformedTangentVector4 = rotationMatrix * tangent4;
            
            // Keep track of rotated point and vector; the rotation does not
            // change length of vector so we don't need to worry about 
            // normalzing it
            rotatedPoints[i].push_back(Vector3(transformedPoint4));
            rotatedTangentVectors[i].push_back(Vector3(transformedTangentVector4));
        }
    }
    
    assert(rotatedPoints.size() == numAnglesToRotate);
    assert(rotatedPoints[0].size() == numPointsToEvaluateAlongCurve);
            
            
    for (int i = 0; i < rotatedPoints.size(); i++) {
        for (int j = 0; j < rotatedPoints[i].size(); j++) {
            cout << "i: " << i << "j: " << j << ": " << rotatedPoints[i][j] << endl;
        }
    }        
            
    // TODO: 3D normal vector
    // TODO: texture coordinate
    
    // We have all of our points; now we need to connect them up correctly. 

    // TODO: this seems wrong.  why isn't it - 1?
    // how many rows of faces?
    const int NUM_ROWS = numPointsToEvaluateAlongCurve - 1;
    const int numFacesPerRow = numAnglesToRotate;
    const int NUM_COMPONENTS_PER_ROW =
        NUM_COMPONENTS_PER_RECTANGULAR_FACE * numFacesPerRow;
    
    	
    numVertices = NUM_COMPONENTS_PER_ROW * NUM_ROWS;
    numIndices = numVertices;
    
    // Need 3 times as much space because each vertex has an x,y,z component
    vertices = new float[3 * numVertices];	
    indices = new int[numIndices];	
    	
	for (int row = 0; row < NUM_ROWS; row++) {


		// The "face"th and ("face"+1)%numFacesPerRow vertices in row "row"
		// form a rectangular face with the "face"th and ("face"+1)%numFacesPerRow
		// vertices in row "row" + 1.
		for (int face = 0; face < numFacesPerRow; face++) {
            
            // Face 1: Upper right triangle of rectangular face
            // Vertex 1: Upper left corner
            Vector3 f1_1 = rotatedPoints[face][row];
            // Lower right corner
            Vector3 f1_2 = rotatedPoints[(face+1) % numFacesPerRow][row+1];
            // Upper right corner
            Vector3 f1_3 = rotatedPoints[(face+1) % numFacesPerRow][row];
            
            
            // Face 2: Lower left triangle of rectangular face
            // Upper left corner (same as f1_1)
            Vector3 f2_1 = f1_1;
            // Lower left corner
            Vector3 f2_2 = rotatedPoints[face][row+1];
            // Lower right corner: (same as f1_2)
            Vector3 f2_3 = f1_2;
            
            // Now we need to actually fill in the vertices array with the
            // raw float values from our vertices.  We are filling in
            // 6 vertices, for a total of 18 floats added to the array per
            // iteration of this inner loop.  
            int startIndex = (row * NUM_COMPONENTS_PER_ROW) + (NUM_COMPONENTS_PER_RECTANGULAR_FACE * face);
            
            
            fillInVertex(vertices, startIndex,    f1_1);
            fillInVertex(vertices, startIndex+3,  f1_2);
            fillInVertex(vertices, startIndex+6,  f1_3);
            fillInVertex(vertices, startIndex+9,  f2_1);
            fillInVertex(vertices, startIndex+12, f2_2);
            fillInVertex(vertices, startIndex+15, f2_3);
/*
            fillInVertex(normals, startIndex,    f1_1n);
            fillInVertex(normals, startIndex+3,  f1_2n);
            fillInVertex(normals, startIndex+6,  f1_3n);
            fillInVertex(normals, startIndex+9,  f2_1n);
            fillInVertex(normals, startIndex+12, f2_2n);
            fillInVertex(normals, startIndex+15, f2_3n);*/
		}
	}
	
	// Fill in the indices array
    for (int i = 0; i < numIndices; i++) {
        indices[i] = i;
    }
	   
	std::cout << "numPointsAlongCurve: " << numPointsToEvaluateAlongCurve <<
    "numAngles: " << numAnglesToRotate  << "Number of vertices: " << numVertices << std::endl; 
    
    
    
    
}



// HELPER METHODS


/** Helper method to get vertexData, create buffers for the vertices, indices, colors, and then
 * fill them in
 * @param o
 * @param vertices			the (x,y,z) locations of all the vertices.  MUST NOT BE NULL
 * @param normals           the vector for normals at each vertex
 * @param colors			the (r,g,b) values at each vertex
 * @param indices			the indices into the vertices array that make up each triangular face
 * @param numVertices       how many vertices, size of vertices array is 3 * numVertices
 * @param numIndices    	the size of the indices array
**/
void GeometryFactory::fillInObject(Object *o, float *vertices, float *normals, float *colors, int *indices,
								   int numVertices, int numIndices) 
{
    
    // Calculate the bounding sphere and save it to the object
    float radius;
    Vector4 center;
    calculateBoundingSphere(vertices, numVertices, center, radius);
    o->setSphereCenter(center);
    o->setSphereRadius(radius);
    
    VertexData& vertexData = o->vertexData;

    assert (vertices != NULL);
    assert (numVertices >= 1);

	// one element for vertices
	vertexData.vertexDeclaration.addElement(0, 0, 3, 3*sizeof(float), RE167::VES_POSITION);
	vertexData.createVertexBuffer(0, 3 * numVertices * sizeof(float), (unsigned char*) vertices);
	
	//TODO: Put back in the color stuff.  Figure out why those objects that
	//have colors defined don't work correctly with the material colors
	/*
	if (colors != NULL)
	{
	    // - one element for vertex colors
    	vertexData.vertexDeclaration.addElement(1, 0, 3, 3*sizeof(int), RE167::VES_DIFFUSE);
    	vertexData.createVertexBuffer(1, 3 * numVertices * sizeof(float), (unsigned char*) colors);
	}*/

    if (normals != NULL)
    {
        // one element for normals
        vertexData.vertexDeclaration.addElement(2, 0, 3, 3*sizeof(float), RE167::VES_NORMAL);
        vertexData.createVertexBuffer(2, 3 * numVertices * sizeof(float), (unsigned char*) normals);
    }
        
        
    // Add spherical texture coordinates to object    
    float * texCoords = NULL;
    //createSphericalCoordinates(vertices, normals, indices, texCoords, numVertices, numIndices);
    createPositionalSphericalCoordinates(vertices, indices, texCoords, numVertices, numIndices);
    assert(texCoords != NULL);

    vertexData.vertexDeclaration.addElement(3, 0, 2, 2*sizeof(float),
        RE167::VES_TEXTURE_COORDINATES);
    vertexData.createVertexBuffer(3, numVertices * 2*sizeof(float), (unsigned char*) texCoords);
    
    if (texCoords != NULL) {
        delete[] texCoords;
        texCoords = NULL;
    }
        
        
        
	vertexData.createIndexBuffer(numIndices, indices);
}

/** Debugging method that prints all the vertices out neatly
* @param verticesArray	the vertices to print
* @param numElements	the length of the verticesArray
* @param entriesPerLine	how many entries to print per line
**/
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
**/
void GeometryFactory::printVectorArray(Vector3 *vectorArray, const int numElements, int entriesPerLine) {
	for (int i = 0, count = 0; i < numElements; i++) {
		if (i % entriesPerLine == 0) {
			std::cout<<std::endl;
		}
		std::cout<< vectorArray[i];
		count++;
	}
}


void GeometryFactory::runTestSuite() {

    std::cout<< "Running test suite of normalization" << std::endl;

    Vector3 test1[] = {Vector3::ZERO_VECTOR, Vector3::ZERO_VECTOR, Vector3::ZERO_VECTOR, Vector3::ZERO_VECTOR};
    Vector3 test2[] = {Vector3(1,0,0), Vector3(0,1,0), Vector3(0,0,0), Vector3(0,0,1)};
    Vector3 test3[] = {Vector3(1.2, 13.5, -23.5), Vector3(100.5, 23.5, -32.553), Vector3(13.05f, 23, 0),
        Vector3(99.3, 10, -9.053).crossProduct(Vector3(11.85, 9.5, 23.5))};



    std::vector<Vector3*> tests;
    tests.push_back(test1);
    tests.push_back(test2);
    tests.push_back(test3);

    for (unsigned int i = 0; i < tests.size(); i++) {
        Vector3 * test = tests[i];
        assert (calculateTriangleNormal(test[0], test[1], test[2]) == test[3]);
    }
    
}

void GeometryFactory::fillInVertex(float *&vertices, int startIndex, const Vector3 &vertex) {
    vertices[startIndex  ] = vertex.getX();
    vertices[startIndex+1] = vertex.getY();
    vertices[startIndex+2] = vertex.getZ();
}