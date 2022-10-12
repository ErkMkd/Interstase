
#include "Bruit.h"

using namespace std;

//=========================================================
//          Générateur de bruits.
//=========================================================


    //=============================================================================================
    //                               Constructeur:
    //=============================================================================================
        Bruit::Bruit(const char* p_nom)
        {
            position_trame_aleatoire_precedente=0;
            borne_0=(double)rand()/32768.;
            borne_1=(double)rand()/32768.;
            reset_suite();
            determine_nom(p_nom);
        }


        void Bruit::determine_nom(const char* p_nom)
        {
            uint32_t i,taille;
            if (p_nom==NULL)nom[0]=0;
            else
            {
                taille=strlen(p_nom);
                if (taille>63) taille=63;
                for (i=0;i<taille;i++)
                {
                    nom[i]=p_nom[i];
                }
                nom[i]=0;
            }
        }

    //-----------------------------------------------------
    //  Interpolation linéaire entre deux valeurs a et b
    //  t varie entre 0 et 1 (0=a, 1=b)
    // renvoie la valeur intermédiaire.
    //-----------------------------------------------------
        float Bruit::interpolation_lineaire_f(float a,float b,float t)
        {
            return a*(1-t)+b*t;
        }
        double Bruit::interpolation_lineaire_d(double a,double b,double t)
        {
            return a*(1-t)+b*t;
        }

        void Bruit::interpolation_lineaire_2f(float* a,float* b,float t,float* res)
        {
            res[0]= a[0]*(1-t)+b[0]*t;
            res[1]= a[1]*(1-t)+b[1]*t;
        }

        void Bruit::interpolation_lineaire_2d(double* a,double* b,double t,double* res)
        {
            res[0]= a[0]*(1-t)+b[0]*t;
            res[1]= a[1]*(1-t)+b[1]*t;
        }


        void Bruit::interpolation_lineaire_3f(float* a,float* b,float t,float* res)
        {
            res[0]= a[0]*(1-t)+b[0]*t;
            res[1]= a[1]*(1-t)+b[1]*t;
            res[2]= a[2]*(1-t)+b[2]*t;
        }

        void Bruit::interpolation_lineaire_3d(double* a,double* b,double t,double* res)
        {
            res[0]= a[0]*(1-t)+b[0]*t;
            res[1]= a[1]*(1-t)+b[1]*t;
            res[2]= a[2]*(1-t)+b[2]*t;
        }



    //-----------------------------------------------------
    //  Interpolation sinusoïdale entre deux valeurs a et b
    //  x varie entre 0 et 1 (0=a, 1=b)
    // renvoie la valeur intermédiaire.
    //-----------------------------------------------------
        float Bruit::interpolation_cosinusoidale_f(float a,float b,float t,float durete)
        {
            return interpolation_lineaire_f(a,b,(-cosf(M_PI*pow(t,durete))+1.)/2.);
        }

        double Bruit::interpolation_cosinusoidale_d(double a,double b,double t,double durete)
        {
            if(durete>0.)return interpolation_lineaire_d(a,b,(-cos(M_PI*pow(t,durete))+1.)/2.);
            if(durete<0.)return interpolation_lineaire_d(a,b,(cos(M_PI*pow(1.-t,-durete))+1.)/2.);
            return interpolation_lineaire_f(a,b,(-cosf(M_PI*t)+1.)/2.);
        }

        void Bruit::interpolation_cosinusoidale_2f(float* a,float* b,float t,float* res)
        {
            interpolation_lineaire_2f(a,b,(-cosf(M_PI*t)+1.)/2.,res);
        }

        void Bruit::interpolation_cosinusoidale_2d(double* a,double* b,double t,double* res)
        {
            interpolation_lineaire_2d(a,b,(-cos(M_PI*t)+1.)/2.,res);
        }


        void Bruit::interpolation_cosinusoidale_3f(float* a,float* b,float t,float* res)
        {
            interpolation_lineaire_3f(a,b,(-cosf(M_PI*t)+1.)/2.,res);
        }

        void Bruit::interpolation_cosinusoidale_3d(double* a,double* b,double t,double* res)
        {
            interpolation_lineaire_3d(a,b,(-cos(M_PI*t)+1.)/2.,res);
        }




    //=============================================================================================
    //             Génère un bruit de base variant de 0 à 1 dans un tempon en 2 dimensions.
    //=============================================================================================
        void Bruit::genere_bruit_f(uint32_t largeur, uint32_t hauteur, float intensite, float* tempon)

        {
            uint32_t x,y;

            if (intensite>1.)intensite=1.;

            for (y=0;y<hauteur;y++)
            {
                for(x=0;x<largeur;x++)
                {
                    tempon[x+y*largeur]=intensite*float(rand())/32768.;
                }
            }
        }

        void Bruit::genere_bruit_d(uint32_t largeur, uint32_t hauteur, double intensite, double* tempon)

        {
            uint32_t x,y;

            if (intensite>1.)intensite=1.;

            for (y=0;y<hauteur;y++)
            {
                for(x=0;x<largeur;x++)
                {
                    tempon[x+y*largeur]=intensite*double(rand())/32768.;
                }
            }
        }

    //===================================================================
    //   Génère un tableau de pixels en nuances de gris
    //   avec un bruit de Perlin
    //===================================================================

    //------------------------------------------------------
    //      Génère une couche de bruit de perlin
    //------------------------------------------------------

        void Bruit::interpolation_octave_f(uint32_t largeur_fondamentale, uint32_t hauteur_fondamentale,uint32_t largeur_tempon,uint32_t hauteur_tempon,float persistance,uint32_t octave_i,vector<float> &valeurs_octave, float *tempon)
        {
            uint32_t x,y;
            uint32_t xf,yf,xfc,yfc; //Position sur les valeurs aléatoires
            float xi,yi;    //Position dans l'intervalle entre deux valeurs aléatoires (valeurs comprises entre 0 et 1 )
            uint32_t largeur_octave=largeur_fondamentale*(octave_i+1);
            uint32_t hauteur_octave=hauteur_fondamentale*(octave_i+1);

            //----- Génère les valeurs intermédiaires pour arriver au total demandé:
            for (y=0;y<hauteur_tempon;y++)
            {
                yi=(y*(hauteur_octave)/(float)hauteur_tempon);
                yf=(uint32_t)yi;
                yi-=yf;
                yfc=(yf+1)%hauteur_octave;

                for (x=0;x<largeur_tempon;x++)
                {
                    xi=(x*(largeur_octave)/(float)largeur_tempon);
                    xf=(uint32_t)xi;
                    xi-=xf;
                    xfc=(xf+1)%largeur_octave;

                    float a=valeurs_octave[xf+yf*largeur_octave];
                    float b=valeurs_octave[xfc+yf*largeur_octave];
                    float c=valeurs_octave[xf+yfc*largeur_octave];
                    float d=valeurs_octave[xfc+yfc*largeur_octave];

                    float i_ab=interpolation_cosinusoidale_f(a,b,xi);
                    float i_cd=interpolation_cosinusoidale_f(c,d,xi);

                    float intensite=interpolation_cosinusoidale_f(i_ab,i_cd,yi);

                    tempon[x+y*largeur_tempon]+=intensite*powf(persistance,(float)octave_i);
                }
            }

        }

        void Bruit::interpolation_octave_d(uint32_t largeur_fondamentale, uint32_t hauteur_fondamentale,uint32_t largeur_tempon,uint32_t hauteur_tempon,double persistance,uint32_t octave_i,vector<double> &valeurs_octave, double *tempon)
        {
            uint32_t x,y;
            uint32_t xf,yf,xfc,yfc; //Position sur les valeurs aléatoires
            double xi,yi;    //Position dans l'intervalle entre deux valeurs aléatoires (valeurs comprises entre 0 et 1 )
            uint32_t largeur_octave=largeur_fondamentale*(octave_i+1);
            uint32_t hauteur_octave=hauteur_fondamentale*(octave_i+1);

            //----- Génère les valeurs intermédiaires pour arriver au total demandé:
            for (y=0;y<hauteur_tempon;y++)
            {
                yi=(y*(hauteur_octave)/(double)hauteur_tempon);
                yf=(uint32_t)yi;
                yi-=yf;
                yfc=(yf+1)%hauteur_octave;

                for (x=0;x<largeur_tempon;x++)
                {
                    xi=(x*(largeur_octave)/(double)largeur_tempon);
                    xf=(uint32_t)xi;
                    xi-=xf;
                    xfc=(xf+1)%largeur_octave;

                    double a=valeurs_octave[xf+yf*largeur_octave];
                    double b=valeurs_octave[xfc+yf*largeur_octave];
                    double c=valeurs_octave[xf+yfc*largeur_octave];
                    double d=valeurs_octave[xfc+yfc*largeur_octave];

                    double i_ab=interpolation_cosinusoidale_d(a,b,xi);
                    double i_cd=interpolation_cosinusoidale_d(c,d,xi);

                    double intensite=interpolation_cosinusoidale_d(i_ab,i_cd,yi);

                    tempon[x+y*largeur_tempon]+=intensite*pow(persistance,(double)octave_i);
                }
            }

        }

    //============================================================================
    //      Génère des valeurs comprises entre 0 et 1 avec la fonction f(x,y)=x
    //      et rempli un tempon en 2 dimensions
    //============================================================================

        void Bruit::genere_bruit_2D_pur_f(uint32_t largeur_tempon,uint32_t hauteur_tempon, float *tempon)
        {
            uint32_t x,y;
            float valeur;

            for(x=0;x<largeur_tempon;x++)
            {
                valeur=(float)x/largeur_tempon;
                for (y=0;y<hauteur_tempon;y++)
                tempon[x+y*largeur_tempon]=valeur;
            }

        }

        void Bruit::genere_bruit_2D_pur_d(uint32_t largeur_tempon,uint32_t hauteur_tempon, double *tempon)
        {
            uint32_t x,y;
            double valeur;

            for(x=0;x<largeur_tempon;x++)
            {
                valeur=(double)x/largeur_tempon;
                for (y=0;y<hauteur_tempon;y++)
                tempon[x+y*largeur_tempon]=valeur;
            }

        }

    //=====================================================================================================
    //      Génère des valeurs comprises entre 0 et 1 avec la fonction f(x,y)=x+y**amplitude*sin(frequence)
    //      et rempli un tempon en 2 dimensions
    //=====================================================================================================

        void Bruit::genere_bruit_2D_sinusoide_f(uint32_t largeur_tempon,uint32_t hauteur_tempon,  float amplitude, float frequence, float *tempon)
        {
            uint32_t x,y;
            float valeur;

            for(y=0;y<hauteur_tempon;y++)
            {
                for (x=0;x<largeur_tempon;x++)
                {
                    valeur=((float)x/largeur_tempon)+amplitude*sin(((float)y/hauteur_tempon)*2*M_PI*frequence);
                    if (valeur<0.)valeur=0.;
                    if (valeur>1.)valeur=1.;
                    tempon[x+y*largeur_tempon]=valeur;
                }
            }

        }

    //=====================================================================================================
    //      Génère des valeurs comprises entre 0 et 1 avec la fonction f(x)=1-cos(frequence)/2
    //      et rempli un tempon en 2 dimensions
    //=====================================================================================================

        void Bruit::genere_bruit_2D_lignes_f(uint32_t largeur_tempon,uint32_t hauteur_tempon, float frequence, float *tempon)
        {
            uint32_t x,y;
            float valeur;

            for(x=0;x<largeur_tempon;x++)
            {
                valeur=(1.-cosf(M_PI*(float)x/largeur_tempon*frequence))/2;

                for (y=0;y<hauteur_tempon;y++)
                {
                    tempon[x+y*largeur_tempon]=valeur;
                }
            }

        }

    //==================================================================================================================================
    //      Génère des valeurs comprises entre 0 et 1 en combinant la fonction f(x)=1-cos(frequence)/2 et un tempon de perturbation
    //      et rempli un tempon en 2 dimensions
    //==================================================================================================================================

        void Bruit::genere_bruit_2D_lignes_perturbation_f(uint32_t largeur_tempon,uint32_t hauteur_tempon, float frequence, float perturbation, float* tempon_perturbation, float *tempon_resultat)
        {
            uint32_t x,y,i;
            float valeur;

            for(y=0;y<hauteur_tempon;y++)
            {
                for (x=0;x<hauteur_tempon;x++)
                {
                    i=x+y*largeur_tempon;
                    valeur=(1.-cosf(frequence*2*M_PI*((float)x/largeur_tempon+perturbation*tempon_perturbation[i])))/2.;
                    tempon_resultat[i]=valeur;
                }
            }

        }

    //=====================================================================================================
    //      Génère des valeurs comprises entre 0 et 1 en superposant deux tempons A et B.
    //      et rempli un troisième tempon en 2 dimensions (ce 3ème tempon peut être A ou B ou un autre).
    //=====================================================================================================
        void Bruit::superpose_tempons_f (uint32_t largeur, uint32_t hauteur, float* tempon_A, float intensite_A, float* tempon_B, float intensite_B, float *tempon_resultat)
        {
            uint32_t x,y,i;
            for (y=0;y<hauteur;y++)
            {
                for (x=0;x<largeur;x++)
                {
                    i=x+y*largeur;
                    tempon_resultat[i]=(tempon_A[i]*intensite_A+tempon_B[i]*intensite_B)/(intensite_A+intensite_B);
                }
            }
        }

    //============================================================================
    //      Génère des valeurs comprises entre 0 et 1 avec un bruit de Perlin
    //      et rempli un tempon en 2 dimensions
    //============================================================================

        void Bruit::genere_bruit_de_Perlin_2D_f(uint32_t largeur_fondamentale, uint32_t hauteur_fondamentale,uint32_t largeur_tempon,uint32_t hauteur_tempon, uint32_t nbr_octaves, float persistance,float* tempon)
        {
            vector<float> octave;

            uint32_t i,x,y;
            float amplitude;

            //----- Génère les couches:

            for (i=0;i<nbr_octaves;i++)
            {
                octave.clear();
                //Génère les valeurs de base, comprises entre 0 et 1
                for (y=0;y<hauteur_fondamentale*(i+1);y++)
                    for (x=0;x<largeur_fondamentale*(i+1);x++)
                        octave.push_back((float)rand()/32768.);

                interpolation_octave_f(largeur_fondamentale,hauteur_fondamentale,largeur_tempon,hauteur_tempon,persistance,i,octave,tempon);
            }


            //----- Génère les valeurs finales:
            amplitude=(1.-persistance)/(1.-powf(persistance,(float)(nbr_octaves)));

            for (y=0;y<largeur_tempon;y++)
                for (x=0;x<largeur_tempon;x++)
                    tempon[x+y*largeur_tempon]=tempon[x+y*largeur_tempon]*amplitude;
        }


        void Bruit::genere_bruit_de_Perlin_2D_d(uint32_t largeur_fondamentale, uint32_t hauteur_fondamentale,uint32_t largeur_tempon,uint32_t hauteur_tempon, uint32_t nbr_octaves, double persistance,double* tempon)
        {
            vector<double> octave;

            uint32_t i,x,y;
            double amplitude;

            //----- Génère les couches:

            for (i=0;i<nbr_octaves;i++)
            {
                octave.clear();
                //Génère les valeurs de base, comprises entre 0 et 1
                for (y=0;y<hauteur_fondamentale*(i+1);y++)
                for (x=0;x<largeur_fondamentale*(i+1);x++)
                    octave.push_back((double)rand()/32768.);

                interpolation_octave_d(largeur_fondamentale,hauteur_fondamentale,largeur_tempon,hauteur_tempon,persistance,i,octave,tempon);
            }


            //----- Génère les valeurs finales:
            amplitude=(1.-persistance)/(1.-powf(persistance,(double)(nbr_octaves)));

            for (y=0;y<largeur_tempon;y++)
                for (x=0;x<largeur_tempon;x++)
                    tempon[x+y*largeur_tempon]*=amplitude;
        }

    //Les valeurs sont comprises entre 0 et 255:
        bool Bruit::genere_bruit_de_Perlin_2D_8bits(uint32_t largeur_fondamentale, uint32_t hauteur_fondamentale,uint32_t largeur_tempon,uint32_t hauteur_tempon, uint32_t nbr_octaves, float persistance,uint8_t* tempon)
        {
            vector<float> octave;

            uint32_t i,x,y;
            float amplitude;

            float* tempon_f=(float*)calloc(sizeof(float),hauteur_tempon*largeur_tempon);
            if (tempon_f==NULL) return false;

            //----- Génère les couches:

            for (i=0;i<nbr_octaves;i++)
            {
                octave.clear();
                //Génère les valeurs de base, comprises entre 0 et 1
                for (y=0;y<hauteur_fondamentale*(i+1);y++)
                    for (x=0;x<largeur_fondamentale*(i+1);x++)
                        octave.push_back(float(rand())/32768.);

                interpolation_octave_f(largeur_fondamentale,hauteur_fondamentale,largeur_tempon,hauteur_tempon,persistance,i,octave,tempon_f);
            }


            //----- Génère les valeurs finales:
            amplitude=(1.-persistance)/(1.-powf(persistance,(float)(nbr_octaves)))*255.;

            for (y=0;y<largeur_tempon;y++)
                for (x=0;x<largeur_tempon;x++)
                    {
                        float v=tempon_f[x+y*largeur_tempon]*amplitude;
                        if (v>255.)v=255.;
                        tempon[x+y*largeur_tempon]=uint8_t(v);
                    }
            free(tempon_f);

            return true;
        }


    //===================================================================================
    //          Génère une valeur temporelle aléatoire, lissée selon un bruit de Perlin
    //          La valeur renvoyée est comprise entre 0 et 1
    //          t: temps en s
    //          t_prec: temps précédent en s
    //          intervalle: l'intervalle de temps entre les valeurs aléatoires à interpôler (en s)
    //===================================================================================
        double Bruit::valeur_aleatoire_temporelle(double t, double intervalle,double durete)
        {
            double position_interpolation=t/intervalle;
            uint32_t position_trame_aleatoire=(uint32_t)position_interpolation;
            position_interpolation-=position_trame_aleatoire_precedente;

            //Génère une nouvelle valeur aléatoire:
            if (position_trame_aleatoire>position_trame_aleatoire_precedente)
            {
                position_trame_aleatoire_precedente=position_trame_aleatoire;
                borne_0=borne_1;
                borne_1=(double)rand()/32768.;
                position_interpolation=0.;
            }
            //Interpolation entre les deux valeurs aléatoires:
            return interpolation_cosinusoidale_d(borne_0,borne_1,position_interpolation,durete);
        }


        //--------- Avec des bornes d'interpolation extérieures:
        double Bruit::valeur_aleatoire_temporelle(double t, double intervalle,float borne0,float borne1,double durete)
        {
            double position_interpolation=t/intervalle;
            uint32_t position_trame_aleatoire=(uint32_t)position_interpolation;
            position_interpolation-=position_trame_aleatoire_precedente;

            //Génère une nouvelle valeur aléatoire:
            if (position_trame_aleatoire>position_trame_aleatoire_precedente)
            {
                position_trame_aleatoire_precedente=position_trame_aleatoire;
                position_interpolation=0.;
            }
            //Interpolation entre les deux valeurs aléatoires:
            return interpolation_cosinusoidale_d(borne0,borne1,position_interpolation,durete);
        }


    //===================================================================================
    //          Génère une valeur temporelle selon une suite donnée.
    //          La valeur renvoyée est comprise entre 0 et 1
    //          t: temps en s
    //          t_prec: temps précédent en s
    //          intervalle: l'intervalle de temps entre les valeurs à interpôler (en s)
    //          suite: le tableau de valeurs à interpoler
    //          taille: le nombre de valeurs dans la suite.
    //===================================================================================
        double Bruit::valeur_suite_temporelle(double t, double intervalle,double* suite,uint32_t taille)
        {
            if (t0==-1.) t0=t;
            t-=t0;
            double position_interpolation=t/intervalle;
            uint32_t position_trame=(uint32_t)position_interpolation;
            position_interpolation-=position_trame;
            position_trame=position_trame%taille;
            borne_0=suite[position_trame];
            if (position_trame==taille-1) borne_1=suite[0]; else borne_1=suite[position_trame+1];
            //Interpolation entre les deux valeurs aléatoires:
            return interpolation_cosinusoidale_d(borne_0,borne_1,position_interpolation);
        }

    //------------------------------------------
    //Sert à initialiser une suite fixe
    //------------------------------------------

        void Bruit::reset_suite()
        {
            t0=-1.;
        }


