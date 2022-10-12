#ifndef BRUIT_H_INCLUDED
#define BRUIT_H_INCLUDED


#include "glew.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include "math.h"

using namespace std;

//=========================================================
//          Générateur de bruits.
//=========================================================

class Bruit
{
    public:
        char nom[64];       //Pour le débugage
        double borne_0;     //Pratique pour reprendre les bornes d'un bruit aléatoire temporel avec une courbe d'interpolation différente...
        double borne_1;

    private:
        uint32_t position_trame_aleatoire_precedente;

        double t0;

    public:
        Bruit(const char* p_nom=NULL);
        void determine_nom(const char* p_nom);
        static float interpolation_lineaire_f(float a,float b,float t);
        static double interpolation_lineaire_d(double a,double b,double t);
        static void interpolation_lineaire_2f(float* a,float* b,float t,float* res);
        static void interpolation_lineaire_2d(double* a,double* b,double t,double* res);
        static void interpolation_lineaire_3f(float* a,float* b,float t,float* res);
        static void interpolation_lineaire_3d(double* a,double* b,double t,double* res);
        static float interpolation_cosinusoidale_f(float a,float b,float t,float durete=1.);
        static double interpolation_cosinusoidale_d(double a,double b,double t,double durete=1.);
        static void interpolation_cosinusoidale_2f(float* a,float* b,float t,float* res);
        static void interpolation_cosinusoidale_2d(double* a,double* b,double t,double* res);
        static void interpolation_cosinusoidale_3f(float* a,float* b,float t,float* res);
        static void interpolation_cosinusoidale_3d(double* a,double* b,double t,double* res);

        static void genere_bruit_f(uint32_t largeur, uint32_t hauteur, float intensite, float* tempon);
        static void genere_bruit_d(uint32_t largeur, uint32_t hauteur, double intensite, double* tempon);
        static void genere_bruit_2D_pur_f(uint32_t largeur_tempon,uint32_t hauteur_tempon, float *tempon);
        static void genere_bruit_2D_pur_d(uint32_t largeur_tempon,uint32_t hauteur_tempon, double *tempon);
        static void genere_bruit_2D_sinusoide_f(uint32_t largeur_tempon,uint32_t hauteur_tempon,  float amplitude, float frequence, float *tempon);
        static void genere_bruit_2D_lignes_f(uint32_t largeur_tempon,uint32_t hauteur_tempon, float frequence, float *tempon);
        static void genere_bruit_2D_lignes_perturbation_f(uint32_t largeur_tempon,uint32_t hauteur_tempon, float frequence, float perturbation, float* tempon_perturbation, float *tempon_resultat);
        static void superpose_tempons_f (uint32_t largeur, uint32_t hauteur, float* tempon_A, float intensite_A, float* tempon_B, float intensite_B, float *tempon_resultat);
        static void genere_bruit_de_Perlin_2D_f(uint32_t largeur_fondamentale, uint32_t hauteur_fondamentale,uint32_t largeur_tempon,uint32_t hauteur_tempon, uint32_t nbr_octaves, float persistance,float* tempon);
        static void genere_bruit_de_Perlin_2D_d(uint32_t largeur_fondamentale, uint32_t hauteur_fondamentale,uint32_t largeur_tempon,uint32_t hauteur_tempon, uint32_t nbr_octaves, double persistance,double* tempon);
        static bool genere_bruit_de_Perlin_2D_8bits(uint32_t largeur_fondamentale, uint32_t hauteur_fondamentale,uint32_t largeur_tempon,uint32_t hauteur_tempon, uint32_t nbr_octaves, float persistance,uint8_t* tempon);
        double valeur_aleatoire_temporelle(double t, double intervalle,double durete=1.);
        double valeur_aleatoire_temporelle(double t, double intervalle,float borne0,float borne1,double durete=1.);
        double valeur_suite_temporelle(double t, double intervalle,double* suite,uint32_t taille);
        void reset_suite();

    private:
        static void interpolation_octave_f(uint32_t largeur_fondamentale, uint32_t hauteur_fondamentale,uint32_t largeur_tempon,uint32_t hauteur_tempon,float persistance,uint32_t octave_i,vector<float> &valeurs_octave, float *tempon);
        static void interpolation_octave_d(uint32_t largeur_fondamentale, uint32_t hauteur_fondamentale,uint32_t largeur_tempon,uint32_t hauteur_tempon,double persistance,uint32_t octave_i,vector<double> &valeurs_octave, double *tempon);

};




#endif // BRUIT_H_INCLUDED
