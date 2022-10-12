

//=========================== Définitions des types de fichiers ===============================

#include "gestion_fichiers.h"

 using namespace std;

//char* charge_fichier(const char *filename,unsigned int type);
//char* lecture_chaine(FILE* fichier);
//unsigned int retournement_uint(unsigned int donnee);
//unsigned short retournement_ushort(unsigned short donnee);
//float retournement_float(float donnee);

    //==============================================================================
    //Remet en ordre les octets après la lecture d'une donnée dans un fichier:
    //==============================================================================

    uint64_t Fichiers::retournement_uint64(uint64_t donnee)
    {
        return ((donnee>>56)&0xff) | ((donnee>>32)&0xff00) | ((donnee>>24)&0xff00) | ((donnee>>8)&0xff00)
        | ((donnee<<8)&0xff0000) | ((donnee<<24)&0xff000000) | ((donnee<<32)&0xff00) | ((donnee<<56)&0xff00);
    }

    uint32_t Fichiers::retournement_uint32(uint32_t donnee)
    {
        return ((donnee>>24)&0xff) | ((donnee>>8)&0xff00) | ((donnee<<8)&0xff0000) | ((donnee<<24)&0xff000000);
    }

    uint16_t Fichiers::retournement_uint16(uint16_t donnee)
    {
        return ((donnee>>8)&0xff) | ((donnee<<8)&0xff00);
    }

    float Fichiers::retournement_float(float donnee)
    {
        char octets[4];
        void* conv=&donnee;
        octets[0] = ((char*)conv)[3];
        octets[1] = ((char*)conv)[2];
        octets[2] = ((char*)conv)[1];
        octets[3] = ((char*)conv)[0];
        ((char*)conv)[3]=octets[3];
        ((char*)conv)[2]=octets[2];
        ((char*)conv)[1]=octets[1];
        ((char*)conv)[0]=octets[0];

        return donnee;//((donnee>>24)&0xff) | ((donnee>>8)&0xff00) | ((donnee<<8)&0xff0000) | ((donnee<<24)&0xff000000);
    }

    //======================================================================================
    //              Lit une chaine de caractère
    //              Le pointeur de fichier doit pointer sur la chaîne.
    //              En sortie, le pointeur de fichier est callé sur le premier octet après la chaîne.
    //              La chaîne doit être terminée par un 0
    ///              Ne pas oublier de libérer la mémoire lorsque la chaîne ne sert plus.
    //======================================================================================
    char* Fichiers::lecture_chaine(FILE* fichier)
    {
        unsigned int position_mem=ftell(fichier);
        unsigned int taille_chaine=0;
        unsigned char octet;
        do
        {
            octet=fgetc(fichier);
            taille_chaine++;
        }
        while(octet!=0);
        char* chaine=(char*)malloc(taille_chaine);
        fseek(fichier,position_mem,SEEK_SET);
        if (chaine==0)return 0;
        fgets(chaine,taille_chaine,fichier);
        fseek(fichier,position_mem+taille_chaine,SEEK_SET);
        return chaine;
    }

    //======================================================================================
    //              Duplique une chaine de caractère
    //              La chaîne doit être terminée par un 0
    ///              Ne pas oublier de libérer la mémoire lorsque la chaîne ne sert plus.
    //======================================================================================
    char* Fichiers::copie_chaine(char* chaine_source, uint32_t taille_max)
    {
        uint32_t i,j;
        for (i=0;i<taille_max;i++) if (chaine_source[i]==0) break;
        char* chaine=(char*)malloc(i+1);
        if (chaine==NULL)return NULL;
        for (j=0;j<=i;j++) chaine[j]=chaine_source[j];
        return chaine;
    }

    //======================================================================================
    //                  Récupère le nom d'un fichier sans le chemin:
    //      La chaîne de destination peut être le même tableau. Le chemin est alors écrasé
    //======================================================================================

    void Fichiers::recupere_nom_fichier(char* nom_complet, char* nom_fichier)
    {
        uint32_t taille_nom=strlen(nom_complet);
        uint32_t i,j;
        for (i=taille_nom;i>0;i--)
        {
            if (nom_complet[i]=='/')
            {
                i++;
                for (j=0;j<(taille_nom-i);j++)
                {
                    nom_fichier[j]=nom_complet[i+j];
                }
                nom_fichier[j]=0;
                break;
            }
        }
    }

    void Fichiers::recupere_nom_fichier(char* nom_complet)
    {
        recupere_nom_fichier(nom_complet,nom_complet);
    }

    //======================================================================================
    //                  Lit et écrit des données binaires
    //======================================================================================

    //-----------------------------------------
    //          Lecture des données
    //  Renvoie le nombre d'octets lus
    // Renvoie -1 si une erreur s'est produite
    //----------------------------------------
    int32_t Fichiers::charge_donnees_binaires(char* tempon,uint32_t taille_tempon, char* nom_fichier)
    {
        ifstream fichier_flux;
        uint32_t taille_fichier,taille_lecture;

        //---Ouverture fichier:
        fichier_flux.open(nom_fichier, ios::in | ios::binary | ios::ate );

        if (!fichier_flux.is_open())
        {
            std::cout<<"ERREUR - Impossible d'ouvrir le fichier "<<nom_fichier<<std::endl;
            return false;
        }

        //---Taille du fichier:
        taille_fichier=fichier_flux.tellg();

        if (taille_fichier<taille_tempon) taille_lecture=taille_fichier;
        else taille_lecture=taille_tempon;

        //----- Chargement:
        fichier_flux.seekg(0,ios::beg);
        fichier_flux.read((char*)tempon,taille_lecture);
        fichier_flux.close();

        return taille_lecture;
    }


    //------------------------------------------------------
    //      Renvoie la taille d'un fichier en octets
    //------------------------------------------------------
    uint32_t Fichiers::renvoie_taille_fichier(char* nom_fichier)
    {
        ifstream fichier_flux;
        uint32_t taille_fichier;

        //---Ouverture fichier:
        fichier_flux.open(nom_fichier, ios::in | ios::binary | ios::ate );

        if (!fichier_flux.is_open())
        {
            std::cout<<"ERREUR - Fichiers::renvoie_taille_fichier() - Impossible d'ouvrir le fichier : "<<nom_fichier<<std::endl;
            return 0;
        }

        //---Taille du fichier:
        taille_fichier=fichier_flux.tellg();
        fichier_flux.close();
        return taille_fichier;
    }

    //--------------------------------------------------------------
    //      Chargement avec allocation de la mémoire nécessaire
    /// Ne pas oublier de libérer la mémoire !!!
    //--------------------------------------------------------------
    char* Fichiers::charge_donnees_binaires_allocation(char* nom_fichier)
    {
        ifstream fichier_flux;
        uint32_t taille_fichier;

        //---Ouverture fichier:
        fichier_flux.open(nom_fichier, ios::in | ios::binary | ios::ate );

        if (!fichier_flux.is_open())
        {
            std::cout<<"ERREUR - Fichiers::charge_donnees_binaires_allocation - Impossible d'ouvrir le fichier "<<nom_fichier<<std::endl;
            return NULL;
        }

        //---Taille du fichier:
        taille_fichier=fichier_flux.tellg();

        //----- Allocation:
        char* tempon=(char*)malloc(taille_fichier);
        if(tempon==NULL)
        {
            fichier_flux.close();
            return NULL;
        }
        //----- Chargement:
        fichier_flux.seekg(0,ios::beg);
        fichier_flux.read((char*)tempon,taille_fichier);
        fichier_flux.close();

        /*
        for(uint32_t i=0;i<taille_fichier;i++)
        {
            cout<<uint16_t(tempon[i])<<" ";
        }
        cout<<"ICI"<<endl;
        */

        return tempon;
    }

    //---------------------------------------------------
    //      Sauve les données dans un fichier
    /// Si le fichier existait déjà, il est écrasé !!!
    //---------------------------------------------------
    bool Fichiers::sauve_donnees_binaires(char* donnees,uint32_t taille, char* nom_fichier)
    {
        ofstream fichier_flux;

        //---Ouverture fichier:
        fichier_flux.open(nom_fichier, ios::out | ios::binary | ios::trunc );

        if (!fichier_flux.is_open())
        {
            std::cout<<"ERREUR - Impossible d'ouvrir le fichier "<<nom_fichier<<std::endl;
            return false;
        }

        fichier_flux.write((char*)donnees,taille);
        fichier_flux.close();

        /*
        for(uint32_t i=0;i<taille;i++)
        {
            cout<<uint16_t(donnees[i])<<" ";
        }
        cout<<endl;
        */

        return true;
    }
