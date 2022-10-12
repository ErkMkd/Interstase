

#include "Lumieres.h"


uint32_t Lumieres::drapeaux_sources=0x00000000;

void Lumieres::lumiere_couleur(GLuint id_openGl,float amb_r,float amb_v,float amb_b,
										float diff_r,float diff_v,float diff_b,
										float spec_r,float spec_v,float spec_b)
{
	float amb[4]={amb_r,amb_v,amb_b,1.};
	float diff[4]={diff_r,diff_v,diff_b,1.};
	float spec[4]={spec_r,spec_v,spec_b,1.};
	glLightfv(id_openGl, GL_AMBIENT,  amb);
	glLightfv(id_openGl, GL_DIFFUSE,  diff);
    glLightfv(id_openGl, GL_SPECULAR, spec);
}

void Lumieres::lumiere_couleur(GLuint id_openGl,float amb[3],float diff[3],float spec[3])
{
    float c_amb[4]={amb[0],amb[1],amb[2],1.};
    float c_diff[4]={diff[0],diff[1],diff[2],1.};
    float c_spec[4]={spec[0],spec[1],spec[2],1.};
	glLightfv(id_openGl, GL_AMBIENT,  c_amb);
	glLightfv(id_openGl, GL_DIFFUSE,  c_diff);
    glLightfv(id_openGl, GL_SPECULAR, c_spec);
}

void Lumieres::lumiere_attenuation(GLuint id_openGl, float ac, float al, float as)
{
    glLightf(id_openGl,GL_CONSTANT_ATTENUATION,ac);
	glLightf(id_openGl,GL_LINEAR_ATTENUATION,al);
    glLightf(id_openGl,GL_QUADRATIC_ATTENUATION,as);
}

void Lumieres::lumiere_position(GLuint id_openGl,float pos[3])
{
	float position[4]={pos[0],pos[1],pos[2],1.};
	glLightfv(id_openGl, GL_POSITION, position);
}

void Lumieres::lumiere_position(GLuint id_openGl,float x,float y,float z)
{
	float position[4]={x,y,z,1.};
	glLightfv(id_openGl, GL_POSITION, position);
}

void Lumieres::lumiere_directionnelle_orientation(GLuint id_openGl,float dx,float dy,float dz)
{
	float direction[4]={dx,dy,dz,1.};
	glLightfv(id_openGl, GL_SPOT_DIRECTION, direction);
}

void Lumieres::lumiere_infinie_orientation(GLuint id_openGl,float dx,float dy,float dz)
{
	float direction[4]={dx,dy,dz,0.};
	glLightfv(id_openGl, GL_POSITION, direction);
}

void Lumieres::lumiere_infinie_orientation(GLuint id_openGl,float dir[3])
{
	float direction[4]={dir[0],dir[1],dir[2],0.};
	glLightfv(id_openGl, GL_POSITION, direction);
}


void Lumieres::lumiere_ouverture(GLuint id_openGl,float ouverture)	//180 indique que la lumière est omnidirectionnelle
{
	glLightf(id_openGl, GL_SPOT_CUTOFF,ouverture);
}




