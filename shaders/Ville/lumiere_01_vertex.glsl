//======================================================================
//				Rendu en aplat sans �clairage
//======================================================================

varying vec3 position_point;

void main( void )
{
	//Donn�es de l'observateur:
    gl_Position =  gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_FrontColor = gl_Color;
    position_point=gl_ModelViewMatrix * gl_Vertex;
}
