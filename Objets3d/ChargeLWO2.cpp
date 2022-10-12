/*
=====================================================================================================================

                    Chargeur de fichier LWO2 - V2 - Mars 2011

=====================================================================================================================
*/

/*
*   Chargement des textures:
*   ------------------------
*
*   Les répertoires des images utilisées pour les textures sont redirigés vers "chemin_images"
*   Si ce chemin n'est pas défini (NULL), le chemin figurant dans le fichier LWO2 est utilisé.
*
*/

#include "ChargeLWO2.h"

//#include "chargeurs_images\Image.cpp"

using namespace std;


//---------------------------------------------------------------
//      Définition des bitmaps (blocs CLIP)
//      Les plugins et les filtres ne sont pas pris en compte
//      Les animations ne sont pas prises en compte.
//      Seulement les images fixes au format TIFF (CLIP STIL)
//---------------------------------------------------------------

LWO2_Bitmap::LWO2_Bitmap()
{
    fichier=NULL;
    image=NULL;
    contraste=0.;
    luminosite=0.;
    saturation=0.;
    decallage_teinte=0.;
    gamma=1.;
    negatif=false;

}

LWO2_Bitmap::~LWO2_Bitmap()
{
    if (CHARGELWO2_DEBUG) cout<<"DESTRUCTION DU BITMAP ' "<<fichier<<" '"<<endl;
    if (fichier!=NULL)free(fichier);
    if (image!=NULL) delete image;
}



//---------------------------------------------------------------
//      Définition des polygones
//      Seuls les polygones de type "FACE" sont pris en compte.
//---------------------------------------------------------------

LWO2_Polygone::LWO2_Polygone()
{
    indices_sommets=NULL;
}
LWO2_Polygone::~LWO2_Polygone()
{
    if (indices_sommets!=NULL) free(indices_sommets);
}


//---------------------------------------------------------------
//Bloc VMAP= Vertex Mapping, en français: paramètres de sommet
//  Seuls les types TXUV sont pris en compte
//---------------------------------------------------------------


LWO2_ParametresSommets::LWO2_ParametresSommets()
{
    nom=NULL;
}

LWO2_ParametresSommets::~LWO2_ParametresSommets()
{
   if (nom!=NULL)free(nom);
}


//---------------------------------------------------------------
//      Définition des textures de surface. (Surface bloc
//      Seules les textures UV et les projections planes sont implémentées pour le moment.
//      Le fondu d'effet de texture (FALL) est ignoré.
//---------------------------------------------------------------

LWO2_Texture::LWO2_Texture()
{
    chaine_tri=NULL;
    objet_de_reference=NULL;
    active=true;
    fondu_pixels=true;
    systeme_coordonnees=LWO2_REPERE_RELATIF;
    centre[0]=0.;
    centre[1]=0.;
    centre[2]=0.;
    echelle[0]=1.;
    echelle[1]=1.;
    echelle[2]=1.;
    rotation[0]=0.;
    rotation[1]=0.;
    rotation[2]=0.;
}

LWO2_Texture::~LWO2_Texture()
{
    if (chaine_tri!=NULL) free(chaine_tri);
    if (objet_de_reference!=NULL) free(objet_de_reference);
}

//-------------------------------
//  Surfaces
//  Les enveloppes ne sont pas récupérées.
//--------------------------------

LWO2_Surface::LWO2_Surface()
{
    nom=NULL;
    //Valeurs par défaut:
    diffusion=1.0;
    luminescence=0.;
    specularite=0.;
    reflexion=0.;
    transparence=0.;
    teinte_lumiere=0.;
    terminateur=0.5;
    rugosite=1.0;
    brillance=0.4;  //Valeur par défaut donnée par la documentation du format LWO2
    double_face=false;
    adoucissement=false;
    halo=false;
}

LWO2_Surface::~LWO2_Surface()
{
    if (nom!=NULL)free(nom);
    for(uint32_t i=0;i<textures.size();i++)
    {
        delete textures[i];
    }
}

//----------------------------------------
//          Calques
//----------------------------------------
LWO2_Calque::LWO2_Calque()
{
    nom=NULL;
    sommets=NULL;
    parent=-1;
}

LWO2_Calque::~LWO2_Calque()
{
    if (nom!=NULL)free(nom);

    if (sommets!=NULL)free(sommets);

    uint32_t i;
    for(i=0 ;i<parametres_sommets.size();i++)
    {
        delete parametres_sommets[i];
    }

    for(i=0 ;i<polygones.size();i++)
    {
        delete polygones[i];
    }
}

uint32_t LWO2_Calque::renvoie_nbr_polygones()
{
    return (uint32_t)polygones.size();
}

LWO2_Polygone* LWO2_Calque::renvoie_polygone(uint32_t indice)
{
    if (indice>=polygones.size()) return NULL;
    return polygones[indice];
}


//------------Définition des blocs, avec respect des hiérarchies: (C'est asser complexe, voir la doc du format LWO2)


//************************************************************************************************
//************************************************************************************************
//                              DEFINITION DE LA CLASSE
//************************************************************************************************
//************************************************************************************************



    //=================================================================
    //                  Constructeur
    //=================================================================
    //Entrées:  nom_fichier: le nom du fichier
    //          chemin_images: le répertoire où se trouvent les images utilisées pour les textures.
    ChargeLWO2::ChargeLWO2(const char* nom_fichier,const char* p_chemin_images)
    {
        taille_fichier=0;
        nbrCalques=0;
        nbrSurfaces=0;
        duree_chargement=0.;

        chemin_fichier=nom_fichier;
        chemin_images=p_chemin_images;

        erreur=CHARGELWO2_OK;

        duree_chargement=glfwGetTime();

        analyse(); //Traduction du verbe anglais "to parse"

        duree_chargement=glfwGetTime()-duree_chargement;

        if (CHARGELWO2_DEBUG && CHARGELWO2_DEBUG_DUREES) cout<<"=======>Durée du chargement de l'objet "<<chemin_fichier<<" : "<<duree_chargement<<" s"<<endl;
    }

    //============= Destructeur:
    ChargeLWO2::~ChargeLWO2()
    {
        uint32_t i;
        if (donnees_LWO2!=NULL) free(donnees_LWO2);
        //Destruction des bitmaps(CLIP):
        for (i=0;i< Bitmaps.size();i++)
        {
            delete Bitmaps[i];
        }
        for (i=0;i< calques_lwo2.size();i++)
        {
            delete calques_lwo2[i];
        }
        for (i=0;i<table_etiquettes.size();i++)
        {
            free(table_etiquettes[i]);
        }
        for (i=0;i<table_parametres_sommets.size();i++)
        {
            delete table_parametres_sommets[i];
        }
        for (i=0;i<surfaces_lwo2.size();i++)
        {
            delete(surfaces_lwo2[i]);
        }
    }

    //======================================================================
    //                Transcryptage
    //======================================================================
    bool ChargeLWO2::analyse()
    {
        if (CHARGELWO2_DEBUG) cout<<"=================  LECTURE DU FICHIER LWO2: ' "<<chemin_fichier<<" ' "<<"==================="<<endl<<endl;

        //------------ Test le format du fichier:

        //---Ouverture fichier:
        fichier_flux.open(chemin_fichier, ios::in | ios::binary | ios::ate);

        if (!fichier_flux.is_open())
        {
            cout<<"ERREUR - LWO2 - Impossible d'ouvrir le fichier"<<endl;
            erreur=CHARGELWO2_ERREUR_FICHIER;
            return false;
        }
        //---Taille du fichier:
        taille_fichier=fichier_flux.tellg();
        if ( CHARGELWO2_DEBUG ) cout<<"Taille du fichier: "<<taille_fichier<<endl;

        //---Test le format du fichier:
        uint32_t id4_FORM;
        uint32_t taille_lue;
        uint32_t id4_LWO2;

        fichier_flux.seekg(0,ios::beg);

        fichier_flux.read((char*)&id4_FORM,4);
        fichier_flux.read((char*)&taille_lue,4);
        fichier_flux.read((char*)&id4_LWO2,4);

        id4_FORM=retournement_uint32(id4_FORM);
        taille_lue=retournement_uint32(taille_lue);
        id4_LWO2=retournement_uint32(id4_LWO2);

        if ((id4_FORM!=ID_FORM) ||(id4_LWO2!=ID_LWO2))
        {
            cout<<"ERREUR - LWO2 - Le fichier n'est pas au format LWO2"<<endl;
            erreur=CHARGELWO2_ERREUR_FICHIER;
            fichier_flux.close();
            return false;
        }

        if (taille_fichier!=(taille_lue+8))
        {
            cout<<"ERREUR - LWO2 - Le fichier est corrompu - taille fichier : "<<taille_fichier<<" - taille lue : "<<taille_lue<<endl;
            erreur=CHARGELWO2_ERREUR_FICHIER;
            fichier_flux.close();
            return false;
        }

        //--------- copie le fichier en mémoire:
        fichier_flux.seekg(0,ios::beg);
        donnees_LWO2=(char*)malloc(taille_fichier);
        if (donnees_LWO2==NULL)
        {
            cout<<"ERREUR - LWO2 - Mémoire insuffisante pour charger le fichier"<<endl;
            erreur=CHARGELWO2_ERREUR_MEMOIRE;
            fichier_flux.close();
            return false;

        }
        fichier_flux.read(donnees_LWO2,taille_fichier);
        fichier_flux.close();

        //-------- Création des structures:
        position=8;
        if (!creation_table_etiquettes() )  { free (donnees_LWO2); return false; }  //TAGS
        if (!creation_bitmaps() )           { free (donnees_LWO2); return false; }  //CLIP
        if (!creation_calques_lwo2())       { free (donnees_LWO2); return false; }  //LAYR
        if (!creation_parametres_sommets()) { free (donnees_LWO2); return false; }  //VMAP - VMAD
        if (!creation_surfaces_lwo2())      { free (donnees_LWO2); return false; }  //SURF

        //-------- Fin:
        if (CHARGELWO2_DEBUG) cout<<endl<<"========== Fichier lu avec succès ! ============"<<endl<<endl<<endl;
        free(donnees_LWO2);
        donnees_LWO2=NULL;
        return true;
    }

    //======================================================================
    //                  Lit les blocs TAGS (étiquettes)
    //======================================================================
    bool ChargeLWO2::creation_table_etiquettes()
    {
        if (CHARGELWO2_DEBUG) cout<<endl<<"---- Création des étiquettes: ----"<<endl;

        uint32_t i;

        while(position<taille_fichier)
        {
            ptr_lwo2=(uint32_t*)(donnees_LWO2+position);
            uint32_t id=retournement_uint32(*ptr_lwo2);
            if(id==ID_TAGS)
            {
                position_bloc=4;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
                taille_bloc=retournement_uint32(*ptr_lwo2)+8;
                position_bloc+=4;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);

                while(position_bloc<taille_bloc)
                {
                    char* etiquette=copie_chaine((char*)ptr_lwo2);
                    if (etiquette==NULL)
                    {
                        cout<<"Erreur dans ChargeLWO2::creation_table_etiquettes()- Mémoire insuffisante pour créer les chaînes de caractères"<<endl;
                        erreur=CHARGELWO2_ERREUR_MEMOIRE;
                        return false;
                    }
                    table_etiquettes.push_back(etiquette);
                    position_bloc+=(strlen(etiquette)+2) & 0xfffffe;
                    ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
                }

                for (i=0;i<table_etiquettes.size();i++)
                {
                    if (CHARGELWO2_DEBUG) cout<<"        Etiquette: "<<table_etiquettes[i]<<endl;
                }

                position+=taille_bloc;
                return true;
            }
            else position+=2;
        }
        return true;
    }
    //======================================================================
    //                  Lit les blocs CLIP (bitmap)
    //======================================================================
    bool ChargeLWO2::creation_bitmaps()
    {
        if (CHARGELWO2_DEBUG) cout<<endl<<"---- Création des bitmaps ----"<<endl;

        uint32_t    identifiant;
        //---- Lecture des blocs:
        while(position<(taille_fichier-4))
        {
            ptr_lwo2=(uint32_t*)(donnees_LWO2+position);
            if(retournement_uint32(*ptr_lwo2)==ID_CLIP)
            {
                position_bloc=0;
                position_bloc+=4; ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc); //On saute CLIP
                taille_bloc=retournement_uint32(*ptr_lwo2)+8; //+8 car je compte CLIP et sa taille
                position_bloc+=4; ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc); //On saute la taille du bloc
                identifiant=retournement_uint32(*ptr_lwo2);               //L'identifiant du clip
                position_bloc+=4; ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
                //Ne lit que les blocs de type STIL:
                if (retournement_uint32(*ptr_lwo2)==ID_STIL)
                {
                    position_bloc+=4; ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
                    taille_sous_bloc=retournement_uint16(*(uint16_t*)ptr_lwo2);
                    position_bloc+=2; ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
                    char* fichier_image=copie_chaine((char*)ptr_lwo2);
                    if (fichier_image==NULL)
                    {
                        cout<<"Erreur dans ChargeLWO2::creation_bitmaps() 1 - Mémoire insuffisante pour créer les chaînes de caractères"<<endl;
                        erreur=CHARGELWO2_ERREUR_MEMOIRE;
                        return false;
                    }
                    position_bloc+= taille_sous_bloc; //On saute le bloc STIL

                    LWO2_Bitmap* bitmap=new LWO2_Bitmap();
                    Bitmaps.push_back(bitmap);
                    bitmap->identite=identifiant;

                    //Initialise le chemin du fichier:

                    if (chemin_images!=NULL)
                    {
                        recupere_nom_fichier(fichier_image);
                        bitmap->fichier=(char*)malloc(strlen(fichier_image)+strlen(chemin_images)+1);
                        if (bitmap->fichier==NULL)
                        {
                            cout<<"Erreur dans ChargeLWO2::creation_bitmaps() 2 - Mémoire insuffisante pour créer les chaînes de caractères"<<endl;
                            erreur=CHARGELWO2_ERREUR_MEMOIRE;
                            return false;
                        }
                        strcpy(bitmap->fichier,chemin_images);
                        strcat(bitmap->fichier,fichier_image);
                        free(fichier_image);
                    }

                    else  bitmap->fichier=fichier_image;

                    //Chargement de l'image:
                    bitmap->image=new Image(bitmap->fichier);
                    bitmap->image->renversement_horizontal();

                    //recherche les sous-blocs de réglage de l'image:

                    while (position_bloc<taille_bloc-4)
                    {
                        ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
                        uint16_t drapeau;
                        switch (retournement_uint32(*ptr_lwo2))
                        {
                            case ID_CONT:
                                bitmap->contraste=recupere_sous_bloc_FP4();
                                if (CHARGELWO2_DEBUG) cout<<"Contraste:"<<bitmap->contraste<<endl;
                                break;
                            case ID_BRIT:
                                bitmap->luminosite=recupere_sous_bloc_FP4();
                                if (CHARGELWO2_DEBUG) cout<<"Luminosite:"<<bitmap->luminosite<<endl;
                                break;
                            case ID_SATR:
                                bitmap->saturation=recupere_sous_bloc_FP4();
                                if (CHARGELWO2_DEBUG) cout<<"Saturation:"<<bitmap->saturation<<endl;
                                break;
                            case ID_HUE:
                                bitmap->decallage_teinte=recupere_sous_bloc_FP4();
                                if (CHARGELWO2_DEBUG) cout<<"Décallage de teinte:"<<bitmap->decallage_teinte<<endl;
                                break;
                            case ID_GAMM:
                                bitmap->gamma=recupere_sous_bloc_FP4();
                                if (CHARGELWO2_DEBUG) cout<<"Gamma:"<<bitmap->gamma<<endl;
                                break;
                            case ID_NEGA:
                                drapeau=recupere_sous_bloc_U2();
                                if (drapeau!=0)
                                {
                                    bitmap->negatif=true;
                                    if (CHARGELWO2_DEBUG) cout<<"Image en négatif"<<endl;
                                }
                                break;
                            default:
                                position_bloc+=2;
                                break;

                        }

                    }

                    //Fin, on cherche le bloc CLIP suivant:
                    position+=position_bloc;
                }

            }

            else position+=2;
        }

        return true;
    }

    //======================================================================
    //                  Lit les blocs LAYR (calque)
    //======================================================================
    bool ChargeLWO2::creation_calques_lwo2()
    {
        if (CHARGELWO2_DEBUG) cout<<endl<<"---- Création des calques ----"<<endl;
        position=8;

        //---- Lecture des blocs:
        while(position<(taille_fichier-4))
        {
            ptr_lwo2=(uint32_t*)(donnees_LWO2+position);
            if(retournement_uint32(*ptr_lwo2)==ID_LAYR)
            {
                position_bloc=0;
                position_bloc+=4; ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc); //On saute LAYR
                taille_bloc=retournement_uint32(*ptr_lwo2)+8; //+8 car je compte LAYR et sa taille
                position_bloc+=4; ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc); //On saute la taille du bloc

                LWO2_Calque* calque=new LWO2_Calque();
                calques_lwo2.push_back(calque);

                calque->index=retournement_uint16(*(uint16_t*)ptr_lwo2);
                position_bloc+=2; ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
                calque->drapeaux=retournement_uint16(*(uint16_t*)ptr_lwo2);
                position_bloc+=2; ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
                recupere_vecteur_3f(calque->pivot);
                calque->nom=copie_chaine((char*)ptr_lwo2);

                position_bloc+=(strlen(calque->nom)+2) & 0x7ffffffe; ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
                if ((taille_bloc-position_bloc)>=2)
                {
                    calque->parent=(int16_t)retournement_uint16(*(int16_t*)ptr_lwo2);
                }

                position+=taille_bloc;

                //---- Lecture des objets propres au calque:
                uint32_t    position_LAYR=position;
                if(!creation_points(calque)) return false;
                position=position_LAYR;
                if(!creation_polygones(calque)) return false;

                //---- Affiche les données:
                if(CHARGELWO2_DEBUG)
                {
                    cout<<"** Calque : "<<calque->index<<endl;
                    cout<<"   Nom: "<<calque->nom<<endl;
                    if (calque->parent==-1)cout<<"   Parent: aucun"<<endl;
                    else cout<<"   Parent: "<<dec<<calque->parent<<endl;
                    cout<<"   Drapeaux:"<<hex<<calque->drapeaux<<endl;
                    cout<<dec;
                    cout<<"   Pivot - X: "<<calque->pivot[0]<<" - Y: "<<calque->pivot[1]<<" - Z: "<<calque->pivot[2]<<endl;
                    cout<<"   Polygones: "<<calque->polygones.size()<<endl;
                    cout<<"   Sommets: "<<calque->nbrSommets<<endl;

                }

            }
            else { position+=2;}
        }
        return true;
    }

    //======================================================================
    //                  Lit les blocs PNTS (sommets)
    ///         !!! Convertis les nombre en double précision !!!
    //======================================================================
    bool ChargeLWO2::creation_points(LWO2_Calque* calque)
    {
        uint32_t i;
        while(position<taille_fichier)
        {
            ptr_lwo2=(uint32_t*)(donnees_LWO2+position);
            uint32_t id=retournement_uint32(*ptr_lwo2);
            if(id==ID_PNTS)
            {
                position_bloc=4;
                ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
                calque->nbrSommets=retournement_uint32(*ptr_lwo2);
                calque->nbrSommets/=12;
                position_bloc+=4; ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);

                //Allocation des sommets:
                calque->sommets=(float*)malloc(calque->nbrSommets*sizeof(float)*3);

                if(calque->sommets==NULL)
                {
                    cout<<"Erreur dans ChargeLWO2::creation_points() - Mémoire insuffisante"<<endl;
                    erreur=CHARGELWO2_ERREUR_MEMOIRE;
                    return false;
                }

                //Récupération des coordonnées:
                for(i=0;i<calque->nbrSommets;i++)
                {
                    recupere_vecteur_3f(&calque->sommets[i*3]);
                }

                position+=calque->nbrSommets*12+8;
                return true;
            }

            //Si on sort du calque actuel:
            else if (id==ID_LAYR)
            {
                if (CHARGELWO2_DEBUG) cout<<"Avertissement: Le calque "<<calque->index<<" ne contient pas de points"<<endl;
                return true;
            }

            else position+=2;
        }

        //Fin des données, aucun sommet trouvé:
        if (CHARGELWO2_DEBUG) cout<<"Avertissement: Le calque "<<calque->index<<" ne contient pas de points"<<endl;
        return true;
    }

    //======================================================================
    //                  Lit les blocs POLS (polygones)
    //======================================================================
    bool ChargeLWO2::creation_polygones(LWO2_Calque* calque)
    {
        uint32_t i,id;
        while(position<taille_fichier)
        {
            ptr_lwo2=(uint32_t*)(donnees_LWO2+position);
            id=retournement_uint32(*ptr_lwo2);
            if(id==ID_POLS)
            {
                position_bloc=4;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
                taille_bloc=retournement_uint32(*ptr_lwo2)+8;
                position_bloc+=4;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
                uint32_t type_polygones=retournement_uint32(*ptr_lwo2);
                position_bloc+=4;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);

                if (type_polygones==ID_FACE)
                {
                    vector<LWO2_Polygone*> table_polygones; //Table provisoire. Elle est insérée à la table des polygones du calque
                                                            //après la création des liens avec les étiquettes (surfaces, part, groupes de filtre)

                    while (position_bloc<taille_bloc)
                    {
                        LWO2_Polygone* polygone=new LWO2_Polygone();
                        table_polygones.push_back(polygone);
                        polygone->nbrSommets=(uint32_t)retournement_uint16(*(uint16_t*)ptr_lwo2)& 0x3ff; //Masquage des drapeaux
                        position_bloc+=2;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
                        //Allocation de la table des indices de sommets:
                        polygone->indices_sommets=(uint32_t*)malloc(polygone->nbrSommets*4);

                        if (polygone->indices_sommets==NULL)
                        {
                            cout<<"Erreur dans ChargeLWO2::creation_polygones() - Mémoire insuffisante"<<endl;
                            erreur=CHARGELWO2_ERREUR_MEMOIRE;
                            return false;
                        }
                        //Récupération des indices de sommets:
                        for (i=0;i<polygone->nbrSommets;i++)
                        {
                                polygone->indices_sommets[i]=recupere_VX();
                        }
                    }

                    position+=taille_bloc;

                    //Lecture des liens avec les surfaces ou les groupements de polygones:
                    if ( !creation_liens_polygones(&table_polygones) )
                    {
                        for (i=0;i<table_polygones.size();i++)
                        {
                             delete table_polygones[i];
                        }
                        return false;
                    }

                    //Copie la nouvelle table de polygones dans le calque:
                    for (i=0;i<table_polygones.size();i++)
                    {
                         calque->polygones.push_back(table_polygones[i]);
                    }


                }
                else position+=taille_bloc;

            }
            //Test si on arrive au calque suivant:
            else if (id==ID_LAYR)
            {
                return true;
            }
            else
            {
                position+=2;
            }
        }
        return true;
    }
    //======================================================================
    //                  Lit les blocs PTAG (liens polygones)
    //======================================================================
    bool ChargeLWO2::creation_liens_polygones(vector<LWO2_Polygone*>* table_polygones)
    {
        uint32_t indice_polygone;
        while(position<taille_fichier)
        {
            ptr_lwo2=(uint32_t*)(donnees_LWO2+position);
            uint32_t id=retournement_uint32(*ptr_lwo2);
            if(id==ID_PTAG)
            {
                position_bloc=4;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
                taille_bloc=retournement_uint32(*ptr_lwo2)+8;
                position_bloc+=4;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
                uint32_t type_lien=retournement_uint32(*ptr_lwo2); //SURF, PART ou SMGP
                position_bloc+=4;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);

                if (type_lien==ID_SURF)
                {
                    while(position_bloc<taille_bloc)
                    {
                        indice_polygone=recupere_VX();
                        (*table_polygones)[indice_polygone]->indice_etiquette_surface=retournement_uint16(*(uint16_t*)ptr_lwo2);
                        position_bloc+=2;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
                    }
                }
                else if (type_lien==ID_PART)
                {
                    while(position_bloc<taille_bloc)
                    {
                        indice_polygone=recupere_VX();
                        (*table_polygones)[indice_polygone]->indice_etiquette_part=retournement_uint16(*(uint16_t*)ptr_lwo2);
                        position_bloc+=2;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
                    }
                }
                else if (type_lien==ID_SMGP)
                {
                    while(position_bloc<taille_bloc)
                    {
                        indice_polygone=recupere_VX();
                        (*table_polygones)[indice_polygone]->indice_etiquette_groupe_filtre=retournement_uint16(*(uint16_t*)ptr_lwo2);
                        position_bloc+=2;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
                    }
                }
                else
                {
                    cout<<"Avertissement dans ChargeLWO2::creation_liens_polygones() - Type de lien inconnu: "<<hex<<type_lien<<endl;
                    cout<<dec;
                }
                position+=taille_bloc;
            }
            //Test si on passe à une autre table de définition de polygones ouy carrément à un autre calque:
            else if (id==ID_POLS || id==ID_LAYR) return true;
            //Parcours les données:
            else position+=2;
        }
        return true;
    }

    //======================================================================
    //                  Création des paramètres de sommets (VMAP et VMAD)
    //======================================================================
    bool ChargeLWO2::creation_parametres_sommets()
    {
        if (CHARGELWO2_DEBUG) cout<<endl<<"---- Création des coordonnées UV ----"<<endl;
        uint32_t i;
        LWO2_ParametresSommets* parametres_UV;
        position=8;
        uint32_t type_texture; //Seul le type TXUV est pris en compte

        //Lecture des blocs VMAP:
        while(position<taille_fichier)
        {
            ptr_lwo2=(uint32_t*)(donnees_LWO2+position);
            uint32_t id=retournement_uint32(*ptr_lwo2);
            if(id==ID_IMAP)    //On saute les blocs IMAP car ils peuvent contenir un identifiant VMAP, ce qui entrainerait des confusions
            {
                position_bloc=4;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
                taille_bloc=(uint32_t)retournement_uint16(*(uint16_t*)ptr_lwo2);
                position+=8+taille_bloc;
            }
            else if(id==ID_VMAP)
            {
                position_bloc=4;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
                taille_bloc=retournement_uint32(*ptr_lwo2)+8;
                position_bloc+=4;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);

                type_texture=retournement_uint32(*ptr_lwo2);

                if (type_texture==ID_TXUV)
                {
                    position_bloc+=6;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc); //On saute type+dimension
                    parametres_UV= new LWO2_ParametresSommets();
                    table_parametres_sommets.push_back(parametres_UV);
                    parametres_UV->nom=copie_chaine((char*)ptr_lwo2);
                    if (parametres_UV->nom==NULL)
                    {
                        cout<<"Erreur dans Charge_LWO2::creation_parametres_sommets() - Mémoire insuffisante"<<endl;
                        erreur=CHARGELWO2_ERREUR_MEMOIRE;
                        return false;
                    }
                    position_bloc+=(strlen(parametres_UV->nom)+2) & 0xfffffffe;
                    ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);

                    if (CHARGELWO2_DEBUG) cout<<"*** Objet UV: "<<parametres_UV->nom<<endl;

                    while (taille_bloc>position_bloc)
                    {
                        LWO2_Sommet_UV* sommet_UV=ajoute_LWO2_sommet_UV(parametres_UV);
                        sommet_UV->indice_sommet=recupere_VX();
                        sommet_UV->U=retournement_float(*(float*)ptr_lwo2);
                        position_bloc+=4;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
                        sommet_UV->V=retournement_float(*(float*)ptr_lwo2);
                        position_bloc+=4;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
                        if (CHARGELWO2_DEBUG && CHARGELWO2_DEBUG_UV) cout<<"   "<<parametres_UV->sommets_UV.size()<<" : Sommet: "<<sommet_UV->indice_sommet<<" U="<<sommet_UV->U<<"  V="<<sommet_UV->V<<endl;
                    }
                }

                position+=taille_bloc;
            }
            else position+=2;
        }

        //------------- Lecture des blocs VMAD éventuels (création des coutures):
        position=8;
        //Lecture des blocs VMAP:
        while(position<taille_fichier)
        {
            ptr_lwo2=(uint32_t*)(donnees_LWO2+position);
            uint32_t id=retournement_uint32(*ptr_lwo2);
            if(id==ID_VMAD)
            {
                position_bloc=4;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
                taille_bloc=retournement_uint32(*ptr_lwo2)+8;
                position_bloc+=4;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);

                type_texture=retournement_uint32(*ptr_lwo2);

                if (type_texture==ID_TXUV)
                {
                    position_bloc+=6;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc); //On saute type+dimension

                    parametres_UV=NULL;
                    //Recherche l'objet LWO2_parametres_sommets correcpondant:
                    for (i=0;i<table_parametres_sommets.size();i++)
                    {
                        if (strcmp((char*)ptr_lwo2,table_parametres_sommets[i]->nom)==0)
                        {
                            parametres_UV=table_parametres_sommets[i];
                            break;
                        }
                    }

                    if (parametres_UV!=NULL)
                    {
                        parametres_UV->couture=true;
                        position_bloc+=(strlen((char*)ptr_lwo2)+2) & 0xfffffffe;
                        ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);

                        if (CHARGELWO2_DEBUG) cout<<"*** Objet UV de couture: "<<parametres_UV->nom<<endl;

                        while (taille_bloc>position_bloc)
                        {
                            LWO2_Sommet_UV_couture* sommet_UV=ajoute_LWO2_sommet_UV_couture(parametres_UV);
                            sommet_UV->indice_sommet=recupere_VX();
                            sommet_UV->indice_polygone=recupere_VX();
                            sommet_UV->U=retournement_float(*(float*)ptr_lwo2);
                            position_bloc+=4;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
                            sommet_UV->V=retournement_float(*(float*)ptr_lwo2);
                            position_bloc+=4;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
                            if (CHARGELWO2_DEBUG && CHARGELWO2_DEBUG_UV) cout<<"   "<<parametres_UV->sommets_UV.size()<<" : Sommet: "<<sommet_UV->indice_sommet<<" : polygone: "<<sommet_UV->indice_polygone<<" U="<<sommet_UV->U<<"  V="<<sommet_UV->V<<endl;
                        }
                    }

                }

                position+=taille_bloc;
            }
            else position+=2;
        }
        return true;
    }

    //Ajoute un sommet à une table de coordonnées UV:
    LWO2_Sommet_UV* ChargeLWO2::ajoute_LWO2_sommet_UV (LWO2_ParametresSommets* parametres_UV)
    {
        LWO2_Sommet_UV sommet;
        parametres_UV->sommets_UV.push_back(sommet);
        return &(parametres_UV->sommets_UV[parametres_UV->sommets_UV.size()-1]);
    }

    //Ajoute un sommet de couture à une table de coordonnées UV:
    LWO2_Sommet_UV_couture* ChargeLWO2::ajoute_LWO2_sommet_UV_couture (LWO2_ParametresSommets* parametres_UV)
    {
        LWO2_Sommet_UV_couture sommet;
        parametres_UV->sommets_UV_couture.push_back(sommet);
        return &(parametres_UV->sommets_UV_couture[parametres_UV->sommets_UV_couture.size()-1]);
    }

    //======================================================================
    //                  Création des surfaces
    //======================================================================
    bool ChargeLWO2::creation_surfaces_lwo2()
    {
        if (CHARGELWO2_DEBUG) cout<<endl<<"---- Création des Surfaces ----"<<endl;
        position=8;
        //Lecture des blocs SURF:
        while(position<taille_fichier)
        {
            ptr_lwo2=(uint32_t*)(donnees_LWO2+position);
            uint32_t id=retournement_uint32(*ptr_lwo2);
            //On saute les blocs PTAG qui peuvent contenir un identifiant SURF
            if(id==ID_PTAG)
            {
                position+=4;ptr_lwo2=(uint32_t*)(donnees_LWO2+position);
                taille_bloc=retournement_uint32(*ptr_lwo2);
                position+=taille_bloc+4;
            }
            else if(id==ID_SURF)
            {
                position_bloc=4;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
                taille_bloc=retournement_uint32(*ptr_lwo2)+8;
                position_bloc+=4;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);

                LWO2_Surface* surface=new LWO2_Surface();
                surfaces_lwo2.push_back(surface);

                //Récupère le nom de la surface:
                surface->nom=copie_chaine((char*)ptr_lwo2);
                if (surface->nom==NULL)
                {
                    cout<<"Erreur dans ChargeLWO2::creation_surfaces_lwo2() - Mémoire insuffisante"<<endl;
                    erreur=CHARGELWO2_ERREUR_MEMOIRE;
                    return false;
                }

                if (CHARGELWO2_DEBUG && CHARGELWO2_DEBUG_SURFACES)
                {
                    cout<<"  ****** Surface: "<<surface->nom<<" - Taille chunk: "<<taille_bloc<<endl;
                }

                position_bloc+=(strlen((char*)ptr_lwo2)+2) & 0xfffffffe;
                ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);

                //Récupère le nom de la source:
                surface->nom_source=copie_chaine((char*)ptr_lwo2);
                if (surface->nom_source==NULL)
                {
                    cout<<"Erreur dans ChargeLWO2::creation_surfaces_lwo2() - Mémoire insuffisante"<<endl;
                    erreur=CHARGELWO2_ERREUR_MEMOIRE;
                    return false;
                }
                position_bloc+=(strlen((char*)ptr_lwo2)+2) & 0xfffffffe;
                ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);

                //Lecture des sous-blocs de la surface:

                while(position_bloc<taille_bloc)
                {
                    ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
                   id=retournement_uint32(*ptr_lwo2);
                   position_bloc+=4;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
                   taille_sous_bloc=(uint32_t)retournement_uint16(*(uint16_t*)ptr_lwo2);
                   position_bloc+=2;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
                   switch (id)
                   {
                        case ID_COLR:
                            surface->couleur[0]=retournement_float(*((float*)ptr_lwo2));
                            surface->couleur[1]=retournement_float(*((float*)ptr_lwo2+1));
                            surface->couleur[2]=retournement_float(*((float*)ptr_lwo2+2));
                        break;
                        case ID_DIFF:
                            surface->diffusion=retournement_float(*((float*)ptr_lwo2));
                        break;
                        case ID_LUMI:
                            surface->luminescence=retournement_float(*((float*)ptr_lwo2));
                        break;
                        case ID_SPEC:
                            surface->specularite=retournement_float(*((float*)ptr_lwo2));
                        break;
                        case ID_REFL:
                            surface->reflexion=retournement_float(*((float*)ptr_lwo2));
                        break;
                        case ID_TRAN:
                            surface->transparence=retournement_float(*((float*)ptr_lwo2));
                        break;
                        case ID_TRNL:
                            surface->translucidite=retournement_float(*((float*)ptr_lwo2));
                        break;
                        case ID_GLOS:
                            surface->brillance=retournement_float(*((float*)ptr_lwo2));
                        break;
                        case ID_SHRP:
                            surface->terminateur=retournement_float(*((float*)ptr_lwo2));
                        break;
                        case ID_BUMP:
                            surface->rugosite=retournement_float(*((float*)ptr_lwo2));
                        break;
                        case ID_SIDE:
                            if(retournement_uint16(*((uint16_t*)ptr_lwo2))==3) surface->double_face=true;
                        break;
                        case ID_SMAN:
                            surface->angle_limite_adoucissement=retournement_float(*((float*)ptr_lwo2));    //En radians
                            if (surface->angle_limite_adoucissement>0.) surface->adoucissement=true;
                        break;
                        case ID_RFOP:
                            //cout<<"RFOP"<<endl;
                        break;
                        case ID_RIMG:
                            //cout<<"RIMG"<<endl;
                        break;
                        case ID_RSAN:
                            //cout<<"RSAN"<<endl;
                        break;
                        case ID_RBLR:
                            //cout<<"RBLR"<<endl;
                        break;
                        case ID_RIND:
                            //cout<<"RIND"<<endl;
                        break;
                        case ID_TROP:
                            //cout<<"TROP"<<endl;
                        break;
                        case ID_TIMG:
                            //cout<<"TIMG"<<endl;
                        break;
                        case ID_TBLR:
                            //cout<<"TBLR"<<endl;
                        break;
                        case ID_CLRH:
                            surface->teinte_lumiere=retournement_float(*((float*)ptr_lwo2));
                        break;
                        case ID_CLRF:
                            //cout<<"CLRF"<<endl;
                        break;
                        case ID_ADTR:
                            //cout<<"ADTR"<<endl;
                        break;
                        case ID_GLOW:
                            //cout<<"GLOW"<<endl;
                        break;
                        case ID_GVAL:
                            surface->intensite_halo=retournement_float(*((float*)ptr_lwo2));
                            if (surface->intensite_halo>0.)surface->halo=true;
                        break;
                        case ID_LINE:
                            //cout<<"LINE"<<endl;
                        break;
                        case ID_ALPH:
                            //cout<<"ALPH"<<endl;
                        break;
                        case ID_VCOL:
                            //cout<<"VCOL"<<endl;
                        break;
                        case ID_VERS:
                            //cout<<"VERS"<<endl;
                        break;
                        case ID_NODS:
                            //cout<<"NODS"<<endl;
                        break;
                        //Création des textures (BLOK):
                        case ID_BLOK:
                            if(!creation_textures_lwo2(surface)) return false;
                        break;
                        //NVSK "Exclude from VSTACK:
                        case ID_NVSK:
                            //cout<<"NVSK"<<endl;
                        break;
                        //Si aucun identifiant reconnu:
                        default:
                            cout<<"Bloc de surface inconnu : "<<converti_uint32_chaine(id)<<endl;
                            //cout<<dec;
                        break;
                   }
                   position_bloc+=taille_sous_bloc;
                }
                position+=taille_bloc;
            }
            else position+=2;
        }
        return true;
    }

    //======================================================================
    //                  Création des textures (BLOK)
    //  Seuls les bloc de type IMAP avec les projections UV (TXUV) sont pris en compte.
    //======================================================================
    bool ChargeLWO2::creation_textures_lwo2(LWO2_Surface* surface)
    {
        uint32_t temp32;
        position_sous_bloc=0;
        uint32_t id=retournement_uint32(*ptr_lwo2);
        if (CHARGELWO2_DEBUG && CHARGELWO2_DEBUG_TEXTURES) cout<<"         .Texture - "<<converti_uint32_chaine(id)<<endl;
        uint32_t taille_sous_bloc_BLOK;
        LWO2_Texture* texture=new LWO2_Texture();
        surface->textures.push_back(texture);
        texture->type=id;
        position_sous_bloc+=6;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc+position_sous_bloc);
        texture->chaine_tri=copie_chaine((char*)ptr_lwo2);
        if (CHARGELWO2_DEBUG && CHARGELWO2_DEBUG_TEXTURES)
        {
            cout<<"               Chaîne de tri: "<<hex;
            uint32_t taille_chaine=strlen(texture->chaine_tri);
            for (uint16_t i=0 ; i<taille_chaine;i++)
            {
                cout<<(((uint32_t)texture->chaine_tri[i]) & 0xff);
            }
            cout<<dec<<endl;
        }
        if(texture->chaine_tri==NULL)
        {
            cout<<"Erreur dans ChargeLWO2::creation_textures_lwo2() - Mémoire insuffisante"<<endl;
            erreur=CHARGELWO2_ERREUR_MEMOIRE;
            return false;
        }
        position_sous_bloc+=(strlen(texture->chaine_tri)+2) & 0xfffffffe;
        ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc+position_sous_bloc);

        //Lecture des paramètres de la texture:
        //if (texture->type==ID_IMAP)
        //{
            while(position_sous_bloc<taille_sous_bloc)
            {
                ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc+position_sous_bloc);
                id=retournement_uint32(*ptr_lwo2);
                position_sous_bloc+=4;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc+position_sous_bloc);
                taille_sous_bloc_BLOK=(uint32_t)retournement_uint16(*(uint16_t*)ptr_lwo2);
                position_sous_bloc+=2;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc+position_sous_bloc);
                switch(id)
                {
                    case ID_CHAN:
                        texture->canal=retournement_uint32(*ptr_lwo2);
                        texture->nom_canal[4]=0;
                        texture->nom_canal[0]=(char)(texture->canal>>24);
                        texture->nom_canal[1]=(char)(texture->canal>>16);
                        texture->nom_canal[2]=(char)(texture->canal>>8);
                        texture->nom_canal[3]=(char)(texture->canal);
                        if (CHARGELWO2_DEBUG && CHARGELWO2_DEBUG_TEXTURES) cout<<"               Canal: "<<texture->nom_canal<<endl;
                        break;
                    case ID_ENAB:
                        if(retournement_uint16(*(uint16_t*)ptr_lwo2)==0) texture->active=false;
                        break;
                    case ID_OPAC:
                        //cout<<"OPAC"<<endl;
                        break;
                    case ID_AXIS:
                        texture->axe_projection=retournement_uint16(*(uint16_t*)ptr_lwo2);
                        break;

                    //Chunks propres aux blocs IMAP:
                        case ID_PROJ:
                            texture->type_projection=retournement_uint16(*(uint16_t*)ptr_lwo2);//Pour le moment seul le type UV (5) est pris en compte.
                            break;
                        break;
                        case ID_IMAG:
                            texture->identite_clip=recupere_VX_sans_decallage_ptr();
                            break;
                        case ID_WRAP:
                            temp32=retournement_uint32(*ptr_lwo2);
                            texture->mode_debordement_largeur=temp32>>16;
                            texture->mode_debordement_hauteur=temp32 & 0xffff;
                            break;
                        case ID_WRPW:
                            texture->nbr_debordements_largeur=retournement_float(*(float*)ptr_lwo2);
                            break;
                        case ID_WRPH:
                            texture->nbr_debordements_hauteur=retournement_float(*(float*)ptr_lwo2);
                            break;
                        case ID_VMAP:
                            texture->Texture_UV=renvoie_ParametresSommets((char*)ptr_lwo2);
                            break;

                        case ID_AAST:
                            //cout<<"AAST"<<endl;
                            break;

                        case ID_PIXB:
                            if(retournement_uint16(*(uint16_t*)ptr_lwo2)==0)texture->fondu_pixels=false;
                            break;

                        case ID_STCK:
                            //cout<<"STCK"<<endl;
                            break;

                        case ID_TAMP:
                            texture->amplitude_rugosite=retournement_float(*(float*)ptr_lwo2);
                            break;

                        case ID_NEGA:
                            //cout<<"NEGA"<<endl;
                            break;

                        //Les sous-blocs suivants TMAP sont inclus dans la taille du bloc TMAP...A surveiller.
                        case ID_TMAP:
                            //cout<<"TMAP"<<endl;
                            break;
                                case ID_CNTR:
                                    recupere_vecteur_3f_sans_decallage_ptr(texture->centre);
                                    break;
                                case ID_SIZE:
                                    recupere_vecteur_3f_sans_decallage_ptr(texture->echelle);
                                    break;
                                case ID_ROTA:
                                    recupere_vecteur_3f_sans_decallage_ptr(texture->rotation);
                                    break;
                                case ID_OREF:
                                    //cout<<"OREF"<<endl;
                                    break;
                                case ID_FALL:
                                    //cout<<"FALL"<<endl;
                                    break;
                                case ID_CSYS:
                                    //cout<<"CSYS"<<endl;
                                    texture->systeme_coordonnees=retournement_uint16(*(uint16*)ptr_lwo2); //Repère de référence pour les textures projetées
                                    break;
                        //Chunks propres aux blocs PROC:
                        //Chunks propres aux blocs GRAD:
                        //Chunks propres aux blocs SHDR:
                        default:
                            if(CHARGELWO2_DEBUG && CHARGELWO2_DEBUG_TEXTURES) cout<<"Bloc de texture inconnu ou non pris en compte: "<<converti_uint32_chaine(id)<<endl;
                        break;

                }
                position_sous_bloc+=taille_sous_bloc_BLOK;
            }
        //}
        //Tri les textures:

        //Débuguage:

        return true;
    }

    //------------------------------------------------------------------------------------
    //      Récupère un indice de type 'VX' :
    //      Si le premier octet est à 0xFF, l'indice s'étend sur les 3 octets suivants (24 bits)
    //      Sinon l'indice s'étend sur l'octet testé plus le suivant (16 bits)
    //------------------------------------------------------------------------------------

    uint32_t ChargeLWO2::recupere_VX()
    {
        uint32_t donnee;
        if( *(uint8_t*)ptr_lwo2==0xff ) {donnee=retournement_uint32(*ptr_lwo2) & 0xffffff; position_bloc+=4;}
        else { donnee=(uint32_t)retournement_uint16(*(uint16_t*)ptr_lwo2) ; position_bloc+=2;}
        ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
        return donnee;
    }

     uint32_t ChargeLWO2::recupere_VX_sans_decallage_ptr()
    {
        uint32_t donnee;
        if( *(uint8_t*)ptr_lwo2==0xff ) {donnee=retournement_uint32(*ptr_lwo2) & 0xffffff;}
        else { donnee=(uint32_t)retournement_uint16(*(uint16_t*)ptr_lwo2) ;}
        return donnee;
    }


    //------------------------------------------------------------------------------------
    //      Récupère le nombre flottant dans un sous-bloc ne comportant que cette donnée
    //      Positionne le pointeur sur la fin du bloc.
    //------------------------------------------------------------------------------------
    float ChargeLWO2::recupere_sous_bloc_FP4()
    {
        position_bloc+=4;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
        uint16_t taille=retournement_uint16(*(uint16_t*)ptr_lwo2);
        position_bloc+=2;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
        float donnee=retournement_float(*(float*)ptr_lwo2);
        position_bloc+=taille;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
        return donnee;
    }

    //------------------------------------------------------------------------------------
    //      Récupère le nombre 16bits dans un sous-bloc ne comportant que cette donnée
    //      Positionne le pointeur sur la fin du bloc.
    //------------------------------------------------------------------------------------
    uint16_t ChargeLWO2::recupere_sous_bloc_U2()
    {
        position_bloc+=4;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
        uint16_t taille=retournement_uint16(*(uint16_t*)ptr_lwo2);
        position_bloc+=2;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
        uint16_t donnee=retournement_uint16(*(uint16_t*)ptr_lwo2);
        position_bloc+=taille;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
        return donnee;
    }
    //------------------------------------------------------------------------------------
    //      Récupère des coordonnées tridimensionnelles
    //      Positionne le pointeur sur la fin du vecteur.
    //------------------------------------------------------------------------------------
    void ChargeLWO2::recupere_vecteur_3f(float* coordonnees)
    {
        for(uint16_t i=0;i<3;i++)
        {
            coordonnees[i]=retournement_float(*(float*)ptr_lwo2);
            position_bloc+=4;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
        }
    }

    void ChargeLWO2::recupere_vecteur_3f_sans_decallage_ptr(float* coordonnees)
    {
        uint32_t position_vecteur=0;
        for(uint16_t i=0;i<3;i++)
        {
            coordonnees[i]=retournement_float(*(float*)(ptr_lwo2+position_vecteur));
            position_vecteur+=4;
        }
    }

    //Le vecteur est convertis en double précision:
    void ChargeLWO2::recupere_vecteur_3f_convertion_3d(double* coordonnees)
    {
        for(uint16_t i=0;i<3;i++)
        {
            coordonnees[i]=(double)retournement_float(*(float*)ptr_lwo2);
            position_bloc+=4;ptr_lwo2=(uint32_t*)(donnees_LWO2+position+position_bloc);
        }

    }

   //=================================================================
    //                 Fonctions utiles
    //=================================================================
    char* ChargeLWO2::converti_uint32_chaine(uint32_t id)
    {
        uint32_converti[0]=(char)(id>>24)&0xff;
        uint32_converti[1]=(char)(id>>16)&0xff;
        uint32_converti[2]=(char)(id>>8)&0xff;
        uint32_converti[3]=(char)id&0xff;
        uint32_converti[4]=0;
        return uint32_converti;
    }

    const char* ChargeLWO2::renvoie_nom_fichier()
    {
        return chemin_fichier;
    }

    LWO2_Calque* ChargeLWO2::renvoie_LWO2_calque(uint32_t numCalque)
    {
        if (numCalque>=calques_lwo2.size()) return NULL;
        return calques_lwo2[numCalque];
    }

    LWO2_Calque* ChargeLWO2::renvoie_LWO2_calque_id(uint32_t idCalque)
    {
        for(uint32_t i=0; i<calques_lwo2.size();i++)
        {
            LWO2_Calque* calque=calques_lwo2[i];
            if(calque->index==idCalque) return calque;
        }
        return NULL;
    }

    LWO2_Calque* ChargeLWO2::renvoie_LWO2_calque_nom(char* nom_calque)
    {
        uint32_t i;
        for(i=0;i<calques_lwo2.size();i++)
        {
            if(strcmp(calques_lwo2[i]->nom,nom_calque)==0) return calques_lwo2[i];
        }
        return NULL;
    }

    uint32_t ChargeLWO2::renvoie_nbrBitmaps()
    {
        return (uint32_t)Bitmaps.size();
    }

    uint32_t ChargeLWO2::renvoie_nbrSurfaces()
    {
        return (uint32_t)surfaces_lwo2.size();
    }

    uint32_t ChargeLWO2::renvoie_nbrCalques()
    {
        return (uint32_t)calques_lwo2.size();
    }

    LWO2_Surface* ChargeLWO2::renvoie_LWO2_surface(uint32_t indice)
    {
        if(indice>=surfaces_lwo2.size()) return NULL;
        return surfaces_lwo2[indice];
    }

    LWO2_Surface* ChargeLWO2::renvoie_LWO2_surface(char* nom)
    {
        for (uint32_t i=0; i<surfaces_lwo2.size();i++)
        {
            if (strcmp(nom,surfaces_lwo2[i]->nom)==0) return surfaces_lwo2[i];
        }
        return NULL;
    }

    char* ChargeLWO2::renvoie_etiquette(uint32_t indice)
    {
        if ( table_etiquettes.size()<= indice ) return NULL;
        return table_etiquettes[indice];
    }

    //Renvoie le bitmap correspondant à l'identifiant
    LWO2_Bitmap* ChargeLWO2::renvoie_LWO2_bitMap_id(uint32_t id)
    {
        uint32_t i;
        LWO2_Bitmap* bitmap;
        for (i=0;i<Bitmaps.size();i++)
        {
            bitmap=Bitmaps[i];
            if (bitmap->identite == id) return bitmap;
        }
        return NULL;
    }

    //Renvoie le bitmap correspondant à l'index dans le tableau des bitmaps
    LWO2_Bitmap* ChargeLWO2::renvoie_LWO2_bitmap_index(uint32_t indice)
    {
        if(indice>=Bitmaps.size()) return NULL;
        return Bitmaps[indice];
    }

    //------------------------------------------------------------------------------------
    //      Renvoie l'objet Parametres_Sommets en fonction du nom spécifié en entrée
    //      Renvoie NULL si aucun objet ne porte ce nom.
    //------------------------------------------------------------------------------------
    LWO2_ParametresSommets* ChargeLWO2::renvoie_ParametresSommets(char* nom_cherche)
    {
        for(uint32_t i=0;i<table_parametres_sommets.size();i++)
        {
            if (strcmp(nom_cherche,table_parametres_sommets[i]->nom)==0) return table_parametres_sommets[i];
        }
        return NULL;
    }
