#ifndef EDITEUR_TRAJECTOIRE_H_INCLUDED
#define EDITEUR_TRAJECTOIRE_H_INCLUDED

#include <stdlib.h>
#include "trajectoire.h"

class EditeurTrajectoire
{
    public:
        static uint32_t ET_point_actuel;
        static float ET_vitesse_deplacement;
        static float ET_position_trajectoire;
        static float ET_pas_position_trajectoire;
        static char ET_fichier_trajectoire[];

        //Position actuelle: x,y,z rotations: roulis, tangage, lacet
        //Ne pas oublier de rafraichir ces coordonnées, et de les lire à la sortie
        static float ET_x,ET_y,ET_z,ET_r,ET_t,ET_l;
        static uint16_t trajectoire_courante;
        static vector<Trajectoire*> liste_trajectoires;

    public:
        static void init();
        static void ajoute_trajectoire(Trajectoire* trajectoire);
        static void destruction_trajectoires();
        static void affiche_trajectoires();
        static void affiche_trajectoire(uint16_t trajectoire_id,bool drapeau_active=false);
        static void edit_trajectoire(int key,bool etat);
};
#endif // EDITEUR_TRAJECTOIRE_H_INCLUDED
