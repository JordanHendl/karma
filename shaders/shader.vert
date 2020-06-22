#version 450 core
#extension GL_ARB_separate_shader_objects : enable

layout( location = 0 )  in  vec4 vertex; ///< <vec2 position, vec2 texCoords>
layout( location = 0 ) out vec2 TexCoords;

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
  TexCoords   = vertex.zw ;
  gl_Position = constants.projection * info.model * vec4( vertex.xy, 0.0, 1.0 ) ;
}