#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#include "../glew.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <math.h>

#include "Image_infos.h"
#include "Image_TIFF.h"
#include "Image_BMP.h"
#include "Image_JPEG.h"

#define IMAGE_DEBUG true


//========= Erreurs:
#define IMAGE_OK 0
#define IMAGE_ERREUR_FICHIER 1
#define IMAGE_ERREUR_TEXTURE 2
#define IMAGE_ERREUR_BUFFER 3
#define IMAGE_ERREUR_FORMAT 4
#define IMAGE_ERREUR_LECTURE 5
#define IMAGE_ERREUR_MEMOIRE 6
#define IMAGE_ERREUR_GLU 7

#define IMAGEID2_(a,b) (((a)<<8)|(b))
#define IMAGEFORMAT_BMP   IMAGEID2_('B','M')    //Format BMP
#define IMAGEFORMAT_MM    IMAGEID2_('M','M')    //Les octets des images TIFF se présentent dans l'ordre
#define IMAGEFORMAT_II    IMAGEID2_('I','I')    //Les octets des images TIFF nécessitent un retournement.
#define IMAGEFORMAT_PNG   0x89504e0d0a1a0a      //Les fichier PNG commencent toujours par cette série
#define IMAGEFORMAT_JPEG_SOI  0xffd8      //Les fichier JPEG commencent toujours par ce marqueur

//============== Facteurs pour les convertions de formats:

//Lors de la convertion en niveaux de gris, les composantes n'étants pas perçues avec la même intensité, un facteur leur est attribué.
#define IMAGE_FACTEUR_GRIS_R 0.11
#define IMAGE_FACTEUR_GRIS_V 0.59
#define IMAGE_FACTEUR_GRIS_B 0.3

using namespace std;

//*************************************************************
// Format de base d'une image
//*************************************************************

class Image: public Fichiers
{
    public:
        GLuint texture_id; //L'identitifant texture utilisé par openGl - Généré par genere_texture_openGL()
        GLuint buffer_id;   //L'identifiant du tempon de pixels - Généré par genere_buffer_openGl()
        GLenum  format_pixels; //GL_RGB ou GL_RGBA
        GLenum  type_pixels;    //GL_BYTE,GL_UNSIGNED_INT, etc...

        uint32_t largeur;
        uint32_t hauteur;
        uint16_t erreur;
        const char* nom_image;  //Le nom du fichier d'origine
        uint8_t* image;    //Les données décompressées
        uint16_t bits_par_pixel;
        uint16_t octets_par_pixel;
        uint32_t nbr_couleurs;
        uint32_t palette[256];  //Dans le cas d'une image aux couleurs indexées.

    private:
        ifstream    fichier_flux;
        uint32_t    taille_fichier;

    protected:
        static bool erreur_openGl(const char* message);
        void    initialise_format_OpenGl();

public:
    Image(const char* p_nom_image);
    Image(Image& image_originale);
    ~Image();
    bool genere_texture_openGL (bool drapeau_mipmap=false,
                                GLint recouvrement_s=GL_CLAMP,GLint recouvrement_t=GL_CLAMP,
                                GLint type_grossisement=GL_LINEAR, GLint type_diminution=GL_LINEAR);
    bool genere_buffer_openGl();
    void renversement_horizontal();
    void renversement_vertical();
    bool redimensionne(int32_t nl, int32_t nh);
    bool format_pixels_RGBA(uint8_t valeur_alpha=0xff);
    bool format_pixels_RVB();
    bool format_pixels_ALPHA();
    bool format_pixels_LUMINANCE();
    bool format_pixels_LUMINANCE_ALPHA();
    bool format_pixels_INTENSITY();
    void determine_contraste(float contraste);
    void determine_luminosite(float luminosite);
    void determine_saturation(float saturation);
    void niveaux_gris();
    void determine_decallage_teinte(float angle);
    void determine_gamma(float gamma);
    void negatif();
    bool contours();
    bool convertion_texture_normales(float amplitude);
    void calcul_normale_pente(float x,float y, float z, float* resultat);
    uint32_t renvoie_texel(float u, float v);

private:
    bool test_format();
    bool charge_BMP();
    bool charge_PNG();
    bool charge_TIFF();
    bool charge_JPEG();
    void renversement_horizontal_1();
    void renversement_horizontal_2();
    void renversement_horizontal_3();
    void renversement_horizontal_4();
    void renversement_vertical_1();
    void renversement_vertical_2();
    void renversement_vertical_3();
    void renversement_vertical_4();
    void convertion_RVBA_gris(uint8_t* nouveau_tempon, uint8_t valeur_alpha);
    void convertion_RVB_RVBA(uint8_t* nouveau_tempon, uint8_t valeur_alpha);
    void convertion_8_RVB(uint8_t* nouveau_tempon);
    void convertion_RVBA_RVB(uint8_t* nouveau_tempon);
    bool convertion_8bits();
    void convertion_RVB_8(uint8_t* nouveau_tempon);
    void convertion_RVBA_8(uint8_t* nouveau_tempon);

};

#endif // IMAGE_H_INCLUDED
