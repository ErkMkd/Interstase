
#include "vec3f.h"

vec3f::vec3f(float px, float py,float pz)
{
    x=px;y=py;z=pz;
}

void vec3f::init(float px,float py,float pz)
{
    x=px;y=py;z=pz;
}

void vec3f::normalise()
{
    float n=sqrtf(x*x+y*y+z*z);
    if(n!=0.)
    {
        x/=n;
        y/=n;
        z/=n;
    }
}

void vec3f::produit_vectoriel(vec3f v1,vec3f v2)
{
    x=v1.y*v2.z-v1.z*v2.y;
    y=v1.z*v2.x-v1.x*v2.z;
    z=v1.x*v2.y-v1.y*v2.x;
}

float vec3f::renvoie_norme()
{
    return sqrtf(x*x+y*y+z*z);
}

