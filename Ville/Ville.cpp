#include "Ville.h"

#define VILLE_DEBUG_TRAFIC false
using namespace std;

Repere Ville::objTest_parent;
Repere Ville::objTest_obsBkp;
Repere Ville::objTest_enfant;
Repere Ville::objTest_enfant_temp;
Repere Ville::objTest_enfant_temp2;

float Ville::t_prec=0.;

float Ville::angleObj=0.;
int Ville::num_etages_debug=10;
float Ville::xTetra=0.;

uint16_t Ville::erreur=0;

bool Ville::drapeau_premier_rendu=true;
bool Ville::drapeau_affiche_Edora=false;
float Ville::alpha_Edora=0.;

//Parametres terrain:
float Ville::partSize=VILLE_PART_SIZE;
float Ville::distanceMax=VILLE_DISTANCE_MAX;
int Ville::nbrDalles=0;
float Ville::marge_terrain=1.4;
Texgen* Ville::bitmap_terrain=NULL;
uint8_t* Ville::map_terrain=NULL;
uint8_t* Ville::map_terrain_temp=NULL;
uint8_t* Ville::map_routes=NULL;
int Ville::taille_terrain=0;
bool Ville::drapeau_affiche_bitmap_terrain=false;
vec2f Ville::triangle_visible[3];
int Ville::triangle_visible_int[6];
uint32_t Ville::couleurs_iters[128];

//Generateur via Mandelbox:
uint8_t Ville::terrain_iterMax=25;
float Ville::terrain_s=2.;
float Ville::terrain_r=0.5;
float Ville::terrain_zoom=1000.;

uint32_t Ville::objet_ptr;
uint32_t Ville::objet_grille_ptr;
objet_visible_ville Ville::objets[VILLE_NBR_OBJETS_MAX];
objet_visible_ville Ville::grille[VILLE_NBR_OBJETS_MAX];

element_route Ville::tab_routes[16];

//Gestion des véhicules:
uint16_t Ville::nbrVehicules=1;
ville_vehicule Ville::vehicules[VILLE_NBR_VEHICULES_MAX];
float Ville::palette_vehicules[3*VILLE_NBR_COULEURS_VEHICULES]={1.,0.,0.,
                                                                0.,1.,0.,
                                                                0.,0.,1.,
                                                                1.,1.,0.,
                                                                1.,0.,1.,
                                                                0.,1.,1.,
                                                                1.,1.,1.,
                                                                1.,0.5,0.};

//Paramètres scène:
Repere Ville::obs;
vec3f Ville::point_vise;
vec3f Ville::position_edora(-24220.4,125.,-2356.61);
Bruit Ville::clignotement_Edora;

float Ville::aX=0.;
float Ville::aY=0.;
float Ville::aZ=0.;
float Ville::aR=0.;
float Ville::aT=0.;
float Ville::aL=0.;

//Commandes clavier:
bool Ville::dep_droite=false;
bool Ville::dep_gauche=false;
bool Ville::dep_avant=false;
bool Ville::dep_arriere=false;
bool Ville::dep_haut=false;
bool Ville::dep_bas=false;
bool Ville::rot_L_gauche=false;
bool Ville::rot_L_droite=false;
bool Ville::rot_T_haut=false;
bool Ville::rot_T_bas=false;
bool Ville::rot_R_gauche=false;
bool Ville::rot_R_droite=false;

int Ville::depId=2;
bool Ville::drapeau_affiche_trame=false;

float Ville::couleur_soleil[3]={255./255.,224./255.,155./255.};
float Ville::couleur_ciel[4]={255./255.,142./255.,124./255.,1.};
float Ville::couleur_brouillard[4]={couleur_ciel[0],couleur_ciel[1],couleur_ciel[2],couleur_ciel[3]};
float Ville::distance_brouillard=5000.; ///initialisé à distanceMax dans l'init().
float Ville::couleur_fond_eau[4]={48./255./2.,127./255./2.,105/255./2.,1.};

float Ville::L0_orientation[3]={-0.724259,0.539958,-0.428829};
float Ville::L0_amb[4];//={0.,0.,0.,1.};
float Ville::L0_diff[4]={255./255.,237./255.,131./255.,1.};
float Ville::L0_spec[4]={L0_diff[0],L0_diff[1],L0_diff[2],L0_diff[3]};

float Ville::L1_orientation[3];
float Ville::L1_amb[4]={0.,0.,0.,1.};
float Ville::L1_diff[4]={72./255.,87./255.,135./255.,1.};
float Ville::L1_spec[4]={L1_diff[0],L1_diff[1],L1_diff[2],L1_diff[3]};



bool Ville::init_shaders()
{
    return true;
}

void Ville::detruit_shaders()
{

}

bool Ville::init_textures()
{
    /*
    tex_couleurs=new Image("textures_ville/couleurs.tif");
    tex_normales=new Image("textures_ville/normal_map.tif");
    tex_profondeurs=new Image("textures_ville/relief_map.tif");
    textureCones01=new Texgen(256,256,GL_RG,GL_FLOAT);
    //textureCones01->calcule_conesRelief(tex_profondeurs,0.1);

    if(tex_couleurs==NULL || tex_normales==NULL || tex_normales==NULL || textureCones01==NULL
       || tex_couleurs->erreur!=IMAGE_OK
       || tex_normales->erreur!=IMAGE_OK
       || tex_profondeurs->erreur!=IMAGE_OK
       || textureCones01->erreur!=TEXGEN_OK
       ) return false;

    tex_couleurs->renversement_horizontal();
    tex_profondeurs->renversement_horizontal();
    tex_normales->renversement_horizontal();

    if(!tex_couleurs->genere_texture_openGL(false,GL_REPEAT,GL_REPEAT))return false;
    if(!tex_normales->genere_texture_openGL(false,GL_REPEAT,GL_REPEAT))return false;
    if(!tex_profondeurs->genere_texture_openGL(false,GL_REPEAT,GL_REPEAT))return false;
    if(!textureCones01->genere_framebuffer())return false;



    //Génère les textures de relief:
    textureCones01->calcule_conesRelief_GPU(tex_profondeurs,.163,GVars::ecran);

    glUseProgram(0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
    */
    return true;

}

bool Ville::init_scene()
{
    float RGBf[3];
    /*GFunc::RGBi_RGBf(0xf3587e,RGBf);
    couleur_brouillard[0]=RGBf[0];
    couleur_brouillard[1]=RGBf[1];
    couleur_brouillard[2]=RGBf[2];
    couleur_brouillard[3]=1.;
    couleur_ciel[0]=RGBf[0];
    couleur_ciel[1]=RGBf[1];
    couleur_ciel[2]=RGBf[2];
    couleur_ciel[3]=1.;*/
    L0_amb[0]=couleur_ciel[0]/10.;
    L0_amb[1]=couleur_ciel[1]/10.;
    L0_amb[2]=couleur_ciel[2]/10.;

    /*
    GFunc::RGBi_RGBf(0x307f69,RGBf);
    L1_diff[0]=RGBf[0];
    L1_diff[1]=RGBf[1];
    L1_diff[2]=RGBf[2];
    L1_diff[3]=1.;
    */

    /*
    L0_diff[0]=1.;
    L0_diff[1]=1.;
    L0_diff[2]=1.;
    L0_diff[3]=1.;
    */

    //obs.initXYZ(-543.*partSize,3.,463.25*partSize);
    //obs.initRTL(0.,0.,180.);
    distance_brouillard=distanceMax;
    if(!init_shaders()) return false;
    if(!init_textures()) return false;

    taille_terrain=roundf((distanceMax/partSize*2.)*sqrtf(2.)*marge_terrain+20.);
    taille_terrain=(taille_terrain+4)&0b1111111111111100;
    cout<<"Taille du terrain visible:"<<taille_terrain<<endl;
    bitmap_terrain=new Texgen(int(taille_terrain),int(taille_terrain),GL_RGBA,GL_UNSIGNED_BYTE,GL_CLAMP,GL_CLAMP);
    if(bitmap_terrain==NULL || bitmap_terrain->erreur!=TEXGEN_OK) return false;

    map_terrain=(uint8_t*)malloc(taille_terrain*taille_terrain*sizeof(uint8_t)*3);
    if(map_terrain==NULL) {cout<<"Impossible d'allouer map_terrain"<<endl;return false;}
    map_terrain_temp=map_terrain+taille_terrain*taille_terrain*sizeof(uint8_t);
    map_routes=map_terrain_temp+taille_terrain*taille_terrain*sizeof(uint8_t);

    if(!init_elements(partSize,VILLE_BATIMENT_HAUTEUR_ETAGE)) return false;
    //Tableau de correspondance code/objets éléments route:
    tab_routes[0].angleRoute=0.;
    tab_routes[0].route_id=route_01_carrefour_4_liste_id;
    tab_routes[1].angleRoute=180.;
    tab_routes[1].route_id=route_01_carrefour_3_liste_id;
    tab_routes[2].angleRoute=90.;
    tab_routes[2].route_id=route_01_carrefour_3_liste_id;
    tab_routes[3].angleRoute=0.;
    tab_routes[3].route_id=route_01_virage90_droite_liste_id;
    tab_routes[4].angleRoute=0.;
    tab_routes[4].route_id=route_01_carrefour_3_liste_id;
    tab_routes[5].angleRoute=0.;
    tab_routes[5].route_id=route_01_droite_liste_id;
    tab_routes[6].angleRoute=-90.;
    tab_routes[6].route_id=route_01_virage90_droite_liste_id;
    tab_routes[7].angleRoute=0.;
    tab_routes[7].route_id=route_01_parking_liste_id;
    tab_routes[8].angleRoute=-90.;
    tab_routes[8].route_id=route_01_carrefour_3_liste_id;
    tab_routes[9].angleRoute=90.;
    tab_routes[9].route_id=route_01_virage90_droite_liste_id;
    tab_routes[10].angleRoute=-90.;
    tab_routes[10].route_id=route_01_droite_liste_id;
    tab_routes[11].angleRoute=90.;
    tab_routes[11].route_id=route_01_parking_liste_id;
    tab_routes[12].angleRoute=180.;
    tab_routes[12].route_id=route_01_virage90_droite_liste_id;
    tab_routes[13].angleRoute=180.;
    tab_routes[13].route_id=route_01_parking_liste_id;
    tab_routes[14].angleRoute=-90.;
    tab_routes[14].route_id=route_01_parking_liste_id;
    tab_routes[15].angleRoute=0.;
    tab_routes[15].route_id=route_01_espace_vert_liste_id;
    init_palette_terrain();

    for(int i=0;i<4;i++)
    {
        uint16_t s1=tetraedre_faces[i*3];
        uint16_t s2=tetraedre_faces[i*3+1];
        uint16_t s3=tetraedre_faces[i*3+2];
        vec3f v1(tetraedre_sommets[s3*3]-tetraedre_sommets[s1*3],tetraedre_sommets[s3*3+1]-tetraedre_sommets[s1*3+1],tetraedre_sommets[s3*3+2]-tetraedre_sommets[s1*3+2]);
        vec3f v2(tetraedre_sommets[s2*3]-tetraedre_sommets[s1*3],tetraedre_sommets[s2*3+1]-tetraedre_sommets[s1*3+1],tetraedre_sommets[s2*3+2]-tetraedre_sommets[s1*3+2]);
        vec3f prodVec(0.,0.,0.);
        prodVec.produit_vectoriel(v2,v1);
        prodVec.normalise();
        cout<<prodVec.x<<" ; "<<prodVec.y<<" ; "<<prodVec.z<<endl;
    }


    GVars::vaisseau_2->determine_position(-24264.7,217.533,-2501.57);
    GVars::vaisseau_2->determine_RTL(0.,0.,-90.);
    GVars::vaisseau_1->determine_position(-24264.7,217.533,-2501.57);
    GVars::vaisseau_1->determine_RTL(0.,0.,-90.);

    return true;
}

void Ville::detruit_textures()
{
    if(textureCones01!=NULL) delete textureCones01;
    if(tex_couleurs!=NULL) delete tex_couleurs;
    if(tex_normales!=NULL) delete tex_normales;
    if(tex_profondeurs !=NULL) delete tex_profondeurs;
}

void Ville::detruit_scene()
{
    detruit_textures();
    if(glIsList(triangle_liste_id)) glDeleteLists(triangle_liste_id,1);
    if(glIsList(disque_liste_id)) glDeleteLists(disque_liste_id,1);
	if(glIsList(sphere_liste_id)) glDeleteLists(sphere_liste_id,1);
	if(glIsList(cylindre_liste_id)) glDeleteLists(cylindre_liste_id,1);
	if(glIsList(tore_liste_id))     glDeleteLists(tore_liste_id,1);
	if(glIsList(cube_liste_id))     glDeleteLists(cube_liste_id,1);
	if(glIsList(tetraedre_liste_id))     glDeleteLists(tetraedre_liste_id,1);
	if(glIsList(cube02_liste_id))     glDeleteLists(cube02_liste_id,1);
	if(glIsList(plan01_liste_id))     glDeleteLists(plan01_liste_id,1);
	if(glIsList(plan02_liste_id))     glDeleteLists(plan02_liste_id,1);
	if(glIsList(rectangle_contours_liste_id))    glDeleteLists(rectangle_contours_liste_id,1);

	if(glIsList(immeuble_base_liste_id))    glDeleteLists(immeuble_base_liste_id,1);
	if(glIsList(immeuble_01_liste_id))    glDeleteLists(immeuble_01_liste_id,1);
	if(glIsList(immeuble_02_liste_id))    glDeleteLists(immeuble_02_liste_id,1);
	if(glIsList(immeuble_03_liste_id))    glDeleteLists(immeuble_03_liste_id,1);
	if(glIsList(immeuble_04_liste_id))    glDeleteLists(immeuble_04_liste_id,1);

	if(glIsList(route_01_droite_liste_id))    glDeleteLists(route_01_droite_liste_id,1);
	if(glIsList(route_01_virage90_droite_liste_id))    glDeleteLists(route_01_virage90_droite_liste_id,1);
	if(glIsList(route_01_virage90_gauche_liste_id))    glDeleteLists(route_01_virage90_gauche_liste_id,1);
	if(glIsList(route_01_carrefour_3_liste_id))    glDeleteLists(route_01_carrefour_3_liste_id,1);
	if(glIsList(route_01_carrefour_4_liste_id))    glDeleteLists(route_01_carrefour_4_liste_id,1);
	if(glIsList(route_01_parking_liste_id))    glDeleteLists(route_01_parking_liste_id,1);
	if(glIsList(route_01_espace_vert_liste_id))    glDeleteLists(route_01_espace_vert_liste_id,1);
	if(glIsList(bordure_eau_base_liste_id))    glDeleteLists(bordure_eau_base_liste_id,1);

	detruit_shaders();

	if(bitmap_terrain!=NULL)delete bitmap_terrain;
	if(map_terrain!=NULL) free(map_terrain);
}


void Ville::clavier(int key,bool etat)
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

            case GLFW_KEY_Y:
                if(etat && drapeau_reliefs) drapeau_reliefs=false;
                else if(etat && (!drapeau_reliefs)) drapeau_reliefs=true;
                break;

            case GLFW_KEY_B:
                if(etat && drapeau_affiche_bitmap_terrain) drapeau_affiche_bitmap_terrain=false;
                else if(etat && (!drapeau_affiche_bitmap_terrain)) drapeau_affiche_bitmap_terrain=true;
                break;

            case GLFW_KEY_G:
                if(etat && drapeau_affiche_trame) drapeau_affiche_trame=false;
                else if(etat && (!drapeau_affiche_trame)) drapeau_affiche_trame=true;
                break;

            case GLFW_KEY_1:
                if(!etat)
                {
                    terrain_iterMax--;
                    if(terrain_iterMax==0)terrain_iterMax=1;
                    init_palette_terrain();
                }
                break;
            case GLFW_KEY_2:
                if(!etat)
                {
                    terrain_iterMax++;
                    init_palette_terrain();
                }
                break;
            case GLFW_KEY_3:
                if(etat)
                {
                    terrain_s+=0.001;
                }
                break;
             case GLFW_KEY_4:
                if(etat)
                {
                    terrain_s-=0.001;
                    if(terrain_s<0.003)terrain_s=0.003;
                }
                break;

            case GLFW_KEY_5:
                if(etat)
                {
                    terrain_r+=0.001;
                }
                break;
             case GLFW_KEY_6:
                if(etat)
                {
                    terrain_r-=0.001;
                    if(terrain_r<0.003)terrain_r=0.003;
                }
                break;

            case GLFW_KEY_7:
                if(etat)
                {
                    terrain_zoom*=1.01;
                }
                break;
             case GLFW_KEY_8:
                if(etat)
                {
                    terrain_zoom*=0.99;
                }
                break;



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
        EditeurTrajectoire::edit_trajectoire(key,etat);
        cosmo->x=EditeurTrajectoire::ET_x;
        cosmo->y=EditeurTrajectoire::ET_y;
        cosmo->z=EditeurTrajectoire::ET_z;
        */

        /*
        EditeurTrajectoire::ET_x=obs.x;
        EditeurTrajectoire::ET_y=obs.y;
        EditeurTrajectoire::ET_z=-obs.z;
        EditeurTrajectoire::edit_trajectoire(key,etat);
        obs.x=EditeurTrajectoire::ET_x;
        obs.y=EditeurTrajectoire::ET_y;
        obs.z=-EditeurTrajectoire::ET_z;
        */
        EditeurTrajectoire::ET_x=GVars::vaisseau_2->x;
        EditeurTrajectoire::ET_y=GVars::vaisseau_2->y;
        EditeurTrajectoire::ET_z=GVars::vaisseau_2->z;
        EditeurTrajectoire::edit_trajectoire(key,etat);
        GVars::vaisseau_2->x=EditeurTrajectoire::ET_x;
        GVars::vaisseau_2->y=EditeurTrajectoire::ET_y;
        GVars::vaisseau_2->z=EditeurTrajectoire::ET_z;

        /*
        EditeurTrajectoire::ET_x=point_vise.x;
        EditeurTrajectoire::ET_y=point_vise.y;
        EditeurTrajectoire::ET_z=point_vise.z;
        EditeurTrajectoire::edit_trajectoire(key,etat,EditeurTrajectoire::ma_trajectoire);
        point_vise.x=EditeurTrajectoire::ET_x;
        point_vise.y=EditeurTrajectoire::ET_y;
        point_vise.z=EditeurTrajectoire::ET_z;
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

void Ville::maj_deplacement()
{
    float deceleration=0.75;
    float acceleration=.005;

    float decel_ang=.85;
    float accel_ang=0.07;

    float raX,raZ;


    /*
    float vDep=sqrtf(aX*aX+aY*aY+aZ*aZ);
    if(vDep>0.025)
    {*/
        aX*=deceleration;
        aY*=deceleration;
        aZ*=deceleration;
    //}


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
         raX=aX;
        raZ=aZ;

		L0_orientation[0]+=raX;
		L0_orientation[1]+=aY;
		L0_orientation[2]+=raZ;

		float n=sqrtf(L0_orientation[0]*L0_orientation[0]+L0_orientation[1]*L0_orientation[1]+L0_orientation[2]*L0_orientation[2]);
		L0_orientation[0]/=n;
		L0_orientation[1]/=n;
		L0_orientation[2]/=n;

		if (GVars::disp_donnees)
        {
            cout<<"L0 - X="<<L0_orientation[0]<<" Y="<<L0_orientation[1]<<" Z="<<L0_orientation[2]<<endl;
        }
    }

    if(depId==2)
    {
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

		GFunc::rotation_plan(-GVars::vaisseau_2->l,aX,aZ,raX,raZ);

		GVars::vaisseau_2->x+=raX;
		GVars::vaisseau_2->y+=aY;
		GVars::vaisseau_2->z+=raZ;

		GVars::vaisseau_2->r+=aR;
		GVars::vaisseau_2->t+=aT;
		GVars::vaisseau_2->l+=aL;


		if (GVars::disp_donnees)
        {
            cout<<"Vaisseau - X="<<GVars::vaisseau_2->x<<" Y="<<GVars::vaisseau_2->y<<" Z="<<GVars::vaisseau_2->z<<" H="<<GVars::vaisseau_2->ex<<" R="<<GVars::vaisseau_2->r<<" T="<<GVars::vaisseau_2->t<<" L="<<GVars::vaisseau_2->l<<endl;
        }


        /*
        objTest_parent.x+=raX;
		objTest_parent.y+=aY;
		objTest_parent.z-=raZ;

		objTest_parent.r+=aR;
		objTest_parent.t+=aT;
		objTest_parent.l+=aL;

		if (GVars::disp_donnees)
        {
            cout<<"ObjTest parent - X="<<objTest_parent.x<<" Y="<<objTest_parent.y<<" Z="<<objTest_parent.z<<" R="<<objTest_parent.r<<" T="<<objTest_parent.t<<" L="<<objTest_parent.l<<endl;
        }
        */


        /*
        position_edora.x+=aX;
        position_edora.y+=aY;
        position_edora.z+=aZ;
        if (GVars::disp_donnees)
        {
            cout<<"Edora - X="<<position_edora.x<<" Y="<<position_edora.y<<" Z="<<position_edora.z<<endl;
        }
        */

	}

	if (depId==4)
	{
        raX=aX;
        raZ=aZ;

        objTest_enfant.x+=raX;
		objTest_enfant.y+=aY;
		objTest_enfant.z-=raZ;

		objTest_enfant.r+=aR;
		objTest_enfant.t+=aT;
		objTest_enfant.l+=aL;

		if (GVars::disp_donnees)
        {
            cout<<"ObjTest enfant - X="<<objTest_enfant.x<<" Y="<<objTest_enfant.y<<" Z="<<objTest_enfant.z<<" R="<<objTest_enfant.r<<" T="<<objTest_enfant.t<<" L="<<objTest_enfant.l<<endl;
        }

        /*
		point_vise.x+=aX;
		point_vise.y+=aY;
		point_vise.z+=aZ;
		if (GVars::disp_donnees)
        {
            cout<<"Point visé - X="<<point_vise.x<<" Y="<<point_vise.y<<" Z="<<point_vise.z<<endl;

        }
        */

	}

    if (depId==5)
	{
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
        //angleObj=180.;
}

void Ville::genere_grille(int ymin,int ymax,float p_x0,float p_x1,float d1,float d2)
{
	float x0=p_x0;
    float x1=p_x1;
        for(int y=ymin;y<=ymax;y++)
        {
            for(int x=int(x0);x<int(x1);x++)
            {
                nbrDalles++;
                if(x&01==0 && y&01==0 || y&01==1 && x&01==1) continue;
                float xDalle=float(x)*partSize;
                float zDalle=float(y)*partSize;
                float yDalle=VILLE_ALTITUDE_ROUTES+0.1;//((cosf(zDalle/10.346)-1)+(cosf(xDalle/5.789)-1)+(sinf(xDalle*3.36-zDalle*2.487)-1)*10)*3;
                glPushMatrix();
                glTranslatef(xDalle,yDalle,zDalle);
                //glRotatef(0,-1.,1.,0.);
                glScalef(partSize,1,partSize);
                glRotatef(90,1.,0.,0.);
                sauve_objet_grille(rectangle_contours_liste_id);
                //glCallList(rectangle_contours_liste_id);

                glPopMatrix();
            }
            x0+=d1;
            x1+=d2;
        }
}

void Ville::affiche_rectangle_terrain(int x, int y, int tx, int ty, uint8_t c)
{
    for(int i=0;i<ty;i++)
    {
        for(int j=0;j<tx;j++)
        {
            affiche_pixel_terrain(j+x,i+y,c);
        }
    }
}

void Ville::affiche_pixel_terrain(int x, int y, uint8_t c)
{
    //if(x>=0 && y>=0 && x<bitmap_terrain->largeur && y<bitmap_terrain->hauteur)
       *(uint32_t*)(bitmap_terrain->bitmap+(x+y*bitmap_terrain->largeur)*4)=couleurs_iters[c];
}


// Fonction importante. Elle permet une première interprétation du code fourni par l'algo de base
void Ville::assigne_element(int x, int y, uint8_t c)
{
    uint8_t nouveau_c=c;
    if(c==4 || c==5 || c==6 || c==8 || c==3 || c==2 || c==1) nouveau_c=VILLE_ELEMENT_EAU;
    else if (c==terrain_iterMax || c==7 || c==10 || c==11 || c==12 ) nouveau_c=VILLE_ELEMENT_ROUTE;
    else if (c>0 && c<4) nouveau_c+=VILLE_ELEMENT_BATIMENT0-1;
    else if (c>7)nouveau_c-=(VILLE_ELEMENT_BATIMENT0-1);

    if(drapeau_affiche_bitmap_terrain) affiche_pixel_terrain(x,y,nouveau_c);
    map_terrain_temp[x+y*taille_terrain]=nouveau_c;
}


void Ville::init_palette_terrain()
{
    uint32_t coul=0xff000000;
    for(uint8_t i=0;i<terrain_iterMax;i++)
    {
        coul=coul+0x122517;
        coul=coul | 0xff000000;
        couleurs_iters[i]=coul;
    }

    couleurs_iters[VILLE_ELEMENT_RIEN]=0xffffffff;
    couleurs_iters[VILLE_ELEMENT_EAU]=0xffff0000;
    couleurs_iters[VILLE_ELEMENT_ROUTE]=0xff404040;
    couleurs_iters[VILLE_ELEMENT_ESPACE_VERT]=0xff00ff00;
}

//------------- Passe 1: Génère la texture de base:
void Ville::rempli_champ_vision_mandelbox(int ymin,int ymax,float p_x0,float p_x1,float d1,float d2)
{

    int mod1=32;
    int mod2=6;
    float x0=p_x0;
    float x1=p_x1;
    int xoffset,yoffset;
    uint8_t iter;
    uint32_t i;
    float  Cr,Ci,R;
    bool pOut;

    float Vr,Vi,tmp;

        for(int y=ymin;y<=ymax;y++)
        {
            yoffset=int(roundf(float(y)+obs.z/partSize))+taille_terrain/2;

            for(int x=int(x0);x<int(x1);x++)
            {
                Cr=float(x)/terrain_zoom;
                Ci=float(y)/terrain_zoom;
                pOut=false;

                Vr=0.;
                Vi=0.;
                iter=0;
                while((Vr*Vr+Vi*Vi)<7. && iter<terrain_iterMax)
                {
                        if(Vr>1.) Vr=2-Vr;
                        else if(Vr<-1) Vr=-2-Vr;
                        if(Vi>1.) Vi=2-Vi;
                        else if(Vi<-1) Vi=-2-Vi;
                        R=sqrtf(Vr*Vr+Vi*Vi);
                        if(R<terrain_r)
                        {
                            Vr/=sqrtf(terrain_r);
                            Vi/=sqrtf(terrain_r);
                        }
                        else if(R<1.)
                        {
                            Vr/=sqrtf(R);
                            Vi/=sqrtf(R);
                        }

                        Vr=Vr*terrain_s+Cr;
                        Vi=Vi*terrain_s+Ci;

                        iter++;


                }

                xoffset=int(roundf(float(x)-obs.x/partSize))+taille_terrain/2;

                assigne_element(xoffset,yoffset,iter);  //Le code donné par l'algo de base est ajusté pour correspondre à un élément de la ville.
            }
            x0+=d1;
            x1+=d2;
        }

        if(drapeau_affiche_trame)
        {
            genere_grille(ymin,ymax,p_x0,p_x1,d1,d2);
        }
}

//------------- Passe 2 : Création des espaces verts
void Ville::creation_espaces_verts(int ymin,int ymax,float p_x0,float p_x1,float d1,float d2)
{
     int xoffset,yoffset;
	float x0=p_x0;
    float x1=p_x1;
    uint8_t objet_id;
    uint8_t code;
        for(int y=ymin;y<=ymax;y++)
        {
            yoffset=int(roundf(float(y)+obs.z/partSize))+taille_terrain/2;
            for(int x=int(x0);x<int(x1);x++)
            {
                xoffset=int(roundf(float(x)-obs.x/partSize))+taille_terrain/2;
                objet_id=*(map_terrain_temp+xoffset+taille_terrain*yoffset);
                if(objet_id==VILLE_ELEMENT_ROUTE)
                {
                    code=analyse_alentours_8bits(xoffset,yoffset,map_terrain_temp);
                    if(code==0 || code==0xff)
                    {
                        if(drapeau_affiche_bitmap_terrain) affiche_pixel_terrain(xoffset,yoffset,VILLE_ELEMENT_ESPACE_VERT);
                        map_terrain[xoffset+yoffset*taille_terrain]=VILLE_ELEMENT_ESPACE_VERT;
                    }

                    else map_terrain[xoffset+yoffset*taille_terrain]=VILLE_ELEMENT_ROUTE;

                }
                else map_terrain[xoffset+yoffset*taille_terrain]=objet_id;
            }
            x0+=d1;
            x1+=d2;
        }
}
//-------------Passe 3: Organise les routes:

void Ville::organise_routes(int ymin,int ymax,float p_x0,float p_x1,float d1,float d2)
{
    int xoffset,yoffset;
	float x0=p_x0;
    float x1=p_x1;
    uint8_t objet_id;
    uint8_t code;
        for(int y=ymin;y<=ymax;y++)
        {
            yoffset=int(roundf(float(y)+obs.z/partSize))+taille_terrain/2;
            for(int x=int(x0);x<int(x1);x++)
            {
                xoffset=int(roundf(float(x)-obs.x/partSize))+taille_terrain/2;
                objet_id=*(map_terrain+xoffset+taille_terrain*yoffset);
                if(objet_id==VILLE_ELEMENT_ROUTE)
                {
                    code=analyse_alentours_4bits(xoffset,yoffset,map_terrain);
                    if(code==0xF) //Espace vert
                    {
                        *(map_routes+xoffset+taille_terrain*yoffset)=0xffff;
                        *(map_terrain+xoffset+taille_terrain*yoffset)=VILLE_ELEMENT_ESPACE_VERT;
                        if(drapeau_affiche_bitmap_terrain) affiche_pixel_terrain(xoffset,yoffset,VILLE_ELEMENT_ESPACE_VERT);
                    }
                    else *(map_routes+xoffset+taille_terrain*yoffset)=code;
                }
                else *(map_routes+xoffset+taille_terrain*yoffset)=0xffff;   //Code pour un élément autre qu'une route
            }
            x0+=d1;
            x1+=d2;
        }
}

//-------------Passe 4: Optimise les routes, les buildings et créé la liste des objets à afficher
void Ville::optimise_terrain(int ymin,int ymax,float p_x0,float p_x1,float d1,float d2)
{
    float xElem,yElem,zElem,elemScaleX,elemScaleY,elemScaleZ;
    int nombre_etages;
    int xoffset,yoffset; // Position dans le bitmap du terrain (centré sur l'observateur)
	float x0=p_x0;
    float x1=p_x1;
    uint8_t objet_id,code;

        for(int y=ymin;y<=ymax;y++)
        {
            yoffset=int(roundf(float(y)+obs.z/partSize))+taille_terrain/2;
            zElem=float(y)*partSize;
            for(int x=int(x0);x<int(x1);x++)
            {
                xoffset=int(roundf(float(x)-obs.x/partSize))+taille_terrain/2;
                xElem=float(x)*partSize;
                objet_id=*(map_terrain+xoffset+taille_terrain*yoffset);
                if(objet_id==VILLE_ELEMENT_ROUTE)
                {
                    yElem=VILLE_ALTITUDE_ROUTES;
                    glPushMatrix();
                        elemScaleX=1.;
                        elemScaleY=1.;
                        elemScaleZ=1.;
                        glTranslatef(xElem,yElem,zElem);
                        code=optimise_element_route(xoffset,yoffset);
                        //code=*(map_routes+xoffset+taille_terrain*yoffset);
                        glRotatef(tab_routes[code].angleRoute,0.,1.,0.);
                        glScalef(elemScaleX,elemScaleY,elemScaleZ);
                        sauve_objet(tab_routes[code].route_id,xoffset,yoffset,x,y,objet_id);
                    glPopMatrix();
                }
                else if(objet_id==VILLE_ELEMENT_ESPACE_VERT)
                {
                    yElem=VILLE_ALTITUDE_ROUTES;
                    glPushMatrix();
                        elemScaleX=1.;
                        elemScaleY=1.;
                        elemScaleZ=1.;
                        glTranslatef(xElem,yElem,zElem);
                        //glRotatef(tab_routes[code].angleRoute,0.,1.,0.);
                        glScalef(elemScaleX,elemScaleY,elemScaleZ);
                        sauve_objet(route_01_espace_vert_liste_id,xoffset,yoffset,x,y,objet_id);
                    glPopMatrix();
                }

                else if(objet_id>=VILLE_ELEMENT_BATIMENT0)
                {

                        elemScaleX=partSize;
                        elemScaleZ=partSize;
                        GLuint liste_id;

                        if(objet_id==VILLE_ELEMENT_BATIMENT0+5)
                        {
                            nombre_etages=int(2+2+2*cos(zElem/33.)*cos(xElem/15.));
                            elemScaleY=VILLE_BATIMENT_HAUTEUR_ETAGE*nombre_etages;
                            yElem=VILLE_ALTITUDE_ROUTES;
                            liste_id=immeuble_01_liste_id;
                            glPushMatrix();
                            glTranslatef(xElem,yElem,zElem);
                            glScalef(elemScaleX,elemScaleY,elemScaleZ);
                            sauve_objet(liste_id,xoffset,yoffset,x,y,objet_id,nombre_etages);
                            glPopMatrix();
                        }


                        else if(objet_id==VILLE_ELEMENT_BATIMENT0+2)
                        {
                            code=analyse_alentours_8bits(xoffset,yoffset,map_terrain);

                            //Grande structure:
                            if(code==0x00)
                            {
                                nombre_etages=int(20+140+140*cos(zElem/1.1)*cos(xElem/3.5));
                                elemScaleY=VILLE_BATIMENT_HAUTEUR_ETAGE*nombre_etages;
                                yElem=VILLE_ALTITUDE_ROUTES;
                                liste_id=immeuble_04_liste_id;
                                glPushMatrix();
                                glTranslatef(xElem,yElem,zElem);
                                elemScaleX*=1.+0.01*cos(zElem/13.352)*cos(xElem/3.5);
                                elemScaleZ*=1.+0.01*cos(zElem/3.52)*cos(xElem/23.5);
                                glScalef(elemScaleX,elemScaleY,elemScaleZ);
                                sauve_objet(liste_id,xoffset,yoffset,x,y,objet_id,nombre_etages);
                                glPopMatrix();
                            }
                            else
                            {
                                //*(map_terrain+xoffset+yoffset*taille_terrain)=VILLE_ELEMENT_EAU;


                                yElem=VILLE_ALTITUDE_ROUTES;
                                nombre_etages=int(1.+20+20*cos(zElem/20.)*cos(xElem/30.));
                                //elemScaleY=1.+20+20*cos(zElem/20.)*cos(xElem/30.);
                                elemScaleY=VILLE_BATIMENT_HAUTEUR_ETAGE*nombre_etages;
                                glPushMatrix();
                                    elemScaleX=partSize;
                                    elemScaleZ=partSize;
                                    glTranslatef(xElem,yElem,zElem);
                                    glScalef(elemScaleX,elemScaleY,elemScaleZ);
                                    sauve_objet(immeuble_02_liste_id,xoffset,yoffset,x,y,objet_id,nombre_etages);
                                glPopMatrix();



                            }
                        }
                        else if(objet_id==VILLE_ELEMENT_BATIMENT0+6)
                        {
                            nombre_etages=int(1+5+5*cos(zElem/22.)*cos(xElem/37.));
                            elemScaleY=VILLE_BATIMENT_HAUTEUR_ETAGE*nombre_etages;
                            yElem=VILLE_ALTITUDE_ROUTES;
                            liste_id=immeuble_03_liste_id;
                            glPushMatrix();
                            glTranslatef(xElem,yElem,zElem);
                            glScalef(elemScaleX,elemScaleY,elemScaleZ);
                            sauve_objet(liste_id,xoffset,yoffset,x,y,objet_id,nombre_etages);
                            glPopMatrix();
                        }
                        else if(objet_id==VILLE_ELEMENT_BATIMENT0+7)
                        {
                            nombre_etages=int(10+10+10*cos(zElem/127.)*cos(xElem/6.));
                            elemScaleY=VILLE_BATIMENT_HAUTEUR_ETAGE*nombre_etages;
                            yElem=VILLE_ALTITUDE_ROUTES;
                            liste_id=immeuble_01_liste_id;
                            glPushMatrix();
                            glTranslatef(xElem,yElem,zElem);
                            glScalef(elemScaleX,elemScaleY,elemScaleZ);
                            sauve_objet(liste_id,xoffset,yoffset,x,y,objet_id,nombre_etages);
                            glPopMatrix();
                        }
                        else if(objet_id==VILLE_ELEMENT_BATIMENT0+8)
                        {
                            nombre_etages=int(2+2+2*cos(zElem/3.)*cos(xElem/9.));
                            elemScaleY=VILLE_BATIMENT_HAUTEUR_ETAGE*nombre_etages;
                            yElem=VILLE_ALTITUDE_ROUTES;
                            liste_id=immeuble_02_liste_id;
                            glPushMatrix();
                            glTranslatef(xElem,yElem,zElem);
                            glScalef(elemScaleX,elemScaleY,elemScaleZ);
                            sauve_objet(liste_id,xoffset,yoffset,x,y,objet_id,nombre_etages);
                            glPopMatrix();
                        }
                        else if(objet_id==VILLE_ELEMENT_BATIMENT0+10)
                        {
                            nombre_etages=int(8+5+5*cos(zElem/60.)*cos(xElem/150.));
                            elemScaleY=VILLE_BATIMENT_HAUTEUR_ETAGE*nombre_etages;
                            yElem=VILLE_ALTITUDE_ROUTES;
                            liste_id=immeuble_01_liste_id;
                            glPushMatrix();
                            glTranslatef(xElem,yElem,zElem);
                            glScalef(elemScaleX,elemScaleY,elemScaleZ);
                            sauve_objet(liste_id,xoffset,yoffset,x,y,objet_id,nombre_etages);
                            glPopMatrix();
                        }


                        else if(objet_id==VILLE_ELEMENT_BATIMENT0+15)
                        {
                            nombre_etages=int(20+7+7*cos(zElem/30.)*cos(xElem/25.));
                            elemScaleY=VILLE_BATIMENT_HAUTEUR_ETAGE*nombre_etages;
                            yElem=VILLE_ALTITUDE_ROUTES;
                            liste_id=immeuble_03_liste_id;
                            glPushMatrix();
                            glTranslatef(xElem,yElem,zElem);
                            glScalef(elemScaleX,elemScaleY,elemScaleZ);
                            sauve_objet(liste_id,xoffset,yoffset,x,y,objet_id,nombre_etages);
                            glPopMatrix();
                        }

                        else if(objet_id==VILLE_ELEMENT_BATIMENT0+16)
                        {
                            nombre_etages=int(10+50+50*cos(zElem/111.)*cos(xElem/80.));
                            elemScaleY=VILLE_BATIMENT_HAUTEUR_ETAGE*nombre_etages;
                            yElem=VILLE_ALTITUDE_ROUTES;
                            liste_id=immeuble_02_liste_id;
                            glPushMatrix();
                            glTranslatef(xElem,yElem,zElem);
                            glScalef(elemScaleX,elemScaleY,elemScaleZ);
                            sauve_objet(liste_id,xoffset,yoffset,x,y,objet_id,nombre_etages);
                            glPopMatrix();
                        }

                        else
                        {
                            nombre_etages=int(1+2.5+2.5*cos(zElem/75.)*cos(xElem/100.));
                            elemScaleY=VILLE_BATIMENT_HAUTEUR_ETAGE*nombre_etages;
                            yElem=VILLE_ALTITUDE_ROUTES;
                            liste_id=immeuble_01_liste_id;
                            glPushMatrix();
                            glTranslatef(xElem,yElem,zElem);
                            glScalef(elemScaleX,elemScaleY,elemScaleZ);
                            sauve_objet(liste_id,xoffset,yoffset,x,y,objet_id,nombre_etages);
                            glPopMatrix();

                        }



                }


                if(objet_id!=VILLE_ELEMENT_EAU)genere_bordures(xoffset,yoffset,x,y);

            }
            x0+=d1;
            x1+=d2;
        }
}

void Ville::genere_bordures(int xoffset,int yoffset,int x,int y)
{
    uint8_t code=analyse_alentours_eau(xoffset,yoffset);
    float xElem=float(x)*partSize;
    float yElem=float(y)*partSize;
    switch(code)
    {
        case 0:return;break;
        case 1:
                glPushMatrix();
                glTranslatef(xElem,0.,yElem);
                glRotatef(180.,0.,1.,0.);
                sauve_objet(bordure_eau_base_liste_id,xoffset,yoffset,x,y,0);
                glPopMatrix();
                break;
        case 2:
                glPushMatrix();
                glTranslatef(xElem,0.,yElem);
                glRotatef(90.,0.,1.,0.);
                sauve_objet(bordure_eau_base_liste_id,xoffset,yoffset,x,y,0);
                glPopMatrix();
                break;
        case 3:
                glPushMatrix();
                glTranslatef(xElem,0.,yElem);
                glRotatef(90.,0.,1.,0.);
                sauve_objet(bordure_eau_base_liste_id,xoffset,yoffset,x,y,0);
                glPopMatrix();

                glPushMatrix();
                glTranslatef(xElem,0.,yElem);
                glRotatef(180.,0.,1.,0.);
                sauve_objet(bordure_eau_base_liste_id,xoffset,yoffset,x,y,0);
                glPopMatrix();
                break;
        case 4:
                glPushMatrix();
                //glRotatef(90.,0.,1.,0.);
                glTranslatef(xElem,0.,yElem);
                sauve_objet(bordure_eau_base_liste_id,xoffset,yoffset,x,y,0);
                glPopMatrix();
                break;
        case 5:
                glPushMatrix();
                //glRotatef(90.,0.,1.,0.);
                glTranslatef(xElem,0.,yElem);
                sauve_objet(bordure_eau_base_liste_id,xoffset,yoffset,x,y,0);
                glPopMatrix();

                glPushMatrix();
                glTranslatef(xElem,0.,yElem);
                glRotatef(180.,0.,1.,0.);
                sauve_objet(bordure_eau_base_liste_id,xoffset,yoffset,x,y,0);
                glPopMatrix();
                break;
        case 6:
                glPushMatrix();
                //glRotatef(90.,0.,1.,0.);
                glTranslatef(xElem,0.,yElem);
                sauve_objet(bordure_eau_base_liste_id,xoffset,yoffset,x,y,0);
                glPopMatrix();

                glPushMatrix();
                glTranslatef(xElem,0.,yElem);
                glRotatef(90.,0.,1.,0.);
                sauve_objet(bordure_eau_base_liste_id,xoffset,yoffset,x,y,0);
                glPopMatrix();
                break;
        case 7:
                glPushMatrix();
                glTranslatef(xElem,0.,yElem);
                sauve_objet(bordure_eau_base_liste_id,xoffset,yoffset,x,y,0);
                glPopMatrix();

                glPushMatrix();
                glTranslatef(xElem,0.,yElem);
                glRotatef(90.,0.,1.,0.);
                sauve_objet(bordure_eau_base_liste_id,xoffset,yoffset,x,y,0);
                glPopMatrix();

                glPushMatrix();
                glTranslatef(xElem,0.,yElem);
                glRotatef(180.,0.,1.,0.);
                sauve_objet(bordure_eau_base_liste_id,xoffset,yoffset,x,y,0);
                glPopMatrix();
                break;
        case 8:
                glPushMatrix();
                glTranslatef(xElem,0.,yElem);
                glRotatef(-90.,0.,1.,0.);
                sauve_objet(bordure_eau_base_liste_id,xoffset,yoffset,x,y,0);
                glPopMatrix();
                break;
        case 9:
                glPushMatrix();
                glTranslatef(xElem,0.,yElem);
                glRotatef(-90.,0.,1.,0.);
                sauve_objet(bordure_eau_base_liste_id,xoffset,yoffset,x,y,0);
                glPopMatrix();

                glPushMatrix();
                glTranslatef(xElem,0.,yElem);
                glRotatef(180.,0.,1.,0.);
                sauve_objet(bordure_eau_base_liste_id,xoffset,yoffset,x,y,0);
                glPopMatrix();
                break;
        case 10:
                glPushMatrix();
                glTranslatef(xElem,0.,yElem);
                glRotatef(90.,0.,1.,0.);
                sauve_objet(bordure_eau_base_liste_id,xoffset,yoffset,x,y,0);
                glPushMatrix();

                glPopMatrix();
                glTranslatef(xElem,0.,yElem);
                glRotatef(-90.,0.,1.,0.);
                sauve_objet(bordure_eau_base_liste_id,xoffset,yoffset,x,y,0);
                glPopMatrix();
                break;
        case 11:
                glPushMatrix();
                glTranslatef(xElem,0.,yElem);
                glRotatef(90.,0.,1.,0.);
                sauve_objet(bordure_eau_base_liste_id,xoffset,yoffset,x,y,0);
                glPopMatrix();

                glPushMatrix();
                glTranslatef(xElem,0.,yElem);
                glRotatef(180.,0.,1.,0.);
                sauve_objet(bordure_eau_base_liste_id,xoffset,yoffset,x,y,0);
                glPopMatrix();

                glPushMatrix();
                glTranslatef(xElem,0.,yElem);
                glRotatef(-90.,0.,1.,0.);
                sauve_objet(bordure_eau_base_liste_id,xoffset,yoffset,x,y,0);
                glPopMatrix();
                break;
        case 12:
                glPushMatrix();
                glTranslatef(xElem,0.,yElem);
                sauve_objet(bordure_eau_base_liste_id,xoffset,yoffset,x,y,0);
                glPopMatrix();

                glPushMatrix();
                glTranslatef(xElem,0.,yElem);
                glRotatef(-90.,0.,1.,0.);
                sauve_objet(bordure_eau_base_liste_id,xoffset,yoffset,x,y,0);
                glPopMatrix();
                break;
        case 13:
                glPushMatrix();
                glTranslatef(xElem,0.,yElem);
                sauve_objet(bordure_eau_base_liste_id,xoffset,yoffset,x,y,0);
                glPopMatrix();

                glPushMatrix();
                glTranslatef(xElem,0.,yElem);
                glRotatef(-90.,0.,1.,0.);
                sauve_objet(bordure_eau_base_liste_id,xoffset,yoffset,x,y,0);
                glPopMatrix();

                glPushMatrix();
                glTranslatef(xElem,0.,yElem);
                glRotatef(180.,0.,1.,0.);
                sauve_objet(bordure_eau_base_liste_id,xoffset,yoffset,x,y,0);
                glPopMatrix();
                break;
        case 14:
                glPushMatrix();
                glTranslatef(xElem,0.,yElem);
                sauve_objet(bordure_eau_base_liste_id,xoffset,yoffset,x,y,0);
                glPopMatrix();

                glPushMatrix();
                glTranslatef(xElem,0.,yElem);
                glRotatef(90.,0.,1.,0.);
                sauve_objet(bordure_eau_base_liste_id,xoffset,yoffset,x,y,0);
                glPopMatrix();

                glPushMatrix();
                glTranslatef(xElem,0.,yElem);
                glRotatef(-90.,0.,1.,0.);
                sauve_objet(bordure_eau_base_liste_id,xoffset,yoffset,x,y,0);
                glPopMatrix();
                break;
        case 15:
                glPushMatrix();
                glTranslatef(xElem,0.,yElem);
                sauve_objet(bordure_eau_base_liste_id,xoffset,yoffset,x,y,0);
                glPopMatrix();

                glPushMatrix();
                glTranslatef(xElem,0.,yElem);
                glRotatef(90.,0.,1.,0.);
                sauve_objet(bordure_eau_base_liste_id,xoffset,yoffset,x,y,0);
                glPopMatrix();

                glPushMatrix();
                glTranslatef(xElem,0.,yElem);
                glRotatef(90.,0.,1.,0.);
                sauve_objet(bordure_eau_base_liste_id,xoffset,yoffset,x,y,0);
                glPopMatrix();

                glPushMatrix();
                glTranslatef(xElem,0.,yElem);
                glRotatef(90.,0.,1.,0.);
                sauve_objet(bordure_eau_base_liste_id,xoffset,yoffset,x,y,0);
                glPopMatrix();

                break;

        default:break;
    }
}

void Ville::genere_ville()
{
    vec2f direction;
    GFunc::rotation_plan(obs.l,0.,-1.,direction.x,direction.y);
    if(drapeau_affiche_bitmap_terrain)efface_bitmap_terrain();
    efface_map_terrain();
    objet_ptr=0;
    objet_grille_ptr=0;

    //Dimension d'un élément de la trame:

    //Distance max des objets à afficher (en unité d'espace)


    //Calcul du triangle O,A,B du champ de vision:
    float distAB=distanceMax*float(GVars::largeur_ecran)/(GVars::distanceFocale);
    vec3f VUab;
    VUab.x=-direction.y;    //Produit vectoriel
    VUab.y=direction.x;
    direction.x*=distanceMax;
    direction.y*=distanceMax;
    VUab.x*=distAB/2.;
    VUab.y*=distAB/2.;

    triangle_visible[0].x=obs.x;
    triangle_visible[0].y=-obs.z;
    triangle_visible[1].x=obs.x+direction.x-VUab.x;
    triangle_visible[1].y=-obs.z+direction.y-VUab.y;
    triangle_visible[2].x=obs.x+direction.x+VUab.x;
    triangle_visible[2].y=-obs.z+direction.y+VUab.y;

    //Agrandissement:

    float cx=(triangle_visible[0].x+triangle_visible[1].x+triangle_visible[2].x)/3;
    float cy=(triangle_visible[0].y+triangle_visible[1].y+triangle_visible[2].y)/3;
    triangle_visible[0].x=(triangle_visible[0].x-cx)*marge_terrain+cx;
    triangle_visible[0].y=(triangle_visible[0].y-cy)*marge_terrain+cy;
    triangle_visible[1].x=(triangle_visible[1].x-cx)*marge_terrain+cx;
    triangle_visible[1].y=(triangle_visible[1].y-cy)*marge_terrain+cy;
    triangle_visible[2].x=(triangle_visible[2].x-cx)*marge_terrain+cx;
    triangle_visible[2].y=(triangle_visible[2].y-cy)*marge_terrain+cy;



	//Affiche les dalles unitaires:
	//J'utilise la bonne vieille méthode de remplissage de triangle en 2d :)
        //Calcul les bornes du triangle du champ de vision:

        triangle_visible_int[0]=int(roundf(triangle_visible[0].x/partSize));
        triangle_visible_int[1]=int(roundf(triangle_visible[0].y/partSize));
        triangle_visible_int[2]=int(roundf(triangle_visible[1].x/partSize));
        triangle_visible_int[3]=int(roundf(triangle_visible[1].y/partSize));
        triangle_visible_int[4]=int(roundf(triangle_visible[2].x/partSize));
        triangle_visible_int[5]=int(roundf(triangle_visible[2].y/partSize));

        int indiceA=0;
        int ymin=triangle_visible_int[1];
        int ymax=ymin;
        int ytot;
        if(triangle_visible_int[3]<ymin){ymin=triangle_visible_int[3];indiceA=1;}
        if(triangle_visible_int[5]<ymin){ymin=triangle_visible_int[5];indiceA=2;}
        if(triangle_visible_int[3]>ymax){ymax=triangle_visible_int[3];}
        if(triangle_visible_int[5]>ymax){ymax=triangle_visible_int[5];}
        ytot=ymax-ymin;
        float dAB,dBC,dAC;
        int dxAB,dyAB,dxBC,dyBC,dxAC,dyAC;

        int indiceB=(indiceA+1)%3;
        int indiceC=(indiceA+2)%3;
        if(triangle_visible_int[indiceA*2+1]==triangle_visible_int[indiceC*2+1])
        {
            indiceA=indiceC;
            indiceB=(indiceA+1)%3;
            indiceC=(indiceA+2)%3;
        }

        dxAB=triangle_visible_int[indiceB*2]-triangle_visible_int[indiceA*2];
        dyAB=triangle_visible_int[indiceB*2+1]-triangle_visible_int[indiceA*2+1];
        dxBC=triangle_visible_int[indiceC*2]-triangle_visible_int[indiceB*2];
        dyBC=triangle_visible_int[indiceC*2+1]-triangle_visible_int[indiceB*2+1];
        dxAC=triangle_visible_int[indiceC*2]-triangle_visible_int[indiceA*2];
        dyAC=triangle_visible_int[indiceC*2+1]-triangle_visible_int[indiceA*2+1];

        if(dyAB==0)dAB=0;else dAB=float(dxAB)/float(dyAB);
        if(dyBC==0)dBC=0;else dBC=float(dxBC)/float(dyBC);
        if(dyAC==0)dAC=0;else dAC=float(dxAC)/float(dyAC);

        float x0,x1;
        int x,y;
        //Détermine la configuration (4 possibles)
        //Pour chaque cas:
                //Passe 1: Remplissage des cases, map de base avec un premier réajustement des codes (ex: plusieurs codes peuvent correspondre à l'eau)
                //Passe 2: Organisation des routes

            //Cas 1:
            //           A*******B
            //            *****
            //             C

            if(dyAB==0)
            {
                //Passe 1:
                rempli_champ_vision_mandelbox(ymin,ymax,float(triangle_visible_int[indiceA*2]),float(triangle_visible_int[indiceB*2]),dAC,dBC);
                //Passe 2:
                creation_espaces_verts(ymin,ymax,float(triangle_visible_int[indiceA*2]),float(triangle_visible_int[indiceB*2]),dAC,dBC);
                //Passe 3:
                organise_routes(ymin,ymax,float(triangle_visible_int[indiceA*2]),float(triangle_visible_int[indiceB*2]),dAC,dBC);
                //Passe 4:
                optimise_terrain(ymin,ymax,float(triangle_visible_int[indiceA*2]),float(triangle_visible_int[indiceB*2]),dAC,dBC);
            }

            //Cas 2:
            //             A
            //            *****
            //           C*******B
            else if(dyBC==0)
            {
                //Passe 1:
                rempli_champ_vision_mandelbox(ymin,ymax,float(triangle_visible_int[indiceA*2]),float(triangle_visible_int[indiceA*2]),dAC,dAB);
                //Passe 2:
                creation_espaces_verts(ymin,ymax,float(triangle_visible_int[indiceA*2]),float(triangle_visible_int[indiceA*2]),dAC,dAB);
                //Passe 3:
                organise_routes(ymin,ymax,float(triangle_visible_int[indiceA*2]),float(triangle_visible_int[indiceA*2]),dAC,dAB);
                //Passe 4:
                optimise_terrain(ymin,ymax,float(triangle_visible_int[indiceA*2]),float(triangle_visible_int[indiceA*2]),dAC,dAB);
            }

            //Cas 3:
            //             A
            //           ***
            //          C***
            //           ****
            //             *B

            else if(dyAB>dyAC)
            {
                //Passe 1:
                rempli_champ_vision_mandelbox(ymin,triangle_visible_int[indiceC*2+1]-1,
                                            float(triangle_visible_int[indiceA*2]),
                                            float(triangle_visible_int[indiceA*2]),
                                            dAC,dAB);
                rempli_champ_vision_mandelbox(triangle_visible_int[indiceC*2+1],ymax,
                                           float(triangle_visible_int[indiceC*2]),
                                           float(triangle_visible_int[indiceA*2])+dAB*(float(triangle_visible_int[indiceC*2+1])-float(ymin)),
                                           dBC,dAB);
                //Passe 2:
                creation_espaces_verts(ymin,triangle_visible_int[indiceC*2+1]-1,
                                            float(triangle_visible_int[indiceA*2]),
                                            float(triangle_visible_int[indiceA*2]),
                                            dAC,dAB);
                creation_espaces_verts(triangle_visible_int[indiceC*2+1],ymax,
                                           float(triangle_visible_int[indiceC*2]),
                                           float(triangle_visible_int[indiceA*2])+dAB*(float(triangle_visible_int[indiceC*2+1])-float(ymin)),
                                           dBC,dAB);

                //Passe 3:
                organise_routes(ymin,triangle_visible_int[indiceC*2+1]-1,
                                            float(triangle_visible_int[indiceA*2]),
                                            float(triangle_visible_int[indiceA*2]),
                                            dAC,dAB);
                organise_routes(triangle_visible_int[indiceC*2+1],ymax,
                                           float(triangle_visible_int[indiceC*2]),
                                           float(triangle_visible_int[indiceA*2])+dAB*(float(triangle_visible_int[indiceC*2+1])-float(ymin)),
                                           dBC,dAB);
                //Passe 4:
                optimise_terrain(ymin,triangle_visible_int[indiceC*2+1]-1,
                                            float(triangle_visible_int[indiceA*2]),
                                            float(triangle_visible_int[indiceA*2]),
                                            dAC,dAB);
                optimise_terrain(triangle_visible_int[indiceC*2+1],ymax,
                                           float(triangle_visible_int[indiceC*2]),
                                           float(triangle_visible_int[indiceA*2])+dAB*(float(triangle_visible_int[indiceC*2+1])-float(ymin)),
                                           dBC,dAB);
            }
            //Cas 4:
            //             A
            //             ***
            //             ***B
            //            ****
            //            C*
            else //if(dyAB<dyAC)
            {

                //Passe 1:
                rempli_champ_vision_mandelbox(ymin,triangle_visible_int[indiceB*2+1]-1,
                                           float(triangle_visible_int[indiceA*2]),
                                           float(triangle_visible_int[indiceA*2]),
                                           dAC,dAB);
                rempli_champ_vision_mandelbox(triangle_visible_int[indiceB*2+1],ymax,
                                           float(triangle_visible_int[indiceA*2])+dAC*(float(triangle_visible_int[indiceB*2+1])-float(ymin)),
                                           float(triangle_visible_int[indiceB*2]),
                                           dAC,dBC);
                //Passe 2:
                creation_espaces_verts(ymin,triangle_visible_int[indiceB*2+1]-1,
                                           float(triangle_visible_int[indiceA*2]),
                                           float(triangle_visible_int[indiceA*2]),
                                           dAC,dAB);
                creation_espaces_verts(triangle_visible_int[indiceB*2+1],ymax,
                                           float(triangle_visible_int[indiceA*2])+dAC*(float(triangle_visible_int[indiceB*2+1])-float(ymin)),
                                           float(triangle_visible_int[indiceB*2]),
                                           dAC,dBC);
                //Passe 3:
                organise_routes(ymin,triangle_visible_int[indiceB*2+1]-1,
                                           float(triangle_visible_int[indiceA*2]),
                                           float(triangle_visible_int[indiceA*2]),
                                           dAC,dAB);
                organise_routes(triangle_visible_int[indiceB*2+1],ymax,
                                           float(triangle_visible_int[indiceA*2])+dAC*(float(triangle_visible_int[indiceB*2+1])-float(ymin)),
                                           float(triangle_visible_int[indiceB*2]),
                                           dAC,dBC);
                //Passe 4:
                optimise_terrain(ymin,triangle_visible_int[indiceB*2+1]-1,
                                           float(triangle_visible_int[indiceA*2]),
                                           float(triangle_visible_int[indiceA*2]),
                                           dAC,dAB);
                optimise_terrain(triangle_visible_int[indiceB*2+1],ymax,
                                           float(triangle_visible_int[indiceA*2])+dAC*(float(triangle_visible_int[indiceB*2+1])-float(ymin)),
                                           float(triangle_visible_int[indiceB*2]),
                                           dAC,dBC);
            }


            if(drapeau_affiche_bitmap_terrain)bitmap_terrain->maj_texture();
}

void Ville::sauve_objet(GLuint liste_id,int posX,int posY, int x, int y, uint8_t couleur, int nombre_etages)
{
    if(objet_ptr<VILLE_NBR_OBJETS_MAX)
    {
        objets[objet_ptr].liste_id=liste_id;
        glGetFloatv(GL_MODELVIEW_MATRIX,objets[objet_ptr].matrice);
        objets[objet_ptr].couleur=couleur;
        objets[objet_ptr].posX=posX;
        objets[objet_ptr].posY=posY;
        objets[objet_ptr].x=x;
        objets[objet_ptr].y=y;
        objets[objet_ptr].nombre_etages=nombre_etages;
        objet_ptr++;
    }

}

void Ville::sauve_objet_grille(GLuint liste_id)
{
    if(objet_grille_ptr<VILLE_NBR_OBJETS_MAX)
    {
        grille[objet_grille_ptr].liste_id=liste_id;
        glGetFloatv(GL_MODELVIEW_MATRIX,grille[objet_grille_ptr].matrice);
        objet_grille_ptr++;
    }

}

uint8_t Ville::analyse_alentours_4bits(int x, int y,uint8_t* terrain)
{
    int offset=(x+taille_terrain*y);
    uint8_t c=*(terrain+offset);
    uint8_t code=0x0000;
    //4 bits:
    if(*(terrain+offset+(-taille_terrain))!=c)      code|=0x01;
    if(*(terrain+offset+(1))!=c)                    code|=0x02;
    if(*(terrain+offset+(taille_terrain))!=c)       code|=0x04;
    if(*(terrain+offset+(-1))!=c)                   code|=0x08;
    return code;
}

uint8_t Ville::analyse_alentours_8bits(int x, int y,uint8_t* terrain)
{
    int offset=(x+taille_terrain*y);
    uint8_t c=*(terrain+offset);
    uint8_t code=0x0000;
    //8 bits:
    if(*(terrain+offset+(-taille_terrain-1))!=c)    code|=0x01;
    if(*(terrain+offset+(-taille_terrain))!=c)      code|=0x02;
    if(*(terrain+offset+(1-taille_terrain))!=c)     code|=0x04;
    if(*(terrain+offset+(1))!=c)                    code|=0x08;
    if(*(terrain+offset+(taille_terrain+1))!=c)     code|=0x10;
    if(*(terrain+offset+(taille_terrain))!=c)       code|=0x20;
    if(*(terrain+offset+(taille_terrain-1))!=c)     code|=0x40;
    if(*(terrain+offset+(-1))!=c)                   code|=0x80;
    return code;
}

//Renvoie 1 là ou les éléments alentours sont de l'eau
uint8_t Ville::analyse_alentours_eau(int x, int y)
{
    int offset=(x+taille_terrain*y);
    uint8_t code=0x0000;
    //4 bits:
    if(*(map_terrain+offset+(-taille_terrain))==VILLE_ELEMENT_EAU)      code|=0x01;
    if(*(map_terrain+offset+(1))==VILLE_ELEMENT_EAU)                    code|=0x02;
    if(*(map_terrain+offset+(taille_terrain))==VILLE_ELEMENT_EAU)       code|=0x04;
    if(*(map_terrain+offset+(-1))==VILLE_ELEMENT_EAU)                   code|=0x08;
    return code;
}


uint8_t Ville::optimise_element_route(int x, int y)
{
    int offset=(x+taille_terrain*y);

    uint8_t c=*(map_routes+offset);
    uint8_t code=c;
    uint8_t e1=*(map_routes+offset+(-taille_terrain));
    uint8_t e2=*(map_routes+offset+(1));
    uint8_t e3=*(map_routes+offset+(taille_terrain));
    uint8_t e4=*(map_routes+offset+(-1));

    if(c==1)
    {
        if(e3==4)
        {
            code=5;
            *(map_routes+offset)=5;
            *(map_routes+offset+(taille_terrain))=5;
        }
    }

    else if(c==2)
    {
        if(e4==8)
            {
                code=10;
                *(map_routes+offset)=10;
                *(map_routes+offset+(-1))=10;
            }
    }

    else if(c==4)
    {
        if(e1==1)
            {
                code=5;
                *(map_routes+offset)=5;
                *(map_routes+offset+(-taille_terrain))=5;
            }
    }
    else if(c==8)
    {
        if(e2==2)
            {
                code=10;
                *(map_routes+offset)=10;
                *(map_routes+offset+(1))=10;
            }
    }

    return code;
}

void Ville::calcul_couleur_immeubles(float* c)
{
    c[0]=c[0]/3.+0.4;
    c[1]=c[1]/3.+0.4;
    c[2]=c[2]/3.+0.6;
    if(c[0]>1.)c[0]=1.;
    if(c[1]>1.)c[1]=1.;
    if(c[2]>1.)c[2]=1.;
}


void Ville::affiche_objets()
{
    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FOG);
    glEnable(GL_CULL_FACE);
    GLfloat c[3];
    glLineWidth(4.);
    for(int i=0;i<objet_ptr;i++)
    {
        glPushMatrix();
        glMultMatrixf(objets[i].matrice);
        GFunc::BGRi_RGBf(couleurs_iters[objets[i].couleur],c);



        if(objets[i].liste_id==immeuble_01_liste_id)
        {
            glUseProgram(ShadersVille::fenetre01->programme_id);
            glUniform1f(ShadersVille::fenetre01->partSize,partSize);
            glUniform1f(ShadersVille::fenetre01->hauteur_etage,VILLE_BATIMENT_HAUTEUR_ETAGE);
            glUniform1i(ShadersVille::fenetre01->nombre_etages,objets[i].nombre_etages);
            calcul_couleur_immeubles(c);
            glColor3fv(c);
        }
        else if(objets[i].liste_id==immeuble_02_liste_id)
        {
            glUseProgram(ShadersVille::fenetre02->programme_id);
            glUniform1f(ShadersVille::fenetre02->partSize,partSize);
            glUniform1f(ShadersVille::fenetre02->hauteur_etage,VILLE_BATIMENT_HAUTEUR_ETAGE);
            glUniform1i(ShadersVille::fenetre02->nombre_etages,objets[i].nombre_etages);
            calcul_couleur_immeubles(c);
            glColor3fv(c);
        }
        else if(objets[i].liste_id==immeuble_03_liste_id)
        {
            glUseProgram(ShadersVille::fenetre03->programme_id);
            glUniform1f(ShadersVille::fenetre03->partSize,partSize);
            glUniform1f(ShadersVille::fenetre03->hauteur_etage,VILLE_BATIMENT_HAUTEUR_ETAGE);
            glUniform1i(ShadersVille::fenetre03->nombre_etages,objets[i].nombre_etages);
            glRotatef(float((objets[i].x*5+objets[i].y)%4)*90.,0.,1.,0.);
            calcul_couleur_immeubles(c);
            glColor3fv(c);
        }
        else if(objets[i].liste_id==immeuble_04_liste_id)
        {

            glUseProgram(ShadersVille::fenetre04->programme_id);
            glUniform1f(ShadersVille::fenetre04->partSize,partSize);
            glUniform1f(ShadersVille::fenetre04->hauteur_etage,VILLE_BATIMENT_HAUTEUR_ETAGE);
            glUniform1i(ShadersVille::fenetre04->nombre_etages,objets[i].nombre_etages);
            calcul_couleur_immeubles(c);
            glColor3fv(c);

            //glUseProgram(0);
            //glRotatef(float((objets[i].x*5+objets[i].y)%4)*90.,0.,1.,0.);
        }
        else if(objets[i].liste_id==route_01_espace_vert_liste_id)
        {
            glRotatef(float((objets[i].x*5+objets[i].y)%4)*90.,0.,1.,0.);
            glScalef(1.,1.5+sin(float(objets[i].y)/DEG_RAD*451.)*cos(float(objets[i].x)/DEG_RAD*607.)*1.2,1.);
        }

        glCallList(objets[i].liste_id);
        glUseProgram(0);
        glPopMatrix();
    }
}

void Ville::affiche_objets_reflet()
{
    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FOG);
    glEnable(GL_CULL_FACE);
    GLfloat c[3];
    glLineWidth(4.);
    for(int i=0;i<objet_ptr;i++)
    {
        if(objets[i].couleur==VILLE_ELEMENT_ROUTE) continue;
        glPushMatrix();
        glMultMatrixf(objets[i].matrice);
        GFunc::BGRi_RGBf(couleurs_iters[objets[i].couleur],c);



        if(objets[i].liste_id==immeuble_01_liste_id)
        {
            glUseProgram(ShadersVille::fenetre01->programme_id);
            glUniform1f(ShadersVille::fenetre01->partSize,partSize);
            glUniform1f(ShadersVille::fenetre01->hauteur_etage,VILLE_BATIMENT_HAUTEUR_ETAGE);
            glUniform1i(ShadersVille::fenetre01->nombre_etages,objets[i].nombre_etages);
            calcul_couleur_immeubles(c);
            glColor3fv(c);
        }
        else if(objets[i].liste_id==immeuble_02_liste_id)
        {
            glUseProgram(ShadersVille::fenetre02->programme_id);
            glUniform1f(ShadersVille::fenetre02->partSize,partSize);
            glUniform1f(ShadersVille::fenetre02->hauteur_etage,VILLE_BATIMENT_HAUTEUR_ETAGE);
            glUniform1i(ShadersVille::fenetre02->nombre_etages,objets[i].nombre_etages);
            calcul_couleur_immeubles(c);
            glColor3fv(c);
        }
        else if(objets[i].liste_id==immeuble_03_liste_id)
        {
            glUseProgram(ShadersVille::fenetre03->programme_id);
            glUniform1f(ShadersVille::fenetre03->partSize,partSize);
            glUniform1f(ShadersVille::fenetre03->hauteur_etage,VILLE_BATIMENT_HAUTEUR_ETAGE);
            glUniform1i(ShadersVille::fenetre03->nombre_etages,objets[i].nombre_etages);
            glRotatef(float((objets[i].x*5+objets[i].y)%4)*90.,0.,1.,0.);
            calcul_couleur_immeubles(c);
            glColor3fv(c);
        }
        else if(objets[i].liste_id==immeuble_04_liste_id)
        {

            glUseProgram(ShadersVille::fenetre04->programme_id);
            glUniform1f(ShadersVille::fenetre04->partSize,partSize);
            glUniform1f(ShadersVille::fenetre04->hauteur_etage,VILLE_BATIMENT_HAUTEUR_ETAGE);
            glUniform1i(ShadersVille::fenetre04->nombre_etages,objets[i].nombre_etages);
            calcul_couleur_immeubles(c);
            glColor3fv(c);

            //glUseProgram(0);
            //glRotatef(float((objets[i].x*5+objets[i].y)%4)*90.,0.,1.,0.);
        }
        else if(objets[i].liste_id==route_01_espace_vert_liste_id)
        {
            glRotatef(float((objets[i].x*5+objets[i].y)%4)*90.,0.,1.,0.);
            glScalef(1.,1.5+sin(float(objets[i].y)/DEG_RAD*451.)*cos(float(objets[i].x)/DEG_RAD*607.)*1.2,1.);
        }

        glCallList(objets[i].liste_id);
        glUseProgram(0);
        glPopMatrix();
    }
}

//===============================================================================================================================================================
//
//      Gestion trafic
//
//  Le "Périmètre de vie" est la zone autour de l'observateur où se trouvent les véhicules.
//  Les véhicules qui sortent du champ de vision sont mis en attente, jusqu'à régénération de la ville, ou jusqu'à ce qu'ils sortent du périmètre de vie.
//
//===============================================================================================================================================================

#define VILLE_MAX_VEHICULE_POS_ITER 10

void Ville::reset_vehicule(uint16_t id_vehicule)
{
    float distance_strates_circulation=partSize*3.;
    //Nouveaux paramètres:
    vehicules[id_vehicule].couleur=uint8_t(rand() % VILLE_NBR_COULEURS_VEHICULES);
    vehicules[id_vehicule].posPart=0.5;
    vehicules[id_vehicule].vitesse_max=(float(rand() % 90 + 20) )*1000./3600.;    //vitesse max comprise entre 20 et 110 km/h (transformée en m/s)
    vehicules[id_vehicule].vitesse=vehicules[id_vehicule].vitesse_max;//float(rand())/float(RAND_MAX)*vehicules[id_vehicule].vitesse_max;
    vehicules[id_vehicule].id_intOut=uint8_t(0xff);  //0xff = Entrée/Sortie indéterminées

    vehicules[id_vehicule].typeObjet=uint8_t(rand()%4);
    int de=rand()%7;
    if(de<=2) de=0;
    else if(de<=4 ) de=1;
    else if(de==5 ) de=3;
    else if(de==6 ) de=4;
    if(de>0)vehicules[id_vehicule].altitude=float(de)*distance_strates_circulation+float(rand())/float(RAND_MAX)*20.;
    else vehicules[id_vehicule].altitude=2.;

    vehicules[id_vehicule].echelle_x=(float(rand())/float(RAND_MAX)*1.5+1.823);
    vehicules[id_vehicule].echelle_y=(float(rand())/float(RAND_MAX)*2.+1.505);
    vehicules[id_vehicule].echelle_z=(float(rand())/float(RAND_MAX)*2.+4.556);
}



//Positionne un véhicule sur un cercle à la limite du périmètre de vie

void Ville::recycle_vehicule(uint16_t id_vehicule,int cx,int cy)
{
    int x,y;
    int offsetX,offsetY;
    uint8_t c;
    int distanceMax_part=int(distanceMax/partSize);
    int largeur_bande=4;
    int distanceMin_part=distanceMax_part-largeur_bande;    //Largeur de la bande périphérique où sont positionnés les nouveau véhicules
    int cptr=VILLE_MAX_VEHICULE_POS_ITER;   //Nombre d'essais de positionnement. Si le champ est saturé, on affiche pas le véhicule.
    do
    {
        float angle=float(rand())/float(RAND_MAX)*360.;
        float r=float(rand()%largeur_bande + distanceMin_part);

        x=int(r*cosf(angle));
        y=int(r*sinf(angle));

        offsetX=x+taille_terrain/2;
        offsetY=y+taille_terrain/2;
        c=*(map_terrain+offsetX+offsetY*taille_terrain);

        cptr-=1;
    } while (c!=VILLE_ELEMENT_ROUTE && c!=0 && cptr>0);

    if(cptr==0 || c==0) vehicules[id_vehicule].drapeau_hors_champ=true;
    else vehicules[id_vehicule].drapeau_hors_champ=false;
    vehicules[id_vehicule].partX=x+cx;
    vehicules[id_vehicule].partY=y+cy;

    #if VILLE_DEBUG_TRAFIC
    cout<<"ici2"<<endl;
    cout<<"X="<<vehicules[id_vehicule].partX<<" Y="<<vehicules[id_vehicule].partY<<" Element:"<<int(c)<<" Hors-champ:"<<vehicules[id_vehicule].drapeau_hors_champ<<endl;
    #endif

    reset_vehicule(id_vehicule);
}



//Appelé à chaque changement distants d'observateur
void Ville::positionne_vehicules()
{
    uint16_t i;
    int cx=int(roundf(obs.x/partSize));
    int cy=int(roundf(-obs.z/partSize));
    srand(1);       //Pour avoir toujours la même séquence de positionnements aléatoires
    for(i=0;i<VILLE_NBR_VEHICULES_MAX;i++)
    {
        positionne_vehicule(i,cx,cy);
    }
}

void Ville::positionne_vehicule(uint16_t id_vehicule,int cx,int cy)
{
    int x,y;
    int offsetX,offsetY;
    uint8_t c;
    int distanceMax_part=int(distanceMax/partSize);
    int cptr=VILLE_MAX_VEHICULE_POS_ITER;   //Nombre d'essais de positionnement. Si le champ est saturé, on affiche pas le véhicule.
    do
    {
        x=rand() % distanceMax_part*2 - distanceMax_part;
        y=rand() % distanceMax_part*2 - distanceMax_part;
        offsetX=x+taille_terrain/2;
        offsetY=y+taille_terrain/2;
        c=*(map_terrain+offsetX+offsetY*taille_terrain);
        cptr-=1;
    } while (c!=VILLE_ELEMENT_ROUTE && c!=0 && cptr>0);

    if(cptr=0 || c==0) vehicules[id_vehicule].drapeau_hors_champ=true;
    else vehicules[id_vehicule].drapeau_hors_champ=false;
    vehicules[id_vehicule].partX=x+cx;
    vehicules[id_vehicule].partY=y+cy;

    #if VILLE_DEBUG_TRAFIC
    cout<<"ici1"<<endl;
    cout<<"X="<<vehicules[id_vehicule].partX<<" Y="<<vehicules[id_vehicule].partY<<" Element:"<<int(c)<<" Hors-champ:"<<vehicules[id_vehicule].drapeau_hors_champ<<endl;
    #endif
    reset_vehicule(id_vehicule);
}

void Ville::rafraichi_vehicules()
{
    uint16_t i;
    int cx=int(roundf(obs.x/partSize));
    int cy=int(roundf(-obs.z/partSize));
    for(i=0;i<VILLE_NBR_VEHICULES_MAX;i++)
    {
       rafraichi_vehicule(i,cx,cy);
    }
}

void Ville::rafraichi_vehicule(uint16_t id_vehicule,int cx,int cy)
{
    uint8_t c;
    int offsetX,offsetY;
    int distanceMax_part=int(distanceMax/partSize);
    int x=vehicules[id_vehicule].partX;
    int y=vehicules[id_vehicule].partY;
    float dx=float(x-cx);
    float dy=float(y-cy);
    int dist=int(round(sqrtf(dx*dx+dy*dy)));
    if (dist>=distanceMax_part)
    {
        #if VILLE_DEBUG_TRAFIC
        cout<<"Sortie du périmètre de vie"<<endl;
        #endif
        recycle_vehicule(id_vehicule,cx,cy);
    }
    else
    {
        offsetX=x-cx+taille_terrain/2;
        offsetY=y-cy+taille_terrain/2;
        c=*(map_terrain+offsetX+offsetY*taille_terrain);
        if(!vehicules[id_vehicule].drapeau_hors_champ)
        {
            if(c==0)
            {
                vehicules[id_vehicule].drapeau_hors_champ=true;
                #if VILLE_DEBUG_TRAFIC
                cout<<"ici3"<<endl;
                cout<<"X="<<vehicules[id_vehicule].partX<<" Y="<<vehicules[id_vehicule].partY<<" Element:"<<int(c)<<" Hors-champ:"<<vehicules[id_vehicule].drapeau_hors_champ<<endl;
                #endif
            }
            else if (c!=VILLE_ELEMENT_ROUTE) recycle_vehicule(id_vehicule,cx,cy);
            else mouvement_vehicule(id_vehicule,cx,cy);
        }
        else
        {
            if(c==VILLE_ELEMENT_ROUTE)
            {
                vehicules[id_vehicule].drapeau_hors_champ=false;
                #if VILLE_DEBUG_TRAFIC
                cout<<"ici4"<<endl;
                cout<<"X="<<vehicules[id_vehicule].partX<<" Y="<<vehicules[id_vehicule].partY<<" Element:"<<int(c)<<" Hors-champ:"<<vehicules[id_vehicule].drapeau_hors_champ<<endl;
                #endif
            }
            else if (c!=0) recycle_vehicule(id_vehicule,cx,cy);
        }

    }
}

void Ville::determine_vehicule_in_out(uint16_t id_vehicule, uint8_t code_route)
{
    /*
  uint8_t route0[12*2]={0,1, 0,2, 0,3, 1,0, 1,2, 1,3, 2,0, 2,1, 2,3, 3,0, 3,1, 3,2};
  uint8_t route1[6*2]={1,2, 1,3, 2,1, 2,3, 3,1, 3,2};
  uint8_t route2[6*2]={0,2, 0,3, 2,0, 2,3, 3,0, 3,2};
  uint8_t route3[2*2]={3,2, 2,3};
  uint8_t route4[6*2]={0,1, 0,3,1,0,1,3,3,0,3,1};
  uint8_t route5[2*2]={3,1,1,3};
  uint8_t route6[2*2]={0,3,3,0};
  uint8_t route7[1*2]={3,3};
  uint8_t route8[6*2]={0,1,0,2,1,0,1,2,2,0,2,1};
  uint8_t route9[2*2]={1,2,2,1};
  uint8_t route10[2*2]={0,2,2,0};
  uint8_t route11[1*2]={2,2};
  uint8_t route12[2*2]={0,1,1,0};
  uint8_t route13[1*2]={1,1};
  uint8_t route14[1*2]={0,0};
  uint8_t* tabs_inout[15]={route0,route1,route2,route3,route4,route5,route6,route7,route8,route9,route10,route11,route12,route13,route14};
  uint8_t tab_numCombinaisons[15]={12,6,6,2,6,2,2,1,6,2,2,1,2,1,1};
  int combinaison=rand() % tab_numCombinaisons[code_route];
  vehicules[id_vehicule].id_in=tabs_inout[code_route][code_route*2];
  vehicules[id_vehicule].id_out=tabs_inout[code_route][code_route*2+1];
  */
  //Parcours:    0,1  0,2  0,3  1,0  1,2  1,3  2,0  2,1  2,3  3,0  3,1  3,2  0,0  1,1  2,2  3,3
  //Identifiants: 0    1    2    3    4    5    6    7    8    9    10  11    12   13   14   15
  uint8_t route0[12]={0,1,2,3,4,5,6,7,8,9,10,11};
  uint8_t route1[6]={4, 5, 7, 8, 10, 11};
  uint8_t route2[6]={1, 2, 6, 8, 9, 11};
  uint8_t route3[2]={8, 11};
  uint8_t route4[6]={0, 2, 3, 5, 9, 10};
  uint8_t route5[2]={5,10};
  uint8_t route6[2]={2,9};
  uint8_t route7[1]={15};
  uint8_t route8[6]={0, 1, 3, 4, 6, 7};
  uint8_t route9[2]={4,7};
  uint8_t route10[2]={1, 6};
  uint8_t route11[1]={14};
  uint8_t route12[2]={0, 3};
  uint8_t route13[1]={13};
  uint8_t route14[1]={12};
  uint8_t* tabs_inout[15]={route0,route1,route2,route3,route4,route5,route6,route7,route8,route9,route10,route11,route12,route13,route14};
  uint32_t tab_numCombinaisons[15]={12,6,6,2,6,2,2,1,6,2,2,1,2,1,1};

  int combinaison=rand() % tab_numCombinaisons[code_route];
  vehicules[id_vehicule].id_intOut=tabs_inout[code_route][combinaison];
  //cout<<int(vehicules[id_vehicule].id_intOut)<<endl;
}

void Ville::change_vehicule_position(uint16_t id_vehicule,int cx,int cy)
{
    int offsetX,offsetY,x,y;
    uint8_t code_route,choix_out;
    ville_vehicule* vehicule=&vehicules[id_vehicule];

    uint8_t tab_inOuts[16*2]={0,1, 0,2, 0,3, 1,0, 1,2, 1,3, 2,0, 2,1, 2,3, 3,0, 3,1, 3,2, 0,0, 1,1, 2,2, 3,3};
    uint8_t vOut=tab_inOuts[vehicule->id_intOut*2+1]; //Voie de sortie dans l'ancienne route
    uint8_t vIn; //Voie d'entrée dans la nouvelle route
    switch(vOut)
    {
        case 0:
            vehicule->partY-=1;
            vIn=2;
            break;
        case 1:
            vehicule->partX+=1;
            vIn=3;
            break;
        case 2:
            vehicule->partY+=1;
            vIn=0;
            break;
        case 3:
            vehicule->partX-=1;
            vIn=1;
            break;
        default:break;
    }
    vehicule->posPart-=1.;

    x=vehicule->partX;
    y=vehicule->partY;
    offsetX=x-cx+taille_terrain/2;
    offsetY=y-cy+taille_terrain/2;
    code_route=*(map_routes+offsetX+taille_terrain*offsetY);

    /*
    uint8_t choix_route0[1,2,3, 0,2,3, 0,1,3, 0,1,2];
    uint8_t choix_route1[0,0, 2,3, 1,3, 1,2];
    uint8_t choix_route2[2,3, 0,0, 0,3, 0,2];
    uint8_t choix_route4[1,3, 0,3, 0,0, 0,1];
    uint8_t choix_route8[1,2, 0,2, 0,1, 0,0];
    */

    uint8_t choix_route0[3*4]={0,1,2, 3,4,5, 6,7,8, 9,10,11};
    uint8_t choix_route1[2*4]={0,0, 4,5, 7,8, 10,11};
    uint8_t choix_route2[2*4]={1,2, 0,0, 6,8, 9,11};
    uint8_t choix_route4[2*4]={0,2, 3,5, 0,0, 9,10};
    uint8_t choix_route8[2*4]={0,1, 3,4, 6,7, 0,0};

    switch (code_route)
    {
        case 0:
            choix_out=rand()%3;
            vehicule->id_intOut=choix_route0[choix_out+3*vIn];
            break;
        case 1:
            choix_out=rand()%2;
            vehicule->id_intOut=choix_route1[choix_out+2*vIn];
            break;
        case 2:
            choix_out=rand()%2;
            vehicule->id_intOut=choix_route2[choix_out+2*vIn];
            break;
        case 3:
            if(vIn==3)vehicule->id_intOut=11;else vehicule->id_intOut=8;
            break;
        case 4:
            choix_out=rand()%2;
            vehicule->id_intOut=choix_route4[choix_out+2*vIn];
            break;
        case 5:
            if(vIn==3)vehicule->id_intOut=10;else vehicule->id_intOut=5;
            break;
        case 6:
            if(vIn==0)vehicule->id_intOut=2;else vehicule->id_intOut=9;
            break;
        case 7:
            vehicule->id_intOut=15;
            break;
        case 8:
            choix_out=rand()%2;
            vehicule->id_intOut=choix_route8[choix_out+2*vIn];
            break;
        case 9:
            if(vIn==1)vehicule->id_intOut=4;else vehicule->id_intOut=7;
            break;
        case 10:
            if(vIn==0)vehicule->id_intOut=1;else vehicule->id_intOut=6;
            break;
        case 11:
            vehicule->id_intOut=14;
            break;
        case 12:
            if(vIn==0)vehicule->id_intOut=0;else vehicule->id_intOut=3;
            break;
        case 13:
            vehicule->id_intOut=13;
            break;
        case 14:
            vehicule->id_intOut=12;
            break;
        default:break;
    }
    vehicule->code_route=code_route;
}

void Ville::mouvement_vehicule(uint16_t id_vehicule,int cx,int cy)
{
    int offsetX,offsetY;
    int x=vehicules[id_vehicule].partX;
    int y=vehicules[id_vehicule].partY;
    offsetX=x-cx+taille_terrain/2;
    offsetY=y-cy+taille_terrain/2;
    ville_vehicule* vehicule=&vehicules[id_vehicule];

    uint8_t code_route=*(map_routes+offsetX+taille_terrain*offsetY);
    if(vehicule->id_intOut==0xff)
    {
        determine_vehicule_in_out(id_vehicule,code_route);
        vehicule->code_route=code_route;
    }
    //Si la route a changée en cours de parcours ! Ca arrive par effet de cascade sur les bordures de champ de vision
    if(vehicule->code_route!=code_route)
    {
            determine_vehicule_in_out(id_vehicule,code_route);
            vehicule->code_route=code_route;
    }
    if(vehicule->posPart>1.)
    {
        change_vehicule_position(id_vehicule,cx,cy);
    }

    float ctx,cty,rtx,rty,at0,at1,atInter;

    switch (vehicule->id_intOut)
    {
        case 0:
            ctx=0.5;
            cty=-0.5;
            rtx=0.625;
            rty=0.625;
            at0=180.*DEG_RAD;
            at1=90.*DEG_RAD;
            atInter=at0+(at1-at0)*vehicule->posPart;
            vehicule->xIn=cos(atInter)*rtx+ctx;
            vehicule->yIn=sin(atInter)*rty+cty;
            break;
        case 1:
            vehicule->xIn=-0.125;
            vehicule->yIn=-0.5+vehicule->posPart;
            break;

        case 2:
            ctx=-0.5;
            cty=-0.5;
            rtx=0.375;
            rty=0.375;
            at0=0.*DEG_RAD;
            at1=90.*DEG_RAD;
            atInter=at0+(at1-at0)*vehicule->posPart;
            vehicule->xIn=cos(atInter)*rtx+ctx;
            vehicule->yIn=sin(atInter)*rty+cty;
            break;

        case 3:
            ctx=0.5;
            cty=-0.5;
            rtx=0.375;
            rty=0.375;
            at0=90.*DEG_RAD;
            at1=180.*DEG_RAD;
            atInter=at0+(at1-at0)*vehicule->posPart;
            vehicule->xIn=cos(atInter)*rtx+ctx;
            vehicule->yIn=sin(atInter)*rty+cty;
            break;
        case 4:
            ctx=0.5;
            cty=0.5;
            rtx=0.625;
            rty=0.625;
            at0=-90.*DEG_RAD;
            at1=-180.*DEG_RAD;
            atInter=at0+(at1-at0)*vehicule->posPart;
            vehicule->xIn=cos(atInter)*rtx+ctx;
            vehicule->yIn=sin(atInter)*rty+cty;
            break;
        case 5:
            vehicule->xIn=0.5-vehicule->posPart;
            vehicule->yIn=-0.125;
            break;
        case 6:
            vehicule->xIn=0.125;
            vehicule->yIn=0.5-vehicule->posPart;
            break;
        case 7:
            ctx=0.5;
            cty=0.5;
            rtx=0.375;
            rty=0.375;
            at0=-180.*DEG_RAD;
            at1=-90.*DEG_RAD;
            atInter=at0+(at1-at0)*vehicule->posPart;
            vehicule->xIn=cos(atInter)*rtx+ctx;
            vehicule->yIn=sin(atInter)*rty+cty;
            break;
        case 8:
            ctx=-0.5;
            cty=0.5;
            rtx=0.625;
            rty=0.625;
            at0=0.*DEG_RAD;
            at1=-90.*DEG_RAD;
            atInter=at0+(at1-at0)*vehicule->posPart;
            vehicule->xIn=cos(atInter)*rtx+ctx;
            vehicule->yIn=sin(atInter)*rty+cty;
            break;
        case 9:
            ctx=-0.5;
            cty=-0.5;
            rtx=0.625;
            rty=0.625;
            at0=90.*DEG_RAD;
            at1=0.*DEG_RAD;
            atInter=at0+(at1-at0)*vehicule->posPart;
            vehicule->xIn=cos(atInter)*rtx+ctx;
            vehicule->yIn=sin(atInter)*rty+cty;
            break;
        case 10:
            vehicule->xIn=-0.5+vehicule->posPart;
            vehicule->yIn=0.125;
            break;
        case 11:
            ctx=-0.5;
            cty=0.5;
            rtx=0.375;
            rty=0.375;
            at0=-90.*DEG_RAD;
            at1=0.*DEG_RAD;
            atInter=at0+(at1-at0)*vehicule->posPart;
            vehicule->xIn=cos(atInter)*rtx+ctx;
            vehicule->yIn=sin(atInter)*rty+cty;
            break;
        case 12:
            ctx=0.;
            cty=-0.5;
            rtx=0.125;
            rty=0.5;
            at0=180.*DEG_RAD;
            at1=0.*DEG_RAD;
            atInter=at0+(at1-at0)*vehicule->posPart;
            vehicule->xIn=cos(atInter)*rtx+ctx;
            vehicule->yIn=sin(atInter)*rty+cty;
            break;
        case 13:
            ctx=0.5;
            cty=0.;
            rtx=0.5;
            rty=0.125;
            at0=-90.*DEG_RAD;
            at1=-270.*DEG_RAD;
            atInter=at0+(at1-at0)*vehicule->posPart;
            vehicule->xIn=cos(atInter)*rtx+ctx;
            vehicule->yIn=sin(atInter)*rty+cty;
            break;
        case 14:
            ctx=0.;
            cty=0.5;
            rtx=0.125;
            rty=0.5;
            at0=0.*DEG_RAD;
            at1=-180.*DEG_RAD;
            atInter=at0+(at1-at0)*vehicule->posPart;
            vehicule->xIn=cos(atInter)*rtx+ctx;
            vehicule->yIn=sin(atInter)*rty+cty;
            break;

        case 15:
            ctx=-0.5;
            cty=0.;
            rtx=0.5;
            rty=0.125;
            at0=90.*DEG_RAD;
            at1=-90.*DEG_RAD;
            atInter=at0+(at1-at0)*vehicule->posPart;
            vehicule->xIn=cos(atInter)*rtx+ctx;
            vehicule->yIn=sin(atInter)*rty+cty;
            break;


        default: break;

    }
    vehicule->posPart+=(GVars::temps_scene-t_prec)*vehicule->vitesse/partSize;
    //if(vehicule->posPart>1.)vehicule->posPart=0.;
}

void Ville::affiche_vehicules()
{
 uint16_t i;
    for(i=0;i<VILLE_NBR_VEHICULES_MAX;i++)
    {
        if(!vehicules[i].drapeau_hors_champ)affiche_vehicule(i);
    }
}
void Ville::affiche_vehicule(uint16_t id_vehicule)
{

    glPushMatrix();
        float x=(float(vehicules[id_vehicule].partX)+vehicules[id_vehicule].xIn)*partSize;
        float z=(float(vehicules[id_vehicule].partY)+vehicules[id_vehicule].yIn)*partSize;
        float dx=x-vehicules[id_vehicule].xPrec;
        float dz=z-vehicules[id_vehicule].zPrec;
        float n=sqrt(dx*dx+dz*dz);
        if(n>0.)
        {
            dx/=n;
            dz/=n;
            float cosDir=-dz;
            float sinDir=-dx;
            if(cosDir<-1.)cosDir=-1.;
            if(cosDir>1.)cosDir=1.;
            if(sinDir<-1.)sinDir=-1.;
            if(sinDir>1.)sinDir=1.;
            if(sinDir>=0.) vehicules[id_vehicule].rot=RAD_DEG*acos(cosDir);
            else vehicules[id_vehicule].rot=RAD_DEG*acos(-cosDir)+180.;
        }
        //vehicules[id_vehicule].rot=0;
        glTranslatef(x,VILLE_ALTITUDE_ROUTES+1.505/2.,z);
        glRotatef(vehicules[id_vehicule].rot,0.,1.,0.);
        float intensite_ombre=1./vehicules[id_vehicule].altitude;
        if(intensite_ombre>1.)intensite_ombre=1.;
        GFunc::affiche_ombre_sol(0.,-1.505/2.+0.05,0.,
                                vehicules[id_vehicule].echelle_x,vehicules[id_vehicule].echelle_y,vehicules[id_vehicule].echelle_z,intensite_ombre);
        glTranslatef(0.,vehicules[id_vehicule].altitude/vehicules[id_vehicule].echelle_y,0.);
        //glScalef(vehicules[id_vehicule].echelle_x,vehicules[id_vehicule].echelle_y,vehicules[id_vehicule].echelle_z);
        //glScalef(1.823,1.505,4.556);
        glRotatef(-90.,1.,0.,0.);
        GLuint tab_types_vehicules[5]={sphere_liste_id,cube02_liste_id,tetraedre_liste_id,cylindre_liste_id,tore_liste_id};
        glColor3fv(&palette_vehicules[vehicules[id_vehicule].couleur*3]);
        glCallList(tab_types_vehicules[vehicules[id_vehicule].typeObjet]);

        vehicules[id_vehicule].xPrec=x;
        vehicules[id_vehicule].zPrec=z;
    glPopMatrix();
}

//========================================================================================================
//                          Affichages terrain
//========================================================================================================

void Ville::affiche_reflet_eau()
{
    glShadeModel( GL_FLAT );
	glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glDisable(GL_FOG);
    glDisable(GL_LIGHTING);
	desactive_textures();

	glActiveTexture(GL_TEXTURE0);
	//glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,GVars::texture_reflet_eau->texture_id);
	glUseProgram(ShadersObjets3d::reflet_eau->programme_id);
	glUniform3f(ShadersObjets3d::reflet_eau->position_obs,obs.x,obs.y,-obs.z);
    glUniform1i(ShadersObjets3d::reflet_eau->texture_reflet,0);
    glUniform1f(ShadersObjets3d::reflet_eau->largeur_ecran,float(GVars::largeur_ecran));
    glUniform1f(ShadersObjets3d::reflet_eau->hauteur_ecran,float(GVars::hauteur_ecran));
    glUniform1f(ShadersObjets3d::reflet_eau->t,float(GVars::temps_scene));

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,ShadersObjets3d::texture_bruit_id);
    glUniform1f(ShadersObjets3d::reflet_eau->transparence,1.);
    glUniform1f(ShadersObjets3d::reflet_eau->attenuation_speculaire,500.);
    glUniform1i(ShadersObjets3d::reflet_eau->texture_bruit,1);
    glUniform1f(ShadersObjets3d::reflet_eau->intensite_ondes,.1);
    glUniform1f(ShadersObjets3d::reflet_eau->vitesse_ondes,2.4);
    glUniform1f(ShadersObjets3d::reflet_eau->quantite_ondes,3.);
    glUniform1f(ShadersObjets3d::reflet_eau->frequence_ondes,.1);
    glUniform1f(ShadersObjets3d::reflet_eau->amplitude_ondes,5.);//10
    glUniform1f(ShadersObjets3d::reflet_eau->niveau_perturbations_ondes,5.);
    glUniform1f(ShadersObjets3d::reflet_eau->echelle_texture_bruit_ondes,25.);
    glUniform3f(ShadersObjets3d::reflet_eau->position_source,L0_orientation[0],L0_orientation[1],L0_orientation[2]);

    glUniform4f(ShadersObjets3d::reflet_eau->couleur_fond,couleur_fond_eau[0],couleur_fond_eau[1],couleur_fond_eau[2],couleur_fond_eau[3]);

    //glUniform4f(ShadersObjets3d::reflet_eau->couleur_speculaire,L0_spec[0],L0_spec[1],L0_spec[2],L0_spec[3]);
    float couleur_speculaire[4]={.6,.6,.6,1.};
    glMaterialfv(GL_FRONT,GL_SPECULAR,couleur_speculaire);
    glMaterialf(GL_FRONT, GL_SHININESS, 5.);

    glColor3f(1.,1.,0.);
    glBegin(GL_TRIANGLES);
        glVertex3f(triangle_visible[0].x,-0.02,triangle_visible[0].y);
        glVertex3f(triangle_visible[1].x,-0.02,triangle_visible[1].y);
        glVertex3f(triangle_visible[2].x,0.,triangle_visible[2].y);
    glEnd();
    glUseProgram(0);
    glActiveTexture(GL_TEXTURE0);
    //glEnable(GL_FOG);
}

void Ville::affiche_grille()
{
    glShadeModel( GL_FLAT );
	glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glDisable(GL_FOG);
    glDisable(GL_LIGHTING);
    glBlendFunc(GL_SRC_ALPHA,GL_DST_ALPHA);
	glLineWidth(1.);
	desactive_textures();
	glUseProgram(0);
    glColor4f(0.,1.,1.,.33);
    for(int i=0;i<objet_grille_ptr;i++)
    {
        glPushMatrix();
        glMultMatrixf(grille[i].matrice);
        glCallList(grille[i].liste_id);
        glPopMatrix();
    }
}




void Ville::efface_bitmap_terrain()
{
    for(uint32_t i=0;i<bitmap_terrain->largeur*bitmap_terrain->hauteur;i++)
        {
            *(uint32_t*)(bitmap_terrain->bitmap+i*4)=0xff000000;
        }
}

void Ville::efface_map_terrain()
{
    for(uint32_t i=0;i<taille_terrain*taille_terrain/4;i++)
        {
            *(uint32_t*)(map_terrain+i*4)=0x00000000;
            *(uint32_t*)(map_terrain_temp+i*4)=0x00000000;
        }
}

void Ville::affiche_bitmap_terrain()
{
    GFunc::vue_ortho();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	desactive_textures();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,bitmap_terrain->texture_id);
	//glBindTexture(GL_TEXTURE_2D,FrameBuffers::fbTex[FBO_SCREENSHOT]);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);

    float l=2.*float(bitmap_terrain->largeur)/float(GVars::largeur_ecran)*2.-1.;
    float h=2.*float(bitmap_terrain->hauteur)/float(GVars::hauteur_ecran)*2.-1.;

	glColor4f(1.,1.,1.,1.);
	glTranslatef(0.,0.,-1);
	glBegin(GL_POLYGON);
        glTexCoord2f(0.,1.);
        glVertex2f(0.-1.,0.-1.);
        glTexCoord2f(0.,0.);
        glVertex2f(0.-1.,h);
        glTexCoord2f(1.,0.);
        glVertex2f(l,h);
        glTexCoord2f(1.,1.);
        glVertex2f(l,0.-1.);
	glEnd();


	GFunc::vue_perspective();
}

void Ville::eclairages()
{
    // Ambiance générale:

        glClearColor(couleur_ciel[0],couleur_ciel[1],couleur_ciel[2],1.);

        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, L0_amb);

        glFogi(GL_FOG_MODE,GL_LINEAR);
        glFogfv(GL_FOG_COLOR,couleur_brouillard);
        glFogf(GL_FOG_START,0.);
        glFogf(GL_FOG_END,distance_brouillard);
        glFogf(GL_FOG_DENSITY,1.);
        glHint(GL_FOG_HINT,GL_NICEST);

        Lumieres::lumiere_attenuation(GL_LIGHT0,.1,0.1,0.1);
        Lumieres::lumiere_attenuation(GL_LIGHT1,.1,0.1,0.1);
        Lumieres::lumiere_couleur(GL_LIGHT0,L0_amb,L0_diff,L0_spec);
        Lumieres::lumiere_couleur(GL_LIGHT1,L1_amb,L1_diff,L1_spec);

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        glEnable(GL_FOG);
        Lumieres::drapeaux_sources|=0b11;
}

void Ville::maj_positions_lumieres()
{
    Lumieres::lumiere_infinie_orientation(GL_LIGHT0,L0_orientation);
    L1_orientation[0]=-L0_orientation[0];
    L1_orientation[1]=L0_orientation[1];
    L1_orientation[2]=-L0_orientation[2];
    Lumieres::lumiere_infinie_orientation(GL_LIGHT1,L1_orientation);
}

void Ville::affiche_Edora()
{
    float b=clignotement_Edora.valeur_aleatoire_temporelle(GVars::temps_reel,0.05,.5);
    glPushMatrix();
        glEnable(GL_FOG);
        glTranslatef(position_edora.x,position_edora.y,position_edora.z);
        glRotatef(180.,0.,1.,0.);
        glScalef(80.,-80.*3.2,1.);
        glEnable(GL_COLOR_MATERIAL);
        glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glDisable(GL_CULL_FACE);
        glDisable(GL_LIGHTING);
        glUseProgram(0);
        glBindTexture(GL_TEXTURE_2D,GVars::spritesDemo->renvoie_texture_id_sprite(SPR_EDORA));
        GFunc::affiche_texture(alpha_Edora+0.2*b);
        glDisable(GL_COLOR_MATERIAL);
        glEnable(GL_LIGHTING);
        glDisable(GL_BLEND);
        glDisable(GL_COLOR_MATERIAL);
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void Ville::affiche_soleil()
{
    float angleSoleil;
    vec3f positionSoleil(L0_orientation[0],L0_orientation[1],L0_orientation[2]);
    positionSoleil.normalise();
    vec3f normaleDisque(0.,1.,0.);
    vec3f axe(0.,0.,0.);
    axe.produit_vectoriel(normaleDisque,positionSoleil);
    angleSoleil=RAD_DEG*acosf(normaleDisque.x*positionSoleil.x+normaleDisque.y*positionSoleil.y+normaleDisque.z*positionSoleil.z);

    glEnable(GL_CLIP_PLANE0);
    GLdouble equ_horizon[4]={0.,1.,0.,1.};
    glClipPlane(GL_CLIP_PLANE0,equ_horizon);

    glPushMatrix();
        glDisable(GL_FOG);
        glEnable(GL_COLOR_MATERIAL);
        glDisable(GL_CULL_FACE);
        glDisable(GL_LIGHTING);
        glDepthMask(GL_FALSE);
        glDisable(GL_DEPTH_TEST);
        glUseProgram(0);

        glColor3fv(couleur_soleil);//);
        glTranslatef(positionSoleil.x*10.,positionSoleil.y*10.,positionSoleil.z*10.);
        //glRotatef();
        glScalef(7.,7.,7.);
        glRotatef(angleSoleil,axe.x,axe.y,axe.z);
        glCallList(disque_liste_id);
    glPopMatrix();

    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CLIP_PLANE0);

}

void Ville::render_scene()
{
    //cout<<"test1:";if(erreur_openGl("Erreur lors de l'initialisation du FBO ")) {erreur=-1;} cout<<endl;

    eclairages();

    glUseProgram(0);
	GFunc::vue_perspective(GVars::zMin,GVars::zMax);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	GFunc::desactive_textures();
	glDisable(GL_BLEND);


    nbrDalles=0;
	genere_ville();
	if(drapeau_premier_rendu)
    {
        positionne_vehicules();
        drapeau_premier_rendu=false;
    }
    rafraichi_vehicules();

    //---------------- Gestion cosmonaute:

    GVars::drapeau_affiche_ombre_cosmo=true;
    float position_cosmo[3];
    GVars::cosmo->renvoie_position(position_cosmo);
    uint8_t cosmo_elem_id=renvoie_element_id( int(round(position_cosmo[0]/partSize)),int(round(position_cosmo[2]/partSize)));

    /*
    if(cosmo_elem_id==VILLE_ELEMENT_EAU || cosmo_elem_id==VILLE_ELEMENT_RIEN) GVars::altitude_ombre_cosmo=0.1*GVars::cosmo->echelle;
    else GVars::altitude_ombre_cosmo=VILLE_ALTITUDE_ROUTES+0.03;
        */

    //---------------Affiche le reflet:
        glViewport(0,0,GVars::texture_reflet_eau->largeur,GVars::texture_reflet_eau->hauteur);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,GVars::texture_reflet_eau->fbo_id);

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glDisable(GL_FOG);
        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
        GFunc::init_matrice_observateur_reflet_eau(&obs,0.,false);
        affiche_soleil();
        glLoadIdentity();
        GFunc::init_matrice_observateur_reflet_eau(&obs,0.);
        maj_positions_lumieres();
        glEnable(GL_CLIP_PLANE0);
        GLdouble equ_eau[4]={0.,1.,0.,VILLE_MARGE_IMMERGEE};
        glClipPlane(GL_CLIP_PLANE0,equ_eau);

        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT,GL_DIFFUSE);
        init_materiau_simple_face(0xffffffff,0x000000ff,0xffff00ff,0x000000ff,10.);
        affiche_objets_reflet();
        init_materiau_simple_face(0xffffffff,0x000000ff,0xffff00ff,0xffffffff,10.);
        affiche_vehicules();
        equ_eau[3]=0.1;
        glClipPlane(GL_CLIP_PLANE0,equ_eau);
        GVars::cosmo->drapeau_affiche_visage=false;
        GVars::drapeau_affiche_ombre_cosmo=false;
        GFunc::affiche_cosmonaute();
        GVars::cosmo->drapeau_affiche_visage=true;
        GVars::drapeau_affiche_ombre_cosmo=true;
        glDisable(GL_COLOR_MATERIAL);

        /*
        glPushMatrix();
            glTranslatef(-560,5.5,-504.);
            glRotatef(angleObj,-1.,1.,0.);
            glScalef(1,1,1);
            glTranslatef(0.,0.5,0.);
            glCallList(plan01_liste_id);
            //affiche_plan01();
            glUseProgram(0);
        glPopMatrix();
        */

        if(drapeau_affiche_Edora)affiche_Edora();

        glDisable(GL_CLIP_PLANE0);


    //------------------Affiche la scène principale:
    glEnable(GL_FOG);
    glViewport(0,0,GVars::largeur_ecran,GVars::hauteur_ecran);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,FrameBuffers::fbos[FBO_ECRAN]);

    glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	GFunc::init_matrice_observateur(&obs,false);
    affiche_soleil();
    glLoadIdentity();
	//glClear(GL_DEPTH_BITS);
    GFunc::init_matrice_observateur(obs.x,obs.y,obs.z,obs.r,obs.t,obs.l);
    maj_positions_lumieres();
    //glDepthMask(GL_TRUE);

    /*
	glPushMatrix();
        glTranslatef(-560,5.5,-504.);
        glRotatef(angleObj,-1.,1.,0.);
        glScalef(1,1,1);
        glTranslatef(0.,0.5,0.);
        //affiche_plan01();
        //glCallList(plan01_liste_id);
        affiche_plan01();

        glUseProgram(0);
	glPopMatrix();

        desactive_textures();

    init_materiau_simple_face(0xffffffff,0x000000ff,0xffff00ff,0x404040ff,20.);


    glPushMatrix();
        int xt=-28;
        int zt=-23;
        glTranslatef(float(xt)*partSize,7.,float(zt)*partSize);
        glScalef(2.,2.,2.);
        glRotatef(angleTetra,1.,1.,0.);
        angleTetra+=1.;
        glCallList(sphere_liste_id);
        //xTetra-=0.1;
    glPopMatrix();
    */

    //---------- Affiche les objets:
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT,GL_DIFFUSE);
        init_materiau_simple_face(0xffffffff,0x000000ff,0xffff00ff,0x000000ff,10.);
        affiche_objets();
        init_materiau_simple_face(0xffffffff,0x000000ff,0xffff00ff,0xffffffff,10.);
        glEnable(GL_CULL_FACE);
        affiche_vehicules();


    //---------- Affichage de la trame de base en fils de fer:
        /*
        if(drapeau_affiche_trame)
        {
            affiche_grille();
            glPushMatrix();
                glTranslatef(0.,0.07,0.);
                glRotatef(-90,1.,0.,0.);
                affiche_trame_cercle();
            glPopMatrix();
        }
        */

    //------------- Objets additionnels:
    glUseProgram(0);
    desactive_textures();
    float obsPos[3]={obs.x,obs.y,-obs.z};
    glPushMatrix();
    GVars::vaisseau_2->calcul_matrice();
    GVars::vaisseau_2->affiche(obsPos);
    glPopMatrix();
    glPushMatrix();
        GFunc::affiche_ombre_sol(GVars::vaisseau_2->x,VILLE_ALTITUDE_ROUTES+0.1,GVars::vaisseau_2->z,GVars::vaisseau_2->ex*100.,1.,GVars::vaisseau_2->ez*100.,0.5);
    glPopMatrix();
    affiche_reflet_eau();
    //--------- affiche Edora:
    if(drapeau_affiche_Edora)affiche_Edora();
    //


    //---------- debugage:
    glEnable(GL_LIGHTING);

    /*
    glPushMatrix();
        objTest_parent.calcul_matrice();
        glDisable(GL_BLEND);
        glColor3f(1.,0.,0.);
        glCallList(cube02_liste_id);
    glPopMatrix();
    glPushMatrix();
        objTest_parent.affiche_repere_vectoriel();
    glPopMatrix();
    */

    /*
        objTest_obsBkp.copieRepere(obs);
        objTest_obsBkp.z=-objTest_obsBkp.z;
        objTest_obsBkp.l=-objTest_obsBkp.l;
        objTest_obsBkp.t=-objTest_obsBkp.t;
        objTest_enfant_temp.copieRepere(objTest_enfant);
        objTest_enfant_temp.translation_repere(&objTest_obsBkp);

    glPushMatrix();
        objTest_enfant_temp.calcul_matrice();
        glColor3f(0.,0.,1.);
        glCallList(cube02_liste_id);
    glPopMatrix();
    glPushMatrix();
        objTest_enfant_temp.affiche_repere_vectoriel();
    glPopMatrix();
    */


    //---------- Point visé:
    /*
    glPushMatrix();
        glTranslatef(point_vise.x,point_vise.y,point_vise.z);
        glDisable(GL_LIGHTING);
        glDisable(GL_BLEND);
        glColor3f(1.,1.,1.);
        glCallList(sphere_liste_id);
        glEnable(GL_LIGHTING);
    glPopMatrix();
    */

	if(drapeau_affiche_bitmap_terrain) affiche_bitmap_terrain();
    t_prec=GVars::temps_scene;
    //cout<<"test2:";if(erreur_openGl("Erreur lors de l'initialisation du FBO ")) {erreur=-1;} cout<<endl;
}

uint8_t Ville::renvoie_element_id(int x, int y)
{
    int xObs=int(round(obs.x/partSize));
    int yObs=int(round(-obs.z/partSize));
    int xRel=x-xObs;
    int yRel=y-yObs;
    if(xRel>=taille_terrain/2 || yRel>=taille_terrain/2 || xRel<=-taille_terrain/2 || yRel<=-taille_terrain/2) return VILLE_ELEMENT_RIEN;
    else return *(map_terrain+taille_terrain/2+xRel+(taille_terrain/2+yRel)*taille_terrain);
}

char* Ville::renvoie_nom_element_obs()
{
    char* rien={"RIEN"};
    char* eau={"EAU"};
    char* route={"ROUTE"};
    char* espace_vert={"ESPACE VERT"};
    char* batiment={"BATIMENT"};
    char* noms[]={rien,eau,route,espace_vert,batiment};

    uint8_t c=*(map_terrain+taille_terrain/2+taille_terrain/2*taille_terrain);
    if(c>VILLE_ELEMENT_BATIMENT0)c=VILLE_ELEMENT_BATIMENT0;
    return noms[c];
}
