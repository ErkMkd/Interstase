
//------------
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

//Structure de d?finition des objets:
#define	OBJTYPE	0
#define POSX 	1
#define POSY 	2
#define POSZ 	3
#define CR	 	4
#define CV		5
#define CB		6

//Luminosit?:
#define INTENS	7
#define ATT 		8

#define TAILLE_LUM		9	//La structure des sources lumineuses s'arr?te ici

//Mat?riau:
#define DIFF		9
#define SPEC	10
#define BRILL	11
#define TRANSP	12
#define REFLEC	13
#define REFRAC	14

//Donn?es suppl?mentaires des sph?res:
#define RAY		15

//Donn?es suppl?mentaires pour les textures
#define C2R		15	//Couleur n?2
#define C2V		16
#define C2B		17
#define CAR_XL	18	//Largeur des carreaux
#define CAR_ZL	19	//Profondeur

#define TAILLE_OBJ		20

#define NUM_OBJETS		16
#define NUM_LUM		1

uniform float random_number;
uniform float temps;
uniform int numReflec;	//Nombre d'it?ration de r?flections (de 0 ? 3)
uniform mat4 Obs;		//Matrice de l'observateur
uniform  vec2 ecranDim;	//Dimensions de l'?cran
uniform float distanceFocale;
uniform float ecran_ratio;
uniform float distance_brouillard;

uniform float C_ciel[TAILLE_CIEL];
uniform float L_amb;
uniform vec3 C_amb;

uniform float L1[TAILLE_LUM];

uniform float objets[TAILLE_OBJ*NUM_OBJETS];

struct Ray
{
	vec3 pos;
	vec3 dir;
};

struct Intersection
{
	float zDist;
	vec3 P;
	vec3 N;
	vec3 reflec;	//Direction du rayon reflet?
	float l1;	//Intensit? de la lumi?re 1 - ombre port?es
	bool ok;
	float t;

};

Intersection inter_scene[NUM_OBJETS];	//Les intersections avec les objets de la sc?ne
Intersection inter_scene_lum[NUM_OBJETS];	//Les intersections des lumi?res avec les objets de la sc?ne (ombres)


vec3 couleur;
float ZBuffer;

//----------- Calcul les points d'intersections avec une sph?re

#define EPSILON 1e-2
#define EPSILON2 1e-4

void intersection_sphere(Ray ray,int id)
{
		int o=id*TAILLE_OBJ;
		inter_scene[id].ok=false;
		float t2;
	 //Calcul le rayon dans le rep?re de la sph?re:
                Ray RayS;
                RayS.pos=ray.pos- vec3(objets[o+POSX],objets[o+POSY],objets[o+POSZ]);        //Point origine du rayon
                RayS.dir=ray.dir;          //Vecteur directeur du rayon dans le rep?re de la sph?re
	 //D?terminant:
		float a=RayS.dir.x*RayS.dir.x+RayS.dir.y*RayS.dir.y+RayS.dir.z*RayS.dir.z;
		float b=2.*(RayS.pos.x*RayS.dir.x+RayS.pos.y*RayS.dir.y+RayS.pos.z*RayS.dir.z);
		float c=RayS.pos.x*RayS.pos.x+RayS.pos.y*RayS.pos.y+RayS.pos.z*RayS.pos.z-objets[o+RAY]*objets[o+RAY];

		float delta=b*b-4.*a*c;

		 if(delta>0.)
		 {
			delta=sqrt(delta);
			inter_scene[id].t=(-b-delta)/(2.*a);
			t2=(-b+delta)/(2.*a);
			if(inter_scene[id].t>EPSILON && t2 >EPSILON)
			{
				if(t2<inter_scene[id].t) inter_scene[id].t=t2;
				inter_scene[id].ok=true;
			}

			else if(inter_scene[id].t>EPSILON && t2<=EPSILON)
			{
				inter_scene[id].ok=true;
			}
			else if(inter_scene[id].t<=EPSILON && t2>EPSILON)
			{
				inter_scene[id].ok=true;
				inter_scene[id].t=t2;
			}
		}

		else if (delta==0.)
		{
			inter_scene[id].t=-b/2.*a;
			if(inter_scene[id].t>EPSILON) inter_scene[id].ok=true;
		}

		if(inter_scene[id].ok)
		{
			inter_scene[id].P=ray.pos+ray.dir*inter_scene[id].t;		//Point dans l'espace absolu
			inter_scene[id].zDist=length(inter_scene[id].P-ray.pos);

			if(inter_scene[id].zDist<ZBuffer)
			{
				ZBuffer=inter_scene[id].zDist+EPSILON;
			}
			else inter_scene[id].ok=false;
		}
}

// Renvoie true si le point est ? l'interieur d'une sph?re

bool renvoi_interieur_sphere(vec3 pos, vec3 posSphere,float r)
{
	vec3 dist=pos-posSphere;
	if(length(dist)<r) return true; else return false;
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

//Intersection d'un rayon lumineux avec une sph?re:

bool intersection_sphere_lum(Ray ray,int id)
{
		int o=id*TAILLE_OBJ;
		float t2;
	 //Calcul le rayon dans le rep?re de la sph?re:
                Ray RayS;
                RayS.pos=ray.pos- vec3(objets[o+POSX],objets[o+POSY],objets[o+POSZ]);        //Point origine du rayon
                RayS.dir=ray.dir;          //Vecteur directeur du rayon dans le rep?re de la sph?re
	 //D?terminant:
		float a=RayS.dir.x*RayS.dir.x+RayS.dir.y*RayS.dir.y+RayS.dir.z*RayS.dir.z;
		float b=2.*(RayS.pos.x*RayS.dir.x+RayS.pos.y*RayS.dir.y+RayS.pos.z*RayS.dir.z);
		float c=RayS.pos.x*RayS.pos.x+RayS.pos.y*RayS.pos.y+RayS.pos.z*RayS.pos.z-objets[o+RAY]*objets[o+RAY];

		float delta=b*b-4.*a*c;

		 if(delta>0.)
		 {
			delta=sqrt(delta);
			inter_scene_lum[id].t=(-b-delta)/(2.*a);
			t2=(-b+delta)/(2.*a);
			if(inter_scene_lum[id].t>EPSILON && t2 >EPSILON)
			{
				if(t2<inter_scene_lum[id].t) inter_scene_lum[id].t=t2;
				if(inter_scene_lum[id].t<1.) return true;
			}

			else if(inter_scene_lum[id].t>EPSILON && t2<=EPSILON && inter_scene_lum[id].t<1.) return true;

			else if(inter_scene_lum[id].t<=EPSILON && t2>EPSILON && t2<1.) { inter_scene_lum[id].t=t2;return true;}
		}

		else if (delta==0.)
		{
			inter_scene_lum[id].t=-b/2.*a;
			if(inter_scene_lum[id].t>EPSILON && inter_scene_lum[id].t<1.) return true;
		}

		return false;
}

//------------------------------------- Calcul l'intersection avec le sol


void intersection_sol(Ray ray,int id)
{
	// le sol n'a pas de rep?re propre, il est fixe dans l'espace
	int o=id*TAILLE_OBJ;
	inter_scene[id].ok=false;
	if(ray.dir.y<-EPSILON2 || ray.dir.y>EPSILON2)
	{
		inter_scene[id].t=-ray.pos.y/ray.dir.y;
		if(inter_scene[id].t>EPSILON)
		{
			inter_scene[id].P=ray.pos+ray.dir*inter_scene[id].t;
			inter_scene[id].zDist=length(inter_scene[id].P-ray.pos);

			if(inter_scene[id].zDist<ZBuffer)
			{
				ZBuffer=inter_scene[id].zDist+EPSILON;
				inter_scene[id].ok=true;
			}
		}
	}
}


//Servira pour ajouter un effet de bump...
void calcul_normale_Sol(int id)
{
	float amplX=0.03+0.007*cos(inter_scene[id].P.x*2);
	float amplY=0.02+0.003*sin(inter_scene[id].P.z*3);
	float perturbX=(amplX*cos(inter_scene[id].P.x*5));
	float perturbZ=(amplY*cos(inter_scene[id].P.z*4.333));
	float rx=sin(perturbX);
	float ry=cos(perturbX);
	inter_scene[id].N=vec3(rx,ry*cos(perturbZ),ry*sin(perturbZ));

}

void calcul_reflec_Sol(int id, Ray ray)
{

	inter_scene[id].reflec=reflect(ray.dir,inter_scene[id].N);
}


bool intersection_sol_lum(Ray ray,int id)
{
	// le sol n'a pas de rep?re propre, il est fixe dans l'espace
	if(ray.dir.y<-EPSILON2 || ray.dir.y>EPSILON2 )
	{
		inter_scene_lum[id].t=-ray.pos.y/ray.dir.y;
		if(inter_scene_lum[id].t>EPSILON && inter_scene_lum[id].t<1.) return true;
	}
	return false;
}



//Calcul les intersections d'un rayon avec les objets de la sc?ne
//Renvoi l'identifiant de l'objet impact?, -1 si aucun objet impact?


int calcul_intersections_sans_ombres(Ray ray)
{
	ZBuffer=100000.;
	/// ATTENTION  dans un souci d'optimisation, la position des objets dans la table doit ?tre connue du programme
	intersection_sol(ray,0);
	intersection_sphere(ray,1);
	intersection_sphere(ray,2);
	intersection_sphere(ray,3);
	intersection_sphere(ray,4);
	intersection_sphere(ray,5);
	intersection_sphere(ray,6);
	intersection_sphere(ray,7);
	intersection_sphere(ray,8);
	intersection_sphere(ray,9);
	intersection_sphere(ray,10);
	intersection_sphere(ray,11);
	intersection_sphere(ray,12);
	intersection_sphere(ray,13);
	intersection_sphere(ray,14);
	intersection_sphere(ray,15);
	/*intersection_sphere(ray,16);
	/*intersection_sphere(ray,17);
	/*intersection_sphere(ray,18);*/

	int n,l;
	for (n=0;n<NUM_OBJETS;)
	{
		if(inter_scene[n].ok && inter_scene[n].zDist<=ZBuffer)
		{
			inter_scene[n].l1=1.;
			return n;
		}
		n++;
	}
	return -1;
}

int calcul_intersections(Ray ray)
{
	ZBuffer=100000.;
	/// ATTENTION  dans un souci d'optimisation, la position des objets dans la table doit ?tre connue du programme
	intersection_sol(ray,0);
	intersection_sphere(ray,1);
	intersection_sphere(ray,2);
	intersection_sphere(ray,3);
	intersection_sphere(ray,4);
	intersection_sphere(ray,5);
	intersection_sphere(ray,6);
	intersection_sphere(ray,7);
	intersection_sphere(ray,8);
	intersection_sphere(ray,9);
	intersection_sphere(ray,10);
	intersection_sphere(ray,11);
	intersection_sphere(ray,12);
	intersection_sphere(ray,13);
	intersection_sphere(ray,14);
	intersection_sphere(ray,15);
	/*intersection_sphere(ray,16);
	/*intersection_sphere(ray,17);
	/*intersection_sphere(ray,18);*/


	int n,l;
	for (n=0;n<NUM_OBJETS;)
	{
		if(inter_scene[n].ok && inter_scene[n].zDist<=ZBuffer)
		{
			//Intersection avec les sources lumineuses:



			Ray rayLum;
			rayLum.pos=inter_scene[n].P;
			rayLum.dir=vec3(L1[POSX],L1[POSY],L1[POSZ])-rayLum.pos;

			if( intersection_sol_lum(rayLum,0)
				|| intersection_sphere_lum(rayLum,1)
				|| intersection_sphere_lum(rayLum,2)
				|| intersection_sphere_lum(rayLum,3)
				|| intersection_sphere_lum(rayLum,4)
				|| intersection_sphere_lum(rayLum,5)
				|| intersection_sphere_lum(rayLum,6)
				|| intersection_sphere_lum(rayLum,7)
				|| intersection_sphere_lum(rayLum,8)
				|| intersection_sphere_lum(rayLum,9)
				|| intersection_sphere_lum(rayLum,10)
				|| intersection_sphere_lum(rayLum,11)
				|| intersection_sphere_lum(rayLum,12)
				|| intersection_sphere_lum(rayLum,13)
				|| intersection_sphere_lum(rayLum,14)
				|| intersection_sphere_lum(rayLum,15)
				/*|| intersection_sphere_lum(rayLum,16)
				/*|| intersection_sphere_lum(rayLum,17)
				/*|| intersection_sphere_lum(rayLum,18)*/
				) inter_scene[n].l1=0.;

			else inter_scene[n].l1=1.;

			return n;
		}
		n++;
	}
	return -1;
}


//-------- Couleur diffuse renvoy?e par une source ponctuelle
vec3 calcul_diffusion_ponctuelle(vec3 point, vec3 normale,float lum[TAILLE_LUM])
{
	vec3 dir=point;
	dir=point-vec3(lum[POSX],lum[POSY],lum[POSZ]);
	float Nd=length(dir);
	float i=max(-(dir.x*normale.x+dir.y*normale.y+dir.z*normale.z)/Nd,0.);
	return vec3(lum[CR],lum[CV],lum[CB])*i*lum[INTENS]*min(1./(lum[ATT]*Nd),1.);
}


//-------- Couleur sp?culaire renvoy?e par une source
// Nous sommes dans le rep?re absolu.
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


//Calcul l'?clairage du sol:
vec3 renvoi_couleur_sol(int id,Ray ray)
{
	vec3 c;
	vec3 lum_amb;	//Eclairage ambiant sur l'objet
	vec3 lum_diff;
	vec3 lum_spec;
	int o=id*TAILLE_OBJ;

	//--------Couleur en fonction de la texture:
	vec3 co;
	int xBloc=round(inter_scene[id].P.x/objets[o+CAR_XL]);
	int zBloc=round(inter_scene[id].P.z/objets[o+CAR_ZL]);
	xBloc&=1;
	zBloc&=1;
	if (xBloc==0)
	{
		if(zBloc==0)co=vec3(objets[o+CR],objets[o+CV],objets[o+CB]);
		else {co=vec3(objets[o+C2R],objets[o+C2V],objets[o+C2B]);}
	}
	else
	{
		if(zBloc==0){co=vec3(objets[o+C2R],objets[o+C2V],objets[o+C2B]);}
		else co=vec3(objets[o+CR],objets[o+CV],objets[o+CB]);
	}

	//--------

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
		case 7:calcul_normale_sphere(id);c=renvoi_couleur_sphere(id,ray);break;
		case 8:calcul_normale_sphere(id);c=renvoi_couleur_sphere(id,ray);break;
		case 9:calcul_normale_sphere(id);c=renvoi_couleur_sphere(id,ray);break;
		case 10:calcul_normale_sphere(id);c=renvoi_couleur_sphere(id,ray);break;
		case 11:calcul_normale_sphere(id);c=renvoi_couleur_sphere(id,ray);break;
		case 12:calcul_normale_sphere(id);c=renvoi_couleur_sphere(id,ray);break;
		case 13:calcul_normale_sphere(id);c=renvoi_couleur_sphere(id,ray);break;
		case 14:calcul_normale_sphere(id);c=renvoi_couleur_sphere(id,ray);break;
		case 15:calcul_normale_sphere(id);c=renvoi_couleur_sphere(id,ray);break;
		/*case 16:calcul_normale_sphere(id);c=renvoi_couleur_sphere(id,ray);break;
		/*case 17:calcul_normale_sphere(id);c=renvoi_couleur_sphere(id,ray);break;
		/*case 18:calcul_normale_sphere(id);c=renvoi_couleur_sphere(id,ray);break;*/
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
		case 7:calcul_reflec_sphere(id,ray);break;
		case 8:calcul_reflec_sphere(id,ray);break;
		case 9:calcul_reflec_sphere(id,ray);break;
		case 10:calcul_reflec_sphere(id,ray);break;
		case 11:calcul_reflec_sphere(id,ray);break;
		case 12:calcul_reflec_sphere(id,ray);break;
		case 13:calcul_reflec_sphere(id,ray);break;
		case 14:calcul_reflec_sphere(id,ray);break;
		case 15:calcul_reflec_sphere(id,ray);break;
		/*case 16:calcul_reflec_sphere(id,ray);break;
		/*case 17:calcul_reflec_sphere(id,ray);break;
		/*case 18:calcul_reflec_sphere(id,ray);break;*/
		default:break;
	}
}

//------------------------------------------------------------------------
//		Entr?e
//------------------------------------------------------------------------

vec3 couleur_ciel;
vec3 couleur_reflet1;
vec3 couleur_reflet2;
vec3 couleur_reflet3;


void main( void )
{
		Ray rayRefl,rayRefr,RayObs,RayAbs;
		float reflec_i1,reflec_i2,reflec_i3;

        //Calcul le vecteur directeur du rayon dans le rep?re de l'observateur:
                float x=gl_FragCoord.x-ecranDim.x/2.;
                float y=(gl_FragCoord.y-ecranDim.y/2.)*ecran_ratio;


                RayObs.pos=vec3(0.,0.,0.);
                RayObs.dir=vec3(x/1000.,y/1000.,distanceFocale/1000.);


	//Calcul la position et le vecteur directeur du rayon dans l'espace absolu (pour le moment l'observateur est dans le m?me rep?re que l'espace)

		RayAbs.pos=(Obs*vec4(RayObs.pos,1.)).xyz;
		RayAbs.dir=(Obs*vec4(RayObs.dir,1.)).xyz-RayAbs.pos;

	//Couleur du ciel:
		float alphaRay=min(acos(dot(normalize(vec3(RayAbs.dir.x,0.,RayAbs.dir.z)),normalize(RayAbs.dir)))/(1.5708/1.),1.);
		if(RayAbs.dir.y<-EPSILON2)
		{
			couleur_ciel=vec3(
							C_ciel[NADIR_CR]*alphaRay+C_ciel[HORIZB_CR]*(1.-alphaRay),
							C_ciel[NADIR_CV]*alphaRay+C_ciel[HORIZB_CV]*(1.-alphaRay),
							C_ciel[NADIR_CB]*alphaRay+C_ciel[HORIZB_CB]*(1.-alphaRay)
							);
			//couleur_ciel=vec3(C_ciel[NADIR_CR],C_ciel[NADIR_CV],C_ciel[NADIR_CB]);

		}
		else if(RayAbs.dir.y>=-EPSILON2 && RayAbs.dir.y<EPSILON2)
		{
			couleur_ciel=vec3(C_ciel[HORIZB_CR],C_ciel[HORIZB_CV],C_ciel[HORIZB_CB]);
		}
		else
		{
			couleur_ciel=vec3(
							C_ciel[ZENITH_CR]*alphaRay+C_ciel[HORIZH_CR]*(1.-alphaRay),
							C_ciel[ZENITH_CV]*alphaRay+C_ciel[HORIZH_CV]*(1.-alphaRay),
							C_ciel[ZENITH_CB]*alphaRay+C_ciel[HORIZH_CB]*(1.-alphaRay)
							);
			//couleur_ciel=vec3(C_ciel[ZENITH_CR],C_ciel[ZENITH_CV],C_ciel[ZENITH_CB]);

		}
		couleur=couleur_ciel;


	//------------Calcul les intersections du rayon avec les objets:
	int id;
	float distance_objet=1000000.;
	id=calcul_intersections(RayAbs);


	//Calcul la couleur du pixel:
		if(id>=0)
		{
			distance_objet=inter_scene[id].zDist;

			couleur=calcul_couleur_intersection(id,RayAbs);

			if(objets[id*TAILLE_OBJ+REFLEC]>0. && numReflec>0)
			{
				couleur_reflet1=couleur_ciel;
				calcul_reflection(id,RayAbs);
				rayRefl.pos=inter_scene[id].P;
				rayRefl.dir=inter_scene[id].reflec;

				reflec_i1=objets[id*TAILLE_OBJ+REFLEC];

				id=calcul_intersections(rayRefl);

				if(id>=0)
				{
					couleur_reflet1=calcul_couleur_intersection(id,rayRefl);

					//2?me passage r?flection:

					if(objets[id*TAILLE_OBJ+REFLEC]>0. && numReflec>1)
					{
						couleur_reflet2=couleur_ciel;
						calcul_reflection(id,rayRefl);
						rayRefl.pos=inter_scene[id].P;
						rayRefl.dir=inter_scene[id].reflec;
						reflec_i2=objets[id*TAILLE_OBJ+REFLEC];

						id=calcul_intersections_sans_ombres(rayRefl);
						if(id>=0)
						{
							couleur_reflet2=calcul_couleur_intersection(id,rayRefl);

							//3?me passage r?flection:

							if(objets[id*TAILLE_OBJ+REFLEC]>0. && numReflec>2)
							{
								couleur_reflet3=couleur_ciel;
								calcul_reflection(id,rayRefl);
								rayRefl.pos=inter_scene[id].P;
								rayRefl.dir=inter_scene[id].reflec;
								reflec_i3=objets[id*TAILLE_OBJ+REFLEC];

								id=calcul_intersections_sans_ombres(rayRefl);
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

		//1er passage de r?flection:
		//Brouillard:
		if(distance_objet>distance_brouillard)couleur=couleur_ciel;
		else
		{
			distance_objet/=distance_brouillard;
			couleur=couleur*(1.-distance_objet)+couleur_ciel*distance_objet;
		}

        gl_FragColor = vec4(couleur,1.);
}
