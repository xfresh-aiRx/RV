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
 * file .......: ltiNoise_template.h
 * authors ....: Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 06.05.99
 * revisions ..: $Id: ltiNoise_template.h,v 1.8 2003/02/17 07:16:45 alvarado Exp $
 */

namespace lti {
  //--- scalarFunctor parameters

  /// constructor, sets the parameters, i.e. the noise distribution.
  template<class T>
  noise<T>::noise(const parameters& theParams) {
    setParameters(theParams);
  }

  // constructor, sets the noise distribution
  template<class T>
  noise<T>::noise(const continuousRandomDistribution& theDistribution) {
    parameters tmpParam;
    tmpParam.setDistribution(theDistribution);
    setParameters(tmpParam);
  }

  // returns the current parameters
  template<class T>
  const typename noise<T>::parameters& noise<T>::getParameters() const {
    const parameters* params =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if (isNull(params)) {
      throw functor::invalidParametersException(getTypeName());
    }
    return *params;
  }

  // onPlace version of apply.
  template<class T>
  vector<T>& noise<T>::apply(vector<T>& theObject) const {
    double tmp;
    typename vector<T>::iterator iter;
    const parameters& tmpParam = getParameters();

    for(iter=theObject.begin();iter!=theObject.end();iter++) {
      tmp = tmpParam.getDistribution().draw(); // get a random number
      tmp += *iter;


      if(tmp>static_cast<double>(tmpParam.upperLimit)) {
        tmp = static_cast<double>(tmpParam.upperLimit);
      } else if (tmp<static_cast<double>(tmpParam.lowerLimit)) {
        tmp = static_cast<double>(tmpParam.lowerLimit);
      }

      *iter=static_cast<T>(tmp);
    }
    return theObject;
  }

  // onCopy version of apply.
  template<class T>
  vector<T>& noise<T>::apply(const vector<T>& theObject,
           vector<T>& newObject) const {
    newObject.copy(theObject);
    return apply(newObject);
  }

  // onPlace version of apply.
  template<class T>
  matrix<T>& noise<T>::apply(matrix<T>& theObject) const {

    int y;

    for (y=0;y<theObject.rows();y++) {
      apply(theObject.getRow(y));
    }

    return theObject;
  }

  // onCopy version of apply.
  template<class T>
  matrix<T>& noise<T>::apply(const matrix<T>& theObject,
           matrix<T>& newObject) const {
    newObject.copy(theObject);
    return apply(newObject);
  }

}

