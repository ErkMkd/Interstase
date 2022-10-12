#ifndef HYPNOSTRUCTURE_H_INCLUDED
#define HYPNOSTRUCTURE_H_INCLUDED

#include "glew.h"
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <iostream>
#include "math.h"
#include "Variables.h"
#include "fonctions.h"
#include "Lumieres.h"
#include "repere.h"
#include "vec3f.h"
#include "Primitives.h"

#define NUM_COTES_DISQUE 32

#define NUM_SECTIONS_CYL 2
#define NUM_COTES_CYL 8
#define NUM_POINTS_CYL NUM_SECTIONS_CYL*NUM_COTES_CYL
#define NUM_FACES_CYL (NUM_SECTIONS_CYL-1)*NUM_COTES_CYL+2

#define NUM_COTES_SPHERE 16
#define NUM_SEGMENTS_SPHERE 16
#define NUM_POINTS_SPHERE NUM_COTES_SPHERE*(NUM_SEGMENTS_SPHERE-1)+2
#define NUM_FACES_SPHERE NUM_SEGMENTS_SPHERE*NUM_COTES_SPHERE

#define NUM_COTES_TORE 12
#define NUM_SECTIONS_TORE 32
#define NUM_POINTS_TORE NUM_COTES_TORE*NUM_SECTIONS_TORE
#define NUM_FACES_TORE					 NUM_POINTS_TORE

//Arrière plan
#define HS_NUM_OBJETS		1
#define HS_NUM_LUM		1


class HypnoStructure : public Primitives
{
    public:
        static Repere obs;
        static vec3f point_vise;
        static Repere structure;

        static float aX;
        static float aY;
        static float aZ;
        static float aR;
        static float aT;
        static float aL;

        static float coordsDisque[3*NUM_COTES_DISQUE];

        static float coordsCylindre[3*NUM_POINTS_CYL];
        static uint16_t  facesCylindre[(NUM_FACES_CYL-2)*4+2*NUM_COTES_CYL];
        static float normalesCylindre[3*NUM_POINTS_CYL];

        static float coordsSphere[3*NUM_POINTS_SPHERE];
        static uint16_t facesSphere[NUM_FACES_SPHERE*4-(2*NUM_COTES_SPHERE)];
        static float normalesSphere[3*NUM_POINTS_SPHERE];

        static float coordsTore[3*NUM_POINTS_TORE];
        static uint16_t facesTore[NUM_FACES_TORE*4];
        static float normalesTore[3*NUM_POINTS_TORE];

        static GLuint disque_liste_id;
        static GLuint sphere_liste_id;
        static GLuint cylindre_liste_id;
        static GLuint tore_liste_id;

        static uint16_t itermax;
        static float angle_rot_roue;
        static float vitesse_rot_roue;
        static float temps_prec;

        //========= Arrière plan (générés en ray tracing)
        //Paramètres
        static int32_t numReflec;
        static int32_t numRefrac;

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
        static float objets[TAILLE_OBJ*HS_NUM_OBJETS];	//Le tableau contenant les objets d'arrière plan de la scène

        //Commandes clavier:
        static bool dep_droite;
        static bool dep_gauche;
        static bool dep_avant;
        static bool dep_arriere;
        static bool dep_haut;
        static bool dep_bas;
        static bool rot_L_gauche;
        static bool rot_L_droite;
        static bool rot_T_haut;
        static bool rot_T_bas;
        static bool rot_R_gauche;
        static bool rot_R_droite;

        static int depId;


    public:
        static bool init_scene_hypnostructure();
        static void detruit_hypnostructure();
        static void affiche_disque();
        static void affiche_cylindre();
        static void affiche_sphere(float luminosite=0.);
        static void affiche_tore();
        static void affiche_elements(float echelle_tore, uint16_t num_roues,float decallage_angle_roue,float angle,uint16_t iter);
        static void affiche_tige(float r_disque,float r_cyl,float h_cyl,float echelle_sphere,uint16_t numSpheresCyl, float repartitionSphereCyl,uint16_t iter);
        static void clavier(int key,bool etat);
        static void maj_deplacement();
        static int stock_objet(int j,float obj[TAILLE_OBJ]);
        static void init_tableau_objets();
        static void init_affiche_hypnostructure_ap();
        static void affiche_hypnostructure_ap();
        static void init_affiche_hypnostructure();
        static void eclairages();
        static void maj_positions_lumieres();
        static void affiche_hypnostructure();
        static void render_scene_hypnostructure();
};

#endif // HYPNOSTRUCTURE_H_INCLUDED
