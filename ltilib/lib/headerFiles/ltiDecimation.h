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
 * file .......: ltiDecimation.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 6.12.2000
 * revisions ..: $Id: ltiDecimation.h,v 1.15 2005/06/16 09:32:42 doerfler Exp $
 */

#ifndef _LTI_DECIMATION_H_
#define _LTI_DECIMATION_H_


#include "ltiObject.h"
#include "ltiMatrix.h"
#include "ltiVector.h"
#include "ltiFilter.h"

namespace lti {
  /**
   * To decimate an image or matrix means to take each n-th pixel.  It
   * is much simpler than lti::downsampling, which also applies a filter
   * kernel before the decimation.
   * @see lti::upsampling, lti::filledUpsampling, lti::downsampling
   */
  class decimation : public filter {
  public:
    /**
     * the parameters for the class decimation
     */
    class parameters : public filter::parameters {
    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * destructor
       */
      ~parameters();

      /**
       * returns name of this type
       */
      const char* getTypeName() const;

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

#     ifdef _LTI_MSC_6
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead!
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead!
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const;
#     endif

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * the decimation factor
       * Only the pixels with coordinates x=n*factor.x and y=m*factor.y, with
       * n and m integers will be taken.  For vector decimation, only factor.x
       * will be considered.
       * the default value is (2,2)
       */
      point factor;
    };

    /**
     * default constructor
     */
    decimation();

    /**
     * constructor with a default parameters with the decimation factor
     * given by the point factor. (see lti::decimation::parameters::factor)
     */
    decimation(const point& factor);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    decimation(const decimation& other);

    /**
     * destructor
     */
    virtual ~decimation();

    /**
     * returns the name of this type ("decimation")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on the given %parameter.
     * @param srcdest matrix<T> with the source data.  The result
     *                 will be left here too.
     * @return true if successful, false otherwise.
     */
    template<class T>
    bool apply(matrix<T>& srcdest) const {
      // implementation must be here due to MS-VC++ bug
      matrix<T> tmp;
      if (apply(srcdest,tmp)) {
        tmp.detach(srcdest);
        return true;
      }
      return false;
    };

    /**
     * operates on the given %parameter.
     * @param srcdest vector<T> with the source data.  The result
     *                 will be left here too.
     * @return true if successful, false otherwise.
     */
    template<class T>
    bool apply(vector<T>& srcdest) const {
      // implementation must be here due to MS-VC++ bug
      vector<T> tmp;
      if (apply(srcdest,tmp)) {
        tmp.detach(srcdest);
        return true;
      }
      return false;
    };

    /**
     * operates on a copy of the given %parameters.
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     * @return true if successful, false otherwise.
     */
    template<class T>
    bool apply(const matrix<T>& src,matrix<T>& dest) const {
      // implementation must be here due to MS-VC++ bug
      point newSize;
      const parameters& param = getParameters();

      newSize.x = (src.columns()+(param.factor.x-1))/param.factor.x;
      newSize.y = (src.rows()+(param.factor.y-1))/param.factor.y;

      dest.resize(newSize,T(),false,false);
      int y,x,yy;
      typename vector<T>::iterator it,eit;

      for (y=0,yy=0;yy<dest.rows();y+=param.factor.y,++yy) {
        vector<T>& vct = dest.getRow(yy);
        for (x=0,it=vct.begin(),eit=vct.end();it!=eit;x+=param.factor.x,++it) {
          (*it)=src.at(y,x);
        }
      }

      return true;
    };

    /**
     * operates on a copy of the given %parameters.
     * @param src vector<T> with the source data.
     * @param dest vector<T> where the result will be left.
     * @return true if successful, false otherwise.
     */
    template<class T>
    bool apply(const vector<T>& src,vector<T>& dest) const {
      // implementation must be here due to MS-VC++ bug

      int newSize;
      const parameters& param = getParameters();

      newSize = (src.columns()+(param.factor.x-1))/param.factor.x;

      dest.resize(newSize,T(),false,false);
      int x;
      typename vector<T>::iterator it,eit;

      for (x=0,it=dest.begin(),eit=dest.end();it!=eit;x+=param.factor.x,++it) {
        (*it)=src.at(x);
      }

      return true;
    };

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    decimation& copy(const decimation& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;
  };
}

// due to MS-VC++ bug this file lost its value
//#include "ltiDecimation_template.h"

#endif

