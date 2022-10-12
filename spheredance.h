#ifndef SPHEREDANCE_H_INCLUDED
#define SPHEREDANCE_H_INCLUDED


#include "glew.h"
#include <GLFW/glfw3.h>

#include "repere.h"
#include "Variables.h"
#include "fonctions.h"
#include "FrameBuffers.h"
#include "Trajectoire.h"

#include "math.h"

#define SD_NUM_OBJETS		16
#define SD_NUM_LUM		1
#define SD_ANNEAU_NUM_SPHERES 14

class SphereDance
{
    public:
        //Paramètres
        static int groupe_courant;
        static int32_t numReflec;
        static float vitesse_rotation_spheres;
        //Observateur
        static float zMin;
        static float zMax;
        static Repere obs;
        static float obsX;
        static float obsY;
        static float obsZ;

        static float obsR;
        static float obsT;
        static float obsL;

        static float aX;
        static float aY;
        static float aZ;

        static float aR;
        static float aT;
        static float aL;

        //Ciel:
        static float C_ciel[TAILLE_CIEL];
        //Lumière ambiante:
        static float L_amb;
        static float C_amb[3];
        static float distance_brouillard;

        //Lumière ponctuelle:
        static float L1[TAILLE_LUM];
        //Sol:
        static float Sol[TAILLE_OBJ];
        //Sphères:
        static float S1[TAILLE_OBJ];
        static float S2[TAILLE_OBJ];
        static float S3[TAILLE_OBJ];
        static float S4[TAILLE_OBJ];
        static float S5[TAILLE_OBJ];
        static float S6[TAILLE_OBJ];
        static float S7[TAILLE_OBJ];
        static float S8[TAILLE_OBJ];
        static float S9[TAILLE_OBJ];
        static float S10[TAILLE_OBJ];
        static float S11[TAILLE_OBJ];
        static float S12[TAILLE_OBJ];
        static float S13[TAILLE_OBJ];
        static float S14[TAILLE_OBJ];
        static float S15[TAILLE_OBJ];
        //static float S16[TAILLE_OBJ];

        static float* anneau_spheres[SD_ANNEAU_NUM_SPHERES];
        static float positions_spheres[SD_ANNEAU_NUM_SPHERES*3];
        static float positions_spheres_rot[SD_ANNEAU_NUM_SPHERES*3];

        static float objets[TAILLE_OBJ*SD_NUM_OBJETS];	//Le tableau contenant les objets de la scène
        static float alpha_ay;

        //Texture de transition avec la scène Mandelbrot:
        static float fracTexDistMin;
        static float fracTexDistMax;
        static bool drapeau_affiche_fracTex;

        //Commandes clavier:
        static bool dep_droite;
        static bool dep_gauche;
        static bool dep_haut;
        static bool dep_bas;
        static bool dep_avant;
        static bool dep_arriere;

        static bool rot_droite;
        static bool rot_gauche;
        static bool rot_haut;
        static bool rot_bas;
        static bool rot_r_gauche;
        static bool rot_r_droite;

        static bool numReflec_0;
        static bool numReflec_1;
        static bool numReflec_2;
        static bool numReflec_3;


        static bool numRefrac_0;
        static bool numRefrac_1;
        static bool numRefrac_2;
        static bool numRefrac_3;

        static int depId;

        static bool inc_FracTexMax;
        static bool inc_FracTexMin;
        static bool dec_FracTexMax;
        static bool dec_FracTexMin;

        //Edora:
        static vec3f position_edora;
        static float alpha_Edora;
        static float angle_Edora;



    public:
        static bool init_scene_spheredance();
        static void clavier(int key,bool etat);
        static void maj_deplacement();
        static int stock_objet(int j,float obj[TAILLE_OBJ]);
        static void init_tableau_objets();
        static void init_affiche_spheredance();
        static void eclairages();
        static void maj_positions_eclairages();
        static void affiche_fractaleShot();
        static void affiche_spheredance();
        static void affiche_Edora();
        static void render_spheredance();

};

#endif // SPHEREDANCE_H_INCLUDED
