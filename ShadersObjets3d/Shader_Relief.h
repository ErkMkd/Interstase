#ifndef SHADER_RELIEF_H_INCLUDED
#define SHADER_RELIEF_H_INCLUDED

#include "../Shader.h"

/// Pour un relief cohérent,la texture ne doit pas être déformée

class Shader_Relief: public Shader
{
    public:
        GLint texture_couleur;
        GLint texture_normales;
        GLint texture_profondeurs; /// !!! Contient aussi le rayons des cônes à l'altitude 0 - profondeur,rayon(float,float)
        GLint echelle_texture; //echelle des coordonnées UV par rapport à l'espace (ex si echelle=1, 1U=1X=1Y=1Z=1V)
        GLint tangente;
        GLint binormale;
        GLint relief_on;

        Shader_Relief(const char* p_nom,char* p_source_vertex,char* p_source_fragment);
        ~Shader_Relief();
};

#endif // SHADER_RELIEF_H_INCLUDED
