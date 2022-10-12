#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED


#include "GestionShaders.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <string.h>

#define SHADER_DEBUG true

//------- Codes d'erreur:
#define SHADER_OK 0
#define SHADER_ERREUR_SOURCE 1
#define SHADER_ERREUR_OBJETS 2
#define SHADER_ERREUR_COMPILATION 3
#define SHADER_ERREUR_PROGRAMME 4
#define SHADER_ERREUR_LIAISON 5
#define SHADER_ERREUR_VALIDATION 6
#define SHADER_ERREUR_OPENGL 7
#define SHADER_ERREUR_VARIABLES 8    //Sert pour les héritiers, lorsque les variables ne sont pas trouvées dans le programme.

class Shader : public GestionShaders
{
    public:
        GLuint programme_id;
        uint16_t erreur;
        char nom[256];

    protected:

        char *source_vertex;
        char *source_fragment;
        GLuint vertex_id;
        GLuint fragment_id;

        bool drapeau_vertex;
        bool drapeau_fragment;
        const GLchar* tableau_sources[1];

    public:
        Shader(const char* p_nom, char* p_source_vertex, char* p_source_fragment);
        ~Shader();

};


#endif // SHADER_H_INCLUDED
