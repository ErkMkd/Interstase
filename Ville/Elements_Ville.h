#ifndef ELEMENTS_VILLE_H_INCLUDED
#define ELEMENTS_VILLE_H_INCLUDED

#include "../glew.h"
#include <GLFW/glfw3.h>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include "../Primitives.h"
#include "../ShadersObjets3d/ShadersObjets3d.h"
#include "Shaders/ShadersVille.h"
#include "../chargeurs_images/Image.h"
#include "../Texgen.h"
#include "../Bruit.h"
#include "../Variables.h"

#define VILLE_ALTITUDE_ROUTES 5.

#define VILLE_NUM_COTES_DISQUE 64

#define VILLE_NUM_SECTIONS_CYL 2
#define VILLE_NUM_COTES_CYL 12
#define VILLE_NUM_POINTS_CYL VILLE_NUM_SECTIONS_CYL*VILLE_NUM_COTES_CYL
#define VILLE_NUM_FACES_CYL (VILLE_NUM_SECTIONS_CYL-1)*VILLE_NUM_COTES_CYL+2

#define VILLE_NUM_COTES_SPHERE 5
#define VILLE_NUM_SEGMENTS_SPHERE 4
#define VILLE_NUM_POINTS_SPHERE VILLE_NUM_COTES_SPHERE*(VILLE_NUM_SEGMENTS_SPHERE-1)+2
#define VILLE_NUM_FACES_SPHERE VILLE_NUM_SEGMENTS_SPHERE*VILLE_NUM_COTES_SPHERE

#define VILLE_NUM_COTES_TORE 10
#define VILLE_NUM_SECTIONS_TORE 12
#define VILLE_NUM_POINTS_TORE VILLE_NUM_COTES_TORE*VILLE_NUM_SECTIONS_TORE
#define VILLE_NUM_FACES_TORE VILLE_NUM_POINTS_TORE

class Elements_Ville : public Primitives
{
    public:
        static bool drapeau_reliefs;
        static float coordsTore[3*VILLE_NUM_POINTS_TORE];
        static uint16_t facesTore[VILLE_NUM_FACES_TORE*4];
        static float normalesTore[3*VILLE_NUM_POINTS_TORE];

        static float coordsDisque[3*VILLE_NUM_COTES_DISQUE];
        static float coordsCylindre[3*VILLE_NUM_POINTS_CYL];
        static uint16_t  facesCylindre[(VILLE_NUM_FACES_CYL-2)*4+2*VILLE_NUM_COTES_CYL];
        static float normalesCylindre[3*VILLE_NUM_POINTS_CYL];

        static float coordsSphere[3*VILLE_NUM_POINTS_SPHERE +4];    //?? Bug si pas de +4 ??
        static uint16_t facesSphere[VILLE_NUM_FACES_SPHERE*4-(2*VILLE_NUM_COTES_SPHERE)];
        static float normalesSphere[3*VILLE_NUM_POINTS_SPHERE+4];


        //Textures:

        static Image* tex_couleurs;
        static Image* tex_normales;
        static Image* tex_profondeurs;
        static Texgen* textureCones01;

        static GLuint triangle_liste_id;
        static GLuint disque_liste_id;
        static GLuint sphere_liste_id;
        static GLuint cylindre_liste_id;
        static GLuint tore_liste_id;
        static GLuint cube_liste_id;
        static GLuint cube02_liste_id;
        static GLuint rectangle_contours_liste_id;
        static GLuint plan01_liste_id;
        static GLuint plan02_liste_id;
        static GLuint tetraedre_liste_id;

        static float immeuble_base_sommets[8*3];
        static uint16_t immeuble_base_faces[5*4];
        static float immeuble_base_normales[5*3];

        static GLuint immeuble_base_liste_id;
        static GLuint immeuble_01_liste_id;
        static GLuint immeuble_02_liste_id;
        static GLuint immeuble_03_liste_id;
        static GLuint immeuble_04_liste_id;

        static GLfloat couleur_trottoir[3];
        static GLfloat couleur_chaussee[3];
        static GLfloat couleur_troncs[3];
        static GLfloat couleur_feuillages[3];
        static GLfloat couleur_fenetres[3];
        static GLfloat couleur_lumiere_feuillages[4];
        static GLfloat couleur_epaisseur_bordures_eau[3];
        static GLfloat speculaire_trottoir[4];
        static GLfloat speculaire_chaussee[4];
        static GLfloat speculaire_feuillages[4];
        static GLfloat speculaire_immeubles[4];
        static GLfloat speculaire_vitres[4];
        static GLuint route_01_droite_liste_id;
        static GLuint route_01_virage90_droite_liste_id;
        static GLuint route_01_virage90_gauche_liste_id;
        static GLuint route_01_carrefour_4_liste_id;
        static GLuint route_01_carrefour_3_liste_id;
        static GLuint route_01_parking_liste_id;
        static GLuint route_01_espace_vert_liste_id;

        static GLuint bordure_eau_base_liste_id;


        //Fonctions:
    private:
        static void genere_immeuble_base();
        static void genere_immeuble_01();
        static void genere_immeuble_02();
        static void genere_immeuble_03();
        static void genere_immeuble_04();

        static void genere_coords_cercle(float* coords,float cx,float cz, float r, int nbrSegments,float angleDepart=0.,float angle=360.);
        static void genere_route01_droite(float echelle);
        static void genere_route01_virage90_droite(float echelle);
        static void genere_route01_virage90_gauche(float echelle);
        static void genere_route01_carrefour_3(float echelle);
        static void genere_route01_carrefour_4(float echelle);
        static void genere_route01_parking(float echelle);
        static void genere_route01_espace_vert(float echelle);
        static void genere_bordure_eau_base(float echelle);
    public:
        static bool erreur_openGl(const char* message);
        static void desactive_textures();
        static void affiche_triangle();
        static void affiche_disque();
        static void affiche_cylindre();
        static void affiche_sphere(float luminosite=0.);
        static void affiche_tore();
        static void affiche_rectangle_contours();
        static void affiche_plan01();
        static void affiche_plan02();
        static void affiche_cube();
        static void affiche_cube02();
        static void affiche_tetraedre();

        static void affiche_trame_cercle();

        static bool init_elements(float partSize,float hauteur_etage);
};

#endif // ELEMENTS_VILLE_H_INCLUDED
