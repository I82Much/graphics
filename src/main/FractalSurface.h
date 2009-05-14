#ifndef FRACTAL_SURFACE_H
#define FRACTAL_SURFACE_H

#include "Object.h"

/* 2^n */
#define exp_two(n) (1 << (n))

/* int/int => float division */
#define divi(a,b) (((float) (a)) / ((float) (b)))


namespace RE167 {
  /* Factory for fractal surfaces. Please see documentation in the
     implementation file. */
  class FractalSurface {
  public:
    static Object* buildSurfaceAmong(const Vector3 &a, const Vector3 &b, const Vector3 &c, const Vector3 &d, int m, float roughness, float scale);
    
  private:
    static int     vertexIndex(int i, int j, int m);
    static int     heightArraySize(int nIterations);
    static float** buildHeightArray(int nIterations, float roughness);
    static void    squareUpdate(float** heights, int size, int r, int c, int* count, float* sum);
    static float   genPert(float pertRange);
  };
}

#endif
