#ifndef IMAGE_BMP_H_INCLUDED
#define IMAGE_BMP_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "../gestion_fichiers.h"
#include "Image_infos.h"

#define DEBUG_IMAGE_BMP false

#define IMAGE_BMP_ID2_(a,b) (((a)<<8)|(b))
#define IMAGE_BMP_ID   IMAGE_BMP_ID2_('B','M')

using namespace std;

//*************************************************************
// Chargement d'une image au format BMP
//*************************************************************


class Image_BMP: public Fichiers
{
    public:

    static Image_infos* charge(const char *nom_fichier);

};



#endif // IMAGE_BMP_H_INCLUDED
