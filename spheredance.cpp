
#include "spheredance.h"


//Paramètres
int SphereDance::groupe_courant=0;
int32_t SphereDance::numReflec=4;
float SphereDance::vitesse_rotation_spheres=.05;
float SphereDance::zMin=1.;
float SphereDance::zMax=2000.;

//Observateur
Repere SphereDance::obs;
float SphereDance::obsX=0.;
float SphereDance::obsY=115;
float SphereDance::obsZ=0.;

float SphereDance::obsR=0.;
float SphereDance::obsT=90.;
float SphereDance::obsL=180.;

float SphereDance::aX=0.;
float SphereDance::aY=0.;
float SphereDance::aZ=0.;

float SphereDance::aR=0.;
float SphereDance::aT=0.;
float SphereDance::aL=0.;

//Ciel:
float SphereDance::C_ciel[TAILLE_CIEL]={
						255./255.,224./255.,155./255.,
						255./255.,127./255.,202./255.,
						255./255.,127./255.,202./255.,
						233./255.,56./255.,63./255.
						};
//Lumière ambiante:
float SphereDance::L_amb=0.2;
float SphereDance::C_amb[3]={0.3,0.5,1.};
float SphereDance::distance_brouillard=200.;

//Lumière ponctuelle:
float SphereDance::L1[TAILLE_LUM];


//Sol:
float SphereDance::Sol[TAILLE_OBJ];

//Sphère:

float SphereDance::S1[TAILLE_OBJ];
float SphereDance::S2[TAILLE_OBJ];
float SphereDance::S3[TAILLE_OBJ];
float SphereDance::S4[TAILLE_OBJ];
float SphereDance::S5[TAILLE_OBJ];
float SphereDance::S6[TAILLE_OBJ];
float SphereDance::S7[TAILLE_OBJ];
float SphereDance::S8[TAILLE_OBJ];
float SphereDance::S9[TAILLE_OBJ];
float SphereDance::S10[TAILLE_OBJ];
float SphereDance::S11[TAILLE_OBJ];
float SphereDance::S12[TAILLE_OBJ];
float SphereDance::S13[TAILLE_OBJ];
float SphereDance::S14[TAILLE_OBJ];
float SphereDance::S15[TAILLE_OBJ];
//float SphereDance::S16[TAILLE_OBJ];

#define SD_ANNEAU_NUM_SPHERES 14
float* SphereDance::anneau_spheres[SD_ANNEAU_NUM_SPHERES]={SphereDance::S2,SphereDance::S3,SphereDance::S4,SphereDance::S5,SphereDance::S6,SphereDance::S7,
                                                            SphereDance::S8,SphereDance::S9,SphereDance::S10,SphereDance::S11,SphereDance::S12,
                                                            SphereDance::S13,SphereDance::S14,SphereDance::S15};
float SphereDance::positions_spheres[SD_ANNEAU_NUM_SPHERES*3];
float SphereDance::positions_spheres_rot[SD_ANNEAU_NUM_SPHERES*3];

float SphereDance::objets[TAILLE_OBJ*SD_NUM_OBJETS];	//Le tableau contenant les objets de la scène

float SphereDance::alpha_ay=0.;

//Commandes clavier:
    bool SphereDance::dep_droite=false;
    bool SphereDance::dep_gauche=false;
    bool SphereDance::dep_haut=false;
    bool SphereDance::dep_bas=false;
    bool SphereDance::dep_avant=false;
    bool SphereDance::dep_arriere=false;

    bool SphereDance::rot_droite=false;
    bool SphereDance::rot_gauche=false;
    bool SphereDance::rot_haut=false;
    bool SphereDance::rot_bas=false;
    bool SphereDance::rot_r_gauche=false;
    bool SphereDance::rot_r_droite=false;

    bool SphereDance::numReflec_0=false;
    bool SphereDance::numReflec_1=false;
    bool SphereDance::numReflec_2=false;
    bool SphereDance::numReflec_3=false;

    bool SphereDance::numRefrac_0=false;
    bool SphereDance::numRefrac_1=false;
    bool SphereDance::numRefrac_2=false;
    bool SphereDance::numRefrac_3=false;

    int SphereDance::depId=1;

    bool SphereDance::inc_FracTexMin=false;
    bool SphereDance::inc_FracTexMax=false;
    bool SphereDance::dec_FracTexMin=false;
    bool SphereDance::dec_FracTexMax=false;

    float SphereDance::fracTexDistMax=50.;
    float SphereDance::fracTexDistMin=20.;
    bool SphereDance::drapeau_affiche_fracTex=true;

    vec3f SphereDance::position_edora(0.,4.85637,13.8505);
    float SphereDance::alpha_Edora=1.;
    float SphereDance::angle_Edora=180.;

//Paramètres Greets:


bool SphereDance::init_scene_spheredance()
{
    obsY=GVars::distanceFocale*16./float(GVars::largeur_ecran);
    obs.initXYZ(obsX,obsY,obsZ);
    obs.initRTL(obsR,obsT,obsL);

	SphereDance::Sol[OBJTYPE]=OBJTYPE_SOL;

	Sol[CR]=255./255.;
	Sol[CV]=248./255.;
	Sol[CB]=108./255.;

	Sol[DIFF]=1.;
	Sol[SPEC]=.25;
	Sol[BRILL]=4.;

	Sol[TRANSP]=0.5;
	Sol[REFLEC]=.5;
	Sol[REFRAC]=1.333;

	Sol[C2R]=135./255.;
	Sol[C2V]=67./255.;
	Sol[C2B]=170./255;
	Sol[CAR_XL]=10.;
	Sol[CAR_ZL]=10.;

	Raytracer01::init_lum(L1,0.,10.,100.,	1.,1.,1.,	1.,0.001);

	Raytracer01::init_sphere(S1,20.,		0.,2.,0.,	1.,0.1,0.1,	1.,0.5,20.,	.5,.8,0.3);
	Raytracer01::init_sphere(S2,1.,		-25.,5,20.,	1,.5,.1,		1.,1.,200.,	0.9,0.4,1.01);
	Raytracer01::init_sphere(S3,2.,		-14.,5.5,20.,	1.,1.,.1,		1.,.4,50.,		0.9,0.8,1.02);
	Raytracer01::init_sphere(S4,3.,		-3.,6,20.,		.1,1.,.1,		1.,.5,20.,		0.9,0.9,1.03);
	Raytracer01::init_sphere(S5,4.,		8.,6.5,20.,	1.,.1,1.,		1.,1.,30.,	0.5,.6,1.04);
	Raytracer01::init_sphere(S6,5.,		19.,7,20.,	0.,1.,0.,		1.,1.,70.,	0.,0.7,1.05);

	Raytracer01::init_sphere(S7,6.,		33.,13,25.,	1.,1.,0.,		1.,1.,10.,	0.4,0.7,1.333);
	Raytracer01::init_sphere(S8,7.,		33.,38,25.,	0.5,.5,1.,		1.,1.,20.,	0.5,0.7,1.01);
	Raytracer01::init_sphere(S9,8.,		33.,61,25.,	1.,1.,1.,		1.,1.,40.,	0.,0.7,1.2);
	Raytracer01::init_sphere(S10,7.,		33.,82,25.,	0.,1.,1.,		1.,1.,50.,	0.,0.7,1.005);
	Raytracer01::init_sphere(S11,6.,		33.,101,25.,	1.,0.,1.,		1.,1.,60.,	0.,0.7,1.005);
	Raytracer01::init_sphere(S12,5.,		33.,118,25.,	0.5,1.,0.5,	1.,1.,7.,	0.,0.7,1.005);
	Raytracer01::init_sphere(S13,4.,		-32.,4.,25.,	1.,0.,0.,		1.,1.,7.,	0.,0.4,1.005);
	Raytracer01::init_sphere(S14,3.,		-39.,5.,25.,	1.,0.5,0.1,	1.,1.,7.,	0.,0.5,1.005);
	Raytracer01::init_sphere(S15,2.,		-46.,6.,25.,	1.,0.8,0.2,	1.,1.,7.,	0.,0.6,1.005);
	//init_sphere(S16,5.,	0.,300.,25.,	1.,0.8,0.2,	1.,1.,7.,	0.,0.9,1.005);

	float acx=0.;
	float acz=0.;
	float acy=6.;

	float arx=20.;
	float arz=20.;

	float s=(2.*M_PI)/SD_ANNEAU_NUM_SPHERES;

	for(int i=0;i<SD_ANNEAU_NUM_SPHERES;i++)
	{
		positions_spheres[i*3]=acx+arx*cos(s*i);
		positions_spheres[i*3+1]=acy;
		positions_spheres[i*3+2]=acz+arz*sin(s*i);
	}

	return true;
}

void SphereDance::clavier(int key,bool etat)
{

    if(!GVars::editionTrajectoire)
    {
		switch (key)
        {
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

                case GLFW_KEY_F1: depId=1;break;
                case GLFW_KEY_F2: depId=2;break;
                case GLFW_KEY_F3: depId=3;break;
                case GLFW_KEY_F4: depId=4;break;
                case GLFW_KEY_F5: depId=5;break;
                /*
                case GLFW_KEY_KP_ADD: groupe_courant++; if (groupe_courant>=GVars::nbrGroupes)groupe_courant=GVars::nbrGroupes; break;
                case GLFW_KEY_KP_SUBTRACT: groupe_courant--; if (groupe_courant<=0)groupe_courant=0; break;
                */

                case GLFW_KEY_A: inc_FracTexMin=etat;break;
                case GLFW_KEY_Q: dec_FracTexMin=etat;break;
                case GLFW_KEY_Z: inc_FracTexMax=etat;break;
                case GLFW_KEY_S: dec_FracTexMax=etat;break;


        }
    }
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
        }
    }
}

void SphereDance::maj_deplacement()
{
	uint32_t i;
	GFunc::rotation_points(0,0,0, 0.,1.,0.,alpha_ay,positions_spheres,positions_spheres_rot,SD_ANNEAU_NUM_SPHERES);
	alpha_ay=vitesse_rotation_spheres*GVars::temps_scene;
	for( i=0;i<SD_ANNEAU_NUM_SPHERES;i++)
	{
		anneau_spheres[i][POSX]=positions_spheres_rot[3*i]+S1[POSX];
		anneau_spheres[i][POSY]=positions_spheres_rot[3*i+1]+S1[POSY];
		anneau_spheres[i][POSZ]=positions_spheres_rot[3*i+2]+S1[POSZ];
	}

	if(numReflec_0)numReflec=0;
	if(numReflec_1)numReflec=1;
	if(numReflec_2)numReflec=2;
	if(numReflec_3)numReflec=3;

	if (depId==1)
	{
		S1[POSX]+=aX;
		S1[POSY]+=aY;
		S1[POSZ]+=aZ;
		if (GVars::disp_donnees)
        {
            cout<<"S1 - X="<<S1[POSX]<<" Y="<<S1[POSY]<<" Z="<<S1[POSZ]<<endl;

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

        }

	}

	if (depId==3)
	{


        /*
		L1[POSX]+=aX;
		L1[POSY]+=aY;
		L1[POSZ]+=aZ;
		*/


		position_edora.x+=aX;
		position_edora.y+=aY;
		position_edora.z+=aZ;
		angle_Edora+=aL;

		if (GVars::disp_donnees)
        {
            cout<<"Edora - X="<<position_edora.x<<" Y="<<position_edora.y<<" Z="<<position_edora.z<<" L="<<angle_Edora<<endl;

        }

	}

	if (depId==5)
	{
		float raX,raZ;
		Volume* cosmonaute=GVars::cosmo->renvoie_volume_parent();
		//GFunc::rotation_plan(-cosmonaute->l,aX,aZ,raX,raZ);
            raX=aX;
            raZ=aZ;

		cosmonaute->x+=raX/10.;
		cosmonaute->y+=aY/10.;
		cosmonaute->z-=raZ/10.;

		cosmonaute->r+=aR;
		cosmonaute->t+=aT;
		cosmonaute->l+=aL;

		if (GVars::disp_donnees)
        {
            cout<<"Cosmonaute - X="<<cosmonaute->x<<" Y="<<cosmonaute->y<<" Z="<<cosmonaute->z<<" H="<<GVars::cosmo->echelle<<" R="<<cosmonaute->r<<" T="<<cosmonaute->t<<" L="<<cosmonaute->l<<endl;

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

    float vitesseDist=0.1;
    if(inc_FracTexMin) {fracTexDistMin+=vitesseDist;cout<<"distMin="<<fracTexDistMin<<" distMax="<<fracTexDistMax<<endl;}
    if(inc_FracTexMax) {fracTexDistMax+=vitesseDist;cout<<"distMin="<<fracTexDistMin<<" distMax="<<fracTexDistMax<<endl;}
    if(dec_FracTexMin) {fracTexDistMin-=vitesseDist;cout<<"distMin="<<fracTexDistMin<<" distMax="<<fracTexDistMax<<endl;}
    if(dec_FracTexMax) {fracTexDistMax-=vitesseDist;cout<<"distMin="<<fracTexDistMin<<" distMax="<<fracTexDistMax<<endl;}
    if (GVars::disp_donnees)
        {
            cout<<"distMin="<<fracTexDistMin<<" distMax="<<fracTexDistMax<<endl;
        }

	GFunc::init_matrice_observateur_raytrace(&obs);

	GVars::disp_donnees=false;

}

int SphereDance::stock_objet(int j,float obj[TAILLE_OBJ])
{
	for(int i=0;i<TAILLE_OBJ;i++)objets[j++]=obj[i];
	return j;
}

void SphereDance::init_tableau_objets()
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
	j=stock_objet(j,S13);
	j=stock_objet(j,S14);
	j=stock_objet(j,S15);
	//j=stock_objet(j,S16);

}

//*************************************************************************
//      Affiche la texture de la fractale au sol des sphères qui dansent
//*************************************************************************


void SphereDance::affiche_fractaleShot()
{
    float num_fracTexRepeat=19.;
    float xsize=16.*num_fracTexRepeat;
    float ysize=9.*num_fracTexRepeat;
    //GFunc::sauve_OpenGl();


        glPushMatrix();

        glTranslatef(0.,0.,0.);

        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_LIGHTING);

        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,FrameBuffers::fbTex[FBO_SCREENSHOT]);

        glUseProgram(ShadersDemo::programme_alphaDist);
        glUniform1i(ShadersDemo::uniform_alphaDist_texture,0);

        glUniform3f(ShadersDemo::uniform_alphaDist_point,SphereDance::obs.x,SphereDance::obs.y,-SphereDance::obs.z);
        glUniform1f(ShadersDemo::uniform_alphaDist_distMin,fracTexDistMin);
        glUniform1f(ShadersDemo::uniform_alphaDist_distMax,fracTexDistMax);

        glBegin( GL_QUADS );
        //glColor3f(1.,1.,1.);
        glTexCoord2f(0.5*num_fracTexRepeat+0.5,-0.5*num_fracTexRepeat+0.5);//(1.,0.);
        glVertex3f(-xsize/2.,0.,-ysize/2.);
        glTexCoord2f(0.5*num_fracTexRepeat+0.5,0.5*num_fracTexRepeat+0.5);//(1.,1.);
        glVertex3f(-xsize/2.,0.,ysize/2.);
        glTexCoord2f(-0.5*num_fracTexRepeat+0.5,0.5*num_fracTexRepeat+0.5);//(0.,1.)
        glVertex3f(xsize/2.,0.,ysize/2.);
        glTexCoord2f(-0.5*num_fracTexRepeat+0.5,-0.5*num_fracTexRepeat+0.5);//(0.,0.);
        glVertex3f(xsize/2.,0.,-ysize/2.);
        glEnd( );

        glPopMatrix();

        glUseProgram(0);

        glDisable(GL_TEXTURE_2D);

    //GFunc::restitue_OpenGl();
}

void SphereDance::init_affiche_spheredance()
{
	GFunc::vue_ortho();
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

    /*
    GVars::glow_contraste0=0.5;
	GVars::glow_contraste=0.;
	GVars::glow_saturation=1.;
	GVars::taille_hallo=10.;
	GVars::saturation_hallo=1.;
	GVars::alpha_scene=1.;
	GVars::alpha_glow=1.;
	*/

    /*
	GVars::glow_contraste0=0.25;
	GVars::glow_contraste=5.;
	GVars::glow_saturation=1.;
	GVars::saturation_halo=1.;
	GVars::taille_hallo=20;
	GVars::alpha_scene=1.;
	GVars::alpha_glow=1.;
	*/

	init_tableau_objets();
	glUseProgram(ShadersDemo::programme_spheredance);
	glUniform1f(ShadersDemo::uniform_spheredance_random_numer,(float)rand()/32768.);
	glUniform1f(ShadersDemo::uniform_spheredance_temps,glfwGetTime());
	glUniform1i(ShadersDemo::uniform_spheredance_numReflec,numReflec);
	glUniformMatrix4fv(ShadersDemo::uniform_spheredance_Obs,1,GL_FALSE,GVars::ObsMat->matrice);
	glUniform2f(ShadersDemo::uniform_spheredance_ecranDim,GVars::largeur_ecran,GVars::hauteur_ecran);
	glUniform2f(ShadersDemo::uniform_spheredance_ajustement_resolution,1280./float(GVars::largeur_ecran),720./float(GVars::hauteur_ecran));
	glUniform1f(ShadersDemo::uniform_spheredance_distanceFocale,GVars::distanceFocale);
	glUniform1f(ShadersDemo::uniform_spheredance_ecran_ratio,GVars::ecran_ratio);
	glUniform1fv(ShadersDemo::uniform_spheredance_C_ciel,TAILLE_CIEL,C_ciel);
	glUniform3fv(ShadersDemo::uniform_spheredance_C_amb,1,C_amb);
	glUniform1f(ShadersDemo::uniform_spheredance_L_amb,L_amb);
	glUniform1f(ShadersDemo::uniform_spheredance_distanceBrouillard,distance_brouillard);
	glUniform1fv(ShadersDemo::uniform_spheredance_L1,TAILLE_LUM,L1);
	glUniform1fv(ShadersDemo::uniform_spheredance_objets,15*SD_NUM_OBJETS,objets);
}

void SphereDance::eclairages()
{
    float couleur_amb[4]={C_amb[0]*L_amb,C_amb[1]*L_amb,C_amb[2]*L_amb,1.};
    float couleur_lum[3]={L1[CR]*L1[INTENS],L1[CV]*L1[INTENS],L1[CB]*L1[INTENS]};
    float couleur_fog[4]={C_ciel[HORIZB_CR],C_ciel[HORIZB_CV],C_ciel[HORIZB_CB],1.};

    Lumieres::lumiere_attenuation(GL_LIGHT0,0.,L1[ATT],0.);
    Lumieres::lumiere_position(GL_LIGHT0,L1[POSX],L1[POSY],-L1[POSZ]);
    Lumieres::lumiere_couleur(GL_LIGHT0,
                            0.,0.,0.,
                            couleur_lum[0],couleur_lum[1],couleur_lum[2],
                            couleur_lum[0],couleur_lum[1],couleur_lum[2]);
    Lumieres::lumiere_ouverture(GL_LIGHT0,180.);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, couleur_amb);

    glFogi(GL_FOG_MODE,GL_LINEAR);
    glFogfv(GL_FOG_COLOR,couleur_fog);
    glFogf(GL_FOG_START,0.);
    glFogf(GL_FOG_END,distance_brouillard);
    glFogf(GL_FOG_DENSITY,1.);
    glHint(GL_FOG_HINT,GL_NICEST);

    glEnable(GL_LIGHTING);
    glEnable(GL_FOG);
    glEnable(GL_LIGHT0);
    glDisable(GL_LIGHT1);
    Lumieres::drapeaux_sources=0b1;
}

void SphereDance::affiche_Edora()
{
    glPushMatrix();
        glEnable(GL_FOG);
        glTranslatef(position_edora.x,position_edora.y,position_edora.z);
        glRotatef(angle_Edora,0.,1.,0.);
        glScalef(1.6,-1.6*2.,1.);
        glEnable(GL_COLOR_MATERIAL);
        glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glDisable(GL_CULL_FACE);
        glDisable(GL_LIGHTING);
        glUseProgram(0);
        glBindTexture(GL_TEXTURE_2D,GVars::spritesDemo->renvoie_texture_id_sprite(SPR_EDORA2));
        GFunc::affiche_texture(alpha_Edora);
        glDisable(GL_COLOR_MATERIAL);
        glEnable(GL_LIGHTING);
        glDisable(GL_BLEND);
        glDisable(GL_COLOR_MATERIAL);
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void SphereDance::maj_positions_eclairages()
{
    Lumieres::lumiere_position(GL_LIGHT0,L1[POSX],L1[POSY],-L1[POSZ]);
}

void SphereDance::affiche_spheredance()
{
	glBegin( GL_QUADS );
    glVertex3f(-1,1,-1.);
    glVertex3f(1.,1.,-1.);
    glVertex3f(1.,-1.,-1.);
    glVertex3f(-1.,-1.,-1.);
    glEnd( );
}


void SphereDance::render_spheredance()
{
    init_affiche_spheredance();
    affiche_spheredance();
    GFunc::vue_perspective(GVars::zMin,GVars::zMax);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    GFunc::init_matrice_observateur(&SphereDance::obs);
    if(drapeau_affiche_fracTex)affiche_fractaleShot();
    affiche_Edora();

}

