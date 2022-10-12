
#include "FrameBuffers.h"
#include "Fonctions.h"

using namespace std;

//Frame buffer objets - Tempons de rendus

    GLuint 	FrameBuffers::fbos[NUM_FBOS]={0};
    GLuint	FrameBuffers::fbTex[NUM_FBOS]={0};
    GLuint	FrameBuffers::fbDepth[NUM_FBOS]={0};

    bool FrameBuffers::erreur_openGl(const char* message)
    {
        GLenum erreur_openGl=glGetError();
        if (erreur_openGl!=GL_NO_ERROR)
        {
            cout<<message<<" : "<<gluErrorString(erreur_openGl)<<endl;
            return true;
        }
        return false;
    }

    void FrameBuffers::supprime_frameBuffers()
    {
        glDeleteFramebuffers(NUM_FBOS,fbos);
        glDeleteRenderbuffers(NUM_FBOS,fbDepth);
        glDeleteTextures(NUM_FBOS,fbTex);
    }

    bool FrameBuffers::verification_fbo()
    {
        GLenum fb_status;
        fb_status=glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
        if(fb_status!=GL_FRAMEBUFFER_COMPLETE)
        {
            cout<<"Erreur de vérifiaction du frameBuffer. Status:"<<fb_status<<endl;
            return false;
        }
        return true;
    }

    bool FrameBuffers::initialise_frameBuffers()
    {
        GLfloat couleur_bords[3]={0.,0.,0.};

        glGenTextures(NUM_FBOS,fbTex);
        if(erreur_openGl("Erreur lors de l'allocation des identifiants de textures de rendus - "))return false;
        glGenFramebuffers(NUM_FBOS,fbos);
        if(erreur_openGl("Erreur lors de l'allocation des identifiants de tempons de rendus - "))return false;
        glGenRenderbuffers(NUM_FBOS,fbDepth);
        if(erreur_openGl("Erreur lors de l'allocation des identifiants de tempons de profondeur - "))return false;
        // Tempon de rendu principal:
        glBindRenderbuffer(GL_RENDERBUFFER,fbDepth[FBO_ECRAN]);
        glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT24,GVars::largeur_ecran,GVars::hauteur_ecran);
        if(erreur_openGl("Erreur lors de la création des tempons de profondeur - "))return false;

        glBindTexture(GL_TEXTURE_2D,fbTex[FBO_ECRAN]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
        glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,couleur_bords);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);    //Il semble que ce paramètre active la texture.....
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,GVars::largeur_ecran,GVars::hauteur_ecran,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,fbos[FBO_ECRAN]);
        glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,fbDepth[FBO_ECRAN]);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,fbTex[FBO_ECRAN],0);

        if(!verification_fbo())return false;

        // Tempon de rendu réduit
        glBindTexture(GL_TEXTURE_2D,fbTex[FBO_SMALL_1]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
        glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,couleur_bords);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);    //Il semble que ce paramètre active la texture.....
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB8,FBO_SMALL_L,FBO_SMALL_H,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,fbos[FBO_SMALL_1]);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,fbTex[FBO_SMALL_1],0);

        if(!verification_fbo())return false;

        // Tempon de rendu réduit
        glBindTexture(GL_TEXTURE_2D,fbTex[FBO_SMALL_2]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
        glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,couleur_bords);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);    //Il semble que ce paramètre active la texture.....
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB8,FBO_SMALL_L,FBO_SMALL_H,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,fbos[FBO_SMALL_2]);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,fbTex[FBO_SMALL_2],0);

        if(!verification_fbo())return false;

        // Tempon de rendu dimensions écran/2:

        glBindTexture(GL_TEXTURE_2D,fbTex[FBO_DEMI_ECRAN]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
        glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,couleur_bords);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);    //Il semble que ce paramètre active la texture.....
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB8,GVars::largeur_ecran/2,GVars::hauteur_ecran/2,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,fbos[FBO_DEMI_ECRAN]);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,fbTex[FBO_DEMI_ECRAN],0);

        if(!verification_fbo())return false;

        if(erreur_openGl("Erreur lors de l'initialisation des tempons de rendus - "))return false;

        // Tempon de sauvegarde écran:

        glBindTexture(GL_TEXTURE_2D,fbTex[FBO_SCREENSHOT]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_MIRRORED_REPEAT);
        glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,couleur_bords);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);    //Il semble que ce paramètre active la texture.....
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB8,GVars::largeur_ecran,GVars::hauteur_ecran,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,fbos[FBO_SCREENSHOT]);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,fbTex[FBO_SCREENSHOT],0);

        if(!verification_fbo())return false;

        if(erreur_openGl("Erreur lors de l'initialisation des tempons de rendus - "))return false;


        // Tempon de rendu visible écran:

        glBindTexture(GL_TEXTURE_2D,fbTex[FBO_ECRAN_2]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_MIRRORED_REPEAT);
        glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,couleur_bords);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);    //Il semble que ce paramètre active la texture.....
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB8,GVars::largeur_ecran,GVars::hauteur_ecran,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,fbos[FBO_ECRAN_2]);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,fbTex[FBO_ECRAN_2],0);

        if(!verification_fbo())return false;

        if(erreur_openGl("Erreur lors de l'initialisation des tempons de rendus - "))return false;




        //Restitution des tempons par défaut:
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
        glBindRenderbuffer(GL_RENDERBUFFER,0);
        glBindTexture(GL_TEXTURE_2D,0);

            return true;

    }

    bool FrameBuffers::redimensionne_frameBuffers()
    {
        glBindTexture(GL_TEXTURE_2D,fbTex[FBO_ECRAN]);
        glBindRenderbuffer(GL_RENDERBUFFER,fbDepth[FBO_ECRAN]);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,GVars::largeur_ecran,GVars::hauteur_ecran,0,GL_RGB,GL_UNSIGNED_BYTE,NULL); //Alloue la mémoire automatiquement.
        if(erreur_openGl("Erreur lors du redimensionnement des tempons de rendus - "))return false;
        glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT24,GVars::largeur_ecran,GVars::hauteur_ecran);
        if(erreur_openGl("Erreur lors du redimensionnement des tempons de profondeur - "))return false;

        glBindTexture(GL_TEXTURE_2D,fbTex[FBO_SCREENSHOT]);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB8,GVars::largeur_ecran,GVars::hauteur_ecran,0,GL_RGB,GL_UNSIGNED_BYTE,NULL); //Alloue la mémoire automatiquement.
        if(erreur_openGl("Erreur lors du redimensionnement des tempons de rendus - "))return false;

        glBindTexture(GL_TEXTURE_2D,fbTex[FBO_ECRAN_2]);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB8,GVars::largeur_ecran,GVars::hauteur_ecran,0,GL_RGB,GL_UNSIGNED_BYTE,NULL); //Alloue la mémoire automatiquement.
        if(erreur_openGl("Erreur lors du redimensionnement des tempons de rendus - "))return false;

        glBindTexture(GL_TEXTURE_2D,fbTex[FBO_DEMI_ECRAN]);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB8,GVars::largeur_ecran/2,GVars::hauteur_ecran/2,0,GL_RGB,GL_UNSIGNED_BYTE,NULL); //Alloue la mémoire automatiquement.
        if(erreur_openGl("Erreur lors du redimensionnement des tempons de rendus - "))return false;
        return true;
    }

    void FrameBuffers::screenShot()
    {
        GFunc::sauve_OpenGl();
        GFunc::vue_ortho();
        glViewport(0,0,GVars::largeur_ecran,GVars::hauteur_ecran);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,fbos[FBO_SCREENSHOT]);
        glDisable(GL_BLEND);
        glUseProgram(0);
        GFunc::desactive_textures();

        //glClearColor(1.,0.,0.,1.);
        //glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
        //return;

        glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,fbTex[FBO_ECRAN]);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        glEnable(GL_COLOR_MATERIAL);
        glDisable(GL_CULL_FACE);
        glColorMaterial(GL_FRONT_AND_BACK,GL_DIFFUSE);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glColor3f(1.,1.,1.);
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

        GFunc::restitue_OpenGl();
    }

    void FrameBuffers::affiche_screenShot()
    {
        GFunc::sauve_OpenGl();
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glEnable(GL_COLOR_MATERIAL);
            glColorMaterial(GL_FRONT_AND_BACK,GL_DIFFUSE);
            GFunc::vue_ortho();
            glViewport(0,0,GVars::largeur_ecran,GVars::hauteur_ecran);
            //glBindFramebuffer(GL_DRAW_FRAMEBUFFER,FrameBuffers::fbos[FBO_ECRAN]);
            GFunc::desactive_textures();
            glActiveTexture(GL_TEXTURE0);
            glEnable(GL_TEXTURE_2D);
            glEnable(GL_BLEND);
            glDisable(GL_LIGHTING);
            glDisable(GL_CULL_FACE);
            glDisable(GL_DEPTH_TEST);
            glUseProgram(0);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
            glBindTexture(GL_TEXTURE_2D,fbTex[FBO_SCREENSHOT]);
            GFunc::affiche_texture(GVars::alpha_screenShot);
        GFunc::restitue_OpenGl();
    }



