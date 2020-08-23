#version 450 core
#extension GL_ARB_separate_shader_objects : enable

layout ( location = 0 ) in vec2 vertex      ;
layout ( location = 1 ) out vec2 tex_coords ;

layout( binding = 1 ) uniform projection
{
  mat4 proj ;
};

layout( push_constant ) uniform matrices 
{
  mat4 model_mat ;
  vec2 bl        ;
  vec2 tr        ;
  vec2 tl        ;
  vec2 br        ;
};

void main()
{
  if( gl_VertexIndex == 0 || gl_VertexIndex == 3 ) 
  {
    tex_coords = bl ;
  }


  if( gl_VertexIndex == 1 || gl_VertexIndex == 5 ) 
  {
    tex_coords = tr ;
  }

  if( gl_VertexIndex == 2 ) 
  {
    tex_coords = tl ;
  }

  if( gl_VertexIndex == 4 ) 
  {
    tex_coords = br ;
  }

  gl_Position = proj * model_mat * vec4( vertex.xy, 0.0, 1.0 ) ;
}
