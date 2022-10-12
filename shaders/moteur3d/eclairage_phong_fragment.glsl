/*

		Eclairage phong - Fragment shader

*/

uniform int drapeaux_sources;

varying vec3 direction_sommet_oeil;
varying vec3 directions_sources[8];
varying float distances_sources[8];
varying vec3 normale_face;

void main( void )
{
	vec3 normale=normalize(normale_face);
	vec3 direction_oeil_unitaire=normalize(direction_sommet_oeil);
	vec3 couleur_rgb=vec3(0.,0.,0.);
	vec3 couleur_speculaire=vec3 (0.,0.,0.);
    float attenuation;

	for (int n=0; n<gl_MaxLights;n++)
	{
		if ( !(drapeaux_sources & (1<<n)) ) continue;
		vec3 direction_source_unitaire	= normalize(directions_sources[n]);
		float angle_source_normale  = max(dot(normale,direction_source_unitaire),0.);
		vec3 reflet_source			= normalize(direction_oeil_unitaire+direction_source_unitaire);

		//Composantes:

		if(gl_LightSource[n].position.w>0.)
        {
           attenuation		=	1./ (gl_LightSource[n].constantAttenuation
									+ gl_LightSource[n].linearAttenuation * distances_sources[n]
									+  distances_sources[n] * distances_sources[n] * gl_LightSource[n].quadraticAttenuation);
        }

        else attenuation=1.;

		vec3 couleur_ambiante   = 	gl_FrontMaterial.ambient.rgb*gl_LightSource[n].ambient.rgb;

		vec3 couleur_diffuse 	= 	gl_LightSource[n].diffuse.rgb * gl_FrontMaterial.diffuse.rgb * angle_source_normale;

		couleur_rgb				+=	attenuation * (couleur_ambiante + couleur_diffuse);
		if (angle_source_normale>0.)
		{
			couleur_speculaire 	+= attenuation* (
													gl_LightSource[n].specular.rgb * gl_FrontMaterial.specular.rgb
													* pow(max(dot(reflet_source,normale),0.),gl_FrontMaterial.shininess)
												);
		}
	}
	vec3 couleur_ambiante_globale = gl_FrontMaterial.ambient.rgb*gl_LightModel.ambient.rgb;
	vec4 couleur=vec4(gl_FrontMaterial.emission.rgb + couleur_ambiante_globale + couleur_rgb , gl_FrontMaterial.diffuse.a);
    vec4 couleur_pixel=clamp(vec4(couleur_speculaire,0.) + couleur,0.,1.);

    //Brouillard:

    float distance_pixel=length(direction_sommet_oeil);
    float facteurBrouillard=clamp((distance_pixel-gl_Fog.start)/(gl_Fog.end-gl_Fog.start),0.,1.)*gl_Fog.density;

    //Pixel:

    gl_FragColor=mix(couleur,gl_Fog.color,facteurBrouillard);
}
