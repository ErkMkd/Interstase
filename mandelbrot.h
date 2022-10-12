#ifndef MANDELBROT_H_INCLUDED
#define MANDELBROT_H_INCLUDED

#include "glew.h"
#include <GLFW/glfw3.h>

#include "Variables.h"
#include "fonctions.h"
#include "couleurs.h"
#include "Bruit.h"
#include "Champ_Etoiles.h"

#include "math.h"

#define MB_NUM_COULEURS 512
#define MB_NUM_ETAPES 10

class Mandelbrot
{
    public:
        static Repere obs;
        static float perturbationObs;
        static Bruit pertrube_obsX;
        static Bruit pertrube_obsY;
        static Bruit pertrube_obsZ;
        static float zMin;
        static float zMax;
        static GLdouble xpos;
        static GLdouble ypos;
        static GLdouble zoom;
        static GLfloat zoom_ar;
        static GLfloat cxTex;
        static GLfloat cyTex;
        static GLdouble facteur_zoom;
        static GLdouble azoom;
        static GLdouble axpos;
        static GLdouble aypos;
        static GLdouble azpos;
        static GLdouble deceleration;
        static GLdouble acceleration;
        static GLint iter;
        static GLdouble t;
        static GLfloat alpha;
        static GLfloat perturbation_alpha;
        static GLuint palette_tex_id;
        static float decallage_palette;
        static uint32_t palette[MB_NUM_COULEURS+1]; //+1 pour les débordements
        static Bruit rotation_Perlin;
        static float facteur_perturbation_rotation;
        //Lumieres:
        static float L0_attenuation;
        static float distance_brouillard;
        static float couleur_amb[4];
        static float couleur_fog[4];
        static float L0_position[3];
        static float L0_amb[3];
        static float L0_diff[3];
        static float L0_spec[3];

        static float L1_orientation[3];
        static float L1_amb[3];
        static float L1_diff[3];
        static float L1_spec[3];

        //Sprites:
        static bool drapeau_affiche_screenShot;
        static bool drapeau_affiche_nuages;
        static bool drapeau_affiche_vaisseau;
        static Champ_Etoiles* nuages;
        static Image* nuage_1;

        //Commandes clavier:
        static bool dep_droite;
        static bool dep_gauche;
        static bool dep_avant;
        static bool dep_arriere;
        static bool dep_haut;
        static bool dep_bas;
        static bool zoom_in;
        static bool zoom_out;
        static bool rot_CCW;
        static bool rot_CW;
        static bool t_plus;
        static bool t_moins;
        static bool iter_plus;
        static bool iter_moins;
        static int depId;

        static bool rot_L_gauche;
        static bool rot_L_droite;
        static bool rot_T_haut;
        static bool rot_T_bas;
        static bool rot_R_gauche;
        static bool rot_R_droite;

        //------- Contrôles de déplacement:
        static float aX;
        static float aY;
        static float aZ;
        static float aR;
        static float aT;
        static float aL;

    public:
        static bool init_scene_mandelbrot();
        static bool init_sprites();
        static void detruit_scene_mandelbrot();
        static void clavier(int key,bool etat);
        static void maj_deplacement();
        static void init_affiche_mandelbrot();
        static void eclairages();
        static void affiche_mandelbrot();
        static void affiche_nuages();
        static void affiche_vaisseau();
        static void render_scene_mandelbrot();
};

#endif // MANDELBROT_H_INCLUDED
