#include "Credits_Greets.h"

#define CREDITS_CODE 29
#define CREDITS_GFX 26
#define CREDITS_MODELING 27
#define CREDITS_MUSICS 28

bool Credits_Greets::drapeau_init_greets=false;
bool Credits_Greets::drapeau_init_credits=false;
float Credits_Greets::t0;

void Credits_Greets::affiche_credits()
{
    if(!drapeau_init_credits)
    {
        drapeau_init_credits=true;
        t0=GVars::temps;

    }
    uint16_t tableau_credits[4]={CREDITS_CODE,CREDITS_GFX,CREDITS_MUSICS,CREDITS_MODELING};

    int nbrCredits=4;
    float beat=60./125.;
    float duree_fade_in=2.*beat;
    float duree_in=3.*beat;
    float duree_fade_out=2.*beat;
    float duree_entre_groupes=5.*beat;
    float cx=0.5;
    float cy=2./3.;//0.1284;
    float r_t0=0.;
    float r_max=0.;
    float alpha_max=1.;
    float rot_max=0.;
    float zoom_min=1.;
    float zoom_max=0.;
    float facteur_repartition=2.;

    float duree_mvt=duree_fade_in+duree_in+duree_fade_out;

    float t=GVars::temps-t0;
    for(int i=0;i<nbrCredits;i++)
    {
        uint16_t indice=tableau_credits[i];
        float t0Sprite=duree_entre_groupes*float(i);
        if(t<t0Sprite || t>t0Sprite+duree_fade_in+duree_in+duree_fade_out)
        {GVars::credits_greets->cache_sprite(indice);continue;}

        float a=360./float(nbrCredits)*float(i)*DEG_RAD*facteur_repartition;
        float facteur_mvt=Interpolations::interpolation_cosinusoidale_f(0.,1.,(t-t0Sprite)/duree_mvt);
        float r=r_t0+(r_max-r_t0)*facteur_mvt;
        GVars::credits_greets->determine_position_sprite(indice,cx+r*cos(a),cy+r*sin(a));
        GVars::credits_greets->determine_echelle_sprite(indice,zoom_min+zoom_max*facteur_mvt);
        GVars::credits_greets->determine_rotation_sprite(indice,-cos(a)*rot_max*facteur_mvt);

        GVars::credits_greets->montre_sprite(indice);
        if (t<t0Sprite+duree_fade_in)
        {
            GVars::credits_greets->determine_alpha_sprite(indice,((t-t0Sprite)/duree_fade_in)*alpha_max);
        }
        else if(t>t0Sprite+duree_fade_in+duree_in)
        {
            GVars::credits_greets->determine_alpha_sprite(indice,(1.-(t-t0Sprite-duree_fade_in-duree_in)/duree_fade_out)*alpha_max);
        }
    }
    GVars::credits_greets->affiche_sprites_2d();
}

void Credits_Greets::affiche_greets()
{
    if(!drapeau_init_greets)
    {
        drapeau_init_greets=true;
        t0=GVars::temps;

    }
    int nbrGroupes=26;
    float beat=60./125.;
    float duree_fade_in=2.*beat;
    float duree_in=1.*beat;
    float duree_fade_out=5.*beat;
    float duree_entre_groupes=1.*beat;
    float cx=0.33;
    float cy=0.5;
    float r_t0=0.05;
    float r_max=.2;
    float facteur_repartition=87.256;
    float alpha_max=0.75;
    float rot_max=5.;
    float zoom_min=0.8;
    float zoom_max=1.1;

    float duree_mvt=duree_fade_in+duree_in+duree_fade_out;

    float t=GVars::temps-t0;
    for(int i=0;i<nbrGroupes;i++)
    {
        float t0Sprite=duree_entre_groupes*float(i);
        if(t<t0Sprite || t>t0Sprite+duree_fade_in+duree_in+duree_fade_out)
        {GVars::credits_greets->cache_sprite(i);continue;}

        float a=360./float(nbrGroupes)*float(i)*DEG_RAD*facteur_repartition;
        float facteur_mvt=Interpolations::interpolation_cosinusoidale_f(0.,1.,(t-t0Sprite)/duree_mvt);
        float r=r_t0+(r_max-r_t0)*facteur_mvt;
        GVars::credits_greets->determine_position_sprite(i,cx+r*cos(a),cy+r*sin(a));
        GVars::credits_greets->determine_echelle_sprite(i,zoom_min+zoom_max*facteur_mvt);
        GVars::credits_greets->determine_rotation_sprite(i,-cos(a)*rot_max*facteur_mvt);

        GVars::credits_greets->montre_sprite(i);
        if (t<t0Sprite+duree_fade_in)
        {
            GVars::credits_greets->determine_alpha_sprite(i,((t-t0Sprite)/duree_fade_in)*alpha_max);
        }
        else if(t>t0Sprite+duree_fade_in+duree_in)
        {
            GVars::credits_greets->determine_alpha_sprite(i,(1.-(t-t0Sprite-duree_fade_in-duree_in)/duree_fade_out)*alpha_max);
        }
    }
    GVars::credits_greets->affiche_sprites_2d();

    /*
    GVars::credits_greets->cache();
    uint16_t indice=CREDITS_GFX;
    GVars::credits_greets->montre_sprite(indice);
    GVars::credits_greets->determine_position_sprite(indice,0.5,0.5);
    GVars::credits_greets->determine_position_centre_sprite(indice,0.,0.);
    GVars::credits_greets->determine_echelle_sprite(indice,1.);

    GVars::credits_greets->determine_proportionnel_sprite(indice,true);
    GVars::credits_greets->determine_centre_sprite(indice,true);
    //GVars::credits_greets->affiche_sprites_2d();
    GVars::credits_greets->cache();*/
}
