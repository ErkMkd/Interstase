#ifndef SURFACE_H_INCLUDED
#define SURFACE_H_INCLUDED



/// Les textures de la surface sont d�truites par celle-ci !!!

//----------------------------------
//  Structure des surfaces:
//
//  Une surface ne peut �tre rattach�e qu'� un seul volume.
//  Si la surface est opaque, il n'y a pas de tri des faces.
//  Si la surface est transparente, ses faces sont stock�es dans la table des faces transparentes du volume.
//     Une surface est consid�r�e comme transparente si:
//                  -son niveau de transparence > 0,
//                  -la texture de couleur a une couche alpha non uniforme ou <> 0xff
//                  -il y a une texture de transparence
//
//          a priori, la transparence d'un fragment est calcul� comme suit:
//                  Tf=transparence+Af+Tt avec une limitation � 1. Af=Alpha, Tt=transparence de la texture de transparence
//
// Le nombre maximum de textures prises en compte d�pend de l'impl�mentation de gestion du mode "Multitextures" d'OpenGl.
// En cas de nombre de textures sup�rieur au nombre maximum de textures max d'OpenGl, un ordre de priorit� est appliqu�,
// correspondant � l'ordre de d�finition des vecteurs de textures ci-apr�s.
//
//----------------------------------

#include "Texture.h"
#include "Face.h"
#include <list>

#define SURFACE_OK 0

//--------------------------------------------
//      Constantes de gestion des rendus
//--------------------------------------------

#define SURFACE_NBR_RENDUS 128

//Drapeaux de rendu des surfaces:
#define SURFACE_BIT_TEXTURE_COULEUR 0
#define SURFACE_BIT_RUGOSITE 1
#define SURFACE_BIT_REFLEXION_CUBE 2
#define SURFACE_BIT_ONDES 3
#define SURFACE_BIT_OMBRES 4
#define SURFACE_BIT_PHONG  5
#define SURFACE_BIT_TRANSITION 6 //Transition entre deux textures de couleur, avec une texture de modulation de transition

#define SURFACE_MASK_TEXTURE_COULEUR (1<<SURFACE_BIT_TEXTURE_COULEUR)
#define SURFACE_MASK_RUGOSITE (1<<SURFACE_BIT_RUGOSITE)
#define SURFACE_MASK_REFLEXION_CUBE (1<<SURFACE_BIT_REFLEXION_CUBE)
#define SURFACE_MASK_ONDES (1<<SURFACE_BIT_ONDES)
#define SURFACE_MASK_OMBRES (1<<SURFACE_BIT_OMBRES)
#define SURFACE_MASK_PHONG (1<<SURFACE_BIT_PHONG)
#define SURFACE_MASK_TRANSITION (1<<SURFACE_BIT_TRANSITION)

//Modes de rendus des surfaces:

#define SURFACE_RENDU_COULEUR                                    0x0000
#define SURFACE_RENDU_TEXTURE_COULEUR                            0x0001
#define SURFACE_RENDU_RUGOSITE                                   0x0002
#define SURFACE_RENDU_RUGOSITE_TEXTURE_COULEUR                   0x0003
#define SURFACE_RENDU_REFLEXION_CUBE                             0x0004
#define SURFACE_RENDU_REFLEXION_CUBE_TEXTURE_COULEUR             0x0005
#define SURFACE_RENDU_REFLEXION_CUBE_RUGOSITE                    0x0006
#define SURFACE_RENDU_REFLEXION_CUBE_RUGOSITE_TEXTURE_COULEUR    0x0007

#define SURFACE_RENDU_ONDES                                          0x0008
#define SURFACE_RENDU_ONDES_TEXTURE_COULEUR                          0x0009
#define SURFACE_RENDU_ONDES_RUGOSITE                                 0x000A
#define SURFACE_RENDU_ONDES_RUGOSITE_TEXTURE_COULEUR                 0x000B
#define SURFACE_RENDU_ONDES_REFLEXION_CUBE                           0x000C
#define SURFACE_RENDU_ONDES_REFLEXION_CUBE_TEXTURE_COULEUR           0x000D
#define SURFACE_RENDU_ONDES_REFLEXION_CUBE_RUGOSITE                  0x000E
#define SURFACE_RENDU_ONDES_REFLEXION_CUBE_RUGOSITE_TEXTURE_COULEUR  0x000F

#define SURFACE_RENDU_OMBRES                                         0x0010
#define SURFACE_RENDU_OMBRES_TEXTURE_COULEUR                         0x0011
#define SURFACE_RENDU_OMBRES_RUGOSITE                                0x0012
#define SURFACE_RENDU_OMBRES_RUGOSITE_TEXTURE_COULEUR                0x0013
#define SURFACE_RENDU_OMBRES_REFLEXION_CUBE                          0x0014
#define SURFACE_RENDU_OMBRES_REFLEXION_CUBE_TEXTURE_COULEUR          0x0015
#define SURFACE_RENDU_OMBRES_REFLEXION_CUBE_RUGOSITE                 0x0016
#define SURFACE_RENDU_OMBRES_REFLEXION_CUBE_RUGOSITE_TEXTURE_COULEUR 0x0017

#define SURFACE_RENDU_OMBRES_ONDES                                           0x0018
#define SURFACE_RENDU_OMBRES_ONDES_TEXTURE_COULEUR                           0x0019
#define SURFACE_RENDU_OMBRES_ONDES_RUGOSITE                                  0x001A
#define SURFACE_RENDU_OMBRES_ONDES_RUGOSITE_TEXTURE_COULEUR                  0x001B
#define SURFACE_RENDU_OMBRES_ONDES_REFLEXION_CUBE                            0x001C
#define SURFACE_RENDU_OMBRES_ONDES_REFLEXION_CUBE_TEXTURE_COULEUR            0x001D
#define SURFACE_RENDU_OMBRES_ONDES_REFLEXION_CUBE_RUGOSITE                   0x001E
#define SURFACE_RENDU_OMBRES_ONDES_REFLEXION_CUBE_RUGOSITE_TEXTURE_COULEUR   0x001F

#define SURFACE_RENDU_PHONG                                              0x0020
#define SURFACE_RENDU_PHONG_TEXTURE_COULEUR                              0x0021

//#define SURFACE_RENDU_PHONG_RUGOSITE                                   0x0022    ///Inutilis�
#define SURFACE_RENDU_EFFET_SPECIAL                                      0x0022    ///Mode de rendu des effets sp�ciaux

//#define SURFACE_RENDU_PHONG_RUGOSITE_TEXTURE_COULEUR                   0x0023    ///Inutilis�
#define SURFACE_RENDU_APLAT                                              0x0023    ///A la place de SURFACE_RENDU_PHONG_RUGOSITE_TEXTURE_COULEUR

#define SURFACE_RENDU_PHONG_REFLEXION_CUBE                               0x0024
#define SURFACE_RENDU_PHONG_REFLEXION_CUBE_TEXTURE_COULEUR               0x0025
//#define SURFACE_RENDU_PHONG_REFLEXION_CUBE_RUGOSITE                    0x0026    ///Inutilis�
//#define SURFACE_RENDU_PHONG_REFLEXION_CUBE_RUGOSITE_TEXTURE_COULEUR    0x0027    ///Inutilis�

/*Inutilis�s
#define SURFACE_RENDU_PHONG_ONDES                                          0x0028
#define SURFACE_RENDU_PHONG_ONDES_TEXTURE_COULEUR                          0x0029
#define SURFACE_RENDU_PHONG_ONDES_RUGOSITE                                 0x002A
#define SURFACE_RENDU_PHONG_ONDES_RUGOSITE_TEXTURE_COULEUR                 0x002B
#define SURFACE_RENDU_PHONG_ONDES_REFLEXION_CUBE                           0x002C
#define SURFACE_RENDU_PHONG_ONDES_REFLEXION_CUBE_TEXTURE_COULEUR           0x002D
#define SURFACE_RENDU_PHONG_ONDES_REFLEXION_CUBE_RUGOSITE                  0x002E
#define SURFACE_RENDU_PHONG_ONDES_REFLEXION_CUBE_RUGOSITE_TEXTURE_COULEUR  0x002F
*/

#define SURFACE_RENDU_PHONG_OMBRES                                         0x0030
#define SURFACE_RENDU_PHONG_OMBRES_TEXTURE_COULEUR                         0x0031
//#define SURFACE_RENDU_PHONG_OMBRES_RUGOSITE                                0x0032    ///Inutilis�
#define SURFACE_RENDU_PHONG_OMBRES_RUGOSITE_TEXTURE_COULEUR                0x0033
#define SURFACE_RENDU_PHONG_OMBRES_REFLEXION_CUBE                          0x0034
#define SURFACE_RENDU_PHONG_OMBRES_REFLEXION_CUBE_TEXTURE_COULEUR          0x0035
//#define SURFACE_RENDU_PHONG_OMBRES_REFLEXION_CUBE_RUGOSITE                 0x0036    ///Inutilis�
//#define SURFACE_RENDU_PHONG_OMBRES_REFLEXION_CUBE_RUGOSITE_TEXTURE_COULEUR 0x0037    ///Inutilis�

/*Inutilis�s
#define SURFACE_RENDU_PHONG_OMBRES_ONDES                                           0x0038
#define SURFACE_RENDU_PHONG_OMBRES_ONDES_TEXTURE_COULEUR                           0x0039
#define SURFACE_RENDU_PHONG_OMBRES_ONDES_RUGOSITE                                  0x004A
#define SURFACE_RENDU_PHONG_OMBRES_ONDES_RUGOSITE_TEXTURE_COULEUR                  0x004B
#define SURFACE_RENDU_PHONG_OMBRES_ONDES_REFLEXION_CUBE                            0x004C
#define SURFACE_RENDU_PHONG_OMBRES_ONDES_REFLEXION_CUBE_TEXTURE_COULEUR            0x004D
#define SURFACE_RENDU_PHONG_OMBRES_ONDES_REFLEXION_CUBE_RUGOSITE                   0x004E
#define SURFACE_RENDU_PHONG_OMBRES_ONDES_REFLEXION_CUBE_RUGOSITE_TEXTURE_COULEUR   0x004F
*/

//Effets(priorit� sur le rendu):
#define SURFACE_RENDU_TRANSITION_TEXTURES_COULEUR 50


//=======================================================================================
//
//=======================================================================================

class Surface
{
    private:
        uint16_t bits_rendus;   //Drapeaux constituant le code du shader utilis� pour la surface
                                //Le mode de rendu est g�n�r� � partir de ces bits, car il y a parfois des adaptations � faire (shaders incompatibles)
    public:
        uint16_t erreur;
        char nom[256];
        Volume* volume;  //Le volume auquel appartient la surface
        uint16_t mode_rendu;    //Code du renddksEffetSpecial* effet_special;
        bool drapeau_recoit_ombres;  //True si la surface peut recevoir des ombres port�es.
        bool drapeau_projete_ombres;
        bool drapeau_phong;     //True si la surface est �clair�e en Phong
        bool drapeau_effet_special; //True si un effet sp�cial est utilis� pour la surface (priorit� sur les autres rendus)
        bool drapeau_transition;    //True si la transition entre deux textures est activ�e

        bool double_face;   //Affiche les deux c�t�s des faces
        bool drapeau_adoucissement; //True si les faces sont adoucies
        bool drapeau_transparence;  //Champ initialis� par le volume - True si la surface est transparente.

        float angle_limite_adoucissement;   //en radians
        GLfloat* normales_sommets; //Table utilis�e pour le mode de rendu par tableaux de pointeurs
        uint32_t nbrNormales;

        float couleur_diffusion[4];
        float couleur_speculaire[4];
        float couleur_ambiance[4]; //R�flexion de la lumi�re ambiante
        float couleur_luminescence[4]; //Luminescence de la surface

        float luminescence;
        float diffusion;
        float ambiance;
        float specularite;

        float brillance;    //=Shininess
        float transparence;
        float transparence_speculaire;
        float translucidite; //Comment le g�rer?
        float reflexion;// G�r�e par les shaders
        float rugosite; //"Bump", g�r� par les shaders

        //Effet d'ondulations aqueuses
        float intensite_ondes;  //Mettre � 0 pour d�sactiver l'effet
        float amplitude_ondes;  //d�faut: 0.2
        float frequence_ondes;  //d�faut: 2
        float vitesse_ondes;    //d�faut: 1
        float quantite_ondes;   //d�faut: 5
        float perturbation_ondes; //d�faut: 10
        float echelle_perturbation_ondes; //d�faut: 10

        //Effet de transition entre 2 textures = SURFACE_RENDU_TRANSITION_TEXTURES_COULEUR
        // -1 si pas de texture au poste respectif. (c'est alors la couleur de la surface qui peut servir...)
        int32_t indice_texture_depart;
        int32_t indice_texture_destination;
        int32_t indice_texture_transition;     // texture utilis�e comme masque de transition
        float niveau_transition;    //0: texture depart, 1:texture destination


        //Rendus avec une texture de couleur:
        uint32_t indice_texture_couleur;    //L'indice de la texture de couleur utilis�e par les rendus avec une texture de couleur

        //Table globale des textures:
        vector<Texture*> textures;

        //Textures actives:
        vector<Texture*> textures_couleur;       //Image RVB,ARVB,Gris
        vector<Texture*> textures_diffusion;     //Image en niveaux de gris
        vector<Texture*> textures_ambiance;      //NG
        vector<Texture*> textures_luminescence;   //NG
        vector<Texture*> textures_transparence;  //NG
        vector<Texture*> textures_reflexion;     //NG
        vector<Texture*> textures_rugosite;      //Plusieurs modes de rugosit� possibles: par d�callage (NG) ou d�viation de normales (RVB, ARVB)

        //Textures inactives:
        vector<Texture*> textures_couleur_inactives;
        vector<Texture*> textures_diffusion_inactives;
        vector<Texture*> textures_ambiance_inactives;
        vector<Texture*> textures_luminescence_inactives;
        vector<Texture*> textures_transparence_inactives;
        vector<Texture*> textures_reflexion_inactives;
        vector<Texture*> textures_rugosite_inactives;

        //Sommets li�s � la surface: sert pour les g�n�rateurs de cooronn�es UV de textures - table initialis�e par Volume::initialise_indices_sommets_surfaces()
        vector<uint32_t> indices_sommets;
        //faces li�es � la surface: Ces champs sont remplis automatiquement par le volume.
        uint32_t nbr_faces;
        vector<Face*> faces;

        //Ces tableaux ne servent que pour le rendu en mode imm�diat (donc aussi pour g�n�rer les listes openGl...)
        vector<Face*> faces_adoucies;
        vector<Face*> faces_partiellement_adoucies; //Les faces dont une ou plusieurs ar�tes forment un angle > � l'angle d'adoucissement.


    public:
        Surface(char* p_nom=NULL);
        Surface(Surface& surface_originale);
        ~Surface();
        void initRVBA(float* couleur,float r, float v, float b, float a=1.);
        void determine_couleur_ambiance(uint32_t couleur, float intensite=0.2);
        void determine_couleur_diffusion(uint32_t couleur, float intensite=1.);
        void determine_couleur_speculaire(uint32_t couleur, float intensite=0.5);
        void determine_couleur_luminescence(uint32_t couleur, float intensite=0.2);
        void determine_ambiance(float intensite);
        void determine_diffusion(float intensite);
        void determine_specularite(float intensite);
        void determine_luminescence(float intensite);
        void determine_transparence(float p_transparence);
        void determine_reflexion(float p_reflexion);
        void determine_transparence_speculaire(float p_transparence);
        void determine_intensite_ondes(float p_intensite);
        void applique_teinte_diffusion(uint32_t teinte);
        void applique_teinte_specularite(uint32_t teinte);
        void applique_teinte_luminescence(uint32_t teinte);
        void applique_teinte_ambiance(uint32_t teinte);
        Texture* renvoie_texture_nom(char* nom_texture);
        void ajoute_texture(Texture* texture);
        bool texture_est_active(Texture* texture);
        uint16_t renvoie_texture_active_index(Texture* texture);
        uint16_t renvoie_texture_inactive_index(Texture* texture);
        void desactive_texture(Texture* texture);
        void active_texture(Texture* texture);
        void active_toutes_les_textures();
        void rafraichi_faces_adoucies();
        void ajoute_face_adoucie(Face* face);
        void ajoute_face_partiellement_adoucie(Face* face);
        bool alloue_tableau_normales_sommets(uint32_t p_nbrSommets);
        void inverse_normales();
        void determine_phong(bool p_drapeau);
        void determine_transition(bool p_drapeau);
        void initialise_mode_rendu();
        void determine_recoit_ombres(bool d);
        void determine_projete_ombres(bool d);
        void determine_ombre_portees(bool d);
        void affiche_debug();

    private:
        void determine_couleur_ARVB(float* tab,uint32_t couleur);
        void applique_teinte_ARVB(float* tab, uint32_t teinte);
};




#endif // SURFACE_H_INCLUDED
