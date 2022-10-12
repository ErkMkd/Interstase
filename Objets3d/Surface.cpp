

/// Les textures de la surface sont détruites par celle-ci !!!

//----------------------------------
//  Structure des surfaces:
//
//  Une surface ne peut être rattachée qu'à un seul volume.
//  Si la surface est opaque, il n'y a pas de tri des faces.
//  Si la surface est transparente, ses faces sont stockées dans la table des faces transparentes du volume.
//     Une surface est considérée comme transparente si:
//                  -son niveau de transparence > 0,
//                  -la texture de couleur a une couche alpha non uniforme ou <> 0xff
//                  -il y a une texture de transparence
//
//          a priori, la transparence d'un fragment est calculé comme suit:
//                  Tf=transparence+Af+Tt avec une limitation à 1. Af=Alpha, Tt=transparence de la texture de transparence
//
// Le nombre maximum de textures prises en compte dépend de l'implémentation de gestion du mode "Multitextures" d'OpenGl.
// En cas de nombre de textures supérieur au nombre maximum de textures max d'OpenGl, un ordre de priorité est appliqué,
// correspondant à l'ordre de définition des vecteurs de textures ci-après.
//
//----------------------------------

#include "Surface.h"

        //----------------------------------------------------
        //Constructeur:
        //----------------------------------------------------

        Surface::Surface(char* p_nom)
        {
            erreur=SURFACE_OK;
            uint32_t i,taille;
            normales_sommets=NULL;
            nbr_faces=0;

            if (p_nom==NULL) nom[0]=0;
            else
            {
                taille=strlen(p_nom);
                if (taille>256-1)taille=256-1;
                for(i=0;i<taille;i++)
                {
                    nom[i]=p_nom[i];
                }
                nom[i]=0;
            }

            //Couleurs:
            initRVBA(couleur_ambiance,1.,1.,1.);
            initRVBA(couleur_diffusion,.8,.8,.8);
            initRVBA(couleur_speculaire,1.,1.,1.);
            initRVBA(couleur_luminescence,1.,1.,1.);

            //Paramètres de base:
            ambiance=1.;
            specularite=0.5;
            diffusion=1.;
            luminescence=0.;
            brillance=10.;
            transparence=0.;
            transparence_speculaire=0.;
            translucidite=0.;
            reflexion=0.;
            rugosite=0.;

            drapeau_transparence=false;
            drapeau_adoucissement=false;
            angle_limite_adoucissement=89.;
            double_face=false;

            //Ondes aqueuses:
            intensite_ondes=0.;
            amplitude_ondes=0.2;
            frequence_ondes=2.;
            vitesse_ondes=1.5;
            quantite_ondes=5.;
            perturbation_ondes=7;
            echelle_perturbation_ondes=10.;

            //Transition:
            indice_texture_depart=-1;
            indice_texture_destination=-1;
            indice_texture_transition=-1;

            //Mode de rendu par défaut:
            bits_rendus=0;
            mode_rendu=SURFACE_RENDU_COULEUR;
            drapeau_projete_ombres=false;
            drapeau_recoit_ombres=false;
            drapeau_phong=false;
            drapeau_transition=false;
            drapeau_effet_special=false;

            //Indices des textures par défaut:
            indice_texture_couleur=0;
        }

        //----------------------------------------------------
        //Constructeur de copie:
        //Les faces ne sont pas copiées.
        //----------------------------------------------------
        Surface::Surface(Surface& surface_originale)
        {
            uint32_t i;
            nbr_faces=0;
            erreur=surface_originale.erreur;
            for(i=0;i<256;i++)
            {
                nom[i]=surface_originale.nom[i];
                if (nom[i]==0) break;
            }
            volume=surface_originale.volume;    ///COHERENCE A REVOIR - Logiquement, la nouvelle surface n'est pas encore liée à un volume.
                                                ///C'est ce dernier qui va initialiser l'ensemble de la nouvelle surface.
            double_face=surface_originale.double_face;
            drapeau_adoucissement=surface_originale.drapeau_adoucissement;
            angle_limite_adoucissement=surface_originale.angle_limite_adoucissement;
            drapeau_transparence=surface_originale.drapeau_transparence;        /// Idem
            for(i=0;i<4;i++)
            {
                couleur_diffusion[i]=surface_originale.couleur_diffusion[i];
                couleur_luminescence[i]=surface_originale.couleur_luminescence[i];
                couleur_ambiance[i]=surface_originale.couleur_ambiance[i];
                couleur_speculaire[i]=surface_originale.couleur_speculaire[i];
            }
            luminescence=surface_originale.luminescence;
            diffusion=surface_originale.diffusion;
            specularite=surface_originale.specularite;
            ambiance=surface_originale.ambiance;
            transparence=surface_originale.transparence;
            transparence_speculaire=surface_originale.transparence_speculaire;
            translucidite=surface_originale.translucidite;
            brillance=surface_originale.brillance;
            reflexion=surface_originale.reflexion;
            rugosite=surface_originale.rugosite;

            intensite_ondes=surface_originale.intensite_ondes;
            amplitude_ondes=surface_originale.amplitude_ondes;
            frequence_ondes=surface_originale.frequence_ondes;
            vitesse_ondes=surface_originale.vitesse_ondes;
            quantite_ondes=surface_originale.quantite_ondes;
            perturbation_ondes=surface_originale.perturbation_ondes;
            echelle_perturbation_ondes=surface_originale.echelle_perturbation_ondes;


            mode_rendu=surface_originale.mode_rendu;
            bits_rendus=surface_originale.bits_rendus;
            drapeau_projete_ombres=surface_originale.drapeau_projete_ombres;
            drapeau_recoit_ombres=surface_originale.drapeau_recoit_ombres;
            drapeau_phong=surface_originale.drapeau_phong;
            drapeau_transition=surface_originale.drapeau_transition;
            drapeau_effet_special=surface_originale.drapeau_effet_special;

            indice_texture_couleur=surface_originale.indice_texture_couleur;


            //---------Copie les textures

            // Ambiance
            for (i=0;i<surface_originale.textures_ambiance.size();i++)
            {
                Texture* nouvelle_texture=new Texture(*surface_originale.textures_ambiance[i]);
                textures_ambiance.push_back(nouvelle_texture);
                textures.push_back(nouvelle_texture);
            }
            for (i=0;i<surface_originale.textures_ambiance_inactives.size();i++)
            {
                Texture* nouvelle_texture=new Texture(*surface_originale.textures_ambiance_inactives[i]);
                textures_ambiance_inactives.push_back(nouvelle_texture);
                textures.push_back(nouvelle_texture);
            }

            //Couleur
            for (i=0;i<surface_originale.textures_couleur.size();i++)
            {
                Texture* nouvelle_texture=new Texture(*surface_originale.textures_couleur[i]);
                textures_couleur.push_back(nouvelle_texture);
                textures.push_back(nouvelle_texture);
            }
            for (i=0;i<surface_originale.textures_couleur_inactives.size();i++)
            {
                Texture* nouvelle_texture=new Texture(*surface_originale.textures_couleur_inactives[i]);
                textures_couleur_inactives.push_back(nouvelle_texture);
                textures.push_back(nouvelle_texture);
            }

            //Diffusion
            for (i=0;i<surface_originale.textures_diffusion.size();i++)
            {
                Texture* nouvelle_texture=new Texture(*surface_originale.textures_diffusion[i]);
                textures_diffusion.push_back(nouvelle_texture);
                textures.push_back(nouvelle_texture);
            }
            for (i=0;i<surface_originale.textures_diffusion_inactives.size();i++)
            {
                Texture* nouvelle_texture=new Texture(*surface_originale.textures_diffusion_inactives[i]);
                textures_diffusion_inactives.push_back(nouvelle_texture);
                textures.push_back(nouvelle_texture);
            }

            //Luminescence
            for (i=0;i<surface_originale.textures_luminescence.size();i++)
            {
                Texture* nouvelle_texture=new Texture(*surface_originale.textures_luminescence[i]);
                textures_luminescence.push_back(nouvelle_texture);
                textures.push_back(nouvelle_texture);
            }
            for (i=0;i<surface_originale.textures_luminescence_inactives.size();i++)
            {
                Texture* nouvelle_texture=new Texture(*surface_originale.textures_luminescence_inactives[i]);
                textures_luminescence_inactives.push_back(nouvelle_texture);
                textures.push_back(nouvelle_texture);
            }

            //Réflexion:
            for (i=0;i<surface_originale.textures_reflexion.size();i++)
            {
                Texture* nouvelle_texture=new Texture(*surface_originale.textures_reflexion[i]);
                textures_reflexion.push_back(nouvelle_texture);
                textures.push_back(nouvelle_texture);
            }
            for (i=0;i<surface_originale.textures_reflexion_inactives.size();i++)
            {
                Texture* nouvelle_texture=new Texture(*surface_originale.textures_reflexion_inactives[i]);
                textures_reflexion_inactives.push_back(nouvelle_texture);
                textures.push_back(nouvelle_texture);
            }

            //Rugosité:
            for (i=0;i<surface_originale.textures_rugosite.size();i++)
            {
                Texture* nouvelle_texture=new Texture(*surface_originale.textures_rugosite[i]);
                textures_rugosite.push_back(nouvelle_texture);
                textures.push_back(nouvelle_texture);
            }
            for (i=0;i<surface_originale.textures_rugosite_inactives.size();i++)
            {
                Texture* nouvelle_texture=new Texture(*surface_originale.textures_rugosite_inactives[i]);
                textures_rugosite_inactives.push_back(nouvelle_texture);
                textures.push_back(nouvelle_texture);
            }

            //Transparence:
            for (i=0;i<surface_originale.textures_transparence.size();i++)
            {
                Texture* nouvelle_texture=new Texture(*surface_originale.textures_transparence[i]);
                textures_transparence.push_back(nouvelle_texture);
                textures.push_back(nouvelle_texture);
            }
            for (i=0;i<surface_originale.textures_transparence_inactives.size();i++)
            {
                Texture* nouvelle_texture=new Texture(*surface_originale.textures_transparence_inactives[i]);
                textures_transparence_inactives.push_back(nouvelle_texture);
                textures.push_back(nouvelle_texture);
            }
        }

        //----------------------------------------------------
        //Destructeur:
        /// Les textures sont détruites !
        //----------------------------------------------------
        Surface::~Surface()
        {
            uint32_t i;
            active_toutes_les_textures();
            for(i=0;i<textures_ambiance.size();i++)
            {
                if (textures_ambiance[i]!=NULL) delete textures_ambiance[i];
            }

            for(i=0;i<textures_couleur.size();i++)
            {
                if (textures_couleur[i]!=NULL) delete textures_couleur[i];
            }

            for(i=0;i<textures_diffusion.size();i++)
            {
                if (textures_diffusion[i]!=NULL) delete textures_diffusion[i];
            }

            for(i=0;i<textures_luminescence.size();i++)
            {
                if (textures_luminescence[i]!=NULL) delete textures_luminescence[i];
            }

            for(i=0;i<textures_reflexion.size();i++)
            {
                if (textures_reflexion[i]!=NULL) delete textures_reflexion[i];
            }

            for(i=0;i<textures_rugosite.size();i++)
            {
                if (textures_rugosite[i]!=NULL) delete textures_rugosite[i];
            }

            for(i=0;i<textures_transparence.size();i++)
            {
                if (textures_transparence[i]!=NULL) delete textures_transparence[i];
            }
            if (normales_sommets!=NULL) free (normales_sommets);
        }

        //----------------------------------------------------
        //              Gestion du matériau:
        //----------------------------------------------------

        void Surface::initRVBA(float* couleur,float r, float v, float b, float a)
        {
            couleur[0]=r;couleur[1]=v;couleur[2]=b;couleur[3]=a;
        }

        void Surface::determine_couleur_ambiance(uint32_t couleur, float intensite)
        {
            determine_couleur_ARVB(couleur_ambiance,couleur);
            ambiance=intensite;
        }

        void Surface::determine_couleur_diffusion(uint32_t couleur, float intensite)
        {
            determine_couleur_ARVB(couleur_diffusion,couleur);
            diffusion=intensite;
        }

        void Surface::determine_couleur_speculaire(uint32_t couleur, float intensite)
        {
            determine_couleur_ARVB(couleur_speculaire,couleur);
            specularite=intensite;
        }

        void Surface::determine_couleur_luminescence(uint32_t couleur, float intensite)
        {
            determine_couleur_ARVB(couleur_luminescence,couleur);
            luminescence=intensite;
        }

        void Surface::determine_ambiance(float intensite)
        {
            ambiance=intensite;
        }

        void Surface::determine_diffusion(float intensite)
        {
            diffusion=intensite;
        }

        void Surface::determine_specularite(float intensite)
        {
            specularite=intensite;
        }

        void Surface::determine_luminescence(float intensite)
        {
            luminescence=intensite;
        }

        void Surface::determine_transparence(float p_transparence)
        {
            transparence=p_transparence;
            if (transparence>0.)drapeau_transparence=true;
            else drapeau_transparence=false;
        }

        void Surface::determine_reflexion(float p_reflexion)
        {
            reflexion=p_reflexion;
            initialise_mode_rendu();
        }

        void Surface::determine_transparence_speculaire(float p_transparence)
        {
            transparence_speculaire=p_transparence;
        }

        void Surface::determine_intensite_ondes(float p_intensite)
        {
            intensite_ondes=p_intensite;
            initialise_mode_rendu();
        }

        void Surface::applique_teinte_diffusion(uint32_t teinte)
        {
            applique_teinte_ARVB(couleur_diffusion,teinte);
        }

        void Surface::applique_teinte_specularite(uint32_t teinte)
        {
            applique_teinte_ARVB(couleur_speculaire,teinte);
        }
        void Surface::applique_teinte_luminescence(uint32_t teinte)
        {
            applique_teinte_ARVB(couleur_luminescence,teinte);
        }
        void Surface::applique_teinte_ambiance(uint32_t teinte)
        {
            applique_teinte_ARVB(couleur_ambiance,teinte);
        }

        void Surface::determine_couleur_ARVB(float* tab,uint32_t couleur)
        {
            tab[0]=(float)((couleur>>16)&0xff)/255.;
            tab[1]=(float)((couleur>>8)&0xff)/255.;
            tab[2]=(float)(couleur&0xff)/255.;
            tab[3]=(float)((couleur>>24)&0xff)/255.;
        }

        void Surface::applique_teinte_ARVB(float* tab, uint32_t teinte)
        {
            float r=(float)((teinte>>16)&0xff)/255.;
            float v=(float)((teinte>>8)&0xff)/255.;
            float b=(float)(teinte & 0xff)/255.;
            float a=(float)((teinte>>24)&0xff)/255.;
            tab[0]*=r;
            tab[1]*=v;
            tab[2]*=b;
            tab[3]*=a;

            /*
            tab[0]/=2.;
            tab[1]/=2.;
            tab[2]/=2.;
            tab[3]/=2.;
            */
        }

        //----------------------------------------------------
        //              Gestion des textures:
        //----------------------------------------------------

        Texture* Surface::renvoie_texture_nom(char* nom_texture)
        {
            for (uint32_t i=0;i<textures.size();i++)
            {
                if(strcmp(textures[i]->nom,nom_texture)==0) return textures[i];
            }
            return NULL;
        }


        //Ajoute une texture:
        //La texture est active par défaut
        void Surface::ajoute_texture(Texture* texture)
        {
            textures.push_back(texture);
            switch(texture->canal)
            {
                case TEXTURE_CANAL_AMBIANCE:
                    if(texture->image==NULL)textures_ambiance_inactives.push_back(texture);
                    else textures_ambiance.push_back(texture);
                    break;
                case TEXTURE_CANAL_COULEUR:
                    if(texture->image==NULL)textures_couleur_inactives.push_back(texture);
                    else textures_couleur.push_back(texture);
                    break;
                case TEXTURE_CANAL_DIFFUSION:
                    if(texture->image==NULL)textures_diffusion_inactives.push_back(texture);
                    else textures_diffusion.push_back(texture);
                    break;
                case TEXTURE_CANAL_LUMINESCENCE:
                    if(texture->image==NULL)textures_luminescence_inactives.push_back(texture);
                    else textures_luminescence.push_back(texture);
                    break;
                case TEXTURE_CANAL_REFLEXION:
                    if(texture->image==NULL)textures_reflexion_inactives.push_back(texture);
                    else textures_reflexion.push_back(texture);
                    break;
                case TEXTURE_CANAL_RUGOSITE:
                    if(texture->image==NULL)textures_rugosite_inactives.push_back(texture);
                    else textures_rugosite.push_back(texture);
                    break;
                case TEXTURE_CANAL_TRANSPARENCE:
                    if(texture->image==NULL)textures_transparence_inactives.push_back(texture);
                    else textures_transparence.push_back(texture);
                    break;
                default:
                    cout<<"Avertissement dans Surface::ajoute_texture(): texture de type inconnu"<<endl;
                    break;
            }
        }

        bool Surface::texture_est_active(Texture* texture)
        {
            uint16_t i;
            switch (texture->canal)
            {
                case TEXTURE_CANAL_COULEUR: for (i=0;i<textures_couleur.size();i++) if (textures_couleur[i]==texture) return true; break;
                case TEXTURE_CANAL_AMBIANCE: for (i=0;i<textures_ambiance.size();i++) if (textures_ambiance[i]==texture) return true; break;
                case TEXTURE_CANAL_DIFFUSION: for (i=0;i<textures_diffusion.size();i++) if (textures_diffusion[i]==texture) return true; break;
                case TEXTURE_CANAL_LUMINESCENCE: for (i=0;i<textures_luminescence.size();i++) if (textures_luminescence[i]==texture) return true; break;
                case TEXTURE_CANAL_REFLEXION: for (i=0;i<textures_reflexion.size();i++) if (textures_reflexion[i]==texture) return true; break;
                case TEXTURE_CANAL_RUGOSITE: for (i=0;i<textures_rugosite.size();i++) if (textures_rugosite[i]==texture) return true; break;
                case TEXTURE_CANAL_TRANSPARENCE: for (i=0;i<textures_transparence.size();i++) if (textures_transparence[i]==texture) return true; break;
            }
            return false;
        }

        //Renvoie 0xffff si la texture n'est pas dans les tables:
        uint16_t Surface::renvoie_texture_active_index(Texture* texture)
        {
            uint16_t i;
            switch (texture->canal)
            {
                case TEXTURE_CANAL_COULEUR: for (i=0;i<textures_couleur.size();i++) if (textures_couleur[i]==texture) return i; break;
                case TEXTURE_CANAL_AMBIANCE: for (i=0;i<textures_ambiance.size();i++) if (textures_ambiance[i]==texture) return i; break;
                case TEXTURE_CANAL_DIFFUSION: for (i=0;i<textures_diffusion.size();i++) if (textures_diffusion[i]==texture) return i; break;
                case TEXTURE_CANAL_LUMINESCENCE: for (i=0;i<textures_luminescence.size();i++) if (textures_luminescence[i]==texture) return i; break;
                case TEXTURE_CANAL_REFLEXION: for (i=0;i<textures_reflexion.size();i++) if (textures_reflexion[i]==texture) return i; break;
                case TEXTURE_CANAL_RUGOSITE: for (i=0;i<textures_rugosite.size();i++) if (textures_rugosite[i]==texture) return i; break;
                case TEXTURE_CANAL_TRANSPARENCE: for (i=0;i<textures_transparence.size();i++) if (textures_transparence[i]==texture) return i; break;
            }
            return 0xffff;
        }

        //Renvoie 0xffff si la texture n'est pas dans les tables:
        uint16_t Surface::renvoie_texture_inactive_index(Texture* texture)
        {
            uint16_t i;
            switch (texture->canal)
            {
                case TEXTURE_CANAL_COULEUR: for (i=0;i<textures_couleur_inactives.size();i++) if (textures_couleur_inactives[i]==texture) return i; break;
                case TEXTURE_CANAL_AMBIANCE: for (i=0;i<textures_ambiance_inactives.size();i++) if (textures_ambiance_inactives[i]==texture) return i; break;
                case TEXTURE_CANAL_DIFFUSION: for (i=0;i<textures_diffusion_inactives.size();i++) if (textures_diffusion_inactives[i]==texture) return i; break;
                case TEXTURE_CANAL_LUMINESCENCE: for (i=0;i<textures_luminescence_inactives.size();i++) if (textures_luminescence_inactives[i]==texture) return i; break;
                case TEXTURE_CANAL_REFLEXION: for (i=0;i<textures_reflexion_inactives.size();i++) if (textures_reflexion_inactives[i]==texture) return i; break;
                case TEXTURE_CANAL_RUGOSITE: for (i=0;i<textures_rugosite_inactives.size();i++) if (textures_rugosite_inactives[i]==texture) return i; break;
                case TEXTURE_CANAL_TRANSPARENCE: for (i=0;i<textures_transparence_inactives.size();i++) if (textures_transparence_inactives[i]==texture) return i; break;
            }
            return 0xffff;
        }

        void Surface::desactive_texture(Texture* texture)
        {
            uint16_t i=renvoie_texture_active_index(texture);
            if (i==0xffff) return;

            //vector<Texture*>::iterator it;

            switch (texture->canal)
            {
                case TEXTURE_CANAL_COULEUR: textures_couleur.erase(textures_couleur.begin()+i); textures_couleur_inactives.push_back(texture); break;
                case TEXTURE_CANAL_AMBIANCE: textures_ambiance.erase(textures_ambiance.begin()+i); textures_ambiance_inactives.push_back(texture); break;
                case TEXTURE_CANAL_DIFFUSION: textures_diffusion.erase(textures_diffusion.begin()+i); textures_diffusion_inactives.push_back(texture); break;
                case TEXTURE_CANAL_LUMINESCENCE: textures_luminescence.erase(textures_luminescence.begin()+i); textures_luminescence_inactives.push_back(texture); break;
                case TEXTURE_CANAL_REFLEXION: textures_reflexion.erase(textures_reflexion.begin()+i); textures_reflexion_inactives.push_back(texture); break;
                case TEXTURE_CANAL_RUGOSITE:
                    cout<<"DESACTIVE LA TEXTURE DE RUGOSITE"<<endl;
                    textures_rugosite.erase(textures_rugosite.begin()+i); textures_rugosite_inactives.push_back(texture);
                    break;
                case TEXTURE_CANAL_TRANSPARENCE: textures_transparence.erase(textures_transparence.begin()+i); textures_transparence_inactives.push_back(texture); break;
            }
        }

        void Surface::active_texture(Texture* texture)
        {
            if(texture->image==NULL)return;
            uint16_t i=renvoie_texture_inactive_index(texture);
            if (i==0xffff) return;

            switch (texture->canal)
            {
                case TEXTURE_CANAL_COULEUR: textures_couleur_inactives.erase(textures_couleur_inactives.begin()+i); textures_couleur.push_back(texture); break;
                case TEXTURE_CANAL_AMBIANCE: textures_ambiance_inactives.erase(textures_ambiance_inactives.begin()+i); textures_ambiance.push_back(texture); break;
                case TEXTURE_CANAL_DIFFUSION: textures_diffusion_inactives.erase(textures_diffusion_inactives.begin()+i); textures_diffusion.push_back(texture); break;
                case TEXTURE_CANAL_LUMINESCENCE: textures_luminescence_inactives.erase(textures_luminescence_inactives.begin()+i); textures_luminescence.push_back(texture); break;
                case TEXTURE_CANAL_REFLEXION: textures_reflexion_inactives.erase(textures_reflexion_inactives.begin()+i); textures_reflexion.push_back(texture); break;
                case TEXTURE_CANAL_RUGOSITE: textures_rugosite_inactives.erase(textures_rugosite_inactives.begin()+i); textures_rugosite.push_back(texture); break;
                case TEXTURE_CANAL_TRANSPARENCE: textures_transparence_inactives.erase(textures_transparence_inactives.begin()+i); textures_transparence.push_back(texture); break;
            }
        }

        ///ATTENTION !!! MEME LES TEXTURES SANS IMAGE DEFINIE SONT ACTIVEES!! CRASH ASSURE!!!
        void Surface::active_toutes_les_textures()
        {
            uint16_t i;
            for(i=0;i<textures_ambiance_inactives.size();i++) textures_ambiance.push_back(textures_ambiance_inactives[i]);
            for(i=0;i<textures_couleur_inactives.size();i++) textures_couleur.push_back(textures_couleur_inactives[i]);
            for(i=0;i<textures_diffusion_inactives.size();i++) textures_diffusion.push_back(textures_diffusion_inactives[i]);
            for(i=0;i<textures_luminescence_inactives.size();i++) textures_luminescence_inactives.push_back(textures_luminescence_inactives[i]);
            for(i=0;i<textures_reflexion_inactives.size();i++) textures_reflexion.push_back(textures_reflexion_inactives[i]);
            for(i=0;i<textures_rugosite_inactives.size();i++) textures_rugosite.push_back(textures_rugosite_inactives[i]);
            for(i=0;i<textures_transparence_inactives.size();i++) textures_transparence.push_back(textures_transparence_inactives[i]);
            textures_ambiance_inactives.clear();
            textures_couleur_inactives.clear();
            textures_diffusion_inactives.clear();
            textures_luminescence_inactives.clear();
            textures_reflexion_inactives.clear();
            textures_rugosite_inactives.clear();
            textures_transparence_inactives.clear();

        }

         //----------------------------------------------------
        //              Gestion de l'adoucissement
        //----------------------------------------------------

        //Réinitialise les listes d'adoucissement:

         ///Les drapeaux des faces doivent être correctement initialisés
        void Surface::rafraichi_faces_adoucies()
        {
            faces_adoucies.clear();
            faces_partiellement_adoucies.clear();
            for (uint32_t i=0;i< nbr_faces;i++ )
            {
                if(faces[i]->drapeau_partiellement_adoucie) faces_partiellement_adoucies.push_back(faces[i]);
                else faces_adoucies.push_back(faces[i]);
            }
        }

        void Surface::ajoute_face_adoucie(Face* face)
        {
            faces_adoucies.push_back(face);
        }

        void Surface::ajoute_face_partiellement_adoucie(Face* face)
        {
            faces_partiellement_adoucies.push_back(face);
        }

        //-------------------------------------------------------
        //      Gestion du tableau de normales des sommets
        //-------------------------------------------------------
        bool Surface::alloue_tableau_normales_sommets(uint32_t p_nbrSommets)
        {
            normales_sommets=(GLfloat*)malloc(p_nbrSommets*sizeof(GLfloat)*3);
            if (normales_sommets==NULL) return false;
            nbrNormales=p_nbrSommets;
            return true;
        }

        void Surface::inverse_normales()
        {
            for (uint32_t i=0;i<nbrNormales;i++)
            {
                normales_sommets[3*i]=-normales_sommets[3*i];
                normales_sommets[3*i+1]=-normales_sommets[3*i+1];
                normales_sommets[3*i+2]=-normales_sommets[3*i+2];
            }
        }

        //=======================================================
        //              Gestion des modes de rendu:
        //=======================================================


        void Surface::determine_phong(bool p_drapeau)
        {
            drapeau_phong=p_drapeau;
            initialise_mode_rendu();
        }

        void Surface::determine_transition(bool p_drapeau)
        {
            drapeau_transition=p_drapeau;
            initialise_mode_rendu();
        }


        //------- Le mode de rendu est déterminé en fonction des paramètres de la surface:
        void Surface::initialise_mode_rendu()
        {
            bits_rendus=0;
            if(drapeau_effet_special)
            {
                mode_rendu=SURFACE_RENDU_EFFET_SPECIAL;
                return;
            }
            if (textures_couleur.size()>0 && textures_couleur[0]->type_projection==TEXTURE_PROJECTION_UV) bits_rendus|=SURFACE_MASK_TEXTURE_COULEUR;
            if (textures_rugosite.size()>0 && textures_rugosite[0]->type_projection==TEXTURE_PROJECTION_UV) bits_rendus|=SURFACE_MASK_RUGOSITE;
            if (reflexion>0.) bits_rendus|=SURFACE_MASK_REFLEXION_CUBE;
            if (intensite_ondes>0.) bits_rendus|=SURFACE_MASK_ONDES;
            if (drapeau_recoit_ombres) bits_rendus|=SURFACE_MASK_OMBRES;
            if(drapeau_phong)
            {
               if((bits_rendus&(SURFACE_MASK_ONDES|SURFACE_MASK_RUGOSITE))==0) bits_rendus|=SURFACE_MASK_PHONG;
            }
            //Pour le moment, la transition se fait sans autre rendu.
            if(drapeau_transition)
            {
                mode_rendu=SURFACE_RENDU_TRANSITION_TEXTURES_COULEUR;
                return;
            }

            //Génère le code du shader:
            mode_rendu=bits_rendus;
        }

        //-----------------------------------------------------
        //          Gestion des ombres
        //-----------------------------------------------------
        void Surface::determine_recoit_ombres(bool d)
        {
            drapeau_recoit_ombres=d;
            initialise_mode_rendu();
        }

        void Surface::determine_projete_ombres(bool d)
        {
            drapeau_projete_ombres=d;
            initialise_mode_rendu();
        }

        void Surface::determine_ombre_portees(bool d)
        {
            drapeau_projete_ombres=d;
            drapeau_recoit_ombres=d;
            initialise_mode_rendu();
        }

        //--------------------------------------------------------
        //                  Débugage:
        //--------------------------------------------------------

        void Surface::affiche_debug()
        {
            cout<<"            Surface: "<<nom<<endl;
            cout<<"                     double-face                 : "<<boolalpha<<double_face<<endl;
            cout<<"                     angle limite d'adoucissement: "<<angle_limite_adoucissement<<endl;
            cout<<"                     adoucissement               : "<<boolalpha<<drapeau_adoucissement<<endl;
            cout<<"                     diffusion            : "<<dec<<diffusion<<hex<<" couleur: #"<<(((uint32_t)(couleur_diffusion[0]*255)<<16)|((uint32_t)(couleur_diffusion[0]*255)<<8)|(uint32_t)(couleur_diffusion[0]*255))<<endl;
            cout<<"                     specularite          : "<<dec<<specularite<<hex<<" couleur: #"<<(((uint32_t)(couleur_speculaire[0]*255)<<16)|((uint32_t)(couleur_speculaire[0]*255)<<8)|(uint32_t)(couleur_speculaire[0]*255))<<endl;
            cout<<"                     ambiance             : "<<dec<<ambiance<<hex<<" couleur: #"<<(((uint32_t)(couleur_ambiance[0]*255)<<16)|((uint32_t)(couleur_ambiance[0]*255)<<8)|(uint32_t)(couleur_ambiance[0]*255))<<endl;
            cout<<"                     luminescence         : "<<dec<<luminescence<<hex<<" couleur: #"<<(((uint32_t)(couleur_luminescence[0]*255)<<16)|((uint32_t)(couleur_luminescence[0]*255)<<8)|(uint32_t)(couleur_luminescence[0]*255))<<endl;
            cout<<"                     brillance            : "<<dec<<brillance<<endl;
            cout<<"                     transparence         : "<<dec<<transparence<<endl;
            cout<<"                     translucidite        : "<<dec<<translucidite<<endl;
            cout<<"                     reflexion            : "<<dec<<reflexion<<endl;
            cout<<"                     rugosite             : "<<dec<<rugosite<<endl;

        }

