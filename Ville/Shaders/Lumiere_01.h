#ifndef LUMIERE_01_H_INCLUDED
#define LUMIERE_01_H_INCLUDED

#include "../../Shader.h"


class Lumiere_01: public Shader
{
  public:

    Lumiere_01(const char* p_nom,char* p_source_vertex,char* p_source_fragment);
    ~Lumiere_01();
};


#endif // LUMIERE_01_H_INCLUDED
