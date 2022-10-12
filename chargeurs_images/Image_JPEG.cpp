
#include "Image_JPEG.h"

using namespace std;


//*************************************************************************
//          Chargement d'une image au format TIFF
//          Les images TIFF peuvent être stockées sous forme de bandes.
//*************************************************************************

    Image_infos* Image_JPEG::charge(const char *nom_fichier)
    {
        //---Initialise la structure de décompression:
        jpeg_decompress_struct decompression_infos;
        jpeg_error_mgr gestionnaire_erreurs;
        decompression_infos.err=jpeg_std_error(&gestionnaire_erreurs);
        jpeg_create_decompress(&decompression_infos);

        //--- Ouverture du fichier:
        FILE* fichier;
        if ((fichier=fopen(nom_fichier,"rb"))==NULL)
        {
            cout<<"Erreur dans Image_JPG::charge() - Impossible de charger le fichier ' "<<" '"<<endl;
            return NULL;
        }

        jpeg_stdio_src(&decompression_infos,fichier);

        //--- Lecture des paramètres:
        jpeg_read_header(&decompression_infos,true);
        jpeg_calc_output_dimensions(&decompression_infos);

        //--- Test le format des couleurs:
        //Seules les images RGB sont prises en charge.

        if ((decompression_infos.jpeg_color_space-1) == JCS_CMYK)
        {
            cout<<"Erreur dans Image_JPG::charge() - Format CMYK non supporté"<<endl;
            fclose(fichier);
            return NULL;
        }

        //--- Saisie des paramètres:
        Image_infos* parametres=new Image_infos();

        parametres->largeur=decompression_infos.output_width;
        parametres->hauteur=decompression_infos.output_height;
        parametres->octets_par_pixel=decompression_infos.output_components;

        if (DEBUG_IMAGE_JPEG)
        {
            cout<<"largeur: "<<parametres->largeur<<endl;
            cout<<"hauteur:"<<parametres->hauteur<<endl;
            cout<<"Composantes par pixel: "<<parametres->octets_par_pixel<<endl;
        }
        parametres->bits_par_pixel=parametres->octets_par_pixel*8;

        //--- Allocations:
        parametres->image=(uint8_t*)malloc(parametres->octets_par_pixel*parametres->largeur*parametres->hauteur);
        if (parametres->image==NULL)
        {
            cout<<"Erreur dans Image_JPEG::charge() - Mémoire insuffisante pour l'image ' "<<nom_fichier<<" '"<<endl;
            fclose(fichier);
            delete parametres;
            return NULL;
        }

        //Création d'un tableau de pointeurs sur les lignes de l'image:
        JSAMPLE* tableau_scanlines[parametres->hauteur];

        for (uint32_t i=0;i<parametres->hauteur;i++)
        {
            tableau_scanlines[i]=(JSAMPLE*)(parametres->image+i*parametres->largeur*parametres->octets_par_pixel);
        }

        //--- Décompression:

        jpeg_start_decompress(&decompression_infos);
        uint32_t ligne=0;
        while (decompression_infos.output_scanline<parametres->hauteur)
        {
            ligne+=jpeg_read_scanlines(&decompression_infos,&tableau_scanlines[ligne],parametres->hauteur);;
        }

        //--- Destruction de la structure de décompression:

        jpeg_finish_decompress(&decompression_infos);
        jpeg_destroy_decompress(&decompression_infos);
        fclose(fichier);

        //delete parametres;
        return parametres;

    }
