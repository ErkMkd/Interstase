#include "Final.h"

    Repere Final::obs;
    TexteMap* Final::texteFin=NULL;
    Champ_Etoiles* Final::starfield=NULL;
    Champ_Etoiles* Final::starfield2=NULL;
    Champ_Etoiles* Final::starfield3=NULL;
    Champ_Etoiles* Final::starfield4=NULL;
    Champ_Etoiles* Final::starfield5=NULL;
    Champ_Etoiles* Final::starfield6=NULL;
    Champ_Etoiles* Final::starfield7=NULL;
    Image* Final::etoile=NULL;
    Image* Final::etoile2=NULL;
    Image* Final::etoile3=NULL;
    Image* Final::etoile4=NULL;
    Image* Final::etoile5=NULL;
    Image* Final::etoile6=NULL;
    Image* Final::etoile7=NULL;
    Sprites_2d* Final::sprites=NULL;
    const char Final::f1[]="images/logovale15.tif";
    const char Final::f2[]="images/design_final.jpg";
    const char Final::f3[]="images/cosmonaute.tif";
    const char* Final::fichiersSprites[]={Final::f1,Final::f2,Final::f3};

    float Final::cosmoX=0.840012;
    float Final::cosmoY=0.58881;

    float Final::couleur_ciel[4]={0.,0.,0.,1.};
    float Final::distance_brouillard=5000.;

    bool Final::dep_droite=false;
    bool Final::dep_gauche=false;
    bool Final::dep_avant=false;
    bool Final::dep_arriere=false;
    bool Final::dep_haut=false;
    bool Final::dep_bas=false;
    bool Final::rot_L_gauche=false;
    bool Final::rot_L_droite=false;
    bool Final::rot_T_haut=false;
    bool Final::rot_T_bas=false;
    bool Final::rot_R_gauche=false;
    bool Final::rot_R_droite=false;



    bool Final::zoom_in=false;
    bool Final::zoom_out=false;
    int Final::depId=1;

    float Final::aX=0.;
    float Final::aY=0.;
    float Final::aZ=0.;
    float Final::aR=0.;
    float Final::aT=0.;
    float Final::aL=0.;

     bool Final::init_scene()
     {

        //Initialise le scroll-text:
        texteFin=new TexteMap("polices/end_font.tif",GVars::largeur_ecran,GVars::hauteur_ecran);
        if(texteFin==NULL || texteFin->erreur!=TEXTEMAP_OK) return false;
        texteFin->mode_rendu=TEXTEMAP_RENDU_2D;
        texteFin->determine_position_curseur(0.358726,0.);
        texteFin->determine_echelle(float(texteFin->renvoie_largeur_caracteres())/float(GVars::largeur_ecran));
        texteFin->determine_espace_lignes(0.);
        texteFin->determine_espace_lettres(0.1);
        texteFin->determine_texte_fichier("texte_final.txt");
        texteFin->centrage=true;
        texteFin->initialise_scrollText_vertical(0.,1.,0.);

        //Init les sprites
        sprites=new Sprites_2d(FINAL_SPR_NUM,Final::fichiersSprites);
        if(Final::sprites->erreur!=SPR2D_OK) return false;

        sprites->determine_position_sprite(FINAL_SPR_LOGOMKD,0.358726,0.516241);
        sprites->determine_echelle_sprite(FINAL_SPR_LOGOMKD,.943883);
        sprites->determine_position_sprite(FINAL_SPR_DESIGN,0.857891,0.509098);
        sprites->determine_echelle_sprite(FINAL_SPR_DESIGN,1.);

        //Init le starfield:
        etoile=new Image("images/etoile_b00.tif");
        etoile2=new Image("images/etoile_a00.tif");
        etoile3=new Image("images/etoile_b01.tif");
        etoile4=new Image("images/etoile_nebuleuse_01.tif");
        etoile5=new Image("images/etoile_a03.tif");
        etoile6=new Image("images/nebuleuse_03.tif");
        etoile7=new Image("images/nebuleuse_01.tif");

        starfield=new Champ_Etoiles();
        starfield2=new Champ_Etoiles();
        starfield3=new Champ_Etoiles();
        starfield4=new Champ_Etoiles();
        starfield5=new Champ_Etoiles();
        starfield6=new Champ_Etoiles();
        starfield7=new Champ_Etoiles();

        vec3f v(-84.,0.,-250.);
        starfield->initialise(
                                5000.,2000.,1000.,
                                6000,
                                etoile,
                                60.,
                                1.,0.,&v,1.);
        starfield2->initialise(
                                5000.,2000.,1000.,
                                4000,
                                etoile2,
                                60.,
                                1.,0.,&v,1.);
        starfield3->initialise(
                                5000.,2000.,1000.,
                                2000,
                                etoile3,
                                70.,
                                1.,0.,&v,1.);
        starfield4->initialise(
                                6000.,4000.,3000.,
                                30,
                                etoile4,
                                300.,
                                1.,-4.456,&v,1.);
        starfield5->initialise(
                                5000.,2000.,1000.,
                                50,
                                etoile5,
                                80.,
                                1.,0.,&v,1.);
        starfield6->initialise(
                                15000.,10000.,8000.,
                                20,
                                etoile6,
                                4500.,
                                1.,-3.4713,&v,1.);
        starfield7->initialise(
                                15000.,10000.,8000.,
                                30,
                                etoile7,
                                3700.,
                                1.,5.179,&v,1.);


    /*
    float p_profondeur,
                    float p_largeur,
                    float p_hauteur,
                    unsigned int p_nbr_etoiles,
                    Image *p_image,
                    float echelle,
                    float p_alpha,
                    float p_vitesse_rotation,
                    vec3f *p_vitesse,
                    float masse //Masse des étoiles
    */
        if(starfield->erreur!=0) return false;
        return true;
     }
     void Final::detruit_scene()
     {
        if(starfield!=NULL)delete starfield;
        if(starfield2!=NULL)delete starfield2;
        if(starfield3!=NULL)delete starfield3;
        if(starfield4!=NULL)delete starfield4;
        if(starfield5!=NULL)delete starfield5;
        if(starfield6!=NULL)delete starfield6;
        if(starfield7!=NULL)delete starfield7;
        if(etoile!=NULL) delete etoile;
        if(etoile2!=NULL) delete etoile2;
        if(etoile3!=NULL) delete etoile3;
        if(etoile4!=NULL) delete etoile4;
        if(etoile5!=NULL) delete etoile5;
        if(etoile6!=NULL) delete etoile6;
        if(etoile7!=NULL) delete etoile7;
        if(sprites!=NULL) delete sprites;
        if(texteFin!=NULL)delete texteFin;
     }

     void Final::clavier(int key,bool etat)
     {
                 switch (key)
            {
                    case GLFW_KEY_RIGHT:dep_droite=etat;break;
                    case GLFW_KEY_LEFT:dep_gauche=etat;break;
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

                    case GLFW_KEY_KP_ADD:zoom_in=etat;break;
                    case GLFW_KEY_KP_SUBTRACT:zoom_out=etat;break;


                    case GLFW_KEY_F1: depId=1;break;
                    case GLFW_KEY_F2: depId=2;break;
                    case GLFW_KEY_F3: depId=3;break;
                    case GLFW_KEY_F4: depId=4;break;
                    case GLFW_KEY_F5: depId=5;break;

                    case GLFW_KEY_F12: if(etat)GVars::editionTrajectoire=true;break; //Editeur de trajectoire
            }

     }
     void Final::maj_deplacement()
     {
         uint32_t indice;
        float deceleration=0.75;
        float acceleration=.001;
        float decel_ang=.85;
        float accel_ang=0.07;
        float raX,raZ;

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

        if(depId==1)
        {
            indice=FINAL_SPR_ASTRONAUTE;
        }
        if(depId==2)
        {
            //indice=FINAL_SPR_LOGOMKD;
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
        if(depId==3)
        {
            indice=FINAL_SPR_DESIGN;
        }

            float x=sprites->renvoie_sprite_x(indice);
            float y=sprites->renvoie_sprite_y(indice);
            float e=sprites->renvoie_echelle_sprite(indice);
            x+=aX;
            y+=aY;
            sprites->determine_position_sprite(indice,x,y);
            if(zoom_in)e*=1.01;
            if(zoom_out)e*=.99;
            sprites->determine_echelle_sprite(indice,e);

            if(GVars::disp_donnees)
            {
                cout<<"Sprite X="<<x<<" Y="<<y<<" H="<<e<<endl;
            }

        GVars::disp_donnees=false;


        sprites->determine_position_sprite(FINAL_SPR_ASTRONAUTE,cosmoX+0.01*cos(GVars::temps_scene*2.123),cosmoY+0.005*sin(GVars::temps_scene*3.77951));
        sprites->determine_echelle_sprite(FINAL_SPR_ASTRONAUTE,1.+0.07*cos(GVars::temps_scene*0.78512));
        sprites->determine_rotation_sprite(FINAL_SPR_ASTRONAUTE,5.*cos(GVars::temps_scene*0.88512));

        texteFin->maj_scrolling_vertical(GVars::temps_scene);

     }
     void Final::init_affiche()
     {
            glFogi(GL_FOG_MODE,GL_LINEAR);
            glFogfv(GL_FOG_COLOR,couleur_ciel);
            glFogf(GL_FOG_START,0.);
            glFogf(GL_FOG_END,distance_brouillard);
            glFogf(GL_FOG_DENSITY,1.);
            glHint(GL_FOG_HINT,GL_NICEST);
            glEnable(GL_FOG);
     }
     void Final::affiche()
     {
         /*
        sprites->cache_sprite(FINAL_SPR_ASTRONAUTE);
        sprites->cache_sprite(FINAL_SPR_DESIGN);
        sprites->montre_sprite(FINAL_SPR_LOGOMKD);
        sprites->affiche_sprites_2d();
        */

        glClearColor(0.,0.,0.,1.);
        float couleur_fog[4]={0.,0.,0.,0.};
        glFogfv(GL_FOG_COLOR,couleur_fog);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glDisable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        glUseProgram(0);
        glDisable(GL_DEPTH_TEST); //Désactive la prise en compte de la profondeur ("z-buffer")
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        glDepthMask(GL_FALSE);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        //glCullFace(GL_CCW);
        GFunc::vue_perspective(GVars::zMin,GVars::zMax);
        GFunc::init_matrice_observateur(&obs);
        starfield6->affiche_deplacement_vecteur_direction(GVars::temps_scene,0.);
        starfield7->affiche_deplacement_vecteur_direction(GVars::temps_scene,0.);
        starfield4->affiche_deplacement_vecteur_direction(GVars::temps_scene,0.);
        starfield->affiche_deplacement_vecteur_direction(GVars::temps_scene,0.);
        starfield2->affiche_deplacement_vecteur_direction(GVars::temps_scene,0.);
        starfield3->affiche_deplacement_vecteur_direction(GVars::temps_scene,0.);
        starfield5->affiche_deplacement_vecteur_direction(GVars::temps_scene,0.);

        //starfield->affiche_deplacement_axe_z(0.1,GVars::temps_scene,0.);

        sprites->montre_sprite(FINAL_SPR_ASTRONAUTE);
        sprites->montre_sprite(FINAL_SPR_DESIGN);
        sprites->montre_sprite(FINAL_SPR_LOGOMKD);
        sprites->affiche_sprites_2d();

        //Affiche le scroll-texte:

        glEnable(GL_BLEND);
        glDepthMask(GL_FALSE);
        texteFin->affiche();
        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);

     }

     void Final::render_scene()
     {
        init_affiche();
        affiche();
     }
