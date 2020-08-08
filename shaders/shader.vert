#version 450 core
#extension GL_ARB_separate_shader_objects : enable

layout ( location = 0 ) in  vec4 vertex     ; ///< Input in the following format: vec2 position, vec2 texCoords>
layout ( location = 1 ) out vec2 tex_coords ; ///< Texture coordinate output of this stage.

layout( push_constant ) uniform matrices 
{
  mat4 model_mat ;
  mat4 proj      ;
};

void main()
{
  tex_coords   = vertex.zw ;
  
  gl_Position = proj * model_mat * vec4( vertex.xy, 0.0, 1.0 ) ;
}
