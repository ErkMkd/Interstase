#ifndef INTERPOLATIONS_H_INCLUDED
#define INTERPOLATIONS_H_INCLUDED

#define INTERPOLATION_LINEAIRE 0
#define INTERPOLATION_COSINUS 1
#define INTERPOLATION_BEZIER 2
#define INTERPOLATION_INCONNUE 255

#include "glew.h"
#include <GLFW/glfw3.h>
#include "math.h"
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include "Courbe_Bezier_Plan.h"

using namespace std;

class Interpolations
{

    public:

        static float interpolation_lineaire_f(float a,float b,float t);
        static double interpolation_lineaire_d(double a,double b,double t);
        static void interpolation_lineaire_2f(float* a,float* b,float t,float* res);
        static void interpolation_lineaire_2d(double* a,double* b,double t,double* res);
        static void interpolation_lineaire_3f(float* a,float* b,float t,float* res);
        static void interpolation_lineaire_4f(float* a,float* b,float t,float* res);
        static void interpolation_lineaire_3d(double* a,double* b,double t,double* res);
        static float interpolation_cosinusoidale_f(float a,float b,float t);
        static double interpolation_cosinusoidale_d(double a,double b,double t);
        static void interpolation_cosinusoidale_2f(float* a,float* b,float t,float* res);
        static void interpolation_cosinusoidale_2d(double* a,double* b,double t,double* res);
        static void interpolation_cosinusoidale_3f(float* a,float* b,float t,float* res);
        static void interpolation_cosinusoidale_4f(float* a,float* b,float t,float* res);
        static void interpolation_cosinusoidale_3d(double* a,double* b,double t,double* res);
};

#define ENVELOPPE_INTERPOLATION_OK 0

class Enveloppe_Interpolation: public Interpolations
{
    public:
        uint16_t erreur;
        uint8_t type_interpolation;

    private:
        float* points;
        uint32_t nbrPoints;

    public:
        Enveloppe_Interpolation(float* p_points,uint32_t p_nbrPoints,uint8_t p_type=INTERPOLATION_LINEAIRE);
        ~Enveloppe_Interpolation();
        float renvoie_valeur(float t);
};

#define INTERPOLATION_PARAMETRE_OK 0
#define INTERPOLATION_PARAMETRE_ERREUR_MEMOIRE 1

class Interpolation_Parametre: public Interpolations
{
    public:
        uint16_t erreur;
        float t0;
        float valeur_depart;
        float valeur_fin;
        float duree;
        uint8_t type_interpolation;
        Courbe_Bezier_Plan* courbe_bezier;
        Enveloppe_Interpolation* enveloppe;
        bool drapeau_en_cours;

    public:

        Interpolation_Parametre(float p_t0,float p_depart,float p_fin,float p_duree,uint8_t p_type=INTERPOLATION_LINEAIRE,Courbe_Bezier_Plan* p_courbe=NULL,Enveloppe_Interpolation* p_enveloppe=NULL);
        ~Interpolation_Parametre();
        void determine_parametres(float p_t0,float p_depart,float p_fin,float p_duree,uint8_t p_type=INTERPOLATION_INCONNUE,Courbe_Bezier_Plan* p_courbe=NULL,Enveloppe_Interpolation* p_enveloppe=NULL);
        void determine_courbe_Bezier(Courbe_Bezier_Plan* p_courbe);
        void reset();
        float renvoie_valeur_parametre(float t);
};

#define SI_TYPE_TEMPS_DUREES 0  //Les indications temporelles sont des durées
#define SI_TYPE_TEMPS_REPERES 1 //Les indications temporelles sont des repères sur une ligne temporelle.
class Suite_Interpolations :public Interpolations
{
    public:
        float t0;
        float duree_totale;
        float* tableau_valeurs;
        uint8_t* tableau_types;
        Courbe_Bezier_Plan** tableau_courbes;
        uint16_t nbrEtapes;
        bool drapeau_en_cours;
        bool drapeau_bouclage;

    public:
        Suite_Interpolations(float p_t0,float* p_valeurs, uint8_t* p_types,uint16_t p_nbrEtapes,bool p_bouclage=false,uint8_t type_temps=SI_TYPE_TEMPS_DUREES,Courbe_Bezier_Plan** p_courbes=NULL);
        ~Suite_Interpolations();
        void inverse();
        void maj_duree_totale();
        void determine_t0(float p_t0);
        float renvoie_valeur(float t);
};

class Suite_Interpolations_3f :public Interpolations
{
    public:
        float t0;
        float duree_totale;
        float* tableau_valeurs;
        uint8_t* tableau_types;
        Courbe_Bezier_Plan** tableau_courbes;
        uint16_t nbrEtapes;
        bool drapeau_en_cours;
        bool drapeau_bouclage;

    public:
        Suite_Interpolations_3f(float p_t0,float* p_valeurs, uint8_t* p_types,uint16_t p_nbrEtapes,bool p_bouclage=false,uint8_t type_temps=SI_TYPE_TEMPS_DUREES,Courbe_Bezier_Plan** p_courbes=NULL);
        ~Suite_Interpolations_3f();
        void maj_duree_totale();
        void determine_t0(float p_t0);
        void renvoie_valeurs(float t,float* resultat );
};



class Interpolation_Parametre_2f: public Interpolations
{
    public:
        uint16_t erreur;
        float t0;
        float valeur_depart[2];
        float valeur_fin[2];
        float duree;
        bool drapeau_en_cours;
        uint8_t type_interpolation;
        Courbe_Bezier_Plan* courbe_bezier;

    public:

        Interpolation_Parametre_2f(float p_t0,float* p_depart,float* p_fin,float p_duree,uint8_t p_type=INTERPOLATION_LINEAIRE,Courbe_Bezier_Plan* p_courbe=NULL);
        ~Interpolation_Parametre_2f();
        void determine_parametres(float p_t0,float* p_depart,float* p_fin,float p_duree,uint8_t p_type=INTERPOLATION_INCONNUE,Courbe_Bezier_Plan* p_courbe=NULL);
        void renvoie_valeur_parametre(float t, float* resultat);
};


class Interpolation_Parametre_2d: public Interpolations
{
    public:
        uint16_t erreur;
        double t0;
        double valeur_depart[2];
        double valeur_fin[2];
        double duree;
        bool drapeau_en_cours;
        uint8_t type_interpolation;
        Courbe_Bezier_Plan* courbe_bezier;

    public:

        Interpolation_Parametre_2d(double p_t0,double* p_depart,double* p_fin,double p_duree,uint8_t p_type=INTERPOLATION_LINEAIRE,Courbe_Bezier_Plan* p_courbe=NULL);
        ~Interpolation_Parametre_2d();
        void determine_parametres(double p_t0,double* p_depart,double* p_fin,double p_duree,uint8_t p_type,Courbe_Bezier_Plan* p_courbe=NULL);
        void renvoie_valeur_parametre(double t, double* resultat);
};



class Interpolation_Parametre_3f: public Interpolations
{
    public:
        uint16_t erreur;
        float t0;
        float valeur_depart[3];
        float valeur_fin[3];
        float duree;
        bool drapeau_en_cours;
        uint8_t type_interpolation;
        Courbe_Bezier_Plan* courbe_bezier;

    public:

        Interpolation_Parametre_3f(float p_t0,float* p_depart,float* p_fin,float p_duree,uint8_t p_type=INTERPOLATION_LINEAIRE,Courbe_Bezier_Plan* p_courbe=NULL);
        ~Interpolation_Parametre_3f();
        void determine_parametres(float p_t0,float* p_depart,float* p_fin,float p_duree,uint8_t p_type,Courbe_Bezier_Plan* p_courbe=NULL);
        void determine_valeurs(float* p_depart, float* p_fin);
        void determine_valeurs_depart(float* p_depart);
        void determine_valeurs_fin(float* p_fin);
        void determine_courbe_Bezier(Courbe_Bezier_Plan* p_courbe);
        void renvoie_valeur_parametre(float t, float* resultat);
};

class Interpolation_Parametre_multi: public Interpolations
{
    public:
        uint16_t erreur;
        float t0;
        float valeur_depart[256];
        float valeur_fin[256];
        float duree;
        uint16_t num_parametres;
        bool drapeau_en_cours;
        uint8_t type_interpolation;
        Courbe_Bezier_Plan* courbe_bezier;

    public:

        Interpolation_Parametre_multi(float p_t0,float* p_depart,float* p_fin,uint16_t numParams,float p_duree,uint8_t p_type=INTERPOLATION_LINEAIRE,Courbe_Bezier_Plan* p_courbe=NULL);
        ~Interpolation_Parametre_multi();
        void determine_parametres(float p_t0,float* p_depart,float* p_fin,float p_duree,uint8_t p_type,Courbe_Bezier_Plan* p_courbe=NULL);
        void determine_valeurs(float* p_depart, float* p_fin);
        void determine_valeurs_depart(float* p_depart);
        void determine_valeurs_fin(float* p_fin);
        void determine_courbe_Bezier(Courbe_Bezier_Plan* p_courbe);
        void renvoie_valeur_parametre(float t, float* resultat);
};


class Interpolation_Parametre_3d: public Interpolations
{
    public:
        uint16_t erreur;
        double t0;
        double valeur_depart[3];
        double valeur_fin[3];
        double duree;
        bool drapeau_en_cours;
        uint8_t type_interpolation;
        Courbe_Bezier_Plan* courbe_bezier;

    public:

        Interpolation_Parametre_3d(double p_t0,double* p_depart,double* p_fin,double p_duree,uint8_t p_type=INTERPOLATION_LINEAIRE,Courbe_Bezier_Plan* p_courbe=NULL);
        ~Interpolation_Parametre_3d();
        void determine_parametres(double p_t0,double* p_depart,double* p_fin,double p_duree,uint8_t p_type,Courbe_Bezier_Plan* p_courbe=NULL);
        void renvoie_valeur_parametre(double t, double* resultat);

};

class Interpolation_Parametre_rvba: public Interpolations
{
    public:
        uint16_t erreur;
        float t0;
        uint32_t valeur_depart;
        uint32_t valeur_fin;
        float duree;
        bool drapeau_en_cours;
        uint8_t type_interpolation;
        Courbe_Bezier_Plan* courbe_bezier;
        float rvba_depart[4];
        float rvba_fin[4];

    private:

    public:

        Interpolation_Parametre_rvba(float p_t0,uint32_t p_depart,uint32_t p_fin,float p_duree,uint8_t p_type=INTERPOLATION_LINEAIRE,Courbe_Bezier_Plan* p_courbe=NULL);
        ~Interpolation_Parametre_rvba();
        void determine_parametres(float p_t0,uint32_t p_depart,uint32_t p_fin,float p_duree,uint8_t p_type,Courbe_Bezier_Plan* p_courbe=NULL);
        uint32_t renvoie_valeur_parametre(float t);
};

#endif // INTERPOLATIONS_H_INCLUDED
