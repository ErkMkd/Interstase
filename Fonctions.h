#ifndef FONCTIONS_H_INCLUDED
#define FONCTIONS_H_INCLUDED

#include "glew.h"
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <iostream>
#include "math.h"

#include "Musique.h"
#include "repere.h"
#include "FrameBuffers.h"
#include "ShadersDemo.h"
#include "Intro.h"
#include "HypnoStructure.h"
#include "Final.h"
#include "WaterWorld.h"
#include "SphereDance.h"
#include "Mandelbrot.h"
#include "Ville/Ville.h"
#include "editeur_trajectoire.h"

#define DEG_RAD M_PI/180.
#define RAD_DEG 180./M_PI

using namespace std;



class GFunc
{
    public:
    static GLboolean eclairages_mem;
    static GLboolean blend_mem;
    static GLboolean mask_mem;
    static GLboolean color_material_mem;
    static GLint blendSrc;
    static GLint blendDst;
    static GLboolean depthTest_mem;
    static GLboolean culling_mem;
    static GLint frontFace_mem;

    #define EDIT_RIEN 0
    #define EDIT_FILTRE_HSV 1
    #define EDIT_GLOW 2
    #define EDIT_HIDE 3
    #define NUM_EDIT_SETS 3

    static int edit_keys;

    static void glfw_error_callback(int error,const char* description);
    static void dimensionne_ecran();
    static void redimensionne_ecran(GLFWwindow* window, int width, int height);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void setup_opengl();
    static void sauve_OpenGl();
    static void restitue_OpenGl();
    static void desactive_textures();
    static void init_matrice_observateur_raytrace(float x,float y, float z, float r, float t, float l);
    static void init_matrice_observateur_raytrace(Repere* rep);
    static void init_matrice_observateur(float x,float y, float z, float r, float t, float l,bool translation=true);
    static void init_matrice_observateur(Repere* rep,bool translation=true);
    static void init_matrice_observateur_reflet_eau(Repere* rep,float altitude_plan,bool translation=true);
    static void calcul_coordonnees_ecran(float* point,float* pointEcran);
    static void rotation_plan(float angle,float sX, float sY, float& dX, float& dY);
    static void maj_distanceFocale();
    static void vue_ortho();
    static void vue_perspective(float zMin=1.,float zMax=10000.);
    static void RGBi_RGBf(uint32_t RGBi,float RGBf[3]);
    static void BGRi_RGBf(uint32_t BGRi,float RGBf[3]);
    static void genere_ombre_sol_listes();
    static void affiche_texture(float alpha=1.,float r=1., float v=1., float b=1.);
    static void affiche_ombre_sol(float x,float y, float z, float sx,float sy,float sz,float alpha,GLuint texture_id=0);
    static void affiche_ombre_sol_flat(float x,float y, float z, float sx,float sy,float sz,float alpha);
    static void rotation_points(float cx,float cy,float cz,float ax, float ay, float az, float angle, float* points, float* points_resultat, uint32_t numPoints);
    static void rotation_RTL(float cx, float cy, float cz,float roulis,float tangage, float lacet, float* points, float* resultat,uint32_t numPoints);
    static void affiche_reflet_eau(parametres_reflet_eau& params,float altitude_plan=0.,float taille_plan=1000.); //Position de la lumière pour l'effet spéculaire
    static void animation_cosmo_decollage(float altitude_sol=0.);
    static void affiche_cosmonaute();
    static void affiche_ombre_cosmo_sol();
    static void rendu_reflet_cosmonaute(float altitude_plan=0.);
};
#endif // FONCTIONS_H_INCLUDED
