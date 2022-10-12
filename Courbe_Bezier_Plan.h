#ifndef COURBE_BEZIER_PLAN_H_INCLUDED
#define COURBE_BEZIER_PLAN_H_INCLUDED

#include "Vec2d.h"
#include "math.h"
#include <iostream>

class Courbe_Bezier_Plan
	{

    public:
        double precision; //Précision pour la recherche par dicotomie

    private:
		double P0_x;
		double P0_y;
		double P1_x;
		double P1_y;
		double P2_x;
		double P2_y;
		double P3_x;
		double P3_y;
		double tangenteA_x;
		double tangenteA_y;
		double tangenteB_x;
		double tangenteB_y;
		double facteur;
        double alpha;
		double beta;
		double gamma;
		double delta;

    public:
		Courbe_Bezier_Plan ( vec2d& P0, vec2d& P1, vec2d& P2, vec2d& P3 , double fact);
		Courbe_Bezier_Plan ( double P0x=0., double P0y=0., double P1x=1., double P1y=0., double P2x=0., double P2y=1., double P3x=1., double P3y=1., double fact=1.);
		void determine_P0(vec2d& P);
		void determine_P0(double px, double py);
		void determine_P1(vec2d& P);
		void determine_P1(double px,double py);
		void determine_P2(vec2d& P);
		void determine_P2(double px,double py);
		void determine_P3(vec2d& P);
		void determine_P3(double px, double py);
		void determine_facteur(double f);
        void copie_parametres(Courbe_Bezier_Plan* courbe_org);
        void renvoie_P0(vec2d* point);
		void renvoie_P1(vec2d* point);
		void renvoie_P2(vec2d* point);
		void renvoie_P3(vec2d* point);
		double renvoie_facteur();
        void renvoie_position(double t, vec2d *res);
		double renvoie_ordonnee(double t);
		double renvoie_abscisse(double t);
		double renvoie_ordonnee_via_abscisse(double xt);

    private:
        void resolution();
        double renvoie_t1_retournement_x();
		double renvoie_t2_retournement_x();
	};


#endif // COURBE_BEZIER_PLAN_H_INCLUDED
