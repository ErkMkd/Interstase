
//-----------------------------------------------------------------------------------
//  Classe des faces:
/// ATTENTION: les tables des arêtes sont libérées par le destructeur des faces.
//-----------------------------------------------------------------------------------

#include "Face.h"
using namespace std;

        Face::Face(uint16_t p_nbrSommets, uint32_t* p_aretes, char* p_nom_surface,uint8_t p_drapeaux_liberation_memoire)
        {

            nbrSommets=p_nbrSommets;
            aretes=p_aretes;
            erreur=FACE_OK;

            volume=NULL;
            surface=NULL;
            liens_aretes=NULL;
            normales_sommets=NULL;
            drapeau_partiellement_adoucie=false;
            drapeaux_liberation_memoire=p_drapeaux_liberation_memoire;

            if(!allocation_normales_sommets())
            {
                cout<<"ERREUR dans Face::Face() - Mémoire insuffisante pour allouer la table des normales des sommets"<<endl;
                erreur=FACE_ERREUR_MEMOIRE;

            }

            if (!copie_nom_surface(p_nom_surface)) erreur=FACE_ERREUR_SURFACE;
        }




        //=================================================
        //              CONSTRUCTEUR de copie
        //  La face copiée n'est pas attribuée à un volume.
        //
        //=================================================

        Face::Face(Face& face_originale)
        {
            nbrSommets=face_originale.nbrSommets;
            aretes=NULL;
            erreur=FACE_OK;
            volume=NULL;
            surface=NULL;
            liens_aretes=NULL;
            normales_sommets=NULL;
            drapeau_partiellement_adoucie=false;

            if(!allocation_aretes() || !allocation_normales_sommets())
            {
               cout<<"ERREUR dans Face::Face(Face&) - Mémoire insuffisante pour allouer les tables"<<endl;
                erreur=FACE_ERREUR_MEMOIRE;
            }

            //Copie les arêtes:
            for(uint32_t i=0;i<nbrSommets;i++) aretes[i]=face_originale.aretes[i];

            if (!copie_nom_surface(face_originale.nom_surface)) erreur=FACE_ERREUR_SURFACE;
        }

        //=================================================
        //              DESTRUCTEUR
        //=================================================

        Face::~Face()
        {
            //La table des arêtes est libérée par le volume, au besoin.
            if (liens_aretes!=NULL) free(liens_aretes);
            if ( ((drapeaux_liberation_memoire&FACE_LIBERE_ARETES)!=0 ) &&  aretes!=NULL ) free (aretes);
            if (normales_sommets!=NULL) free(normales_sommets);
        }



        //---------------------------------------------------
        //  Allocation du tableau des normales des sommets
        //---------------------------------------------------

        bool Face::allocation_normales_sommets()
        {
            normales_sommets=(GLfloat*)malloc(nbrSommets*sizeof(GLfloat)*3);
            if (normales_sommets==NULL) return false;
            return true;
        }

        //-----------------------------------------
        //  Allocation du tableau des aretes
        //  Sert uniquement lors de la copie
        //-----------------------------------------
        bool Face::allocation_aretes()
        {
            aretes=(uint32_t*)malloc(nbrSommets*sizeof(uint32_t));
            if (aretes==NULL) return false;
            drapeaux_liberation_memoire=FACE_LIBERE_ARETES;
            return true;
        }
        //----------------------------------
        //      Copie le nom de la surface
        //----------------------------------
        bool Face::copie_nom_surface(char* p_nom)
        {
            uint32_t i,taille;
            if (p_nom==NULL) return false;
            else
            {
                taille=strlen(p_nom);
                if (taille>256-1)taille=256-1;  //Il peut y avoir une erreur, mais il faut le faire!
                for(i=0;i<taille;i++) nom_surface[i]=p_nom[i];
                nom_surface[i]=0;
                return true;
            }
        }


        void Face::affiche_centre_gravite()
        {
            cout<<centre_gravite[0]<<" "<<centre_gravite[1]<<" "<<centre_gravite[2]<<endl;
        }


        //--------------------------------------------
        //      Inverse les normales de la face
        //--------------------------------------------
        void Face::inverse_normales()
        {
            normale[0]=-normale[0];
            normale[1]=-normale[1];
            normale[2]=-normale[2];
            for (uint16_t i=0;i<nbrSommets;i++)
            {
                normales_sommets[3*i]=-normales_sommets[3*i];
                normales_sommets[3*i+1]=-normales_sommets[3*i+1];
                normales_sommets[3*i+2]=-normales_sommets[3*i+2];
            }
        }
