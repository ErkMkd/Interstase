//======================================================================
//				Eclairage de base.
//		Je ne sais pas comment savoir si les lumières de bases d'OpenGL
//		sont activées ou pas. Il faut donc entrer en paramètre les drapeaux
//		d'activation des sources. bit n:true = source n:activée
//======================================================================


uniform int drapeaux_sources;	//Les bits d'activation des sources lumineuses

varying vec4 couleur;
varying vec3 couleur_speculaire; //Spérarée pour apparaître par dessus les textures

void main( void )
{
	//Données de l'observateur:
    gl_Position =  gl_ModelViewProjectionMatrix * gl_Vertex;
	vec3 position_sommet = (gl_ModelViewMatrix * gl_Vertex).xyz;
    vec3 normale 				= normalize(gl_NormalMatrix * gl_Normal);
	vec3 direction_oeil 	= -normalize(position_sommet);

	// Calcul des sources:

			couleur_speculaire=vec3 (0.,0.,0.);
			vec3 couleur_rgb=vec3(0.,0.,0.);

			for(int n=0;n<gl_MaxLights;n++)
			{
				if ( !(drapeaux_sources & (1<<n)) ) continue;

				vec3 position_source 		= (gl_LightSource[n].position).xyz;
				vec3 direction_source 		= position_source-position_sommet;
				float distance_source 		= length(direction_source);
				direction_source			= normalize(direction_source);
				float angle_source_normale  = max(dot(normale,direction_source),0.);
				vec3 reflet_source			= normalize(direction_oeil+direction_source);


				//Composantes:

				float attenuation		=	1./ (gl_LightSource[n].constantAttenuation
											+ gl_LightSource[n].linearAttenuation * distance_source
											+  distance_source * distance_source * gl_LightSource[n].quadraticAttenuation);

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

		//----------- Couleur de base:
		vec3 couleur_ambiante_globale = gl_FrontMaterial.ambient.rgb*gl_LightModel.ambient.rgb;
		couleur=vec4(gl_FrontMaterial.emission.rgb + couleur_ambiante_globale + couleur_rgb , gl_FrontMaterial.diffuse.a);
}
