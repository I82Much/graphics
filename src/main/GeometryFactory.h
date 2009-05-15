#ifndef _GEOMETRY_FACTORY_H__
#define _GEOMETRY_FACTORY_H__

#include "RE167_global.h"
#include <vector>
#include "Vector3.h"
#include "Basis.h"

namespace RE167 {

    class Object;
    class Vector3;
    class Vector4;
    class Matrix4;
    class Basis;
    class Spline;
    
    class RE167_EXPORT GeometryFactory 
    {
    public:

        // Represents the attributes at each vertex in our geometry
        struct VertexAttributes {
            Vector3 position;
            Vector3 normal;
            Vector3 textureCoords;
            Vector3 color;
        };
        
        // Represents a quadrilateral face (not necessarily coplanar) on a piece of
        // geometry.
        struct Face {
            VertexAttributes upperLeft;
            VertexAttributes upperRight;
            VertexAttributes lowerRight;
            VertexAttributes lowerLeft;
        };
        
        
        
        

    	static const int NUM_TRIANGLES_PER_RECTANGULAR_FACE;
    	static const int NUM_VERTICES_PER_TRIANGLE;
    	static const int NUM_VERTICES_PER_RECTANGULAR_FACE;
        
    	static const int NUM_COMPONENTS_PER_VERTEX;
    	static const int NUM_COMPONENTS_PER_RECTANGULAR_FACE;
        static const int NUM_POSITION_COMPONENTS_PER_VERTEX;
        static const int NUM_COLOR_COMPONENTS_PER_VERTEX;
        static const int NUM_NORMAL_COMPONENTS_PER_VERTEX;
        static const int NUM_TEXTURE_COMPONENTS_PER_VERTEX;
        
        
        static Object * createObjectFromFaces( std::vector<Face> faces, 
                                            bool hasNormals, 
                                            bool hasColors,
                                            bool hasTextureCoords);
        

    	static void createCube(Object *o);
    	static void createHouses(Object *object);

    	static void createObject(Object *o, char * filePath, bool normalize = true);
    	static void createTerrainFromPGM(Object *o, char * filepath, bool normalize = true);

    	static void createSphere(Object *o, int numRows = 10, int numFacesPerRow = 20);

    	static void createSphere(int numRows, int numFacesPerRow,
                                 float *&vertices,
                                 float *&normals,
                                 float *&colors,
                                 int *&indices,
                                 int &numVertices,
                                 int &numIndices);


    	static void createTaperedCylinder(Object *o, int numHeightSegments = 1,
    								int numSlices = 20, float topRadius = 1.0f,
    								float bottomRadius = 1.0f);
	
    	static void createTaperedCylinder(int numRows,
            int numFacesPerRow,
            float topRadius,
            float bottomRadius,
            float *&vertices,
            float *&normals,
            float *&colors,
            int *&indices,
            int &numVertices,
            int &numIndices);


    	static void createCylinder(Object *o, int numHeightSegments = 1,
            int numSlices = 20, float radius = 1.0f);


    	static void createCylinder(int numRows,
            int numFacesPerRow,
            float radius,
            float *&vertices,
            float *&normals,
            float *&colors,
            int *&indices,
            int &numVertices,
            int &numIndices);

        static Object * createLoft(
            const Spline &shape,
            const Spline &path,
            const int numShapeSegments = 10,
            const int numPathSegments = 10,
            const bool normalize = true
        );
        
        
        static std::vector<Face> createLoftFaces(
            const Spline &shape,
            const Spline &path,
            const int numShapeSegments,
            const int numPathSegments
        );
        
        static Object * createSurfaceOfRevolution( 
            const Spline &generatrix,
            int numPointsToEvaluateAlongCurve = 10,
            int numberOfAnglesToRotate = 18);

        static void createSurfaceOfRevolution(
            const Spline &generatrix,
            int numPointsToEvaluateAlongCurve,
            int numberOfAnglesToRotate,
            // outputs
            float *&vertices,
            float *&normals,
            float *&textureCoords,
            float *&colors,
            int *&indices,
            int &numVertices,
            int &numIndices);
                               
                                



    	static void createCone(Object *o, int numHeightSegments = 1,
    							int numSlices = 20, float bottomRadius = 1.0f);


    	static void createCone(int numRows,
    							int numFacesPerRow,
    							float bottomRadius,
    							float *&vertices,
    							float *&normals,
    							float *&colors,
    							int *&indices,
    							int &numVertices,
    							int &numIndices);

        static void calculateSphericalNormals(float *vertices, 
                                            float *&normals, 
                                            int *indices, 
                                            int numVertices, 
                                            int numIndices);

        static void fillInVertex(float *&vertices, int startIndex, const Vector3 &vertex);
        static void fillIn2DCoords(float *& textureCoords, int startIndex, const Vector3 &uv);                        

    	static void printVerticesArray(float *verticesArray, const int numElements, int entriesPerLine = 10);
    	static void printVectorArray(Vector3 *vectorArray, const int numElements, int entriesPerLine = 10);

        static void calculateBoundingBox(float *vertices, int numVertices,
            Vector3 &vMin, Vector3 &vMax);
            
        static void calculateBoundingSphere(float * vertices, int numVertices, 
            Vector4 &center, float & radius);

        static void createSphericalCoordinates(float *vertices,
                                                float *normals,
                                                int *indices,
                                                float *&texCoords,
                                                int numVertices,
                                                int numIndices);
    
        static void createPositionalSphericalCoordinates(float *vertices,
                                                    int *indices,
                                                    float *&texCoords,
                                                    int numVertices,
                                                    int numIndices);

        static void calculateNormals(float *vertices, int *indices, float *&normals,
            int numVertices, int numIndices);
        

    
        /**
        * Return a unit vector that is the normal to the
        * triangle defined by v1, v2, v3
        */
        static Vector3 calculateTriangleNormal(const Vector3 &v1, 
                                                const Vector3 &v2, 
                                                const Vector3 &v3);
    
      
        static void fillInObject(Object *o, float *vertices, float *normals, float * textureCoords, float *colors, int *indices,
            int numVertices, int numIndices);
       
        static void runTestSuite();

    private:

        static void createConnectivity(
            const std::vector <std::vector<Vector3> > &values,
            int numPointsRows,
            int numPointsCols,
            float *&floatValues,
            int numComponents
        );
                           
        static void createConnectivity(
            const std::vector <std::vector<Vector3> > &vecVertices,
            const std::vector <std::vector<Vector3> > &vecNormals,
            const std::vector <std::vector<Vector3> > &vecTexCoords,
            int numVerticesRows,
            int numVerticesCols,
            float *&vertices,
            float *&normals,
            float *&textureCoords
        );                       

        static std::vector<Face> calculateFaces(
            const std::vector <std::vector<Vector3> > &vecVertices,
            const std::vector <std::vector<Vector3> > &vecNormals,
            const std::vector <std::vector<Vector3> > &vecTexCoords,
            int numVerticesRows,
            int numVerticesCols
        );

    };

}

#endif
