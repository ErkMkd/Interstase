

/*
        Classe de base de définition des shaders
*/

#include "Shader.h"
using namespace std;

        Shader::Shader(const char* p_nom,char* p_source_vertex,char* p_source_fragment)
        {
            erreur=SHADER_OK;
            programme_id=0;
            source_vertex=NULL;
            source_fragment=NULL;
            vertex_id=0;
            fragment_id=0;

            memcpy(nom,p_nom,strlen(p_nom)+1);

            if(SHADER_DEBUG) cout<<endl<<"$$$$$$$$$$$$$$$$$$$$ Création du Shader: "<<nom<<endl;

            if(p_source_vertex!=NULL) drapeau_vertex=true; else drapeau_vertex=false;
            if(p_source_fragment!=NULL) drapeau_fragment=true; else drapeau_fragment=false;

            if(drapeau_vertex) source_vertex=charge_source_shader(p_source_vertex);
            if(drapeau_fragment) source_fragment=charge_source_shader(p_source_fragment);

            if ((drapeau_vertex && source_vertex==NULL) || (drapeau_fragment && source_fragment==NULL))
            {
                erreur=SHADER_ERREUR_SOURCE;
                return;
            }

            //---------- Création des shaders:
            if (SHADER_DEBUG) cout<<"Création des objets shaders...";
            if(drapeau_vertex) vertex_id=glCreateShader(GL_VERTEX_SHADER);
            if(drapeau_fragment) fragment_id=glCreateShader(GL_FRAGMENT_SHADER);
            if ((drapeau_vertex && vertex_id==0) || (drapeau_fragment && fragment_id==0))
            {
                cout<<"ERREUR dans Shader::Shader() - Impossible de créer les objets Shader"<<endl;
                erreur=SHADER_ERREUR_OBJETS;
                return;
            }
            if (SHADER_DEBUG) cout<<"OK"<<endl;

            //---------- Lie les sources avec les objets shader:
            if (SHADER_DEBUG) cout<<"Liaisons des sources aux shaders...";

            if(drapeau_vertex)
            {
                tableau_sources[0]=source_vertex;
                glShaderSource(vertex_id,1,tableau_sources,NULL);
            }
            if(drapeau_fragment)
            {
                tableau_sources[0]=source_fragment;
                glShaderSource(fragment_id,1,tableau_sources,NULL);
            }

            if (SHADER_DEBUG) cout<<"OK"<<endl;

            //---------- Compilation des shaders:
            if (SHADER_DEBUG) cout<<"Compilation des shaders...";
            if(drapeau_vertex)
            {
                if(!compile_shader(vertex_id))
                {
                    erreur=SHADER_ERREUR_COMPILATION;
                    return;
                }

            }

            if(drapeau_fragment)
            {
                if(!compile_shader(fragment_id))
                {
                    erreur=SHADER_ERREUR_COMPILATION;
                    return;
                }

            }

            if (SHADER_DEBUG) cout<<"OK"<<endl;

            //----------- Création du programme
            if (SHADER_DEBUG) cout<<"Création du programme...";

            programme_id=glCreateProgram();
            if (programme_id==0)
            {
                cout<<"ERREUR dans Shader::Shader() - Impossible de créer le programme"<<endl;
                erreur=SHADER_ERREUR_PROGRAMME;
                return;
            }

            if (SHADER_DEBUG) cout<<"OK"<<endl;

            //----------- Liaison des shaders aux programmes:
            if(drapeau_vertex)glAttachShader(programme_id,vertex_id);
            if(drapeau_fragment)glAttachShader(programme_id,fragment_id);

            //----------- Liaisons des programmes de shaders:

            if (SHADER_DEBUG) cout<<"Liaison du programme...";

            if (!relie_programme_shader(programme_id))
            {
                cout<<"ERREUR dans Shaders::Shaders() - Impossible de lier le programme"<<endl;
                erreur=SHADER_ERREUR_LIAISON;
                return;
            }

            if (SHADER_DEBUG) cout<<"OK"<<endl;

            //----------- Vérifie la validité des shaders:
            if (SHADER_DEBUG) cout<<"Validation du programme ...";
            if (!valide_programme_shader(programme_id))
            {
                cout<<"ERREUR dans Shader::Shader() - Le programme est invalide sur cette machine!"<<endl;
                erreur=SHADER_ERREUR_VALIDATION;
                return;
            }
            if (SHADER_DEBUG) cout<<"OK"<<endl;

            //--------- Erreur OpenGl:
            if (erreur_openGl("ERREUR dans Shader::Shader() :")) erreur=SHADER_ERREUR_OPENGL;

        }

        //=====================================
        //      Destructeur
        //=====================================
        Shader::~Shader()
        {
            if (programme_id!=0) glDeleteProgram(programme_id);
            if(fragment_id!=0) glDeleteShader(fragment_id);
            if(vertex_id!=0) glDeleteShader(vertex_id);
            if (source_vertex!=NULL) free(source_vertex);
            if (source_fragment!=NULL) free(source_fragment);
        }


