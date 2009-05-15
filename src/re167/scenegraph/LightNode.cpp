#include "LightNode.h"

using namespace RE167;

// This is a no-op method, since the only reason a lightnode is in the
// graph is to be positioned relative to other objects.  The SceneManager
// needs to traverse the graph, find the LightNodes, and then 
// activate those lights.
void LightNode::draw(const Matrix4 &t, RenderContext * context, Camera * camera, bool cull)
{}

void LightNode::setSpotDirection(const Vector3& s) {
	spotDirection = s.normalize();
	updateLight();
}

void LightNode::setPosition(const Vector3& p) {
	position = p;
	updateLight();
}

void LightNode::setDirection(const Vector3& d) {
	direction = d;
	updateLight();
}

void LightNode::setTransformation(const Matrix4& t) {
	transform = t;
//	spotDirection = Vector3(transform*Vector4(spotDirection));
//	position = Vector3(transform*Vector4(position));
//	direction = Vector3(transform*Vector4(direction));
	updateLight();
	
}

void LightNode::updateLight() {
	light->setSpotDirection(Vector3(transform*Vector4::homogeneousVector(spotDirection)));
//	light->setSpotDirection(spotDirection);
	light->setPosition(Vector3(transform*Vector4(position)));
	light->setDirection(Vector3(transform*Vector4::homogeneousVector(direction)));
}