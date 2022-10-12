#include "Elements_Ville.h"


bool Elements_Ville::drapeau_reliefs=true;
//---------- Primitives:

float Elements_Ville::coordsTore[3*VILLE_NUM_POINTS_TORE];
uint16_t Elements_Ville::facesTore[VILLE_NUM_FACES_TORE*4];
float Elements_Ville::normalesTore[3*VILLE_NUM_POINTS_TORE];

float Elements_Ville::coordsDisque[3*VILLE_NUM_COTES_DISQUE];

float Elements_Ville::coordsCylindre[3*VILLE_NUM_POINTS_CYL];
uint16_t  Elements_Ville::facesCylindre[(VILLE_NUM_FACES_CYL-2)*4+2*VILLE_NUM_COTES_CYL];
float Elements_Ville::normalesCylindre[3*VILLE_NUM_POINTS_CYL];

float Elements_Ville::coordsSphere[3*VILLE_NUM_POINTS_SPHERE +4];   //?? Bug si pas de +4 ??
uint16_t Elements_Ville::facesSphere[VILLE_NUM_FACES_SPHERE*4-(2*VILLE_NUM_COTES_SPHERE)];
float Elements_Ville::normalesSphere[3*VILLE_NUM_POINTS_SPHERE+4];



//Textures:

Image* Elements_Ville::tex_couleurs=NULL;
Image* Elements_Ville::tex_normales=NULL;
Image* Elements_Ville::tex_profondeurs=NULL;
Texgen* Elements_Ville::textureCones01=NULL;

GLuint Elements_Ville::triangle_liste_id=0;
GLuint Elements_Ville::disque_liste_id=0;
GLuint Elements_Ville::sphere_liste_id=0;
GLuint Elements_Ville::cylindre_liste_id=0;
GLuint Elements_Ville::tore_liste_id=0;
GLuint Elements_Ville::cube_liste_id=0;
GLuint Elements_Ville::cube02_liste_id=0;
GLuint Elements_Ville::rectangle_contours_liste_id=0;   //Simple lignes de contours
GLuint Elements_Ville::plan01_liste_id=0;    //Plan de tests
GLuint Elements_Ville::plan02_liste_id=0;    //Dalle de 1m*1m
GLuint Elements_Ville::tetraedre_liste_id=0;

GLuint Elements_Ville::immeuble_base_liste_id=0;
GLuint Elements_Ville::immeuble_01_liste_id=0;
GLuint Elements_Ville::immeuble_02_liste_id=0;
GLuint Elements_Ville::immeuble_03_liste_id=0;
GLuint Elements_Ville::immeuble_04_liste_id=0;

GLfloat Elements_Ville::couleur_trottoir[3]={200./255.,200./255.,200./255.};
GLfloat Elements_Ville::couleur_chaussee[3]={117./255.,117./255.,117./255.};
GLfloat Elements_Ville::couleur_troncs[3]={75./255.,49./255.,3./255.};
GLfloat Elements_Ville::couleur_feuillages[3]={48./255.,127./255.,105./255.};
GLfloat Elements_Ville::couleur_lumiere_feuillages[4]={48./255.,127./255.,105./255.,0.};
GLfloat Elements_Ville::speculaire_feuillages[4]={0.2,0.2,0.2,1.};
GLfloat Elements_Ville::speculaire_trottoir[4]={0.,0.,0.,1.};
GLfloat Elements_Ville::speculaire_chaussee[4]={0.,0.,0.,1.};
GLfloat Elements_Ville::speculaire_immeubles[4]={0.4,0.4,0.4,1.};
GLfloat Elements_Ville::speculaire_vitres[4]={1.,1.,1.,1.};
GLfloat Elements_Ville::couleur_fenetres[3]={0.01,0.01,0.02};
GLfloat Elements_Ville::couleur_epaisseur_bordures_eau[3]={247./255.,147./255.,30./255.};
GLuint Elements_Ville::route_01_droite_liste_id=0;
GLuint Elements_Ville::route_01_virage90_droite_liste_id=0;
GLuint Elements_Ville::route_01_virage90_gauche_liste_id=0;
GLuint Elements_Ville::route_01_carrefour_4_liste_id=0;
GLuint Elements_Ville::route_01_carrefour_3_liste_id=0;
GLuint Elements_Ville::route_01_parking_liste_id=0;
GLuint Elements_Ville::route_01_espace_vert_liste_id=0;

GLuint Elements_Ville::bordure_eau_base_liste_id=0;

float Elements_Ville::immeuble_base_sommets[8*3]={-0.5,1.,0.5,
                                    0.5,1.,0.5,
                                    0.5,1.,-0.5,
                                    -0.5,1.,-0.5,
                                    -0.5,0.,-0.5,
                                    0.5,0.,-0.5,
                                    0.5,0.,0.5,
                                    -0.5,0.,0.5};

uint16_t Elements_Ville::immeuble_base_faces[5*4]={0,3,2,1,
                                    0,1,6,7,
                                    1,2,5,6,
                                    2,3,4,5,
                                    0,7,4,3};

float Elements_Ville::immeuble_base_normales[5*3]={0.,1.,0.,
                                        0.,0.,1.,
                                        1.,0.,0.,
                                        0.,0.,-1.,
                                        -1.,0.,0.};

bool Elements_Ville::erreur_openGl(const char* message)
    {
        GLenum erreur_openGl=glGetError();
        if (erreur_openGl!=GL_NO_ERROR)
        {
            cout<<message<<" : "<<gluErrorString(erreur_openGl)<<endl;
            return true;
        }
        return false;
    }

void Elements_Ville::desactive_textures()
{
    uint16_t i;
    GLint nbr_unites_textures_max;
    glGetIntegerv(GL_MAX_TEXTURE_UNITS,&nbr_unites_textures_max);
    for (i=0;i<nbr_unites_textures_max;i++)
        {
            glActiveTexture(GL_TEXTURE0+i);
            glDisable(GL_TEXTURE_2D);
            glClientActiveTexture(GL_TEXTURE0+i);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }
        glActiveTexture(GL_TEXTURE0);
}

void Elements_Ville::affiche_cylindre()
{
	int i;

	/*
	glBegin( GL_LINE_LOOP );
	glColor3f(1.,1.,1.);

	for(i=0;i<NUM_POINTS_CYL;i++) glVertex3fv(&coordsCylindre[3*i]);
	glEnd( );
	*/

    //init_materiau_simple_face(0xffffffff,0x000000ff,0xffffffff,0xffffffff,40.);

    glShadeModel( GL_FLAT );

		glBegin(GL_QUADS);
            uint32_t n=0;

            for(i=0;i<VILLE_NUM_FACES_CYL-2;i++)
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

		n=4*(VILLE_NUM_FACES_CYL-2);

		glBegin(GL_POLYGON);
            glNormal3f(0.,-1.,0.);
            for(i=0;i<VILLE_NUM_COTES_CYL;i++)
            {
                glVertex3fv(&coordsCylindre[facesCylindre[n++]*3]);
            }
        glEnd();

        glBegin(GL_POLYGON);
            glNormal3f(0.,1.,0.);
            for(i=0;i<VILLE_NUM_COTES_CYL;i++)
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

void Elements_Ville::affiche_triangle()
{

	//glShadeModel( GL_FLAT );

	//init_materiau_double_face(0xffffffff,0x000000ff,0xffffffff,0xffffffff,40.);


    glBegin( GL_TRIANGLES );
        glNormal3f(0.,0.,1.);
        glVertex2fv(&triangle_sommets[0]);
        glVertex2fv(&triangle_sommets[2]);
        glVertex2fv(&triangle_sommets[4]);
	glEnd( );
}

void Elements_Ville::affiche_disque()
{

	//glShadeModel( GL_FLAT );

	//init_materiau_double_face(0xffffffff,0x000000ff,0xffffffff,0xffffffff,40.);

    glBegin( GL_POLYGON );
	glNormal3f(0.,1.,0.);
	for(int i=0;i<VILLE_NUM_COTES_DISQUE;i++) glVertex3fv(&coordsDisque[3*i]);
	glEnd( );
}

void Elements_Ville::affiche_sphere(float luminosite)
{
	int i;
	/*
	glBegin( GL_LINE_LOOP );
	glColor3f(1.,1.,1.);
	for(i=0;i<NUM_POINTS_SPHERE;i++) glVertex3fv(&coordsSphere[3*i]);
	glEnd( );
	*/

	//glShadeModel( GL_FLAT );
	//init_materiau_simple_face(0xffffffff,0x000000ff,0xffffffff,0xffffffff,40.);


		glBegin(GL_QUADS);
            uint32_t n=0;

            for(i=0;i<VILLE_NUM_FACES_SPHERE-2*VILLE_NUM_COTES_SPHERE;i++)
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

            for(i=0;i<2*VILLE_NUM_COTES_SPHERE;i++)
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

void Elements_Ville::affiche_tore()
{
	int i;
	/*
	glBegin( GL_LINE_LOOP );
	glColor3f(1.,1.,1.);
	for(int i=0;i<NUM_POINTS_TORE;i++) glVertex3fv(&coordsTore[3*i]);
	glEnd( );
	*/

	glShadeModel( GL_FLAT );
	//init_materiau_simple_face(0xffffffff,0x000000ff,0xffffffff,0xffffffff,40.);

		glBegin(GL_QUADS);
            uint32_t n=0;

            for(i=0;i<VILLE_NUM_FACES_TORE;i++)
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
	for(int i=0;i<VILLE_NUM_POINTS_TORE;i++)
	{
		glVertex3fv(&coordsTore[3*i]);
		glVertex3f(coordsTore[3*i]+normalesTore[3*i]/10.,
					coordsTore[3*i+1]+normalesTore[3*i+1]/10.,
					coordsTore[3*i+2]+normalesTore[3*i+2]/10.);
	}
	glEnd();
	*/


}

void Elements_Ville::affiche_plan01()
{
    desactive_textures();

	glShadeModel( GL_SMOOTH );

	init_materiau_simple_face(0xffffffff,0x000000ff,0xffffffff,0xf0f0f0ff,40.);


        //Texture Couleur:
        glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,tex_couleurs->texture_id);

        //Initialise la texture des normales:

        glActiveTexture(GL_TEXTURE1);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,tex_normales->texture_id);


        //Initialise la texture des profondeurs:

        glActiveTexture(GL_TEXTURE2);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,textureCones01->texture_id);
        glUseProgram(ShadersObjets3d::relief->programme_id);
        glUniform1i(ShadersObjets3d::relief->texture_couleur,0);
        glUniform1i(ShadersObjets3d::relief->texture_normales,1);
        glUniform1i(ShadersObjets3d::relief->texture_profondeurs,2);
        glUniform1f(ShadersObjets3d::relief->echelle_texture,1.);

        /*
        if(drapeau_reliefs)glUniform1i(ShadersObjets3d::relief->relief_on,1);
        else glUniform1i(ShadersObjets3d::relief->relief_on,0);
            */
            glUniform1i(ShadersObjets3d::relief->relief_on,drapeau_reliefs);


        GLfloat tangente[3]={1.,0.,0.};
        GLfloat binormale[3]={0.,1.,0.};
        GLfloat normale[3]={0.,0.,1.};
        GLfloat uv[2*4]={0,0, 1,0, 1,1, 0,1};


		glBegin(GL_QUADS);
                glNormal3fv(&normale[0]);

                glVertexAttrib3fv(ShadersObjets3d::relief->tangente,tangente);
                glVertexAttrib3fv(ShadersObjets3d::relief->binormale,binormale);

                glMultiTexCoord2fv(GL_TEXTURE0,&uv[6]);
                //glMultiTexCoord2fv(GL_TEXTURE1,&uv[0]);
                //glMultiTexCoord2fv(GL_TEXTURE2,&uv[0]);
                glVertex2fv(&plan_sommets[6]);

                glMultiTexCoord2fv(GL_TEXTURE0,&uv[4]);
                //glMultiTexCoord2fv(GL_TEXTURE1,&uv[2]);
                //glMultiTexCoord2fv(GL_TEXTURE2,&uv[2]);
                glVertex2fv(&plan_sommets[4]);

                glMultiTexCoord2fv(GL_TEXTURE0,&uv[2]);
                //glMultiTexCoord2fv(GL_TEXTURE1,&uv[4]);
                //glMultiTexCoord2fv(GL_TEXTURE2,&uv[4]);
                glVertex2fv(&plan_sommets[2]);

                glMultiTexCoord2fv(GL_TEXTURE0,&uv[0]);
                //glMultiTexCoord2fv(GL_TEXTURE1,&uv[6]);
                //glMultiTexCoord2fv(GL_TEXTURE2,&uv[6]);
                glVertex2fv(&plan_sommets[0]);
		glEnd();
        glUseProgram(0);

}

// Simple carré
void Elements_Ville::affiche_plan02()
{
    GLfloat normale[3]={0.,0.,1.};

		glBegin(GL_QUADS);
                glNormal3fv(&normale[0]);
                glVertex2fv(&plan_sommets[6]);
                glVertex2fv(&plan_sommets[4]);
                glVertex2fv(&plan_sommets[2]);
                glVertex2fv(&plan_sommets[0]);
		glEnd();

}

// Contours d'un carré (ou rectangle)
void Elements_Ville::affiche_rectangle_contours()
{
		glBegin(GL_LINE_LOOP);
                glVertex2fv(&plan_sommets[6]);
                glVertex2fv(&plan_sommets[4]);
                glVertex2fv(&plan_sommets[2]);
                glVertex2fv(&plan_sommets[0]);
		glEnd();
}


void Elements_Ville::affiche_cube()
{
	int i;

	glShadeModel( GL_FLAT );
	glActiveTexture(GL_TEXTURE2);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);

	init_materiau_simple_face(0xffffffff,0x000000ff,0xffffffff,0x808080f,40.);
    float diff_cube[6*4]= {1.,0.,0.,1.,
                            0.,1.,0.,1.,
                            0.,0.,1.,1.,
                            .8,.8,0.,1.,
                            .8,0.,0.8,1.,
                            0.,.8,.8,1.};

		glBegin(GL_QUADS);
            uint32_t n=0;

            for(i=0;i<6;i++)
            {
                glNormal3fv(&cube_normales[i*3]);

                glColor4fv(&diff_cube[4*i]);
                glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, &diff_cube[4*i]);
                glVertex3fv(&cube_sommets[cube_faces[i*4]*3]);
                glVertex3fv(&cube_sommets[cube_faces[i*4+1]*3]);
                glVertex3fv(&cube_sommets[cube_faces[i*4+2]*3]);
                glVertex3fv(&cube_sommets[cube_faces[i*4+3]*3]);
            }

		glEnd();
}

void Elements_Ville::affiche_cube02()
{
	int i;
		glBegin(GL_QUADS);

            for(i=0;i<6;i++)
            {
                glNormal3fv(&cube_normales[i*3]);
                glVertex3fv(&cube_sommets[cube_faces[i*4]*3]);
                glVertex3fv(&cube_sommets[cube_faces[i*4+1]*3]);
                glVertex3fv(&cube_sommets[cube_faces[i*4+2]*3]);
                glVertex3fv(&cube_sommets[cube_faces[i*4+3]*3]);
            }

		glEnd();
}

void Elements_Ville::affiche_tetraedre()
{
	int i;
		glBegin(GL_TRIANGLES);

            for(i=0;i<4;i++)
            {
                glNormal3fv(&tetraedre_normales[i*3]);
                glVertex3fv(&tetraedre_sommets[tetraedre_faces[i*3]*3]);
                glVertex3fv(&tetraedre_sommets[tetraedre_faces[i*3+1]*3]);
                glVertex3fv(&tetraedre_sommets[tetraedre_faces[i*3+2]*3]);
            }
		glEnd();
}



void Elements_Ville::genere_route01_droite(float echelle)
{
    int nbrSommets=8;

    GLfloat coords[3*8]={
                        0.,0.,0.,
                        0.,0.,-0.25,
                        0.,0.,-0.75,
                        0.,0.,-1.,
                        1.,0.,-1.,
                        1.,0.,-0.75,
                        1.,0.,-0.25,
                        1.,0.,0.
                        };
    GLfloat normales[1*3]=
                        {
                            0.,1.,0.
                        };


    //Mise à l'échelle pour éviter les glScale à tout bout de champ.
    for(int i=0;i<nbrSommets;i++)
    {
        coords[3*i]=(coords[3*i]-0.5)*echelle;
        coords[3*i+2]=(coords[3*i+2]+0.5)*echelle;
    }

    glNewList(route_01_droite_liste_id,GL_COMPILE);
        glMaterialfv(GL_FRONT,GL_SPECULAR, speculaire_trottoir);
        glMaterialf(GL_FRONT,GL_SHININESS, 20.);
        glBegin(GL_QUADS);
            glColor3fv(couleur_trottoir);
            glNormal3fv(&normales[0*3]);

            glVertex3fv(&coords[0*3]);
            glVertex3fv(&coords[1*3]);
            glVertex3fv(&coords[6*3]);
            glVertex3fv(&coords[7*3]);

            glVertex3fv(&coords[2*3]);
            glVertex3fv(&coords[3*3]);
            glVertex3fv(&coords[4*3]);
            glVertex3fv(&coords[5*3]);
        glEnd();
        glMaterialfv(GL_FRONT,GL_SPECULAR, speculaire_chaussee);
        glMaterialf(GL_FRONT,GL_SHININESS, 30.);
        glBegin(GL_QUADS);
            glColor3fv(couleur_chaussee);

            glVertex3fv(&coords[1*3]);
            glVertex3fv(&coords[2*3]);
            glVertex3fv(&coords[5*3]);
            glVertex3fv(&coords[6*3]);
        glEnd();
	glEndList();
}


void Elements_Ville::genere_coords_cercle( float* coords,float cx, float cz, float r,int nbrSegments,float angleDepart, float angle)
{
    int i;
    float x,z;
    int cnt=nbrSegments;
    if(angle==360.)cnt--;
    for (i=0;i<=cnt;i++)
    {
        x=r*cos(DEG_RAD*(angleDepart+angle/float(nbrSegments)*float(i)));
        z=r*sin(DEG_RAD*(angleDepart+angle/float(nbrSegments)*float(i)));
        coords[2*i]=x+cx;
        coords[2*i+1]=-z+cz;

    }
}

void Elements_Ville::genere_route01_virage90_droite(float echelle)
{
    int nbrSegments=8;
    int nbrSommets=4+(nbrSegments+1)*2;

    int i;
    float coords_cercle[(nbrSegments+1)*2];
    float coords[3*nbrSommets];
    coords[0]=0.;coords[1]=0.;coords[2]=0.;
    coords[3]=0.;coords[4]=0.;coords[5]=-1.;
    coords[6]=1.;coords[7]=0.;coords[8]=-1.;
    coords[9]=1.;coords[10]=0.;coords[11]=0.;
    genere_coords_cercle(coords_cercle,0.,0.,0.75,nbrSegments,0.,90.);
    int ptr=4;
    for(i=0;i<=nbrSegments;i++)
    {
        coords[ptr*3]=coords_cercle[2*i]/3.;
        coords[ptr*3+1]=0;
        coords[ptr*3+2]=coords_cercle[2*i+1]/3.;
        ptr++;
    }
    for(i=0;i<=nbrSegments;i++)
    {
        coords[ptr*3]=coords_cercle[2*i];
        coords[ptr*3+1]=0;
        coords[ptr*3+2]=coords_cercle[2*i+1];
        ptr++;
    }

    GLfloat normales[1*3]=
                        {
                            0.,1.,0.
                        };


    //Mise à l'échelle pour éviter les glScale à tout bout de champ.
    for(i=0;i<nbrSommets;i++)
    {
        coords[3*i]=(coords[3*i]-0.5)*echelle;
        coords[3*i+2]=(coords[3*i+2]+0.5)*echelle;
    }

    glNewList(route_01_virage90_droite_liste_id,GL_COMPILE);

        /*
        glBegin(GL_POINTS);
            glColor3fv(couleur_trottoir);
            glNormal3fv(&normales[0*3]);
            for(i=0;i<nbrSommets;i++)
            {
                glVertex3fv(&coords[i*3]);
            }
        glEnd();
        */
        glMaterialfv(GL_FRONT,GL_SPECULAR, speculaire_trottoir);
        glMaterialf(GL_FRONT,GL_SHININESS, 20.);
        glBegin(GL_TRIANGLE_FAN);
            glColor3fv(couleur_trottoir);
            glNormal3fv(&normales[0*3]);

            glVertex3fv(&coords[0*3]);
            ptr=4+nbrSegments;
            for(i=0;i<=nbrSegments;i++)
            {
                glVertex3fv(&coords[ptr*3]);
                ptr--;
            }
        glEnd();

        glBegin(GL_TRIANGLE_FAN);
            glVertex3fv(&coords[2*3]);

            glVertex3fv(&coords[3*3]);
            ptr=4+nbrSegments+1;
            for(i=0;i<=nbrSegments;i++)
            {
                glVertex3fv(&coords[ptr*3]);
                ptr++;
            }
            glVertex3fv(&coords[1*3]);
        glEnd();

        glMaterialfv(GL_FRONT,GL_SPECULAR, speculaire_chaussee);
        glMaterialf(GL_FRONT,GL_SHININESS, 30.);
        glBegin(GL_QUADS);
            glColor3fv(couleur_chaussee);
            for(ptr=4;ptr<nbrSegments+4;ptr++)
            {
                glVertex3fv(&coords[ptr*3]);
                glVertex3fv(&coords[(ptr+1)*3]);
                glVertex3fv(&coords[(ptr+nbrSegments+2)*3]);
                glVertex3fv(&coords[(ptr+nbrSegments+1)*3]);
            }
        glEnd();

	glEndList();
}


void Elements_Ville::genere_route01_virage90_gauche(float echelle)
{
    int nbrSegments=8;
    int nbrSommets=4+(nbrSegments+1)*2;

    int i;
    float coords_cercle[(nbrSegments+1)*2];
    float coords[3*nbrSommets];
    coords[0]=0.;coords[1]=0.;coords[2]=0.;
    coords[3]=0.;coords[4]=0.;coords[5]=-1.;
    coords[6]=1.;coords[7]=0.;coords[8]=-1.;
    coords[9]=1.;coords[10]=0.;coords[11]=0.;
    genere_coords_cercle(coords_cercle,0.,-1.,0.75,nbrSegments,-90.,90.);
    int ptr=4;
    for(i=0;i<=nbrSegments;i++)
    {
        coords[ptr*3]=coords_cercle[2*i]/3.;
        coords[ptr*3+1]=0;
        coords[ptr*3+2]=(coords_cercle[2*i+1]+1.)/3.-1.;
        ptr++;
    }
    for(i=0;i<=nbrSegments;i++)
    {
        coords[ptr*3]=coords_cercle[2*i];
        coords[ptr*3+1]=0;
        coords[ptr*3+2]=coords_cercle[2*i+1];
        ptr++;
    }

    GLfloat normales[1*3]=
                        {
                            0.,1.,0.
                        };


    //Mise à l'échelle pour éviter les glScale à tout bout de champ.
    for(i=0;i<nbrSommets;i++)
    {
        coords[3*i]=(coords[3*i]-0.5)*echelle;
        coords[3*i+2]=(coords[3*i+2]+0.5)*echelle;
    }

    glNewList(route_01_virage90_gauche_liste_id,GL_COMPILE);

        /*
        glBegin(GL_POINTS);
            glColor3fv(couleur_trottoir);
            glNormal3fv(&normales[0*3]);
            for(i=0;i<nbrSommets;i++)
            {
                glVertex3fv(&coords[i*3]);
            }
        glEnd();
        */

        glMaterialfv(GL_FRONT,GL_SPECULAR, speculaire_trottoir);
        glMaterialf(GL_FRONT,GL_SHININESS, 20.);
        glBegin(GL_TRIANGLE_FAN);
            glColor3fv(couleur_trottoir);
            glNormal3fv(&normales[0*3]);

            glVertex3fv(&coords[1*3]);
            ptr=4+nbrSegments;
            for(i=0;i<=nbrSegments;i++)
            {
                glVertex3fv(&coords[ptr*3]);
                ptr--;
            }
        glEnd();

        glBegin(GL_TRIANGLE_FAN);
            glVertex3fv(&coords[3*3]);

            glVertex3fv(&coords[0*3]);
            ptr=4+nbrSegments+1;
            for(i=0;i<=nbrSegments;i++)
            {
                glVertex3fv(&coords[ptr*3]);
                ptr++;
            }
            glVertex3fv(&coords[2*3]);
        glEnd();

        glMaterialfv(GL_FRONT,GL_SPECULAR, speculaire_chaussee);
        glMaterialf(GL_FRONT,GL_SHININESS, 30.);
        glBegin(GL_QUADS);
            glColor3fv(couleur_chaussee);
            for(ptr=4;ptr<nbrSegments+4;ptr++)
            {
                glVertex3fv(&coords[ptr*3]);
                glVertex3fv(&coords[(ptr+1)*3]);
                glVertex3fv(&coords[(ptr+nbrSegments+2)*3]);
                glVertex3fv(&coords[(ptr+nbrSegments+1)*3]);
            }
        glEnd();

	glEndList();
}

void Elements_Ville::genere_route01_carrefour_3(float echelle)
{
    int nbrSegments=8;
    int nbrSommets=6+(nbrSegments+1)*2;

    int i;
    float coords_cercle[(nbrSegments+1)*2];
    float coords[3*nbrSommets];
    coords[0]=0.;coords[1]=0.;coords[2]=0.;
    coords[3]=0.;coords[4]=0.;coords[5]=-0.25;
    coords[6]=0.;coords[7]=0.;coords[8]=-1.;
    coords[9]=1.;coords[10]=0.;coords[11]=-1.;
    coords[12]=1.;coords[13]=0.;coords[14]=-.25;
    coords[15]=1.;coords[16]=0.;coords[17]=0.;

    genere_coords_cercle(coords_cercle,0.,0.,0.25,nbrSegments,-90.,90.);
    int ptr=6;
    for(i=0;i<=nbrSegments;i++)
    {
        coords[ptr*3]=coords_cercle[2*i];
        coords[ptr*3+1]=0.;
        coords[ptr*3+2]=coords_cercle[2*i+1]-1.;
        ptr++;
    }
    for(i=0;i<=nbrSegments;i++)
    {
        coords[ptr*3]=-coords_cercle[2*i]+1.;
        coords[ptr*3+1]=0.;
        coords[ptr*3+2]=coords_cercle[2*i+1]-1.;
        ptr++;
    }

    GLfloat normales[1*3]=
                        {
                            0.,1.,0.
                        };


    //Mise à l'échelle pour éviter les glScale à tout bout de champ.
    for(i=0;i<nbrSommets;i++)
    {
        coords[3*i]=(coords[3*i]-0.5)*echelle;
        coords[3*i+2]=(coords[3*i+2]+0.5)*echelle;
    }

    glNewList(route_01_carrefour_3_liste_id,GL_COMPILE);

        /*
        glBegin(GL_POINTS);
            glColor3fv(couleur_trottoir);
            glNormal3fv(&normales[0*3]);
            for(i=0;i<nbrSommets;i++)
            {
                glVertex3fv(&coords[i*3]);
            }
        glEnd();
        */


        glMaterialfv(GL_FRONT,GL_SPECULAR, speculaire_trottoir);
        glMaterialf(GL_FRONT,GL_SHININESS, 20.);
        glBegin(GL_POLYGON);
            glColor3fv(couleur_trottoir);
            glNormal3fv(&normales[0*3]);

            glVertex3fv(&coords[2*3]);
            ptr=6+nbrSegments;
            for(i=0;i<=nbrSegments;i++)
            {
                glVertex3fv(&coords[ptr*3]);
                ptr--;
            }
        glEnd();

        glBegin(GL_POLYGON);
            glVertex3fv(&coords[3*3]);

            ptr=6+nbrSegments+1;
            for(i=0;i<=nbrSegments;i++)
            {
                glVertex3fv(&coords[ptr*3]);
                ptr++;
            }
        glEnd();


        glBegin(GL_QUADS);
            glVertex3fv(&coords[0*3]);
            glVertex3fv(&coords[1*3]);
            glVertex3fv(&coords[4*3]);
            glVertex3fv(&coords[5*3]);

            glMaterialfv(GL_FRONT,GL_SPECULAR, speculaire_chaussee);
            glMaterialf(GL_FRONT,GL_SHININESS, 30.);
            glColor3fv(couleur_chaussee);

            for(ptr=6;ptr<nbrSegments+6;ptr++)
            {
                glVertex3fv(&coords[ptr*3]);
                glVertex3fv(&coords[(ptr+1)*3]);
                glVertex3fv(&coords[(ptr+nbrSegments+2)*3]);
                glVertex3fv(&coords[(ptr+nbrSegments+1)*3]);
            }

            glVertex3fv(&coords[6*3]);
            glVertex3fv(&coords[(6+nbrSegments+1)*3]);
            glVertex3fv(&coords[4*3]);
            glVertex3fv(&coords[1*3]);

        glEnd();


	glEndList();
}

void Elements_Ville::genere_route01_carrefour_4(float echelle)
{
    int nbrSegments=8;
    int nbrSommets=4+(nbrSegments+1)*4;

    int i;
    float coords_cercle[(nbrSegments+1)*2];
    float coords[3*nbrSommets];
    coords[0]=0.;coords[1]=0.;coords[2]=0.;
    coords[3]=0.;coords[4]=0.;coords[5]=-1.;
    coords[6]=1.;coords[7]=0.;coords[8]=-1.;
    coords[9]=1.;coords[10]=0.;coords[11]=0.;

    genere_coords_cercle(coords_cercle,0.,0.,0.25,nbrSegments,0.,90.);
    int ptr=4;
    for(i=0;i<=nbrSegments;i++)
    {
        coords[ptr*3]=coords_cercle[2*i];
        coords[ptr*3+1]=0.;
        coords[ptr*3+2]=coords_cercle[2*i+1];

        coords[(ptr+nbrSegments+1)*3]=coords_cercle[2*i];
        coords[(ptr+nbrSegments+1)*3+1]=0.;
        coords[(ptr+nbrSegments+1)*3+2]=-coords_cercle[2*i+1]-1.;

        coords[(ptr+nbrSegments*2+2)*3]=-coords_cercle[2*i]+1.;
        coords[(ptr+nbrSegments*2+2)*3+1]=0.;
        coords[(ptr+nbrSegments*2+2)*3+2]=-coords_cercle[2*i+1]-1.;

        coords[(ptr+nbrSegments*3+3)*3]=-coords_cercle[2*i]+1.;
        coords[(ptr+nbrSegments*3+3)*3+1]=0.;
        coords[(ptr+nbrSegments*3+3)*3+2]=coords_cercle[2*i+1];

        ptr++;
    }

    GLfloat normales[1*3]=
                        {
                            0.,1.,0.
                        };


    //Mise à l'échelle pour éviter les glScale à tout bout de chant.
    for(i=0;i<nbrSommets;i++)
    {
        coords[3*i]=(coords[3*i]-0.5)*echelle;
        coords[3*i+2]=(coords[3*i+2]+0.5)*echelle;
    }

    glNewList(route_01_carrefour_4_liste_id,GL_COMPILE);

        /*
        glBegin(GL_POINTS);
            glColor3fv(couleur_trottoir);
            glNormal3fv(&normales[0*3]);
            for(i=0;i<nbrSommets;i++)
            {
                glVertex3fv(&coords[i*3]);
            }
        glEnd();
        */


        glMaterialfv(GL_FRONT,GL_SPECULAR, speculaire_trottoir);
        glMaterialf(GL_FRONT,GL_SHININESS, 20.);
        glBegin(GL_POLYGON);
            glColor3fv(couleur_trottoir);
            glNormal3fv(&normales[0*3]);

            glVertex3fv(&coords[0*3]);
            ptr=4+nbrSegments;
            for(i=0;i<=nbrSegments;i++)
            {
                glVertex3fv(&coords[ptr*3]);
                ptr--;
            }
        glEnd();

        glBegin(GL_POLYGON);
            glVertex3fv(&coords[1*3]);

            ptr=4+nbrSegments+1;
            for(i=0;i<=nbrSegments;i++)
            {
                glVertex3fv(&coords[ptr*3]);
                ptr++;
            }
        glEnd();

        glBegin(GL_POLYGON);
            glVertex3fv(&coords[2*3]);

            ptr=4+(nbrSegments+1)*3-1;
            for(i=0;i<=nbrSegments;i++)
            {
                glVertex3fv(&coords[ptr*3]);
                ptr--;
            }
        glEnd();

        glBegin(GL_POLYGON);
            glVertex3fv(&coords[3*3]);

            ptr=4+(nbrSegments+1)*3;
            for(i=0;i<=nbrSegments;i++)
            {
                glVertex3fv(&coords[ptr*3]);
                ptr++;
            }
        glEnd();

        glMaterialfv(GL_FRONT,GL_SPECULAR, speculaire_chaussee);
        glMaterialf(GL_FRONT,GL_SHININESS, 30.);
        glBegin(GL_QUADS);
            glColor3fv(couleur_chaussee);

            for(ptr=4;ptr<nbrSegments+4;ptr++)
            {
                glVertex3fv(&coords[ptr*3]);
                glVertex3fv(&coords[(ptr+1)*3]);
                glVertex3fv(&coords[(ptr+nbrSegments+2)*3]);
                glVertex3fv(&coords[(ptr+nbrSegments+1)*3]);
            }

            for(ptr=4+nbrSegments*2+2;ptr<4+nbrSegments*3+2;ptr++)
            {
                glVertex3fv(&coords[ptr*3]);
                glVertex3fv(&coords[(ptr+1)*3]);
                glVertex3fv(&coords[(ptr+nbrSegments+2)*3]);
                glVertex3fv(&coords[(ptr+nbrSegments+1)*3]);
            }

            glVertex3fv(&coords[4*3]);
            glVertex3fv(&coords[(4+nbrSegments+1)*3]);
            glVertex3fv(&coords[(4+nbrSegments*2+2)*3]);
            glVertex3fv(&coords[(4+nbrSegments*3+3)*3]);

        glEnd();


	glEndList();
}

void Elements_Ville::genere_route01_parking(float echelle)
{
    int nbrSommets=16;

    int i;

    float coords[3*16]={ 0.,0.,0.,
                                0.,0.,-0.25,
                                0.,0.,-0.75,
                                0.,0.,-1.,
                                1.,0.,-1.,
                                1.,0.,-0.,
                                0.20,0.,-0.15,
                                0.15,0.,-0.20,
                                0.10,0.,-0.25,
                                0.10,0.,-0.75,
                                0.15,0.,-0.80,
                                0.20,0.,-0.85,
                                0.80,0.,-0.85,
                                0.85,0.,-0.80,
                                0.85,0.,-0.20,
                                0.80,0.,-0.15};


    GLfloat normales[1*3]=
                        {
                            0.,1.,0.
                        };


    //Mise à l'échelle pour éviter les glScale à tout bout de chant.
    for(i=0;i<nbrSommets;i++)
    {
        coords[3*i]=(coords[3*i]-0.5)*echelle;
        coords[3*i+2]=(coords[3*i+2]+0.5)*echelle;
    }

    glNewList(route_01_parking_liste_id,GL_COMPILE);

        /*
        glBegin(GL_POINTS);
            glColor3fv(couleur_trottoir);
            glNormal3fv(&normales[0*3]);
            for(i=0;i<nbrSommets;i++)
            {
                glVertex3fv(&coords[i*3]);
            }
        glEnd();
        */
        glMaterialfv(GL_FRONT,GL_SPECULAR, speculaire_trottoir);
        glMaterialf(GL_FRONT,GL_SHININESS, 20.);
        glBegin(GL_POLYGON);
            glColor3fv(couleur_trottoir);
            glNormal3fv(&normales[0*3]);

            glVertex3fv(&coords[0*3]);
            glVertex3fv(&coords[1*3]);
            glVertex3fv(&coords[8*3]);
            glVertex3fv(&coords[7*3]);
            glVertex3fv(&coords[6*3]);
        glEnd();

        glBegin(GL_POLYGON);
            glVertex3fv(&coords[2*3]);
            glVertex3fv(&coords[3*3]);
            glVertex3fv(&coords[11*3]);
            glVertex3fv(&coords[10*3]);
            glVertex3fv(&coords[9*3]);
        glEnd();

        glBegin(GL_QUADS);
            glVertex3fv(&coords[3*3]);
            glVertex3fv(&coords[4*3]);
            glVertex3fv(&coords[12*3]);
            glVertex3fv(&coords[11*3]);

            glVertex3fv(&coords[4*3]);
            glVertex3fv(&coords[5*3]);
            glVertex3fv(&coords[14*3]);
            glVertex3fv(&coords[13*3]);

            glVertex3fv(&coords[0*3]);
            glVertex3fv(&coords[6*3]);
            glVertex3fv(&coords[15*3]);
            glVertex3fv(&coords[5*3]);
        glEnd();

        glBegin(GL_TRIANGLES);
            glVertex3fv(&coords[4*3]);
            glVertex3fv(&coords[13*3]);
            glVertex3fv(&coords[12*3]);

            glVertex3fv(&coords[5*3]);
            glVertex3fv(&coords[15*3]);
            glVertex3fv(&coords[14*3]);
        glEnd();

        glMaterialfv(GL_FRONT,GL_SPECULAR, speculaire_chaussee);
        glMaterialf(GL_FRONT,GL_SHININESS, 30.);
        glBegin(GL_QUADS);
            glColor3fv(couleur_chaussee);

            glVertex3fv(&coords[1*3]);
            glVertex3fv(&coords[2*3]);
            glVertex3fv(&coords[9*3]);
            glVertex3fv(&coords[8*3]);
        glEnd();

        glBegin(GL_POLYGON);
            glVertex3fv(&coords[6*3]);
            glVertex3fv(&coords[7*3]);
            glVertex3fv(&coords[8*3]);
            glVertex3fv(&coords[9*3]);
            glVertex3fv(&coords[10*3]);
            glVertex3fv(&coords[11*3]);
        glEnd();

        glBegin(GL_POLYGON);
            glVertex3fv(&coords[6*3]);
            glVertex3fv(&coords[11*3]);
            glVertex3fv(&coords[12*3]);
            glVertex3fv(&coords[13*3]);
            glVertex3fv(&coords[14*3]);
            glVertex3fv(&coords[15*3]);
        glEnd();

	glEndList();
}

void Elements_Ville::genere_route01_espace_vert(float echelle)
{
    int nbrSommets=18;
    int num_arbres=3;

    int i;

    float coords[3*18]={ 0.,0.,0.,
                            0.,0.,-1.,
                            1.,0.,-1.,
                            1.,0.,0.,

                            0.25,0.,-0.10,
                            0.25,15.,-0.10,

                            0.30,0.,-0.40,
                            0.30,12.,-0.40,

                            0.25,0.,-0.75,
                            0.25,8.,-0.75,

                            0.55,0.,-0.65,
                            0.55,13.,-0.65,

                            0.65,0.,-0.80,
                            0.65,5.,-0.80,

                            0.75,0.,-.45,
                            0.75,8.5,-0.45,

                            0.65,0.,-0.20,
                            0.65,11.8,-0.20};

    float* coords_sol=&coords[0];
    float* coords_troncs=&coords[3*4];

    GLfloat normales[1*3]=
                        {
                            0.,1.,0.,
                        };


    //Mise à l'échelle pour éviter les glScale à tout bout de chant.
    for(i=0;i<nbrSommets;i++)
    {
        coords[3*i]=(coords[3*i]-0.5)*echelle;
        coords[3*i+2]=(coords[3*i+2]+0.5)*echelle;
    }

    glNewList(route_01_espace_vert_liste_id,GL_COMPILE);
    glShadeModel(GL_FLAT);
        /*
        glBegin(GL_POINTS);
            glColor3fv(couleur_trottoir);
            glNormal3fv(&normales[0*3]);
            for(i=0;i<nbrSommets;i++)
            {
                glVertex3fv(&coords[i*3]);
            }
        glEnd();
        */
        glMaterialfv(GL_FRONT,GL_SPECULAR, speculaire_trottoir);
         glMaterialf(GL_FRONT,GL_SHININESS, 20.);
        glBegin(GL_QUADS);
            glColor3fv(couleur_trottoir);
            glNormal3fv(&normales[0*3]);

            glVertex3fv(&coords_sol[0*3]);
            glVertex3fv(&coords_sol[1*3]);
            glVertex3fv(&coords_sol[2*3]);
            glVertex3fv(&coords_sol[3*3]);
        glEnd();

        glLineWidth(4.*float(GVars::largeur_ecran)/1280.);
        glBegin(GL_LINES);
            glColor3fv(couleur_troncs);

            for(i=0;i<num_arbres*2;i++)
            {
                glVertex3fv(&coords_troncs[i*3]);
            }
        glEnd();
        //glUseProgram(ShadersVille::face_observateur->programme_id);
        //glDisable(GL_CULL_FACE);
         glShadeModel(GL_SMOOTH);
         glMaterialfv(GL_FRONT,GL_SPECULAR, speculaire_feuillages);
         glMaterialf(GL_FRONT,GL_SHININESS, 50.);
         glMaterialfv(GL_FRONT,GL_EMISSION, couleur_lumiere_feuillages);
        glColor3fv(couleur_feuillages);
        for(i=0;i<num_arbres;i++)
        {
            float scale=coords_troncs[(2*i+1)*3+1];
            glPushMatrix();
            glTranslatef(coords_troncs[(2*i+1)*3],coords_troncs[(2*i+1)*3+1]-scale*0.9/2.1,coords_troncs[(2*i+1)*3+2]);
            glScalef(scale*.5,scale*0.9,scale*.5);
            glRotatef(i*20.,0.,1.,0.);
            glCallList(sphere_liste_id);
            //glRotatef(90.,0.,1.,0.);
            //glCallList(tetraedre_liste_id);
            glPopMatrix();
        }

        float noir[4]={0.,0.,0.,0.};
        glMaterialfv(GL_FRONT,GL_EMISSION, noir);
        glUseProgram(0);
        glShadeModel(GL_FLAT);
        //glEnable(GL_CULL_FACE);


        //GLboolean light_mem;
        //light_mem=glIsEnabled(GL_LIGHTING);
        glDepthMask(GL_FALSE);
        //glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glDisable(GL_LIGHTING);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,GVars::ombre_sol->texture_id);
        glColor4f(1.,1.,1.,0.25);
        for(i=0;i<num_arbres;i++)
        {
            float scale=coords_troncs[(2*i+1)*3+1]*0.66;
            glPushMatrix();
                glTranslatef(coords_troncs[(2*i+1)*3],0.05,coords_troncs[(2*i+1)*3+2]);
                glScalef(scale,1.,scale);
                glCallList(GVars::liste_ombre_sol_id);
            glPopMatrix();
        }
        glEnable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);
        //glEnable(GL_DEPTH_TEST);


	glEndList();
}

void Elements_Ville::genere_bordure_eau_base(float echelle)
{
    int nbrSommets=4;
    int i;
    float coords[3*4]= { 0.,0.,0.,
                        0.,1.,0.,
                        1.,1.,0.,
                        1.,0.,0.
                        };
    float normales[3]={0.,0.,1.};

    //Mise à l'échelle pour éviter les glScale à tout bout de chant.
    for(i=0;i<nbrSommets;i++)
    {
        coords[3*i]=(coords[3*i]-0.5)*echelle;
        coords[3*i+1]=(coords[3*i+1]-1.)*echelle+VILLE_ALTITUDE_ROUTES;
        coords[3*i+2]=(coords[3*i+2]+0.5)*echelle;
    }

    glNewList(bordure_eau_base_liste_id,GL_COMPILE);

        glBegin(GL_QUADS);
            glColor3fv(couleur_epaisseur_bordures_eau);
            glNormal3fv(&normales[0*3]);

            glVertex3fv(&coords[0*3]);
            glVertex3fv(&coords[1*3]);
            glVertex3fv(&coords[2*3]);
            glVertex3fv(&coords[3*3]);
        glEnd();

	glEndList();
}

void Elements_Ville::genere_immeuble_base()
{
    int i;

    glNewList(immeuble_base_liste_id,GL_COMPILE);
            glBegin(GL_QUADS);
                for(i=0;i<5;i++)
                {
                    glNormal3fv(&immeuble_base_normales[i*3]);
                    glVertex3fv(&immeuble_base_sommets[immeuble_base_faces[i*4]*3]);
                    glVertex3fv(&immeuble_base_sommets[immeuble_base_faces[i*4+1]*3]);
                    glVertex3fv(&immeuble_base_sommets[immeuble_base_faces[i*4+2]*3]);
                    glVertex3fv(&immeuble_base_sommets[immeuble_base_faces[i*4+3]*3]);
                }
            glEnd();
    glEndList();
}

void Elements_Ville::genere_immeuble_01()
{
    int i;

    float immeuble_01_sommets[8*3]={-0.3,1.,0.3,
                                    0.3,1.,0.3,
                                    0.3,1.,-0.3,
                                    -0.3,1.,-0.3,
                                    -0.3,0.,-0.3,
                                    0.3,0.,-0.3,
                                    0.3,0.,0.3,
                                    -0.3,0.,0.3};

    float immeuble_01_sommets_2[8*3];

    for(i=0;i<8;i++)
    {
        immeuble_01_sommets_2[i*3]=immeuble_01_sommets[i*3]*0.95;
        immeuble_01_sommets_2[i*3+1]=immeuble_01_sommets[i*3+1]*0.999;
        immeuble_01_sommets_2[i*3+2]=immeuble_01_sommets[i*3+2]*0.95;
    }

    glNewList(immeuble_01_liste_id,GL_COMPILE);

        //glShadeModel( GL_SMOOTH );
        /*
        glActiveTexture(GL_TEXTURE2);
        glDisable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE1);
        glDisable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        glDisable(GL_TEXTURE_2D);
        */


        //init_materiau_simple_face(0xffffffff,0x000000ff,0xd08080ff,0x000000ff,40.);

            //glUniform1i(ShadersVille::fenetre01->nombre_etages,1);
            glMaterialfv(GL_FRONT,GL_SPECULAR, speculaire_immeubles);
            glMaterialf(GL_FRONT,GL_SHININESS, 15.);
            glDisable(GL_CULL_FACE);
            glBegin(GL_QUADS);


                for(i=0;i<5;i++)
                {
                    glNormal3fv(&immeuble_base_normales[i*3]);
                    glVertex3fv(&immeuble_01_sommets[immeuble_base_faces[i*4]*3]);
                    glVertex3fv(&immeuble_01_sommets[immeuble_base_faces[i*4+1]*3]);
                    glVertex3fv(&immeuble_01_sommets[immeuble_base_faces[i*4+2]*3]);
                    glVertex3fv(&immeuble_01_sommets[immeuble_base_faces[i*4+3]*3]);
                }

            glEnd();
            glEnable(GL_CULL_FACE);
            glUseProgram(ShadersVille::vitres->programme_id);

            glMaterialfv(GL_FRONT,GL_SPECULAR, speculaire_vitres);
            glMaterialf(GL_FRONT,GL_SHININESS, 20.);
            glColor4fv(couleur_fenetres);
            glBegin(GL_QUADS);


                for(i=1;i<5;i++)
                {
                    glNormal3fv(&immeuble_base_normales[i*3]);
                    glVertex3fv(&immeuble_01_sommets_2[immeuble_base_faces[i*4]*3]);
                    glVertex3fv(&immeuble_01_sommets_2[immeuble_base_faces[i*4+1]*3]);
                    glVertex3fv(&immeuble_01_sommets_2[immeuble_base_faces[i*4+2]*3]);
                    glVertex3fv(&immeuble_01_sommets_2[immeuble_base_faces[i*4+3]*3]);
                }

            glEnd();

            glUseProgram(0);

            /*
            glColor3fv(couleur_fenetres);
            glBegin(GL_QUADS);

                for(i=1;i<5;i++)
                {
                    glNormal3fv(&immeuble_base_normales[i*3]);
                    glVertex3fv(&immeuble_01_sommets_2[immeuble_base_faces[i*4]*3]);
                    glVertex3fv(&immeuble_01_sommets_2[immeuble_base_faces[i*4+1]*3]);
                    glVertex3fv(&immeuble_01_sommets_2[immeuble_base_faces[i*4+2]*3]);
                    glVertex3fv(&immeuble_01_sommets_2[immeuble_base_faces[i*4+3]*3]);
                }

            glEnd();
            */

            glColor3fv(couleur_trottoir);
            glMaterialfv(GL_FRONT,GL_SPECULAR, speculaire_trottoir);
            glMaterialf(GL_FRONT,GL_SHININESS, 20.);
            glBegin(GL_QUADS);
                glNormal3f(0.,1.,0.);
                glVertex3f(-0.5,0.,0.5);
                glVertex3f(-0.5,0.,-0.5);
                glVertex3f(0.5,0.,-0.5);
                glVertex3f(0.5,0.,0.5);
            glEnd();

            //glDisable(GL_LIGHTING);
            glPointSize(1.5*float(GVars::largeur_ecran)/1280.);

            //glEnable(GL_POINT_SMOOTH);
            //glEnable(GL_BLEND);
            //glDepthMask(GL_FALSE);
            //glDisable(GL_FOG);
            //glFogi(GL_FOG_MODE,GL_EXP);
            //glFogf(GL_FOG_DENSITY,1.);
            glUseProgram(ShadersVille::lumiere01->programme_id);
            glBegin(GL_POINTS);
                /*
                glColor3f(0.,1.,1.);
                glVertex3f(-0.29,1.01,-0.29);
                glVertex3f(0.29,1.01,-0.29);
                glVertex3f(0.29,1.01,0.29);
                glVertex3f(-0.29,1.01,0.29);
                */
                glColor3f(0,1.,0.);
                glVertex3f(-0.,1.01,-0.);
            glEnd();
            glUseProgram(0);
            //glDisable(GL_BLEND);
            //glDisable(GL_POINT_SMOOTH);
            //glEnable(GL_LIGHTING);
            //glDepthMask(GL_TRUE);
            //glFogi(GL_FOG_MODE,GL_LINEAR);
            //glFogf(GL_FOG_DENSITY,1.);
            //glEnable(GL_FOG);

        //glUseProgram(0);
    glEndList();
}

void Elements_Ville::genere_immeuble_02()
{
    int i;

    float immeuble_02_sommets[8*3]={-0.33,1.,0.33,
                                    0.33,1.,0.33,
                                    0.33,1.,-0.33,
                                    -0.33,1.,-0.33,
                                    -0.5,0.,-0.5,
                                    0.5,0.,-0.5,
                                    0.5,0.,0.5,
                                    -0.5,0.,0.5};

    float immeuble_02_sommets_2[8*3]={-0.30,1.,0.30,
                                    0.30,1.,0.30,
                                    0.30,1.,-0.30,
                                    -0.30,1.,-0.30,
                                    -0.47,0.,-0.47,
                                    0.47,0.,-0.47,
                                    0.47,0.,0.47,
                                    -0.47,0.,0.47};

    glNewList(immeuble_02_liste_id,GL_COMPILE);


        //init_materiau_simple_face(0xffffffff,0x000000ff,0xd08080ff,0x000000ff,40.);

            //glUniform1i(ShadersVille::fenetre01->nombre_etages,1);

            glDisable(GL_CULL_FACE);

            glMaterialfv(GL_FRONT,GL_SPECULAR, speculaire_immeubles);
            glMaterialf(GL_FRONT,GL_SHININESS, 15.);
            glBegin(GL_QUADS);


                for(i=0;i<5;i++)
                {
                    glNormal3fv(&immeuble_base_normales[i*3]);
                    glVertex3fv(&immeuble_02_sommets[immeuble_base_faces[i*4]*3]);
                    glVertex3fv(&immeuble_02_sommets[immeuble_base_faces[i*4+1]*3]);
                    glVertex3fv(&immeuble_02_sommets[immeuble_base_faces[i*4+2]*3]);
                    glVertex3fv(&immeuble_02_sommets[immeuble_base_faces[i*4+3]*3]);
                }

            glEnd();
             glEnable(GL_CULL_FACE);

            glUseProgram(ShadersVille::vitres->programme_id);
            glMaterialfv(GL_FRONT,GL_SPECULAR, speculaire_vitres);
            glMaterialf(GL_FRONT,GL_SHININESS, 20.);

            glColor3fv(couleur_fenetres);
            glBegin(GL_QUADS);

                for(i=1;i<5;i++)
                {
                    glNormal3fv(&immeuble_base_normales[i*3]);
                    glVertex3fv(&immeuble_02_sommets_2[immeuble_base_faces[i*4]*3]);
                    glVertex3fv(&immeuble_02_sommets_2[immeuble_base_faces[i*4+1]*3]);
                    glVertex3fv(&immeuble_02_sommets_2[immeuble_base_faces[i*4+2]*3]);
                    glVertex3fv(&immeuble_02_sommets_2[immeuble_base_faces[i*4+3]*3]);
                }

            glEnd();
            glUseProgram(0);

            glLineWidth(1.*float(GVars::largeur_ecran)/1280.);
                    glColor3f(.0,.2,.2);
            glBegin(GL_LINES);
                    glVertex3f(-0.1,1.,-0.1);
                    glVertex3f(-0.1,1.05,-0.1);
                    glVertex3f(0.1,1.,0.1);
                    glVertex3f(0.1,1.1,0.1);
            glEnd();

            //glDisable(GL_LIGHTING);
            glUseProgram(ShadersVille::lumiere01->programme_id);
            glPointSize(1.*float(GVars::largeur_ecran)/1280.);
            glColor3f(1.,1.,0.);
            //glEnable(GL_POINT_SMOOTH);
            //glEnable(GL_BLEND);
            //glDisable(GL_FOG);
            //glFogi(GL_FOG_MODE,GL_EXP2);
            //glFogf(GL_FOG_DENSITY,.1);
            //glDepthMask(GL_FALSE);
            glBegin(GL_POINTS);
                glVertex3f(-0.1,1.05,-0.1);
                glVertex3f(0.1,1.1,0.1);
            glEnd();
            glUseProgram(0);
            //glDisable(GL_BLEND);
            //glEnable(GL_LIGHTING);
            //glDepthMask(GL_TRUE);
            //glDisable(GL_POINT_SMOOTH);
            //glEnable(GL_FOG);
            //glFogi(GL_FOG_MODE,GL_LINEAR);
            //glFogf(GL_FOG_DENSITY,1.);

        //glUseProgram(0);
    glEndList();
}

void Elements_Ville::genere_immeuble_03()
{
    int i;

    float immeuble_03_sommets[8*3]={-0.5,1.,0.5,
                                    0.5,1.,0.5,
                                    0.5,1.,0.25,
                                    -0.5,1.,0.25,
                                    -0.5,0.,0.,
                                    0.5,0.,0.,
                                    0.5,0.,0.5,
                                    -0.5,0.,0.5};

    float immeuble_03_sommets_2[8*3]={-0.48,1.,0.48,
                                    0.48,1.,0.48,
                                    0.48,1.,0.27,
                                    -0.48,1.,0.27,
                                    -0.48,0.,0.02,
                                    0.48,0.,0.02,
                                    0.48,0.,0.48,
                                    -0.48,0.,0.48};

    glNewList(immeuble_03_liste_id,GL_COMPILE);


        //init_materiau_simple_face(0xffffffff,0x000000ff,0xd08080ff,0x000000ff,40.);

            //glUniform1i(ShadersVille::fenetre01->nombre_etages,1);

            glDisable(GL_CULL_FACE);
            glMaterialfv(GL_FRONT,GL_SPECULAR, speculaire_immeubles);
            glMaterialf(GL_FRONT,GL_SHININESS, 15.);

            glBegin(GL_QUADS);


                for(i=0;i<5;i++)
                {
                    glNormal3fv(&immeuble_base_normales[i*3]);
                    glVertex3fv(&immeuble_03_sommets[immeuble_base_faces[i*4]*3]);
                    glVertex3fv(&immeuble_03_sommets[immeuble_base_faces[i*4+1]*3]);
                    glVertex3fv(&immeuble_03_sommets[immeuble_base_faces[i*4+2]*3]);
                    glVertex3fv(&immeuble_03_sommets[immeuble_base_faces[i*4+3]*3]);
                }

            glEnd();
             glEnable(GL_CULL_FACE);

            glUseProgram(ShadersVille::vitres->programme_id);
            glMaterialfv(GL_FRONT,GL_SPECULAR, speculaire_vitres);
            glMaterialf(GL_FRONT,GL_SHININESS, 20.);

            glColor3fv(couleur_fenetres);
            glBegin(GL_QUADS);

                for(i=1;i<5;i++)
                {
                    glNormal3fv(&immeuble_base_normales[i*3]);
                    glVertex3fv(&immeuble_03_sommets_2[immeuble_base_faces[i*4]*3]);
                    glVertex3fv(&immeuble_03_sommets_2[immeuble_base_faces[i*4+1]*3]);
                    glVertex3fv(&immeuble_03_sommets_2[immeuble_base_faces[i*4+2]*3]);
                    glVertex3fv(&immeuble_03_sommets_2[immeuble_base_faces[i*4+3]*3]);
                }

            glEnd();
            glUseProgram(0);
            /*
            glLineWidth(1.);
                    glColor3f(.0,.2,.2);
            glBegin(GL_LINES);
                    glVertex3f(-0.1,1.,0.1);
                    glVertex3f(-0.1,1.05,-0.1);
                    glVertex3f(0.1,1.,0.1);
                    glVertex3f(0.1,1.1,0.1);
            glEnd();

            glDisable(GL_LIGHTING);
            glPointSize(4.);
            glColor3f(1.,1.,0.);
            glEnable(GL_POINT_SMOOTH);
            glEnable(GL_BLEND);
            glBegin(GL_POINTS);
                glVertex3f(-0.1,1.05,-0.1);
                glVertex3f(0.1,1.1,0.1);
            glEnd();
            glDisable(GL_BLEND);
            glEnable(GL_LIGHTING);

            glColor3fv(couleur_trottoir);
            glBegin(GL_QUADS);
                glNormal3f(0.,1.,0.);
                glVertex3f(-0.5,0.,0.5);
                glVertex3f(-0.5,0.,-0.5);
                glVertex3f(0.5,0.,-0.5);
                glVertex3f(0.5,0.,0.5);
            glEnd();
            */

        glColor3fv(couleur_trottoir);
        glMaterialfv(GL_FRONT,GL_SPECULAR, speculaire_trottoir);
        glMaterialf(GL_FRONT,GL_SHININESS, 20.);
            glBegin(GL_QUADS);
                glNormal3f(0.,1.,0.);
                glVertex3f(-0.5,0.,0.5);
                glVertex3f(-0.5,0.,-0.5);
                glVertex3f(0.5,0.,-0.5);
                glVertex3f(0.5,0.,0.5);
            glEnd();



            glLineWidth(1.*float(GVars::largeur_ecran)/1280.);
                    glColor3f(.2,.2,.2);
            glBegin(GL_LINES);
                    glVertex3f(-0.25,1.,0.3);
                    glVertex3f(-0.25,1.05,0.3);
                    glVertex3f(0.25,1.,0.4);
                    glVertex3f(0.25,1.1,0.4);
            glEnd();
            glLineWidth(2.*float(GVars::largeur_ecran)/1280.);
            glBegin(GL_LINES);
                    glVertex3f(0.,1.,0.25);
                    glVertex3f(0.,1.075,0.25);
            glEnd();

            //glDisable(GL_LIGHTING);
            glUseProgram(ShadersVille::lumiere01->programme_id);
            glPointSize(2.*float(GVars::largeur_ecran)/1280.);
            glColor3f(1.,1.,1.);
            //glEnable(GL_POINT_SMOOTH);
            //glEnable(GL_BLEND);
            //glDepthMask(GL_FALSE);
            //glDisable(GL_FOG);
            //glFogi(GL_FOG_MODE,GL_EXP2);
            //glFogf(GL_FOG_DENSITY,.1);
            glBegin(GL_POINTS);
            glVertex3f(0.,1.07525,0.25);
            glEnd();
            glUseProgram(0);
            //glDepthMask(GL_TRUE);
            //glDisable(GL_BLEND);
            //glEnable(GL_LIGHTING);
            //glDisable(GL_POINT_SMOOTH);
            //glEnable(GL_FOG);
            //glFogi(GL_FOG_MODE,GL_LINEAR);
            //glFogf(GL_FOG_DENSITY,1.);

    glEndList();
}

void Elements_Ville::genere_immeuble_04()
{
    int i;
    float rc2=cosf(M_PI/2.);
    float immeuble_04_normales[9*3]={-rc2,0.,rc2,
                                    0.,0.,1.,
                                    rc2,0.,rc2,
                                    1.,0.,0.,
                                    rc2,0.,-rc2,
                                    0.,0.,-1.,
                                    -rc2,0.,-rc2,
                                    -1.,0.,0.,
                                    0.,1.,0. };

    float immeuble_04_tangentes[9*3]={rc2,0.,rc2,
                                    1.,0.,0.,
                                    rc2,0.,-rc2,
                                    0.,0.,-1.,
                                    -rc2,0.,-rc2,
                                    -1.,0.,0.,
                                    -rc2,0.,rc2,
                                    0.,0.,1.,
                                    1.,0.,0. };

    /*
    float immeuble_04_binormales[9*3]={-rc2,0.,rc2,
                                    0.,0.,1.,
                                    rc2,0.,rc2,
                                    1.,0.,0.,
                                    rc2,0.,-rc2,
                                    0.,0.,-1.,
                                    -rc2,0.,-rc2,
                                    -1.,0.,0.,
                                    0.,1.,0. };
                                    */

    int immeuble_04_faces[8*4+8]={0,8,9,7,
                                0,1,15,8,
                                1,2,14,15,
                                2,3,13,14,
                                3,4,12,13,
                                4,5,11,12,
                                5,6,10,11,
                                6,7,9,10,
                                7,6,5,4,3,2,1,0 };

    float immeuble_04_sommets[16*3]={
                                    -1.,1.,1.5,
                                    1.,1.,1.5,
                                    1.5,1.,1.,
                                    1.5,1.,-0.5,
                                    .5,1.,-1.5,
                                    -1.,1.,-1.5,
                                    -1.5,1.,-1.,
                                    -1.5,1.,1.,

                                    -.5,0.,1.5,
                                    -1.5,0.,.5,
                                    -1.5,0.,-.5,
                                    -.5,0.,-1.5,
                                    .5,0.,-1.5,
                                    1.5,0.,-.5,
                                    1.5,0.,.5,
                                    .5,0.,1.5};

    float immeuble_04_sommets_2[16*3];
    for(i=0;i<16;i++)
    {
        immeuble_04_sommets_2[i*3]=immeuble_04_sommets[i*3]*0.95;
        immeuble_04_sommets_2[i*3+1]=immeuble_04_sommets[i*3+1]*0.999;
        immeuble_04_sommets_2[i*3+2]=immeuble_04_sommets[i*3+2]*0.95;
    }

    glNewList(immeuble_04_liste_id,GL_COMPILE);


        //init_materiau_simple_face(0xffffffff,0x000000ff,0xd08080ff,0x000000ff,40.);

            //glUniform1i(ShadersVille::fenetre01->nombre_etages,1);

            glDisable(GL_CULL_FACE);
            glMaterialfv(GL_FRONT,GL_SPECULAR, speculaire_immeubles);
            glMaterialf(GL_FRONT,GL_SHININESS, 15.);

            glBegin(GL_QUADS);
                for(i=0;i<8;i++)
                {
                    glNormal3fv(&immeuble_04_normales[i*3]);
                    glVertexAttrib3fv(ShadersVille::fenetre04->tangente,&immeuble_04_tangentes[3*i]);
                    glVertex3fv(&immeuble_04_sommets[immeuble_04_faces[i*4]*3]);
                    glVertex3fv(&immeuble_04_sommets[immeuble_04_faces[i*4+1]*3]);
                    glVertex3fv(&immeuble_04_sommets[immeuble_04_faces[i*4+2]*3]);
                    glVertex3fv(&immeuble_04_sommets[immeuble_04_faces[i*4+3]*3]);
                }
            glEnd();


            glBegin(GL_POLYGON);
                    glNormal3fv(&immeuble_04_normales[8*3]);
                    glVertexAttrib3fv(ShadersVille::fenetre04->tangente,&immeuble_04_tangentes[3*8]);
                    glVertex3fv(&immeuble_04_sommets[immeuble_04_faces[8*4]*3]);
                    glVertex3fv(&immeuble_04_sommets[immeuble_04_faces[8*4+1]*3]);
                    glVertex3fv(&immeuble_04_sommets[immeuble_04_faces[8*4+2]*3]);
                    glVertex3fv(&immeuble_04_sommets[immeuble_04_faces[8*4+3]*3]);
                    glVertex3fv(&immeuble_04_sommets[immeuble_04_faces[8*4+4]*3]);
                    glVertex3fv(&immeuble_04_sommets[immeuble_04_faces[8*4+5]*3]);
                    glVertex3fv(&immeuble_04_sommets[immeuble_04_faces[8*4+6]*3]);
                    glVertex3fv(&immeuble_04_sommets[immeuble_04_faces[8*4+7]*3]);
            glEnd();

             glEnable(GL_CULL_FACE);

            glUseProgram(ShadersVille::vitres->programme_id);
            glMaterialfv(GL_FRONT,GL_SPECULAR, speculaire_vitres);
            glMaterialf(GL_FRONT,GL_SHININESS, 20.);
            glColor3fv(couleur_fenetres);
            glBegin(GL_QUADS);
                for(i=0;i<8;i++)
                {
                    glNormal3fv(&immeuble_04_normales[i*3]);
                    glVertexAttrib3fv(ShadersVille::fenetre04->tangente,&immeuble_04_tangentes[3*i]);
                    glVertex3fv(&immeuble_04_sommets_2[immeuble_04_faces[i*4]*3]);
                    glVertex3fv(&immeuble_04_sommets_2[immeuble_04_faces[i*4+1]*3]);
                    glVertex3fv(&immeuble_04_sommets_2[immeuble_04_faces[i*4+2]*3]);
                    glVertex3fv(&immeuble_04_sommets_2[immeuble_04_faces[i*4+3]*3]);
                }
            glEnd();

            glUseProgram(0);

            glLineWidth(1.*float(GVars::largeur_ecran)/1280.);
            glColor3f(.1,.1,.1);
            glBegin(GL_LINES);
                for(i=0;i<8;i++)
                {
                    glVertex3fv(&immeuble_04_sommets_2[i*3]);
                    glVertex3f(immeuble_04_sommets_2[i*3],immeuble_04_sommets_2[i*3+1]+0.02,immeuble_04_sommets_2[i*3+2]);
                }

            glEnd();

            /*
            glLineWidth(2.);
            glBegin(GL_LINES);
                    glVertex3f(0.,1.,0.25);
                    glVertex3f(0.,1.075,0.25);
            glEnd();
            */

            //glDisable(GL_LIGHTING);
            //glDepthMask(GL_FALSE);
            //glDisable(GL_FOG);
            //glFogi(GL_FOG_MODE,GL_EXP2);
            //glFogf(GL_FOG_DENSITY,.1);
            glUseProgram(ShadersVille::lumiere01->programme_id);
            glPointSize(2.*float(GVars::largeur_ecran)/1280.);

            //glEnable(GL_POINT_SMOOTH);
            //glEnable(GL_BLEND);
             glColor3f(1.,1.,0.);
            glBegin(GL_POINTS);
                for(i=0;i<8;i++)
                {
                    glVertex3f(immeuble_04_sommets_2[i*3],immeuble_04_sommets_2[i*3+1]+0.02,immeuble_04_sommets_2[i*3+2]);
                }
            glEnd();
            glColor3f(.5,1.,1.);
            glPointSize(1.5*float(GVars::largeur_ecran)/1280.);
            glBegin(GL_POINTS);
                for(i=0;i<8;i++)
                {
                    glVertex3f(immeuble_04_sommets[i*3]*1.01,immeuble_04_sommets[i*3+1]-0.5,immeuble_04_sommets[i*3+2]*1.01);
                }
            glEnd();
            glColor3f(.2,1.,.2);
            glPointSize(1.5*float(GVars::largeur_ecran)/1280.);
            glBegin(GL_POINTS);
                for(i=0;i<4;i++)
                {
                    glVertex3f(immeuble_04_sommets[i*2*3]*1.01,immeuble_04_sommets[i*2*3+1]-0.25,immeuble_04_sommets[i*2*3+2]*1.01);
                }
            glEnd();
            glUseProgram(0);
            //glDisable(GL_BLEND);
            //glDepthMask(GL_TRUE);
            //glEnable(GL_LIGHTING);
            //glDisable(GL_POINT_SMOOTH);
            //glEnable(GL_FOG);
            //glFogi(GL_FOG_MODE,GL_LINEAR);
            //glFogf(GL_FOG_DENSITY,1.);
            /*
            glColor3fv(couleur_trottoir);
            glBegin(GL_QUADS);
                glNormal3f(0.,1.,0.);
                glVertex3f(-0.5,0.,0.5);
                glVertex3f(-0.5,0.,-0.5);
                glVertex3f(0.5,0.,-0.5);
                glVertex3f(0.5,0.,0.5);
            glEnd();
            */


    glEndList();
}

void Elements_Ville::affiche_trame_cercle()
{
    int nbrCotes=64;
    float rayon=100.;
    int i;
    float angleSection=2.*M_PI/float(nbrCotes);
    GLfloat coords_circonference[2*nbrCotes];
    for (i=0;i<nbrCotes;i++)
    {
        float a=angleSection*float(i);
        coords_circonference[2*i]=rayon*cosf(a);
        coords_circonference[2*i+1]=rayon*sinf(a);
    }

    glDisable(GL_LIGHTING);
    glColor3f(0.,1.,1.);
    glBegin(GL_LINE_LOOP);
        for(i=0;i<nbrCotes;i++)
        {
            glVertex2fv(&coords_circonference[2*i]);
        }
    glEnd();
    glBegin(GL_LINES);
        for(i=0;i<nbrCotes/2;i++)
        {
            glVertex2fv(&coords_circonference[2*i]);
            glVertex2fv(&coords_circonference[2*(i+nbrCotes/2)]);
        }
    glEnd();
    glPointSize(4.*float(GVars::largeur_ecran)/1280.);
    glBegin(GL_POINTS);
        for(i=0;i<nbrCotes;i++)
        {
            glVertex2fv(&coords_circonference[2*i]);
        }
    glEnd();

}

bool Elements_Ville::init_elements(float partSize,float hauteur_etage)
{
    glEnable(GL_NORMALIZE);

	genere_disque(1.,VILLE_NUM_COTES_DISQUE,coordsDisque);
	genere_cylindre(.5,1.,VILLE_NUM_COTES_CYL,VILLE_NUM_SECTIONS_CYL,coordsCylindre);
	genere_faces_cylindre(VILLE_NUM_COTES_CYL,VILLE_NUM_SECTIONS_CYL,facesCylindre);
	genere_sphere(.5,VILLE_NUM_COTES_SPHERE,VILLE_NUM_SEGMENTS_SPHERE,coordsSphere);
	cout<<"DEBUG VILLE: "<<sizeof(coordsSphere)/12<<" - "<<VILLE_NUM_POINTS_SPHERE<<endl;
	genere_faces_sphere(VILLE_NUM_COTES_SPHERE,VILLE_NUM_SEGMENTS_SPHERE,facesSphere);
	genere_tore(.5,0.1,VILLE_NUM_COTES_TORE,VILLE_NUM_SECTIONS_TORE,coordsTore);
	genere_faces_tore(VILLE_NUM_COTES_TORE,VILLE_NUM_SECTIONS_TORE,facesTore);
	genere_normales_cylindre(VILLE_NUM_POINTS_CYL,coordsCylindre,normalesCylindre);
	genere_normales_tore(1.,VILLE_NUM_POINTS_TORE,coordsTore,normalesTore);
	genere_normales_sphere(VILLE_NUM_POINTS_SPHERE,coordsSphere,normalesSphere);

    triangle_liste_id=glGenLists(1);
	disque_liste_id=glGenLists(1);
	sphere_liste_id=glGenLists(1);
	cylindre_liste_id=glGenLists(1);
	tore_liste_id=glGenLists(1);
	cube_liste_id=glGenLists(1);
	cube02_liste_id=glGenLists(1);
	tetraedre_liste_id=glGenLists(1);
	rectangle_contours_liste_id=glGenLists(1);
	//plan01_liste_id=glGenLists(1);
	plan02_liste_id=glGenLists(1);

	immeuble_base_liste_id=glGenLists(1);
	immeuble_01_liste_id=glGenLists(1);
	immeuble_02_liste_id=glGenLists(1);
	immeuble_03_liste_id=glGenLists(1);
	immeuble_04_liste_id=glGenLists(1);

	route_01_droite_liste_id=glGenLists(1);
	route_01_virage90_droite_liste_id=glGenLists(1);
	route_01_virage90_gauche_liste_id=glGenLists(1);
	route_01_carrefour_3_liste_id=glGenLists(1);
	route_01_carrefour_4_liste_id=glGenLists(1);
	route_01_parking_liste_id=glGenLists(1);
	route_01_espace_vert_liste_id=glGenLists(1);

	bordure_eau_base_liste_id=glGenLists(1);

    glNewList(triangle_liste_id,GL_COMPILE);
	affiche_triangle();
	glEndList();
    //glNewList(plan01_liste_id,GL_COMPILE);
	//affiche_plan01();
	glEndList();
	desactive_textures();
	glUseProgram(0);

	glNewList(plan02_liste_id,GL_COMPILE);
	affiche_plan02();
	glEndList();

	glNewList(rectangle_contours_liste_id,GL_COMPILE);
	affiche_rectangle_contours();
	glEndList();

    glNewList(cube_liste_id,GL_COMPILE);
	affiche_cube();
	glEndList();

	glNewList(cube02_liste_id,GL_COMPILE);
	affiche_cube02();
	glEndList();

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

    glNewList(tetraedre_liste_id,GL_COMPILE);
	affiche_tetraedre();
	glEndList();

    genere_immeuble_01();
    genere_immeuble_02();
    genere_immeuble_03();
    genere_immeuble_04();
    genere_immeuble_base();

    genere_route01_droite(partSize);
    genere_route01_virage90_droite(partSize);
    genere_route01_virage90_gauche(partSize);
    genere_route01_carrefour_3(partSize);
    genere_route01_carrefour_4(partSize);
    genere_route01_parking(partSize);
    genere_route01_espace_vert(partSize);
    genere_bordure_eau_base(partSize);


    return true;
}
