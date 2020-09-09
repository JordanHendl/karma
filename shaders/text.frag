#version 450 core
#extension GL_ARB_separate_shader_objects : enable

layout( location = 0 ) out vec4 out_color  ; ///< The output color of this render pass.
layout( location = 3 ) in  vec2 tex_coords ; ///< The input texture coordinates to use for sampling.
layout( location = 4 ) in  vec4 in_color   ; ///< The input texture coordinates to use for sampling.

layout( binding = 0 ) uniform sampler2D image ; 



void main()
{
  vec4 text_color ;

  text_color = texture( image, tex_coords ) ;

  if( text_color.r < 0.1 ) discard ;
  
  out_color = in_color ;
}
