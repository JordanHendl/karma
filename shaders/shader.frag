#version 450 core
#extension GL_ARB_separate_shader_objects : enable

layout( location = 1 ) in  vec2 TexCoords ;
layout( location = 0 ) out vec4 outColor  ;

layout( binding = 0 ) uniform sampler2D image ;


layout ( binding = 2, std140 ) uniform colors
{
  float red ; ///< Projection Matrix.
};

void main()
{
  vec4 color = texture( image, TexCoords ) ;
  if( color.a < 0.05 )
  {
    discard ;
  }

  //outColor = mix( color, info.sprite_color, 0.5 ) ;

    outColor = vec4( red, color.xyz ) ;
}
