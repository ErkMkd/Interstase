#ifndef REPERE_H_INCLUDED
#define REPERE_H_INCLUDED


#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "glew.h"
#include <GLFW/glfw3.h>
#include "math.h"
#include "vec3f.h"

using namespace std;
class Repere
{
	public:
		float x;
		float y;
		float z;
		float r;
		float t;
		float l;
		float ex;
		float ey;
		float ez;

    private:
        vec3f axe_x; //Sert pour les calculs de changements de repères
        vec3f axe_y;
        vec3f axe_z;

    public:

        Repere(float p_x=0., float p_y=0., float p_z=0., float p_r=0., float p_t=0., float p_l=0.);
        void initXYZ(float p_x, float p_y, float p_z);
        void initXYZ(float xyz[3]);
        void initRTL(float p_r, float p_t, float p_l);
        void initRTL(float rtl[3]);
        void copieRepere(Repere& source);
        void visePoint(float px, float py, float pz);
        void visePoint(float p[3]);
        void visePoint(vec3f &p);
        void calcul_matrice();

        void calcul_repere_vectoriel();
        void translation_repere(Repere* parent);
        void translate_point_orientation(float p[3]);   //Le point est translaté uniquement par l'orientation du repère.
        void calcul_repere_relatif(Repere* parent);
        void affiche_repere_vectoriel();

    private:
        void axe_relatif(vec3f* axe,Repere* parent);
        void rotation_rtl_axe_repere_vectoriel(vec3f& axe);
        float renvoie_roulis_relatif();
        float renvoie_tangage_relatif();
        float renvoie_lacet_relatif();


};

#endif // REPERE_H_INCLUDED
