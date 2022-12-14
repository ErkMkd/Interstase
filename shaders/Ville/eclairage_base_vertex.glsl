//======================================================================
//  Eclairage de base de la ville
//======================================================================

//Varying:

varying float distance_pixel;
varying vec4 couleur;

//varying vec3 couleur_speculaire; //Sp?rar?e pour appara?tre par dessus les textures
#define NUM_LUMIERES 2
void main( void )
{
	//Donn?es de l'observateur:
    gl_Position =  gl_ModelViewProjectionMatrix * gl_Vertex;
	vec3 position_sommet = (gl_ModelViewMatrix * gl_Vertex).xyz;
    vec3 normale 				= normalize(gl_NormalMatrix * gl_Normal);

	vec3 direction_oeil 	= -normalize(position_sommet);

    distance_pixel  =   length(position_sommet);    //Sert pour le brouillard

	//vec3 position_source;
	vec3 direction_source;
	float distance_source;
	float attenuation;

	// Calcul des sources:

			//couleur_speculaire=vec3 (0.,0.,0.);
			vec3 couleur_rgb=vec3(0.,0.,0.);

			for(int n=0;n<NUM_LUMIERES;n++)
			{
                direction_source 		= normalize(gl_LightSource[n].position.xyz);
                attenuation		        =	1.;//1./gl_LightSource[n].constantAttenuation;

				float angle_source_normale  = max(dot(normale,direction_source),0.);
				vec3 reflet_source			= normalize(direction_oeil+direction_source);
                float angle_oeil_source     = max(dot(reflet_source,normale),0.);

				//Composantes:

				vec3 couleur_ambiante   = 	gl_FrontMaterial.ambient.rgb*gl_LightSource[n].ambient.rgb;

				vec3 couleur_diffuse 	= 	gl_LightSource[n].diffuse.rgb * angle_source_normale;
				//vec3 couleur_diffuse 	= 	gl_LightSource[n].diffuse.rgb * gl_FrontMaterial.diffuse.rgb * angle_source_normale;

				couleur_rgb				+=	attenuation * (couleur_ambiante + couleur_diffuse);

                /*couleur_speculaire 	+= attenuation* (
															gl_LightSource[n].specular.rgb * gl_FrontMaterial.specular.rgb
															* pow(angle_oeil_source,gl_FrontMaterial.shininess)
														);*/



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

		//----------- Couleur de base:
		vec3 couleur_ambiante_globale = gl_FrontMaterial.ambient.rgb*gl_LightModel.ambient.rgb;
		couleur=vec4(gl_FrontMaterial.emission.rgb + couleur_ambiante_globale + couleur_rgb , gl_FrontMaterial.diffuse.a);
		//couleur=vec4(1.,0.,0.,1.);
}
