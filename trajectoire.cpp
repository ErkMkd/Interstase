
#include "Trajectoire.h"

        //=============================================================================================================
        //                      CONSTRUCTEUR
        //  drapeau_longueur à false si on ne veut pas se servir des longueurs des courbes (gourmand en temps machine)
        //=============================================================================================================

        Trajectoire::Trajectoire(const char* p_nom, float* p_points, uint32_t p_nbrPoints,float p_facteur_lissage,bool p_drapeau_longueurs, float p_precision_calcul_longueurs)
        {
            echelle=1.;
            facteur_lissage=p_facteur_lissage;
            drapeau_longueurs=p_drapeau_longueurs;
            precision_calcul_longueur=p_precision_calcul_longueurs;
            determine_points(p_points,p_nbrPoints);
            strcpy(nom,p_nom);
        }

        //Constructeur de copie:
        Trajectoire::Trajectoire(Trajectoire& trajectoire_org)
        {

            facteur_lissage=trajectoire_org.renvoie_facteur_lissage();
            drapeau_longueurs=trajectoire_org.renvoie_drapeau_longueurs();
            precision_calcul_longueur=trajectoire_org.renvoie_precision_calcul_longueur();
            determine_points(trajectoire_org.renvoie_points(),trajectoire_org.renvoie_nombre_points());
            echelle=1.;
            determine_echelle_trajectoire(trajectoire_org.renvoie_echelle_trajectoire());
        }

        //=========================== DESTRUCTEUR ========================================
        Trajectoire::~Trajectoire()
        {
            liste_tri_points.clear();
        }

        //================================================================================
        //                      Fonctions de calcul
        //================================================================================
        float Trajectoire::normef(float* vecteur)
        {
            return sqrtf(vecteur[0]*vecteur[0]+vecteur[1]*vecteur[1]+vecteur[2]*vecteur[2]);
        }

        void Trajectoire::normalisef(float* vecteur)
        {
            float norme=normef(vecteur);
            vecteur[0]/=norme;
            vecteur[1]/=norme;
            vecteur[2]/=norme;
        }

        //--------------------------------------------------------------------------------------
        //      Calcul le vecteur unitaire tangent à deux segments (au point p2)
        //--------------------------------------------------------------------------------------

        void Trajectoire::calcul_tangente_unitairef(float* p1, float* p2, float* p3, float* tangente)
        {
            float vecp1p2[3],vecp2p3[3];

            if (p3==NULL)
            {
                tangente[0]=p2[0]-p1[0];
                tangente[1]=p2[1]-p1[1];
                tangente[2]=p2[2]-p1[2];
                normalisef(tangente);
            }

            else if (p1==NULL)
            {
                tangente[0]=p3[0]-p2[0];
                tangente[1]=p3[1]-p2[1];
                tangente[2]=p3[2]-p2[2];
                normalisef(tangente);
            }

            else
            {

                vecp1p2[0]=p2[0]-p1[0];
                vecp1p2[1]=p2[1]-p1[1];
                vecp1p2[2]=p2[2]-p1[2];

                vecp2p3[0]=p3[0]-p2[0];
                vecp2p3[1]=p3[1]-p2[1];
                vecp2p3[2]=p3[2]-p2[2];

                normalisef(vecp1p2);
                normalisef(vecp2p3);

                tangente[0]=vecp1p2[0]+vecp2p3[0];
                tangente[1]=vecp1p2[1]+vecp2p3[1];
                tangente[2]=vecp1p2[2]+vecp2p3[2];

                normalisef(tangente);
            }
        }

        //--------------------------------------------------------------------------------------
        //  Calcul la position du point entre l'intervalle [p2,p3] à l'instant t, 0 <= t <= 1.
        //  La travjectoire est lissée.
        //---------------------------------------------------------------------------------------
        void Trajectoire::calcul_interpolation_spline(float* p1, float* p2, float* p3, float* p4, float t,float facteur, float* resultat)
        {
            float t1[3],t2[3];
            float vi1[3],vi2[3],vi3[3];
            float i1[3],i2[3],i3[3],i4[3],i5[3],i6[3],i7[3];
            float vecp2p3[3];

            vecp2p3[0]=p3[0]-p2[0];
            vecp2p3[1]=p3[1]-p2[1];
            vecp2p3[2]=p3[2]-p2[2];

            float norme=normef(vecp2p3)/2.*facteur;

            calcul_tangente_unitairef(p1,p2,p3,t1);
            calcul_tangente_unitairef(p2,p3,p4,t2);

            t1[0]*=norme;
            t1[1]*=norme;
            t1[2]*=norme;

            t2[0]*=norme;
            t2[1]*=norme;
            t2[2]*=norme;


            //Calcul des points de contrôle i1 et i2 et du vecteur vi=i1i2
            i1[0]=p2[0]+t1[0];
            i1[1]=p2[1]+t1[1];
            i1[2]=p2[2]+t1[2];

            i2[0]=p3[0]-t2[0];
            i2[1]=p3[1]-t2[1];
            i2[2]=p3[2]-t2[2];

            vi1[0]=i2[0]-i1[0];
            vi1[1]=i2[1]-i1[1];
            vi1[2]=i2[2]-i1[2];

            //Calcul des points intermédiaires
            //
            //  i3=p2+t*t1
            //  i4=i1+t*vi1
            //  i5=i2+t*t2
            i3[0]=p2[0]+t*t1[0];
            i3[1]=p2[1]+t*t1[1];
            i3[2]=p2[2]+t*t1[2];

            i4[0]=i1[0]+t*vi1[0];
            i4[1]=i1[1]+t*vi1[1];
            i4[2]=i1[2]+t*vi1[2];

            i5[0]=i2[0]+t*t2[0];
            i5[1]=i2[1]+t*t2[1];
            i5[2]=i2[2]+t*t2[2];

            //Calcul des vecteurs intermédiaires:

            vi2[0]=i4[0]-i3[0];
            vi2[1]=i4[1]-i3[1];
            vi2[2]=i4[2]-i3[2];

            vi3[0]=i5[0]-i4[0];
            vi3[1]=i5[1]-i4[1];
            vi3[2]=i5[2]-i4[2];

            //Calcul des deuxièmes points intermédiaires:
            //
            // i6=i3+t*vi2
            // i7=i4+t*vi3

            i6[0]=i3[0]+t*vi2[0];
            i6[1]=i3[1]+t*vi2[1];
            i6[2]=i3[2]+t*vi2[2];

            i7[0]=i4[0]+t*vi3[0];
            i7[1]=i4[1]+t*vi3[1];
            i7[2]=i4[2]+t*vi3[2];

            //Calcul du point cherché:
            //
            // p(t)=i6+t*(i7-i6)

            resultat[0]=i6[0]+t*(i7[0]-i6[0]);
            resultat[1]=i6[1]+t*(i7[1]-i6[1]);
            resultat[2]=i6[2]+t*(i7[2]-i6[2]);
        }

        //--------------------------------------------------------------------------------------
        //  Calcul la position du point entre l'intervalle [p2,p3] à l'instant t, 0 <= t <= 1.
        //  Renvoie également la tangente à cette position
        //  La travjectoire est lissée.
        //---------------------------------------------------------------------------------------
        void Trajectoire::calcul_interpolation_spline_tangente(float* p1, float* p2, float* p3, float* p4, float t,float facteur, float* position, float* tangente)
        {
            float t1[3],t2[3];
            float vi1[3],vi2[3],vi3[3];
            float i1[3],i2[3],i3[3],i4[3],i5[3],i6[3],i7[3];
            float vecp2p3[3];

            vecp2p3[0]=p3[0]-p2[0];
            vecp2p3[1]=p3[1]-p2[1];
            vecp2p3[2]=p3[2]-p2[2];

            float norme=normef(vecp2p3)/2.*facteur;

            calcul_tangente_unitairef(p1,p2,p3,t1);
            calcul_tangente_unitairef(p2,p3,p4,t2);

            t1[0]*=norme;
            t1[1]*=norme;
            t1[2]*=norme;

            t2[0]*=norme;
            t2[1]*=norme;
            t2[2]*=norme;


            //Calcul des points de contrôle i1 et i2 et du vecteur vi=i1i2
            i1[0]=p2[0]+t1[0];
            i1[1]=p2[1]+t1[1];
            i1[2]=p2[2]+t1[2];

            i2[0]=p3[0]-t2[0];
            i2[1]=p3[1]-t2[1];
            i2[2]=p3[2]-t2[2];

            vi1[0]=i2[0]-i1[0];
            vi1[1]=i2[1]-i1[1];
            vi1[2]=i2[2]-i1[2];

            //Calcul des points intermédiaires
            //
            //  i3=p2+t*t1
            //  i4=i1+t*vi1
            //  i5=i2+t*t2
            i3[0]=p2[0]+t*t1[0];
            i3[1]=p2[1]+t*t1[1];
            i3[2]=p2[2]+t*t1[2];

            i4[0]=i1[0]+t*vi1[0];
            i4[1]=i1[1]+t*vi1[1];
            i4[2]=i1[2]+t*vi1[2];

            i5[0]=i2[0]+t*t2[0];
            i5[1]=i2[1]+t*t2[1];
            i5[2]=i2[2]+t*t2[2];

            //Calcul des vecteurs intermédiaires:

            vi2[0]=i4[0]-i3[0];
            vi2[1]=i4[1]-i3[1];
            vi2[2]=i4[2]-i3[2];

            vi3[0]=i5[0]-i4[0];
            vi3[1]=i5[1]-i4[1];
            vi3[2]=i5[2]-i4[2];

            //Calcul des deuxièmes points intermédiaires:
            //
            // i6=i3+t*vi2
            // i7=i4+t*vi3

            i6[0]=i3[0]+t*vi2[0];
            i6[1]=i3[1]+t*vi2[1];
            i6[2]=i3[2]+t*vi2[2];

            i7[0]=i4[0]+t*vi3[0];
            i7[1]=i4[1]+t*vi3[1];
            i7[2]=i4[2]+t*vi3[2];

            //Calcul du point cherché:
            //
            // p(t)=i6+t*(i7-i6)

            position[0]=i6[0]+t*(i7[0]-i6[0]);
            position[1]=i6[1]+t*(i7[1]-i6[1]);
            position[2]=i6[2]+t*(i7[2]-i6[2]);

            //Calcul de la tangente:
            //On répète les opérations avec un t légèrement supérieur
            t+=0.001;

            i3[0]=p2[0]+t*t1[0];
            i3[1]=p2[1]+t*t1[1];
            i3[2]=p2[2]+t*t1[2];

            i4[0]=i1[0]+t*vi1[0];
            i4[1]=i1[1]+t*vi1[1];
            i4[2]=i1[2]+t*vi1[2];

            i5[0]=i2[0]+t*t2[0];
            i5[1]=i2[1]+t*t2[1];
            i5[2]=i2[2]+t*t2[2];

            vi2[0]=i4[0]-i3[0];
            vi2[1]=i4[1]-i3[1];
            vi2[2]=i4[2]-i3[2];

            vi3[0]=i5[0]-i4[0];
            vi3[1]=i5[1]-i4[1];
            vi3[2]=i5[2]-i4[2];

            i6[0]=i3[0]+t*vi2[0];
            i6[1]=i3[1]+t*vi2[1];
            i6[2]=i3[2]+t*vi2[2];

            i7[0]=i4[0]+t*vi3[0];
            i7[1]=i4[1]+t*vi3[1];
            i7[2]=i4[2]+t*vi3[2];

            tangente[0]=i6[0]+t*(i7[0]-i6[0])-position[0];
            tangente[1]=i6[1]+t*(i7[1]-i6[1])-position[1];
            tangente[2]=i6[2]+t*(i7[2]-i6[2])-position[2];

            norme=sqrtf(tangente[0]*tangente[0]+tangente[1]*tangente[1]+tangente[2]*tangente[2]);
            tangente[0]/=norme;
            tangente[1]/=norme;
            tangente[2]/=norme;


        }

        //--------------------------------------------------------------------------------------
        //  Calcul la longueur d'un intervalle
        //---------------------------------------------------------------------------------------
        float Trajectoire::calcul_longueur_intervalle(uint32_t point)
        {
            float position0[3],position1[3];
            float *pos0=position0,*pos1=position1;
            float* temp;
            float longueur=0.;
            float* p0;
            float* p3;
            uint32_t i;
            if (point+1>=nbrPoints) return 0.;
            pos0[0]=points[3*point];
            pos0[1]=points[3*point+1];
            pos0[2]=points[3*point+2];
            if (point==0)
            {
                p0=NULL;
                if (nbrPoints==2) p3=NULL;
                else p3=&points[6];
            }
            else
            {
                p0=&points[3*(point-1)];
                if (point==nbrPoints-2) p3=NULL;
                else p3=&points[(point+2)*3];
            }

            for (i=0;i<=precision_calcul_longueur;i++)
            {
                calcul_interpolation_spline(p0,&points[point*3],&points[(point+1)*3],p3,(float)i/precision_calcul_longueur,facteur_lissage,pos1);

                pos0[0]=pos1[0]-pos0[0];
                pos0[1]=pos1[1]-pos0[1];
                pos0[2]=pos1[2]-pos0[2];
                longueur+=sqrtf(pos0[0]*pos0[0]+pos0[1]*pos0[1]+pos0[2]*pos0[2]);
                temp=pos0;
                pos0=pos1;
                pos1=temp;
            }
            return longueur;
        }

        //------------------------------------------------
        //  Remet à jour les longueurs des intervalles
        //------------------------------------------------

        void Trajectoire::maj_longueurs_intervalles()
        {
            for (uint32_t i=0;i<nbrPoints;i++)  longueurs[i]=calcul_longueur_intervalle(i);
        }
        //---------------------------------
        //  Calcul la longueur totale
        //---------------------------------
        void Trajectoire::calcul_longueur_totale()
        {
            longueur_totale=0.;
            for (uint32_t i=0;i<nbrPoints;i++) longueur_totale+=longueurs[i];
        }

        //--------------------------------------------------------------------------------------
        //  Calcul la position du point  à l'intervalle spécifié, à l'instant t, 0 <= t <= 1.
        //  t=0 :départ intervalle,
        //  t=1 :fin intervalle
        //  La travjectoire est lissée.
        //---------------------------------------------------------------------------------------

        void Trajectoire::renvoie_position_intervalle(uint32_t intervalle,float t, float* point)
        {
            if (intervalle>=nbrPoints-1 || nbrPoints==1 ) {point[0]=0.; point[1]=0.; point[2]=0.;}

            else if (intervalle==0)
            {
                if (nbrPoints>=3)calcul_interpolation_spline(NULL,&points[0],&points[3],&points[6],t,facteur_lissage,point);
                else if (nbrPoints==2)calcul_interpolation_spline(NULL,&points[0],&points[3],NULL,t,facteur_lissage,point);
            }

            else if (intervalle==nbrPoints-2) calcul_interpolation_spline(&points[(intervalle-1)*3],&points[intervalle*3],&points[(intervalle+1)*3],NULL,t,facteur_lissage,point);
            else calcul_interpolation_spline(&points[(intervalle-1)*3],&points[intervalle*3],&points[(intervalle+1)*3],&points[(intervalle+2)*3],t,facteur_lissage,point);
        }

        //==============================================================================
        //                      Gestion de la trajectoire
        //==============================================================================

        void Trajectoire::determine_drapeau_longueurs(bool d)
        {
            if ((!drapeau_longueurs) && d)
            {
                maj_longueurs_intervalles();
                calcul_longueur_totale();
            }
            drapeau_longueurs=d;
        }
        bool Trajectoire::renvoie_drapeau_longueurs()
        {
            return drapeau_longueurs;
        }

        void Trajectoire::ajoute_point (float x, float y, float z)
        {
            points[nbrPoints*3]=x;
            points[nbrPoints*3+1]=y;
            points[nbrPoints*3+2]=z;
            nbrPoints++;
            if (nbrPoints==1)longueurs[0]=0.;
            else if (nbrPoints==2)longueurs[nbrPoints-1]=calcul_longueur_intervalle(nbrPoints-2);
            else if (nbrPoints>2)
            {
                longueurs[nbrPoints-3]=calcul_longueur_intervalle(nbrPoints-3);
                longueurs[nbrPoints-2]=calcul_longueur_intervalle(nbrPoints-2);
            }
            if (drapeau_longueurs) calcul_longueur_totale();
        }

        void Trajectoire::supprime_point(uint32_t i)
        {
            uint32_t j;
            if (i>=nbrPoints) return;
            nbrPoints--;
            for(j=i;j<nbrPoints;j++)
            {
                points[3*j]=points[3*(j+1)];
                points[3*j+1]=points[3*(j+1)+1];
                points[3*j+2]=points[3*(j+1)+2];
                longueurs[j]=longueurs[j+1];
            }
            //Recalcul les longueurs:
            if(i<nbrPoints-1)longueurs[i]=calcul_longueur_intervalle(i);
            if(i>0)longueurs[i-1]=calcul_longueur_intervalle(i-1);
            if(i>1)longueurs[i-2]=calcul_longueur_intervalle(i-2);
            if (drapeau_longueurs) calcul_longueur_totale();

        }

        void Trajectoire::renvoie_point(uint32_t i, float* point)
        {
            if (nbrPoints<=i) return;
            point[0]=points[3*i];
            point[1]=points[3*i+1];
            point[2]=points[3*i+2];
        }

        uint32_t Trajectoire::renvoie_nbrPoints()
        {
            return nbrPoints;
        }

        float Trajectoire::renvoie_longueur_intervalle(uint32_t point)
        {
            if (point<nbrPoints) return longueurs[point];
            else return 0.;
        }

        //--------------------------------------------
        //Détermine les points de la trajectoire:
        //--------------------------------------------
        void Trajectoire::determine_point(uint32_t i, float x, float y, float z)
        {
            if (nbrPoints<=i) return;
            points[3*i]=x;
            points[3*i+1]=y;
            points[3*i+2]=z;
            //Recalcul les longueurs:
            if(i<nbrPoints-2)longueurs[i+1]=calcul_longueur_intervalle(i+1);
            if(i<nbrPoints-1)longueurs[i]=calcul_longueur_intervalle(i);
            if(i>0)longueurs[i-1]=calcul_longueur_intervalle(i-1);
            if(i>1)longueurs[i-2]=calcul_longueur_intervalle(i-2);
            if (drapeau_longueurs) calcul_longueur_totale();

        }

        void Trajectoire::determine_point(uint32_t i, float* pt)
        {
            determine_point(i,pt[0],pt[1],pt[2]);

        }


        //----- Détermine les coordonnées des points:
        void Trajectoire::determine_points(float* p_points,uint32_t p_nbrPoints)
        {
            nbrPoints=p_nbrPoints;

            for (uint32_t i=0;i<nbrPoints;i++)
            {
                points[3*i]=p_points[3*i];
                points[3*i+1]=p_points[3*i+1];
                points[3*i+2]=p_points[3*i+2];
            }

            longueur_totale=0.;

            if (drapeau_longueurs)
            {
                maj_longueurs_intervalles();
                calcul_longueur_totale();
            }
        }

        void Trajectoire::determine_points(double* p_points, uint32_t p_nbrPoints)
        {
            nbrPoints=p_nbrPoints;

            for (uint32_t i=0;i<nbrPoints;i++)
            {
                points[3*i]=float(p_points[3*i]);
                points[3*i+1]=float(p_points[3*i+1]);
                points[3*i+2]=float(p_points[3*i+2]);
            }

            longueur_totale=0.;

            if (drapeau_longueurs)
            {
                maj_longueurs_intervalles();
                calcul_longueur_totale();
            }
        }

        //Fonction un peu particulière: les points en entrée sont enregistrés
        //dans l'ordre inverse.
        void Trajectoire::determine_points_inverses(float* p_points,uint32_t p_nbrPoints)
        {
            nbrPoints=p_nbrPoints;
            uint32_t j=0;
            for (uint32_t i=nbrPoints-1;i<nbrPoints;i--)
            {
                points[3*(j)]=p_points[3*i];
                points[3*(j)+1]=p_points[3*i+1];
                points[3*(j)+2]=p_points[3*i+2];
                j++;
            }

            longueur_totale=0.;

            if (drapeau_longueurs)
            {
                maj_longueurs_intervalles();
                calcul_longueur_totale();
            }
        }

        void Trajectoire::determine_points_inverses(double* p_points,uint32_t p_nbrPoints)
        {
            nbrPoints=p_nbrPoints;
            uint32_t j=0;
            for (uint32_t i=nbrPoints-1;i<nbrPoints;i--)
            {
                points[3*(j)]=float(p_points[3*i]);
                points[3*(j)+1]=float(p_points[3*i+1]);
                points[3*(j)+2]=float(p_points[3*i+2]);
                j++;
            }

            longueur_totale=0.;

            if (drapeau_longueurs)
            {
                maj_longueurs_intervalles();
                calcul_longueur_totale();
            }
        }

        float* Trajectoire::renvoie_points()
        {
            return points;
        }

        uint32_t Trajectoire::renvoie_nombre_points()
        {
            return nbrPoints;
        }

        void Trajectoire::deplace_point(uint32_t point,float dx,float dy,float dz)
        {
            if (point>=nbrPoints)return;
            points[3*point]+=dx;
            points[3*point+1]+=dy;
            points[3*point+2]+=dz;
            //Recalcul les longueurs:
            if (drapeau_longueurs)
            {
                if(point<nbrPoints-2)longueurs[point+1]=calcul_longueur_intervalle(point+1);
                if(point<nbrPoints-1)longueurs[point]=calcul_longueur_intervalle(point);
                if(point>0)longueurs[point-1]=calcul_longueur_intervalle(point-1);
                if(point>1)longueurs[point-2]=calcul_longueur_intervalle(point-2);
                calcul_longueur_totale();
            }

        }

        //Le point 0 de la trajectoire est positionné selon les coordonnées spécifiées,
        // et tout les autres points sont déplacés relativement à ce point 0.
        // Les distances ne changent pas.
        void Trajectoire::positionne_trajectoire_point_0(float px,float py,float pz)
        {
            float dx=px-points[0];
            float dy=py-points[1];
            float dz=pz-points[2];
            for (uint32_t i=0;i<nbrPoints;i++)
            {
                points[3*i]+=dx;
                points[3*i+1]+=dy;
                points[3*i+2]+=dz;
            }
        }

        void Trajectoire::positionne_trajectoire(float dx,float dy,float dz)
        {
            for (uint32_t i=0;i<nbrPoints;i++)
            {
                points[3*i]+=dx;
                points[3*i+1]+=dy;
                points[3*i+2]+=dz;
            }
        }



        //Rotation des points de la trajectoire d'angle "angle" et d'axe rx,ry,rz
        //Pas besoin de recalculer les longueurs...
        void Trajectoire::rotation_trajectoire(float angle,float* r)
        {
            for (uint32_t i=0;i<nbrPoints;i++)
            {
                float x=points[3*i];
                float y=points[3*i+1];
                float z=points[3*i+2];
                float prod_scal = r[0]*x+r[1]*y+r[2]*z;

                float cos_angle = cos(angle);
                float sin_angle = sin(angle);

                points[3*i] = cos_angle * x + sin_angle * (r[1] * z - r[2] * y) + (1. - cos_angle) * prod_scal * r[0];
                points[3*i+1] = cos_angle * y + sin_angle * (r[2] * x - r[0] * z) + (1. - cos_angle) * prod_scal * r[1];
                points[3*i+2] = cos_angle * z + sin_angle * (r[0] * y - r[1] * x) + (1. - cos_angle) * prod_scal * r[2];
            }
        }

        void Trajectoire::determine_echelle_trajectoire(float p_echelle)
        {
            float facteur=p_echelle/echelle;
            uint32_t i;
            echelle=p_echelle;
            if (drapeau_longueurs)
            {
               for (i=0;i<nbrPoints;i++)
                {
                    points[3*i]*=facteur;
                    points[3*i+1]*=facteur;
                    points[3*i+2]*=facteur;
                    longueurs[i]*=facteur;
                }
                longueur_totale*=facteur;
            }
            else
            {
                for (i=0;i<nbrPoints;i++)
                {
                    points[3*i]*=facteur;
                    points[3*i+1]*=facteur;
                    points[3*i+2]*=facteur;
                }
            }

        }

        float Trajectoire::renvoie_echelle_trajectoire()
        {
            return echelle;
        }


        void Trajectoire::affiche_position_point(uint32_t point)
        {
            if (point>=nbrPoints)return;
            cout<<"px: "<<points[3*point]<<" py: "<<points[3*point+1]<<" pz: "<<points[3*point+2]<<" Intervalle: "<<renvoie_longueur_intervalle(point)<<endl;
        }

        float Trajectoire::renvoie_facteur_lissage()
        {
            return facteur_lissage;
        }

        void Trajectoire::determine_facteur_lissage(float f)
        {
            facteur_lissage=f;
        }

        void Trajectoire::determine_precision_calcul_longueur(float p)
        {
            precision_calcul_longueur=p;
        }

        float Trajectoire::renvoie_precision_calcul_longueur()
        {
            return precision_calcul_longueur;
        }

        bool Trajectoire::sauve_trajectoire(char* nom_fichier)
        {
            return Fichiers::sauve_donnees_binaires((char*)points,nbrPoints*3*sizeof(float),nom_fichier);
        }

        bool Trajectoire::charge_trajectoire(char* nom_fichier)
        {
            int i;
            for(i=0;i<strlen(nom_fichier);i++)
            {
                nom[i]=nom_fichier[i];
            }
            nom[i]=0;
            int32_t taille=Fichiers::charge_donnees_binaires((char*)points,TRAJECTOIRE_NBR_POINTS_MAX*3*sizeof(float),nom_fichier);
            if (taille==-1) return false;
            nbrPoints=(uint32_t)taille/(3*sizeof(float));
            if (drapeau_longueurs)
            {
                maj_longueurs_intervalles();
                calcul_longueur_totale();
            }
            return true;
        }

        //------- Renvoie la longueur de la trajectoire ------------

        float Trajectoire::renvoie_longueur_totale()
        {
            return longueur_totale;
        }

        //---------------------------------------
        //  Tri les points le long de l'axe X,Y ou Z
        // Sens: false= descendant
        //       true= ascendant
        //---------------------------------------
        //Le test utilisé pour le tri des points:
		bool Trajectoire::comparaison_descendant(tri_point_trajectoire t1, tri_point_trajectoire t2) { return t1.valeur > t2.valeur; }
		bool Trajectoire::comparaison_ascendant(tri_point_trajectoire t1, tri_point_trajectoire t2) { return t1.valeur < t2.valeur; }

        void Trajectoire::tri_points(uint8_t p_axe,bool sens)
        {
            uint32_t axe=0;
            uint32_t i;
            liste_tri_points.clear();
            switch (p_axe)
            {
                case TRAJECTOIRE_AXE_X: axe=0; break;
                case TRAJECTOIRE_AXE_Y: axe=1; break;
                case TRAJECTOIRE_AXE_Z: axe=2; break;
            }

            //Copiage dans la liste de tri:
            for (i=0;i<nbrPoints;i++)
            {
                tri_point_trajectoire* pt=ajoute_tri_point();
                pt->valeur=points[i*3+axe];
                pt->x=points[3*i];
                pt->y=points[3*i+1];
                pt->z=points[3*i+2];
            }

            // Tri:
            if (!sens) std::sort(liste_tri_points.begin(),liste_tri_points.end(),comparaison_descendant);
            else std::sort(liste_tri_points.begin(),liste_tri_points.end(),comparaison_ascendant);

            // Copiage dans le tableau des points:
            for (i=0;i<nbrPoints;i++)
            {
                points[3*i]=liste_tri_points[i].x;
                points[3*i+1]=liste_tri_points[i].y;
                points[3*i+2]=liste_tri_points[i].z;
            }

            //Rafraichi les longueurs:
            if (drapeau_longueurs)
            {
                maj_longueurs_intervalles();
                calcul_longueur_totale();
            }

        }

        tri_point_trajectoire* Trajectoire::ajoute_tri_point()
        {
            tri_point_trajectoire pt;
            liste_tri_points.push_back(pt);
            return &liste_tri_points[liste_tri_points.size()-1];
        }

        //------------------------------------------------------------------------------------
        //      Renvoie une position sur la trajectoire
        //  t doit varier entre 0 et 1
        //  Cette fonction ne marche que si le calcul des longueurs de courbes est activé.
        //------------------------------------------------------------------------------------
        void Trajectoire::renvoie_position(float t, float* point)
        {
            uint32_t i;

            if (!drapeau_longueurs) { point[0]=0.;point[1]=0.;point[2]=0.; return; }

            if(longueur_totale==0)
            {
                if (nbrPoints==1) {point[0]=points[0];point[1]=points[1];point[2]=points[2];}
                else {point[0]=0.;point[1]=0.;point[2]=0.;}
                return;
            }

            if (t<0.) t=0.;
            if (t>=1.)
            {
                point[0]=points[3*(nbrPoints-1)];
                point[1]=points[3*(nbrPoints-1)+1];
                point[2]=points[3*(nbrPoints-1)+2];
                return;
            }

            float position=t*longueur_totale;
            float position_test=0;
            float *p0,*p3;


            //Recherche l'intervalle:

            for (i=0;i<nbrPoints;i++)
            {
                position_test+=longueurs[i];
                if (position_test>position) break;
            }

            //Dernier point:
            if (i==nbrPoints-1) {point[0]=points[3*i];point[1]=points[3*i+1];point[2]=points[3*i+2];return;}

            position=1.-((position_test-position)/longueurs[i]);
            if(i==0) p0=NULL; else p0=&points[3*(i-1)];
            if(i==nbrPoints-2)p3=NULL; else p3=&points[3*(i+2)];

            calcul_interpolation_spline(p0,&points[3*i],&points[3*(i+1)],p3,position,facteur_lissage,point);
        }

        //--------------------------------------------
        //      Renvoie la tangente en un point
        //--------------------------------------------
        void Trajectoire::renvoie_tangente(float t,float* tangente)
        {
            uint32_t i;
            tangente[0]=0.;
            tangente[1]=0.;
            tangente[2]=0.;
            if (!drapeau_longueurs || longueur_totale==0. ) return;

            if (t>1.) t=1.;
            if (t<0.) t=0.;

            float position=t*longueur_totale;
            float position_test=0;
            float *p0,*p3;


            //Recherche l'intervalle:

            for (i=0;i<nbrPoints;i++)
            {
                position_test+=longueurs[i];
                if (position_test>position) break;
            }

            //Dernier point:
            if (i==nbrPoints-1) return;

            position=1.-((position_test-position)/longueurs[i]);
            if(i==0) p0=NULL; else p0=&points[3*(i-1)];
            if(i==nbrPoints-2)p3=NULL; else p3=&points[3*(i+2)];

            float point[3];
            calcul_interpolation_spline_tangente(p0,&points[3*i],&points[3*(i+1)],p3,position,facteur_lissage,point,tangente);
        }

        //--------------------------------------------
        //      Renvoie la position et la tangente
        //--------------------------------------------
        void Trajectoire::renvoie_position_tangente(float t, float* point,float* tangente)
        {
            uint32_t i;
            tangente[0]=0.;
            tangente[1]=0.;
            tangente[2]=0.;
            if (!drapeau_longueurs) { point[0]=0.;point[1]=0.;point[2]=0.; return; }

            if(longueur_totale==0)
            {
                if (nbrPoints==1) {point[0]=points[0];point[1]=points[1];point[2]=points[2];}
                else {point[0]=0.;point[1]=0.;point[2]=0.;}
                return;
            }

            if (t<0.) t=0.;
            if (t>1.) {point[0]=points[3*(nbrPoints-1)];point[1]=points[3*(nbrPoints-1)+1];point[2]=points[3*(nbrPoints-1)+2];return;}

            float position=t*longueur_totale;
            float position_test=0;
            float *p0,*p3;


            //Recherche l'intervalle:

            for (i=0;i<nbrPoints;i++)
            {
                position_test+=longueurs[i];
                if (position_test>position) break;
            }

            //Dernier point:
            if (i==nbrPoints-1) {point[0]=points[3*i];point[1]=points[3*i+1];point[2]=points[3*i+2];return;}

            position=1.-((position_test-position)/longueurs[i]);
            if(i==0) p0=NULL; else p0=&points[3*(i-1)];
            if(i==nbrPoints-2)p3=NULL; else p3=&points[3*(i+2)];

            calcul_interpolation_spline_tangente(p0,&points[3*i],&points[3*(i+1)],p3,position,facteur_lissage,point,tangente);
        }

        //--------------------------------------------------------------------------------------------
        //      Affiche une trajectoire avec des lignes droites
        //--------------------------------------------------------------------------------------------

        void Trajectoire::affiche_trajectoire_lignesf( bool drapeau_lignes, bool drapeau_tangentes)
        {
            uint32_t i;

            if(drapeau_lignes)
            {
                glBegin(GL_LINE_STRIP);
                    glColor3f(0.,0.,1.);
                    for (i=0;i<nbrPoints;i++) glVertex3fv(&points[i*3]);
                glEnd();
            }


            if (drapeau_tangentes)
            {
                float tangente[3];
                glBegin(GL_LINES);
                glColor3f(0.,1.,1.);


                    calcul_tangente_unitairef(NULL,&points[0],&points[3],tangente);
                    tangente[0]+=points[0];
                    tangente[1]+=points[1];
                    tangente[2]+=points[2];
                    glVertex3fv(&points[0]);
                    glVertex3fv(tangente);


                    for (i=1;i<nbrPoints-1;i++)
                    {
                        calcul_tangente_unitairef(&points[3*(i-1)],&points[3*i],&points[3*(i+1)],tangente);
                        tangente[0]+=points[3*i];
                        tangente[1]+=points[3*i+1];;
                        tangente[2]+=points[3*i+2];;
                        glVertex3fv(&points[3*i]);
                        glVertex3fv(tangente);

                    }

                    calcul_tangente_unitairef(&points[3*(i-1)],&points[3*i],NULL,tangente);
                    tangente[0]+=points[3*i];
                    tangente[1]+=points[3*i+1];
                    tangente[2]+=points[3*i+2];
                    glVertex3fv(&points[3*i]);
                    glVertex3fv(tangente);


                glEnd();
            }
        }

        //--------------------------------------------------------------------------------------------
        //  Affiche une courbe:
        //  trajectoire: tableau de points
        //  nbr_points: le nombre de points;
        //  nbr_etapes: le nombre de segments à afficher entre les points (précision de la courbe)
        //--------------------------------------------------------------------------------------------

        void Trajectoire::affiche_courbe(uint32_t nbr_etapes,uint32_t couleur)
        {
            uint32_t i;
            uint32_t j;
            float position[3];
            uint8_t r=(couleur>>16)&0xff;
            uint8_t v=(couleur>>8)&0xff;
            uint8_t b=(couleur)&0xff;
            uint8_t a=(couleur>>24)&0xff;

            glColor4ub(r,v,b,a);

            if (nbrPoints<2) return;

            //--- Affiche le point de départ:
            glPointSize(6);
            glBegin(GL_POINTS);
                glVertex3fv(&points[0]);
            glEnd();

            //--- Deux points:

            if (nbrPoints==2)
            {
                glBegin(GL_LINE_STRIP);
                for (j=0 ;j<=nbr_etapes ; j++ )
                {
                   calcul_interpolation_spline(NULL,&points[0],&points[3],NULL,(float)j/nbr_etapes,facteur_lissage,position);
                   glVertex3fv(position);
                }
                glEnd();
                return;
            }

            //--- Plus de deux points:

            glBegin(GL_LINE_STRIP);
                for (j=0 ;j<=nbr_etapes ; j++ )
                {
                   calcul_interpolation_spline(NULL,&points[0],&points[3],&points[6],(float)j/nbr_etapes,facteur_lissage,position);
                   glVertex3fv(position);
                }
            glEnd();
            glBegin(GL_LINE_STRIP);
                for ( i=1 ; i<nbrPoints-2 ; i++ )
                {
                    for ( j=0 ;j<=nbr_etapes ; j++ )
                    {
                       calcul_interpolation_spline(&points[3*(i-1)],&points[3*i],&points[3*(i+1)],&points[3*(i+2)],(float)j/nbr_etapes,facteur_lissage,position);
                       glVertex3fv(position);
                    }
                }
            glEnd();
            glBegin(GL_LINE_STRIP);

                for (j=0 ;j<=nbr_etapes ; j++ )
                    {
                       calcul_interpolation_spline(&points[3*(nbrPoints-3)],&points[3*(nbrPoints-2)],&points[3*(nbrPoints-1)],NULL,(float)j/nbr_etapes,facteur_lissage,position);
                       glVertex3fv(position);
                    }
                glVertex3fv(&points[(nbrPoints-1)*3]);
            glEnd();
        }

        //--------------------------------------------------------------------------------------------
        //  Affiche les points
        //--------------------------------------------------------------------------------------------

        void Trajectoire::affiche_points(uint32_t couleur,float taille)
        {
            uint32_t i;
            uint8_t r=(couleur>>16)&0xff;
            uint8_t v=(couleur>>8)&0xff;
            uint8_t b=(couleur)&0xff;
            uint8_t a=(couleur>>24)&0xff;
            glColor4ub(r,v,b,a);
            glPointSize(taille);
            glBegin(GL_POINTS);

                for (i=0;i<nbrPoints;i++) glVertex3fv(&points[3*i]);
            glEnd();
        }

        void Trajectoire::affiche_point(uint32_t point, uint32_t couleur, float taille)
        {
            uint8_t r=(couleur>>16)&0xff;
            uint8_t v=(couleur>>8)&0xff;
            uint8_t b=(couleur)&0xff;
            uint8_t a=(couleur>>24)&0xff;

            if (point>=nbrPoints) return;
            glColor4ub(r,v,b,a);
            glPointSize(taille);
            glBegin(GL_POINTS);
                glVertex3fv(&points[3*point]);
            glEnd();
        }
