#ifndef SAUVE_RENDU_H_INCLUDED
#define SAUVE_RENDU_H_INCLUDED

/*-----------------------------------------------------------
                Sauve l'écran au format png
------------------------------------------------------------*/

#include "glew.h"
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

#include <libpng12/png.h>
#include "Fonctions.h"
#include "Variables.h"
#include "FrameBuffers.h"

using namespace std;

class Sauve_Rendu
{
    public:
    static uint8_t *pixels;
    static GLsizei largeur;
    static GLsizei hauteur;
    static bool alloue_pixels(GLsizei p_largeur, GLsizei p_hauteur);
    static void copy_texture_en_memoire(int source_fbo,int destination_fbo);
    static bool sauve_fichier(const char *filename,int source_fbo,int destination_fbo);
};

#endif // SAUVE_RENDU_H_INCLUDED
