#version 450 core
#extension GL_ARB_separate_shader_objects : enable

struct InstanceData
{
  mat4 model_mat ;
  vec2 bl        ;
  vec2 tr        ;
  vec2 tl        ;
  vec2 br        ;
};

layout ( location = 0 ) in  vec2 vertex     ;
layout ( location = 1 ) out vec2 tex_coords ;

layout( binding = 1 ) uniform projection
{
  mat4 proj ;
};

layout( binding = 2 ) uniform offsets
{
  InstanceData instances[1024] ;
};

void main()
{
  uint idx = gl_InstanceIndex ;

  if( gl_VertexIndex == 0 || gl_VertexIndex == 3 ) 
  {
    tex_coords = instances[ idx ].bl ;
  }

  if( gl_VertexIndex == 1 || gl_VertexIndex == 5 ) 
  {
    tex_coords = instances[ idx ].tr ;
  }

  if( gl_VertexIndex == 2 ) 
  {
    tex_coords = instances[ idx ].tl ;
  }

  if( gl_VertexIndex == 4 ) 
  {
    tex_coords = instances[ idx ].br ;
  }

  gl_Position = proj * instances[ idx ].model_mat * vec4( vertex.xy, 0.0, 1.0 ) ;
}
