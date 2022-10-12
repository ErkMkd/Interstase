uniform sampler2D texture;

#define M_PI 3.14159

uniform float H;
uniform float S;
uniform float V; //Limite 0 pour le contrast (les valeurs inférieures sont assombries, les supérieures éclaircies);

void main(void)
{
	vec4 c;
	vec4 p=texture2D(texture,gl_TexCoord[0].xy);

	float VSU = V*S*cos(H*M_PI/180.);
    float VSW = V*S*sin(H*M_PI/180.);

    c.r = (.299*V+.701*VSU+.168*VSW)*p.r
        + (.587*V-.587*VSU+.330*VSW)*p.g
        + (.114*V-.114*VSU-.497*VSW)*p.b;
    c.g = (.299*V-.299*VSU-.328*VSW)*p.r
        + (.587*V+.413*VSU+.035*VSW)*p.g
        + (.114*V-.114*VSU+.292*VSW)*p.b;
    c.b = (.299*V-.3*VSU+1.25*VSW)*p.r
        + (.587*V-.588*VSU-1.05*VSW)*p.g
        + (.114*V+.886*VSU-.203*VSW)*p.b;
    c.a=p.a;

	gl_FragColor = c;
}
