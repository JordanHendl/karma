#ifndef CASPER_SHADER_H
#define CASPER_SHADER_H

typedef struct VkVertexInputBindingDescription   VkVertexInputBindingDescription   ;
typedef struct VkVertexInputAttributeDescription VkVertexInputAttributeDescription ;

typedef struct VkShaderModule_T*        VkShaderModule        ;
typedef struct VkDescriptorSetLayout_T* VkDescriptorSetLayout ;

namespace tools
{
  namespace shader
  {
    class ShaderIterator ;
    
    enum ShaderStage : unsigned ;
  }
}

namespace casper
{
  namespace vulkan
  {
    /** TODO
     */
    class Device ;

    /**
     */
    class Shader
    {
      public:
        /**
         */
        Shader() ;
        
        /**
         */
        ~Shader() ;
        
        /**
         * @param path
         */
        void load( const char* path ) ;
        
        /**
         * @return 
         */
        unsigned numShaderModules() const ;
        
        /**
         * @param index
         * @return 
         */
        const VkShaderModule& module( ::tools::shader::ShaderStage stage ) const ;
        
        /**
         * @return 
         */
        const VkDescriptorSetLayout* layout() const ;
                
        /**
         * @return 
         */
        const VkVertexInputAttributeDescription* attributeVertexDesc() const ;
        
        /**
         * @return 
         */
        const VkVertexInputBindingDescription* bindingVertexDesc() const ;
        
        /**
         * @return 
         */
        unsigned numVertexAttr() const ;
        
        /**
         * @return 
         */
        unsigned numVertexDesc() const ;
        
        /**
         * @param device
         * @param path
         */
        void read( const Device& device, const char* path ) ;
        
        /**
         * @return 
         */
        const ::tools::shader::ShaderIterator& begin() const ;
        
        /**
         * @return 
         */
        const ::tools::shader::ShaderIterator& end() const ;
      private:
        
        /**
         */
        struct ShaderData* shader_data ;
        
        /**
         * @return 
         */
        ShaderData& data() ;
        
        /**
         * @return 
         */
        const ShaderData& data() const ;
    } ;
  }
}

#endif