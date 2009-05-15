#include "FractalSurface.h"
#include "Vector3.h"
#include "GeometryFactory.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

using namespace RE167;

/* Returns an object representing a surface that spans the space between the
   the 4 given points. Points a, b, c, and d represent the top-left,
   top-right, bottom-right, and bottom-left corners of the space. These
   points do not need to be coplanar. The surface formed by this method
   is perturbed according to an unerlying fractal terrain surface. Thus
   the roughness parameter corresponds to the roughness of the underlying
   fractal terrain. The paramater m determines how many grid intervals there
   are to be along the da-> and cb-> vectors. The scale paramater stretches the
   pertubations: >1 makes them larger, <1 smaller, and =1 does not change them.
   The method will determine an approprate number of grid intervals to place 
   along the ab-> and dc-> vectors based on their relative length to the da->
   and cb-> vectors.
   The method forces the terrain to intersect the da-> and cb-> vectors,
   thus allowing one to tile multiple such surfaces next to each other with
   continuity, though perhaps at the expense of some seam artifacts. */
Object* FractalSurface::buildSurfaceAmong(const Vector3 &a, const Vector3 &b, const Vector3 &c, const Vector3 &d, int m, float roughness, float scale) { 
  // compute the edge vectors and use them to determine the relative sizes
  // of the edges. use these sizes to determine how many intervals we need in
  // the "i" direction given that we have m in the "j" direction.
  Vector3 dc = c - d;
  Vector3 ab = b - a;
  Vector3 da = a - d;
  Vector3 cb = b - c;
  float i_size = std::max(dc.magnitude(), ab.magnitude());
  float j_size = std::max(da.magnitude(), cb.magnitude());
  int n = (int) (m * (i_size/j_size));
  
  // determine the smallest fractal height array that will give us height
  // values for all points in our n x m surface
  int nIterations = 1;
  while (heightArraySize(nIterations) < std::max(n,m)+1) { nIterations++; }
  float** heights = buildHeightArray(nIterations, roughness);
  
  // prepare geometry data strucures. we will need both the vertices array
  // and the points grid because the former is used for Object creation and the
  // latter for normals calculation.
  int nVertices = (n+1)*(m+1);
  int nTrigs = n*m*2;
  int nIndices = 3 * nTrigs;
  Vector3* points  = new Vector3[nVertices];
  float* vertices  = new float[nVertices*3];
  int*   indices   = new int[nIndices];
  float* normals   = new float[nVertices*3];
  float* texcoords = new float[nVertices*2];
  int iV = 0;
  int iI = 0;
  int iN = 0;
  int iT = 0;
  
  // compute the points on the surface among the 4 given points using the
  // standard surface-patch approach, but the perturb those points according
  // to the fractal height array to introduce randomness into the surface
  for (int i = 0; i <= n; i++) {
    for (int j = 0; j <= m; j++) {
      Vector3 dc_s = dc * divi(i,n);
      Vector3 ab_s = ab * divi(i,n);
      Vector3 vert = (a + ab_s) - (d + dc_s);
      Vector3 vert_s = vert * divi(j,m);
      Vector3 cb_s = cb * divi(j,m);
      Vector3 da_s = da * divi(j,m);
      Vector3 horz = (d + da_s) - (c + cb_s);
      Vector3 norm = (vert.crossProduct(horz).normalize());
      float pert = heights[i][j];
      float t = std::min(1 - divi(i,n), divi(i,n));
      float cap = 2 * (-(t*t) + t);
      Vector3 point = d + dc_s + vert_s + (norm * cap * pert * scale);
      
      points[vertexIndex(i, j, m)] = point;
      vertices[iV++] = point.getX();
      vertices[iV++] = point.getY();
      vertices[iV++] = point.getZ();
    }
  }
  
  // specify the indices for the triangulation. for each square formed among
  // adjacent points we build a (lower-left,upper-right,upper-left) triangle
  // and a (lower-left,lower-right,upper-right) triangle.
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      indices[iI++] = vertexIndex(i,   j,   m);
      indices[iI++] = vertexIndex(i+1, j+1, m);
      indices[iI++] = vertexIndex(i,   j+1, m);
      
      indices[iI++] = vertexIndex(i,   j,   m);
      indices[iI++] = vertexIndex(i+1, j,   m);
      indices[iI++] = vertexIndex(i+1, j+1, m);
    }
  }
  
  // compute the surface normals based on neighboring points
  for (int i = 0; i <= n; i++) {
    for (int j = 0; j <= m; j++) {
      Vector3 right = points[vertexIndex((i==n) ? i : i+1, j, m)];
      Vector3 left  = points[vertexIndex((i==0) ? 0 : i-1, j, m)];
      Vector3 up    = points[vertexIndex(i, (j==n) ? j : j+1, m)];
      Vector3 down  = points[vertexIndex(i, (j==0) ? j : j-1, m)];
      Vector3 norm = (right - left).crossProduct(up - down);
      normals[iN++] = norm.getX();
      normals[iN++] = norm.getY();
      normals[iN++] = norm.getZ();
    }
  }
  
  // assign texture coordinates so that the unit square correspons to roughly
  // one surface. in particular we need to wrap the texture in one direction
  // if the surface is not square.
  for (int i = 0; i <= n; i++) {
    for (int j = 0; j <= m; j++) {      
      texcoords[iT++] = divi(i, std::min(n, m));
      texcoords[iT++] = divi(j, std::min(n, m));
    }
  }
  
  // use the geometry data to build an Object and return it
  Object* o = new Object();
  GeometryFactory::fillInObject(
    o, vertices, normals, texcoords, NULL, indices, nVertices, nIndices);
  return o;
}

/* Returns the index of the vertex at (i,j) in a map of height m. */
int FractalSurface::vertexIndex(int i, int j, int m) {
  return (i*(m+1)) + j;
}

/* Returns the size (i.e. width or height) of the height array that will
   be generated with the given number of iterations. */
int FractalSurface::heightArraySize(int nIterations) {
  return exp_two(nIterations) + 1;
}

/* Returns a 2d grid of height values corresponding to a fractal terrain
   generated over the given number of iterations and with the specified
   roughness coefficient. */
float** FractalSurface::buildHeightArray(int nIterations, float roughness) {  
  // determine the size of the heights array to be constructed and initialize
  // a corresponding empty array.
  int size = heightArraySize(nIterations);
  float** heights = new float*[size];
  for (int x = 0; x < size; x++) {
    heights[x] = new float[size];
  }
  
  // initialize the four corners to 0
  heights[0][0] = heights[0][size-1] = heights[size-1][0] = heights[size-1][size-1] = 0.0;
  
  for (int iter = 0; iter < nIterations; iter++) {
    // pertubation range for this iteration
    float pertRange = pow(2.0, -(roughness * iter));
    
    // stride for this iteration; the width of the squares and diamonds
    int stride = exp_two(nIterations - iter);
    int hStride = stride/2;
    
    // pass through the array and perform the diamond step on each square
    for (int x = 0; x < size-1; x+= stride) {
      for (int y = 0; y < size-1; y+= stride) {
        // the value in the middle of the square is the average of its four
        // corners plus some pertubation amount
        float average = (heights[x][y] +        heights[x][y+stride] +
                         heights[x+stride][y] + heights[x+stride][y+stride]) / 4.0;
        float pert = genPert(pertRange);
        heights[x+hStride][y+hStride] = average + pert;
      }
    }
    
    // pass throught the array and perform the square step for each diamend
    bool evenStep = true;
    for (int x = 0; x <= size-1; x += hStride) {
      for (int y = (evenStep ? 0 : -hStride); y < size-1; y += stride) {
        // the value in the middle of a diamond is calculated similarly as
        // in the square case above, but we need to be careful about points that
        // are off the edge of the map
        int count = 0;
        float sum = 0.0;
        squareUpdate(heights, size, x,         y,         &count, &sum);
        squareUpdate(heights, size, x-hStride, y+hStride, &count, &sum);
        squareUpdate(heights, size, x+hStride, y+hStride, &count, &sum);
        squareUpdate(heights, size, x,         y+stride,  &count, &sum);
        float average = sum / count;
        float pert = genPert(pertRange);
        heights[x][y+hStride] = average + pert;
      }
      evenStep = !evenStep;
    }
  }
  
  // return the completed heights array
  return heights;
}

/* Helper method for performing the square update step in the diamond-square
   fractal terrain algorithm. */
void FractalSurface::squareUpdate(float** heights, int size, int r, int c, int* count, float* sum) {
  if ((r >= 0) && (r < size) && (c >= 0) && (c < size)) {
    (*count)++;
    (*sum) += heights[r][c];
  }
}

/* Returns a random number selected uniformly from
   [-pertRange, pertRange]. */
float FractalSurface::genPert(float pertRange) {
  float unitRand = ((float) rand()) / RAND_MAX;
  return ((unitRand * 2.0) - 1.0) * pertRange;
}
