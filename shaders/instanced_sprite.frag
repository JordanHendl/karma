#version 450 core
#extension GL_ARB_separate_shader_objects : enable

layout( location = 0 ) out vec4 out_color  ; ///< The output color of this render pass.
layout( location = 1 ) in  vec2 tex_coords ; ///< The input texture coordinates to use for sampling.

layout( binding = 0 ) uniform sampler2D image ; 

void main()
{
  vec4 color ;
  
  color = texture( image, tex_coords ) ;
  if( color.a < 0.1 ) discard ;

  out_color = vec4( color ) ;
}
