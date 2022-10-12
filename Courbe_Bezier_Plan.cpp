#include "Courbe_Bezier_Plan.h"


		Courbe_Bezier_Plan::Courbe_Bezier_Plan ( vec2d& P0, vec2d& P1, vec2d& P2, vec2d& P3 , double fact)
		{
		    precision=1e-6;
			facteur = fact;
			P0_x = P0.x;
			P0_y = P0.y;
			P1_x = P1.x;
			P1_y = P1.y;
			P2_x = P2.x;
			P2_y = P2.y;
			P3_x = P3.x;
			P3_y = P3.y;
			tangenteA_x = (P1_x - P0_x) * facteur + P0_x;
			tangenteA_y = (P1_y - P0_y) * facteur + P0_y;
			tangenteB_x = (P2_x - P3_x) * facteur + P3_x;
			tangenteB_y = (P2_y - P3_y) * facteur + P3_y;
			resolution();
		}

		Courbe_Bezier_Plan::Courbe_Bezier_Plan ( double P0x, double P0y, double P1x, double P1y, double P2x, double P2y, double P3x, double P3y, double fact)
		{
		    precision=1e-6;
			facteur = fact;
			P0_x = P0x;
			P0_y = P0y;
			P1_x = P1x;
			P1_y = P1y;
			P2_x = P2x;
			P2_y = P2y;
			P3_x = P3x;
			P3_y = P3y;
			tangenteA_x = (P1_x - P0_x) * facteur + P0_x;
			tangenteA_y = (P1_y - P0_y) * facteur + P0_y;
			tangenteB_x = (P2_x - P3_x) * facteur + P3_x;
			tangenteB_y = (P2_y - P3_y) * facteur + P3_y;
			resolution();
		}

		void Courbe_Bezier_Plan::determine_P0(vec2d& P)
		{
			P0_x = P.x;
			P0_y = P.y;
			tangenteA_x = (P1_x - P0_x) * facteur + P0_x;
			tangenteA_y = (P1_y - P0_y) * facteur + P0_y;
			resolution();
		}

		void Courbe_Bezier_Plan::determine_P0(double px, double py)
		{
			P0_x = px;
			P0_y = py;
			tangenteA_x = (P1_x - P0_x) * facteur + P0_x;
			tangenteA_y = (P1_y - P0_y) * facteur + P0_y;
			resolution();
		}

		void Courbe_Bezier_Plan::determine_P1(vec2d& P)
		{
			P1_x = P.x;
			P1_y = P.y;
			tangenteA_x = (P1_x - P0_x) * facteur + P0_x;
			tangenteA_y = (P1_y - P0_y) * facteur + P0_y;
			resolution();
		}

		void Courbe_Bezier_Plan::determine_P1(double px,double py)
		{
			P1_x = px;
			P1_y = py;
			tangenteA_x = (P1_x - P0_x) * facteur + P0_x;
			tangenteA_y = (P1_y - P0_y) * facteur + P0_y;
			resolution();
		}

		void Courbe_Bezier_Plan::determine_P2(vec2d& P)
		{
			P2_x = P.x;
			P2_y = P.y;
			tangenteB_x = (P2_x - P3_x) * facteur + P3_x;
			tangenteB_y = (P2_y - P3_y) * facteur + P3_y;
			resolution();
		}

		void Courbe_Bezier_Plan::determine_P2(double px,double py)
		{
			P2_x = px;
			P2_y = py;
			tangenteB_x = (P2_x - P3_x) * facteur + P3_x;
			tangenteB_y = (P2_y - P3_y) * facteur + P3_y;
			resolution();
		}

		void Courbe_Bezier_Plan::determine_P3(vec2d& P)
		{
			P3_x = P.x;
			P3_y = P.y;
			tangenteB_x = (P2_x - P3_x) * facteur + P3_x;
			tangenteB_y = (P2_y - P3_y) * facteur + P3_y;
			resolution();
		}

		void Courbe_Bezier_Plan::determine_P3(double px, double py)
		{
			P3_x = px;
			P3_y = py;
			tangenteB_x = (P2_x - P3_x) * facteur + P3_x;
			tangenteB_y = (P2_y - P3_y) * facteur + P3_y;
			resolution();
		}

		void Courbe_Bezier_Plan::determine_facteur(double f)
		{
			facteur = f;
			tangenteA_x = (P1_x - P0_x) * facteur + P0_x;
			tangenteA_y = (P1_y - P0_y) * facteur + P0_y;
			tangenteB_x = (P2_x - P3_x) * facteur + P3_x;
			tangenteB_y = (P2_y - P3_y) * facteur + P3_y;
			resolution();
		}

        void Courbe_Bezier_Plan::copie_parametres(Courbe_Bezier_Plan* courbe_org)
        {
            vec2d point;
            courbe_org->renvoie_P0(&point);
            determine_P0(point);
            courbe_org->renvoie_P1(&point);
            determine_P1(point);
            courbe_org->renvoie_P2(&point);
            determine_P2(point);
            courbe_org->renvoie_P3(&point);
            determine_P3(point);
        }

        void Courbe_Bezier_Plan::renvoie_P0(vec2d* point)
		{
		    point->x=P0_x;
		    point->y=P0_y;
		}

		void Courbe_Bezier_Plan::renvoie_P1(vec2d* point)
		{
		    point->x=P1_x;
		    point->y=P1_y;
		}

		void Courbe_Bezier_Plan::renvoie_P2(vec2d* point)
		{
		    point->x=P2_x;
		    point->y=P2_y;
		}

		void Courbe_Bezier_Plan::renvoie_P3(vec2d* point)
		{
		    point->x=P3_x;
		    point->y=P3_y;
		}

		double Courbe_Bezier_Plan::renvoie_facteur()
		{
			return facteur;
		}

		//Renvoie la postion d'un point de la courbe pour une valeur de t comprise entre 0 (point A) et 1 (point B)
        void Courbe_Bezier_Plan::renvoie_position(double t, vec2d *res)
		{
			if (t > 1) { t = 1; }
			else if (t < 0) { t = 0; }


			double t1 = 1 - t;
			double t12  = t1 * t1;
			double t13  = t12 * t1;

			double t2 = t * t;
			double t3 = t2 * t;

			res->x = P0_x * t13 + 3 * tangenteA_x * t * t12 + 3 * tangenteB_x * t2 * t1 + P3_x * t3;
			res->y = P0_y * t13 + 3 * tangenteA_y * t * t12 + 3 * tangenteB_y * t2 * t1 + P3_y * t3;

		}

		//Renvoie la coordonnée en y d'un point de la courbe pour une valeur de t comprise entre 0 (point A) et 1 (point B)
		double Courbe_Bezier_Plan::renvoie_ordonnee(double t)
		{
			//if (t > 1) { t = 1; }
			//else if (t < 0) { t = 0; }

			double t1 = 1 - t;
			double t12  = t1 * t1;
			double t13  = t12 * t1;

			double t2 = t * t;
			double t3 = t2 * t;

			return P0_y * t13 + 3 * tangenteA_y * t * t12 + 3 * tangenteB_y * t2 * t1 + P3_y * t3;
		}

		//Renvoie la coordonnée en x d'un point de la courbe pour une valeur de t comprise entre 0 (point A) et 1 (point B)
		double Courbe_Bezier_Plan::renvoie_abscisse(double t)
        {
			//if (t > 1) { t = 1; }
			//else if (t < 0) { t = 0; }

			double t1 = 1 - t;
			double t12  = t1 * t1;
			double t13  = t12 * t1;

			double t2 = t * t;
			double t3 = t2 * t;

			return P0_x * t13 + 3 * tangenteA_x * t * t12 + 3 * tangenteB_x * t2 * t1 + P3_x * t3;

		}

		//Renvoie l'ordonnée d'un point de la courbe pour une valeur de l'abscisse comprise entre P0x et P3x.
		//Il y a quelques restrictions:
		// -P0_x doit être inférieur à P3_x
		//Renvoie NaN si aucune solution.
		double Courbe_Bezier_Plan::renvoie_ordonnee_via_abscisse(double xt)
		{

			if (xt<P0_x || xt>P3_x) return 0;
			double t = 0.5;
			double div = 0.25;
			double t1 = renvoie_t1_retournement_x();
			double x_test;
			//Cas d'un retournement:
			if (t1 > 0)
			{

				double abscisse_t1 = renvoie_abscisse(t1);
				while (true) //for (var i:int = 0; i < 10;i++)
				{

					x_test = renvoie_abscisse(t);
					if ((t > t1) && (x_test<abscisse_t1))
					{
						x_test = abscisse_t1;
					}

					if (x_test == xt) break;
					if (x_test < xt)
					{
						if ((xt - x_test) < precision) break;
						else t += div;
					}
					else
					{
						if ((x_test-xt) < precision) break;
						else t -= div;
					}
					div /= 2.;
				}
			}
			//Pas de retournement:
			else
			{
				while (true) //for (var i:int = 0; i < 10;i++)
				{
					x_test = renvoie_abscisse(t);
					//if (t>0.00001)printf("xt:%f, x_test:%f, t:%f\n",xt,x_test,t);
					if (x_test == xt) break;
					if (x_test < xt)
					{
						if ((xt - x_test) < precision) break;
						else t += div;
					}
					else
					{
						if ((x_test-xt) < precision) break;
						else t -= div;
					}
					div /= 2.;
				}
			}

			return renvoie_ordonnee(t);
		}

		//Calcul les données qui servent pour résoudre y(x):
        void Courbe_Bezier_Plan::resolution()
		{
			alpha = -P0_x + 3 * tangenteA_x - 3 * tangenteB_x + P3_x;
			beta = 2 * P0_x - 4 * tangenteA_x + 2 * tangenteB_x;
			gamma = tangenteA_x - P0_x;
			delta = beta * beta - 4 * alpha * gamma;
		}

		//Si la courbe effectue un retour sur X, renvoie la valeur de t ou ce retour débute.
		//Sinon renvoie 0.
        double Courbe_Bezier_Plan::renvoie_t1_retournement_x()
		{
			if (delta > 0)
			{
				double t = ( -beta - sqrtf(delta)) / ( 2 * alpha);
				if (t<1 && t>0) return t;
				else return 0;
			}
			else return 0;
		}


		//Si la courbe effectue un retour sur X, renvoie la valeur de t ou ce retour débute.
		//Sinon renvoie 0.
		double Courbe_Bezier_Plan::renvoie_t2_retournement_x()
		{
			if (delta > 0)
			{
				double t = ( -beta + sqrtf(delta)) / ( 2 * alpha);
				if (t<1 && t>0) return t;
				else return 0;
			}
			else return 0;
		}


