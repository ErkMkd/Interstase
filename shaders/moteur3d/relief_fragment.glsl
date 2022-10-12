//-------------------------------------------------------------------------
//			Caclul de la rugosité, avec texture de couleur.
//			La couche Alpha de la texture de couleur sert pour la transparence en bascule
//			Alpha>0.5, le pixel est affiché, sinon il est absent.
//-------------------------------------------------------------------------

uniform sampler2D texture_couleur;
uniform sampler2D texture_normales;
uniform sampler2D texture_profondeurs;
uniform float echelle_texture;
uniform int relief_on;

varying vec2 coordonnees_textures;

varying vec3 direction_oeil_sommet;
varying vec3 direction_source_sommet;
//varying float distance_source;

#define M_PIS2 1.5708

void main( void )
{
    vec3 direction_oeil_unitaire=normalize(direction_oeil_sommet);

    vec3 coordRelief=vec3(coordonnees_textures*echelle_texture,0.);
    vec3 Vdecal=coordRelief;

	//Recherche du bon texel:
	float profondeur=-texture2D( texture_profondeurs, coordRelief.xy/echelle_texture ).x;
    float beta=acos(-direction_oeil_unitaire.z);
    float t=1.;
    float angleAlpha;
    int iter=40;    //Nombre maximum d'itération, au cas où.

    if(relief_on!=0)
    {
        //while(profondeur<0. && t>1e-8 && iter-- >0)
        while(profondeur<0. && t>1e-8 && iter-- >0)
        {
            angleAlpha=texture2D( texture_profondeurs, coordRelief.xy/echelle_texture ).y*M_PIS2;
            t=-profondeur*sin(angleAlpha)/sin(angleAlpha+beta);
            coordRelief+=direction_oeil_unitaire*t;
            profondeur=-texture2D( texture_profondeurs, coordRelief.xy/echelle_texture ).x-coordRelief.z;
        }
    }


    Vdecal=coordRelief-Vdecal;  //Position du pixel affiché par rapport au pixel de départ.
    vec3 direction_source_unitaire=normalize(direction_source_sommet+Vdecal);


	vec4 texel=texture2D( texture_couleur, coordRelief.xy/echelle_texture );

	//if (texel.a<0.5) discard;	//Transparence

	vec3 normale_pixel=normalize(texture2D(texture_normales,coordRelief.xy/echelle_texture).xyz*2.-1.);


	float facteur_diffuse = max(0.,dot(normale_pixel,direction_source_unitaire));
	float angle_source_normale=max(0.,dot(direction_source_unitaire,normale_pixel));
	vec3 reflet_source =  direction_source_unitaire-angle_source_normale*normale_pixel*2.;
	float facteur_speculaire=pow(max(0.,dot(reflet_source,direction_oeil_unitaire)),gl_FrontMaterial.shininess);

	vec4 couleur_diffuse = gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse;
	vec4 couleur_speculaire = gl_LightSource[0].specular * gl_FrontMaterial.specular;

	float attenuation=1. ;/*
                            / (gl_LightSource[0].constantAttenuation
							+ gl_LightSource[0].linearAttenuation * distance_source
							+  distance_source * distance_source * gl_LightSource[0].quadraticAttenuation);
                        */
	float alpha=couleur_diffuse.a + facteur_speculaire*couleur_speculaire.a;


	gl_FragColor =	clamp(vec4(
							attenuation*
							(facteur_diffuse*couleur_diffuse*texel.rgb
							+facteur_speculaire*couleur_speculaire.rgb)
							,alpha),0.,1.);

}
