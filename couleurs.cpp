#include "couleurs.h"

using namespace std;

        void Couleurs::couleur_degrade_rvba(uint8_t indice_depart, float* couleur_depart, uint8_t indice_fin, float* couleur_fin,float* tableau_couleurs)
        {
            if (indice_depart>=indice_fin) return;

            float nbr_etapes=(float)(indice_fin-indice_depart);

            float rp=(couleur_fin[0]-couleur_depart[0])/nbr_etapes;
            float vp=(couleur_fin[1]-couleur_depart[1])/nbr_etapes;
            float bp=(couleur_fin[2]-couleur_depart[2])/nbr_etapes;
            float ap=(couleur_fin[3]-couleur_depart[3])/nbr_etapes;

            float r=couleur_depart[0];
            float v=couleur_depart[1];
            float b=couleur_depart[2];
            float a=couleur_depart[3];

            for (uint16_t i=indice_depart;i<indice_fin;i++)
            {
                tableau_couleurs[4*i]=r;
                tableau_couleurs[4*i+1]=v;
                tableau_couleurs[4*i+2]=b;
                tableau_couleurs[4*i+3]=a;
                r+=rp;v+=vp;b+=bp;a+=ap;
            }

            tableau_couleurs[4*indice_fin]=couleur_fin[0];
            tableau_couleurs[4*indice_fin+1]=couleur_fin[1];
            tableau_couleurs[4*indice_fin+2]=couleur_fin[2];
            tableau_couleurs[4*indice_fin+3]=couleur_fin[3];
        }

        void Couleurs::couleur_degrade_rvb(uint8_t indice_depart, float* couleur_depart, uint8_t indice_fin, float* couleur_fin,float* tableau_couleurs)
        {
            if (indice_depart>=indice_fin) return;

            float nbr_etapes=(float)(indice_fin-indice_depart);

            float rp=(couleur_fin[0]-couleur_depart[0])/nbr_etapes;
            float vp=(couleur_fin[1]-couleur_depart[1])/nbr_etapes;
            float bp=(couleur_fin[2]-couleur_depart[2])/nbr_etapes;

            float r=couleur_depart[0];
            float v=couleur_depart[1];
            float b=couleur_depart[2];

            for (uint16_t i=indice_depart;i<indice_fin;i++)
            {
                tableau_couleurs[3*i]=r;
                tableau_couleurs[3*i+1]=v;
                tableau_couleurs[3*i+2]=b;
                r+=rp;v+=vp;b+=bp;
            }

            tableau_couleurs[3*indice_fin]=couleur_fin[0];
            tableau_couleurs[3*indice_fin+1]=couleur_fin[1];
            tableau_couleurs[3*indice_fin+2]=couleur_fin[2];
        }

        void Couleurs::couleur_degrade_rvba_i(uint32_t indice_depart, uint32_t couleur_depart, uint32_t indice_fin, uint32_t couleur_fin,uint32_t* tableau_couleurs )
        {
            if (indice_depart>indice_fin)
            {
                uint32_t temp=indice_fin;
                indice_fin=indice_depart;
                indice_depart=temp;
                temp=couleur_depart;
                couleur_depart=couleur_fin;
                couleur_fin=temp;
            }

            uint32_t nbr_etapes=indice_fin-indice_depart;

            float a0=couleur_depart&0xff;
            float b0=(couleur_depart>>8)&0xff;
            float v0=(couleur_depart>>16)&0xff;
            float r0=(couleur_depart>>24)&0xff;

            float a1=couleur_fin&0xff;
            float b1=(couleur_fin>>8)&0xff;
            float v1=(couleur_fin>>16)&0xff;
            float r1=(couleur_fin>>24)&0xff;

            float rp=(r1-r0)/(float)nbr_etapes;
            float vp=(v1-v0)/(float)nbr_etapes;
            float bp=(b1-b0)/(float)nbr_etapes;
            float ap=(a1-a0)/(float)nbr_etapes;

            for (uint32_t i=indice_depart;i<indice_fin;i++)
            {
                // les octets sont inversés ! (little endian / big endian )
                tableau_couleurs[i]=uint32_t(r0)|(uint32_t(v0)<<8)|(uint32_t(b0)<<16)|(uint32_t(a0)<<24);
                r0+=rp;v0+=vp;b0+=bp;a0+=ap;
            }

            tableau_couleurs[indice_fin]=couleur_fin;

        }

        void Couleurs::couleur_gradients_rvba_i(uint32_t numCouleurs,uint16_t numEtapes,uint32_t* gradients, uint32_t* tableau_couleurs)
        {
            float pas=(float)(numCouleurs)/(float)(numEtapes-1);
            float pos=0.;
            for(uint16_t i=0;i<numEtapes-1;i++)
            {
                couleur_degrade_rvba_i((uint32_t)pos,gradients[i],uint32_t(pos+pas),gradients[i+1],tableau_couleurs);
                //cout<<dec<<i<<" / "<<(uint32_t)pos<<" / "<<uint32_t(pos+pas)<<hex<<" / "<<gradients[i]<<" / "<<gradients[i+1]<<endl;
                pos+=pas;
            }
        }

