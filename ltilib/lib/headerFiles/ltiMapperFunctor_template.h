/*
 * Copyright (C) 1998, 1999, 2000, 2001
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


/* -----------------------------------------------------------------------
 * project ....: LTI Digitale Bild/Signal Verarbeitungsbibliothek
 * file .......: ltiMapperFunctor_template.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 10.7.2000
 * revisions ..: $Id: ltiMapperFunctor_template.h,v 1.12 2005/02/02 14:16:23 doerfler Exp $
 */

namespace lti {
  // --------------------------------------------------
  // mapperFunctor::parameters
  // --------------------------------------------------

  // following code is now in the header file because of problems with the
  // MS-VC++ (as usual it cannot deal with standard ANSI C++...)
#if 0
  // default constructor
  template<class Tin, class Tout>
  mapperFunctor<Tin,Tout>::parameters::parameters()
    : functor::parameters() {
  }

  // copy constructor
  template<class Tin, class Tout>
  mapperFunctor<Tin,Tout>::parameters::parameters(const parameters& other) {
    copy(other);
  }

  // destructor
  template<class Tin, class Tout>
  mapperFunctor<Tin,Tout>::parameters::~parameters() {
  }

  // get type name
  template<class Tin, class Tout>
  const char* mapperFunctor<Tin,Tout>::parameters::getTypeName() const {
    return "mapperFunctor::parameters";
  }

  // copy member

  template<class Tin, class Tout>
  mapperFunctor<Tin,Tout>::parameters&
    mapperFunctor<Tin,Tout>::parameters::copy(const parameters& other) {
# ifndef _LTI_MSC_6
    // MS Visual C++ 6 is not able to compile this...
    functor::parameters::copy(other);
# else
    // ...so we have to use this workaround.
    // Conditional on that, copy may not be virtual.
    functor::parameters& (functor::parameters::* p_copy)
      (const functor::parameters&) =
      functor::parameters::copy;
    (this->*p_copy)(other);
# endif

      lowerLimitsInput.copy(other.lowerLimitsInput);
      lowerLimitsOutput.copy(other.lowerLimitsOutput);
      upperLimitsInput.copy(other.upperLimitsInput);
      upperLimitsOutput.copy(other.upperLimitsOutput);

    return *this;
  }

  // generate from a histogram description
  template<class Tin, class Tout>
  void mapperFunctor<Tin,Tout>::parameters::generateFrom(
      const histogram& histo) {
    lowerLimitsInput.resize(histo.dimensions(),Tin(0.0),false);
    upperLimitsInput.resize(histo.dimensions(),Tout(1.0),false);
    lowerLimitsOutput.castFrom(histo.getFirstCell());
    upperLimitsOutput.castFrom(histo.getLastCell());
  }

  // clone member
  template<class Tin, class Tout>
  functor::parameters* mapperFunctor<Tin,Tout>::parameters::clone() const {
    return new parameters(*this);
  }
#endif // MS VC++ Bug

  // --------------------------------------------------
  // mapperFunctor<Tin,Tout>
  // --------------------------------------------------

  // default constructor
  template<class Tin, class Tout>
  mapperFunctor<Tin,Tout>::mapperFunctor<Tin,Tout>()
    : functor(){
  }

  // copy constructor
  template<class Tin, class Tout>
  mapperFunctor<Tin,Tout>::mapperFunctor<Tin,Tout>(const mapperFunctor<Tin,Tout>& other) {
    copy(other);
  }

  // destructor
  template<class Tin, class Tout>
  mapperFunctor<Tin,Tout>::~mapperFunctor<Tin,Tout>() {
  }

  // returns the name of this type
  template<class Tin, class Tout>
  const char* mapperFunctor<Tin,Tout>::getTypeName() const {
    return "mapperFunctor";
  }

  // copy member
  template<class Tin, class Tout>
  mapperFunctor<Tin,Tout>&
  mapperFunctor<Tin,Tout>::copy(const mapperFunctor<Tin,Tout>& other) {
    functor::copy(other);
    return (*this);
  }

  // clone member
  template<class Tin, class Tout>
  functor* mapperFunctor<Tin,Tout>::clone() const {
    return new mapperFunctor<Tin,Tout>(*this);
  }

  // return parameters
  template<class Tin, class Tout>
  const typename mapperFunctor<Tin,Tout>::parameters&
  mapperFunctor<Tin,Tout>::getParameters() const {
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


  // On copy apply for type vector!
  template<class Tin, class Tout>
  vector<Tout>& mapperFunctor<Tin,Tout>::apply(const vector<Tin>& src,
                                                     vector<Tout>& dest) const{
    throw invalidMethodException();

    return dest;
  };

  // On copy apply for type ivector!
  template<class Tin, class Tout>
  point& mapperFunctor<Tin,Tout>::apply(const vector<Tin>& src,
                                              point& dest) const {
    throw invalidMethodException();

    return dest;
  };

  // On copy apply for type point!
  template<class Tin, class Tout>
    point& mapperFunctor<Tin,Tout>::apply(const Tin& src0,
                                          const Tin& src1,
                                              point& dest) const {
    throw invalidMethodException();

    return dest;
  };

  // On copy apply for type ivector!
  template<class Tin, class Tout>
  point& mapperFunctor<Tin,Tout>::apply(const point& src,
                                              point& dest) const {
    throw invalidMethodException();

    return dest;
  };

  // --------------- 1D -----------------


  // On copy apply for type int!
  template<class Tin, class Tout>
  Tout& mapperFunctor<Tin,Tout>::apply(const vector<Tin>& src,
                                                    Tout& dest) const {
    throw invalidMethodException();

    return dest;
  };

  // On copy apply for type int!
  template<class Tin, class Tout>
  Tout& mapperFunctor<Tin,Tout>::apply(const Tin& src,
                                            Tout& dest) const {
    throw invalidMethodException();

    return dest;
  };


}

