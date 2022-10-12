#include "TexteMap.h"

    TexteMap::TexteMap(const char* fichier_image,uint32_t p_largeur_ecran,uint32_t p_hauteur_ecran,float p_base,float p_echelle,float p_espace_lettres,float p_espace,float p_espace_lignes)
    {
        erreur=TEXTEMAP_OK;
        largeur_ecran=p_largeur_ecran;
        hauteur_ecran=p_hauteur_ecran;
        mode_rendu=TEXTEMAP_RENDU_2D;
        texte=NULL;
        couleur=0xffffff;
        alpha=1.;
        couleur_fond[0]=0.;
        couleur_fond[1]=0.;
        couleur_fond[2]=0.;
        type_grossissement=GL_LINEAR;
        type_diminution=GL_LINEAR;
        nbr_lignes=0;
        pointeur_ligne=0;
        nbr_lignes_max=1;
        taille_texte=0;
        tailles_lignes=NULL;
        indices_lignes=NULL;
        centrage=false;

        police=new Image(fichier_image);
        if (police==NULL || police->erreur!=IMAGE_OK) { erreur=TEXTEMAP_ERREUR_IMAGE; return; }
        if (!police->genere_texture_openGL(true)){ erreur=TEXTEMAP_ERREUR_IMAGE; return; };

        largeur_caractere=police->largeur/16;
        echelle=1.;

        initialise_coordonnees_lettres();
        initialise_espaces();
        curseur_x=0.;
        curseur_y=0.;

        scroll_ymax=1.;
        scroll_ymin=0.;

        determine_echelle(p_echelle);
        determine_espace_lettres(p_espace_lettres);
        determine_espace_lignes(p_espace_lignes);
        determine_espace(p_espace);
        base=p_base;

        t_prec=-1.;
    }

    TexteMap::~TexteMap()
    {
        if (police!=NULL) delete police;
        if (texte!=NULL) free(texte);
        if (tailles_lignes!=NULL) free(tailles_lignes);
        if (indices_lignes!=NULL) free(indices_lignes);
    }

    //====================================================================
    //              Désactive toutes les textures.
    //====================================================================
    void TexteMap::desactive_textures()
        {
            uint16_t i;
            GLint nbr_unites_textures_max;
            glGetIntegerv(GL_MAX_TEXTURE_UNITS,&nbr_unites_textures_max);
            for (i=0;i<nbr_unites_textures_max;i++)
                {
                    glActiveTexture(GL_TEXTURE0+i);
                    glDisable(GL_TEXTURE_2D);
                    glClientActiveTexture(GL_TEXTURE0+i);
                    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                }
                glActiveTexture(GL_TEXTURE0);
		}
    //====================================================================
    //              Initialisations
    //====================================================================
    void TexteMap::initialise_scrollText_vertical(float p_ymin,float p_ymax,float p_vitesse)
    {
        vitesse_verticale=p_vitesse;
        scroll_ymin=p_ymin;
        scroll_ymax=p_ymax;
    }

    void TexteMap::determine_vitesse_scroll_vertical(float v)
    {
        vitesse_verticale=v;
    }

    void TexteMap::initialise_coordonnees_lettres()
    {
        uint16_t x,y;

        //Coordonnées du quadrilatère:
        rectangle[0]=0.;
        rectangle[1]=0.;
        rectangle[2]=0.;

        rectangle[3]=1.;
        rectangle[4]=0.;
        rectangle[5]=0.;

        rectangle[6]=1.;
        rectangle[7]=1.;
        rectangle[8]=0.;

        rectangle[9]=0.;
        rectangle[10]=1.;
        rectangle[11]=0.;

        normale[0]=0.;
        normale[1]=0.;
        normale[2]=1.;

        //Coordonnées de texture:
        for(y=0;y<16;y++)
        {
            for (x=0;x<16;x++)
            {
                coordonnees_lettres[(x+y*16)*8]=x/16.;
                coordonnees_lettres[(x+y*16)*8+1]=y/16.;
                coordonnees_lettres[(x+y*16)*8+2]=(x+1)/16.;
                coordonnees_lettres[(x+y*16)*8+3]=y/16.;
                coordonnees_lettres[(x+y*16)*8+4]=(x+1)/16.;
                coordonnees_lettres[(x+y*16)*8+5]=(y+1)/16.;
                coordonnees_lettres[(x+y*16)*8+6]=x/16.;
                coordonnees_lettres[(x+y*16)*8+7]=(y+1)/16.;
            }
        }
    }

    void TexteMap::initialise_espaces()
    {
        uint8_t* pixels=police->image;
        uint32_t x,y,px,py,pos_image_x,pos_image_y;
        uint32_t couleur;

        for(y=0;y<16;y++)
        {
            for (x=0;x<16;x++)
            {
                pos_image_x=x*largeur_caractere;
                pos_image_y=y*largeur_caractere;
                espaces_lettres[x+y*16]=1.;
                for (px=pos_image_x+largeur_caractere-1;px>pos_image_x;px--)
                {
                    bool colonne_vide=true;
                    for (py=pos_image_y;py<pos_image_y+largeur_caractere;py++)
                    {

                        couleur=*((uint32_t*)(pixels+(px+py*largeur_caractere*16)*4));
                        //cout<<hex<<couleur<<"  ";
                        if (couleur!=0x00000000) {colonne_vide=false; break;}
                    }
                    //cout<<endl;
                    if (colonne_vide)espaces_lettres[x+y*16]-=1./largeur_caractere;

                }
            }
        }
    }

    //-------------------------------
    //Détermine le texte à afficher:
    //-------------------------------

    //--------- Détermine le texte à partir d'un fichier:
    bool TexteMap::determine_texte_fichier(char* p_nom_fichier)
    {
        if (texte!=NULL)free(texte);
        if(tailles_lignes!=NULL) free(tailles_lignes);
        if(indices_lignes!=NULL) free(indices_lignes);

        taille_texte=Fichiers::renvoie_taille_fichier(p_nom_fichier);;
        if (taille_texte==0)
        {
            cout<<"ERREUR dans TexteMap::determine_texte_fichier() - Impossible de déterminer la taille du fichier "<<p_nom_fichier<<endl;
            erreur=TEXTEMAP_ERREUR_FICHIER;
            return false;
        }

        texte=Fichiers::charge_donnees_binaires_allocation(p_nom_fichier);
        if (texte==NULL)
        {
            cout<<"ERREUR dans TexteMap::determine_texte_fichier() - Impossible de charger le fichier: "<<p_nom_fichier<<endl;
            erreur=TEXTEMAP_ERREUR_FICHIER;
            return false;
        }

        texte[taille_texte]=0; //Au cas où...
        return initialise_texte();
    }

    //------------- Détermine le texte à partir d'une chaine:
    bool TexteMap::determine_texte(char* p_texte)
    {
        if (texte!=NULL)free(texte);
        if(tailles_lignes!=NULL) free(tailles_lignes);
        if(indices_lignes!=NULL) free(indices_lignes);
        taille_texte=strlen(p_texte);
        texte=(char*)malloc(taille_texte+1);
        if (texte==NULL)
        {
            cout<<"ERREUR dans TexteMap::determine_texte() - Impossible d'allouer la chaîne de caractères: "<<p_texte<<endl;
            erreur=TEXTEMAP_ERREUR_MEMOIRE;
            return false;
        }
        memcpy(texte,p_texte,taille_texte);
        texte[taille_texte]=0;

        /*
        cout<<"Codes texte:"<<endl;
        for(uint32_t i=0;i<taille_texte;i++)
        {
            cout<<uint16_t(texte[i])<<" ";
        }
        cout<<endl;
        */
        //------- Compte le nombre de lignes:
        return initialise_texte();
    }

    //------------------ Initialise les données pour l'affichage:

    bool TexteMap::initialise_texte()
    {

        uint32_t i,j;
        nbr_lignes=1;
        pointeur_ligne=0;
        for(i=0;i<taille_texte;i++) if(texte[i]==10)nbr_lignes++;
        nbr_lignes_max=nbr_lignes;          ///Valeur par défaut, à changer après si besoin...

        //---------- Allocation du tableau des tailles et indices de lignes:
        tailles_lignes=(uint16_t*)malloc(nbr_lignes*sizeof(uint16_t));
        indices_lignes=(uint32_t*)malloc(nbr_lignes*sizeof(uint32_t));
        if (tailles_lignes==NULL || indices_lignes==NULL)
        {
            cout<<"ERREUR dans TexteMap::initialise_texte() - Impossible d'allouer les tables d'informations des lignes (tailles,indices)"<<endl;
            erreur=TEXTEMAP_ERREUR_MEMOIRE;
            return false;
        }
        //--------- Calcul les tailles des lignes:
        j=0;
        tailles_lignes[j]=0;
        indices_lignes[j]=0;
        for(i=0;i<taille_texte;i++)
        {
            tailles_lignes[j]++;    //!!!le retour est compté !!!
            if(texte[i]==10)
            {
                j++;
                tailles_lignes[j]=0;
                indices_lignes[j]=i+1;
            }

        }
        return true;
    }

    //----------------------------------------
    //Détermine les dimensions de l'écran
    //----------------------------------------
    void TexteMap::determine_resolution_ecran(uint32_t pl,uint32_t ph)
    {
        largeur_ecran=pl;
        hauteur_ecran=ph;
    }

    //-------------------------------
    //Détermine la position du curseur
    //-------------------------------
    void TexteMap::determine_position_curseur(float px,float py)
    {
        curseur_x=px;
        curseur_y=py;
    }

    //-----------------------------------------------------
    //      Espacement entre les lettres,
    //      1.=largeur du rectangle contenant les lettres.
    //-----------------------------------------------------
    void TexteMap::determine_espace_lettres(float p_espace)
    {
        espace_lettres=p_espace;
    }

    //-----------------------------------------------------
    //      Espacement entre les lignes,
    //      1.=hauteur du rectangle contenant les lettres.
    //-----------------------------------------------------
    void TexteMap::determine_espace_lignes(float p_espace)
    {
        espace_lignes=p_espace;
    }

    //-----------------------------------------------------
    //     Taille du caractère ESPACE
    //     1.=largeur du carré contenant les lettres.
    //-----------------------------------------------------
    void TexteMap::determine_espace(float p_espace)
    {
        espace=p_espace;
    }

    //=======================================================================
    //                          Determine/Renvoie
    //=======================================================================
    void TexteMap::determine_echelle(float e)
    {
        echelle=e;
    }

    uint32_t TexteMap::renvoie_largeur_caracteres()
    {
        return largeur_caractere;
    }
    //=======================================================================
    //                          Affichages
    //=======================================================================
    void TexteMap::begin_texte()
    {
        eclairages_mem=glIsEnabled(GL_LIGHTING);
        glGetBooleanv(GL_DEPTH_WRITEMASK,&mask_mem);
        color_material_mem=glIsEnabled(GL_COLOR_MATERIAL);
        blend_mem=glIsEnabled(GL_BLEND);
        depthTest_mem=glIsEnabled(GL_DEPTH_TEST);
        glGetIntegerv(GL_BLEND_SRC_ALPHA,&blendSrc);
        glGetIntegerv(GL_BLEND_DST_ALPHA,&blendDst);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);

        culling_mem=glIsEnabled(GL_CULL_FACE);
        glDisable(GL_CULL_FACE);
        glGetIntegerv(GL_FRONT_FACE,&frontFace_mem);
        glFrontFace(GL_CCW);

        glDisable(GL_COLOR_MATERIAL);

        float r=float((couleur>>16)&0xff)/255.;
        float v=float((couleur>>8)&0xff)/255.;
        float b=float(couleur&0xff)/255.;
        glColor4f(r,v,b,alpha);
        desactive_textures();
        glUseProgram(0);

        glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,police->texture_id);
        glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,couleur_fond);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,type_grossissement);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,type_diminution);
        glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

    }

    void TexteMap::begin_texte_2d()
    {
        float rapport_ecran=(float)hauteur_ecran/float(largeur_ecran);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0.,1.,0.,rapport_ecran);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(curseur_x,curseur_y*rapport_ecran,0.);
    }

    void TexteMap::end_texte()
    {
        if (color_material_mem) glEnable(GL_COLOR_MATERIAL);
        glDisable(GL_TEXTURE_2D);
        if(!blend_mem)glDisable(GL_BLEND);
        glDepthMask(mask_mem);
        glBlendFunc(blendSrc,blendDst);
        if (eclairages_mem) glEnable(GL_LIGHTING);
        if(depthTest_mem)glEnable(GL_DEPTH_TEST);
        if(!culling_mem)glDisable(GL_CULL_FACE);
        glFrontFace(frontFace_mem);
    }

    void TexteMap::end_texte_2d()
    {
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    }


    void TexteMap::affiche()
    {
        begin_texte();

            if (mode_rendu==TEXTEMAP_RENDU_2D) affiche_2d();
            else if (mode_rendu==TEXTEMAP_RENDU_3D) affiche_3d();

        end_texte();

    }

    //---------------------------------------------------
    //  Affiche les lettres de face, sans perspective
    //---------------------------------------------------



    void TexteMap::affiche_2d()
    {
        begin_texte_2d();

            affiche_texte(texte);

        end_texte_2d();
    }

    //--------------------------------------------------------------------------
    //  Affiche les lettres dans l'espace.
    //  Le texte est géré comme un affichage transparent (Z-buffer désactivé)
    //  donc pour les recouvrements il faut faire attention à l'ordre d'affichage.
    //--------------------------------------------------------------------------
    void TexteMap::affiche_3d()
    {
        affiche_texte(texte);
    }


    //-------------------------------------
    //  Affichage de base:
    //-------------------------------------

    void TexteMap::affiche_texte(char* texte)
    {
        uint32_t t,i,c,j;
        float largeur_texte;
        float position_depart_ligne;

        glPushMatrix();
        glScalef(echelle,echelle,1.);
        uint16_t ligne_actuelle=pointeur_ligne;

        //Lecture des lignes:
        for(t=0;t<nbr_lignes_max;t++)
        {
            if(ligne_actuelle>=nbr_lignes) break;
            largeur_texte=0.;
            position_depart_ligne=0.;
            j=indices_lignes[ligne_actuelle];
            //Centrage de la ligne:
            if (centrage)
            {
                for (i=0;i<tailles_lignes[ligne_actuelle];i++)
                {
                    c=uint32_t(texte[j+i]);
                    if (c==' ') largeur_texte+=espace;
                    else largeur_texte+=espaces_lettres[c]+espace_lettres;
                }
                position_depart_ligne=-largeur_texte/2.;
            }

            //Affiche la phrase:
            glPushMatrix();
            glTranslatef(position_depart_ligne,-base,0.);
            for (i=0;i<tailles_lignes[ligne_actuelle];i++)
            {
                c=uint32_t(texte[j+i]);
                if (c==' ')
                {
                    glTranslatef(espace,0.,0.);
                }
                else
                {
                    glBegin(GL_QUADS);
                    glNormal3fv(normale);
                    //cout<<"Point "<<i<<" "<<rectangle[0]<<" "<<rectangle[1]<<" "<<rectangle[2]<<endl;
                    glTexCoord2fv(&coordonnees_lettres[c*8+6]);
                    glVertex3fv(&rectangle[0]);
                    glTexCoord2fv(&coordonnees_lettres[c*8+4]);
                    glVertex3fv(&rectangle[3]);
                    glTexCoord2fv(&coordonnees_lettres[c*8+2]);
                    glVertex3fv(&rectangle[6]);
                    glTexCoord2fv(&coordonnees_lettres[c*8+0]);
                    glVertex3fv(&rectangle[9]);
                    glEnd();
                    glTranslatef(espaces_lettres[c]+espace_lettres,0.,0.);
                }
            }
            glPopMatrix();
            glTranslatef(0.,-1.-espace_lignes,0.);   //Ligne suivante
            ligne_actuelle++;
        }

        glPopMatrix();
    }

    //------------------------------------------------
    //  Affichage 2d d'une chaine dynamique:
    //------------------------------------------------
    void TexteMap::affiche_chaine_2d(char* chaine)
    {
        begin_texte();
        begin_texte_2d();

        uint32_t cptr_ligne,i,c;

        uint32_t taille_chaine=strlen(chaine);
        uint32_t nbr_lignes_chaine=1;
        for(i=0;i<taille_chaine;i++) if(chaine[i]==10)nbr_lignes_chaine++;

        glPushMatrix();
        glScalef(echelle,echelle,1.);
        i=0;
        //Lecture des lignes:
        for(cptr_ligne=0;cptr_ligne<nbr_lignes_chaine;cptr_ligne++)
        {
            //Affiche la phrase:
            glPushMatrix();
            glTranslatef(0.,float(cptr_ligne)*-espace_lignes,0.);
            do
            {
                c=uint32_t(chaine[i++]);
                if (c==' ')
                {
                    glTranslatef(espace,0.,0.);
                }

                else if(c!=10 && c!=0)
                {
                    glBegin(GL_QUADS);
                    glNormal3fv(normale);
                    //cout<<"Point "<<i<<" "<<rectangle[0]<<" "<<rectangle[1]<<" "<<rectangle[2]<<endl;
                    glTexCoord2fv(&coordonnees_lettres[c*8+6]);
                    glVertex3fv(&rectangle[0]);
                    glTexCoord2fv(&coordonnees_lettres[c*8+4]);
                    glVertex3fv(&rectangle[3]);
                    glTexCoord2fv(&coordonnees_lettres[c*8+2]);
                    glVertex3fv(&rectangle[6]);
                    glTexCoord2fv(&coordonnees_lettres[c*8+0]);
                    glVertex3fv(&rectangle[9]);
                    glEnd();
                    glTranslatef(espaces_lettres[c]+espace_lettres,0.,0.);
                }
            } while (c!=0 && c!=10);
            glPopMatrix();
        }

        glPopMatrix();


        end_texte_2d();
        end_texte();
    }

    //-------------------------------------------------------
    //      Gestion du scrolling vertical
    //-------------------------------------------------------

    void TexteMap::maj_scrolling_vertical(float t)
    {
        if(t_prec==-1.){t_prec=t;return;} //Initialisation du delta t

        float rapport_ecran=(float)hauteur_ecran/largeur_ecran;
        float posy=vitesse_verticale*(t-t_prec);

        curseur_y+=posy;
        float hauteur_lettres=float(police->hauteur)/16./float(hauteur_ecran);
        float hauteur_ligne=espace_lignes+hauteur_lettres;

        if(curseur_y>= scroll_ymax + hauteur_ligne*float(nbr_lignes))curseur_y=scroll_ymin;

        t_prec=t;

        /*
        if(vitesse_verticale>0.)
        {
            if(curseur_y+posy>scroll_ymax)
            {
                pointeur_ligne++;
                posy-=(echelle*(1.+espace_lignes))/rapport_ecran;
            }
            if (pointeur_ligne>=nbr_lignes)
            {
                curseur_y=scroll_ymin;
                pointeur_ligne=0;
            }
        }

        else
        {
            if(curseur_y+posy<scroll_ymin)
            {
                pointeur_ligne--;
                curseur_y+=(echelle*(1.+espace_lignes))/rapport_ecran;
            }
        }
        */
    }
