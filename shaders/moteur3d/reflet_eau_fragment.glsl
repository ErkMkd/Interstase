//=================================================================================
//		Remplissage avec la texture de reflet de la scène (générée au préalable)
//=================================================================================

uniform vec3 position_obs;
uniform vec3 position_source;
uniform sampler2D texture_reflet;
uniform float largeur_ecran;
uniform float hauteur_ecran;
uniform float t;
uniform vec4 couleur_fond;
//uniform vec4 couleur_speculaire;

varying vec3 position_sommet;
varying vec3 position_sommet_modelview;
//varying vec3 position_sommet_obs;

//Texture de bruit et contrôles des ondes:
varying vec2 texture_bruit_coord;
uniform sampler2D texture_bruit;
uniform float quantite_ondes;
uniform float frequence_ondes;
uniform float amplitude_ondes;
uniform float niveau_perturbations_ondes;
uniform float intensite_ondes;
uniform float vitesse_ondes;
uniform float transparence; //Transparence de l'eau
uniform float attenuation_speculaire; //Atténuation de la couleur spéculaire avec la distance


float distMax=100.;

void main( void )
{
    vec3 vec_obs=position_sommet-position_obs;
    vec3 vec_obs_unitaire=normalize(vec_obs);
    float dist=length(vec_obs);
    //----------------------------Calcul de la normale:

        float pas=vitesse_ondes*quantite_ondes*t;

        float perturbation=niveau_perturbations_ondes*(texture2D(texture_bruit,texture_bruit_coord)*2.-1.);
        //float perturbation=texture2D(texture_bruit,texture_bruit_coord);


        vec3 normale=vec3(0.,1.,0.);
        vec3 tangente_sommet=vec3(1.,0.,0.);
        vec3 binormale_sommet=vec3(0.,0.,1.);

        normale+=normalize(tangente_sommet)
                    *cos(
                            position_sommet.x*quantite_ondes+perturbation
                            +sin(position_sommet.z*frequence_ondes)*(amplitude_ondes*quantite_ondes)
                            +pas
                        )*intensite_ondes;


        normale+=normalize(binormale_sommet)
                    *sin(
                            position_sommet.z*quantite_ondes+perturbation
                            +cos(position_sommet.x*frequence_ondes/10.123)*(amplitude_ondes*quantite_ondes)
                            +pas
                        )*intensite_ondes*perturbation*0.9;

        normale=normalize(normale);



    //-------------------------------------------------




    //Distance à la surface:

    vec2 coordsTexReflet;
    float fogFacteur=clamp((dist-gl_Fog.start)/(gl_Fog.end-gl_Fog.start),0.,1.)*gl_Fog.density;
    //vec4 couleuFond=mix(couleur_fond,gl_Fog.color,fogFacteur);
    vec4 couleuFond=couleur_fond;

    int fx=int(gl_FragCoord.x);
    int fy=int(gl_FragCoord.y);

    if(int(fy)&1==0 && fx&1==0)
    {

            coordsTexReflet=vec2(1.-(gl_FragCoord.x+3)/largeur_ecran+normale.x*0.25/dist,(gl_FragCoord.y+3)/hauteur_ecran+normale.z*0.25/dist);

    }
    else if(fy&1==1 && fx&1==1)
    {

            coordsTexReflet=vec2(1.-(gl_FragCoord.x-3)/largeur_ecran+normale.x*0.25/dist,(gl_FragCoord.y-3)/hauteur_ecran+normale.z*0.25/dist);

    }

    else
    {
       coordsTexReflet=vec2(1.-(gl_FragCoord.x)/largeur_ecran+normale.x*0.25/dist,(gl_FragCoord.y)/hauteur_ecran+normale.z*0.25/dist);
    }

    /*
    float blurFacteur=.5+2.*(1.-clamp((dist-3.)/(25-3.),0.,1.));

    //coordsTexReflet=vec2(1.-(gl_FragCoord.x)/largeur_ecran+normale.x*0.25/dist,(gl_FragCoord.y)/hauteur_ecran+normale.z*0.25/dist);
    vec2 pixSize=vec2(1./largeur_ecran,1./hauteur_ecran);
    vec4 couleur=texture2D( texture_reflet, coordsTexReflet+blurFacteur*vec2(0.,-pixSize.y));
    couleur+=texture2D( texture_reflet, coordsTexReflet+blurFacteur*vec2(pixSize.x,0.));
    couleur+=texture2D( texture_reflet, coordsTexReflet+blurFacteur*vec2(0.,pixSize.y));
    couleur+=texture2D( texture_reflet, coordsTexReflet+blurFacteur*vec2(-pixSize.x,0.));
    couleur+=texture2D( texture_reflet, coordsTexReflet-blurFacteur*pixSize);
    couleur+=texture2D( texture_reflet, coordsTexReflet+blurFacteur*pixSize);
    couleur+=texture2D( texture_reflet, coordsTexReflet+blurFacteur*vec2(-pixSize.x,pixSize.y));
    couleur+=texture2D( texture_reflet, coordsTexReflet+blurFacteur*vec2(pixSize.x,-pixSize.y));
    couleur+=texture2D( texture_reflet, coordsTexReflet+blurFacteur*vec2(0.,-2*pixSize.y));
    couleur+=texture2D( texture_reflet, coordsTexReflet+blurFacteur*vec2(2*pixSize.x,0.));
    couleur+=texture2D( texture_reflet, coordsTexReflet+blurFacteur*vec2(0.,2*pixSize.y));
    couleur+=texture2D( texture_reflet, coordsTexReflet+blurFacteur*vec2(-2*pixSize.x,0.));
    couleur/=12.;*/
    //dist/=distMax;

    //Angle par rapport à la surface
    float angleVagues=max(dot(vec_obs_unitaire,-normale),0.);
    float angleVert=max(dot(-vec_obs_unitaire,vec3(0.,1.,0.)),0.);
    angleVagues=mix(angleVagues,1.,angleVert)*transparence;

    vec3 reflet_source	= -normalize(vec_obs_unitaire-normalize(position_source));

    vec4 couleur_speculaire 	= clamp((1./*-fogFacteur*/)/dist*attenuation_speculaire,0.,1.)
                                *(
                                    gl_LightSource[0].specular * gl_FrontMaterial.specular
                                    * pow(max(dot(reflet_source,normale),0.),gl_FrontMaterial.shininess)
                                  );




    //pow(max(dot(-vec_obs_unitaire,normale),0.),2.);
    //angleVagues=0.;

    //gl_FragColor =  texture2D( texture_reflet, coordsTexReflet );
    //gl_FragColor =  clamp((1.-dist+angleVagues)/2.,0.,1.)*couleuFond+clamp((dist+(1-angleVagues))/2,0.,1.)*texture2D( texture_reflet, coordsTexReflet );
    //gl_FragColor =  max(1.-dist,0.)*couleuFond+min(dist,1.)*texture2D( texture_reflet, coordsTexReflet );
    //gl_FragColor =  max(angleVagues,0.)*couleuFond+min(1.-angleVagues,1.)*texture2D( texture_reflet, coordsTexReflet );


    //Pour le floutage:
    //gl_FragColor = mix(couleur,couleuFond,angleVagues);
    //gl_FragColor =clamp(mix(couleur,couleuFond,angleVagues)+couleurSpeculaire,0.,1.);
    //Sans floutage:
    gl_FragColor =mix(
                        clamp(mix(texture2D( texture_reflet, coordsTexReflet),couleuFond,angleVagues)+couleur_speculaire,0.,1.),
                        gl_Fog.color,
                        fogFacteur
                    );
    //gl_FragColor =clamp(texture2D( texture_reflet, coordsTexReflet)+couleuFond*angleVagues+couleurSpeculaire,0.,1.);

}
