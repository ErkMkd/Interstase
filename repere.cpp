#include "repere.h"


	Repere::Repere(float p_x, float p_y, float p_z, float p_r, float p_t, float p_l)
	{
		x=p_x;y=p_y;z=p_z;
		r=p_r;t=p_t;l=p_l;
		ex=1.;ey=1.;ez=1.;
	}

	void Repere::initXYZ(float p_x, float p_y, float p_z)
	{
	    x=p_x;y=p_y;z=p_z;
	}
	void Repere::initXYZ(float xyz[3])
	{
	    x=xyz[0];y=xyz[1];z=xyz[2];
	}


	void Repere::initRTL(float p_r, float p_t, float p_l)
	{
	    r=p_r;t=p_t;l=p_l;
	}
	void Repere::initRTL(float rtl[3])
	{
	    r=rtl[0];t=rtl[1];l=rtl[2];
	}

	void Repere::copieRepere(Repere& source)
	{
	    x=source.x; y=source.y; z=source.z;
	    r=source.r;
	    t=source.t;
	    l=source.l;
	    ex=source.ex;
	    ey=source.ey;
	    ez=source.ez;
	}

	void Repere::visePoint(float px,float py,float pz)
	{

	    float OPx=px-x;
	    float OPy=py-y;
	    float OPz=pz-z;
	    //Calcul du lacet:
	    float NP=sqrt(OPx*OPx+OPz*OPz);
        float temp=OPz/NP;
        if(temp>1.)temp=1.; else if(temp<-1.)temp=-1.;
        l=acos(temp)/M_PI*180.;
        if(OPx<0.)l=-l;
        //Calcul du tangage:
        //Application du lacet au vecteur Oz:
        float OZz=cos(l/180*M_PI);
        float OZx=sin(l/180*M_PI);

        NP=sqrt(OPx*OPx+OPz*OPz+OPy*OPy);
        temp=(OZx*OPx+OZz*OPz)/NP;
        if(temp>1.)temp=1.; else if(temp<-1.)temp=-1.;
        t=-acos(temp)/M_PI*180.;
        if(OPy<0.)t=-t;
	}

	void Repere::visePoint(float p[3])
	{
        visePoint(p[0],p[1],p[2]);
	}

	void Repere::visePoint(vec3f &p)
	{
	    visePoint(p.x,p.y,p.z);
	}

	void Repere::calcul_matrice()
    {
        //glLoadIdentity();
        glTranslatef(x,y,z);
        glRotatef(l,0.,1.,0.);
        glRotatef(t,1.,0.,0.);
        glRotatef(r,0.,0.,1.);
        glScalef(ex,ey,ez);
        //glGetFloatv(GL_MODELVIEW_MATRIX,matrice->matrice);
    }


    //===========================================================
    //  Calculs sur le repère vectoriel
    //===========================================================
    void Repere::calcul_repere_vectoriel()
    {

            axe_x.init(1.,0.,0.);
            axe_y.init(0.,1.,0.);
            axe_z.init(0.,0.,1.);

        rotation_rtl_axe_repere_vectoriel(axe_x);
        rotation_rtl_axe_repere_vectoriel(axe_y);
        rotation_rtl_axe_repere_vectoriel(axe_z);


    }

    void Repere::rotation_rtl_axe_repere_vectoriel(vec3f& axe)
    {
        float px=axe.x;
        float py=axe.y;
        float pz=axe.z;
        float sin_aR=sin(r/180.*M_PI);
        float cos_aR=cos(r/180.*M_PI);
        float sin_aT=sin(-t/180.*M_PI);
        float cos_aT=cos(-t/180.*M_PI);
        float sin_aL=sin(l/180.*M_PI);
        float cos_aL=cos(l/180.*M_PI);

        axe.x= sin_aL * ( pz*cos_aT - sin_aT * ( px*sin_aR + py*cos_aR ) ) +  cos_aL * ( px*cos_aR - py*sin_aR );
        axe.y= pz * sin_aT + cos_aT * ( px*sin_aR + py*cos_aR );
        axe.z= cos_aL * ( pz*cos_aT - sin_aT * ( px*sin_aR + py*cos_aR ) ) - sin_aL * ( px*cos_aR - py*sin_aR );
    }

    //Translate un point par le repère, sans affecter son origine. (translation d'orientation)
    //Sert notamment pour les lumières situées à une distance infinie.
    void Repere::translate_point_orientation(float p[3])
    {
        calcul_repere_vectoriel();
        float intermediaire_x = p[0] * axe_x.x + p[1] * axe_y.x + p[2] * axe_z.x;
        float intermediaire_y = p[0] * axe_x.y + p[1] * axe_y.y + p[2] * axe_z.y;
        p[2] = p[0] * axe_x.z + p[1] * axe_y.z + p[2] * axe_z.z;
        p[0] = intermediaire_x;
        p[1] = intermediaire_y;
    }

    //Translate un repere par un autre repère:

    void Repere::translation_repere(Repere* parent)
    {
        calcul_repere_vectoriel();
        parent->calcul_repere_vectoriel();

        float fiston_abs_x = x;
        float fiston_abs_y = y;
        float fiston_abs_z = z;

        float rep_abs_axe_x_x = parent->axe_x.x;
        float rep_abs_axe_x_y = parent->axe_x.y;
        float rep_abs_axe_x_z = parent->axe_x.z;

        float rep_abs_axe_y_x = parent->axe_y.x;
        float rep_abs_axe_y_y = parent->axe_y.y;
        float rep_abs_axe_y_z = parent->axe_y.z;

        float rep_abs_axe_z_x = parent->axe_z.x;
        float rep_abs_axe_z_y = parent->axe_z.y;
        float rep_abs_axe_z_z = parent->axe_z.z;

        float rep_abs_x = parent->x;
        float rep_abs_y = parent->y;
        float rep_abs_z = parent->z;

        float intermediaire_x;
        float intermediaire_y;

        x = rep_abs_x + fiston_abs_x * rep_abs_axe_x_x +  fiston_abs_y * rep_abs_axe_y_x + fiston_abs_z * rep_abs_axe_z_x;
        y = rep_abs_y + fiston_abs_x * rep_abs_axe_x_y +  fiston_abs_y * rep_abs_axe_y_y + fiston_abs_z * rep_abs_axe_z_y;
        z = rep_abs_z + fiston_abs_x * rep_abs_axe_x_z +  fiston_abs_y * rep_abs_axe_y_z + fiston_abs_z * rep_abs_axe_z_z;

        intermediaire_x = axe_x.x * rep_abs_axe_x_x + axe_x.y * rep_abs_axe_y_x + axe_x.z * rep_abs_axe_z_x;
        intermediaire_y = axe_x.x * rep_abs_axe_x_y + axe_x.y * rep_abs_axe_y_y + axe_x.z * rep_abs_axe_z_y;
        axe_x.z = axe_x.x * rep_abs_axe_x_z + axe_x.y * rep_abs_axe_y_z + axe_x.z * rep_abs_axe_z_z;
        axe_x.x = intermediaire_x;
        axe_x.y = intermediaire_y;

        intermediaire_x = axe_y.x * rep_abs_axe_x_x + axe_y.y * rep_abs_axe_y_x + axe_y.z * rep_abs_axe_z_x;
        intermediaire_y = axe_y.x * rep_abs_axe_x_y + axe_y.y * rep_abs_axe_y_y + axe_y.z * rep_abs_axe_z_y;
        axe_y.z = axe_y.x * rep_abs_axe_x_z + axe_y.y * rep_abs_axe_y_z + axe_y.z * rep_abs_axe_z_z;
        axe_y.x = intermediaire_x;
        axe_y.y = intermediaire_y;

        intermediaire_x = axe_z.x * rep_abs_axe_x_x + axe_z.y * rep_abs_axe_y_x + axe_z.z * rep_abs_axe_z_x;
        intermediaire_y = axe_z.x * rep_abs_axe_x_y + axe_z.y * rep_abs_axe_y_y + axe_z.z * rep_abs_axe_z_y;
        axe_z.z = axe_z.x * rep_abs_axe_x_z + axe_z.y * rep_abs_axe_y_z + axe_z.z * rep_abs_axe_z_z;
        axe_z.x = intermediaire_x;
        axe_z.y = intermediaire_y;

        r=renvoie_roulis_relatif();
        t=renvoie_tangage_relatif();
        l=renvoie_lacet_relatif();
    }

    //Calcul les coordonnees relatives d'un repère par rapport à un autre repère:
    void Repere::calcul_repere_relatif(Repere* parent)
    {
        calcul_repere_vectoriel();
        parent->calcul_repere_vectoriel();

        float xTemp = x - parent->x;
        float yTemp = y - parent->y;
        float zTemp = z - parent->z;

        //Projection de la nouvelle position sur les axes du nouveau repère par produits scalaires:
        x = parent->axe_x.x * xTemp + parent->axe_x.y * yTemp + parent->axe_x.z * zTemp;
        y = parent->axe_y.x * xTemp + parent->axe_y.y * yTemp + parent->axe_y.z * zTemp;
        z = parent->axe_z.x * xTemp + parent->axe_z.y * yTemp + parent->axe_z.z * zTemp;


        axe_relatif(&axe_x, parent);
        axe_relatif(&axe_y, parent);
        axe_relatif(&axe_z, parent);

        r=renvoie_roulis_relatif();
        t=renvoie_tangage_relatif();
        l=renvoie_lacet_relatif();


    }

    //Calcul l'orientation d'un axe relativement à un repère:
    void Repere::axe_relatif(vec3f* axe,Repere* parent)
    {
        //Projection de l'axe:
        double xTemp = axe->x ;
        double yTemp = axe->y ;
        double zTemp = axe->z ;

        //Projection de la nouvelle position sur les axes du nouveau repère par produits scalaires:
        axe->x = parent->axe_x.x * xTemp + parent->axe_x.y * yTemp + parent->axe_x.z * zTemp ;
        axe->y = parent->axe_y.x * xTemp + parent->axe_y.y * yTemp + parent->axe_y.z * zTemp ;
        axe->z = parent->axe_z.x * xTemp + parent->axe_z.y * yTemp + parent->axe_z.z * zTemp ;
    }

    void Repere::affiche_repere_vectoriel()
    {
        calcul_repere_vectoriel();
        bool light_mem=false;
        bool depthTest_mem=false;
        if(glIsEnabled(GL_LIGHTING))light_mem=true;
        if(glIsEnabled(GL_DEPTH_TEST))depthTest_mem=true;
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glLineWidth(2.);
        glEnable(GL_COLOR_MATERIAL);
        glBegin(GL_LINES);
        glColor3f(1.,0.,0.);
        glVertex3f(x,y,z);
        glVertex3f(axe_x.x+x,axe_x.y+y,axe_x.z+z);
        glColor3f(0.,1.,0.);
        glVertex3f(x,y,z);
        glVertex3f(axe_y.x+x,axe_y.y+y,axe_y.z+z);
        glColor3f(0.,0.,1.);
        glVertex3f(x,y,z);
        glVertex3f(axe_z.x+x,axe_z.y+y,axe_z.z+z);
        glEnd();
        if(light_mem)glEnable(GL_LIGHTING);
        if(depthTest_mem)glEnable(GL_DEPTH_TEST);
    }

	//============================================================================================
    //      Calcul les angles tangage,lacet et roulis en fonction de l'orientation du repère
    //============================================================================================

        float Repere::renvoie_roulis_relatif()
		{
			float norme_vect_z = sqrtf(axe_z.x * axe_z.x + axe_z.z * axe_z.z);
			float temp_roulis = -axe_z.x / norme_vect_z * axe_x.z + axe_z.z / norme_vect_z * axe_x.x;

			if (temp_roulis > 1.) temp_roulis = 1.;
			else if (temp_roulis < -1.) temp_roulis = -1.;

			float roulis = -acosf(temp_roulis);
			if (axe_x.y < 0) roulis = -roulis;

			return -roulis/M_PI*180.;
		}

		float Repere::renvoie_tangage_relatif()
		{
			return -asinf(axe_z.y)/M_PI*180.;
		}

		float Repere::renvoie_lacet_relatif()
		{
			float norme_vect_z = sqrtf(axe_z.x * axe_z.x + axe_z.z * axe_z.z);

			float lacet_temp = axe_z.z / norme_vect_z;

			if (lacet_temp > 1.) lacet_temp = 1.;
			else if (lacet_temp < -1.) lacet_temp = -1.;

			float lacet = acosf(lacet_temp);
			if (axe_x.z > 0.) lacet = -lacet;

			return lacet/M_PI*180.;
		}

