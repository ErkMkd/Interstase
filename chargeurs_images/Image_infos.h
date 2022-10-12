#ifndef IMAGE_INFOS_H_INCLUDED
#define IMAGE_INFOS_H_INCLUDED

#include <iostream>

class Image_infos
{
    public:
        uint32_t    largeur;
        uint32_t    hauteur;
        uint8_t*    image;
        uint16_t    bits_par_pixel;
        uint16_t    octets_par_pixel;
        uint32_t    palette[256];
        //uint16_t    octets_par_pixels;

    Image_infos();

};


#endif // IMAGE_INFOS_H_INCLUDED
