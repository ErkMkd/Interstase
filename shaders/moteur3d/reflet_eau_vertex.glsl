//===========================================================================================
//				Remplissage avec la texture de reflet de la sc�ne (g�n�r�e au pr�alable)
//===========================================================================================

varying vec3 position_sommet;
varying vec2 texture_bruit_coord;

uniform float vitesse_ondes;
uniform float echelle_texture_bruit_ondes;
uniform float t;

void main( void )
{
	//Donn�es de l'observateur:
    gl_Position =  gl_ModelViewProjectionMatrix * gl_Vertex;
    position_sommet=gl_Vertex.xyz;

    //Coordonn�es de la texture de bruit:
	float pas_texture=(vitesse_ondes*t);
	//texture_bruit_coord			=	((position_sommet).xz+vec2(-pas_texture,-pas_texture))/echelle_texture_bruit_ondes;
	texture_bruit_coord			=	((position_sommet).xz)/echelle_texture_bruit_ondes;

}
