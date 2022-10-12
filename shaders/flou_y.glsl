uniform int taille_flou;
uniform float saturation_flou;
uniform sampler2D texture;

void main(void)
{
   int dy;
   float taille_texture=textureSize(texture,0).y;
   float intensite;
   float amplitude=0.;
   vec4 p=vec4 ( 0.);
   for (dy=-taille_flou;dy<taille_flou;dy++)
   {
      intensite=1.-abs(float(dy)/float(taille_flou));
      p+= intensite *(texture2D(texture,vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+float(dy)/taille_texture)));
      amplitude+=intensite;
   }
   p*=saturation_flou/amplitude;
   //p.a=1.;

   gl_FragColor = p;
}
