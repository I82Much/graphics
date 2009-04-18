#include "RenderWidget0.h"

#include "Camera.h"
#include "Object.h"

#include "SWWidget.h"
//#include "SWRenderContext.h"
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
#include "SceneManager.h"
#include "Material.h"


#include "scenegraph/Shape3D.h"
#include "scenegraph/TransformGroup.h"
#include "Texture.h"

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
	
	// Show the bunnies by default
    sceneManager->setRoot(geometryGroup);
	
    //test();
	
	
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

void RenderWidget0::test()
{
    Object * cube = sceneManager->createObject();
    GeometryFactory::createCube(cube);
    
    std::cout << "Center: " << cube->getSphereCenter() << " Radius: " << cube->getSphereRadius() << std::endl;
/*    assert(cube->getSphereCenter() == Vector4(0,0,0,1));
    assert(cube->getSphereRadius() == sqrt(2.0f));*/
    
    sceneManager->getRoot()->addChild(new Shape3D(cube));
    
}


// TODO: All these new calls will be memory leaks if we don't free them at the end somewhere
void RenderWidget0::initRobot()
{
    Shader * phongShader = new Shader("src/Shaders/diffuse_shading.vert", 
        "src/Shaders/diffuse_shading.frag");
    Shader * texture2D = new Shader("src/Shaders/texture2D.vert", 
        "src/Shaders/texture2D.frag");
    

    Material * brass = new Material(Brass);
    Material * blackRubber = new Material(Black_Rubber);
    Material * polishedSilver = new Material(Polished_Silver);
    Material * pewter = new Material(Pewter);
    
    /*brass->setShader(phongShader);
    pewter->setShader(phongShader);
    polishedSilver->setShader(phongShader);
    blackRubber->setShader(phongShader);*/
    
    // Create the geometry
    RE167::Object * cube = sceneManager->createObject();
    GeometryFactory::createCube(cube);
    cube->setMaterial(brass);
    
    RE167::Object * taperedCylinder = sceneManager->createObject();
    GeometryFactory::createTaperedCylinder(taperedCylinder, 1, 20, .8f, .5f);
    taperedCylinder->setTransformation(Matrix4::scale(1,0.5,1));
    taperedCylinder->setMaterial(brass);
    
    
    // Cylinders used for arms and legs
    RE167::Object * legCylinder = sceneManager->createObject();
    GeometryFactory::createCylinder(legCylinder);
    // Make it so that the origin of the cylinder is at the top of the cylinder
    // rather than in the middle
    legCylinder->setTransformation(Matrix4::translate(0,-1,0) * Matrix4::scale(.5,1,.5));
    legCylinder->setMaterial(polishedSilver);
    
    
    RE167::Object * armCylinder = sceneManager->createObject();
    GeometryFactory::createCylinder(armCylinder);
    // Make it so that the origin of the cylinder is at the top of the cylinder
    // rather than in the middle.  Will be 1.5 units long instead of .75
    armCylinder->setTransformation(Matrix4::translate(0,-.75,0) * Matrix4::scale(.5,.75,.5));
    armCylinder->setMaterial(pewter);
    
    Shape3D * armShape = new Shape3D(armCylinder);
    

    // Represents the center of the torso.  Its center is at (0,1,0) relative to pelvis
    torsoTransform = new TransformGroup(Matrix4::translate(0,1,0));
    // Right shoulder is at (1,1,0) relative to center of torso.  It also is rotated 90 degrees
    // about the z axis so that the local coordinate system of the right shoulder points into
    // the body.
    TransformGroup * rightShoulderTransform = new TransformGroup(Matrix4::translate(1,1,0) * Matrix4::rotateZ(BasicMath::radians(90)));
    // Left shoulder is at (-1,1,0)
    TransformGroup * leftShoulderTransform = new TransformGroup(Matrix4::translate(-1,1,0) * Matrix4::rotateZ(BasicMath::radians(-90)));
    
    leftUpperArm = new TransformGroup();
    rightUpperArm = new TransformGroup();
    
    leftUpperArm->addChild(armShape);
    rightUpperArm->addChild(armShape);
    
    
    leftLowerArm = new TransformGroup(Matrix4::translate(0,-1.5,0));
    rightLowerArm = new TransformGroup(Matrix4::translate(0,-1.5,0));

    leftUpperArm->addChild(leftLowerArm);
    rightUpperArm->addChild(rightLowerArm);

    
    leftLowerArm->addChild(armShape);
    rightLowerArm->addChild(armShape);
    
    
    leftHand = new TransformGroup(Matrix4::translate(0,-2,0) * Matrix4::scale(0.3f, 0.5f, 0.3f));
    rightHand = new TransformGroup(Matrix4::translate(0,-2,0) * Matrix4::scale(0.3f, 0.5f, 0.3f));
    
    leftHand->addChild(new Shape3D(cube));
    rightHand->addChild(new Shape3D(cube));
    
    leftLowerArm->addChild(leftHand);
    rightLowerArm->addChild(rightHand);
    
    
    
    rightShoulderTransform->addChild(leftUpperArm);
    
    leftShoulderTransform->addChild(rightUpperArm);
    
    
    
    
    // Center of head is (0,1.5,0) relative to torso
    TransformGroup * centerOfHeadTransform = new TransformGroup(Matrix4::translate(0,1.5,0));
    TransformGroup * robotHead = new TransformGroup(Matrix4::scale(0.5,0.5,0.5));
    robotHead->addChild(new Shape3D(cube));
    
    
    // Represents the center of the pelvis of the robot.  Pelvis itself will be
    // represented as a shallow cone
    pelvis = new TransformGroup(Matrix4::translate(0,-1,0));
    
    pelvis->addChild(new Shape3D(taperedCylinder));
    pelvis->addChild(torsoTransform);
    
    

    centerOfHeadTransform->addChild(robotHead);
    
    torsoTransform->addChild(new Shape3D(cube));
    
    
    torsoTransform->addChild(centerOfHeadTransform);
    torsoTransform->addChild(rightShoulderTransform);
    torsoTransform->addChild(leftShoulderTransform);

    
    
    // Share the same cylinder object for all the legs
    
    // Legs will be positioned relative to the pelvis; slightly below
    leftLeg = new TransformGroup(Matrix4::translate(-.7f, -.1, 0));
    rightLeg = new TransformGroup(Matrix4::translate(.7f, -.1, 0));
    
    
    pelvis->addChild(leftLeg);
    pelvis->addChild(rightLeg);
    
    leftLeg->addChild(new Shape3D(legCylinder));
    rightLeg->addChild(new Shape3D(legCylinder));
    
    leftShin = new TransformGroup(Matrix4::translate(0,-2,0));
    leftShin->addChild(new Shape3D(legCylinder));

    leftLeg->addChild(leftShin);
    
    rightShin = new TransformGroup(Matrix4::translate(0,-2,0));
    rightShin->addChild(new Shape3D(legCylinder));
    rightLeg->addChild(rightShin);
    
 
   
    TransformGroup * root = sceneManager->getRoot();
    root->addChild(pelvis);
    
    robotGroup = pelvis;
    
    
    
    
    
}



void RenderWidget0::initCamera()
{
    Vector3 cameraCenter = Vector3(0,0,10);
	Vector3 lookAtPoint = Vector3(0,0,-1);
	Vector3 upVector = Vector3(0,1,0);

	camera = sceneManager->createCamera();
	
	// TODO: the camera stuff will only work if setFrustum is called first!!
	
	camera->setFrustum(1, 100, 1, BasicMath::radians(60));
	camera->changeSettings(cameraCenter, lookAtPoint, upVector);
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
    
    /*
    // Make a green spotlight coming from the right
    Light * green = sceneManager->createLight();
    green->setType(Light::SPOT);
    green->setAmbientColor(Vector3(.2,.2,.2));
    green->setDiffuseColor(Vector3(0,1,0));
    green->setSpecularColor(Vector3(1,1,1));
    green->setSpotDirection(Vector3(0,0,-1));
    green->setPosition(Vector3(1,.5,1));
    */
    
    
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
    
    float SLOW_DEGREES_PER_TICK = 0.1;
    float degree = counter * SLOW_DEGREES_PER_TICK;
    
    float FAST_DEGREES_PER_TICK = 0.2;
    
    float MAX_EXTENSION = 45.0f;
    
    static float sign = 1.0f;

    if (degree > MAX_EXTENSION) {
        counter = 0;
        sign *= -1;
    }
    
    leftLeg->setTransformation(leftLeg->getTransformation() * Matrix4::rotateX(BasicMath::radians(sign * SLOW_DEGREES_PER_TICK)));
    leftShin->setTransformation(leftShin->getTransformation() * Matrix4::rotateX(BasicMath::radians(sign * FAST_DEGREES_PER_TICK)));
    
    leftUpperArm->setTransformation(leftUpperArm->getTransformation() * Matrix4::rotateZ(BasicMath::radians(sign * FAST_DEGREES_PER_TICK)));
    rightUpperArm->setTransformation(rightUpperArm->getTransformation() * Matrix4::rotateZ(BasicMath::radians(sign * FAST_DEGREES_PER_TICK)));
    
    leftLowerArm->setTransformation(leftLowerArm->getTransformation() * Matrix4::rotateX(BasicMath::radians(-sign * FAST_DEGREES_PER_TICK)));
    rightLowerArm->setTransformation(rightLowerArm->getTransformation() * Matrix4::rotateX(BasicMath::radians(-sign * SLOW_DEGREES_PER_TICK)));
    
    leftHand->setTransformation(leftHand->getTransformation() * Matrix4::rotateY(BasicMath::radians(-sign * SLOW_DEGREES_PER_TICK)));
    rightHand->setTransformation(rightHand->getTransformation() * Matrix4::rotateY(BasicMath::radians(-sign * SLOW_DEGREES_PER_TICK)));
    
    torsoTransform->setTransformation(torsoTransform->getTransformation() * Matrix4::rotateY(BasicMath::radians(-sign * SLOW_DEGREES_PER_TICK)));
    
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

/**
* Sets up a scene with >= 100 instances of a complicated object for testing
* the Object-Level Culling
**/
void RenderWidget0::createTestScene()
{
    Material * brass = new Material(Brass);                                 
    Material * pewter = new Material(Pewter);
    Material * polishedSilver = new Material(Polished_Silver);
    Material * blackRubber = new Material(Black_Rubber);
    Material * ruby = new Material(Ruby);
    Material * turquoise = new Material(Turquoise);   
    Material * mattePearl = new Material(Pearl);   
    Material * defaultMat = new Material();
    Material * emerald = new Material(Emerald);
    
    
    const int NUM_OBJECTS = 100;
    const int NUM_ROWS = (int) sqrt(static_cast<float>(NUM_OBJECTS));
    const int NUM_COLS = NUM_ROWS;

    const float SPACING = 2;
        
    RE167::Object * bunny = sceneManager->createObject();
    GeometryFactory::createObject(bunny, "objects/bunny.obj");
    
    // Test object stuff
    /*   Object * bunny = new Object();
       GeometryFactory::createObject(bunny, "objects/teapot.obj");*/
       std::cout << "Center of bunny: " << bunny->getSphereCenter() << " radius of sphere: " << bunny->getSphereRadius() << std::endl;
    
    
    
    Shape3D * metalBunny = new Shape3D(bunny, polishedSilver);
    Shape3D * emeraldBunny = new Shape3D(bunny, emerald);
    Shape3D * rubyBunny = new Shape3D(bunny, ruby);
    
    Shape3D * bunnies[] = {
        new Shape3D(bunny, brass),
        new Shape3D(bunny, pewter),
        new Shape3D(bunny, polishedSilver),
        new Shape3D(bunny, blackRubber),
        new Shape3D(bunny, ruby),
        new Shape3D(bunny, turquoise),
        new Shape3D(bunny, mattePearl),
        new Shape3D(bunny, defaultMat),
        new Shape3D(bunny, emerald)
    };
    
    
    geometryGroup = new TransformGroup();
    for (int i = 0, counter = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            TransformGroup * curBunny = new TransformGroup(Matrix4::translate(i * SPACING, j * SPACING, 0));
            curBunny->addChild(bunnies[counter % 9]);
            geometryGroup->addChild(curBunny);
            counter++;
        }
    }
}

void RenderWidget0::switchScene()
{
    static bool robot = true;
    
    robot = !robot;
    
    if (robot) {
        std::cout << "Switched to robot scene." << std::endl;
        this->parentWidget()->setWindowTitle("Do the Robot!");
        sceneManager->setRoot(robotGroup);
    }
    else {
        std::cout << "Switched to rabbit scene." << std::endl;
        this->parentWidget()->setWindowTitle("Multiply like rabbits!");
        sceneManager->setRoot(geometryGroup);
    }
}

void RenderWidget0::toggleCulling()
{
    static bool objectLevelCulling = true;
    objectLevelCulling = !objectLevelCulling;
    std::cout << "Object level culling: " << (objectLevelCulling ? "true" : "false") << std::endl;
    sceneManager->setObjectLevelCulling(objectLevelCulling);
}

void RenderWidget0::keyPressEvent ( QKeyEvent * k )
{
        
	switch ( k->key() )  {
    // reload
    case Qt::Key_R:                               
        camera->resetViewMatrix();
        break;
    // move forward
    case Qt::Key_Up: // Qt::Key_W
        camera->setViewMatrix(Matrix4::translate(0,0,1) * camera->getViewMatrix());
        break;
    // Move camera backwards
    case Qt::Key_Down: //Qt::Key_S:
        camera->setViewMatrix(Matrix4::translate(0,0,-1) * camera->getViewMatrix());
        break;
    // Move camera left
    case Qt::Key_Left: //Key_A:
        camera->setViewMatrix(Matrix4::translate(1,0,0) * camera->getViewMatrix());
        break;
    // Move camera right
    case Qt::Key_Right: //D:
        camera->setViewMatrix(Matrix4::translate(-1,0,0) * camera->getViewMatrix());
        break;
    
    // Switch scenes    
    case Qt::Key_S:
        switchScene();
        break;
    
    // Toggle object level culling
    case Qt::Key_C:
        toggleCulling();
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



