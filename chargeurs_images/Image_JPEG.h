#ifndef IMAGE_JPEG_H_INCLUDED
#define IMAGE_JPEG_H_INCLUDED

#include <string.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "jpeglib.h"
#include "Image_infos.h"

using namespace std;

#define DEBUG_IMAGE_JPEG false

class Image_JPEG
{
    public:

    static Image_infos* charge(const char *nom_fichier);
};

#endif // IMAGE_JPEG_H_INCLUDED
