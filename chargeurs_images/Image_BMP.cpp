
#include "Image_BMP.h"

using namespace std;

//*************************************************************
// Chargement d'une image au format BMP
//*************************************************************

    Image_infos* Image_BMP::charge(const char *nom_fichier)
    {
        uint32_t taille_fichier;
        uint32_t debut_image;
        uint32_t taille_entete;
        uint16_t nbr_plans;
        uint32_t compression;
        uint32_t taille_totale; //0 Lorqu'il n'y a pas de compression
        uint32_t resolution_h;
        uint32_t resolution_v;
        uint32_t nbr_couleurs;
        uint32_t nbr_couleurs_importantes;
        ifstream    fichier;
        Image_infos* parametres=new Image_infos();


         //---Ouverture fichier:
        fichier.open(nom_fichier, ios::in | ios::binary | ios::ate );

        if (!fichier.is_open())
        {
            cout<<"ERREUR - Image_BMP::charge() - Impossible d'ouvrir le fichier: ' "<<nom_fichier<<" '"<<endl;
            delete parametres;
            return NULL;
        }

        //Taille du fichier:
        taille_fichier=fichier.tellg();

        //Test la Signature BM:
        uint16_t    BM_id;
        fichier.seekg(0,ios::beg);
        fichier.read((char*)&BM_id,2);
        BM_id=retournement_uint16(BM_id);

        if (BM_id!=IMAGE_BMP_ID)
        {
            cout<<"ERREUR dans Image_BMP::charge() - Le fichier n'est pas au format BMP: ' "<<nom_fichier<<" '"<<endl;
            delete parametres;
            return NULL;
        }

        //Test si la taille écrite dans l'entête correspond à la taille calculée:
        uint32_t taille_ecrite_dans_fichier;
        fichier.read((char*)&taille_ecrite_dans_fichier,4); //Pas besoin de retourner les octets

        if (taille_fichier!=taille_ecrite_dans_fichier)
        {
            cout<<"ERREUR dans Image_BMP::charge() - Les données sont corrompues dans le fichier ' "<<nom_fichier<<" '"<<endl;
            delete parametres;
            return NULL;
        }

        //On saute 4 octets réservés:
        fichier.seekg(4,ios::cur);

        //Lecture des données de l'image:
        fichier.read((char*)&debut_image,4);
        fichier.read((char*)&taille_entete,4);
        fichier.read((char*)&parametres->largeur,4);
        fichier.read((char*)&parametres->hauteur,4);
        fichier.read((char*)&nbr_plans,2);
        fichier.read((char*)&parametres->bits_par_pixel,2);
        fichier.read((char*)&compression,4);
        fichier.read((char*)&taille_totale,4);
        fichier.read((char*)&resolution_h,4);
        fichier.read((char*)&resolution_v,4);
        fichier.read((char*)&nbr_couleurs,4);
        if (parametres->bits_par_pixel==4)nbr_couleurs=16;
        else if ((parametres->bits_par_pixel==8) && (nbr_couleurs==0))nbr_couleurs=256;
        fichier.read((char*)&nbr_couleurs_importantes,4);

        if (DEBUG_IMAGE_BMP)
        {
            cout<<"Début des données de l'image: "<<dec<<debut_image<<endl;
            cout<<"taille de l'entête: "<<hex<<taille_entete<<endl;
            cout<<"Dimensions: "<<dec<<parametres->largeur<<" * "<<parametres->hauteur<<endl;
            cout<<"Bits par pixel: "<<parametres->bits_par_pixel<<endl;
            cout<<"Méthode de compression: "<<compression<<endl;
            cout<<"Taille totale de l'image: "<<taille_totale<<endl;
            cout<<"Résolution horizontale: "<<resolution_h<<endl;
            cout<<"Résolution verticale: "<<resolution_v<<endl;
            cout<<"Nombre de couleurs: "<<nbr_couleurs<<endl;
            cout<<"Nombre de couleurs importantes: "<<nbr_couleurs_importantes<<endl;

        }

        //Lecture de la palette si il y en a une:
        uint32_t i;
        if (nbr_couleurs>256)
        {
            cout<<"Erreur dans Image_BMP::charge() - Nombre de couleurs supérieur à 256 dans l'image ' "<<nom_fichier<<" '"<<endl;
            free(parametres->image);
            delete parametres;
            return NULL;
        }
        for (i=0;i<nbr_couleurs;i++)
        {
            fichier.read((char*)&parametres->palette[i],4);
        }


        //Calcul de la taille de l'image:
        parametres->octets_par_pixel=parametres->bits_par_pixel/8;
        uint32_t nbr_octets=parametres->largeur*parametres->hauteur*parametres->octets_par_pixel;


        //Allocation de la mémoire:
        parametres->image  = (uint8_t *) malloc (nbr_octets);

        if (parametres->image==NULL)
        {
            cout<<"Erreur dans Image_BMP::charge() - Impossible d'allouer la mémoire pour l'image ' "<<nom_fichier<<" '"<<endl;
            delete parametres;
            return NULL;
        }



        //Lecture des données:

        uint8_t* image=parametres->image;
        fichier.seekg(debut_image,ios::beg);
        fichier.read((char*)image,nbr_octets);
        fichier.close();

        //Remise en ordre des données:
        //Parce que cet incongru BMP stock ses pixels à l'envers.
        char temp_r,temp_v,temp_b,temp_a;

        switch (parametres->bits_par_pixel)
        {
            case 32:
                for (uint32_t y=0;y<(parametres->hauteur)/2+(parametres->hauteur&1);y++)
                {
                    for (uint32_t x=0;x<parametres->largeur;x++)
                    {
                        int pixel_haut=4*(y*parametres->largeur+x);
                        int pixel_bas=4*((parametres->hauteur-1-y)*parametres->largeur+x);
                        temp_r=image[pixel_haut+2];
                        temp_v=image[pixel_haut+1];
                        temp_b=image[pixel_haut];
                        temp_a=image[pixel_haut+3];
                        image[pixel_haut+2]=image[pixel_bas];
                        image[pixel_haut+1]=image[pixel_bas+1];
                        image[pixel_haut]=image[pixel_bas+2];
                        image[pixel_haut+3]=image[pixel_bas+3];
                        image[pixel_bas]=temp_r;
                        image[pixel_bas+1]=temp_v;
                        image[pixel_bas+2]=temp_b;
                        image[pixel_bas+3]=temp_a;
                    }
                }
                break;

            case 24:
                for (uint32_t y=0;y<parametres->hauteur/2;y++)
                {
                    for (uint32_t x=0;x<parametres->largeur;x++)
                    {
                        int pixel_haut=3*(y*parametres->largeur+x);
                        int pixel_bas=3*((parametres->hauteur-1-y)*parametres->largeur+x);
                        temp_r=image[pixel_haut+2];
                        temp_v=image[pixel_haut+1];
                        temp_b=image[pixel_haut];
                        image[pixel_haut+2]=image[pixel_bas];
                        image[pixel_haut+1]=image[pixel_bas+1];
                        image[pixel_haut]=image[pixel_bas+2];
                        image[pixel_bas]=temp_r;
                        image[pixel_bas+1]=temp_v;
                        image[pixel_bas+2]=temp_b;
                    }
                }
                break;
            case 16:
                break;
            case 8:
                break;
            case 4:
                break;
            default:
                break;
        }

        return parametres;
    }

