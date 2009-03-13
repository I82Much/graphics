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

/**
* This class is an OpenGL window, with support for mouse dragging
* and key presses.
* @modified Nicholas Dunn
* @date   February 21, 2009
*/


RenderWidget0::RenderWidget0()
{
	RenderContext *rs = new GLRenderContext();
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

	// Camera
	camera = sceneManager->createCamera();


	Vector3 cameraCenter = Vector3(0,0,40);
	Vector3 lookAtPoint = Vector3(0,0,0);
	Vector3 upVector = Vector3(0,1,0);

	camera = sceneManager->createCamera();
	camera->changeSettings(cameraCenter, lookAtPoint, upVector);

	Vector3 cameraCenter2 = Vector3(-10, 40, 40);
	Vector3 lookAtPoint2 = Vector3(-5,0,0);
	Vector3 upVector2 = Vector3(0, 1, 0);
	
	//camera->changeSettings(cameraCenter2, lookAtPoint2, upVector2);

	camera->setFrustum(1, 100, 1, BasicMath::radian(60));

	/*
	dragon = sceneManager->createObject();
	GeometryFactory::createObject(dragon, "objects/dragon_smooth.obj");
	

	bunny = sceneManager->createObject();
	GeometryFactory::createObject(bunny, "objects/bunny.obj");
	
	terrain = sceneManager->createObject();
	GeometryFactory::createTerrainFromPGM(terrain, "objects/Heightmap.pgm");
	*/
    /*
    cube = sceneManager->createObject();
    GeometryFactory::createCube(cube);
	*/
	
		
	houses = sceneManager->createObject();
	GeometryFactory::createHouses(houses);



	// Code to draw house provided by Prof. Palacio
	/*
	// A house
	int nVerts = 42;
	float vertices[] = {-4,-4,4, 4,-4,4, 4,4,4, -4,4,4,		 // front face
		                -4,-4,-4, -4,-4,4, -4,4,4, -4,4,-4,  // left face
						4,-4,-4,-4,-4,-4, -4,4,-4, 4,4,-4,   // back face
						4,-4,4, 4,-4,-4, 4,4,-4, 4,4,4,		 // right face
						4,4,4, 4,4,-4, -4,4,-4, -4,4,4,		 // top face
						-4,-4,4, -4,-4,-4, 4,-4,-4, 4,-4,4,  // bottom face

						-20,-4,20, 20,-4,20, 20,-4,-20, -20,-4,-20,  // ground floor
						-4,4,4, 4,4,4, 0,8,4,						 // roof
						4,4,4, 4,4,-4, 0,8,-4, 0,8,4,
						-4,4,4, 0,8,4, 0,8,-4, -4,4,-4,
						4,4,-4, -4,4,-4, 0,8,-4};

	float colors[] = {1,0,0, 1,0,0, 1,0,0, 1,0,0,	// front face
					  0,1,0, 0,1,0, 0,1,0, 0,1,0,	// left face
					  1,0,0, 1,0,0, 1,0,0, 1,0,0,	// back face
					  0,1,0, 0,1,0, 0,1,0, 0,1,0,	// right face
					  0,0,1, 0,0,1, 0,0,1, 0,0,1,	// top face
					  0,0,1, 0,0,1, 0,0,1, 0,0,1,	// bottom face
	
					  0,0.5,0, 0,0.5,0, 0,0.5,0, 0,0.5,0,	// ground floor
					  0,0,1, 0,0,1, 0,0,1,					// roof
					  1,0,0, 1,0,0, 1,0,0, 1,0,0,
					  0,1,0, 0,1,0, 0,1,0, 0,1,0,
					  0,0,1, 0,0,1, 0,0,1,};

	// Add an object to the scene
	object = sceneManager->createObject();

	// Set up the vertex data
	VertexData& vertexData = object->vertexData;

	// Specify the elements of the vertex data:
	// - one element for vertex positions
	vertexData.vertexDeclaration.addElement(0, 0, 3, 3*sizeof(float), RE167::VES_POSITION);
	// - one element for vertex colors
	vertexData.vertexDeclaration.addElement(1, 0, 3, 3*sizeof(int), RE167::VES_DIFFUSE);
	
	// Create the buffers and load the data
	vertexData.createVertexBuffer(0, nVerts*3*sizeof(float), (unsigned char*)vertices);
	vertexData.createVertexBuffer(1, nVerts*3*sizeof(float), (unsigned char*)colors);

	// The index data that stores the connectivity of the triangles
	int indices[] = {0,2,3, 0,1,2,			// front face
		             4,6,7, 4,5,6,			// left face
					 8,10,11, 8,9,10,		// back face
					 12,14,15, 12,13,14,	// right face
					 16,18,19, 16,17,18,	// top face
					 20,22,23, 20,21,22,	// bottom face
	                 
					 24,26,27, 24,25,26,	// ground floor
					 28,29,30,				// roof
					 31,33,34, 31,32,33,
					 35,37,38, 35,36,37,
					 39,40,41};	

	vertexData.createIndexBuffer(60, indices);
	*/
	// Trigger timer event every 5ms.
	timerId = startTimer(5);

	
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
	//bunny->setTransformation(Matrix4::translate(1,0,0) * Matrix4::rotateY(0.05f * counter));
	//dragon->setTransformation(Matrix4::translate(-1,0,0) * Matrix4::rotateY(-0.05f * counter));
	//object->setTransformation(Matrix4::rotateY(-0.05f * counter));
	//camera->setViewMatrix(camera->getViewMatrix() * Matrix4::rotateY(-0.005f));

    //cube->setTransformation(cube->getTransformation() * Matrix4::rotateY(-0.05f) * Matrix4::rotateX(-0.05f));

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

	//object->setTransformation(rotation * object->getTransformation());

	// Apply transformation to the camera
	camera->setViewMatrix(camera->getViewMatrix() * rotation);

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
	//object->resetTransformation();
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

void RenderWidget0::keyPressEvent ( QKeyEvent * k ) 
{
	switch ( k->key() )  {
		case Qt::Key_R:                               // reload
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

	}
}
void RenderWidget0::keyReleaseEvent ( QKeyEvent * e) 
{
	
}



