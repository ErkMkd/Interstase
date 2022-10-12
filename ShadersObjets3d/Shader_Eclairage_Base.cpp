

#include "Shader_Eclairage_Base.h"

    //======================================
    //  Constructeur
    //======================================
    Shader_Eclairage_Base::Shader_Eclairage_Base(const char* p_nom,char* p_source_vertex,char* p_source_fragment):Shader(p_nom,p_source_vertex,p_source_fragment)
    {
        //--------- Création des liens avec les variables type "uniform":
        if(erreur==SHADER_OK)
        {
            drapeaux_sources=glGetUniformLocation(programme_id,"drapeaux_sources");

            if (erreur_openGl("ERREUR dans Shader_Eclairage_Base::Shader_Eclairage_Base() :"))erreur=SHADER_ERREUR_VARIABLES;
        }
    }

    //=========================================
    //      Destructeur
    //=========================================
        Shader_Eclairage_Base::~Shader_Eclairage_Base()
        {

        }



