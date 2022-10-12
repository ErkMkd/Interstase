#ifndef SHADER_APLAT_BRUT_H_INCLUDED
#define SHADER_APLAT_BRUT_H_INCLUDED

#include "../Shader.h"

class Shader_Aplat_Brut: public Shader
{
  public:
    Shader_Aplat_Brut(const char* p_nom,char* p_source_vertex,char* p_source_fragment);
    ~Shader_Aplat_Brut();
};

#endif // SHADER_APLAT_BRUT_H_INCLUDED
