
#include "Raytracer01.h"

void Raytracer01::init_couleur_sol(float Sol[TAILLE_OBJ],uint32_t c1,uint32_t c2)
{
    float r,v,b;
    r=float((c1>>16)&0xff)/255.;
    v=float((c1>>8)&0xff)/255.;
    b=float(c1&0xff)/255.;
    Sol[CR]=r;
    Sol[CV]=v;
    Sol[CB]=b;
    r=float((c2>>16)&0xff)/255.;
    v=float((c2>>8)&0xff)/255.;
    b=float(c2&0xff)/255.;
    Sol[C2R]=r;
    Sol[C2V]=v;
    Sol[C2B]=b;
}

void Raytracer01::init_Sol(float Sol[TAILLE_OBJ],float carX,float carZ,uint32_t c1RVB,uint32_t c2RVB, float diff, float spec, float brill, float transp, float reflec, float refrac)
{
    float c1r,c1v,c1b,c2r,c2v,c2b;
    c1r=float((c1RVB>>16)&0xff)/255.;
    c1v=float((c1RVB>>8)&0xff)/255.;
    c1b=float(c1RVB&0xff)/255.;
    c2r=float((c2RVB>>16)&0xff)/255.;
    c2v=float((c2RVB>>8)&0xff)/255.;
    c2b=float(c2RVB&0xff)/255.;
    Raytracer01::init_Sol(Sol,carX,carZ,c1r,c1v,c1b,c2r,c2v,c2b,diff,spec,brill,transp,reflec,refrac);
}

void Raytracer01::init_Sol(float Sol[TAILLE_OBJ],float carX,float carZ,float c1r,float c1v,float c1b,float c2r,float c2v,float c2b, float diff, float spec, float brill, float transp, float reflec, float refrac)
{
    Sol[OBJTYPE]=OBJTYPE_SOL;

	Sol[CR]=c1r;
	Sol[CV]=c1v;
	Sol[CB]=c1b;

	Sol[DIFF]=diff;
	Sol[SPEC]=spec;
	Sol[BRILL]=brill;

	Sol[TRANSP]=transp;
	Sol[REFLEC]=reflec;
	Sol[REFRAC]=refrac;

	Sol[C2R]=c2r;
	Sol[C2V]=c2v;
	Sol[C2B]=c2b;
	Sol[CAR_XL]=carX;
	Sol[CAR_ZL]=carZ;
}
void Raytracer01::init_ciel(float Ciel[TAILLE_CIEL],uint32_t zenith,uint32_t horizH,uint32_t horizB, uint32_t nadir)
{
    float r,v,b;
    r=float((zenith>>16)&0xff)/255.;
    v=float((zenith>>8)&0xff)/255.;
    b=float(zenith&0xff)/255.;
    Ciel[ZENITH_CR]=r;
    Ciel[ZENITH_CV]=v;
    Ciel[ZENITH_CB]=b;
    r=float((horizH>>16)&0xff)/255.;
    v=float((horizH>>8)&0xff)/255.;
    b=float(horizH&0xff)/255.;
    Ciel[HORIZH_CR]=r;
    Ciel[HORIZH_CV]=v;
    Ciel[HORIZH_CB]=b;
    r=float((horizB>>16)&0xff)/255.;
    v=float((horizB>>8)&0xff)/255.;
    b=float(horizB&0xff)/255.;
    Ciel[HORIZB_CR]=r;
    Ciel[HORIZB_CV]=v;
    Ciel[HORIZB_CB]=b;
    r=float((nadir>>16)&0xff)/255.;
    v=float((nadir>>8)&0xff)/255.;
    b=float(nadir&0xff)/255.;
    Ciel[NADIR_CR]=r;
    Ciel[NADIR_CV]=v;
    Ciel[NADIR_CB]=b;
}

void Raytracer01::init_C_amb(float Camb[3], uint32_t c)
{
    float r,v,b;
    r=float((c>>16)&0xff)/255.;
    v=float((c>>8)&0xff)/255.;
    b=float(c&0xff)/255.;
    Camb[0]=r;
    Camb[1]=v;
    Camb[2]=b;
}

void Raytracer01::init_couleur_lum(float L[TAILLE_LUM],uint32_t c, float i)
{
    float r,v,b;
    r=float((c>>16)&0xff)/255.;
    v=float((c>>8)&0xff)/255.;
    b=float(c&0xff)/255.;
    L[CR]=r;
    L[CV]=v;
    L[CB]=b;
    if(i>0.)L[INTENS]=i;
}

void Raytracer01::init_couleur_lum(float L[TAILLE_LUM],float r,float v,float b, float i)
{
    L[CR]=r;
    L[CV]=v;
    L[CB]=b;
    if(i>0.)L[INTENS]=i;
}

void Raytracer01::init_lum(float L[TAILLE_LUM],float x, float y, float z, float cr,float cv,float cb,float i,float att )
{
	L[OBJTYPE]=OBJTYPE_LUM;
	L[INTENS]=i;
	L[CR]=cr;
	L[CV]=cv;
	L[CB]=cb;
	L[POSX]=x;
	L[POSY]=y;
	L[POSZ]=z;
	L[ATT]=att;
}

void Raytracer01::init_lum_position(float L[TAILLE_LUM],float x, float y, float z)
{
    L[POSX]=x;
	L[POSY]=y;
	L[POSZ]=z;
}

void Raytracer01::init_lum_position(float L[TAILLE_LUM],float pos[3])
{
    L[POSX]=pos[0];
	L[POSY]=pos[1];
	L[POSZ]=pos[2];
}


void Raytracer01::init_sphere(float S[TAILLE_OBJ],float r, float x,float y,float z, float cr,float cv,float cb, float diff, float spec, float brill,float transp,float reflec, float refrac)
{
	S[OBJTYPE]=OBJTYPE_SPHERE;
	S[RAY]=r;
	S[POSX]=x;
	S[POSY]=y;
	S[POSZ]=z;

	S[CR]=cr;
	S[CV]=cv;
	S[CB]=cb;

	S[DIFF]=diff;
	S[SPEC]=spec;
	S[BRILL]=brill;

	S[TRANSP]=transp;
	S[REFLEC]=reflec;
	S[REFRAC]=refrac;
}

void Raytracer01::init_sphere(float S[TAILLE_OBJ],float r, float x,float y,float z, uint32_t c, float diff, float spec, float brill,float transp,float reflec, float refrac)
{
    float cr,cv,cb;
    cr=float((c>>16)&0xff)/255.;
    cv=float((c>>8)&0xff)/255.;
    cb=float(c&0xff)/255.;
    init_sphere(S,r,x,y,z,cr,cv,cb,diff,spec,brill,transp,reflec,refrac);
}

void Raytracer01::init_sphere_material(float S[TAILLE_OBJ],float diff, float spec, float brill,float transp,float reflec, float refrac)
{
	S[OBJTYPE]=OBJTYPE_SPHERE;

	S[DIFF]=diff;
	S[SPEC]=spec;
	S[BRILL]=brill;

	S[TRANSP]=transp;
	S[REFLEC]=reflec;
	S[REFRAC]=refrac;
}
