#include "Agnomes.h"

Agnomes::Agnomes(int l_ecran, int h_ecran,float p_horiz,uint32_t resolution_reference_x,uint32_t resolution_reference_y)
{
    erreur=AGNOMES_OK;
    const char nomFichier[]="Images/Agnome_polyspheriqueb.tif";
    const char* fichier[]={nomFichier};
    sprite=new Sprites_2d(1,fichier);
    if(sprite==NULL || sprite->erreur!=SPR2D_OK) erreur=AGNOMES_ERREUR_SPRITE;
    sprite->determine_resolution_reference_XY(resolution_reference_x,resolution_reference_y);
    sprite->determine_centre_sprite(0,false);
    sprite->determine_position_centre_sprite(0,0.5,0.033128834);//0.805558);
    position_horizon=p_horiz;
    hauteur_ecran=h_ecran;
    largeur_ecran=l_ecran;
    t_prec=0.;

    xpos_min=0.;
    xpos_max=1.;

    distance_fondu=0.2;
    position_apparitions=0.;
    position_disparitions=0.;

    random_agnomes();
}

Agnomes::~Agnomes()
{
    if(sprite!=NULL)delete sprite;
}

static int compare(const void* a,const void* b)
{
    if(*(float*)a < *(float*)b) return -1;
    if(*(float*)a > *(float*)b) return 1;
    else return 0;
}

void Agnomes::random_agnomes()
{
    int i;
    float valeurs[NUM_AGNOMES*2];
    for (i=0;i<NUM_AGNOMES;i++)
    {
        foule[i].amplitude_oscillation=float(rand())/32768.* (AGNOMES_AMPLITUDE_MAX-AGNOMES_AMPLITUDE_MIN) + AGNOMES_AMPLITUDE_MIN;
        foule[i].vitesse_oscillation=float(rand())/32768. * (AGNOMES_VITESSE_MAX-AGNOMES_VITESSE_MIN) + AGNOMES_VITESSE_MIN;
        foule[i].echelle=1./(float(rand())/32768.*(AGNOMES_DIST_MAX-AGNOMES_DIST_MIN)+AGNOMES_DIST_MIN);
        foule[i].xPos=float(rand())/32768.*(xpos_max-xpos_min)+xpos_min;
        foule[i].angle_oscillation=(float(rand())/32768.-0.5)*M_PI;
        valeurs[2*i]=foule[i].echelle;
        valeurs[2*i+1]=foule[i].xPos;
    }
    qsort(valeurs,NUM_AGNOMES,sizeof(float)*2,compare);
    for(i=0;i<NUM_AGNOMES;i++)
    {
       foule[i].echelle=valeurs[2*i];
       foule[i].xPos=valeurs[2*i+1];
    }

    foule[NUM_AGNOMES-1].echelle=1.;
    foule[NUM_AGNOMES-1].xPos=5./7.;



}

void Agnomes::determine_resolution_reference(uint32_t rx,uint32_t ry)
{
    sprite->determine_resolution_reference_XY(rx,ry);
}

void Agnomes::affiche(float t)
{

    int i;
    float angle;
    for (i=0;i<NUM_AGNOMES;i++)
    {
        foule[i].angle_oscillation+=M_PI*foule[i].vitesse_oscillation*(t-t_prec);
        angle=foule[i].amplitude_oscillation*cos(foule[i].angle_oscillation);
        sprite->determine_echelle_sprite(0,foule[i].echelle);
        sprite->determine_position_sprite(0,foule[i].xPos,position_horizon-0.92*foule[i].echelle);

        /*
        if(i!=(NUM_AGNOMES-1))
        {
            sprite->determine_echelle_Y_sprite(0,1+0.05*cos(foule[i].angle_oscillation*2.));
            sprite->determine_rotation_sprite(0,angle);
        }
        else
        {
            sprite->determine_echelle_Y_sprite(0,1.);
            sprite->determine_rotation_sprite(0,0.);
        }
        */
        sprite->determine_rotation_sprite(0,angle);

        float position_sprite=(foule[i].xPos-xpos_min)/(xpos_max-xpos_min);

        //Apparition:
        float position_fondu=(position_sprite-((position_apparitions*(1.+distance_fondu)-distance_fondu)))/distance_fondu;
        float facteur;
        if(position_fondu<0.)facteur=1.;
        else if(position_fondu>1.)facteur=0.;
        else facteur=1.-position_fondu;
        //Disparition:
        position_fondu=(position_sprite-(position_disparitions*(1.+distance_fondu)-distance_fondu))/distance_fondu;
        if(position_fondu<0.)facteur=0.;
        else if(position_fondu>1.)facteur=facteur;
        else facteur=position_fondu;


        sprite->determine_echelle_Y_sprite(0,(sin(facteur*M_PI/2.)));//*sprite->renvoie_echelle_Y_sprite(0));
        sprite->determine_alpha_sprite(0,facteur);


        sprite->affiche_sprites_2d();
    }

    t_prec=t;


}
