#ifndef CASPER_SHADER_H
#define CASPER_SHADER_H
#include <map>
#include <string>
#include <vector>

namespace glslang
{
  class TProgram ;
  class TShader  ;
}

typedef struct VkDescriptorSetLayout_T* VkDescriptorSetLayout ;
typedef struct VkDevice_T*              VkDevice              ;

namespace casper
{
  namespace tools
  {
    typedef std::map<std::string, std::vector<unsigned>>::iterator ShaderIterator ;
    
    class Shader
    {
      public:
        Shader() ;
        ~Shader() ;
        void initialize() ;
        void initialize( const VkDevice &device ) ;
        void addShader( const char* path ) ;
        unsigned char* spirv( const char* name ) ;
        unsigned numDescriptors() const ;
        const VkDescriptorSetLayout* descriptors() const ;


        void write( const char* path ) ;
        void read( const VkDevice& device, const char* path ) ;

        ShaderIterator begin() ;
        ShaderIterator end() ;
      private:

        void generateDescriptorSetBindings( const char* name, glslang::TProgram& program, uint32_t stage_flag ) ;
        void loadShader( const char* path, const char* name ) ;
        struct ShaderData* shader_data ;
        ShaderData& data() ;
        const ShaderData& data() const ;
    } ;
  }
}

#endif