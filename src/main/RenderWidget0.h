#ifndef RenderWidget0_H
#define RenderWidget0_H

#include <QWidget>
#include "SWRenderWidget.h"
#include "GLRenderWidget.h"
#include "SceneManager.h"
#include "Object.h"
#include "Vector3.h"

using namespace RE167;

class RenderWidget0 : public SWRenderWidget
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
	

	Vector3 mapToUnitSphere(int x, int y, int width, int height); 

private:
	SceneManager *sceneManager;
	Camera *camera;

	Object *object;
	Object *houses;
	Object *terrain;
	Object *bunny;
    Object *cube;
	Object *dragon;
	
	// Hold the last mouse position
	int lastX;
	int lastY;

	int timerId;
	int counter;
};

#endif // BASICAPP_H
