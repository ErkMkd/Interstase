#ifndef SHADER_FACE_OBSERVATEUR_H_INCLUDED
#define SHADER_FACE_OBSERVATEUR_H_INCLUDED

#include "../../Shader.h"


class Shader_Face_Observateur: public Shader
{
  public:

    Shader_Face_Observateur(const char* p_nom,char* p_source_vertex,char* p_source_fragment);
    ~Shader_Face_Observateur();
};


#endif // SHADER_FACE_OBSERVATEUR_H_INCLUDED
