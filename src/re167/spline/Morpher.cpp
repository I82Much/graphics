#include "Morpher.h"
#include "BasicMath.h"

using namespace RE167;

Morpher::Morpher(Spline * first_, Spline * second_) : first(first_), second(second_) {}
Morpher::~Morpher() {}


Vector3 Morpher::position(float t) const
{
    assert(0 <= t && t <= 1.0);
    return BasicMath::lerp(first->position(t), second->position(t), t);
}
Vector3 Morpher::tangent(float t) const
{
    assert(0 <= t && t <= 1.0);
    return BasicMath::lerp(first->tangent(t), second->tangent(t), t);
}
Vector3 Morpher::acceleration(float t) const
{
    assert(0 <= t && t <= 1.0);
    return BasicMath::lerp(first->acceleration(t), second->acceleration(t), t);
}
