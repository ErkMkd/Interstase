

#include "GestionShaders.h"

using namespace std;

/*

       Fonctions de base pour la gestion des shaders

*/

        bool GestionShaders::erreur_openGl(const char* message)
        {
            GLenum erreur_openGl=glGetError();
            if (erreur_openGl!=GL_NO_ERROR)
            {
                cout<<message<<" : "<<gluErrorString(erreur_openGl)<<endl;
                return true;
            }
            return false;
        }
        //-----------------------------------------
        //  Charge un fichier source d'un shader:
        //-----------------------------------------
        char* GestionShaders::charge_source_shader(const char* nom_fichier)
        {
            cout <<"------- Chargement du fichier source: "<<nom_fichier<<endl;
            ifstream fichier_flux;
            uint32_t taille_fichier;

            fichier_flux.open(nom_fichier, ios::in | ios::binary | ios::ate);
            if (!fichier_flux.is_open())
                {
                    std::cout<<"ERREUR - Impossible d'ouvrir le fichier "<<nom_fichier<<std::endl;
                    return NULL;
                }
            //---Taille du fichier:
            taille_fichier=fichier_flux.tellg();
            cout<<"Taille du fichier: "<<taille_fichier<<endl;
            //--- Pointeur au début du fichier:
            fichier_flux.seekg(0,ios::beg);
            GLchar* texte =(GLchar*)malloc((taille_fichier+1)*sizeof(GLchar));
            if (texte==NULL)
            {
                cout<<"ERREUR - Mémoire insuffisante pour charger le fichier "<<nom_fichier<<endl;
                fichier_flux.close();
                return NULL;
            }
            fichier_flux.read(texte,taille_fichier);
            fichier_flux.close();
            texte[taille_fichier]=0;
            //cout<<texte<<endl;
            return texte;
        }

        //-----------------------------------------
        //  Compile un shader
        //-----------------------------------------

        bool GestionShaders::compile_shader(GLuint shader_id)
        {
            char message_erreur[TAILLEMSG_GLSL];
            GLint taille_message;
            GLint status;
            message_erreur[TAILLEMSG_GLSL-1]=0;

            glCompileShader(shader_id);
            glGetShaderiv(shader_id,GL_COMPILE_STATUS,&status);
            if (status==GL_FALSE)
            {
                glGetShaderiv(shader_id,GL_INFO_LOG_LENGTH,&taille_message);
                glGetShaderInfoLog(shader_id,TAILLEMSG_GLSL-1,&taille_message,message_erreur);
                cout<<"ERREUR de compilation du shader "<<shader_id<<":"<<endl;
                cout<<"Taille du message:"<<taille_message<<endl;
                cout<<message_erreur<<endl;
                return false;
            }
            cout<<"Shader "<<shader_id<<" compilé avec succès"<<endl;
            return true;
        }

        //-----------------------------------------
        //  Liaisons d'un programme de shaders
        //-----------------------------------------
        bool GestionShaders::relie_programme_shader(GLuint programme_id)
        {
            char message_erreur[TAILLEMSG_GLSL];
            GLint taille_message;
            GLint status;
            message_erreur[TAILLEMSG_GLSL-1]=0;

            glLinkProgram(programme_id);
            glGetProgramiv(programme_id,GL_LINK_STATUS,&status);
            if (status==GL_FALSE)
            {
                glGetProgramiv(programme_id,GL_INFO_LOG_LENGTH,&taille_message);
                glGetProgramInfoLog(programme_id,TAILLEMSG_GLSL-1,&taille_message,message_erreur);
                cout<<"ERREUR de liaison du programme de shaders "<<programme_id<<":"<<endl;
                cout<<message_erreur<<endl;
                return false;
            }

            cout<<"Programme de shaders "<<programme_id<<" lié avec succès"<<endl;
            return true;
        }

        //-----------------------------------------
        //  Validation d'un programme de shaders
        //-----------------------------------------
        bool GestionShaders::valide_programme_shader(GLuint programme_id)
        {
            GLint status;
            glValidateProgram(programme_id);
            glGetProgramiv(programme_id,GL_VALIDATE_STATUS,&status);
            if (status==GL_FALSE)
            {
                cout<<"ERREUR le programme de shader "<<programme_id<<" n'est pas valide!"<<endl;
                return false;
            }
            cout<<"Le programme de shader "<<programme_id<<" est opérationnel"<<endl;
            return true;
        }
