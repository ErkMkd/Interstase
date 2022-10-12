#ifndef FRAMEBUFFERS_H_INCLUDED
#define FRAMEBUFFERS_H_INCLUDED

#include "glew.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "Variables.h"

//static bool erreur_openGl(char* message);

#define NUM_FBOS 6
#define FBO_ECRAN 0
#define FBO_DEMI_ECRAN 1
#define FBO_SMALL_1 2
#define FBO_SMALL_2 3
#define FBO_SCREENSHOT 4    //Sert à sauvegarder un rendu, pour les transitions
#define FBO_ECRAN_2 5 //Pour l'image finale

#define FBO_SMALL_L 640 //RESOLUTION_RENDU_DEMO_X/2
#define FBO_SMALL_H 360 //RESOLUTION_RENDU_DEMO_Y/2


//Frame buffer objets - Tempons de rendus

class FrameBuffers
{
    public:
        static GLuint 	fbos[NUM_FBOS];
        static GLuint	fbDepth[NUM_FBOS];
        static GLuint	fbTex[NUM_FBOS];

        static bool erreur_openGl(const char* message);
        static void supprime_frameBuffers();
        static bool verification_fbo();
        static bool initialise_frameBuffers();
        static bool redimensionne_frameBuffers();
        static void affiche_screenShot();
        static void screenShot();

};


#endif // FRAMEBUFFERS_H_INCLUDED
