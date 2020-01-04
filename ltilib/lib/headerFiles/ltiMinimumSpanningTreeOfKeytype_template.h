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
 * file .......: ltiMinimumSpanningTreeOfKeytype_template.h
 * authors ....: Jens Paustenbach
 * organization: LTI, RWTH Aachen
 * creation ...: 8.5.2003
 * revisions ..: $Id: ltiMinimumSpanningTreeOfKeytype_template.h,v 1.5 2004/05/03 12:20:02 alvarado Exp $
 */

namespace lti {
  // --------------------------------------------------
  // minimumSpanningTreeOfKeytype
  // --------------------------------------------------

  // default constructor
  template<class K,class V,class Distantor>
  minimumSpanningTreeOfKeytype<K,V,Distantor>::minimumSpanningTreeOfKeytype()
    : minimumSpanningTree<K,V,Distantor>(){

//     //TODO: comment the attributes of your functor
//     // If you add more attributes manually, do not forget to do following:
//     // 1. indicate in the default constructor the default values
//     // 2. make sure that the copy member also copy your new attributes, or
//     //    to ensure there, that these attributes are properly initialized.

//     // create an instance of the parameters with the default values
//     parameters defaultParameters;
//     // set the default parameters
//     setParameters(defaultParameters);


  }

  // default constructor
  template<class K,class V,class Distantor>
  minimumSpanningTreeOfKeytype<K,V,Distantor>::minimumSpanningTreeOfKeytype(const parameters& par)
    : minimumSpanningTree<K,V,Distantor>() {

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    // set the given parameters
//     setParameters(par);
  }


  // copy constructor
  template<class K,class V,class Distantor>
  minimumSpanningTreeOfKeytype<K,V,Distantor>::minimumSpanningTreeOfKeytype(const minimumSpanningTreeOfKeytype<K,V,Distantor>& other)
    : minimumSpanningTree<K,V,Distantor>() {
    copy(other);
  }

  // destructor
  template<class K,class V,class Distantor>
  minimumSpanningTreeOfKeytype<K,V,Distantor>::~minimumSpanningTreeOfKeytype() {
  }

  // returns the name of this type
  template<class K,class V,class Distantor>
  const char* minimumSpanningTreeOfKeytype<K,V,Distantor>::getTypeName() const {
    return "minimumSpanningTreeOfKeytype<K,V,Distantor>";
  }

  // copy member
  template<class K,class V,class Distantor>
  minimumSpanningTreeOfKeytype<K,V,Distantor>&
  minimumSpanningTreeOfKeytype<K,V,Distantor>::
  copy(const minimumSpanningTreeOfKeytype<K,V,Distantor>& other) {
      minimumSpanningTree<K,V,Distantor>::copy(other);

    return (*this);
  }

  // alias for copy member
  template<class K,class V,class Distantor>
  minimumSpanningTreeOfKeytype<K,V,Distantor>&
  minimumSpanningTreeOfKeytype<K,V,Distantor>::operator=(const minimumSpanningTreeOfKeytype<K,V,Distantor>& other) {
    return (copy(other));
  }


  // clone member
  template<class K,class V,class Distantor>
  functor* minimumSpanningTreeOfKeytype<K,V,Distantor>::clone() const {
    return new minimumSpanningTreeOfKeytype<K,V,Distantor>(*this);
  }

  // return parameters
  template<class K,class V,class Distantor>
  const typename minimumSpanningTreeOfKeytype<K,V,Distantor>::parameters&
  minimumSpanningTreeOfKeytype<K,V,Distantor>::getParameters() const {
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

  template<class K,class V,class Distantor>
  bool minimumSpanningTreeOfKeytype<K,V,Distantor>::
  apply(const matrix<K>& src,
        const matrix<V>& datas,
        weightedGraph<K,V,distance_type>& graph) const {
    std::vector<K> tmpKey;
    std::vector<V> tmpData;
    int nbColumns=src.columns();
    int i,j;
    for (i=0; i<src.rows(); i++) {
      for (j=0; j<src.columns(); j++) {
        tmpKey.push_back(src.at(i,j));
        tmpData.push_back(datas.at(i,j));
      }
    }
    return minimumSpanningTree<K,V,Distantor>::apply(tmpKey,tmpData,graph);
  };
  
}

