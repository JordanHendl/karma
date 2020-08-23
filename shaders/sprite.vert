#version 450 core
#extension GL_ARB_separate_shader_objects : enable

layout ( location = 0 ) in  vec2  vertex    ;
layout ( location = 1 ) in  float tex_index ;

layout ( location = 2 ) out vec2 tex_coords ;

layout( binding = 1 ) uniform projection
{
  mat4 proj ;
};

layout( binding = 2 ) uniform camera 
{
  mat4 view ;
};

layout( push_constant ) uniform matrices 
{
  mat4 model_mat  ;
  vec2 offsets[4] ;
};

void main()  
{
  uint idx = uint( tex_index ) ;

  tex_coords  = offsets[ idx ]                                        ;
  gl_Position = proj * view * model_mat * vec4( vertex.xy, 0.0, 1.0 ) ;
}
