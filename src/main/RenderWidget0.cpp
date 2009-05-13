#include "RenderWidget0.h"

#include "Camera.h"
#include "Object.h"

#include "SWWidget.h"
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
#include "spline/BezierCurve.h"
#include "spline/Circle.h"
#include "spline/Helix.h"
#include "spline/Morpher.h"
#include "spline/Square.h"
#include "spline/PiecewiseSpline.h"





#include "Vector3.h"
#include "Texture.h"

/**
* This class is an OpenGL window, with support for mouse dragging
* and key presses.
* @modified Nicholas Basis
* @date   February 21, 2009
*/


using namespace RE167;
using std::cout;
using std::endl;
RenderWidget0::RenderWidget0()
{
    // Store a reference for later
    this->rs = new GLRenderContext();
        
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
	//initLights();
    //initStillLife();

    test();
	
	// Trigger timer event every 5ms.
	timerId = startTimer(5);
}

void RenderWidget0::initMaterials()
{}

void RenderWidget0::initCamera() {
    Vector3 cameraCenter = Vector3(0,0,10);
	Vector3 lookAtPoint = Vector3(0,0,-1);
	Vector3 upVector = Vector3(0,1,0);

//	camera = sceneManager->createCamera();
	
	// TODO: the camera stuff will only work if setFrustum is called first!!
	
	Camera* camera = new Camera();
	camera->setFrustum(0.5, 100, 1, BasicMath::radians(60));
	camera->changeSettings(cameraCenter, lookAtPoint, upVector);
	
	
    stillCamera = new CameraNode(camera);
	stillCamera->makeUnaffected(); // now the still camera will not be affected by the scenegraph
    
    sceneManager->getRoot()->addChild(stillCamera);
	
    
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
	blue->setSpotCutoff(90.0);
	blue->setSpotExponent(1.0);
    
    
    LightNode * blueLight = new LightNode(blue);

/*    
    // Create a white light
    Light * white = sceneManager->createLight();
	white->setType(Light::SPOT);
    white->setSpotDirection(Vector3(1,1,1));
    white->setDiffuseColor(Vector3(1,1,1));
    white->setAmbientColor(Vector3(.2,.2,.2));
    white->setSpecularColor(Vector3(1,1,1));
	white->setSpotCutoff(90.0);
	white->setSpotExponent(1.0);
    
    LightNode * whiteLight = new LightNode(white);
*/    
    
    sceneManager->getRoot()->addChild(blueLight);
//	sceneManager->getRoot()->addChild(whiteLight);
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
    static int segment = 0;
	
	// true indicates that adaptive sampling will be used
	static const std::vector<Basis> referenceFrames = track->getReferenceFrames(10000, true);
	
    static const int numSegments = referenceFrames.size();
	
    Vector3 loc = referenceFrames[segment % numSegments].getOrigin();
    segment++;
    
    // in minecart coordinates, the z-axis is the front and the y-axis points up
	Vector3 zAxis(0,0,1);
	Vector3 yAxis(0,1,0);
	// we want the z-axis to line up with the tangent vector and the y-axis to line up with the normal vector
	Vector3 tangent = referenceFrames[(segment-1) % numSegments].getV();
	Vector3 normal = referenceFrames[(segment-1) % numSegments].getV();
	
	Matrix4 rotationMatrix;
	if (zAxis == tangent) {
		rotationMatrix = Matrix4::IDENTITY;
	}
	else {
		Vector3 axis = tangent.crossProduct(zAxis).normalize();
		Vector4 axisOfRotationVec4 = Vector4::homogeneousVector(axis);
		
		float angle = Vector3::angleBetween(tangent, zAxis);
		rotationMatrix = Matrix4::rotate(axisOfRotationVec4, angle);
	}
	
	// then we need to make sure that the y-axis lines up with the normal vector
	Matrix4 rotationMatrix2;
	Vector4 newYAxis = (rotationMatrix*Vector4::homogeneousVector(yAxis)).normalize();
	if (newYAxis == normal) {
		rotationMatrix2 = Matrix4::IDENTITY;
	}
	else {
		Vector3 axis = normal.crossProduct(newYAxis).normalize();
		Vector4 axisOfRotationVec4 = Vector4::homogeneousVector(axis);
		
		float angle = Vector3::angleBetween(normal,newYAxis);
		rotationMatrix2 = Matrix4::rotate(axisOfRotationVec4,angle);
	}
	
    minecart->setTransformation(Matrix4::translate(loc.getX(), loc.getY(), loc.getZ())*rotationMatrix2*rotationMatrix);
    
	// we now add code to make the camera follow the mine cart
	// We do not need to worry about changing the center of projection because in the scene graph, the camera is a child
	// of the minecart so that center should follow the minecart.
	// We do have to change the lookAtPoint and the lookUpVector and for that we need the referenceFrames for the track
	// the tangent is v and the normal is u
	// (the -1 is because segment has been incremented)
	Vector3 newLookAt = movingCamera->getCenterOfProjection() + referenceFrames[(segment-1) % numSegments].getV();
	Vector3 newLookUp = referenceFrames[(segment-1) % numSegments].getU();
	
	// now we update the camera
	movingCamera->updateProjection(movingCamera->getCenterOfProjection(), newLookAt, newLookUp);
//	whiteLight->setSpotDirection(referenceFrames[(segment-1) % numSegments].getV());
		    
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
Vector3 RenderWidget0::mapToUnitSphere(int x, int y, int width, int height) 
{
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


void RenderWidget0::toggleCulling()
{
    static bool objectLevelCulling = true;
    objectLevelCulling = !objectLevelCulling;
    std::cout << "Object level culling: " << (objectLevelCulling ? "true" : "false") << std::endl;
    sceneManager->setObjectLevelCulling(objectLevelCulling);
}


// TODO: Clean all this code up
void RenderWidget0::keyPressEvent ( QKeyEvent * k )
{
    
    TransformGroup * root = sceneManager->getRoot();
         
	switch ( k->key() )  {
			// reload
		case Qt::Key_R:                               
			stillCamera->resetTransformation();
			break;
		// move forward
		case Qt::Key_Up: // Qt::Key_W
			stillCamera->translateCamera(0,0,-1);
			break;
		// Move camera backwards
		case Qt::Key_Down: //Qt::Key_S:
			stillCamera->translateCamera(0,0,1);
			break;
		// Move camera left
		case Qt::Key_Left: //Key_A:
			stillCamera->translateCamera(-1,0,0);
			break;
		// Move camera right
		case Qt::Key_Right: //D:
			stillCamera->translateCamera(1,0,0);
			break;
    
    
		// Toggle object level culling
		case Qt::Key_C:
			toggleCulling();
			break;
    
		// Move camera up
		case Qt::Key_Q:
			stillCamera->translateCamera(0,1,0);
			break;
		// Move camera down
		case Qt::Key_Z:
			stillCamera->translateCamera(0,-1,0);
			break;
			
			
			
		// switch between moving camera and still camera
		case Qt::Key_M:
			if (stillCamera->inUse()) {
				stillCamera->disable();
				movingCamera->use();
			}
			else {
				movingCamera->disable();
				stillCamera->use();
			}
			break;
	}
}
void RenderWidget0::keyReleaseEvent ( QKeyEvent * e)
{

}

void RenderWidget0::toggleWireframe()
{
    rs->toggleWireframe();
}


void RenderWidget0::test() 
{
    
    Vector3 minecart1(3,3,0);
    Vector3 minecart2(2.5,2,0);
    Vector3 minecart3(2,1,0);
    Vector3 minecart4(1.5,0,0);
    Vector3 minecart5(1,0,0);
    Vector3 minecart6(.5,0,0);
    Vector3 minecart7(0,0,0);
    
    Vector3 minecartArray[] = {minecart1, minecart2, minecart3, minecart4, minecart5, minecart6, minecart7 };
    
    BezierCurve minecartProfile(minecartArray, sizeof(minecartArray) / sizeof(Vector3));
    
    /*
     Object * mineCartObj = sceneManager->createObject();
     GeometryFactory::createSphere(mineCartObj);
     mineCartObj->setTransformation(Matrix4::scale(.001,.001,.001));
     */

     Object * minecartObj = GeometryFactory::createSurfaceOfRevolution(minecartProfile, 3, 4);

     // Need to compensate for surface of revolution weirdness.
     minecartObj->setTransformation(Matrix4::rotateX(BasicMath::radians(90)) * Matrix4::rotateY(BasicMath::radians(45)));

     Shape3D * minecartShape = new Shape3D(minecartObj);
     minecart = new TransformGroup();
     minecart->addChild(minecartShape);

    

    // this shader supports two spot lights
	Shader* twoSpotTexture = new Shader("src/Shaders/finalSpotLights.vert", "src/Shaders/finalSpotLights.frag");
	twoSpotTexture = NULL;
	// this shader should support 8 lights - 2 spot lights and 6 point lights
//	Shader* lightingTexture = new Shader("src/Shaders/finalLight.vert", "src/Shaders/finalLight.frag");

    
    Vector3 track1(0, -7998, 0);
    Vector3 track2(0, -3895, 0);
    Vector3 track3(0, -793, 0);
    Vector3 track4(209, 11, 0);
    Vector3 track5(934, 446, 0);
    Vector3 track6(1777, 22, 0);
    Vector3 track7(2011, -934, -500);
    Vector3 track8(2011, -2458, -2536);
    Vector3 track9(1986, -4130, -4754);
    Vector3 track10(1986, -4100, -4800);
    Vector3 track11(3000, -8000, -5000);
    Vector3 track12(3166, -13013, -4700);
    Vector3 track13(0, -15000, -5869);
    Vector3 track14(-2000, -15000, -5869);
    Vector3 track15(-3000, -15000, -5869);
    Vector3 track16(-4000, -13000, -5869);
    Vector3 trackArray[] = {track1, track2, track3, track4, track5, track6, 
        track7, track8, track9, track10, track11, track12, track13, track14, track15, track16};


    /*track = new Helix(10);
    track->setTransformation(Matrix4::scale(3,3,10));
*/

    track = new BezierCurve(trackArray, sizeof(trackArray) / sizeof(Vector3));
    track->setTransformation(Matrix4::scale(.001, .001, .001));

    


    Helix helix(2);
    //    helix.setTransformation(Matrix4::scale(1,1,20));
    
    Vector3 s1(2,0,0);
    Vector3 s2(1,0,0);
    Vector3 s3(0,0,2);
    Vector3 s4(0,0,3);
    Vector3 s5(0,0,2);
    Vector3 s6(-1,0,0);
    Vector3 s7(-2,0,0);
        
    Vector3 torch1(.5,2,0);
    Vector3 torch2(.3,1.5,0);
    Vector3 torch3(.1,.1,0);
    Vector3 torch4(0,0,0);
    
    BezierCurve torchCurve(torch1, torch2, torch3, torch4);
    Object * torch = GeometryFactory::createSurfaceOfRevolution(torchCurve);
    sceneManager->getRoot()->addChild(new Shape3D(torch));
    
    
    Vector3 str1(0,1,0);
    Vector3 str2(0,.5,0);
    Vector3 str3(0,-.5,0);
    Vector3 str4(0,-1,0);
        
    BezierCurve straightLine(str1, str2, str3, str4);
    
    Circle circle2;
    circle2.setTransformation(Matrix4::rotateX(BasicMath::radians(90)));
    
    Square square;
    square.setTransformation(Matrix4::scale(4,4,4) * Matrix4::rotateX(BasicMath::radians(90)));
    
    Object * loft = GeometryFactory::createLoft(square, *track , 5 ,50);
    
    Material * extrudedShapeMaterial = new Material(Brass);
	//extrudedShapeMaterial->setShader(twoSpotTexture);
    
    // Set up the textures

    //http://ryane.com/wp-content/uploads/2007/04/rock_02.jpg
    QImage *rockImg = new QImage("images/rock_02.jpg", "jpg");
    assert(rockImg != NULL);
    Texture *rockTexture = new Texture(rockImg);
    extrudedShapeMaterial->setTexture(rockTexture);

    loft->setMaterial(extrudedShapeMaterial);
    
    Circle circle;
    circle.setTransformation(Matrix4::scale(.5,.5,.5) * Matrix4::rotateX(BasicMath::radians(90)));
    
     
    track->setTransformation(Matrix4::translate(0,0,-3) * track->getTransformation());
    

    Object * trackLoft = GeometryFactory::createLoft(circle,  *track, 10 ,50);
    //trackLoft->setMaterial(new Material(Bronze));
    
    
//    sceneManager->getRoot()->addChild(new Shape3D(loft));
    sceneManager->getRoot()->addChild(new Shape3D(trackLoft));
    
    
 
	// first we have to create the camera and cameraNode:
	Camera* moveCamera = new Camera();
	movingCamera = new CameraNode(moveCamera);
	// now we modify the camera so that it sits where we want it to relative to the minecart
	movingCamera->updateProjection(Vector3(0,0.5,0),Vector3(0,0.5,-1),Vector3(0,1,0));
	// then we add it as a child of the minecart so that it follows the cart around
	minecart->addChild(movingCamera);
	
	// last thing to do is make sure that when the scene starts, we are using the moving camera, not the still one
	movingCamera->use();
	stillCamera->disable();
	
	
	// now we have to set up the lighting....
    // Create a white light
    Light * white = sceneManager->createLight();
	white->setType(Light::SPOT);
    white->setSpotDirection(Vector3(0,0,-1));
    white->setDiffuseColor(Vector3(1,1,1));
    white->setAmbientColor(Vector3(.2,.2,.2));
    white->setSpecularColor(Vector3(1,1,1));
	white->setSpotCutoff(30.0);
	white->setSpotExponent(1.0);
	white->setPosition(Vector3(0,0,0));
    
    whiteLight = new LightNode(white);
	whiteLight->setPosition(Vector3(0,0,0));
	minecart->addChild(whiteLight);
	
	

    sceneManager->getRoot()->addChild(minecart);
    
    
}


