#ifndef AFFICHAGESDEBOGUE_H_INCLUDED
#define AFFICHAGESDEBOGUE_H_INCLUDED

#include "../glew.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "math.h"

using namespace std;

class AffichagesDebogue
{
    public:
        static void affiche_point(float Ax,float Ay, float Az, uint32_t couleur);
        static void affiche_segment(float Ax,float Ay, float Az,
                                float Bx,float By, float Bz, uint32_t couleur);
        static void affiche_polygone(vector<uint32_t>* aretes,uint16_t nbrSommets, vector<float>* tabSommets,uint32_t couleur);
};

#endif // AFFICHAGESDEBOGUE_H_INCLUDED
