#ifndef RenderWidget0_H
#define RenderWidget0_H

#include <QWidget>
#include "GLRenderWidget.h"


#include "scenegraph/CameraNode.h"


namespace RE167 {

    class Object;
    class SceneManager;
    class TransformGroup;
    class BezierCurve;
    class Shape3D;
    class Vector3;
    class CameraNode;
    class Spline;
    class Shader;
    
    class RE167_EXPORT RenderWidget0 : public GLRenderWidget
    {
    

    public:
        RenderWidget0();
        ~RenderWidget0();

    	void startAnimation();
    	void stopAnimation();
    	void toggleWireframe();

    protected:
    	// Event handlers. These are virtual methods of the base class.
    	// They are called automatically to handle specific events.

    	// Called when the render window is ready.
    	void initSceneEvent();

    	// Called when the scene in the render window needs to be re-rendered.
    	void renderSceneEvent();

    	// Called when the render widget is resized.
    	void resizeRenderWidgetEvent(const QSize &s);

        // Virtual methods of QWidget class. This is a subset. Re-implement any
        // that you would like to handle.
        void timerEvent(QTimerEvent *t);
        void mousePressEvent(QMouseEvent *e);
        void mouseMoveEvent(QMouseEvent *e);
        void mouseReleaseEvent(QMouseEvent *e);

        void keyPressEvent ( QKeyEvent * e );
        void keyReleaseEvent ( QKeyEvent * e);
	
        // TODO: this is the reason we have to include Vector3.  
        Vector3 mapToUnitSphere(int x, int y, int width, int height); 
    	

    private:
    	SceneManager *sceneManager;
        CameraNode *stillCamera; // this is the camera that does not move on its own and allows the trackball
		CameraNode* movingCamera; // this is the camera that follows the minecart

        TransformGroup *minecart;


        Spline * track;
        Spline * square;
        Spline * helix;
        Spline * circle;
        Spline * torchCurve;
        
        Spline * firstTrackCrossSection;
        Spline * minecartProfile;
        
        Shader * twoSpotTexture;
        Shader * lightingTexture;
        
        
        // Texture * rockTexture;
        //         Texture * metalTexture;
        //         
        
		// added a LightNode to start experimenting with putting a light in front of the minecart
		LightNode* whiteLight;


	
	    // Hold the last mouse position
        int lastX;
        int lastY;

        int timerId;
        int counter;

        void initCameras();
        void initShaders();
        void initLights();
        void initMaterials();
        void initSplines();
        void initGeometry();
        
        void switchScene();
        void toggleCulling();
        void test();
        
        GLRenderContext * rs;
        
    };

}

#endif // BASICAPP_H
