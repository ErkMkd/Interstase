#ifndef CHAMP_ETOILES_H_INCLUDED
#define CHAMP_ETOILES_H_INCLUDED

#include "glew.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>


#include "chargeurs_images/Image.h"
#include "vec3f.h"

#define CHAMP_ETOILES_OK 0

//============= Structure des �toiles
typedef struct
{
    float x;
    float y;
    float z;
    vec3f vitesse;
    vec3f propulsion;
    float coefficient_frottement;
    float masse;
}
particule_etoile;

//====================================================================================
//                                  Champ d'�toiles
//====================================================================================

class Champ_Etoiles
{
    public:

        Image *image;       //L'image utilis�e pour les �toiles
        float profondeur; //Profondeur du champ d'�toiles
        float largeur;  //Dimensions du champ d'�toiles
        float hauteur;
        unsigned int nbr_etoiles; //Nombre d'�toiles dans le champ
        float largeur_face;  //Dimensions des polygones en unit�s d'espace
        float hauteur_face;
        float angle;    //Param�tres de la rotation en profondeur
        float vitesse_rotation;
        float alpha; //Transparence du champ
        float alpha_fondu; //Alpha pour les fondus, multipli� � l'alpha
        particule_etoile *etoiles; //Pointeur sur les structures de d�finition des �toiles
        int erreur;
        float t_prec;
        float facteur_vitesse;  //Coefficient de multiplication de la vitesse des �toiles

    public:

        Champ_Etoiles();
        void initialise(float p_profondeur,
                        float p_largeur,
                        float p_hauteur,
                        unsigned int p_nbr_etoiles,
                        Image *p_image,
                        float echelle,
                        float p_alpha,
                        float p_vitesse_rotation,
                        vec3f *p_vitesse,
                        float masse //Masse des �toiles
                        );
        ~Champ_Etoiles();
        bool determine_image(Image* p_image);
        void affiche_deplacement_axe_z(float pas,float delta_t, float angle_orientation);
        void affiche_deplacement_vecteur_direction(float t, float angle_orientation);
        void affiche_deplacement_centre_gravite(float delta_t, float angle_orientation, vec3f *centre_gravite);
        float determine_coefficient_frottement(float vlim,float valeur_propulsion);
        void rafraichi_vitesse(float delta_t, particule_etoile *particule);
        void rafraichi_position(float delta_t, particule_etoile *particule);
};

#endif // CHAMP_ETOILES_H_INCLUDED
