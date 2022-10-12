

varying vec4 couleur;
varying vec3 couleur_speculaire;

void main( void )
{
    gl_FragColor =  clamp(vec4(couleur_speculaire,0.) + couleur,0.,1.);
}