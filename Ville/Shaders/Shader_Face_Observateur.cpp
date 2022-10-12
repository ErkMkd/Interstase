#include "Shader_Face_Observateur.h"

    //======================================
    //  Constructeur
    //======================================
    Shader_Face_Observateur::Shader_Face_Observateur(const char* p_nom,char* p_source_vertex,char* p_source_fragment):Shader(p_nom,p_source_vertex,p_source_fragment)
    {
        //--------- Création des liens avec les variables type "uniform":
        if(erreur==SHADER_OK)
        {
            //partSize=glGetUniformLocation(programme_id,"partSize");


            if (erreur_openGl("ERREUR dans Shader_Fenetre_01::Shader_Face_Observateur() :"))erreur=SHADER_ERREUR_VARIABLES;
        }
    }

    //=========================================
    //      Destructeur
    //=========================================
        Shader_Face_Observateur::~Shader_Face_Observateur()
        {

        }
