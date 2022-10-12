
#include "editeur_trajectoire.h"


uint32_t EditeurTrajectoire::ET_point_actuel=0;
float EditeurTrajectoire::ET_vitesse_deplacement=0.1;
float EditeurTrajectoire::ET_position_trajectoire=0.;
float EditeurTrajectoire::ET_pas_position_trajectoire=0.005;
char EditeurTrajectoire::ET_fichier_trajectoire[]={"trajectoires/trajectoire_edit.traj"};

//Position actuelle: x,y,z rotations: roulis, tangage, lacet
//Ne pas oublier de rafraichir ces coordonnées, et de les lire à la sortie
float EditeurTrajectoire::ET_x;
float EditeurTrajectoire::ET_y;
float EditeurTrajectoire::ET_z;
float EditeurTrajectoire::ET_r;
float EditeurTrajectoire::ET_t;
float EditeurTrajectoire::ET_l;

vector<Trajectoire*> EditeurTrajectoire::liste_trajectoires;
uint16_t EditeurTrajectoire::trajectoire_courante=0;


//============================================
//		Edition des trajectoires
//============================================

void EditeurTrajectoire::init()
{
	//trajectoire_courante=new Trajectoire();
	trajectoire_courante=0;
}

void EditeurTrajectoire::ajoute_trajectoire(Trajectoire* trajectoire)
{
	liste_trajectoires.push_back(trajectoire);
}

void EditeurTrajectoire::destruction_trajectoires()
{
	//if(trajectoire_courante!=NULL)delete trajectoire_courante;
	trajectoire_courante=0;
	liste_trajectoires.clear();
}

//===================================================
//          Affiche les trajectoires
//===================================================
void EditeurTrajectoire::affiche_trajectoires()
{
    uint16_t i;
    for(i=0;i<liste_trajectoires.size();i++)
    {
        if(i==trajectoire_courante)affiche_trajectoire(i,true);
        else affiche_trajectoire(i,false);
    }
}

void EditeurTrajectoire::affiche_trajectoire(uint16_t trajectoire_id,bool drapeau_active)
{
	//glLoadIdentity();
	glUseProgram(0);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	//glDisable(GL_DEPTH_TEST);
	//oeil_1->positionne_scene(t);
	Trajectoire* ma_trajectoire=liste_trajectoires[trajectoire_id];

	if(drapeau_active)
    {
        glLineWidth(2.);
        ma_trajectoire->affiche_courbe(20,0x80ffff00);
        ma_trajectoire->affiche_points(0xf0ff0000,5);
        ma_trajectoire->affiche_point(ET_point_actuel,0xffff6060,8);
    }
    else
    {
        glLineWidth(2.);
        ma_trajectoire->affiche_courbe(20,0x40ffffff);
        ma_trajectoire->affiche_points(0x40ffffff,4);
    }


	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);
	//glEnable(GL_DEPTH_TEST);ma_trajectoire
}



void EditeurTrajectoire::edit_trajectoire(int key,bool etat)
{
    if(liste_trajectoires.empty()) return;
    if(trajectoire_courante>=liste_trajectoires.size()) trajectoire_courante=liste_trajectoires.size()-1;

    Trajectoire* ma_trajectoire=liste_trajectoires[trajectoire_courante];
    if(ET_point_actuel>=ma_trajectoire->renvoie_nombre_points())ET_point_actuel=ma_trajectoire->renvoie_nombre_points()-1;

	float coordonnees_trajectoire[3];
    if(etat)
    {
        switch (key)
        {
                    case GLFW_KEY_KP_DIVIDE:
                        ET_vitesse_deplacement*=0.5;cout<<"Pas="<<ET_vitesse_deplacement<<endl;
                        ET_pas_position_trajectoire*=0.5;cout<<"Pas trajectoire="<<ET_pas_position_trajectoire<<endl;
                        break;
                    case GLFW_KEY_KP_MULTIPLY:
                        ET_vitesse_deplacement*=2.;cout<<"Pas="<<ET_vitesse_deplacement<<endl;
                        ET_pas_position_trajectoire*=2.;cout<<"Pas trajectoire="<<ET_pas_position_trajectoire<<endl;
                        break;
                    case GLFW_KEY_ENTER: //ENTREE
                        ma_trajectoire->ajoute_point(ET_x,ET_y,ET_z);
                        ET_point_actuel=ma_trajectoire->renvoie_nombre_points()-1;
                        break;
                    case GLFW_KEY_DELETE: //SUPPR
                        ma_trajectoire->supprime_point(ET_point_actuel);
                        if (ET_point_actuel>=ma_trajectoire->renvoie_nombre_points())ET_point_actuel=ma_trajectoire->renvoie_nombre_points()-1;
                        break;
                    case GLFW_KEY_Q:
                        ma_trajectoire->deplace_point(ET_point_actuel,-ET_vitesse_deplacement,0.,0.);
                        ma_trajectoire->affiche_position_point(ET_point_actuel);
                        break;
                    case GLFW_KEY_S:
                        ma_trajectoire->deplace_point(ET_point_actuel,ET_vitesse_deplacement,0.,0.);
                        ma_trajectoire->affiche_position_point(ET_point_actuel);
                        break;
                    case GLFW_KEY_A:
                        ma_trajectoire->deplace_point(ET_point_actuel,0.,ET_vitesse_deplacement,0.);
                        ma_trajectoire->affiche_position_point(ET_point_actuel);
                        break;
                    case GLFW_KEY_W:
                        ma_trajectoire->deplace_point(ET_point_actuel,0.,-ET_vitesse_deplacement,0.);
                        ma_trajectoire->affiche_position_point(ET_point_actuel);
                        break;
                    case GLFW_KEY_Z:
                        ma_trajectoire->deplace_point(ET_point_actuel,0.,0.,-ET_vitesse_deplacement);
                        ma_trajectoire->affiche_position_point(ET_point_actuel);
                        break;
                    case GLFW_KEY_X:
                        ma_trajectoire->deplace_point(ET_point_actuel,0.,0.,ET_vitesse_deplacement);
                        ma_trajectoire->affiche_position_point(ET_point_actuel);
                        break;
                    case GLFW_KEY_KP_ENTER:
                        ma_trajectoire->determine_point(ET_point_actuel,ET_x,ET_y,ET_z);
                        break;
                    case GLFW_KEY_C:
                        ET_position_trajectoire-=ET_pas_position_trajectoire;
                        cout<<"Position trajectoire t="<<ET_position_trajectoire;
                        ma_trajectoire->renvoie_position(ET_position_trajectoire,coordonnees_trajectoire);
                        ET_x=coordonnees_trajectoire[0];
                        ET_y=coordonnees_trajectoire[1];
                        ET_z=coordonnees_trajectoire[2];
                        cout<<" Coordonnées X:"<<coordonnees_trajectoire[0]<<" Y:"<<coordonnees_trajectoire[1]<<" Z:"<<coordonnees_trajectoire[2]<<endl;
                        break;
                    case GLFW_KEY_V:
                        ET_position_trajectoire+=ET_pas_position_trajectoire;
                        cout<<"Position trajectoire t="<<ET_position_trajectoire;
                        ma_trajectoire->renvoie_position(ET_position_trajectoire,coordonnees_trajectoire);
                        ET_x=coordonnees_trajectoire[0];
                        ET_y=coordonnees_trajectoire[1];
                        ET_z=coordonnees_trajectoire[2];
                        cout<<" Coordonnées X:"<<coordonnees_trajectoire[0]<<" Y:"<<coordonnees_trajectoire[1]<<" Z:"<<coordonnees_trajectoire[2]<<endl;
                        break;
                    case GLFW_KEY_P:
                        cout<<"Position éditée: "<<ET_x<<","<<ET_y<<","<<ET_z<<","
                                                <<ET_r<<","<<ET_t<<","<<ET_l<<endl;
                        break;

                    case GLFW_KEY_KP_ADD:
                        ET_point_actuel++;
                        if (ET_point_actuel>=ma_trajectoire->renvoie_nombre_points())ET_point_actuel=ma_trajectoire->renvoie_nombre_points()-1;
                        ma_trajectoire->affiche_position_point(ET_point_actuel);
                        break;

                    case GLFW_KEY_KP_SUBTRACT:
                        if (ET_point_actuel!=0)ET_point_actuel--;
                        ma_trajectoire->affiche_position_point(ET_point_actuel);
                        break;

                    //Touches F..
                    case GLFW_KEY_F1:
                        if (ma_trajectoire->sauve_trajectoire(ma_trajectoire->nom)) cout<<"Trajectoire "<<ma_trajectoire->nom<<" sauvée avec succès!"<<endl;
                        break;

                    case GLFW_KEY_F2:
                        if (ma_trajectoire->charge_trajectoire(ma_trajectoire->nom))
                        {
                            cout<<"Trajectoire "<<ma_trajectoire->nom<<" chargée avec succès!"<<endl;
                            ET_point_actuel=0;
                        }
                        break;
                    //Gestion des trajectoires:
                    case GLFW_KEY_TAB:
                        trajectoire_courante++;
                        if(trajectoire_courante>=liste_trajectoires.size()) trajectoire_courante=0;
                        ma_trajectoire=liste_trajectoires[trajectoire_courante];
                        cout<<"Trajectoire courante: "<<ma_trajectoire->nom<<endl;

                    default:
                        break;
        }
    }
}
