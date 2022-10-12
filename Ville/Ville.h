#ifndef VILLE_H_INCLUDED
#define VILLE_H_INCLUDED

#include "../glew.h"
#include <GLFW/glfw3.h>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include "../fonctions.h"
#include "../repere.h"
#include "../ShadersObjets3d/ShadersObjets3d.h"
#include "../chargeurs_images/Image.h"
#include "../Texgen.h"
#include "../vec2f.h"
#include "../FrameBuffers.h"
#include "../Bruit.h"
#include "Elements_Ville.h"

#define VILLE_NBR_OBJETS_MAX 20000
#define VILLE_MARGE_IMMERGEE 5.


//Codes des éléments:
#define VILLE_ELEMENT_RIEN 0
#define VILLE_ELEMENT_EAU 1
#define VILLE_ELEMENT_ROUTE 2
#define VILLE_ELEMENT_ESPACE_VERT 3
#define VILLE_ELEMENT_BATIMENT0 4   //Premier code des bâtiments (tout les suivants sont des bâtiments)

//Paramètres des bâtiments:
#define VILLE_BATIMENT_HAUTEUR_ETAGE 3.

//Paramètres de gestion du trafic:
#if FIXED_STEP_RENDERER
    #define VILLE_DISTANCE_MAX 1800
    #define VILLE_NBR_VEHICULES_MAX 5000 //1000*DistanceMax²/640000
#else
    #define VILLE_DISTANCE_MAX 500
    #define VILLE_NBR_VEHICULES_MAX 2000 //1000*DistanceMax²/640000
#endif

#define VILLE_PART_SIZE 30.0f
#define VILLE_NBR_COULEURS_VEHICULES 8

//Element de la liste d'affichage de la ville:
typedef struct
{
    GLfloat matrice[16];
    GLuint liste_id;
    int posX;   //Position dans le bitmap
    int posY;
    int x;   //Position dans la trame
    int y;
    uint8_t couleur;
    int nombre_etages;  //Pour les batiments, le nombre d'étages
}
objet_visible_ville;


typedef struct
{
    float angleRoute;
    GLuint route_id;
}
element_route;


typedef struct
{
    //Numéros des voies:
    //      1
    //      ||
    //  4  =  = 2
    //      ||
    //      3

    int partX;  //Position sur la trame
    int partY;
    float altitude;
    float vitesse_max;  //en m/s
    float vitesse;
    float posPart;  //Position dans le parcours dans l'élément de trame (de 0 à 1)
    float xIn;  //Position relative dans l'élément de trame
    float yIn;
    uint8_t id_intOut;  //identifiant du parcours dans l'élément.
    uint8_t couleur;
    uint8_t code_route; //Type de route sur laquelle se trouve le véhicule
    bool drapeau_hors_champ;    //true lorsque le véhicule est hors du champ de vision.
    float xPrec;    //Pour orienter le véhicule
    float zPrec;
    float rot;  //angle de rotation
    float echelle_x;
    float echelle_y;
    float echelle_z;
    uint8_t typeObjet;

} ville_vehicule;



class Ville : public Elements_Ville
{
    private:
            static bool drapeau_premier_rendu;


            static uint32_t objet_ptr;
            static uint32_t objet_grille_ptr;
            static objet_visible_ville objets[VILLE_NBR_OBJETS_MAX];
            static objet_visible_ville grille[VILLE_NBR_OBJETS_MAX];

            static element_route tab_routes[16];    //Paramètres des éléments de voirie: objet et orientation

            static float t_prec;    //Pour les mouvements des véhicules


    public:
            static Repere objTest_parent;
            static Repere objTest_obsBkp;
            static Repere objTest_enfant;
            static Repere objTest_enfant_temp;
            static Repere objTest_enfant_temp2;

            static uint16_t erreur;
            static bool drapeau_affiche_Edora;
            static float alpha_Edora;

            static float angleObj;
            static int num_etages_debug;
            static float xTetra;

        // Paramètres terrain:
            static float partSize;
            static float distanceMax;
            static int nbrDalles;
            static float marge_terrain;
            static Texgen* bitmap_terrain;
            static uint8_t* map_terrain;
            static uint8_t* map_terrain_temp;
            static uint8_t* map_routes;
            static int taille_terrain;
            static bool drapeau_affiche_bitmap_terrain;
            static vec2f triangle_visible[3];
            static int triangle_visible_int[6];
            static uint32_t couleurs_iters[128];
        //Mandelbox:
            static uint8_t terrain_iterMax;
            static float terrain_r;
            static float terrain_s;
            static float terrain_zoom;

        //----- Paramètres de rendu:
            static Texgen* texture_reflet_eau;   //Texture de rendu du reflet de la scène dans l'eau

        //------- Contrôles de déplacement:
            static float aX;
            static float aY;
            static float aZ;
            static float aR;
            static float aT;
            static float aL;

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
            static bool drapeau_affiche_trame;


        static Repere obs;
        static vec3f point_vise;
        static vec3f position_edora;
        static Bruit clignotement_Edora;

        static float couleur_soleil[3];
        static float couleur_ciel[4];
        static float couleur_brouillard[4];
        static float couleur_fond_eau[4];
        static float distance_brouillard;

        static float L0_orientation[3];
        static float L0_amb[4];
        static float L0_diff[4];
        static float L0_spec[4];


        static float L1_orientation[3];
        static float L1_amb[4];
        static float L1_diff[4];
        static float L1_spec[4];

    //Gestion trafic:
    public:

            static ville_vehicule vehicules[VILLE_NBR_VEHICULES_MAX];
            static uint16_t nbrVehicules;
    public:

            static float palette_vehicules[3*VILLE_NBR_COULEURS_VEHICULES];


    public:
        static char* renvoie_nom_element_obs(); //Renvoie le nom de l'élément où se trouve l'observateur
        static uint8_t renvoie_element_id(int x, int y); //Renvoie l'identifiant de l'élément correspondant à la position.

        static void genere_ville();

        static bool init_textures();
        static bool init_scene();
        static bool init_shaders();
        static void detruit_textures();
        static void detruit_scene();
        static void detruit_shaders();

        static void clavier(int key, bool etat);
        static void maj_deplacement();

        static void eclairages();
        static void maj_positions_lumieres();
        static void render_scene();
    private:
        //Attention, sauver, puis afficher dans le même ordre !
        static void sauve_objet(GLuint list_id,int posX,int posY, int x,int y, uint8_t couleur,int nombre_etages=1);
        static void sauve_objet_grille(GLuint list_id);
        static void affiche_objets();
        static void affiche_objets_reflet();
        static void affiche_grille();
        static void affiche_Edora();
        static void affiche_soleil();

        static uint8_t analyse_alentours_4bits(int x,int y, uint8_t* terrain);
        static uint8_t analyse_alentours_8bits(int x,int y,uint8_t* terrain);
        static uint8_t analyse_alentours_eau(int x, int y);
        static uint8_t optimise_element_route(int x, int y);
        static void genere_bordures(int xoffset,int yoffset,int x,int y);

        static void assigne_element(int x,int y,uint8_t c);
        static void affiche_pixel_terrain(int x,int y,uint8_t c);
        static void affiche_rectangle_terrain(int x, int y, int tx, int ty, uint8_t c);
        static void efface_bitmap_terrain();
        static void efface_map_terrain();
        static void affiche_bitmap_terrain();
        static void affiche_reflet_eau();
        static void init_palette_terrain();
        static void rempli_champ_vision_mandelbox(int ymin,int ymax,float p_x0,float p_x1,float d1,float d2);
        static void genere_grille(int ymin,int ymax,float p_x0,float p_x1,float d1,float d2);
        static void calcul_couleur_immeubles(float* c);

        //Les fonctions de génération des routes, à éxécuter dans cet ordre ! :
        static void creation_espaces_verts(int ymin,int ymax,float p_x0,float p_x1,float d1,float d2);
        static void organise_routes(int ymin,int ymax,float p_x0,float p_x1,float d1,float d2);
        static void optimise_terrain(int ymin,int ymax,float p_x0,float p_x1,float d1,float d2);

        //Gestion trafic:

        static void reset_vehicule(uint16_t id_vehicule);
        static void positionne_vehicules();
        static void positionne_vehicule(uint16_t id_vehicule,int cx,int cy);
        static void recycle_vehicule(uint16_t id_vehicule,int cx,int cy);
        static void rafraichi_vehicules();
        static void rafraichi_vehicule(uint16_t id_vehicule,int cx,int cy);
        static void affiche_vehicules();
        static void affiche_vehicule(uint16_t id_vehicule);
        static void change_vehicule_position(uint16_t id_vehicule,int cx,int cy);
        static void determine_vehicule_in_out(uint16_t id_vehicule, uint8_t code_route);
        static void mouvement_vehicule(uint16_t id_vehicule,int cx,int cy);
};

#endif // VILLE_H_INCLUDED
