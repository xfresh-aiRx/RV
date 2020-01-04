/*
 * Copyright (C) 1998 - 2004
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 *
 * This file is part of the LTI-Computer Vision Library (LTI-Lib)
 *
 * The LTI-Lib is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License (LGPL)
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * The LTI-Lib is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the LTI-Lib; see the file LICENSE.  If
 * not, write to the Free Software Foundation, Inc., 59 Temple Place -
 * Suite 330, Boston, MA 02111-1307, USA.
 */


/* -------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiBayerDemosaicing.cpp
 * authors ....: Arnd Hannemann 
 * organization: LTI, RWTH Aachen
 * creation ...: 28.10.2004
 * revisions ..: $Id: ltiBayerDemosaicing.cpp,v 1.3 2005/06/16 12:38:53 doerfler Exp $
 */

//TODO: include files
#include "ltiBayerDemosaicing.h"


#undef _LTI_DEBUG
//#define _LTI_DEBUG 1
#include "ltiDebug.h"

namespace lti {
  // --------------------------------------------------
  // bayerDemosaicing::parameters
  // --------------------------------------------------

  // default constructor
  bayerDemosaicing::parameters::parameters()
    : transform::parameters() {
    //TODO: Initialize your parameter values!
    // If you add more parameters manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new parameters
    // 3. make sure that the read and write members also read and
    //    write your parameters
    
    method = Simple;
    bayerPattern = RGGB;
  }

  // copy constructor
  bayerDemosaicing::parameters::parameters(const parameters& other)
    : transform::parameters() {
    copy(other);
  }

  // destructor
  bayerDemosaicing::parameters::~parameters() {
  }

  // get type name
  const char* bayerDemosaicing::parameters::getTypeName() const {
    return "bayerDemosaicing::parameters";
  }

  // copy member

  bayerDemosaicing::parameters&
    bayerDemosaicing::parameters::copy(const parameters& other) {
# ifndef _LTI_MSC_6
    // MS Visual C++ 6 is not able to compile this...
    transform::parameters::copy(other);
# else
    // ...so we have to use this workaround.
    // Conditional on that, copy may not be virtual.
    transform::parameters& (transform::parameters::* p_copy)
      (const transform::parameters&) =
      transform::parameters::copy;
    (this->*p_copy)(other);
# endif

    
      method = other.method;
      bayerPattern = other.bayerPattern;

    return *this;
  }

  // alias for copy member
  bayerDemosaicing::parameters&
    bayerDemosaicing::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // clone member
  functor::parameters* bayerDemosaicing::parameters::clone() const {
    return new parameters(*this);
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was succeful
   */
# ifndef _LTI_MSC_6
  bool bayerDemosaicing::parameters::write(ioHandler& handler,
                                         const bool complete) const
# else
  bool bayerDemosaicing::parameters::writeMS(ioHandler& handler,
                                           const bool complete) const
# endif
  {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      
      lti::write(handler,"bayerMethod",method);
      lti::write(handler,"bayerPattern",bayerPattern);
    }

# ifndef _LTI_MSC_6
    // This is the standard C++ code, which MS Visual C++ 6 is not able to
    // compile...
    b = b && transform::parameters::write(handler,false);
# else
    bool (transform::parameters::* p_writeMS)(ioHandler&,const bool) const =
      transform::parameters::writeMS;
    b = b && (this->*p_writeMS)(handler,false);
# endif

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

# ifdef _LTI_MSC_6
  bool bayerDemosaicing::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    // ...we need this workaround to cope with another really awful MSVC bug.
    return writeMS(handler,complete);
  }
# endif

  /*
   * read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if read was succeful
   */
# ifndef _LTI_MSC_6
  bool bayerDemosaicing::parameters::read(ioHandler& handler,
                                        const bool complete)
# else
  bool bayerDemosaicing::parameters::readMS(ioHandler& handler,
                                          const bool complete)
# endif
  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
      lti::read(handler,"bayerMethod",method);
      lti::read(handler,"bayerPattern",bayerPattern);
    }

# ifndef _LTI_MSC_6
    // This is the standard C++ code, which MS Visual C++ 6 is not able to
    // compile...
    b = b && transform::parameters::read(handler,false);
# else
    bool (transform::parameters::* p_readMS)(ioHandler&,const bool) =
      transform::parameters::readMS;
    b = b && (this->*p_readMS)(handler,false);
# endif

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

# ifdef _LTI_MSC_6
  bool bayerDemosaicing::parameters::read(ioHandler& handler,
                                        const bool complete) {
    // ...we need this workaround to cope with another really awful MSVC
    // bug.
    return readMS(handler,complete);
  }
# endif

  // --------------------------------------------------
  // bayerDemosaicing
  // --------------------------------------------------

  // default constructor
  bayerDemosaicing::bayerDemosaicing()
    : transform(){

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);


  }

  // default constructor
  bayerDemosaicing::bayerDemosaicing(const parameters& par)
    : transform() {

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  bayerDemosaicing::bayerDemosaicing(const bayerDemosaicing& other)
    : transform() {
    copy(other);
  }

  // destructor
  bayerDemosaicing::~bayerDemosaicing() {
  }

  // returns the name of this type
  const char* bayerDemosaicing::getTypeName() const {
    return "bayerDemosaicing";
  }

  // copy member
  bayerDemosaicing&
    bayerDemosaicing::copy(const bayerDemosaicing& other) {
      transform::copy(other);

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    return (*this);
  }

  // alias for copy member
  bayerDemosaicing&
    bayerDemosaicing::operator=(const bayerDemosaicing& other) {
    return (copy(other));
  }


  // clone member
  functor* bayerDemosaicing::clone() const {
    return new bayerDemosaicing(*this);
  }

  // return parameters
  const bayerDemosaicing::parameters&
    bayerDemosaicing::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(getTypeName());
    }
    return *par;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  // inline function for basic color reconstruction
  bool bayerDemosaicing::nearestNeighborDemosaicing(const matrix<ubyte>& src,
                                                    matrix<rgbPixel>& dest)
    const { 

    // destination image has one row less than source
    const int dRows = src.rows()-1;
    // destination image has one column less than source
    const int dCols = src.columns()-1;
    
    dest.resize(dRows,dCols,rgbPixel(),false,false);

    // iterator through destination row
    vector<rgbPixel>::iterator dIt;
    // end of destination row
    vector<rgbPixel>::iterator dIte;
    
    // iterator through source rows
    vector<ubyte>::const_iterator sIt1;
    vector<ubyte>::const_iterator sIt2;    
       
    int i;
    for (i=0;i<dRows;i++) {
      if (even(i)) {
        sIt1=src.getRow(i).begin();
        sIt2=src.getRow(i+1).begin();
      } else {
        sIt2=src.getRow(i).begin();
        sIt1=src.getRow(i+1).begin();
      }

      dIte = dest.getRow(i).end()-1;     
      dIt = dest.getRow(i).begin();
      ubyte r,g,b;

      for (;dIt != dIte; ++dIt) {
        // for even...
        // R1 G1 R2 G2     P1_r = R1
        // G3 B1 G4 B2     P1_g = G3
        //                 P1_b = B1
        r = *sIt1; ++sIt1;
        g = *sIt2; ++sIt2;
        b = *sIt2;
        (*dIt).set(r,g,b);
        // P2_r = R2    P2_g = G1  P2_b = B1
        ++dIt;
        b = *sIt2; ++sIt2;
        g = *sIt1; ++sIt1;
        r = *sIt1;  
        (*dIt).set(r,g,b);
        _lti_debug("r:"<<r<<" g:"<<g<<" b:"<<b<<"\n");
      }
      //right border
      if (dIt != dest.getRow(i).end()-1) {
        ++dIt;
        r = *sIt1; ++sIt1;
        g = *sIt2; ++sIt2;
        b = *sIt2;
        (*dIt).set(r,g,b);
      }
      
    }

    return true;
  }


  // inline function for Simple demosaicing
  // this algorithm is taken from AVT Marlin Technical Manual
  bool bayerDemosaicing::simpleDemosaicing(const matrix<ubyte>& src, 
                                            matrix<rgbPixel>& dest) const {
    // the size of the resulting matrix is reduced in both dimensions of 1
    const int dRows = src.rows()-1;
    const int dCols = src.columns()-1;
    

    dest.resize(dRows,dCols,rgbPixel(),false,false);

    // iterator through destination
    vector<rgbPixel>::iterator dIt;
    // end of each destineation row
    vector<rgbPixel>::iterator dIte;
    vector<ubyte>::const_iterator sIt1;
    vector<ubyte>::const_iterator sIt2;
    ubyte r,g,b;
    int i;
    for (i=0;i<dRows;i++) {
      if (even(i)) {
        sIt1=src.getRow(i).begin();
        sIt2=src.getRow(i+1).begin();
      } else {
        sIt2=src.getRow(i).begin();
        sIt1=src.getRow(i+1).begin();
      }

      dIte = dest.getRow(i).end()-1;     
      dIt = dest.getRow(i).begin();
      
      for (;dIt != dIte; ++dIt) {
        // for even...
        // R1 G1 R2 G2     P1_r = R1
        // G3 B1 G4 B2     P1_g = (G1+G3)/2
        //                 P1_b = B1
        r = *sIt1; ++sIt1;
        g = (*sIt1 + *sIt2) / 2; ++sIt2;
        b = *sIt2;
        (*dIt).set(r,g,b);
        // P2_r = R2    P2_g = (G1+G4)/2   P2_b = B1
        ++dIt;
        b = *sIt2; sIt2++;
        g = (*sIt1 + *sIt2) / 2; ++sIt1;
        r = *sIt1;
        
        (*dIt).set(r,g,b);
        _lti_debug("r:"<<r<<" g:"<<g<<" b:"<<b<<"\n");
      }
      
      //right border
      if (dIt != dest.getRow(i).end()-1) {
        ++dIt;
        r = *sIt1; ++sIt1;
        g = (*sIt1 + *sIt2) / 2; ++sIt2;
        b = *sIt2;
        (*dIt).set(r,g,b);
      }
    }
    return true;
  }

  // On copy apply for type matrix<ubyte>!
  bool bayerDemosaicing::apply(const matrix<ubyte>& src,
                               matrix<rgbPixel>& dest) const {

    const parameters& par = getParameters();
    switch (par.method) {
      case (Simple): {
        return simpleDemosaicing(src,dest);
        break;
      }
      case (NearestNeighbor): {
        return nearestNeighborDemosaicing(src,dest);
        break;
      }
    }
    return true;
  };

  

  // -------------------------------------------------------------------
  // global read and write functions for enumerations
  // -------------------------------------------------------------------
  bool read(ioHandler& handler,bayerDemosaicing::eBayerMethod& data) {

    std::string str;
    if (handler.read(str)) {
      if (str == "Simple") {
        data = bayerDemosaicing::Simple;
      } else if (str == "NearestNeighbor") {
        data = bayerDemosaicing::NearestNeighbor;
      } else {
        // default
        data = bayerDemosaicing::Simple;
      }
      return true;
    } else {
      return false;
    }    
  }

  bool write(ioHandler& handler,const bayerDemosaicing::eBayerMethod& data) {
    bool b=false;
    std::string str;
    switch(data) {
      case bayerDemosaicing::Simple: {
        b=handler.write("Simple");
        break;
      }
      case bayerDemosaicing::NearestNeighbor: {
        b=handler.write("NearestNeighbor");
        break;
      }
    }
    return b;
  }


  bool read(ioHandler& handler,bayerDemosaicing::eBayerPattern& data) {

    std::string str;
    if (handler.read(str)) {
      if (str == "RGGB") {
        data = bayerDemosaicing::RGGB;
      } else {
        // default
        data = bayerDemosaicing::RGGB;
      }
      return true;
    } else {
      return false;
    }    
  }

  bool write(ioHandler& handler,const bayerDemosaicing::eBayerPattern& data) {
    bool b=false;
    std::string str;
    switch(data) {
      case bayerDemosaicing::RGGB: {
        b=handler.write("RGGB");
        break;
      }
    }
    return b;
  }

}

#include "ltiUndebug.h"
