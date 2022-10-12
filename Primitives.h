#ifndef PRIMITIVES_H_INCLUDED
#define PRIMITIVES_H_INCLUDED

#include "glew.h"
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <iostream>
#include "math.h"

#define DEG_RAD M_PI/180.

class Primitives
{
    public:
        static float triangle_sommets[2*3];
        static float plan_sommets[2*4];
        static float cube_sommets[3*8];
        static uint16_t cube_faces[4*6];
        static float cube_normales[3*6];
        static float tetraedre_sommets[3*4];
        static uint16_t tetraedre_faces[3*4];
        static float tetraedre_normales[3*4];

        static float materiau_amb[4];
        static float materiau_diff[4];
        static float materiau_lum[4];
        static float materiau_spec[4];


    public:

        static void genere_disque(float rayon,uint16_t numCotes,float* coordsTab, float y=0.);
        static void genere_cylindre(float rayon, float hauteur, uint16_t numCotes, uint16_t numSections, float* coordsTab);
        static void genere_faces_cylindre(uint16_t numCotes, uint16_t numSections, uint16_t* facesTab);
        static void genere_normales_cylindre(uint32_t numPoints, float* coordsTab, float* normalesTab);
        static void genere_tore(float rayon1,float rayon2, uint16_t numCotes, uint16_t numSections, float* coordsTab);
        static void genere_faces_tore(uint16_t numCotes, uint16_t numSections, uint16_t* facesTab);
        static void genere_normales_tore(float rayon,uint32_t numPoints,float* coordsTab, float* normalesTab);
        static void genere_sphere(float rayon, uint16_t numCotes, uint16_t numSegments, float* coordsTab);
        static void genere_faces_sphere(uint16_t numCotes, uint16_t numSegments, uint16_t* facesTab);
        static void genere_normales_sphere(uint32_t numPoints, float* coordsTab, float* normalesTab);

        static void init_materiau_simple_face(float Ramb,float Vamb, float Bamb,float Aamb,
                                       float Rdiff,float Vdiff,float Bdiff,float Adiff,
                                       float Rlum,float Vlum,float Blum,float Alum,
                                       float Rspec,float Vspec,float Bspec,float Aspec,
                                       float brill);

        static void init_materiau_simple_face(uint32_t RVBAamb, uint32_t RVBAdiff, uint32_t RVBAlum, uint32_t RVBAspec,float brill);

        static void init_materiau_double_face(float Ramb,float Vamb, float Bamb,float Aamb,
                                       float Rdiff,float Vdiff,float Bdiff,float Adiff,
                                       float Rlum,float Vlum,float Blum,float Alum,
                                       float Rspec,float Vspec,float Bspec,float Aspec,
                                       float brill);

        static void init_materiau_double_face(uint32_t RVBAamb, uint32_t RVBAdiff, uint32_t RVBAlum, uint32_t RVBAspec,float brill);

};

#endif // PRIMITIVES_H_INCLUDED
