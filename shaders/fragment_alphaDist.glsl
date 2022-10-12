uniform sampler2D texture_couleur;

uniform float distMin;  //Distance de départ transparence (alpha=1.)
uniform float distMax;  //Distance max transparence (alpha=0.)

varying vec2 coordonnees_texture_couleur;
varying vec3 vecteur_point;


void main( void )
{
    float distance_point=length(vecteur_point);
    vec3 texel=texture2D( texture_couleur, coordonnees_texture_couleur ).rgb;
    float alpha=clamp(1.-(distance_point-distMin)/(distMax-distMin),0.,1.);
    //if(distance_point > distMin ) {texel=vec3(1.,1.,0.);alpha=1.;}
	gl_FragColor =vec4(texel,alpha);
}
