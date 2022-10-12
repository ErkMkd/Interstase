
#include "Shader_Reflet_eau.h"


    //======================================
    //  Constructeur
    //======================================
        Shader_Reflet_eau::Shader_Reflet_eau(const char* p_nom,char* p_source_vertex,char* p_source_fragment):Shader(p_nom,p_source_vertex,p_source_fragment)
        {
            //--------- Création des liens avec les variables type "uniform":
            if(erreur==SHADER_OK)
            {
                //--------- Création des liens avec les variables type "uniform":

                position_obs=glGetUniformLocation(programme_id,"position_obs");
                position_source=glGetUniformLocation(programme_id,"position_source");
                texture_reflet=glGetUniformLocation(programme_id,"texture_reflet");
                largeur_ecran=glGetUniformLocation(programme_id,"largeur_ecran");
                hauteur_ecran=glGetUniformLocation(programme_id,"hauteur_ecran");
                t=glGetUniformLocation(programme_id,"t");

                texture_bruit=glGetUniformLocation(programme_id,"texture_bruit");
                vitesse_ondes=glGetUniformLocation(programme_id,"vitesse_ondes");
                echelle_texture_bruit_ondes=glGetUniformLocation(programme_id,"echelle_texture_bruit_ondes");
                quantite_ondes=glGetUniformLocation(programme_id,"quantite_ondes");
                frequence_ondes=glGetUniformLocation(programme_id,"frequence_ondes");
                amplitude_ondes=glGetUniformLocation(programme_id,"amplitude_ondes");
                niveau_perturbations_ondes=glGetUniformLocation(programme_id,"niveau_perturbations_ondes");
                intensite_ondes=glGetUniformLocation(programme_id,"intensite_ondes");
                couleur_fond=glGetUniformLocation(programme_id,"couleur_fond");
                transparence=glGetUniformLocation(programme_id,"transparence");
                attenuation_speculaire=glGetUniformLocation(programme_id,"attenuation_speculaire");


                if (erreur_openGl("ERREUR dans Shader_Reflet_eau::Shader_Reflet_eau() :"))erreur=SHADER_ERREUR_VARIABLES;
            }
        }

    //=========================================
    //      Destructeur
    //=========================================
        Shader_Reflet_eau::~Shader_Reflet_eau()
        {

        }

