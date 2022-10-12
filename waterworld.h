#ifndef WATERWORLD_H_INCLUDED
#define WATERWORLD_H_INCLUDED

#include "glew.h"
#include <GLFW/glfw3.h>
#include "math.h"
#include "Variables.h"
#include "Interpolations.h"
#include "repere.h"
#include "Fonctions.h"
#include "Objets3d/Volume.h"
#include "Agnomes.h"
#include "time.h"

#define WW_NUM_OBJETS	13
#define WW_NUM_LUM		1



class WaterWorld
{
    public:
        static Repere obs;
        static bool drapeau_affiche_ville;
        static bool drapeau_affiche_Interstase;
        static bool drapeau_affiche_nuees;
        //Paramètres
        static int32_t numReflec;
        static int32_t numRefrac;

        static float vitesse_ondulations;

        //Observateur

        static float aX;
        static float aY;
        static float aZ;

        static float aR;
        static float aT;
        static float aL;

        //Ciel:
        static float distance_brouillard;
        static float C_ciel[TAILLE_CIEL];
        //Lumière ambiante:
        static float L_amb;
        static float C_amb[3]; //4c80ff

        //Lumière ponctuelle:
        static float L1[TAILLE_LUM];


        //Sol:
        static float amplitude_vagues;
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

        //Mouvements:
        static float obsX;
        static float obsY;
        static float obsZ;

        static float obsR0;
        static float obsT0;
        static float obsL0;

        static float xS1;
        static float yS1;
        static float zS1;
        static float rS1;

        static float xS2;
        static float yS2;
        static float zS2;
        static float rS2;
        static float orbS2;
        static float vOrbS2;

        static float xS3;
        static float yS3;
        static float zS3;
        static float rS3;
        static float orbS3;
        static float vOrbS3;

        static float xS4;
        static float yS4;
        static float zS4;
        static float rS4;
        static float orbS4;
        static float vOrbS4;

        static float xS5;
        static float yS5;
        static float zS5;
        static float rS5;
        static float orbS5;
        static float vOrbS5;

        static float xS6;
        static float yS6;
        static float zS6;
        static float rS6;
        static float orbS6;
        static float vOrbS6;

        static float S2S6_angle_t0;

        static float xS7;
        static float yS7;
        static float zS7;
        static float rS7;
        static float orbS7;
        static float S7_angle_t0;
        static float S7_epsilon_Vangle;

        static float xS8;//33.;
        static float yS8;//38.;
        static float zS8;//25.;
        static float rS8;//25.;

        static float xS9;
        static float yS9;
        static float zS9;
        static float rS9;

        static float objets[TAILLE_OBJ*WW_NUM_OBJETS];	//Le tableau contenant les objets de la scène

        //Données d'animations:

        static float rebondS1;
        static float rotxS1;
        static float rotzS1;

        static float angleS2;
        static float angleS3;
        static float angleS4;
        static float angleS5;
        static float angleS6;
        static float angleS7;

        //Reflet cosmonaute:
        static parametres_reflet_eau params_reflet_cosmo;
        static float altitude_eau_cosmo;

        //Sprite Edora:
        static bool drapeau_affiche_Edora;
        static float xEdora;
        static float yEdora;
        static float eEdora;
        static float alphaEdora;

        //Sprites des Agnomes Polysphériques:
        static Agnomes* agnomes;
        static bool drapeau_affiche_agnomes;


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

        static bool inc_alt_reflet;
        static bool dec_alt_reflet;

        static bool numReflec_0;
        static bool numReflec_1;
        static bool numReflec_2;
        static bool numReflec_3;

        static bool numRefrac_0;
        static bool numRefrac_1;
        static bool numRefrac_2;
        static bool numRefrac_3;

        static int depId;

    public:
        static bool init_scene_waterworld();
        static void detruit_scene_waterworld();
        static void clavier(int key,bool etat);
        static void maj_deplacement();
        static int stock_objet(int j,float obj[TAILLE_OBJ]);
        static void init_tableau_objets();
        static void init_affiche_waterworld();
        static void eclairages();
        static void maj_positions_lumieres();
        static void affiche_waterworld();
        static void render_waterworld();
        static void affiche_Edora();
};

#endif // WATERWORLD_H_INCLUDED
