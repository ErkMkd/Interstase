//-------------------------------
// Rendu des vitres
//-------------------------------
//Uniform:



//Varying:

varying vec3 position_sommet;
varying vec3 normale;
//varying vec3 couleur_speculaire;
#define NUM_LUMIERES 2

void main( void )
{
    vec3 direction_oeil = -normalize(position_sommet);
    vec3 normalePixel=normalize(normale);

	// Calcul des sources:

			vec4 couleur_speculaire=vec4 (0.,0.,0.,0.);
			vec4 couleur_rgba=vec4(0.,0.,0.,0.);

			for(int n=0;n<NUM_LUMIERES;n++)
			{
                vec3 direction_source 		= normalize(gl_LightSource[n].position.xyz);
                float attenuation		        =	1.;//1./gl_LightSource[n].constantAttenuation;

				float angle_source_normale  = max(dot(normalePixel,direction_source),0.);
				vec3 reflet_source			= normalize(direction_oeil+direction_source);
                float angle_oeil_source     = max(dot(reflet_source,normalePixel),0.);

				//Composantes:

				vec4 couleur_ambiante   = 	gl_FrontMaterial.ambient*gl_LightSource[n].ambient;

				//vec3 couleur_diffuse 	= 	gl_LightSource[n].diffuse.rgb * angle_source_normale;
				vec4 couleur_diffuse 	= 	gl_LightSource[n].diffuse * gl_Color * angle_source_normale;

				couleur_rgba				+=	attenuation * (couleur_ambiante + couleur_diffuse);

                couleur_speculaire 	+= attenuation* (
															gl_LightSource[n].specular * gl_FrontMaterial.specular
															* pow(angle_oeil_source,gl_FrontMaterial.shininess)
														);



			}
        //------------- Lumieres ponctuelles:

        /*
        position_source 		= (gl_LightSource[n].position).xyz;
                    direction_source 		= position_source-position_sommet;
                    distance_source 		= length(direction_source);

                    attenuation		        =	1./ (gl_LightSource[n].constantAttenuation
											+ gl_LightSource[n].linearAttenuation * distance_source
											+  distance_source * distance_source * gl_LightSource[n].quadraticAttenuation);
        */

    //----------- Couleur:
    vec4 couleur_ambiante_globale = gl_FrontMaterial.ambient*gl_LightModel.ambient;
    vec4 couleur=clamp(gl_FrontMaterial.emission + couleur_ambiante_globale + couleur_rgba + couleur_speculaire,0.,1.);


    float distance_pixel  =   length(position_sommet);
    float facteurBrouillard=clamp((distance_pixel-gl_Fog.start)/(gl_Fog.end-gl_Fog.start),0.,1.);
    vec4 couleur_pixel=mix(couleur,gl_Fog.color,facteurBrouillard);
    gl_FragColor = couleur_pixel;
}
