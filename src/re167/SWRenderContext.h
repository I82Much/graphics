#ifndef __SWRenderContext_h__
#define __SWRenderContext_h__


#include "RE167_global.h"
#include "RenderContext.h"
#include "VertexDeclaration.h"
#include "Matrix4.h"
#include "SWZBuffer.h"

namespace RE167 {

	/** This class implements the abstract base class RenderContext and
		it is the main interface to the low-level graphics API.
	@remarks
		This class also instantiates the Singleton class to make sure there is
		only one instance at any time.
	*/

	class SWWidget;
		
	class RE167_EXPORT SWRenderContext : public RenderContext
	{
	public:
		void init();
		void setViewport(int width, int height);
		void beginFrame();
		void endFrame();
		void setModelViewMatrix(const Matrix4 &m);
		void setProjectionMatrix(const Matrix4 &m);
		/** This is the main method for rendering objects. Note that it uses
			OpenSW vertex arrays. See the OpenSW 2.0 book for details.
		*/
		void render(Object *object);

		/** Set the widget that will receive rendering output.
		*/
		void setWidget(SWWidget *swWidget);

		static SWRenderContext& getSingleton(void);
        static SWRenderContext* getSingletonPtr(void);

	private:

        enum TriangleLocation {INSIDE, OUTSIDE, ON_EDGE};
        enum TriangleEdge {AB, AC, BC };


		SWWidget *mswWidget;
		QImage *image;

		Matrix4 projection;
		Matrix4 modelview;
		Matrix4 viewport;
		Matrix4 total;

        int width;
        int height;

        int lastFrameTime;
        int curFrameTime;

        int numFramesRendered;
        QTime *clock;

        SWZBuffer *buffer;

        TriangleLocation fromBarycentric(float alpha, float beta, float gamma);

        bool isOnLeftEdge(float alpha, float beta, float gamma, float vertices[3][2]);
        bool isOnTopHorizontalEdge(float alpha, float beta, float gamma, float vertices[3][2]);


        QRgb linearInterpolation(float alpha, float beta, float gamma, float colors[3][4]);
        QRgb perspectiveCorrectInterpolation(float alpha, float beta, float gamma, float depths[3], float colors[3][4]);

        bool offscreen(float left, float right, float top, float bottom);
		void rasterizeTriangle(float p[3][4], float n[3][3], float c[3][4]);
        void drawTile(int left, int right, int top, int bottom, float vertices[3][2],
                      float colors[3][4], float depths[3]);

		Vector3 barycentric(float point[2], float vertices[3][2]);
        bool tileOverlapsTriangle(int left, int right, int top, int bottom, float vertices[3][2]);
	};

}

#endif
