#include "Texgen.h"
using namespace std;

    bool Texgen::erreur_openGl(const char* message)
    {
        GLenum erreur_openGl=glGetError();
        if (erreur_openGl!=GL_NO_ERROR)
        {
            cout<<message<<" : "<<gluErrorString(erreur_openGl)<<endl;
            return true;
        }
        return false;
    }

    void Texgen::desactive_textures()
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

    void Texgen::init_dataSize()
    {

        switch (type)
        {
            case GL_UNSIGNED_BYTE: octets_par_composante=1; break;
            case GL_FLOAT: octets_par_composante=4; break;
        }

        switch (format)
        {
            case GL_RG: octets_par_pixel=octets_par_composante*2; break;
            case GL_RGB: octets_par_pixel=octets_par_composante*3; break;
            case GL_RGBA: octets_par_pixel=octets_par_composante*4; break;
        }
    }

    bool Texgen::alloue_bitmap()
    {
        bitmap=(uint8_t*)(malloc(octets_par_pixel*largeur*hauteur));
        if(bitmap==NULL)return false;
        else return true;
    }

    Texgen::Texgen(GLsizei p_largeur,GLsizei p_hauteur, GLint p_format, GLint p_type,GLint recouvrement_s,GLint recouvrement_t )
    {
        erreur=TEXGEN_OK;
        fbo_id=0;
        fbo_depth_id=0;
        texture_id=0;
        largeur=p_largeur;
        hauteur=p_hauteur;
        format=p_format;
        type=p_type;
        bitmap=NULL;

        init_dataSize();
        if(!alloue_bitmap()) {erreur=TEXGEN_ERREUR_MEMOIRE; return;};

        couleur_bords[0]=0.;
        couleur_bords[1]=0.;
        couleur_bords[2]=0.;
        couleur_bords[3]=1.;

        glGenTextures(1,&texture_id);
        glBindTexture(GL_TEXTURE_2D,texture_id);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,recouvrement_s);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,recouvrement_t);
        glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,couleur_bords);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);    //Il semble que ce paramètre active la texture.....
        glTexImage2D(GL_TEXTURE_2D,0,format,largeur,hauteur,0,format,type,bitmap); //data=NULL: Alloue automatiquement la mémoire

        if(erreur_openGl("Erreur lors de la création de la texture - ")) erreur=TEXGEN_ERREUR_TEXTURE;
        cout<<"Texture allouée avec succès - Dimensions: "<<largeur<<" X "<<hauteur<<endl;

    }

    Texgen::~Texgen()
    {
        if(glIsFramebuffer(fbo_id))glDeleteFramebuffers(1,&fbo_id);
        if(glIsRenderbuffer(fbo_depth_id))glDeleteRenderbuffers(1,&fbo_depth_id);
        if(glIsTexture(texture_id))glDeleteTextures(1,&texture_id);
        if(bitmap!=NULL)free(bitmap);
    }

    bool Texgen::genere_framebuffer(bool drapeau_depthBuffer)
    {
        cout<<"01"<<endl;
        if(!glIsTexture(texture_id)) {erreur=TEXGEN_ERREUR_TEXTURE;return false;}
        cout<<"02"<<endl;
        glGenFramebuffers(1,&fbo_id);
        cout<<"03"<<endl;
        if(erreur_openGl("Erreur lors de la création du fbo - ")) {erreur=TEXGEN_ERREUR_FBO;return false;}
        cout<<"04"<<endl;
        if(drapeau_depthBuffer)
        {
            glGenRenderbuffers(1,&fbo_depth_id);
            if(erreur_openGl("Erreur lors de la création du tempon de profondeur - ")) {erreur=TEXGEN_ERREUR_FBO_DEPTH;return false;}
            glBindRenderbuffer(GL_RENDERBUFFER,fbo_depth_id);
            glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT32,largeur,hauteur);
            if(erreur_openGl("Erreur lors de la création du tempon de profondeur - ")) {erreur=TEXGEN_ERREUR_FBO_DEPTH;return false;}
        }

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,fbo_id);
        if(drapeau_depthBuffer)
        {
            glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,fbo_depth_id);
        }
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,texture_id,0);
        GLenum fb_status;
        fb_status=glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
        if(fb_status!=GL_FRAMEBUFFER_COMPLETE)
        {
            cout<<"Erreur de vérifiaction du frameBuffer. Status:"<<fb_status<<endl;
            return false;
        }

        if(erreur_openGl("Erreur lors de la création du fbo - ")) {erreur=TEXGEN_ERREUR_FBO;return false;}

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
        glBindTexture(GL_TEXTURE_2D,0);

        return true;
    }

    void Texgen::texCRpart(GLint x,GLint y,GLsizei l, GLsizei h,GLFWwindow* ecran)
    {
            glViewport(x,y,l,h);

            glBegin(GL_QUADS);
                glColor4f(1.,0.,1.,1.);

                glTexCoord2f(0.,0.);
                glVertex3f(-1.,-1.,-1.);

                glTexCoord2f(1.,0.);
                glVertex3f(1.,-1.,-1.);

                glTexCoord2f(1.,1.);
                glVertex3f(1.,1.,-1.);

                glTexCoord2f(0.,1.);
                glVertex3f(-1.,1.,-1.);
            glEnd();

            glfwSwapBuffers(ecran);

    }

    void Texgen::calcule_conesRelief_GPU(Image* tex_profondeur,float echelle_profondeur,GLFWwindow* ecran)
    {

        // Génère les textures:
        glShadeModel( GL_SMOOTH );
        glDisable(GL_CULL_FACE);
        glDisable(GL_LIGHTING);
        glDisable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1,1,-1.,1.,.5,10.);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity( );


        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,fbo_id);
        desactive_textures();
        glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,tex_profondeur->texture_id);
        glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);

        glUseProgram(ShadersObjets3d::relief_genCones->programme_id);
        glUniform1i(ShadersObjets3d::relief_genCones->texture_profondeurs,0);
        glUniform1i(ShadersObjets3d::relief_genCones->largeur_texture_sortie,largeur);
        glUniform1i(ShadersObjets3d::relief_genCones->hauteur_texture_sortie,hauteur);
        glUniform1f(ShadersObjets3d::relief_genCones->echelle_profondeur,echelle_profondeur);

        int partL=128;
        int partH=128;

        int restL=largeur%partL;
        int restH=hauteur%partH;
        int nbrPartL=(largeur-restL)/partL;
        int nbrPartH=(hauteur-restH)/partH;

        //cout<<nbrPartL<<" "<<restL<<" "<<nbrPartH<<" "<<restH<<endl;

        int x,y;

        for(y=0;y<nbrPartH;y++)
        {
            for(x=0;x<nbrPartL;x++)
            {
                texCRpart(partL*x,partH*y,partL,partH,ecran);
            }
            if(restL>0) texCRpart(partL*x,partH*y,restL,partH,ecran);
        }
        if(restH>0)
        {
            for(x=0;x<nbrPartL;x++)
            {
                texCRpart(partL*x,partH*y,partL,restH,ecran);
            }
            if(restL>0) texCRpart(partL*x,partH*y,restL,restH,ecran);
        }

    }

    //!!! Ne sert pas à redimensionner la texture, juste mettre à jours le contenu du bitmap.
    void Texgen::maj_texture()
    {
        if(glIsTexture(texture_id))
        {
            glBindTexture(GL_TEXTURE_2D,texture_id);
            glTexImage2D(GL_TEXTURE_2D,0,format,largeur,hauteur,0,format,type,bitmap); //data=NULL: Alloue automatiquement la mémoire
            if(erreur_openGl("Erreur lors de la mise à jour de la texture - ")) {erreur=TEXGEN_ERREUR_TEXTURE;}
        }

        if(glIsRenderbuffer(fbo_depth_id))
        {
            glBindRenderbuffer(GL_RENDERBUFFER,fbo_depth_id);
            glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT32,largeur,hauteur);
            if(erreur_openGl("Erreur lors de la mise à jour du tempon de profondeur - ")) {erreur=TEXGEN_ERREUR_FBO_DEPTH;}
        }

    }

    void Texgen::efface_texture()
    {
        for(uint32_t i=0;i<(octets_par_pixel*largeur*hauteur)/4;i++)
        {
            *(uint32_t*)(bitmap+i*4)=0;
        }
    }

    void Texgen::calcule_conesRelief_CPU(Image* texture_profondeur,float echelle_profondeur)
    {
        int tx,ty;
        if(format!=GL_RG || type!=GL_FLOAT) return;

        #define TEXGEN_EPSILON_PROFONDEUR 1e-6

        for(ty=0;ty<hauteur;ty++)
        {
            cout<<"ligne:"<<ty<<"Octets par pixels:"<<texture_profondeur->octets_par_pixel<<endl;
            for(tx=0;tx<largeur;tx++)
            {
                float x0=float(tx)/float(largeur);
                float y0=float(ty)/float(hauteur);

                uint32_t texel=(texture_profondeur->renvoie_texel(x0,y0)>>8)&0xff;
                float profondeur0=float(texel)/255.*echelle_profondeur;
                //cout<<hex<<texel<<endl;

                float angleMin=3.14159/2.;
                float angle;
                vec3f vCone(0.,0.,0.);
                float x1,y1,profondeur1;
                int px,py;


                for(py=0;py<hauteur;py++)
                {
                    for(px=0;px<largeur;px++)
                    {
                        x1=float(px)/float(largeur);
                        y1=float(py)/float(hauteur);
                        profondeur1=float((texture_profondeur->renvoie_texel(x1,y1)>>8)&0xff)/255.*echelle_profondeur;
                        if(profondeur1-TEXGEN_EPSILON_PROFONDEUR<profondeur0)
                        {
                            vCone.x=x1-x0;
                            vCone.y=y1-y0;
                            vCone.z=profondeur0-profondeur1;
                            vCone.normalise();
                            angle=acosf(vCone.z);
                            if(angle<angleMin)angleMin=angle;
                        }
                    }
                }


                //vec4 texel=texture2D( texture_profondeurs, coordonnees_texture );
                uint32_t offsetTexgen=(tx+ty*largeur)*8;
                *((float*)(bitmap+offsetTexgen))=profondeur0;
                *((float*)(bitmap+offsetTexgen+4))=angleMin/(3.14159/2.);
            }
        }
        maj_texture();
    }
