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

#include "scenegraph/LightNode.h"

#include "scenegraph/Shape3D.h"
#include "scenegraph/TransformGroup.h"
#include "scenegraph/CameraNode.h"
#include "BezierCurve.h"
#include "Vector3.h"
#include "Texture.h"

/**
* This class is an OpenGL window, with support for mouse dragging
* and key presses.
* @modified Nicholas Dunn
* @date   February 21, 2009
*/


using namespace RE167;
using std::cout;
using std::endl;
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
    //initStillLife();

    test();
	
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

void RenderWidget0::initStillLife()
{
    
    
    // try making table
    Vector3 t1(0,4,0);
    Vector3 t2(1,4,0);
    Vector3 t3(2,4,0);
    Vector3 t4(5,4,0);
    // Right side
    Vector3 t5(5,3.5,0);
    Vector3 t6(5,3,0);
    Vector3 t7(5,2.5,0);

    // bottom side
    Vector3 t8(2,2.5,0);
    Vector3 t9(1.5,2.5,0);
    Vector3 t10(.5,2.5,0);
    
    // curve 1
    Vector3 t11(.5,2.5,0);
    Vector3 t12(.7,2,0);
    Vector3 t13(.5,-2,0);
    
    // Bottom base
    Vector3 t14(1,-2.5,0);
    Vector3 t15(2,-3,0);
    Vector3 t16(3,-3.5,0);
    
    Vector3 table[] = {t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16};
    BezierCurve tableCurve(table, 16);
        
    // Make the goblet
    Vector3 goblet1(1,4.3,0);
    Vector3 goblet2(1,0.3,0);
    Vector3 goblet3(-.1,3.3f,0);
    Vector3 goblet4(.1f,0.3f,0);
    Vector3 goblet5(.4f,.2f,0);
    Vector3 goblet6(.7f,.1f,0);
    Vector3 goblet7(1.0f,0.0f,0);
    
    Vector3 gobletArray[] = {goblet1, goblet2, goblet3, goblet4, goblet5,goblet6,goblet7};
    BezierCurve gobletCurve(gobletArray, 7);
        
    // Make the pitcher
    Vector3 pitcher0(44,109+176,0);
    Vector3 pitcher1(29,61+176,0);
    Vector3 pitcher2(26,28+176,0);
    Vector3 pitcher3(34,15+176,0);
    Vector3 pitcher4(70, -21+176,0);
    Vector3 pitcher5(104,-55+176,0);
    Vector3 pitcher6(110,-105+176,0);
    Vector3 pitcher7(114,-152+176,0);
    Vector3 pitcher8(86,-172+176,0);
    Vector3 pitcher9(0,-176+176,0);
    
    Vector3 pitcherArray[] = {pitcher0, pitcher1, pitcher2, pitcher3, 
        pitcher4, pitcher5, pitcher6, pitcher7, pitcher8, pitcher9 };
    BezierCurve pitcherCurve(pitcherArray, 10);
    
    
    // Make a plate
    Vector3 plate0(0,1,0);
    Vector3 plate1(2,1,0);
    Vector3 plate2(3,1,0);
    Vector3 plate3(4,1.6,0);
    
    // Little curved lip
    Vector3 plate4(4.5,1.7,0);
    Vector3 plate5(4.5,.7,0);
    Vector3 plate6(4,.6,0);

    // Bottom part
    Vector3 plate7(3,0,0);
    Vector3 plate8(2,0,0);
    Vector3 plate9(0,0,0);
    
    Vector3 plateArray[] = {plate0, plate1, plate2, plate3, 
        plate4, plate5, plate6, plate7, plate8, plate9 };
    BezierCurve plateCurve(plateArray, 10);
    
    Material * gobletMaterial = new Material();
    Material * tableMaterial = new Material(Polished_Gold);
    Material * plateMaterial = new Material(Pearl);
    Material * pitcherMaterial = new Material(Jade);
    
    // Set up the textures
    // http://friday.westnet.com/~crywalt/dymaxion_2003/earthmap10k.reduced.jpg
    QImage *texImg = new QImage("earthmap.jpg", "jpg");
    assert(texImg != NULL);
    Texture *earthMap = new Texture(texImg);
    pitcherMaterial->setTexture(earthMap);

    // http://hi-and-low.typepad.com/my_weblog/images/2007/09/30/stripes_02.jpg
    QImage *stripeImg = new QImage("stripes_02.jpg", "jpg");
    assert(stripeImg != NULL);
    Texture *stripeTex = new Texture(stripeImg);
    tableMaterial->setTexture(stripeTex);
    
    // http://colorvisiontesting.com/plate%20with%205.jpg
    QImage *patternImg = new QImage("plate with 5.jpg", "jpg");
    assert(patternImg != NULL);
    Texture *patternTex = new Texture(patternImg);
    plateMaterial->setTexture(patternTex);
    
    // http://images.google.com/hosted/life/f?q=sculpture+source:life&prev=/images%3Fq%3Dsculpture%2Bsource:life%26ndsp%3D18%26hl%3Den%26sa%3DN%26start%3D54&imgurl=f4c508a0f589ee70
    QImage *davidImg = new QImage("david.jpg", "jpg");
    assert(davidImg != NULL);
    Texture *davidTex = new Texture(davidImg);
    gobletMaterial->setTexture(davidTex);
    
    
    Object * tableObject = sceneManager->createObject();
    Object * plateObject = sceneManager->createObject();
    Object * gobletObject = sceneManager->createObject();
    Object * pitcherObject = sceneManager->createObject();
    
    tableObject->setMaterial(tableMaterial);
    gobletObject->setMaterial(gobletMaterial);
    plateObject->setMaterial(plateMaterial);
    pitcherObject->setMaterial(pitcherMaterial);
    
    
    GeometryFactory::createSurfaceOfRevolution(pitcherObject,pitcherCurve);
    GeometryFactory::createSurfaceOfRevolution(tableObject,tableCurve);
    GeometryFactory::createSurfaceOfRevolution(gobletObject, gobletCurve);        
    GeometryFactory::createSurfaceOfRevolution(plateObject, plateCurve);    
    
    
    TransformGroup * tableGroup = new TransformGroup(Matrix4::scale(0.6,0.6,0.6));
    tableGroup->addChild(new Shape3D(tableObject));
    TransformGroup * tableTop = new TransformGroup(Matrix4::translate(0,4,0));
    tableGroup->addChild(tableTop);


    TransformGroup * plateGroup = new TransformGroup(Matrix4::translate(0,0,1.5));
    plateObject->setTransformation(Matrix4::scale(0.5,0.5,0.5));
    plateGroup->addChild(new Shape3D(plateObject));
    tableTop->addChild(plateGroup);
    

    // We constructed the goblet to have origin at the bottom of it
    gobletObject->setTransformation(Matrix4::scale(0.6,0.6,0.6));
    TransformGroup * gobletTransform = new TransformGroup(Matrix4::translate(1.5,0,-2));
    gobletTransform->addChild(new Shape3D(gobletObject));
    plateGroup->addChild(gobletTransform);

    
    pitcherObject->setTransformation(Matrix4::scale(0.01, 0.01, 0.01)); 
    // Make the pitcher be a little to the right of the goblet   
    TransformGroup * pitcherTransform = new TransformGroup(Matrix4::translate(-2.5,0,-3));
    pitcherTransform->addChild(new Shape3D(pitcherObject));
    plateGroup->addChild(pitcherTransform);
    

    
    sceneManager->setRoot(tableGroup);


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
	
	
    CameraNode * cn = new CameraNode(camera);
//    torsoTransform->addChild(cn);
  //  geometryGroup->addChild(cn);
    
    //sceneManager->getRoot()->addChild(cn);
	
    
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
    blue->setPosition(Vector3(0,1,0));
    
    
    LightNode * blueLight = new LightNode(blue);

    
    // Create a white light
    Light * white = sceneManager->createLight();
    white->setDirection(Vector3(1,1,1));
    white->setDiffuseColor(Vector3(1,1,1));
    white->setAmbientColor(Vector3(.2,.2,.2));
    white->setSpecularColor(Vector3(1,1,1));
    
    LightNode * whiteLight = new LightNode(white);
    
    
    sceneManager->getRoot()->addChild(blueLight);
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
	

	// Apply transformation to the objects in the scene
    TransformGroup * root = sceneManager->getRoot();
    root->setTransformation(rotation * root->getTransformation());
	
//	camera->setViewMatrix(camera->getViewMatrix() * rotation);
    
    
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


void RenderWidget0::toggleCulling()
{
    static bool objectLevelCulling = true;
    objectLevelCulling = !objectLevelCulling;
    std::cout << "Object level culling: " << (objectLevelCulling ? "true" : "false") << std::endl;
    sceneManager->setObjectLevelCulling(objectLevelCulling);
}

void RenderWidget0::keyPressEvent ( QKeyEvent * k )
{
    
    TransformGroup * root = sceneManager->getRoot();
         
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
        root->setTransformation(Matrix4::translate(1,0,0) * root->getTransformation());
    
        //camera->setViewMatrix(Matrix4::translate(1,0,0) * camera->getViewMatrix());
        break;
    // Move camera right
    case Qt::Key_Right: //D:
        root->setTransformation(Matrix4::translate(-1,0,0) * root->getTransformation());
    
        //camera->setViewMatrix(Matrix4::translate(-1,0,0) * camera->getViewMatrix());
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

void RenderWidget0::test() 
{
    
    
    Vector3 t1(0,4,1);
    Vector3 t2(1,3,5);
    Vector3 t3(2,2,6);
    Vector3 t4(3,3,3);
    Vector3 t5(5,3,2);
    Vector3 t6(5.5,4,4);
    Vector3 t7(6,1,5);
    
    Vector3 p1(1,4,0);
    Vector3 p2(2,3,0);
    Vector3 p3(3,2,0);
    Vector3 p4(4,1,0);
    
    

    Vector3 pathArray[] = {t1,t2,t3,t4,t5,t6,t7};
    //Vector3 pathArray[] = {p1,p2,p3,p4};


    BezierCurve path(pathArray, 7);

    Vector3 c1(0,0,.5);
    Vector3 c2(.5,0,.5);
    Vector3 c3(.5,0,-.5);
    Vector3 c4(0,0,-.5);
    Vector3 c5(-.5,0,-.5);
    Vector3 c6(-.5,0,.5);
    Vector3 c7(0,0,.5);
    
    
    Vector3 s1(2,0,0);
    Vector3 s2(1,0,0);
    Vector3 s3(0,0,2);
    Vector3 s4(0,0,3);
    Vector3 s5(0,0,2);
    Vector3 s6(-1,0,0);
    Vector3 s7(-2,0,0);
    
    
    Vector3 shapeArray[] = {c1,c2,c3,c4,c5,c6,c7};
    //Vector3 shapeArray[] = {s1,s2,s3,s4,s5,s6,s7};
    
    BezierCurve curvedLine(shapeArray, 7);
    
    Object * loft = sceneManager->createObject();
    GeometryFactory::createLoft(loft, curvedLine, path,18 ,18);
    
    
    Material * extrudedShapeMaterial = new Material();
    
    // Set up the textures
    // http://friday.westnet.com/~crywalt/dymaxion_2003/earthmap10k.reduced.jpg
    QImage *texImg = new QImage("earthmap.jpg", "jpg");
    assert(texImg != NULL);
    Texture *earthMap = new Texture(texImg);
    extrudedShapeMaterial->setTexture(earthMap);
    
    
    loft->setMaterial(extrudedShapeMaterial);
    
    sceneManager->getRoot()->addChild(new Shape3D(loft));
    
    
    
    
}


