
#include "WaterWorld.h"

bool WaterWorld::drapeau_affiche_ville=false;
bool WaterWorld::drapeau_affiche_Interstase=false;
bool WaterWorld::drapeau_affiche_nuees=false;
//Paramètres
int32_t WaterWorld::numReflec=3;
int32_t WaterWorld::numRefrac=1;

float WaterWorld::vitesse_ondulations=0.1;

//Observateur
Repere WaterWorld::obs;

float WaterWorld::aX=0.;
float WaterWorld::aY=0.;
float WaterWorld::aZ=0.;

float WaterWorld::aR=0.;
float WaterWorld::aT=0.;
float WaterWorld::aL=0.;

//Ciel:
float WaterWorld::distance_brouillard=500.;
float WaterWorld::C_ciel[TAILLE_CIEL]={
						17./255.,56./255.,155./255.,    //11389b
						76./255.,128./255.,255./255., //4d80ff
						76./255.,128./255.,255./255.,
						19./255.,39./255.,89./255. //132759
						};
//Lumière ambiante:
float WaterWorld::L_amb=0.2;
float WaterWorld::C_amb[3]={0.3,0.5,1.}; //4c80ff

//Lumière ponctuelle:
float WaterWorld::L1[TAILLE_LUM];


//Sol:
float WaterWorld::amplitude_vagues=1.;
float WaterWorld::Sol[TAILLE_OBJ];

//Sphères:

float WaterWorld::S1[TAILLE_OBJ];
float WaterWorld::S2[TAILLE_OBJ];
float WaterWorld::S3[TAILLE_OBJ];
float WaterWorld::S4[TAILLE_OBJ];
float WaterWorld::S5[TAILLE_OBJ];
float WaterWorld::S6[TAILLE_OBJ];
float WaterWorld::S7[TAILLE_OBJ];
float WaterWorld::S8[TAILLE_OBJ];
float WaterWorld::S9[TAILLE_OBJ];
float WaterWorld::S10[TAILLE_OBJ];
float WaterWorld::S11[TAILLE_OBJ];
float WaterWorld::S12[TAILLE_OBJ];




//Mouvements:
float WaterWorld::obsX=0.;
float WaterWorld::obsY=29.5;
float WaterWorld::obsZ=162.;
float WaterWorld::obsR0=0.;
float WaterWorld::obsT0=100.;
float WaterWorld::obsL0=0.;

float WaterWorld::xS1=0.;
float WaterWorld::yS1=3.6;
float WaterWorld::zS1=303.6;
float WaterWorld::rS1=200.;

float WaterWorld::xS2=-25.;
float WaterWorld::yS2=WaterWorld::yS8;
float WaterWorld::zS2=20.;
float WaterWorld::rS2=2.;
float WaterWorld::orbS2=25.;
float WaterWorld::vOrbS2=2.134;

float WaterWorld::xS3=-25.;
float WaterWorld::yS3=WaterWorld::yS8;
float WaterWorld::zS3=30.;
float WaterWorld::rS3=WaterWorld::rS2*NOMBREDOR;
float WaterWorld::orbS3=WaterWorld::orbS2*NOMBREDOR;
float WaterWorld::vOrbS3=vOrbS2/NOMBREDOR;

float WaterWorld::xS4=-25.;
float WaterWorld::yS4=WaterWorld::yS8;
float WaterWorld::zS4=20.;
float WaterWorld::rS4=WaterWorld::rS3*NOMBREDOR;
float WaterWorld::orbS4=WaterWorld::orbS3*NOMBREDOR;
float WaterWorld::vOrbS4=vOrbS3/NOMBREDOR;

float WaterWorld::xS5=-25.;
float WaterWorld::yS5=WaterWorld::yS8;
float WaterWorld::zS5=20.;
float WaterWorld::rS5=WaterWorld::rS4*NOMBREDOR;
float WaterWorld::orbS5=WaterWorld::orbS4*NOMBREDOR;
float WaterWorld::vOrbS5=vOrbS4/NOMBREDOR;

float WaterWorld::xS6=-25.;
float WaterWorld::yS6=WaterWorld::yS8;
float WaterWorld::zS6=20.;
float WaterWorld::rS6=WaterWorld::rS5*NOMBREDOR;
float WaterWorld::orbS6=WaterWorld::orbS5*NOMBREDOR;
float WaterWorld::vOrbS6=vOrbS5/NOMBREDOR;

float WaterWorld::S2S6_angle_t0=1.05;

float WaterWorld::xS7=33.;
float WaterWorld::yS7=13.;
float WaterWorld::zS7=25.;
float WaterWorld::rS7=20.;
float WaterWorld::orbS7=110.;
float WaterWorld::S7_angle_t0=2.786;
float WaterWorld::S7_epsilon_Vangle=0.36;

float WaterWorld::xS8=2.0417;//33.;
float WaterWorld::yS8=242.509;//38.;
float WaterWorld::zS8=229.786;//25.;
float WaterWorld::rS8=12.;//25.;

float WaterWorld::xS9=33.;
float WaterWorld::yS9=61.;
float WaterWorld::zS9=25.;
float WaterWorld::rS9=11.;

float WaterWorld::objets[TAILLE_OBJ*WW_NUM_OBJETS];	//Le tableau contenant les objets de la scène

float WaterWorld::rebondS1=0.;
float WaterWorld::rotxS1=0.;
float WaterWorld::rotzS1=0.;

float WaterWorld::angleS2=0.;
float WaterWorld::angleS3=0.3;
float WaterWorld::angleS4=0.6;
float WaterWorld::angleS5=0.9;
float WaterWorld::angleS6=1.2;
float WaterWorld::angleS7=0.;

//Sprite Edora:
bool WaterWorld::drapeau_affiche_Edora=false;
float WaterWorld::xEdora=0.5;
float WaterWorld::yEdora=0.572222;
float WaterWorld::eEdora=1.;
float WaterWorld::alphaEdora=0.;

//Sprites Agnomes Polysphériques:
Agnomes* WaterWorld::agnomes=NULL;
bool WaterWorld::drapeau_affiche_agnomes=false;

//Cosmonaute:
parametres_reflet_eau WaterWorld::params_reflet_cosmo;
float WaterWorld::altitude_eau_cosmo=-120.;
//Commandes clavier:
bool WaterWorld::inc_alt_reflet;
bool WaterWorld::dec_alt_reflet;

bool WaterWorld::dep_droite=false;
bool WaterWorld::dep_gauche=false;
bool WaterWorld::dep_haut=false;
bool WaterWorld::dep_bas=false;
bool WaterWorld::dep_avant=false;
bool WaterWorld::dep_arriere=false;

bool WaterWorld::rot_droite=false;
bool WaterWorld::rot_gauche=false;
bool WaterWorld::rot_haut=false;
bool WaterWorld::rot_bas=false;
bool WaterWorld::rot_r_gauche=false;
bool WaterWorld::rot_r_droite=false;

bool WaterWorld::numReflec_0=false;
bool WaterWorld::numReflec_1=false;
bool WaterWorld::numReflec_2=false;
bool WaterWorld::numReflec_3=false;

bool WaterWorld::numRefrac_0=false;
bool WaterWorld::numRefrac_1=false;
bool WaterWorld::numRefrac_2=false;
bool WaterWorld::numRefrac_3=false;

int WaterWorld::depId=1;


void WaterWorld::detruit_scene_waterworld()
{
    if(agnomes!=NULL)delete agnomes;
}

bool WaterWorld::init_scene_waterworld()
{

    agnomes=new Agnomes(GVars::largeur_ecran,GVars::hauteur_ecran,242./720.,1280,720);
    if(agnomes==NULL || agnomes->erreur!=AGNOMES_OK) return false;

    obs.initXYZ(obsX,obsY,obsZ);
    obs.initRTL(obsR0,obsT0,obsL0);

    //Raytracer01::init_Sol(Sol,10.,10.,     51./255.,179./255.,102./255.,   26./255.,89./255.,51./255.,     1.,0.25,4.,     0.5,0.75,1.333 );
    Raytracer01::init_C_amb(C_amb,0x4c80ff);
    Raytracer01::init_ciel(C_ciel,0x11389b,0x4c80ff,0x4c80ff,0x132759);
    Raytracer01::init_Sol(Sol,10.,10.,0x33b366,0x1a5933,     1.,0.25,4.,     0.5,0.75,1.333 );

	Raytracer01::init_lum(L1,-50,80.,1.,	1.,1.,1.,	0.,0.001);  //-50 80 1

	Raytracer01::init_sphere(S1,rS1,	xS1,yS1,zS1,	1.,0.1,0.1,	1.,0.5,20.,	.5,1.,0.3);


	Raytracer01::init_sphere(S2,rS2,		xS2,yS2,zS2,	0xfff76c,		1.,1.,200.,	0.9,0.4,1.01);
	Raytracer01::init_sphere(S3,rS3,		xS3,yS3,zS3,	0xffcc65,		1.,.4,50.,		0.9,0.8,1.02);
	Raytracer01::init_sphere(S4,rS4,		xS4,yS4,zS4,	0xffab60,		1.,.5,20.,		0.9,0.9,1.03);
	Raytracer01::init_sphere(S5,rS5,		xS5,yS5,zS5,	0xff8b5b,		1.,1.,30.,	0.5,.6,1.04);
	Raytracer01::init_sphere(S6,rS6,		xS6,yS6,zS6,	0xff8080,		1.,1.,70.,	0.,0.7,1.05);



	Raytracer01::init_sphere(S7,rS7,		xS7,yS7,zS7,	1.,1.,0.,		1.,1.,200.,	.5,1.,1.333);
	Raytracer01::init_sphere(S8,rS8,		xS8,yS8,zS8,	0.5,.5,1.,		1.,1.,20.,	0.5,0.7,1.01);

	Raytracer01::init_sphere(S9,rS9,		xS9,yS9,zS9,	1.,1.,1.,		1.,1.,40.,	0.,0.7,1.2);

	Raytracer01::init_sphere(S10,10.,		33.,82,25.,	0.,1.,1.,		1.,1.,50.,	0.,0.7,1.005);
	Raytracer01::init_sphere(S11,9.,		33.,101,25.,	1.,0.,1.,		1.,1.,60.,	0.,0.7,1.005);
	Raytracer01::init_sphere(S12,8.,		33.,118,25.,	0.5,1.,0.5,	1.,1.,7.,	0.,0.7,1.005);


    params_reflet_cosmo.transparence=1.;
    params_reflet_cosmo.attenuation_speculaire=200.;
    params_reflet_cosmo.intensite_ondes=0.3;
    params_reflet_cosmo.vitesse_ondes=15.;
    params_reflet_cosmo.quantite_ondes=0.3;
    params_reflet_cosmo.frequence_ondes=.01;
    params_reflet_cosmo.amplitude_ondes=50.;
    params_reflet_cosmo.niveau_perturbations_ondes=5.;
    params_reflet_cosmo.echelle_texture_bruit_ondes=100.;
    params_reflet_cosmo.position_source[0]=L1[POSX]-obs.x;
    params_reflet_cosmo.position_source[1]=L1[POSY]-obs.y;
    params_reflet_cosmo.position_source[2]=-L1[POSZ]+obs.z;
    params_reflet_cosmo.couleur_fond[0]=C_ciel[NADIR_CR];
    params_reflet_cosmo.couleur_fond[1]=C_ciel[NADIR_CV];
    params_reflet_cosmo.couleur_fond[2]=C_ciel[NADIR_CB];
    params_reflet_cosmo.couleur_fond[3]=1.;

    params_reflet_cosmo.brillance=20.;
    params_reflet_cosmo.couleur_speculaire[0]=L1[CR]*L1[INTENS];
    params_reflet_cosmo.couleur_speculaire[1]=L1[CV]*L1[INTENS];
    params_reflet_cosmo.couleur_speculaire[2]=L1[CB]*L1[INTENS];
    params_reflet_cosmo.couleur_speculaire[3]=1.;

	return true;
}

void WaterWorld::clavier(int key,bool etat)
{
    if(!GVars::editionTrajectoire)
    {
		switch (key)
        {
                case GLFW_KEY_KP_ADD: inc_alt_reflet=etat;break;
                case GLFW_KEY_KP_SUBTRACT: dec_alt_reflet=etat;break;
                case GLFW_KEY_RIGHT:dep_droite=etat;break;
                case GLFW_KEY_LEFT:dep_gauche=etat;break;
                case GLFW_KEY_UP:dep_avant=etat ;break;
                case GLFW_KEY_DOWN:dep_arriere=etat;break;
                case GLFW_KEY_PAGE_UP:dep_haut=etat ;break;
                case GLFW_KEY_PAGE_DOWN:dep_bas=etat;break;
                case GLFW_KEY_KP_1:rot_r_gauche=etat;break;
                case GLFW_KEY_KP_3:rot_r_droite=etat;break;
                case GLFW_KEY_KP_4:rot_gauche=etat;break;
                case GLFW_KEY_KP_6:rot_droite=etat;break;
                case GLFW_KEY_KP_8:rot_bas=etat;break;
                case GLFW_KEY_KP_2:rot_haut=etat;break;
                case GLFW_KEY_1: numReflec_0=etat;break;
                case GLFW_KEY_2: numReflec_1=etat;break;
                case GLFW_KEY_3: numReflec_2=etat;break;
                case GLFW_KEY_4: numReflec_3=etat;break;
                case GLFW_KEY_5: numRefrac_0=etat;break;
                case GLFW_KEY_6: numRefrac_1=etat;break;
                case GLFW_KEY_7: numRefrac_2=etat;break;
                case GLFW_KEY_8: numRefrac_3=etat;break;
                case GLFW_KEY_F1: depId=1;break;
                case GLFW_KEY_F2: depId=2;break;
                case GLFW_KEY_F3: depId=3;break;
                case GLFW_KEY_F4: depId=4;break;
                case GLFW_KEY_F5: depId=5;break;
                case GLFW_KEY_F6: depId=6;break;
                case GLFW_KEY_F7: depId=7;break;    //Edora
                case GLFW_KEY_BACKSPACE: GVars::raz_cosmonaute=etat;break;

                case GLFW_KEY_F12: if(etat)GVars::editionTrajectoire=true;break; //Editeur de trajectoire
        }
    }

    //Si on est en mode "édition de trajectoires" :
    else
    {
        Volume* cosmo=GVars::cosmo->renvoie_volume_parent();
        EditeurTrajectoire::ET_x=cosmo->x;
        EditeurTrajectoire::ET_y=cosmo->y;
        EditeurTrajectoire::ET_z=cosmo->z;
        EditeurTrajectoire::edit_trajectoire(key,etat);
        cosmo->x=EditeurTrajectoire::ET_x;
        cosmo->y=EditeurTrajectoire::ET_y;
        cosmo->z=EditeurTrajectoire::ET_z;
        switch (key)
        {
            case GLFW_KEY_F12: if(etat)GVars::editionTrajectoire=false;break;

            case GLFW_KEY_RIGHT:dep_droite=etat;break;
            case GLFW_KEY_LEFT:dep_gauche=etat;break;
            case GLFW_KEY_UP:dep_avant=etat ;break;
            case GLFW_KEY_DOWN:dep_arriere=etat;break;
            case GLFW_KEY_PAGE_UP:dep_haut=etat ;break;
            case GLFW_KEY_PAGE_DOWN:dep_bas=etat;break;
            case GLFW_KEY_KP_1:rot_r_gauche=etat;break;
            case GLFW_KEY_KP_3:rot_r_droite=etat;break;
            case GLFW_KEY_KP_4:rot_gauche=etat;break;
            case GLFW_KEY_KP_6:rot_droite=etat;break;
            case GLFW_KEY_KP_8:rot_bas=etat;break;
            case GLFW_KEY_KP_2:rot_haut=etat;break;
            case GLFW_KEY_1: depId=1;break;
            case GLFW_KEY_2: depId=2;break;
            case GLFW_KEY_3: depId=3;break;
            case GLFW_KEY_4: depId=4;break;
            case GLFW_KEY_5: depId=5;break;
            case GLFW_KEY_6: depId=6;break;
        }
    }

}


void WaterWorld::maj_deplacement()
{
	if(numReflec_0)numReflec=0;
	if(numReflec_1)numReflec=1;
	if(numReflec_2)numReflec=2;
	if(numReflec_3)numReflec=3;

	if(numRefrac_0)numRefrac=0;
	if(numRefrac_1)numRefrac=1;
	if(numRefrac_2)numRefrac=2;
	if(numRefrac_3)numRefrac=3;
	/*
	S1[POSY]=6.+5.*cos(rebondS1); rebondS1+=0.01;
	S1[POSX]=0+15.*cos(rotxS1);rotxS1+=0.0053;
	S1[POSZ]=30+20.*sin(rotzS1);rotzS1+=0.0027;
	*/

	S2[POSX]=S8[POSX]+(orbS2*cos(angleS2));
	S2[POSY]=S8[POSY];
	S2[POSZ]=S8[POSZ]+(orbS2*sin(angleS2));
	angleS2=vOrbS2*GVars::temps_scene+S2S6_angle_t0;

	S3[POSX]=S8[POSX]+(orbS3*cos(angleS3));
	S3[POSY]=S8[POSY];
	S3[POSZ]=S8[POSZ]+(orbS3*sin(angleS3));
	angleS3=vOrbS3*GVars::temps_scene+S2S6_angle_t0;

    S4[POSX]=S8[POSX]+(orbS4*cos(angleS4));
    S4[POSY]=S8[POSY];
	S4[POSZ]=S8[POSZ]+(orbS4*sin(angleS4));
	angleS4=vOrbS4*GVars::temps_scene+S2S6_angle_t0;

	S5[POSX]=S8[POSX]+(orbS5*cos(angleS5));
	S5[POSY]=S8[POSY];
	S5[POSZ]=S8[POSZ]+(orbS5*sin(angleS5));
	angleS5=vOrbS5*GVars::temps_scene+S2S6_angle_t0;

	S6[POSX]=S8[POSX]+(orbS6*cos(angleS6));
	S6[POSY]=S8[POSY];
	S6[POSZ]=S8[POSZ]+(orbS6*sin(angleS6));
	angleS6=vOrbS6*GVars::temps_scene+S2S6_angle_t0;

    S7[POSZ]=S1[POSZ]+(orbS7*cos(angleS7));
    S7[POSY]=S1[POSY]+(orbS7*sin(angleS7));
    S7[POSX]=S1[POSX];
    angleS7=(2.*M_PI)/((8.*16.*GVars::beat)+S7_epsilon_Vangle)*(GVars::temps_scene)+S7_angle_t0;


    S9[POSX]=S8[POSX];
    S9[POSY]=S8[POSY]+23.;
    S9[POSZ]=S8[POSZ];


	if (depId==1)
	{
		S1[POSX]+=aX;
		S1[POSY]+=aY;
		S1[POSZ]+=aZ;
		if (GVars::disp_donnees)
        {
            cout<<"S1 - X="<<S1[POSX]<<" Y="<<S1[POSY]<<" Z="<<S1[POSZ]<<endl;
            GVars::disp_donnees=false;
        }
	}

	if (depId==2)
	{
		float raX,raZ;
		GFunc::rotation_plan(-obs.l,aX,aZ,raX,raZ);



		obs.x+=raX;
		obs.y+=aY;
		obs.z+=raZ;

		obs.r+=aR;
		obs.t+=aT;
		obs.l+=aL;

		if (GVars::disp_donnees)
        {
            cout<<"Obs - X="<<obs.x<<" Y="<<obs.y<<" Z="<<obs.z<<" R="<<obs.r<<" T="<<obs.t<<" L="<<obs.l<<endl;
            GVars::disp_donnees=false;
        }

	}

	if (depId==3)
	{
		L1[POSX]+=aX;
		L1[POSY]+=aY;
		L1[POSZ]+=aZ;
		if (GVars::disp_donnees)
        {
            cout<<"L1 - X="<<L1[POSX]<<" Y="<<L1[POSY]<<" Z="<<L1[POSZ]<<endl;
            GVars::disp_donnees=false;
        }
	}

	if (depId==4)
	{
		S8[POSX]+=aX;
		S8[POSY]+=aY;
		S8[POSZ]+=aZ;
		if (GVars::disp_donnees)
        {
            cout<<"S8 - X="<<S8[POSX]<<" Y="<<S8[POSY]<<" Z="<<S8[POSZ]<<endl;
            GVars::disp_donnees=false;
        }
	}
	if (depId==5)
	{
		float raX,raZ;
		Volume* cosmonaute=GVars::cosmo->renvoie_volume_parent();
		//GFunc::rotation_plan(-cosmonaute->l,aX,aZ,raX,raZ);
            raX=aX;
            raZ=aZ;

		cosmonaute->x+=raX;
		cosmonaute->y+=aY;
		cosmonaute->z-=raZ;

		cosmonaute->r+=aR;
		cosmonaute->t+=aT;
		cosmonaute->l+=aL;

		if (GVars::disp_donnees)
        {
            cout<<"Cosmonaute - X="<<cosmonaute->x<<" Y="<<cosmonaute->y<<" Z="<<cosmonaute->z<<" R="<<cosmonaute->r<<" T="<<cosmonaute->t<<" L="<<cosmonaute->l<<" H="<<GVars::cosmo->echelle<<endl;
            GVars::disp_donnees=false;
        }
	}
	if(depId==6)
    {
        if(dep_haut){S2S6_angle_t0+=0.01;GVars::disp_donnees=true;}
        if(dep_bas){S2S6_angle_t0-=0.01;GVars::disp_donnees=true;}
        if(dep_droite){S7_epsilon_Vangle+=0.01;GVars::disp_donnees=true;}
        if(dep_gauche){S7_epsilon_Vangle-=0.01;GVars::disp_donnees=true;}
        if (GVars::disp_donnees)
        {
            cout<<"S2S6 angle_t0="<<S2S6_angle_t0<<" ; epsilon_Vangle="<<S7_epsilon_Vangle<<endl;
            GVars::disp_donnees=false;
        }
    }


        float deceleration=0.75;
        float acceleration=.05;

        float decel_ang=.85;
        float accel_ang=0.07;

        aX*=deceleration;
        aY*=deceleration;
        aZ*=deceleration;

        aR*=decel_ang;
        aT*=decel_ang;
        aL*=decel_ang;

        if(dep_droite)aX=aX/deceleration+acceleration;
        if(dep_gauche)aX=aX/deceleration-acceleration;

         if(dep_haut)aY=aY/deceleration+acceleration;
         if(dep_bas)aY=aY/deceleration-acceleration;

	if(dep_avant)aZ=aZ/deceleration+acceleration;
         if(dep_arriere)aZ=aZ/deceleration-acceleration;

	if(rot_droite)aL=aL/decel_ang+accel_ang;
        if(rot_gauche)aL=aL/decel_ang-accel_ang;
        if(rot_bas)aT=aT/decel_ang+accel_ang;
        if(rot_haut)aT=aT/decel_ang-accel_ang;
	if(rot_r_droite)aR=aR/decel_ang-accel_ang;
        if(rot_r_gauche)aR=aR/decel_ang+accel_ang;

    if(inc_alt_reflet)altitude_eau_cosmo+=0.1;
    if(dec_alt_reflet)altitude_eau_cosmo-=0.1;


    if(depId==7)
    {
        if(dep_droite)xEdora+=1./float(GVars::largeur_ecran);
        if(dep_gauche)xEdora-=1./float(GVars::largeur_ecran);
        if(dep_avant)yEdora+=1./float(GVars::hauteur_ecran);
        if(dep_arriere)yEdora-=1./float(GVars::hauteur_ecran);
        if(dep_haut)eEdora*=1.01;
        if(dep_bas)eEdora*=0.99;

        if (GVars::disp_donnees)
        {
            cout<<"Edora x="<<GVars::spritesDemo->renvoie_sprite_x(SPR_EDORA)<<"y="<<GVars::spritesDemo->renvoie_sprite_y(SPR_EDORA)<<" e="<<GVars::spritesDemo->renvoie_echelle_sprite(SPR_EDORA)<<endl;
            GVars::disp_donnees=false;
        }
    }


	GFunc::init_matrice_observateur_raytrace(&obs);

}

int WaterWorld::stock_objet(int j,float obj[TAILLE_OBJ])
{
	for(int i=0;i<TAILLE_OBJ;i++)objets[j++]=obj[i];
	return j;
}

void WaterWorld::init_tableau_objets()
{
	int j=0;
	j=stock_objet(j,Sol);
	j=stock_objet(j,S1);
	j=stock_objet(j,S2);

	j=stock_objet(j,S3);
	j=stock_objet(j,S4);
	j=stock_objet(j,S5);
	j=stock_objet(j,S6);
	j=stock_objet(j,S7);
	j=stock_objet(j,S8);
	j=stock_objet(j,S9);
	j=stock_objet(j,S10);
	j=stock_objet(j,S11);
	j=stock_objet(j,S12);

}

void WaterWorld::init_affiche_waterworld()
{
	GFunc::vue_ortho();
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

    /*
	GVars::glow_contraste0=0.75;
	GVars::glow_contraste=5.;
	GVars::glow_saturation=1.;
	GVars::saturation_hallo=1.;
	GVars::taille_hallo=20;
	GVars::alpha_scene=1.;
	GVars::alpha_glow=.5;
	*/

	init_tableau_objets();
	glUseProgram(ShadersDemo::programme_waterworld);
	glUniform1f(ShadersDemo::uniform_waterworld_random_numer,(float)rand()/32768.);
	glUniform1f(ShadersDemo::uniform_waterworld_temps,GVars::temps_scene*vitesse_ondulations);
	glUniform1i(ShadersDemo::uniform_waterworld_numReflec,numReflec);
	glUniform1i(ShadersDemo::uniform_waterworld_numRefrac,numRefrac);
	glUniformMatrix4fv(ShadersDemo::uniform_waterworld_Obs,1,GL_FALSE,GVars::ObsMat->matrice);
	glUniform2f(ShadersDemo::uniform_waterworld_ecranDim,GVars::largeur_ecran,GVars::hauteur_ecran);
	glUniform2f(ShadersDemo::uniform_waterworld_ajustement_resolution,1280./float(GVars::largeur_ecran),720./float(GVars::hauteur_ecran));
	glUniform1f(ShadersDemo::uniform_waterworld_distanceFocale,GVars::distanceFocale);
	glUniform1f(ShadersDemo::uniform_waterworld_ecran_ratio,GVars::ecran_ratio);
	glUniform1fv(ShadersDemo::uniform_waterworld_C_ciel,TAILLE_CIEL,C_ciel);
	glUniform3fv(ShadersDemo::uniform_waterworld_C_amb,1,C_amb);
	glUniform1f(ShadersDemo::uniform_waterworld_L_amb,L_amb);
	glUniform1f(ShadersDemo::uniform_waterworld_distanceBrouillard,distance_brouillard);
	glUniform1f(ShadersDemo::uniform_waterworld_amplitude_vagues,amplitude_vagues);
	glUniform1fv(ShadersDemo::uniform_waterworld_L1,TAILLE_LUM,L1);
	glUniform1fv(ShadersDemo::uniform_waterworld_objets,TAILLE_OBJ*8/*WW_NUM_OBJETS*/,objets);
}
void WaterWorld::eclairages()
{
    Lumieres::drapeaux_sources=0b1;
    float couleur[4]={C_amb[0]*L_amb,C_amb[1]*L_amb,C_amb[2]*L_amb,1.};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, couleur);

    couleur[0]=L1[CR]*L1[INTENS];couleur[1]=L1[CV]*L1[INTENS];couleur[2]=L1[CB]*L1[INTENS];

    Lumieres::lumiere_couleur(GL_LIGHT0,
                                    0.,0.,0.,
                                    couleur[0],couleur[1],couleur[2],
                                    couleur[0],couleur[1],couleur[2]);
    Lumieres::lumiere_attenuation(GL_LIGHT0,0.,L1[ATT],0.);
    Lumieres::lumiere_position(GL_LIGHT0,L1[POSX],L1[POSY],-L1[POSZ]);

    couleur[0]=C_ciel[ZENITH_CR];couleur[1]=C_ciel[ZENITH_CV];couleur[2]=C_ciel[ZENITH_CB];
    couleur[3]=0.;  ///Transparent !!!
    glFogi(GL_FOG_MODE,GL_LINEAR);
    glFogfv(GL_FOG_COLOR,couleur);
    glFogf(GL_FOG_START,0.);
    glFogf(GL_FOG_END,distance_brouillard);
    glFogf(GL_FOG_DENSITY,1.);
    glHint(GL_FOG_HINT,GL_NICEST);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glDisable(GL_LIGHT1);
    glEnable(GL_FOG);

    //Eclairages du reflet du cosmonaute:
    params_reflet_cosmo.position_source[0]=L1[POSX]-obs.x;
    params_reflet_cosmo.position_source[1]=L1[POSY]-obs.y;
    params_reflet_cosmo.position_source[2]=-L1[POSZ]+obs.z;
    params_reflet_cosmo.couleur_fond[0]=C_ciel[NADIR_CR];
    params_reflet_cosmo.couleur_fond[1]=C_ciel[NADIR_CV];
    params_reflet_cosmo.couleur_fond[2]=C_ciel[NADIR_CB];
    params_reflet_cosmo.couleur_fond[3]=1.;

    params_reflet_cosmo.brillance=40.;
    params_reflet_cosmo.couleur_speculaire[0]=L1[CR]*L1[INTENS];
    params_reflet_cosmo.couleur_speculaire[1]=L1[CV]*L1[INTENS];
    params_reflet_cosmo.couleur_speculaire[2]=L1[CB]*L1[INTENS];
    params_reflet_cosmo.couleur_speculaire[3]=1.;

}

void WaterWorld::maj_positions_lumieres()
{
    Lumieres::lumiere_position(GL_LIGHT0,L1[POSX],L1[POSY],-L1[POSZ]);
}

void WaterWorld::affiche_waterworld()
{
	glBegin( GL_QUADS );
        glVertex3f(-1,1,-1.);
        glVertex3f(1.,1.,-1.);
        glVertex3f(1.,-1.,-1.);
        glVertex3f(-1.,-1.,-1.);
        glEnd( );


}

void WaterWorld::affiche_Edora()
{
    /*
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    GFunc::vue_perspective(GVars::zMin,GVars::zMax);
    GFunc::init_matrice_observateur(GVars::obs);
    */


    if(drapeau_affiche_Edora)
    {
            GVars::spritesDemo->montre_sprite(SPR_EDORA);
            GVars::spritesDemo->determine_position_sprite(SPR_EDORA,xEdora,yEdora);
            GVars::spritesDemo->determine_position_centre_sprite(SPR_EDORA,0.5,.9331);
            GVars::spritesDemo->determine_echelle_sprite(SPR_EDORA,eEdora);
            GVars::spritesDemo->determine_alpha_sprite(SPR_EDORA,alphaEdora);
            GVars::spritesDemo->determine_proportionnel_sprite(SPR_EDORA,true);
            GVars::spritesDemo->determine_centre_sprite(SPR_EDORA,false);
            GVars::spritesDemo->affiche_sprites_2d();
            GVars::spritesDemo->cache_sprite(SPR_EDORA);
    }
}


void WaterWorld::render_waterworld()
{
    init_affiche_waterworld();
    affiche_waterworld();
    affiche_Edora();

    if(drapeau_affiche_ville)
    {
        GVars::spritesDemo->montre_sprite(SPR_VILLE);
        GVars::spritesDemo->affiche_sprites_2d();
    }else GVars::spritesDemo->cache_sprite(SPR_VILLE);




    if(drapeau_affiche_agnomes)
    {
        agnomes->xpos_min=0.;
        agnomes->xpos_max=1.;
        agnomes->affiche(GVars::temps_scene);
    }
}



