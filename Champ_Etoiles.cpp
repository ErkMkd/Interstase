#include "Champ_etoiles.h"


    //======================================= Constructeur ================================

    Champ_Etoiles::Champ_Etoiles()
    {
        erreur=CHAMP_ETOILES_OK;
        etoiles=NULL;
        alpha_fondu=1.;
        image=NULL;
    }

    //Initialisation du champ d'étoiles:

    void Champ_Etoiles::initialise(float p_profondeur,
                    float p_largeur,
                    float p_hauteur,
                    unsigned int p_nbr_etoiles,
                    Image *p_image,
                    float echelle,
                    float p_alpha,
                    float p_vitesse_rotation,
                    vec3f *p_vitesse,
                    float masse //Masse des étoiles
                    )
    {
        profondeur=p_profondeur;
        largeur=p_largeur;
        hauteur=p_hauteur;
        nbr_etoiles=p_nbr_etoiles;
        largeur_face=echelle;
        vitesse_rotation=p_vitesse_rotation;
        alpha=p_alpha;
        angle=0;
        t_prec=-1;
        facteur_vitesse=1.;

        //Allocation des structures des étoiles:
        etoiles  = (particule_etoile *) malloc (sizeof(particule_etoile)*nbr_etoiles);
        if (etoiles==NULL)
        {
            printf("Impossible d'allouer la mémoire pour le champ d'étoiles\n");
            erreur=-1;
        }

        //Initialisation des étoiles:
        //La fonction rand() renvoie un int compris entre 0 et 32768 (en fait un "unsigned short")
        unsigned int i;
        for (i=0;i<nbr_etoiles;i++)
        {

            etoiles[i].x=float(rand())/32768.*largeur-largeur/2.;
            etoiles[i].y=float(rand())/32768.*hauteur-hauteur/2.;
            etoiles[i].z=float(rand())/32768.*profondeur-profondeur/2.;
            etoiles[i].vitesse.x=p_vitesse->x;
            etoiles[i].vitesse.y=p_vitesse->y;
            etoiles[i].vitesse.z=p_vitesse->z;
            etoiles[i].propulsion.x=0;
            etoiles[i].propulsion.y=0;
            etoiles[i].propulsion.z=-50;
            etoiles[i].masse=masse;
            etoiles[i].coefficient_frottement=determine_coefficient_frottement(200.,100.);

            //printf("position: %f,%f,%f\n",position_x,position_y,etoiles[i].z);
        }

        //Initialise la texture openGl de l'image:
        if(!determine_image(p_image))
            {
                printf("Impossible de créer la texture pour le champ d'étoiles\n");
                erreur=-1;
            }
    }

         //==================== Destructeur:
    Champ_Etoiles::~Champ_Etoiles()
    {
        //Libération de la mémoire utilisée pour les étoiles:
        if (etoiles != NULL) free(etoiles);
        etoiles=NULL;
    }


    bool Champ_Etoiles::determine_image(Image* p_image)
    {
        image=p_image;
        hauteur_face=largeur_face*float(image->hauteur)/float(image->largeur);
        return image->genere_texture_openGL(true);
    }

    //============================== Affichage du champ d'étoiles:

    //En paramètres, la vitesse du déplacement, et l'angle d'orientation des faces.
    //Avec cette fonction, le vecteur de direction du déplacement des étoiles n'est pas pris
    //en compte. Le déplacement se fait le long de l'axe Z , et les étoiles qui dépassent
    //en profondeur sont replacées au hazard du plan X,Y à l'autre bout.

    void Champ_Etoiles::affiche_deplacement_axe_z(float pas,float t, float angle_orientation)
    {
        glPushMatrix();
        glRotatef(angle,0,0,1.);                //Rotation des étoiles autour de l'axe de leur trajectoire
        glBindTexture(GL_TEXTURE_2D,image->texture_id);
        angle=vitesse_rotation*t;
        float delta_t=t-t_prec;
        t_prec=t;

        unsigned int i;
        for (i=0;i<nbr_etoiles;i++)
        {
            //Affichage:

            float position=etoiles[i].z;
            float alpha_final=alpha_fondu*alpha*(1-fabs(position)/(profondeur/2.));

            glPushMatrix();
            glTranslatef(etoiles[i].x,etoiles[i].y,etoiles[i].z);
            glRotatef(angle_orientation,0,1,0);
            glBegin(GL_QUADS);
               glColor4f(1.0,1.0,1.0,alpha_final);
               glTexCoord2f(0,0); glVertex2f(-largeur_face/2,hauteur_face/2);
               glTexCoord2f(0,1); glVertex2f(-largeur_face/2,-hauteur_face/2);
               glTexCoord2f(1,1); glVertex2f(largeur_face/2,-hauteur_face/2);
               glTexCoord2f(1,0); glVertex2f(largeur_face/2,hauteur_face/2);
            glEnd();
            glPopMatrix();


            //Bouclage:
            etoiles[i].z+=pas*delta_t;
            if (etoiles[i].z>profondeur/2)
            {
                etoiles[i].x=float(rand())/32768.*largeur-largeur/2;
                etoiles[i].y=float(rand())/32768.*hauteur-hauteur/2;
                etoiles[i].z=-profondeur/2;
            }
            else if (etoiles[i].z<-profondeur/2)
            {
                etoiles[i].x=float(rand())/32768.*largeur-largeur/2;
                etoiles[i].y=float(rand())/32768.*hauteur-hauteur/2;
                etoiles[i].z=profondeur/2;
            }
        }
        glPopMatrix();
    }


    //Affiche les étoiles avec un déplacement suivant leur vecteur de direction.
    //Delta_t  est l'intervalle de temps (en secondes) qui sert à calculer le pas à ajouter aux étoiles:
    void Champ_Etoiles::affiche_deplacement_vecteur_direction(float t, float angle_orientation)
    {
        if(t_prec<0.){t_prec=t;return;}
        glPushMatrix();
        glRotatef(angle,0,0,1.);                //Rotation des étoiles autour de l'axe de leur trajectoire
        glBindTexture(GL_TEXTURE_2D,image->texture_id);
        angle=vitesse_rotation*t;
        float delta_t=t-t_prec;
        t_prec=t;

        unsigned int i;
        for (i=0;i<nbr_etoiles;i++)
        {
            //Affichage:

            float position=etoiles[i].z;
            float alpha_final=alpha_fondu*alpha*(1.-fabs(position)/(profondeur/2));

            glPushMatrix();
            glTranslatef(etoiles[i].x,etoiles[i].y,etoiles[i].z);
            glRotatef(angle_orientation,0,1,0);
            glBegin(GL_TRIANGLE_FAN);
               glColor4f(1.0,1.0,1.0,alpha_final);
               glTexCoord2f(0,0); glVertex2f(-largeur_face/2,hauteur_face/2);
               glTexCoord2f(0,1); glVertex2f(-largeur_face/2,-hauteur_face/2);
               glTexCoord2f(1,1); glVertex2f(largeur_face/2,-hauteur_face/2);
               glTexCoord2f(1,0); glVertex2f(largeur_face/2,hauteur_face/2);
            glEnd();
            glPopMatrix();


            //Déplacement de l'étoile:
            //float valeur_propulsion=sqrtf(etoiles[i].propulsion.x*etoiles[i].propulsion.x + etoiles[i].propulsion.y*etoiles[i].propulsion.y +etoiles[i].propulsion.z*etoiles[i].propulsion.z);
            //etoiles[i].coefficient_frottement=determine_coefficient_frottement(40,valeur_propulsion);

            rafraichi_position(delta_t,&etoiles[i]);
            //etoiles[i].x+=etoiles[i].vitesse.x*delta_t;
            //etoiles[i].y+=etoiles[i].vitesse.y*delta_t;
            //etoiles[i].z+=etoiles[i].vitesse.z*delta_t;

            //Dépassement de la zone:
            if (etoiles[i].x < -largeur/2) etoiles[i].x=largeur/2;
            else if (etoiles[i].x > largeur/2) etoiles[i].x=-largeur/2;

            if (etoiles[i].y < -hauteur/2) etoiles[i].y=hauteur/2;
            else if (etoiles[i].y > hauteur/2) etoiles[i].y=-hauteur/2;

             if (etoiles[i].z < -profondeur/2) etoiles[i].z=profondeur/2;
            else if (etoiles[i].z > profondeur/2) etoiles[i].z=-profondeur/2;


        }
        glPopMatrix();
    }

    //Affiche les étoiles avec un déplacement suivant leur vecteur de direction.
    //Le vecteur de direction est calculé suivant l'attraction de centre de gravité.
    //Delta_t  est l'intervalle de temps (en secondes) qui sert à calculer le pas à ajouter aux étoiles:
    void Champ_Etoiles::affiche_deplacement_centre_gravite(float delta_t, float angle_orientation, vec3f *centre_gravite)
    {
        glRotatef(angle,0,0,1);                //Rotation des étoiles autour de l'axe de leur trajectoire
        glBindTexture(GL_TEXTURE_2D,image->texture_id);
        angle+=vitesse_rotation*delta_t;
        float valeur_propulsion;


        unsigned int i;
        for (i=0;i<nbr_etoiles;i++)
        {
            //Affichage:

            float position=etoiles[i].z;
            float alpha_final=alpha_fondu*alpha*(1-fabs(position)/(profondeur/2));

            glPushMatrix();
            glTranslatef(etoiles[i].x,etoiles[i].y,etoiles[i].z);
            glRotatef(angle_orientation,0,1,0);
            glBegin(GL_TRIANGLE_FAN);
               glColor4f(1.0,1.0,1.0,alpha_final);
               glTexCoord2f(0,0); glVertex2f(-largeur_face/2,hauteur_face/2);
               glTexCoord2f(0,1); glVertex2f(-largeur_face/2,-hauteur_face/2);
               glTexCoord2f(1,1); glVertex2f(largeur_face/2,-hauteur_face/2);
               glTexCoord2f(1,0); glVertex2f(largeur_face/2,hauteur_face/2);
            glEnd();
            glPopMatrix();


            //Déplacement de l'étoile:
            float pos_x=centre_gravite->x-etoiles[i].x;
            float pos_y=centre_gravite->y-etoiles[i].y;
            float pos_z=centre_gravite->z-etoiles[i].z;
            float distance_centre_gravite= sqrtf(pos_x*pos_x + pos_y*pos_y + pos_z*pos_z);
            valeur_propulsion=fabs(2000-distance_centre_gravite)/2000*500;
            etoiles[i].propulsion.x=pos_x/distance_centre_gravite*valeur_propulsion;
            etoiles[i].propulsion.y=pos_y/distance_centre_gravite*valeur_propulsion;
            etoiles[i].propulsion.z=pos_z/distance_centre_gravite*valeur_propulsion;
            //etoiles[i].coefficient_frottement=determine_coefficient_frottement(100,valeur_propulsion);

            rafraichi_position(delta_t,&etoiles[i]);
            //etoiles[i].x+=etoiles[i].vitesse.x*delta_t;
            //etoiles[i].y+=etoiles[i].vitesse.y*delta_t;
            //etoiles[i].z+=etoiles[i].vitesse.z*delta_t;

            //Dépassement de la zone:
            if (etoiles[i].x < -largeur/2) etoiles[i].x=largeur/2;
            else if (etoiles[i].x > largeur/2) etoiles[i].x=-largeur/2;

            if (etoiles[i].y < -hauteur/2) etoiles[i].y=hauteur/2;
            else if (etoiles[i].y > hauteur/2) etoiles[i].y=-hauteur/2;

             if (etoiles[i].z < -profondeur/2) etoiles[i].z=profondeur/2;
            else if (etoiles[i].z > profondeur/2) etoiles[i].z=-profondeur/2;


        }
    }

    //*********************************************************************************
    // Cinetique des particules
    //*********************************************************************************

    float Champ_Etoiles::determine_coefficient_frottement(float vlim,float valeur_propulsion)
    {
        return valeur_propulsion / vlim;
    }

    //Calcul la vitesse instantannée.
    void Champ_Etoiles::rafraichi_vitesse(float delta_t, particule_etoile *particule)
    {
        particule->vitesse.x += delta_t * ( -particule->coefficient_frottement / particule->masse * particule->vitesse.x + particule->propulsion.x / particule->masse);
        particule->vitesse.y += delta_t * ( -particule->coefficient_frottement / particule->masse * particule->vitesse.y + particule->propulsion.y / particule->masse);
        particule->vitesse.z += delta_t * ( -particule->coefficient_frottement / particule->masse * particule->vitesse.z + particule->propulsion.z / particule->masse);

        if (particule->vitesse.x * particule->vitesse.x + particule->vitesse.y * particule->vitesse.y + particule->vitesse.z * particule->vitesse.z < 1e-5)
        {
            particule->vitesse.x=0;
            particule->vitesse.y=0;
            particule->vitesse.z=0;
        }
    }

    void Champ_Etoiles::rafraichi_position(float delta_t, particule_etoile *particule)
    {
        //rafraichi_vitesse(delta_t, particule);
        particule->x += particule->vitesse.x * facteur_vitesse * delta_t;
        particule->y += particule->vitesse.y * facteur_vitesse * delta_t;
        particule->z += particule->vitesse.z * facteur_vitesse * delta_t;
    }

