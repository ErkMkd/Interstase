#include "Primitives.h"

using namespace std;

//============================================================================================
//          Coordonnées des formes trop simples pour nécessiter des fonctions génératrices
//============================================================================================

float Primitives::plan_sommets[2*4]={-.5,-.5, .5,-.5, .5,.5, -.5,.5};

float Primitives::triangle_sommets[2*3]={0.,1.,
                                -0.8660254,-0.5,
                                0.8660254,-0.5};

float Primitives::cube_sommets[3*8]={-0.5,0.5,-0.5,
                                0.5,0.5,-0.5,
                                0.5,-0.5,-0.5,
                                -0.5,-0.5,-0.5,
                                -0.5,0.5,0.5,
                                -0.5,-0.5,0.5,
                                0.5,-0.5,0.5,
                                0.5,0.5,0.5};

uint16_t Primitives::cube_faces[4*6]={3,2,1,0,
                                2,6,7,1,
                                7,6,5,4,
                                4,5,3,0,
                                1,7,4,0,
                                3,5,6,2};

float Primitives::cube_normales[3*6]={0.,0.,-1.,
                                1.,0.,0.,
                                0.,0.,1.,
                                -1.,0.,0.,
                                0.,1.,0.,
                                0.,-1.,0.};

float Primitives::tetraedre_sommets[3*4]={-500.e-3,-177.8314e-3,-288.6751e-3,
                                            500.e-3,-177.8314e-3,-288.6751e-3,
                                            0.,-177.8314e-3,577.3503e-3,
                                            0.,533.4935e-3,0.};
float Primitives::tetraedre_normales[3*4]={0.,-1.,0.,
                                            0.,0.376041,-0.926603,
                                            -0.802462,0.376041,0.463301,
                                            0.802462,0.376041,0.463301};
uint16_t Primitives::tetraedre_faces[3*4]={0,2,1,
                                            0,1,3,
                                            0,3,2,
                                            1,2,3};
//=============================================================
//          Matériaux:
//=============================================================

float Primitives::materiau_amb[4]={0.2,0.2,0.2,1.};
float Primitives::materiau_diff[4]={0.8,0.8,0.8,1.};
float Primitives::materiau_lum[4]={0.,0.,0.,1.};
float Primitives::materiau_spec[4]={1.,1.,1.,1.};

//=============================================================
//          Génère les coordonnées de formes primitives
//=============================================================

void Primitives::genere_disque(float rayon,uint16_t numCotes,float* coordsTab, float y)
{
	float astep=2.*M_PI/numCotes;
	float a=0.;
	for(int i=0;i<numCotes;i++)
	{
		coordsTab[3*i]=rayon*cos(a);
		coordsTab[3*i+1]=y;
		coordsTab[3*i+2]=rayon*sin(a);
		a+=astep;
	}
}

void Primitives::genere_cylindre(float rayon, float hauteur, uint16_t numCotes, uint16_t numSections, float* coordsTab)
{
	float y=-hauteur/2.;
	float ystep=hauteur/(numSections-1);
	for(int i=0;i<numSections;i++)
	{
		float* coordsTabSegment=coordsTab+i*3*numCotes;
		genere_disque(rayon,numCotes,coordsTabSegment,y);
		y+=ystep;
	}
}

void Primitives::genere_faces_cylindre(uint16_t numCotes, uint16_t numSections, uint16_t* facesTab)
{
	uint32_t k=0;
	uint16_t i,j,p;
	uint16_t n=numCotes*numSections;
	for(i=0;i<numSections-1;i++)
	{
		uint16_t m=(i*numCotes);
		for(j=0;j<numCotes;j++)
		{
			p=(j+1)%numCotes;
			facesTab[k]=m+j;
			facesTab[k+1]=m+p;
			facesTab[k+2]=m+p+numCotes;
			facesTab[k+3]=m+j+numCotes;
			k+=4;
		}
	}
	for(i=0;i<numCotes;i++)
	{
		facesTab[k]=(numCotes-1)-i;
		facesTab[k+numCotes]=n-numCotes+i;
		k++;
	}
}

void Primitives::genere_normales_cylindre(uint32_t numPoints, float* coordsTab, float* normalesTab)
{
	for (uint32_t i=0;i<numPoints;i++)
	{
		float px=coordsTab[3*i];
		float pz=coordsTab[3*i+2];
		float n=sqrtf(px*px+pz*pz);
		normalesTab[3*i]=px/n;
		normalesTab[3*i+1]=0.;
		normalesTab[3*i+2]=pz/n;
	}
}



void Primitives::genere_tore(float rayon1,float rayon2, uint16_t numCotes, uint16_t numSections, float* coordsTab)
{
	float astep=M_PI*2./numCotes;
	float aSection=0.;
	for(int i=0;i<numCotes;i++)
	{
		float rSections=rayon1+rayon2*cos(aSection);
		float y=rayon2*sin(aSection);
		float* coordsTabSections=coordsTab+3*i*numSections;
		genere_disque(rSections,numSections,coordsTabSections,y);
		aSection+=astep;
	}
}

void Primitives::genere_faces_tore(uint16_t numCotes, uint16_t numSections, uint16_t* facesTab)
{
	uint32_t k=0;
	uint16_t i,j,p;
	uint16_t n=numSections*numCotes;
	for(i=0;i<numCotes;i++)
	{
		uint16_t m=(i*numSections);
		for(j=0;j<numSections;j++)
		{
			p=(j+1)%numSections;
			facesTab[k]=m+j;
			facesTab[k+1]=m+p;
			facesTab[k+2]=(m+p+numSections)%n;
			facesTab[k+3]=(m+j+numSections)%n;
			k+=4;
		}
	}
}

void Primitives::genere_normales_tore(float rayon,uint32_t numPoints,float* coordsTab, float* normalesTab)
{
	for (uint32_t i=0; i<numPoints;i++)
	{
		float px=coordsTab[3*i];
		float pz=coordsTab[3*i+2];
		float n=sqrtf(px*px+pz*pz);
		float cx=px/n*rayon;
		float cz=pz/n*rayon;
		float nx=px-cx;
		float ny=coordsTab[3*i+1];
		float nz=pz-cz;
		n=sqrtf(nx*nx+ny*ny+nz*nz);
		normalesTab[3*i]=nx/n;
		normalesTab[3*i+1]=ny/n;
		normalesTab[3*i+2]=nz/n;
	}
}

void Primitives::genere_sphere(float rayon, uint16_t numCotes, uint16_t numSegments, float* coordsTab)
{
	float astep=M_PI/numSegments;
	float aSections=astep;
	float r,y;
	coordsTab[0]=0.;
	coordsTab[1]=rayon;
	coordsTab[2]=0.;
	int dernier=3*(numCotes*(numSegments-1)+1);
	coordsTab[dernier]=0.;
	coordsTab[dernier+1]=-rayon;
	coordsTab[dernier+2]=0.;

	float* coordsTabSections=&coordsTab[3];
	for(int i=0;i<numSegments-1;i++)
	{
		y=rayon*cos(aSections);
		r=rayon*sin(aSections);
		float* coordsTabSection=&coordsTabSections[3*i*numCotes];
		genere_disque(r,numCotes,coordsTabSection,y);
		aSections+=astep;
	}
}

void Primitives::genere_faces_sphere(uint16_t numCotes, uint16_t numSegments, uint16_t* facesTab)
{
	uint32_t k=0;
	uint16_t i,j,p;
	uint16_t m;
	for(i=0;i<numSegments-2;i++)
	{
		m=(i*numCotes)+1;
		for(j=0;j<numCotes;j++)
		{
			p=(j+1)%numCotes;
			facesTab[k]=m+j;
			facesTab[k+1]=m+j+numCotes;
			facesTab[k+2]=m+p+numCotes;
			facesTab[k+3]=m+p;
			k+=4;
		}
	}

	m=1+numCotes*(numSegments-1);	//indice du dernier point

	for(i=0;i<numCotes-1;i++)
	{
		facesTab[k]=0;
		facesTab[k+1]=i+1;
		facesTab[k+2]=i+2;

		facesTab[k+numCotes*3]=m;
		facesTab[k+numCotes*3+1]=m-i-1;
		facesTab[k+numCotes*3+2]=m-i-2;

		k+=3;
	}
	facesTab[k]=0;
	facesTab[k+1]=numCotes;
	facesTab[k+2]=1;

	facesTab[k+numCotes*3]=m;
	facesTab[k+numCotes*3+1]=m-numCotes;
	facesTab[k+numCotes*3+2]=m-1;
}

void Primitives::genere_normales_sphere(uint32_t numPoints, float* coordsTab, float* normalesTab)
{
	for(uint32_t i=0;i<numPoints;i++)
	{
		float px=coordsTab[3*i];
		float py=coordsTab[3*i+1];
		float pz=coordsTab[3*i+2];
		float n=sqrtf(px*px+py*py+pz*pz);
		normalesTab[3*i]=px/n;
		normalesTab[3*i+1]=py/n;
		normalesTab[3*i+2]=pz/n;
	}
}


void Primitives::init_materiau_simple_face(float Ramb,float Vamb, float Bamb,float Aamb,
                                       float Rlum,float Vlum,float Blum,float Alum,
                                       float Rdiff,float Vdiff,float Bdiff,float Adiff,
                                       float Rspec,float Vspec,float Bspec,float Aspec,
                                       float materiau_brill)
{
    glEnable(GL_CULL_FACE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);

    materiau_amb[0]=Ramb;materiau_amb[1]=Vamb;materiau_amb[2]=Bamb;materiau_amb[3]=Aamb;
    materiau_lum[0]=Rlum;materiau_lum[1]=Vlum;materiau_lum[2]=Blum;materiau_lum[3]=Alum;
    materiau_diff[0]=Rdiff;materiau_diff[1]=Vdiff;materiau_diff[2]=Bdiff;materiau_diff[3]=Adiff;
    materiau_spec[0]=Rspec;materiau_spec[1]=Vspec;materiau_spec[2]=Bspec;materiau_spec[3]=Aspec;

    glColor4fv(materiau_diff);
    glMaterialfv(GL_FRONT,GL_AMBIENT,materiau_amb);
    glMaterialfv(GL_FRONT,GL_DIFFUSE, materiau_diff);
    glMaterialfv(GL_FRONT,GL_EMISSION, materiau_lum);
    glMaterialfv(GL_FRONT,GL_SPECULAR, materiau_spec);
    glMaterialf(GL_FRONT, GL_SHININESS, materiau_brill);
}

void Primitives::init_materiau_simple_face(uint32_t RVBAamb, uint32_t RVBAlum, uint32_t RVBAdiff, uint32_t RVBAspec,float materiau_brill)
{
    glEnable(GL_CULL_FACE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);

    materiau_amb[0]=float((RVBAamb>>24)&0xff)/255.;materiau_amb[1]=float((RVBAamb>>16)&0xff)/255.;materiau_amb[2]=float((RVBAamb>>8)&0xff)/255.;materiau_amb[3]=float(RVBAamb&0xff)/255.;
    materiau_lum[0]=float((RVBAlum>>24)&0xff)/255.;materiau_lum[1]=float((RVBAlum>>16)&0xff)/255.;materiau_lum[2]=float((RVBAlum>>8)&0xff)/255.;materiau_lum[3]=float(RVBAlum&0xff)/255.;
    materiau_diff[0]=float((RVBAdiff>>24)&0xff)/255.;materiau_diff[1]=float((RVBAdiff>>16)&0xff)/255.;materiau_diff[2]=float((RVBAdiff>>8)&0xff)/255.;materiau_diff[3]=float(RVBAdiff&0xff)/255.;
    materiau_spec[0]=float((RVBAspec>>24)&0xff)/255.;materiau_spec[1]=float((RVBAspec>>16)&0xff)/255.;materiau_spec[2]=float((RVBAspec>>8)&0xff)/255.;materiau_spec[3]=float(RVBAspec&0xff)/255.;

    glColor4fv(materiau_diff);
    glMaterialfv(GL_FRONT,GL_AMBIENT,materiau_amb);
    glMaterialfv(GL_FRONT,GL_DIFFUSE, materiau_diff);
    glMaterialfv(GL_FRONT,GL_EMISSION, materiau_lum);
    glMaterialfv(GL_FRONT,GL_SPECULAR, materiau_spec);
    glMaterialf(GL_FRONT, GL_SHININESS, materiau_brill);
}

void Primitives::init_materiau_double_face(float Ramb,float Vamb, float Bamb,float Aamb,
                               float Rlum,float Vlum,float Blum,float Alum,
                               float Rdiff,float Vdiff,float Bdiff,float Adiff,
                               float Rspec,float Vspec,float Bspec,float Aspec,
                               float materiau_brill)
{
    glDisable(GL_CULL_FACE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);

    materiau_amb[0]=Ramb;materiau_amb[1]=Vamb;materiau_amb[2]=Bamb;materiau_amb[3]=Aamb;
    materiau_lum[0]=Rlum;materiau_lum[1]=Vlum;materiau_lum[2]=Blum;materiau_lum[3]=Alum;
    materiau_diff[0]=Rdiff;materiau_diff[1]=Vdiff;materiau_diff[2]=Bdiff;materiau_diff[3]=Adiff;
    materiau_spec[0]=Rspec;materiau_spec[1]=Vspec;materiau_spec[2]=Bspec;materiau_spec[3]=Aspec;

    glColor4fv(materiau_diff);
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,materiau_amb);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, materiau_diff);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION, materiau_lum);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, materiau_spec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, materiau_brill);
}

void Primitives::init_materiau_double_face(uint32_t RVBAamb, uint32_t RVBAlum, uint32_t RVBAdiff, uint32_t RVBAspec,float materiau_brill)
{
    glDisable(GL_CULL_FACE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);

    materiau_amb[0]=float((RVBAamb>>24)&0xff)/255.;materiau_amb[1]=float((RVBAamb>>16)&0xff)/255.;materiau_amb[2]=float((RVBAamb>>8)&0xff)/255.;materiau_amb[3]=float(RVBAamb&0xff)/255.;
    materiau_lum[0]=float((RVBAlum>>24)&0xff)/255.;materiau_lum[1]=float((RVBAlum>>16)&0xff)/255.;materiau_lum[2]=float((RVBAlum>>8)&0xff)/255.;materiau_lum[3]=float(RVBAlum&0xff)/255.;
    materiau_diff[0]=float((RVBAdiff>>24)&0xff)/255.;materiau_diff[1]=float((RVBAdiff>>16)&0xff)/255.;materiau_diff[2]=float((RVBAdiff>>8)&0xff)/255.;materiau_diff[3]=float(RVBAdiff&0xff)/255.;
    materiau_spec[0]=float((RVBAspec>>24)&0xff)/255.;materiau_spec[1]=float((RVBAspec>>16)&0xff)/255.;materiau_spec[2]=float((RVBAspec>>8)&0xff)/255.;materiau_spec[3]=float(RVBAspec&0xff)/255.;

    glColor4fv(materiau_diff);
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,materiau_amb);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, materiau_diff);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION, materiau_lum);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, materiau_spec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, materiau_brill);
}
