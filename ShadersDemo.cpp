#include "ShadersDemo.h"

    //---------------- Alpha en fonction de la distance à un point:
    GLchar* ShadersDemo::source_vertex_alphaDist=NULL;
    GLchar* ShadersDemo::source_fragment_alphaDist=NULL;
    GLuint ShadersDemo::shader_alphaDist_id1=0;
    GLuint ShadersDemo::shader_alphaDist_id2=0;
    GLuint ShadersDemo::programme_alphaDist=0;

    GLint ShadersDemo::uniform_alphaDist_texture=0;
    GLint ShadersDemo::uniform_alphaDist_point=0;
    GLint ShadersDemo::uniform_alphaDist_distMin=0;
    GLint ShadersDemo::uniform_alphaDist_distMax=0;

    //---------------- Floutage:
    GLchar* ShadersDemo::source_fragment_flou_x=NULL;
    GLchar* ShadersDemo::source_fragment_flou_y=NULL;
    GLuint ShadersDemo::shader_floux_id=0;
    GLuint ShadersDemo::shader_flouy_id=0;
    GLuint ShadersDemo::programme_floux_id=0;
    GLuint ShadersDemo::programme_flouy_id=0;

    GLint ShadersDemo::uniform_floux_taille=0;
    GLint ShadersDemo::uniform_flouy_taille=0;
    GLint ShadersDemo::uniform_floux_saturation=0;
    GLint ShadersDemo::uniform_flouy_saturation=0;
    GLint ShadersDemo::uniform_floux_texture=0;
    GLint ShadersDemo::uniform_flouy_texture=0;

    //---------------- Filtres:
    GLchar* ShadersDemo::source_fragment_filtres=NULL;
    GLuint ShadersDemo::shader_filtres_id=0;
    GLuint ShadersDemo::programme_filtres_id=0;

    GLuint ShadersDemo::uniform_filtres_texture=0;
    GLuint ShadersDemo::uniform_filtres_saturation=0;
    GLuint ShadersDemo::uniform_filtres_contraste=0;
    GLuint ShadersDemo::uniform_filtres_seuil=0;

    //---------------- Filtre Hue, Saturation, Value:
    GLchar* ShadersDemo::source_fragment_filtre_HSV=NULL;
    GLuint ShadersDemo::shader_filtre_HSV_id=0;
    GLuint ShadersDemo::programme_filtre_HSV_id=0;

    GLuint ShadersDemo::uniform_filtre_HSV_texture=0;
    GLuint ShadersDemo::uniform_filtre_HSV_Hue=0;
    GLuint ShadersDemo::uniform_filtre_HSV_Saturation=0;
    GLuint ShadersDemo::uniform_filtre_HSV_Value=0;

    //======== Shader de la fractale de Mandelbrot:
    GLchar* ShadersDemo::source_vertex_mandelbrot=NULL;
    GLchar* ShadersDemo::source_fragment_mandelbrot=NULL;
    GLuint ShadersDemo::shader_mandelbrot_id1=0;
    GLuint ShadersDemo::shader_mandelbrot_id2=0;
    GLuint ShadersDemo::programme_mandelbrot=0;

    GLint ShadersDemo::uniform_mandelbrot_ecran_ratio=0;
    GLint ShadersDemo::uniform_mandelbrot_ajustement_resolution=0;
    GLint ShadersDemo::uniform_mandelbrot_zoom=0;
    GLint ShadersDemo::uniform_mandelbrot_xpos=0;
    GLint ShadersDemo::uniform_mandelbrot_ypos=0;
    GLint ShadersDemo::uniform_mandelbrot_alpha=0;
    GLint ShadersDemo::uniform_mandelbrot_t=0;
    GLint ShadersDemo::uniform_mandelbrot_iter=0;
    GLint ShadersDemo::uniform_mandelbrot_ecranDim=0;
    GLint ShadersDemo::uniform_mandelbrot_palette=0;
    GLint ShadersDemo::uniform_mandelbrot_arriere_plan=0;
    GLint ShadersDemo::uniform_mandelbrot_decallage_palette=0;

    //======== Shader du raytracer waterworld:

    GLchar* ShadersDemo::source_vertex_waterworld=NULL;
    GLchar* ShadersDemo::source_fragment_waterworld=NULL;
    GLuint ShadersDemo::shader_waterworld_id1=0;
    GLuint ShadersDemo::shader_waterworld_id2=0;
    GLuint ShadersDemo::programme_waterworld=0;

    GLint ShadersDemo::uniform_waterworld_random_numer=0;
    GLint ShadersDemo::uniform_waterworld_temps=0;
    GLint ShadersDemo::uniform_waterworld_numReflec=0;
    GLint ShadersDemo::uniform_waterworld_numRefrac=0;
    GLint ShadersDemo::uniform_waterworld_Obs=0;
    GLint ShadersDemo::uniform_waterworld_ecranDim=0;
    GLint ShadersDemo::uniform_waterworld_ajustement_resolution=0;
    GLint ShadersDemo::uniform_waterworld_distanceFocale=0;
    GLint ShadersDemo::uniform_waterworld_ecran_ratio=0;
    GLint ShadersDemo::uniform_waterworld_distanceBrouillard=0;
    GLint ShadersDemo::uniform_waterworld_amplitude_vagues=0;

    GLint ShadersDemo::uniform_waterworld_C_ciel=0;
    GLint ShadersDemo::uniform_waterworld_C_amb=0;
    GLint ShadersDemo::uniform_waterworld_L_amb=0;

    GLint ShadersDemo::uniform_waterworld_L1=0;

    GLint ShadersDemo::uniform_waterworld_objets=0;

    //======== Shader du raytracer spheredance:

    GLchar* ShadersDemo::source_vertex_spheredance=NULL;
    GLchar* ShadersDemo::source_fragment_spheredance=NULL;
    GLuint ShadersDemo::shader_spheredance_id1=0;
    GLuint ShadersDemo::shader_spheredance_id2=0;
    GLuint ShadersDemo::programme_spheredance=0;

    GLint ShadersDemo::uniform_spheredance_random_numer=0;
    GLint ShadersDemo::uniform_spheredance_temps=0;
    GLint ShadersDemo::uniform_spheredance_numReflec=0;
    GLint ShadersDemo::uniform_spheredance_Obs=0;
    GLint ShadersDemo::uniform_spheredance_ecranDim=0;
    GLint ShadersDemo::uniform_spheredance_ajustement_resolution=0;
    GLint ShadersDemo::uniform_spheredance_distanceFocale=0;
    GLint ShadersDemo::uniform_spheredance_ecran_ratio=0;

    GLint ShadersDemo::uniform_spheredance_C_ciel=0;
    GLint ShadersDemo::uniform_spheredance_C_amb=0;
    GLint ShadersDemo::uniform_spheredance_L_amb=0;

    GLint ShadersDemo::uniform_spheredance_L1=0;

    GLint ShadersDemo::uniform_spheredance_objets=0;
    GLint ShadersDemo::uniform_spheredance_distanceBrouillard=0;

    //======== Shader du raytracer hypnostructure:

    GLchar* ShadersDemo::source_vertex_hypnostructure=NULL;
    GLchar* ShadersDemo::source_fragment_hypnostructure=NULL;
    GLuint ShadersDemo::shader_hypnostructure_id1=0;
    GLuint ShadersDemo::shader_hypnostructure_id2=0;
    GLuint ShadersDemo::programme_hypnostructure=0;

    GLint ShadersDemo::uniform_hypnostructure_random_numer=0;
    GLint ShadersDemo::uniform_hypnostructure_temps=0;
    GLint ShadersDemo::uniform_hypnostructure_numReflec=0;
    GLint ShadersDemo::uniform_hypnostructure_numRefrac=0;
    GLint ShadersDemo::uniform_hypnostructure_Obs=0;
    GLint ShadersDemo::uniform_hypnostructure_ecranDim=0;

    GLint ShadersDemo::uniform_hypnostructure_distanceFocale=0;
    GLint ShadersDemo::uniform_hypnostructure_ecran_ratio=0;
    GLint ShadersDemo::uniform_hypnostructure_distanceBrouillard=0;

    GLint ShadersDemo::uniform_hypnostructure_C_ciel=0;
    GLint ShadersDemo::uniform_hypnostructure_C_amb=0;
    GLint ShadersDemo::uniform_hypnostructure_L_amb=0;

    GLint ShadersDemo::uniform_hypnostructure_L1=0;

    GLint ShadersDemo::uniform_hypnostructure_objets=0;

    //--------------------------------------------------------------
    //      Création des shaders globaux de la démo
    //--------------------------------------------------------------

    bool ShadersDemo::init_shader_alphaDist()
	{
		 //---------- Charge les sources des shaders:

                source_vertex_alphaDist=charge_source_shader("shaders/vertex_alphaDist.glsl");
                source_fragment_alphaDist=charge_source_shader("shaders/fragment_alphaDist.glsl");
                if (source_fragment_alphaDist==NULL || source_vertex_alphaDist==NULL) return false;

                 //---------- Création des shaders:
                shader_alphaDist_id1=glCreateShader(GL_VERTEX_SHADER);
                shader_alphaDist_id2=glCreateShader(GL_FRAGMENT_SHADER);

                if (shader_alphaDist_id1==0 || shader_alphaDist_id2 == 0)
                {
                        cout<<"ERREUR - Impossible de créer les objets Shader"<<endl;
                        return false;
                }


                //---------- Lie les sources avec les objets shader:
                const GLchar* tableau_sources[]={source_vertex_alphaDist};

                glShaderSource(shader_alphaDist_id1,1,tableau_sources,NULL);
                tableau_sources[0]=source_fragment_alphaDist;
                glShaderSource(shader_alphaDist_id2,1,tableau_sources,NULL);

                //---------- Compilation des shaders:
                if (!compile_shader(shader_alphaDist_id1)) return false;
                if (!compile_shader(shader_alphaDist_id2)) return false;



                //----------- Création des programmes
                programme_alphaDist=glCreateProgram();
                if (programme_alphaDist==0)
                {
                        cout<<"ERREUR lors de la création du programme de liaison des shaders"<<endl;
                        return false;
                }

                //----------- Liaison des shaders aux programmes:
                glAttachShader(programme_alphaDist,shader_alphaDist_id1);
                glAttachShader(programme_alphaDist,shader_alphaDist_id2);

                //----------- Liaisons des programmes de shaders:
                if (!relie_programme_shader(programme_alphaDist)) return false;

                //----------- Vérifie la validité des shaders:

                if (!valide_programme_shader(programme_alphaDist)) return false;

                //----------- Création des liens avec les variables:

                uniform_alphaDist_texture=glGetUniformLocation(programme_alphaDist,"texture_couleur");
                uniform_alphaDist_point=glGetUniformLocation(programme_alphaDist,"point");
                uniform_alphaDist_distMin=glGetUniformLocation(programme_alphaDist,"distMin");
                uniform_alphaDist_distMax=glGetUniformLocation(programme_alphaDist,"distMax");
                return true;
	}

	bool ShadersDemo::init_shader_mandelbrot()
	{
		 //---------- Charge les sources des shaders:

                source_vertex_mandelbrot=charge_source_shader("shaders/vertex_shader_mandelbrot.glsl");
                source_fragment_mandelbrot=charge_source_shader("shaders/fragment_shader_mandelbrot.glsl");
                if (source_fragment_mandelbrot==NULL || source_vertex_mandelbrot==NULL) return false;

                 //---------- Création des shaders:
                shader_mandelbrot_id1=glCreateShader(GL_VERTEX_SHADER);
                shader_mandelbrot_id2=glCreateShader(GL_FRAGMENT_SHADER);

                if (shader_mandelbrot_id1==0 || shader_mandelbrot_id2 == 0)
                {
                        cout<<"ERREUR - Impossible de créer les objets Shader"<<endl;
                        return false;
                }


                //---------- Lie les sources avec les objets shader:
                const GLchar* tableau_sources[]={source_vertex_mandelbrot};

                glShaderSource(shader_mandelbrot_id1,1,tableau_sources,NULL);
                tableau_sources[0]=source_fragment_mandelbrot;
                glShaderSource(shader_mandelbrot_id2,1,tableau_sources,NULL);

                //---------- Compilation des shaders:
                if (!compile_shader(shader_mandelbrot_id1)) return false;
                if (!compile_shader(shader_mandelbrot_id2)) return false;



                //----------- Création des programmes
                programme_mandelbrot=glCreateProgram();
                if (programme_mandelbrot==0)
                {
                        cout<<"ERREUR lors de la création du programme de liaison des shaders"<<endl;
                        return false;
                }

                //----------- Liaison des shaders aux programmes:
                glAttachShader(programme_mandelbrot,shader_mandelbrot_id1);
                glAttachShader(programme_mandelbrot,shader_mandelbrot_id2);

                //----------- Liaisons des programmes de shaders:
                if (!relie_programme_shader(programme_mandelbrot)) return false;

                //----------- Vérifie la validité des shaders:

                if (!valide_programme_shader(programme_mandelbrot)) return false;

                //----------- Création des liens avec les variables:

                uniform_mandelbrot_ecran_ratio=glGetUniformLocation(programme_mandelbrot,"ecran_ratio");
                uniform_mandelbrot_ajustement_resolution=glGetUniformLocation(programme_mandelbrot,"ajustement_resolution");
                uniform_mandelbrot_zoom=glGetUniformLocation(programme_mandelbrot,"zoom");
                uniform_mandelbrot_xpos=glGetUniformLocation(programme_mandelbrot,"xpos");
                uniform_mandelbrot_ypos=glGetUniformLocation(programme_mandelbrot,"ypos");
                uniform_mandelbrot_alpha=glGetUniformLocation(programme_mandelbrot,"alpha");
                uniform_mandelbrot_t=glGetUniformLocation(programme_mandelbrot,"t");
                uniform_mandelbrot_iter=glGetUniformLocation(programme_mandelbrot,"iter");
                uniform_mandelbrot_ecranDim=glGetUniformLocation(programme_mandelbrot,"ecranDim");
                uniform_mandelbrot_palette=glGetUniformLocation(programme_mandelbrot,"palette");
                uniform_mandelbrot_arriere_plan=glGetUniformLocation(programme_mandelbrot,"arriere_plan");
                uniform_mandelbrot_decallage_palette=glGetUniformLocation(programme_mandelbrot,"decallage_palette");

                return true;
	}


	bool ShadersDemo::init_shader_waterworld()
	{
                 //---------- Charge les sources des shaders:

                source_vertex_waterworld=charge_source_shader("shaders/vertex_shader_waterworld.glsl");
                source_fragment_waterworld=charge_source_shader("shaders/fragment_shader_waterworld.glsl");
                if (source_fragment_waterworld==NULL || source_vertex_waterworld==NULL) return false;

                 //---------- Création des shaders:
                shader_waterworld_id1=glCreateShader(GL_VERTEX_SHADER);
                shader_waterworld_id2=glCreateShader(GL_FRAGMENT_SHADER);

                if (shader_waterworld_id1==0 || shader_waterworld_id2 == 0)
                {
                        cout<<"ERREUR - Impossible de créer les objets Shader - Waterworld"<<endl;
                        return false;
                }

                //---------- Lie les sources avec les objets shader:
                const GLchar* tableau_sources[]={source_vertex_waterworld};

                glShaderSource(shader_waterworld_id1,1,tableau_sources,NULL);
                tableau_sources[0]=source_fragment_waterworld;
                glShaderSource(shader_waterworld_id2,1,tableau_sources,NULL);

                //---------- Compilation des shaders:
                if (!compile_shader(shader_waterworld_id1)) return false;
                if (!compile_shader(shader_waterworld_id2)) return false;

                //----------- Création des programmes
                programme_waterworld=glCreateProgram();
                if (programme_waterworld==0)
                {
                        cout<<"ERREUR lors de la création du programme de liaison des shaders"<<endl;
                        return false;
                }

                //----------- Liaison des shaders aux programmes:
                glAttachShader(programme_waterworld,shader_waterworld_id1);
                glAttachShader(programme_waterworld,shader_waterworld_id2);

                //----------- Liaisons des programmes de shaders:
                if (!relie_programme_shader(programme_waterworld)) return false;

                //----------- Vérifie la validité des shaders:

                if (!valide_programme_shader(programme_waterworld)) return false;

                //----------- Création des liens avec les variables:


                uniform_waterworld_random_numer=glGetUniformLocation(programme_waterworld,"random_number");
                uniform_waterworld_temps=glGetUniformLocation(programme_waterworld,"temps");
                uniform_waterworld_Obs=glGetUniformLocation(programme_waterworld,"Obs");

                uniform_waterworld_ecranDim=glGetUniformLocation(programme_waterworld,"ecranDim");
                uniform_waterworld_ajustement_resolution=glGetUniformLocation(programme_waterworld,"ajustement_resolution");
                uniform_waterworld_distanceFocale=glGetUniformLocation(programme_waterworld,"distanceFocale");
                uniform_waterworld_ecran_ratio=glGetUniformLocation(programme_waterworld,"ecran_ratio");
                uniform_waterworld_C_ciel=glGetUniformLocation(programme_waterworld,"C_ciel");
                uniform_waterworld_C_amb=glGetUniformLocation(programme_waterworld,"C_amb");
                uniform_waterworld_L_amb=glGetUniformLocation(programme_waterworld,"L_amb");
                uniform_waterworld_distanceBrouillard=glGetUniformLocation(programme_waterworld,"distance_brouillard");
                uniform_waterworld_amplitude_vagues=glGetUniformLocation(programme_waterworld,"amplitude_vagues");

                uniform_waterworld_L1=glGetUniformLocation(programme_waterworld,"L1");

                uniform_waterworld_objets=glGetUniformLocation(programme_waterworld,"objets");

                uniform_waterworld_numReflec=glGetUniformLocation(programme_waterworld,"numReflec");
                uniform_waterworld_numRefrac=glGetUniformLocation(programme_waterworld,"numRefrac");

                return true;
	}

	//===================================================
	//			Scene spheredance
	//===================================================

	bool ShadersDemo::init_shader_spheredance()
	{
                 //---------- Charge les sources des shaders:

                source_vertex_spheredance=charge_source_shader("shaders/vertex_shader_waterworld.glsl");
                source_fragment_spheredance=charge_source_shader("shaders/fragment_shader_spheredance.glsl");
                if (source_fragment_spheredance==NULL || source_vertex_spheredance==NULL) return false;

                 //---------- Création des shaders:
                shader_spheredance_id1=glCreateShader(GL_VERTEX_SHADER);
                shader_spheredance_id2=glCreateShader(GL_FRAGMENT_SHADER);

                if (shader_spheredance_id1==0 || shader_spheredance_id2 == 0)
                {
                        cout<<"ERREUR - Impossible de créer les objets Shader - Spheredance"<<endl;
                        return false;
                }

                //---------- Lie les sources avec les objets shader:
                const GLchar* tableau_sources[]={source_vertex_spheredance};

                glShaderSource(shader_spheredance_id1,1,tableau_sources,NULL);
                tableau_sources[0]=source_fragment_spheredance;
                glShaderSource(shader_spheredance_id2,1,tableau_sources,NULL);

                //---------- Compilation des shaders:
                if (!compile_shader(shader_spheredance_id1)) return false;
                if (!compile_shader(shader_spheredance_id2)) return false;

                //----------- Création des programmes
                programme_spheredance=glCreateProgram();
                if (programme_spheredance==0)
                {
                        cout<<"ERREUR lors de la création du programme de liaison des shaders"<<endl;
                        return false;
                }

                //----------- Liaison des shaders aux programmes:
                glAttachShader(programme_spheredance,shader_spheredance_id1);
                glAttachShader(programme_spheredance,shader_spheredance_id2);

                //----------- Liaisons des programmes de shaders:
                if (!relie_programme_shader(programme_spheredance)) return false;

                //----------- Vérifie la validité des shaders:

                if (!valide_programme_shader(programme_spheredance)) return false;

                //----------- Création des liens avec les variables:


                uniform_spheredance_random_numer=glGetUniformLocation(programme_spheredance,"random_number");
                uniform_spheredance_temps=glGetUniformLocation(programme_spheredance,"temps");
                uniform_spheredance_Obs=glGetUniformLocation(programme_spheredance,"Obs");

                uniform_spheredance_ecranDim=glGetUniformLocation(programme_spheredance,"ecranDim");
                uniform_spheredance_ajustement_resolution=glGetUniformLocation(programme_spheredance,"ajustement_resolution");
                uniform_spheredance_distanceFocale=glGetUniformLocation(programme_spheredance,"distanceFocale");
                uniform_spheredance_ecran_ratio=glGetUniformLocation(programme_spheredance,"ecran_ratio");
                uniform_spheredance_C_ciel=glGetUniformLocation(programme_spheredance,"C_ciel");
                uniform_spheredance_C_amb=glGetUniformLocation(programme_spheredance,"C_amb");
                uniform_spheredance_L_amb=glGetUniformLocation(programme_spheredance,"L_amb");

                uniform_spheredance_L1=glGetUniformLocation(programme_spheredance,"L1");

                uniform_spheredance_objets=glGetUniformLocation(programme_spheredance,"objets");

                uniform_spheredance_numReflec=glGetUniformLocation(programme_spheredance,"numReflec");
                uniform_spheredance_distanceBrouillard=glGetUniformLocation(programme_spheredance,"distance_brouillard");

                return true;
	}

	//===================================================
	//			Scene Hypnostructure
	//===================================================

	bool ShadersDemo::init_shader_hypnostructure()
	{
                 //---------- Charge les sources des shaders:

                source_vertex_hypnostructure=charge_source_shader("shaders/vertex_shader_hypnostructure.glsl");
                source_fragment_hypnostructure=charge_source_shader("shaders/fragment_shader_hypnostructure.glsl");
                if (source_fragment_hypnostructure==NULL || source_vertex_hypnostructure==NULL) return false;

                 //---------- Création des shaders:
                shader_hypnostructure_id1=glCreateShader(GL_VERTEX_SHADER);
                shader_hypnostructure_id2=glCreateShader(GL_FRAGMENT_SHADER);

                if (shader_hypnostructure_id1==0 || shader_hypnostructure_id2 == 0)
                {
                        cout<<"ERREUR - Impossible de créer les objets Shader - Hypnostructure"<<endl;
                        return false;
                }

                //---------- Lie les sources avec les objets shader:
                const GLchar* tableau_sources[]={source_vertex_hypnostructure};

                glShaderSource(shader_hypnostructure_id1,1,tableau_sources,NULL);
                tableau_sources[0]=source_fragment_hypnostructure;
                glShaderSource(shader_hypnostructure_id2,1,tableau_sources,NULL);

                //---------- Compilation des shaders:
                if (!compile_shader(shader_hypnostructure_id1)) return false;
                if (!compile_shader(shader_hypnostructure_id2)) return false;

                //----------- Création des programmes
                programme_hypnostructure=glCreateProgram();
                if (programme_hypnostructure==0)
                {
                        cout<<"ERREUR lors de la création du programme de liaison des shaders"<<endl;
                        return false;
                }

                //----------- Liaison des shaders aux programmes:
                glAttachShader(programme_hypnostructure,shader_hypnostructure_id1);
                glAttachShader(programme_hypnostructure,shader_hypnostructure_id2);

                //----------- Liaisons des programmes de shaders:
                if (!relie_programme_shader(programme_hypnostructure)) return false;

                //----------- Vérifie la validité des shaders:

                if (!valide_programme_shader(programme_hypnostructure)) return false;

                //----------- Création des liens avec les variables:


                uniform_hypnostructure_random_numer=glGetUniformLocation(programme_hypnostructure,"random_number");
                uniform_hypnostructure_temps=glGetUniformLocation(programme_hypnostructure,"temps");
                uniform_hypnostructure_Obs=glGetUniformLocation(programme_hypnostructure,"Obs");

                uniform_hypnostructure_ecranDim=glGetUniformLocation(programme_hypnostructure,"ecranDim");
                uniform_hypnostructure_distanceBrouillard=glGetUniformLocation(programme_hypnostructure,"distance_brouillard");
                uniform_hypnostructure_distanceFocale=glGetUniformLocation(programme_hypnostructure,"distanceFocale");
                uniform_hypnostructure_ecran_ratio=glGetUniformLocation(programme_hypnostructure,"ecran_ratio");
                uniform_hypnostructure_C_ciel=glGetUniformLocation(programme_hypnostructure,"C_ciel");
                uniform_hypnostructure_C_amb=glGetUniformLocation(programme_hypnostructure,"C_amb");
                uniform_hypnostructure_L_amb=glGetUniformLocation(programme_hypnostructure,"L_amb");

                uniform_hypnostructure_L1=glGetUniformLocation(programme_hypnostructure,"L1");

                uniform_hypnostructure_objets=glGetUniformLocation(programme_hypnostructure,"objets");

                uniform_hypnostructure_numReflec=glGetUniformLocation(programme_hypnostructure,"numReflec");
                uniform_hypnostructure_numRefrac=glGetUniformLocation(programme_hypnostructure,"numRefrac");

                return true;
	}



	bool ShadersDemo::init_flou()
	{
                 //---------- Charge les sources des shaders:

                source_fragment_flou_x=charge_source_shader("shaders/flou_x.glsl");
                source_fragment_flou_y=charge_source_shader("shaders/flou_y.glsl");

                if (source_fragment_flou_x==NULL || source_fragment_flou_y==NULL) return false;

                 //---------- Création des shaders:
                shader_floux_id=glCreateShader(GL_FRAGMENT_SHADER);
                shader_flouy_id=glCreateShader(GL_FRAGMENT_SHADER);


                if (shader_floux_id==0 || shader_flouy_id == 0)
                {
                        cout<<"ERREUR - Impossible de créer les objets Shader - Flou"<<endl;
                        return false;
                }

                //---------- Lie les sources avec les objets shader:
                const GLchar* tableau_sources[]={source_fragment_flou_x};

                glShaderSource(shader_floux_id,1,tableau_sources,NULL);
                tableau_sources[0]=source_fragment_flou_y;
                glShaderSource(shader_flouy_id,1,tableau_sources,NULL);

                //---------- Compilation des shaders:
                if (!compile_shader(shader_floux_id)) return false;
                if (!compile_shader(shader_flouy_id)) return false;

                //----------- Création des programmes
                programme_floux_id=glCreateProgram();
                programme_flouy_id=glCreateProgram();
                if (programme_floux_id==0 || programme_flouy_id==0)
                {
                        cout<<"ERREUR lors de la création du programme de liaison des shaders - Flou"<<endl;
                        return false;
                }

                //----------- Liaison des shaders aux programmes:
                //glAttachShader(programme_id1,shader_id1);
                glAttachShader(programme_floux_id,shader_floux_id);
                glAttachShader(programme_flouy_id,shader_flouy_id);

                //----------- Liaisons des programmes de shaders:
                if (!relie_programme_shader(programme_floux_id)) return false;
                if (!relie_programme_shader(programme_flouy_id)) return false;

                //----------- Vérifie la validité des shaders:

                if (!valide_programme_shader(programme_floux_id)) return false;
                if (!valide_programme_shader(programme_flouy_id)) return false;

                //----------- Création des liens avec les variables:

                uniform_floux_taille=glGetUniformLocation(programme_floux_id,"taille_flou");
                uniform_floux_saturation=glGetUniformLocation(programme_floux_id,"saturation_flou");
                uniform_floux_texture=glGetUniformLocation(programme_floux_id,"texture");

                uniform_flouy_taille=glGetUniformLocation(programme_flouy_id,"taille_flou");
                uniform_flouy_saturation=glGetUniformLocation(programme_flouy_id,"saturation_flou");
                uniform_flouy_texture=glGetUniformLocation(programme_flouy_id,"texture");

                return true;
	}

	bool ShadersDemo::init_filtres()
	{
                 //---------- Charge les sources des shaders:

                source_fragment_filtres=charge_source_shader("shaders/filtres.glsl");

                if (source_fragment_filtres==NULL) return false;

                 //---------- Création des shaders:
                shader_filtres_id=glCreateShader(GL_FRAGMENT_SHADER);



                if (shader_filtres_id==0)
                {
                        cout<<"ERREUR - Impossible de créer les objets Shader - Filtres"<<endl;
                        return false;
                }

                //---------- Lie les sources avec les objets shader:
                const GLchar* tableau_sources[]={source_fragment_filtres};

                glShaderSource(shader_filtres_id,1,tableau_sources,NULL);

                //---------- Compilation des shaders:
                if (!compile_shader(shader_filtres_id)) return false;

                //----------- Création des programmes
                programme_filtres_id=glCreateProgram();

                if (programme_filtres_id==0)
                {
                        cout<<"ERREUR lors de la création du programme de liaison des shaders - Filtres"<<endl;
                        return false;
                }

                //----------- Liaison des shaders aux programmes:
                //glAttachShader(programme_id1,shader_id1);
                glAttachShader(programme_filtres_id,shader_filtres_id);

                //----------- Liaisons des programmes de shaders:
                if (!relie_programme_shader(programme_filtres_id)) return false;

                //----------- Vérifie la validité des shaders:

                if (!valide_programme_shader(programme_filtres_id)) return false;

                //----------- Création des liens avec les variables:

                uniform_filtres_texture=glGetUniformLocation(programme_filtres_id,"texture");
                uniform_filtres_saturation=glGetUniformLocation(programme_filtres_id,"saturation");
                uniform_filtres_contraste=glGetUniformLocation(programme_filtres_id,"contraste");
                uniform_filtres_seuil=glGetUniformLocation(programme_filtres_id,"seuil");

                return true;
	}

	bool ShadersDemo::init_filtre_HSV()
	{
                 //---------- Charge les sources des shaders:

                source_fragment_filtre_HSV=charge_source_shader("shaders/filtre_HSV.glsl");

                if (source_fragment_filtre_HSV==NULL) return false;

                 //---------- Création des shaders:
                shader_filtre_HSV_id=glCreateShader(GL_FRAGMENT_SHADER);

                if (shader_filtre_HSV_id==0)
                {
                        cout<<"ERREUR - Impossible de créer les objets Shader - Filtre HSV"<<endl;
                        return false;
                }

                //---------- Lie les sources avec les objets shader:
                const GLchar* tableau_sources[]={source_fragment_filtre_HSV};

                glShaderSource(shader_filtre_HSV_id,1,tableau_sources,NULL);

                //---------- Compilation des shaders:
                if (!compile_shader(shader_filtre_HSV_id)) return false;

                //----------- Création des programmes
                programme_filtre_HSV_id=glCreateProgram();

                if (programme_filtre_HSV_id==0)
                {
                        cout<<"ERREUR lors de la création du programme de liaison des shaders - Filtre HSV"<<endl;
                        return false;
                }

                //----------- Liaison des shaders aux programmes:
                //glAttachShader(programme_id1,shader_id1);
                glAttachShader(programme_filtre_HSV_id,shader_filtre_HSV_id);

                //----------- Liaisons des programmes de shaders:
                if (!relie_programme_shader(programme_filtre_HSV_id)) return false;

                //----------- Vérifie la validité des shaders:

                if (!valide_programme_shader(programme_filtre_HSV_id)) return false;

                //----------- Création des liens avec les variables:

                uniform_filtre_HSV_texture=glGetUniformLocation(programme_filtre_HSV_id,"texture");
                uniform_filtre_HSV_Hue=glGetUniformLocation(programme_filtre_HSV_id,"H");
                uniform_filtre_HSV_Saturation=glGetUniformLocation(programme_filtre_HSV_id,"S");
                uniform_filtre_HSV_Value=glGetUniformLocation(programme_filtre_HSV_id,"V");


                return true;
	}

        bool ShadersDemo::initShaders()
        {
                bool erreur=false;

                if (!GLEW_ARB_vertex_program || !GLEW_ARB_fragment_program)
                {
                        cout<<"Impossible d'initialiser les shaders !!!"<<endl;
                        erreur=true;
                }
                if (!glewIsSupported("GL_ARB_shading_language_100")) {cout<<"GLSL non supporté"<<endl; erreur=true;}
                if (!glewIsSupported("GL_ARB_shader_objects")) {cout<<"Objets shaders non supportés"<<endl;erreur=true;}
                if (!glewIsSupported("GL_ARB_vertex_shader")) {cout<<"Shaders de sommets non supportés"<<endl;erreur=true;}
                if (!glewIsSupported("GL_ARB_fragment_shader")) {cout<<"Shaders de fragments non supportés"<<endl;erreur=true;}
                if (erreur) return false;

                 //--------- Création des programmes:=0

                 if(!init_shader_mandelbrot())return false;
                 if(!init_shader_waterworld())return false;
                 if(!init_shader_spheredance())return false;
                 if(!init_shader_hypnostructure())return false;
                 if(!init_shader_alphaDist())return false;
                 if(!init_flou())return false;
                 if(!init_filtres())return false;
                 if(!init_filtre_HSV())return false;

                 return true;
        }

        //---------------------------------------------------------------
        //      Destruction des shaders
        //---------------------------------------------------------------
        void ShadersDemo::freeShaders()
        {
		 //Fractale Mandelbrot
                if (source_fragment_mandelbrot!=NULL) free (source_fragment_mandelbrot);
                if (source_vertex_mandelbrot!=NULL) free (source_vertex_mandelbrot);
                if (programme_mandelbrot!=0)
                {
                        glDetachShader(programme_mandelbrot,shader_mandelbrot_id1);
                        glDetachShader(programme_mandelbrot,shader_mandelbrot_id2);
                        glDeleteProgram(programme_mandelbrot);
                }
                if (shader_mandelbrot_id1!=0) glDeleteShader(shader_mandelbrot_id1);
                if (shader_mandelbrot_id2!=0) glDeleteShader(shader_mandelbrot_id2);

		//Raytracer waterworld
                if (source_fragment_waterworld!=NULL) free (source_fragment_waterworld);
                if (source_vertex_waterworld!=NULL) free (source_vertex_waterworld);
                if (programme_waterworld!=0)
                {
                        glDetachShader(programme_waterworld,shader_waterworld_id1);
                        glDetachShader(programme_waterworld,shader_waterworld_id2);
                        glDeleteProgram(programme_waterworld);
                }
                if (shader_waterworld_id1!=0) glDeleteShader(shader_waterworld_id1);
                if (shader_waterworld_id2!=0) glDeleteShader(shader_waterworld_id2);

        //Raytracer spheredance
                if (source_fragment_spheredance!=NULL) free (source_fragment_spheredance);
                if (source_vertex_spheredance!=NULL) free (source_vertex_spheredance);
                if (programme_spheredance!=0)
                {
                        glDetachShader(programme_spheredance,shader_spheredance_id1);
                        glDetachShader(programme_spheredance,shader_spheredance_id2);
                        glDeleteProgram(programme_spheredance);
                }
                if (shader_spheredance_id1!=0) glDeleteShader(shader_spheredance_id1);
                if (shader_spheredance_id2!=0) glDeleteShader(shader_spheredance_id2);

        //Raytracer hypnostructure
                if (source_fragment_hypnostructure!=NULL) free (source_fragment_hypnostructure);
                if (source_vertex_hypnostructure!=NULL) free (source_vertex_hypnostructure);
                if (programme_hypnostructure!=0)
                {
                        glDetachShader(programme_hypnostructure,shader_hypnostructure_id1);
                        glDetachShader(programme_hypnostructure,shader_hypnostructure_id2);
                        glDeleteProgram(programme_hypnostructure);
                }
                if (shader_hypnostructure_id1!=0) glDeleteShader(shader_hypnostructure_id1);
                if (shader_hypnostructure_id2!=0) glDeleteShader(shader_hypnostructure_id2);

		//Floutage
		if (source_fragment_flou_x!=NULL) free (source_fragment_flou_x);
		if (source_fragment_flou_y!=NULL) free (source_fragment_flou_y);
                if (programme_floux_id!=0)
                {
                        glDetachShader(programme_floux_id,shader_floux_id);
                        glDeleteProgram(programme_floux_id);
                }
                if (programme_flouy_id!=0)
                {
                        glDetachShader(programme_flouy_id,shader_flouy_id);
                        glDeleteProgram(programme_flouy_id);
                }
                if (shader_floux_id!=0) glDeleteShader(shader_floux_id);
                if (shader_flouy_id!=0) glDeleteShader(shader_flouy_id);

            //Filtres
                if (source_fragment_filtres!=NULL) free (source_fragment_filtres);

                if (programme_filtres_id!=0)
                {
                        glDetachShader(programme_filtres_id,shader_filtres_id);
                        glDeleteProgram(programme_filtres_id);
                }

                if (shader_filtres_id!=0) glDeleteShader(shader_filtres_id);

            //Filtre HSV
                if (source_fragment_filtre_HSV!=NULL) free (source_fragment_filtre_HSV);

                if (programme_filtre_HSV_id!=0)
                {
                        glDetachShader(programme_filtre_HSV_id,shader_filtre_HSV_id);
                        glDeleteProgram(programme_filtre_HSV_id);
                }

                if (shader_filtre_HSV_id!=0) glDeleteShader(shader_filtre_HSV_id);

            //Fractale Mandelbrot
                if (source_fragment_alphaDist!=NULL) free (source_fragment_alphaDist);
                if (source_vertex_alphaDist!=NULL) free (source_vertex_alphaDist);
                if (programme_alphaDist!=0)
                {
                        glDetachShader(programme_alphaDist,shader_alphaDist_id1);
                        glDetachShader(programme_alphaDist,shader_alphaDist_id2);
                        glDeleteProgram(programme_alphaDist);
                }
                if (shader_alphaDist_id1!=0) glDeleteShader(shader_alphaDist_id1);
                if (shader_alphaDist_id2!=0) glDeleteShader(shader_alphaDist_id2);

        }

        void ShadersDemo::programme_filtres(GLfloat saturation,GLfloat contraste,GLfloat seuil, GLuint texture)
        {
            glUseProgram(programme_filtres_id);
            glUniform1i(uniform_filtres_texture,texture);
            glUniform1f(uniform_filtres_saturation,saturation);
            glUniform1f(uniform_filtres_contraste,contraste);
            glUniform1f(uniform_filtres_seuil,seuil);
        }

        void ShadersDemo::programme_filtre_HSV(GLfloat hue,GLfloat saturation,GLfloat value, GLuint texture)
        {
            glUseProgram(programme_filtre_HSV_id);
            glUniform1i(uniform_filtre_HSV_texture,texture);
            glUniform1f(uniform_filtre_HSV_Hue,hue);
            glUniform1f(uniform_filtre_HSV_Saturation,saturation);
            glUniform1f(uniform_filtre_HSV_Value,value);
        }

        void ShadersDemo::programme_floux(GLuint taille_hallo,GLfloat saturation_hallo,GLuint texture)
        {
            glUseProgram(programme_floux_id);
            glUniform1i(uniform_floux_taille,taille_hallo);
            glUniform1f(uniform_floux_saturation,saturation_hallo);
            glUniform1i(uniform_floux_texture,texture);
        }

        void ShadersDemo::programme_flouy(GLuint taille_hallo,GLfloat saturation_hallo,GLuint texture)
        {
            glUseProgram(programme_flouy_id);
            glUniform1i(uniform_flouy_taille,taille_hallo);
            glUniform1f(uniform_flouy_saturation,saturation_hallo);
            glUniform1i(uniform_flouy_texture,texture);
        }

