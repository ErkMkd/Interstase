varying vec2 coordonnees_texture_arriere_plan;
void main( void )
{
    coordonnees_texture_arriere_plan     = gl_MultiTexCoord1.xy;
    gl_Position = ftransform();
}
