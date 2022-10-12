#include "Shader_relief_genCones.h"

//======================================
//  Constructeur
//======================================
        Shader_relief_genCones::Shader_relief_genCones(const char* p_nom,char* p_source_vertex,char* p_source_fragment):Shader(p_nom,p_source_vertex,p_source_fragment)
        {
            //--------- Création des liens avec les variables type "uniform":
            if(erreur==SHADER_OK)
            {
                //--------- Création des liens avec les variables type "uniform":

                texture_profondeurs=glGetUniformLocation(programme_id,"texture_profondeurs");
                largeur_texture_sortie=glGetUniformLocation(programme_id,"largeur_texture_sortie");
                hauteur_texture_sortie=glGetUniformLocation(programme_id,"hauteur_texture_sortie");
                echelle_profondeur=glGetUniformLocation(programme_id,"echelle_profondeur");

                if (erreur_openGl("ERREUR dans Shader_relief_genCones::Shader_relief_genCones() :"))erreur=SHADER_ERREUR_VARIABLES;
            }
        }

//=========================================
//      Destructeur
//=========================================
        Shader_relief_genCones::~Shader_relief_genCones()
        {

        }
