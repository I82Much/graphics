#ifndef RenderWidget0_H
#define RenderWidget0_H

#include <QWidget>
//#include "SWRenderWidget.h"
#include "GLRenderWidget.h"
//#include "Object.h"
#include "Vector3.h"


namespace RE167 {

    class Object;
    class SceneManager;
    class TransformGroup;
    
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
	
        // TODO: this is the reason we have to include Vector3.  Ugh.  I could make it take in a Vector3 & instead.
    	Vector3 mapToUnitSphere(int x, int y, int width, int height); 

    private:
    	SceneManager *sceneManager;
        Camera *camera;

        // TODO: clean this stuff out
    	Object *object;
    	Object *houses;
    	Object *terrain;
    	Object *bunny;
        Object *cube;
    	Object *dragon;
        Object *sphere;
        Object *teapot;
        Object *buddha;
        Object *earth;
    
        TransformGroup * leftLeg;
        TransformGroup * robotGroup;
        TransformGroup * geometryGroup;
	
    	// Hold the last mouse position
    	int lastX;
    	int lastY;

    	int timerId;
    	int counter;
	
        void initCamera();
        void initLights();
        void initMaterials();
        void initGeometry();
        void initRobot();
        void createTestScene();
    
    
        void switchScene();
    };

}

#endif // BASICAPP_H
