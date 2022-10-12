#include "Cosmonaute.h"

//===================================
//      Constructeur
//===================================
Cosmonaute::Cosmonaute(const char* nomFichierLWO)
{
    erreur=COSMO_OK;
    objetLWO=NULL;
    volumesConv=NULL;
    echelle=1.;
    drapeau_affiche_visage=true;
    uint32_t i;

    objetLWO=new ChargeLWO2(nomFichierLWO);

    if(objetLWO==NULL || objetLWO->erreur!=CHARGELWO2_OK)
    {
        erreur=COSMO_CHARGELWO;
        return;
    }

    volumesConv=new ConvertionLWO2(objetLWO);
    if(volumesConv==NULL || volumesConv->erreur!=CONVERTIONLWO2_OK)
    {
        erreur=COSMO_CONVERTIONLWO;
        return;
    }

    char* noms[]={"Cosmo_tronc",
                "Cosmo_visiere",
                "Cosmo_tete",
                "Cosmo_avantbras_G",
                "Cosmo_avantbras_D",
                "Cosmo_bras_G",
                "Cosmo_bras_D",
                "Cosmo_epaule_D",
                "Cosmo_epaule_G",
                "Cosmo_pied_D",
                "Cosmo_pied_G",
                "Cosmo_molet_D",
                "Cosmo_molet_G",
                "Cosmo_jambe_D",
                "Cosmo_jambe_G",
                "Cosmo_Bassin",
                "Cosmo_main_G",
                "Cosmo_main_D",
                "Cosmo_oeil_D",
                "Cosmo_oeil_G"
                };

    for (i=0;i<COSMO_NBR_OBJETS;i++)
    {
        volumes[i]=volumesConv->renvoie_volume_nom(noms[i]);
        cout<<volumes[i]->nom<<endl;
    }
    Surface* peau=volumes[COSMO_TETE]->renvoie_surface_nom("Face");
    peau->mode_rendu=SURFACE_RENDU_PHONG;
    //Surface* base_2=volumes[COSMO_TRONC]->renvoie_surface_nom("base_2");
    //base_2->mode_rendu=SURFACE_RENDU_PHONG;


    ab_tronc=1.;
    ab_tete=1.;
    ab_bras=1.;
    ab_jambes=1.;
    ab_yeux=1.;

    bruit_tronc_r=new Bruit();
    bruit_tronc_t=new Bruit();
    bruit_tronc_l=new Bruit();
    bruit_tete_r=new Bruit();
    bruit_tete_t=new Bruit();
    bruit_tete_l=new Bruit();
    bruit_yeux_t=new Bruit();
    bruit_yeux_l=new Bruit();

    bruit_epauleG_r=new Bruit();
    bruit_epauleG_t=new Bruit();
    bruit_epauleG_l=new Bruit();
    bruit_epauleD_r=new Bruit();
    bruit_epauleD_t=new Bruit();
    bruit_epauleD_l=new Bruit();
    bruit_avantBrasD_l=new Bruit();
    bruit_avantBrasG_l=new Bruit();
    bruit_mainD_r=new Bruit();
    bruit_mainD_t=new Bruit();
    bruit_mainD_l=new Bruit();
    bruit_mainG_r=new Bruit();
    bruit_mainG_t=new Bruit();
    bruit_mainG_l=new Bruit();

    bruit_jambeD_r=new Bruit();
    bruit_jambeD_t=new Bruit();
    bruit_jambeD_l=new Bruit();
    bruit_jambeG_r=new Bruit();
    bruit_jambeG_t=new Bruit();
    bruit_jambeG_l=new Bruit();
    bruit_moletD_t=new Bruit();
    bruit_moletG_t=new Bruit();
    bruit_piedD_r=new Bruit();
    bruit_piedD_t=new Bruit();
    bruit_piedD_l=new Bruit();
    bruit_piedG_r=new Bruit();
    bruit_piedG_t=new Bruit();
    bruit_piedG_l=new Bruit();

}

//===================================
//      Destructeur
//===================================
Cosmonaute::~Cosmonaute()
{

    if(volumesConv!=NULL) delete volumesConv;
    if(objetLWO!=NULL) delete objetLWO;

    if(bruit_tronc_r!=NULL)delete bruit_tronc_r;
    if(bruit_tronc_t!=NULL)delete bruit_tronc_t;
    if(bruit_tronc_l!=NULL)delete bruit_tronc_l;
    if(bruit_tete_r!=NULL)delete bruit_tete_r;
    if(bruit_tete_t!=NULL)delete bruit_tete_t;
    if(bruit_tete_l!=NULL)delete bruit_tete_l;
    if(bruit_yeux_t!=NULL)delete bruit_yeux_t;
    if(bruit_yeux_l!=NULL)delete bruit_yeux_l;

    if(bruit_epauleG_r!=NULL)delete bruit_epauleG_r;
    if(bruit_epauleG_t!=NULL)delete bruit_epauleG_t;
    if(bruit_epauleG_l!=NULL)delete bruit_epauleG_l;
    if(bruit_epauleD_r!=NULL)delete bruit_epauleD_r;
    if(bruit_epauleD_t!=NULL)delete bruit_epauleD_t;
    if(bruit_epauleD_l!=NULL)delete bruit_epauleD_l;
    if(bruit_avantBrasD_l!=NULL)delete bruit_avantBrasD_l;
    if(bruit_avantBrasG_l!=NULL)delete bruit_avantBrasG_l;
    if(bruit_mainD_r!=NULL)delete bruit_mainD_r;
    if(bruit_mainD_t!=NULL)delete bruit_mainD_t;
    if(bruit_mainD_l!=NULL)delete bruit_mainD_l;
    if(bruit_mainG_r!=NULL)delete bruit_mainG_r;
    if(bruit_mainG_t!=NULL)delete bruit_mainG_t;
    if(bruit_mainG_l!=NULL)delete bruit_mainG_l;

    if(bruit_jambeD_r!=NULL)delete bruit_jambeD_r;
    if(bruit_jambeD_t!=NULL)delete bruit_jambeD_t;
    if(bruit_jambeD_l!=NULL)delete bruit_jambeD_l;
    if(bruit_jambeG_r!=NULL)delete bruit_jambeG_r;
    if(bruit_jambeG_t!=NULL)delete bruit_jambeG_t;
    if(bruit_jambeG_l!=NULL)delete bruit_jambeG_l;
    if(bruit_moletD_t!=NULL)delete bruit_moletD_t;
    if(bruit_moletG_t!=NULL)delete bruit_moletG_t;
    if(bruit_piedD_r!=NULL)delete bruit_piedD_r;
    if(bruit_piedD_t!=NULL)delete bruit_piedD_t;
    if(bruit_piedD_l!=NULL)delete bruit_piedD_l;
    if(bruit_piedG_r!=NULL)delete bruit_piedG_r;
    if(bruit_piedG_t!=NULL)delete bruit_piedG_t;
    if(bruit_piedG_l!=NULL)delete bruit_piedG_l;




}

//===================================
//      Initialisations
//===================================
void Cosmonaute::init_listes()
{
    for(uint16_t i=0;i<COSMO_NBR_OBJETS;i++)
    {
        //if(i==COSMO_TETE)volumes[i]->determine_mode_rendu(VOLUME_MODE_IMMEDIAT);
        //else
            volumes[i]->determine_mode_rendu(VOLUME_MODE_LISTE);
    }
}

//===================================
//      Coordonnées
//===================================
void Cosmonaute::determine_position(float px,float py,float pz)
{
    volumes[COSMO_BASSIN]->determine_position(px,py,pz);
}

void Cosmonaute::determine_position(float pos[3])
{
    volumes[COSMO_BASSIN]->determine_position(pos);
}

void Cosmonaute::determine_RTL(float pr,float pt,float pl)
{
    volumes[COSMO_BASSIN]->determine_RTL(pr,pt,pl);
}

void Cosmonaute::determine_RTL(float rtl[3])
{
    volumes[COSMO_BASSIN]->determine_RTL(rtl);
}

//Sert pour les accès directs aux coordonnées
Volume* Cosmonaute::renvoie_volume_parent()
{
    return volumes[COSMO_BASSIN];
}

void Cosmonaute::renvoie_position(float* position)
{
    position[0]=volumes[COSMO_BASSIN]->x;
    position[1]=volumes[COSMO_BASSIN]->y;
    position[2]=volumes[COSMO_BASSIN]->z;
}

void Cosmonaute::renvoie_RTL(float* rtl)
{
    rtl[0]=volumes[COSMO_BASSIN]->r;
    rtl[1]=volumes[COSMO_BASSIN]->t;
    rtl[2]=volumes[COSMO_BASSIN]->l;
}

//===================================================================================
//      Postures
//===================================================================================
void Cosmonaute::debout()
{
    rotation_membre(COSMO_TRONC,0.,0.,0.);
    rotation_membre(COSMO_EPAULE_D,-39.7,4.8,7.3);
    rotation_membre(COSMO_EPAULE_G,39.6,10.,0.7);
    rotation_membre(COSMO_AVANTBRAS_D,0.,7.8,-19.1);
    rotation_membre(COSMO_AVANTBRAS_G,0.,-10.3,18.8);
    rotation_membre(COSMO_MAIN_D,8.8,7.8,0.);
    rotation_membre(COSMO_MAIN_G,-9.1,-10.3,0.);
    rotation_membre(COSMO_JAMBE_D,2.9,0.,9.6);
    rotation_membre(COSMO_JAMBE_G,-6.7,0.,-11.9);
    rotation_membre(COSMO_MOLET_D,0.,0.,0.);
    rotation_membre(COSMO_MOLET_G,0.,0.,0.);
    rotation_membre(COSMO_PIED_D,-2.9,0.,6.5);
    rotation_membre(COSMO_PIED_G,6.9,0.,-7.);
    rotation_membre(COSMO_OEIL_D,0.,0.,0.);
    rotation_membre(COSMO_TETE,0.,0.,0.);

}

//===================================================================================
//      Mixage entre mouvements aléatoires et position debout
//      mix=0 ->debout
//      mix=1 ->mouvements aléatoires
//===================================================================================

void Cosmonaute::mix_debout_mvt_bruits(double temps, float mix, float amplitude_bruits)
{
    int i;
    float rtl[3];
    float debout_RTLs[COSMO_NBR_OBJETS*3];
    float mvtBruits_RTLs[COSMO_NBR_OBJETS*3];
    debout();
    for(i=0;i<COSMO_NBR_OBJETS;i++)
    {
        debout_RTLs[i*3]=volumes[i]->r;
        debout_RTLs[i*3+1]=volumes[i]->t;
        debout_RTLs[i*3+2]=volumes[i]->l;
    }
    mouvement_bruits(temps,amplitude_bruits);

    for(i=0;i<COSMO_NBR_OBJETS;i++)
    {
        mvtBruits_RTLs[i*3]=volumes[i]->r;
        mvtBruits_RTLs[i*3+1]=volumes[i]->t;
        mvtBruits_RTLs[i*3+2]=volumes[i]->l;
    }

    //Interpolation:
    for(i=0;i<COSMO_NBR_OBJETS;i++)
    {
        rtl[0]=debout_RTLs[i*3]*(1.-mix)+mvtBruits_RTLs[i*3]*mix;
        rtl[1]=debout_RTLs[i*3+1]*(1.-mix)+mvtBruits_RTLs[i*3+1]*mix;
        rtl[2]=debout_RTLs[i*3+2]*(1.-mix)+mvtBruits_RTLs[i*3+2]*mix;
        volumes[i]->determine_RTL(rtl);
    }
}

//===================================================================================
//      Mouvements
//  Les angles de roulis et de lacet sont inversés par conformité avec lightwave
//===================================================================================
void Cosmonaute::mouvement_bruits(double temps, float amplitude)
{
    rotation_membre(COSMO_TRONC,
        ab_tronc*amplitude*(bruit_tronc_r->valeur_aleatoire_temporelle(temps,11.54)*45.-22.5),
        ab_tronc*amplitude*(bruit_tronc_t->valeur_aleatoire_temporelle(temps,6.136)*82.5-22.5),
        ab_tronc*amplitude*(bruit_tronc_l->valeur_aleatoire_temporelle(temps,4.4169)*45.-22.5)
        );

    rotation_membre(COSMO_EPAULE_G,
        ab_bras*amplitude*(bruit_epauleG_r->valeur_aleatoire_temporelle(temps,3.41)*80.-30.)+15,
        ab_bras*amplitude*(bruit_epauleG_t->valeur_aleatoire_temporelle(temps,5.123)*90.-45.),
        ab_bras*amplitude*(bruit_epauleG_l->valeur_aleatoire_temporelle(temps,2.752)*72.5-22.5)
        );
    rotation_membre(COSMO_EPAULE_D,
        ab_bras*amplitude*(bruit_epauleD_r->valeur_aleatoire_temporelle(temps,4.111)*80.-50.)-15,
        ab_bras*amplitude*(bruit_epauleD_t->valeur_aleatoire_temporelle(temps,6.325)*90.-45.),
        ab_bras*amplitude*(bruit_epauleD_l->valeur_aleatoire_temporelle(temps,3.37)*72.5-50.)
        );

    rotation_membre(COSMO_AVANTBRAS_D,
        0.,
        0.,
        ab_bras*amplitude*(bruit_avantBrasD_l->valeur_aleatoire_temporelle(temps,1.87931)*110.-110.)
        );
    rotation_membre(COSMO_AVANTBRAS_G,
        0.,
        0.,
        ab_bras*amplitude*(bruit_avantBrasG_l->valeur_aleatoire_temporelle(temps,1.87931)*110.)
        );

    rotation_membre(COSMO_MAIN_D,
        ab_bras*amplitude*(bruit_mainD_r->valeur_aleatoire_temporelle(temps,5.113)*120.-60.),
        ab_bras*amplitude*(bruit_mainD_t->valeur_aleatoire_temporelle(temps,3.456)*90.-45.),
        ab_bras*amplitude*(bruit_mainD_l->valeur_aleatoire_temporelle(temps,4.194)*52.5-22.5)
        );
    rotation_membre(COSMO_MAIN_G,
        ab_bras*amplitude*(bruit_mainG_r->valeur_aleatoire_temporelle(temps,2.413)*120.-60.),
        ab_bras*amplitude*(bruit_mainG_t->valeur_aleatoire_temporelle(temps,5.579)*90.-45.),
        ab_bras*amplitude*(bruit_mainG_l->valeur_aleatoire_temporelle(temps,3.6441)*52.5-30.)
        );

    rotation_membre(COSMO_JAMBE_D,
        ab_jambes*amplitude*(bruit_jambeD_r->valeur_aleatoire_temporelle(temps,4.486)*33.-3.),
        ab_jambes*amplitude*(bruit_jambeD_t->valeur_aleatoire_temporelle(temps,3.110)*140.-80.),
        ab_jambes*amplitude*(bruit_jambeD_l->valeur_aleatoire_temporelle(temps,2.1325)*30.-15.)
        );
    rotation_membre(COSMO_JAMBE_G,
        ab_jambes*amplitude*(bruit_jambeG_r->valeur_aleatoire_temporelle(temps,2.708)*33.-30.),
        ab_jambes*amplitude*(bruit_jambeG_t->valeur_aleatoire_temporelle(temps,4.2867)*140.-80.),
        ab_jambes*amplitude*(bruit_jambeG_l->valeur_aleatoire_temporelle(temps,5.648961)*30.-15.)
        );

    rotation_membre(COSMO_MOLET_D,
        0.,
        ab_jambes*amplitude*(bruit_moletD_t->valeur_aleatoire_temporelle(temps,3.267)*100.),
        0.
        );
    rotation_membre(COSMO_MOLET_G,
        0.,
        ab_jambes*amplitude*(bruit_moletG_t->valeur_aleatoire_temporelle(temps,4.123)*100.),
        0.
        );

    rotation_membre(COSMO_PIED_D,
        ab_jambes*amplitude*(bruit_piedD_r->valeur_aleatoire_temporelle(temps,3.731)*20.-15.),
        ab_jambes*amplitude*(bruit_piedD_t->valeur_aleatoire_temporelle(temps,2.9723)*60.-30.),
        ab_jambes*amplitude*(bruit_piedD_l->valeur_aleatoire_temporelle(temps,4.62222)*30.-15.)
        );
    rotation_membre(COSMO_PIED_G,
        ab_jambes*amplitude*(bruit_piedG_r->valeur_aleatoire_temporelle(temps,4.468)*20.-5.),
        ab_jambes*amplitude*(bruit_piedG_t->valeur_aleatoire_temporelle(temps,5.2364)*60.-30.),
        ab_jambes*amplitude*(bruit_piedG_l->valeur_aleatoire_temporelle(temps,2.16974)*30.-15.)
        );

    rotation_membre(COSMO_OEIL_D,
        0.,
        ab_yeux*amplitude*(bruit_yeux_t->valeur_aleatoire_temporelle(temps,4.21/2.,-32.)*24.-12.),
        ab_yeux*amplitude*(bruit_yeux_l->valeur_aleatoire_temporelle(temps,2.7214/2.,-32.)*40.-20.)
        );

    rotation_membre(COSMO_TETE,
        0., //ab_tete*amplitude*(bruit_tete_r->valeur_aleatoire_temporelle(temps,5.12/2.,8)*10.-5.),
        ab_tete*amplitude*(bruit_tete_t->valeur_aleatoire_temporelle(temps,4.21/2.,bruit_yeux_t->borne_0,bruit_yeux_t->borne_1,-4)*12.-6.),
        ab_tete*amplitude*(bruit_tete_l->valeur_aleatoire_temporelle(temps,2.7214/2.,bruit_yeux_l->borne_0,bruit_yeux_l->borne_1,-4)*22.-11.)
        );


}

void Cosmonaute::RTL_limites(float rtl[3],float limites[6])
{
    for(uint8_t i=0; i<3;i++)
    {
       if(rtl[i]<limites[2*i])rtl[i]=limites[2*i];
        else if(rtl[i]>limites[2*i+1])rtl[i]=limites[2*i+1];
    }
}

void Cosmonaute::rotation_membre(uint16_t membreId,float r, float t, float l)
{
    float rtl[3];
    rtl[0]=r;
    rtl[1]=t;
    rtl[2]=l;
    rotation_membre(membreId,rtl);
}
void Cosmonaute::rotation_membre(uint16_t membreId,float rtl[3])
{
    float tronc_R_min=-22.5;
    float tronc_R_max=22.5;
    float tronc_T_min=-22.5;
    float tronc_T_max=60.;
    float tronc_L_min=-22.5;
    float tronc_L_max=22.5;

    float tete_R_min=-5.;
    float tete_R_max=5.;
    float tete_T_min=-6.;
    float tete_T_max=6.;
    float tete_L_min=-11.;
    float tete_L_max=11.;

    float epauleD_R_min=-50.;
    float epauleD_R_max=30.;
    float epauleD_T_min=-45.;
    float epauleD_T_max=45.;
    float epauleD_L_min=-50.;
    float epauleD_L_max=22.5;

    float epauleG_R_min=-30.;
    float epauleG_R_max=50.;
    float epauleG_T_min=-45.;
    float epauleG_T_max=45.;
    float epauleG_L_min=-22.5;
    float epauleG_L_max=50.;

    float avantBrasD_R_min=0.;
    float avantBrasD_R_max=0.;
    float avantBrasD_T_min=-45.;
    float avantBrasD_T_max=45.;
    float avantBrasD_L_min=-110.;
    float avantBrasD_L_max=0.;

    float avantBrasG_R_min=0.;
    float avantBrasG_R_max=0.;
    float avantBrasG_T_min=-45.;
    float avantBrasG_T_max=45.;
    float avantBrasG_L_min=0.;
    float avantBrasG_L_max=110.;

    float MainD_R_min=-60.;
    float MainD_R_max=60.;
    float MainD_T_min=-45.;
    float MainD_T_max=45.;
    float MainD_L_min=-22.5;
    float MainD_L_max=30.;

    float MainG_R_min=-60.;
    float MainG_R_max=60.;
    float MainG_T_min=-45.;
    float MainG_T_max=45.;
    float MainG_L_min=-30.;
    float MainG_L_max=22.5;

    float JambeD_R_min=-30.;
    float JambeD_R_max=30.;
    float JambeD_T_min=-80.;
    float JambeD_T_max=60.;
    float JambeD_L_min=-15.;
    float JambeD_L_max=15.;

    float JambeG_R_min=-30.;
    float JambeG_R_max=30.;
    float JambeG_T_min=-80.;
    float JambeG_T_max=60.;
    float JambeG_L_min=-15.;
    float JambeG_L_max=15.;

    float MoletD_R_min=0.;
    float MoletD_R_max=0.;
    float MoletD_T_min=0.;
    float MoletD_T_max=100.;
    float MoletD_L_min=0.;
    float MoletD_L_max=0.;

    float MoletG_R_min=0.;
    float MoletG_R_max=0.;
    float MoletG_T_min=0.;
    float MoletG_T_max=100.;
    float MoletG_L_min=0.;
    float MoletG_L_max=0.;

    float PiedD_R_min=-15.;
    float PiedD_R_max=5.;
    float PiedD_T_min=-30.;
    float PiedD_T_max=30.;
    float PiedD_L_min=-15.;
    float PiedD_L_max=15.;

    float PiedG_R_min=-5.;
    float PiedG_R_max=15.;
    float PiedG_T_min=-30.;
    float PiedG_T_max=30.;
    float PiedG_L_min=-15.;
    float PiedG_L_max=15.;

    float OeilD_R_min=0.;
    float OeilD_R_max=0.;
    float OeilD_T_min=-12.;
    float OeilD_T_max=12.;
    float OeilD_L_min=-20.;
    float OeilD_L_max=20.;

    float OeilG_R_min=0.;
    float OeilG_R_max=0.;
    float OeilG_T_min=-12.;
    float OeilG_T_max=12.;
    float OeilG_L_min=-20.;
    float OeilG_L_max=20.;

    float limites[6];


    switch (membreId)
    {
        case COSMO_TRONC:
                limites[0]=tronc_R_min; limites[1]=tronc_R_max;
                limites[2]=tronc_T_min; limites[3]=tronc_T_max;
                limites[4]=tronc_L_min; limites[5]=tronc_L_max;
            break;
        case COSMO_TETE:
                limites[0]=tete_R_min; limites[1]=tete_R_max;
                limites[2]=tete_T_min; limites[3]=tete_T_max;
                limites[4]=tete_L_min; limites[5]=tete_L_max;
            break;
        case COSMO_EPAULE_D:
                limites[0]=epauleD_R_min; limites[1]=epauleD_R_max;
                limites[2]=epauleD_T_min; limites[3]=epauleD_T_max;
                limites[4]=epauleD_L_min; limites[5]=epauleD_L_max;
            break;
        case COSMO_EPAULE_G:
                limites[0]=epauleG_R_min; limites[1]=epauleG_R_max;
                limites[2]=epauleG_T_min; limites[3]=epauleG_T_max;
                limites[4]=epauleG_L_min; limites[5]=epauleG_L_max;
            break;
        case COSMO_AVANTBRAS_D:
                limites[0]=avantBrasD_R_min; limites[1]=avantBrasD_R_max;
                limites[2]=avantBrasD_T_min; limites[3]=avantBrasD_T_max;
                limites[4]=avantBrasD_L_min; limites[5]=avantBrasD_L_max;
                rtl[1]=volumes[COSMO_MAIN_D]->t;
            break;
        case COSMO_AVANTBRAS_G:
                limites[0]=avantBrasG_R_min; limites[1]=avantBrasG_R_max;
                limites[2]=avantBrasG_T_min; limites[3]=avantBrasG_T_max;
                limites[4]=avantBrasG_L_min; limites[5]=avantBrasG_L_max;
                rtl[1]=volumes[COSMO_MAIN_G]->t;
            break;
        case COSMO_MAIN_D:
                limites[0]=MainD_R_min; limites[1]=MainD_R_max;
                limites[2]=MainD_T_min; limites[3]=MainD_T_max;
                limites[4]=MainD_L_min; limites[5]=MainD_L_max;
            break;
        case COSMO_MAIN_G:
                limites[0]=MainG_R_min; limites[1]=MainG_R_max;
                limites[2]=MainG_T_min; limites[3]=MainG_T_max;
                limites[4]=MainG_L_min; limites[5]=MainG_L_max;
            break;
        case COSMO_JAMBE_D:
                limites[0]=JambeD_R_min; limites[1]=JambeD_R_max;
                limites[2]=JambeD_T_min; limites[3]=JambeD_T_max;
                limites[4]=JambeD_L_min; limites[5]=JambeD_L_max;
            break;
        case COSMO_JAMBE_G:
                limites[0]=JambeG_R_min; limites[1]=JambeG_R_max;
                limites[2]=JambeG_T_min; limites[3]=JambeG_T_max;
                limites[4]=JambeG_L_min; limites[5]=JambeG_L_max;
            break;
        case COSMO_MOLET_D:
                limites[0]=MoletD_R_min; limites[1]=MoletD_R_max;
                limites[2]=MoletD_T_min; limites[3]=MoletD_T_max;
                limites[4]=MoletD_L_min; limites[5]=MoletD_L_max;
            break;
        case COSMO_MOLET_G:
                limites[0]=MoletG_R_min; limites[1]=MoletG_R_max;
                limites[2]=MoletG_T_min; limites[3]=MoletG_T_max;
                limites[4]=MoletG_L_min; limites[5]=MoletG_L_max;
            break;
        case COSMO_PIED_D:
                limites[0]=PiedD_R_min; limites[1]=PiedD_R_max;
                limites[2]=PiedD_T_min; limites[3]=PiedD_T_max;
                limites[4]=PiedD_L_min; limites[5]=PiedD_L_max;
            break;
        case COSMO_PIED_G:
                limites[0]=PiedG_R_min; limites[1]=PiedG_R_max;
                limites[2]=PiedG_T_min; limites[3]=PiedG_T_max;
                limites[4]=PiedG_L_min; limites[5]=PiedG_L_max;
            break;
        case COSMO_OEIL_D:
                limites[0]=OeilD_R_min; limites[1]=OeilD_R_max;
                limites[2]=OeilD_T_min; limites[3]=OeilD_T_max;
                limites[4]=OeilD_L_min; limites[5]=OeilD_L_max;
            break;
        case COSMO_OEIL_G:
                limites[0]=OeilG_R_min; limites[1]=OeilG_R_max;
                limites[2]=OeilG_T_min; limites[3]=OeilG_T_max;
                limites[4]=OeilG_L_min; limites[5]=OeilG_L_max;
            break;
        default: return; break;

    }
    RTL_limites(rtl,limites);
    volumes[membreId]->determine_RTL(-rtl[0],rtl[1],-rtl[2]);

    //Asservissements:
    switch (membreId)
    {
        case COSMO_EPAULE_D:
                volumes[COSMO_BRAS_D]->determine_RTL(-rtl[0],rtl[1],-rtl[2]);
            break;
        case COSMO_EPAULE_G:
                volumes[COSMO_BRAS_G]->determine_RTL(-rtl[0],rtl[1],-rtl[2]);
            break;
        case COSMO_MAIN_D:
                rtl[0]=volumes[COSMO_AVANTBRAS_D]->r;
                rtl[2]=volumes[COSMO_AVANTBRAS_D]->l;
                volumes[COSMO_AVANTBRAS_D]->determine_RTL(rtl[0],rtl[1],rtl[2]);
            break;
        case COSMO_MAIN_G:
                rtl[0]=volumes[COSMO_AVANTBRAS_G]->r;
                rtl[2]=volumes[COSMO_AVANTBRAS_G]->l;
                volumes[COSMO_AVANTBRAS_G]->determine_RTL(rtl[0],rtl[1],rtl[2]);
            break;
        case COSMO_OEIL_D:
                volumes[COSMO_OEIL_G]->determine_RTL(0.,rtl[1],-rtl[2]);
    }

}

void Cosmonaute::raz()
{
    for (int i=0;i<COSMO_NBR_OBJETS;i++)
    {
        volumes[i]->determine_RTL(0.,0.,0.);
    }
}

void Cosmonaute::determine_echelle(float e)
{
    echelle=e;
    volumes[COSMO_BASSIN]->determine_echelle(e,e,e);
}

//===================================
//      Affichage
//===================================
void Cosmonaute::eclairages()
{
    glEnable(GL_LIGHT2);
    Lumieres::drapeaux_sources|=0b100;
    Lumieres::lumiere_attenuation(GL_LIGHT2,0.0,0.1/echelle,0.);
    Lumieres::lumiere_couleur(GL_LIGHT2,
                            0.,0.,0.,
                            0.1,0.1,0.005,
                            0.,0.,0.);

    Lumieres::lumiere_ouverture(GL_LIGHT2,38.);
    Lumieres::lumiere_position(GL_LIGHT2,.620303-volumes[COSMO_TRONC]->x,6.4782-volumes[COSMO_TRONC]->y,1.8427-volumes[COSMO_TRONC]->z);
}

void Cosmonaute::affiche(float obs[3])
{
    //volumes[COSMO_EPAULE_D]->determine_RTL(0.,0.,22.);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        volumes[COSMO_BASSIN]->calcul_matrice();
        volumes[COSMO_BASSIN]->affiche(obs);
        glPushMatrix();
            volumes[COSMO_TRONC]->calcul_matrice();
                if(drapeau_affiche_visage)
                {
                    eclairages();
                    glPushMatrix();
                        glRotatef(213.5,0.,1.,0.);
                        glRotatef(-31.6,1.,0.,0.);
                        glRotatef(2.6,0.,0.,1.);
                        Lumieres::lumiere_directionnelle_orientation(GL_LIGHT2,0.,0.,1.);
                    glPopMatrix();
                }

            volumes[COSMO_TRONC]->affiche(obs);
            if(drapeau_affiche_visage)
                {
                    glPushMatrix();
                        volumes[COSMO_TETE]->calcul_matrice();
                        volumes[COSMO_TETE]->affiche(obs);
                        glPushMatrix();
                            volumes[COSMO_OEIL_D]->calcul_matrice();
                            volumes[COSMO_OEIL_D]->affiche(obs);
                        glPopMatrix();
                        glPushMatrix();
                            volumes[COSMO_OEIL_G]->calcul_matrice();
                            volumes[COSMO_OEIL_G]->affiche(obs);
                        glPopMatrix();
                    glPopMatrix();
                    glDisable(GL_LIGHT2);
                    Lumieres::drapeaux_sources&=0b11111011;
                }
            glPushMatrix();
                volumes[COSMO_EPAULE_D]->calcul_matrice();
                volumes[COSMO_EPAULE_D]->affiche(obs);
                    volumes[COSMO_BRAS_D]->calcul_matrice();
                    volumes[COSMO_BRAS_D]->affiche(obs);
                        volumes[COSMO_AVANTBRAS_D]->calcul_matrice();
                        volumes[COSMO_AVANTBRAS_D]->affiche(obs);
                            volumes[COSMO_MAIN_D]->calcul_matrice();
                            volumes[COSMO_MAIN_D]->affiche(obs);
            glPopMatrix();
            glPushMatrix();
                volumes[COSMO_EPAULE_G]->calcul_matrice();
                volumes[COSMO_EPAULE_G]->affiche(obs);
                    volumes[COSMO_BRAS_G]->calcul_matrice();
                    volumes[COSMO_BRAS_G]->affiche(obs);
                        volumes[COSMO_AVANTBRAS_G]->calcul_matrice();
                        volumes[COSMO_AVANTBRAS_G]->affiche(obs);
                            volumes[COSMO_MAIN_G]->calcul_matrice();
                            volumes[COSMO_MAIN_G]->affiche(obs);
            glPopMatrix();
        glPopMatrix();
        glPushMatrix();
            volumes[COSMO_JAMBE_D]->calcul_matrice();
            volumes[COSMO_JAMBE_D]->affiche(obs);
                volumes[COSMO_MOLET_D]->calcul_matrice();
                volumes[COSMO_MOLET_D]->affiche(obs);
                    volumes[COSMO_PIED_D]->calcul_matrice();
                    volumes[COSMO_PIED_D]->affiche(obs);
        glPopMatrix();
        glPushMatrix();
            volumes[COSMO_JAMBE_G]->calcul_matrice();
            volumes[COSMO_JAMBE_G]->affiche(obs);
                volumes[COSMO_MOLET_G]->calcul_matrice();
                volumes[COSMO_MOLET_G]->affiche(obs);
                    volumes[COSMO_PIED_G]->calcul_matrice();
                    volumes[COSMO_PIED_G]->affiche(obs);
        glPopMatrix();
        //Objets transparents:
        glPushMatrix();
                volumes[COSMO_TRONC]->calcul_matrice();
                volumes[COSMO_VISIERE]->calcul_matrice();
                volumes[COSMO_VISIERE]->affiche(obs);
        glPopMatrix();
    glPopMatrix();

}
