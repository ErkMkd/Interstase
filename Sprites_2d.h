#ifndef SPRITES_2D_H_INCLUDED
#define SPRITES_2D_H_INCLUDED

/*
        Les sprites 2d sont affichés par transferts de pixels. On peut donc effectuer des
        traitements graphiques au pixel dans ces tempons.
*/

#include "glew.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <string>

#include "chargeurs_images/Image.h"

#define SPR2D_OK 0
#define SPR2D_ERREUR_MEMOIRE 1
#define SPR2D_ERREUR_IMAGE 2
#define SPR2D_ERREUR_TEXTURE 3

#define SPR2D_TYPE_MULTIFICHIERS 0
#define SPR2D_TYPE_TILE 1

typedef struct
{
    Image* image;
    float x;    //Position de l'image à l'écran
    float y;
    float e;    //échelle générale (echelle_finale_X=e*sx, echelle_finale_Y=e*sy)
    float sx;   //echelle X
    float sy;   //echelle Y
    float r;    //Angle de rotation (en degrés)
    float alpha;
    float cx;   //Position du centre du sprite dans la texture. Il faut que drapeau_centre soit à false pour utiliser cette option.
    float cy;
    uint32_t resoRef_x;    //La résolution de référence. Ex: Si le bitmap a été prévu pour un écran de 1280*720, et que l'écran de rendu final
    uint32_t resoRef_y;    //n'est pas dans la même résolution, le sprite est mis à l'échelle.
    bool drapeau_stencil;   //Si "true", le sprite est un cache à afficher dans le tempon "stencil" d'OpenGl
    bool drapeau_affiche;
    bool drapeau_centre;    //true si le point 0 du sprite est au centre de la texture.
    bool drapeau_proportionnel; //true si les coordonnées du sprite sont proportionnelles à l'écran (0,0 : coin inférieur gauche; 1,1 : coin supérieur droit)
    //Infos pour le type Tile:
    float texCoords[4*2];   //Coordonnées sur la texture
} sprite_2d;

class Sprites_2d
{
    private:
        uint32_t nbrImages;
        vector<sprite_2d> sprites;
        static uint32_t resolution_x;
        static uint32_t resolution_y;
        uint8_t type;
        Image* image_elements;  //Sert pour le type Tile
        uint16_t largeur_element;
        uint16_t hauteur_element;

    private:
        void affiche_multi();
        void affiche_tile();


    public:
        uint16_t erreur;

        //Multi fichiers
        Sprites_2d (uint32_t p_nbrImages,const char** noms_fichiers);
        //Tile:
        Sprites_2d (uint16_t p_largeur_element, uint16_t p_hauteur_element, const char* nom_fichier);
        ~Sprites_2d();
        void cache();
        void montre();
        void montre_sprite(uint32_t indice);
        void cache_sprite(uint32_t indice);
        void determine_position_sprite(uint32_t indice, float x, float y);
        void determine_position_centre_sprite(uint32_t indice, float cx, float cy);
        float renvoie_sprite_x(uint32_t indice);
        float renvoie_sprite_y(uint32_t indice);
        void determine_alpha_sprite(uint32_t indice, float a);
        void determine_rotation_sprite(uint32_t indice, float r);
        void determine_centre_sprite(uint32_t indice, bool c);
        void determine_proportionnel_sprite(uint32_t indice, bool p);
        static void determine_resolution(uint32_t r_l, uint32_t r_h);
        void determine_echelle_sprite(uint32_t indice, float echelle);
        void determine_echelle_XY_sprite(uint32_t indice, float sXY[2]);
        void determine_echelle_X_sprite(uint32_t indice, float p_sx);
        void determine_echelle_Y_sprite(uint32_t indice, float p_sy);
        void determine_resolution_reference_XY_sprite(uint32_t indice,uint32_t resRef_x,uint32_t resRef_y);
        void determine_resolution_reference_XY(uint32_t resRef_x,uint32_t resRef_y);
        float renvoie_echelle_X_sprite(uint32_t indice);
        float renvoie_echelle_Y_sprite(uint32_t indice);
        float renvoie_echelle_sprite(uint32_t indice);
        float renvoie_alpha_sprite(uint32_t indice);
        GLint renvoie_texture_id_sprite(uint32_t indice);
        void determine_stencil(uint32_t indice,bool p_drapeau);
        uint32_t renvoie_largeur_sprite(uint32_t indice);
        uint32_t renvoie_hauteur_sprite(uint32_t indice);
        void affiche_sprites_2d();
};


#endif // SPRITES_2D_H_INCLUDED
