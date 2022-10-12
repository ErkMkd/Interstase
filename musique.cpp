
#include "Musique.h"


using namespace std;

    HSTREAM Musique::musique_id;
    float Musique::tempon_FFT[2048];

        bool Musique::init_musique(const char *fichier_musique)
        {
            //Initialise la librairie:
                BASS_Init(
                        -1,     //L'audio-device utilis�e. -1 = device par d�faut du syt�me
                        44100,  //Fr�quence d'�chantillonnage
                        BASS_DEVICE_LATENCY, //Active la latence du device (dur�e entre la demande d'un son et le moment o� il va commencer � �tre jou�.)
                        0, //Fen�tre actuellement au premier plan. Il vaut mieux avoir ouvert l'�cran avant de cr�er un objet Musique.
                        NULL //Classe de l'objet � cr�er ?
                        );

                int e=BASS_ErrorGetCode();

                if(e!=BASS_OK)
                {
                    cout<<"ERREUR lors de l'initialisation de la librairie BASS !!! Code: "<<e<<endl;
                    return false;
                }

            //Cr�ation du flux:
                musique_id = BASS_StreamCreateFile(
                                false,  //true si la musique est lue depuis la m�moire
                                fichier_musique,    //Le fichier
                                0,                //D�part de la musique
                                0,                //Longueur de la musique
                                BASS_SAMPLE_LOOP //Lib�ration de la m�moire � la fin de la musique
                                );

            int erreur=BASS_ErrorGetCode();
            if(erreur==BASS_OK) return true;
            return false;
        }

        void Musique::detruit_musique()
        {
                BASS_ChannelStop(musique_id);
                BASS_StreamFree(musique_id);
                BASS_Stop();
        }

        void Musique::musique_joue(bool repete)
        {
                BASS_ChannelPlay(musique_id,repete); //Le bool�en sert � pr�ciser si l'on rejoue depuis le d�but ou pas.
        }

        void Musique::musique_stop()
        {
                BASS_ChannelStop(musique_id);
        }

        void Musique::musique_determine_volume(float v) //Entre 0 et 1
        {
                BASS_ChannelSetAttribute(musique_id,BASS_ATTRIB_VOL,v);
        }

        void Musique::musique_renvoie_FFT()
        {
                BASS_ChannelGetData(musique_id,tempon_FFT,BASS_DATA_FFT512);
        }


