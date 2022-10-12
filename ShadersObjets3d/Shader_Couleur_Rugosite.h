#ifndef SHADER_COULEUR_RUGOSITE_H_INCLUDED
#define SHADER_COULEUR_RUGOSITE_H_INCLUDED

#include "../Shader.h"


class Shader_Couleur_Rugosite: public Shader
{
  public:
        GLint texture_couleur;
        GLint texture_rugosite;
        GLint tangente;
        GLint binormale;

        Shader_Couleur_Rugosite(const char* p_nom,char* p_source_vertex,char* p_source_fragment);
        ~Shader_Couleur_Rugosite();
};


#endif // SHADER_COULEUR_RUGOSITE_H_INCLUDED
