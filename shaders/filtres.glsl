uniform sampler2D texture;

#define FACTEUR_GRIS_R 0.11
#define FACTEUR_GRIS_V 0.59
#define FACTEUR_GRIS_B 0.3

uniform float saturation;
uniform float contraste;
uniform float seuil; //Limite 0 pour le contrast (les valeurs inférieures sont assombries, les supérieures éclaircies);

void main(void)
{
	vec4 c;
	vec4 p=texture2D(texture,gl_TexCoord[0].xy);
	float c0=p.r*FACTEUR_GRIS_R+p.g*FACTEUR_GRIS_V+p.b*FACTEUR_GRIS_B;

	//Saturation:
	c.r=saturation*(p.r-c0)+c0;
	c.g=saturation*(p.g-c0)+c0;
	c.b=saturation*(p.b-c0)+c0;

	//Contraste:
	c.r=clamp(c.r+contraste*(c0-seuil),0.,1.);
	c.g=clamp(c.g+contraste*(c0-seuil),0.,1.);
	c.b=clamp(c.b+contraste*(c0-seuil),0.,1.);


	//c.a=clamp(c.b+contraste*(c0-seuil),0.,1.);	//Couche alpha en fonction de la luminosité du pixel
	c.a=1.;

	gl_FragColor = c;
}
