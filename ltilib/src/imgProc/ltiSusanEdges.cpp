/*
 * Copyright (C) 1998, 1999, 2000, 2001
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 *
 * This file can be used with the LTI-Computer Vision Library (LTI-Lib)
 * (please note this file is NOT part of the LTI-Lib)
 * This code is based on the original C code by S. Smith, which contains
 * following header:
 *
 **********************************************************************
 *
 * SUSAN Version 2l by Stephen Smith
 *
 * Oxford Centre for Functional Magnetic Resonance Imaging of the
 * Brain, Department of Clinical Neurology, Oxford University, Oxford,
 * UK (Previously in Computer Vision and Image Processing Group - now
 * Computer Vision and Electro Optics Group - DERA Chertsey, UK)
 *
 * Email:    steve@fmrib.ox.ac.uk
 * WWW:      http://www.fmrib.ox.ac.uk/~steve
 *
 * (C) Crown Copyright (1995-1999), Defence Evaluation and Research Agency,
 * Farnborough, Hampshire, GU14 6TD, UK
 * DERA WWW site:
 * http://www.dera.gov.uk/
 * DERA Computer Vision and Electro Optics Group WWW site:
 * http://www.dera.gov.uk/imageprocessing/dera/group_home.html
 * DERA Computer Vision and Electro Optics Group point of contact:
 * Dr. John Savage, jtsavage@dera.gov.uk, +44 1344 633203
 *
 * A UK patent has been granted: "Method for digitally processing
 * images to determine the position of edges and/or corners therein for
 * guidance of unmanned vehicle", UK Patent 2272285. Proprietor:
 * Secretary of State for Defence, UK. 15 January 1997
 *
 * This code is issued for research purposes only and remains the
 * property of the UK Secretary of State for Defence. This code must
 * not be passed on without this header information being kept
 * intact. This code must not be sold.
 **********************************************************************
 *
 *
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
 *
 */

/*----------------------------------------------------------------
 * project ....: LTI Digital Image/Signal Processing Library
 * file .......: ltiSusanEdges.cpp
 * authors ....: Stefan Syberichs
 * organization: LTI, RWTH Aachen
 * creation ...: 30.03.2000
 * revisions ..: $Id: ltiSusanEdges.cpp,v 1.24 2005/06/16 12:35:44 doerfler Exp $
 */

#include "ltiSusan.h"
#include "ltiSusanEdges.h"
#include "ltiImage.h"

namespace lti {

  // //////////////////////////////////////////////////////////////////////
  //                         susanEdges::parameters                      //
  // //////////////////////////////////////////////////////////////////////


  // default constructor
  susanEdges::parameters::parameters()
    : edgeDetector::parameters(),threshold(20) {
  }

  // copy constructor
  susanEdges::parameters::parameters(const parameters& other)
    : edgeDetector::parameters()  {
    copy(other);
  }

  // destuctor
  susanEdges::parameters::~parameters() {
  }

  // returns the name of the parameter type
  const char* susanEdges::parameters::getTypeName() const {
    return "susanEdges::parameters";
  }

  // clone method
  functor::parameters* susanEdges::parameters::clone() const {
    return (new parameters(*this));
  }

  // copy method
  susanEdges::parameters& susanEdges::parameters::copy(const parameters& other) {
# ifndef _LTI_MSC_6
    // MS Visual C++ 6 is not able to compile this...
    edgeDetector::parameters::copy(other);
# else
    // ...so we have to use this workaround.
    // Conditional on that, copy may not be virtual.
    edgeDetector::parameters& (edgeDetector::parameters::* p_copy)
      (const edgeDetector::parameters&) = edgeDetector::parameters::copy;
    (this->*p_copy)(other);
#  endif

    threshold  = other.threshold;

    return (*this);
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was succeful
   */
# ifndef _LTI_MSC_6
  bool susanEdges::parameters::write(ioHandler& handler,
                                         const bool complete) const
# else
  bool susanEdges::parameters::writeMS(ioHandler& handler,
                                           const bool complete) const
# endif
  {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"threshold",threshold);
    }


# ifndef _LTI_MSC_6
    // This is the standard C++ code, which MS Visual C++ 6 is not able to
    // compile...
    b = b && modifier::parameters::write(handler,false);
# else
    bool (modifier::parameters::* p_writeMS)(ioHandler&,const bool) const =
      modifier::parameters::writeMS;
    b = b && (this->*p_writeMS)(handler,false);
# endif

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

# ifdef _LTI_MSC_6
  bool susanEdges::parameters::write(ioHandler& handler,
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
   * @return true if write was succeful
   */
# ifndef _LTI_MSC_6
  bool susanEdges::parameters::read(ioHandler& handler,
                                    const bool complete)
# else
  bool susanEdges::parameters::readMS(ioHandler& handler,
                                      const bool complete)
# endif
  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"threshold",threshold);
    }

# ifndef _LTI_MSC_6
    // This is the standard C++ code, which MS Visual C++ 6 is not able to
    // compile...
    b = b && modifier::parameters::read(handler,false);
# else
    bool (modifier::parameters::* p_readMS)(ioHandler&,const bool) =
      modifier::parameters::readMS;
    b = b && (this->*p_readMS)(handler,false);
# endif

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

# ifdef _LTI_MSC_6
  bool susanEdges::parameters::read(ioHandler& handler,
                                    const bool complete) {
    // ...we need this workaround to cope with another really awful MSVC
    // bug.
    return readMS(handler,complete);
  }
# endif


  // //////////////////////////////////////////////////////////////////////
  //                                 susanEdges                          //
  // //////////////////////////////////////////////////////////////////////


  susanEdges::susanEdges() : edgeDetector() {
    parameters defaultParam;
    setParameters(defaultParam);
  }

  susanEdges::susanEdges(const parameters& par) : edgeDetector() {
    setParameters(par);
  }

  // copy constrcutor
  susanEdges::susanEdges(const susanEdges& other) : edgeDetector() {
    copy(other);
  }

   // destructor
   susanEdges::~susanEdges() {
   }

   // clone method
   functor* susanEdges::clone() const {
     // calls the copy constructor...
     return (new susanEdges(*this));
   }

  // returns the name of the modifier type
   const char* susanEdges::getTypeName() const {
     return "susanEdges";
   }

   // returns the current parameters
   const susanEdges::parameters& susanEdges::getParameters() const  {
     const parameters* params=0;
     params =
       dynamic_cast<const susanEdges::parameters*>(&functor::getParameters());
     if(params==0) {
       throw invalidParametersException(getTypeName());
     }

     return *params;
   }

   // overload setParameters to detect range errors
   bool susanEdges::updateParameters() {

     if (getParameters().threshold < 1 || getParameters().threshold > 255) {
       setStatusString("Threshold not in valid range 1<=threshold <=255");
       return false;
     }

     return true;
   }

   // apply on copy for channel8's
   bool susanEdges::apply(const channel8& src, channel8& dest) const  {

     susan detector;
     const parameters& par = getParameters();

     // set brightness treshold
     detector.setThreshold(par.threshold);
     detector.setOffOnValues(par.noEdgeValue,par.edgeValue);

     // detect edges and store in dest
     detector.binaryEdges(src, dest);

     return true;

   }

   // apply on copy for channel8's
   bool susanEdges::apply(const channel& src, channel& dest) const  {

     susan detector;
     const parameters& par = getParameters();

     // set brightness treshold
     detector.setThreshold(getParameters().threshold);
     detector.setOffOnValues(par.noEdgeValue,par.edgeValue);

     // detect edges and store in dest
     detector.binaryEdges(src, dest);

     return true;
   }

   // apply on copy for channel8's
   bool susanEdges::apply(const channel& src, channel8& dest) const {
     channel8 tmp;
     tmp.castFrom(src);
     return apply(tmp,dest);
   }

   bool susanEdges::apply(const image& src, image& dest) const {
     return edgeDetector::apply(src,dest);
   }

   // apply on copy for images, returning a channel
   // The image is splitted into RGB components,
   // the output channel is OR-combined of the edge results of
   // every channel
   bool susanEdges::apply(const image& src, channel8& dest) const {
     return edgeDetector::apply(src,dest);
   }

   // onPlace version of apply for lti::channel8's.
   // is slower than OnCopy .
   bool susanEdges::apply(channel8& srcdest) const {
     return edgeDetector::apply(srcdest);
   }

   // onPlace version of apply for lti::channel8's.
   // is slower than OnCopy .
   bool susanEdges::apply(channel& srcdest) const {
     return edgeDetector::apply(srcdest);
   }


   // OnPlace version of apply for lti::images.
   // is slower than OnCopy.
   bool susanEdges::apply(image& srcdest) const {
     return edgeDetector::apply(srcdest);
   }

}; // namespace lti




