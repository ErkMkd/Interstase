#ifndef VECTEUR_H_INCLUDED
#define VECTEUR_H_INCLUDED

#include "../glew.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include "math.h"

using namespace std;
#define VECTEUR_EPSILON 1e-8

class Vecteur
{

    public:

        float x;
        float y;
        float z;

        static float produit_scalaire( Vecteur* v1, Vecteur* v2 );
        static void projection_orthogonale(float px, float py, float pz, Vecteur* A, Vecteur* B, Vecteur* C, Vecteur* resultat);
		static float distance_point_plan(float px, float py, float pz,Vecteur* A,Vecteur* B,Vecteur* C);
		static void intersection_certaine_de_droites(Vecteur* A, Vecteur* B, Vecteur* C, Vecteur* D, Vecteur* res);
		static bool inclus_dans_polygone(vector<uint32_t>* tabAretes, vector<float>* tabSommets,uint32_t nbr_sommets,
                                        Vecteur* normale,
                                        float x, float y, float z );
        static float intersection_droite_plan(Vecteur* segment_A, Vecteur* segment_B, Vecteur* plan_O, Vecteur* plan_U, Vecteur* plan_V, Vecteur* N);

        Vecteur ( float px = 1., float py = 0., float pz = 0. );
        void rotation(float* pt, float angle,float* axe, float* resultat);
        void inverse();
        void determine(float px,float py,float pz);
        bool est_nul();
		float norme();
		float norme_carre();
        void unitaire();
		void produit_reel(float reel);
		void produit_vectoriel(Vecteur* v1, Vecteur* v2);
		void produit_vecteur_reel(Vecteur* vecteur,float reel);
		void vecteur_nul();
        void rotation(float angle, Vecteur* axe);
        void rotation(float angle,float rx,float ry,float rz);
		void rotation_RTL(float roulis,float tangage, float lacet);

		virtual void copie(Vecteur* source);
		virtual void toString();
};



#endif // VECTEUR_H_INCLUDED
