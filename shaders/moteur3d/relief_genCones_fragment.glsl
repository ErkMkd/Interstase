uniform sampler2D texture_profondeurs;
uniform int largeur_texture_sortie;
uniform int hauteur_texture_sortie;
uniform float echelle_profondeur;

//varying vec2 coordonnees_texture;
#define M_PIS2 1.5708
#define EPSILON_PROFONDEUR 1e-6

void main( void )
{
    float x0=gl_FragCoord.x/float(largeur_texture_sortie);
    float y0=gl_FragCoord.y/float(hauteur_texture_sortie);
    float profondeur0=vec4(texture2D( texture_profondeurs, vec2(x0,y0) )).r*echelle_profondeur;

    float angleMin=M_PIS2;
    float angle;
    vec3 vCone;
    float x1,y1,profondeur1;
    int px,py;

    for(py=0;py<hauteur_texture_sortie;py++)
    {
        for(px=0;px<largeur_texture_sortie;px++)
        {
            x1=float(px)/float(largeur_texture_sortie);
            y1=float(py)/float(hauteur_texture_sortie);
            profondeur1=texture2D( texture_profondeurs, vec2(x1,y1) ).r*echelle_profondeur;
            if(profondeur1-EPSILON_PROFONDEUR<profondeur0)
            {
                vCone=vec3(x1-x0,y1-y0,profondeur0-profondeur1);
                angle=acos(normalize(vCone).z);
                if(angle<angleMin)angleMin=angle;
            }
        }
    }


    //vec4 texel=texture2D( texture_profondeurs, coordonnees_texture );

	gl_FragColor = vec4(profondeur0,angleMin/M_PIS2,0.,1.);
}
