varying vec2 coordonnees_texture_couleur;

void main( void )
{
    gl_Position =  gl_ModelViewProjectionMatrix * gl_Vertex;
    coordonnees_texture_couleur     = gl_MultiTexCoord0.xy;
}
