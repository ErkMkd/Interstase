#ifndef CHARGELWO2_H_INCLUDED
#define CHARGELWO2_H_INCLUDED

/*
=====================================================================================================================

                    Chargeur de fichier LWO2 - V2 - Mars 2011

=====================================================================================================================
*/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "../glew.h"
#include <GLFW/glfw3.h>
#include "../chargeurs_images/Image.h"
//#include "gestion_fichiers.cpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;



/*
*   Chargement des textures:
*   ------------------------
*
*   Les répertoires des images utilisées pour les textures sont redirigés vers "chemin_images"
*   Si ce chemin n'est pas défini (NULL), le chemin figurant dans le fichier LWO2 est utilisé.
*
*/

//======== Codes des erreurs:

#define CHARGELWO2_OK 0
#define CHARGELWO2_ERREUR_LWO2 1
#define CHARGELWO2_ERREUR_CALQUES 2
#define CHARGELWO2_ERREUR_SURFACES 3
#define CHARGELWO2_ERREUR_VOLUMES 4
#define CHARGELWO2_ERREUR_UV 5
#define CHARGELWO2_ERREUR_CLIPS 6
#define CHARGELWO2_ERREUR_BLOCS 7
#define CHARGELWO2_ERREUR_FICHIER 8
#define CHARGELWO2_ERREUR_MEMOIRE 9

//+++++ Nouvelle version
#define CHARGELWO2_DEBUG false
#define CHARGELWO2_DEBUG_UV false
#define CHARGELWO2_DEBUG_DUREES false
#define CHARGELWO2_DEBUG_SURFACES false
#define CHARGELWO2_DEBUG_TEXTURES false


/* chunk and subchunk IDs */

//======================== Codes ASCII des majuscules
// Ca peut servir dans de cas de bloc inconnus
//A=0x41    //N=0x4E    0=0x30
//B=0x42    //O=0x4F    1=0x31
//C=0x43    //P=0x50    2=0x32
//D=0x44    //Q=0x51    3=0x33
//E=0x45    //R=0x52    4=0x34
//F=0x46    //S=0x53    5=0x35
//G=0x47    //T=0x54
//H=0x48    //U=0x55
//I=0x49    //V=0x56
//J=0x4A    //W=0x57
//K=0x4B    //X=0x58
//L=0x4C    //Y=0x59
//M=0x4D    //Z=0x5A

#define LWID_(a,b,c,d) (((a)<<24)|((b)<<16)|((c)<<8)|(d))

#define ID_FORM  LWID_('F','O','R','M')
#define ID_LWO2  LWID_('L','W','O','2')
#define ID_LWOB  LWID_('L','W','O','B')

/* Identifiants récents: */
#define ID_VERS  LWID_('V','E','R','S')
#define ID_NODS  LWID_('N','O','D','S')

/* top-level chunks */
#define ID_LAYR  LWID_('L','A','Y','R')
#define ID_TAGS  LWID_('T','A','G','S')
#define ID_PNTS  LWID_('P','N','T','S')
#define ID_BBOX  LWID_('B','B','O','X')
#define ID_VMAP  LWID_('V','M','A','P')
#define ID_VMAD  LWID_('V','M','A','D')
#define ID_POLS  LWID_('P','O','L','S')
#define ID_PTAG  LWID_('P','T','A','G')
#define ID_ENVL  LWID_('E','N','V','L')
#define ID_CLIP  LWID_('C','L','I','P')
#define ID_SURF  LWID_('S','U','R','F')
#define ID_DESC  LWID_('D','E','S','C')
#define ID_TEXT  LWID_('T','E','X','T')
#define ID_ICON  LWID_('I','C','O','N')

/* types VMAP */
#define ID_PICK  LWID_('P','I','C','K')
#define ID_WGHT  LWID_('W','G','H','T')
#define ID_MNVW  LWID_('M','N','V','W')
#define ID_TXUV  LWID_('T','X','U','V')
#define ID_RGB   LWID_('R','G','B',' ')
#define ID_RGBA  LWID_('R','G','B','A')
#define ID_MORF  LWID_('M','O','R','F')
#define ID_SPOT  LWID_('S','P','O','T')

/* polygon types */
#define ID_FACE  LWID_('F','A','C','E')
#define ID_CURV  LWID_('C','U','R','V')
#define ID_PTCH  LWID_('P','T','C','H')
#define ID_MBAL  LWID_('M','B','A','L')
#define ID_BONE  LWID_('B','O','N','E')

/* polygon tags */
#define ID_SURF  LWID_('S','U','R','F')
#define ID_PART  LWID_('P','A','R','T')
#define ID_SMGP  LWID_('S','M','G','P')

/* envelopes */
#define ID_PRE   LWID_('P','R','E',' ')
#define ID_POST  LWID_('P','O','S','T')
#define ID_KEY   LWID_('K','E','Y',' ')
#define ID_SPAN  LWID_('S','P','A','N')
#define ID_TCB   LWID_('T','C','B',' ')
#define ID_HERM  LWID_('H','E','R','M')
#define ID_BEZI  LWID_('B','E','Z','I')
#define ID_BEZ2  LWID_('B','E','Z','2')
#define ID_LINE  LWID_('L','I','N','E')
#define ID_STEP  LWID_('S','T','E','P')

/* clips */
#define ID_STIL  LWID_('S','T','I','L')
#define ID_ISEQ  LWID_('I','S','E','Q')
#define ID_ANIM  LWID_('A','N','I','M')
#define ID_XREF  LWID_('X','R','E','F')
#define ID_STCC  LWID_('S','T','C','C')
#define ID_TIME  LWID_('T','I','M','E')
#define ID_CONT  LWID_('C','O','N','T')
#define ID_BRIT  LWID_('B','R','I','T')
#define ID_SATR  LWID_('S','A','T','R')
#define ID_HUE   LWID_('H','U','E',' ')
#define ID_GAMM  LWID_('G','A','M','M')
#define ID_NEGA  LWID_('N','E','G','A')
#define ID_IFLT  LWID_('I','F','L','T')
#define ID_PFLT  LWID_('P','F','L','T')

/* surfaces */
#define ID_COLR  LWID_('C','O','L','R')
#define ID_LUMI  LWID_('L','U','M','I')
#define ID_DIFF  LWID_('D','I','F','F')
#define ID_SPEC  LWID_('S','P','E','C')
#define ID_GLOS  LWID_('G','L','O','S')
#define ID_REFL  LWID_('R','E','F','L')
#define ID_RFOP  LWID_('R','F','O','P')
#define ID_RIMG  LWID_('R','I','M','G')
#define ID_RSAN  LWID_('R','S','A','N')
#define ID_RBLR  LWID_('R','B','L','R')
#define ID_TRAN  LWID_('T','R','A','N')
#define ID_TROP  LWID_('T','R','O','P')
#define ID_TIMG  LWID_('T','I','M','G')
#define ID_TBLR  LWID_('T','B','L','R')
#define ID_RIND  LWID_('R','I','N','D')
#define ID_TRNL  LWID_('T','R','N','L')
#define ID_BUMP  LWID_('B','U','M','P')
#define ID_SMAN  LWID_('S','M','A','N')
#define ID_SIDE  LWID_('S','I','D','E')
#define ID_CLRH  LWID_('C','L','R','H')
#define ID_CLRF  LWID_('C','L','R','F')
#define ID_GLOW  LWID_('G','L','O','W')
#define ID_ADTR  LWID_('A','D','T','R')
#define ID_SHRP  LWID_('S','H','R','P')
#define ID_LINE  LWID_('L','I','N','E')
#define ID_LSIZ  LWID_('L','S','I','Z')
#define ID_ALPH  LWID_('A','L','P','H')
#define ID_VCOL  LWID_('V','C','O','L')
#define ID_AVAL  LWID_('A','V','A','L')
#define ID_GVAL  LWID_('G','V','A','L')
#define ID_BLOK  LWID_('B','L','O','K')
#define ID_NVSK  LWID_('N','V','S','K')

/* texture layer */
#define ID_TYPE  LWID_('T','Y','P','E')
#define ID_CHAN  LWID_('C','H','A','N')
#define ID_NAME  LWID_('N','A','M','E')
#define ID_ENAB  LWID_('E','N','A','B')
#define ID_OPAC  LWID_('O','P','A','C')
#define ID_FLAG  LWID_('F','L','A','G')
#define ID_PROJ  LWID_('P','R','O','J')
#define ID_STCK  LWID_('S','T','C','K')
#define ID_TAMP  LWID_('T','A','M','P')

/* texture coordinates */
#define ID_TMAP  LWID_('T','M','A','P')
#define ID_AXIS  LWID_('A','X','I','S')
#define ID_CNTR  LWID_('C','N','T','R')
#define ID_SIZE  LWID_('S','I','Z','E')
#define ID_ROTA  LWID_('R','O','T','A')
#define ID_OREF  LWID_('O','R','E','F')
#define ID_FALL  LWID_('F','A','L','L')
#define ID_CSYS  LWID_('C','S','Y','S')

/* image map */
#define ID_IMAP  LWID_('I','M','A','P')
#define ID_IMAG  LWID_('I','M','A','G')
#define ID_WRAP  LWID_('W','R','A','P')
#define ID_WRPW  LWID_('W','R','P','W')
#define ID_WRPH  LWID_('W','R','P','H')
#define ID_VMAP  LWID_('V','M','A','P')
#define ID_AAST  LWID_('A','A','S','T')
#define ID_PIXB  LWID_('P','I','X','B')

/* procedural */
#define ID_PROC  LWID_('P','R','O','C')
#define ID_COLR  LWID_('C','O','L','R')
#define ID_VALU  LWID_('V','A','L','U')
#define ID_FUNC  LWID_('F','U','N','C')
#define ID_FTPS  LWID_('F','T','P','S')
#define ID_ITPS  LWID_('I','T','P','S')
#define ID_ETPS  LWID_('E','T','P','S')

/* gradient */
#define ID_GRAD  LWID_('G','R','A','D')
#define ID_GRST  LWID_('G','R','S','T')
#define ID_GREN  LWID_('G','R','E','N')
#define ID_PNAM  LWID_('P','N','A','M')
#define ID_INAM  LWID_('I','N','A','M')
#define ID_GRPT  LWID_('G','R','P','T')
#define ID_FKEY  LWID_('F','K','E','Y')
#define ID_IKEY  LWID_('I','K','E','Y')

/* shader */
#define ID_SHDR  LWID_('S','H','D','R')
#define ID_DATA  LWID_('D','A','T','A')

//======================== Structures des définitions de blocs(pour la définition des textures):

//Types d'opacité (définis par le sous-chunk OPAC):
#define LWO2_OPAC_Normale 0
#define LWO2_OPAC_Soustraction 1
#define LWO2_OPAC_Difference 2
#define LWO2_OPAC_Multiplication 3
#define LWO2_OPAC_Division 4
#define LWO2_OPAC_Alpha 5
#define LWO2_OPAC_Deplacement_texture 6
#define LWO2_OPAC_Addition 7

//Axes de déplacement dans le cas d'un type OPAC_Deplacement_texture (sous-chunk AXIS)
#define LWO2_AXIS_X 0
#define LWO2_AXIS_Y 1
#define LWO2_AXIS_Z 2

//Systèmes de coordonnées (bloc CSYS)
#define LWO2_REPERE_RELATIF 0
#define LWO2_REPERE_ABSOLU 1

//Modes de projections des textures. Pour le moment seul le mode UV est implémenté:
#define LWO2_TYPE_PROJECTION_PLAN 0
#define LWO2_TYPE_PROJECTION_CYLINDRIQUE 1
#define LWO2_TYPE_PROJECTION_SPHERIQUE 2
#define LWO2_TYPE_PROJECTION_CUBIQUE 3
#define LWO2_TYPE_PROJECTION_FRONTALE 4
#define LWO2_TYPE_PROJECTION_UV 5

//Modes de débordement des textures:
#define LWO2_MODE_DEBORDEMENT_DECOUPAGE 0
#define LWO2_MODE_DEBORDEMENT_REPETITION 1
#define LWO2_MODE_DEBORDEMENT_REFLEXION 2
#define LWO2_MODE_DEBORDEMENT_BORD 3

//---------------------------------------------------------------
//      Définition des bitmaps (blocs CLIP)
//      Les plugins et les filtres ne sont pas pris en compte
//      Les animations ne sont pas prises en compte.
//      Seulement les images fixes au format TIFF (CLIP STIL)
//---------------------------------------------------------------
class LWO2_Bitmap
{
    public:
        uint32_t identite;
        char*   fichier;
        Image* image;
        float   contraste;          //Défaut=0
        float   luminosite;         //Défaut=0
        float   saturation;         //Défaut=0
        float   decallage_teinte;   //defaut=0
        float   gamma;              //Défaut=1
        bool    negatif;            //true=invertion RGB

        LWO2_Bitmap();
        ~LWO2_Bitmap();

};

//---------------------------------------------------------------
//      Définition des polygones
//      Seuls les polygones de type "FACE" sont pris en compte.
//---------------------------------------------------------------

class LWO2_Polygone
{
    public:
                //Ces trois indices pointent dans la table des étiquettes.
    uint16_t    indice_etiquette_surface; //Surface appliquée au polygone
    uint16_t    indice_etiquette_part;    //Donnée d'édition, c'est la couleur donnée aux arêtes
    uint16_t    indice_etiquette_groupe_filtre;   //Le groupe de filtrage (shading) auquel appartient le polygone

    uint32_t    nbrSommets;
    uint32_t*   indices_sommets; //"Vertexs" - Indices dans la table des sommets du calque

    LWO2_Polygone();
    ~LWO2_Polygone();
};

//---------------------------------------------------------------
//Bloc VMAP= Vertex Mapping, en français: paramètres de sommet
//  Seuls les types TXUV sont pris en compte
//---------------------------------------------------------------
typedef struct
{
    uint32_t indice_sommet;     //Le sommet à corriger
    uint32_t indice_polygone;   //Le polygone concerné par la correction
    float U;            //Les coordonnées UV de substitution
    float V;
}LWO2_Sommet_UV_couture;

typedef struct
{
    uint32_t indice_sommet;
    float U;
    float V;
}LWO2_Sommet_UV;


class LWO2_ParametresSommets
{
    public:
    char*       nom;    //Utilisé pour faire le lien avec les blocs définis dans les surfaces (voir plus haut)
    uint32_t    dimensions;
    vector<LWO2_Sommet_UV>   sommets_UV;
    //Ces deux champs servent pour corriger les polygones situés à la couture des textures UV:
    //Ce genre de paramètres de sommet est couplé avec des paramètres portants le même nom.
    //Les vecteurs de couture se substituent aux autres vecteurs lorsque les polygones spécifiés sont calculés.
    bool                couture;    //true=couture
    vector<LWO2_Sommet_UV_couture>    sommets_UV_couture;  //Polygones concernés par la couture

    LWO2_ParametresSommets();
    ~LWO2_ParametresSommets();
};

//---------------------------------------------------------------
//      Définition des textures de surface. (Surface bloc
//      Seules les textures UV et les projections planes sont implémentées pour le moment.
//      Le fondu d'effet de texture (FALL) est ignoré.
//---------------------------------------------------------------

class LWO2_Texture
{
    public:
    uint32_t type;  //IMAP (ou PROC ou GRAD ou SHDR, non pris en compte pour le moment)
    char* chaine_tri;   //La chaine utilisée pour le tri des calques de textures
    uint32_t canal; //COLR,DIFF,LUMI,SPEC,GLOS,REFL,TRAN,RIND,TRNL,BUMP
    char nom_canal[5];  //Le nom du canal en chaine de caractères. C'est plus simple à débuguer.
    uint16_t active;    //true ou false
    bool fondu_pixels;  //True: lorsque les pixels de la texture sont plus gros que les pixels de l'écran, ils sont fondus.
    uint16_t type_opacite;  //0:Normale, 1:Soustraction, 2:Différence, 3:Multiplication,4:Division, 5:Alpha, 6:Déplacement de texture, 7: Addition
    float valeur_opacite;
    float amplitude_rugosite;   //Sert pour les textures de rugosité(Bump). La valeur des pixels est multipliée par cette amplitude.
    char* objet_de_reference;   //Le nom de l'objet de référence pour le repère des données précédentes.
    uint16_t systeme_coordonnees; //Pour les textures de projection ,spécifie le repère de référence (absolu ("world coordinates") ou relatif ("object coordinates") )
    float centre[3];        //Paramètres des textures projetées
    float echelle[3];
    float rotation[3];      //"head","pitch","bank" roulis,tangage,lacet en radians
    //Données propres au type IMAP:
    uint16_t    type_projection; //0:Plan, 1: Cylindrique, 2:Sphérique, 3:Cubique, 4: Frontale, 5:UV
    uint16_t    axe_projection; //0:X, 1:Y, 2:Z (Ne sert pas pour les UV)
    uint32_t identite_clip;  //L'identité du clip utilisé (image bitmap)
    uint16_t mode_debordement_largeur;  //0:découpe (=clamp), 1:répétition, 2:miroir, 3:couleur des bords
    uint16_t mode_debordement_hauteur;
    float nbr_debordements_largeur; //Nombre de répétition pour les projection sphériques et cylindriques.
    float nbr_debordements_hauteur;
    LWO2_ParametresSommets* Texture_UV;   //objet UV (structure définie ci-dessus)

    LWO2_Texture();
    ~LWO2_Texture();
};

//-------------------------------
//  Surfaces
//  Les enveloppes ne sont pas récupérées.
//--------------------------------

class LWO2_Surface
{
    public:
    char* nom; //Nom de la surface
    char* nom_source; //Le nom de la surface de laquelle est dérivée celle-ci. Le traitement des liens entre les surfaces et leurs sources est effectué après avoir distribué les données LWO2.
    uint32_t taille_chunk; //Taille du chunk SURF
    bool  double_face;    //false si le devant des faces est affiché, true si les deux cotés des faces sont visibles
    float couleur[3];      //COLR Couleur {R,V,B} en nombres flottants
    float luminescence;       //LUMI
    float diffusion;        //DIFF
    float specularite;      //SPEC
    float brillance;        //GLOS
    float reflexion;        //REFL
    float transparence;     //TRAN
    float translucidite;    //TRNL
    float teinte_lumiere;   //CLRH  taux de renvoie de la couleur de la source lumineuse. 0=renvoie toute la couleur de la source, 1.=ne renvoie que la couleur de la surface
    float terminateur;      //SHRP rapidité du fondu du terminateur des ombres (pris en compte par openGL?)
    float rugosite;         //BUMP  //A voir avec les shaders...
    float intensite_halo;             //GLOW ou GVAL - L'épaisseur du halo
    bool  halo;             //true si la surface émet un halo
    bool  adoucissement;    //True si les faces sont adoucies.

    float angle_limite_adoucissement; //l'angle entre les faces adjacentes au-delà duquel la couleur des sommets ne sont plus interpolées.
    vector<LWO2_Texture*> textures;    //Blocs de définition des textures ou des filtres (shaders) seules les définitions de textures UV sont récupérées.

    LWO2_Surface();
    ~LWO2_Surface();
};

//----------------------------------------
//          Calques
//----------------------------------------
class LWO2_Calque
{
    public:
        char*       nom;
        uint16_t    index;
        int16_t     parent; //L'index du calque parent. -1 si aucun parent.
        uint16_t    drapeaux;   //bit0=1 : le calque est masqué.
        float       pivot[3];
        float       boite[6];
        uint32_t    nbrSommets;
        float*      sommets;
        vector<LWO2_ParametresSommets*>   parametres_sommets;    //Blocs VMAP
        vector<LWO2_Polygone*> polygones;

    LWO2_Calque();
    ~LWO2_Calque();
    uint32_t renvoie_nbr_polygones();
    LWO2_Polygone* renvoie_polygone(uint32_t indice);
};




//------------Définition des blocs, avec respect des hiérarchies: (C'est asser complexe, voir la doc du format LWO2)


//************************************************************************************************
//************************************************************************************************
//                              DEFINITION DE LA CLASSE
//************************************************************************************************
//************************************************************************************************


class ChargeLWO2: public Fichiers
{
    public:
        int erreur;
    private:
        const char* chemin_fichier;
        const char*   chemin_images;
        uint32_t nbrCalques;
        uint32_t nbrSurfaces;
        double duree_chargement;
        ifstream fichier_flux;
        uint32_t taille_fichier;
        char* donnees_LWO2;  //Le fichier chargé en mémoire - Le traitement est plus rapide.
        vector<LWO2_Bitmap*> Bitmaps;  //La bibliothèque d'images du fichier LWO2
        vector<LWO2_Calque*> calques_lwo2;  //Les calques contenants les volumes
        vector<LWO2_Surface*> surfaces_lwo2;
        uint32_t*   ptr_lwo2;   //Tête de lecture des données au format LWO2
        uint32_t    taille_bloc,taille_sous_bloc;    //Sert lors de la recherche des sous-blocs
        uint32_t    position,position_bloc,position_sous_bloc;
        vector<char*> table_etiquettes;
        vector<LWO2_ParametresSommets*> table_parametres_sommets; //Les définitions de coordonnées UV utilisées par les textures
        //Arborescence basique du fichier
        char uint32_converti[5];   //Sert pour le debuguage, pour afficher les noms des blocs.

    public:
        ChargeLWO2(const char* nom_fichier,const char* p_chemin_images=NULL);
        ~ChargeLWO2();
        const char* renvoie_nom_fichier();
        LWO2_Calque* renvoie_LWO2_calque(uint32_t numCalque);
        LWO2_Calque* renvoie_LWO2_calque_id(uint32_t idCalque);
        LWO2_Calque* renvoie_LWO2_calque_nom(char* nom_calque);
        uint32_t renvoie_nbrBitmaps();
        uint32_t renvoie_nbrSurfaces();
        uint32_t renvoie_nbrCalques();
        LWO2_Surface* renvoie_LWO2_surface(uint32_t indice);
        LWO2_Surface* renvoie_LWO2_surface(char* nom);
        char* renvoie_etiquette(uint32_t indice);
        LWO2_Bitmap* renvoie_LWO2_bitMap_id(uint32_t id);
        LWO2_Bitmap* renvoie_LWO2_bitmap_index(uint32_t indice);
        LWO2_ParametresSommets* renvoie_ParametresSommets(char* nom_cherche);

    private:
        bool analyse();
        bool creation_table_etiquettes();
        bool creation_bitmaps();
        bool creation_calques_lwo2();
        bool creation_points(LWO2_Calque* calque);
        bool creation_polygones(LWO2_Calque* calque);
        bool creation_liens_polygones(vector<LWO2_Polygone*>* table_polygones);
        bool creation_parametres_sommets();
        LWO2_Sommet_UV* ajoute_LWO2_sommet_UV (LWO2_ParametresSommets* parametres_UV);
        LWO2_Sommet_UV_couture* ajoute_LWO2_sommet_UV_couture (LWO2_ParametresSommets* parametres_UV);
        bool creation_surfaces_lwo2();
        bool creation_textures_lwo2(LWO2_Surface* surface);
        uint32_t recupere_VX();
        uint32_t recupere_VX_sans_decallage_ptr();
        float recupere_sous_bloc_FP4();
        uint16_t recupere_sous_bloc_U2();
        void recupere_vecteur_3f(float* coordonnees);
        void recupere_vecteur_3f_sans_decallage_ptr(float* coordonnees);
        void recupere_vecteur_3f_convertion_3d(double* coordonnees);
        char* converti_uint32_chaine(uint32_t id);
};


#endif // CHARGELWO2_H_INCLUDED
