/*
*
*       Demo INTERSTASES- Mankind
*
*
*/
//#define GLEW_STATIC

#include "glew.h"
#include <GLFW/glfw3.h>


#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "math.h"

#include "gestion_fichiers.h"
#include "matrice4x4.h"
#include "chargeurs_images/Image.h"

#include "Musique.h"
#include "repere.h"

#include "Variables.h"
#include "FrameBuffers.h"
#include "fonctions.h"
#include "Intro.h"
#include "waterworld.h"
#include "mandelbrot.h"
#include "spheredance.h"
#include "hypnostructure.h"
#include "Final.h"
#include "editeur_trajectoire.h"
#include "Ville/Ville.h"

#include "ShadersObjets3d/ShadersObjets3d.h"
#include "Ville/Shaders/ShadersVille.h"

#include "bass/bass.h"
        using namespace std;

#define FULLSCREEN false
#define JOUE_MUSIQUE false
#define VERSION_LONGUE false

//================ Gestion du rendu en précalc:


#include "timeline.cpp"

void detruit_textes()
{
    if(GVars::infosDemo!=NULL)delete GVars::infosDemo;

}

void detruit_bruits()
{

}

void detruit_textures()
{
    if(GVars::texture_reflet_eau!=NULL) delete GVars::texture_reflet_eau;
    if(GVars::espace_01!=NULL)delete GVars::espace_01;
    if(GVars::etoile_01!=NULL)delete GVars::etoile_01;
    if(GVars::ombre_sol!=NULL)delete GVars::ombre_sol;
    if(GVars::credits_greets!=NULL)delete GVars::credits_greets;
    if(GVars::spritesDemo!=NULL)delete GVars::spritesDemo;
    if(glIsList(GVars::liste_ombre_sol_flat_id))glDeleteLists(GVars::liste_ombre_sol_flat_id,1);
    if(glIsList(GVars::liste_ombre_sol_id))glDeleteLists(GVars::liste_ombre_sol_id,1);
}

void detruit_matrices()
{
    if(GVars::ObsMat!=NULL) delete GVars::ObsMat;
}

void detruit_LWO()
{
    if(GVars::cosmo!=NULL) delete GVars::cosmo;
    if(GVars::vaisseaux_conv!=NULL)delete GVars::vaisseaux_conv;
    if(GVars::vaisseaux_LWO!=NULL)delete GVars::vaisseaux_LWO;
}

static void fin_programme( int code=0 )
{
	if(JOUE_MUSIQUE)Musique::detruit_musique();
    ShadersDemo::freeShaders();
	FrameBuffers::supprime_frameBuffers();
	Ville::detruit_scene();
	Intro::detruit_scene();
	Final::detruit_scene();
	WaterWorld::detruit_scene_waterworld();
	Mandelbrot::detruit_scene_mandelbrot();
	detruit_matrices();
	detruit_textures();
	detruit_LWO();
	detruit_textes();
	detruit_bruits();
	ShadersObjets3d::freeShadersObjets3d();
	ShadersVille::freeShadersVille();
    if(GVars::ecran!=NULL) glfwDestroyWindow(GVars::ecran);
    glfwTerminate();
    cout<<"Fin du programme..."<<endl;
    exit( code );
}

bool init_bruits()
{

    return true;
}

bool init_matrices()
{
    GVars::ObsMat=new Matrice4x4f;
    return true;
}

bool initialise_textes()
{
    GVars::infosDemo=new TexteMap("polices/Groupe.tif",GVars::largeur_ecran,GVars::hauteur_ecran);
    if(GVars::infosDemo==NULL || GVars::infosDemo->erreur!=TEXTEMAP_OK) return false;
    GVars::infosDemo->mode_rendu=TEXTEMAP_RENDU_2D;
    GVars::infosDemo->determine_position_curseur(0.01,0.97);
    GVars::infosDemo->determine_echelle(float(GVars::infosDemo->renvoie_largeur_caracteres())/float(GVars::largeur_ecran_visible));
    GVars::infosDemo->determine_espace_lignes(1.1);
    return true;
}

bool initialise_textures()
{
    GVars::espace_01=new Image("images/cosmos.tif");
    GVars::etoile_01=new Image("images/etoile_a00.tif");
    GVars::ombre_sol=new Image("images/ombre_sol.tif");
    GVars::spritesDemo=new Sprites_2d(SPR_NUM,GVars::fichiersSprites);
    if(GVars::spritesDemo==NULL
       || GVars::spritesDemo->erreur!=SPR2D_OK
       || GVars::espace_01->erreur!=IMAGE_OK
       || GVars::etoile_01->erreur!=IMAGE_OK
       || GVars::ombre_sol->erreur!=IMAGE_OK
       ) return false;
    GVars::spritesDemo->determine_resolution(GVars::largeur_ecran,GVars::hauteur_ecran);
    GVars::spritesDemo->determine_resolution_reference_XY(1280,720);
    GVars::spritesDemo->cache();

    GVars::credits_greets=new Sprites_2d(430,80,"images/greets_credits.tif");
    if(GVars::credits_greets==NULL || GVars::credits_greets->erreur!=SPR2D_OK) return false;
    GVars::credits_greets->cache();
    GVars::credits_greets->determine_resolution_reference_XY(1280,720);

    GVars::liste_ombre_sol_id=glGenLists(1);
    GVars::liste_ombre_sol_flat_id=glGenLists(1);
	GFunc::genere_ombre_sol_listes();

    GVars::texture_reflet_eau=new Texgen(GVars::largeur_ecran,GVars::hauteur_ecran,GL_RGBA,GL_UNSIGNED_BYTE,GL_MIRRORED_REPEAT,GL_MIRRORED_REPEAT);
    if(GVars::texture_reflet_eau==NULL || GVars::texture_reflet_eau->erreur!=TEXGEN_OK) return false;
    if(!GVars::texture_reflet_eau->genere_framebuffer(true)) return false;
    return GVars::espace_01->genere_texture_openGL(false,GL_MIRRORED_REPEAT,GL_MIRRORED_REPEAT)
        | GVars::ombre_sol->genere_texture_openGL(false);
}

bool initialise_LWO()
{
    GVars::cosmo=new Cosmonaute("lwo/cosmo_hi_articule.lwo");
    if(GVars::cosmo==NULL || GVars::cosmo->erreur!=COSMO_OK) return false;

    //-------- Vaisseaux:

    GVars::vaisseaux_LWO=new ChargeLWO2("lwo/objets-space-trip/vaisseaux.lwo","lwo/objets-space-trip/Images/");

    if(GVars::vaisseaux_LWO==NULL || GVars::vaisseaux_LWO->erreur!=CHARGELWO2_OK)
    {
        return false;
    }

    GVars::vaisseaux_conv=new ConvertionLWO2(GVars::vaisseaux_LWO);
    if(GVars::vaisseaux_conv==NULL || GVars::vaisseaux_conv->erreur!=CONVERTIONLWO2_OK)
    {
        return false;
    }

    GVars::vaisseau_1=GVars::vaisseaux_conv->renvoie_volume_index(0);
    GVars::vaisseau_2=GVars::vaisseaux_conv->renvoie_volume_index(1);

    return true;
}



int main()
{
        //Pour afficher correctement les accents dans la console....Juste pour le débuguage.
        //SetConsoleOutputCP(1252);


        glfwSetErrorCallback(GFunc::glfw_error_callback);

        if(!glfwInit())
        {
                cout<<"Erreur lors de l'initialisation de GLFW"<<endl;
              exit(EXIT_FAILURE);
        }

        glfwWindowHint(GLFW_RESIZABLE,GL_TRUE);

        #if FULLSCREEN
		GVars::ecran = glfwCreateWindow(GVars::largeur_ecran_visible,GVars::hauteur_ecran_visible, "Interstase", glfwGetPrimaryMonitor(), NULL);
        #else
		GVars::ecran = glfwCreateWindow(GVars::largeur_ecran_visible,GVars::hauteur_ecran_visible, "Interstase", NULL, NULL);
         #endif

         if(GVars::ecran==NULL)
         {
                 cout<<"Erreur - Impossible d'initialiser l'écran"<<endl;
               fin_programme();
                exit(EXIT_FAILURE);
        }
        glfwSetInputMode(GVars::ecran,GLFW_CURSOR,GLFW_CURSOR_HIDDEN);


        glfwMakeContextCurrent(GVars::ecran);

        glfwSetWindowSizeCallback(GVars::ecran,GFunc::redimensionne_ecran);

        glfwSetKeyCallback(GVars::ecran,GFunc::key_callback);

        cout<<"==================================================================="<<endl;
        cout<<"                     Informations OpenGl"<<endl;
        cout<<"==================================================================="<<endl<<endl;

        cout<<"Version: "<<glGetString(GL_VERSION)<<endl;
        cout<<"Chipset graphique: "<<glGetString(GL_RENDERER)<<endl;
        cout<<"Fabricant: "<<glGetString(GL_VENDOR)<<endl;
        //cout<<"Extensions: "<<glGetString(GL_EXTENSIONS)<<endl;
        cout<<endl;

        //=============== Init Glew
        GLenum err = glewInit();
        if (GLEW_OK != err)
        {
                /* Problem: glewInit failed, something is seriously wrong. */
                fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
                fin_programme();
                exit(EXIT_FAILURE);
        }
        else cout<<"Status: Using GLEW "<< glewGetString(GLEW_VERSION)<<endl;


        GFunc::setup_opengl();

        GFunc::maj_distanceFocale();

        if(!FrameBuffers::initialise_frameBuffers())
        {
            cout<<"Erreur - Impossible d'initialiser les tempons de rendus"<<endl;
            fin_programme();
            exit(EXIT_FAILURE);
        }

        if(!init_trajectoires())
        {
            cout<<"Erreur - Impossible de charger les trajectoires"<<endl;
            fin_programme();
            exit(EXIT_FAILURE);
        }

        if(!initialise_textes())
        {
            cout<<"Erreur - Impossible d'initialiser les textes"<<endl;
            fin_programme();
            exit(EXIT_FAILURE);
        }

        if(!init_bruits())
        {
            cout<<"Erreur - Impossible d'initialiser les bruits"<<endl;
            fin_programme();
            exit(EXIT_FAILURE);
        }

        if(!initialise_textures())
        {
            cout<<"Erreur - Impossible d'initialiser les textures"<<endl;
            fin_programme();
            exit(EXIT_FAILURE);
        }

        if(!initialise_LWO())
        {
            cout<<"Erreur - Impossible d'initialiser les objet LWO"<<endl;
            fin_programme();
            exit(EXIT_FAILURE);
        }

        if(!init_matrices())
        {
            cout<<"Erreur - Impossible d'initialiser les matrices"<<endl;
            fin_programme();
            exit(EXIT_FAILURE);
        }

        if(!Intro::init_scene())
        {

            cout<<"Erreur - Impossible d'initialiser la scène d'introduction"<<endl;
            fin_programme();
            exit(EXIT_FAILURE);
        }

        if(!WaterWorld::init_scene_waterworld())
        {

            cout<<"Erreur - Impossible d'initialiser la scène Waterworld"<<endl;
            fin_programme();
            exit(EXIT_FAILURE);
        }
        if(!SphereDance::init_scene_spheredance())
        {
            cout<<"Erreur - Impossible d'initialiser la scène Spheredance"<<endl;
            fin_programme();
            exit(EXIT_FAILURE);
        }
        if(!Mandelbrot::init_scene_mandelbrot())
        {
            cout<<"Erreur - Impossible d'initialiser la scène Mandelbrot"<<endl;
            fin_programme();
            exit(EXIT_FAILURE);
        }
        if(!HypnoStructure::init_scene_hypnostructure())
        {
            cout<<"Erreur - Impossible d'initialiser la scène Hypnostructure"<<endl;
            fin_programme();
            exit(EXIT_FAILURE);
        }

        if(!Final::init_scene())
        {
            cout<<"Erreur - Impossible d'initialiser la scène Finale"<<endl;
            fin_programme();
            exit(EXIT_FAILURE);
        }

        if(!ShadersDemo::initShaders())
        {
            cout<<"Erreur - Impossible d'initialiser les shaders"<<endl;
            fin_programme();
            exit(EXIT_FAILURE);
        }

        ShadersObjets3d::initShadersObjets3d();
        if(ShadersObjets3d::erreur!=SHADERS_OK)
        {
            cout<<"Erreur - Impossible d'initialiser les shaders des objets 3d"<<endl;
            fin_programme();
            exit(EXIT_FAILURE);
        }
        ShadersVille::initShadersVille();
        if(ShadersVille::erreur!=SHADERS_VILLE_OK)
        {
            cout<<"Erreur - Impossible d'initialiser les shaders du générateur de ville"<<endl;
            fin_programme();
            exit(EXIT_FAILURE);
        }



        if(!Ville::init_scene())
        {
            cout<<"Erreur - Impossible d'initialiser la scène Ville"<<endl;
            fin_programme();
            exit(EXIT_FAILURE);
        }

        GFunc::vue_perspective();

    //Création des listes OpenGl:


        GVars::cosmo->init_listes();


        //GFunc::vue_ortho();

	//if(!init_musique("musique/Hyperspheres beyond space.wav"))
	if(JOUE_MUSIQUE)
    {

        //if(!Musique::init_musique("musique/Les_voix_des_etres.ogg"))
        //if(!Musique::init_musique("musique/Atomic_Lazeroid.ogg"))
        //if(!Musique::init_musique("musique/La_nebuleuse_de_laigle.ogg"))
        //if(!Musique::init_musique("musique/Omaya.ogg"))
        //if(!Musique::init_musique("musique/Interstase.ogg"))
        //if(!Musique::init_musique("musique/Hyperspheres_aux_ombres_oblongues_Interstase_courte.wav"))
        //if(!Musique::init_musique("musique/hyperspheres.ogg"))
        //if(!Musique::init_musique("musique/analogik_3VIL.wav"))
        //if(!Musique::init_musique("musique/Envois-du-bois-Interstase.ogg"))
        //if(!Musique::init_musique("musique/CiytY-grOove-05.ogg"))
        //if(!Musique::init_musique("musique/Inter-05-courte-04.ogg"))
        if(!Musique::init_musique("musique/interstase.ogg"))
        {
            cout<<"Erreur - Impossible d'initialiser la musique"<<endl;
            fin_programme();
            exit(EXIT_FAILURE);
        }
    }


    //*********************** Initialisation de l'éditeur de trajectoire ***************
		EditeurTrajectoire::init();
    //***********************************************************************************

	//Musique::musique_joue();

	glfwSetTime(0.);

        while(!glfwWindowShouldClose(GVars::ecran))
        {

            #if !FIXED_STEP_RENDERER
                GVars::temps_prec=GVars::temps_reel;
                GVars::temps_reel=glfwGetTime();
                #if AFFICHE_TIMELINE
                GVars::vitesse_temps=float(int(GVars::vitesse_temps*10.))/10.;
                GVars::temps+=(GVars::temps_reel-GVars::temps_prec)*GVars::vitesse_temps;
                #else
                GVars::temps=GVars::temps_reel;
                #endif
                //GVars::temps=GVars::temps_reel;
            #else
                GVars::numero_frame++;
                GVars::temps_prec=GVars::temps_reel;
                GVars::temps_reel=double(GVars::numero_frame)/FIXED_STEP_FPS;
                GVars::temps=GVars::temps_reel;
            #endif

            switch (GVars::scene_id)
            {
                case SCENE_INTRO:Intro::maj_deplacement();break;
                case SCENE_WATERWORLD:WaterWorld::maj_deplacement();break;
                case SCENE_VILLE:Ville::maj_deplacement();break;
                case SCENE_MANDELBROT:Mandelbrot::maj_deplacement();break;
                case SCENE_SPHEREDANCE:SphereDance::maj_deplacement();break;
                case SCENE_HYPNOSTRUCTURE:HypnoStructure::maj_deplacement();break;
                case SCENE_FINAL:Final::maj_deplacement();break;
            }



            draw_screen();
            if(GVars::drapeau_fin_demo)glfwSetWindowShouldClose(GVars::ecran,GL_TRUE);

            glfwPollEvents();
        }

		EditeurTrajectoire::destruction_trajectoires();
        fin_programme();
        exit(EXIT_SUCCESS);
}


