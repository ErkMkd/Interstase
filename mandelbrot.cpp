#include "mandelbrot.h"

Repere Mandelbrot::obs;
Bruit Mandelbrot::pertrube_obsX;
Bruit Mandelbrot::pertrube_obsY;
Bruit Mandelbrot::pertrube_obsZ;
float Mandelbrot::perturbationObs=1.;

GLdouble Mandelbrot::xpos=0.0297733;//0.0297733;
GLdouble Mandelbrot::ypos=0.0071582;//0.0071582;
GLdouble Mandelbrot::zoom=610.3;//465.3;
GLfloat Mandelbrot::zoom_ar=.7;
GLfloat Mandelbrot::cxTex=0.5;
GLfloat Mandelbrot::cyTex=0.5;
GLdouble Mandelbrot::facteur_zoom=.99;

float Mandelbrot::zMin=1.;
float Mandelbrot::zMax=1000.;

GLdouble Mandelbrot::azoom=1.;
GLdouble Mandelbrot::axpos=0.;
GLdouble Mandelbrot::aypos=0.;
GLdouble Mandelbrot::azpos=0.;
GLdouble Mandelbrot::deceleration=0.7;
GLdouble Mandelbrot::acceleration=1.5;
GLint Mandelbrot::iter=2;

GLdouble Mandelbrot::t=1.;
GLfloat Mandelbrot::alpha=0.;
GLfloat Mandelbrot::perturbation_alpha=0.;
GLfloat Mandelbrot::decallage_palette=0.;
Bruit Mandelbrot::rotation_Perlin;
float Mandelbrot::facteur_perturbation_rotation=0.;

GLuint Mandelbrot::palette_tex_id=0;
uint32_t Mandelbrot::palette[MB_NUM_COULEURS+1]; //+1 pour les débordements

//Lumieres:
float Mandelbrot::L0_attenuation=1.;
float Mandelbrot::distance_brouillard=2000.;
float Mandelbrot::couleur_amb[4]={0.,0.,0.,1.};
float Mandelbrot::couleur_fog[4]={0.,0.,0.,1.};
float Mandelbrot::L0_position[3];
float Mandelbrot::L0_amb[3];
float Mandelbrot::L0_diff[3];
float Mandelbrot::L0_spec[3];

float Mandelbrot::L1_orientation[3];
float Mandelbrot::L1_amb[3]={0.,0.,0.};
float Mandelbrot::L1_diff[3];
float Mandelbrot::L1_spec[3];


//Sprites:
bool Mandelbrot::drapeau_affiche_screenShot=true;
bool Mandelbrot::drapeau_affiche_nuages=true;
bool Mandelbrot::drapeau_affiche_vaisseau=true;
Champ_Etoiles* Mandelbrot::nuages=NULL;
Image* Mandelbrot::nuage_1=NULL;


//Commandes clavier:
bool Mandelbrot::dep_droite=false;
bool Mandelbrot::dep_gauche=false;
bool Mandelbrot::dep_avant=false;
bool Mandelbrot::dep_arriere=false;
bool Mandelbrot::dep_haut=false;
bool Mandelbrot::dep_bas=false;
bool Mandelbrot::zoom_in=false;
bool Mandelbrot::zoom_out=false;
bool Mandelbrot::rot_CCW=false;
bool Mandelbrot::rot_CW=false;
bool Mandelbrot::t_plus=false;
bool Mandelbrot::t_moins=false;
bool Mandelbrot::iter_plus=false;
bool Mandelbrot::iter_moins=false;
int Mandelbrot::depId=1;

bool Mandelbrot::rot_L_gauche=false;
bool Mandelbrot::rot_L_droite=false;
bool Mandelbrot::rot_T_haut=false;
bool Mandelbrot::rot_T_bas=false;
bool Mandelbrot::rot_R_gauche=false;
bool Mandelbrot::rot_R_droite=false;

float Mandelbrot::aX=0.;
float Mandelbrot::aY=0.;
float Mandelbrot::aZ=0.;
float Mandelbrot::aR=0.;
float Mandelbrot::aT=0.;
float Mandelbrot::aL=0.;

bool Mandelbrot::init_scene_mandelbrot()
{
	//uint32_t gradients[]={0x132759ff,0x11389bff,0x4c80ffff,0x8743aaff,0xe9383fff,0xff7fcaff,0xffe09bff,0xfff86cff,0x33b366ff,0x1a5933ff};
	uint32_t gradients[]={0x11389bff,0x4c80ffff,0x17d722ff,0xffff00ff,0xf9001eff,0x8743aaff,0xff8080ff,0xff9a22ff,0xffe09bff,0x1a5933ff};

	Couleurs::couleur_gradients_rvba_i(MB_NUM_COULEURS,MB_NUM_ETAPES,gradients,palette);
    for(int i=0;i<MB_NUM_COULEURS/10;i++)
    {
        palette[i*10]=((palette[i*10]<<1) & 0x7f7f7f7f ) | 0x000000ff;
    }

	glGenTextures(1,&palette_tex_id);
	if (palette_tex_id==0)
        {
            cout<<endl<<"Erreur - Impossible de créer la palette de la scène Mandelbrot"<<endl;
            return false;
        }
         glBindTexture(GL_TEXTURE_1D,palette_tex_id);
        glTexParameterf(GL_TEXTURE_1D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameterf(GL_TEXTURE_1D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameterf(GL_TEXTURE_1D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexImage1D(GL_TEXTURE_1D, //Type de  texture
                        0,      //Niveau de détails de la texture
                        GL_RGBA,      //Format de la texture
                        MB_NUM_COULEURS, //taille de la texture en pixels
                        0,      //Largeur du bord. l et h sont incrémentées de 2x cette valeur
                        GL_RGBA, //Format des pixels
                        GL_UNSIGNED_BYTE, //Type des données
                        palette);


        return init_sprites();
}

bool Mandelbrot::init_sprites()
{
    //Init le starfield:
        nuage_1=new Image("images/nuage_01.tif");
        if(nuage_1==NULL || nuage_1->erreur!=IMAGE_OK) return false;

        nuages=new Champ_Etoiles();
        if(nuages==NULL ||nuages->erreur!=CHAMP_ETOILES_OK) return false;


        vec3f v(-1.,-1.,0.);
        v.normalise();
        nuages->initialise(
                                500.,500.,500.,
                                400,
                                nuage_1,
                                150.,
                                1.,0.,&v,1.);
        nuages->alpha=0.;
        return true;
}

void Mandelbrot::detruit_scene_mandelbrot()
{
	if (glIsTexture(palette_tex_id))
	 {
	 	glDeleteTextures(1,&palette_tex_id);
	 }
	 if(nuages!=NULL) delete nuages;
	 if(nuage_1!=NULL) delete nuage_1;
}

void Mandelbrot::clavier(int key,bool etat)
{
    if(!GVars::editionTrajectoire)
    {
        switch(key)
        {
                case GLFW_KEY_F1:depId=1;break;
                case GLFW_KEY_F2:depId=2;break;
                case GLFW_KEY_F3:depId=3;break;
                case GLFW_KEY_F4:depId=4;break;
                case GLFW_KEY_F5:depId=5;break;
        }

        if(depId==1)
        {
            switch (key)
            {
                case GLFW_KEY_LEFT:dep_gauche=etat;break;
                case GLFW_KEY_UP:dep_haut=etat ;break;
                case GLFW_KEY_DOWN:dep_bas=etat;break;
                case GLFW_KEY_RIGHT:dep_droite=etat;break;

                case GLFW_KEY_PAGE_UP:dep_avant=etat ;break;
                case GLFW_KEY_PAGE_DOWN:dep_arriere=etat;break;

                case GLFW_KEY_KP_ADD:zoom_in=etat;break;
                case GLFW_KEY_KP_SUBTRACT:zoom_out=etat;break;
                case GLFW_KEY_KP_1:t_plus=etat;break;
                case GLFW_KEY_KP_3:t_moins=etat;break;
                case GLFW_KEY_KP_7:iter_moins=etat;break;
                case GLFW_KEY_KP_9:iter_plus=etat;break;
                case GLFW_KEY_KP_4:rot_CCW=etat;break;
                case GLFW_KEY_KP_6:rot_CW=etat;break;
                case GLFW_KEY_N:decallage_palette+=1./MB_NUM_COULEURS;break;
                case GLFW_KEY_B:decallage_palette-=1./MB_NUM_COULEURS;break;
            }
        }
        else
        {
            switch (key)
            {
                case GLFW_KEY_RIGHT: dep_droite=etat;break;
                case GLFW_KEY_LEFT: dep_gauche=etat;break;
                case GLFW_KEY_UP: dep_avant=etat ;break;
                case GLFW_KEY_DOWN: dep_arriere=etat;break;

                case GLFW_KEY_KP_4:rot_L_droite=etat;break;
                case GLFW_KEY_KP_6:rot_L_gauche=etat;break;
                case GLFW_KEY_KP_8:rot_T_bas=etat;break;
                case GLFW_KEY_KP_2:rot_T_haut=etat;break;
                case GLFW_KEY_KP_1:rot_R_droite=etat;break;
                case GLFW_KEY_KP_3:rot_R_gauche=etat;break;
                case GLFW_KEY_PAGE_UP:dep_haut=etat;break;
                case GLFW_KEY_PAGE_DOWN:dep_bas=etat;break;
            }
        }

    }
    else
    {
        //depId=5;
        switch (key)
        {
            case GLFW_KEY_LEFT:dep_gauche=etat;break;
            case GLFW_KEY_UP:dep_haut=etat ;break;
            case GLFW_KEY_DOWN:dep_bas=etat;break;
            case GLFW_KEY_RIGHT:dep_droite=etat;break;

            case GLFW_KEY_PAGE_UP:dep_avant=etat ;break;
            case GLFW_KEY_PAGE_DOWN:dep_arriere=etat;break;

            //case GLFW_KEY_KP_ADD:zoom_in=etat;break;
            //case GLFW_KEY_KP_SUBTRACT:zoom_out=etat;break;
            case GLFW_KEY_KP_1:t_plus=etat;break;
            case GLFW_KEY_KP_3:t_moins=etat;break;
            case GLFW_KEY_KP_7:iter_moins=etat;break;
            case GLFW_KEY_KP_9:iter_plus=etat;break;
            case GLFW_KEY_KP_4:rot_CCW=etat;break;
            case GLFW_KEY_KP_6:rot_CW=etat;break;
            case GLFW_KEY_N:decallage_palette+=1./MB_NUM_COULEURS;break;
            case GLFW_KEY_B:decallage_palette-=1./MB_NUM_COULEURS;break;
        }


        Volume* cosmo=GVars::cosmo->renvoie_volume_parent();
        EditeurTrajectoire::ET_x=cosmo->x;
        EditeurTrajectoire::ET_y=cosmo->y;
        EditeurTrajectoire::ET_z=cosmo->z;
        EditeurTrajectoire::edit_trajectoire(key,etat);
        cosmo->x=EditeurTrajectoire::ET_x;
        cosmo->y=EditeurTrajectoire::ET_y;
        cosmo->z=EditeurTrajectoire::ET_z;
    }


}

void Mandelbrot::maj_deplacement()
{
    //Déplacement fractale:

        perturbation_alpha=facteur_perturbation_rotation*((rotation_Perlin.valeur_aleatoire_temporelle(GVars::temps_scene,0.48*8.,1.)-0.5)*2.);

        axpos*=deceleration;
        aypos*=deceleration;
        azpos*=deceleration;

        if(dep_droite) {axpos=axpos/deceleration+acceleration;}
        if(dep_gauche){axpos=axpos/deceleration-acceleration;}
        if(dep_haut){aypos=aypos/deceleration+acceleration;}
        if(dep_bas){aypos=aypos/deceleration-acceleration;}
        if(dep_avant)azpos=azpos/deceleration+acceleration;
        if(dep_arriere)azpos=azpos/deceleration-acceleration;

    // Déplacement objets 3d:
        float decelerationObj=0.75;
        float accelerationObj=.005;
        float decel_ang=.85;
        float accel_ang=0.07;
        if(dep_droite)aX=aX/decelerationObj+accelerationObj;
        if(dep_gauche)aX=aX/decelerationObj-accelerationObj;

        if(dep_haut)aY=aY/decelerationObj+accelerationObj;
        if(dep_bas)aY=aY/decelerationObj-accelerationObj;

        if(dep_avant)aZ=aZ/decelerationObj+accelerationObj;
        if(dep_arriere)aZ=aZ/decelerationObj-accelerationObj;

        if(rot_L_droite)aL=aL/decel_ang+accel_ang;
        if(rot_L_gauche)aL=aL/decel_ang-accel_ang;
        if(rot_T_bas)aT=aT/decel_ang+accel_ang;
        if(rot_T_haut)aT=aT/decel_ang-accel_ang;
        if(rot_R_droite)aR=aR/decel_ang-accel_ang;
        if(rot_R_gauche)aR=aR/decel_ang+accel_ang;





        float raX,raZ;

        aX*=decelerationObj;
        aY*=decelerationObj;
        aZ*=decelerationObj;

        aR*=decel_ang;
        aT*=decel_ang;
        aL*=decel_ang;

    if(depId==1)
    {

        double c=cos(alpha);
        double s=sin(alpha);

        double depx=axpos*c-aypos*s;
        double depy=axpos*s+aypos*c;

        cxTex+=depx/20000.;
        cyTex-=depy/20000.;

        double echelle=pow(facteur_zoom,zoom);

        xpos+=depx*echelle;
        ypos+=depy*echelle;

        if(zoom_in){zoom+=5.;zoom_ar+=facteur_zoom/2000.;}
        else if(zoom_out)
            {
                zoom-=5.;
                //if(zoom<0.25)zoom=0.25;
                zoom_ar-=facteur_zoom/2000.;
            }
        if(rot_CCW)alpha+=0.01;
        else if (rot_CW)alpha-=0.01;
        if(t_plus)t+=.001;
        else if(t_moins)t-=.001;
        if(iter_plus)iter++;
        else if (iter_moins){iter--;if (iter<1)iter=1;}

         if(GVars::disp_donnees)
                {
                    cout<<"xpos:"<<xpos<<" ypos:"<<ypos<<" zoom:"<<zoom<<" t:"<<t<<" decallage_palette:"<<decallage_palette<<" Angle:"<<alpha*180./M_PI<<endl;
                    GVars::disp_donnees=false;
                }
    }


    else if(depId==5)
    {
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
            cout<<"Cosmonaute - X="<<cosmonaute->x<<" Y="<<cosmonaute->y<<" Z="<<cosmonaute->z<<" H="<<GVars::cosmo->echelle<<" R="<<cosmonaute->r<<" T="<<cosmonaute->t<<" L="<<cosmonaute->l<<endl;
            GVars::disp_donnees=false;
        }
    }

    else if(depId==3)
    {
        float dep[3]{aX,aY,aZ};

        GFunc::rotation_RTL(0.,0.,0.,GVars::vaisseau_2->r*DEG_RAD,GVars::vaisseau_2->t*DEG_RAD,-GVars::vaisseau_2->l*DEG_RAD,dep,dep,1);

		GVars::vaisseau_2->x+=dep[0];
		GVars::vaisseau_2->y+=dep[1];
		GVars::vaisseau_2->z-=dep[2];

		GVars::vaisseau_2->r+=aR;
		GVars::vaisseau_2->t+=aT;
		GVars::vaisseau_2->l+=aL;

		if (GVars::disp_donnees)
        {
            cout<<"Vaisseau - X="<<GVars::vaisseau_2->x<<" Y="<<GVars::vaisseau_2->y<<" Z="<<GVars::vaisseau_2->z<<" R="<<GVars::vaisseau_2->r<<" T="<<GVars::vaisseau_2->t<<" L="<<GVars::vaisseau_2->l<<endl;
            GVars::disp_donnees=false;
        }
    }


    //decallage_palette=GVars::temps/100.;

}

void Mandelbrot::init_affiche_mandelbrot()
{
	GFunc::vue_ortho();
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_1D,palette_tex_id);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,GVars::espace_01->texture_id);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

    /*
	GVars::glow_contraste0=0.5;
	GVars::glow_contraste=0.;
	GVars::glow_saturation=1.;
	GVars::taille_hallo=10;
	GVars::saturation_hallo=1.;
	GVars::alpha_scene=1.;
	GVars::alpha_glow=1.;
	*/

	/*
	GVars::glow_contraste0=0.75;
	GVars::glow_contraste=5.;
	GVars::glow_saturation=1.;
	GVars::saturation_halo=1.;
	GVars::taille_hallo=20;
	GVars::alpha_scene=1.;
	GVars::alpha_glow=.5;
	*/

	glUseProgram(ShadersDemo::programme_mandelbrot);
	glUniform1f(ShadersDemo::uniform_mandelbrot_ecran_ratio,GVars::ecran_ratio);
	glUniform2f(ShadersDemo::uniform_mandelbrot_ajustement_resolution,1280./float(GVars::largeur_ecran),720./float(GVars::hauteur_ecran));
	glUniform1d(ShadersDemo::uniform_mandelbrot_zoom,pow(facteur_zoom,zoom));
	glUniform1d(ShadersDemo::uniform_mandelbrot_xpos,xpos);
	glUniform1d(ShadersDemo::uniform_mandelbrot_ypos,ypos);
	glUniform1f(ShadersDemo::uniform_mandelbrot_alpha,alpha+perturbation_alpha);
	glUniform1d(ShadersDemo::uniform_mandelbrot_t,1./t);
	glUniform1i(ShadersDemo::uniform_mandelbrot_iter,iter);
	glUniform1i(ShadersDemo::uniform_mandelbrot_palette,0);
	glUniform1i(ShadersDemo::uniform_mandelbrot_arriere_plan,1);
	glUniform1f(ShadersDemo::uniform_mandelbrot_decallage_palette,decallage_palette);
	glUniform2f(ShadersDemo::uniform_mandelbrot_ecranDim,GVars::largeur_ecran,GVars::hauteur_ecran);
}



void Mandelbrot::eclairages()
{
    Lumieres::lumiere_attenuation(GL_LIGHT0,L0_attenuation,0.00001,0.);
    Lumieres::lumiere_position(GL_LIGHT0,L0_position);//1000.,1000.,-5000.);

    Lumieres::lumiere_couleur(GL_LIGHT0,L0_amb,L0_diff,L0_spec);
    /*
                                0.,0.,0.,
                                1.,1.,0.1,
                                1.,1.,0.1);
    */
    Lumieres::lumiere_ouverture(GL_LIGHT0,180.);

    Lumieres::lumiere_attenuation(GL_LIGHT1,0.0,0.0,0.);
    Lumieres::lumiere_couleur(GL_LIGHT1,L1_amb,L1_diff,L1_spec);
                            /*
                            0.,0.,0.,
                            0.3,0.3,0.6,
                            0.,0.,0.);
                            */
    Lumieres::lumiere_infinie_orientation(GL_LIGHT1,L1_orientation);//0.,0.,1.);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, couleur_amb);
    glFogi(GL_FOG_MODE,GL_LINEAR);
    glFogfv(GL_FOG_COLOR,couleur_fog);
    glFogf(GL_FOG_START,0.);
    glFogf(GL_FOG_END,distance_brouillard);
    glFogf(GL_FOG_DENSITY,1.);
    glHint(GL_FOG_HINT,GL_NICEST);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_FOG);
    Lumieres::drapeaux_sources=0b11;
    glEnable(GL_LIGHTING);
}

void Mandelbrot::affiche_mandelbrot()
{
    float dEcran=float(GVars::largeur_ecran)/float(GVars::hauteur_ecran);
    float c=cos(-alpha-perturbation_alpha);
	float s=sin(-alpha-perturbation_alpha);
	float texCoordsRot[8];
    float texCoords[8]={ -0.5/zoom_ar,0.5/(zoom_ar*dEcran),
                        0.5/zoom_ar,0.5/(zoom_ar*dEcran),
                        0.5/zoom_ar,-0.5/(zoom_ar*dEcran),
                        -0.5/zoom_ar,-0.5/(zoom_ar*dEcran),
                        };

	for(uint16_t i=0;i<4;i++)
    {

        texCoordsRot[2*i]=(texCoords[2*i]*c-texCoords[2*i+1]*s)+cxTex;
        texCoordsRot[2*i+1]=(texCoords[2*i]*s+texCoords[2*i+1]*c)+cyTex;
        /*
        texCoordsRot[2*i]=texCoords[2*i]+cxTex;
        texCoordsRot[2*i+1]=texCoords[2*i+1]+cyTex;
        */
    }

	glBegin( GL_QUADS );
        glMultiTexCoord2fv(GL_TEXTURE1,&texCoordsRot[0]);
        glVertex3f(-1.,-1.,-1.);
        glMultiTexCoord2fv(GL_TEXTURE1,&texCoordsRot[2]);
        glVertex3f(1.,-1.,-1.);
        glMultiTexCoord2fv(GL_TEXTURE1,&texCoordsRot[4]);
        glVertex3f(1.,1.,-1.);
        glMultiTexCoord2fv(GL_TEXTURE1,&texCoordsRot[6]);
        glVertex3f(-1,1,-1.);
        glEnd( );
}

void Mandelbrot::affiche_nuages()
{
    GFunc::desactive_textures();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glUseProgram(0);
    glDisable(GL_DEPTH_TEST); //Désactive la prise en compte de la profondeur ("z-buffer")
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    //glDepthMask(GL_FALSE);
    //glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    //glCullFace(GL_CCW);
    GFunc::vue_perspective(zMin,zMax);
    GFunc::init_matrice_observateur(&obs);
    nuages->affiche_deplacement_vecteur_direction(GVars::temps_scene,0.);
}

void Mandelbrot::affiche_vaisseau()
{
        glLoadIdentity();
        GFunc::vue_perspective(10.,1500.);
        GFunc::init_matrice_observateur(&obs);
        glUseProgram(0);
        GFunc::desactive_textures();
        eclairages();
        glFogf(GL_FOG_START,800.);
        glFogf(GL_FOG_END,1500.);
        float obsPos[3]={obs.x,obs.y,-obs.z};
        glPushMatrix();
        GVars::vaisseau_2->calcul_matrice();
        GVars::vaisseau_2->affiche(obsPos);
        glPopMatrix();
    }

void Mandelbrot::render_scene_mandelbrot()
{
    obs.x=(pertrube_obsX.valeur_aleatoire_temporelle(GVars::temps_scene,.05)*2.-1.)*perturbationObs;
    obs.y=(pertrube_obsY.valeur_aleatoire_temporelle(GVars::temps_scene,.05)*2.-1.)*perturbationObs;
    obs.z=(pertrube_obsZ.valeur_aleatoire_temporelle(GVars::temps_scene,.05)*2.-1.)*perturbationObs;

    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    init_affiche_mandelbrot();
    affiche_mandelbrot();

    if(drapeau_affiche_screenShot)FrameBuffers::affiche_screenShot();
    if(drapeau_affiche_nuages)affiche_nuages();
    if(drapeau_affiche_vaisseau) affiche_vaisseau();
}
