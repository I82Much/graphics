#include "RenderWidget0.h"

#include "Camera.h"

#include "SWWidget.h"
#include "SWRenderContext.h"
#include "GLRenderContext.h"

#include "GLWidget.h"
#include "GLRenderContext.h"
#include "VertexData.h"

#include <QtOpenGL>
#include "GeometryFactory.h"
#include "ColorFactory.h"
#include "Vector4.h"
#include "BasicMath.h"

#include "PGMReader.h"
#include "Shader.h"

#include "Materials.h"


#include "Shape3D.h"
#include "TransformGroup.h"


/**
* This class is an OpenGL window, with support for mouse dragging
* and key presses.
* @modified Nicholas Dunn
* @date   February 21, 2009
*/


using namespace RE167;

RenderWidget0::RenderWidget0()
{
	RenderContext *rs = new GLRenderContext();
    //rs->init();


	sceneManager = 0;
	counter = 0;
	QWidget::grabKeyboard();
}

RenderWidget0::~RenderWidget0()
{
	if(sceneManager)
	{
		delete sceneManager;
	}
}



void RenderWidget0::initSceneEvent()
{
    
    
    
	sceneManager = new SceneManager();

    initCamera();
    initLights();
//    initGeometry();
    initRobot();
    createTestScene();
	
	// Trigger timer event every 5ms.
	timerId = startTimer(5);
}

void RenderWidget0::initMaterials()
{}

void RenderWidget0::initGeometry()
{
    
      
    
    Shader * phongShader = new Shader("src/Shaders/diffuse_shading.vert", 
        "src/Shaders/diffuse_shading.frag");
    Shader * texture2D = new Shader("src/Shaders/texture2D.vert", 
        "src/Shaders/texture2D.frag");
    
    Material * brass = new Material(Brass);                                 
    Material * pewter = new Material(Pewter);
    Material * polishedSilver = new Material(Polished_Silver);
    Material * blackRubber = new Material(Black_Rubber);
    Material * ruby = new Material(Ruby);
    Material * turquoise = new Material(Turquoise);   
    Material * mattePearl = new Material(Pearl);   
    Material * defaultMat = new Material();
    Material * emerald = new Material(Emerald);
    
    brass->setShader(phongShader);
    pewter->setShader(phongShader);
    polishedSilver->setShader(phongShader);
    blackRubber->setShader(phongShader);
    ruby->setShader(phongShader);
    turquoise->setShader(phongShader);
    mattePearl->setShader(phongShader);
    emerald->setShader(phongShader);
    
    
    mattePearl->setShininess(2);
    
    // Textured materials
    Material * earthMat = new Material();
    Material * stripes = new Material();
    earthMat->setShader(texture2D);
    stripes->setShader(texture2D);
    

    // Make a shiny brass pedestal
    RE167::Object * pedestal1 = sceneManager->createObject();
    GeometryFactory::createCylinder(pedestal1, 1, 40, 1.0f);
    pedestal1->setTransformation(Matrix4::scale(1,2,1) * Matrix4::translate(-2,0,0));
    pedestal1->setMaterial(brass);


    // Create the pedestal for the globe
    RE167::Object * pedestal2 = sceneManager->createObject();
    GeometryFactory::createCylinder(pedestal2, 1, 40, 1.0f);
    pedestal2->setTransformation(Matrix4::scale(1,2,1) * Matrix4::translate(2,0,0));
    pedestal2->setMaterial(polishedSilver);

    
    earth = sceneManager->createObject();
    GeometryFactory::createSphere(earth, 30, 30);
    earth->setTransformation(Matrix4::translate(2,3,0));
    earth->setMaterial(earthMat);
    
    RE167::Object * cube = sceneManager->createObject();
    GeometryFactory::createCube(cube);
    cube->setTransformation(Matrix4::translate(0,2,0));
    cube->setMaterial(pewter);
    
    
    RE167::Object * teapot = sceneManager->createObject();
    GeometryFactory::createObject(teapot, "objects/teapot.obj");
    teapot->setTransformation(Matrix4::translate(-2,2,0));
    teapot->setMaterial(mattePearl);
    
          
    // http://friday.westnet.com/~crywalt/dymaxion_2003/earthmap10k.reduced.jpg
    QImage *texImg = new QImage("earthmap.jpg", "jpg");
    assert(texImg != NULL);
    Texture *picture = new Texture(texImg);
    earthMat->setTexture(picture);

      
    // http://hi-and-low.typepad.com/my_weblog/images/2007/09/30/stripes_02.jpg
    QImage *stripeImg = new QImage("stripes_02.jpg", "jpg");
    assert(stripeImg != NULL);
    Texture *stripeTex = new Texture(stripeImg);
    stripes->setTexture(stripeTex);
   
    bunny = sceneManager->createObject();
    GeometryFactory::createObject(bunny, "objects/bunny.obj");
    bunny->setTransformation(Matrix4::translate(0,4,0));
    bunny->setMaterial(stripes);
    
    geometryGroup = new TransformGroup();
    geometryGroup->addChild(new Shape3D(bunny));
    //world->addChild(new Shape3D(bunny));
    
    
    
 
}


// TODO: All these new calls will be memory leaks if we don't free them at the end somewhere
void RenderWidget0::initRobot()
{
    
    // Create the geometry
    RE167::Object * cube = sceneManager->createObject();
    GeometryFactory::createCube(cube);
    
    // Cylinders used for arms and legs
    RE167::Object * cylinder = sceneManager->createObject();
    GeometryFactory::createCylinder(cylinder);
    // Make it so that the origin of the cylinder is at the top of the cylinder
    // rather than in the middle
    cylinder->setTransformation(Matrix4::translate(0,-1,0));
    
    // Sphere 
    RE167::Object * sphere = sceneManager->createObject();
    GeometryFactory::createSphere(sphere);
    
    // Represents the center of the torso
    TransformGroup * torsoTransform = new TransformGroup();
    // Right shoulder is at (1,1,0) relative to center of torso
    TransformGroup * rightShoulderTransform = new TransformGroup(Matrix4::translate(1,1,0));
    // Left shoulder is at (-1,1,0)
    TransformGroup * leftShoulderTransform = new TransformGroup(Matrix4::translate(-1,1,0));
    
    // Center of head is (0,2,0)
    TransformGroup * centerOfHeadTransform = new TransformGroup(Matrix4::translate(0,2,0));
    

    centerOfHeadTransform->addChild(new Shape3D(sphere));
    
    
    
    rightShoulderTransform->addChild(new Shape3D(cylinder));
    
    leftShoulderTransform->addChild(new Shape3D(cylinder));
    
    torsoTransform->addChild(new Shape3D(cube));
    
    
    torsoTransform->addChild(centerOfHeadTransform);
    torsoTransform->addChild(rightShoulderTransform);
    torsoTransform->addChild(leftShoulderTransform);
    
    
    // Share the same cylinder object for all the legs
    

    leftLeg = new TransformGroup(Matrix4::translate(-1,-1,0) * Matrix4::scale(.5,1,.5));
    TransformGroup * rightLeg = new TransformGroup(Matrix4::translate(1,-1,0) * Matrix4::scale(.5,1,.5));
    
    leftLeg->addChild(new Shape3D(cylinder));
    rightLeg->addChild(new Shape3D(cylinder));
    
    TransformGroup * leftShin = new TransformGroup(Matrix4::translate(0,-2,0) * leftLeg->getTransformation());
    leftShin->addChild(new Shape3D(cylinder));

    leftLeg->addChild(leftShin);
    
    
    
    torsoTransform->addChild(leftLeg);
    torsoTransform->addChild(rightLeg);
    
    TransformGroup * leftArm = new TransformGroup();
    TransformGroup * rightArm = new TransformGroup();

    TransformGroup * root = sceneManager->getRoot();
    root->addChild(torsoTransform);
    
    robotGroup = torsoTransform;

}



void RenderWidget0::initCamera()
{
    Vector3 cameraCenter = Vector3(0,0,10);
	Vector3 lookAtPoint = Vector3(0,0,-1);
	Vector3 upVector = Vector3(0,1,0);

	camera = sceneManager->createCamera();
	camera->changeSettings(cameraCenter, lookAtPoint, upVector);

	camera->setFrustum(1, 100, 1, BasicMath::radian(60));
}

void RenderWidget0::initLights()
{
    // Make a blue spotlight coming from the left
    Light * blue = sceneManager->createLight();
    blue->setType(Light::SPOT);
    blue->setAmbientColor(Vector3(.2,.2,.2));
    blue->setDiffuseColor(Vector3(0,0,1));
    blue->setSpecularColor(Vector3(1,1,1));
    blue->setSpotDirection(Vector3(0,0,1));
    blue->setPosition(Vector3(-1,.5,1));
    
    // Create a white light
    Light * white = sceneManager->createLight();
    white->setDirection(Vector3(1,1,1));
    white->setDiffuseColor(Vector3(1,1,1));
    white->setAmbientColor(Vector3(.2,.2,.2));
    white->setSpecularColor(Vector3(1,1,1));
    
}


void RenderWidget0::renderSceneEvent()
{
	sceneManager->renderScene();
}

void RenderWidget0::resizeRenderWidgetEvent(const QSize &s)
{
}

void RenderWidget0::timerEvent(QTimerEvent *t)
{
    float DEGREES_PER_TICK = 0.005;
    float degree = counter * DEGREES_PER_TICK;
    
    float MAX_EXTENSION = 45.0f;
    float MIN_EXTENSION = -25.0f;
    
    
    
    
    leftLeg->setTransformation(Matrix4::rotateX(.005 * sin(counter)) * leftLeg->getTransformation());
    //earth->setTransformation(earth->getTransformation() * Matrix4::rotateY(0.005));
	updateScene();
	counter++;
}

void RenderWidget0::mousePressEvent(QMouseEvent *e)
{
	lastX = e->x();
	lastY = e->y();
}

/**
* Handles mouse drags by repositioning camera on virtual
* trackball
*/
void RenderWidget0::mouseMoveEvent(QMouseEvent *e)
{
	int x = e->x();
	int y = e->y();

    
	int canvasWidth = this->width();
	int canvasHeight = this->height();

	// Calculate where (x,y) maps to in the unit sphere
	Vector3 curVector = mapToUnitSphere(x, y, canvasWidth, canvasHeight);

	// Calculate where (lastX, lastY) maps to in the unit sphere
	Vector3 lastVector = mapToUnitSphere(lastX, lastY, canvasWidth, canvasHeight);


	// Calculate the vector around which we need to rotate to take us from lastPoint to curPoint along
	// the sphere
	Vector3 axis = lastVector.crossProduct(curVector);

	// Check whether the cross product is 0.  If it is, then normalizing this
	// will cause a crash.  Just change the vector slightly
	if (axis.magnitude() == 0.0f) {
		axis = Vector3(axis.getX() + .01f, axis.getY() + .01f, axis.getZ() + .01f);
	}

	Vector4 axis4 = Vector4(axis.getX(), axis.getY(), axis.getZ(), 0).normalize();
	float angle = Vector3::angleBetween(lastVector, curVector);


	// Calculate the angle we would need
	// Calculate the transformation needed to rotate
	// the camera
	Matrix4 rotation = Matrix4::rotate(axis4, angle);

	
	// Apply transformation to each object in scene
	

	// Apply transformation to the camera
	camera->setViewMatrix(camera->getViewMatrix() * rotation);
    
    
    /*
    int dx = lastX - x;
    int dy = lastY - y;
    
    static const float scale = 0.5;
    
    float oldYaw = camera->getYaw();
    float oldPitch = camera->getPitch();
    
    camera->setYaw(oldYaw + (dx * scale));
    camera->setPitch(oldPitch + (dy * scale));
    */
    
    
	// Current points are now old points.
	lastX = e->x();
	lastY = e->y();
}


/**
* Given a point in screen coordinates, relative to upper left corner
* of window, returns a vector that represents the point's location in
* the unit sphere.  Does this by first calculating where the point lies
* in the unit square, and then calculating what the z value must be to lie
* on the unit sphere.  Note: If the point could not lie on the unit sphere,
* this method will force it to lie on the sphere by normalizing the (x,y) vector.
*/
Vector3 RenderWidget0::mapToUnitSphere(int x, int y, int width, int height) {
	int smallerSide = std::min(width, height);

	// Since we want the trackball to remain a sphere, we make
	// our sphere have radius min(width, height).

	// Calculate our x and y position relative to the center of
	// the screen.
	int dx = x - (width / 2);
	int dy = y - (height / 2);

	// Divide through by the smaller side.  Now all of the points along
	// the smaller side axis are in the range [-1, 1].  Those on the
	// bigger side might be outside of this range, but they'll be clamped
	// to lie on the sphere anyways.

	float x1 = static_cast<float>(dx) / (smallerSide / 2);
	// y value must be flipped
	float y1 = - static_cast<float>(dy) / (smallerSide / 2);
	float z1;

	float distFromCenter = (x1 * x1) + (y1 * y1);

	// Point does not lie in unit circle; therefore it cannot lie
	// in unit sphere.  Force the point to lie on unit circle with
	// z = 0
	if (distFromCenter > 1) {
		z1 = 0;
		// Normalize x1 and y1 so that the vector (x1, y1, z) has unit
		// length
		x1 = x1 / distFromCenter;
		y1 = y1 / distFromCenter;
	}
	else {
		// Use the formula for a sphere, x^2 + y^2 + z^2 = r^2
		// to solve for z
		z1 = sqrt(1 - (x1 * x1) - (y1 * y1));
	}
	return Vector3(x1, y1, z1);
}


void RenderWidget0::mouseReleaseEvent(QMouseEvent *e)
{
}


void RenderWidget0::startAnimation()
{
	if(!timerId)
	{
		timerId = startTimer(5);
	}
}

void RenderWidget0::stopAnimation()
{
	if(timerId)
	{
		killTimer(timerId);
		timerId = 0;
	}
}

void RenderWidget0::toggleWireframe()
{

}

void RenderWidget0::createTestScene()
{
    
    const int NUM_OBJECTS = 100;
    const int NUM_ROWS = (int) sqrt(static_cast<float>(NUM_OBJECTS));
    const int NUM_COLS = NUM_ROWS;
    
    
    // Each bunny will be spaced .5 apart
    const float SPACING = 1;
    
    
    
    RE167::Object * bunny = sceneManager->createObject();
    GeometryFactory::createObject(bunny, "objects/bunny.obj");
    
    geometryGroup = new TransformGroup();
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            geometryGroup->addChild(new Shape3D(bunny, Matrix4::translate(i * SPACING, j * SPACING, 0)));
        }
    }
    

    
    
    
}

void RenderWidget0::switchScene()
{
    static bool robot = true;
    
    robot = !robot;
    
    if (robot) {
        sceneManager->setRoot(robotGroup);
    }
    else {
        sceneManager->setRoot(geometryGroup);
    }
    
}

void RenderWidget0::keyPressEvent ( QKeyEvent * k )
{
	switch ( k->key() )  {
    // reload
    case Qt::Key_R:                               
        camera->resetViewMatrix();
        break;
    // move forward
    case Qt::Key_W:
        camera->setViewMatrix(Matrix4::translate(0,0,1) * camera->getViewMatrix());
        break;
    // Move camera backwards
    case Qt::Key_S:
        camera->setViewMatrix(Matrix4::translate(0,0,-1) * camera->getViewMatrix());
        break;
    // Move camera left
    case Qt::Key_A:
        camera->setViewMatrix(Matrix4::translate(1,0,0) * camera->getViewMatrix());
        break;
    // Move camera right
    case Qt::Key_D:
        camera->setViewMatrix(Matrix4::translate(-1,0,0) * camera->getViewMatrix());
        break;
    
    // Switch scenes    
    case Qt::Key_T:
        switchScene();
        break;
    
    // Move camera up
    case Qt::Key_Q:
        camera->setViewMatrix(Matrix4::translate(0,-1,0) * camera->getViewMatrix());
        break;
    // Move camera down
    case Qt::Key_Z:
        camera->setViewMatrix(Matrix4::translate(0,1,0) * camera->getViewMatrix());
        break;
	}
}
void RenderWidget0::keyReleaseEvent ( QKeyEvent * e)
{

}



