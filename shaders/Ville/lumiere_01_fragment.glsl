//======================================================================
//				Rendu en aplat sans éclairage
//======================================================================

varying vec3 position_point;

void main( void )
{
    float distance_pixel=length(position_point);
    float depart_fog=gl_Fog.end-(gl_Fog.end-gl_Fog.start)/100.;
    float facteurBrouillard=clamp((distance_pixel-depart_fog)/(gl_Fog.end-depart_fog),0.,1.);
    gl_FragColor=mix(gl_Color,gl_Fog.color,facteurBrouillard);

}
