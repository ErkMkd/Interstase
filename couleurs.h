#ifndef COULEURS_H_INCLUDED
#define COULEURS_H_INCLUDED

#include <stdlib.h>
#include <iostream>

using namespace std;

class Couleurs
{
        public:
            static void couleur_degrade_rvba(uint8_t indice_depart, float* couleur_depart, uint8_t indice_fin, float* couleur_fin,float* tableau_couleurs);
            static void couleur_degrade_rvb(uint8_t indice_depart, float* couleur_depart, uint8_t indice_fin, float* couleur_fin,float* tableau_couleurs);
            static void couleur_degrade_rvba_i(uint32_t indice_depart, uint32_t couleur_depart, uint32_t indice_fin, uint32_t couleur_fin,uint32_t* tableau_couleurs );
            static void couleur_gradients_rvba_i(uint32_t numCouleurs,uint16_t numEtapes,uint32_t* gradients, uint32_t* tableau_couleurs);
};

#endif // COULEURS_H_INCLUDED
