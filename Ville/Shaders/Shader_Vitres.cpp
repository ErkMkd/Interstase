#include "Shader_Vitres.h"

    //======================================
    //  Constructeur
    //======================================
    Shader_Vitres::Shader_Vitres(const char* p_nom,char* p_source_vertex,char* p_source_fragment):Shader(p_nom,p_source_vertex,p_source_fragment)
    {
        //--------- Création des liens avec les variables type "uniform":
        if(erreur==SHADER_OK)
        {

            //if (erreur_openGl("ERREUR dans Shader_Vitres::Shader_Vitres() :"))erreur=SHADER_ERREUR_VARIABLES;
        }
    }

    //=========================================
    //      Destructeur
    //=========================================
        Shader_Vitres::~Shader_Vitres()
        {

        }
