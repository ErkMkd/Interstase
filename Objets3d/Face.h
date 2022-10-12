#ifndef FACE_H_INCLUDED
#define FACE_H_INCLUDED

#include "../glew.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "math.h"

using namespace std;

#define FACE_OK 0
#define FACE_ERREUR_SURFACE 1 //Si aucune surface n'est d�finie pour la face.
#define FACE_ERREUR_MEMOIRE 2

#define FACE_LIBERE_RIEN 0x00
#define FACE_LIBERE_ARETES 0x01

//Structure utilis�e pour le tri des faces transparentes:
class Face;
class Surface;
class Volume;

class tri_face
{   public:
    float distance;        //Distance de la face � l'oeil
    Face* face;   //L'indice de la face
};

//===========================================================================================
//                                  LA CLASSE
//===========================================================================================

class Face
{
    public:
        //------ Donn�es fournies au volume � sa cr�ation:
        uint16_t erreur;
        uint16_t nbrSommets;
        uint32_t* aretes;   //Les aretes doivent boucler (dernier sommets li� au premier)

        //------- Donn�es initialis�es automatiquement:
        uint32_t    indice; //L'indice de la face dans le volume
        Volume*  volume; //Le volume auquel appartient la face
        char nom_surface[128];  //Le nom de la surface � laquelle est li�e la face. Ca sert dans le cas d'une modification des surfaces du volume, pour refaire les liens.
        Surface* surface;   //La surface � laquelle est affect�e la face. Ce champ doit �tre rempli par l'utilisateur. Une erreur est g�n�r�e si il est vide.

        GLfloat normale[3];
        GLfloat* normales_sommets; //La table des normales aux sommets de la face. Cr��e lors de l'initialisation des surfaces
        bool drapeau_partiellement_adoucie; //True si au moins une ar�te n'est pas adoucie (angle superieur � la limite de la surface)
        GLfloat tangente[3];
        GLfloat binormale[3];
        float centre_gravite[3]; //Le centre de gravit� de la face
        uint32_t* liens_aretes; //Indices des ar�tes de la face dans la table des ar�tes du volume.
        uint8_t drapeaux_liberation_memoire;

    public:
        Face(uint16_t p_nbrSommets, uint32_t* p_aretes, char* p_nom_surface,uint8_t p_drapeaux_liberation_memoire=FACE_LIBERE_ARETES);
        Face(Face& face_originale);
        ~Face();
        void affiche_centre_gravite();
        void inverse_normales();

    private:
        bool allocation_normales_sommets();
        bool allocation_aretes();
        bool copie_nom_surface(char* p_nom);

};



#endif // FACE_H_INCLUDED
