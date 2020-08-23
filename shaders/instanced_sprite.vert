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

layout( push_constant ) uniform matrices 
{
  mat4 view ;
};

layout( binding = 2 ) buffer offsets
{
  InstanceData instances[] ;
};


void main()
{
  int  idx          ;
  bool top_left     ;
  bool bottom_left  ;
  bool top_right    ;
  bool bottom_right ;
  

  bottom_left  = gl_VertexIndex == 0 || gl_VertexIndex == 3 ;
  bottom_right = gl_VertexIndex == 4                        ;
  top_left     = gl_VertexIndex == 2                        ;
  top_right    = gl_VertexIndex == 1 || gl_VertexIndex == 5 ;
  idx          = gl_InstanceIndex                           ;
  
  if( bottom_left ) 
  {
    tex_coords = instances[ idx ].bl ;
  }

  if( top_right ) 
  {
    tex_coords = instances[ idx ].tr ;
  }

  if( top_left ) 
  {
    tex_coords = instances[ idx ].tl ;
  }

  if( bottom_right ) 
  {
    tex_coords = instances[ idx ].br ;
  }
  
  gl_Position = proj * view * instances[ idx ].model_mat * vec4( vertex.xy, 0.0, 1.0 ) ;
}
