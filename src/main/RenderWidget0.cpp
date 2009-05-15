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
#include "FractalSurface.h"
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
#include "scenegraph/SplineLighting.h"
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

    initShaders();
	
	initSplines();
    initGeometry();
	initLights();
    initCameras();
    
	
	// Trigger timer event every 5ms.
	timerId = startTimer(5);
}

/**
* Creates all the splines we need for our scene, including the track spline
* and the cross sectional splines.
**/
void RenderWidget0::initSplines() {
    // This is the track that we use for our geometry and for our camera
    // to move around
    // An (almost) C1 continue curve.  See the picture in Data:Screenshots/Final curve.png
    Vector3 track0(145, 187, 0);
    Vector3 track1(197, 433, 0);
    Vector3 track2(408, 395, 0);
    Vector3 track3(545, 284, 0);
    Vector3 track4(629, 214, 0);
    Vector3 track5(622, 183, -100);
    Vector3 track6(599, 139, -200);
    Vector3 track7(568, 65, -300);
    Vector3 track8(342, 10, -350);
    Vector3 track9(362, 193, -375);
    Vector3 track10(385, 439, -400);
    Vector3 track11(520, 477, -400);
    Vector3 track12(624, 363, -350);
    Vector3 track13(751, 241, -300);
    Vector3 track14(732, 14, -300);
    Vector3 track15(582, 20, -200);
    // Need to put some intermediate points so
    // we do not travel so absurdly fast
    Vector3 track16(463.25, 21, -175);
    Vector3 track17(344.5, 22, -150);
    Vector3 track18(225.75, 23, -125);

    Vector3 track19(107, 24, -100);
    Vector3 track20(123, 63, 0);
    Vector3 track21(145, 187, 0);

    Vector3 trackArray[] = {
        track0, track1, track2, track3, track4, track5, track6, track7, 
        track8, track9, track10, track11, track12, track13, track14, 
        track15, track16, track17, track18, track19, track20, track21
    };
    
    track = new BezierCurve(trackArray, sizeof(trackArray) / sizeof(Vector3));


}



void RenderWidget0::initMaterials()
{}


void RenderWidget0::initCameras() {
	// this creates the still camera that ignores the scenegraph, even though it is part of it
	// Note: given the size of the new track, the still camera will probably be inside the track
	// and therefore won't see much
	
    Vector3 cameraCenter = Vector3(0,0,10);
	Vector3 lookAtPoint = Vector3(0,0,-1);
	Vector3 upVector = Vector3(0,1,0);

//	camera = sceneManager->createCamera();
	
	// TODO: the camera stuff will only work if setFrustum is called first!!
	
	Camera* camera = new Camera();
	camera->setFrustum(0.5, 600, 1, BasicMath::radians(90));
	camera->changeSettings(cameraCenter, lookAtPoint, upVector);
	
	
    stillCamera = new CameraNode(camera);
	stillCamera->makeUnaffected(); // now the still camera will not be affected by the scenegraph
    
    sceneManager->getRoot()->addChild(stillCamera);
	
	
    assert(minecart != NULL);
	
	// first we have to create the camera and cameraNode:
	Camera* moveCamera = new Camera();
	movingCamera = new CameraNode(moveCamera);
	// now we modify the camera so that it sits where we want it to relative to the minecart
	movingCamera->updateProjection(Vector3(0,0,0),Vector3(0,0,-1),Vector3(0,1,0));
	// then we add it as a child of the minecart so that it follows the cart around
	minecart->addChild(movingCamera);
	
	// last thing to do is make sure that when the scene starts, we are using the moving camera, not the still one
	movingCamera->use();
	stillCamera->disable();
    
    
}

void RenderWidget0::initShaders()
{
    // this shader supports two spot lights
    twoSpotTexture = new Shader("src/Shaders/finalSpotLights.vert", "src/Shaders/finalSpotLights.frag");
	// this shader should support 8 lights - 2 spot lights and 6 point lights
	// It slows down rendering and was never fully used because we never got the torch lights working
    //twoSpotTexture = new Shader("src/Shaders/finalLight.vert", "src/Shaders/finalLight.frag");
    
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
    
    sceneManager->getRoot()->addChild(blueLight);

	// This implemets the torch lights; however, we were not able to get them working as point lights
	// Thus, they are just pretty shapes.
	sceneManager->getRoot()->addChild(new SplineLighting(track, SplineLighting::W, 3.5, 10000, 100, twoSpotTexture));
	sceneManager->getRoot()->addChild(new SplineLighting(track, SplineLighting::W, -3.5, 10000, 100, twoSpotTexture));

    // now we have to set up the lighting....
    // Create a white light
    Light * white = sceneManager->createLight();
	white->setType(Light::SPOT);
    white->setSpotDirection(Vector3(0,0,-1));
    white->setDiffuseColor(Vector3(1,1,1));
    white->setAmbientColor(Vector3(.2,.2,.2));
    white->setSpecularColor(Vector3(1,1,1));
	white->setSpotCutoff(90.0);
	white->setSpotExponent(1.0);
	white->setPosition(Vector3(0,0,0));
    
    whiteLight = new LightNode(white);
	whiteLight->setPosition(Vector3(0,0,0));
	minecart->addChild(whiteLight);
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
    
	Vector3 tangent = referenceFrames[(segment-1) % numSegments].getV();
	assert(tangent.isUnitVector());
	Vector3 normal = referenceFrames[(segment-1) % numSegments].getW();
		
	minecart->setTransformation(Matrix4::translate(loc.getX(), loc.getY(), loc.getZ()));
    
	// we now add code to make the camera follow the mine cart
	// We do not need to worry about changing the center of projection because in the scene graph, the camera is a child
	// of the minecart so that center should follow the minecart.
	// We do have to change the lookAtPoint and the lookUpVector and for that we need the referenceFrames for the track
    Vector3 newCenter(0,0,0);
	Vector3 newLookAt = newCenter + tangent;
	Vector3 newLookUp = normal;
	
	// now we update the camera
	movingCamera->updateProjection(newCenter, newLookAt, newLookUp);
	whiteLight->setSpotDirection(-tangent);
	whiteLight->setPosition(newCenter);
		    
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


void RenderWidget0::keyPressEvent ( QKeyEvent * k )
{
    static const int LARGE_CAMERA_MOVEMENT = 10;
	static const int SMALL_CAMERA_MOVEMENT = 1;
	static int movement = LARGE_CAMERA_MOVEMENT;
	
    TransformGroup * root = sceneManager->getRoot();
         
	switch ( k->key() )  {
			// reload
		case Qt::Key_R:                               
			stillCamera->resetTransformation();
			break;
		// move still camera forward
		case Qt::Key_Up: // Qt::Key_W
			stillCamera->translateCamera(0,0,-movement);
			break;
		// Move still camera backwards
		case Qt::Key_Down: //Qt::Key_S:
			stillCamera->translateCamera(0,0,movement);
			break;
		// Move still camera left
		case Qt::Key_Left: //Key_A:
			stillCamera->translateCamera(-movement,0,0);
			break;
		// Move still camera right
		case Qt::Key_Right: //D:
			stillCamera->translateCamera(movement,0,0);
			break;
		// Move still camera up
		case Qt::Key_Q:
			stillCamera->translateCamera(0,movement,0);
			break;
		// Move still camera down
		case Qt::Key_Z:
			stillCamera->translateCamera(0,-movement,0);
			break;
		// change the movement from small to large or vice versa
		case Qt::Key_A:
			movement = (movement == LARGE_CAMERA_MOVEMENT)? SMALL_CAMERA_MOVEMENT : LARGE_CAMERA_MOVEMENT;
			break;
			
    
		// Toggle object level culling
		case Qt::Key_C:
			toggleCulling();
			break;
    

		
		// rotate the moving camera to the left
		case Qt::Key_J:
			movingCamera->setRotation(movingCamera->getRotation() + BasicMath::radians(10));
			break;
		// rotate the moving camera to the right
		case Qt::Key_L:
			movingCamera->setRotation(movingCamera->getRotation() - BasicMath::radians(10));
			break;
		// put moving camera back at initial direction
		case Qt::Key_I:
			movingCamera->setRotation(0);
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



/**
* Creates all of the geometry in the scene, using helper methods in 
* GeometryFactory
**/
void RenderWidget0::initGeometry() 
{
    // The minecart (which actually isn't rendered) is what moves around the
    // scene.  Our camera follows it
	minecart = new TransformGroup();

    static const int NUM_SEGMENTS_TO_SAMPLE_ALONG_CURVE = 500;
    
    Square tunnelCrossSection;
    // Make it bigger and again, lying in xz plane
    tunnelCrossSection.setTransformation(Matrix4::scale(4,4,4) * Matrix4::rotateX(BasicMath::radians(90)));

    // Create the textures
    
    Material * extrudedShapeMaterial = new Material(Brass);
    //http://ryane.com/wp-content/uploads/2007/04/rock_02.jpg
    QImage *rockImg = new QImage("images/rock_02.jpg", "jpg");
    Texture *rockTexture = new Texture(rockImg);
    
    extrudedShapeMaterial->setTexture(rockTexture);
    extrudedShapeMaterial->setShader(twoSpotTexture);

    Material *trackMaterial = new Material();
    // http://www.stock-textures.com/images/wallpapers/44719319/Metal/metal-scratch.jpg
    QImage *metalImg = new QImage("images/metal-scratch.jpg", "jpg");
    Texture *metalTexture = new Texture(rockImg);
    trackMaterial->setTexture(metalTexture);
    trackMaterial->setShader(twoSpotTexture);
    
    // Specify how fine the geometry mesh for the tunner will be; a higher
    // value is finer. Probably 20 - 100.
    static const int TUNNEL_SURFACE_INTERVALS = 32;
    
    // Fractal terrain roughness paramater for the tunnel. Probably 0.4 - 1.0.
    static const float TUNNEL_ROUGHNESS = 0.5;
    
    // Scale paramater determining how far the pertubations jut in and out of
    // the surface. 1.0 is baseline.
    static const float TUNNEL_HEIGHT_SCALE = 1.5;
    
    // Implementation detail to ensure fully overlapping walls - see below.
    static const float TUNNEL_PANEL_STRETCH = 0.1;
    
    static const bool USE_FRACTAL_TERRAIN = true;
    static const int NUM_SIDES_OF_TUNNEL = 4;
    
    // This is a heavy duty method.  It calculates all the geometry created by
    // extruding the shape spline along the path spline.
    std::vector<GeometryFactory::Face> faces = 
        GeometryFactory::createLoftFaces(tunnelCrossSection, 
                                        *track, 
                                        NUM_SIDES_OF_TUNNEL, 
                                        NUM_SEGMENTS_TO_SAMPLE_ALONG_CURVE);
    
    if (USE_FRACTAL_TERRAIN) {
        // The createLoft method returns a sequence of Face objects that each
        // describe a panel in the tunnel. For each such face we will take the four
        // corners and pass them to the fracal surface patch creation method.
        for (std::vector<GeometryFactory::Face>::iterator i = faces.begin(); i != faces.end(); i++) {
          GeometryFactory::Face face = *i;
          Vector3 ll = face.lowerLeft.position;
          Vector3 ul = face.upperLeft.position;
          Vector3 ur = face.upperRight.position;
          Vector3 lr = face.lowerRight.position;
          // We stretch the surfaces a little so that adjacent walls are fully
          // joined at the crease that goes in the same direction as the track.
          // Without such a stretch, gaps would tend to form because of the
          // unevenness introduced by the fractal terrain pertubations.
          Object* shaftWall = 
            FractalSurface::buildSurfaceAmong(
              ll+ (ll - lr)*TUNNEL_PANEL_STRETCH,
              ul+ (ul - ur)*TUNNEL_PANEL_STRETCH,
              ur+ (lr - ll)*TUNNEL_PANEL_STRETCH,
              lr+ (ur - ul)*TUNNEL_PANEL_STRETCH,
              TUNNEL_SURFACE_INTERVALS,
              TUNNEL_ROUGHNESS,
              TUNNEL_HEIGHT_SCALE);
          shaftWall->setMaterial(extrudedShapeMaterial);
          sceneManager->getRoot()->addChild(new Shape3D(shaftWall));
        }     
    }
    // Create the plain faces without any fractal geometry
    else {
        Object * tunnelObject = GeometryFactory::createObjectFromFaces(faces, true, false, true);
        sceneManager->getRoot()->addChild(new Shape3D(tunnelObject));
    }
        
    static const int AMOUNT_TO_MOVE_TRACK_DOWN = 3;
    
    Circle trackCrossSection;
    // Cross sections for oru lofts have to lie in xz plane.
    trackCrossSection.setTransformation(Matrix4::scale(.5,.5,.5) * Matrix4::rotateX(BasicMath::radians(90)));

    track->setTransformation(Matrix4::translate(0,AMOUNT_TO_MOVE_TRACK_DOWN,0) * track->getTransformation());

    Object * trackLoft = GeometryFactory::createLoft(trackCrossSection, *track, 10, NUM_SEGMENTS_TO_SAMPLE_ALONG_CURVE);
    
    // Move the track back to the center of the tunnel
    track->setTransformation(Matrix4::translate(0,-AMOUNT_TO_MOVE_TRACK_DOWN,0) * track->getTransformation());
    
    // Make the track be scratchy metal
    trackLoft->setMaterial(trackMaterial);
    
    sceneManager->getRoot()->addChild(new Shape3D(trackLoft));
    sceneManager->getRoot()->addChild(minecart);
    
    
}


