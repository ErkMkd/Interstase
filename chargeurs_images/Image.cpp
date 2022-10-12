

#include "Image.h"
using namespace std;


//*************************************************************
// Format de base d'une image
//*************************************************************


    bool Image::erreur_openGl(const char* message)
    {
        GLenum erreur_openGl=glGetError();
        if (erreur_openGl!=GL_NO_ERROR)
        {
            cout<<message<<" : "<<gluErrorString(erreur_openGl)<<endl;
            return true;
        }
         return false;
    }

    //=======================================================
    //              Constructeur
    //=======================================================

    Image::Image(const char* p_nom_image)
    {
        image=NULL;
        texture_id=0;
        format_pixels=0;
        type_pixels=0;
        largeur=0;
        hauteur=0;
        bits_par_pixel=0;
        octets_par_pixel=0;
        nbr_couleurs=0;
        erreur=IMAGE_OK;
        nom_image=p_nom_image;

        if (IMAGE_DEBUG) cout << "======================= Chargement de l'image ' "<< p_nom_image << " '"<<endl;

        if (test_format()) initialise_format_OpenGl();

        if (IMAGE_DEBUG)
        {
            cout<<"Largeur: "<<largeur<<endl;
            cout<<"Hauteur: "<<hauteur<<endl;
            cout<<"Octets par pixels: "<<octets_par_pixel<<endl;
            cout<<"Format OpenGl: "<<hex<<format_pixels<<dec<<endl;
        }
    }

    //=======================================================
    //              Constructeur de copie
    //=======================================================
    Image::Image(Image& image_originale)
    {
        uint32_t i;
        if (IMAGE_DEBUG) cout<<"Copie de "<<image_originale.nom_image<<endl;

        if (image_originale.erreur!=IMAGE_OK) return;

        texture_id=0;
        buffer_id=0;
        format_pixels=image_originale.format_pixels;
        type_pixels=image_originale.type_pixels;
        largeur=image_originale.largeur;
        hauteur=image_originale.hauteur;
        erreur=IMAGE_OK;
        nom_image=image_originale.nom_image; //Peut-être modifier le nom?
        bits_par_pixel=image_originale.bits_par_pixel;
        octets_par_pixel=image_originale.octets_par_pixel;
        nbr_couleurs=image_originale.nbr_couleurs;
        for( i=0 ; i<nbr_couleurs ; i++ )
        {
            palette[i]=image_originale.palette[i];
        }

        image=(uint8_t*)malloc(largeur*hauteur*octets_par_pixel);

        if (image==NULL)
        {
            cout<<"ERREUR dans Image::Image(Image&) - Mémoire insuffisante pour copier l'image ' "<<nom_image<<" '"<<endl;
            erreur=IMAGE_ERREUR_MEMOIRE;
            return;
        }

        for (i=0;i<largeur*hauteur*octets_par_pixel;i++)
        {
            image[i]=image_originale.image[i];
        }

    }

    //--------------------------------------------
    //      Destructeur
    //--------------------------------------------
    Image::~Image()
    {
        if (image!=NULL) free(image);
        if (texture_id!=0) glDeleteTextures(1,&texture_id);
        #ifdef GLEW_STATIC
        if (buffer_id!=0) glDeleteBuffers(1,&buffer_id);
        #endif
    }

    //--------------------------------------------------------------
    //          Test le fichier pour déterminer le format de l'image
    //--------------------------------------------------------------

    bool Image::test_format()
    {
        //---Ouverture fichier:
        fichier_flux.open(nom_image, ios::in | ios::binary | ios::ate );

        if (!fichier_flux.is_open())
        {
            cout<<"ERREUR - Image - Impossible d'ouvrir le fichier: ' "<<nom_image<<" '"<<endl;
            erreur=IMAGE_ERREUR_FICHIER;
            return false;
        }

        //---Taille du fichier:
        taille_fichier=fichier_flux.tellg();
        if ( IMAGE_DEBUG ) cout<<"Taille du fichier: "<<taille_fichier<<endl;

        //--- Test BMP:
        fichier_flux.seekg(0,ios::beg);

        uint16_t id_BM;
        uint32_t    taille_lue;
        fichier_flux.read((char*)&id_BM,2);
        id_BM=retournement_uint16(id_BM);
        if (id_BM==IMAGEFORMAT_BMP)
        {
            fichier_flux.read((char*)&taille_lue,4);
            if (taille_fichier==taille_lue)
            {
                fichier_flux.close();
                if (IMAGE_DEBUG) cout << "Format: BMP"<<endl;
                return charge_BMP();
            }
        }

        //---- Test PNG:
        fichier_flux.seekg(0,ios::beg);

        uint64_t PNG_id;    //Les 8 premiers octets contiennent: 137 80 78 71 13 10 26 10

        fichier_flux.read((char*)&PNG_id,8);
        PNG_id=retournement_uint64(PNG_id);
        if (PNG_id==IMAGEFORMAT_PNG)
        {
            fichier_flux.close();
            if (IMAGE_DEBUG) cout << "Format: PNG"<<endl;
            return charge_PNG();
        }

        //--- Test TIFF:

        fichier_flux.seekg(0,ios::beg);

        uint16_t TIFF_ordre_octets;   //"MM" ou "II", selon le cas, détermine l'ordre d'apparition des octets.
        uint16_t TIFF_nombre_42;
        fichier_flux.read((char*)&TIFF_ordre_octets,2);
        fichier_flux.read((char*)&TIFF_nombre_42,2);     //Le numéro "42" apparait dans tout les fichier TIFF.

        if ((TIFF_ordre_octets==IMAGEFORMAT_II)||(TIFF_ordre_octets==IMAGEFORMAT_MM))
        {
            if (TIFF_ordre_octets==IMAGEFORMAT_MM) TIFF_nombre_42=retournement_uint16(TIFF_nombre_42);

            if (TIFF_nombre_42==42)
            {
                fichier_flux.close();
                if (IMAGE_DEBUG) cout << "Format: TIFF"<<endl;
                return charge_TIFF();
            }
        }

        //--- Test JPEG:

        fichier_flux.seekg(0,ios::beg);
        uint16_t    jpeg_SOI;
        fichier_flux.read((char*)&jpeg_SOI,2);
        jpeg_SOI=retournement_uint16(jpeg_SOI);

        if(jpeg_SOI==IMAGEFORMAT_JPEG_SOI)
        {
            fichier_flux.close();
            if (IMAGE_DEBUG) cout << "Format: JPEG"<<endl;
            return charge_JPEG();
        }

        //--- Format non reconnu:
        cout<<"Erreur dans Image.test_format() - Format non reconnu !!!  - ' "<<nom_image<<" '"<<endl;
        erreur=IMAGE_ERREUR_FORMAT;
        fichier_flux.close();
        return false;
    }

    //--------------------------------------------------------------
    //          Charge les données au format BMP
    //--------------------------------------------------------------
    bool Image::charge_BMP()
    {
        Image_infos* parametres=Image_BMP::charge(nom_image);
        if (parametres==NULL)
        {
            erreur=IMAGE_ERREUR_LECTURE;
            return false;
        }

        largeur=parametres->largeur;
        hauteur=parametres->hauteur;
        image=parametres->image;
        bits_par_pixel=parametres->bits_par_pixel;
        octets_par_pixel=parametres->octets_par_pixel;

        delete parametres;
        return true;
    }

    //--------------------------------------------------------------
    //          Charge les données au format PNG
    //--------------------------------------------------------------
    bool Image::charge_PNG()
    {
        cout<<"*** Format PNG non pris en charge !!! ***"<<endl;
        erreur=IMAGE_ERREUR_FORMAT;
        return false;
    }

    //--------------------------------------------------------------
    //          Charge les données au format TIFF
    //--------------------------------------------------------------
    bool Image::charge_TIFF()
    {
        Image_infos* parametres=Image_TIFF::charge(nom_image);
        if (parametres==NULL)
        {
            erreur=IMAGE_ERREUR_LECTURE;
            return false;
        }
        largeur=parametres->largeur;
        hauteur=parametres->hauteur;
        image=parametres->image;
        bits_par_pixel=parametres->bits_par_pixel;
        octets_par_pixel=parametres->octets_par_pixel;

        delete parametres;
        return true;
    }

    //--------------------------------------------------------------
    //          Charge les données au format JPEG
    //--------------------------------------------------------------
    bool Image::charge_JPEG()
    {
        Image_infos* parametres=Image_JPEG::charge(nom_image);
        if (parametres==NULL)
        {
            erreur=IMAGE_ERREUR_LECTURE;
            return false;
        }
        largeur=parametres->largeur;
        hauteur=parametres->hauteur;
        image=parametres->image;
        bits_par_pixel=parametres->bits_par_pixel;
        octets_par_pixel=parametres->octets_par_pixel;

        delete parametres;
        return true;
    }

    //--------------------------------------------------------------
    //          Initialise le format des pixels pour OpenGL
    //--------------------------------------------------------------
    void Image::initialise_format_OpenGl()
    {
        switch (bits_par_pixel)
        {
            case 8:
                format_pixels=GL_ALPHA8;    //GL_LUMINANCE8
                type_pixels=GL_UNSIGNED_BYTE;
                break; //!! format_pixels A CHANGER SELON L'USAGE DE L'IMAGE
            case 15:
                cout<<"Attention: Format 15 bits/pixels - Compatibilité douteuse..."<<endl;
                format_pixels=GL_RGB16;
                type_pixels=GL_UNSIGNED_BYTE;
                break;
            case 16:
                format_pixels=GL_RGB16;
                type_pixels=GL_UNSIGNED_BYTE;
                break;
            case 24:
                format_pixels=GL_RGB;
                type_pixels=GL_UNSIGNED_BYTE;
                break;
            case 32:
                format_pixels=GL_RGBA;
                type_pixels=GL_UNSIGNED_BYTE;
                break;
            default:
                cout<<"Erreur dans Image.initialise_format_OpenGl() - Format d'image non supporté par OpenGL - ' "<<nom_image<<" '"<<endl;
                erreur=IMAGE_ERREUR_FORMAT;
                break;
        }
    }

    //=======================================================================================
    //                  Genere un objet texture pour OpenGl:
    //=======================================================================================
    bool Image::genere_texture_openGL (bool drapeau_mipmap,
                                GLint recouvrement_s,GLint recouvrement_t,
                                GLint type_grossisement, GLint type_diminution)
    {

        #if IMAGE_DEBUG
            cout<<"Génère une texture pour OpenGl : ' "<<nom_image<<" '"<<endl;;
        #endif

        if (glIsTexture(texture_id)) return true;

        glGenTextures(1,&texture_id); //Allocation de l'identifiants de texture
        if (texture_id==0)
        {
            cout<<endl<<"Erreur dans Image.genere_texture_openGl() - Impossible de créer la texture "<<nom_image<<endl;
            erreur=IMAGE_ERREUR_TEXTURE;
            return false;
        }
        #if IMAGE_DEBUG
            cout<<" - Identifiant:"<<texture_id<<endl;
        #endif

        glBindTexture(GL_TEXTURE_2D,texture_id); //Sélection de la texture courante

        glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,recouvrement_s);          //valeur: la valeur du paramètre.
        glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,recouvrement_t); //Mode de débordement vertical de la texture 2d

        glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,type_grossisement); //Mode d'étirement de la texture
        glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,type_diminution); //Mode de rétrécissement de la texture

        //Génère les textures en "mipmaping", c'est à dire qu'en fonction de la taille de la texture
        //à l'écran, des bitmaps de différentes tailles sont utilisés. Cela peut optimiser l'affichage.
        if (drapeau_mipmap)
        {
            gluBuild2DMipmaps(GL_TEXTURE_2D,
                            GL_RGBA,    //Format de la texture
                            largeur,hauteur,
                            format_pixels,type_pixels,
                            image);
        }
        //Pas de texture de tailles différentes.
        else
        {
            glTexImage2D(GL_TEXTURE_2D, //Type de  texture
                        0,      //Niveau de détails de la texture
                        GL_RGBA,      //Format de la texture: 1:R, 2:RA, 3:RVB, 4:RVBA
                        largeur,hauteur, //largeur et hauteur de la texture en pixels
                        0,      //Largeur du bord. l et h sont incrémentées de 2x cette valeur
                        format_pixels, //Format des pixels
                        type_pixels, //Type des données
                        image);
        }


        return true;
    }

    //=======================================================================================
    //                  Genere un objet buffer pour OpenGl:
    //  Sert pour le déplacement de blocs de pixels.
    //=======================================================================================
    bool Image::genere_buffer_openGl()
    {

        if ( IMAGE_DEBUG ) cout<<"Génère un objet Buffer pour OpenGl : ' "<<nom_image<<" '"<<endl;

        //Création de l'objet:
        #ifdef GLEW_STATIC
        glGenBuffers(1,&buffer_id);
        if (buffer_id==0)
        {
            cout<<"Erreur dans Image.genere_buffer_openGl() - Impossible de créer l'objet buffer "<<nom_image<<endl;
            erreur=IMAGE_ERREUR_BUFFER;
            return false;
        }

        //Réserve la mémoire:
        //Création du type de buffer:
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER,buffer_id);
        glBufferData(GL_PIXEL_UNPACK_BUFFER,octets_par_pixel*largeur*hauteur,image,GL_STATIC_DRAW);
        if(erreur_openGl("Erreur dans Image::genere_buffer_openGL() : ")) return false;

        //Désactive l'usage du buffer de pixels:
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER,0);
        #endif
        return true;
    }

//=====================================================================
//          Orientation de l'image
//=====================================================================

    //----- Renversement d'axe horizontal:
    void Image::renversement_horizontal()
    {
        switch ( octets_par_pixel )
            {
                case 1:
                    renversement_horizontal_1();
                    break;
                case 2:
                    renversement_horizontal_2();
                    break;
                case 3:
                    renversement_horizontal_3();
                    break;
                case 4:
                    renversement_horizontal_4();
                    break;
                default:break;
            }
    }

    //-------- Renversement d'axe vertical:
    void Image::renversement_vertical()
    {
        switch (octets_par_pixel)
        {
            case 1:
                renversement_vertical_1();
                break;
            case 2:
                renversement_vertical_2();
                break;
            case 3:
                renversement_vertical_3();
                break;
            case 4:
                renversement_vertical_4();
                break;
            default:break;
        }
    }

    void Image::renversement_horizontal_1()
    {
        uint32_t taille_ligne=largeur;
        char* image_debut=(char*)image;
        char* image_fin=(char*)image+(hauteur-1)*taille_ligne;
        char temp;
        for (uint32_t y=0;y< hauteur/(int)2;y++)
        {
            for (uint32_t x=0;x<largeur;x++)
            {
                temp=*(image_debut+x);
                *(image_debut+x)=*(image_fin+x);
                *(image_fin+x)=temp;
            }
            image_fin-=taille_ligne;
            image_debut+=taille_ligne;
        }
    }

    void Image::renversement_horizontal_2()
    {
        uint32_t taille_ligne=largeur;
        uint16_t* image_debut=(uint16_t*)image;
        uint16_t* image_fin=(uint16_t*)image+(hauteur-1)*taille_ligne;
        uint16_t temp;
        for (uint32_t y=0;y< hauteur/(int)2;y++)
        {
            for (uint32_t x=0;x<largeur;x++)
            {
                temp=*(image_debut+x);
                *(image_debut+x)=*(image_fin+x);
                *(image_fin+x)=temp;
            }
            image_fin-=taille_ligne;
            image_debut+=taille_ligne;
        }
    }


    void Image::renversement_horizontal_3()
    {
        uint32_t taille_ligne=largeur*octets_par_pixel;
        char* image_debut=(char*)image;
        char* image_fin=(char*)image+(hauteur-1)*taille_ligne;
        char temp;
        for (uint32_t y=0;y< hauteur/(int)2;y++)
        {
            for (uint32_t x=0;x<largeur*3;x++)
            {
                temp=*(image_debut+x);
                *(image_debut+x)=*(image_fin+x);
                *(image_fin+x)=temp;
            }
            image_fin-=taille_ligne;
            image_debut+=taille_ligne;
        }
    }

    void Image::renversement_horizontal_4()
    {
        uint32_t taille_ligne=largeur;
        uint32_t* image_debut=(uint32_t*)image;
        uint32_t* image_fin=(uint32_t*)image+(hauteur-1)*taille_ligne;
        uint32_t temp;
        for (uint32_t y=0;y< hauteur/(int)2;y++)
        {
            for (uint32_t x=0;x<largeur;x++)
            {
                temp=*(image_debut+x);
                *(image_debut+x)=*(image_fin+x);
                *(image_fin+x)=temp;
            }
            image_fin-=taille_ligne;
            image_debut+=taille_ligne;
        }
    }

    void Image::renversement_vertical_1()
    {
        uint32_t taille_ligne=largeur;
        char temp;
        for (uint32_t y=0;y<hauteur;y++)
        {
            char*depart_ligne=(char*)image+y*taille_ligne;
            char*fin_ligne=(char*)depart_ligne+(largeur-1);
            for (uint32_t x=0;x<(largeur>>1);x++)
            {
                temp=*(fin_ligne-x);
                *(fin_ligne-x)=*(depart_ligne+x);
                *(depart_ligne+x)=temp;
            }
        }
    }

    void Image::renversement_vertical_2()
    {
        uint32_t taille_ligne=largeur;
        uint16_t temp;
        for (uint32_t y=0;y<hauteur;y++)
        {
            uint16_t*depart_ligne=(uint16_t*)image+y*taille_ligne;
            uint16_t*fin_ligne=(uint16_t*)depart_ligne+(largeur-1);
            for (uint32_t x=0;x<(largeur>>1);x++)
            {
                temp=*(fin_ligne-x);
                *(fin_ligne-x)=*(depart_ligne+x);
                *(depart_ligne+x)=temp;
            }
        }
    }

    void Image::renversement_vertical_3()
    {
        uint32_t taille_ligne=largeur*3;
        char tempr,tempv,tempb;
        for (uint32_t y=0;y<hauteur;y++)
        {
            uint8_t*depart_ligne=(uint8_t*)image+y*taille_ligne;
            uint8_t*fin_ligne=(uint8_t*)depart_ligne+(largeur-1)*3;
            for (uint32_t x=0;x<(largeur>>1);x++)
            {
                tempr=*(fin_ligne-x*3);
                tempv=*(fin_ligne-x*3+1);
                tempb=*(fin_ligne-x*3+2);
                *(fin_ligne-x*3)=*(depart_ligne+x*3);
                *(fin_ligne-x*3+1)=*(depart_ligne+x*3+1);
                *(fin_ligne-x*3+2)=*(depart_ligne+x*3+2);
                *(depart_ligne+x*3)=tempr;
                *(depart_ligne+x*3+1)=tempv;
                *(depart_ligne+x*3+2)=tempb;
            }
        }
    }

    void Image::renversement_vertical_4()
    {
        uint32_t taille_ligne=largeur;
        uint32_t temp;
        for (uint32_t y=0;y<hauteur;y++)
        {
            uint32_t*depart_ligne=(uint32_t*)image+y*taille_ligne;
            uint32_t*fin_ligne=(uint32_t*)depart_ligne+(largeur-1);
            for (uint32_t x=0;x<(largeur>>1);x++)
            {
                temp=*(fin_ligne-x);
                *(fin_ligne-x)=*(depart_ligne+x);
                *(depart_ligne+x)=temp;
            }
        }
    }


//=====================================================================
//          Convertion du format de l'image
//=====================================================================

//------------------------------------
//  Redimensionne l'image
/// ATTENTION - Il faut que la fenêtre OpenGl soit ouverte.
//------------------------------------

    bool Image::redimensionne(int32_t nl, int32_t nh)
    {
        glPixelStorei(GL_PACK_ALIGNMENT,1);
        uint8_t* nouveau_tempon=(uint8_t*)malloc(nl*nh*octets_par_pixel);
        if (nouveau_tempon==NULL)
        {
            cout<<"Erreur dans Image.redimensionne() - Mémoire insuffisante"<<endl;
            erreur=IMAGE_ERREUR_MEMOIRE;
            return false;
        }

        int32_t retour=gluScaleImage(format_pixels,largeur,hauteur,type_pixels,image,nl,nh,type_pixels,nouveau_tempon);

        if (retour!=0)
        {
            cout<<"Erreur dans Image.redimensionne() - Code GLU: "<<retour<<endl;
            erreur=IMAGE_ERREUR_GLU;
            free(nouveau_tempon);
            return false;
        }

        free(image);
        image=nouveau_tempon;
        largeur=nl;
        hauteur=nh;

        return true;
    }

//------------------------------------
//  Converti l'image au format RGBA
//------------------------------------


    bool Image::format_pixels_RGBA(uint8_t valeur_alpha)
    {
        if(octets_par_pixel==4)
        {
            format_pixels=GL_RGBA;
            return true;
        }
        uint8_t* nouveau_tempon=(uint8_t*)malloc(largeur*hauteur*4);
        if (nouveau_tempon==NULL)
        {
            cout<<"Erreur dans Image.format_pixels_RVBA() - Mémoire insuffisante"<<endl;
            erreur=IMAGE_ERREUR_MEMOIRE;
            return false;
        }

        switch (octets_par_pixel)
        {
            case 1:
                convertion_RVBA_gris(nouveau_tempon,valeur_alpha);
                break;
            case 2:
                cout<<"Erreur dans Image.format_pixels_RVBA() - Format de pixels 16 bits non pris en compte pour le moment"<<endl;
                free(nouveau_tempon);
                return false;
                break;
            case 3:
                convertion_RVB_RVBA(nouveau_tempon,valeur_alpha);
                break;
            default:
                cout<<"Erreur dans Image.format_pixels_RVBA() - Format de pixels inconnu "<<endl;
                free(nouveau_tempon);
                return false;
                break;

        }

        free(image);
        image=nouveau_tempon;
        format_pixels=GL_RGBA;
        type_pixels=GL_UNSIGNED_BYTE;
        octets_par_pixel=4;
        return true;
    }

    void Image::convertion_RVBA_gris(uint8_t* nouveau_tempon, uint8_t valeur_alpha)
    {
        for(uint32_t i=0;i<largeur*hauteur;i++)
        {
            ((uint32_t*)nouveau_tempon)[i]=(image[i]<<16) | (image[i]<<8) | (image[i]) | (valeur_alpha<<24);
        }
    }

    void Image::convertion_RVB_RVBA(uint8_t* nouveau_tempon, uint8_t valeur_alpha)
    {
        for(uint32_t i=0;i<largeur*hauteur;i++)
        {
            ((uint32_t*)nouveau_tempon)[i]=(image[3*i]) | (image[3*i+1]<<8) | (image[3*i+2]<<16) | (valeur_alpha<<24);
        }
    }

//------------------------------------------------------
//      Converti l'image en RVB
//------------------------------------------------------

    bool Image::format_pixels_RVB()
    {
        if (octets_par_pixel==3) return true;
        uint8_t* nouveau_tempon=(uint8_t*) malloc(largeur*hauteur*3);
        if (nouveau_tempon==NULL)
        if (nouveau_tempon==NULL)
        {
            cout<<"Erreur dans Image.format_pixels_RVB() - Mémoire insuffisante"<<endl;
            erreur=IMAGE_ERREUR_MEMOIRE;
            return false;
        }
        switch (octets_par_pixel)
        {
            case 1:
                convertion_8_RVB(nouveau_tempon);
                break;
            case 2:
                cout<<"Erreur dans Image.format_pixels_RVB() - Format de pixels 16 bits non pris en compte pour le moment"<<endl;
                free(nouveau_tempon);
                return false;
                break;
            case 4:
                convertion_RVBA_RVB(nouveau_tempon);
                break;
            default:
                cout<<"Erreur dans Image.format_pixels_RVB() - Format de pixels inconnu "<<endl;
                free(nouveau_tempon);
                return false;
        }

        free(image);
        image=nouveau_tempon;
        format_pixels=GL_RGB;
        type_pixels=GL_UNSIGNED_BYTE;
        octets_par_pixel=3;
        return true;
    }


    void Image::convertion_8_RVB(uint8_t* nouveau_tempon)
    {
        uint8_t c;
        for (uint32_t i=0;i<largeur*hauteur;i++)
        {
            c=image[i];
            nouveau_tempon[i*3]=c;
            nouveau_tempon[i*3+1]=c;
            nouveau_tempon[i*3+2]=c;
        }
    }

    void Image::convertion_RVBA_RVB(uint8_t* nouveau_tempon)
    {
        for (uint32_t i=0;i<largeur*hauteur;i++)
        {
            nouveau_tempon[i*3]=image[i*4];
            nouveau_tempon[i*3+1]=image[i*4+1];
            nouveau_tempon[i*3+2]=image[i*4+2];
        }
    }

//------------------------------------------------------
//      Converti l'image en couche alpha
//------------------------------------------------------

    bool Image::format_pixels_ALPHA()
    {
        if(convertion_8bits())
        {
            format_pixels=GL_ALPHA;
            return true;
        }
        else return false;
    }

//------------------------------------------------------
//      Converti l'image en couche luminance
//------------------------------------------------------
    bool Image::format_pixels_LUMINANCE()
    {
         if(convertion_8bits())
        {
            format_pixels=GL_LUMINANCE;
            return true;
        }
        else return false;
    }

//------------------------------------------------------
//      Converti l'image en couche luminance-alpha
//------------------------------------------------------
    bool Image::format_pixels_LUMINANCE_ALPHA()
    {
         if(convertion_8bits())
        {
            format_pixels=GL_LUMINANCE4_ALPHA4;
            return true;
        }
        else return false;
    }

//------------------------------------------------------
//      Converti l'image en couche d'intensité
//------------------------------------------------------
    bool Image::format_pixels_INTENSITY()
    {
         if(convertion_8bits())
        {
            format_pixels=GL_INTENSITY;
            return true;
        }
        else return false;
    }

//------------------------------------------------------
//      Convertions 8bits/pixel
//------------------------------------------------------

    bool Image::convertion_8bits()
    {
        if ( octets_par_pixel == 1 )
        {
            return true;
        }

        uint8_t* nouveau_tempon=(uint8_t*) malloc(largeur*hauteur);
        if (nouveau_tempon==NULL)
        {
            cout<<"Erreur dans Image.convertion_8bits() - Mémoire insuffisante"<<endl;
            erreur=IMAGE_ERREUR_MEMOIRE;
            return false;
        }

        switch (octets_par_pixel)
        {
            case 2:
                cout<<"Erreur dans Image.convertion_8bits() - Format de pixels 16 bits non pris en compte pour le moment"<<endl;
                free(nouveau_tempon);
                return false;
                break;
            case 3:
                convertion_RVB_8(nouveau_tempon);
                break;
            case 4:
                convertion_RVBA_8(nouveau_tempon);
                break;
            default:
                cout<<"Erreur dans Image.convertion_8bits() - Format de pixels inconnu "<<endl;
                free(nouveau_tempon);
                return false;
                break;
        }

        free(image);
        image=nouveau_tempon;
        type_pixels=GL_UNSIGNED_BYTE;
        octets_par_pixel=1;
        return true;
    }

    void Image::convertion_RVB_8(uint8_t* nouveau_tempon)
    {
        float r,v,b,gris;
        for (uint32_t i=0;i<largeur*hauteur;i++)
        {
            r=(float)image[3*i+2];
            v=(float)image[3*i+1];
            b=(float)image[3*i];
            gris=r*IMAGE_FACTEUR_GRIS_R+v*IMAGE_FACTEUR_GRIS_V+b*IMAGE_FACTEUR_GRIS_B;
            nouveau_tempon[i]=(uint8_t)gris;
        }
    }

    void Image::convertion_RVBA_8(uint8_t* nouveau_tempon)
    {
        float r,v,b,gris;
        for (uint32_t i=0;i<largeur*hauteur;i++)
        {
            r=(float)image[4*i+3];
            v=(float)image[4*i+2];
            b=(float)image[4*i+1];
            gris=r*IMAGE_FACTEUR_GRIS_R+v*IMAGE_FACTEUR_GRIS_V+b*IMAGE_FACTEUR_GRIS_B;
            nouveau_tempon[i]=(uint8_t)gris;
        }
    }

//=====================================================================
//          Filtres (contraste, luminosité, saturation...)
//=====================================================================

    //-----------------------------------------
    //  Réglage du contraste.
    //  la valeur est comprise entre -1 et 1.
    //-----------------------------------------
    void Image::determine_contraste(float contraste)
     {
        uint32_t i;
        if (contraste==0.)return;
        else if (contraste<-1.) contraste=-1.;
        else if(contraste>1.) contraste=1.;


        for(i=0;i<largeur*hauteur*octets_par_pixel;i++)
        {
            float amplitude;
            float composante=(float)image[i]-127.5;

            if (contraste>0.) amplitude=127.5-fabsf(composante);
            else amplitude=fabsf(composante);

            if (composante>0)composante+=amplitude*contraste;
            else composante-=amplitude*contraste;

            image[i]=(uint8_t)(composante+127.5);
        }
     }

    //-----------------------------------------
    //  Réglage de la luminosité.
    //  la valeur est comprise entre -1 et 1.
    //-----------------------------------------
    void Image::determine_luminosite(float luminosite)
    {
        uint32_t i;
        if (luminosite==0.)return;
        else if (luminosite<-1.) luminosite=-1.;
        else if(luminosite>1.) luminosite=1.;


        for(i=0;i<largeur*hauteur*octets_par_pixel;i++)
        {
            float amplitude;
            float composante=(float)image[i];

            if (luminosite>0.) amplitude=255-composante;
            else amplitude=composante;

            composante+=luminosite*amplitude;

            image[i]=(uint8_t)(composante);
        }
    }

    //-----------------------------------------
    //  Réglage de la saturation.
    //  la valeur est comprise entre -1 et 1.
    //-----------------------------------------
    void Image::determine_saturation(float saturation)
    {
        uint32_t i;
        if (saturation==0.)return;
        else if (saturation<-1.) saturation=-1.;
        else if(saturation>1.) saturation=1.;

        //Image en niveaux de gris:
        if (saturation==-1.)
        {
            for(i=0;i<largeur*hauteur;i++)
            {
                float r=(float)image[i*octets_par_pixel];
                float v=(float)image[i*octets_par_pixel+1];
                float b=(float)image[i*octets_par_pixel+2];

                float gris=r*IMAGE_FACTEUR_GRIS_R+v*IMAGE_FACTEUR_GRIS_V+b*IMAGE_FACTEUR_GRIS_B;

                image[i*octets_par_pixel]=(uint8_t)gris;
                image[i*octets_par_pixel+1]=(uint8_t)gris;
                image[i*octets_par_pixel+2]=(uint8_t)gris;
            }
        }

        else
        {
            for(i=0;i<largeur*hauteur*octets_par_pixel;i++)
            {
                float composante=(float)image[i];
                //***
                image[i]=(uint8_t)(composante);
            }
        }

    }

    //----------------------------------------
    //      Image en niveau de gris
    //----------------------------------------
    void Image::niveaux_gris()
    {
        determine_saturation(-1.);
    }

    //-----------------------------------------
    //  Décallage de teinte
    //  la valeur est comprise entre 0 et 1.
    //-----------------------------------------
    void Image::determine_decallage_teinte(float angle)
    {
        uint32_t i;
        if (angle==0.)return;
        else if (angle<0.) angle=0.;
        else if(angle>1.) angle=1.;

        for(i=0;i<largeur*hauteur*octets_par_pixel;i++)
        {
            float composante=(float)image[i];
            //***
            image[i]=(uint8_t)(composante);
        }
    }

    //-----------------------------------------
    //  Gamma
    //  la valeur est comprise entre 0 et 2.
    //-----------------------------------------
    void Image::determine_gamma(float gamma)
    {
        uint32_t i;

        if (gamma==1.)return;
        else if (gamma<0.) gamma=0.;
        else if(gamma>2.) gamma=2.;

        for(i=0;i<largeur*hauteur*octets_par_pixel;i++)
        {
            float composante=(float)image[i];
            //***
            image[i]=(uint8_t)(composante);
        }
    }

    //-----------------------------------------
    //  Négatif
    /// La couche alpha est également affectée !
    //-----------------------------------------
    void Image::negatif()
    {
        uint32_t i;

        for(i=0;i<largeur*hauteur*octets_par_pixel;i++)
        {
            int8_t composante=(int8_t)image[i];
            composante=-composante-1;
            image[i]=composante;
        }
    }

    //---------------------------------------------------------------------
    //                      Filtre de contours
    //          Utilisation des matrices de convolution de Sobel
    //          Les contours sont rendu en blanc, le font en noir.
    //---------------------------------------------------------------------
    bool Image::contours()
    {
        uint32_t i,x,y,xc,yc,offset,yc_depart,yc_fin,xc_depart,xc_fin;
        float pixel,pixelx,pixely;
        uint16_t taille_matrice=3;
        uint32_t centrage_matrice=(taille_matrice&0xfffe)>>1;
        cout<<centrage_matrice<<endl;

        //Image en niveaux de gris et convertion en RVB:
        if (octets_par_pixel>1)
        {
            if (!format_pixels_LUMINANCE())
            {
                cout<<"ERREUR dans Image::contours() - impossible de convertir l'image en 8bits/pixels"<<endl;
                return false;
            }
        }

        //Tempon de travail, représentant les niveaux des pixels:
        float* tempon=(float*)malloc(largeur*hauteur*sizeof(float));
        if (tempon==NULL)
        {
            cout<<"ERREUR dans Image:contours() - Mémoire insuffisante pour appliquer le filtre"<<endl;
            return false;
        }
        for(i=0;i<largeur*hauteur;i++) tempon[i]=(float)image[i]/255.;

        //Convolution:
        float matrice_convolution_x[3][3]={ {1.,0.,-1.}, {2.,0.,-2.} , {1.,0.,-1.} };
        float matrice_convolution_y[3][3]={ {1.,2.,1.}, {0.,0.,0.} , {-1.,-2.,-1.} };

        for (y=0;y<hauteur;y++)
        {
            if (y==0)yc_depart=1; else yc_depart=0;
            if (y==hauteur-1) yc_fin=taille_matrice-1; else yc_fin=taille_matrice;
            for (x=0;x<largeur;x++)
            {
                pixelx=0.;
                pixely=0.;
                if(x==0)xc_depart=1; else xc_depart=0;
                if (x==largeur-1) xc_fin=taille_matrice-1; else xc_fin=taille_matrice;

                for(yc=yc_depart;yc<yc_fin;yc++)
                {
                    for(xc=xc_depart;xc<xc_fin;xc++)
                    {
                        offset=((y+yc-centrage_matrice)*largeur+x+xc-centrage_matrice);
                        pixelx+=tempon[offset]*matrice_convolution_x[yc][xc];
                        pixely+=tempon[offset]*matrice_convolution_y[yc][xc];
                    }
                }
                pixel=sqrt(pixelx*pixelx+pixely*pixely);
                if (pixel>1.)pixel=1.;
                image[(y*largeur+x)]=uint8_t(pixel*255.);
                //image[(y*largeur+x)*3+1]=uint8_t(pixel[1]*255.);
                //image[(y*largeur+x)*3+2]=uint8_t(pixel[0]*255.);
            }
        }

        free(tempon);
        return true;
    }

    //---------------------------------------------------------------------
    //  Convertion en texture de normales, pour la rugosité des textures
    //  L'amplitude est le facteur de pente entre deux niveaux consécutifs.
    //---------------------------------------------------------------------
    bool Image::convertion_texture_normales(float amplitude)
    {
        uint32_t i,x,y;
        float h_pixel,h_voisin,h_niveau;
        //Image en niveaux de gris et convertion en RVB:
        if (octets_par_pixel>1) niveaux_gris();
        if (!format_pixels_RVB()) return false;

        //Tempon de travail, représentant les niveaux des pixels:
        float* tempon=(float*)malloc(largeur*hauteur*sizeof(float));
        if (tempon==NULL)
        {
            cout<<"ERREUR dans Image:convertion_texture_normales() - Mémoire insuffisante"<<endl;
            return false;
        }
        for(i=0;i<largeur*hauteur;i++) tempon[i]=float(image[3*i])/255.;

        //calcul des normales
        float normales_pixel[9][3];
        for(y=1;y<hauteur-1;y++)
        {
            for(x=1;x<largeur;x++)
            {
                h_pixel=tempon[x+y*largeur];
                h_voisin=tempon[x-1+(y-1)*largeur];
                h_niveau=(h_voisin-h_pixel)*amplitude;
                calcul_normale_pente(-1.,-1.,h_niveau,&normales_pixel[0][0]);

                h_voisin=tempon[x+(y-1)*largeur];
                h_niveau=(h_voisin-h_pixel)*amplitude;
                calcul_normale_pente(0.,-1.,h_niveau,&normales_pixel[1][0]);

                h_voisin=tempon[x+1+(y-1)*largeur];
                h_niveau=(h_voisin-h_pixel)*amplitude;
                calcul_normale_pente(1.,-1.,h_niveau,&normales_pixel[2][0]);

                h_voisin=tempon[x-1+y*largeur];
                h_niveau=(h_voisin-h_pixel)*amplitude;
                calcul_normale_pente(-1.,0.,h_niveau,&normales_pixel[3][0]);

                h_voisin=tempon[x+1+y*largeur];
                h_niveau=(h_voisin-h_pixel)*amplitude;
                calcul_normale_pente(1.,0.,h_niveau,&normales_pixel[5][0]);

                h_voisin=tempon[x-1+(y+1)*largeur];
                h_niveau=(h_voisin-h_pixel)*amplitude;
                calcul_normale_pente(-1.,1.,h_niveau,&normales_pixel[6][0]);

                h_voisin=tempon[x+(y+1)*largeur];
                h_niveau=(h_voisin-h_pixel)*amplitude;
                calcul_normale_pente(0.,1.,h_niveau,&normales_pixel[7][0]);

                h_voisin=tempon[x+1+(y+1)*largeur];
                h_niveau=(h_voisin-h_pixel)*amplitude;
                calcul_normale_pente(1.,1.,h_niveau,&normales_pixel[8][0]);

                normales_pixel[4][0]=(normales_pixel[0][0]
                                    +normales_pixel[1][0]
                                    +normales_pixel[2][0]
                                    +normales_pixel[3][0]
                                    +normales_pixel[5][0]
                                    +normales_pixel[6][0]
                                    +normales_pixel[7][0]
                                    +normales_pixel[8][0])/8.;

                normales_pixel[4][1]=(normales_pixel[0][1]
                                    +normales_pixel[1][1]
                                    +normales_pixel[2][1]
                                    +normales_pixel[3][1]
                                    +normales_pixel[5][1]
                                    +normales_pixel[6][1]
                                    +normales_pixel[7][1]
                                    +normales_pixel[8][1])/8.;

                normales_pixel[4][2]=(normales_pixel[0][2]
                                    +normales_pixel[1][2]
                                    +normales_pixel[2][2]
                                    +normales_pixel[3][2]
                                    +normales_pixel[5][2]
                                    +normales_pixel[6][2]
                                    +normales_pixel[7][2]
                                    +normales_pixel[8][2])/8.;

                image[(x+y*largeur)*3+2]=uint8_t(normales_pixel[4][2]*255.);
                image[(x+y*largeur)*3+1]=uint8_t(normales_pixel[4][1]*255.);
                image[(x+y*largeur)*3]=uint8_t(normales_pixel[4][0]*255.);
            }
        }
        free(tempon);
        return true;
    }

    void Image::calcul_normale_pente(float x,float y, float z, float* resultat)
    {
        float xn,yn,zn;
        if (z==0)
        {
            xn=x;yn=y;zn=0;
        }
        else
        {
            xn=-x*z*z;
            yn=-y*z*z;
            zn=(x*x+y*y)*z*z;
        }

        float norme=sqrtf(xn*xn+yn*yn+zn*zn);
        resultat[0]=xn/norme;
        resultat[1]=yn/norme;
        resultat[2]=zn/norme;
        //if(resultat[2]<0.)cout<<"!!!"<<endl;
        //cout<<"x:"<<resultat[0]<<" y:"<<resultat[1]<<" z:"<<resultat[2]<<endl;
    }

    uint32_t Image::renvoie_texel(float u,float v)
    {
        float xt=float(largeur)*u;
        float yt=float(hauteur)*v;
        int x0=int(xt);
        int y0=int(yt);
        uint32_t offset0=(x0+y0*largeur)*octets_par_pixel;
        uint32_t r,g,b,a;
        switch(octets_par_pixel)
        {
            case 1:
                r=*(image+offset0);
                g=r;
                b=r;
                a=0xff;
                break;

            case 3:
                r=*(image+offset0);
                g=*(image+offset0+1);
                b=*(image+offset0+2);
                a=0xff;
                break;

            case 4:
                r=*(image+offset0);
                g=*(image+offset0+1);
                b=*(image+offset0+2);
                a=*(image+offset0+3);
                break;
            default:break;
        }
        uint32_t texel=(r<<24)|(g<<16)|(b<<8)|a;
        return texel;
    }

