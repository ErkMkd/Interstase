uniform sampler2D texture_couleur;
varying vec2 coordonnees_texture_couleur;

void main( void )
{
	gl_FragColor =texture2D( texture_couleur, coordonnees_texture_couleur );
}
