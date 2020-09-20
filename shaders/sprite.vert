#version 450 core
#extension GL_ARB_separate_shader_objects : enable

struct InstanceData
{
  mat4  model_mat ;
  vec4  index     ;
};

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

layout( binding = 3 ) uniform SpriteInfo 
{
  uint sprite_width  ;
  uint sprite_height ;
  uint image_width   ;
  uint image_height  ;
};

layout( binding = 4 ) readonly buffer offsets
{
  InstanceData instances[] ;
};


void main()  
{
  uint idx                = uint( tex_index )                                ;
  uint sprite             = uint( instances[ gl_InstanceIndex ].index.x )    ;
  uint num_sprites_in_row = image_width  / sprite_width                      ; 
  uint sprite_y_index     = sprite / num_sprites_in_row                      ;
  uint sprite_x_index     = sprite - ( sprite_y_index * num_sprites_in_row ) ;
  uint sprite_ypixel      = sprite_y_index * sprite_height                   ;
  uint sprite_xpixel      = sprite_x_index * sprite_width                    ;
  
  vec2 texarray[ 4 ] ;

  texarray[ 0 ].x = float( sprite_xpixel                 ) / float( image_width  ) ;
  texarray[ 0 ].y = float( sprite_ypixel + sprite_height ) / float( image_height ) ;
  texarray[ 1 ].x = float( sprite_xpixel + sprite_width  ) / float( image_width  ) ;
  texarray[ 1 ].y = float( sprite_ypixel                 ) / float( image_height ) ;
  texarray[ 2 ].x = float( sprite_xpixel                 ) / float( image_width  ) ;
  texarray[ 2 ].y = float( sprite_ypixel                 ) / float( image_height ) ;
  texarray[ 3 ].x = float( sprite_xpixel + sprite_width  ) / float( image_width  ) ;
  texarray[ 3 ].y = float( sprite_ypixel + sprite_height ) / float( image_height ) ;

  tex_coords  = texarray[ idx ]                                                                   ;
  gl_Position = proj * view * instances[ gl_InstanceIndex ].model_mat * vec4( vertex.xy, 0.0, 1.0 ) ;
}
