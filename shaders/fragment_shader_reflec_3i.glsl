
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
#define ATT 		8

#define TAILLE_LUM		9	//La structure des sources lumineuses s'arrête ici

//Matériau:
#define DIFF		9
#define SPEC	10
#define BRILL	11
#define TRANSP	12
#define REFLEC	13
#define REFRAC	14

//Structure des sphères:
#define RAY		15

#define TAILLE_OBJ		16

#define NUM_OBJETS		7
#define NUM_LUM		1

uniform int numReflec;	//Nombre d'itération de réflections (de 0 à 3)
uniform int numRefrac;	//Nombre d'itération de réfraction (de 0 à 3)
uniform mat4 Obs;		//Matrice de l'observateur
uniform  vec2 ecranDim;	//Dimensions de l'écran
uniform float distanceFocale;
uniform float ecran_ratio;

uniform vec3 C_ciel;
uniform float L_amb;
uniform vec3 C_amb;

uniform float L1[TAILLE_LUM];

uniform float objets[TAILLE_OBJ*NUM_OBJETS];

float refrac_amb=1.	;//indice de réfraction entre les objets

struct Ray
{
	vec3 pos;
	vec3 dir;
	float refrac_i;	//Indice de réfraction du milieu dans lequel se propage le rayon
};

struct Intersection
{
	float zDist;
	vec3 P;
	vec3 N;
	vec3 reflec;	//Direction du rayon refleté
	vec3 refrac;	//Direction du rayon réfracté
	float refrac_i;	//Indice de réfraction après calcul de la réfraction à travers un objet. Ex: pour le sol, la réfraction en sortie n'est pas forcément la même.
	float l1;	//Intensité de la lumière 1 - ombre portées
	bool ok;
	float t;

};

Intersection inter_scene[NUM_OBJETS];	//Les intersections avec les objets de la scène
Intersection inter_scene_lum[NUM_OBJETS];	//Les intersections des lumières avec les objets de la scène (ombres)


vec3 couleur;
float ZBuffer;

//----------- Calcul les points d'intersections avec une sphère

float epsilon=1e-2;

Intersection intersection_sphere(Ray ray,int id)
{
		int o=id*TAILLE_OBJ;
		Intersection i;
		i.ok=false;
		float t2;
	 //Calcul le rayon dans le repère de la sphère:
                Ray RayS;
                RayS.pos=ray.pos- vec3(objets[o+POSX],objets[o+POSY],objets[o+POSZ]);        //Point origine du rayon
                RayS.dir=ray.dir;          //Vecteur directeur du rayon dans le repère de la sphère
	 //Déterminant:
		float a=RayS.dir.x*RayS.dir.x+RayS.dir.y*RayS.dir.y+RayS.dir.z*RayS.dir.z;
		float b=2.*(RayS.pos.x*RayS.dir.x+RayS.pos.y*RayS.dir.y+RayS.pos.z*RayS.dir.z);
		float c=RayS.pos.x*RayS.pos.x+RayS.pos.y*RayS.pos.y+RayS.pos.z*RayS.pos.z-objets[o+RAY]*objets[o+RAY];

		float delta=b*b-4.*a*c;

		 if(delta>0.)
		 {
			delta=sqrt(delta);
			i.t=(-b-delta)/(2.*a);
			t2=(-b+delta)/(2.*a);
			if(i.t>epsilon && t2 >epsilon)
			{
				if(t2<i.t) i.t=t2;
				i.ok=true;
			}

			else if(i.t>epsilon && t2<=epsilon)
			{
				i.ok=true;
			}
			else if(i.t<=epsilon && t2>epsilon)
			{
				i.ok=true;
				i.t=t2;
			}
		}

		else if (delta==0.)
		{
			i.t=-b/2.*a;
			if(i.t>epsilon) i.ok=true;
		}

		if(i.ok)
		{
			i.P=ray.pos+ray.dir*i.t;		//Point dans l'espace absolu
			i.zDist=length(i.P-ray.pos);

			if(i.zDist<ZBuffer)
			{
				ZBuffer=i.zDist+epsilon;
			}
			else i.ok=false;
		}
	return i;
}


void calcul_normale_sphere(int id)
{
	int o=id*TAILLE_OBJ;
	vec3 p=inter_scene[id].P-vec3(objets[o+POSX],objets[o+POSY],objets[o+POSZ]);
	float d=length(p);
	inter_scene[id].N=p/d;
}

void calcul_reflec_sphere(int id, Ray ray)
{
	inter_scene[id].reflec=reflect(ray.dir,inter_scene[id].N);
}


//Intersection d'un rayon lumineux avec une sphère:

Intersection intersection_sphere_lum(Ray ray,int id)
{
		Intersection i;
		int o=id*TAILLE_OBJ;
		i.ok=false;
		float t2;
	 //Calcul le rayon dans le repère de la sphère:
                Ray RayS;
                RayS.pos=ray.pos- vec3(objets[o+POSX],objets[o+POSY],objets[o+POSZ]);        //Point origine du rayon
                RayS.dir=ray.dir;          //Vecteur directeur du rayon dans le repère de la sphère
	 //Déterminant:
		float a=RayS.dir.x*RayS.dir.x+RayS.dir.y*RayS.dir.y+RayS.dir.z*RayS.dir.z;
		float b=2.*(RayS.pos.x*RayS.dir.x+RayS.pos.y*RayS.dir.y+RayS.pos.z*RayS.dir.z);
		float c=RayS.pos.x*RayS.pos.x+RayS.pos.y*RayS.pos.y+RayS.pos.z*RayS.pos.z-objets[o+RAY]*objets[o+RAY];

		float delta=b*b-4.*a*c;

		 if(delta>0.)
		 {
			delta=sqrt(delta);
			i.t=(-b-delta)/(2.*a);
			t2=(-b+delta)/(2.*a);
			if(i.t>epsilon && t2 >epsilon)
			{
				if(t2<i.t) i.t=t2;
				if(i.t<1.)i.ok=true;
			}

			else if(i.t>epsilon && t2<=epsilon && i.t<1.) i.ok=true;

			else if(i.t<=epsilon && t2>epsilon && t2<1.) {i.t=t2; i.ok=true;}
		}

		else if (delta==0.)
		{
			i.t=-b/2.*a;
			if(i.t>epsilon && i.t<1.) i.ok=true;
		}

	return i;
}

//------------------------------------- Calcul l'intersection avec le sol

Intersection intersection_sol(Ray ray,int id)
{
	// le sol n'a pas de repère propre, il est fixe dans l'espace
	Intersection i;
	int o=id*TAILLE_OBJ;
	i.ok=false;
	if(ray.dir.y!=0.)
	{
		i.t=-ray.pos.y/ray.dir.y;
		if(i.t>epsilon)
		{
			i.P=ray.pos+ray.dir*i.t;
			i.zDist=length(i.P-ray.pos);

			if(i.zDist<ZBuffer)
			{
				ZBuffer=i.zDist+epsilon;
				i.ok=true;
			}
		}
	}
	return i;
}

//Servira pour ajouter un effet de bump...
void calcul_normale_Sol(int id)
{
	inter_scene[id].N=vec3(0.,1.,0.);
}

void calcul_reflec_Sol(int id, Ray ray)
{

	inter_scene[id].reflec=reflect(ray.dir,inter_scene[id].N);
}




Intersection intersection_sol_lum(Ray ray,int id)
{
	// le sol n'a pas de repère propre, il est fixe dans l'espace
	Intersection i;
	i.ok=false;
	if(ray.dir.y!=0.)
	{
		i.t=-ray.pos.y/ray.dir.y;
		if(i.t>epsilon && i.t<1.) i.ok=true;
	}
	return i;
}



//Calcul les intersections d'un rayon avec les objets de la scène
//Renvoi l'identifiant de l'objet impacté, -1 si aucun objet impacté


int calcul_intersections(Ray ray)
{
	ZBuffer=100000.;
	/// ATTENTION  dans un souci d'optimisation, la position des objets dans la table doit être connue du programme
	inter_scene[0]=intersection_sol(ray,0);
	inter_scene[1]=intersection_sphere(ray,1);
	inter_scene[2]=intersection_sphere(ray,2);
	inter_scene[3]=intersection_sphere(ray,3);
	inter_scene[4]=intersection_sphere(ray,4);
	inter_scene[5]=intersection_sphere(ray,5);
	inter_scene[6]=intersection_sphere(ray,6);


	int n,l;
	for (n=0;n<NUM_OBJETS;)
	{
		if(inter_scene[n].ok && inter_scene[n].zDist<=ZBuffer)
		{
			//Intersection avec les sources lumineuses:

			inter_scene[n].l1=1.;

			Ray rayLum;
			rayLum.pos=inter_scene[n].P;
			rayLum.dir=vec3(L1[POSX],L1[POSY],L1[POSZ])-rayLum.pos;
			ZBuffer=100000.;

			inter_scene_lum[0]=intersection_sol_lum(rayLum,0);
			inter_scene_lum[1]=intersection_sphere_lum(rayLum,1);
			inter_scene_lum[2]=intersection_sphere_lum(rayLum,2);
			inter_scene_lum[3]=intersection_sphere_lum(rayLum,3);
			inter_scene_lum[4]=intersection_sphere_lum(rayLum,4);
			inter_scene_lum[5]=intersection_sphere_lum(rayLum,5);
			inter_scene_lum[6]=intersection_sphere_lum(rayLum,6);

			for (l=0;l<NUM_OBJETS;)
			{
				if(inter_scene_lum[l].ok)
				{
					inter_scene[n].l1=max(inter_scene[n].l1-(1.-objets[l*TAILLE_OBJ+TRANSP]),0.);
				}
				l++;
			}
			return n;
		}
		n++;
	}
	return -1;
}


//-------- Couleur diffuse renvoyée par une source ponctuelle
vec3 calcul_diffusion_ponctuelle(vec3 point, vec3 normale,float lum[TAILLE_LUM])
{
	vec3 dir=point;
	dir=point-vec3(lum[POSX],lum[POSY],lum[POSZ]);
	float Nd=length(dir);
	float i=max(-(dir.x*normale.x+dir.y*normale.y+dir.z*normale.z)/Nd,0.);
	return vec3(lum[CR],lum[CV],lum[CB])*i*lum[INTENS]*min(1./(lum[ATT]*Nd),1.);
}


//-------- Couleur spéculaire renvoyée par une source
// Nous sommes dans le repère absolu.
vec3 calcul_speculaire(vec3 point, vec3 normale,vec3 posObs, float brillance,float lum[TAILLE_LUM])
{
	vec3 direction_L=point-vec3(lum[POSX],lum[POSY],lum[POSZ]);
	vec3 direction_O=point-posObs;
	float Nd=length(direction_L);
	float Np=length(direction_O);
	vec3 reflet=reflect(direction_L,normale);
	float i=max(-(direction_O.x*reflet.x+direction_O.y*reflet.y+direction_O.z*reflet.z)/(Np*Nd),0.);
	i=pow(i,brillance);
	return vec3(lum[CR],lum[CV],lum[CB])*i*lum[INTENS]*min(1./(lum[ATT]*Nd),1.);
}


//Calcul l'éclairage du sol:
vec3 renvoi_couleur_sol(int id,Ray ray)
{
	vec3 c;
	vec3 lum_amb;	//Eclairage ambiant sur l'objet
	vec3 lum_diff;
	vec3 lum_spec;
	int o=id*TAILLE_OBJ;
	vec3 co=vec3(objets[o+CR],objets[o+CV],objets[o+CB]);

	lum_amb=co*C_amb*L_amb;

	lum_diff=calcul_diffusion_ponctuelle(inter_scene[id].P,inter_scene[id].N,L1)*inter_scene[id].l1;
	lum_spec=calcul_speculaire(inter_scene[id].P,inter_scene[id].N,ray.pos,objets[o+BRILL],L1)*inter_scene[id].l1;
	c=clamp(
			(co*lum_diff*objets[o+DIFF]	 + lum_amb )
			* (1.-objets[o+REFLEC])
			+ lum_spec*objets[o+SPEC]
			,0.,1.);
	return c;
}

vec3 renvoi_couleur_sphere(int id, Ray ray)
{
	vec3 c;
	vec3 lum_amb;	//Eclairage ambiant sur l'objet
	vec3 lum_diff;
	vec3 lum_spec;
	int o=id*TAILLE_OBJ;
	vec3 co=vec3(objets[o+CR],objets[o+CV],objets[o+CB]);

	lum_amb=co*C_amb*L_amb;
	lum_diff=calcul_diffusion_ponctuelle(inter_scene[id].P,inter_scene[id].N,L1)*inter_scene[id].l1;
	lum_spec=calcul_speculaire(inter_scene[id].P,inter_scene[id].N,ray.pos,objets[o+BRILL],L1)*inter_scene[id].l1;
	c=clamp(
			(co*lum_diff*objets[o+DIFF]+lum_amb)
			* (1.-objets[o+REFLEC])
			+lum_spec*objets[o+SPEC]
			,0.,1.);
	return c;
}



//-------------------------------------------------------------------------------------------------------------
//	Distributeur des fonctions de calcul de couleur selon l'identifiant de l'objet
//-------------------------------------------------------------------------------------------------------------
vec3 calcul_couleur_intersection(int id,Ray ray)
{
	vec3 c;
	switch (id)
	{
		case 0:calcul_normale_Sol(id);c=renvoi_couleur_sol(id,ray);break;
		case 1:calcul_normale_sphere(id);c=renvoi_couleur_sphere(id,ray);break;
		case 2:calcul_normale_sphere(id);c=renvoi_couleur_sphere(id,ray);break;
		case 3:calcul_normale_sphere(id);c=renvoi_couleur_sphere(id,ray);break;
		case 4:calcul_normale_sphere(id);c=renvoi_couleur_sphere(id,ray);break;
		case 5:calcul_normale_sphere(id);c=renvoi_couleur_sphere(id,ray);break;
		case 6:calcul_normale_sphere(id);c=renvoi_couleur_sphere(id,ray);break;
		default:break;
	}
	return c;
}

void calcul_reflection(int id, Ray ray)
{
	switch (id)
	{
		case 0:calcul_reflec_Sol(id,ray);break;
		case 1:calcul_reflec_sphere(id,ray);break;
		case 2:calcul_reflec_sphere(id,ray);break;
		case 3:calcul_reflec_sphere(id,ray);break;
		case 4:calcul_reflec_sphere(id,ray);break;
		case 5:calcul_reflec_sphere(id,ray);break;
		case 6:calcul_reflec_sphere(id,ray);break;
		default:break;
	}
}

//------------------------------------------------------------------------
//		Entrée
//------------------------------------------------------------------------


void main( void )
{
		couleur=C_ciel;
		vec3 couleur_reflet1=C_ciel;
		vec3 couleur_reflet2=C_ciel;
		vec3 couleur_reflet3=C_ciel;

		vec3 couleur_refrac1=C_ciel;
		vec3 couleur_refrac2=C_ciel;
		vec3 couleur_refrac3=C_ciel;

		Ray rayRefl,rayRefr,RayObs,RayAbs;
		float reflec_i1,reflec_i2,reflec_i3;
		float transp_i1,transp_i2,transp_i3;

        //Calcul le vecteur directeur du rayon dans le repère de l'observateur:
                float x=gl_FragCoord.x-ecranDim.x/2.;
                float y=(gl_FragCoord.y-ecranDim.y/2.)*ecran_ratio;


                RayObs.pos=vec3(0.,0.,0.);
                RayObs.dir=vec3(x/1000.,y/1000.,distanceFocale/1000.);

                //RayObs.refrac_i=1.

	//Calcul la position et le vecteur directeur du rayon dans l'espace absolu (pour le moment l'observateur est dans le même repère que l'espace)

		RayAbs.pos=(Obs*vec4(RayObs.pos,1.)).xyz;
		RayAbs.dir=(Obs*vec4(RayObs.dir,1.)).xyz-RayAbs.pos;

		RayAbs.refrac_i=refrac_amb;	/// CALCULER L'INDICE DE REFRACTION AU NIVEAU DE L'API (dans quel milieu est l'observateur?)

	//------------Calcul les intersections du rayon avec les objets:
	int id;

	id=calcul_intersections(RayAbs);


	//Calcul la couleur du pixel:
		if(id>=0)
		{
			couleur=calcul_couleur_intersection(id,RayAbs);

			if(objets[id*TAILLE_OBJ+REFLEC]>0. && numReflec>0)
			{

				calcul_reflection(id,RayAbs);
				rayRefl.pos=inter_scene[id].P;
				rayRefl.dir=inter_scene[id].reflec;

				reflec_i1=objets[id*TAILLE_OBJ+REFLEC];

				id=calcul_intersections(rayRefl);

				if(id>=0)
				{
					couleur_reflet1=calcul_couleur_intersection(id,rayRefl);

					//2ème passage réflection:

					if(objets[id*TAILLE_OBJ+REFLEC]>0. && numReflec>1)
					{
						calcul_reflection(id,rayRefl);
						rayRefl.pos=inter_scene[id].P;
						rayRefl.dir=inter_scene[id].reflec;
						reflec_i2=objets[id*TAILLE_OBJ+REFLEC];

						id=calcul_intersections(rayRefl);
						if(id>=0)
						{
							couleur_reflet2=calcul_couleur_intersection(id,rayRefl);

							//3ème passage réflection:

							if(objets[id*TAILLE_OBJ+REFLEC]>0. && numReflec>2)
							{
								calcul_reflection(id,rayRefl);
								rayRefl.pos=inter_scene[id].P;
								rayRefl.dir=inter_scene[id].reflec;
								reflec_i3=objets[id*TAILLE_OBJ+REFLEC];

								id=calcul_intersections(rayRefl);
								if(id>=0)couleur_reflet3=calcul_couleur_intersection(id,rayRefl);
								couleur+=couleur_reflet3*reflec_i3*reflec_i2*reflec_i1;
							}
						}

						couleur+=couleur_reflet2*reflec_i2*reflec_i1;
					}
				}
				couleur+=couleur_reflet1*reflec_i1;
			}

		}

		//1er passage de réflection:

        gl_FragColor = vec4(couleur,1.);
}
