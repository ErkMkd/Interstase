#ifndef FINAL_H_INCLUDED
#define FINAL_H_INCLUDED

#include "glew.h"
#include <GLFW/glfw3.h>

#include "Variables.h"
#include "fonctions.h"
#include "Champ_Etoiles.h"
#include "Sprites_2d.h"
#include "chargeurs_images/Image.h"

class Final
{
    public:
        static TexteMap* texteFin;

        static Repere obs;
        static Champ_Etoiles* starfield;
        static Champ_Etoiles* starfield2;
        static Champ_Etoiles* starfield3;
        static Champ_Etoiles* starfield4;
        static Champ_Etoiles* starfield5;
        static Champ_Etoiles* starfield6;
        static Champ_Etoiles* starfield7;
        static Image* etoile;
        static Image* etoile2;
        static Image* etoile3;
        static Image* etoile4;
        static Image* etoile5;
        static Image* etoile6;
        static Image* etoile7;
        #define FINAL_SPR_LOGOMKD 0
        #define FINAL_SPR_DESIGN 1
        #define FINAL_SPR_ASTRONAUTE 2
        #define FINAL_SPR_NUM 3

        static Sprites_2d* sprites;
        static const char f1[];
        static const char f2[];
        static const char f3[];
        static const char* fichiersSprites[];
        static float cosmoX;
        static float cosmoY;

        static float couleur_ciel[4];
        static float distance_brouillard;

        static bool dep_droite;
        static bool dep_gauche;
        static bool dep_haut;
        static bool dep_bas;
        static bool zoom_in;
        static bool zoom_out;
        static bool dep_arriere;
        static int depId;

        static bool dep_avant;
        static bool rot_L_gauche;
        static bool rot_L_droite;
        static bool rot_T_haut;
        static bool rot_T_bas;
        static bool rot_R_gauche;
        static bool rot_R_droite;


        static float aX;
        static float aY;
        static float aZ;
        static float aR;
        static float aT;
        static float aL;

        static bool init_scene();
        static void detruit_scene();
        static void clavier(int key,bool etat);
        static void maj_deplacement();
        static void init_affiche();
        static void affiche();
        static void render_scene();
};
#endif // FINAL_H_INCLUDED
