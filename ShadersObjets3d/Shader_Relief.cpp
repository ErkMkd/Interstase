#include "Shader_Relief.h"

//======================================
//  Constructeur
//======================================
        Shader_Relief::Shader_Relief(const char* p_nom,char* p_source_vertex,char* p_source_fragment):Shader(p_nom,p_source_vertex,p_source_fragment)
        {
            //--------- Création des liens avec les variables type "uniform":
            if(erreur==SHADER_OK)
            {
                //--------- Création des liens avec les variables type "uniform":

                texture_couleur=glGetUniformLocation(programme_id,"texture_couleur");
                texture_normales=glGetUniformLocation(programme_id,"texture_normales");
                texture_profondeurs=glGetUniformLocation(programme_id,"texture_profondeurs");
                echelle_texture=glGetUniformLocation(programme_id,"echelle_texture");
                relief_on=glGetUniformLocation(programme_id,"relief_on");

                //--------- Création des liens avec les variables type "attribute":

                tangente=glGetAttribLocation(programme_id,"tangente");
                binormale=glGetAttribLocation(programme_id,"binormale");

                if (erreur_openGl("ERREUR dans Shader_Relief::Shader_Relief() :"))erreur=SHADER_ERREUR_VARIABLES;
            }
        }

//=========================================
//      Destructeur
//=========================================
        Shader_Relief::~Shader_Relief()
        {

        }
