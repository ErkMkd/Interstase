#include "Variables.h"

    bool GVars::drapeau_fin_demo=false;
    uint8_t GVars::scene_id=SCENE_WATERWORLD;
    uint8_t GVars::scene_num=0;
    double GVars::temps=0.;
    double GVars::temps_scene;  //Temps relatif d'une scène
    double GVars::vitesse_temps=1.;
    double GVars::temps_reel=0.;
    double GVars::temps_prec=0.;
    float GVars::beat=0.4;
    uint32_t GVars::numero_frame=0;

    Repere* GVars::obs=NULL;
    float GVars::zMin;
    float GVars::zMax;

    //=======================================

    //Ecran:
    GLFWwindow* GVars::ecran = NULL;

    int GVars::largeur_ecran=RESOLUTION_RENDU_DEMO_X*2;
    int GVars::hauteur_ecran=RESOLUTION_RENDU_DEMO_Y*2;
    int GVars::largeur_ecran_visible=RESOLUTION_DEMO_X;
    int GVars::hauteur_ecran_visible=RESOLUTION_DEMO_Y;
    float GVars::ecran_ratio=1.;


    float GVars::Fov=100.;
    float GVars::distanceFocale;
    Matrice4x4f* GVars::ObsMat=NULL;

    //====== Déclancheurs:
     bool GVars::drapeau_screenShot=false;  //True si on a besoin d'un screenshot, mis à false automatiquement après la sauvegarde de l'écran

    //===== paramètres post-rendu:

    bool GVars::drapeau_filtre_HSV=true;
    bool GVars::drapeau_affiche_scene=true;
    bool GVars::drapeau_affiche_glow=true;
    bool GVars::drapeau_affiche_screenShot=false;

    bool GVars::drapeau_blend_glow=false;

    float GVars::glow_contraste0=0.8;
    float GVars::glow_contraste=5.;
    float GVars::glow_saturation=1.;
    float GVars::saturation_hallo=1.;
    int GVars::taille_hallo=20;
    float GVars::alpha_scene=1.;
    float GVars::alpha_screenShot=1.;
    float GVars::alpha_glow=.5;
    float GVars::hue=0.;
    float GVars::saturation=1.;
    float GVars::value=1.;

    //------------ commandes clavier:

    bool GVars::disp_donnees=false; //Affiche les données pour créer des clés d'animation.

    //Texteslogovale15.tif
    TexteMap* GVars::infosDemo=NULL;

    //Textures:
    Image* GVars::espace_01=NULL;
    Image* GVars::etoile_01=NULL;
    Image* GVars::ombre_sol=NULL;
    GLuint GVars::liste_ombre_sol_id=0;
    GLuint GVars::liste_ombre_sol_flat_id=0;
    Texgen* GVars::texture_reflet_eau=NULL;

    bool GVars::drapeau_affiche_mkd=false;

    //Sprites:
    Sprites_2d* GVars::spritesDemo=NULL;

    const char GVars::f1[]="images/Interstases_gwen_b.tif";
    const char GVars::f2[]="images/ville.tif";
    const char GVars::f3[]="images/Edora_2.tif";
    const char GVars::f4[]="images/nebuleuse_02.jpg";
    const char GVars::f5[]="images/mankind_logo.tif";
    const char GVars::f6[]="images/oeil_logo.tif";
    const char GVars::f7[]="images/Edora.tif";

    Sprites_2d* GVars::credits_greets=NULL;
    bool GVars::drapeau_affiche_greets=false;
    bool GVars::drapeau_affiche_credits=false;
    bool GVars::drapeau_affiche_theEnd=false;

    const char* GVars::fichiersSprites[]={GVars::f1,GVars::f2,GVars::f3,GVars::f4,GVars::f5,GVars::f6,GVars::f7};

    //Objet LWO:
    ChargeLWO2* GVars::vaisseaux_LWO=NULL;
    ConvertionLWO2* GVars::vaisseaux_conv=NULL;
    Volume* GVars::vaisseau_1=NULL;
    Volume* GVars::vaisseau_2=NULL;


    //Cosmonaute:
    bool GVars::drapeau_affiche_cosmonaute=true;
    bool GVars::drapeau_affiche_reflet_cosmonaute=false;
    bool GVars::drapeau_rotation_cosmo_auto=false;
    float GVars::facteur_rotation_cosmo_auto=1.;
    bool GVars::drapeau_cosmo_mvt_bruits=true;
    bool GVars::drapeau_cosmo_debout=false;
    bool GVars::drapeau_cosmo_mvt_bruits_mix_debout=false;
    bool GVars::drapeau_affiche_ombre_cosmo=false;
    bool GVars::raz_cosmonaute=false;
    Cosmonaute* GVars::cosmo=NULL;
    float GVars::cosmo_R_auto_t0=0.;
    float GVars::cosmo_T_auto_t0=0.;
    float GVars::cosmo_L_auto_t0=0.;
    float GVars::cosmo_R0_auto=31.;
    float GVars::cosmo_T0_auto=15.;
    float GVars::cosmo_L0_auto=67.;
    float GVars::cosmoR=0.;
    float GVars::cosmoT=0.;
    float GVars::cosmoL=0.;
    float GVars::cosmo_mix_debout_bruits=0.;
    bool GVars::cosmo_echelle_inc=false;
    bool GVars::cosmo_echelle_dec=false;

    float GVars::cosmo_couleur_amb[4];
    float GVars::cosmo_couleur_lum[3];
    float GVars::cosmo_couleur_fog[3];
    float GVars::cosmo_fog_zMin;
    float GVars::cosmo_fog_zMax;
    float GVars::altitude_ombre_cosmo=5.02;

    //Editeur trajectoires:
    bool GVars::editionTrajectoire=false;

