#ifndef COSMONAUTE_H_INCLUDED
#define COSMONAUTE_H_INCLUDED

#include "../glew.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <string>
#include "math.h"

#include "ChargeLWO2.h"
#include "ConvertionLWO2.h"
#include "../Lumieres.h"
#include "Volume.h"
#include "../Bruit.h"

#define COSMO_TRONC 0
#define COSMO_VISIERE 1
#define COSMO_TETE 2
#define COSMO_AVANTBRAS_G 3
#define COSMO_AVANTBRAS_D 4
#define COSMO_BRAS_G 5
#define COSMO_BRAS_D 6
#define COSMO_EPAULE_D 7
#define COSMO_EPAULE_G 8
#define COSMO_PIED_D 9
#define COSMO_PIED_G 10
#define COSMO_MOLET_D 11
#define COSMO_MOLET_G 12
#define COSMO_JAMBE_D 13
#define COSMO_JAMBE_G 14
#define COSMO_BASSIN 15
#define COSMO_MAIN_G 16
#define COSMO_MAIN_D 17
#define COSMO_OEIL_D 18
#define COSMO_OEIL_G 19

#define COSMO_NBR_OBJETS 20

#define COSMO_OK 0
#define COSMO_CHARGELWO 1
#define COSMO_CONVERTIONLWO 2

class Cosmonaute
{
    public:
        uint16_t erreur;

        float echelle;  //Pour régler la taille du cosmonaute

        float ab_tronc;
        float ab_tete;
        float ab_yeux;
        float ab_bras;
        float ab_jambes;

        boolean drapeau_affiche_visage; //False si on affiche pas le visage (visière opaque)

        Bruit* bruit_tronc_r;
        Bruit* bruit_tronc_t;
        Bruit* bruit_tronc_l;
        Bruit* bruit_tete_r;
        Bruit* bruit_tete_t;
        Bruit* bruit_tete_l;
        Bruit* bruit_yeux_t;
        Bruit* bruit_yeux_l;

        Bruit* bruit_epauleD_r;
        Bruit* bruit_epauleD_t;
        Bruit* bruit_epauleD_l;
        Bruit* bruit_epauleG_r;
        Bruit* bruit_epauleG_t;
        Bruit* bruit_epauleG_l;

        Bruit* bruit_avantBrasD_l;
        Bruit* bruit_avantBrasG_l;
        Bruit* bruit_mainD_r;
        Bruit* bruit_mainD_t;
        Bruit* bruit_mainD_l;
        Bruit* bruit_mainG_r;
        Bruit* bruit_mainG_t;
        Bruit* bruit_mainG_l;

        Bruit* bruit_jambeD_r;
        Bruit* bruit_jambeD_t;
        Bruit* bruit_jambeD_l;
        Bruit* bruit_moletD_t;
        Bruit* bruit_piedD_r;
        Bruit* bruit_piedD_t;
        Bruit* bruit_piedD_l;
        Bruit* bruit_jambeG_r;
        Bruit* bruit_jambeG_t;
        Bruit* bruit_jambeG_l;
        Bruit* bruit_moletG_t;
        Bruit* bruit_piedG_r;
        Bruit* bruit_piedG_t;
        Bruit* bruit_piedG_l;

    private:
        ChargeLWO2* objetLWO;
        ConvertionLWO2* volumesConv;
        Volume* volumes[COSMO_NBR_OBJETS];

    public:
        Cosmonaute(const char* nomFichierLWO);
        ~Cosmonaute();

        void init_listes();
        void determine_position(float px,float py,float pz);
        void determine_position(float pos[3]);
        void determine_RTL(float pr,float pt, float pl);
        void determine_RTL(float rtl[3]);
        void raz();
        void determine_echelle(float e);
        void rotation_membre(uint16_t membreId,float r, float t, float l);
        void rotation_membre(uint16_t membreId,float rtl[3]);
        void mouvement_bruits(double temps, float amplitude=1.);
        void debout();
        void mix_debout_mvt_bruits(double temps, float mix, float amplitude_bruits);
        void affiche(float obs[3]);
        Volume* renvoie_volume_parent();
        void renvoie_position(float* position);
        void renvoie_RTL(float* rtl);
    private:
        void eclairages();
        void RTL_limites(float rtl[3],float limites[6]);
};

#endif // COSMONAUTE_H_INCLUDED
