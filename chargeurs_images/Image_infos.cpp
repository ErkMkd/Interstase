

//---------------------------------------------------------------
//          Information sur l'image
//      C'est une structure interm�diaire, renvoy�e par
//      les classes statiques g�rants les diff�rents formats
//---------------------------------------------------------------

#include "Image_infos.h"


    Image_infos::Image_infos()
    {
        largeur=0;
        hauteur=0;
        image=NULL;
        bits_par_pixel=0;
        octets_par_pixel=0;
    }

