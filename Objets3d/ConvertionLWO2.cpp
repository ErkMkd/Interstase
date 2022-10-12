

/*
*
*
*       Convertisseur des données LWO2 aux formats exploitables par le moteur
*       LES COORDONNEES EN X SONT INVERSEES !!!
*
*/
#include "ConvertionLWO2.h"


        ConvertionLWO2::ConvertionLWO2(ChargeLWO2* p_donneesLWO2)
        {
            erreur=CONVERTIONLWO2_OK;
            donneesLWO2=p_donneesLWO2;
            nbrVolumes=0;
            tableau_volumes_convertis=NULL;

            duree_totale=glfwGetTime();

            if (donneesLWO2==NULL)
            {
                cout<<"Erreur dans ConvertionLWO2() - Données LWO2 invalides"<<endl;
                erreur=CONVERTIONLWO2_ERREUR_LWO2_INVALIDE;
                return;
            }
            if (CONVERTIONLWO2_DEBUG)
            {
                cout<<endl<<"======================== Convertion des données LWO2 : "<<donneesLWO2->renvoie_nom_fichier()<<" =========================="<<endl<<endl;
            }

            //Création des volumes vides:
            if (!creation_volumes()) return;
            //Création des sommets:
            if (!creation_sommets()) return;
            //Création des surfaces:
            if (!creation_surfaces()) return;
            //Création des textures:
            if (!creation_textures()) return;
            //Création des polygones:
            if (!creation_polygones()) return;
            //Répartition des surfaces dans les volumes:
            if (!repartition_surfaces()) return;


            duree_totale=glfwGetTime()-duree_totale;

            if (CONVERTIONLWO2_DEBUG)
            {
                cout<<endl<<"Convertion réussie..."<<endl;

                if (CONVERTIONLWO2_DEBUG_DUREES) cout<<"Durée de la convertion: "<<duree_totale<<" ms"<<endl ;

                cout<<"======================== Fin de convertion des données LWO2 : "<<donneesLWO2->renvoie_nom_fichier()<<" =========================="<<endl<<endl;
            }


        }

        //====================================================
        //          Destructeur
        //====================================================
        ConvertionLWO2::~ConvertionLWO2()
        {
            uint32_t i;
            for (i=0;i<volumes.size();i++)
            {
                if (volumes[i]!=NULL) delete volumes[i];
            }

            for (i=0;i<surfaces.size();i++)
            {
                if (surfaces[i]!=NULL) delete surfaces[i];
            }

            if (tableau_volumes_convertis!=NULL) free (tableau_volumes_convertis);
        }

        //====================================================
        //          Création des volumes vides:
        //====================================================

        void ConvertionLWO2::calcul_pivot(LWO2_Calque* calque,float vPos[3])
        {
            if(calque->parent==-1) {vPos[0]=calque->pivot[0];vPos[1]=calque->pivot[1];vPos[2]=calque->pivot[2];}
            else
            {
                LWO2_Calque* calque_parent=donneesLWO2->renvoie_LWO2_calque_id(calque->parent);
                calcul_pivot(calque_parent,vPos);
                vPos[0]=calque->pivot[0]-calque_parent->pivot[0];
                vPos[1]=calque->pivot[1]-calque_parent->pivot[1];
                vPos[2]=calque->pivot[2]-calque_parent->pivot[2];
            }
        }

        bool ConvertionLWO2::creation_volumes()
        {
            if (CONVERTIONLWO2_DEBUG)
            {
                cout<<"     - - - - - Création des volumes - - - - -"<<endl;
            }
            uint32_t i;
            nbrVolumes=donneesLWO2->renvoie_nbrCalques();

            for (i=0;i<nbrVolumes;i++)
            {
                LWO2_Calque* calque=donneesLWO2->renvoie_LWO2_calque(i);
                if (calque==NULL)
                {
                    cout<<"Erreur dans ConvertionLWO2.creation_volumes() - Données LWO2 invalides"<<endl;
                    erreur=CONVERTIONLWO2_ERREUR_LWO2_INVALIDE;
                    return false;
                }

                float vPos[3];
                calcul_pivot(calque,vPos);

                Volume* nouveau_volume=new Volume(calque->nom,
                                                        VOLUME_LIBERE_SOMMETS|VOLUME_LIBERE_FACES|VOLUME_LIBERE_SURFACES,
                                                        -vPos[0],vPos[1],vPos[2]
                                                        );
                volumes.push_back(nouveau_volume);
                if (nouveau_volume==NULL || nouveau_volume->erreur!=VOLUME_OK)
                {
                    cout<<"Erreur dans ConvertionLWO2.creation_volumes() - Erreur lors de la création du volume ' "<<calque->nom<<" '"<<endl;
                    erreur=CONVERTIONLWO2_ERREUR_MEMOIRE;
                    return false;
                }

                nouveau_volume->determine_sens_polygones(GL_CW);

                if (CONVERTIONLWO2_DEBUG)
                {
                    cout<<"   --- Volume créé:"<<nouveau_volume->nom<<endl;
                }
            }

            return true;
        }

        //====================================================
        //          Création des sommets:
        //====================================================
        bool ConvertionLWO2::creation_sommets()
        {
            if (CONVERTIONLWO2_DEBUG)
            {
                cout<<"     - - - - - Création des sommets - - - - -"<<endl;
            }
            uint32_t i,j;
            for (i=0;i<nbrVolumes;i++)
            {
                LWO2_Calque* calque=donneesLWO2->renvoie_LWO2_calque(i); //La validité des calques à déjà été testée précédemment
                uint32_t nbrSommets=calque->nbrSommets;
                GLfloat* sommets=(GLfloat*)malloc(nbrSommets*(sizeof(GLfloat))*3);
                if (sommets==NULL)
                {
                    cout<<"Erreur dans ConvertionLWO2.creation_sommets() - Mémoire insuffisante"<<endl;
                    erreur=CONVERTIONLWO2_ERREUR_MEMOIRE;
                    return false;
                }
                for (j=0;j<calque->nbrSommets;j++)
                {
                    sommets[j*3]=-((GLfloat)calque->sommets[j*3]-(GLfloat)calque->pivot[0]);
                    sommets[j*3+1]=(GLfloat)calque->sommets[j*3+1]-(GLfloat)calque->pivot[1];
                    sommets[j*3+2]=((GLfloat)calque->sommets[j*3+2]-(GLfloat)calque->pivot[2]);
                }
                if (!volumes[i]->determine_sommets(sommets,nbrSommets))
                {
                    erreur=CONVERTIONLWO2_ERREUR_VOLUME;
                    return false;
                }
                if (CONVERTIONLWO2_DEBUG)
                {
                    cout<<"          Nombre de sommets volume ' "<<volumes[i]->nom<<" ' :"<<volumes[i]->nbrSommets<<endl;
                }
            }
            return true;
        }

        //====================================================
        //          Création des surfaces:
        //====================================================
        bool ConvertionLWO2::creation_surfaces()
        {
            uint32_t i,nbrSurfacesLWO2;
            Surface* nouvelle_surface;

            if (CONVERTIONLWO2_DEBUG)
            {
                cout<<"     - - - - - Création des surfaces - - - - -"<<endl;
            }

            nbrSurfacesLWO2=donneesLWO2->renvoie_nbrSurfaces();

            for (i=0;i<nbrSurfacesLWO2;i++)
            {
                LWO2_Surface* surfaceLWO2=donneesLWO2->renvoie_LWO2_surface(i);
                if (surfaceLWO2==NULL)
                {
                    cout<<"Erreur dans ConvertionLWO2.creation_surfaces() - Données LWO2 invalides"<<endl;
                    erreur=CONVERTIONLWO2_ERREUR_LWO2_INVALIDE;
                    return false;
                }

                //-----------Surface non-texturée:
                //if (surfaceLWO2->textures.size()==0)
                //{
                    nouvelle_surface=new Surface(surfaceLWO2->nom);
                    surfaces.push_back(nouvelle_surface);
                    if (nouvelle_surface==NULL || nouvelle_surface->erreur!=SURFACE_OK)
                    {
                        cout<<"Erreur dans ConvertionLWO2.creation_surfaces() - Mémoire insuffisante"<<endl;
                        erreur=CONVERTIONLWO2_ERREUR_MEMOIRE;
                        return false;
                    }

                    copie_parametres_surface(surfaceLWO2,nouvelle_surface);

                //}
                //-----------Surface texturée:
                //  Je sépare les coutures de texture, si il y en a, et je crée une nouvelle surface avec.
                //  Les faces concernées sont raccordées à cette nouvelle surface.
                //else
                //{

                //}
                if (CONVERTIONLWO2_DEBUG)
                {
                    if (CONVERTIONLWO2_DEBUG_SURFACES) nouvelle_surface->affiche_debug();
                    else cout<<"              Surface: "<<nouvelle_surface->nom<<endl;
                }

            }

            return true;
        }

        //--------------------------------------------------------------
        //  Copie les paramètres de la surface LWO2 vers la surface DKS
        //--------------------------------------------------------------
        void ConvertionLWO2::copie_parametres_surface(LWO2_Surface* surfaceLWO2, Surface* nouvelle_surface)
        {
            nouvelle_surface->double_face=surfaceLWO2->double_face;
            nouvelle_surface->drapeau_adoucissement=surfaceLWO2->adoucissement;
            nouvelle_surface->couleur_diffusion[0]=surfaceLWO2->couleur[0];
            nouvelle_surface->couleur_diffusion[1]=surfaceLWO2->couleur[1];
            nouvelle_surface->couleur_diffusion[2]=surfaceLWO2->couleur[2];
            nouvelle_surface->couleur_speculaire[0]=1.;
            nouvelle_surface->couleur_speculaire[1]=1.;
            nouvelle_surface->couleur_speculaire[2]=1.;
            nouvelle_surface->couleur_ambiance[0]=1.;
            nouvelle_surface->couleur_ambiance[1]=1.;
            nouvelle_surface->couleur_ambiance[2]=1.;
            nouvelle_surface->couleur_luminescence[0]=surfaceLWO2->couleur[0];
            nouvelle_surface->couleur_luminescence[1]=surfaceLWO2->couleur[1];
            nouvelle_surface->couleur_luminescence[2]=surfaceLWO2->couleur[2];
            nouvelle_surface->luminescence=surfaceLWO2->luminescence;
            nouvelle_surface->diffusion=surfaceLWO2->diffusion;
            nouvelle_surface->ambiance=1.;
            nouvelle_surface->specularite=surfaceLWO2->specularite;
            nouvelle_surface->brillance=surfaceLWO2->brillance*128.;
            nouvelle_surface->transparence=surfaceLWO2->transparence;
            nouvelle_surface->translucidite=surfaceLWO2->translucidite;
            nouvelle_surface->reflexion=surfaceLWO2->reflexion;
            nouvelle_surface->rugosite=surfaceLWO2->rugosite;
            nouvelle_surface->angle_limite_adoucissement=surfaceLWO2->angle_limite_adoucissement;

        }

        //===============================================================================
        //                      Création des textures:
        //===============================================================================

        bool ConvertionLWO2::creation_textures()
        {
            uint32_t i,j,k,nbrSurfacesLWO2;
            Surface* surfaceDKS;
            Texture* textureDKS;
            LWO2_Surface* surfaceLWO2;
            LWO2_Texture* textureLWO2;
            LWO2_Bitmap* bitmapLWO2;
            LWO2_ParametresSommets* sommetsUV_LWO2;
            Image* image;

            #if CONVERTIONLWO2_DEBUG
                cout<<"     - - - - - Création des textures - - - - -"<<endl;
            #endif

            //--------- Applique les filtres des clips LWO2 aux images:
            for (i=0;i<donneesLWO2->renvoie_nbrBitmaps();i++)
            {
                bitmapLWO2=donneesLWO2->renvoie_LWO2_bitmap_index(i);
                image=bitmapLWO2->image;
                image->determine_luminosite(bitmapLWO2->luminosite);
                image->determine_contraste(bitmapLWO2->contraste);
                image->determine_decallage_teinte(bitmapLWO2->decallage_teinte);
                image->determine_saturation(bitmapLWO2->saturation);
                image->determine_gamma(bitmapLWO2->gamma);
                if (bitmapLWO2->negatif) image->negatif();
            }

            //--------- Création des textures au format DKS:
            nbrSurfacesLWO2=donneesLWO2->renvoie_nbrSurfaces();

            for (i=0;i<nbrSurfacesLWO2;i++)
            {
                surfaceLWO2=donneesLWO2->renvoie_LWO2_surface(i);
                surfaceDKS=surfaces[i]; //Les tables correspondent


                #if (CONVERTIONLWO2_DEBUG && CONVERTIONLWO2_DEBUG_TEXTURES)

                    cout<<"               Surface LWO2 : "<<surfaceLWO2->nom<<endl;
                #endif


                for(j=0;j<surfaceLWO2->textures.size();j++)
                {
                    textureLWO2=surfaceLWO2->textures[j];

                    //Type de texture (image, procédurale...)
                    if (textureLWO2->type != ID_IMAP) continue;

                    //Type IMAP (image map)
                    bitmapLWO2=donneesLWO2->renvoie_LWO2_bitMap_id(textureLWO2->identite_clip);
                    uint8_t canal_texture,type_projection;

                    //Canal de la texture: COLR,DIFF,LUMI,SPEC,GLOS,REFL,TRAN,RIND,TRNL,BUMP
                    switch(textureLWO2->canal)
                    {
                        case ID_COLR:
                            canal_texture=TEXTURE_CANAL_COULEUR;
                            break;
                        case ID_DIFF:
                            canal_texture=TEXTURE_CANAL_DIFFUSION;
                            break;
                        case ID_LUMI:
                            canal_texture=TEXTURE_CANAL_LUMINESCENCE;
                            break;
                        case ID_SPEC:
                            cout<<"Avertissement - Texture de spécularité non prise en compte"<<endl;
                            continue;
                            break;
                        case ID_GLOS:
                            cout<<"Avertissement - Texture de brillance non prise en compte"<<endl;
                            continue;
                            break;
                        case ID_REFL:
                            canal_texture=TEXTURE_CANAL_REFLEXION;
                            break;
                        case ID_TRAN:
                            canal_texture=TEXTURE_CANAL_TRANSPARENCE;
                            break;
                        case ID_RIND:
                            cout<<"Avertissement - Texture de réfraction non prise en compte"<<endl;
                            continue;
                            break;
                        case ID_TRNL:
                            cout<<"Avertissement - Texture de translucidité non prise en compte"<<endl;
                            continue;
                            break;
                        case ID_BUMP:
                            canal_texture=TEXTURE_CANAL_RUGOSITE;
                            break;
                        default:
                            cout<<"Avertissement dans ConvertionLWO2::creation_textures() - canal de texture inconnu :"<<textureLWO2->canal<<endl;
                            continue;
                            break;

                    }

                    //Type de projection:

                    switch (textureLWO2->type_projection)
                    {
                        case LWO2_TYPE_PROJECTION_CUBIQUE:
                            cout<<"Avertissement - Projection cubique non prise en compte"<<endl;
                            continue;
                            break;
                        case LWO2_TYPE_PROJECTION_CYLINDRIQUE:
                            cout<<"Avertissement - Projection cylindrique non prise en compte"<<endl;
                            continue;
                            break;
                        case LWO2_TYPE_PROJECTION_FRONTALE:
                            cout<<"Avertissement - Projection frontale non prise en compte"<<endl;
                            continue;
                            break;
                        case LWO2_TYPE_PROJECTION_PLAN:
                            type_projection=TEXTURE_PROJECTION_PLAN;
                            break;
                        case LWO2_TYPE_PROJECTION_SPHERIQUE:
                            cout<<"Avertissement - Projection sphérique non prise en compte"<<endl;
                            continue;
                            break;
                        case LWO2_TYPE_PROJECTION_UV:
                            type_projection=TEXTURE_PROJECTION_UV;
                            break;
                        default:
                            cout<<"Avertissement dans ConvertionLWO2::creation_textures() - Type de projection inconnue :"<<textureLWO2->type_projection<<endl;
                            continue;
                            break;
                    }

                    //Création de la texture:
                    textureDKS=new Texture();

                    //Données diverses:
                    textureDKS->canal=canal_texture;
                    textureDKS->type_projection=type_projection;

                    switch(textureLWO2->mode_debordement_largeur)
                    {
                        case LWO2_MODE_DEBORDEMENT_BORD: textureDKS->recouvrement_s=GL_CLAMP_TO_EDGE; break;  //Voir à changer avec GL_CLAMP_TO_EDGE....
                        case LWO2_MODE_DEBORDEMENT_DECOUPAGE: textureDKS->recouvrement_s=GL_CLAMP_TO_BORDER; break;
                        case LWO2_MODE_DEBORDEMENT_REFLEXION: textureDKS->recouvrement_s=GL_MIRRORED_REPEAT; break;
                        case LWO2_MODE_DEBORDEMENT_REPETITION: textureDKS->recouvrement_s=GL_REPEAT; break;
                        default: textureDKS->recouvrement_s=GL_REPEAT;
                    }
                    switch(textureLWO2->mode_debordement_hauteur)
                    {
                        case LWO2_MODE_DEBORDEMENT_BORD: textureDKS->recouvrement_t=GL_CLAMP_TO_EDGE; break;  //Voir à changer avec GL_CLAMP_TO_EDGE....
                        case LWO2_MODE_DEBORDEMENT_DECOUPAGE: textureDKS->recouvrement_t=GL_CLAMP_TO_BORDER; break;
                        case LWO2_MODE_DEBORDEMENT_REFLEXION: textureDKS->recouvrement_t=GL_MIRRORED_REPEAT; break;
                        case LWO2_MODE_DEBORDEMENT_REPETITION: textureDKS->recouvrement_t=GL_REPEAT; break;
                        default: textureDKS->recouvrement_t=GL_REPEAT;
                    }

                    if (textureLWO2->fondu_pixels)
                    {
                        textureDKS->type_diminution=GL_LINEAR;
                        textureDKS->type_grossissement=GL_LINEAR;
                    }
                    else
                    {
                        textureDKS->type_diminution=GL_NEAREST;
                        textureDKS->type_grossissement=GL_NEAREST;
                    }
                    //Génère l'objet texture d'openGl:
                    if(bitmapLWO2!=NULL) textureDKS->determine_image(bitmapLWO2->image);
                    //Ajoute la texture à la surface:
                    surfaceDKS->ajoute_texture(textureDKS);
                    //activation de la texture:
                    if (textureLWO2->active==0) surfaceDKS->desactive_texture(textureDKS);
                    //else surfaceDKS->active_texture(textureDKS);

                    //Texture UV:
                    if (type_projection==TEXTURE_PROJECTION_UV)
                    {
                        sommetsUV_LWO2=textureLWO2->Texture_UV;
                        textureDKS->determine_nom(sommetsUV_LWO2->nom);
                        for (k=0;k<sommetsUV_LWO2->sommets_UV.size();k++)
                        {
                            LWO2_Sommet_UV* sommetUV_LWO2=&sommetsUV_LWO2->sommets_UV[k];
                            SommetUV* sommetUV_DKS=ajoute_sommet_UV(textureDKS);
                            sommetUV_DKS->indice_sommet=sommetUV_LWO2->indice_sommet;
                            sommetUV_DKS->U=sommetUV_LWO2->U;
                            sommetUV_DKS->V=sommetUV_LWO2->V;
                        }
                        textureDKS->nbrSommets=textureDKS->coordonnees_UV.size();
                    }

                    //Projection plan:
                    else if (type_projection==TEXTURE_PROJECTION_PLAN)
                    {
                        switch (textureLWO2->axe_projection)
                        {
                            case LWO2_AXIS_X:
                                textureDKS->axe_projection[0]=1.;
                                textureDKS->axe_projection[0]=0.;
                                textureDKS->axe_projection[0]=0.;
                                textureDKS->axe_projection[0]=0.;
                                break;
                            case LWO2_AXIS_Y:
                                textureDKS->axe_projection[0]=0.;
                                textureDKS->axe_projection[0]=1.;
                                textureDKS->axe_projection[0]=0.;
                                textureDKS->axe_projection[0]=0.;
                                break;
                            case LWO2_AXIS_Z:
                                textureDKS->axe_projection[0]=0.;
                                textureDKS->axe_projection[0]=0.;
                                textureDKS->axe_projection[0]=1.;
                                textureDKS->axe_projection[0]=0.;
                                break;
                        }

                        ///Récupérer les paramètres de centrage, mise à l'échelle et rotation !!!
                        if (textureLWO2->systeme_coordonnees==LWO2_REPERE_RELATIF)
                        {
                            textureDKS->mode_projection=GL_OBJECT_LINEAR;
                            textureDKS->reference_projection=GL_OBJECT_PLANE;
                        }
                        else if (textureLWO2->systeme_coordonnees==LWO2_REPERE_ABSOLU)
                        {
                            textureDKS->mode_projection=GL_EYE_LINEAR;
                            textureDKS->reference_projection=GL_EYE_PLANE;
                        }
                    }

                    //Débugage:
                    #if (CONVERTIONLWO2_DEBUG && CONVERTIONLWO2_DEBUG_TEXTURES)
                        cout<<"                     Canal : "<<(uint32_t)textureDKS->canal<<endl;
                        cout<<"                     Wrap S: "<<textureDKS->recouvrement_s<<endl;
                        cout<<"                     Wrap T: "<<textureDKS->recouvrement_t<<endl;

                        cout<<"                     Projection : ";
                        if (textureDKS->type_projection==TEXTURE_PROJECTION_UV)
                        {
                            cout<<"UV"<<endl;
                            cout<<"Nom:"<<textureDKS->nom<<endl;
                            cout<<"                     Nombre de sommets UV : "<<textureDKS->nbrSommets<<endl;
                            if (bitmapLWO2==NULL) cout<<"                           Image non définie ! "<<endl;
                            else
                            {
                                cout<<"                     Image : "<<textureDKS->image->nom_image<<endl;
                            }
                        }
                        else if (textureDKS->type_projection==TEXTURE_PROJECTION_PLAN)
                        {
                            cout<<"Plan"<<endl;
                            cout<<"                     Axe: "<<"X:"<<textureDKS->axe_projection[0]<<" Y:"<<textureDKS->axe_projection[1]<<" Z:"<<textureDKS->axe_projection[2]<<" W:"<<textureDKS->axe_projection[3]<<endl;
                            cout<<"                     Repère:"<<textureDKS->reference_projection<<endl;
                        }
                    #endif
                }
            }

            return true;
        }

        //Ajoute un sommet à une table de coordonnées UV:

        SommetUV* ConvertionLWO2::ajoute_sommet_UV (Texture* textureDKS)
        {
            SommetUV sommet;
            textureDKS->coordonnees_UV.push_back(sommet);
            return &(textureDKS->coordonnees_UV[textureDKS->coordonnees_UV.size()-1]);
        }

        //====================================================
        //          Création des polygones:
        //====================================================
        bool ConvertionLWO2::creation_polygones()
        {
            uint32_t i,j,k,nbrPolygones;

            #if CONVERTIONLWO2_DEBUG
                cout<<"     - - - - - Création des polygones - - - - -"<<endl;
            #endif

            for (i=0;i<nbrVolumes;i++)
            {
                LWO2_Calque* calque=donneesLWO2->renvoie_LWO2_calque(i);
                Volume* volume=volumes[i];
                nbrPolygones=calque->polygones.size();
                if (nbrPolygones==0) continue;
                //Allocation du tableau de pointeurs:
                #if CONVERTIONLWO2_DEBUG
                    cout<<"              Volume: "<<volume->nom<<endl;
                #endif
                Face** tableFaces=(Face**)calloc(nbrPolygones,sizeof(Face*));
                if (tableFaces==NULL)
                {
                    cout<<"Erreur dans ConvertionLWO2.creation_polygones() - Mémoire insuffisante pour allouer le tableau des faces"<<endl;
                    erreur=CONVERTIONLWO2_ERREUR_MEMOIRE;
                    return false;
                }

                //Création des faces:
                for (j=0;j<nbrPolygones;j++)
                {
                    LWO2_Polygone* faceLWO2=calque->polygones[j];
                    uint16_t nbrSommets=faceLWO2->nbrSommets;
                    uint32_t* sommets=(uint32_t*)malloc((nbrSommets+1)*sizeof(uint32_t));
                    if (sommets==NULL)
                    {
                        libere_table_faces_en_construction(tableFaces,nbrPolygones);
                        cout<<"Erreur dans ConvertionLWO2.creation_polygones() - Mémoire insuffisante pour allouer le tableau des arêtes"<<endl;
                        erreur=CONVERTIONLWO2_ERREUR_MEMOIRE;
                        return false;
                    }

                    for(k=0;k<nbrSommets;k++)
                    {
                        sommets[k]=faceLWO2->indices_sommets[k];   //Invertion de l'ordre (car lightwave est en repère indirect
                    }

                    sommets[k]=sommets[0];   //Bouclage:

                    char* nom_surface=donneesLWO2->renvoie_etiquette(faceLWO2->indice_etiquette_surface);
                    if (nom_surface==NULL)
                    {
                        libere_table_faces_en_construction(tableFaces,nbrPolygones);
                        cout<<"Erreur dans ConvertionLWO2.creation_polygones() - Pas de surface définie pour un polygone"<<endl;
                        erreur=CONVERTIONLWO2_ERREUR_LWO2_INVALIDE;
                        return false;
                    }

                    Face* face=new Face(nbrSommets,sommets,nom_surface);
                    tableFaces[j]=face;
                    if (face==NULL || face->erreur!=FACE_OK)
                    {
                        libere_table_faces_en_construction(tableFaces,nbrPolygones);
                        cout<<"Erreur dans ConvertionLWO2.creation_polygones() - Impossible de créer les faces"<<endl;
                        erreur=CONVERTIONLWO2_ERREUR_MEMOIRE;
                        return false;
                    }
                    if (CONVERTIONLWO2_DEBUG && CONVERTIONLWO2_DEBUG_FACES) cout<<"                      face:"<<j<<" Surface:"<<face->nom_surface<<endl;
                }
                if (!volume->determine_faces(tableFaces,nbrPolygones))
                {
                    erreur=CONVERTIONLWO2_ERREUR_VOLUME;
                    return false;
                }
                if (CONVERTIONLWO2_DEBUG) cout<<"                      Nombre de faces: "<<volume->nbrFaces<<endl;
            }

            return true;

        }

        //Au cas où une erreur se produit en cours de création des faces:
        void ConvertionLWO2::libere_table_faces_en_construction(Face** table,uint32_t nbrPolygones)
        {
            for(uint32_t i=0;i<nbrPolygones;i++)
            {
                if (table[i]!=NULL) delete(table[i]);
            }
            free(table);
        }

        //==============================================================
        //
        //          Répartition des surfaces dans les volumes
        //
        //          Dans un fichier LWO2, une surface peut servir
        //          à plusieurs volumes.
        //          Dans le moteur DKS, une surface ne peut
        //          être liée qu'à un seul volume. Il faut donc
        //          dupliquer certaines surfaces.
        //
        //==============================================================
        bool ConvertionLWO2::repartition_surfaces()
        {
            uint32_t i,j,k;
            vector<Surface*> surfaces_volume; //Table de stockage des surfaces liée au volume

            #if (CONVERTIONLWO2_DEBUG && CONVERTIONLWO2_DEBUG_REPARTITION_SURFACES)
                cout<<"     - - - - - Répartition des surfaces - - - - -"<<endl;
            #endif


            for (i=0;i<nbrVolumes;i++)
            {
                surfaces_volume.clear();
                Volume* volume=volumes[i];
                #if (CONVERTIONLWO2_DEBUG&& CONVERTIONLWO2_DEBUG_REPARTITION_SURFACES)
                    cout<<"        ---Volume: "<<volume->nom<<endl;
                #endif
                if (volume->nbrFaces==0) continue;
                for (j=0;j<volume->nbrFaces;j++)
                {
                    Surface* surface_testee=renvoie_surface(volume->faces[j]->nom_surface);
                    if (surface_testee==NULL)
                    {
                        cout<<"Erreur dans ConvertionLWO2.repartition_surfaces() - Surface inconnue: "<<volume->faces[j]->nom_surface<<endl;
                        #if (CONVERTIONLWO2_DEBUG&& CONVERTIONLWO2_DEBUG_REPARTITION_SURFACES)
                            cout<<"Surfaces créées:"<<endl;
                            for (i=0;i<surfaces.size();i++) cout<<surfaces[i]->nom<<endl;
                        #endif
                        erreur=CONVERTIONLWO2_ERREUR_LWO2_INVALIDE;
                        return false;
                    }
                    #if (CONVERTIONLWO2_DEBUG&& CONVERTIONLWO2_DEBUG_REPARTITION_SURFACES)
                        cout<<"              Face:"<<j<<" Surface: "<<surface_testee->nom<<endl;
                    #endif

                    for (k=0;k<surfaces_volume.size();k++)
                    {
                        if (surface_testee==surfaces_volume[k]) break;
                    }
                    if (k==surfaces_volume.size()) surfaces_volume.push_back(surface_testee);
                }
                #if (CONVERTIONLWO2_DEBUG&& CONVERTIONLWO2_DEBUG_REPARTITION_SURFACES)
                    cout<<"          Nombre de surfaces: "<<surfaces_volume.size()<<endl;
                #endif

                //Création de la table des surfaces du volume:
                Surface** table_surfaces=(Surface**)malloc(surfaces_volume.size()*sizeof(Surface*));
                if (table_surfaces==NULL)
                {
                    cout<<"Erreur dans ConvertionLWO2.repartition_surfaces() - Impossible d'allouer la table des surfaces du volume: '"<<volume->nom<<" '"<<endl;
                    erreur=CONVERTIONLWO2_ERREUR_MEMOIRE;
                    surfaces_volume.clear();
                    return false;
                }
                //Copie des surfaces:
                for (k=0;k<surfaces_volume.size();k++)
                {
                    table_surfaces[k]=new Surface(*surfaces_volume[k]);
                }

                if (!volume->determine_surfaces(table_surfaces,surfaces_volume.size()))
                {
                    cout<<"Erreur dans ConvertionLWO2.repartition_surfaces() - Impossible d'initialiser les surfaces du volume ' "<<volume->nom<<" '"<<endl;
                    erreur=CONVERTIONLWO2_ERREUR_VOLUME;
                    surfaces_volume.clear();
                    return false;
                }

                //Initialise les modes de rendu des surfaces:
                volume->initialise_modes_rendus_surfaces();
                volume->maj_listes();
            }
            return true;
        }

        //Renvoie une surface d'après son nom:
        Surface* ConvertionLWO2::renvoie_surface(char* nom_surface)
        {
            for (uint32_t i=0;i<surfaces.size();i++)
            {
                if(strcmp(surfaces[i]->nom,nom_surface)==0) return surfaces[i];
            }
            return NULL;
        }

        //====================================================
        //          Fonctions utiles:
        //====================================================

        //---------------------------
        //  Copiage d'une chaine.
        //  renvoie NULL si une erreur s'est produite.
        //---------------------------
        char* ConvertionLWO2::copie_chaine(char* chaine)
        {
            uint32_t taille=strlen(chaine)+1; //+1 pour le 0 de fin
            char* copie=(char*)malloc(taille);
            if (copie==NULL) return NULL;
            memcpy(copie,chaine,taille);
            return copie;
        }

        //===================================================================================================================
        //                                  Fonctions de renvoie de données
        //===================================================================================================================


        uint32_t ConvertionLWO2::renvoie_nbrVolumes()
        {
            return nbrVolumes;
        }

        Volume* ConvertionLWO2::renvoie_volume_index(uint32_t i)
        {
            if (i>=nbrVolumes) return NULL;
            return volumes[i];
        }

        Volume* ConvertionLWO2::renvoie_volume_nom(char* p_nom)
        {
            uint32_t i;
            for (i=0;i<nbrVolumes;i++)
            {
                if (strcmp(p_nom,volumes[i]->nom)==0) return volumes[i];
            }
            cout<<"ERREUR dans ConvertionLWO2::renvoie_volume_nom - Volume inconnu: "<<p_nom<<endl;
            return NULL;
        }

        //---------------------------------------------------------------
        //      Renvoie les volumes convertis sous forme d'un tableau
        //      Ce tableau est supprimé par la classe. Ne pas s'en occuper.
        //---------------------------------------------------------------
        Volume** ConvertionLWO2::renvoie_volumes_convertis()
        {
            if (tableau_volumes_convertis==NULL)
            {
                tableau_volumes_convertis=(Volume**)malloc(nbrVolumes*sizeof(Volume*));
                if (tableau_volumes_convertis==NULL)
                {
                    cout<<"ERREUR dans ConvertionLWO2::renvoie_volumes_convertis() - mémoire insuffisante"<<endl;
                    erreur=CONVERTIONLWO2_ERREUR_MEMOIRE;
                    return NULL;
                }
                for (uint32_t i=0;i<nbrVolumes;i++) tableau_volumes_convertis[i]=volumes[i];
            }
            return tableau_volumes_convertis;
        }

        //------------------------------------------------
        //          Renvoie la position du pivot
        //------------------------------------------------
        void ConvertionLWO2::renvoie_pivot_nom(char* nom_volume,double* pivot)
        {
            LWO2_Calque* calque=donneesLWO2->renvoie_LWO2_calque_nom(nom_volume);
            if(calque==NULL)
            {
                cout<<"************** ERREUR!!!!: Calque introuvable - "<<nom_volume<<endl;
                return;
            }
            pivot[0]=calque->pivot[0];
            pivot[1]=calque->pivot[1];
            pivot[2]=calque->pivot[2];
        }

        void ConvertionLWO2::renvoie_pivot(uint32_t indice,double pivot[3])
        {
            LWO2_Calque* calque=donneesLWO2->renvoie_LWO2_calque(indice);
            pivot[0]=calque->pivot[0];
            pivot[1]=calque->pivot[1];
            pivot[2]=calque->pivot[2];
        }



