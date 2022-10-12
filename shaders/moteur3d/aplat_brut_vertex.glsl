//======================================================================
//				Rendu en aplat sans éclairage
//======================================================================

void main( void )
{
	//Données de l'observateur:
    gl_Position =  gl_ModelViewProjectionMatrix * gl_Vertex;		
}