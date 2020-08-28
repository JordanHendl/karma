#include "KAL_Interface.h"
#include <log/Log.h>
#include "database/Database.h"
#include "Configuration.h"
#include <Parser.h>
#include <Bus.h>
#include <Signal.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <vector>
#include <filesystem>
#include <stdlib.h>
#include <stdio.h>
#include <map>

struct KAL_InterfaceData
{
  typedef std::map<std::string, Mix_Chunk*> SoundMap ;
  typedef std::map<std::string, Mix_Music*> MusicMap ;

  SoundMap                       sound_map          ;
  MusicMap                       music_map          ;
  ::data::module::Bus            bus                ; ///< The bus to use for data transfer.
  ::karma::config::Configuration config             ; ///< The Configuration to initialize the graphics library with.
  kal::database::Database        database           ;
  std::string                    window             ; ///< The name of the current active window.
  std::string                    module_path        ; ///< The path to the modules on the filesystem.
  std::string                    module_config_path ; ///< The path to the modules on the filesystem.
  std::string                    database_path      ; ///< The path to the modules on the filesystem.
  bool                           initialized        ; ///< Whether or not the audio library is initialized.

  void fixString( std::string& str ) ;
  void playSound( const char* img_name ) ;
  void playMusic( unsigned loops, const char* img_name ) ;
  void setDatabasePath( const char* path ) ;
  void directoryCheck() ;
};


void KAL_InterfaceData::playSound( const char* name )
{
  auto iter = this->sound_map.find( name ) ;
  
  if( iter == this->sound_map.end() )
  {
    this->sound_map.insert( { name, Mix_LoadWAV( this->database.sound( name ) ) } ) ;
    Mix_PlayChannel( -1, this->sound_map[ name ], 0 ) ;
  }
  else Mix_PlayChannel( -1, iter->second, 0 ) ;
}

void KAL_InterfaceData::playMusic( unsigned loops, const char* name )
{
  auto iter = this->music_map.find( name ) ;
  
  if( iter == this->music_map.end() )
  {
    this->music_map.insert( { name, Mix_LoadMUS( this->database.music( name ) ) } ) ;
    Mix_PlayMusic( this->music_map[ name ], loops ) ;
  }
  else Mix_PlayMusic( iter->second, loops ) ;
}

void KAL_InterfaceData::setDatabasePath( const char* path )
{
  this->database_path = path ;
}

void KAL_InterfaceData::fixString( std::string& str )
{
  const char c = str.back() ;

  if( c == '\\' || c == '/' )
  {
    str.pop_back() ;
  }
}

void KAL_InterfaceData::directoryCheck()
{
  namespace fs = std::filesystem ;
  
  fs::path        path   ;
  fs::file_status status ;
  bool            valid  ;
  
  valid = true ;
  
  if( !valid )
  {
    ::karma::log::Log::output( karma::log::Log::Level::Fatal, "Cannot find required directories. Please execute program in the correct directory." ) ;
    ::karma::log::Log::write() ;
    exit( EXIT_FAILURE ) ;
  }
    
}

KAL_Interface::KAL_Interface()
{
  this->kal_data = new KAL_InterfaceData() ;
}

KAL_Interface::~KAL_Interface()
{
  delete this->kal_data ;
}

void KAL_Interface::shutdown()
{
  karma::log::Log::write() ;
}

void KAL_Interface::initialize()
{
  using namespace karma::log ;
  
  SDL_Init( SDL_INIT_AUDIO ) ;
  
  if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
  {
    printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
  }

  std::string path ;
  
  path = "./" ;
  
  data().fixString( path ) ;
  data().directoryCheck()  ;
  
  if( !data().initialized )
  {
    data().database.subscribe( 0 ) ;
    
    data().bus( "database_path" ).attach( this->kal_data, &KAL_InterfaceData::setDatabasePath ) ;
    data().bus( "kal::sound"    ).attach( this->kal_data, &KAL_InterfaceData::playSound       ) ;
    data().bus( "kal::music"    ).attach( this->kal_data, &KAL_InterfaceData::playMusic       ) ;

    data().config     .initialize( "./setup.json", 0                 ) ;
    data().config     .initialize( data().database_path.c_str()  , 0 ) ;
    data().config     .initialize( data().database_path.c_str()  , 0 ) ;
    
    karma::log::Log::write() ;
  }
}

void KAL_Interface::loadPack( const char* path )
{
//  this->unloadPack() ;
//  data().config.initialize( path ) ;
}

KAL_InterfaceData& KAL_Interface::data()
{
  return *this->kal_data ;
}

const KAL_InterfaceData& KAL_Interface::data() const
{
  return *this->kal_data ;
}
