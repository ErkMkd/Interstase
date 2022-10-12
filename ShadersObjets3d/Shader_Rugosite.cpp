

#include "Shader_Rugosite.h"



    //======================================
    //  Constructeur
    //======================================
        Shader_Rugosite::Shader_Rugosite(const char* p_nom,char* p_source_vertex,char* p_source_fragment):Shader(p_nom,p_source_vertex,p_source_fragment)
        {
            //--------- Création des liens avec les variables type "uniform":
            if(erreur==SHADER_OK)
            {
                //--------- Création des liens avec les variables type "uniform":

                texture_rugosite=glGetUniformLocation(programme_id,"texture_rugosite");

                //--------- Création des liens avec les variables type "attribute":

                tangente=glGetAttribLocation(programme_id,"tangente");
                binormale=glGetAttribLocation(programme_id,"binormale");

                if (erreur_openGl("ERREUR dans Shader_Rugosite::Shader_Rugosite() :"))erreur=SHADER_ERREUR_VARIABLES;
            }
        }

    //=========================================
    //      Destructeur
    //=========================================
        Shader_Rugosite::~Shader_Rugosite()
        {

        }




