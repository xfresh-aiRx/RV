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
 * file .......: ltiLinearMapperFunctor.cpp
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 10.7.2000
 * revisions ..: $Id: ltiLinearMapperFunctor_template.h,v 1.13 2005/02/02 14:16:23 doerfler Exp $
 */

namespace lti {
  // --------------------------------------------------
  // linearMapperFunctor<Tin,Tout>::parameters
  // --------------------------------------------------

  // following code is now in the header file because of problems with the
  // MS-VC++ (as usual it cannot deal with standard ANSI C++...)
#if 0
  // default constructor
  template<class Tin, class Tout>
  linearMapperFunctor<Tin,Tout>::parameters::parameters()
    : mapperFunctor<Tin,Tout>::parameters() {

  }

  // copy constructor
  template<class Tin, class Tout>
  linearMapperFunctor<Tin,Tout>::parameters::parameters(const parameters& other) {
    copy(other);
  }

  // destructor
  template<class Tin, class Tout>
    lti::linearMapperFunctor<Tin,Tout>::parameters::~parameters() {
  }

  // get type name
  template<class Tin, class Tout>
  const char* linearMapperFunctor<Tin,Tout>::parameters::getTypeName() const {
    return "linearMapperFunctor::parameters";
  }

  // copy member
  template<class Tin, class Tout>
  linearMapperFunctor<Tin,Tout>::parameters&
    linearMapperFunctor<Tin,Tout>::parameters::copy(const parameters& other) {
# ifndef _LTI_MSC_6
    // MS Visual C++ 6 is not able to compile this...
    mapperFunctor<Tin,Tout>::parameters::copy(other);
# else
    // ...so we have to use this workaround.
    // Conditional on that, copy may not be virtual.
    mapperFunctor<Tin,Tout>::parameters&
      (mapperFunctor<Tin,Tout>::parameters::* p_copy)
      (const mapperFunctor<Tin,Tout>::parameters&) =
      mapperFunctor<Tin,Tout>::parameters::copy;
    (this->*p_copy)(other);
# endif

    return *this;
  }

  // clone member
  template<class Tin, class Tout>
  functor::parameters*
      linearMapperFunctor<Tin,Tout>::parameters::clone() const {
    return new parameters(*this);
  }

#endif
  // --------------------------------------------------
  // linearMapperFunctor<Tin,Tout>
  // --------------------------------------------------

  // default constructor
  template<class Tin, class Tout>
  linearMapperFunctor<Tin,Tout>::linearMapperFunctor<Tin,Tout>()
    : mapperFunctor<Tin,Tout>(){
  }

  // copy constructor
  template<class Tin, class Tout>
  linearMapperFunctor<Tin,Tout>::linearMapperFunctor<Tin,Tout>(
      const linearMapperFunctor<Tin,Tout>& other) {
    copy(other);
  }

  // destructor
  template<class Tin, class Tout>
  linearMapperFunctor<Tin,Tout>::~linearMapperFunctor<Tin,Tout>() {
  }

  // returns the name of this type
  template<class Tin, class Tout>
  const char* linearMapperFunctor<Tin,Tout>::getTypeName() const {
    return "linearMapperFunctor";
  }

  // copy member
  template<class Tin, class Tout>
  linearMapperFunctor<Tin,Tout>& linearMapperFunctor<Tin,Tout>
  ::copy(const linearMapperFunctor<Tin,Tout>& other) {
    mapperFunctor<Tin,Tout>::copy(other);
    return (*this);
  }

  // clone member
  template<class Tin, class Tout>
  functor* linearMapperFunctor<Tin,Tout>::clone() const {
    return new linearMapperFunctor<Tin,Tout>(*this);
  }

  // return parameters
  template<class Tin, class Tout>
  const typename linearMapperFunctor<Tin,Tout>::parameters&
    linearMapperFunctor<Tin,Tout>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(getTypeName());
    }

    return *par;
  }

  template<class Tin,class Tout>
  bool linearMapperFunctor<Tin,Tout>::updateParameters() {

    const linearMapperFunctor<Tin,Tout>::parameters& param = getParameters();
    
    if ((param.lowerLimitsInput.size() != param.upperLimitsInput.size()) ||
        (param.lowerLimitsOutput.size()!= param.upperLimitsOutput.size())||
        (param.lowerLimitsInput.size() != param.upperLimitsOutput.size())) {
      throw invalidParametersException(getTypeName());
    }
    
    // calculate the slope
    int i,dim;
    float tmp1,tmp2;
    dim = param.lowerLimitsInput.size();
    
    slope.resize(dim,0.0f,false,false);
    b.resize(dim,0.0f,false,false);
    
    for (i=0;i<dim;i++) {
      tmp1 = float(param.upperLimitsInput.at(i)-
                   param.lowerLimitsInput.at(i));
      
      tmp2 = float(param.upperLimitsOutput.at(i)-
                   param.lowerLimitsOutput.at(i));
      
      if (tmp1 != 0) {
        slope.at(i) = tmp2/tmp1;
        b.at(i) = float(param.lowerLimitsOutput.at(i))-
          (tmp2*float(param.lowerLimitsInput.at(i))/tmp1);
      } else {
        slope.at(i) = float(1.0e+30);
        b.at(i) = 0;
      }
    }
    return true;    
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  // On copy apply for type vector!
  template<class Tin, class Tout>
  vector<Tout>& linearMapperFunctor<Tin,Tout>::apply(const vector<Tin>& src,
                                                     vector<Tout>& dest) const{

    // vectors must have the correct dimension!
    assert (src.size() == slope.size());

    // destination vector with the correct dimension!
    dest.resize(src.size(),Tout(),false,false);

    int i;
    for (i=0;i<src.size();i++) {
      dest.at(i) = Tout((src.at(i)*slope.at(i)) + b.at(i));
    }

    return dest;
  };

  // On copy apply for type ivector!
  template<class Tin, class Tout>
  point& linearMapperFunctor<Tin,Tout>::apply(const vector<Tin>& src,
                                              point& dest) const {
    // vectors must have the correct dimension!
    assert(src.size() == 2);

    dest.x = int(src.at(0)*slope.at(0) + b.at(0));
    dest.y = int(src.at(1)*slope.at(1) + b.at(1));

    return dest;
  };

  // On copy apply for type point!
  template<class Tin, class Tout>
    point& linearMapperFunctor<Tin,Tout>::apply(const Tin& src0,
                                                const Tin& src1,
                                                point& dest) const {
    // vectors must have the correct dimension!
    assert (slope.size() == 2);

    dest.x = int(src1*slope.at(0) + b.at(0));
    dest.y = int(src0*slope.at(1) + b.at(1));

    return dest;
  };

  // On copy apply for type ivector!
  template<class Tin, class Tout>
  point& linearMapperFunctor<Tin,Tout>::apply(const point& src,
                                              point& dest) const {
    // vectors must have the correct dimension!
    assert (slope.size() == 2);

    dest.x = int(src.x*slope.at(0) + b.at(0));
    dest.y = int(src.y*slope.at(1) + b.at(1));

    return dest;
  };

  // --------------- 1D -----------------

  // On copy apply for type int!
  template<class Tin, class Tout>
  Tout& linearMapperFunctor<Tin,Tout>::apply(const vector<Tin>& src,
                                                          Tout& dest) const {

    // vectors must have the correct dimension!
    assert (src.size() == 1);

    dest = Tout(src.at(0)*slope.at(0) + b.at(0));

    return dest;
  };

  // On copy apply for type int!
  template<class Tin, class Tout>
  Tout& linearMapperFunctor<Tin,Tout>::apply(const Tin& src,
                                            Tout& dest) const {

    // vectors must have the correct dimension!
    assert (src.size() == 1);

    dest = Tout(src*slope.at(0) + b.at(0));

    return dest;
  };



}

