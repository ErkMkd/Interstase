#ifndef VARIABLES_H_INCLUDED
#define VARIABLES_H_INCLUDED

#include "glew.h"
#include <GLFW/glfw3.h>

#include "Matrice4x4.h"
#include "chargeurs_images/Image.h"
#include "Objets3d/ChargeLWO2.h"
#include "Objets3d/ConvertionLWO2.h"
#include "Sprites_2d.h"
#include "TexteMap.h"
#include "Texgen.h"
#include "Objets3d/Cosmonaute.h"
#include "Raytracer01.h"
#include "repere.h"

#define NOMBREDOR 1.61803

//================ Résolution visible de la démo:
#define FIXED_STEP_RENDERER false
#define RESOLUTION_DEMO_X 1280
#define RESOLUTION_DEMO_Y 720

//================ Scènes ======================

#define SCENE_INTRO 1
#define SCENE_WATERWORLD 2
#define SCENE_VILLE 3
#define SCENE_MANDELBROT 4
#define SCENE_SPHEREDANCE 5
#define SCENE_HYPNOSTRUCTURE 6
#define SCENE_FINAL 7

#define NUM_SCENES 7

//================ Paramètres rendu en précalc:

#define FIXED_STEP_FPS 60.f

#define RESOLUTION_RENDU_DEMO_X 1280
#define RESOLUTION_RENDU_DEMO_Y 720

//=============== Gestion timeline:

#define AFFICHE_TIMELINE true

 //========== Synchros musique ===========
//#define BEAT 0.01 //Durée d'une pulsation musicale (1 beat)

//Bloc de paramètres pour l'affichage du reflet dans l'eau
typedef struct
{
    float transparence;
    float attenuation_speculaire;
    float intensite_ondes;
    float vitesse_ondes;
    float quantite_ondes;
    float frequence_ondes;
    float amplitude_ondes;
    float niveau_perturbations_ondes;
    float echelle_texture_bruit_ondes;
    float position_source[3];
    float couleur_fond[4];

    float brillance;
    float couleur_speculaire[4];

} parametres_reflet_eau;

//------------
class GVars
{
    public:
        static bool drapeau_fin_demo;
        static uint8_t scene_id;
        static uint8_t scene_num;
        static double temps;
        static double temps_scene;
        static double temps_reel;
        static double temps_prec;
        static double vitesse_temps;
        static float beat;

        static uint32_t numero_frame;

        static Repere* obs;  //Observateur actif
        static float zMin;  //Profondeur de champ (frustum)
        static float zMax;

        //=======================================

        //Ecran:
        static GLFWwindow* ecran;

        static int largeur_ecran;
        static int hauteur_ecran;
        static int largeur_ecran_visible;   //La résolution finale visible. Cette résolution peut être inférieure
        static int hauteur_ecran_visible;   //à celle de l'écran de rendu, pour l'antialiasing.
        static float ecran_ratio;


        static float Fov;
        static float distanceFocale;
        static Matrice4x4f* ObsMat;

        //======= Déclancheurs:
        static bool drapeau_screenShot;

        //===== paramètres post-rendu:

        static bool drapeau_filtre_HSV;
        static bool drapeau_affiche_scene;
        static bool drapeau_affiche_glow;
        static bool drapeau_affiche_screenShot;

        static bool drapeau_blend_glow; //false=GL_SRC_ALPHA,GL_DST_ALPHA, true=GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA



        static float glow_contraste0;
        static float glow_contraste;
        static float glow_saturation;
        static float saturation_hallo;
        static int taille_hallo;
        static float alpha_scene;
        static float alpha_screenShot;
        static float alpha_glow;
        static float hue;
        static float saturation;
        static float value;

        //------------ commandes clavier:

        static bool disp_donnees; //Affiche les données pour créer des clés d'animation.



        //Texte
        static TexteMap* infosDemo;

        //Textures:
        static Image* espace_01;
        static Image* etoile_01;
        static Image* ombre_sol;
        static GLuint liste_ombre_sol_id;
        static GLuint liste_ombre_sol_flat_id;
        static bool drapeau_affiche_mkd;
        static Texgen* texture_reflet_eau;

        //Sprites:
        #define SPR_INTERSTASE 0
        #define SPR_VILLE 1
        #define SPR_EDORA2 2
        #define SPR_NEBULEUSE 3
        #define SPR_LOGO_MKD 4
        #define SPR_OEILLOGO 5
        #define SPR_EDORA 6
        #define SPR_NUM 7
        static Sprites_2d* spritesDemo;
        static const char f1[];
        static const char f2[];
        static const char f3[];
        static const char f4[];
        static const char f5[];
        static const char f6[];
        static const char f7[];
        static const char* fichiersSprites[];

        //Greets/Crédits
        #define SPR_ZEEND 30
        static Sprites_2d* credits_greets;
        static bool drapeau_affiche_greets;
        static bool drapeau_affiche_credits;
        static bool drapeau_affiche_theEnd;

        //Objets LWO:
        static ChargeLWO2* vaisseaux_LWO;
        static ConvertionLWO2* vaisseaux_conv;
        static Volume* vaisseau_1;
        static Volume* vaisseau_2;

        //Cosmonaute:

        static bool drapeau_affiche_cosmonaute;
        static bool drapeau_affiche_reflet_cosmonaute;
        static bool drapeau_rotation_cosmo_auto; //True si le cosmonaute tourne sur lui-même au pif
        static float facteur_rotation_cosmo_auto;
        static bool drapeau_cosmo_mvt_bruits; //True si le cosmonaute s'agite au pif
        static bool drapeau_cosmo_debout; //True si le cosmonaute se tient debout
        static bool drapeau_cosmo_mvt_bruits_mix_debout; //True si le cosmonaute s'agite au pif ou se tient debout, avec un coefficient de mix entre les deux mouvements.
        static bool drapeau_affiche_ombre_cosmo;
        static bool raz_cosmonaute;
        static Cosmonaute* cosmo;
        static float cosmo_R_auto_t0;
        static float cosmo_T_auto_t0;
        static float cosmo_L_auto_t0;
        static float cosmo_R0_auto;
        static float cosmo_T0_auto;
        static float cosmo_L0_auto;
        static float cosmoR;
        static float cosmoT;
        static float cosmoL;
        static float cosmo_mix_debout_bruits;
        static bool cosmo_echelle_inc;
        static bool cosmo_echelle_dec;

        static float cosmo_couleur_amb[4];
        static float cosmo_couleur_lum[3];
        static float cosmo_couleur_fog[3];
        static float cosmo_fog_zMin;
        static float cosmo_fog_zMax;
        static float altitude_ombre_cosmo;

        // Editeur de trajectoires:
        static bool editionTrajectoire;
};
#endif // VARIABLES_H_INCLUDED
