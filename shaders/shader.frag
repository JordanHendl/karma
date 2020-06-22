#version 450 core
#extension GL_ARB_separate_shader_objects : enable

layout( location = 0 ) in  vec2 TexCoords ;
layout( location = 0 ) out vec4 outColor  ;

layout( binding = 0 ) uniform sampler2D image ;

layout ( binding = 1, std140 ) uniform SpriteConstants
{
  mat4 projection ; ///< Projection Matrix.
} constants ;

layout ( binding = 2, std140 ) uniform SpriteInfo
{
  vec4 sprite_color ; ///< Color to use for the sprite.
  mat4 model        ; ///< Model Matrix.
} info ;

void main()
{
  vec4 color = texture( image, TexCoords ) ;
  if( color.a < 0.05 )
  {
    discard ;
  }

  outColor = mix( color, info.sprite_color, 0.5 ) ;
}