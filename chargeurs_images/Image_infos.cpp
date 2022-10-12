

//---------------------------------------------------------------
//          Information sur l'image
//      C'est une structure intermédiaire, renvoyée par
//      les classes statiques gérants les différents formats
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

