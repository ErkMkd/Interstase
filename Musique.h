#ifndef MUSIQUE_H_INCLUDED
#define MUSIQUE_H_INCLUDED

#include "bass/bass.h"
#include <iostream>


class Musique
{
    public:
        static HSTREAM musique_id;
        static float tempon_FFT[2048];

        static bool init_musique(const char *fichier_musique);
        static void detruit_musique();
        static void musique_joue(bool repete=false);
        static void musique_stop();
        static void musique_determine_volume(float v); //Entre 0 et 1
        static void musique_renvoie_FFT();

};

#endif // MUSIQUE_H_INCLUDED
