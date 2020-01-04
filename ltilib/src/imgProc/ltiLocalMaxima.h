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


/*--------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiLocalMaxima.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 1.11.2002
 * revisions ..: $Id: ltiLocalMaxima.h,v 1.14 2005/06/16 12:35:11 doerfler Exp $
 */

#ifndef _LTI_LOCAL_MAXIMA_H_
#define _LTI_LOCAL_MAXIMA_H_

#include "ltiMaximumFilter.h"
#include "ltiMatrix.h"
#include "ltiPointList.h"
#include <list>

namespace lti {
  /**
   * This simple functor tries to find the local maxima at a given window
   * and suppresses all other values, setting them the "nonMaxValue" given
   * in the parameters (usually 0).
   *
   * The template parameter T defines the type of the matrix or vector
   * to be filtered.
   *
   * An instance of this class cannot be used from different threads
   * (not thread-save).  If you have multiple threads, use simply one
   * functor instance for each thread.
   *
   * This class is used for example in the extraction of the maxima in
   * the Hough Transform.
   *
   * A very similar functor with more use in edge-detection applications is
   * the lti::nonMaximaSuppression, which is used for example in the Canny
   * edge detection.  
   *
   * @see parameters
   * @see lti::nonMaximaSuppression
   */
  template<class T>
  class localMaxima : public maximumFilter<T> {
  public:
    /**
     * the parameters for the class localMaxima
     */
    class parameters : public maximumFilter<T>::parameters {
    public:
      /**
       * default constructor
       */
      parameters() : maximumFilter<T>::parameters() {
        relativeMinimum = float(0.0f);
        noMaxValue = ubyte(0);
        hystheresisThreshold = -1.0f;
	maxNumber = -1;
      };

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other) : maximumFilter<T>::parameters() {
        copy(other);
      };

      /**
       * destructor
       */
      ~parameters() {
      };

      /**
       * returns name of this type
       */
      const char* getTypeName() const {
        return "localMaxima::parameters";
      };

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other) {
#     ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        maximumFilter<T>::parameters::copy(other);
#     else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        maximumFilter<T>::parameters& (maximumFilter<T>::parameters::* p_copy)
          (const maximumFilter<T>::parameters&) =
          maximumFilter<T>::parameters::copy;
        (this->*p_copy)(other);
#    endif

        relativeMinimum = other.relativeMinimum;
        noMaxValue = other.noMaxValue;
        hystheresisThreshold = other.hystheresisThreshold;
	maxNumber = other.maxNumber;
	return *this;
      };

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other) {
        return copy(other);
      };

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const {
        return new parameters(*this);
      };

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be
       *        written.
       * @return true if write was successful
       */
#     ifndef _LTI_MSC_6
      virtual bool write(ioHandler& handler,const bool complete=true) const
#     else
      bool writeMS(ioHandler& handler,const bool complete=true) const
#     endif
      {
        bool b = true;
        if (complete) {
          b = handler.writeBegin();
        }

        if (b) {
          lti::write(handler,"relativeMinimum",relativeMinimum);
          lti::write(handler,"noMaxValue",noMaxValue);
          lti::write(handler,"hystheresisThreshold",hystheresisThreshold);
	  lti::write(handler,"maxNumber",maxNumber);
		    
        }

#     ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not able to
        // compile...
        b = b && maximumFilter<T>::parameters::write(handler,false);
#     else
        bool (maximumFilter<T>::parameters::* p_writeMS)(ioHandler&,
                                                         const bool) const =
          maximumFilter<T>::parameters::writeMS;
        b = b && (this->*p_writeMS)(handler,false);
#     endif

        if (complete) {
          b = b && handler.writeEnd();
        }

        return b;
      }

#     ifdef _LTI_MSC_6
      virtual bool write(ioHandler& handler,
                         const bool complete = true) const {
        // ...we need this workaround to cope with another really
        // awful MSVC bug.
        return writeMS(handler,complete);
      }
#     endif


      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also read, otherwise only the data block will be read.
       * @return true if write was successful
       */
#     ifndef _LTI_MSC_6
      virtual bool read(ioHandler& handler,const bool complete = true)
#     else
      bool readMS(ioHandler& handler,const bool complete=true)
#     endif
      {
        bool b = true;
        if (complete) {
          b = handler.readBegin();
        }

        if (b) {
          lti::read(handler,"relativeMinimum",relativeMinimum);
          lti::read(handler,"noMaxValue",noMaxValue);
          lti::read(handler,"hystheresisThreshold",hystheresisThreshold);
	  lti::read(handler,"maxNumber",maxNumber);  
	}

#     ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not able to
        // compile...
        b = b && maximumFilter<T>::parameters::read(handler,false);
#     else
        bool (maximumFilter<T>::parameters::* p_readMS)(ioHandler&,
                                                        const bool) =
          maximumFilter<T>::parameters::readMS;
        b = b && (this->*p_readMS)(handler,false);
#     endif

        if (complete) {
          b = b && handler.readEnd();
        }

        return b;
      }

#     ifdef _LTI_MSC_6
      virtual bool read(ioHandler& handler,const bool complete=true) {
        // ...we need this workaround to cope with another really awful MSVC
        // bug.
        return readMS(handler,complete);
      }
#      endif

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * All pixels with values smaller than the maximum found in the
       * input channel multiplied by "relativeMinimum" will be
       * suppressed.  (Zero means, all detected local minima will be kept)
       *
       * Default value: 0.0f;
       */
      float relativeMinimum;

      /**
       * All non-maximum values will be set to this value.  All maxima will
       * keep their value.
       *
       * Default value: 0
       */
      T noMaxValue;

      /**
       * This value offers a more stronger suppression of non-maxima.
       * If a local maxima is detected in a given window (defined by
       * the attribute maximumFilter<T>::parameters::kernelSize) a
       * region search algorithm will look for pixels around with a value
       * greater than the local maxima found, the search will consider
       * for the expansion only pixels with a value greater than the
       * found local maxima multiplied by this hystheresis value.
       *
       * The seach will be done for each local maxima, starting with the
       * smallest ones.
       *
       * To activate this region-search mode, use a value between 0 (meaning
       * "search for a global maximum") and 1 ("detect all local minima").
       *
       * A negative value or a value greater 1 deactivates this search
       * (saves some time).
       *
       * Default value: -1 (search deactivated)
       */
      float hystheresisThreshold;

      /**
       * Maximum number of local maxima to be kept.
       *
       * If you are only interested in a fixed number of maxima, the set
       * this attribute to a value larger or equal 1.  If you want to
       * always obtaine the maximum number of local maxima, set this value
       * to -1.
       * 
       * Default value: -1 (not used)
       */
      int maxNumber;
    };

    /**
     * default constructor
     */
    localMaxima();

    /**
     * Construct a functor using the given parameters
     */
    localMaxima(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    localMaxima(const localMaxima& other);

    /**
     * destructor
     */
    virtual ~localMaxima();

    /**
     * returns the name of this type ("localMaxima")
     */
    virtual const char* getTypeName() const;

    /**
     * Extract the local maxima from srcdest and leave the result there too,
     * consisting on the maxima values in their respective positions and
     * in the rest of the image the value indicated in parameters::noMaxValue.
     *
     * @param srcdest matrix<T> with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(matrix<T>& srcdest);

    /**
     * Extract the local maxima from srcdest and leave the result there too,
     * consisting on the maxima values in their respective positions and
     * in the rest of the image the value indicated in parameters::noMaxValue.
     *
     * @param srcdest vector<T> with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(vector<T>& srcdest);

    /**
     * Extract the local maxima from src and leave the result in dest,
     * consisting on the maxima values in their respective positions and
     * in the rest of the image the value indicated in parameters::noMaxValue.
     *
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<T>& src,matrix<T>& dest);

    /**
     * Extract the local maxima from src and leave the result in dest,
     * consisting on the maxima values in their respective positions
     * and in the rest of the image the value indicated in
     * parameters::noMaxValue.  Those positions with local maxima are
     * also stored in the \a theMaxs list.
     *
     * operates on a copy of the given %parameters.
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     * @param theMaxs list of points where the local maxima are found.
     *                It is sorted in decreasing order of the maxima values
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<T>& src,matrix<T>& dest,pointList& theMaxs);

    /**
     * Extract the local maxima from src and leave their position in the
     * given \a theMaxs point-list.
     *
     * @param src matrix<T> with the source data.
     * @param theMaxs list of points where the local maxima are found.
     *                It is sorted in decreasing order of the maxima values
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<T>& src,pointList& theMaxs);

    /**
     * Extract the local maxima from src and leave the result in dest,
     * consisting on the maxima values in their respective positions and
     * in the rest of the image the value indicated in parameters::noMaxValue.
     *
     * @param src vector<T> with the source data.
     * @param dest vector<T> where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const vector<T>& src,vector<T>& dest);

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    localMaxima& copy(const localMaxima& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    localMaxima& operator=(const localMaxima& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * set functor's parameters.
     * This member makes a copy of <em>theParam</em>: the functor
     * will keep its own copy of the parameters!
     * @return true if successful, false otherwise
     */
    virtual bool updateParameters();

  protected:
    /**
     * operates on a copy of the given %parameters.
     * @param src vector<T> with the source data.
     * @param maxRes vector<T> with the source data, where is has been
     *               filtered to contain the max of columns.
     * @param dest vector<T> where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const vector<T>& src,const vector<T>& maxRes,
               vector<T>& dest);

    /**
     * this function evaluates if the given srcVal can be a local maximum
     * depending on the content of the buffer array (in the parent class).
     */
    inline T getValue(const T srcVal) const;

    /**
     * value used to indicate a "suppressed" value (usually 0)
     */
    T suppressValue;

    /**
     * minimumT is the smallest number that type T can describe
     */
    static const T minimumT;


    /**
     * Suppression with hystheresis.
     *
     * Given all local maxima in localMax, this method sorts them
     * and beginning with the smallest ones, it starts a region
     * search through all pixels with a value greater than the local
     * maximum being evaluated multiplied with the parameter
     * hystheresisThreshold.  All other local maxima found in the region
     * will be absorved if they are smaller, or they eliminate the actual
     * local maximum if they are greater.
     *
     * @param src the source image
     * @param localMax local maxima found.  It may contain at the end
     *                 of the algorithm less maxima as in the beginning.
     *                 Also the minimumT value will be replaced by
     *                 suppressValue.
     * @param maxList list of local maxima sorted in descending order.
     */
    bool hystheresisSuppression(const matrix<T>& src,
                                      matrix<T>& localMax,
                                      pointList& maxList) const;

  };
}

#endif


