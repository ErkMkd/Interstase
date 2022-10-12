#ifndef SHADER_FENETRE_02_H_INCLUDED
#define SHADER_FENETRE_02_H_INCLUDED

#include "../../Shader.h"


class Shader_Fenetre_02: public Shader
{
  public:
    GLint partSize;
    GLint hauteur_etage;
    GLint nombre_etages;

    Shader_Fenetre_02(const char* p_nom,char* p_source_vertex,char* p_source_fragment);
    ~Shader_Fenetre_02();
};

#endif // SHADER_FENETRE_02_H_INCLUDED
