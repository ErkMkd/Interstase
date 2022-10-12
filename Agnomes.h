#ifndef AGNOMES_H_INCLUDED
#define AGNOMES_H_INCLUDED

#include "Sprites_2d.h"
#include "stdlib.h"

using namespace std;

#define AGNOMES_OK 0
#define AGNOMES_ERREUR_SPRITE 1

typedef struct
{
    float vitesse_oscillation;
    float amplitude_oscillation;
    float echelle;
    float xPos;
    float angle_oscillation;


} agnome;

#define NUM_AGNOMES 160
#define AGNOMES_AMPLITUDE_MIN 0.
#define AGNOMES_AMPLITUDE_MAX 4.
#define AGNOMES_VITESSE_MIN .2
#define AGNOMES_VITESSE_MAX .8
#define AGNOMES_DIST_MIN 3.5
#define AGNOMES_DIST_MAX 30.

class Agnomes
{
    private:
        Sprites_2d* sprite;
        float t_prec;
        agnome foule[NUM_AGNOMES];
    public:
        uint16_t erreur;
        int largeur_ecran;
        int hauteur_ecran;
        float position_horizon;
        float position_apparitions;
        float position_disparitions;
        float distance_fondu;
        float xpos_min;
        float xpos_max;

    private:
        //int compare(const void* a,const void* b);
        void random_agnomes();
    public:
        Agnomes(int l_ecran, int h_ecran,float p_horiz,uint32_t resolution_reference_x,uint32_t resolution_reference_y);
        ~Agnomes();
        void determine_resolution_reference(uint32_t rx,uint32_t ry);
        void affiche(float t);
};

#endif // AGNOMES_H_INCLUDED
