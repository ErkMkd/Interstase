uniform int taille_flou;
uniform float saturation_flou;
uniform sampler2D texture;

void main(void)
{
   int dx;
   float taille_texture=textureSize(texture,0).x;
   float intensite;
   float amplitude=0.;
   vec4 p=vec4 ( 0.);
   for (dx=-taille_flou;dx<taille_flou;dx++)
   {
      intensite=1.-abs(float(dx)/float(taille_flou));
      p+= intensite *(texture2D(texture,vec2(gl_TexCoord[0].x+float(dx)/taille_texture,gl_TexCoord[0].y)));
      amplitude+=intensite;
   }
   p*=saturation_flou/amplitude;
   //p.a=1.;


   gl_FragColor = p;
}
