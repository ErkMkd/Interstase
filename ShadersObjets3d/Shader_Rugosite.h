#ifndef SHADER_RUGOSITE_H_INCLUDED
#define SHADER_RUGOSITE_H_INCLUDED

#include "../Shader.h"


class Shader_Rugosite: public Shader
{
  public:
        GLint texture_rugosite;
        GLint tangente;
        GLint binormale;


        Shader_Rugosite(const char* p_nom,char* p_source_vertex,char* p_source_fragment);
        ~Shader_Rugosite();
};

#endif // SHADER_RUGOSITE_H_INCLUDED
