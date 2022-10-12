//-------------------------------------------------------------------------
//			Caclul de la rugosité, avec texture de couleur.
//			La couche Alpha de la texture de couleur sert pour la transparence en bascule
//			Alpha>0.5, le pixel est affiché, sinon il est absent.
//-------------------------------------------------------------------------

uniform sampler2D texture_couleur;
uniform sampler2D texture_rugosite;

varying vec2 coordonnees_texture_couleur;
varying vec2 coordonnees_texture_rugosite;

varying vec3 direction_oeil_sommet;
varying vec3 direction_source_sommet;
varying float distance_source;

void main( void )
{
	vec4 texel=texture2D( texture_couleur, coordonnees_texture_couleur );
	if (texel.a<0.5) discard;	//Transparence
	
	vec3 normale_pixel=normalize(texture2D(texture_rugosite,coordonnees_texture_rugosite).xyz*2.-1.);
	vec3 direction_source_unitaire=normalize(direction_source_sommet);
	vec3 direction_oeil_unitaire=normalize(direction_oeil_sommet);
	float facteur_diffuse = max(0.,dot(normale_pixel,direction_source_unitaire));	
	
	float angle_source_normale=max(0.,dot(direction_source_unitaire,normale_pixel));
	vec3 reflet_source =  direction_source_unitaire-angle_source_normale*normale_pixel*2.;
	float facteur_speculaire=pow(max(0.,dot(reflet_source,direction_oeil_unitaire)),gl_FrontMaterial.shininess);
	
	vec4 couleur_diffuse = gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse;
	vec4 couleur_speculaire = gl_LightSource[0].specular * gl_FrontMaterial.specular;
	
	float attenuation=1./ (gl_LightSource[0].constantAttenuation 
							+ gl_LightSource[0].linearAttenuation * distance_source
							+  distance_source * distance_source * gl_LightSource[0].quadraticAttenuation);
	
	float alpha=couleur_diffuse.a + facteur_speculaire*couleur_speculaire.a;
	
	gl_FragColor =	clamp(vec4(
							attenuation*
							(facteur_diffuse*couleur_diffuse*texel.rgb
							+facteur_speculaire*couleur_speculaire.rgb)
							,alpha),0.,1.);
						
}