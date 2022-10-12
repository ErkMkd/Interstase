

#include "ShadersObjets3d.h"

/*
//*******************************************************************

                    Gestion des shaders au sein du moteur 3d

//*******************************************************************
*/
        uint16_t ShadersObjets3d::erreur=SHADERS_OK;

        GLuint ShadersObjets3d::texture_bruit_id;    //La texture de bruit est utile pour certains shaders

        //Les shaders:
        Shader_Aplat_Brut* ShadersObjets3d::aplat_brut;
        Shader_Eclairage_Base* ShadersObjets3d::eclairage_base;
        Shader_Eclairage_Base* ShadersObjets3d::eclairage_phong;
        Shader_Rugosite* ShadersObjets3d::rugosite;
        Shader_Couleur_Rugosite* ShadersObjets3d::couleur_rugosite;
        Shader_Relief* ShadersObjets3d::relief;
        Shader_relief_genCones* ShadersObjets3d::relief_genCones;
        Shader_Reflet_eau* ShadersObjets3d::reflet_eau;

        char ShadersObjets3d::chemin_sources[256];
        char ShadersObjets3d::nom_fichier_a[256];
        char ShadersObjets3d::nom_fichier_b[256];
        char* ShadersObjets3d::nom_fichier;

        uint8_t ShadersObjets3d::texture_bruit[SHADERS_TAILLE_TEXTURE_BRUIT*SHADERS_TAILLE_TEXTURE_BRUIT];


    //=================================================================
    //                          Constructeur
    //=================================================================


        void ShadersObjets3d::initShadersObjets3d()
        {
            erreur=SHADERS_OK;

            aplat_brut=NULL;
            eclairage_base=NULL;
            eclairage_phong=NULL;
            rugosite=NULL;
            couleur_rugosite=NULL;
            relief=NULL;
            relief_genCones=NULL;
            reflet_eau=NULL;

            //Gestion carte graphique:
            strcpy(chemin_sources,"shaders/moteur3d/");

            //Création des shaders:
            if(!creation_shaders())
            {
                erreur=SHADERS_ERREUR_SHADER;
                return;
            }

            //Création des textures utilisées par les shaders:
            if(!creation_textures()) return;
        }
    //=================================================================
    //                          Destructeur
    //=================================================================
        void ShadersObjets3d::freeShadersObjets3d()
        {
            if(aplat_brut!=NULL) delete aplat_brut;
            if(eclairage_base!=NULL) delete eclairage_base;
            if(eclairage_phong!=NULL) delete eclairage_phong;
            if(rugosite!=NULL) delete rugosite;
            if(couleur_rugosite!=NULL) delete couleur_rugosite;
            if(relief!=NULL) delete relief;
            if(relief_genCones!=NULL) delete relief_genCones;
            if(reflet_eau!=NULL) delete reflet_eau;
        }

    //========================================================================
    //          Création des shaders:
    //========================================================================

        bool ShadersObjets3d::creation_shaders()
        {
            aplat_brut=new Shader_Aplat_Brut("aplat_brut",renvoie_nom_fichier("aplat_brut_vertex.glsl"),renvoie_nom_fichier("aplat_brut_fragment.glsl"));
            if(aplat_brut->erreur!=SHADER_OK) return false;

            eclairage_base=new Shader_Eclairage_Base("eclairage_base",renvoie_nom_fichier("eclairage_base_vertex.glsl"),renvoie_nom_fichier("eclairage_base_fragment.glsl"));
            if(eclairage_base->erreur!=SHADER_OK) return false;

            eclairage_phong=new Shader_Eclairage_Base("eclairage_phong",renvoie_nom_fichier("eclairage_phong_vertex.glsl"),renvoie_nom_fichier("eclairage_phong_fragment.glsl"));
            if(eclairage_phong->erreur!=SHADER_OK) return false;

            rugosite=new Shader_Rugosite("rugosité",renvoie_nom_fichier("rugosite_vertex.glsl"),renvoie_nom_fichier("rugosite_fragment.glsl"));
            if(rugosite->erreur!=SHADER_OK) return false;

            couleur_rugosite=new Shader_Couleur_Rugosite("rugosite_couleur",renvoie_nom_fichier("rugosite_couleur_vertex.glsl"),renvoie_nom_fichier("rugosite_couleur_fragment.glsl"));
            if(couleur_rugosite->erreur!=SHADER_OK) return false;

            relief=new Shader_Relief("relief",renvoie_nom_fichier("relief_vertex.glsl"),renvoie_nom_fichier("relief_fragment.glsl"));
            if(relief->erreur!=SHADER_OK) return false;

            relief_genCones=new Shader_relief_genCones("relief_genCones",renvoie_nom_fichier("relief_genCones_vertex.glsl"),renvoie_nom_fichier("relief_genCones_fragment.glsl"));
            if(relief_genCones->erreur!=SHADER_OK) return false;

            reflet_eau=new Shader_Reflet_eau("reflet_eau",renvoie_nom_fichier("reflet_eau_vertex.glsl"),renvoie_nom_fichier("reflet_eau_fragment.glsl"));
            if(reflet_eau->erreur!=SHADER_OK) return false;
/*
            reflexion_cube=new Shader_Reflexion_Cube("reflexion_cube",renvoie_nom_fichier("reflexion_cube_vertex.glsl"),renvoie_nom_fichier("reflexion_cube_fragment.glsl"));
            if(reflexion_cube->erreur!=SHADER_OK) return false;


            ombres=new Shader_Ombres("ombres",renvoie_nom_fichier("ombres_vertex.glsl"),renvoie_nom_fichier("ombres_fragment.glsl"));
            if(ombres->erreur!=SHADER_OK) return false;

            ombres_texture_couleur=new Shader_Ombres_Texture_Couleur("ombres_texture_couleur",renvoie_nom_fichier("ombres_texture_couleur_vertex.glsl"),renvoie_nom_fichier("ombres_texture_couleur_fragment.glsl"));
            if(ombres_texture_couleur->erreur!=SHADER_OK) return false;


            phong_ombres=new Shader_Ombres("phong_ombres",renvoie_nom_fichier("phong_ombres_vertex.glsl"),renvoie_nom_fichier("phong_ombres_fragment.glsl"));
            if(phong_ombres->erreur!=SHADER_OK) return false;

            ondes=new Shader_Ondes("ondes",renvoie_nom_fichier("ondes_vertex.glsl"),renvoie_nom_fichier("ondes_fragment.glsl"));
            if(ondes->erreur!=SHADER_OK) return false;


            reflexion_cube_ondes=new Shader_Reflexion_Cube_Ondes("reflexion_cube_ondes",renvoie_nom_fichier("reflexion_cube_ondes_vertex.glsl"),renvoie_nom_fichier("reflexion_cube_ondes_fragment.glsl"));
            if(reflexion_cube_ondes->erreur!=SHADER_OK) return false;


            transition_textures_couleur=new Shader_Transition_Textures_Couleur("transition_textures_couleur",renvoie_nom_fichier("transition_textures_couleur_vertex.glsl"),renvoie_nom_fichier("transition_textures_couleur_fragment.glsl"));
            if(transition_textures_couleur->erreur!=SHADER_OK) return false;

            transition_textures_couleur_axe=new Shader_Transition_Textures_Couleur_Axe("transition_textures_couleur_axe",renvoie_nom_fichier("transition_textures_couleur_axe_vertex.glsl"),renvoie_nom_fichier("transition_textures_couleur_axe_fragment.glsl"));
            if(transition_textures_couleur->erreur!=SHADER_OK) return false;

            floutage_x=new Shader_Floutage("floutage_x",NULL,renvoie_nom_fichier("flou_x.glsl"));
            if(floutage_x->erreur!=SHADER_OK) return false;
            floutage_y=new Shader_Floutage("floutage_y",NULL,renvoie_nom_fichier("flou_y.glsl"));
            if(floutage_y->erreur!=SHADER_OK) return false;

            decallage_sommets=new Shader_Decallage_Sommets("decallage_sommets",renvoie_nom_fichier("decallage_sommets_vertex.glsl"),renvoie_nom_fichier("decallage_sommets_fragment.glsl"));
            if(decallage_sommets->erreur!=SHADER_OK) return false;

            masque_2_textures=new Shader_Masque_2_Textures("masque_2_textures",renvoie_nom_fichier("masque_2_textures_vertex.glsl"),renvoie_nom_fichier("masque_2_textures_fragment.glsl"));
            if(masque_2_textures->erreur!=SHADER_OK) return false;

*/
            return true;
        }

    //=========================================================================
    //      Concaténation du chemin des sources avec les noms des fichiers
    //=========================================================================
        char* ShadersObjets3d::renvoie_nom_fichier(const char* nom_source)
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
        bool ShadersObjets3d::creation_textures()
        {
            //---------- Génère la texture de bruit:
            if (!Bruit::genere_bruit_de_Perlin_2D_8bits(4,4,SHADERS_TAILLE_TEXTURE_BRUIT,SHADERS_TAILLE_TEXTURE_BRUIT,12,0.75,texture_bruit))
            {
                cout<<"ERREUR dans Shaders::creation_textures() - Impossible de créer la texture de bruit"<<endl;
                erreur=SHADERS_ERREUR_TEXTURES;
                return false;
            }

            glGenTextures(1,&texture_bruit_id);

            glBindTexture(GL_TEXTURE_2D,texture_bruit_id);

            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);    //Il semble que ce paramètre active la texture.....
            glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,SHADERS_TAILLE_TEXTURE_BRUIT,SHADERS_TAILLE_TEXTURE_BRUIT,0,GL_LUMINANCE,GL_UNSIGNED_BYTE,texture_bruit);

            //glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);


            //--------- Erreur OpenGl:
            if (erreur_openGl("ERREUR dans Shaders::creation_textures() :"))
            {
                erreur=SHADERS_ERREUR_TEXTURES;
                return false;
            }
            else return true;
        }




