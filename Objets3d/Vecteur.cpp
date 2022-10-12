

#include "Vecteur.h"

using namespace std;

		// Constructeur
		Vecteur::Vecteur ( float px, float py, float pz )
		{
			x = px;
			y = py;
			z = pz;
		}


        // L'axe est normalisé à l'unité
        //L'angle est en radians
        void Vecteur::rotation(float* pt, float angle,float* axe, float* resultat)
		{
			/*
			formule d'Olinde Rodrigues :
			V = cos(phi)*U + (1-cos(phi))*((U.N)*N) + sin(phi)[N^U]
			La formule ci-dessus donne l'expression vectorielle du transformé V  d'un vecteur U  quelconque, dans la rotation phi,N d'angle phi, et d'axe N unitaire
			*/

			float prod_scal = axe[0]*pt[0]+axe[1]*pt[1]+axe[2]*pt[2];

			float cos_angle = cos(angle);
			float sin_angle = sin(angle);

			resultat[0] = cos_angle * pt[0] + sin_angle * (axe[1] * pt[2] - axe[2]* pt[1]) + (1. - cos_angle) * prod_scal * axe[0];
			resultat[1] = cos_angle * pt[1] + sin_angle * (axe[2] * pt[0] - axe[0] * pt[2]) + (1. - cos_angle) * prod_scal * axe[1];
			resultat[2] = cos_angle * pt[2] + sin_angle * (axe[0] * pt[1] - axe[1] * pt[0]) + (1. - cos_angle) * prod_scal * axe[2];
		}


        void Vecteur::inverse()
        {
            x=-x;y=-y;z=-z;
        }

        void Vecteur::determine(float px,float py,float pz)
        {
            x=px;y=py;z=pz;
        }

        bool Vecteur::est_nul()
        {
            if (x==0 && y==0 && z==0) return true;
            else return false;
        }

        void Vecteur::copie(Vecteur* source)
        {
            x=source->x;
            y=source->y;
            z=source->z;
        }

		float Vecteur::norme() { return sqrtf(x * x + y * y + z * z); }

		float Vecteur::norme_carre() { return x * x + y * y + z * z; }

        void Vecteur::unitaire()
        {
            float n=sqrtf(x * x + y * y + z * z);
            x/=n;
            y/=n;
            z/=n;
        }

		float Vecteur::produit_scalaire( Vecteur* v1, Vecteur* v2 )
		{
			return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
		}

		void Vecteur::produit_vectoriel(Vecteur* v1, Vecteur* v2)
		{
			x = v1->y * v2->z - v2->y * v1->z;
			y = v1->z * v2->x - v2->z * v1->x;
			z = v1->x * v2->y - v2->x * v1->y;
		}

		void Vecteur::produit_reel(float reel)
		{
			x *= reel;
			y *= reel;
			z *= reel;
		}

		void Vecteur::produit_vecteur_reel(Vecteur* vecteur,float reel)
		{
			x = vecteur->x*reel;
			y = vecteur->y*reel;
			z = vecteur->z*reel;
		}

		void Vecteur::vecteur_nul()
		{
			x = 0; y = 0; z = 0;
		}

		// resultat est le projeté orthogonale de px,py,pz sur le plan (ABC)
		void Vecteur::projection_orthogonale(float px, float py, float pz, Vecteur* A, Vecteur* B, Vecteur* C, Vecteur* resultat)
		{
			// intermédiaires de calcul
			float theta = A->x - B->x;
			float phi = A->x - C->x;

			float alpha = B->y - A->y + theta;
			float beta = B->z - A->z + theta;
			float gamma = C->y - A->y + phi;
			float delta = C->z - A->z + phi;

			float temp = alpha * delta - gamma * beta;

			// n : vecteur normal au plan (ABC)
			float nz = ( alpha * phi - gamma * theta ) / temp;
			float ny = ( delta * theta - beta * phi) / temp;
			float nx = 1 - ny - nz;

			// equation du plan (ABC) : nx * x + ny * y + nz * z + d = 0 or A € (ABC) donc :
			float d = -nx * A->x - ny * A->y - nz * A->z;

			float k = ( -nx * px - ny * py - nz * pz - d) / (nx * nx + ny * ny + nz * nz);

			resultat->x = px + k * nx;
			resultat->y = py + k * ny;
			resultat->z = pz + k * nz;
		}

		float Vecteur::distance_point_plan(float px, float py, float pz,Vecteur* A,Vecteur* B,Vecteur* C)
		{
			// intermédiaires de calcul
			float theta = A->x - B->x;
			float phi = A->x - C->x;

			float alpha = B->y - A->y + theta;
			float beta = B->z - A->z + theta;
			float gamma = C->y - A->y + phi;
			float delta = C->z - A->z + phi;

			float temp = alpha * delta - gamma * beta;

			// n : vecteur normal au plan (ABC)
			float nz = ( alpha * phi - gamma * theta ) / temp;
			float ny = ( delta * theta - beta * phi) / temp;
			float nx = 1 - ny - nz;

			// equation du plan (ABC) : nx * x + ny * y + nz * z + d = 0 or A € (ABC) donc :
			float d = -nx * A->x - ny * A->y - nz * A->z;

			return fabs( -nx * px - ny * py - nz * pz - d) / (nx * nx + ny * ny + nz * nz);
		}

		// calcul l'intersection des droites (AB) et (CD), sans vérification de la réalité de l'intersection trouvée
		//Fonction utilisée notamment par l'interface d'édition de scènes.
		void Vecteur::intersection_certaine_de_droites(Vecteur* A, Vecteur* B, Vecteur* C, Vecteur* D, Vecteur* res)
		{
			float dir_AB_x = B->x - A->x;
			float dir_AB_y = B->y - A->y;
			float dir_AB_z = B->z - A->z;

			float dir_CD_x = D->x - C->x;
			float dir_CD_y = D->y - C->y;

			float tempy = A->y - C->y;
			float tempx = A->x - C->x;

			if ((dir_AB_x == 0) && (dir_AB_y == 0))
			{
				float facteur2;

				if (dir_CD_x == 0) { facteur2 = tempy / dir_CD_y; }
				else { facteur2 = tempx / dir_CD_x; }

				res->x = C->x + facteur2 * dir_CD_x;
				res->y = C->y + facteur2 * dir_CD_y;
				res->z = C->z + facteur2 * (D->z - C->z);

				return;
			}

			float temp = dir_AB_x * dir_CD_y - dir_AB_y * dir_CD_x;
			float facteur = (tempy * dir_CD_x - tempx * dir_CD_y) / temp ;

			res->x = A->x +  dir_AB_x * facteur;
			res->y = A->y +  dir_AB_y * facteur;
			res->z = A->z +  dir_AB_z * facteur;
		}


        // L'axe doit être un vecteur unitaire.
        //L'angle est en radians
        void Vecteur::rotation(float angle, Vecteur* axe)
		{
			/*
			formule d'Olinde Rodrigues :
			V = cos(phi)*U + (1-cos(phi))*((U.N)*N) + sin(phi)[N^U]
			La formule ci-dessus donne l'expression vectorielle du transformé V  d'un vecteur U  quelconque, dans la rotation phi,N d'angle phi, et d'axe N unitaire
			*/

			float prod_scal = produit_scalaire(axe,this);

			float cos_angle = cosf(angle);
			float sin_angle = sinf(angle);

			float xt = cos_angle * x + sin_angle * (axe->y * z - axe->z * y) + (1. - cos_angle) * prod_scal * axe->x;
			float yt = cos_angle * y + sin_angle * (axe->z * x - axe->x * z) + (1. - cos_angle) * prod_scal * axe->y;
			z = cos_angle * z + sin_angle * (axe->x * y - axe->y * x) + (1. - cos_angle) * prod_scal * axe->z;
			x = xt;
			y = yt;
		}

		// L'axe est normalisé à l'unité
        //L'angle est en radians
        void Vecteur::rotation(float angle,float rx,float ry,float rz)
		{
			/*
			formule d'Olinde Rodrigues :
			V = cos(phi)*U + (1-cos(phi))*((U.N)*N) + sin(phi)[N^U]
			La formule ci-dessus donne l'expression vectorielle du transformé V  d'un vecteur U  quelconque, dans la rotation phi,N d'angle phi, et d'axe N unitaire
			*/

			float prod_scal = rx*x+ry*y+rz*z;

			float cos_angle = cos(angle);
			float sin_angle = sin(angle);

			float xt = cos_angle * x + sin_angle * (ry * z - rz * y) + (1. - cos_angle) * prod_scal * rx;
			float yt = cos_angle * y + sin_angle * (rz * x - rx * z) + (1. - cos_angle) * prod_scal * ry;
			z = cos_angle * z + sin_angle * (rx * y - ry * x) + (1. - cos_angle) * prod_scal * rz;
			x = xt;
			y = yt;
		}

        //---------------------------------------------------------------
        //Rotation en fonction des angles de Roulis, Tangage et Lacet:
        //  Les angles sont en radians.
        //---------------------------------------------------------------
		void Vecteur::rotation_RTL(float roulis,float tangage, float lacet)
        {
            float px=x;
            float py=y;
            float pz=z;
            float sin_aR=sin(roulis);
            float cos_aR=cos(roulis);
            float sin_aT=sin(tangage);
            float cos_aT=cos(tangage);
            float sin_aL=sin(lacet);
            float cos_aL=cos(lacet);

            x= sin_aL * ( pz*cos_aT - sin_aT * ( px*sin_aR + py*cos_aR ) ) +  cos_aL * ( px*cos_aR - py*sin_aR );
            y= pz * sin_aT + cos_aT * ( px*sin_aR + py*cos_aR );
            z= cos_aL * ( pz*cos_aT - sin_aT * ( px*sin_aR + py*cos_aR ) ) - sin_aL * ( px*cos_aR - py*sin_aR );
        }

        //Renvoie le paramètre correspondant à l'intersection entre la droite (A,B) et plan(O,U,V)
        //La normale N doit être unitaire.
        //Renvoie Nan si la droite est parallèle au plan.
        float Vecteur::intersection_droite_plan(Vecteur* segment_A, Vecteur* segment_B, Vecteur* plan_O, Vecteur* plan_U, Vecteur* plan_V, Vecteur* N)
        {
            Vecteur AB( segment_B->x - segment_A->x , segment_B->y - segment_A->y , segment_B->z - segment_A->z );
            Vecteur OU( plan_U->x - plan_O->x , plan_U->y - plan_O->y , plan_U->z - plan_O->z );
            Vecteur OV( plan_V->x - plan_O->x , plan_V->y - plan_O->y , plan_V->z - plan_O->z);

            //Calcul la distance:
            float D= - N->x*plan_O->x - N->y*plan_O->y - N->z*plan_O->z;

            //Test la parallèléité:
            float temp=AB.x*N->x+AB.y*N->y+AB.z*N->z;
            if (temp > -VECTEUR_EPSILON && temp < VECTEUR_EPSILON) return NAN;

            //Calcul le facteur:
            //if (N->x<0.) return -(N->x*segment_A->x+N->y*segment_A->y+N->z*segment_A->z+D)/temp;
            //else
            return -(N->x*segment_A->x+N->y*segment_A->y+N->z*segment_A->z+D)/temp;
        }

        /*
		On compte le nombre d'intersection avec la demi droite positive des U
		Test dans l'espace. Le point doit être dans le plan du polygone.
		*/
		bool Vecteur::inclus_dans_polygone(vector<uint32_t>* tabAretes, vector<float>* tabSommets,uint32_t nbr_sommets,
                                        Vecteur* normale,
                                        float x, float y, float z )
		{
			uint32_t nbrInter = 0;
            uint32_t U,V;
            float Up,Vp;

			//Détermine le plan de projection.
			//La composante la plus grande de la normale détermine la dimension ignorée:
			float nx=fabs(normale->x);
			float ny=fabs(normale->y);
			float nz=fabs(normale->z);
			if ((nx > ny) && (nx > nz)) { U=1;V=2;Up=y;Vp=z; }
			else if ((ny > nx) && (ny > nz)) { U=2;V=0;Up=z;Vp=x; }
			else { U=0;V=1;Up=x;Vp=y; }

			float xpA = (*tabSommets)[(*tabAretes)[0]*3+U] - Up;
			float ypA = (*tabSommets)[(*tabAretes)[0]*3+V] - Vp;

			if (xpA == 0 && ypA == 0) {return true;} //Si le point est sur un sommet, inclusion positive.
			else
			{
				if (xpA == 0) { xpA = 1e-4; }
				if (ypA == 0) { ypA = 1e-4; }
			}


			for (uint32_t i = 1; i <= nbr_sommets;i++)
			{
				//Origine du polygone sur le point testé
				float xpB = (*tabSommets)[(*tabAretes)[i]*3+U] - Up;
				float ypB = (*tabSommets)[(*tabAretes)[i]*3+V] - Vp;
				//Si Y des deux sommets de même signes, pas d'intersection:
				if (xpB == 0 && ypB == 0)
				{
					xpB = 1e-4;
					ypB = 1e-4;
				}
				else
				{
					if (xpB == 0) { xpB = 1e-4; }
					if (ypB == 0) { ypB = 1e-4; }
				}


				if (((ypA < 0) && (ypB > 0)) || ((ypA > 0) && (ypB < 0)))
				{
					if ((xpA > 0) && (xpB > 0)) { nbrInter++; }
					else if (xpA - ypA / (ypB - ypA) * (xpB - xpA) > 0) { nbrInter++;}
				}

				xpA = xpB;
				ypA = ypB;
			}

			if (nbrInter & 0x1) { return true; }
			else return false;

		}


		void Vecteur::toString()
		{
			cout<<"Vecteur: x="<<x<<" y="<<y<<" z="<<z<<endl;
		}
