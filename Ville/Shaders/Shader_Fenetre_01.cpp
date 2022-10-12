

#include "Shader_Fenetre_01.h"

    //======================================
    //  Constructeur
    //======================================
    Shader_Fenetre_01::Shader_Fenetre_01(const char* p_nom,char* p_source_vertex,char* p_source_fragment):Shader(p_nom,p_source_vertex,p_source_fragment)
    {
        //--------- Création des liens avec les variables type "uniform":
        if(erreur==SHADER_OK)
        {
            partSize=glGetUniformLocation(programme_id,"partSize");
            hauteur_etage=glGetUniformLocation(programme_id,"hauteur_etage");
            nombre_etages=glGetUniformLocation(programme_id,"nombre_etages");

            if (erreur_openGl("ERREUR dans Shader_Fenetre_01::Shader_Fenetre_01() :"))erreur=SHADER_ERREUR_VARIABLES;
        }
    }

    //=========================================
    //      Destructeur
    //=========================================
        Shader_Fenetre_01::~Shader_Fenetre_01()
        {

        }



