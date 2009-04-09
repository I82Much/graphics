#include "Shape3D.h"

using namespace RE167;


void Shape3D::draw(const Matrix4 &t, RE167::RenderContext *context) { 
    Matrix4 t_new = t * shape->getTransformation();
            
        
    context->setModelViewMatrix(t_new);
    context->setMaterial(shape->getMaterial());
    context->render(shape);
}
