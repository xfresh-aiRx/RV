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


/*----------------------------------------------------------------
 * project ....: LTI Digital Image/Signal Processing Library
 * file .......: ltiMath_template.h
 * authors ....: Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 28.04.99
 * revisions ..: $Id: ltiMath_template.h,v 1.14 2005/10/21 11:16:01 libuda Exp $
 */

namespace lti {

  // returns the value ln( gamma(x) ) for x>0
  template<class T>
  T lnGamma(const T& xx) {
    double x,y,tmp,ser;
    static const double cof[6]={ 76.18009172947146,
                                -86.50532032941677,
                                 24.01409824083091,
                                 -1.231739572450155,
                                  0.1208650973866179e-2,
                                 -0.5395239384953e-5};
    int j;

    if(!(xx>0)) {
      // return std::numeric_limits<T>::quiet_NaN() 
      return static_cast<T>(NaN); // return an invalid number!
    }

    y = x = xx;
    tmp = x+5.5;
    tmp -= (x+0.5)*log(tmp);
    ser = 1.000000000190015;
    for (j=0;j<=5;j++) {
      ser += cof[j]/++y;
    }
    
    return static_cast<T>(-tmp+log(2.5066282746310005*ser/x));
  }

  //the incomplete beta function
  template<class T>
  T betai(const T& a, const T& b, const T& x) {
 
    const double ad = static_cast<double>(a);
    const double bd = static_cast<double>(b);
    const double xd = static_cast<double>(x);

    // the incomplete beta function from 0 to x with parameters a, b
    // x must be in (0,1) (else returns error)
    if ( xd < 0. || xd > 1. ) {
      return static_cast<T>(NaN); // return an invalid number!
    }

    double tmp ( 0. );
    if ( xd > 0. && xd < 1. ) {
      tmp = exp( lnGamma(ad+bd) - lnGamma(ad) - lnGamma(bd)
		 + ad*log(xd) + bd*log(1.-xd) );
    }
    if( x < (ad+1.)/(ad+bd+2.) ) {
      return static_cast<T>(tmp * betacf(ad,bd,xd) / ad);
    }

    return static_cast<T>(1. - tmp * betacf(bd,ad,1.-xd) / bd);
  }
  
}

