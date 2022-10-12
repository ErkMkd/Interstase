#ifndef SHADERSOBJETS3D_H_INCLUDED
#define SHADERSOBJETS3D_H_INCLUDED


#include "../GestionShaders.h"
#include "../Bruit.h"

/*
//*******************************************************************

                    Gestion des shaders au sein du moteur 3d

//*******************************************************************
*/
//------------- Shaders:
#include "Shader_Couleur_Rugosite.h"
#include "Shader_Eclairage_Base.h"
#include "Shader_Rugosite.h"
#include "Shader_Aplat_Brut.h"
#include "Shader_Relief.h"
#include "Shader_relief_genCones.h"
#include "Shader_Reflet_eau.h"

#define SHADERS_TAILLE_TEXTURE_BRUIT 256

//------------- Compatibilité:
#define SHADERS_FABRIQUANT_ATI 1
#define SHADERS_FABRIQUANT_NVIDIA 2


//------------ Codes d'erreur:

#define SHADERS_DEBUG true

#define SHADERS_OK 0
#define SHADERS_ERREUR_SHADER 1
#define SHADERS_ERREUR_FABRICANT 2
#define SHADERS_ERREUR_TEXTURES 3




class ShadersObjets3d: public GestionShaders
{
    public:
        static uint16_t erreur;

        static GLuint texture_bruit_id;    //La texture de bruit est utile pour certains shaders

        //Les shaders:
        static Shader_Aplat_Brut* aplat_brut;
        static Shader_Eclairage_Base* eclairage_base;
        static Shader_Eclairage_Base* eclairage_phong;
        static Shader_Rugosite* rugosite;
        static Shader_Couleur_Rugosite* couleur_rugosite;
        static Shader_Relief* relief;
        static Shader_relief_genCones* relief_genCones;
        static Shader_Reflet_eau* reflet_eau;

    private:
        static char chemin_sources[256];
        static char nom_fichier_a[256];
        static char nom_fichier_b[256];
        static char *nom_fichier;

        static uint8_t texture_bruit[SHADERS_TAILLE_TEXTURE_BRUIT*SHADERS_TAILLE_TEXTURE_BRUIT];


    public:
        static void initShadersObjets3d();
        static void freeShadersObjets3d();

    private:
        static bool creation_shaders();
        static char* renvoie_nom_fichier(const char* nom_source);
        static bool creation_textures();
};


#endif // SHADERSOBJETS3D_H_INCLUDED
