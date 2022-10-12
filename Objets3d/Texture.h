#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include "../glew.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "math.h"

#include "../chargeurs_images/Image.h"

using namespace std;
//----------------------------------
// Classe de gestion des textures
//
//  Une même texture peut servir à plusieurs surfaces.
///  Le tableau des coordonnées UV en entrée est recopié.
///  Le programme appelant peut donc le supprimer après construction de la texture.
//
//
//----------------------------------

//Types de texture:
#define TEXTURE_CANAL_INVALIDE 0
#define TEXTURE_CANAL_COULEUR 1
#define TEXTURE_CANAL_AMBIANCE 2
#define TEXTURE_CANAL_DIFFUSION 3
#define TEXTURE_CANAL_LUMINESCENCE 4
#define TEXTURE_CANAL_TRANSPARENCE 5
#define TEXTURE_CANAL_REFLEXION 6
#define TEXTURE_CANAL_RUGOSITE 7

//Mode de projection:
#define TEXTURE_PROJECTION_UV 1
#define TEXTURE_PROJECTION_PLAN 2
#define TEXTURE_PROJECTION_ENVIRONNEMENT 3

//Erreurs:
#define TEXTURE_OK 0
#define TEXTURE_ERREUR_MEMOIRE 1

//La structure de définition des sommets UV, lors de la création du volume
class SommetUV
{
    public:
        uint32_t indice_sommet;
        float   U;
        float   V;
};

//------ Classe des textures:

class Texture
{
    public:
        static Image* image_transparente;   //Pour faire disparaitre les textures.

        char nom[256];
        uint16_t erreur;
        uint8_t canal;   //Couleur, diffusion, réflexion...
        uint8_t type_projection; //UV, plan ou réflection de l'environnement
        float luminescence; //Sert dans les cas de transition d'une texture à une autre.

        //Paramètres de rendu:
        GLint type_enveloppe;   //GL_MODULATE, GL_REPLACE, GL_DECAL, GL_BLEND, GL_ADD, GL_COMBINE
        GLint recouvrement_s;   //GL_CLAMP, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER, GL_REPEAT, GL_MIRRORED_REPEAT
        GLint recouvrement_t;
        GLint type_grossissement;    //GL_LINEAR, GL_NEAREST
        GLint type_diminution;

        //Paramètres pour les coordonnées générées (autres que UV)
        GLenum mode_projection; //GL_OBJECT_LINEAR, GL_EYE_LINEAR, GL_SPHERE_MAP, GL_REFLECTION_MAP ou GL_NORMAL_MAP
        GLenum reference_projection; //GL_OBJECT_PLANE ou GL_EYE_PLANE
        GLfloat axe_projection[4];

        //Paramètres pour les coordonnées UV:
        uint32_t nbrSommets;
        vector<SommetUV> coordonnees_UV;    //Coordonnées définies par l'utilisateur
        GLfloat* coordonnees_UV_tableau;    //Tableau initialisé par le constructeur du volume. !!! Non dupliqué par le constructeur de copie.

        Image* image;

        //Tables pour les textures de rugosité:
        //Ces tables sont créées par le volume.
        GLfloat* tangentes_faces;
        GLfloat* binormales_faces;
        GLfloat* tangentes_sommets;
        GLfloat* binormales_sommets;
        uint32_t* faces; //Les indexes des faces recouvertes par la texture.
        uint32_t nbrFaces;

        bool drapeau_mipmap;

    public:
        Texture(char* p_nom=NULL,
                   Image* p_image=NULL,
                    uint8_t p_canal=TEXTURE_CANAL_INVALIDE,
                    float* p_uvs=NULL, uint32_t* p_indices_sommets=NULL,
                    uint16_t p_nbrSommets=0,
                    bool p_mipmap=false,
                    GLint p_recouvrement_s=GL_CLAMP,
                    GLint p_recouvrement_t=GL_CLAMP,
                    GLint p_type_grossissement=GL_LINEAR,
                    GLint p_type_diminution=GL_LINEAR
                    );


        Texture(Texture& texture_originale);
        ~Texture();
        void determine_nom(char* p_nom);
        void determine_image(Image* p_image);
        void initialise_image();
        void determine_UV(float* uvs, uint32_t* indices, uint16_t p_nbrSommets);

    private:
        SommetUV* ajoute_sommet_UV();

};

#endif // TEXTURE_H_INCLUDED
