#include "Sauve_Rendu.h"


uint8_t *Sauve_Rendu::pixels=new uint8_t[RESOLUTION_RENDU_DEMO_X*RESOLUTION_RENDU_DEMO_Y*3];
GLsizei Sauve_Rendu::largeur=RESOLUTION_RENDU_DEMO_X;
GLsizei Sauve_Rendu::hauteur=RESOLUTION_RENDU_DEMO_Y;

bool Sauve_Rendu::alloue_pixels(GLsizei p_largeur, GLsizei p_hauteur)
{
    /*
    largeur=p_largeur;
    hauteur=p_hauteur;
    uint8_t *pixels = new uint8_t[largeur * hauteur * 3];
    */
}

void Sauve_Rendu::copy_texture_en_memoire(int source_fbo,int destination_fbo)
{
    glViewport(0,0,largeur,hauteur);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,FrameBuffers::fbos[destination_fbo]);
    glEnable(GL_TEXTURE_2D);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_BLEND);
    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D,FrameBuffers::fbTex[source_fbo]);
    GFunc::affiche_texture(1.);

    glBindFramebuffer(GL_READ_FRAMEBUFFER,FrameBuffers::fbos[destination_fbo]);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, largeur, hauteur, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid *)pixels);
    glBindFramebuffer(GL_READ_FRAMEBUFFER,0);

    // invert pixels (stolen from SOILs source code)
    /*
    for (int j = 0; j * 2 < hauteur; ++j)
    {
        int x = j * largeur * 3;
        int y = (hauteur - 1 - j) * largeur * 3;
        for (int i = largeur * 3; i > 0; --i)
        {
            uint8_t tmp = pixels[x];
            pixels[x] = pixels[y];
            pixels[y] = tmp;
            ++x;
            ++y;
        }
    }
    */
}

bool Sauve_Rendu::sauve_fichier(const char *filename,int source_fbo,int destination_fbo)
{

    copy_texture_en_memoire(source_fbo,destination_fbo);

    // Ecriture des données:
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png)
        return false;

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_write_struct(&png, &info);
        return false;
    }

    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        png_destroy_write_struct(&png, &info);
        return false;
    }

    png_init_io(png, fp);
    png_set_IHDR(png, info, largeur, hauteur, 8 /* depth */, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_colorp palette = (png_colorp)png_malloc(png, PNG_MAX_PALETTE_LENGTH * sizeof(png_color));
    if (!palette) {
        fclose(fp);
        png_destroy_write_struct(&png, &info);
        return false;
    }
    png_set_PLTE(png, info, palette, PNG_MAX_PALETTE_LENGTH);
    png_write_info(png, info);
    png_set_packing(png);

    png_bytepp rows = (png_bytepp)png_malloc(png, hauteur * sizeof(png_bytep));
    for (int i = 0; i < hauteur; ++i)
        rows[i] = (png_bytep)(pixels + (hauteur - i - 1) * largeur * 3);

    png_write_image(png, rows);
    png_write_end(png, info);
    png_free(png, palette);
    png_destroy_write_struct(&png, &info);

    fclose(fp);
    delete[] rows;
    return true;
}
