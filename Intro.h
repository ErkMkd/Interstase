#ifndef INTRO_H_INCLUDED
#define INTRO_H_INCLUDED

#include "glew.h"
#include <GLFW/glfw3.h>

#include "Variables.h"
#include "fonctions.h"
#include "Champ_Etoiles.h"

class Intro
{
    public:
        static Repere obs;
        static float aX;
        static float aY;
        static float aZ;

        static float aR;
        static float aT;
        static float aL;

        //Sprite oeil logo Mankind:
        static float mkdX;
        static float mkdY;
        static float mkdsX;
        static float mkdsY;

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
        static int depId;

        static float nebuleuseH;
        static float nebuleuseR;

        static float C_amb[4];
        static float L1_intensite;
        static float L1_couleur[3];

        static Champ_Etoiles* starfield;

        static bool init_scene();
        static void detruit_scene();
        static void clavier(int key,bool etat);
        static void maj_deplacement();
        static void init_affiche();
        static void eclairages();
        static void affiche();
        static void render_scene();
};

#endif // INTRO_H_INCLUDED
