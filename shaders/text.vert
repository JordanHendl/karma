#version 450 core
#extension GL_ARB_separate_shader_objects : enable

layout ( location = 0 ) in  vec2  vertex ;
layout ( location = 1 ) in  vec2  tex    ;
layout ( location = 2 ) in  mat4  model  ;

layout ( location = 3 ) out vec2 tex_coords ;

layout( binding = 1 ) uniform projection
{
  mat4 proj ;
};

layout( binding = 2 ) uniform camera 
{
  mat4 view ;
};

void main()  
{
  tex_coords  = tex                                               ;
  gl_Position = proj * view * model * vec4( vertex.xy, 0.0, 1.0 ) ;
}
