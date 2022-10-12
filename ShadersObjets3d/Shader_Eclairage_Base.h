#ifndef SHADER_ECLAIRAGE_BASE_H_INCLUDED
#define SHADER_ECLAIRAGE_BASE_H_INCLUDED

#include "../Shader.h"


class Shader_Eclairage_Base: public Shader
{
  public:
    GLint drapeaux_sources;

    Shader_Eclairage_Base(const char* p_nom,char* p_source_vertex,char* p_source_fragment);
    ~Shader_Eclairage_Base();
};

#endif // SHADER_ECLAIRAGE_BASE_H_INCLUDED
