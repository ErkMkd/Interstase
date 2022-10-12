#include "Sprites_2d.h"

    uint32_t Sprites_2d::resolution_x=1280;
    uint32_t Sprites_2d::resolution_y=720;

    Sprites_2d::Sprites_2d (uint32_t p_nbrImages,const char** noms_fichiers)
    {

        erreur=SPR2D_OK;
        nbrImages=p_nbrImages;
        type=SPR2D_TYPE_MULTIFICHIERS;


        // Charge les images:
        for (uint32_t i=0;i<nbrImages;i++)
        {
            sprite_2d nouveau_sprite;
            nouveau_sprite.x=0.;
            nouveau_sprite.y=0.;
            nouveau_sprite.e=1.;
            nouveau_sprite.sx=1.;
            nouveau_sprite.sy=1.;
            nouveau_sprite.cx=0.;
            nouveau_sprite.cy=0.;
            nouveau_sprite.r=0.;
            nouveau_sprite.alpha=1.;
            nouveau_sprite.resoRef_x=resolution_x;
            nouveau_sprite.resoRef_y=resolution_y;
            nouveau_sprite.drapeau_stencil=false;
            nouveau_sprite.drapeau_affiche=true;
            nouveau_sprite.drapeau_proportionnel=true;
            nouveau_sprite.drapeau_centre=true;
            nouveau_sprite.image=new Image(noms_fichiers[i]);
            if (nouveau_sprite.image==0 || nouveau_sprite.image->erreur!=IMAGE_OK)
            {
                erreur=SPR2D_ERREUR_IMAGE;
                return;
            }
            bool texOk=nouveau_sprite.image->genere_texture_openGL();
            if (!texOk)
            {
                erreur=SPR2D_ERREUR_TEXTURE;
                return;
            }
            nouveau_sprite.image->renversement_horizontal();
            sprites.push_back(nouveau_sprite);
        }

    }

    // Les sprites peuvent être regroupés en une seule image:
    Sprites_2d::Sprites_2d (uint16_t p_largeur_element,uint16_t p_hauteur_element,const char* nom_fichier)
    {

        erreur=SPR2D_OK;
        image_elements=NULL;
        type=SPR2D_TYPE_TILE;
        largeur_element=p_largeur_element;
        hauteur_element=p_hauteur_element;

        //Chargement image:
        image_elements=new Image(nom_fichier);
        if (image_elements==NULL || image_elements->erreur!=IMAGE_OK)
        {
            erreur=SPR2D_ERREUR_IMAGE;
            return;
        }
        bool texOk=image_elements->genere_texture_openGL();
        if (!texOk)
        {
            erreur=SPR2D_ERREUR_TEXTURE;
            return;
        }
        image_elements->renversement_horizontal();

        //Calcul nombre d'éléments en largeur et hauteur
        int nbrLarg=(image_elements->largeur-(image_elements->largeur%largeur_element))/largeur_element;
        int nbrHaut=(image_elements->hauteur-(image_elements->hauteur%hauteur_element))/hauteur_element;
        cout<<"Nombre de sprites en largeur: "<<nbrLarg<<" - Nombre en hauteur: "<<nbrHaut<<endl;
        float largeurElementTex=float(largeur_element)/float(image_elements->largeur);
        float largeurElementSuivantTex=float(largeur_element)/float(image_elements->largeur);
        float hauteurElementTex=float(hauteur_element)/float(image_elements->hauteur);
        float hauteurElementSuivantTex=float(hauteur_element)/float(image_elements->hauteur);
        // Création des sprites:
        float tx1,ty1,tx2,ty2;
        for (uint32_t v=0;v<nbrHaut;v++)
        {
            ty1=float(v)*hauteurElementSuivantTex;
            ty2=ty1+hauteurElementTex;

            for (uint32_t u=0;u<nbrLarg;u++)
            {
                sprite_2d nouveau_sprite;
                nouveau_sprite.x=0.;
                nouveau_sprite.y=0.;
                nouveau_sprite.e=1.;
                nouveau_sprite.sx=1.;
                nouveau_sprite.sy=1.;
                nouveau_sprite.cx=0.;
                nouveau_sprite.cy=0.;
                nouveau_sprite.r=0.;
                nouveau_sprite.alpha=1.;
                nouveau_sprite.drapeau_stencil=false;
                nouveau_sprite.drapeau_affiche=true;
                nouveau_sprite.drapeau_proportionnel=true;
                nouveau_sprite.drapeau_centre=true;

                tx1=float(u)*largeurElementSuivantTex;
                tx2=tx1+largeurElementTex;

                nouveau_sprite.texCoords[0]=tx1;
                nouveau_sprite.texCoords[1]=ty2;
                nouveau_sprite.texCoords[2]=tx2;
                nouveau_sprite.texCoords[3]=ty2;
                nouveau_sprite.texCoords[4]=tx2;
                nouveau_sprite.texCoords[5]=ty1;
                nouveau_sprite.texCoords[6]=tx1;
                nouveau_sprite.texCoords[7]=ty1;

                sprites.push_back(nouveau_sprite);
            }
        }

    }

    //================= Destructeur:
    Sprites_2d::~Sprites_2d()
    {
        cout<<"DESTRUCTION SPRITES2D"<<endl;

        if(type==SPR2D_TYPE_MULTIFICHIERS)
        {
            uint32_t taille=sprites.size();
            for (uint32_t i=0;i<taille;i++)
            {
                delete(sprites[i].image);
            }
        }
        else if(type==SPR2D_TYPE_TILE)
        {
            if(image_elements!=NULL) delete image_elements;
        }

        sprites.clear();
    }



    //====================================================
    //  Positionnement des sprites
    //====================================================
    void Sprites_2d::determine_position_sprite(uint32_t indice, float x, float y)
    {
        sprites[indice].x=x;
        sprites[indice].y=y;
    }

    void Sprites_2d::determine_position_centre_sprite(uint32_t indice, float cx, float cy)
    {
        sprites[indice].cx=cx;
        sprites[indice].cy=cy;
    }

    float Sprites_2d::renvoie_sprite_x(uint32_t indice)
    {
        return sprites[indice].x;
    }

    float Sprites_2d::renvoie_sprite_y(uint32_t indice)
    {
        return sprites[indice].y;
    }

    void Sprites_2d::determine_alpha_sprite(uint32_t indice, float a)
    {
        sprites[indice].alpha=a;
    }

    float Sprites_2d::renvoie_alpha_sprite(uint32_t indice)
    {
        return sprites[indice].alpha;
    }

    void Sprites_2d::determine_rotation_sprite(uint32_t indice, float r)
    {
        sprites[indice].r=r;
    }

    void Sprites_2d::determine_resolution_reference_XY_sprite(uint32_t indice,uint32_t resRef_x,uint32_t resRef_y)
    {
        sprites[indice].resoRef_x=resRef_x;
        sprites[indice].resoRef_y=resRef_y;
    }

    void Sprites_2d::determine_resolution_reference_XY(uint32_t resRef_x,uint32_t resRef_y)
    {
        for(uint32_t i=0;i<sprites.size();i++)
        {
            sprites[i].resoRef_x=resRef_x;
            sprites[i].resoRef_y=resRef_y;
        }
    }

    void Sprites_2d::determine_centre_sprite(uint32_t indice, bool c)
    {
        sprites[indice].drapeau_centre=c;
    }

    void Sprites_2d::determine_proportionnel_sprite(uint32_t indice, bool p)
    {
        sprites[indice].drapeau_proportionnel=p;
    }

    //====================================================
    //  PAramètres des sprites:
    //====================================================

    void Sprites_2d::cache()
    {
        for(uint32_t i=0;i<sprites.size();i++)
        {
            sprites[i].drapeau_affiche=false;
        }
    }

    void Sprites_2d::montre()
    {
        for(uint32_t i=0;i<sprites.size();i++)
        {
            sprites[i].drapeau_affiche=true;
        }
    }

    void Sprites_2d::cache_sprite(uint32_t indice)
    {
            sprites[indice].drapeau_affiche=false;
    }
    void Sprites_2d::montre_sprite(uint32_t indice)
    {
            sprites[indice].drapeau_affiche=true;
    }

    void Sprites_2d::determine_resolution(uint32_t r_l, uint32_t r_h)
    {
        resolution_x=r_l;
        resolution_y=r_h;
    }

    void Sprites_2d::determine_echelle_sprite(uint32_t indice, float echelle)
    {
        sprites[indice].e=echelle;
    }

    void Sprites_2d::determine_echelle_XY_sprite(uint32_t indice, float sXY[2])
    {
        sprites[indice].sx=sXY[0];
        sprites[indice].sy=sXY[1];
    }

    void Sprites_2d::determine_echelle_X_sprite(uint32_t indice, float p_sx)
    {
        sprites[indice].sx=p_sx;
    }

    void Sprites_2d::determine_echelle_Y_sprite(uint32_t indice, float p_sy)
    {
        sprites[indice].sy=p_sy;
    }

    float Sprites_2d::renvoie_echelle_sprite(uint32_t indice)
    {
        return sprites[indice].e;
    }

    float Sprites_2d::renvoie_echelle_X_sprite(uint32_t indice)
    {
        return sprites[indice].sx;
    }

    float Sprites_2d::renvoie_echelle_Y_sprite(uint32_t indice)
    {
        return sprites[indice].sy;
    }

    //Le drapeau stencil ne peut être activé que si l'image est en mode 8 bits par pixel
    //Ne fonctionne que sur les sprites en multi images
    void Sprites_2d::determine_stencil(uint32_t indice,bool p_drapeau)
    {
      if(type==SPR2D_TYPE_TILE) return;
        if (sprites[indice].image->bits_par_pixel==8)
        {
            sprites[indice].image->format_pixels=GL_STENCIL_INDEX;
            sprites[indice].drapeau_stencil=p_drapeau;
        }


    }

    uint32_t Sprites_2d::renvoie_largeur_sprite(uint32_t indice)
    {
        if(type==SPR2D_TYPE_MULTIFICHIERS) return sprites[indice].image->largeur;
        else if(type==SPR2D_TYPE_TILE) return largeur_element;
    }

    uint32_t Sprites_2d::renvoie_hauteur_sprite(uint32_t indice)
    {
        if(type==SPR2D_TYPE_MULTIFICHIERS) return sprites[indice].image->hauteur;
        else if(type==SPR2D_TYPE_TILE) return hauteur_element;
    }

    GLint Sprites_2d::renvoie_texture_id_sprite(uint32_t indice)
    {
        if(type==SPR2D_TYPE_MULTIFICHIERS) return sprites[indice].image->texture_id;
        else if(type==SPR2D_TYPE_TILE) return image_elements->texture_id;
    }

    //====================================================
    //  Affichage des sprites via un tempon de pixels
    //====================================================

    void Sprites_2d::affiche_multi()
    {
        for(uint32_t i=0;i<sprites.size();i++)
        {
            if(sprites[i].drapeau_affiche)
            {

                Image* image=sprites[i].image;
                float sl=float(image->largeur);
                float sh=float(image->hauteur);
                float eRef_x=float(resolution_x)/float(sprites[i].resoRef_x);
                float eRef_y=float(resolution_y)/float(sprites[i].resoRef_y);
                glBindTexture(GL_TEXTURE_2D,image->texture_id);

                glPushMatrix();

                if(sprites[i].drapeau_proportionnel)glTranslatef(sprites[i].x*float(resolution_x),sprites[i].y*float(resolution_y),0.);
                else glTranslatef(sprites[i].x,sprites[i].y,0.);

                glScalef(eRef_x*sprites[i].e*sprites[i].sx,eRef_y*sprites[i].e*sprites[i].sy,1.);
                glRotatef(sprites[i].r,0.,0.,1.);
                if(sprites[i].drapeau_centre) glTranslatef(-sl/2.,-sh/2.,0.);
                else
                {
                    glTranslatef(-sl*sprites[i].cx,-sh*sprites[i].cy,0.);
                    //glTranslatef(-sl*sprites[i].cx,-sh*sprites[i].cy,0.);
                }

                glColor4f(1.,1.,1.,sprites[i].alpha);
                glBegin(GL_QUADS);
                glTexCoord2f(0.,1.);
                glVertex3f(0.,0.,-1.); //Coin inférieur gauche
                glTexCoord2f(1.,1.);
                glVertex3f(sl,0.,-1.);   //Coin inférieur droit
                glTexCoord2f(1.,0.);
                glVertex3f(sl,sh,-1.);    //Coin supérieur droit
                glTexCoord2f(0.,0.);
                glVertex3f(0,sh,-1.);       //Coin supérieur gauche
                glEnd();
                glPopMatrix();
            }
        }
    }

    void Sprites_2d::affiche_tile()
    {
        glBindTexture(GL_TEXTURE_2D,image_elements->texture_id);
        float sl=float(largeur_element);
        float sh=float(hauteur_element);


        for(uint32_t i=0;i<sprites.size();i++)
        {
            if(sprites[i].drapeau_affiche)
            {
                float eRef_x=float(resolution_x)/float(sprites[i].resoRef_x);
                float eRef_y=float(resolution_y)/float(sprites[i].resoRef_y);
                glPushMatrix();

                if(sprites[i].drapeau_proportionnel)glTranslatef(sprites[i].x*float(resolution_x),sprites[i].y*float(resolution_y),0.);
                else glTranslatef(sprites[i].x,sprites[i].y,0.);
                glScalef(eRef_x*sprites[i].e*sprites[i].sx,eRef_y*sprites[i].e*sprites[i].sy,1.);
                glRotatef(sprites[i].r,0.,0.,1.);
                if(sprites[i].drapeau_centre) glTranslatef(-sl/2.,-sh/2.,0.);
                else
                {
                    glTranslatef(-sl*sprites[i].cx,-sh*sprites[i].cy,0.);
                    //glTranslatef(-sl*sprites[i].cx,-sh*sprites[i].cy,0.);
                }

                glColor4f(1.,1.,1.,sprites[i].alpha);
                glBegin(GL_QUADS);
                glTexCoord2fv(&sprites[i].texCoords[0]);
                glVertex3f(0.,0.,-1.); //Coin inférieur gauche
                glTexCoord2fv(&sprites[i].texCoords[2]);
                glVertex3f(sl,0.,-1.);   //Coin inférieur droit
                glTexCoord2fv(&sprites[i].texCoords[4]);
                glVertex3f(sl,sh,-1.);    //Coin supérieur droit
                glTexCoord2fv(&sprites[i].texCoords[6]);
                glVertex3f(0,sh,-1.);       //Coin supérieur gauche
                glEnd();
                glPopMatrix();
            }
        }
    }

    void Sprites_2d::affiche_sprites_2d()
    {
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0., (float)resolution_x,0. ,(float)resolution_y,.5,10.);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        GLboolean blend_mem,depthWriteMask_mem;
        GLint blendSrc,blendDst;
        blend_mem=glIsEnabled(GL_BLEND);
        glGetBooleanv(GL_DEPTH_WRITEMASK,&depthWriteMask_mem);
        glGetIntegerv(GL_BLEND_SRC_ALPHA,&blendSrc);
        glGetIntegerv(GL_BLEND_DST_ALPHA,&blendDst);

        bool lighting_mem=glIsEnabled(GL_LIGHTING);
        bool texture_mem=glIsEnabled(GL_TEXTURE_2D);
        bool depth_test_mem=glIsEnabled(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        glUseProgram(0);
        glDisable(GL_DEPTH_TEST); //Désactive la prise en compte de la profondeur ("z-buffer")
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        glDepthMask(GL_FALSE);
        glDisable(GL_CULL_FACE);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK,GL_DIFFUSE);
        glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

         if(type==SPR2D_TYPE_MULTIFICHIERS) affiche_multi();
         else if(type==SPR2D_TYPE_TILE) affiche_tile();

        if (lighting_mem) glEnable(GL_LIGHTING);
        if (depth_test_mem) glEnable(GL_DEPTH_TEST);
        if (!texture_mem) glDisable(GL_TEXTURE_2D);
        glBlendFunc(blendSrc,blendDst);
        if(!blend_mem)glDisable(GL_BLEND);
        glDepthMask(depthWriteMask_mem);


        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    }
