#ifndef TRAJECTOIRE_H_INCLUDED
#define TRAJECTOIRE_H_INCLUDED

#include "glew.h"
#include <GLFW/glfw3.h>
#include "math.h"
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include "gestion_fichiers.h"
#define TRAJECTOIRE_NBR_POINTS_MAX 1000

//Constantes utilisées pour le tri:
#define TRAJECTOIRE_AXE_X 0
#define TRAJECTOIRE_AXE_Y 1
#define TRAJECTOIRE_AXE_Z 2

class tri_point_trajectoire
{
    public:
        float valeur;     //Coordonnées X, Y, ou Z selon l'axe de tri choisi
        float x;
        float y;
        float z;
};

class Trajectoire
{
    private:
        bool drapeau_longueurs;
        float points[3*TRAJECTOIRE_NBR_POINTS_MAX]; //L'espace pour le nombre max de points est réservé :)
        vector<tri_point_trajectoire> liste_tri_points;    //Indices des points triés
        float longueurs[TRAJECTOIRE_NBR_POINTS_MAX];
        uint32_t nbrPoints;
        float facteur_lissage;
        float longueur_totale;
        float echelle;
        float precision_calcul_longueur; //Les segments de courbe sont divisés en ce nombre de segments droits, pour un calcul approchant des longueurs des courbes

        static void calcul_interpolation_spline(float* p1, float* p2, float* p3, float* p4, float t,float facteur, float* resultat);
        static void calcul_interpolation_spline_tangente(float* p1, float* p2, float* p3, float* p4, float t,float facteur, float* position, float* tangente);
        float calcul_longueur_intervalle(uint32_t point);
        void maj_longueurs_intervalles();
        void calcul_longueur_totale();
    public:
        char nom[256];

    public:
        Trajectoire(const char* p_nom="Trajectoire",float* p_points=NULL, uint32_t p_nbrPoints=0,float p_facteur_lissage=.75,bool p_drapeau_longueurs=true, float p_precision_calcul_longueurs=1000. );
        Trajectoire(Trajectoire& trajectoire_org);
        ~Trajectoire();
        static float normef(float* vecteur);
        static void normalisef(float* vecteur);
        static void calcul_tangente_unitairef(float* p1, float* p2, float* p3, float* tangente);
        void renvoie_position_intervalle(uint32_t intervalle,float t, float* point);
        void determine_drapeau_longueurs(bool d);
        bool renvoie_drapeau_longueurs();
        void ajoute_point (float x, float y, float z);
        void supprime_point(uint32_t i);
        void renvoie_point(uint32_t i, float* point);
        uint32_t renvoie_nbrPoints();
        float renvoie_longueur_intervalle(uint32_t point);
        void determine_point(uint32_t i, float x, float y, float z);
        void determine_point(uint32_t i, float* pt);
        void determine_points(float* p_points,uint32_t p_nbrPoints);
        void determine_points(double* p_points, uint32_t p_nbrPoints);
        void determine_points_inverses(float* p_points,uint32_t p_nbrPoints);
        void determine_points_inverses(double* p_points,uint32_t p_nbrPoints);
        float* renvoie_points();
        uint32_t renvoie_nombre_points();
        void deplace_point(uint32_t point,float dx,float dy,float dz);
        void positionne_trajectoire_point_0(float px,float py,float pz);
        void positionne_trajectoire(float dx,float dy,float dz);
        void rotation_trajectoire(float angle,float* r);
        void determine_echelle_trajectoire(float p_echelle);
        float renvoie_echelle_trajectoire();
        void affiche_position_point(uint32_t point);
        float renvoie_facteur_lissage();
        void determine_facteur_lissage(float f);
        void determine_precision_calcul_longueur(float p);
        float renvoie_precision_calcul_longueur();
        bool sauve_trajectoire(char* nom_fichier);
        bool charge_trajectoire(char* nom_fichier);
        float renvoie_longueur_totale();
		static bool comparaison_descendant(tri_point_trajectoire t1, tri_point_trajectoire t2);
		static bool comparaison_ascendant(tri_point_trajectoire t1, tri_point_trajectoire t2);
        void tri_points(uint8_t p_axe,bool sens=true);
        tri_point_trajectoire* ajoute_tri_point();
        void renvoie_position(float t, float* point);
        void renvoie_tangente(float t,float* tangente);
        void renvoie_position_tangente(float t, float* point,float* tangente);
        void affiche_trajectoire_lignesf( bool drapeau_lignes, bool drapeau_tangentes=false);
        void affiche_courbe(uint32_t nbr_etapes,uint32_t couleur=0xffffff00);
        void affiche_points(uint32_t couleur=0xffff0000,float taille=6);
        void affiche_point(uint32_t point, uint32_t couleur=0xffff0000, float taille=6);

};

#endif // TRAJECTOIRE_H_INCLUDED
