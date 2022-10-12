//varying vec2 coordonnees_texture;

void main( void )
{
    //coordonnees_texture = gl_MultiTexCoord0.xy;
    gl_Position =  gl_ModelViewProjectionMatrix * gl_Vertex;
}
