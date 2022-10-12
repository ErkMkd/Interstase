#ifndef LUMIERES_H_INCLUDED
#define LUMIERES_H_INCLUDED

#include "glew.h"
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <iostream>
#include "math.h"

class Lumieres
{
    public:
        static uint32_t drapeaux_sources;  //Drapeaux d'activation des sources lumineuses. Sert aux shaders des objets 3d.
        static GLfloat couleur_brouillard[4];
        static GLfloat lumiere_ambiante[4];
        static void lumiere_couleur(GLuint id_openGl,float amb[3],float diff[3],float spec[3]);
        static void lumiere_couleur(GLuint id_openGl,float amb_r,float amb_v,float amb_b,
                                                float diff_r,float diff_v,float diff_b,
                                                float spec_r,float spec_v,float spec_b);
        static void lumiere_attenuation(GLuint id_openGl, float ac=1., float al=0., float as=0.);
        static void lumiere_position(GLuint id_openGl,float pos[3]);
        static void lumiere_position(GLuint id_openGl,float x,float y,float z);
        static void lumiere_directionnelle_orientation(GLuint id_openGl,float dx,float dy,float dz);
        static void lumiere_infinie_orientation(GLuint id_openGl,float dx,float dy,float dz);
        static void lumiere_infinie_orientation(GLuint id_openGl,float dir[3]);
        static void lumiere_ouverture(GLuint id_openGl,float ouverture);	//180 indique que la lumière est omnidirectionnelle
};

#endif // LUMIERES_H_INCLUDED
