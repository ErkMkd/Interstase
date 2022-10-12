

#include "Texture.h"
using namespace std;
//----------------------------------
// Classe de gestion des textures
//
//  Une même texture peut servir à plusieurs surfaces.
///  Le tableau des coordonnées UV en entrée est recopié.
///  Le programme appelant peut donc le supprimer après construction de la texture.
//
//
//----------------------------------

    //Constructeur:
    Texture::Texture(char* p_nom,
               Image* p_image,
                uint8_t p_canal,
                float* p_uvs, uint32_t* p_indices_sommets,
                uint16_t p_nbrSommets,
                bool p_mipmap,
                GLint p_recouvrement_s,
                GLint p_recouvrement_t,
                GLint p_type_grossissement,
                GLint p_type_diminution
                )
    {

        erreur=TEXTURE_OK;
        drapeau_mipmap=p_mipmap;
        canal=p_canal;
        nbrSommets=0;
        coordonnees_UV_tableau=NULL;
        image=p_image;
        tangentes_sommets=NULL;
        binormales_sommets=NULL;
        tangentes_faces=NULL;
        binormales_faces=NULL;
        faces=NULL;
        nbrFaces=0;
        luminescence=0.;

        //Gestion du nom:
        determine_nom(p_nom);
        //Par défaut, la texture est définie par des coordonnées UV:
        type_enveloppe=GL_MODULATE;
        type_projection=TEXTURE_PROJECTION_UV;
        mode_projection=GL_OBJECT_LINEAR;
        reference_projection=GL_OBJECT_PLANE;
        axe_projection[0]=0.;
        axe_projection[1]=0.;
        axe_projection[2]=1.;
        axe_projection[3]=0.;

        recouvrement_s=p_recouvrement_s;
        recouvrement_t=p_recouvrement_t;
        type_grossissement=p_type_grossissement;
        type_diminution=p_type_diminution;

        if (image!=NULL)
        {
            initialise_image();
            image->genere_texture_openGL(drapeau_mipmap,recouvrement_s,recouvrement_t,type_grossissement,type_diminution);
        }
        if (p_nbrSommets>0 && p_indices_sommets!=NULL && p_uvs!=NULL) determine_UV(p_uvs,p_indices_sommets,p_nbrSommets);
    }

    //------------------Constructeur de copie:
    //Les données initialisées par le volume ne sont
    // pas dupliquées. Elles doivent être à nouveau initialisées par le
    // le volume utilisateur.

    Texture::Texture(Texture& texture_originale)
    {
        uint32_t i;
        erreur=texture_originale.erreur;
        canal=texture_originale.canal;
        nbrSommets=texture_originale.nbrSommets;
        coordonnees_UV_tableau=NULL;
        image=texture_originale.image;
        tangentes_sommets=NULL;
        binormales_sommets=NULL;
        tangentes_faces=NULL;
        binormales_faces=NULL;
        faces=NULL;
        nbrFaces=0;
        determine_nom(texture_originale.nom);      ///Le nom est dupliqué sans modification !!!

        type_enveloppe=texture_originale.type_enveloppe;
        type_projection=texture_originale.type_projection;
        mode_projection=texture_originale.mode_projection;
        reference_projection=texture_originale.reference_projection;
        axe_projection[0]=texture_originale.axe_projection[0];
        axe_projection[1]=texture_originale.axe_projection[1];
        axe_projection[2]=texture_originale.axe_projection[2];
        axe_projection[3]=texture_originale.axe_projection[3];
        recouvrement_s=texture_originale.recouvrement_s;
        recouvrement_t=texture_originale.recouvrement_t;
        type_diminution=texture_originale.type_diminution;
        type_grossissement=texture_originale.type_grossissement;

        for (i=0;i<nbrSommets;i++)
        {
            SommetUV* sommetUV=&texture_originale.coordonnees_UV[i];
            SommetUV* nouveau_sommet=ajoute_sommet_UV();
            nouveau_sommet->indice_sommet=sommetUV->indice_sommet;
            nouveau_sommet->U=sommetUV->U;
            nouveau_sommet->V=sommetUV->V;
        }

    }

    //==================================================
    //                  Destructeur
    ///       L'image n'est pas supprimée.
    //==================================================
    Texture::~Texture()
    {
        if (coordonnees_UV_tableau!=NULL) free(coordonnees_UV_tableau);
        if(tangentes_faces!=NULL) free(tangentes_faces);
        if(binormales_faces!=NULL) free(binormales_faces);
        if(tangentes_sommets!=NULL) free(tangentes_sommets);
        if(binormales_sommets!=NULL) free(binormales_sommets);
        if (faces!=NULL) free(faces);
    }

    //============================================================================
    //                  Gestion de la texture
    //============================================================================
    void Texture::determine_nom(char* p_nom)
    {
        uint32_t i,taille;
        if (p_nom==NULL) nom[0]=0;
            else
            {
                taille=strlen(p_nom);
                if (taille>256-1)taille=256-1;
                for(i=0;i<taille;i++)
                {
                    nom[i]=p_nom[i];
                }
                nom[i]=0;
            }

    }

    void Texture::determine_image(Image* p_image)
    {
        image=p_image;
        if (image!=NULL)
        {
            initialise_image();
            image->genere_texture_openGL(drapeau_mipmap,recouvrement_s,recouvrement_t,type_grossissement,type_diminution);
        }
    }

    //Selon le canal de la texture, le mode OpenGl de l'image peut avoir
    //besoin d'une "mise aux normes"
    void Texture::initialise_image()
    {
        switch (canal)
        {
            case TEXTURE_CANAL_COULEUR:
                if (image->bits_par_pixel==8) image->format_pixels=GL_LUMINANCE;
            break;
        }
    }

    //Détermine les coordonnées UV:
    void Texture::determine_UV(float* uvs, uint32_t* indices, uint16_t p_nbrSommets)
    {
        uint16_t i;
        coordonnees_UV.clear();
        for (i=0;i<p_nbrSommets;i++)
        {
            SommetUV* nouveau_UV=ajoute_sommet_UV();
            nouveau_UV->indice_sommet=indices[i];
            nouveau_UV->U=uvs[2*i];
            nouveau_UV->V=uvs[2*i+1];
        }
        nbrSommets=p_nbrSommets;
    }

    //Cette fonction sert uniquement au constructeur de copie

    SommetUV* Texture::ajoute_sommet_UV()
    {
        SommetUV nouveau_sommet;
        coordonnees_UV.push_back(nouveau_sommet);
        return &(coordonnees_UV[coordonnees_UV.size()-1]);
    }



