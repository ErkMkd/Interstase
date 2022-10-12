#ifndef GESTION_FICHIERS_H_INCLUDED
#define GESTION_FICHIERS_H_INCLUDED

#define GESTION_FICHIERS 1

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>

 using namespace std;

class Fichiers
{
    public:

    static uint64_t retournement_uint64(uint64_t donnee);
    static uint32_t retournement_uint32(uint32_t donnee);
    static uint16_t retournement_uint16(uint16_t donnee);
    static float retournement_float(float donnee);
    static char* lecture_chaine(FILE* fichier);
    static char* copie_chaine(char* chaine_source, uint32_t taille_max=1024);
    static void recupere_nom_fichier(char* nom_complet, char* nom_fichier);
    static void recupere_nom_fichier(char* nom_complet);
    static int32_t charge_donnees_binaires(char* tempon,uint32_t taille_tempon, char* nom_fichier);
    static uint32_t renvoie_taille_fichier(char* nom_fichier);
    static char* charge_donnees_binaires_allocation(char* nom_fichier);
    static bool sauve_donnees_binaires(char* donnees,uint32_t taille, char* nom_fichier);

};


#endif // GESTION_FICHIERS_H_INCLUDED
