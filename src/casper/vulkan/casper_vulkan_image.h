/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   casper_vulkan_image.h
 * Author: jhendl
 *
 * Created on July 3, 2020, 12:51 PM
 */

#ifndef CASPER_VULKAN_IMAGE_H
#define CASPER_VULKAN_IMAGE_H

typedef struct VkImageView_T *VkImageView ;
typedef struct VkSampler_T   *VkSampler   ;

namespace casper
{
  namespace vulkan
  {
    class Image
    {
      public:
        Image() ;
        ~Image() ;
        void initialize( unsigned gpu, unsigned w, unsigned h ) ;
        void initialize( unsigned gpu, const char* img_path ) ;
        const VkSampler& sampler() const ;
        const VkImageView& view() const ;
        void copy( const unsigned char* data ) ;
        void copy( const Image& img ) ;
      private:
        struct ImageData* img_data ;
        ImageData& data() ;
        const ImageData& data() const ;
    };
  }
}

#endif /* CASPER_VULKAN_IMAGE_H */

