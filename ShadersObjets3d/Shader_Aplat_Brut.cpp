

#include "Shader_Aplat_Brut.h"
using namespace std;

/*
    Un tel shader permet le rendu des surfaces en aplats sans �clairages,
    sans avoir � toucher � l'activation/d�sactivation de l'�clairage, qui cause des
    petits probl�mes d'inits avec les listes OpenGl.
*/

    //======================================
    //  Constructeur
    //======================================
    Shader_Aplat_Brut::Shader_Aplat_Brut(const char* p_nom,char* p_source_vertex,char* p_source_fragment) : Shader(p_nom,p_source_vertex,p_source_fragment)
    {
        //--------- Cr�ation des liens avec les variables type "uniform":
        if(erreur!=SHADER_OK)
        {
            //if (erreur_openGl("ERREUR dans Shader_Aplat_Brut::Shader_Aplat_Brut() :"))erreur=DKSSHADER_ERREUR_VARIABLES;
            cout<<"!!! ERREUR lors de la cr�ation du shader Aplat_Brut"<<endl;
        }
    }

    //=========================================
    //      Destructeur
    //=========================================
    Shader_Aplat_Brut::~Shader_Aplat_Brut()
    {

    }



