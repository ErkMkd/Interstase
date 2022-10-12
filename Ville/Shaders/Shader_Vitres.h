#ifndef SHADER_VITRES_H_INCLUDED
#define SHADER_VITRES_H_INCLUDED

#include "../../Shader.h"


class Shader_Vitres: public Shader
{
  public:

    Shader_Vitres(const char* p_nom,char* p_source_vertex,char* p_source_fragment);
    ~Shader_Vitres();
};

#endif // SHADER_VITRES_H_INCLUDED
