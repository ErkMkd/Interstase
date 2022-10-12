
#include "Fonctions.h"

using namespace std;

        GLboolean GFunc::eclairages_mem;
        GLboolean GFunc::blend_mem;
        GLboolean GFunc::mask_mem;
        GLboolean GFunc::color_material_mem;
        GLint GFunc::blendSrc;
        GLint GFunc::blendDst;
        GLboolean GFunc::depthTest_mem;
        GLboolean GFunc::culling_mem;
        GLint GFunc::frontFace_mem;

        int GFunc::edit_keys=EDIT_RIEN;


//---------Fonction appelée par GLFW lorsqu'une erreur se produit:

void GFunc::glfw_error_callback(int error,const char* description)
{
        cout<<"Erreur GLFW: "<<error<<" - "<<description<<endl;
}


void GFunc::dimensionne_ecran()
{
                //float ratio;
                int width, height;
                glfwGetFramebufferSize(GVars::ecran, &width, &height);
                cout<<"Width: "<<width<<" - "<<"Height: "<<height<<endl;
                //ratio = width / (float) height;
                //glViewport(0, 0, width, height);
                //glClear(GL_COLOR_BUFFER_BIT);
}

void GFunc::redimensionne_ecran(GLFWwindow* window, int width, int height)
{
        GVars::largeur_ecran_visible=width;
        GVars::hauteur_ecran_visible=height;
        dimensionne_ecran();
        //FrameBuffers::redimensionne_frameBuffers();
        //Sprites_2d::determine_resolution(GVars::largeur_ecran,GVars::hauteur_ecran);
}

//------------ Gestion du clavier:


void GFunc::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //--------------Touches globales:

        //cout<<key<<endl;
            bool etat=false;
            if(action==GLFW_PRESS || action==GLFW_REPEAT) etat=true; else etat=false;

        if(key==GLFW_KEY_ESCAPE)	glfwSetWindowShouldClose(window,GL_TRUE);
        else if(key==GLFW_KEY_T && etat)GVars::vitesse_temps+=.5;
        else if(key==GLFW_KEY_R && etat)GVars::vitesse_temps-=.5;

        else if(key==GLFW_KEY_HOME) GVars::cosmo_echelle_inc=etat;
        else if(key==GLFW_KEY_END) GVars::cosmo_echelle_dec=etat;

        /*
        else if(!GVars::editionTrajectoire && key==GLFW_KEY_ENTER)
        {
            if(etat)GVars::scene_id++;
            if(GVars::scene_id>NUM_SCENES)GVars::scene_id=1;
        }
        */

        else if(key==GLFW_KEY_SPACE && etat==true) GVars::disp_donnees=true;   //A mettre à false par la scène.

        else if(key==GLFW_KEY_F && etat)
        {
            GFunc::edit_keys++;
            if(GFunc::edit_keys>NUM_EDIT_SETS)GFunc::edit_keys=0;
        }
        else if (key==GLFW_KEY_F12 && etat)
        {
            if(GVars::editionTrajectoire) GVars::editionTrajectoire=false;
            else GVars::editionTrajectoire=true;
        }


    //---------------- Touches spécifiques aux scènes:
        else
        {
            switch (GVars::scene_id)
            {
                case SCENE_INTRO: Intro::clavier(key,etat);break;
                case SCENE_HYPNOSTRUCTURE: HypnoStructure::clavier(key,etat);break;
                case SCENE_MANDELBROT: Mandelbrot::clavier(key,etat);break;
                case SCENE_SPHEREDANCE: SphereDance::clavier(key,etat);break;
                case SCENE_WATERWORLD: WaterWorld::clavier(key,etat);break;
                case SCENE_FINAL: Final::clavier(key,etat);break;
                case SCENE_VILLE: Ville::clavier(key,etat);break;
                default:break;
            }
        }

    //------------Touches d'éditions des filtres:

    if(GFunc::edit_keys==EDIT_FILTRE_HSV)
    {
        if(key==GLFW_KEY_H && etat){if(GVars::drapeau_filtre_HSV)GVars::drapeau_filtre_HSV=false;else GVars::drapeau_filtre_HSV=true;}
        else if(key==GLFW_KEY_I && etat)GVars::hue+=1.;
        else if(key==GLFW_KEY_K && etat)GVars::hue-=1.;
        else if(key==GLFW_KEY_O && etat)GVars::saturation+=0.01;
        else if(key==GLFW_KEY_L && etat)GVars::saturation-=0.01;
        else if(key==GLFW_KEY_P && etat)GVars::value+=0.01;
        else if(key==GLFW_KEY_M && etat)GVars::value-=0.01;
    }
    else if(GFunc::edit_keys==EDIT_GLOW)
    {
        if(key==GLFW_KEY_G && etat){if(GVars::drapeau_affiche_glow)GVars::drapeau_affiche_glow=false;else GVars::drapeau_affiche_glow=true;}
        else if(key==GLFW_KEY_EQUAL && etat)GVars::alpha_glow+=.05;
        else if(key==GLFW_KEY_SEMICOLON && etat)GVars::alpha_glow-=.05;
        else if(key==GLFW_KEY_LEFT_BRACKET && etat)GVars::alpha_scene+=.05;
        else if(key==GLFW_KEY_RIGHT_BRACKET && etat)GVars::alpha_scene-=.05;
        else if(key==GLFW_KEY_0 && etat)GVars::saturation_hallo+=.05;
        else if(key==GLFW_KEY_P && etat)GVars::saturation_hallo-=.05;
        else if(key==GLFW_KEY_9 && etat)GVars::taille_hallo++;
        else if(key==GLFW_KEY_O && etat){GVars::taille_hallo--;if (GVars::taille_hallo<1)GVars::taille_hallo=1;}
        else if(key==GLFW_KEY_8 && etat)GVars::glow_saturation+=.05;
        else if(key==GLFW_KEY_I && etat)GVars::glow_saturation-=.05;
        else if(key==GLFW_KEY_M && etat)GVars::glow_contraste+=.05;
        else if(key==GLFW_KEY_WORLD_1 && etat)GVars::glow_contraste-=.05;
        else if(key==GLFW_KEY_L && etat)GVars::glow_contraste0+=.01;
        else if(key==GLFW_KEY_SLASH && etat)GVars::glow_contraste0-=.01;
        else if(key==GLFW_KEY_B && etat){if(GVars::drapeau_blend_glow)GVars::drapeau_blend_glow=false;else GVars::drapeau_blend_glow=true;}

    }

}

void GFunc::setup_opengl()
{

        /* Our shading model--Gouraud (smooth). */
        glShadeModel( GL_SMOOTH );

        /* Culling. */
        glCullFace( GL_BACK );
        glFrontFace( GL_CW );
        glEnable( GL_CULL_FACE );
        glDisable(GL_LIGHTING);

        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);
        glBlendFunc(GL_SRC_ALPHA,GL_DST_ALPHA);

        /* Set the clear color. */
        glReadBuffer(GL_BACK);
        glDrawBuffer(GL_BACK);
        glDepthFunc(GL_LESS); //Test de comparaison avec la profondeur précédente
        glClearColor( 0, 0, 0, 0 );

        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE); //Sert à définir que c'est la position de l'oeil qui sert pour le calcul de la spécularité.
        glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR); //La lumière spéculaire est ajoutée par dessus le texturage, pour éviter de l'altérer.
}

void GFunc::sauve_OpenGl()
{
        eclairages_mem=glIsEnabled(GL_LIGHTING);
        glGetBooleanv(GL_DEPTH_WRITEMASK,&mask_mem);
        color_material_mem=glIsEnabled(GL_COLOR_MATERIAL);
        blend_mem=glIsEnabled(GL_BLEND);
        depthTest_mem=glIsEnabled(GL_DEPTH_TEST);
        glGetIntegerv(GL_BLEND_SRC_ALPHA,&blendSrc);
        glGetIntegerv(GL_BLEND_DST_ALPHA,&blendDst);
        culling_mem=glIsEnabled(GL_CULL_FACE);
        glGetIntegerv(GL_FRONT_FACE,&frontFace_mem);
}

void GFunc::restitue_OpenGl()
{
    if (color_material_mem) glEnable(GL_COLOR_MATERIAL); else glDisable(GL_COLOR_MATERIAL);
    if (blend_mem)glEnable(GL_BLEND); else glDisable(GL_BLEND);
    glDepthMask(mask_mem);
    glBlendFunc(blendSrc,blendDst);
    if (eclairages_mem) glEnable(GL_LIGHTING); else glDisable(GL_LIGHTING);
    if (depthTest_mem)glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
    if (culling_mem)glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
    glFrontFace(frontFace_mem);
}


//====================================================================
//              Désactive toutes les textures.
//====================================================================
void GFunc::desactive_textures()
{
    uint16_t i;
    GLint nbr_unites_textures_max;
    glGetIntegerv(GL_MAX_TEXTURE_UNITS,&nbr_unites_textures_max);
    for (i=0;i<nbr_unites_textures_max;i++)
        {
            glActiveTexture(GL_TEXTURE0+i);
            glDisable(GL_TEXTURE_2D);
            glClientActiveTexture(GL_TEXTURE0+i);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }
        glActiveTexture(GL_TEXTURE0);
}

void GFunc::init_matrice_observateur_raytrace(float x,float y, float z, float r, float t, float l)
{
	glMatrixMode(GL_MODELVIEW);
	//glLoadMatrixf(ObsMat.matrice);
	glLoadIdentity();
	glTranslatef(x,y,z);
	glRotatef(l,0.,1.,0.);
	glRotatef(t,1.,0.,0.);
	glRotatef(r,0.,0.,1.);

	glGetFloatv(GL_MODELVIEW_MATRIX,GVars::ObsMat->matrice);
	glLoadIdentity();
}

void GFunc::init_matrice_observateur_raytrace(Repere* rep)
{
    init_matrice_observateur_raytrace(rep->x,rep->y,rep->z,rep->r,rep->t,rep->l);
}

void GFunc::init_matrice_observateur(float x,float y, float z, float r, float t, float l,bool translation)
{
		glRotatef(-r,0.,0.,1.);
		glRotatef(t,1.,0.,0.);
		glRotatef(l,0.,1.,0.);
		if(translation) glTranslatef(-x,-y,z);
}
void GFunc::init_matrice_observateur(Repere* rep,bool translation)
{
    glRotatef(-rep->r,0.,0.,1.);
    glRotatef(rep->t,1.,0.,0.);
    glRotatef(rep->l,0.,1.,0.);
    if(translation) glTranslatef(-rep->x,-rep->y,rep->z);
}

//A partir des matrices de projection et de transformation calcul les coordonnées du point sur l'écran:
//Le point de sortie comporte 3 coordonnées, x,y,z-Buffer
void  GFunc::calcul_coordonnees_ecran(float* pointEspace,float* pointEcran)
{
    float p[4]={pointEspace[0],pointEspace[1],pointEspace[2],1.};
    float p_temp[4];
    Matrice4x4f modelView;
    Matrice4x4f projection;
    glGetFloatv(GL_MODELVIEW_MATRIX,modelView.matrice);
    glGetFloatv(GL_PROJECTION_MATRIX,projection.matrice);
    modelView.multiplie_point3d(p,p_temp);
    projection.multiplie_point3d(p_temp,p);
    //Coordonnées normalisées:
    float inverse_w=1./p[3];
    p[0]*=inverse_w;
    p[1]*=inverse_w;
    p[2]*=inverse_w;
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT,vp);
    pointEcran[0]=(p[0]*0.5+0.5)*vp[2]+vp[0];
    pointEcran[1]=(p[1]*0.5+0.5)*vp[3]+vp[1];
    pointEcran[2]=(1.+p[2])*0.5;    //Z doit être compris entre 0 et 1, sinon il est hors-champ.
}

void GFunc::init_matrice_observateur_reflet_eau(Repere* rep,float altitude_plan,bool translation)
{
    init_matrice_observateur(rep->x,altitude_plan-(rep->y-altitude_plan),rep->z,-rep->r+180.,-rep->t,rep->l,translation);
}

// rotation d'un vecteur dans un plan vertical ou horizontal:
void GFunc::rotation_plan(float angle,float sX, float sY, float& dX, float& dY)
{
	float c=cosf(angle*DEG_RAD);
	float s=sinf(angle*DEG_RAD);
	dX=sX*c-sY*s;
	dY=sX*s+sY*c;
}

void GFunc::maj_distanceFocale()
{
                GVars::distanceFocale=GVars::largeur_ecran/(2.*tan(GVars::Fov*DEG_RAD/2.));
}

void GFunc::vue_ortho()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1,1,-1.,1.,.5,10.);
	glMatrixMode(GL_MODELVIEW);
}

void GFunc::vue_perspective(float zMin,float zMax)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLdouble frustum_largeur=GVars::largeur_ecran/2./GVars::distanceFocale*zMin;
	GLdouble frustum_hauteur=GVars::hauteur_ecran/2./GVars::distanceFocale*zMin;
	glFrustum(-frustum_largeur,frustum_largeur,-frustum_hauteur,frustum_hauteur,zMin,zMax);
	glMatrixMode(GL_MODELVIEW);
	//cout<<frustum_largeur<<" / "<<frustum_hauteur<<endl;
}


void GFunc::RGBi_RGBf(uint32_t RGBi,float RGBf[3])
{
    RGBf[0]=float((RGBi>>16)&0xff)/255.;
    RGBf[1]=float((RGBi>>8)&0xff)/255.;
    RGBf[2]=float(RGBi&0xff)/255.;
}

void GFunc::BGRi_RGBf(uint32_t BGRi,float RGBf[3])
{
    RGBf[2]=float((BGRi>>16)&0xff)/255.;
    RGBf[1]=float((BGRi>>8)&0xff)/255.;
    RGBf[0]=float(BGRi&0xff)/255.;
}

void GFunc::affiche_texture(float alpha,float r, float v, float b)
{
	glColor4f(r,v,b,alpha);
        glBegin(GL_QUADS);

            glTexCoord2f(0.,1.);
            glVertex3f(-1.,1.,-1.);

            glTexCoord2f(1.,1.);
            glVertex3f(1.,1.,-1.);

            glTexCoord2f(1.,0.);
            glVertex3f(1.,-1.,-1.);

            glTexCoord2f(0.,0.);
            glVertex3f(-1.,-1.,-1.);
        glEnd();
}

void GFunc::genere_ombre_sol_listes()
{

    glNewList(GVars::liste_ombre_sol_id,GL_COMPILE);
        glBegin(GL_QUADS);
            glTexCoord2f(0.,0.);
            glVertex3f(-0.8,0.,-0.8);
            glTexCoord2f(1.,0.);
            glVertex3f(0.8,0.,-0.8);
            glTexCoord2f(1.,1.);
            glVertex3f(0.8,0.,0.8);
            glTexCoord2f(0.,1.);
            glVertex3f(-0.8,0.,0.8);
        glEnd();
    glEndList();

    glNewList(GVars::liste_ombre_sol_flat_id,GL_COMPILE);
        glBegin(GL_POLYGON);
            float a;
            for(int i=0;i<12;i++)
            {
                float x=cos(a)/2.;
                float z=sin(a)/2.;
                glVertex3f(x,0.,z);
                a+=M_PI/6.;
            }
        glEnd();
    glEndList();
}

void GFunc::affiche_ombre_sol(float x,float y, float z, float sx,float sy,float sz,float alpha,GLuint texture_id)
{
        GLboolean light_mem;
        glTranslatef(x,y,z);
        glScalef(sx,sy,sz);
        glColor4f(1.,1.,1.,alpha);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        //glDisable(GL_CULL_FACE);
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        light_mem=glIsEnabled(GL_LIGHTING);
        glDisable(GL_LIGHTING);
        glDepthMask(GL_FALSE);
        if(texture_id==0)glBindTexture(GL_TEXTURE_2D,GVars::ombre_sol->texture_id);
        else glBindTexture(GL_TEXTURE_2D,texture_id);
        glUseProgram(0);

            glCallList(GVars::liste_ombre_sol_id);

        if(light_mem)glEnable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
}

void GFunc::affiche_ombre_sol_flat(float x,float y, float z, float sx,float sy,float sz,float alpha)
{
        GLboolean light_mem;
        glTranslatef(x,y,z);
        glScalef(sx,sy,sz);
        glColor4f(0.,0.,0.,alpha);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        light_mem=glIsEnabled(GL_LIGHTING);
        glDisable(GL_LIGHTING);

            glCallList(GVars::liste_ombre_sol_flat_id);

        if(light_mem)glEnable(GL_LIGHTING);
        glDisable(GL_BLEND);
}

//---------------------------------------------------------------
//Rotation de points autour d'un axe:
//---------------------------------------------------------------
void GFunc::rotation_points(float cx,float cy,float cz,float ax, float ay, float az, float angle, float* points, float* points_resultat, uint32_t numPoints)
{
	for (uint32_t i = 0; i < numPoints; i++)
	{
		float px = points[3 * i] - cx;
		float py = points[3 * i + 1] - cy;
		float pz = points[3 * i + 2] - cz;
		float prod_scal =  px * ax + py * ay + pz * az;
		float cos_angle = cos(angle);
		float sin_angle = sin(angle);

		points_resultat[3 * i] = cos_angle * px + sin_angle * (ay * pz - az * py) + (1. - cos_angle) * prod_scal * ax + cx;
		points_resultat[3 * i + 1] = cos_angle * py + sin_angle * (az * px - ax * pz) + (1. - cos_angle) * prod_scal * ay + cy;
		points_resultat[3 * i + 2] = cos_angle * pz + sin_angle * (ax * py - ay * px) + (1. - cos_angle) * prod_scal * az + cz;
	}
}


//---------------------------------------------------------------
//Rotation de points en fonction des angles de Roulis, Tangage et Lacet:
//  Les angles sont en radians.
//---------------------------------------------------------------
void GFunc::rotation_RTL(float cx, float cy, float cz,float roulis,float tangage, float lacet, float* points, float* resultat,uint32_t numPoints)
{
	float sin_aR=sin(roulis);
	float cos_aR=cos(roulis);
	float sin_aT=sin(tangage);
	float cos_aT=cos(tangage);
	float sin_aL=sin(lacet);
	float cos_aL=cos(lacet);

	for(uint32_t i=0;i<numPoints;i++)
	{
		float px=points[3*i]-cx;
		float py=points[3*i+1]-cy;
		float pz=points[3*i+2]-cz;
		resultat[3*i]= sin_aL * ( pz*cos_aT - sin_aT * ( px*sin_aR + py*cos_aR ) ) +  cos_aL * ( px*cos_aR - py*sin_aR )+cx;
		resultat[3*i+1]= pz * sin_aT + cos_aT * ( px*sin_aR + py*cos_aR )+cy;
		resultat[3*i+2]= cos_aL * ( pz*cos_aT - sin_aT * ( px*sin_aR + py*cos_aR ) ) - sin_aL * ( px*cos_aR - py*sin_aR )+cz;
	}

}

//======================================================================
//          Affiche la texture de réflexion:
//======================================================================
void GFunc::affiche_reflet_eau(parametres_reflet_eau& params,float altitude_plan,float taille_plan)
{
    glViewport(0,0,GVars::largeur_ecran,GVars::hauteur_ecran);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,FrameBuffers::fbos[FBO_ECRAN]);
    GFunc::vue_perspective(GVars::zMin,GVars::zMax);
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    GFunc::init_matrice_observateur(GVars::obs);

    glShadeModel( GL_FLAT );
	glDisable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDisable(GL_FOG);
    GLfloat fogEndMem;
    glGetFloatv(GL_FOG_END,&fogEndMem);
    glFogf(GL_FOG_END,taille_plan/2.);
    glDisable(GL_LIGHTING);
    //glDisable(GL_DEPTH_TEST);
	GFunc::desactive_textures();

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,GVars::texture_reflet_eau->texture_id);
	glUseProgram(ShadersObjets3d::reflet_eau->programme_id);
	glUniform3f(ShadersObjets3d::reflet_eau->position_obs,GVars::obs->x,GVars::obs->y,-GVars::obs->z);
    glUniform1i(ShadersObjets3d::reflet_eau->texture_reflet,0);
    glUniform1f(ShadersObjets3d::reflet_eau->largeur_ecran,float(GVars::largeur_ecran));
    glUniform1f(ShadersObjets3d::reflet_eau->hauteur_ecran,float(GVars::hauteur_ecran));
    glUniform1f(ShadersObjets3d::reflet_eau->t,float(GVars::temps_scene));

    glActiveTexture(GL_TEXTURE1);
    glUniform1i(ShadersObjets3d::reflet_eau->texture_bruit,1);
    glBindTexture(GL_TEXTURE_2D,ShadersObjets3d::texture_bruit_id);
    glUniform1f(ShadersObjets3d::reflet_eau->transparence,params.transparence);
    glUniform1f(ShadersObjets3d::reflet_eau->attenuation_speculaire,params.attenuation_speculaire);
    glUniform1f(ShadersObjets3d::reflet_eau->intensite_ondes,params.intensite_ondes);
    glUniform1f(ShadersObjets3d::reflet_eau->vitesse_ondes,params.vitesse_ondes);
    glUniform1f(ShadersObjets3d::reflet_eau->quantite_ondes,params.quantite_ondes);
    glUniform1f(ShadersObjets3d::reflet_eau->frequence_ondes,params.frequence_ondes);
    glUniform1f(ShadersObjets3d::reflet_eau->amplitude_ondes,params.amplitude_ondes);
    glUniform1f(ShadersObjets3d::reflet_eau->niveau_perturbations_ondes,params.niveau_perturbations_ondes);
    glUniform1f(ShadersObjets3d::reflet_eau->echelle_texture_bruit_ondes,params.echelle_texture_bruit_ondes);
    glUniform3f(ShadersObjets3d::reflet_eau->position_source,params.position_source[0],params.position_source[1],params.position_source[2]);
    glUniform4f(ShadersObjets3d::reflet_eau->couleur_fond,params.couleur_fond[0],params.couleur_fond[1],params.couleur_fond[2],params.couleur_fond[3]);

    glMaterialf(GL_FRONT,GL_SHININESS,params.brillance);
    glMaterialfv(GL_FRONT,GL_SPECULAR,params.couleur_speculaire);

    glColor4f(1.,1.,1.,1.);
    glBegin(GL_QUADS);
        glVertex3f(taille_plan/2.+GVars::obs->x,altitude_plan,-taille_plan/2.-GVars::obs->z);
        glVertex3f(taille_plan/2.+GVars::obs->x,altitude_plan,taille_plan/2.-GVars::obs->z);
        glVertex3f(-taille_plan/2.+GVars::obs->x,altitude_plan,taille_plan/2.-GVars::obs->z);
        glVertex3f(-taille_plan/2.+GVars::obs->x,altitude_plan,-taille_plan/2.-GVars::obs->z);
    glEnd();
    glUseProgram(0);
    glFogf(GL_FOG_END,fogEndMem);

}
//======================================================================
//          Gestion cosmonaute
//======================================================================
void GFunc::animation_cosmo_decollage(float altitude_sol)
{
        Volume*cosmo=GVars::cosmo->renvoie_volume_parent();
        float cosmo_hauteur_bassin=8.8*GVars::cosmo->echelle;
        //if(cosmo->y<cosmo_hauteur_bassin)cosmo->y=cosmo_hauteur_bassin;

        GVars::cosmo_mix_debout_bruits=(cosmo->y-altitude_sol-cosmo_hauteur_bassin)/(20*GVars::cosmo->echelle)+0.2;
        if(GVars::cosmo_mix_debout_bruits>1.)GVars::cosmo_mix_debout_bruits=1.;
        GVars::cosmo->ab_jambes=.5*GVars::cosmo_mix_debout_bruits-0.2;
}

void GFunc::affiche_cosmonaute()
{
        //sauve_OpenGl();
        glMatrixMode(GL_MODELVIEW);

        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDisable(GL_COLOR_MATERIAL);

        //------------ Mouvements du cosmonaute:
        #if AFFICHE_TIMELINE
        if(GVars::vitesse_temps!=0.)
        {
        #endif
            if(GVars::drapeau_cosmo_mvt_bruits)GVars::cosmo->mouvement_bruits(GVars::temps,1.);
            else if(GVars::drapeau_cosmo_debout)GVars::cosmo->debout();
            else if(GVars::drapeau_cosmo_mvt_bruits_mix_debout)GVars::cosmo->mix_debout_mvt_bruits(GVars::temps,GVars::cosmo_mix_debout_bruits,1.);
            if(GVars::drapeau_rotation_cosmo_auto)
            {
                GVars::cosmoR=GVars::cosmo_R0_auto+(GVars::temps_scene-GVars::cosmo_R_auto_t0)*-10.*GVars::facteur_rotation_cosmo_auto;
                GVars::cosmoT=GVars::cosmo_T0_auto+(GVars::temps_scene-GVars::cosmo_T_auto_t0)*-7.13*GVars::facteur_rotation_cosmo_auto;
                GVars::cosmoL=GVars::cosmo_L0_auto+(GVars::temps_scene-GVars::cosmo_L_auto_t0)*-3.468*GVars::facteur_rotation_cosmo_auto;
                GVars::cosmo->determine_RTL(GVars::cosmoR,GVars::cosmoT,GVars::cosmoL);

            }
        #if AFFICHE_TIMELINE
        }
        else
        {
            if(GVars::raz_cosmonaute)
            {
                GVars::cosmoR=0.;
                GVars::cosmoT=0.;
                GVars::cosmoL=0.;
                GVars::cosmo->determine_RTL(GVars::cosmoR,GVars::cosmoT,GVars::cosmoL);
                GVars::cosmo->raz();
            }
        }
        #endif

        if(GVars::cosmo_echelle_dec)GVars::cosmo->determine_echelle(GVars::cosmo->echelle*0.99);
        if(GVars::cosmo_echelle_inc)GVars::cosmo->determine_echelle(GVars::cosmo->echelle*1.01);

        float obs_pos[3]={GVars::obs->x,GVars::obs->x,GVars::obs->z};
        GVars::cosmo->affiche(obs_pos);
        if(GVars::drapeau_affiche_ombre_cosmo) affiche_ombre_cosmo_sol();

        //GFunc::restitue_OpenGl();
}

//---------- Affiche l'ombre du cosmonaute au sol:
void GFunc::affiche_ombre_cosmo_sol()
{
    Volume* cosmo=GVars::cosmo->renvoie_volume_parent();
    float hauteur_bassin=8.8*GVars::cosmo->echelle;
    float alpha=1-((fabs(cosmo->y-GVars::altitude_ombre_cosmo))/(hauteur_bassin*10.));
    if(alpha>0.)
    {
        affiche_ombre_sol(cosmo->x,GVars::altitude_ombre_cosmo,cosmo->z,GVars::cosmo->echelle*10.,GVars::cosmo->echelle*10.,GVars::cosmo->echelle*10.,alpha*0.5);
    }
}

//----------- Affiche le reflet du cosmonaute dans le plan horizontal (eau)
void GFunc::rendu_reflet_cosmonaute(float altitude_plan)
{
    //---------------Affiche le reflet:
        glViewport(0,0,GVars::texture_reflet_eau->largeur,GVars::texture_reflet_eau->hauteur);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,GVars::texture_reflet_eau->fbo_id);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glEnable(GL_CLIP_PLANE0);
        glDisable(GL_BLEND);
        GLdouble equ_eau[4]={0.,1.,0.,-altitude_plan};
        glClipPlane(GL_CLIP_PLANE0,equ_eau);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT,GL_DIFFUSE);
        GVars::cosmo->drapeau_affiche_visage=false;
        //GVars::drapeau_affiche_ombre_cosmo=false;
        //glPushMatrix();
        //glTranslatef(0.,-altitude_plan,0.);
        GFunc::affiche_cosmonaute();
        //glPopMatrix();
        GVars::cosmo->drapeau_affiche_visage=true;
        //GVars::drapeau_affiche_ombre_cosmo=true;
        glDisable(GL_COLOR_MATERIAL);
        glDisable(GL_CLIP_PLANE0);
}


