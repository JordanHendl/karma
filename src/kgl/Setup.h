/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Setup.h
 * Author: jhendl
 *
 * Created on July 11, 2020, 8:14 PM
 */

#ifndef SETUP_H
#define SETUP_H

namespace kgl
{
  class Setup
  {
    public:
      Setup() ;
      void subscribe( unsigned channel ) ;
      void initialize() ;
      ~Setup() ;
    private:
      struct SetupData* setup_data ;
      SetupData& data() ;
      const SetupData& data() const ;
  };
}

#endif /* SETUP_H */

