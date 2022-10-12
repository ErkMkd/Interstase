
//===================================================================
//             Outils d'interpolations
//===================================================================


#include "Interpolations.h"

using namespace std;

    //-----------------------------------------------------
    //  Interpolation linéaire entre deux valeurs a et b
    //  t varie entre 0 et 1 (0=a, 1=b)
    // renvoie la valeur intermédiaire.
    //-----------------------------------------------------
    float Interpolations::interpolation_lineaire_f(float a,float b,float t)
    {
        return a*(1-t)+b*t;
    }
    double Interpolations::interpolation_lineaire_d(double a,double b,double t)
    {
        return a*(1-t)+b*t;
    }

    void Interpolations::interpolation_lineaire_2f(float* a,float* b,float t,float* res)
    {
        res[0]= a[0]*(1-t)+b[0]*t;
        res[1]= a[1]*(1-t)+b[1]*t;
    }

    void Interpolations::interpolation_lineaire_2d(double* a,double* b,double t,double* res)
    {
        res[0]= a[0]*(1-t)+b[0]*t;
        res[1]= a[1]*(1-t)+b[1]*t;
    }


    void Interpolations::interpolation_lineaire_3f(float* a,float* b,float t,float* res)
    {
        res[0]= a[0]*(1-t)+b[0]*t;
        res[1]= a[1]*(1-t)+b[1]*t;
        res[2]= a[2]*(1-t)+b[2]*t;
    }

    void Interpolations::interpolation_lineaire_4f(float* a,float* b,float t,float* res)
    {
        res[0]= a[0]*(1-t)+b[0]*t;
        res[1]= a[1]*(1-t)+b[1]*t;
        res[2]= a[2]*(1-t)+b[2]*t;
        res[3]= a[3]*(1-t)+b[3]*t;
    }

    void Interpolations::interpolation_lineaire_3d(double* a,double* b,double t,double* res)
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
    float Interpolations::interpolation_cosinusoidale_f(float a,float b,float t)
    {
        return interpolation_lineaire_f(a,b,(-cosf(M_PI*t)+1.)/2.);
    }
    double Interpolations::interpolation_cosinusoidale_d(double a,double b,double t)
    {
        return interpolation_lineaire_d(a,b,(-cos(M_PI*t)+1.)/2.);
    }

    void Interpolations::interpolation_cosinusoidale_2f(float* a,float* b,float t,float* res)
    {
        interpolation_lineaire_2f(a,b,(-cosf(M_PI*t)+1.)/2.,res);
    }

    void Interpolations::interpolation_cosinusoidale_2d(double* a,double* b,double t,double* res)
    {
        interpolation_lineaire_2d(a,b,(-cos(M_PI*t)+1.)/2.,res);
    }


    void Interpolations::interpolation_cosinusoidale_3f(float* a,float* b,float t,float* res)
    {
        interpolation_lineaire_3f(a,b,(-cosf(M_PI*t)+1.)/2.,res);
    }

    void Interpolations::interpolation_cosinusoidale_4f(float* a,float* b,float t,float* res)
    {
        interpolation_lineaire_4f(a,b,(-cosf(M_PI*t)+1.)/2.,res);
    }

    void Interpolations::interpolation_cosinusoidale_3d(double* a,double* b,double t,double* res)
    {
        interpolation_lineaire_3d(a,b,(-cos(M_PI*t)+1.)/2.,res);
    }


//-------------------------------------------------------------------------------------------------------------------------
//===============================================================================================================
//      Enveloppe d'interpolation
//      x(temps) et y(position) varient de 0 à 1. Les points de l'enveloppe doivent être compris entre ces valeurs
//      Type d'interpolation possible entre les points: LINEAIRE et COSINUS
///     ATTENTION à ce que les points se suivent dans un ordre croissant sur l'axe x (temps) !!!
///     Les points situés à x>1. sont ignorés.
//===============================================================================================================
//-------------------------------------------------------------------------------------------------------------------------

    Enveloppe_Interpolation::Enveloppe_Interpolation(float* p_points,uint32_t p_nbrPoints,uint8_t p_type)
    {
        erreur=ENVELOPPE_INTERPOLATION_OK;
        type_interpolation=p_type;
        nbrPoints=p_nbrPoints;
        points=p_points;
    }

    Enveloppe_Interpolation::~Enveloppe_Interpolation()
    {
    }


    float Enveloppe_Interpolation::renvoie_valeur(float t)
    {
        if (t<0.) t=0.;
        if (t>1.) t=1.;
        //Extrémités:
        if (t==0.) return points[1];
        if (t==1.) return points[nbrPoints*2+1];
        //cherche le numéro de l'intervalle:
        uint32_t i;
        for(i=0;i<nbrPoints;i++)
        {
            if (points[(i+1)*2]>=t) break;
        }
        //Position dans l'intervalle:
        float ti=(t-points[i*2])/(points[(i+1)*2]-points[i*2]);

        //Calcul la valeur:
        if (type_interpolation==INTERPOLATION_COSINUS) return interpolation_cosinusoidale_f(points[2*i+1],points[2*i+3],ti);
        else return interpolation_lineaire_f(points[2*i+1],points[2*i+3],ti);
    }

//-------------------------------------------------------------------------------------------------------------------------
//===============================================================================================================
//      Interpolation entre deux valeurs
//      Si le temps de départ est inférieur à 0, l'interpolation débute dès le premier appel à renvoie_valeur_parametre()
//      La courbe en entrée ne sert que pour définir les paramètre de la courbe interne.
//===============================================================================================================
//-------------------------------------------------------------------------------------------------------------------------

    Interpolation_Parametre::Interpolation_Parametre(float p_t0,float p_depart,float p_fin,float p_duree,uint8_t p_type,Courbe_Bezier_Plan* p_courbe,Enveloppe_Interpolation* p_enveloppe)
    {
        erreur=INTERPOLATION_PARAMETRE_OK;
        courbe_bezier=new Courbe_Bezier_Plan();
        if(courbe_bezier==NULL)
        {
            cout<<"ERREUR dans Interpolation_Parametre::Interpolation_Parametre() - Impossible de créer la courbe d'interpolation"<<endl;
            erreur=INTERPOLATION_PARAMETRE_ERREUR_MEMOIRE;
            return;
        }
        determine_parametres(p_t0,p_depart,p_fin,p_duree,p_type,p_courbe);
        enveloppe=p_enveloppe;
    }

    Interpolation_Parametre::~Interpolation_Parametre()
    {
        if (courbe_bezier!=NULL) delete courbe_bezier;
    }

    //Pour définir tout les paramètres d'un coup.
    void Interpolation_Parametre::determine_parametres(float p_t0,float p_depart,float p_fin,float p_duree,uint8_t p_type,Courbe_Bezier_Plan* p_courbe,Enveloppe_Interpolation* p_enveloppe)
    {
        t0=p_t0;
        valeur_depart=p_depart;
        valeur_fin=p_fin;
        duree=p_duree;
        if(p_type!=INTERPOLATION_INCONNUE)type_interpolation=p_type;
        drapeau_en_cours=false;
        if (p_courbe!=NULL) courbe_bezier->copie_parametres(p_courbe);
        enveloppe=p_enveloppe;
    }

    void Interpolation_Parametre::determine_courbe_Bezier(Courbe_Bezier_Plan* p_courbe)
    {
        courbe_bezier->copie_parametres(p_courbe);
    }

    //Réinitialise l'interpolation.
    ///La valeur de t0 est mise à -1 !!!
    void Interpolation_Parametre::reset()
    {
        drapeau_en_cours=false;
        t0=-1.;
    }


    float Interpolation_Parametre::renvoie_valeur_parametre(float t)
    {
        //if (drapeau_terminee) return valeur_fin;
        if (t0<0.) t0=t;
        else if (t<t0) { drapeau_en_cours=false; return valeur_depart;}
        else if (t>t0+duree) { drapeau_en_cours=false; return valeur_fin;}
        drapeau_en_cours=true;

        if (type_interpolation==INTERPOLATION_LINEAIRE)
        {
            if (enveloppe==NULL) return interpolation_lineaire_f(valeur_depart,valeur_fin,(t-t0)/duree);
            else return interpolation_lineaire_f(valeur_depart,valeur_fin,enveloppe->renvoie_valeur((t-t0)/duree));
        }
        else if (type_interpolation==INTERPOLATION_COSINUS)
        {
            if (enveloppe==NULL) return interpolation_cosinusoidale_f(valeur_depart,valeur_fin,(t-t0)/duree);
            else interpolation_cosinusoidale_f(valeur_depart,valeur_fin,enveloppe->renvoie_valeur((t-t0)/duree));
        }

        else if (type_interpolation==INTERPOLATION_BEZIER)
        {
            if (enveloppe==NULL) return interpolation_lineaire_f(valeur_depart,valeur_fin,courbe_bezier->renvoie_ordonnee_via_abscisse((t-t0)/duree));
            else return interpolation_lineaire_f(valeur_depart,valeur_fin,courbe_bezier->renvoie_ordonnee_via_abscisse(enveloppe->renvoie_valeur((t-t0)/duree)));
        }
        return 0.;
    }

//========================================================================================================
//              Suite d'interpolations
//  Les valeurs se présentent comme suit: valeur,duree,...
//  La dernière durée sert dans le cas d'un bouclage.
//  Si t0 en entrée est inférieur à 0, l'interpolation commence dès le premier appel à renvoie_valeur()
//========================================================================================================

    //-----------------------------------------------
    //      Constructeur
    //-----------------------------------------------

    Suite_Interpolations::Suite_Interpolations(float p_t0,float* p_valeurs, uint8_t* p_types,uint16_t p_nbrEtapes,bool p_bouclage,uint8_t type_temps,Courbe_Bezier_Plan** p_courbes)
    {
        t0=p_t0;
        tableau_valeurs=p_valeurs;
        tableau_types=p_types;
        tableau_courbes=p_courbes;
        nbrEtapes=p_nbrEtapes;
        drapeau_en_cours=false;
        drapeau_bouclage=p_bouclage;
        if(type_temps==SI_TYPE_TEMPS_REPERES)
        {
            for(int i=0;i<nbrEtapes-1;i++)
            {
                tableau_valeurs[2*i+1]=tableau_valeurs[2*(i+1)+1]-tableau_valeurs[2*i+1];
            }
            tableau_valeurs[2*(nbrEtapes-1)+1]=0.;
        }
        maj_duree_totale();
    }

    //-----------------------------------------------
    //      Destructeur
    //-----------------------------------------------
    Suite_Interpolations::~Suite_Interpolations()
    {

    }

    //Inverse les valeurs=inversion du mouvement
    void Suite_Interpolations::inverse()
    {
        uint16_t i,j;
        j=nbrEtapes-1;
        for(i=0;i<nbrEtapes/2;i++)
        {
            float val_mem=tableau_valeurs[2*i];
            float t_mem=tableau_valeurs[2*i+1];
            uint8_t type_mem=tableau_types[i];
            tableau_valeurs[2*i]=tableau_valeurs[2*j];
            tableau_valeurs[2*i+1]=tableau_valeurs[2*j+1];
            tableau_types[i]=tableau_types[j];
            tableau_valeurs[2*j]=val_mem;
            tableau_valeurs[2*j+1]=t_mem;
            tableau_types[j]=type_mem;
            if(tableau_courbes!=NULL)
            {
                Courbe_Bezier_Plan* courbe_mem=tableau_courbes[i];
                tableau_courbes[i]=tableau_courbes[j];
                tableau_courbes[j]=courbe_mem;
            }
            j--;
        }
    }
    //-----------------------------------------------
    //      Initialisations
    //-----------------------------------------------
    void Suite_Interpolations::maj_duree_totale()
    {
        uint16_t i;
        duree_totale=0.;
        for(i=0;i<nbrEtapes;i++)
        {
            duree_totale+=tableau_valeurs[i*2+1];
        }
    }

    void Suite_Interpolations::determine_t0(float p_t0)
    {
        t0=p_t0;    //Valeur négative si l'on veut relancer immédiatement le mouvement
    }

    //-----------------------------------------------
    //      Renvoie la valeur en fonction du temps
    //-----------------------------------------------
    float Suite_Interpolations::renvoie_valeur(float t)
    {
        /*
        if (!drapeau_en_cours)
        {
            if(t0<0.) return tableau_valeurs[0];
            else return tableau_valeurs[(nbrEtapes-1)*2];
        }
        */

        if (t0<0.) t0=t;
        else if (t<t0) {drapeau_en_cours=false;return tableau_valeurs[0];}
        else if((!drapeau_bouclage) && t>t0+duree_totale-tableau_valeurs[(nbrEtapes-1)*2+1])
        {
            drapeau_en_cours=false;
            return tableau_valeurs[(nbrEtapes-1)*2];
        }
        else if (t>t0+duree_totale)
        {
            drapeau_en_cours=false;
            return tableau_valeurs[0];
        }
        drapeau_en_cours=true;
        //Détermine l'étape actuelle:
        uint16_t etape_actuelle;
        float duree_actuelle;
        float t_relatif=0.;
        for(etape_actuelle=0;etape_actuelle<nbrEtapes;etape_actuelle++)
        {
            duree_actuelle=tableau_valeurs[etape_actuelle*2+1];
            if((duree_actuelle+t_relatif+t0)>t) break;
            t_relatif+=duree_actuelle;
        }

        float position=(t-t0-t_relatif)/duree_actuelle;
        uint8_t type_actuel=tableau_types[etape_actuelle];
        float valeur_depart=tableau_valeurs[etape_actuelle*2];
        float valeur_fin;

        if(etape_actuelle==nbrEtapes-1) valeur_fin=tableau_valeurs[0];
        else valeur_fin=tableau_valeurs[etape_actuelle*2+2];

        if (type_actuel==INTERPOLATION_LINEAIRE)
        {
            return interpolation_lineaire_f(valeur_depart,valeur_fin,position);
        }
        else if (type_actuel==INTERPOLATION_COSINUS)
        {
            return interpolation_cosinusoidale_f(valeur_depart,valeur_fin,position);
        }

        else if (type_actuel==INTERPOLATION_BEZIER)
        {
            return interpolation_lineaire_f(valeur_depart,valeur_fin,tableau_courbes[etape_actuelle]->renvoie_ordonnee_via_abscisse(position));
        }
        return 0.;
    }


//========================================================================================================
//              Suite d'interpolations avec 3 paramètres en float
//  Les valeurs se présentent comme suit: valeur1,valeur2,valeur3,duree,...
//  La dernière durée sert dans le cas d'un bouclage.
//  Si t0 en entrée est inférieur à 0, l'interpolation commence dès le premier appel à renvoie_valeur()
//========================================================================================================

    //-----------------------------------------------
    //      Constructeur
    //-----------------------------------------------

    Suite_Interpolations_3f::Suite_Interpolations_3f(float p_t0,float* p_valeurs, uint8_t* p_types,uint16_t p_nbrEtapes,bool p_bouclage,uint8_t type_temps,Courbe_Bezier_Plan** p_courbes)
    {
        t0=p_t0;
        tableau_valeurs=p_valeurs;
        tableau_types=p_types;
        tableau_courbes=p_courbes;
        nbrEtapes=p_nbrEtapes;
        drapeau_en_cours=false;
        drapeau_bouclage=p_bouclage;
        if(type_temps==SI_TYPE_TEMPS_REPERES)
        {
            for(int i=0;i<nbrEtapes-1;i++)
            {
                tableau_valeurs[4*i+1]=tableau_valeurs[4*(i+1)+1]-tableau_valeurs[4*i+1];
            }
            tableau_valeurs[4*(nbrEtapes-1)+1]=0.;
        }
        maj_duree_totale();
    }

    //-----------------------------------------------
    //      Destructeur
    //-----------------------------------------------
    Suite_Interpolations_3f::~Suite_Interpolations_3f()
    {

    }

    //-----------------------------------------------
    //      Initialisations
    //-----------------------------------------------
    void Suite_Interpolations_3f::maj_duree_totale()
    {
        uint16_t i;
        duree_totale=0.;
        for(i=0;i<nbrEtapes;i++)
        {
            duree_totale+=tableau_valeurs[i*4+1];
        }
    }

    void Suite_Interpolations_3f::determine_t0(float p_t0)
    {
        t0=p_t0;    //Valeur négative si l'on veut relancer immédiatement le mouvement
    }

    //-----------------------------------------------
    //      Renvoie la valeur en fonction du temps
    //-----------------------------------------------
    void Suite_Interpolations_3f::renvoie_valeurs(float t, float* resultat)
    {
        /*
        if (!drapeau_en_cours)
        {
            if(t0<0.) return tableau_valeurs[0];
            else return tableau_valeurs[(nbrEtapes-1)*2];
        }
        */

        if (t0<0.) t0=t;
        else if (t<t0) {drapeau_en_cours=false;resultat[0]=tableau_valeurs[0];resultat[1]=tableau_valeurs[1];resultat[2]=tableau_valeurs[2];return;}
        else if((!drapeau_bouclage) && t>t0+duree_totale-tableau_valeurs[(nbrEtapes-1)*4+1])
        {
            drapeau_en_cours=false;
            resultat[0]=tableau_valeurs[(nbrEtapes-1)*4];resultat[1]=tableau_valeurs[(nbrEtapes-1)*4+1];resultat[2]=tableau_valeurs[(nbrEtapes-1)*4+2];
            return;
        }
        else if (t>t0+duree_totale)
        {
            drapeau_en_cours=false;
            resultat[0]=tableau_valeurs[0];resultat[1]=tableau_valeurs[1];resultat[2]=tableau_valeurs[2];return;
        }
        drapeau_en_cours=true;
        //Détermine l'étape actuelle:
        uint16_t etape_actuelle;
        float duree_actuelle;
        float t_relatif=0.;
        for(etape_actuelle=0;etape_actuelle<nbrEtapes;etape_actuelle++)
        {
            duree_actuelle=tableau_valeurs[etape_actuelle*4+1];
            if((duree_actuelle+t_relatif+t0)>t) break;
            t_relatif+=duree_actuelle;
        }

        float position=(t-t0-t_relatif)/duree_actuelle;
        uint8_t type_actuel=tableau_types[etape_actuelle];

        float* valeur_depart=&tableau_valeurs[etape_actuelle*4];
        float* valeur_fin;

        if(etape_actuelle==nbrEtapes-1) valeur_fin=&tableau_valeurs[0];
        else valeur_fin=&tableau_valeurs[etape_actuelle*4+2];

        if (type_actuel==INTERPOLATION_LINEAIRE)
        {
            return interpolation_lineaire_3f(valeur_depart,valeur_fin,position,resultat);
        }
        else if (type_actuel==INTERPOLATION_COSINUS)
        {
            return interpolation_cosinusoidale_3f(valeur_depart,valeur_fin,position,resultat);
        }

        else if (type_actuel==INTERPOLATION_BEZIER)
        {
            return interpolation_lineaire_3f(valeur_depart,valeur_fin,tableau_courbes[etape_actuelle]->renvoie_ordonnee_via_abscisse(position),resultat);
        }
    }



//-------------------------------------------------------------------------------------------------------------------------
//===============================================================================================================
//      Interpolation entre deux coordonnées bidimentionnelles
//      Si le temps de départ est inférieur à 0, l'interpolation débute dès le premier appel à renvoie_valeur_parametre()
//      La courbe en entrée ne sert que pour définir les paramètre de la courbe interne.
//===============================================================================================================
//-------------------------------------------------------------------------------------------------------------------------

    Interpolation_Parametre_2f::Interpolation_Parametre_2f(float p_t0,float* p_depart,float* p_fin,float p_duree,uint8_t p_type,Courbe_Bezier_Plan* p_courbe)
    {
        erreur=INTERPOLATION_PARAMETRE_OK;
        courbe_bezier=new Courbe_Bezier_Plan();
        if(courbe_bezier==NULL)
        {
            cout<<"ERREUR dans Interpolation_Parametre_2f::Interpolation_Parametre_2f() - Impossible de créer la courbe d'interpolation"<<endl;
            erreur=INTERPOLATION_PARAMETRE_ERREUR_MEMOIRE;
            return;
        }
        determine_parametres(p_t0,p_depart,p_fin,p_duree,p_type,p_courbe);
    }

    Interpolation_Parametre_2f::~Interpolation_Parametre_2f()
    {
        if (courbe_bezier!=NULL) delete courbe_bezier;
    }

    //Pour définir tout les paramètres d'un coup.
    void Interpolation_Parametre_2f::determine_parametres(float p_t0,float* p_depart,float* p_fin,float p_duree,uint8_t p_type,Courbe_Bezier_Plan* p_courbe)
    {
        t0=p_t0;
        valeur_depart[0]=p_depart[0];
        valeur_depart[1]=p_depart[1];
        valeur_fin[0]=p_fin[0];
        valeur_fin[1]=p_fin[1];
        duree=p_duree;
        if(p_type!=INTERPOLATION_INCONNUE) type_interpolation=p_type;
        drapeau_en_cours=false;
        if (p_courbe!=NULL) courbe_bezier->copie_parametres(p_courbe);
    }


    void Interpolation_Parametre_2f::renvoie_valeur_parametre(float t, float* resultat)
    {
        if (t0<0.) t0=t;
        else if (t<t0) {resultat[0]=valeur_depart[0];resultat[1]=valeur_depart[1]; drapeau_en_cours=false; return;}
        else if (t>t0+duree)  {resultat[0]=valeur_fin[0];resultat[1]=valeur_fin[1]; drapeau_en_cours=false; return;}
        drapeau_en_cours=true;

        if (type_interpolation==INTERPOLATION_LINEAIRE) interpolation_lineaire_2f(valeur_depart,valeur_fin,(t-t0)/duree,resultat);
        else if (type_interpolation==INTERPOLATION_COSINUS) interpolation_cosinusoidale_2f(valeur_depart,valeur_fin,(t-t0)/duree,resultat);
        else if (type_interpolation==INTERPOLATION_BEZIER)
        {
            interpolation_lineaire_2f(valeur_depart,valeur_fin,courbe_bezier->renvoie_ordonnee_via_abscisse((t-t0)/duree),resultat);
        }
    }

//===============================================================================================================
//      Interpolation de deux valeurs - type double
//===============================================================================================================

    Interpolation_Parametre_2d::Interpolation_Parametre_2d(double p_t0,double* p_depart,double* p_fin,double p_duree,uint8_t p_type,Courbe_Bezier_Plan* p_courbe)
    {
        erreur=INTERPOLATION_PARAMETRE_OK;
        courbe_bezier=new Courbe_Bezier_Plan();
        if(courbe_bezier==NULL)
        {
            cout<<"ERREUR dans Interpolation_Parametre_2d::Interpolation_Parametre_2d() - Impossible de créer la courbe d'interpolation"<<endl;
            erreur=INTERPOLATION_PARAMETRE_ERREUR_MEMOIRE;
            return;
        }
        determine_parametres(p_t0,p_depart,p_fin,p_duree,p_type,p_courbe);
    }

    Interpolation_Parametre_2d::~Interpolation_Parametre_2d()
    {
        if (courbe_bezier!=NULL) delete courbe_bezier;
    }

    //Pour définir tout les paramètres d'un coup.
    void Interpolation_Parametre_2d::determine_parametres(double p_t0,double* p_depart,double* p_fin,double p_duree,uint8_t p_type,Courbe_Bezier_Plan* p_courbe)
    {
        t0=p_t0;
        valeur_depart[0]=p_depart[0];
        valeur_depart[1]=p_depart[1];
        valeur_fin[0]=p_fin[0];
        valeur_fin[1]=p_fin[1];
        duree=p_duree;
        type_interpolation=p_type;
        drapeau_en_cours=false;
        if (p_courbe!=NULL) courbe_bezier->copie_parametres(p_courbe);
    }


    void Interpolation_Parametre_2d::renvoie_valeur_parametre(double t, double* resultat)
    {
        if (t0<0.) t0=t;
        else if (t<t0) {resultat[0]=valeur_depart[0];resultat[1]=valeur_depart[1]; drapeau_en_cours=false; return;}
        else if (t>t0+duree)  {resultat[0]=valeur_fin[0];resultat[1]=valeur_fin[1];drapeau_en_cours=false; return;}
        drapeau_en_cours=true;

        if (type_interpolation==INTERPOLATION_LINEAIRE) return interpolation_lineaire_2d(valeur_depart,valeur_fin,(t-t0)/duree,resultat);
        if (type_interpolation==INTERPOLATION_COSINUS) return interpolation_cosinusoidale_2d(valeur_depart,valeur_fin,(t-t0)/duree,resultat);
        else if (type_interpolation==INTERPOLATION_BEZIER)
        {
            interpolation_lineaire_2d(valeur_depart,valeur_fin,courbe_bezier->renvoie_ordonnee_via_abscisse((t-t0)/duree),resultat);
        }
    }


//-------------------------------------------------------------------------------------------------------------------------
//===============================================================================================================
//      Interpolation entre deux coordonnées tridimensionnelle
//      Si le temps de départ est inférieur à 0, l'interpolation débute dès le premier appel à renvoie_valeur_parametre()
//      La courbe en entrée ne sert que pour définir les paramètre de la courbe interne.
//===============================================================================================================
//-------------------------------------------------------------------------------------------------------------------------


    Interpolation_Parametre_3f::Interpolation_Parametre_3f(float p_t0,float* p_depart,float* p_fin,float p_duree,uint8_t p_type,Courbe_Bezier_Plan* p_courbe)
    {
        erreur=INTERPOLATION_PARAMETRE_OK;
        courbe_bezier=new Courbe_Bezier_Plan();
        if(courbe_bezier==NULL)
        {
            cout<<"ERREUR dans Interpolation_Parametre_3f::Interpolation_Parametre_3f() - Impossible de créer la courbe d'interpolation"<<endl;
            erreur=INTERPOLATION_PARAMETRE_ERREUR_MEMOIRE;
            return;
        }
        determine_parametres(p_t0,p_depart,p_fin,p_duree,p_type,p_courbe);
    }

    Interpolation_Parametre_3f::~Interpolation_Parametre_3f()
    {
        if (courbe_bezier!=NULL) delete courbe_bezier;
    }

    void Interpolation_Parametre_3f::determine_parametres(float p_t0,float* p_depart,float* p_fin,float p_duree,uint8_t p_type,Courbe_Bezier_Plan* p_courbe)
    {
        t0=p_t0;
        determine_valeurs(p_depart,p_fin);
        duree=p_duree;
        type_interpolation=p_type;
        drapeau_en_cours=false;
        if (p_courbe!=NULL) courbe_bezier->copie_parametres(p_courbe);
    }

    void Interpolation_Parametre_3f::determine_valeurs(float* p_depart,float* p_fin)
    {
        valeur_depart[0]=p_depart[0];
        valeur_depart[1]=p_depart[1];
        valeur_depart[2]=p_depart[2];
        valeur_fin[0]=p_fin[0];
        valeur_fin[1]=p_fin[1];
        valeur_fin[2]=p_fin[2];
    }

    void Interpolation_Parametre_3f::determine_valeurs_depart(float* p_depart)
    {
        valeur_depart[0]=p_depart[0];
        valeur_depart[1]=p_depart[1];
        valeur_depart[2]=p_depart[2];
    }

    void Interpolation_Parametre_3f::determine_valeurs_fin(float* p_fin)
    {
        valeur_fin[0]=p_fin[0];
        valeur_fin[1]=p_fin[1];
        valeur_fin[2]=p_fin[2];
    }


    void Interpolation_Parametre_3f::determine_courbe_Bezier(Courbe_Bezier_Plan* p_courbe)
    {
        courbe_bezier->copie_parametres(p_courbe);
    }

    void Interpolation_Parametre_3f::renvoie_valeur_parametre(float t, float* resultat)
    {
        if (t0<0.) t0=t;
        else if (t<t0) {resultat[0]=valeur_depart[0];resultat[1]=valeur_depart[1];resultat[2]=valeur_depart[2]; drapeau_en_cours=false; return;}
        else if (t>t0+duree)  {resultat[0]=valeur_fin[0];resultat[1]=valeur_fin[1];resultat[2]=valeur_fin[2]; drapeau_en_cours=false; return;}
        drapeau_en_cours=true;

        if (type_interpolation==INTERPOLATION_LINEAIRE) interpolation_lineaire_3f(valeur_depart,valeur_fin,(t-t0)/duree,resultat);
        else if (type_interpolation==INTERPOLATION_COSINUS) interpolation_cosinusoidale_3f(valeur_depart,valeur_fin,(t-t0)/duree,resultat);
        else if (type_interpolation==INTERPOLATION_BEZIER)
        {
            interpolation_lineaire_3f(valeur_depart,valeur_fin,courbe_bezier->renvoie_ordonnee_via_abscisse((t-t0)/duree),resultat);
        }
    }


//-------------------------------------------------------------------------------------------------------------------------
//===============================================================================================================
//      Interpolation entre plusieurs parametres
//      Si le temps de départ est inférieur à 0, l'interpolation débute dès le premier appel à renvoie_valeur_parametre()
//      La courbe en entrée ne sert que pour définir les paramètre de la courbe interne.
//===============================================================================================================
//-------------------------------------------------------------------------------------------------------------------------


    Interpolation_Parametre_multi::Interpolation_Parametre_multi(float p_t0,float* p_depart,float* p_fin,uint16_t numParams, float p_duree,uint8_t p_type,Courbe_Bezier_Plan* p_courbe)
    {
        erreur=INTERPOLATION_PARAMETRE_OK;
        num_parametres=numParams;
        courbe_bezier=new Courbe_Bezier_Plan();
        if(courbe_bezier==NULL)
        {
            cout<<"ERREUR dans Interpolation_Parametre_multi::Interpolation_Parametre_multi() - Impossible de créer la courbe d'interpolation"<<endl;
            erreur=INTERPOLATION_PARAMETRE_ERREUR_MEMOIRE;
            return;
        }
        determine_parametres(p_t0,p_depart,p_fin,p_duree,p_type,p_courbe);
    }

    Interpolation_Parametre_multi::~Interpolation_Parametre_multi()
    {
        if (courbe_bezier!=NULL) delete courbe_bezier;
    }

    void Interpolation_Parametre_multi::determine_parametres(float p_t0,float* p_depart,float* p_fin,float p_duree,uint8_t p_type,Courbe_Bezier_Plan* p_courbe)
    {
        t0=p_t0;
        determine_valeurs(p_depart,p_fin);
        duree=p_duree;
        type_interpolation=p_type;
        drapeau_en_cours=false;
        if (p_courbe!=NULL) courbe_bezier->copie_parametres(p_courbe);
    }

    void Interpolation_Parametre_multi::determine_valeurs(float* p_depart,float* p_fin)
    {
        uint16_t i;
        for(i=0;i<num_parametres;i++)
        {
            valeur_depart[i]=p_depart[i];
            valeur_fin[i]=p_fin[i];
        }

    }

    void Interpolation_Parametre_multi::determine_valeurs_depart(float* p_depart)
    {
        uint16_t i;
        for(i=0;i<num_parametres;i++) valeur_depart[i]=p_depart[i];
    }

    void Interpolation_Parametre_multi::determine_valeurs_fin(float* p_fin)
    {
        uint16_t i;
        for(i=0;i<num_parametres;i++) valeur_fin[i]=p_fin[i];
    }


    void Interpolation_Parametre_multi::determine_courbe_Bezier(Courbe_Bezier_Plan* p_courbe)
    {
        courbe_bezier->copie_parametres(p_courbe);
    }

    void Interpolation_Parametre_multi::renvoie_valeur_parametre(float t, float* resultat)
    {
        uint16_t i;
        if (t0<0.) t0=t;
        else if (t<t0)
        {
            for(i=0;i<num_parametres;i++) resultat[i]=valeur_depart[i];
            drapeau_en_cours=false; return;
        }
        else if (t>t0+duree)
        {
            for(i=0;i<num_parametres;i++) resultat[i]=valeur_fin[i];
            drapeau_en_cours=false; return;
        }

        drapeau_en_cours=true;

        if (type_interpolation==INTERPOLATION_LINEAIRE)
            for(i=0;i<num_parametres;i++)resultat[i]=interpolation_lineaire_f(valeur_depart[i],valeur_fin[i],(t-t0)/duree);
        else if (type_interpolation==INTERPOLATION_COSINUS)
            for(i=0;i<num_parametres;i++)resultat[i]=interpolation_cosinusoidale_f(valeur_depart[i],valeur_fin[i],(t-t0)/duree);
        else if (type_interpolation==INTERPOLATION_BEZIER)
        {
            float tBezier=courbe_bezier->renvoie_ordonnee_via_abscisse((t-t0)/duree);
            for(i=0;i<num_parametres;i++)resultat[i]=interpolation_lineaire_f(valeur_depart[i],valeur_fin[i],tBezier);
        }
    }

//===============================================================================================================
//      Interpolation de trois valeurs - type double
//===============================================================================================================



    Interpolation_Parametre_3d::Interpolation_Parametre_3d(double p_t0,double* p_depart,double* p_fin,double p_duree,uint8_t p_type,Courbe_Bezier_Plan* p_courbe)
    {
        erreur=INTERPOLATION_PARAMETRE_OK;
        courbe_bezier=new Courbe_Bezier_Plan();
        if(courbe_bezier==NULL)
        {
            cout<<"ERREUR dans Interpolation_Parametre_3d::Interpolation_Parametre_3d() - Impossible de créer la courbe d'interpolation"<<endl;
            erreur=INTERPOLATION_PARAMETRE_ERREUR_MEMOIRE;
            return;
        }
        determine_parametres(p_t0,p_depart,p_fin,p_duree,p_type,p_courbe);
    }

    Interpolation_Parametre_3d::~Interpolation_Parametre_3d()
    {
        if (courbe_bezier!=NULL) delete courbe_bezier;
    }

    //Pour définir tout les paramètres d'un coup.
    void Interpolation_Parametre_3d::determine_parametres(double p_t0,double* p_depart,double* p_fin,double p_duree,uint8_t p_type,Courbe_Bezier_Plan* p_courbe)
    {
        t0=p_t0;
        valeur_depart[0]=p_depart[0];
        valeur_depart[1]=p_depart[1];
        valeur_depart[2]=p_depart[2];
        valeur_fin[0]=p_fin[0];
        valeur_fin[1]=p_fin[1];
        valeur_fin[2]=p_fin[2];
        duree=p_duree;
        type_interpolation=p_type;
        drapeau_en_cours=false;
        if (p_courbe!=NULL) courbe_bezier->copie_parametres(p_courbe);
    }


    void Interpolation_Parametre_3d::renvoie_valeur_parametre(double t, double* resultat)
    {
        if (t0<0.) t0=t;
        else if (t<t0) {resultat[0]=valeur_depart[0];resultat[1]=valeur_depart[1];resultat[2]=valeur_depart[2]; drapeau_en_cours=false; return;}
        else if (t>t0+duree)  {resultat[0]=valeur_fin[0];resultat[1]=valeur_fin[1];resultat[2]=valeur_fin[2]; drapeau_en_cours=false; return;}
        drapeau_en_cours=true;

        if (type_interpolation==INTERPOLATION_LINEAIRE) return interpolation_lineaire_3d(valeur_depart,valeur_fin,(t-t0)/duree,resultat);
        if (type_interpolation==INTERPOLATION_COSINUS) return interpolation_cosinusoidale_3d(valeur_depart,valeur_fin,(t-t0)/duree,resultat);
    }

//-------------------------------------------------------------------------------------------------------------------------
//===============================================================================================================
//      Interpolation entre deux couleurs définies au format 32bits 0xRRVVBBAA
//      Si le temps de départ est inférieur à 0, l'interpolation débute dès le premier appel à renvoie_valeur_parametre()
//      La courbe en entrée ne sert que pour définir les paramètre de la courbe interne.
//===============================================================================================================
//-------------------------------------------------------------------------------------------------------------------------


    Interpolation_Parametre_rvba::Interpolation_Parametre_rvba(float p_t0,uint32_t p_depart,uint32_t p_fin,float p_duree,uint8_t p_type,Courbe_Bezier_Plan* p_courbe)
    {
        erreur=INTERPOLATION_PARAMETRE_OK;
        courbe_bezier=new Courbe_Bezier_Plan();
        if(courbe_bezier==NULL)
        {
            cout<<"ERREUR dans Interpolation_Parametre_rvba::Interpolation_Parametre_rvba() - Impossible de créer la courbe d'interpolation"<<endl;
            erreur=INTERPOLATION_PARAMETRE_ERREUR_MEMOIRE;
            return;
        }
        determine_parametres(p_t0,p_depart,p_fin,p_duree,p_type,p_courbe);
    }

    Interpolation_Parametre_rvba::~Interpolation_Parametre_rvba()
    {
        if (courbe_bezier!=NULL) delete courbe_bezier;
    }

    void Interpolation_Parametre_rvba::determine_parametres(float p_t0,uint32_t p_depart,uint32_t p_fin,float p_duree,uint8_t p_type,Courbe_Bezier_Plan* p_courbe)
    {
        t0=p_t0;
        valeur_depart=p_depart;
        valeur_fin=p_fin;
        rvba_depart[0]=float((valeur_depart>>24)&0xff)/255.;
        rvba_depart[1]=float((valeur_depart>>16)&0xff)/255.;
        rvba_depart[2]=float((valeur_depart>>8)&0xff)/255.;
        rvba_depart[3]=float(valeur_depart&0xff)/255.;
        rvba_fin[0]=float((valeur_fin>>24)&0xff)/255.;
        rvba_fin[1]=float((valeur_fin>>16)&0xff)/255.;
        rvba_fin[2]=float((valeur_fin>>8)&0xff)/255.;
        rvba_fin[3]=float(valeur_fin&0xff)/255.;
        duree=p_duree;
        type_interpolation=p_type;
        drapeau_en_cours=false;
        if (p_courbe!=NULL) courbe_bezier->copie_parametres(p_courbe);
    }


    uint32_t Interpolation_Parametre_rvba::renvoie_valeur_parametre(float t)
    {
        if (t0<0.) t0=t;
        else if (t<t0) {return valeur_depart;drapeau_en_cours=false;}
        else if (t>t0+duree)  {drapeau_en_cours=false; return valeur_fin;}
        drapeau_en_cours=true;

        float resultat[4];

        if (type_interpolation==INTERPOLATION_LINEAIRE) interpolation_lineaire_4f(rvba_depart,rvba_fin,(t-t0)/duree,resultat);
        else if (type_interpolation==INTERPOLATION_COSINUS) interpolation_cosinusoidale_4f(rvba_depart,rvba_fin,(t-t0)/duree,resultat);

        uint32_t rvba=(uint32_t(resultat[0]*255.)<<24)|(uint32_t(resultat[1]*255.)<<16)|(uint32_t(resultat[2]*255.)<<8)|(uint32_t(resultat[3]*255.));
        return rvba;
    }
