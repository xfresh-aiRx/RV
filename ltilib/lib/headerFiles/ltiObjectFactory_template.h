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
 * file .......: ltiObjectFactory.cpp
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 07.11.2001
 * revisions ..: $Id: ltiObjectFactory_template.h,v 1.6 2005/08/02 09:16:54 haehnel Exp $
 */

//#include "ltiObjectFactory.h"

namespace lti {

  // --------------------------------------------------
  // objectFactory
  // --------------------------------------------------

  // default constructor
  template <class T>
  objectFactory<T>::objectFactory(const char** names,
                                  const T* const * objs)
    : object(){
    int i=0;

    lock.lock();
    while (names[i] != 0) {
      objMap[names[i]]=objs[i];
      i++;
    }
    lock.unlock();
  }

  template <class T>
  objectFactory<T>::objectFactory(const T* const* objs)
    : object(){
    int i=0;

    lock.lock();
    while (objs[i] != 0) {
      std::string tmp;
      cn.get(objs[i],tmp);
      objMap[tmp]=objs[i];
      i++;
    }
    lock.unlock();
  }

  // destructor
  template <class T>
  objectFactory<T>::~objectFactory() {
    lock.lock();
    /*
    classMap::iterator iter = objMap.begin();
    while(iter != objMap.end())
    {
      if(iter->second!=NULL)
        delete iter->second;
      ++iter;
    }
    */
    lock.unlock();
  }

  // returns the name of this type
  template <class T>
  const char*
  objectFactory<T>::getTypeName() const {
    return "objectFactory";
  }


  template <class T>
  T* objectFactory<T>::newInstance(const char *name) const {
    std::string tmpn(name);
    lock.lock();
    // since the method is const we can't use the [] operator
    // instead we try to find the name in the keys of the map
    // and check whether it was found.
    typename classMap::const_iterator it
      =objMap.find(tmpn);
    const T* tmp=(it==objMap.end() ? 0 : it->second);
    T* tmpReturn=0;
    if (tmp!=0) {
      tmpReturn=dynamic_cast<T*>(tmp->clone());
    }
    lock.unlock();
    return tmpReturn;
  }



}

