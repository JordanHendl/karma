#ifndef IMAGELOADER_H
#define IMAGELOADER_H

namespace kgl
{
  namespace io
  { 
    /**
     */
    class ImageLoader
    {
      public:
        
        /**
         */
        ImageLoader() ;
        
        /**
         * @param path
         */
        ~ImageLoader() ;
        
        /**
         * @return 
         */
        void load( const char* path ) ;
        
        /**
         */
        void reset() ;
        
        /**
         * @return 
         */
        unsigned width() const ;
        
        /**
         * @return 
         */
        unsigned height() const ;
        
        /**
         * @return 
         */
        unsigned channels() const ;

        /**
         * @return 
         */
        const unsigned char* pixels() const ;
      private:
        
        /**
         */
        struct ImageLoaderData* loader_data ;
        
        /**
         * @return 
         */
        ImageLoaderData& data() ;
        
        /**
         * @return 
         */
        const ImageLoaderData& data() const ;
    };
  }
}

#endif /* IMAGELOADER_H */

