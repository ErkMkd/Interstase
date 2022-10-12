//======================================================================
//				Eclairage phong - Vertex Shader
//======================================================================

uniform int drapeaux_sources;	//Les bits d'activation des sources lumineuses

varying vec3 direction_sommet_oeil;
varying vec3 directions_sources[8];
varying float distances_sources[8];
varying vec3 normale_face;

void main( void )
{
	//Données de l'observateur:
    gl_Position 			=  gl_ModelViewProjectionMatrix * gl_Vertex;
	vec3 position_sommet 	= (gl_ModelViewMatrix * gl_Vertex).xyz;
    normale_face 			= gl_NormalMatrix * gl_Normal;
	direction_sommet_oeil 	= -position_sommet;

	// Calcul des sources:
    vec3 position_source ;
	for(int n=0;n<gl_MaxLights;n++)
	{
		if ( !(drapeaux_sources & (1<<n)) ) continue;
		if(gl_LightSource[n].position.w>0.)
        {
            position_source 		= (gl_LightSource[n].position).xyz;
            directions_sources[n] 		= position_source-position_sommet;
            distances_sources[n] 		= length(directions_sources[n]);
        }
        else
        {
           directions_sources[n] = normalize(gl_LightSource[n].position.xyz);
           distances_sources[n] 		= 1.;
        }

	}
}
