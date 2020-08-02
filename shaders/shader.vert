#version 450 core
#extension GL_ARB_separate_shader_objects : enable

layout ( location = 0 ) in  vec4 vertex     ; ///< Input in the following format: vec2 position, vec2 texCoords>
layout ( location = 1 ) out vec2 tex_coords ; ///< Texture coordinate output of this stage.

layout ( binding = 1, std140 ) uniform model
{
  vec2 position ; ///< Position in screen space. 0-0 is bottom left.
  vec2 rotation ; ///< Euler-Rotation of the image.
};

void main()
{
  tex_coords   = vertex.zw ;
  
  // TODO: apply rotation.

  gl_Position = vec4( vertex.xy + position, 0.0, 1.0 ) ;
}
