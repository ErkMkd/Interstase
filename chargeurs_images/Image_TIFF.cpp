

#include "Image_TIFF.h"

using namespace std;


//*************************************************************************
//          Chargement d'une image au format TIFF
//          Les images TIFF peuvent être stockées sous forme de bandes.
//*************************************************************************



    Image_infos* Image_TIFF::charge(const char *nom_fichier)
    {
        TIFF* fichier_tiff;
        uint32_t bits_par_composante;
        uint32_t taille_bande;  //Nombre d'octet par bande
        uint32_t nbr_bandes;    //Nombre de bandes constituants l'image
        uint32_t taille_image;  //Nombre d'octets occupés en mémoire
        uint32_t composantes_par_pixel;
        Image_infos* parametres=new Image_infos();

        //----Ouverture:

        fichier_tiff=TIFFOpen(nom_fichier,"r");
        if (fichier_tiff==NULL)
        {
            cout<<"Erreur dans Image_TIFF::charge() - Impossible d'ouvrir le fichier: ' "<<nom_fichier<<" '"<<endl;
            return NULL;
        }

        //----Données de l'image:
        TIFFGetField(fichier_tiff, TIFFTAG_IMAGEWIDTH, &parametres->largeur);
        TIFFGetField(fichier_tiff, TIFFTAG_IMAGELENGTH, &parametres->hauteur);
        TIFFGetField(fichier_tiff, TIFFTAG_BITSPERSAMPLE, &bits_par_composante);
        TIFFGetField(fichier_tiff, TIFFTAG_SAMPLESPERPIXEL, &composantes_par_pixel);
        parametres->bits_par_pixel=bits_par_composante*composantes_par_pixel;
        taille_bande=TIFFStripSize(fichier_tiff);
        nbr_bandes=TIFFNumberOfStrips(fichier_tiff);
        taille_image=taille_bande*nbr_bandes;

        //---- Allocation de la mémoire:

        parametres->image = (uint8_t*) malloc(taille_image);

        if (parametres->image == NULL)
        {
            cout<<"Erreur dans Image_TIFF::charge() - Mémoire insuffisante: ' "<<nom_fichier<<" '"<<endl;
            delete parametres;
            return NULL;
        }

        //---- Lecture des données:
        uint32_t pos=0; //Curseur dans les données de l'image
        uint32_t bande_cptr;
        int32_t octets_lus;

        for (bande_cptr=0;bande_cptr<nbr_bandes;bande_cptr++)
        {
            if ((octets_lus=TIFFReadEncodedStrip(fichier_tiff,bande_cptr,parametres->image+pos,taille_bande))==-1)
            {
                cout<<"Image_TIFF::charge() - Erreur dans le décodage de l'image: ' "<<nom_fichier<<" '"<<endl;
                free(parametres->image);
                delete parametres;
                return NULL;
            }
            pos+=octets_lus;
        }

        //----Fermeture:
        TIFFClose(fichier_tiff);
        fichier_tiff=NULL;

        //---- Affiche les données:
        if (DEBUG_IMAGE_TIFF)
        {
            cout<<" Dimensions: "<<parametres->largeur<<" * "<<parametres->hauteur<<endl;
            cout<<" Bits par pixel: "<<parametres->bits_par_pixel<<endl;
            cout<<" Taille des bandes:"<<taille_bande<<endl;
            cout<<" Nombre de bandes:"<<nbr_bandes<<endl;
            cout<<" Taille de l'image:"<<taille_image<<endl;
        }

        parametres->octets_par_pixel=parametres->bits_par_pixel/8;
        return parametres;
    }


