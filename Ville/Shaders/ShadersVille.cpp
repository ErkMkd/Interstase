

#include "ShadersVille.h"

/*
//*******************************************************************

                    Gestion des shaders

//*******************************************************************
*/
        uint16_t ShadersVille::erreur=SHADERS_VILLE_OK;

        //Les shaders:
        Shader_Fenetre_01* ShadersVille::fenetre01=NULL;
        Shader_Fenetre_02* ShadersVille::fenetre02=NULL;
        Shader_Fenetre_03* ShadersVille::fenetre03=NULL;
        Shader_Fenetre_04* ShadersVille::fenetre04=NULL;
        Shader_Vitres* ShadersVille::vitres=NULL;
        Lumiere_01* ShadersVille::lumiere01=NULL;
        Shader_Face_Observateur* ShadersVille::face_observateur=NULL;

        char ShadersVille::chemin_sources[256];
        char ShadersVille::nom_fichier_a[256];
        char ShadersVille::nom_fichier_b[256];
        char* ShadersVille::nom_fichier;


    //=================================================================
    //                          Constructeur
    //=================================================================


        void ShadersVille::initShadersVille()
        {
            erreur=SHADERS_VILLE_OK;

            fenetre01=NULL;
            fenetre02=NULL;
            fenetre03=NULL;
            fenetre04=NULL;
            vitres=NULL;
            lumiere01=NULL;
            face_observateur=NULL;

            strcpy(chemin_sources,"shaders/Ville/");

            //Création des shaders:
            if(!creation_shaders())
            {
                erreur=SHADERS_VILLE_ERREUR_SHADER;
                return;
            }

            //Création des textures utilisées par les shaders:
            if(!creation_textures()) return;
        }
    //=================================================================
    //                          Destructeur
    //=================================================================
        void ShadersVille::freeShadersVille()
        {
            if(fenetre01!=NULL) delete fenetre01;
            if(fenetre02!=NULL) delete fenetre02;
            if(fenetre03!=NULL) delete fenetre03;
            if(fenetre04!=NULL) delete fenetre04;
            if(vitres!=NULL) delete vitres;
            if(lumiere01!=NULL) delete lumiere01;
            if(face_observateur!=NULL) delete face_observateur;

        }

    //========================================================================
    //          Création des shaders:
    //========================================================================

        bool ShadersVille::creation_shaders()
        {
            fenetre01=new Shader_Fenetre_01("fenetre01",renvoie_nom_fichier("fenetre_01_vertex.glsl"),renvoie_nom_fichier("fenetre_01_fragment.glsl"));
            if(fenetre01->erreur!=SHADER_OK) return false;

            fenetre02=new Shader_Fenetre_02("fenetre02",renvoie_nom_fichier("fenetre_02_vertex.glsl"),renvoie_nom_fichier("fenetre_02_fragment.glsl"));
            if(fenetre02->erreur!=SHADER_OK) return false;

            fenetre03=new Shader_Fenetre_03("fenetre03",renvoie_nom_fichier("fenetre_03_vertex.glsl"),renvoie_nom_fichier("fenetre_03_fragment.glsl"));
            if(fenetre03->erreur!=SHADER_OK) return false;

            fenetre04=new Shader_Fenetre_04("fenetre04",renvoie_nom_fichier("fenetre_04_vertex.glsl"),renvoie_nom_fichier("fenetre_04_fragment.glsl"));
            if(fenetre04->erreur!=SHADER_OK) return false;

            vitres=new Shader_Vitres("vitres",renvoie_nom_fichier("vitres_vertex.glsl"),renvoie_nom_fichier("vitres_fragment.glsl"));
            if(vitres->erreur!=SHADER_OK) return false;

            lumiere01=new Lumiere_01("lumiere01",renvoie_nom_fichier("lumiere_01_vertex.glsl"),renvoie_nom_fichier("lumiere_01_fragment.glsl"));
            if(lumiere01->erreur!=SHADER_OK) return false;

            face_observateur=new Shader_Face_Observateur("face_observateur",renvoie_nom_fichier("face_observateur_vertex.glsl"),renvoie_nom_fichier("face_observateur_fragment.glsl"));
            if(face_observateur->erreur!=SHADER_OK) return false;
            return true;
        }

    //=========================================================================
    //      Concaténation du chemin des sources avec les noms des fichiers
    //=========================================================================
        char* ShadersVille::renvoie_nom_fichier(const char* nom_source)
        {
            if(nom_fichier==nom_fichier_a)nom_fichier=nom_fichier_b;
            else if(nom_fichier==nom_fichier_b)nom_fichier=nom_fichier_a;
            else nom_fichier=nom_fichier_a;
            for(uint16_t i=0;i<256;i++)nom_fichier[i]=0;
            strcpy(nom_fichier,chemin_sources);
            strcat(nom_fichier,nom_source);
            return nom_fichier;
        }
    //================================================================================
    //          Création des textures:
    //================================================================================
        bool ShadersVille::creation_textures()
        {
           return true;
        }




