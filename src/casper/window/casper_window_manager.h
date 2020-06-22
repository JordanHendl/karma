/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   WindowManager.h
 * Author: jhendl
 *
 * Created on February 24, 2020, 1:19 AM
 */

#ifndef CASPER_WINDOW_MANAGER_H
#define CASPER_WINDOW_MANAGER_H

namespace casper
{
  class WindowManager 
  {
    public:
      WindowManager();
      ~WindowManager();
      
      void create( const char* name, unsigned width, unsigned height, unsigned monitor = 0 ) ;
      void destroy( const char* name ) ;
      void resize( const char* name, unsigned width, unsigned height ) ;
      void setResizable( const char* name, bool val ) ;
      void setFocus( const char* name, bool val ) ;
      void setTitle( const char* name, const char* title ) ;
      void setCurrent( const char* name ) ;
    private:
      struct WindowManagerData* man_data ;
      WindowManagerData* data() ;
      const WindowManagerData* data() const ;
  };
}

#endif /* CASPER_WINDOW_MANAGER_H */

