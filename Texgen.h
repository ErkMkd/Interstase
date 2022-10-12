#ifndef TEXGEN_H_INCLUDED
#define TEXGEN_H_INCLUDED

#include "glew.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "chargeurs_images/Image.h"
#include "vec3f.h"
#include "ShadersObjets3d/ShadersObjets3d.h"
#include "ShadersObjets3d/Shader_relief_genCones.h"
//#include "Variables.h"

#define TEXGEN_OK 0
#define TEXGEN_ERREUR_TEXTURE 1
#define TEXGEN_ERREUR_FBO 2
#define TEXGEN_ERREUR_FBO_DEPTH 3
#define TEXGEN_ERREUR_MEMOIRE 4

class Texgen
{
    private:
        static bool erreur_openGl(const char* message);
        static void desactive_textures();
        void init_dataSize();
        bool alloue_bitmap();
        void texCRpart(GLint x, GLint y,GLsizei l, GLsizei h,GLFWwindow* ecran);

    public:

        uint16_t erreur;
        GLuint fbo_id;
        GLuint fbo_depth_id;
        GLuint texture_id;

        GLsizei largeur;
        GLsizei hauteur;
        GLenum format;
        GLenum type;
        int octets_par_composante;
        int octets_par_pixel;
        uint8_t* bitmap; //Alloué seulement si la texture est générée via le CPU

        GLfloat couleur_bords[4];


        Texgen(GLsizei p_largeur, GLsizei p_hauteur, GLint p_format, GLint p_type,GLint recouvrement_s=GL_REPEAT,GLint recouvrement_t=GL_REPEAT);
        ~Texgen();
        bool genere_framebuffer(bool drapeau_depthBuffer=false);  //Si il y a besoin de faire un rendu dans la texture

        void maj_texture(); //Mise à jour du bitmap dans la mémoire graphique (ex: lorsque l'on a écrit des pixels avec le CPU)
        void efface_texture();
        void calcule_conesRelief_CPU(Image* texture_profondeur,float echelle_profondeur);
        void calcule_conesRelief_GPU(Image* texture_profondeur,float echelle_profondeur,GLFWwindow* ecran);
};

#endif // TEXGEN_H_INCLUDED
