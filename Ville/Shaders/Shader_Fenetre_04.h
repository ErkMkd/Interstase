#ifndef SHADER_FENETRE_04_H_INCLUDED
#define SHADER_FENETRE_04_H_INCLUDED

#include "../../Shader.h"


class Shader_Fenetre_04: public Shader
{
  public:
    GLint partSize;
    GLint hauteur_etage;
    GLint nombre_etages;
    GLint tangente;

    Shader_Fenetre_04(const char* p_nom,char* p_source_vertex,char* p_source_fragment);
    ~Shader_Fenetre_04();
};


#endif // SHADER_FENETRE_04_H_INCLUDED
