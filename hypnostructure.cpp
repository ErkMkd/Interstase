
#include "hypnostructure.h"

//Observateur

Repere HypnoStructure::obs(0.,20.,-20.);
vec3f HypnoStructure::point_vise;
Repere HypnoStructure::structure(0.,2500.,0.);

float HypnoStructure::aX=0.;
float HypnoStructure::aY=0.;
float HypnoStructure::aZ=0.;
float HypnoStructure::aR=0.;
float HypnoStructure::aT=0.;
float HypnoStructure::aL=0.;

float HypnoStructure::coordsDisque[3*NUM_COTES_DISQUE];

float HypnoStructure::coordsCylindre[3*NUM_POINTS_CYL];
uint16_t  HypnoStructure::facesCylindre[(NUM_FACES_CYL-2)*4+2*NUM_COTES_CYL];
float HypnoStructure::normalesCylindre[3*NUM_POINTS_CYL];

float HypnoStructure::coordsSphere[3*NUM_POINTS_SPHERE];
uint16_t HypnoStructure::facesSphere[NUM_FACES_SPHERE*4-(2*NUM_COTES_SPHERE)];
float HypnoStructure::normalesSphere[3*NUM_POINTS_SPHERE];

float HypnoStructure::coordsTore[3*NUM_POINTS_TORE];
uint16_t HypnoStructure::facesTore[NUM_FACES_TORE*4];
float HypnoStructure::normalesTore[3*NUM_POINTS_TORE];

GLuint HypnoStructure::disque_liste_id=0;
GLuint HypnoStructure::sphere_liste_id=0;
GLuint HypnoStructure::cylindre_liste_id=0;
GLuint HypnoStructure::tore_liste_id=0;


uint16_t HypnoStructure::itermax=1;
float HypnoStructure::angle_rot_roue=0.;
float HypnoStructure::vitesse_rot_roue=0.;
float HypnoStructure::temps_prec=0.;

//========= Arrière plan (générés en ray tracing)
//Paramètres
int32_t HypnoStructure::numReflec=1;
int32_t HypnoStructure::numRefrac=1;

//Ciel:
float HypnoStructure::C_ciel[TAILLE_CIEL]={
						17./255.,56./255.,155./255.,
						0.3,0.5,1.,
						0.3,0.5,1.,
						19./255.,39./255.,89./255.
						};
//Lumière ambiante:
float HypnoStructure::L_amb=0.2;
float HypnoStructure::C_amb[3]={0.3,0.5,1.};
float HypnoStructure::distance_brouillard=5000.;

//Lumière ponctuelle:
float HypnoStructure::L1[TAILLE_LUM];


//Sol:
float HypnoStructure::Sol[TAILLE_OBJ];

float HypnoStructure::objets[TAILLE_OBJ*HS_NUM_OBJETS];	//Le tableau contenant les objets d'arrière plan de la scène

//Commandes clavier:
bool HypnoStructure::dep_droite=false;
bool HypnoStructure::dep_gauche=false;
bool HypnoStructure::dep_avant=false;
bool HypnoStructure::dep_arriere=false;
bool HypnoStructure::dep_haut=false;
bool HypnoStructure::dep_bas=false;
bool HypnoStructure::rot_L_gauche=false;
bool HypnoStructure::rot_L_droite=false;
bool HypnoStructure::rot_T_haut=false;
bool HypnoStructure::rot_T_bas=false;
bool HypnoStructure::rot_R_gauche=false;
bool HypnoStructure::rot_R_droite=false;

int HypnoStructure::depId=1;

//=============================================================
//								Formes
//=============================================================


bool HypnoStructure::init_scene_hypnostructure()
{
    glEnable(GL_NORMALIZE);
    glDisable(GL_COLOR_MATERIAL);
	genere_disque(1.,NUM_COTES_DISQUE,coordsDisque);
	genere_cylindre(1.,1.,NUM_COTES_CYL,NUM_SECTIONS_CYL,coordsCylindre);
	genere_faces_cylindre(NUM_COTES_CYL,NUM_SECTIONS_CYL,facesCylindre);
	genere_sphere(1.,NUM_COTES_SPHERE,NUM_SEGMENTS_SPHERE,coordsSphere);
	genere_faces_sphere(NUM_COTES_SPHERE,NUM_SEGMENTS_SPHERE,facesSphere);
	genere_tore(1.,0.008,NUM_COTES_TORE,NUM_SECTIONS_TORE,coordsTore);
	genere_faces_tore(NUM_COTES_TORE,NUM_SECTIONS_TORE,facesTore);
	genere_normales_cylindre(NUM_POINTS_CYL,coordsCylindre,normalesCylindre);
	genere_normales_tore(1.,NUM_POINTS_TORE,coordsTore,normalesTore);
	genere_normales_sphere(NUM_POINTS_SPHERE,coordsSphere,normalesSphere);

	disque_liste_id=glGenLists(1);
	sphere_liste_id=glGenLists(1);
	cylindre_liste_id=glGenLists(1);
	tore_liste_id=glGenLists(1);

    glUseProgram(0);
	glNewList(disque_liste_id,GL_COMPILE);
	affiche_disque();
	glEndList();
	glNewList(sphere_liste_id,GL_COMPILE);
	affiche_sphere();
	glEndList();
	glNewList(cylindre_liste_id,GL_COMPILE);
	affiche_cylindre();
	glEndList();
	glNewList(tore_liste_id,GL_COMPILE);
	affiche_tore();
	glEndList();

    //Arrière plan (raytracing):
    HypnoStructure::Sol[OBJTYPE]=OBJTYPE_SOL;

	HypnoStructure::Sol[CR]=1.;//51./255.;
	HypnoStructure::Sol[CV]=1.;//179/255.;
	HypnoStructure::Sol[CB]=1.;//102./255.;

	HypnoStructure::Sol[DIFF]=1.;
	HypnoStructure::Sol[SPEC]=.25;
	HypnoStructure::Sol[BRILL]=4.;

	HypnoStructure::Sol[TRANSP]=0.5;
	HypnoStructure::Sol[REFLEC]=0.5;
	HypnoStructure::Sol[REFRAC]=1.;

	Sol[C2R]=0.;//26./255.;
	Sol[C2V]=0.;//89./255.;
	Sol[C2B]=0.;//51./255.;
	Sol[CAR_XL]=100.;
	Sol[CAR_ZL]=100000.;

	Raytracer01::init_lum(L1,0.,1950,-760,	1.,1.,1.,	1.,0.001);


	return true;
}


void HypnoStructure::detruit_hypnostructure()
{
	if(glIsList(disque_liste_id)) glDeleteLists(disque_liste_id,1);
	if(glIsList(sphere_liste_id)) glDeleteLists(sphere_liste_id,1);
	if(glIsList(cylindre_liste_id)) glDeleteLists(cylindre_liste_id,1);
	if(glIsList(tore_liste_id)) glDeleteLists(tore_liste_id,1);
}

void HypnoStructure::affiche_disque()
{
	glDisable(GL_CULL_FACE);

	glShadeModel( GL_FLAT );
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
	glColor3f(1.,1.,1.);

	float amb[4],diff[4],lum[4],spec[4];
	amb[0]=C_amb[0]*L_amb;
	amb[1]=C_amb[1]*L_amb;
	amb[2]=C_amb[2]*L_amb;
	amb[3]=1.;
	diff[0]=255./255.;
	diff[1]=154/255.;
	diff[2]=34./255.;
	diff[3]=1.;
	lum[0]=0.;
	lum[1]=0.;
	lum[2]=0.;
	lum[3]=1.;
	spec[0]=1.;
	spec[1]=1.;
	spec[2]=1.;
	spec[3]=1.;

	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT, amb);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, diff);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION, lum);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, spec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 40.);

    glBegin( GL_POLYGON );
	glNormal3f(0.,1.,0.);
	for(int i=0;i<NUM_COTES_DISQUE;i++) glVertex3fv(&coordsDisque[3*i]);
	glEnd( );
}

void HypnoStructure::affiche_cylindre()
{
	int i;

	/*
	glBegin( GL_LINE_LOOP );
	glColor3f(1.,1.,1.);

	for(i=0;i<NUM_POINTS_CYL;i++) glVertex3fv(&coordsCylindre[3*i]);
	glEnd( );
	*/

	glEnable(GL_CULL_FACE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);
	glShadeModel( GL_FLAT );


		glColor3f(1.,1.,0.);
		float amb[4],diff[4],lum[4],spec[4];
        amb[0]=C_amb[0]*L_amb;
        amb[1]=C_amb[1]*L_amb;
        amb[2]=C_amb[2]*L_amb;
        amb[3]=1.;
        diff[0]=0.;
        diff[1]=1.;
        diff[2]=0.;
        diff[3]=1.;
        lum[0]=0.;
        lum[1]=0.;
        lum[2]=0.;
        lum[3]=1.;
        spec[0]=L1[CR]*L1[INTENS];
        spec[1]=L1[CV]*L1[INTENS];
        spec[2]=L1[CB]*L1[INTENS];
        spec[3]=1.;

        glMaterialfv(GL_FRONT,GL_AMBIENT, amb);
        glMaterialfv(GL_FRONT,GL_DIFFUSE, diff);
        glMaterialfv(GL_FRONT,GL_EMISSION, lum);
        glMaterialfv(GL_FRONT,GL_SPECULAR, spec);
        glMaterialf(GL_FRONT, GL_SHININESS, 40.);

		glBegin(GL_QUADS);
            uint32_t n=0;

            for(i=0;i<NUM_FACES_CYL-2;i++)
            {
                glNormal3fv(&normalesCylindre[facesCylindre[n]*3]);
                glVertex3fv(&coordsCylindre[facesCylindre[n++]*3]);
                glNormal3fv(&normalesCylindre[facesCylindre[n]*3]);
                glVertex3fv(&coordsCylindre[facesCylindre[n++]*3]);
                glNormal3fv(&normalesCylindre[facesCylindre[n]*3]);
                glVertex3fv(&coordsCylindre[facesCylindre[n++]*3]);
                glNormal3fv(&normalesCylindre[facesCylindre[n]*3]);
                glVertex3fv(&coordsCylindre[facesCylindre[n++]*3]);
            }
		glEnd();

		n=4*(NUM_FACES_CYL-2);

		glBegin(GL_POLYGON);
            glNormal3f(0.,-1.,0.);
            for(i=0;i<NUM_COTES_CYL;i++)
            {
                glVertex3fv(&coordsCylindre[facesCylindre[n++]*3]);
            }
        glEnd();

        glBegin(GL_POLYGON);
            glNormal3f(0.,1.,0.);
            for(i=0;i<NUM_COTES_CYL;i++)
            {
                glVertex3fv(&coordsCylindre[facesCylindre[n++]*3]);
            }
		glEnd();

	/*
	glBegin( GL_LINES );
	glColor3f(1.,0.,0.);
	for(i=0;i<NUM_POINTS_CYL;i++)
	{
		glVertex3fv(&coordsCylindre[3*i]);
		glVertex3f(coordsCylindre[3*i]+normalesCylindre[3*i],
					coordsCylindre[3*i+1]+normalesCylindre[3*i+1],
					coordsCylindre[3*i+2]+normalesCylindre[3*i+2]);
	}
	glEnd();
	*/
}

void HypnoStructure::affiche_sphere(float luminosite)
{
	int i;
	/*
	glBegin( GL_LINE_LOOP );
	glColor3f(1.,1.,1.);
	for(i=0;i<NUM_POINTS_SPHERE;i++) glVertex3fv(&coordsSphere[3*i]);
	glEnd( );
	*/

	glEnable(GL_CULL_FACE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);
	glShadeModel( GL_SMOOTH );

		glColor3f(1.,1.,0.);
		float amb[4],diff[4],lum[4],spec[4];
        amb[0]=C_amb[0]*L_amb;
        amb[1]=C_amb[1]*L_amb;
        amb[2]=C_amb[2]*L_amb;
        amb[3]=1.;
        diff[0]=255./255.;
        diff[1]=248./255.;
        diff[2]=108./255.;
        diff[3]=1.;
        lum[0]=luminosite*L1[CR]*L1[INTENS];
        lum[1]=luminosite*L1[CV]*L1[INTENS];
        lum[2]=luminosite*L1[CB]*L1[INTENS];
        lum[3]=1.;
        spec[0]=L1[CR]*L1[INTENS];
        spec[1]=L1[CV]*L1[INTENS];
        spec[2]=L1[CB]*L1[INTENS];
        spec[3]=1.;

        glMaterialfv(GL_FRONT,GL_AMBIENT, amb);
        glMaterialfv(GL_FRONT,GL_DIFFUSE, diff);
        glMaterialfv(GL_FRONT,GL_EMISSION, lum);
        glMaterialfv(GL_FRONT,GL_SPECULAR, spec);
        glMaterialf(GL_FRONT, GL_SHININESS, 40.);


		glBegin(GL_QUADS);
            uint32_t n=0;

            for(i=0;i<NUM_FACES_SPHERE-2*NUM_COTES_SPHERE;i++)
            {
                glNormal3fv(&normalesSphere[facesSphere[n]*3]);
                glVertex3fv(&coordsSphere[facesSphere[n++]*3]);
                glNormal3fv(&normalesSphere[facesSphere[n]*3]);
                glVertex3fv(&coordsSphere[facesSphere[n++]*3]);
                glNormal3fv(&normalesSphere[facesSphere[n]*3]);
                glVertex3fv(&coordsSphere[facesSphere[n++]*3]);
                glNormal3fv(&normalesSphere[facesSphere[n]*3]);
                glVertex3fv(&coordsSphere[facesSphere[n++]*3]);
            }

		glEnd();

		glBegin(GL_TRIANGLES);

            for(i=0;i<2*NUM_COTES_SPHERE;i++)
            {
                glNormal3fv(&normalesSphere[facesSphere[n]*3]);
                glVertex3fv(&coordsSphere[facesSphere[n++]*3]);
                glNormal3fv(&normalesSphere[facesSphere[n]*3]);
                glVertex3fv(&coordsSphere[facesSphere[n++]*3]);
                glNormal3fv(&normalesSphere[facesSphere[n]*3]);
                glVertex3fv(&coordsSphere[facesSphere[n++]*3]);
            }

		glEnd();

	/*
	glBegin( GL_LINES );
	glColor3f(1.,0.,0.);
	for(int i=0;i<NUM_POINTS_SPHERE;i++)
	{
		glVertex3fv(&coordsSphere[3*i]);
		glVertex3f(coordsSphere[3*i]+normalesSphere[3*i],
					coordsSphere[3*i+1]+normalesSphere[3*i+1],
					coordsSphere[3*i+2]+normalesSphere[3*i+2]);
	}
	glEnd();
	*/
}

void HypnoStructure::affiche_tore()
{
	int i;
	/*
	glBegin( GL_LINE_LOOP );
	glColor3f(1.,1.,1.);
	for(int i=0;i<NUM_POINTS_TORE;i++) glVertex3fv(&coordsTore[3*i]);
	glEnd( );
	*/

	glEnable(GL_CULL_FACE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);
	glShadeModel( GL_FLAT );

		glColor3f(1.,1.,0.);
		float amb[4],diff[4],lum[4],spec[4];
        amb[0]=C_amb[0]*L_amb;
        amb[1]=C_amb[1]*L_amb;
        amb[2]=C_amb[2]*L_amb;
        amb[3]=1.;
        diff[0]=0.;
        diff[1]=1.;
        diff[2]=1.;
        diff[3]=1.;
        lum[0]=0.;
        lum[1]=0.;
        lum[2]=0.;
        lum[3]=1.;
        spec[0]=L1[CR]*L1[INTENS];
        spec[1]=L1[CV]*L1[INTENS];
        spec[2]=L1[CB]*L1[INTENS];
        spec[3]=1.;

        glMaterialfv(GL_FRONT,GL_AMBIENT, amb);
        glMaterialfv(GL_FRONT,GL_DIFFUSE, diff);
        glMaterialfv(GL_FRONT,GL_EMISSION, lum);
        glMaterialfv(GL_FRONT,GL_SPECULAR, spec);
        glMaterialf(GL_FRONT, GL_SHININESS, 40.);

		glBegin(GL_QUADS);
            uint32_t n=0;

            for(i=0;i<NUM_FACES_TORE;i++)
            {
                glNormal3fv(&normalesTore[facesTore[n]*3]);
                glVertex3fv(&coordsTore[facesTore[n++]*3]);
                glNormal3fv(&normalesTore[facesTore[n]*3]);
                glVertex3fv(&coordsTore[facesTore[n++]*3]);
                glNormal3fv(&normalesTore[facesTore[n]*3]);
                glVertex3fv(&coordsTore[facesTore[n++]*3]);
                glNormal3fv(&normalesTore[facesTore[n]*3]);
                glVertex3fv(&coordsTore[facesTore[n++]*3]);
            }

		glEnd();

	/*
	glBegin( GL_LINES );
	glColor3f(1.,0.,0.);
	for(int i=0;i<NUM_POINTS_TORE;i++)
	{
		glVertex3fv(&coordsTore[3*i]);
		glVertex3f(coordsTore[3*i]+normalesTore[3*i]/10.,
					coordsTore[3*i+1]+normalesTore[3*i+1]/10.,
					coordsTore[3*i+2]+normalesTore[3*i+2]/10.);
	}
	glEnd();
	*/

}



void HypnoStructure::affiche_tige(float r_disque,float r_cyl,float h_cyl,float echelle_sphere,uint16_t numSpheresCyl, float repartitionSphereCyl,uint16_t iter)
{
	glPushMatrix();
		glTranslatef(0.,h_cyl/2.+r_disque,0.);
		glRotatef(90,1.,0.,0.);
		glScalef(r_disque,r_disque,r_disque);
		if(iter<itermax)affiche_elements(0.5,12,10.,angle_rot_roue*10,iter+1);
		else glCallList(disque_liste_id);//affiche_disque();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.,-h_cyl/2.-r_disque,0.);
		glRotatef(-90,1.,0.,0.);
		glScalef(r_disque,r_disque,r_disque);
		if(iter<itermax)affiche_elements(0.5,12,10.,angle_rot_roue*5,iter+1);
		else glCallList(disque_liste_id);//affiche_disque();
	glPopMatrix();

    /*
	glPushMatrix();
		glScalef(r_cyl,h_cyl,r_cyl);
		glCallList(cylindre_liste_id);
	glPopMatrix();
    */

	//Sphères le long des cylindres:
	glTranslatef(0.,-h_cyl*repartitionSphereCyl/2.,0.);
	for(int j=0;j<numSpheresCyl;j++)
	{
		float e=echelle_sphere*(0.25+0.75*sinf(M_PI/(numSpheresCyl-1)*j));
		glPushMatrix();
		glScalef(e,e,e);
		glCallList(sphere_liste_id);
		glPopMatrix();
		glTranslatef(0.,h_cyl*repartitionSphereCyl/(numSpheresCyl-1),0.);
	}
}



void HypnoStructure::affiche_elements(float echelle_tore, uint16_t num_roues,float decallage_angle_roue,float angle,uint16_t iter)
{
	 float angle_roue=360./num_roues;
	 float r_cyl=echelle_tore/200.;
	 float h_cyl=echelle_tore/.7;
	 float r_disque=echelle_tore/5.;
	 float echelle_sphere=echelle_tore/16.;

    /*
	glPushMatrix();
		glRotatef(90.,1.,0.,0.);
		glScalef(echelle_tore,echelle_tore,echelle_tore);
		glCallList(tore_liste_id);
	glPopMatrix();
	*/

	glPushMatrix();
		for (int i=0;i<num_roues;i++)
		{
			glRotatef(angle_roue,0.,0.,1.);

			glPushMatrix();
				glTranslatef(0.,-echelle_tore,0.);

				glPushMatrix();
					glRotatef(angle+i*decallage_angle_roue,1.,0.,0.);
					affiche_tige(r_disque,r_cyl,h_cyl,echelle_sphere,6,0.5,iter);
				glPopMatrix();

				glPushMatrix();
					glRotatef(90.+angle+i*decallage_angle_roue,1.,0.,0.);
					affiche_tige(r_disque,r_cyl,h_cyl,echelle_sphere,6,0.5,iter);
				glPopMatrix();
			glPopMatrix();
		}
	glPopMatrix();
}

void HypnoStructure::clavier(int key,bool etat)
{
    if(!GVars::editionTrajectoire)
    {
        switch (key)
        {
            case GLFW_KEY_RIGHT: dep_droite=etat;break;
            case GLFW_KEY_LEFT: dep_gauche=etat;break;
            case GLFW_KEY_UP: dep_avant=etat ;break;
            case GLFW_KEY_DOWN: dep_arriere=etat;break;

            case GLFW_KEY_KP_4:rot_L_droite=etat;break;
            case GLFW_KEY_KP_6:rot_L_gauche=etat;break;
            case GLFW_KEY_KP_8:rot_T_bas=etat;break;
            case GLFW_KEY_KP_2:rot_T_haut=etat;break;
            case GLFW_KEY_KP_1:rot_R_droite=etat;break;
            case GLFW_KEY_KP_3:rot_R_gauche=etat;break;
            case GLFW_KEY_PAGE_UP:dep_haut=etat;break;
            case GLFW_KEY_PAGE_DOWN:dep_bas=etat;break;

            case GLFW_KEY_F1: depId=1;break;
            case GLFW_KEY_F2: depId=2;break;
            case GLFW_KEY_F3: depId=3;break;
            case GLFW_KEY_F4: depId=4;break;
            case GLFW_KEY_F5: depId=5;break;
        }
    }
    else
    {
        /*
        Volume* cosmo=GVars::cosmo->renvoie_volume_parent();
        EditeurTrajectoire::ET_x=cosmo->x;
        EditeurTrajectoire::ET_y=cosmo->y;
        EditeurTrajectoire::ET_z=cosmo->z;
        EditeurTrajectoire::edit_trajectoire(key,etat,EditeurTrajectoire::ma_trajectoire);
        cosmo->x=EditeurTrajectoire::ET_x;
        cosmo->y=EditeurTrajectoire::ET_y;
        cosmo->z=EditeurTrajectoire::ET_z;
        */

        EditeurTrajectoire::ET_x=obs.x;
        EditeurTrajectoire::ET_y=obs.y;
        EditeurTrajectoire::ET_z=-obs.z;
        EditeurTrajectoire::edit_trajectoire(key,etat);
        obs.x=EditeurTrajectoire::ET_x;
        obs.y=EditeurTrajectoire::ET_y;
        obs.z=-EditeurTrajectoire::ET_z;

        /*
        EditeurTrajectoire::ET_x=point_vise.x;
        EditeurTrajectoire::ET_y=point_vise.y;
        EditeurTrajectoire::ET_z=-point_vise.z;
        EditeurTrajectoire::edit_trajectoire(key,etat,EditeurTrajectoire::ma_trajectoire);
        point_vise.x=EditeurTrajectoire::ET_x;
        point_vise.y=EditeurTrajectoire::ET_y;
        point_vise.z=-EditeurTrajectoire::ET_z;
        */

        switch (key)
        {
            case GLFW_KEY_RIGHT: dep_droite=etat;break;
            case GLFW_KEY_LEFT: dep_gauche=etat;break;
            case GLFW_KEY_UP: dep_avant=etat ;break;
            case GLFW_KEY_DOWN: dep_arriere=etat;break;

            case GLFW_KEY_KP_4:rot_L_droite=etat;break;
            case GLFW_KEY_KP_6:rot_L_gauche=etat;break;
            case GLFW_KEY_KP_8:rot_T_bas=etat;break;
            case GLFW_KEY_KP_2:rot_T_haut=etat;break;
            case GLFW_KEY_KP_1:rot_R_droite=etat;break;
            case GLFW_KEY_KP_3:rot_R_gauche=etat;break;
            case GLFW_KEY_PAGE_UP:dep_haut=etat;break;
            case GLFW_KEY_PAGE_DOWN:dep_bas=etat;break;

            case GLFW_KEY_1: depId=1;break;
            case GLFW_KEY_2: depId=2;break;
            case GLFW_KEY_3: depId=3;break;
            case GLFW_KEY_4: depId=4;break;
            case GLFW_KEY_5: depId=5;break;
        }
    }
}


void HypnoStructure::maj_deplacement()
{
    float deceleration=0.75;
    float acceleration=.05;

    float decel_ang=.85;
    float accel_ang=0.07;

    aX*=deceleration;
    aY*=deceleration;
    aZ*=deceleration;

    aR*=decel_ang;
    aT*=decel_ang;
    aL*=decel_ang;

    if(dep_droite)aX=aX/deceleration+acceleration;
    if(dep_gauche)aX=aX/deceleration-acceleration;

    if(dep_haut)aY=aY/deceleration+acceleration;
    if(dep_bas)aY=aY/deceleration-acceleration;

	if(dep_avant)aZ=aZ/deceleration+acceleration;
    if(dep_arriere)aZ=aZ/deceleration-acceleration;

	if(rot_L_droite)aL=aL/decel_ang+accel_ang;
    if(rot_L_gauche)aL=aL/decel_ang-accel_ang;
    if(rot_T_bas)aT=aT/decel_ang+accel_ang;
    if(rot_T_haut)aT=aT/decel_ang-accel_ang;
    if(rot_R_droite)aR=aR/decel_ang-accel_ang;
    if(rot_R_gauche)aR=aR/decel_ang+accel_ang;



	angle_rot_roue+=vitesse_rot_roue*(GVars::temps_scene-temps_prec);
	temps_prec=GVars::temps_scene;

	if(depId==1)
    {
        /*
        float raX,raZ;
        GFunc::rotation_plan(-obs.l,aX,aZ,raX,raZ);
        */

        structure.x+=aX;
        structure.y+=aY;
        structure.z+=aZ;

        structure.r+=aR;
		structure.t+=aT;
		structure.l+=aL;
		if (GVars::disp_donnees)
        {
            cout<<"Structure - X="<<structure.x<<" Y="<<structure.y<<" Z="<<structure.z<<" R="<<structure.r<<" T="<<structure.t<<" L="<<structure.l<<endl;
        }
    }

    if(depId==2)
    {
        float raX,raZ;
        GFunc::rotation_plan(-obs.l,aX,aZ,raX,raZ);

        obs.x+=raX;
        obs.y+=aY;
        obs.z+=raZ;

        obs.r+=aR;
		obs.t+=aT;
		obs.l-=aL;
		if (GVars::disp_donnees)
        {
            cout<<"Obs - X="<<obs.x<<" Y="<<obs.y<<" Z="<<obs.z<<" R="<<obs.r<<" T="<<obs.t<<" L="<<obs.l<<endl;

        }
    }
    if (depId==3)
	{
		L1[POSX]+=aX;
		L1[POSY]+=aY;
		L1[POSZ]+=aZ;
		if (GVars::disp_donnees)
        {
            cout<<"L1 - X="<<L1[POSX]<<" Y="<<L1[POSY]<<" Z="<<L1[POSZ]<<endl;

        }
	}

	if (depId==4)
	{
		point_vise.x+=aX;
		point_vise.y+=aY;
		point_vise.z+=aZ;
		if (GVars::disp_donnees)
        {
            cout<<"Point visé - X="<<point_vise.x<<" Y="<<point_vise.y<<" Z="<<point_vise.z<<endl;

        }
	}

    if (depId==5)
	{
		float raX,raZ;
		Volume* cosmonaute=GVars::cosmo->renvoie_volume_parent();
		//GFunc::rotation_plan(-cosmonaute->l,aX,aZ,raX,raZ);
            raX=aX;
            raZ=aZ;

		cosmonaute->x+=raX;
		cosmonaute->y+=aY;
		cosmonaute->z-=raZ;

		cosmonaute->r+=aR;
		cosmonaute->t+=aT;
		cosmonaute->l+=aL;

		if (GVars::disp_donnees)
        {
            cout<<"Cosmonaute - X="<<cosmonaute->x<<" Y="<<cosmonaute->y<<" Z="<<cosmonaute->z<<" H="<<GVars::cosmo->echelle<<" R="<<cosmonaute->r<<" T="<<cosmonaute->t<<" L="<<cosmonaute->l<<endl;

        }
	}

        GVars::disp_donnees=false;
        /*
        structure.l+=0.025*GVars::temps_scene;
        structure.t+=0.01*GVars::temps_scene;
        structure.r-=0.0166*GVars::temps_scene;
        */
}

//====================== Inits affichage arrière plan (raytracing) =============

int HypnoStructure::stock_objet(int j,float obj[TAILLE_OBJ])
{
	for(int i=0;i<TAILLE_OBJ;i++)objets[j++]=obj[i];
	return j;
}

void HypnoStructure::init_tableau_objets()
{
	int j=0;
	j=stock_objet(j,Sol);
}
//==============================================================================

void HypnoStructure::init_affiche_hypnostructure_ap() //ap="Arrière Plan"
{
    GFunc::init_matrice_observateur_raytrace(&obs);

    GFunc::vue_ortho();
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	//glDepthMask(GL_FALSE);
	glDisable(GL_LIGHTING);

    init_tableau_objets();
	glUseProgram(ShadersDemo::programme_hypnostructure);
	glUniform1f(ShadersDemo::uniform_hypnostructure_random_numer,(float)rand()/32768.);
	glUniform1f(ShadersDemo::uniform_hypnostructure_temps,glfwGetTime());
	glUniform1i(ShadersDemo::uniform_hypnostructure_numReflec,numReflec);
	glUniform1i(ShadersDemo::uniform_hypnostructure_numRefrac,numRefrac);
	glUniformMatrix4fv(ShadersDemo::uniform_hypnostructure_Obs,1,GL_FALSE,GVars::ObsMat->matrice);
	glUniform2f(ShadersDemo::uniform_hypnostructure_ecranDim,GVars::largeur_ecran,GVars::hauteur_ecran);
	glUniform1f(ShadersDemo::uniform_hypnostructure_distanceFocale,GVars::distanceFocale);
	glUniform1f(ShadersDemo::uniform_hypnostructure_ecran_ratio,GVars::ecran_ratio);
	glUniform1f(ShadersDemo::uniform_hypnostructure_distanceBrouillard,distance_brouillard);
	glUniform1fv(ShadersDemo::uniform_hypnostructure_C_ciel,TAILLE_CIEL,C_ciel);
	glUniform3fv(ShadersDemo::uniform_hypnostructure_C_amb,1,C_amb);
	glUniform1f(ShadersDemo::uniform_hypnostructure_L_amb,L_amb);
	glUniform1fv(ShadersDemo::uniform_hypnostructure_L1,TAILLE_LUM,L1);
	glUniform1fv(ShadersDemo::uniform_hypnostructure_objets,TAILLE_OBJ*HS_NUM_OBJETS,objets);
}

void HypnoStructure::affiche_hypnostructure_ap()
{
	glBegin( GL_QUADS );
        glVertex3f(-1,1,-1.);
        glVertex3f(1.,1.,-1.);
        glVertex3f(1.,-1.,-1.);
        glVertex3f(-1.,-1.,-1.);
        glEnd( );
}

void HypnoStructure::init_affiche_hypnostructure()
{
    glUseProgram(0);
	GFunc::vue_perspective(GVars::zMin,GVars::zMax);
	GFunc::desactive_textures();
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	Lumieres::drapeaux_sources|=0b1;
	//Lumieres::lumiere_couleur(GL_LIGHT0, 0.,0.,0.25,	1.,1.,1.,	1.,1.,1.);
	//Lumieres::lumiere_attenuation(GL_LIGHT0,1.,0.,0.);



	Lumieres::lumiere_attenuation(GL_LIGHT0,0.,L1[ATT],0.);

    float couleur_amb[4];
    couleur_amb[0]=C_amb[0]*L_amb;
    couleur_amb[1]=C_amb[1]*L_amb;
    couleur_amb[2]=C_amb[2]*L_amb;
    couleur_amb[3]=1.;
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, couleur_amb);

    glFogi(GL_FOG_MODE,GL_LINEAR);
            glFogfv(GL_FOG_COLOR,&C_ciel[HORIZB_CR]);
            glFogf(GL_FOG_START,0.);
            glFogf(GL_FOG_END,distance_brouillard);
            glFogf(GL_FOG_DENSITY,1.);
            glHint(GL_FOG_HINT,GL_NICEST);
    glEnable(GL_FOG);


    Lumieres::lumiere_couleur(GL_LIGHT0,
                                    0.,0.,0.,
                                    L1[CR],L1[CV],L1[CB],
                                    L1[CR],L1[CV],L1[CB]);
    Lumieres::lumiere_position(GL_LIGHT0,L1[POSX],L1[POSY],-L1[POSZ]);

    //glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
}

void HypnoStructure::eclairages()
{
    float couleur_amb[4]={C_amb[0]*L_amb,C_amb[1]*L_amb,C_amb[2]*L_amb,1.};
    float couleur_lum[3]={L1[CR]*L1[INTENS],L1[CV]*L1[INTENS],L1[CB]*L1[INTENS]};
    float couleur_fog[4]={C_ciel[HORIZB_CR],C_ciel[HORIZB_CV],C_ciel[HORIZB_CB],1.};

    glEnable(GL_LIGHTING);
    glEnable(GL_FOG);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    Lumieres::drapeaux_sources=0b11;

    Lumieres::lumiere_attenuation(GL_LIGHT0,0.,L1[ATT],0.);
    Lumieres::lumiere_position(GL_LIGHT0,L1[POSX],L1[POSY],-L1[POSZ]);

    Lumieres::lumiere_couleur(GL_LIGHT0,
                                    0.,0.,0.,
                                    couleur_lum[0],couleur_lum[1],couleur_lum[2],
                                    couleur_lum[0],couleur_lum[1],couleur_lum[2]);
    Lumieres::lumiere_ouverture(GL_LIGHT0,180.);


    Lumieres::lumiere_attenuation(GL_LIGHT1,0.,0.,0.);
    Lumieres::lumiere_couleur(GL_LIGHT1,
                            0.,0.,0.,
                            .5,.02,0.,
                            0.,0.,0.);

    Lumieres::lumiere_infinie_orientation(GL_LIGHT1,0.,0.,1.);
    //Lumieres::lumiere_position(GL_LIGHT1,0.,0.,100.);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, couleur_amb);

    glFogi(GL_FOG_MODE,GL_LINEAR);
    glFogfv(GL_FOG_COLOR,couleur_fog);
    glFogf(GL_FOG_START,0.);
    glFogf(GL_FOG_END,distance_brouillard);
    glFogf(GL_FOG_DENSITY,1.);
    glHint(GL_FOG_HINT,GL_NICEST);
}

void HypnoStructure::maj_positions_lumieres()
{
    Lumieres::lumiere_position(GL_LIGHT0,L1[POSX],L1[POSY],-L1[POSZ]);
    Lumieres::lumiere_infinie_orientation(GL_LIGHT1,0.,0.,1.);
}

void HypnoStructure::affiche_hypnostructure()
{
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	GFunc::init_matrice_observateur(obs.x,obs.y,obs.z,obs.r,obs.t,obs.l);
    maj_positions_lumieres();
    glDisable(GL_COLOR_MATERIAL);
	//Lumieres::lumiere_infinie_orientation(GL_LIGHT0,1.,-1.,1.);
    glPushMatrix();
        glTranslatef(structure.x,structure.y,structure.z);
        glRotatef(structure.l,0.,1.,0.);
        glRotatef(structure.t,1.,0.,0.);
        glRotatef(structure.r,0.,0.,1.);
        affiche_elements(1000.,20,2.,angle_rot_roue,0);
	glPopMatrix();

	glDisable(GL_FOG);

    //Affiche la source lumineuse principale:

	glPushMatrix();
	glTranslatef(L1[POSX],L1[POSY],-L1[POSZ]);
	glScalef(10.,10.,10.);
	affiche_sphere(1.);
	glPopMatrix();

    //Affiche le point de visée de la caméra:

    /*
	glPushMatrix();
	glTranslatef(point_vise.x,point_vise.y,-point_vise.z);
	glScalef(10.,10.,10.);
	affiche_sphere(1.);
	glPopMatrix();
	*/


	glEnable(GL_FOG);
	//glCallList(sphere_liste_id);
}

void HypnoStructure::render_scene_hypnostructure()
{
	glPointSize(1.);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_LIGHT0);
    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHT2);
    init_affiche_hypnostructure_ap();
    affiche_hypnostructure_ap();
    init_affiche_hypnostructure();
    affiche_hypnostructure();
}
