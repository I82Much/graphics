#include "LightNode.h"

using namespace RE167;

// This is a no-op method, since the only reason a lightnode is in the
// graph is to be positioned relative to other objects.  The SceneManager
// needs to traverse the graph, find the LightNodes, and then 
// activate those lights.
void LightNode::draw(const Matrix4 &t, RenderContext * context, Camera * camera, bool cull)
{}


