#ifndef SHADERSVILLE_H_INCLUDED
#define SHADERSVILLE_H_INCLUDED


#include "../../GestionShaders.h"

/*
//*******************************************************************

                    Gestion des shaders au sein du moteur 3d

//*******************************************************************
*/
//------------- Shaders:
#include "Shader_Fenetre_01.h"
#include "Shader_Fenetre_02.h"
#include "Shader_Fenetre_03.h"
#include "Shader_Fenetre_04.h"
#include "Shader_Vitres.h"
#include "Lumiere_01.h"
#include "Shader_Face_Observateur.h"



//------------ Codes d'erreur:

#define SHADERS_VILLE_DEBUG true

#define SHADERS_VILLE_OK 0
#define SHADERS_VILLE_ERREUR_SHADER 1
#define SHADERS_VILLE_ERREUR_FABRICANT 2
#define SHADERS_VILLE_ERREUR_TEXTURES 3




class ShadersVille: public GestionShaders
{
    public:
        static uint16_t erreur;

        //Les shaders:
        static Shader_Fenetre_01* fenetre01;
        static Shader_Fenetre_02* fenetre02;
        static Shader_Fenetre_03* fenetre03;
        static Shader_Fenetre_04* fenetre04;
        static Shader_Vitres* vitres;
        static Lumiere_01* lumiere01;
        static Shader_Face_Observateur* face_observateur;

    private:
        static char chemin_sources[256];
        static char nom_fichier_a[256];
        static char nom_fichier_b[256];
        static char *nom_fichier;

    public:
        static void initShadersVille();
        static void freeShadersVille();

    private:
        static bool creation_shaders();
        static char* renvoie_nom_fichier(const char* nom_source);
        static bool creation_textures();
};


#endif // SHADERSVILLE_H_INCLUDED
