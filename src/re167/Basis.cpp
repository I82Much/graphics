#include "Basis.h"


using namespace RE167;



Basis::Basis(const Vector3 &u_, 
    const Vector3 &v_,
    const Vector3 &w_, 
    const Vector3 &o_) :
    u(u_), v(v_), w(w_), origin(o_), 
    // This matrix transforms from world coordinates into this local coordinate
    // system
    transformation( u.getX(), v.getX(), w.getX(), origin.getX(),
                    u.getY(), v.getY(), w.getY(), origin.getY(),
                    u.getZ(), v.getZ(), w.getZ(), origin.getZ(),
                    0,          0,      0,          1) {}

// No memory allocated, nothing to delete
Basis::~Basis() 
{
    
}

using namespace RE167;