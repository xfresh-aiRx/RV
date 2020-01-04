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


/* -------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiMinimumSpanningTreeOfKeyValuetype_template.h
 * authors ....: Jens Paustenbach
 * organization: LTI, RWTH Aachen
 * creation ...: 8.5.2003
 * revisions ..: $Id: ltiMinimumSpanningTreeOfKeyValuetype_template.h,v 1.5 2004/05/03 12:20:01 alvarado Exp $
 */

namespace lti {
  // --------------------------------------------------
  // minimumSpanningTreeOfKeyValuetype::parameters
  // --------------------------------------------------

  // implementation on header file due to MS VC++ bug

  // --------------------------------------------------
  // minimumSpanningTreeOfKeyValuetype
  // --------------------------------------------------

  // default constructor
  template<class K,class V,class Distantor>
  minimumSpanningTreeOfKeyValuetype<K,V,Distantor>::minimumSpanningTreeOfKeyValuetype()
    : minimumSpanningTree<K,V,Distantor>() {

//     // create an instance of the parameters with the default values
//     parameters defaultParameters;
//     // set the default parameters
//     setParameters(defaultParameters);


  }

  // default constructor
  template<class K,class V,class Distantor>
  minimumSpanningTreeOfKeyValuetype<K,V,Distantor>::minimumSpanningTreeOfKeyValuetype(const parameters& par)
    : minimumSpanningTree<K,V,Distantor>() {

//     // set the given parameters
//     setParameters(par);
  }


  // copy constructor
  template<class K,class V,class Distantor>
  minimumSpanningTreeOfKeyValuetype<K,V,Distantor>::minimumSpanningTreeOfKeyValuetype(const minimumSpanningTreeOfKeyValuetype<K,V,Distantor>& other)
    : minimumSpanningTree<K,V,Distantor>() {
    copy(other);
  }

  // destructor
  template<class K,class V,class Distantor>
  minimumSpanningTreeOfKeyValuetype<K,V,Distantor>::~minimumSpanningTreeOfKeyValuetype() {
  }

  // returns the name of this type
  template<class K,class V,class Distantor>
  const char* minimumSpanningTreeOfKeyValuetype<K,V,Distantor>::getTypeName() const {
    return "minimumSpanningTreeOfKeyValuetype";
  }

  // copy member
  template<class K,class V,class Distantor>
  minimumSpanningTreeOfKeyValuetype<K,V,Distantor>&
  minimumSpanningTreeOfKeyValuetype<K,V,Distantor>::copy(const minimumSpanningTreeOfKeyValuetype<K,V,Distantor>& other) {
    minimumSpanningTree<K,V,Distantor>::copy(other);

    return (*this);
  }

  // alias for copy member
  template<class K,class V,class Distantor>
  minimumSpanningTreeOfKeyValuetype<K,V,Distantor>&
  minimumSpanningTreeOfKeyValuetype<K,V,Distantor>::operator=(const minimumSpanningTreeOfKeyValuetype<K,V,Distantor>& other) {
    return (copy(other));
  }


  // clone member
  template<class K,class V,class Distantor>
  functor* minimumSpanningTreeOfKeyValuetype<K,V,Distantor>::clone() const {
    return new minimumSpanningTreeOfKeyValuetype<K,V,Distantor>(*this);
  }

  // return parameters
  template<class K,class V,class Distantor>
  const typename minimumSpanningTreeOfKeyValuetype<K,V,Distantor>::parameters&
  minimumSpanningTreeOfKeyValuetype<K,V,Distantor>::getParameters() const {
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

  // TODO: apply methods still need to be made template methods.

  template<class K,class V,class Distantor>
  bool minimumSpanningTreeOfKeyValuetype<K,V,Distantor>::apply(
        const matrix<key_value_type>& src,const vector<V>& datas,
        weightedGraph<K,V,distance_type>& graph) const {
      std::vector<K> tmpKey;
      std::vector<V> tmpData;
      int i;
      bool ok;
      for (i=0; i<src.rows(); i++) {
        tmpKey.push_back(src.getRow(i));
        tmpData.push_back(datas.at(i));
      }
      ok = minimumSpanningTree<K,V,Distantor>::apply(tmpKey,tmpData,graph);
      return ok;
    };

}

