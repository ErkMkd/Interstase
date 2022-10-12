#ifndef VEC3F_H_INCLUDED
#define VEC3F_H_INCLUDED

#include "math.h"

class vec3f
{
    public:
        float x;
        float y;
        float z;

        vec3f(float px=0., float py=0.,float pz=0.);
        void init(float px,float py,float pz);
        void normalise();
        void produit_vectoriel(vec3f v1,vec3f v2);
        float renvoie_norme();
};

#endif // VEC3F_H_INCLUDED
