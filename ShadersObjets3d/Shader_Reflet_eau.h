#ifndef SHADER_REFLET_EAU_H_INCLUDED
#define SHADER_REFLET_EAU_H_INCLUDED

#include "../Shader.h"


class Shader_Reflet_eau: public Shader
{
  public:
        GLint position_obs;
        GLint position_source;
        GLint texture_reflet;
        GLint largeur_ecran;
        GLint hauteur_ecran;
        GLint t;

        GLint texture_bruit;
        GLint vitesse_ondes;
        GLint echelle_texture_bruit_ondes;
        GLint quantite_ondes;
        GLint frequence_ondes;
        GLint amplitude_ondes;
        GLint niveau_perturbations_ondes;
        GLint intensite_ondes;
        GLint couleur_fond;
        GLint transparence;
        GLint attenuation_speculaire;

        Shader_Reflet_eau(const char* p_nom,char* p_source_vertex,char* p_source_fragment);
        ~Shader_Reflet_eau();
};


#endif // SHADER_REFLET_EAU_H_INCLUDED
