#ifndef SHADER_RELIEF_GENCONES_H_INCLUDED
#define SHADER_RELIEF_GENCONES_H_INCLUDED

#include "../Shader.h"

//Shader utilitaire, pour générer les cônes de distances des textures de reliefs
//Le framebuffer de sortie contient les cônes pour chaque texels au format profondeur,rayon(float,float)

class Shader_relief_genCones: public Shader
{
    public:
        GLint texture_profondeurs;
        GLint largeur_texture_sortie;
        GLint hauteur_texture_sortie;
        GLint echelle_profondeur;

        Shader_relief_genCones(const char* p_nom,char* p_source_vertex,char* p_source_fragment);
        ~Shader_relief_genCones();
};


#endif // SHADER_RELIEF_GENCONES_H_INCLUDED
