#ifndef GESTIONSHADERS_H_INCLUDED
#define GESTIONSHADERS_H_INCLUDED

#include "glew.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

#define TAILLEMSG_GLSL 10000

class GestionShaders
{
    public:

        static bool erreur_openGl(const char* message);
        static char* charge_source_shader(const char* nom_fichier);
        static bool compile_shader(GLuint shader_id);
        static bool relie_programme_shader(GLuint programme_id);
        static bool valide_programme_shader(GLuint programme_id);
};
#endif // GESTIONSHADERS_H_INCLUDED
