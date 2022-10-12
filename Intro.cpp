#include "Intro.h"

Repere Intro::obs;

float Intro::aX=0.;
float Intro::aY=0.;
float Intro::aZ=0.;

float Intro::aR=0.;
float Intro::aT=0.;
float Intro::aL=0.;

//Sprite oeil logo Mankind:
float Intro::mkdX=688./1280.;
float Intro::mkdY=(720.-301.5)/720.;
float Intro::mkdsX=40./30.;
float Intro::mkdsY=8./31.;


//Commandes clavier:
bool Intro::dep_droite=false;
bool Intro::dep_gauche=false;
bool Intro::dep_haut=false;
bool Intro::dep_bas=false;
bool Intro::dep_avant=false;
bool Intro::dep_arriere=false;

bool Intro::rot_droite=false;
bool Intro::rot_gauche=false;
bool Intro::rot_haut=false;
bool Intro::rot_bas=false;
bool Intro::rot_r_gauche=false;
bool Intro::rot_r_droite=false;
int Intro::depId=1;

float Intro::nebuleuseH=0.5;
float Intro::nebuleuseR=0.;

float Intro::C_amb[4]={0.,0.,0.,1.};
float Intro::L1_intensite=1.;
float Intro::L1_couleur[3]={223./255.,253./255.,255./255.};

Champ_Etoiles* Intro::starfield=NULL;


bool Intro::init_scene()
{
    starfield=new Champ_Etoiles();
    vec3f v(0.,0.,0.1);
    starfield->initialise(

                            5000.,1000.,1000.,
                            2000,
                            GVars::etoile_01,
                            50.,
                            1.,0.75,&v,1.);

    /*
    float p_profondeur,
                    float p_largeur,
                    float p_hauteur,
                    unsigned int p_nbr_etoiles,
                    Image *p_image,
                    float echelle,
                    float p_alpha,
                    float p_vitesse_rotation,
                    vec3f *p_vitesse,
                    float masse //Masse des étoiles
    */
    if(starfield->erreur!=0) return false;

    GVars::spritesDemo->determine_alpha_sprite(SPR_OEILLOGO,0.);
    GVars::spritesDemo->determine_alpha_sprite(SPR_LOGO_MKD,0.);
    GVars::spritesDemo->determine_alpha_sprite(SPR_NEBULEUSE,0.);

    return true;
}
void Intro::detruit_scene()
{

}
void Intro::clavier(int key,bool etat)
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

                    case GLFW_KEY_F1: depId=1;break;
                    case GLFW_KEY_F2: depId=2;break;
                    case GLFW_KEY_F3: depId=3;break;
                    case GLFW_KEY_F4: depId=4;break;
                    case GLFW_KEY_F5: depId=5;break;

                    case GLFW_KEY_F12: if(etat)GVars::editionTrajectoire=true;break; //Editeur de trajectoire
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
        }
    }
}
void Intro::maj_deplacement()
{
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
            cout<<"Cosmonaute - X="<<cosmonaute->x<<" Y="<<cosmonaute->y<<" Z="<<cosmonaute->z<<" R="<<cosmonaute->r<<" T="<<cosmonaute->t<<" L="<<cosmonaute->l<<endl;

        }
	}

	GVars::disp_donnees=false;

}
void Intro::init_affiche()
{

}

void Intro::eclairages()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glDisable(GL_FOG);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, C_amb);
    Lumieres::drapeaux_sources=0b11;


    Lumieres::lumiere_couleur(GL_LIGHT0,
                            0.,0.,0.,
                            L1_couleur[0]*L1_intensite,L1_couleur[1]*L1_intensite,L1_couleur[2]*L1_intensite,
                            L1_couleur[0]*L1_intensite,L1_couleur[1]*L1_intensite,L1_couleur[2]*L1_intensite);
    Lumieres::lumiere_attenuation(GL_LIGHT0,0.0,0.00001,0.);
    Lumieres::lumiere_position(GL_LIGHT0,2000.,1000.,-10000.);

    Lumieres::lumiere_attenuation(GL_LIGHT1,0.0,0.0,0.);
    Lumieres::lumiere_couleur(GL_LIGHT1,
                            0.,0.,0.,
                            0.2,0.2,0.25,
                            0.,0.,0.);
    Lumieres::lumiere_infinie_orientation(GL_LIGHT1,0.,0.,1.);

    glFogf(GL_FOG_DENSITY,0.);  //Pour mes shaders...


}

void Intro::affiche()
{
    GVars::spritesDemo->montre_sprite(SPR_NEBULEUSE);
    GVars::spritesDemo->determine_position_sprite(SPR_NEBULEUSE,0.66,0.66);
    GVars::spritesDemo->determine_rotation_sprite(SPR_NEBULEUSE,nebuleuseR);
    GVars::spritesDemo->determine_echelle_sprite(SPR_NEBULEUSE,nebuleuseH);
    //GVars::spritesDemo->determine_alpha_sprite(SPR_NEBULEUSE,0.75);
    nebuleuseR=45.+0.75*GVars::temps_scene;
    nebuleuseH=1.8+.01*GVars::temps_scene;
    GVars::spritesDemo->affiche_sprites_2d();
    GVars::spritesDemo->cache_sprite(SPR_NEBULEUSE);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
        glDisable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        glUseProgram(0);
        glDisable(GL_DEPTH_TEST); //Désactive la prise en compte de la profondeur ("z-buffer")
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        glDepthMask(GL_FALSE);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
    //glCullFace(GL_CCW);
    GFunc::vue_perspective(GVars::zMin,GVars::zMax);
    starfield->affiche_deplacement_axe_z(10.,GVars::temps_scene,0.);
}
void Intro::render_scene()
{
    init_affiche();
    affiche();
}
