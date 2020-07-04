#version 450 core
#extension GL_ARB_separate_shader_objects : enable

layout ( location = 0 ) in  vec4 vertex; ///< <vec2 position, vec2 texCoords>
layout ( location = 1 ) out vec2 TexCoords;

layout ( binding = 1, std140 ) uniform offsets
{
  vec2 offset ; ///< Projection Matrix.
};

void main()
{
  TexCoords   = vertex.zw ;
  gl_Position = vec4( vertex.xy + offset, 0.0, 1.0 ) ;
}