

//Varying:

varying float distance_pixel;
varying vec4 couleur;

//varying vec3 couleur_speculaire;



void main( void )
{
    //couleur_pixel=clamp(vec4(couleur_speculaire,0.) + couleur,0.,1.);

    float facteurBrouillard=clamp((distance_pixel-gl_Fog.start)/(gl_Fog.end-gl_Fog.start),0.,1.);
    vec4 couleur_pixel=mix(couleur*gl_FrontMaterial.diffuse,gl_Fog.color,facteurBrouillard);
    gl_FragColor = couleur_pixel;
}
