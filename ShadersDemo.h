#ifndef SHADERSDEMO_H_INCLUDED
#define SHADERSDEMO_H_INCLUDED

#include "GestionShaders.h"
#include "Variables.h"

class ShadersDemo :public GestionShaders
{
    public:
        //---------------- Alpha en fonction de la distance à un point:

        static GLchar* source_vertex_alphaDist;
        static GLchar* source_fragment_alphaDist;
        static GLuint shader_alphaDist_id1;
        static GLuint shader_alphaDist_id2;
        static GLuint programme_alphaDist;

        static GLint uniform_alphaDist_texture;
        static GLint uniform_alphaDist_point;
        static GLint uniform_alphaDist_distMin;
        static GLint uniform_alphaDist_distMax;

        //---------------- Floutage:
        static GLchar* source_fragment_flou_x;
        static GLchar* source_fragment_flou_y;
        static GLuint shader_floux_id;
        static GLuint shader_flouy_id;
        static GLuint programme_floux_id;
        static GLuint programme_flouy_id;

        static GLint uniform_floux_taille;
        static GLint uniform_flouy_taille;
        static GLint uniform_floux_saturation;
        static GLint uniform_flouy_saturation;
        static GLint uniform_floux_texture;
        static GLint uniform_flouy_texture;

        //---------------- Filtres:
        static GLchar* source_fragment_filtres;
        static GLuint shader_filtres_id;
        static GLuint programme_filtres_id;

        static GLuint uniform_filtres_texture;
        static GLuint uniform_filtres_saturation;
        static GLuint uniform_filtres_contraste;
        static GLuint uniform_filtres_seuil;

        //------------------ Filtre Hue, Saturation, Value:
        static GLchar* source_fragment_filtre_HSV;
        static GLuint shader_filtre_HSV_id;
        static GLuint programme_filtre_HSV_id;

        static GLuint uniform_filtre_HSV_texture;
        static GLuint uniform_filtre_HSV_Hue;
        static GLuint uniform_filtre_HSV_Saturation;
        static GLuint uniform_filtre_HSV_Value;

        //======== Shader de la fractale de Mandelbrot:
        static GLchar* source_vertex_mandelbrot;
        static GLchar* source_fragment_mandelbrot;
        static GLuint shader_mandelbrot_id1;
        static GLuint shader_mandelbrot_id2;
        static GLuint programme_mandelbrot;

        static GLint uniform_mandelbrot_ecran_ratio;
        static GLint uniform_mandelbrot_ajustement_resolution;
        static GLint uniform_mandelbrot_zoom;
        static GLint uniform_mandelbrot_xpos;
        static GLint uniform_mandelbrot_ypos;
        static GLint uniform_mandelbrot_alpha;
        static GLint uniform_mandelbrot_t;
        static GLint uniform_mandelbrot_iter;
        static GLint uniform_mandelbrot_ecranDim;
        static GLint uniform_mandelbrot_palette;
        static GLint uniform_mandelbrot_arriere_plan;
        static GLint uniform_mandelbrot_decallage_palette;


        //======== Shader du raytracer waterworld:

        static GLchar* source_vertex_waterworld;
        static GLchar* source_fragment_waterworld;
        static GLuint shader_waterworld_id1;
        static GLuint shader_waterworld_id2;
        static GLuint programme_waterworld;

        static GLint uniform_waterworld_random_numer;
        static GLint uniform_waterworld_temps;
        static GLint uniform_waterworld_numReflec;
        static GLint uniform_waterworld_numRefrac;
        static GLint uniform_waterworld_Obs;
        static GLint uniform_waterworld_ecranDim;
        static GLint uniform_waterworld_ajustement_resolution;
        static GLint uniform_waterworld_distanceFocale;
        static GLint uniform_waterworld_ecran_ratio;
        static GLint uniform_waterworld_distanceBrouillard;
        static GLint uniform_waterworld_amplitude_vagues;

        static GLint uniform_waterworld_C_ciel;
        static GLint uniform_waterworld_C_amb;
        static GLint uniform_waterworld_L_amb;

        static GLint uniform_waterworld_L1;

        static GLint uniform_waterworld_objets;

        //======== Shader du raytracer spheredance:

        static GLchar* source_vertex_spheredance;
        static GLchar* source_fragment_spheredance;
        static GLuint shader_spheredance_id1;
        static GLuint shader_spheredance_id2;
        static GLuint programme_spheredance;

        static GLint uniform_spheredance_random_numer;
        static GLint uniform_spheredance_temps;
        static GLint uniform_spheredance_numReflec;
        static GLint uniform_spheredance_Obs;
        static GLint uniform_spheredance_ecranDim;
        static GLint uniform_spheredance_ajustement_resolution;
        static GLint uniform_spheredance_distanceFocale;
        static GLint uniform_spheredance_ecran_ratio;

        static GLint uniform_spheredance_C_ciel;
        static GLint uniform_spheredance_C_amb;
        static GLint uniform_spheredance_L_amb;

        static GLint uniform_spheredance_L1;

        static GLint uniform_spheredance_objets;

        static GLint uniform_spheredance_distanceBrouillard;

        //======== Shader du raytracer hypnostructure:

        static GLchar* source_vertex_hypnostructure;
        static GLchar* source_fragment_hypnostructure;
        static GLuint shader_hypnostructure_id1;
        static GLuint shader_hypnostructure_id2;
        static GLuint programme_hypnostructure;

        static GLint uniform_hypnostructure_random_numer;
        static GLint uniform_hypnostructure_temps;
        static GLint uniform_hypnostructure_numReflec;
        static GLint uniform_hypnostructure_numRefrac;
        static GLint uniform_hypnostructure_Obs;
        static GLint uniform_hypnostructure_ecranDim;
        static GLint uniform_hypnostructure_distanceFocale;
        static GLint uniform_hypnostructure_ecran_ratio;
        static GLint uniform_hypnostructure_distanceBrouillard;

        static GLint uniform_hypnostructure_C_ciel;
        static GLint uniform_hypnostructure_C_amb;
        static GLint uniform_hypnostructure_L_amb;

        static GLint uniform_hypnostructure_L1;

        static GLint uniform_hypnostructure_objets;


    public:
        static bool init_shader_alphaDist();
        static bool init_shader_mandelbrot();
        static bool init_shader_waterworld();
        static bool init_shader_spheredance();
        static bool init_shader_hypnostructure();
        static bool init_flou();
        static bool init_filtres();
        static bool init_filtre_HSV();
        static bool initShaders();
        static void freeShaders();

        static void programme_filtres(GLfloat saturation,GLfloat contraste,GLfloat seuil, GLuint texture=0);
        static void programme_floux(GLuint taille_hallo,GLfloat saturation_hallo,GLuint texture=0);
        static void programme_flouy(GLuint taille_hallo,GLfloat saturation_hallo,GLuint texture=0);
        static void programme_filtre_HSV(GLfloat hue, GLfloat saturation, GLfloat value, GLuint texture=0);
};

#endif // SHADERSDEMO_H_INCLUDED
