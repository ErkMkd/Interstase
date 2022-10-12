#ifndef CONVERTIONLWO2_H_INCLUDED
#define CONVERTIONLWO2_H_INCLUDED

/*
*
*
*       Convertisseur des données LWO2 aux formats exploitables par le moteur
*       LES COORDONNEES EN X SONT INVERSEES !!!
*
*/

#include "Volume.h"
#include "ChargeLWO2.h"


//========= Erreurs:

#define CONVERTIONLWO2_OK 0
#define CONVERTIONLWO2_ERREUR_LWO2_INVALIDE 1
#define CONVERTIONLWO2_ERREUR_MEMOIRE 2
#define CONVERTIONLWO2_ERREUR_VOLUME 3

//========= Débuguage:
#define CONVERTIONLWO2_DEBUG false
#define CONVERTIONLWO2_DEBUG_SURFACES false
#define CONVERTIONLWO2_DEBUG_FACES false
#define CONVERTIONLWO2_DEBUG_REPARTITION_SURFACES false
#define CONVERTIONLWO2_DEBUG_DUREES false
#define CONVERTIONLWO2_DEBUG_TEXTURES false

//==========================================================================================================
//                                      Classe
//==========================================================================================================
class ConvertionLWO2
{
    public:

        uint16_t erreur;

    private:
        ChargeLWO2* donneesLWO2;
        vector<Surface*> surfaces;
        vector<Volume*> volumes;
        Volume** tableau_volumes_convertis;
        uint32_t nbrVolumes;
        double duree_totale;


    public:
        ConvertionLWO2(ChargeLWO2* p_donneesLWO2);
        ~ConvertionLWO2();
        uint32_t renvoie_nbrVolumes();
        Volume* renvoie_volume_index(uint32_t i);
        Volume* renvoie_volume_nom(char* p_nom);
        Volume** renvoie_volumes_convertis();
        void renvoie_pivot_nom(char* nom_volume,double* pivot);
        void renvoie_pivot(uint32_t indice,double pivot[3]);

    private:
        void calcul_pivot(LWO2_Calque* calque,float vPos[3]);
        bool creation_volumes();
        bool creation_sommets();
        bool creation_surfaces();
        void copie_parametres_surface(LWO2_Surface* surfaceLWO2, Surface* nouvelle_surface);
        bool creation_textures();
        SommetUV* ajoute_sommet_UV (Texture* textureDKS);
        bool creation_polygones();
        void libere_table_faces_en_construction(Face** table,uint32_t nbrPolygones);
        bool repartition_surfaces();
        Surface* renvoie_surface(char* nom_surface);
        char* copie_chaine(char* chaine);

};




#endif // CONVERTIONLWO2_H_INCLUDED
