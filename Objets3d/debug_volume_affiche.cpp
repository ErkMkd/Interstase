//------------------------------------------------------
//          Affichages pour le déboguage des rendus
//------------------------------------------------------


//Affiche les normales et les tangentes:


#if VOLUME_DEBUG_AFFICHE_NORMALES
               for(i=0;i<nbrSommets;i++)
                {
                    float n0x=sommets[3*i];
                    float n0y=sommets[3*i+1];
                    float n0z=sommets[3*i+2];
                    float n1x=n0x+(normales_sommets[3*i])*f;
                    float n1y=n0y+(normales_sommets[3*i+1]*f);
                    float n1z=n0z+(normales_sommets[3*i+2])*f;
                    AffichagesDebogue::affiche_segment(n0x,n0y,n0z,n1x,n1y,n1z,0xffff00);

                    n1x=n0x+(tangentes_sommets[3*i])*f;
                    n1y=n0y+(tangentes_sommets[3*i+1]*f);
                    n1z=n0z+(tangentes_sommets[3*i+2])*f;
                    AffichagesDebogue::affiche_segment(n0x,n0y,n0z,n1x,n1y,n1z,0xff0000);

                    n1x=n0x+(binormales_sommets[3*i])*f;
                    n1y=n0y+(binormales_sommets[3*i+1]*f);
                    n1z=n0z+(binormales_sommets[3*i+2])*f;
                    AffichagesDebogue::affiche_segment(n0x,n0y,n0z,n1x,n1y,n1z,0xff);
                }

#endif

#if VOLUME_DEBUG_AFFICHE_NORMALES_SOMMETS_FACES
               uint32_t nf=0;
               for(i=0;i<nbrFaces;i++)
                {
                    Face* face=faces[i];
                    if (!face->drapeau_partiellement_adoucie) continue;
                    if(nf==cptr_debug)
                    {
                        for (uint16_t j=0;j<face->nbrSommets;j++)
                        {
                            uint32_t indice=face->aretes[j];
                            float n0x=sommets[3*indice];
                            float n0y=sommets[3*indice+1];
                            float n0z=sommets[3*indice+2];
                            float n1x=n0x+(face->normales_sommets[3*j])*f;
                            float n1y=n0y+(face->normales_sommets[3*j+1])*f;
                            float n1z=n0z+(face->normales_sommets[3*j+2])*f;
                            AffichagesDebogue::affiche_segment(n0x,n0y,n0z,n1x,n1y,n1z,0xffff00);
                        }
                    }
                    nf++;
                }

#endif

#if VOLUME_DEBUG_AFFICHE_REPERES_TBN_FACES

               for(i=0;i<nbrFaces;i++)
                {
                    Face* face=faces[i];
                    float n0x=face->centre_gravite_relatif[0];
                    float n0y=face->centre_gravite_relatif[1];
                    float n0z=face->centre_gravite_relatif[2];
                    float n1x=n0x+face->normale[0]*f;
                    float n1y=n0y+face->normale[1]*f;
                    float n1z=n0z+face->normale[2]*f;
                    AffichagesDebogue::affiche_segment(n0x,n0y,n0z,n1x,n1y,n1z,0xff0000);

                    n1x=n0x+face->tangente[0]*f;
                    n1y=n0y+face->tangente[1]*f;
                    n1z=n0z+face->tangente[2]*f;
                    AffichagesDebogue::affiche_segment(n0x,n0y,n0z,n1x,n1y,n1z,0x00ff00);


                    n1x=n0x+face->binormale[0]*f;
                    n1y=n0y+face->binormale[1]*f;
                    n1z=n0z+face->binormale[2]*f;
                    AffichagesDebogue::affiche_segment(n0x,n0y,n0z,n1x,n1y,n1z,0xffff00);
                }
#endif

#if VOLUME_DEBUG_AFFICHE_REPERES_TBN_TEXTURES_RUGOSITE

               for(i=0;i<nbrSurfaces;i++)
                {
                    Surface* surface=surfaces[i];
                    for (j=0;j<surface->textures_rugosite.size();j++)
                    {
                        Texture* texture=surface->textures_rugosite[j];
                        for (uint32_t l=0;l<texture->nbrFaces;l++)
                        {
                            uint32_t indice=texture->faces[l];
                            Face* face=faces[indice];
                            float n0x=face->centre_gravite_relatif[0];
                            float n0y=face->centre_gravite_relatif[1];
                            float n0z=face->centre_gravite_relatif[2];
                            float n1x=n0x+face->normale[0]*f;
                            float n1y=n0y+face->normale[1]*f;
                            float n1z=n0z+face->normale[2]*f;
                            AffichagesDebogue::affiche_segment(n0x,n0y,n0z,n1x,n1y,n1z,0xff0000);

                            n1x=n0x+texture->tangentes_faces[3*indice]*f;
                            n1y=n0y+texture->tangentes_faces[3*indice+1]*f;
                            n1z=n0z+texture->tangentes_faces[3*indice+2]*f;
                            AffichagesDebogue::affiche_segment(n0x,n0y,n0z,n1x,n1y,n1z,0x00ff00);


                            n1x=n0x+texture->binormales_faces[3*indice]*f;
                            n1y=n0y+texture->binormales_faces[3*indice+1]*f;
                            n1z=n0z+texture->binormales_faces[3*indice+2]*f;
                            AffichagesDebogue::affiche_segment(n0x,n0y,n0z,n1x,n1y,n1z,0xffff00);


                        }

                    }
                }
#endif

#if VOLUME_DEBUG_AFFICHE_REPERES_TBN_SOMMETS

               for(i=0;i<nbrSurfaces;i++)
                {
                    Surface* surface=surfaces[i];
                    for (j=0;j<surface->textures_rugosite.size();j++)
                    {
                        Texture* texture=surface->textures_rugosite[j];
                        for (uint32_t l=0;l<texture->nbrSommets;l++)
                        {
                            uint32_t indice=texture->coordonnees_UV[l].indice_sommet;
                            float n0x=sommets[indice*3];
                            float n0y=sommets[indice*3+1];
                            float n0z=sommets[indice*3+2];
                            float n1x=n0x+normales_sommets[indice*3]*f;
                            float n1y=n0y+normales_sommets[indice*3+1]*f;
                            float n1z=n0z+normales_sommets[indice*3+2]*f;
                            AffichagesDebogue::affiche_segment(n0x,n0y,n0z,n1x,n1y,n1z,0xff0000);

                            n1x=n0x+texture->tangentes_sommets[3*indice]*f;
                            n1y=n0y+texture->tangentes_sommets[3*indice+1]*f;
                            n1z=n0z+texture->tangentes_sommets[3*indice+2]*f;
                            AffichagesDebogue::affiche_segment(n0x,n0y,n0z,n1x,n1y,n1z,0x00ff00);


                            n1x=n0x+texture->binormales_sommets[3*indice]*f;
                            n1y=n0y+texture->binormales_sommets[3*indice+1]*f;
                            n1z=n0z+texture->binormales_sommets[3*indice+2]*f;
                            AffichagesDebogue::affiche_segment(n0x,n0y,n0z,n1x,n1y,n1z,0xffff00);
                        }

                    }
                }

#endif
