#ifndef IMAGE_TIFF_H_INCLUDED
#define IMAGE_TIFF_H_INCLUDED


#include "tiffio.h"
#include "Image_infos.h"

#include <string.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#define DEBUG_IMAGE_TIFF false


class Image_TIFF
{
    public:

    static Image_infos* charge(const char *nom_fichier);

};

#endif // IMAGE_TIFF_H_INCLUDED
