#ifndef TIMELINE_INCLUDED
#define TIMELINE_INCLUDED

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include "math.h"

#include "Variables.h"
#include "Musique.h"

#include "fonctions.h"
#include "Intro.h"
#include "waterworld.h"
#include "mandelbrot.h"
#include "spheredance.h"
#include "hypnostructure.h"
#include "Final.h"
#include "Ville/Ville.h"
#include "editeur_trajectoire.h"
#include "repere.h"
#include "chargeurs_images/Image.h"
#include "Interpolations.h"
#include "Trajectoire.h"

#define EDIT_VILLE false

static float hs_beat=0.4;//GVars::beat;

static bool drapeau_scene_Intro=true;
static bool drapeau_scene_Waterworld=false;
static bool drapeau_scene_Ville=false;
static bool drapeau_scene_Mandelbrot=false;
static bool drapeau_scene_Spheresdance=false;
static bool drapeau_scene_Hypnostructure=false;
static bool drapeau_scene_Fin=false;


static bool drapeau_init_scene_Intro=false;
static bool drapeau_init_scene_Waterworld=false;
static bool drapeau_init_scene_Ville=false;
static bool drapeau_init_scene_Mandelbrot=false;
static bool drapeau_init_scene_Spheredance=false;
static bool drapeau_init_scene_Hypnostructure=false;
static bool drapeau_init_scene_Fin=false;



static double scene_Intro_t0=0.;
static double scene_Waterworld_t0=scene_Intro_t0+6*16*GVars::beat;
static double scene_Ville_t0=scene_Waterworld_t0+(41*16)*GVars::beat;
static double scene_Mandelbrot_t0=scene_Ville_t0+14*16*GVars::beat;
static double scene_Spheredance_t0=scene_Mandelbrot_t0+(13.5*16+1.6)*GVars::beat;
static double scene_Hypnostructure_t0=scene_Spheredance_t0+6*16*GVars::beat;
#if VERSION_LONGUE
static double scene_Fin_t0=scene_Hypnostructure_t0+15*16*hs_beat;
#else
static double scene_Fin_t0=scene_Spheredance_t0+6*16*GVars::beat;
#endif

static bool etapeInit=false;
static bool rotation_S7=true;
static bool drapeau_shotFractale=false;

static float res2f[2];
static float res3f[3];
static double res2d[2];
static double res3d[3];
static uint32_t couleur1,couleur2,couleur3;


//===================================================
//  Clés d'animation de la scène d'introduction
//===================================================
    static Bruit intro_bruit_alphaFond;
    static Bruit intro_bruit_alphaFond2;
    static Interpolation_Parametre intro_fadeIn_fond(8*GVars::beat,0.,1.,1*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre intro_fadeIn_etoiles(4*GVars::beat,0.,1.,1*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre intro_fadeIn_logo(0.25*16*GVars::beat,0.,1.,1*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre intro_fadeOut_logo(1.25*16*GVars::beat,1.,0.,1*GVars::beat,INTERPOLATION_LINEAIRE);
    static float intro_oeil_e_01[2]={1.,1.};
    static float intro_oeil_e_02[2]={40./30.,8./31.};
    static Interpolation_Parametre_2f intro_clindoeil_01(1.*16*GVars::beat,intro_oeil_e_01,intro_oeil_e_02,0.5*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre_2f intro_clindoeil_02((1.*16+0.5)*GVars::beat,intro_oeil_e_02,intro_oeil_e_01,0.5*GVars::beat,INTERPOLATION_COSINUS);



    static float intro_cosmoXYZ_01[3]={-5.23291,11.6,-3.77499};
    static float intro_cosmoXYZ_02[3]={-5.23291,5.55686,-3.77499};
    static Interpolation_Parametre_3f intro_animCosmo_xyz_01(((1*16))*GVars::beat,intro_cosmoXYZ_01,intro_cosmoXYZ_02,1*16*GVars::beat,INTERPOLATION_COSINUS);
    static Trajectoire intro_trajectoire_cosmo01;
    static Courbe_Bezier_Plan intro_cosmoCourbe_01(0,0, .5,0., 1.,0., 1.,1.);
    static Interpolation_Parametre intro_animCosmo_traj01(2*16*GVars::beat,0.,1.,4*16*GVars::beat,INTERPOLATION_BEZIER,&intro_cosmoCourbe_01);

    static float intro_cosmoRTL_01[3]={219.955,0.,-4.79001};
    static float intro_cosmoRTL_02[3]={650,180,-180};
    static Interpolation_Parametre_3f intro_animCosmo_rtl_01(2*16*GVars::beat,intro_cosmoRTL_01,intro_cosmoRTL_02,8*16*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre intro_animCosmo_alphaTronc(2*16*GVars::beat,0.1,.75,4*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre intro_animFadeIn(2*GVars::beat,0.,1.,1*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre intro_animFadeOut(5*16*GVars::beat,1.,0.,1*16*GVars::beat,INTERPOLATION_LINEAIRE);

//===================================================
//  Clés d'animation de la scène WaterWorld
//===================================================
//Introduction - Sphère décolle au loin

    static Interpolation_Parametre ww_animFadeIn(0*GVars::beat,0.,1.,8*GVars::beat,INTERPOLATION_LINEAIRE);
    static float ww_rtl0[3]={WaterWorld::obsR0,WaterWorld::obsT0,WaterWorld::obsL0};
    static float ww_rtl1[3]={-90.,10.4,0.};
    static Interpolation_Parametre_3f ww_animObs_rtl_01(0.,ww_rtl0,ww_rtl1,8*16*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre ww_animL1_i_01(0.,0.,1.,8*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static float ww_L1_xyz0[3]={-30,120.,150.};//{-50,80.,1.};
    static float ww_L1_xyz1[3]={-4.5,180.,370.};
    static Interpolation_Parametre_3f ww_animL1_xyz_01(0.,ww_L1_xyz0,ww_L1_xyz1,8*16*GVars::beat,INTERPOLATION_COSINUS);

    static Trajectoire ww_trajectoire_cosmo02;
    //static Courbe_Bezier_Plan intro_cosmoCourbe_01(0,0, .5,0., 1.,0., 1.,1.);
    static Interpolation_Parametre ww_animCosmo_traj02(0*GVars::beat,0.,1.,8*16*GVars::beat,INTERPOLATION_COSINUS);

    //Titre

    static Interpolation_Parametre ww_animL1_i_02(8*16*GVars::beat,1.,0.,4*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre ww_animS1_transp_01(8*16*GVars::beat,0.,1.,4*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre ww_animS1_refrac_01(8*16*GVars::beat,1.,.3,4*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre ww_fadeIn_Interstase(8*16*GVars::beat,0.,1.,1*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre ww_fadeOut_Interstase(10*16*GVars::beat,1.,0.,1*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static Courbe_Bezier_Plan ww_bezier_zoom_Interstase(0,0, 0.33,0., 1.,.66, 1.,1.);
    static Interpolation_Parametre ww_zoom_Interstase(8*16*GVars::beat,1.,1.3,3*16*GVars::beat,INTERPOLATION_BEZIER,&ww_bezier_zoom_Interstase);


    //Vers la planète Réfractus
    static Trajectoire ww_trajectoire_cosmo03;
    //static Courbe_Bezier_Plan intro_cosmoCourbe_01(0,0, .5,0., 1.,0., 1.,1.);
    static Interpolation_Parametre ww_animCosmo_traj03(10*16*GVars::beat,0.,1.,6*16*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre ww_animS1_y_01(8*16*GVars::beat+4*GVars::beat,WaterWorld::yS1,228,12*GVars::beat+3*16*GVars::beat,INTERPOLATION_COSINUS);
    static float ww_rtl2[3]={-90.,-90.,0.};
    static Interpolation_Parametre_3f ww_animObs_rtl_02(9*16*GVars::beat,ww_rtl1,ww_rtl2,4*16*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre ww_animObs_z_01(8*16*GVars::beat+4*GVars::beat,WaterWorld::obsZ,WaterWorld::zS1,12*GVars::beat+4*16*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre ww_animL1_i_03(10*16*GVars::beat,0.,1.,2*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre ww_animSol_reflec_01(8*16*GVars::beat+4*GVars::beat,0.75,0.,12*GVars::beat+1*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static float ww_L1_xyz2[3]={-2.,250.,306.};
    static Interpolation_Parametre_3f ww_animL1_xyz_02(10*16*GVars::beat,ww_L1_xyz1,ww_L1_xyz2,2*16*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre_rvba ww_couleurSol_01a(12*16*GVars::beat,0x33b366ff,0xffff00ff,6*16*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre_rvba ww_couleurSol_01b(12*16*GVars::beat,0x1a5933ff,0xff9a22ff,6*16*GVars::beat,INTERPOLATION_COSINUS);

    static Interpolation_Parametre_rvba ww_couleurCiel_01z(12*16*GVars::beat,0x11389bff,0xbf3000ff,6*16*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre_rvba ww_couleurCiel_01h(12*16*GVars::beat,0x4c80ffff,0xff5e54ff,6*16*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre_rvba ww_couleurCiel_01n(12*16*GVars::beat,0x132759ff,0xff8f7dff,6*16*GVars::beat,INTERPOLATION_COSINUS);

    //Horizon avec boules fantomatiques et ovni qui passe:
    static Interpolation_Parametre ww_anim_S7_angle_t0_01(13*16*GVars::beat,WaterWorld::S7_angle_t0,3.09089,3*16*GVars::beat,INTERPOLATION_COSINUS);
    static float ww_L1_xyz3[3]={0.,423.4,305.7};
    static Interpolation_Parametre_3f ww_animL1_xyz_03(16*16*GVars::beat,ww_L1_xyz2,ww_L1_xyz3,2*16*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre ww_animObs_y_01(13*16*GVars::beat,WaterWorld::obsY,228.104,8*16*GVars::beat,INTERPOLATION_COSINUS);
    static float ww_carXY1[2]={10.,10.};
    static float ww_carXY2[2]={100.,100.};
    static Interpolation_Parametre_2f ww_animSol_carXY_01(19*16*GVars::beat,ww_carXY1,ww_carXY2,2*16*GVars::beat,INTERPOLATION_COSINUS);
    static float ww_rtl3[3]={0,-192.586,-180.};
    static Interpolation_Parametre_3f ww_animObs_rtl_03(17*16*GVars::beat,ww_rtl2,ww_rtl3,4*16*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre ww_animBrouillard_dist_01(17*16*GVars::beat,500.,5000.,4*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static float ww_L1_xyzVilleDepart[3]={-5100.,755.,-3000.};
    static Interpolation_Parametre_3f ww_animL1_xyz_villeDepart(18*16*GVars::beat,ww_L1_xyz3,ww_L1_xyzVilleDepart,3*16*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre_rvba ww_couleurCiel_02z(18*16*GVars::beat,0xbf3000ff,0xff5e54ff,3*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre_rvba ww_couleurCiel_02h(18*16*GVars::beat,0xff5e54ff,0x4c80ffff,3*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre_rvba ww_couleurCiel_02n(18*16*GVars::beat,0xff8f7dff,0xeddd1bff,3*16*GVars::beat,INTERPOLATION_LINEAIRE);

    //Ville volante qui passe:
    static float ww_L1_xyzVilleMove[3]={5100.,755.,-3000.};
    static Interpolation_Parametre_3f ww_animL1_xyz_villeMove(21*16*GVars::beat,ww_L1_xyzVilleDepart,ww_L1_xyzVilleMove,4*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre ww_animAgnomes_show_01(21*16*GVars::beat,0.,1.,4*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre ww_animAgnomes_hide_01(24*16*GVars::beat,0.,1.,1*16*GVars::beat,INTERPOLATION_LINEAIRE);

    //Gros cerveau interdimensionnel de Edora:

    static float ww_cosmo_xyz1[3]={-3.41354,167.923,-418.01};
    static float ww_cosmo_xyz2[3]={-3.41354,167.923,-357.894};
    static Courbe_Bezier_Plan ww_bezier_cosmo_xyz_01(0,0, 0.33,0., 1.,.66, 1.,1.);
    static Interpolation_Parametre_3f ww_animCosmo_xyz_01(25*16*GVars::beat,ww_cosmo_xyz1,ww_cosmo_xyz2,2*16*GVars::beat,INTERPOLATION_BEZIER,&ww_bezier_cosmo_xyz_01);

    static Trajectoire ww_trajectoire_cosmo04;
    //static Courbe_Bezier_Plan intro_cosmoCourbe_01(0,0, .5,0., 1.,0., 1.,1.);
    static Interpolation_Parametre ww_animCosmo_traj04(32*16*GVars::beat,0.,1.,(8*16+8.)*GVars::beat,INTERPOLATION_COSINUS);


    static Interpolation_Parametre_rvba ww_couleurSol_03a(25*16*GVars::beat,0xffff00ff,0xffff0000,2*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre_rvba ww_couleurSol_03b(25*16*GVars::beat,0xff9a22ff,0xff8000ff,2*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre_3f ww_animL1_xyz_retourVille(25*16*GVars::beat,ww_L1_xyzVilleMove,ww_L1_xyz3,1*16*GVars::beat,INTERPOLATION_COSINUS);



    static float ww_obs_xyz1[3]={WaterWorld::obsX,228.104,WaterWorld::zS1};//{-50,80.,1.};
    static float ww_obs_xyz2[3]={0.,255.822,497.87};//4.19194
    static Interpolation_Parametre_3f ww_animObs_xyz_01(25*16*GVars::beat,ww_obs_xyz1,ww_obs_xyz2,8*16*GVars::beat,INTERPOLATION_COSINUS);
    static float ww_rtl4[3]={0,-159.397,-180.};
    static Interpolation_Parametre_3f ww_animObs_rtl_04(27*16*GVars::beat,ww_rtl3,ww_rtl4,6*16*GVars::beat,INTERPOLATION_COSINUS);

    static Interpolation_Parametre ww_animSol_reflec_02(25*16*GVars::beat,0.,0.75,4*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre ww_animS1_transp_02(27*16*GVars::beat,1.,0.8,4*16*GVars::beat,INTERPOLATION_LINEAIRE);


    static Interpolation_Parametre ww_animGlow_saturation_halo_01((27*16)*GVars::beat,1.,0.6,(6*16)*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre ww_animGlow_saturation_halo_02((33*16)*GVars::beat,0.6,1.,(3*16)*GVars::beat,INTERPOLATION_LINEAIRE);
    static Courbe_Bezier_Plan ww_bezier_edora_y_01(0,0, 0.,.85, 0.75,1., 1.,1.);
    static Courbe_Bezier_Plan ww_bezier_edora_e_01(0,0, 0.,1., 0.50,1., 1.,1.);
    static Interpolation_Parametre ww_animEdora_y_01((27*16)*GVars::beat,-1.8,0.572222,(6*16)*GVars::beat,INTERPOLATION_BEZIER,&ww_bezier_edora_y_01);
    static Interpolation_Parametre ww_animEdora_e_01((27*16)*GVars::beat,14.,1.,(6*16)*GVars::beat,INTERPOLATION_BEZIER,&ww_bezier_edora_e_01);
    static Interpolation_Parametre ww_animEdora_alpha_01((28*16.+8.)*GVars::beat,0.0,0.5,(3.*16.)*GVars::beat,INTERPOLATION_LINEAIRE);

    static Courbe_Bezier_Plan ww_bezier_edora_y_02(0,0, 0.5,.0, 1.,.9, 1.,1.);
    static Interpolation_Parametre ww_animEdora_y_02((33*16)*GVars::beat,0.572222,3.5,(6*16)*GVars::beat,INTERPOLATION_COSINUS,&ww_bezier_edora_y_02);
    static Interpolation_Parametre ww_animEdora_alpha_02((37*16)*GVars::beat,0.5,0.,(2*16)*GVars::beat,INTERPOLATION_LINEAIRE);

    static float ww_obs_xyz3[3]={0.,259.345,418.528};//4.19194
    static Interpolation_Parametre_3f ww_animObs_xyz_02(34*16*GVars::beat,ww_obs_xyz2,ww_obs_xyz3,2*16*GVars::beat,INTERPOLATION_COSINUS);
    static float ww_rtl5[3]={0.,-9.5,-180.};
    static Interpolation_Parametre_3f ww_animObs_rtl_05(33*16*GVars::beat,ww_rtl4,ww_rtl5,6*16*GVars::beat,INTERPOLATION_COSINUS);

    static Interpolation_Parametre ww_animS1_y_02(36*16*GVars::beat,228.,150.,2*16*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre ww_animObs_y_02(37*16*GVars::beat,259.345,10.,2*16*GVars::beat,INTERPOLATION_COSINUS);
    static float ww_carXY3[2]={5.,500.};
    static Interpolation_Parametre_2f ww_animSol_carXY_02(36*16*GVars::beat,ww_carXY2,ww_carXY3,5*16*GVars::beat,INTERPOLATION_COSINUS);

    static Interpolation_Parametre_rvba ww_couleurSol_02a(39*16*GVars::beat,0xffff0000,0xf9001eff,4*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre_rvba ww_couleurSol_02b(39*16*GVars::beat,0xff8000ff,0xff8080ff,4*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre_rvba ww_couleurCiel_03z(39*16*GVars::beat,0xff5e54ff,0x33b366ff,4*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre_rvba ww_couleurCiel_03h(39*16*GVars::beat,0x4c80ffff,0x17d722ff,4*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre_rvba ww_couleurCiel_03n(39*16*GVars::beat,0xeddd1bff,0x006837ff,4*16*GVars::beat,INTERPOLATION_LINEAIRE);

    static Interpolation_Parametre ww_animS1_transp_03(39*16*GVars::beat,.8,1.,1*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre ww_animS1_refrac_02(39*16*GVars::beat,.3,0.1,1*16*GVars::beat,INTERPOLATION_COSINUS);
    //static float ww_L1_xyz4[3]={-14.8055,272.55,178.977};
    static float ww_L1_xyz4[3]={592.249,144.293,474.168};
    static Interpolation_Parametre_3f ww_animL1_xyz_04(39*16*GVars::beat,ww_L1_xyz3,ww_L1_xyz4,4*16*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre ww_animBrouillard_dist_02(39*16*GVars::beat,5000.,600.,2*16*GVars::beat,INTERPOLATION_COSINUS);

    static Interpolation_Parametre_rvba ww_couleurLum_01(39*16*GVars::beat,0xffffffff,0xffff40ff,4*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre_rvba ww_couleurAmb_01(37*16*GVars::beat,0x4c80ffff,0xffff40ff,4*16*GVars::beat,INTERPOLATION_LINEAIRE);


    static float ww_S1_xyz1[3]={WaterWorld::xS1,150.,WaterWorld::zS1};
    static float ww_S1_xyz2[3]={0.,202.,202.5};
    static Interpolation_Parametre_3f ww_animS1_xyz_01(39*16*GVars::beat,ww_S1_xyz1,ww_S1_xyz2,(1*16+8.)*GVars::beat,INTERPOLATION_COSINUS);
    static float ww_obs_xyz4[3]={0.,10.,418.528};//4.19194
    //static float ww_obs_xyz5[3]={0.,10.,251.383};
    static float ww_obs_xyz5[3]={0.,10.,230.};
    static Interpolation_Parametre_3f ww_animObs_xyz_03(39*16*GVars::beat,ww_obs_xyz4,ww_obs_xyz5,(1*16+8.)*GVars::beat,INTERPOLATION_COSINUS);
    static float ww_rtl6[3]={0.,15.,-180.};
    static Interpolation_Parametre_3f ww_animObs_rtl_06(39*16*GVars::beat,ww_rtl5,ww_rtl6,(1*16+8.)*GVars::beat,INTERPOLATION_COSINUS);

    static Interpolation_Parametre ww_animSol_reflec_03(39*16*GVars::beat,0.75,0.,1*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre ww_animSol_transp_01(39*16*GVars::beat,0.5,1.,1*16*GVars::beat,INTERPOLATION_LINEAIRE);



    static float ww_cosmo_rtl1[3]={0.,0.,0.};   //Raccord calculé à la volée
    static float ww_cosmo_rtl2[3]={0.,90.,90.};
    static Interpolation_Parametre_3f ww_animCosmo_rtl_01(40*16*GVars::beat,ww_cosmo_rtl1,ww_cosmo_rtl2,1.*16*GVars::beat,INTERPOLATION_COSINUS);

    //Basculement:
    static Courbe_Bezier_Plan ww_bezier_rot_obs(0,0, 0.5,0., 1.,0.9, 1.,1.);
    static Interpolation_Parametre ww_rot_obs_t((40.*16+8)*GVars::beat,WaterWorld::obs.t,WaterWorld::obs.t+180.,8*GVars::beat,INTERPOLATION_BEZIER,&ww_bezier_rot_obs);
    static Interpolation_Parametre ww_rot_obs_angle_trajectoire((40.*16+8)*GVars::beat,0.,1.,8.*GVars::beat,INTERPOLATION_BEZIER,&ww_bezier_rot_obs);

    static float ww_cosmo_xyz3[3]={1.96406,7.42375,-201.278};
    static float ww_cosmo_xyz4[3]={1.96406,0.,-201.278};
    static Interpolation_Parametre_3f ww_animCosmo_xyz_02((40*16+8)*GVars::beat,ww_cosmo_xyz3,ww_cosmo_xyz4,8.*GVars::beat,INTERPOLATION_COSINUS);


    //////////////// Le reste est obsolète (coupé au montage :)

    /*
    static Trajectoire ww_trajectoire_cosmo05;
    static Courbe_Bezier_Plan ww_bezier_cosmo_xyz_02(0,0, 0.12,0., .7,1., 1.,1.);
    static Interpolation_Parametre ww_animCosmo_traj05(40*16*GVars::beat,0.,1.,11*16*GVars::beat,INTERPOLATION_BEZIER,&ww_bezier_cosmo_xyz_02);

    static float ww_rtl7[3]={-338.244,56.8643,83.162};
    static Interpolation_Parametre_3f ww_animObs_rtl_07(41*16*GVars::beat,ww_rtl6,ww_rtl7,4*16*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre ww_animSol_reflec_04(41*16*GVars::beat,0.,.8,2*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre ww_animSol_transp_02(41*16*GVars::beat,1.,0.1,2*16*GVars::beat,INTERPOLATION_LINEAIRE);

    static float ww_obs_xyz6[3]={35.7653,1.09145,32.4679};
    static Interpolation_Parametre_3f ww_animObs_xyz_04(41*16*GVars::beat,ww_obs_xyz5,ww_obs_xyz6,4*16*GVars::beat,INTERPOLATION_COSINUS);


    static float ww_obs_xyz7[3]={97.2588,8.62568,41.7656};
    static float ww_obs_rtl8[3]={-352.175,-45.0959,283.382};
    static Interpolation_Parametre_3f ww_animObs_xyz_05(45*16*GVars::beat,ww_obs_xyz6,ww_obs_xyz7,4*16*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre_3f ww_animObs_rtl_08(45*16*GVars::beat,ww_rtl7,ww_obs_rtl8,4*16*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre ww_animS1_transp_04(45*16*GVars::beat,1.,.5,2*16*GVars::beat,INTERPOLATION_LINEAIRE);

    //====================== Retour dans l'espace, autour de la grosse planète rouge:
    static Interpolation_Parametre ww_animGlow_alpha_02(49*16*GVars::beat,.5,1.,1*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre ww_animGlow_alpha_03(50*16*GVars::beat,1.,0.,1*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static float ww_cosmo_xyz3[3]={30.2122,66.2208,-66.108};
    static float ww_cosmo_xyz4[3]={-78.9915,913.182,-656.354};
    static Courbe_Bezier_Plan ww_bezier_cosmo_xyz_03(0,0, 1.,0., 1.,0., 1.,1.);
    static Interpolation_Parametre_3f ww_animCosmo_xyz_02(49*16*GVars::beat,ww_cosmo_xyz3,ww_cosmo_xyz4,1*16*GVars::beat,INTERPOLATION_BEZIER,&ww_bezier_cosmo_xyz_03);

    static float ww_obs_xyz8[3]={57.9412,250.913,-356.397};
    static float ww_obs_rtl9[3]={-339.829,3.70437,360.143};
    //static Interpolation_Parametre ww_animObs_traj01(50*16*GVars::beat,0.,1.,5*16*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre_3f ww_animObs_xyz_06(51*16*GVars::beat,ww_obs_xyz7,ww_obs_xyz8,4*16*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre_3f ww_animObs_rtl_09(50*16*GVars::beat,ww_obs_rtl8,ww_obs_rtl9,5*16*GVars::beat,INTERPOLATION_COSINUS);

    static Interpolation_Parametre ww_animSol_reflec_05(50*16*GVars::beat,.8,0.,3*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre ww_animSol_transp_03(50*16*GVars::beat,.1,1.,3*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre ww_animSol_spec_01(50*16*GVars::beat,.25,0.,3*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre ww_animS1_transp_05(50*16*GVars::beat,.5,0.,3*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre ww_animS1_reflec_01(50*16*GVars::beat,1.,0.,3*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre ww_animS1_spec_01(50*16*GVars::beat,0.5,0.,3*16*GVars::beat,INTERPOLATION_LINEAIRE);
    //static Interpolation_Parametre ww_animS1_brill_01(50*16*GVars::beat,20.,1.,3*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre ww_animS1_refrac_03(50*16*GVars::beat,0.1,1.,1*16*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre ww_animSol_amplVagues_01(50*16*GVars::beat,1.,0.,3*16*GVars::beat,INTERPOLATION_LINEAIRE);

    static Interpolation_Parametre ww_animS2_r_01((50*16)*GVars::beat,WaterWorld::rS2,0.001,1*16*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre ww_animS3_r_01((50*16+2)*GVars::beat,WaterWorld::rS3,0.001,1*16*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre ww_animS4_r_01((50*16+4)*GVars::beat,WaterWorld::rS4,0.001,1*16*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre ww_animS5_r_01((50*16+6)*GVars::beat,WaterWorld::rS5,0.001,1*16*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre ww_animS6_r_01((50*16+8)*GVars::beat,WaterWorld::rS6,0.001,1*16*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre ww_animS7_r_01((50*16+10)*GVars::beat,WaterWorld::rS7,0.001,1*16*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre ww_animS8_r_01((50*16+12)*GVars::beat,WaterWorld::rS8,0.001,1*16*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre ww_animS9_r_01((50*16+14)*GVars::beat,WaterWorld::rS9,0.001,1*16*GVars::beat,INTERPOLATION_COSINUS);

    static Interpolation_Parametre ww_animS2_r_02((52*16)*GVars::beat,.001,WaterWorld::rS2*2,1*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre ww_animS3_r_02((52*16)*GVars::beat,.001,WaterWorld::rS3*2,1*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre ww_animS4_r_02((52*16)*GVars::beat,.001,WaterWorld::rS4*2,1*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre ww_animS5_r_02((52*16)*GVars::beat,.001,WaterWorld::rS5*2,1*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre ww_animS6_r_02((52*16)*GVars::beat,.001,WaterWorld::rS6*2,1*16*GVars::beat,INTERPOLATION_LINEAIRE);


    static Interpolation_Parametre_rvba ww_couleurLum_02(50*16*GVars::beat,0xffff40ff,0xffffffff,4*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre_rvba ww_couleurAmb_02(50*16*GVars::beat,0xffff40ff,0x4c80ffff,4*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static float ww_L1_xyz5[3]={-404.292,266.31,-289.061};
    static Interpolation_Parametre_3f ww_animL1_xyz_05(50*16*GVars::beat,ww_L1_xyz4,ww_L1_xyz5,2*16*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre ww_animNuees_alpha_01(53*16*GVars::beat,0.,1.,1*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre ww_animL1_i_04(53*16*GVars::beat,1.,0.,2*16*GVars::beat,INTERPOLATION_LINEAIRE);

    static Interpolation_Parametre_rvba ww_couleurCiel_04z(50*16*GVars::beat,0x33b366ff,0x000000ff,5*16*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre_rvba ww_couleurCiel_04h(50*16*GVars::beat,0x17d722ff,0x11389bff,5*16*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre_rvba ww_couleurCiel_04n(50*16*GVars::beat,0x006837ff,0x000000ff,5*16*GVars::beat,INTERPOLATION_COSINUS);
    */

//==================================================================================
//=============== Clés d'animation de la scène 2: Ville (Fractalopolis)
//==================================================================================
    static vec3f vl_position_basculement(-21716.6,0.,-5475.08);

    static Courbe_Bezier_Plan vl_bezier_rot_obs(0.,0, 0.,0.5, .5,1., 1.,1.);
    static Interpolation_Parametre vl_rot_obs_angle_trajectoire(0.*GVars::beat,0.,1.,8.*GVars::beat,INTERPOLATION_BEZIER,&vl_bezier_rot_obs);
    static Interpolation_Parametre vl_point_vise_y_01(0.*GVars::beat,0.,1.05744,8.*GVars::beat,INTERPOLATION_COSINUS);

    static float vl_cosmo_xyz1[3]={vl_position_basculement.x,0.,vl_position_basculement.z};
    static float vl_cosmo_xyz2[3]={vl_position_basculement.x,1.2,vl_position_basculement.z};
    static Courbe_Bezier_Plan vl_bezier_cosmo_xyz_01(0.,0, 0.,0.25, .75,1., 1.,1.);
    static Interpolation_Parametre_3f vl_animCosmo_xyz_01(0.*GVars::beat,vl_cosmo_xyz1,vl_cosmo_xyz2,16*GVars::beat,INTERPOLATION_BEZIER,&vl_bezier_cosmo_xyz_01);

    static float vl_cosmo_rtl1[3]={0.,0.,0.};
    static float vl_cosmo_rtl2[3]={0.,0.,0.};
    static Interpolation_Parametre_3f vl_animCosmo_rtl_01(8.*GVars::beat,vl_cosmo_rtl1,vl_cosmo_rtl2,(1*16.)*GVars::beat,INTERPOLATION_COSINUS);

    static Interpolation_Parametre vl_animCosmo_mix_param((8.)*GVars::beat,1.,0.2,(1*16)*GVars::beat,INTERPOLATION_COSINUS);

    static Trajectoire vl_trajectoire_cosmo01;
    static Courbe_Bezier_Plan vl_courbe_cosmo_traj_01(0,0, .5,0., .5,1., 1.,1.);
    static Interpolation_Parametre vl_animCosmo_traj01(1*16*GVars::beat,0.,1.,(12*16)*GVars::beat,INTERPOLATION_BEZIER,&vl_courbe_cosmo_traj_01);

    static Trajectoire vl_trajectoire_obs01;
    static Courbe_Bezier_Plan vl_courbe_obs_traj_01(0,0, .5,0., .6,1., 1.,1.);
    static Interpolation_Parametre vl_animObs_traj01((1*16.+3)*GVars::beat,0.,1.,(12*16+8)*GVars::beat,INTERPOLATION_BEZIER,&vl_courbe_obs_traj_01);



//==================================================================================
//=============== Clés d'animation de la scène 3: Mandelbrot (Fractale cosmique)
//==================================================================================
    //Fondu_enchainé:
    static Interpolation_Parametre mb_alpha_screenShot(0.*16*GVars::beat,1.,0.,8*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre mb_rot_01(0.*16*GVars::beat,M_PI/180.*30.,M_PI/180.*90.,5*16*GVars::beat,INTERPOLATION_COSINUS);
    //Entrée dans la nébuleuse fractale:
    static float mb_cosmo_xyz1[3]={0.,0.,-2000};
    static float mb_cosmo_xyz2[3]={13.8325,-7.03029,-33.6643};
    static Courbe_Bezier_Plan mb_bezier_cosmo_xyz_00(0,0, 0.,1., 0,1., 1.,1.);
    static Interpolation_Parametre_3f mb_animCosmo_xyz_01(8*GVars::beat,mb_cosmo_xyz1,mb_cosmo_xyz2,16*GVars::beat,INTERPOLATION_BEZIER,&mb_bezier_cosmo_xyz_00);

    static Trajectoire mb_trajectoire_cosmo01;
    static Courbe_Bezier_Plan mb_bezier_cosmo_xyz_01(0,0, 1/3.,0., 2/3.,1., 1.,1.);
    static Interpolation_Parametre mb_animCosmo_traj01(2.*16*GVars::beat,0.,1.,6.*16*GVars::beat,INTERPOLATION_BEZIER,&mb_bezier_cosmo_xyz_01);

    static Trajectoire mb_trajectoire_cosmo02;
    static Courbe_Bezier_Plan mb_bezier_cosmo_xyz_02(0,0, 1.,0., 2/3.,1., 1.,1.);
    static Interpolation_Parametre mb_animCosmo_traj02(8.*16*GVars::beat,0.,1.,10.*16*GVars::beat,INTERPOLATION_BEZIER,&mb_bezier_cosmo_xyz_02);

    //static double mb_xyh_01[3]={Mandelbrot::xpos,Mandelbrot::ypos,Mandelbrot::zoom};
    static double mb_xyh_01[3]={0.0297733,0.0071582,610.3};
    static double mb_xyh_02[3]={-0.750914,-0.046051,931.3};
    static Interpolation_Parametre_3d mb_animObs_xyh_01(8.*GVars::beat,mb_xyh_01,mb_xyh_02,.5*16*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre mb_animGlow_alpha_01(8.*GVars::beat,0.,1.,4.*GVars::beat,INTERPOLATION_LINEAIRE);
    //Apparition de la fractale:
    static Interpolation_Parametre mb_animIter_01(1.*16*GVars::beat,2.,300.,1*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static Interpolation_Parametre mb_animPalette_01(0.*16*GVars::beat,0.,-2.,5*16*GVars::beat,INTERPOLATION_COSINUS);
    //Déformation:
    static double mb_xyh_03[3]={-0.721663,-0.00170505,1166.79};
    static Interpolation_Parametre_3d mb_animObs_xyh_02(2*16*GVars::beat,mb_xyh_02,mb_xyh_03,2*16*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre mb_animT_01(2.*16*GVars::beat,1.,0.93,3*16*GVars::beat,INTERPOLATION_COSINUS);
    //Zoom arrière:
    static Interpolation_Parametre mb_animIter_02(4.*16*GVars::beat,300.,41.,3*16*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre mb_animZoom_01(4.*16*GVars::beat,1166.79,800.,3*16*GVars::beat,INTERPOLATION_COSINUS);
    static double mb_xy_01[2]={-0.721663,-0.00170505};
    static double mb_xy_02[2]={-0.69843676267345012,0.32952823843280116};
    static Interpolation_Parametre_2d mb_animObs_xy_01(6*16*GVars::beat,mb_xy_01,mb_xy_02,4*16*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre mb_animT_02(5.*16*GVars::beat,.93,1.046,4*16*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre mb_rot_02(6.*16*GVars::beat,M_PI/180.*90.,0.,4*16*GVars::beat,INTERPOLATION_COSINUS);
    //Banzaaaaaaaaaiiiiiii !
    static Courbe_Bezier_Plan mb_bezier_zoom_01(0,0, .4,.0, .8,1., 1.,1.);
    static Interpolation_Parametre mb_animZoom_02(8.*16*GVars::beat,800.,3400.,(9*16+3.2)*GVars::beat,INTERPOLATION_BEZIER,&mb_bezier_zoom_01);
    static Courbe_Bezier_Plan mb_bezier_zoom_02(0,0, .25,.25, .8,1., 1.,1.);
    static Interpolation_Parametre mb_animZoom_03(10.*16*GVars::beat,0.,3400.,(3.5*16+1.6)*GVars::beat,INTERPOLATION_BEZIER,&mb_bezier_zoom_02);
    static Courbe_Bezier_Plan mb_bezier_iter_01(0,0, 0.,0.25, 0.5,1., 1.,1.);
    static Interpolation_Parametre mb_animIter_03(10.*16*GVars::beat,41.,300.,2*16*GVars::beat,INTERPOLATION_BEZIER,&mb_bezier_iter_01);
    static Interpolation_Parametre mb_rot_03(10.*16*GVars::beat,0.,1000.*M_PI/180.,(3.5*16+1.6)*GVars::beat,INTERPOLATION_COSINUS);

    //==================================================================================
    //=============== Clés d'animation de la scène 4: Spheredance (Psychédélande)
    //==================================================================================

    static Trajectoire sd_trajectoire_cosmo01;
    static Courbe_Bezier_Plan sd_bezier_cosmo_xyz_01(0,0, .6,.0, .8,.9, 1.,1.);
    static Interpolation_Parametre sd_animCosmo_traj01((2*16+4)*GVars::beat,0.,1.,(5.*16+12)*GVars::beat,INTERPOLATION_BEZIER,&sd_bezier_cosmo_xyz_01);
    static Courbe_Bezier_Plan sd_bezier_cosmo_echelle_01(0,0, .3,0., 0.9,.9, 1.,1.);
    static Interpolation_Parametre sd_animCosmo_echelle_01((4*16)*GVars::beat,0.02,.4,(4*16)*GVars::beat,INTERPOLATION_BEZIER,&sd_bezier_cosmo_echelle_01);

    static Interpolation_Parametre sd_animZdistMin_01(1.*16*GVars::beat,20.,0.,3*16*GVars::beat,INTERPOLATION_COSINUS);
    static float sd_S1xyz_01[3]={0.,2.,0.};
    static float sd_S1xyz_02[3]={0.450151,6.64722,-9.48856};
    static Interpolation_Parametre_3f sd_animS1_xyz_01(3.*16*GVars::beat,sd_S1xyz_01,sd_S1xyz_02,3*16*GVars::beat,INTERPOLATION_COSINUS);
    static float sd_obs_rtl_01[3]={0.,90.,180.};
    static float sd_obs_rtl_02[3]={-120.,197.,1.86175};
    static Interpolation_Parametre_3f sd_animObs_rtl_01(0*16*GVars::beat,sd_obs_rtl_01,sd_obs_rtl_02,4*16*GVars::beat,INTERPOLATION_COSINUS);
    static float sd_obs_xyz_01[3]={0.,0.,0.};   //Initialisé à l'init de la scène
    static float sd_obs_xyz_02[3]={0.318706,3.26188,9.8046};
    static Interpolation_Parametre_3f sd_animObs_xyz_01(3*16*GVars::beat,sd_obs_xyz_01,sd_obs_xyz_02,3*16*GVars::beat,INTERPOLATION_COSINUS);
    static float sd_obs_rtl_03[3]={-180.,197.,1.86175};
    static Interpolation_Parametre_3f sd_animObs_rtl_02(4*16*GVars::beat,sd_obs_rtl_02,sd_obs_rtl_03,2*16*GVars::beat,INTERPOLATION_COSINUS);
    static float sd_hsv_01[3]={0.,1.,1.};
    static float sd_hsv_02[3]={0.,2.,1.5};
    static Interpolation_Parametre_3f sd_animHSV_01(4*16*GVars::beat,sd_hsv_01,sd_hsv_02,1*16*GVars::beat,INTERPOLATION_LINEAIRE);
    static float sd_L1xyz_01[3]={0.,10.,100.};
    static float sd_L1xyz_02[3]={0.,50.,0.};
    static Interpolation_Parametre_3f sd_animL1_xyz_01(5.*16*GVars::beat,sd_L1xyz_01,sd_L1xyz_02,1*16*GVars::beat,INTERPOLATION_COSINUS);

    static Interpolation_Parametre sd_animValue_01(5*16*GVars::beat,1.,0.,1*16*GVars::beat,INTERPOLATION_LINEAIRE);

     //==================================================================================
    //=============== Clés d'animation de la scène 5: Hypnostructure (Roue hiératique)
    //==================================================================================


    static Interpolation_Parametre hs_animL1_z_01(0.*16*hs_beat,-1927.,-3731.92,2*16*hs_beat,INTERPOLATION_COSINUS);
    static float hs_hsv_01[3]={187.,2.,1.5};
    static float hs_hsv_02[3]={0.,1.,1.};
    static Interpolation_Parametre_3f hs_animHSV_01(0*16*hs_beat,hs_hsv_01,hs_hsv_02,1*16*hs_beat,INTERPOLATION_COSINUS);

    static float hs_L1_xyz_01[3]={194.3,0.,-760};
    static float hs_L1_xyz_02[3]={0.,1823.42,-500.};
    static Interpolation_Parametre_3f hs_animL1_xyz_01(2*16*hs_beat,hs_L1_xyz_01,hs_L1_xyz_02,2*16*hs_beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre hs_animRoue_rot01(3.*16*hs_beat,1.,2.25,1*16*hs_beat,INTERPOLATION_COSINUS);

    static Trajectoire hs_trajectoire_cosmo01;
    static Courbe_Bezier_Plan hs_bezier_cosmo_xyz_01(0,0, .4,.0, .75,1., 1.,1.);
    static Interpolation_Parametre hs_animCosmo_traj01(4*16*hs_beat,0.,1.,8*16*hs_beat,INTERPOLATION_BEZIER,&hs_bezier_cosmo_xyz_01);
    static Interpolation_Parametre hs_animCosmo_t_01((4.*16+4)*hs_beat,0.,60.,16*hs_beat,INTERPOLATION_COSINUS);

    static Trajectoire hs_trajectoire_obs01;
    static Courbe_Bezier_Plan hs_bezier_obs_xyz_01(0,0, .6,.0, .75,1., 1.,1.);
    static Interpolation_Parametre hs_animObs_traj01((3*16)*hs_beat,0.,1.,11*16*hs_beat,INTERPOLATION_BEZIER,&hs_bezier_obs_xyz_01);

    static Trajectoire hs_trajectoire_vise01;
    static Courbe_Bezier_Plan hs_bezier_vise_xyz_01(0,0, .6,.0, .75,1., 1.,1.);
    static Interpolation_Parametre hs_animVise_traj01((3*16)*hs_beat,0.,1.,9*16*hs_beat,INTERPOLATION_BEZIER,&hs_bezier_vise_xyz_01);

    static float hs_L1_xyz_03[3]={-3.,1987.,236.};
    static Interpolation_Parametre_3f hs_animL1_xyz_02(10.*16*hs_beat,hs_L1_xyz_02,hs_L1_xyz_03,2*16*hs_beat,INTERPOLATION_COSINUS);

    static Interpolation_Parametre hs_animValue_01(14*16*hs_beat,1.,0.,1*16*hs_beat,INTERPOLATION_LINEAIRE);

    //==================================================================================
    //=============== Clés d'animation de la scène 6 Finale:
    //==================================================================================



    static Interpolation_Parametre fin_animLogo_H_01(23*GVars::beat,0.07,1.,1*16*GVars::beat,INTERPOLATION_COSINUS);
    static Interpolation_Parametre fin_animLogo_alpha_01(23*GVars::beat,0.,1.,12*GVars::beat,INTERPOLATION_COSINUS);
    static float fin_cosmo_XY_01[2]={0.358726,0.516241};
    static float fin_cosmo_XY_02[2]={0.840012,0.58881};
    static Interpolation_Parametre_2f fin_animCosmo_xy_01(8.*GVars::beat,fin_cosmo_XY_01,fin_cosmo_XY_02,31*GVars::beat,INTERPOLATION_COSINUS);

    static Interpolation_Parametre fin_animDesign_X_01((22.5+16)*GVars::beat,1.18,0.857891,2*GVars::beat,INTERPOLATION_COSINUS);

//===================================================
//
//===================================================

static bool erreur_openGl(const char* message)
    {
        GLenum erreur_openGl=glGetError();
        if (erreur_openGl!=GL_NO_ERROR)
        {
            cout<<message<<" : "<<gluErrorString(erreur_openGl)<<endl;
            return true;
        }
        return false;
    }

//*************************************************
//          Gestion animations
//*************************************************
static bool init_trajectoires()
{
    /*
    intro_animCosmo_traj01.determine_courbe_Bezier(&intro_cosmoCourbe_01);
    ww_animCosmo_xyz_01.determine_courbe_Bezier(&ww_bezier_cosmo_xyz_01);
    ww_animCosmo_traj05.determine_courbe_Bezier(&ww_bezier_cosmo_xyz_02);
    ww_animCosmo_xyz_02.determine_courbe_Bezier(&ww_bezier_cosmo_xyz_03);
    mb_animCosmo_xyz_01.determine_courbe_Bezier(&mb_bezier_cosmo_xyz_00);
    mb_animCosmo_traj01.determine_courbe_Bezier(&mb_bezier_cosmo_xyz_01);
    mb_animCosmo_traj02.determine_courbe_Bezier(&mb_bezier_cosmo_xyz_02);
    mb_animIter_03.determine_courbe_Bezier(&mb_bezier_iter_01);
    sd_animCosmo_echelle_01.determine_courbe_Bezier(&sd_bezier_cosmo_echelle_01);
    sd_animCosmo_traj01.determine_courbe_Bezier(&sd_bezier_cosmo_xyz_01);
    hs_animCosmo_traj01.determine_courbe_Bezier(&hs_bezier_cosmo_xyz_01);
    hs_animObs_traj01.determine_courbe_Bezier(&hs_bezier_obs_xyz_01);
    hs_animVise_traj01.determine_courbe_Bezier(&hs_bezier_vise_xyz_01);
    */

    return  intro_trajectoire_cosmo01.charge_trajectoire("trajectoires/cosmo01.traj")
            | ww_trajectoire_cosmo02.charge_trajectoire("trajectoires/cosmo02.traj")
            | ww_trajectoire_cosmo03.charge_trajectoire("trajectoires/cosmo03.traj")
            | ww_trajectoire_cosmo04.charge_trajectoire("trajectoires/cosmo04.traj")
            //| ww_trajectoire_cosmo05.charge_trajectoire("trajectoires/cosmo05.traj")
            | mb_trajectoire_cosmo01.charge_trajectoire("trajectoires/cosmo06.traj")
            | mb_trajectoire_cosmo02.charge_trajectoire("trajectoires/cosmo07.traj")
            | sd_trajectoire_cosmo01.charge_trajectoire("trajectoires/cosmo08.traj")
            | hs_trajectoire_cosmo01.charge_trajectoire("trajectoires/cosmo09.traj")
            | hs_trajectoire_obs01.charge_trajectoire("trajectoires/obs02.traj")
            | vl_trajectoire_obs01.charge_trajectoire("trajectoires/obsVille01.traj")
            | vl_trajectoire_cosmo01.charge_trajectoire("trajectoires/cosmo_Ville_01.traj")
            | hs_trajectoire_vise01.charge_trajectoire("trajectoires/vise01.traj")
            ;

}

//**********************************************************************************************************
//-----------Scène 0: Intro
//**********************************************************************************************************
static void scene_Intro()
{
    #if AFFICHE_TIMELINE
    if(GVars::vitesse_temps!=0.)
    {
    #endif
        GVars::temps_scene=GVars::temps-scene_Intro_t0;
        if(!drapeau_init_scene_Intro)
        {
            //GVars::vitesse_temps=.;

            GVars::obs=&Intro::obs;
            GVars::zMin=0.1;
            GVars::zMax=2000.;

            glClearColor(0.,0.,0.,0.);
            GVars::scene_num=SCENE_INTRO;
            GVars::scene_id=SCENE_INTRO;
            drapeau_init_scene_Intro=true;
            GVars::glow_contraste0=0.8;
            GVars::glow_contraste=4.;
            GVars::glow_saturation=1.;
            GVars::saturation_hallo=2.;
            GVars::taille_hallo=20;
            GVars::alpha_scene=1.;
            GVars::alpha_glow=1.;
            //GVars::drapeau_affiche_cosmonaute=false;

            GVars::cosmo->ab_bras=.6;
            GVars::cosmo->ab_jambes=.75;
            GVars::cosmo->ab_tronc=0.1;
            GVars::cosmo->ab_tete=0.75;
            GVars::cosmo->ab_yeux=.7;
            GVars::cosmo->determine_position(-5.23291,11.6,-3.77499);
            //GVars::cosmo->determine_position(-5.23291,5.55686,-3.77499);
            GVars::cosmo->determine_RTL(219.955,0.,-4.79001);
            GVars::drapeau_cosmo_mvt_bruits=true;
            GVars::drapeau_affiche_cosmonaute=true;
            GVars::drapeau_affiche_reflet_cosmonaute=false;


            Intro::starfield->alpha_fondu=0.;
            GVars::spritesDemo->determine_alpha_sprite(SPR_OEILLOGO,1.);
            GVars::spritesDemo->determine_alpha_sprite(SPR_LOGO_MKD,0.);
            GVars::spritesDemo->determine_alpha_sprite(SPR_NEBULEUSE,0.);
        }
        if(GVars::temps_scene<6*16*GVars::beat)
        {
            float a=intro_fadeIn_fond.renvoie_valeur_parametre(GVars::temps_scene);
            float b=intro_bruit_alphaFond.valeur_aleatoire_temporelle(GVars::temps_scene,0.075);
            float b2=intro_bruit_alphaFond2.valeur_aleatoire_temporelle(GVars::temps_scene,0.4);
            a=a*0.5+b*0.1+b2*0.4;
            Intro::L1_intensite=b*0.1+b2*0.4+a*0.5;
            //if(a>1.)a=1.;
            Intro::starfield->alpha_fondu=intro_fadeIn_etoiles.renvoie_valeur_parametre(GVars::temps_scene);
            GVars::spritesDemo->determine_alpha_sprite(SPR_NEBULEUSE,a*0.75);
            if(GVars::temps_scene<1.25*16*GVars::beat)
            {
                GVars::drapeau_affiche_mkd=true;
                GVars::spritesDemo->determine_alpha_sprite(SPR_LOGO_MKD,intro_fadeIn_logo.renvoie_valeur_parametre(GVars::temps_scene));
            }
            else if(GVars::temps_scene<(1.25*16+1)*GVars::beat)
            {
                GVars::spritesDemo->determine_alpha_sprite(SPR_LOGO_MKD,intro_fadeOut_logo.renvoie_valeur_parametre(GVars::temps_scene));
            }
            else
            {
                GVars::drapeau_affiche_mkd=false;
            }

            //GVars::spritesDemo->determine_alpha_sprite(SPR_OEILLOGO,GVars::spritesDemo->renvoie_alpha_sprite(SPR_LOGO_MKD));

            if(GVars::temps_scene<1.*16*GVars::beat)
            {
                intro_clindoeil_01.renvoie_valeur_parametre(GVars::temps_scene,res2f);

            }
            else
            {
                intro_clindoeil_02.renvoie_valeur_parametre(GVars::temps_scene,res2f);
            }
            GVars::spritesDemo->determine_echelle_XY_sprite(SPR_OEILLOGO,res2f);


            if(GVars::temps_scene<2*16*GVars::beat)intro_animCosmo_xyz_01.renvoie_valeur_parametre(GVars::temps_scene,res3f);
            else intro_trajectoire_cosmo01.renvoie_position(intro_animCosmo_traj01.renvoie_valeur_parametre(GVars::temps_scene),res3f);
            GVars::cosmo->determine_position(res3f);

            intro_animCosmo_rtl_01.renvoie_valeur_parametre(GVars::temps_scene,res3f);
            GVars::cosmo->determine_RTL(res3f);
            GVars::cosmo->ab_tronc=intro_animCosmo_alphaTronc.renvoie_valeur_parametre(GVars::temps_scene);

            if(GVars::temps_scene<1*16*GVars::beat) GVars::value=intro_animFadeIn.renvoie_valeur_parametre(GVars::temps_scene);
            else if(GVars::temps_scene>2*16*GVars::beat) GVars::value=intro_animFadeOut.renvoie_valeur_parametre(GVars::temps_scene);

            //cout<<intro_animCosmo_traj01.renvoie_valeur_parametre(GVars::temps_scene)<<" ; "<<res3f[0]<<" ; "<<res3f[1]<<" ; "<<res3f[2]<<endl;
            /*

            else intro_animCosmo_xyz_02.renvoie_valeur_parametre(GVars::temps_scene,res3f);
            GVars::cosmo->determine_position(res3f);
            */
        }
        else
        {
            GVars::drapeau_affiche_mkd=false;
            drapeau_scene_Intro=false;
            drapeau_scene_Waterworld=true;
        }
    #if AFFICHE_TIMELINE
    }
    #endif
}
//**********************************************************************************************************
//-----------Scène 1: Waterworld - Le Monde des Sphères Aquacosmiques
//**********************************************************************************************************
static bool drapeau_basculement_init=false;   ///Mouvement de basculement
static bool drapeau_ww_rtl_01_cosmo_init=false;   ///Raccord pour la rotation du cosmonaute
static float obs_rot_c[3];
static float rayon_basculement;
static float angle_basculement;
static float angle_rot_obs_depart;
static float angle_rot_obs_fin;
static void scene_Waterworld()

{
    #if AFFICHE_TIMELINE
    if(GVars::vitesse_temps!=0.)
    {
    #endif
        GVars::temps_scene=GVars::temps-scene_Waterworld_t0;
            Volume* cosmo=GVars::cosmo->renvoie_volume_parent();
            if(!drapeau_init_scene_Waterworld)
            {
                //GVars::vitesse_temps=30;
                glClearColor(0.,0.,0.,0.);
                GVars::zMin=1.;
                GVars::zMax=1000.;
                GVars::obs=&WaterWorld::obs;
                GVars::scene_num=SCENE_WATERWORLD;
                drapeau_init_scene_Waterworld=true;
                GVars::drapeau_affiche_cosmonaute=true;
                GVars::drapeau_affiche_reflet_cosmonaute=false;
                GVars::scene_id=SCENE_WATERWORLD;
                GVars::glow_contraste0=0.8;
                GVars::saturation_hallo=1.;
                GVars::alpha_glow=1.;
                //GVars::vitesse_temps=20.;

                GVars::cosmo->determine_position(0.,0.,-162.);
                GVars::drapeau_rotation_cosmo_auto=true;
                WaterWorld::drapeau_affiche_Interstase=false;
                GVars::spritesDemo->determine_alpha_sprite(SPR_INTERSTASE,0.);
                WaterWorld::drapeau_affiche_agnomes=false;
            }
            if(GVars::temps_scene<8*16*GVars::beat)
            {
                GVars::value=ww_animFadeIn.renvoie_valeur_parametre(GVars::temps_scene);

                ww_animObs_rtl_01.renvoie_valeur_parametre(GVars::temps_scene,res3f);
                WaterWorld::obs.initRTL(res3f);
                WaterWorld::S1[TRANSP]=ww_animS1_transp_01.renvoie_valeur_parametre(GVars::temps_scene);
                WaterWorld::S1[REFRAC]=ww_animS1_refrac_01.renvoie_valeur_parametre(GVars::temps_scene);
                WaterWorld::L1[INTENS]=ww_animL1_i_01.renvoie_valeur_parametre(GVars::temps_scene);
                ww_animL1_xyz_01.renvoie_valeur_parametre(GVars::temps_scene,res3f);
                WaterWorld::L1[POSX]=res3f[0];
                WaterWorld::L1[POSY]=res3f[1];
                WaterWorld::L1[POSZ]=res3f[2];

                ww_trajectoire_cosmo02.renvoie_position(ww_animCosmo_traj02.renvoie_valeur_parametre(GVars::temps_scene),res3f);
                GVars::cosmo->determine_position(res3f);

                //if (GVars::temps_scene>(7*16+13)*GVars::beat)WaterWorld::drapeau_affiche_Interstase=true;
            }

            else if(GVars::temps_scene<8*16*GVars::beat+4*GVars::beat)
            {
                WaterWorld::S1[TRANSP]=ww_animS1_transp_01.renvoie_valeur_parametre(GVars::temps_scene);
                WaterWorld::S1[REFRAC]=ww_animS1_refrac_01.renvoie_valeur_parametre(GVars::temps_scene);
                WaterWorld::L1[INTENS]=ww_animL1_i_02.renvoie_valeur_parametre(GVars::temps_scene);
                WaterWorld::drapeau_affiche_Interstase=true;
                GVars::spritesDemo->determine_alpha_sprite(SPR_INTERSTASE,ww_fadeIn_Interstase.renvoie_valeur_parametre(GVars::temps_scene));
                GVars::spritesDemo->determine_echelle_sprite(SPR_INTERSTASE,ww_zoom_Interstase.renvoie_valeur_parametre(GVars::temps_scene));
                GVars::drapeau_affiche_cosmonaute=false;
            }

            else if(GVars::temps_scene<13*16*GVars::beat)
            {
                GVars::drapeau_affiche_cosmonaute=true;
                //GVars::drapeau_rotation_cosmo_auto=false;
                ww_trajectoire_cosmo03.renvoie_position(ww_animCosmo_traj03.renvoie_valeur_parametre(GVars::temps_scene),res3f);
                GVars::cosmo->determine_position(res3f);

                WaterWorld::S1[TRANSP]=ww_animS1_transp_01.renvoie_valeur_parametre(GVars::temps_scene);
                WaterWorld::S1[REFRAC]=ww_animS1_refrac_01.renvoie_valeur_parametre(GVars::temps_scene);

                WaterWorld::S1[POSY]=ww_animS1_y_01.renvoie_valeur_parametre(GVars::temps_scene);
                if(GVars::temps_scene<12*16*GVars::beat)
                {
                    GVars::spritesDemo->determine_alpha_sprite(SPR_INTERSTASE,ww_fadeOut_Interstase.renvoie_valeur_parametre(GVars::temps_scene));
                    GVars::spritesDemo->determine_echelle_sprite(SPR_INTERSTASE,ww_zoom_Interstase.renvoie_valeur_parametre(GVars::temps_scene));
                }
                else WaterWorld::drapeau_affiche_Interstase=false;

                ww_animObs_rtl_02.renvoie_valeur_parametre(GVars::temps_scene,res3f);
                WaterWorld::obs.initRTL(res3f);
                WaterWorld::obs.z=ww_animObs_z_01.renvoie_valeur_parametre(GVars::temps_scene);
                WaterWorld::L1[INTENS]=ww_animL1_i_03.renvoie_valeur_parametre(GVars::temps_scene);
                WaterWorld::Sol[REFLEC]=ww_animSol_reflec_01.renvoie_valeur_parametre(GVars::temps_scene);
                ww_animL1_xyz_02.renvoie_valeur_parametre(GVars::temps_scene,res3f);
                WaterWorld::L1[POSX]=res3f[0];
                WaterWorld::L1[POSY]=res3f[1];
                WaterWorld::L1[POSZ]=res3f[2];

                couleur1=ww_couleurSol_01a.renvoie_valeur_parametre(GVars::temps_scene)>>8;
                couleur2=ww_couleurSol_01b.renvoie_valeur_parametre(GVars::temps_scene)>>8;
                Raytracer01::init_couleur_sol(WaterWorld::Sol,couleur1,couleur2);

                couleur1=ww_couleurCiel_01z.renvoie_valeur_parametre(GVars::temps_scene)>>8;
                couleur2=ww_couleurCiel_01h.renvoie_valeur_parametre(GVars::temps_scene)>>8;
                couleur3=ww_couleurCiel_01n.renvoie_valeur_parametre(GVars::temps_scene)>>8;
                Raytracer01::init_ciel(WaterWorld::C_ciel,couleur1,couleur2,couleur2,couleur3);
            }
            //Horizon aux boules fantomatiques
            else if(GVars::temps_scene<18*16*GVars::beat)
            {


                WaterWorld::S7_angle_t0=ww_anim_S7_angle_t0_01.renvoie_valeur_parametre(GVars::temps_scene);


                if(GVars::temps_scene<16*16*GVars::beat)
                {
                    ww_trajectoire_cosmo03.renvoie_position(ww_animCosmo_traj03.renvoie_valeur_parametre(GVars::temps_scene),res3f);
                    GVars::cosmo->determine_position(res3f);
                }
                else
                {
                    GVars::cosmo->determine_position(-3.41354,167.923,-418.01);
                }

                couleur1=ww_couleurSol_01a.renvoie_valeur_parametre(GVars::temps_scene)>>8;
                couleur2=ww_couleurSol_01b.renvoie_valeur_parametre(GVars::temps_scene)>>8;
                Raytracer01::init_couleur_sol(WaterWorld::Sol,couleur1,couleur2);
                couleur1=ww_couleurCiel_01z.renvoie_valeur_parametre(GVars::temps_scene)>>8;
                couleur2=ww_couleurCiel_01h.renvoie_valeur_parametre(GVars::temps_scene)>>8;
                couleur3=ww_couleurCiel_01n.renvoie_valeur_parametre(GVars::temps_scene)>>8;
                Raytracer01::init_ciel(WaterWorld::C_ciel,couleur1,couleur2,couleur2,couleur3);
                ww_animObs_rtl_03.renvoie_valeur_parametre(GVars::temps_scene,res3f);
                WaterWorld::obs.initRTL(res3f);
                WaterWorld::obs.y=ww_animObs_y_01.renvoie_valeur_parametre(GVars::temps_scene);
                WaterWorld::distance_brouillard=ww_animBrouillard_dist_01.renvoie_valeur_parametre(GVars::temps_scene);
                ww_animL1_xyz_03.renvoie_valeur_parametre(GVars::temps_scene,res3f);
                WaterWorld::L1[POSX]=res3f[0];
                WaterWorld::L1[POSY]=res3f[1];
                WaterWorld::L1[POSZ]=res3f[2];

            }
            else if(GVars::temps_scene<21*16*GVars::beat)
            {

                WaterWorld::distance_brouillard=ww_animBrouillard_dist_01.renvoie_valeur_parametre(GVars::temps_scene);
                ww_animObs_rtl_03.renvoie_valeur_parametre(GVars::temps_scene,res3f);
                WaterWorld::obs.initRTL(res3f);
                WaterWorld::obs.y=ww_animObs_y_01.renvoie_valeur_parametre(GVars::temps_scene);
                ww_animL1_xyz_villeDepart.renvoie_valeur_parametre(GVars::temps_scene,res3f);
                WaterWorld::L1[POSX]=res3f[0];
                WaterWorld::L1[POSY]=res3f[1];
                WaterWorld::L1[POSZ]=res3f[2];
                couleur1=ww_couleurCiel_02z.renvoie_valeur_parametre(GVars::temps_scene)>>8;
                couleur2=ww_couleurCiel_02h.renvoie_valeur_parametre(GVars::temps_scene)>>8;
                couleur3=ww_couleurCiel_02n.renvoie_valeur_parametre(GVars::temps_scene)>>8;
                Raytracer01::init_ciel(WaterWorld::C_ciel,couleur1,couleur2,couleur2,couleur3);
                ww_animSol_carXY_01.renvoie_valeur_parametre(GVars::temps_scene,res2f);
                WaterWorld::Sol[CAR_XL]=res2f[0];
                WaterWorld::Sol[CAR_ZL]=res2f[1];
            }

            //Ville qui passe et Agnomes Polysphériques:
            else if(GVars::temps_scene<25*16*GVars::beat)
            {
                WaterWorld::drapeau_affiche_agnomes=true;
                WaterWorld::drapeau_affiche_ville=true;
                ww_animL1_xyz_villeMove.renvoie_valeur_parametre(GVars::temps_scene,res3f);
                WaterWorld::L1[POSX]=res3f[0];
                WaterWorld::L1[POSY]=res3f[1];
                WaterWorld::L1[POSZ]=res3f[2];
                //float xVille=float(GVars::largeur_ecran)/(3800.*2.)*(WaterWorld::L1[POSX]+3800.)-float(GVars::spritesDemo->renvoie_largeur_sprite(1))/2.;
                float xVille=1./(3800.*2.)*(WaterWorld::L1[POSX]+3800.);
                //float xVille=-10.;
                GVars::spritesDemo->determine_position_sprite(SPR_VILLE,xVille,350./720.);

                WaterWorld::agnomes->position_apparitions=ww_animAgnomes_show_01.renvoie_valeur_parametre(GVars::temps_scene);
                WaterWorld::agnomes->position_disparitions=ww_animAgnomes_hide_01.renvoie_valeur_parametre(GVars::temps_scene);
            }

            /*
            else
            {
                GVars::beat=0.4;
                saute=true;
            }
            */


            //Gros cerveau interdimensionnel:
            else if(GVars::temps_scene<33*16*GVars::beat)
            {
                WaterWorld::drapeau_affiche_ville=false;
                WaterWorld::drapeau_affiche_agnomes=false;

                if(GVars::temps_scene<=27*16*GVars::beat)
                {
                    ww_animCosmo_xyz_01.renvoie_valeur_parametre(GVars::temps_scene,res3f);
                    GVars::cosmo->determine_position(res3f);
                }
                else if(GVars::temps_scene<(28*16+11.5)*GVars::beat)
                {
                    GVars::drapeau_affiche_cosmonaute=false;
                }
                else
                {
                    GVars::drapeau_affiche_cosmonaute=true;
                    //Départ à pattern 32:
                    ww_trajectoire_cosmo04.renvoie_position(ww_animCosmo_traj04.renvoie_valeur_parametre(GVars::temps_scene),res3f);
                    GVars::cosmo->determine_position(res3f);
                }

                GVars::saturation_hallo=ww_animGlow_saturation_halo_01.renvoie_valeur_parametre(GVars::temps_scene);

                couleur1=ww_couleurSol_03a.renvoie_valeur_parametre(GVars::temps_scene)>>8;
                couleur2=ww_couleurSol_03b.renvoie_valeur_parametre(GVars::temps_scene)>>8;
                Raytracer01::init_couleur_sol(WaterWorld::Sol,couleur1,couleur2);

                WaterWorld::Sol[REFLEC]=ww_animSol_reflec_02.renvoie_valeur_parametre(GVars::temps_scene);
                ww_animObs_xyz_01.renvoie_valeur_parametre(GVars::temps_scene,res3f);
                WaterWorld::obs.initXYZ(res3f);

                WaterWorld::drapeau_affiche_Edora=true;
                WaterWorld::yEdora=ww_animEdora_y_01.renvoie_valeur_parametre(GVars::temps_scene);
                WaterWorld::eEdora=ww_animEdora_e_01.renvoie_valeur_parametre(GVars::temps_scene);
                WaterWorld::alphaEdora=ww_animEdora_alpha_01.renvoie_valeur_parametre(GVars::temps_scene);



                ww_animObs_rtl_04.renvoie_valeur_parametre(GVars::temps_scene,res3f);
                WaterWorld::obs.initRTL(res3f);
                ww_animL1_xyz_retourVille.renvoie_valeur_parametre(GVars::temps_scene,res3f);
                WaterWorld::L1[POSX]=res3f[0];
                WaterWorld::L1[POSY]=res3f[1];
                WaterWorld::L1[POSZ]=res3f[2];
                WaterWorld::S1[TRANSP]=ww_animS1_transp_02.renvoie_valeur_parametre(GVars::temps_scene);
            }

            else if(GVars::temps_scene<37*16*GVars::beat)
            {
                GVars::saturation_hallo=ww_animGlow_saturation_halo_02.renvoie_valeur_parametre(GVars::temps_scene);

                WaterWorld::yEdora=ww_animEdora_y_02.renvoie_valeur_parametre(GVars::temps_scene);
                WaterWorld::alphaEdora=ww_animEdora_alpha_02.renvoie_valeur_parametre(GVars::temps_scene);

                ww_trajectoire_cosmo04.renvoie_position(ww_animCosmo_traj04.renvoie_valeur_parametre(GVars::temps_scene),res3f);
                GVars::cosmo->determine_position(res3f);

                ww_animObs_xyz_02.renvoie_valeur_parametre(GVars::temps_scene,res3f);
                WaterWorld::obs.initXYZ(res3f);
                ww_animObs_rtl_05.renvoie_valeur_parametre(GVars::temps_scene,res3f);
                WaterWorld::obs.initRTL(res3f);
                WaterWorld::S1[POSY]=ww_animS1_y_02.renvoie_valeur_parametre(GVars::temps_scene);
                ww_animSol_carXY_02.renvoie_valeur_parametre(GVars::temps_scene,res2f);
                WaterWorld::Sol[CAR_XL]=res2f[0];
                WaterWorld::Sol[CAR_ZL]=res2f[1];

            }
            else if(GVars::temps_scene<39*16*GVars::beat)
            {

                WaterWorld::yEdora=ww_animEdora_y_02.renvoie_valeur_parametre(GVars::temps_scene);
                WaterWorld::alphaEdora=ww_animEdora_alpha_02.renvoie_valeur_parametre(GVars::temps_scene);

                ww_trajectoire_cosmo04.renvoie_position(ww_animCosmo_traj04.renvoie_valeur_parametre(GVars::temps_scene),res3f);
                GVars::cosmo->determine_position(res3f);

                ww_animObs_rtl_05.renvoie_valeur_parametre(GVars::temps_scene,res3f);
                WaterWorld::obs.initRTL(res3f);
                WaterWorld::S1[POSY]=ww_animS1_y_02.renvoie_valeur_parametre(GVars::temps_scene);
                WaterWorld::obs.y=ww_animObs_y_02.renvoie_valeur_parametre(GVars::temps_scene);
                ww_animSol_carXY_02.renvoie_valeur_parametre(GVars::temps_scene,res2f);
                WaterWorld::Sol[CAR_XL]=res2f[0];
                WaterWorld::Sol[CAR_ZL]=res2f[1];
                couleur1=ww_couleurAmb_01.renvoie_valeur_parametre(GVars::temps_scene)>>8;
                Raytracer01::init_C_amb(WaterWorld::C_amb,couleur1);
                if(GVars::temps_scene>38*16*GVars::beat)
                {
                    WaterWorld::altitude_eau_cosmo=0.;
                    GVars::drapeau_affiche_reflet_cosmonaute=true;
                }
            }
            else if(GVars::temps_scene<(40*16+8.)*GVars::beat)
            {
                WaterWorld::drapeau_affiche_Edora=false;

                /*
                if(GVars::vitesse_temps==30)
                {
                    GVars::vitesse_temps=1.;
                }
                */

            //GVars::vitesse_temps=1.;

                ww_trajectoire_cosmo04.renvoie_position(ww_animCosmo_traj04.renvoie_valeur_parametre(GVars::temps_scene),res3f);
                GVars::cosmo->determine_position(res3f);

                if(GVars::temps_scene>40*16*GVars::beat)
                {

                    if(!drapeau_ww_rtl_01_cosmo_init)
                    {
                        GVars::altitude_ombre_cosmo=0.02;
                        GVars::drapeau_affiche_ombre_cosmo=true;
                        GVars::drapeau_rotation_cosmo_auto=false;
                        res3f[0]=cosmo->r-float(int(cosmo->r))+float(int(cosmo->r)%360);
                        res3f[1]=cosmo->t-float(int(cosmo->t))+float(int(cosmo->t)%360);
                        res3f[2]=cosmo->l-float(int(cosmo->l))+float(int(cosmo->l)%360);
                        ww_animCosmo_rtl_01.determine_valeurs_depart(res3f);
                        drapeau_ww_rtl_01_cosmo_init=true;
                    }

                    ww_animCosmo_rtl_01.renvoie_valeur_parametre(GVars::temps_scene,res3f);
                    GVars::cosmo->determine_RTL(res3f);
                }

                ww_animSol_carXY_02.renvoie_valeur_parametre(GVars::temps_scene,res2f);
                WaterWorld::Sol[CAR_XL]=res2f[0];
                WaterWorld::Sol[CAR_ZL]=res2f[1];
                WaterWorld::Sol[TRANSP]=ww_animSol_transp_01.renvoie_valeur_parametre(GVars::temps_scene);
                WaterWorld::Sol[REFLEC]=ww_animSol_reflec_03.renvoie_valeur_parametre(GVars::temps_scene);

                WaterWorld::S1[TRANSP]=ww_animS1_transp_03.renvoie_valeur_parametre(GVars::temps_scene);
                WaterWorld::S1[REFRAC]=ww_animS1_refrac_02.renvoie_valeur_parametre(GVars::temps_scene);

                ww_animL1_xyz_04.renvoie_valeur_parametre(GVars::temps_scene,res3f);
                WaterWorld::L1[POSX]=res3f[0];
                WaterWorld::L1[POSY]=res3f[1];
                WaterWorld::L1[POSZ]=res3f[2];


                WaterWorld::distance_brouillard=ww_animBrouillard_dist_02.renvoie_valeur_parametre(GVars::temps_scene);

                couleur1=ww_couleurSol_02a.renvoie_valeur_parametre(GVars::temps_scene)>>8;
                couleur2=ww_couleurSol_02b.renvoie_valeur_parametre(GVars::temps_scene)>>8;
                Raytracer01::init_couleur_sol(WaterWorld::Sol,couleur1,couleur2);

                couleur1=ww_couleurCiel_03z.renvoie_valeur_parametre(GVars::temps_scene)>>8;
                couleur2=ww_couleurCiel_03h.renvoie_valeur_parametre(GVars::temps_scene)>>8;
                couleur3=ww_couleurCiel_03n.renvoie_valeur_parametre(GVars::temps_scene)>>8;
                Raytracer01::init_ciel(WaterWorld::C_ciel,couleur1,couleur2,couleur2,couleur3);

                couleur1=ww_couleurAmb_01.renvoie_valeur_parametre(GVars::temps_scene)>>8;
                Raytracer01::init_C_amb(WaterWorld::C_amb,couleur1);

                couleur1=ww_couleurLum_01.renvoie_valeur_parametre(GVars::temps_scene)>>8;
                Raytracer01::init_couleur_lum(WaterWorld::L1,couleur1);

                ww_animObs_rtl_06.renvoie_valeur_parametre(GVars::temps_scene,res3f);
                WaterWorld::obs.initRTL(res3f);
                ww_animObs_xyz_03.renvoie_valeur_parametre(GVars::temps_scene,res3f);
                WaterWorld::obs.initXYZ(res3f);
                ww_animS1_xyz_01.renvoie_valeur_parametre(GVars::temps_scene,res3f);
                WaterWorld::S1[POSX]=res3f[0];
                WaterWorld::S1[POSY]=res3f[1];
                WaterWorld::S1[POSZ]=res3f[2];

            }

            else if(GVars::temps_scene<(41*16)*GVars::beat)
            {
                if(!drapeau_basculement_init)
                {

                    obs_rot_c[0]=WaterWorld::obs.x;//cosmo->x;
                    obs_rot_c[1]=0.;//cosmo->y;
                    obs_rot_c[2]=cosmo->z;
                    vec3f vecObs(WaterWorld::obs.x-obs_rot_c[0],WaterWorld::obs.y-obs_rot_c[1],-WaterWorld::obs.z-obs_rot_c[2]);
                    rayon_basculement=vecObs.renvoie_norme();
                    vecObs.normalise();
                    angle_rot_obs_depart=acosf(-vecObs.z)*RAD_DEG;
                    angle_rot_obs_fin=angle_rot_obs_depart+180.;

                    ww_rot_obs_angle_trajectoire.valeur_depart=angle_rot_obs_depart;
                    ww_rot_obs_angle_trajectoire.valeur_fin=180.;
                    ww_rot_obs_t.valeur_depart=WaterWorld::obs.t;
                    ww_rot_obs_t.valeur_fin=180.;
                    drapeau_basculement_init=true;
                }

                WaterWorld::obs.t=ww_rot_obs_t.renvoie_valeur_parametre(GVars::temps_scene);
                res3f[0]=WaterWorld::obs.x;
                angle_basculement=ww_rot_obs_angle_trajectoire.renvoie_valeur_parametre(GVars::temps_scene);
                res3f[1]=obs_rot_c[1]+rayon_basculement*sinf(angle_basculement*DEG_RAD);
                res3f[2]=-(obs_rot_c[2]-rayon_basculement*cosf(angle_basculement*DEG_RAD));
                WaterWorld::obs.initXYZ(res3f);

                ww_animCosmo_rtl_01.renvoie_valeur_parametre(GVars::temps_scene,res3f);
                GVars::cosmo->determine_RTL(res3f);
                ww_animCosmo_xyz_02.renvoie_valeur_parametre(GVars::temps_scene,res3f);
                GVars::cosmo->determine_position(res3f);


            }

            else
            {

                drapeau_scene_Waterworld=false;
                drapeau_scene_Ville=true;
                //GVars::drapeau_screenShot=true;
            }
    #if AFFICHE_TIMELINE
    }
    #endif
            if(rotation_S7)
            {
                WaterWorld::S7[POSZ]=WaterWorld::S1[POSZ]+(WaterWorld::orbS7*cos(WaterWorld::angleS7));
                WaterWorld::S7[POSY]=WaterWorld::S1[POSY]+(WaterWorld::orbS7*sin(WaterWorld::angleS7));
                WaterWorld::S7[POSX]=WaterWorld::S1[POSX];
                WaterWorld::angleS7=(2.*M_PI)/((8.*16.*GVars::beat)+WaterWorld::S7_epsilon_Vangle)*(GVars::temps_scene)+WaterWorld::S7_angle_t0;
            }
            else
            {
                WaterWorld::S7[POSX]=WaterWorld::S8[POSX];
                WaterWorld::S7[POSY]=WaterWorld::S8[POSY]-32.;
                WaterWorld::S7[POSZ]=WaterWorld::S8[POSZ];
            }


}

//**********************************************************************************************************
//------------ Scene 2 Fractalopolis: Le Cosmonaute passe dans la Ville
//**********************************************************************************************************

static float res3f_b[3];
static bool vl_drapeau_init_depart_subjection=false;
static bool drapeau_shoot_Ville=false;


static void scene_Ville()
{
    #if AFFICHE_TIMELINE
    if(GVars::vitesse_temps!=0.)
    {
    #endif
        GVars::temps_scene=GVars::temps-scene_Ville_t0;

        Volume*cosmo=GVars::cosmo->renvoie_volume_parent();

        if(!drapeau_init_scene_Ville)
        {
            GVars::Fov=90.;
            GVars::glow_contraste0=0.8;
            GVars::glow_contraste=3.5;
            GVars::glow_saturation=1.;
            GVars::saturation_hallo=1.;
            GVars::taille_hallo=30;
            GVars::alpha_scene=1.;
            GVars::alpha_glow=1.;

            GVars::obs=&Ville::obs;
            GVars::zMax=Ville::distance_brouillard;
            GVars::zMin=.1;
            GVars::scene_id=SCENE_VILLE;
            drapeau_init_scene_Ville=true;
            GVars::scene_num=SCENE_VILLE;

            GVars::drapeau_cosmo_debout=false;
            GVars::drapeau_cosmo_mvt_bruits=false;
            GVars::drapeau_rotation_cosmo_auto=false;
            GVars::drapeau_cosmo_mvt_bruits_mix_debout=true;
            GVars::cosmo_mix_debout_bruits=1.;
            GVars::drapeau_affiche_reflet_cosmonaute=false;
            GVars::drapeau_affiche_cosmonaute=true;
            GVars::drapeau_affiche_ombre_cosmo=true;
            glClearColor(Ville::couleur_ciel[0],Ville::couleur_ciel[1],Ville::couleur_ciel[2],1.);

            //- raccord avec la scène précédente:
                float eCosmo=0.1;
                //Ajuste la position de l'observateur
                obs_rot_c[0]=vl_position_basculement.x;
                obs_rot_c[1]=0.;
                obs_rot_c[2]=vl_position_basculement.z;
                rayon_basculement*=eCosmo; //Ajustement à l'échelle du comonaute
                Ville::obs.initRTL(WaterWorld::obs.r,WaterWorld::obs.t-180.,WaterWorld::obs.l+180.);

                Ville::point_vise.x=vl_position_basculement.x;
                Ville::point_vise.y=0.;//vl_position_basculement.y;
                Ville::point_vise.z=vl_position_basculement.z;



                vl_rot_obs_angle_trajectoire.valeur_depart=ww_rot_obs_angle_trajectoire.valeur_fin;
                vl_rot_obs_angle_trajectoire.valeur_fin=180.-angle_rot_obs_depart;

                //GVars::cosmo->determine_position(-16290.,-0.384246,-13890.);
                GVars::cosmo->determine_echelle(eCosmo);
                res3f[0]=vl_position_basculement.x;
                res3f[1]=cosmo->y*eCosmo;
                res3f[2]=vl_position_basculement.z;
                vl_animCosmo_xyz_01.determine_valeurs_depart(res3f);

                //Ajuste la rotation du cosmonaute:
                //GVars::cosmo->renvoie_RTL(res3f);
                res3f[0]=0.;
                res3f[1]=-90.;
                res3f[2]=90.;
                GVars::cosmo->determine_RTL(res3f);
                //Calculs l'interpolation de rotation:

                float m360=float(int(res3f[0])%360);
                if(m360<180.)res3f_b[0]=res3f[0]-m360;else res3f_b[0]=res3f[0]+360.-m360;
                m360=float(int(res3f[1])%360);
                if(m360<180.)res3f_b[1]=res3f[1]-m360;else res3f_b[1]=res3f[1]+360.-m360;
                m360=float(int(res3f[2])%360);
                if(m360<180.)res3f_b[2]=res3f[2]-m360;else res3f_b[2]=res3f[2]+360.-m360;
                vl_animCosmo_rtl_01.determine_valeurs(res3f,res3f_b);


                #if EDIT_VILLE
                Ville::obs.initXYZ(-21716.6,0.999999,5472.21);
                GVars::cosmo->determine_position(vl_position_basculement.x,1.2,vl_position_basculement.z);
                GVars::zMin=1.;
                GVars::cosmo_mix_debout_bruits=0.5;
                GVars::cosmo->determine_RTL(0.,0.,0.);
                #endif








        }



        if(GVars::temps_scene<1.*16*GVars::beat)
        {
            #if !EDIT_VILLE
            vl_animCosmo_xyz_01.renvoie_valeur_parametre(GVars::temps_scene,res3f);
            GVars::cosmo->determine_position(res3f);
            vl_animCosmo_rtl_01.renvoie_valeur_parametre(GVars::temps_scene,res3f);
            GVars::cosmo->determine_RTL(res3f);
            GVars::cosmo_mix_debout_bruits=vl_animCosmo_mix_param.renvoie_valeur_parametre(GVars::temps_scene);


            //Ville::point_vise.y=vl_point_vise_y_01.renvoie_valeur_parametre(GVars::temps_scene);

                if(GVars::temps_scene<8.*GVars::beat)
                {
                    //Ville::obs.t=vl_rot_obs_t.renvoie_valeur_parametre(GVars::temps_scene);
                    res3f[0]=obs_rot_c[0];
                    angle_basculement=vl_rot_obs_angle_trajectoire.renvoie_valeur_parametre(GVars::temps_scene);
                    res3f[1]=obs_rot_c[1]+rayon_basculement*sinf(angle_basculement*DEG_RAD);
                    res3f[2]=-(obs_rot_c[2]-rayon_basculement*cosf(angle_basculement*DEG_RAD));
                    Ville::obs.initXYZ(res3f);
                }
                else
                {

                }
            #endif
                    Ville::point_vise.x=cosmo->x;
                    Ville::point_vise.y=cosmo->y;
                    Ville::point_vise.z=cosmo->z;

                    if(GVars::vitesse_temps!=0.)Ville::obs.visePoint(Ville::point_vise.x,Ville::point_vise.y,-Ville::point_vise.z);

        }

        else if(GVars::temps_scene<(14.*16)*GVars::beat)
        {
            if(!vl_drapeau_init_depart_subjection)
            {
                vl_drapeau_init_depart_subjection=true;
                GVars::zMin=1.;
            }
            vl_trajectoire_obs01.renvoie_position(vl_animObs_traj01.renvoie_valeur_parametre(GVars::temps_scene),res3f);
            Ville::obs.initXYZ(res3f[0],res3f[1],-res3f[2]);
            vl_trajectoire_cosmo01.renvoie_position(vl_animCosmo_traj01.renvoie_valeur_parametre(GVars::temps_scene),res3f);
            GVars::cosmo->determine_position(res3f[0],res3f[1],res3f[2]);

            Ville::point_vise.x=cosmo->x;
            Ville::point_vise.y=cosmo->y;
            Ville::point_vise.z=cosmo->z;

            if(GVars::vitesse_temps!=0.)Ville::obs.visePoint(Ville::point_vise.x,Ville::point_vise.y,-Ville::point_vise.z);
        }


        else if (!drapeau_shoot_Ville)
        {

            drapeau_shoot_Ville=true;
            GVars::drapeau_screenShot=true;
        }
        else
        {
            drapeau_scene_Ville=false;
            drapeau_scene_Mandelbrot=true;
        }
        //GFunc::animation_cosmo_decollage();



    #if AFFICHE_TIMELINE
    }
    #endif
}
//**********************************************************************************************************
//------------ Scene 3: Mandelbrot - Fractale cosmique
//**********************************************************************************************************

static bool mb_drapeau_init_zoom=false;

static void scene_Mandelbrot()
{
    #if AFFICHE_TIMELINE
    if(GVars::vitesse_temps!=0.)
    {
    #endif
            GVars::temps_scene=GVars::temps-scene_Mandelbrot_t0;

            if(!drapeau_init_scene_Mandelbrot)
            {
                Musique::musique_joue();
                GVars::Fov=100.;
                Musique::musique_joue();
                GVars::obs=&Mandelbrot::obs;
                GVars::zMin=1.;
                GVars::zMax=1000.;
                GVars::drapeau_affiche_glow=false;
                GVars::alpha_glow=0.;
                GVars::glow_contraste0=0.5;
                GVars::glow_contraste=4.;
                GVars::glow_saturation=2.;
                GVars::saturation_hallo=1.3;

                GVars::scene_num=SCENE_MANDELBROT;
                GVars::drapeau_affiche_screenShot=true;
                GVars::scene_id=SCENE_MANDELBROT;
                drapeau_init_scene_Mandelbrot=true;

                GVars::drapeau_rotation_cosmo_auto=true;
                GVars::drapeau_cosmo_mvt_bruits=true;
                GVars::cosmo->determine_echelle(1.);
                GVars::cosmo->ab_bras=.9;
                GVars::cosmo->ab_jambes=1.;
                GVars::cosmo->ab_tronc=0.9;
                GVars::cosmo->ab_tete=1.;
                GVars::cosmo->ab_yeux=.7;
                //GVars::cosmo->determine_position(0.,0.,-2000.);
                GVars::drapeau_affiche_cosmonaute=false;
                GVars::drapeau_affiche_reflet_cosmonaute=false;
                GVars::drapeau_affiche_ombre_cosmo=false;
                glClearColor(0.,0.,0.,1.);
            }
            if(GVars::temps_scene<8*GVars::beat)
            {
                GVars::alpha_screenShot=mb_alpha_screenShot.renvoie_valeur_parametre(GVars::temps_scene);
                Mandelbrot::alpha=mb_rot_01.renvoie_valeur_parametre(GVars::temps_scene);

                mb_animCosmo_xyz_01.renvoie_valeur_parametre(GVars::temps_scene,res3f);
                GVars::cosmo->determine_position(res3f);

            }
            else if(GVars::temps_scene<1*16*GVars::beat)
            {
                GVars::drapeau_affiche_cosmonaute=true;
                mb_animCosmo_xyz_01.renvoie_valeur_parametre(GVars::temps_scene,res3f);
                GVars::cosmo->determine_position(res3f);

                GVars::alpha_screenShot=mb_alpha_screenShot.renvoie_valeur_parametre(GVars::temps_scene);
                Mandelbrot::alpha=mb_rot_01.renvoie_valeur_parametre(GVars::temps_scene);
                GVars::drapeau_affiche_glow=true;       //Pas très élégant, mais bon...
                GVars::alpha_glow=mb_animGlow_alpha_01.renvoie_valeur_parametre(GVars::temps_scene);
                mb_animObs_xyh_01.renvoie_valeur_parametre(GVars::temps_scene,res3d);
                Mandelbrot::xpos=res3d[0];
                Mandelbrot::ypos=res3d[1];
                Mandelbrot::zoom=res3d[2];
            }
            else if(GVars::temps_scene<2*16*GVars::beat)
            {
                GVars::drapeau_affiche_screenShot=false;
                if(GVars::temps_scene<(16+8)*GVars::beat)
                {
                    mb_animCosmo_xyz_01.renvoie_valeur_parametre(GVars::temps_scene,res3f);
                    GVars::cosmo->determine_position(res3f);
                }
                else
                {
                    mb_trajectoire_cosmo01.renvoie_position(mb_animCosmo_traj01.renvoie_valeur_parametre(GVars::temps_scene),res3f);
                    GVars::cosmo->determine_position(res3f);
                }

                Mandelbrot::alpha=mb_rot_01.renvoie_valeur_parametre(GVars::temps_scene);
                Mandelbrot::iter=int(mb_animIter_01.renvoie_valeur_parametre(GVars::temps_scene));
                Mandelbrot::decallage_palette=mb_animPalette_01.renvoie_valeur_parametre(GVars::temps_scene);
            }
            else if(GVars::temps_scene<5*16*GVars::beat)
            {
                mb_trajectoire_cosmo01.renvoie_position(mb_animCosmo_traj01.renvoie_valeur_parametre(GVars::temps_scene),res3f);
                GVars::cosmo->determine_position(res3f);

                Mandelbrot::alpha=mb_rot_01.renvoie_valeur_parametre(GVars::temps_scene);
                Mandelbrot::t=mb_animT_01.renvoie_valeur_parametre(GVars::temps_scene);

                Mandelbrot::decallage_palette=mb_animPalette_01.renvoie_valeur_parametre(GVars::temps_scene);
                mb_animObs_xyh_02.renvoie_valeur_parametre(GVars::temps_scene,res3d);
                Mandelbrot::xpos=res3d[0];
                Mandelbrot::ypos=res3d[1];
                Mandelbrot::zoom=res3d[2];

                Mandelbrot::iter=int(mb_animIter_02.renvoie_valeur_parametre(GVars::temps_scene));

                if(GVars::temps_scene>4*16*GVars::beat)Mandelbrot::zoom=mb_animZoom_01.renvoie_valeur_parametre(GVars::temps_scene);
            }
            else if(GVars::temps_scene<10*16*GVars::beat)
            {
                if(GVars::temps_scene<=8*16*GVars::beat)
                {
                    mb_trajectoire_cosmo01.renvoie_position(mb_animCosmo_traj01.renvoie_valeur_parametre(GVars::temps_scene),res3f);
                    GVars::cosmo->determine_position(res3f);
                }
                else
                {
                    mb_trajectoire_cosmo02.renvoie_position(mb_animCosmo_traj02.renvoie_valeur_parametre(GVars::temps_scene),res3f);
                    GVars::cosmo->determine_position(res3f);
                }

                mb_animObs_xy_01.renvoie_valeur_parametre(GVars::temps_scene,res2d);
                Mandelbrot::xpos=res2d[0];
                Mandelbrot::ypos=res2d[1];
                Mandelbrot::t=mb_animT_02.renvoie_valeur_parametre(GVars::temps_scene);
                Mandelbrot::alpha=mb_rot_02.renvoie_valeur_parametre(GVars::temps_scene);

                if(GVars::temps_scene<8*16*GVars::beat)
                {
                    Mandelbrot::zoom=mb_animZoom_01.renvoie_valeur_parametre(GVars::temps_scene);
                    Mandelbrot::iter=int(mb_animIter_02.renvoie_valeur_parametre(GVars::temps_scene));
                }
                else
                {
                    Mandelbrot::zoom=mb_animZoom_02.renvoie_valeur_parametre(GVars::temps_scene);
                    Mandelbrot::iter=int(mb_animIter_03.renvoie_valeur_parametre(GVars::temps_scene));
                }
            }

            else if(GVars::temps_scene<(13.5*16+1.6)*GVars::beat)
            {
                if(!mb_drapeau_init_zoom)
                {
                    mb_animZoom_03.valeur_depart=mb_animZoom_02.renvoie_valeur_parametre(GVars::temps_scene);
                    mb_drapeau_init_zoom=true;
                }
                mb_trajectoire_cosmo02.renvoie_position(mb_animCosmo_traj02.renvoie_valeur_parametre(GVars::temps_scene),res3f);
                GVars::cosmo->determine_position(res3f);

                mb_animObs_xy_01.renvoie_valeur_parametre(GVars::temps_scene,res2d);
                Mandelbrot::xpos=res2d[0];
                Mandelbrot::ypos=res2d[1];
                Mandelbrot::zoom=mb_animZoom_03.renvoie_valeur_parametre(GVars::temps_scene);
                Mandelbrot::iter=int(mb_animIter_03.renvoie_valeur_parametre(GVars::temps_scene));
                Mandelbrot::alpha=mb_rot_03.renvoie_valeur_parametre(GVars::temps_scene);

            }
            else if (!drapeau_shotFractale)
            {


                GVars::drapeau_screenShot=true;
                drapeau_shotFractale=true;
            }
            else
            {
                drapeau_scene_Mandelbrot=false;
                drapeau_scene_Spheresdance=true;
            }
   #if AFFICHE_TIMELINE
    }
    #endif

}

//**********************************************************************************************************
//------------ Scene 4: Spheredance
//**********************************************************************************************************

static void scene_Spheredance()
{
    #if AFFICHE_TIMELINE
    if(GVars::vitesse_temps!=0.)
    {
    #endif
        GVars::temps_scene=GVars::temps-scene_Spheredance_t0;
        if(!drapeau_init_scene_Spheredance)
        {
            GVars::Fov=100.;
            GVars::obs=&SphereDance::obs;
            GVars::zMin=.1;
            GVars::zMax=2000.;
            GVars::scene_num=SCENE_SPHEREDANCE;

            GVars::alpha_glow=1.;
            GVars::glow_contraste0=0.5;
            GVars::glow_contraste=4.;
            GVars::glow_saturation=2.;
            GVars::saturation_hallo=1.3;

            sd_obs_xyz_01[0]=SphereDance::obsX;
            sd_obs_xyz_01[1]=SphereDance::obsY;
            sd_obs_xyz_01[2]=SphereDance::obsZ;
            sd_animObs_xyz_01.valeur_depart[0]=SphereDance::obsX;
            sd_animObs_xyz_01.valeur_depart[1]=SphereDance::obsY;
            sd_animObs_xyz_01.valeur_depart[2]=SphereDance::obsZ;
            //GVars::drapeau_affiche_screenShot=true;
            GVars::cosmo->determine_position(-2.39,8.4,2.7);
            GVars::cosmo->determine_echelle(.02);
            GVars::scene_id=SCENE_SPHEREDANCE;
            drapeau_init_scene_Spheredance=true;
            GVars::drapeau_affiche_reflet_cosmonaute=false;
            GVars::drapeau_affiche_cosmonaute=true;

        }
        if(GVars::temps_scene<4*16*GVars::beat)
        {
            SphereDance::fracTexDistMin=sd_animZdistMin_01.renvoie_valeur_parametre(GVars::temps_scene);
            sd_animObs_rtl_01.renvoie_valeur_parametre(GVars::temps_scene,res3f);
            SphereDance::obs.initRTL(res3f);
            sd_animObs_xyz_01.renvoie_valeur_parametre(GVars::temps_scene,res3f);
            SphereDance::obs.initXYZ(res3f);

            sd_animS1_xyz_01.renvoie_valeur_parametre(GVars::temps_scene,res3f);
            SphereDance::S1[POSX]=res3f[0];
            SphereDance::S1[POSY]=res3f[1];
            SphereDance::S1[POSZ]=res3f[2];

            sd_trajectoire_cosmo01.renvoie_position(sd_animCosmo_traj01.renvoie_valeur_parametre(GVars::temps_scene),res3f);
            GVars::cosmo->determine_position(res3f);
            GVars::cosmo->determine_echelle(sd_animCosmo_echelle_01.renvoie_valeur_parametre(GVars::temps_scene));

        }
        else if(GVars::temps_scene<6*16*GVars::beat)
        {
            sd_trajectoire_cosmo01.renvoie_position(sd_animCosmo_traj01.renvoie_valeur_parametre(GVars::temps_scene),res3f);
            GVars::cosmo->determine_position(res3f);
            GVars::cosmo->determine_echelle(sd_animCosmo_echelle_01.renvoie_valeur_parametre(GVars::temps_scene));

            sd_animObs_rtl_02.renvoie_valeur_parametre(GVars::temps_scene,res3f);
            SphereDance::obs.initRTL(res3f);
            sd_animObs_xyz_01.renvoie_valeur_parametre(GVars::temps_scene,res3f);
            SphereDance::obs.initXYZ(res3f);

            sd_animS1_xyz_01.renvoie_valeur_parametre(GVars::temps_scene,res3f);
            SphereDance::S1[POSX]=res3f[0];
            SphereDance::S1[POSY]=res3f[1];
            SphereDance::S1[POSZ]=res3f[2];

            sd_animL1_xyz_01.renvoie_valeur_parametre(GVars::temps_scene,res3f);
            SphereDance::L1[POSX]=res3f[0];
            SphereDance::L1[POSY]=res3f[1];
            SphereDance::L1[POSZ]=res3f[2];

            sd_animHSV_01.renvoie_valeur_parametre(GVars::temps_scene,res3f);
            GVars::hue=res3f[0];
            GVars::saturation=res3f[1];
            GVars::value=res3f[2];

            #if !VERSION_LONGUE
            GVars::value=sd_animValue_01.renvoie_valeur_parametre(GVars::temps_scene);
            #endif

        }
        else
        {
            drapeau_scene_Spheresdance=false;
            #if VERSION_LONGUE
            drapeau_scene_Hypnostructure=true;
            #else
            drapeau_scene_Fin=true;
            #endif
        }
    #if AFFICHE_TIMELINE
    }
    #endif
}

//**********************************************************************************************************
//------------ Scene 5: Hypnostructure:
//**********************************************************************************************************
static bool drapeau_init_Hypnostructure_plan2=false;

static void scene_Hypnostructure()
{
    #if AFFICHE_TIMELINE
    if(GVars::vitesse_temps!=0.)
    {
    #endif
        GVars::temps_scene=GVars::temps-scene_Hypnostructure_t0;
        Volume*cosmo=GVars::cosmo->renvoie_volume_parent();
        glClearColor(0.,0.,0.,0.);
        if(!drapeau_init_scene_Hypnostructure)
        {
            GVars::beat=hs_beat;
            GVars::zMin=1.;
            GVars::zMax=10000.;
            GVars::obs=&HypnoStructure::obs;
            GVars::scene_num=SCENE_HYPNOSTRUCTURE;
            GVars::scene_id=SCENE_HYPNOSTRUCTURE;

            GVars::glow_contraste=4.;
            GVars::taille_hallo=20.;
            GVars::saturation_hallo=1.3;
            GVars::alpha_scene=1.;
            GVars::alpha_glow=1.;
            //GVars::drapeau_blend_glow=true;
            GVars::glow_contraste0=0.8;
            GVars::glow_saturation=2.;
            srand(1);


            GVars::saturation=2.;
            GVars::value=1.5;

            HypnoStructure::obs.initXYZ(387.995,24.2139,-3017.88);
            HypnoStructure::obs.initRTL(0.,0.,180);

            GVars::cosmo->determine_position(388.,13.0966,3022.87);
            GVars::altitude_ombre_cosmo=0.;
            GVars::cosmo->raz();
            GVars::cosmo->determine_RTL(0.,0.,180.);
            GVars::cosmo->determine_echelle(1.5);
            GVars::drapeau_rotation_cosmo_auto=false;
            GVars::drapeau_affiche_ombre_cosmo=true;
            GVars::drapeau_cosmo_mvt_bruits_mix_debout=false;
            GVars::drapeau_cosmo_mvt_bruits=true;
            GVars::drapeau_affiche_reflet_cosmonaute=false;
            GVars::drapeau_affiche_cosmonaute=true;
            HypnoStructure::structure.initXYZ(0.,2179.8,0.);
            HypnoStructure::structure.initRTL(0.,30.,0.);
            Raytracer01::init_lum_position(HypnoStructure::L1,1131.28,1262.09,-1927.);

            GVars::cosmo->ab_bras=0.;
            GVars::cosmo->ab_jambes=0.;
            GVars::cosmo->ab_tronc=0.;
            GVars::cosmo->ab_tete=.2;
            GVars::cosmo->ab_yeux=.6;

            GVars::Fov=30.;
            drapeau_init_scene_Hypnostructure=true;

        }


        if(GVars::temps_scene<2*16*GVars::beat)
        {

            //GVars::cosmo->determine_echelle(10.);
            HypnoStructure::L1[POSZ]=hs_animL1_z_01.renvoie_valeur_parametre(GVars::temps_scene);
            hs_animHSV_01.renvoie_valeur_parametre(GVars::temps_scene,res3f);
            GVars::hue=res3f[0];
            GVars::saturation=res3f[1];
            GVars::value=res3f[2];

        }

        else if(GVars::temps_scene<4*16*GVars::beat)
        {
            if(!drapeau_init_Hypnostructure_plan2)
            {
                GVars::glow_contraste0=0.75;
                GVars::glow_saturation=1.;
                GVars::taille_hallo=40.;
                GVars::saturation=1.;
                GVars::saturation_hallo=1.3;
                GVars::value=1.;

                Raytracer01::init_lum_position(HypnoStructure::L1,194.3,0.,-760);
                HypnoStructure::obs.initXYZ(218.849,23.,-3164.31);
                HypnoStructure::obs.initRTL(0.,-18.1719,21.);
                GVars::cosmo->ab_bras=.75;
                GVars::cosmo->ab_jambes=.5;
                GVars::cosmo->ab_tronc=0.9;
                GVars::cosmo->ab_tete=1.;
                GVars::cosmo->ab_yeux=.7;
                GVars::drapeau_cosmo_mvt_bruits_mix_debout=true;
                GVars::drapeau_cosmo_mvt_bruits=false;
                GVars::Fov=100.;
                drapeau_init_Hypnostructure_plan2=true;

                HypnoStructure::point_vise.x=282.824;
                HypnoStructure::point_vise.y=81.6084;
                HypnoStructure::point_vise.z=-2997.61;
            }


            hs_animL1_xyz_01.renvoie_valeur_parametre(GVars::temps_scene,res3f);
            Raytracer01::init_lum_position(HypnoStructure::L1,res3f);

            HypnoStructure::vitesse_rot_roue=hs_animRoue_rot01.renvoie_valeur_parametre(GVars::temps_scene);

            hs_trajectoire_obs01.renvoie_position(hs_animObs_traj01.renvoie_valeur_parametre(GVars::temps_scene),res3f);
            res3f[2]=-res3f[2];
            HypnoStructure::obs.initXYZ(res3f);


            hs_trajectoire_vise01.renvoie_position(hs_animVise_traj01.renvoie_valeur_parametre(GVars::temps_scene),res3f);
            HypnoStructure::point_vise.x=res3f[0];
            HypnoStructure::point_vise.y=res3f[1];
            HypnoStructure::point_vise.z=-res3f[2];
            HypnoStructure::obs.visePoint(HypnoStructure::point_vise.x,HypnoStructure::point_vise.y,HypnoStructure::point_vise.z );

        }

        else if(GVars::temps_scene<14*16*GVars::beat)
        {
            hs_trajectoire_cosmo01.renvoie_position(hs_animCosmo_traj01.renvoie_valeur_parametre(GVars::temps_scene),res3f);
            GVars::cosmo->determine_position(res3f);
            cosmo->t=hs_animCosmo_t_01.renvoie_valeur_parametre(GVars::temps_scene);
            hs_trajectoire_obs01.renvoie_position(hs_animObs_traj01.renvoie_valeur_parametre(GVars::temps_scene),res3f);
            res3f[2]=-res3f[2];
            HypnoStructure::obs.initXYZ(res3f);


            hs_trajectoire_vise01.renvoie_position(hs_animVise_traj01.renvoie_valeur_parametre(GVars::temps_scene),res3f);
            HypnoStructure::point_vise.x=res3f[0];
            HypnoStructure::point_vise.y=res3f[1];
            HypnoStructure::point_vise.z=-res3f[2];
            HypnoStructure::obs.visePoint(HypnoStructure::point_vise.x,HypnoStructure::point_vise.y,HypnoStructure::point_vise.z );


            hs_animL1_xyz_02.renvoie_valeur_parametre(GVars::temps_scene,res3f);
            HypnoStructure::L1[POSX]=res3f[0];
            HypnoStructure::L1[POSY]=res3f[1];
            HypnoStructure::L1[POSZ]=res3f[2];


        }
        else if(GVars::temps_scene<15*16*GVars::beat)
        {
            GVars::value=hs_animValue_01.renvoie_valeur_parametre(GVars::temps_scene);
        }

        else
        {
            drapeau_scene_Hypnostructure=false;
            drapeau_scene_Fin=true;
        }

        //---- Gestion des mouvements du cosmonaute:



        GFunc::animation_cosmo_decollage();

    #if AFFICHE_TIMELINE
    }
    else
    {
        //HypnoStructure::obs.visePoint(HypnoStructure::point_vise.x,HypnoStructure::point_vise.y,HypnoStructure::point_vise.z );
    }
    #endif
            //----------- Rafraichi la courbe éditée en temps réel
            if(EditeurTrajectoire::rafraichissement_courbe)
            {
                cout<<"Rafraichissement courbe !!"<<endl;
                /*
                hs_trajectoire_vise01.determine_points(EditeurTrajectoire::ma_trajectoire->renvoie_points(),EditeurTrajectoire::ma_trajectoire->renvoie_nbrPoints());


                hs_trajectoire_vise01.renvoie_position(hs_animVise_traj01.renvoie_valeur_parametre(GVars::temps_scene),res3f);
                HypnoStructure::point_vise.x=res3f[0];
                HypnoStructure::point_vise.y=res3f[1];
                HypnoStructure::point_vise.z=-res3f[2];
                HypnoStructure::obs.visePoint(HypnoStructure::point_vise.x,HypnoStructure::point_vise.y,HypnoStructure::point_vise.z );
                */

                EditeurTrajectoire::rafraichissement_courbe=false;
            }

            //HypnoStructure::obs.visePoint(HypnoStructure::point_vise.x,HypnoStructure::point_vise.y,HypnoStructure::point_vise.z );

}

//**********************************************************************************************************
//------------ Scene 6: Final, avec crédits, greets, scrolltext...
//**********************************************************************************************************


static void scene_Fin()
{
    #if AFFICHE_TIMELINE
    if(GVars::vitesse_temps!=0.)
    {
    #endif
        GVars::temps_scene=GVars::temps-scene_Fin_t0;

        if(!drapeau_init_scene_Fin)
        {
            glClearColor(Final::couleur_ciel[0],Final::couleur_ciel[1],Final::couleur_ciel[2],Final::couleur_ciel[3]);
            GVars::obs=&Final::obs;
            GVars::zMin=1.;
            GVars::zMax=10000.;
            GVars::scene_id=SCENE_FINAL;
            GVars::drapeau_affiche_cosmonaute=false;
            GVars::value=1.;
            drapeau_init_scene_Fin=true;
            GVars::scene_num=SCENE_FINAL;
            GVars::drapeau_affiche_glow=false;
            GVars::hue=0.;
            GVars::saturation=1.;
            GVars::value=1.;
            //GVars::beat=0.4;
        }

        if(GVars::temps_scene<(22.5+16+2)*GVars::beat)
        {

            fin_animCosmo_xy_01.renvoie_valeur_parametre(GVars::temps_scene,res2f);
            Final::cosmoX=res2f[0];
            Final::cosmoY=res2f[1];

            Final::sprites->determine_alpha_sprite(FINAL_SPR_LOGOMKD,fin_animLogo_alpha_01.renvoie_valeur_parametre(GVars::temps_scene));
            Final::sprites->determine_echelle_sprite(FINAL_SPR_LOGOMKD,fin_animLogo_H_01.renvoie_valeur_parametre(GVars::temps_scene));

            Final::sprites->determine_position_sprite(FINAL_SPR_DESIGN,fin_animDesign_X_01.renvoie_valeur_parametre(GVars::temps_scene),0.509098);
        }
        else
        {
            Final::texteFin->determine_vitesse_scroll_vertical(60./float(GVars::hauteur_ecran));
        }

    #if AFFICHE_TIMELINE
    }
    #endif
}



//*******************************************************************************************************************************************************
//*******************************************************************************************************************************************************
//                                                        Gestion des animations
//*******************************************************************************************************************************************************
//*******************************************************************************************************************************************************

static void timeline()
{


        //**********************************************************************************************************
        //-----------Scène 1: Intro
        //**********************************************************************************************************

        if(drapeau_scene_Intro) scene_Intro();

        //**********************************************************************************************************
        //-----------Scène 2: Le Monde des Sphères Aquacosmiques:
        //**********************************************************************************************************

        if(drapeau_scene_Waterworld) scene_Waterworld();

        //**********************************************************************************************************
        //------------ Scene 3: Ville
        //**********************************************************************************************************

        if(drapeau_scene_Ville) scene_Ville();

        //**********************************************************************************************************
        //------------ Scene 4: Voyage dans la fractale de Mandelbrot:
        //**********************************************************************************************************

        if(drapeau_scene_Mandelbrot) scene_Mandelbrot();

        //**********************************************************************************************************
        //------------ Scene 5: Sphères dance:
        //**********************************************************************************************************

        if(drapeau_scene_Spheresdance) scene_Spheredance();

        //**********************************************************************************************************
        //------------ Scene 6: Hypnostructure:
        //**********************************************************************************************************

        if(drapeau_scene_Hypnostructure) scene_Hypnostructure();

        //**********************************************************************************************************
        //------------ Scene 7: Final, avec crédits, greets, scrolltext...
        //**********************************************************************************************************

        if(drapeau_scene_Fin) scene_Fin();


}

//************************************************
//      Affiche le cosmonaute
//************************************************

// Réflexion du cosmonaute sur un plan horizontal, et déformation avec effet d'eau.
static void affiche_reflet_cosmonaute()
{
        float source_speculaire[3];
        float altitude_plan=0.;
        float dimension_plan;
        parametres_reflet_eau* params;

        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
        GFunc::vue_perspective(GVars::zMin,GVars::zMax);


        if(GVars::scene_id==SCENE_INTRO)
        {
            GFunc::init_matrice_observateur_reflet_eau(GVars::obs,altitude_plan);
            Intro::eclairages();
        }

        else if(GVars::scene_id==SCENE_WATERWORLD)
        {
            altitude_plan=WaterWorld::altitude_eau_cosmo;
            GFunc::init_matrice_observateur_reflet_eau(GVars::obs,altitude_plan);
            WaterWorld::eclairages();
            WaterWorld::maj_positions_lumieres();
            params=&WaterWorld::params_reflet_cosmo;
            dimension_plan=500.;

        }

        else if(GVars::scene_id==SCENE_VILLE)
        {
            GFunc::init_matrice_observateur_reflet_eau(GVars::obs,altitude_plan);
            Ville::eclairages();
            Ville::maj_positions_lumieres();
        }

        else if(GVars::scene_id==SCENE_MANDELBROT)
        {
            GFunc::init_matrice_observateur_reflet_eau(GVars::obs,altitude_plan);
            Mandelbrot::eclairages();
        }
        else if(GVars::scene_id==SCENE_SPHEREDANCE)
        {

            GFunc::init_matrice_observateur_reflet_eau(GVars::obs,altitude_plan);
            SphereDance::eclairages();
            SphereDance::maj_positions_eclairages();

        }
        else if(GVars::scene_id==SCENE_HYPNOSTRUCTURE)
        {
            GFunc::init_matrice_observateur_reflet_eau(GVars::obs,altitude_plan);
            HypnoStructure::eclairages();
            HypnoStructure::maj_positions_lumieres();
        }

        GFunc::rendu_reflet_cosmonaute(altitude_plan);
        GFunc::affiche_reflet_eau(*(params),altitude_plan,dimension_plan);

}

static void affiche_cosmonaute()
{
    GFunc::sauve_OpenGl();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        GFunc::vue_perspective(GVars::zMin,GVars::zMax);
        GFunc::init_matrice_observateur(GVars::obs);


        if(GVars::scene_id==SCENE_INTRO)
        {
            Intro::eclairages();
        }

        else if(GVars::scene_id==SCENE_WATERWORLD)
        {

            //GFunc::init_matrice_observateur(&WaterWorld::obs);
            WaterWorld::eclairages();
            WaterWorld::maj_positions_lumieres();
        }

        else if(GVars::scene_id==SCENE_VILLE)
        {
            //GFunc::init_matrice_observateur(&Ville::obs);
            Ville::eclairages();
            Ville::maj_positions_lumieres();
        }

        else if(GVars::scene_id==SCENE_MANDELBROT)
        {
            Mandelbrot::eclairages();
        }
        else if(GVars::scene_id==SCENE_SPHEREDANCE)
        {

            //GFunc::init_matrice_observateur(&SphereDance::obs);
            SphereDance::eclairages();
            SphereDance::maj_positions_eclairages();

        }
        else if(GVars::scene_id==SCENE_HYPNOSTRUCTURE)
        {
            //GFunc::init_matrice_observateur(&HypnoStructure::obs);
            HypnoStructure::eclairages();
            HypnoStructure::maj_positions_lumieres();

        }

        GFunc::affiche_cosmonaute();
        GFunc::restitue_OpenGl();
}


//*************************************************
//          Affichage principal
//*************************************************
static uint16_t erreur;

static void draw_screen( void )
{
	#if FULLSCREEN
		GVars::ecran_ratio=(GVars::largeur_ecran/(16./9.))/GVars::hauteur_ecran;	//Utiliser si les pixels ne sont pas carrés
	#else
		GVars::ecran_ratio=1.;
    #endif

    timeline();

    GFunc::maj_distanceFocale();

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );

	//============= Rendu dans la texture ============


	glBindFramebuffer(GL_DRAW_FRAMEBUFFER,FrameBuffers::fbos[FBO_ECRAN]);
	glViewport(0,0,GVars::largeur_ecran,GVars::hauteur_ecran);
    glDepthMask(GL_TRUE);
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );


	switch(GVars::scene_id)
	{
		case SCENE_INTRO: Intro::render_scene();break;
		case SCENE_WATERWORLD: WaterWorld::render_waterworld();break;
		case SCENE_VILLE: Ville::render_scene();break;
		case SCENE_MANDELBROT: Mandelbrot::render_scene_mandelbrot();break;
		case SCENE_SPHEREDANCE: SphereDance::render_spheredance();break;
		case SCENE_HYPNOSTRUCTURE: HypnoStructure::render_scene_hypnostructure();break;
		case SCENE_FINAL: Final::render_scene();break;

	}

    /*
	if(GVars::scene_id==SCENE_SPHEREDANCE)
	{
        affiche_fractaleShot();
	}
	*/

    //=============== Affiche le screenShot:
    if(GVars::drapeau_affiche_screenShot)
    {
            FrameBuffers::affiche_screenShot();


    }

    //=============== Saisie d'écran d'arrière-plan:
    if(GVars::drapeau_screenShot)
    {
        FrameBuffers::screenShot();
        GVars::drapeau_screenShot=false;
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,FrameBuffers::fbos[FBO_ECRAN]);


    }

	//=============== Affiche le cosmonaute:

	if(GVars::drapeau_affiche_reflet_cosmonaute) affiche_reflet_cosmonaute();
	if(GVars::drapeau_affiche_cosmonaute) affiche_cosmonaute();

	//=============== Titre INTERSTASE:
	if(WaterWorld::drapeau_affiche_Interstase)
    {
        GVars::spritesDemo->montre_sprite(SPR_INTERSTASE);
        GVars::spritesDemo->determine_position_sprite(SPR_INTERSTASE,0.5,0.5);
        GVars::spritesDemo->affiche_sprites_2d();
        GVars::spritesDemo->cache_sprite(SPR_INTERSTASE);
    }
	//============== Affichage de la texture dans le tempon 2 ============
	GFunc::vue_ortho();
	glEnable(GL_COLOR_MATERIAL);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,FrameBuffers::fbos[FBO_SMALL_1]);
        glEnable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);
        glBindTexture(GL_TEXTURE_2D,FrameBuffers::fbTex[FBO_ECRAN]);

	ShadersDemo::programme_filtres(GVars::glow_saturation,GVars::glow_contraste,GVars::glow_contraste0);

	glViewport(0,0,FBO_SMALL_L,FBO_SMALL_H);
	glClear(GL_COLOR_BUFFER_BIT);

	GFunc::affiche_texture();


        //============== Flou x ============

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,FrameBuffers::fbos[FBO_SMALL_2]);
        glEnable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);
        glBindTexture(GL_TEXTURE_2D,FrameBuffers::fbTex[FBO_SMALL_1]);

        ShadersDemo::programme_floux(GVars::taille_hallo,GVars::saturation_hallo);

        //glViewport(0,0,FBO_SMALL_L,FBO_SMALL_H);
        glClear(GL_COLOR_BUFFER_BIT);

        GFunc::affiche_texture();

         //============== Flou y ============
	//int taille_hallo_y=float(largeur_ecran)/float(hauteur_ecran)*float(taille_hallo);

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,FrameBuffers::fbos[FBO_SMALL_1]);
        glEnable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);
        glBindTexture(GL_TEXTURE_2D,FrameBuffers::fbTex[FBO_SMALL_2]);

        ShadersDemo::programme_flouy(GVars::taille_hallo,GVars::saturation_hallo);

        glViewport(0,0,FBO_SMALL_L,FBO_SMALL_H);
        glClear(GL_COLOR_BUFFER_BIT);

        GFunc::affiche_texture();

        //============== Ajout Glow: ============


        if(GVars::drapeau_affiche_glow)
        {
            glViewport(0,0,GVars::largeur_ecran,GVars::hauteur_ecran);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER,FrameBuffers::fbos[FBO_ECRAN]);
            glEnable(GL_TEXTURE_2D);
            glEnable(GL_BLEND);
            glUseProgram(0);
            if(!GVars::drapeau_blend_glow)glBlendFunc(GL_SRC_ALPHA,GL_DST_ALPHA);
            else glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,FrameBuffers::fbTex[FBO_SMALL_1]);
            GFunc::affiche_texture(GVars::alpha_glow);
        }

        //============== Affichage final ============
        glViewport(0,0,GVars::largeur_ecran,GVars::hauteur_ecran);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
        glEnable(GL_TEXTURE_2D);

        glClear(GL_COLOR_BUFFER_BIT);

        if(GVars::drapeau_affiche_scene)
        {
            if(GVars::drapeau_filtre_HSV)ShadersDemo::programme_filtre_HSV(GVars::hue,GVars::saturation,GVars::value);
            else glUseProgram(0);
            glDisable(GL_BLEND);
            //glBlendFunc(GL_SRC_ALPHA,GL_DST_ALPHA);
            //glUseProgram(0);
            glBindTexture(GL_TEXTURE_2D,FrameBuffers::fbTex[FBO_ECRAN]);
            GFunc::affiche_texture(GVars::alpha_scene);
         }

        if(GVars::drapeau_affiche_mkd)
        {
            glUseProgram(0);
            GVars::spritesDemo->montre_sprite(SPR_LOGO_MKD);
            GVars::spritesDemo->montre_sprite(SPR_OEILLOGO);
            GVars::spritesDemo->determine_centre_sprite(SPR_LOGO_MKD,false);
            GVars::spritesDemo->determine_position_sprite(SPR_LOGO_MKD,.5,.5);
            GVars::spritesDemo->determine_position_sprite(SPR_OEILLOGO,Intro::mkdX,Intro::mkdY);
            GVars::spritesDemo->determine_echelle_sprite(SPR_LOGO_MKD,0.33);
            GVars::spritesDemo->determine_echelle_sprite(SPR_OEILLOGO,0.325);
            //GVars::spritesDemo->determine_echelle_X_sprite(SPR_OEILLOGO,Intro::mkdsX);
            //GVars::spritesDemo->determine_echelle_Y_sprite(SPR_OEILLOGO,Intro::mkdsY);
            GVars::spritesDemo->determine_rotation_sprite(SPR_LOGO_MKD,0.);
            GVars::spritesDemo->affiche_sprites_2d();
            GVars::spritesDemo->cache_sprite(SPR_LOGO_MKD);
            GVars::spritesDemo->cache_sprite(SPR_OEILLOGO);
        }

        //glUseProgram(0);

	//========= Affichage additif =========
    #if AFFICHE_TIMELINE
        char chaine[1024];
        float beat_id=GVars::temps/GVars::beat;
        int patt_id=int(beat_id/16.);
        int beat_in_patt_id=int(beat_id)%16;

        float beat_id_scene=GVars::temps_scene/GVars::beat;
        int patt_id_scene=int(beat_id_scene/16.);
        int beat_in_patt_id_scene=int(beat_id_scene)%16;

        float fps=0.;
        double dt=GVars::temps_reel-GVars::temps_prec;
        if(dt>0.)fps=1./dt;

        if(!GVars::editionTrajectoire)
        {
            if(GFunc::edit_keys==EDIT_RIEN)
            {
               sprintf(chaine, "Globale - Patt: %d Beat: %d\nScene %d - Patt: %d Beat: %d\nTemps(s):%.0f\nTemps X %.1f\nTemps reel: %.0f\nfps: %.2f\nPosition: X=%.2f Z=%.2f\nPosition trame: X=%d Y=%d\nElement: %s\nSurface visible: %d\nIterMax:%d s=%.3f r=%.3f\nZoom=%.6f",
                        patt_id,beat_in_patt_id,
                        GVars::scene_num,patt_id_scene,beat_in_patt_id_scene,
                        GVars::temps,GVars::vitesse_temps,GVars::temps_reel,fps,Ville::obs.x, -Ville::obs.z,int(roundf(Ville::obs.x/Ville::partSize)),int(roundf(-Ville::obs.z/Ville::partSize)),
                        Ville::renvoie_nom_element_obs(),
                        Ville::nbrDalles,
                        Ville::terrain_iterMax,Ville::terrain_s,Ville::terrain_r,Ville::terrain_zoom
                        );
            }
            else if(GFunc::edit_keys==EDIT_FILTRE_HSV)
            {
               sprintf(chaine, "Globale - Patt: %d Beat: %d\nScene %d - Patt: %d Beat: %d\nTemps(s):%.0f\nTemps X %.1f\nTemps reel: %.0f\nfps: %.2f\nH: %.0f S:%.2f V: %.2f\nIter: %d",
                        patt_id,beat_in_patt_id,
                        GVars::scene_num,patt_id_scene,beat_in_patt_id_scene,
                        GVars::temps,
                        GVars::vitesse_temps,GVars::temps_reel,
                        fps,
                        GVars::hue,GVars::saturation,GVars::value,
                        Mandelbrot::iter
                        );
            }
            else if(GFunc::edit_keys==EDIT_GLOW)
            {
               sprintf(chaine,
"Globale - Patt: %d Beat: %d\nScene %d - Patt: %d Beat: %d\nTemps(s):%.0f\nTemps X %.1f\nTemps reel: %.0f\nfps: %.2f\nAffiche glow: %d\nglow_contraste0: %.2f\nglow_contraste: %.2f\nglow_saturation: %.2f\ntaille_hallo: %d\nsaturation_hallo: %.2f\nalpha_scene: %.2f\nalpha_glow: %.2f",
                        patt_id,beat_in_patt_id,
                        GVars::scene_num,patt_id_scene,beat_in_patt_id_scene,
                        GVars::temps,
                        GVars::vitesse_temps,GVars::temps_reel,
                        fps,
                        GVars::drapeau_affiche_glow,
                        GVars::glow_contraste0,
                        GVars::glow_contraste,
                        GVars::glow_saturation,
                        GVars::taille_hallo,
                        GVars::saturation_hallo,
                        GVars::alpha_scene,
                        GVars::alpha_glow
                        );
            }
        }
        else
        {
            sprintf(chaine, "Globale - Patt: %d Beat: %d\nScene %d - Patt: %d Beat: %d\nTemps(s):%.0f\nTemps X %.1f\nTemps reel: %.0f\nfps: %.2f\nEdition trajectoire",
                        patt_id,beat_in_patt_id,
                        GVars::scene_num,patt_id_scene,beat_in_patt_id_scene,
                        GVars::temps,
                        GVars::vitesse_temps,GVars::temps_reel,
                        fps);
        }

        glEnable(GL_BLEND);
        glDepthMask(GL_FALSE);
        GVars::infosDemo->affiche_chaine_2d(chaine);
        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);
    #endif
	//=========== Affichage éditeur de trajectoires:

        if(GVars::editionTrajectoire)
        {
            GFunc::vue_perspective();
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            switch (GVars::scene_id)
            {
                case SCENE_INTRO: GFunc::init_matrice_observateur(&Intro::obs); break;
                case SCENE_VILLE: GFunc::init_matrice_observateur(&Ville::obs);
                                    //EditeurTrajectoire::affiche_trajectoire(&vl_trajectoire_obs01);
                                    EditeurTrajectoire::affiche_trajectoire(&vl_trajectoire_cosmo01);
                                    break;
                case SCENE_WATERWORLD: GFunc::init_matrice_observateur(&WaterWorld::obs); break;
                case SCENE_MANDELBROT: break;//GFunc::init_matrice_observateur(&WaterWorld::obs); break;
                case SCENE_SPHEREDANCE: GFunc::init_matrice_observateur(&SphereDance::obs); break;
                case SCENE_HYPNOSTRUCTURE: GFunc::init_matrice_observateur(&HypnoStructure::obs); break;
            }

            EditeurTrajectoire::affiche_trajectoire(EditeurTrajectoire::ma_trajectoire);
            glLoadIdentity();
        }

	glfwSwapBuffers(GVars::ecran);
}


#endif
