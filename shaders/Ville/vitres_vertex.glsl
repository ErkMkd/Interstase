//======================================================================
//  Vitres des immeubles
//======================================================================

//Uniform:


//Attributes:


//Varying:

varying vec3 position_sommet;
varying vec3 normale;

//varying vec3 couleur_speculaire; //Sp�rar�e pour appara�tre par dessus les textures

void main( void )
{
    gl_FrontColor=gl_Color;
	//Donn�es de l'observateur:
    gl_Position =  gl_ModelViewProjectionMatrix * gl_Vertex;
	position_sommet = (gl_ModelViewMatrix * gl_Vertex).xyz;
    normale = gl_NormalMatrix * gl_Normal;
}
