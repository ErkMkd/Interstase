uniform vec3 point; //Position du point de référence

varying vec2 coordonnees_texture_couleur;
varying vec3 vecteur_point;

void main( void )
{
    gl_Position =  gl_ModelViewProjectionMatrix * gl_Vertex;
    coordonnees_texture_couleur     = gl_MultiTexCoord0.xy;

    //vec3 position_sommet = (gl_ModelViewMatrix * gl_Vertex).xyz;

    vecteur_point = point-gl_Vertex.xyz;
}
