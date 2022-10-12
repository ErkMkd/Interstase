varying vec2 coordonnees_texture_rugosite;

varying vec3 direction_oeil_sommet;
varying vec3 direction_source_sommet;
varying float distance_source;
varying vec4 couleur;

attribute vec3 tangente;
attribute vec3 binormale;

void main( void )
{
    gl_Position =  gl_ModelViewProjectionMatrix * gl_Vertex;
	vec3 position_sommet = (gl_ModelViewMatrix * gl_Vertex).xyz;
    coordonnees_texture_rugosite    = gl_MultiTexCoord0.st;
   
   //--------- Calcul de l'éclairage:
   
    vec3 normale = 			gl_NormalMatrix * gl_Normal;
    vec3 tangente_sommet = 	gl_NormalMatrix * tangente;
    vec3 binormale_sommet = gl_NormalMatrix * binormale;
	
	vec3 position_source 		= (gl_LightSource[0].position).xyz;
	vec3 direction_source 		= position_source-position_sommet;
	distance_source 			= length(direction_source);
	direction_source			=normalize(direction_source);
	float angle_source_normale	=dot(direction_source,normale);
	
	vec3 direction_oeil = normalize(position_sommet);
			vec3 reflet_source 	= direction_source-angle_source_normale*normale*2.; //A SUPPRIMER
   

   //------------- Directions de la source et de l'oeil, dans le repère du sommet:(axeX=Tangente,axeY=Binormale, axeZ=Normale):
   
   direction_oeil_sommet.x=dot(direction_oeil,tangente_sommet);
   direction_oeil_sommet.y=dot(direction_oeil,binormale_sommet);
   direction_oeil_sommet.z=dot(direction_oeil,normale);
   
   direction_source_sommet.x=dot(direction_source,tangente_sommet);
   direction_source_sommet.y=dot(direction_source,binormale_sommet);
   direction_source_sommet.z=dot(direction_source,normale);
   
   couleur=gl_Color;
   
}