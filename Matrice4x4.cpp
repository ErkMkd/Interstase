#include "Matrice4x4.h"

using namespace std;

//Structure de calcul de matrices:
//Cet incongru d'OpenGl inverse les lignes et les colonnes.

        //===================================
        //      CONSTRUCTEUR
        //===================================
        Matrice4x4d::Matrice4x4d(GLdouble a, GLdouble b, GLdouble c, GLdouble d,
                   GLdouble e, GLdouble f, GLdouble g, GLdouble h,
                   GLdouble i, GLdouble j, GLdouble k, GLdouble l,
                   GLdouble m, GLdouble n, GLdouble o, GLdouble p)
        {
            matrice[0]=a;
            matrice[4]=b;
            matrice[8]=c;
            matrice[12]=d;

            matrice[1]=e;
            matrice[5]=f;
            matrice[9]=g;
            matrice[13]=h;

            matrice[2]=i;
            matrice[6]=j;
            matrice[10]=k;
            matrice[14]=l;

            matrice[3]=m;
            matrice[7]=n;
            matrice[11]=o;
            matrice[15]=p;
        }

        void Matrice4x4d::identite()
        {
            matrice[0]=1.;matrice[4]=0.; matrice[8]=0.;matrice[12]=0.;
            matrice[1]=0.;matrice[5]=1.; matrice[9]=0.;matrice[13]=0.;
            matrice[2]=0.;matrice[6]=0.; matrice[10]=1.;matrice[14]=0.;
            matrice[3]=0.;matrice[7]=0.; matrice[11]=0.;matrice[15]=1.;
        }



        //La matrice d'entrée est située à droite:
        void Matrice4x4d::multiplication_droite(Matrice4x4d& m, Matrice4x4d& res)
        {
            res.matrice[0]=matrice[0]*m.matrice[0]+matrice[4]*m.matrice[1]+matrice[8]*m.matrice[2]+matrice[12]*m.matrice[3];
            res.matrice[4]=matrice[0]*m.matrice[4]+matrice[4]*m.matrice[5]+matrice[8]*m.matrice[6]+matrice[12]*m.matrice[7];
            res.matrice[8]=matrice[0]*m.matrice[8]+matrice[4]*m.matrice[9]+matrice[8]*m.matrice[10]+matrice[12]*m.matrice[11];
            res.matrice[12]=matrice[0]*m.matrice[12]+matrice[4]*m.matrice[13]+matrice[8]*m.matrice[14]+matrice[12]*m.matrice[15];

            res.matrice[1]=matrice[1]*m.matrice[0]+matrice[5]*m.matrice[1]+matrice[9]*m.matrice[2]+matrice[13]*m.matrice[3];
            res.matrice[5]=matrice[1]*m.matrice[4]+matrice[5]*m.matrice[5]+matrice[9]*m.matrice[6]+matrice[13]*m.matrice[7];
            res.matrice[9]=matrice[1]*m.matrice[8]+matrice[5]*m.matrice[9]+matrice[9]*m.matrice[10]+matrice[13]*m.matrice[11];
            res.matrice[13]=matrice[1]*m.matrice[12]+matrice[5]*m.matrice[13]+matrice[9]*m.matrice[14]+matrice[13]*m.matrice[15];

            res.matrice[2]=matrice[2]*m.matrice[0]+matrice[6]*m.matrice[1]+matrice[10]*m.matrice[2]+matrice[14]*m.matrice[3];
            res.matrice[6]=matrice[2]*m.matrice[4]+matrice[6]*m.matrice[5]+matrice[10]*m.matrice[6]+matrice[14]*m.matrice[7];
            res.matrice[10]=matrice[2]*m.matrice[8]+matrice[6]*m.matrice[9]+matrice[10]*m.matrice[10]+matrice[14]*m.matrice[11];
            res.matrice[14]=matrice[2]*m.matrice[12]+matrice[6]*m.matrice[13]+matrice[10]*m.matrice[14]+matrice[14]*m.matrice[15];

            res.matrice[3]=matrice[3]*m.matrice[0]+matrice[7]*m.matrice[1]+matrice[11]*m.matrice[2]+matrice[15]*m.matrice[3];
            res.matrice[7]=matrice[3]*m.matrice[4]+matrice[7]*m.matrice[5]+matrice[11]*m.matrice[6]+matrice[15]*m.matrice[7];
            res.matrice[11]=matrice[3]*m.matrice[8]+matrice[7]*m.matrice[9]+matrice[11]*m.matrice[10]+matrice[15]*m.matrice[11];
            res.matrice[15]=matrice[3]*m.matrice[12]+matrice[7]*m.matrice[13]+matrice[11]*m.matrice[14]+matrice[15]*m.matrice[15];


        }

        GLdouble* Matrice4x4d::renvoie_ligne(uint16_t n)
        {
            if (n>3)n=3;
            GLdouble* ligne=lignes[n];

            ligne[0]=matrice[0+n];ligne[1]=matrice[4+n];ligne[2]=matrice[8+n]; ligne[3]=matrice[12+n];
            return ligne;
        }

        void Matrice4x4d::affiche()
        {
            cout<<matrice[0]<<" , "<<matrice[4]<<" , "<<matrice[8]<<" , "<<matrice[12]<<endl;
            cout<<matrice[1]<<" , "<<matrice[5]<<" , "<<matrice[9]<<" , "<<matrice[13]<<endl;
            cout<<matrice[2]<<" , "<<matrice[6]<<" , "<<matrice[10]<<" , "<<matrice[14]<<endl;
            cout<<matrice[3]<<" , "<<matrice[7]<<" , "<<matrice[11]<<" , "<<matrice[15]<<endl;
            cout<<endl;
        }




        //===================================
        //      CONSTRUCTEUR
        //===================================
        Matrice4x4f::Matrice4x4f(GLfloat a, GLfloat b, GLfloat c, GLfloat d,
                   GLfloat e, GLfloat f, GLfloat g, GLfloat h,
                   GLfloat i, GLfloat j, GLfloat k, GLfloat l,
                   GLfloat m, GLfloat n, GLfloat o, GLfloat p)
        {
            matrice[0]=a;
            matrice[4]=b;
            matrice[8]=c;
            matrice[12]=d;

            matrice[1]=e;
            matrice[5]=f;
            matrice[9]=g;
            matrice[13]=h;

            matrice[2]=i;
            matrice[6]=j;
            matrice[10]=k;
            matrice[14]=l;

            matrice[3]=m;
            matrice[7]=n;
            matrice[11]=o;
            matrice[15]=p;
        }

        void Matrice4x4f::identite()
        {
            matrice[0]=1.;matrice[4]=0.; matrice[8]=0.;matrice[12]=0.;
            matrice[1]=0.;matrice[5]=1.; matrice[9]=0.;matrice[13]=0.;
            matrice[2]=0.;matrice[6]=0.; matrice[10]=1.;matrice[14]=0.;
            matrice[3]=0.;matrice[7]=0.; matrice[11]=0.;matrice[15]=1.;
        }

        //Le point doit avoir une composante w [x,y,z,x]
        void Matrice4x4f::multiplie_point3d(float* pointSrc,float* pointDst)
        {
            pointDst[0]=pointSrc[0]*matrice[0]+pointSrc[1]*matrice[4]+pointSrc[2]*matrice[8]+pointSrc[3]*matrice[12];
            pointDst[1]=pointSrc[0]*matrice[1]+pointSrc[1]*matrice[5]+pointSrc[2]*matrice[9]+pointSrc[3]*matrice[13];
            pointDst[2]=pointSrc[0]*matrice[2]+pointSrc[1]*matrice[6]+pointSrc[2]*matrice[10]+pointSrc[3]*matrice[14];
            pointDst[3]=pointSrc[0]*matrice[3]+pointSrc[1]*matrice[7]+pointSrc[2]*matrice[11]+pointSrc[3]*matrice[15];
        }

        //La matrice d'entrée est située à droite:
        void Matrice4x4f::multiplication_droite(Matrice4x4f& m, Matrice4x4f& res)
        {
            res.matrice[0]=matrice[0]*m.matrice[0]+matrice[4]*m.matrice[1]+matrice[8]*m.matrice[2]+matrice[12]*m.matrice[3];
            res.matrice[4]=matrice[0]*m.matrice[4]+matrice[4]*m.matrice[5]+matrice[8]*m.matrice[6]+matrice[12]*m.matrice[7];
            res.matrice[8]=matrice[0]*m.matrice[8]+matrice[4]*m.matrice[9]+matrice[8]*m.matrice[10]+matrice[12]*m.matrice[11];
            res.matrice[12]=matrice[0]*m.matrice[12]+matrice[4]*m.matrice[13]+matrice[8]*m.matrice[14]+matrice[12]*m.matrice[15];

            res.matrice[1]=matrice[1]*m.matrice[0]+matrice[5]*m.matrice[1]+matrice[9]*m.matrice[2]+matrice[13]*m.matrice[3];
            res.matrice[5]=matrice[1]*m.matrice[4]+matrice[5]*m.matrice[5]+matrice[9]*m.matrice[6]+matrice[13]*m.matrice[7];
            res.matrice[9]=matrice[1]*m.matrice[8]+matrice[5]*m.matrice[9]+matrice[9]*m.matrice[10]+matrice[13]*m.matrice[11];
            res.matrice[13]=matrice[1]*m.matrice[12]+matrice[5]*m.matrice[13]+matrice[9]*m.matrice[14]+matrice[13]*m.matrice[15];

            res.matrice[2]=matrice[2]*m.matrice[0]+matrice[6]*m.matrice[1]+matrice[10]*m.matrice[2]+matrice[14]*m.matrice[3];
            res.matrice[6]=matrice[2]*m.matrice[4]+matrice[6]*m.matrice[5]+matrice[10]*m.matrice[6]+matrice[14]*m.matrice[7];
            res.matrice[10]=matrice[2]*m.matrice[8]+matrice[6]*m.matrice[9]+matrice[10]*m.matrice[10]+matrice[14]*m.matrice[11];
            res.matrice[14]=matrice[2]*m.matrice[12]+matrice[6]*m.matrice[13]+matrice[10]*m.matrice[14]+matrice[14]*m.matrice[15];

            res.matrice[3]=matrice[3]*m.matrice[0]+matrice[7]*m.matrice[1]+matrice[11]*m.matrice[2]+matrice[15]*m.matrice[3];
            res.matrice[7]=matrice[3]*m.matrice[4]+matrice[7]*m.matrice[5]+matrice[11]*m.matrice[6]+matrice[15]*m.matrice[7];
            res.matrice[11]=matrice[3]*m.matrice[8]+matrice[7]*m.matrice[9]+matrice[11]*m.matrice[10]+matrice[15]*m.matrice[11];
            res.matrice[15]=matrice[3]*m.matrice[12]+matrice[7]*m.matrice[13]+matrice[11]*m.matrice[14]+matrice[15]*m.matrice[15];


        }

        GLfloat* Matrice4x4f::renvoie_ligne(uint16_t n)
        {
            if (n>3)n=3;
            GLfloat* ligne=lignes[n];

            ligne[0]=matrice[0+n];ligne[1]=matrice[4+n];ligne[2]=matrice[8+n]; ligne[3]=matrice[12+n];
            return ligne;
        }

        void Matrice4x4f::affiche()
        {
            cout<<matrice[0]<<" , "<<matrice[4]<<" , "<<matrice[8]<<" , "<<matrice[12]<<endl;
            cout<<matrice[1]<<" , "<<matrice[5]<<" , "<<matrice[9]<<" , "<<matrice[13]<<endl;
            cout<<matrice[2]<<" , "<<matrice[6]<<" , "<<matrice[10]<<" , "<<matrice[14]<<endl;
            cout<<matrice[3]<<" , "<<matrice[7]<<" , "<<matrice[11]<<" , "<<matrice[15]<<endl;
            cout<<endl;
        }
