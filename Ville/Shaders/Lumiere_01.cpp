#include "Lumiere_01.h"

    //======================================
    //  Constructeur
    //======================================
    Lumiere_01::Lumiere_01(const char* p_nom,char* p_source_vertex,char* p_source_fragment):Shader(p_nom,p_source_vertex,p_source_fragment)
    {
        //--------- Création des liens avec les variables type "uniform":
        if(erreur==SHADER_OK)
        {
            //if (erreur_openGl("ERREUR dans Lumiere_01::Lumiere_01() :"))erreur=SHADER_ERREUR_VARIABLES;
        }
    }

    //=========================================
    //      Destructeur
    //=========================================
        Lumiere_01::~Lumiere_01()
        {

        }
