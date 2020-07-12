#ifndef CASPER_DESCRIPTOR_POOL_H
#define CASPER_DESCRIPTOR_POOL_H

/** Vulkan Typedefs
 */
typedef struct VkDescriptorSetPool_T* VkDescriptorSetPool ;
typedef struct VkDescriptorSet_T*     VkDescriptorSet     ;

namespace casper
{
  namespace vulkan
  {
    /**
     */
    class Buffer ;
    
    class Image          ;
    class Window         ;
    class DescriptorPool ;
    class Shader         ;


    class DescriptorSet
    {
      public:
        
        /**
         */
        DescriptorSet() ;
        
        /**
         * @param set
         */
        ~DescriptorSet() ;
        
        /**
         * @param value
         * @param name
         */
        DescriptorSet( const DescriptorSet& set ) ;
        
        /**
         * @param value
         * @param name
         */
        void setImage( const Image& value, const char* name ) const ;

        /**
         * @param value
         * @param name
         */
        void setBuffer( const Buffer& value, const char* name ) const ;
        
//        /**
//         * @param value
//         * @param name
//         */
//        template<typename TYPE>
//        void set( TYPE value, const char* name ) ;
        
        /**
         * @param set
         */
        void operator=( const DescriptorSet& set ) ;
        
        /**
         * @param id
         * @return 
         */
        const VkDescriptorSet* operator[]( unsigned id ) const ;
        
        /**
         * @param id
         * @return 
         */
        const VkDescriptorSet* get( unsigned id ) const ;
        
      private:
      
        /**
         * @param value
         * @param size
         * @param name
         */
        void setBase( void* value, unsigned size, const char* name ) ;

        /**
         */
        struct DescriptorSetData* descriptor_set_data ;

        /**
         * @return 
         */
        DescriptorSetData& data() ;

        /**
         * @return 
         */
        const DescriptorSetData& data() const ;

        /**
         */
        friend class DescriptorPool ;
    };
    
    
    /**
     */
    class DescriptorPool
    {
      public:
        
        /**
         */
        DescriptorPool() ;
        
        /**
         * @param count
         * @return 
         */
        ~DescriptorPool() ;
        
        /**
         * @param count
         * @return 
         */
        DescriptorSet makeDescriptorSet( unsigned count ) ;
        
        /**
         * @return 
         */
        const VkDescriptorSetPool& pool() const ;

        /**
         * @param window
         * @param shader
         * @param pipeline_name
         */
        void initialize( unsigned gpu, unsigned num_descriptors, const Shader& shader, const char* pipeline_name ) ;

      private:
        
        /**
         */
        struct DescriptorPoolData* pool_data ;
        
        /**
         * @return 
         */
        DescriptorPoolData& data() ;
        
        /**
         * @param value
         * @param name
         */
        const DescriptorPoolData& data() const ;
    };
    
//    template<typename TYPE>
//    void DescriptorSet::set( TYPE value, const char* name )
//    {
//      this->setBase( static_cast<void*>( value ), sizeof( value ), name ) ;
//    }
  }
}
#endif