#ifndef MATRICE4X4_H_INCLUDED
#define MATRICE4X4_H_INCLUDED

#include "GL/GL.h"
#include <iostream>
using namespace std;

class Matrice4x4d
{
    public:
        GLdouble matrice[16];
    private:
        GLdouble lignes[4][4];

    public:


        Matrice4x4d(GLdouble a=1., GLdouble b=0., GLdouble c=0., GLdouble d=0.,
                   GLdouble e=0., GLdouble f=1., GLdouble g=0., GLdouble h=0.,
                   GLdouble i=0., GLdouble j=0., GLdouble k=1., GLdouble l=0.,
                   GLdouble m=0., GLdouble n=0., GLdouble o=0., GLdouble p=1.);

        void identite();
        void multiplication_droite(Matrice4x4d& m, Matrice4x4d& res);
        GLdouble* renvoie_ligne(uint16_t n);
        void affiche();
};

class Matrice4x4f
{
    public:
        GLfloat matrice[16];
    private:
        GLfloat lignes[4][4];

    public:
        Matrice4x4f(GLfloat a=1., GLfloat b=0., GLfloat c=0., GLfloat d=0.,
                   GLfloat e=0., GLfloat f=1., GLfloat g=0., GLfloat h=0.,
                   GLfloat i=0., GLfloat j=0., GLfloat k=1., GLfloat l=0.,
                   GLfloat m=0., GLfloat n=0., GLfloat o=0., GLfloat p=1.);

        void identite();
        void multiplication_droite(Matrice4x4f& m, Matrice4x4f& res);
        void multiplie_point3d(float* pointSrc,float* pointDst);
        GLfloat* renvoie_ligne(uint16_t n);
        void affiche();
};

#endif // MATRICE4X4_H_INCLUDED
