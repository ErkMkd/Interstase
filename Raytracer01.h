#ifndef RAYTRACER01_H_INCLUDED
#define RAYTRACER01_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <iostream>

//Ciel
#define ZENITH_CR	0
#define ZENITH_CV	1
#define ZENITH_CB	2

#define HORIZH_CR	3
#define HORIZH_CV	4
#define HORIZH_CB	5

#define HORIZB_CR	6
#define HORIZB_CV	7
#define HORIZB_CB	8

#define NADIR_CR	9
#define NADIR_CV	10
#define NADIR_CB	11

#define TAILLE_CIEL	12

//Types d'objets:
#define OBJTYPE_SOL 0
#define OBJTYPE_SPHERE 1
#define OBJTYPE_LUM 2

//Structure de définition des objets:
#define	OBJTYPE	0
#define POSX 	1
#define POSY 	2
#define POSZ 	3
#define CR	 	4
#define CV		5
#define CB		6

//Luminosité:
#define INTENS	7
#define ATT 	8

#define TAILLE_LUM		9	//La structure des sources lumineuses s'arrête ici


//Matériau:
#define DIFF	9
#define SPEC	10
#define BRILL	11
#define TRANSP	12
#define REFLEC	13
#define REFRAC	14

//Données supplémentaires des sphères:
#define RAY		15

//Données supplémentaires pour les textures
#define C2R		15	//Couleur n°2
#define C2V		16
#define C2B		17
#define CAR_XL	18	//Largeur des carreaux
#define CAR_ZL	19	//Profondeur


#define TAILLE_OBJ		20


class Raytracer01
{
    public:
        static void init_couleur_sol(float Sol[TAILLE_OBJ],uint32_t c1,uint32_t c2);
        static void init_Sol(float Sol[TAILLE_OBJ],float carX,float carZ,uint32_t c1RVB,uint32_t c2RVB, float diff, float spec, float brill, float transp, float reflec, float refrac);
        static void init_Sol(float Sol[TAILLE_OBJ],float carX,float carZ,float c1r,float c1v,float c1b,float c2r,float c2v,float c2b, float diff, float spec, float brill, float transp, float reflec, float refrac);
        static void init_ciel(float Ciel[TAILLE_CIEL],uint32_t zenith,uint32_t horizH,uint32_t horizB, uint32_t nadir);
        static void init_C_amb(float Camb[3],uint32_t c);
        static void init_couleur_lum(float L[TAILLE_LUM],float r,float v,float b, float i=-1.);
        static void init_couleur_lum(float L[TAILLE_LUM],uint32_t c, float i=-1.);
        static void init_lum(float L[TAILLE_LUM],float x, float y, float z, float cr,float cv,float cb,float i,float att );
        static void init_lum_position(float L[TAILLE_LUM],float x, float y, float z);
        static void init_lum_position(float L[TAILLE_LUM],float pos[3]);
        static void init_sphere(float S[TAILLE_OBJ],float r, float x,float y,float z, float cr,float cv,float cb, float diff, float spec, float brill,float transp,float reflec, float refrac);
        static void init_sphere(float S[TAILLE_OBJ],float r, float x,float y,float z, uint32_t c, float diff, float spec, float brill,float transp,float reflec, float refrac);
        static void init_sphere_material(float S[TAILLE_OBJ],float diff, float spec, float brill,float transp,float reflec, float refrac);
};
#endif // RAYTRACER01_H_INCLUDED
