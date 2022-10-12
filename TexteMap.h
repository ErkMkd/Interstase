#ifndef TEXTEMAP_H_INCLUDED
#define TEXTEMAP_H_INCLUDED


#include "chargeurs_images/Image.h"
#include "gestion_fichiers.h"

#define TEXTEMAP_OK 0
#define TEXTEMAP_ERREUR_IMAGE 1
#define TEXTEMAP_ERREUR_MEMOIRE 2
#define TEXTEMAP_ERREUR_FICHIER 3

//=================================================================================
//                      Affichage de texte via une texture
//      L'image des police doit être un carré d'une puissance de 2 de cotés.
//      Les lettres tiennent dans des carré de 1/16 de côté, et sont rangées
//      dans l'ordre des codes ASCII, de gauche à droite et de bas en haut.
//
//      Les quadrilatères des lettres sont définis dans le sens direct.
//=================================================================================

#define TEXTEMAP_RENDU_2D 1
#define TEXTEMAP_RENDU_3D 2

class TexteMap
{
    public:
        uint16_t erreur;
        float curseur_x;
        float curseur_y;
        uint8_t mode_rendu;
        uint32_t couleur;
        float alpha;
        float couleur_fond[3];

        GLint type_grossissement; //GL_LINEAR ou GL_NEAREST
        GLint type_diminution;

        //Gestion du scroll-text:

        float scroll_ymin;  //Les limites de l'écran sont situées entre 0 et 1
        float scroll_ymax;
        float vitesse_verticale;    //Le temps que met une ligne à traverser l'écran
        bool centrage;              //Le texte est centré sur la position du curseur
        //Données utiles pour le scroll-texte vertical:
    private:
        uint16_t pointeur_ligne;    //Numéro de la première ligne à afficher
        uint16_t nbr_lignes_max;    //Le nombre de lignes maximum à afficher
        float t_prec;



    private:
        char* texte;
        Image* police;
        uint32_t largeur_caractere; //Largeur en pixels des carrés contenants les caractères.
        float base;
        float coordonnees_lettres[256*4*2];
        float normale[3];
        float espaces_lettres[256];
        float rectangle[4*3];
        float echelle;  //Taille des caractère par rapport à la largeur de l'écran=1
        float espace_lettres;
        float espace_lignes;
        float espace;

        uint32_t taille_texte;  //Nombre de caractères
        uint16_t nbr_lignes;    //Le nombre de lignes du texte (chaque ligne est séparée par "\n", automatiquement transcrite en code 10 lors de la compilation)
        uint32_t* indices_lignes;    //La tableau contenant les indices de départ des lignes dans le texte
        uint16_t* tailles_lignes;    //La tableau contenant les tailles des lignes

        uint32_t largeur_ecran;
        uint32_t hauteur_ecran;

        GLboolean eclairages_mem;
        GLboolean blend_mem;
        GLboolean mask_mem;
        GLboolean color_material_mem;
        GLint blendSrc;
        GLint blendDst;
        GLboolean depthTest_mem;
        GLboolean culling_mem;
        GLint frontFace_mem;

    public:
        TexteMap(const char* fichier_image,uint32_t p_largeur_ecran,uint32_t p_hauteur_ecran,float p_base=0.,float p_echelle=1.,float p_espace_lettres=0.15,float p_espace=0.5,float p_espace_lignes=0.15);
        ~TexteMap();
        void initialise_coordonnees_lettres();
        void initialise_espaces();
        void initialise_scrollText_vertical(float p_ymin,float p_ymax,float p_vitesse);
        void determine_vitesse_scroll_vertical(float v);
        bool determine_texte_fichier(char* p_nom_fichier);
        bool determine_texte(char* p_texte);
        void determine_resolution_ecran(uint32_t pl,uint32_t ph);
        void determine_position_curseur(float px,float py);
        void determine_espace_lettres(float p_espace);
        void determine_espace_lignes(float p_espace);
        void determine_espace(float p_espace);
        void determine_echelle(float e);
        void affiche();
        void maj_scrolling_vertical(float t);
        uint32_t renvoie_largeur_caracteres();
        void affiche_chaine_2d(char* chaine);
    private:
        void begin_texte();
        void begin_texte_2d();
        void end_texte();
        void end_texte_2d();
        void desactive_textures();
        bool initialise_texte();
        void affiche_2d();
        void affiche_3d();
        void affiche_texte(char* texte);
};

#endif // TEXTEMAP_H_INCLUDED
