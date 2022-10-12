
//Uniform:

uniform float hauteur_etage; //Hauteur d'un étage
uniform int nombre_etages;
uniform float partSize; //Taille des éléments de trame


//Varying:

varying vec3 position_sommet;
varying vec4 couleur;
varying vec4 couleur_rgba;
varying vec3 coords_pixel;
varying vec3 normalAbs;
varying vec3 normale;

//varying vec3 couleur_speculaire;
#define NUM_LUMIERES 2
void main( void )
{
    //couleur_pixel=clamp(vec4(couleur_speculaire,0.) + couleur,0.,1.);
    vec4 couleur_texture;
    vec4 couleur_fenetres=couleur_rgba*0.2;
    int xPos,yPos,zPos;

    if(normalAbs.y!=0.)
    {
        //xPos=int(coords_pixel.x)
        couleur_texture=couleur_rgba*.6;
    }

    else
    {

        yPos=int(coords_pixel.y/(hauteur_etage)*3.);
        if(yPos==nombre_etages*3-1)
        {
            couleur_texture=couleur_rgba*2.;
        }

        else
        {


            yPos %= 3;


            if(yPos!=0)
            {
                //couleur_texture=couleur_fenetres;
                //couleur_texture=couleur_rgba;


                if(normalAbs.x!=0.)
                {
                    //int zPos=int(coords_pixel.z+20./1.)%3;
                    /*
                    zPos=int(coords_pixel.z)%20;
                    if(zPos!=0)discard;//couleur_texture=couleur_fenetres;
                    else */
                        couleur_texture=couleur_rgba;
                }

                else if(normalAbs.z!=0.)
                {
                    //int xPos=int(coords_pixel.x+20./1.)%3;
                    xPos=int((coords_pixel.x/partSize+0.5)*100.)%10;
                    if(xPos!=0)discard;//couleur_texture=couleur_fenetres;
                    else couleur_texture=couleur_rgba;
                }


                else couleur_texture=vec4(1.,1.,1.,1.);


            }
            else couleur_texture=couleur_rgba;
            //else couleur_texture=couleur_fenetres;
        }
    }

    // Calcul le reflet spéculaire::

    vec3 direction_oeil = -normalize(position_sommet);
    vec3 normalePixel=normalize(normale);

    vec4 couleur_speculaire=vec4 (0.,0.,0.,0.);

    for(int n=0;n<NUM_LUMIERES;n++)
    {
        vec3 direction_source 		= normalize(gl_LightSource[n].position.xyz);
        float attenuation		        =	1.;//1./gl_LightSource[n].constantAttenuation;

        float angle_source_normale  = max(dot(normalePixel,direction_source),0.);
        vec3 reflet_source			= normalize(direction_oeil+direction_source);
        float angle_oeil_source     = max(dot(reflet_source,normalePixel),0.);

        //Composantes:

        couleur_speculaire 	+= attenuation* (
                                                    gl_LightSource[n].specular * gl_FrontMaterial.specular
                                                    * pow(angle_oeil_source,gl_FrontMaterial.shininess)
                                                );
    }


    float distance_pixel  =   length(position_sommet);
    float facteurBrouillard=clamp((distance_pixel-gl_Fog.start)/(gl_Fog.end-gl_Fog.start),0.,1.);
    vec4 couleur_pixel=mix(clamp(couleur*couleur_texture+couleur_speculaire,0.,1.),gl_Fog.color,facteurBrouillard);
    gl_FragColor = couleur_pixel;
}
