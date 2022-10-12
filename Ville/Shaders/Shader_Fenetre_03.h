#ifndef SHADER_FENETRE_03_H_INCLUDED
#define SHADER_FENETRE_03_H_INCLUDED

#include "../../Shader.h"


class Shader_Fenetre_03: public Shader
{
  public:
    GLint partSize;
    GLint hauteur_etage;
    GLint nombre_etages;

    Shader_Fenetre_03(const char* p_nom,char* p_source_vertex,char* p_source_fragment);
    ~Shader_Fenetre_03();
};


#endif // SHADER_FENETRE_03_H_INCLUDED
