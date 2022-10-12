
uniform float ecran_ratio;
uniform double zoom;	//Zoom
uniform double xpos;	//Position sur l'image
uniform double ypos;
uniform double t;		//Paramètre supplémentaire, pour modifier la forme.
uniform float alpha;	//Angle de rotation
uniform float decallage_palette;	//Angle de rotation
uniform  vec2 ecranDim;	//Dimensions de l'écran
uniform sampler1D palette;
uniform sampler2D arriere_plan;
uniform int iter;

#define NUM_COULEURS 512

varying vec2 coordonnees_texture_arriere_plan;

void main( void )
{
        double c=cos(alpha);
        double s=sin(alpha);

        double x=(gl_FragCoord.x-(ecranDim.x/2));
        double y=(gl_FragCoord.y-(ecranDim.y/2))*ecran_ratio;

        double xr=x*c-y*s;
        double yr=x*s+y*c;

        double Cr=xr*zoom+xpos;
        double Ci=yr*zoom+ypos;

        double Zr=0.;
        double Zi=0.;
        double tmp;

	vec3 couleur;

        double N=0;
        int i=0;

        while((Zr*Zr+Zi*Zi)<4. && i<iter)
        {
                tmp=Zr*Zr-Zi*Zi+Cr;
                Zi=2.*t*Zr*Zi+Ci;
                Zr=tmp;
                i++;
        }

        if(i==iter)
		{
			couleur=texture2D(arriere_plan,coordonnees_texture_arriere_plan);
		}
	else
		{
			couleur=texture1D(palette,float(i)/NUM_COULEURS+decallage_palette);
		}

        gl_FragColor = vec4(couleur,1.);

}
