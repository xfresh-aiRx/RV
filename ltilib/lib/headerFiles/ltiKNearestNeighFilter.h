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
 * file .......: ltiKNearestNeighFilter.h
 * authors ....: Axel Berner
 * organization: LTI, RWTH Aachen
 * creation ...: 3.2.2002
 * revisions ..: $Id: ltiKNearestNeighFilter.h,v 1.11 2005/06/16 12:35:11 doerfler Exp $
 */

#ifndef _LTI_KNEARESTNEIGH_FILTER_H_
#define _LTI_KNEARESTNEIGH_FILTER_H_

#include "ltiFilter.h"

namespace lti {
  /**
   * A smoothness filter for label images (channel8,matrix<int>)
   * which uses the idea of the k-Nearest Neighbour concept.  This means,
   * this filter will assign to the origin position of the kernel mask, the
   * label with the most number of instances within the kernel region.
   * The filter removes disturbance pixels and smooths edges.
   *
   * In case that more than one label has the "maximal" number of
   * instances within the kernel mask, then the assigned label will
   * -# remain unchanged, if the kernel origin contains already one of the
   *    most frequent labels.
   * -# be the median of all "winner" labels will.
   *
   * The algorithms uses an optimized histogram actualization method, similar
   * to the one used for the median filter of channel8 (see also Sonka et.al,
   * Image Processing, Analysis, and Machine Vision, 2nd edition, 1998,
   * pp. 74-76
   */
  class kNearestNeighFilter : public filter {
  public:
    /**
     * the parameters for the class kNearestNeighFilter
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
       * read the parameters from the given ioHandler
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
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const;
#     endif

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * kernelSize defines the size of the square filter kernel
       *
       * Default value: 5
       */
      int kernelSize;
    };

    /**
     * default constructor
     */
    kNearestNeighFilter();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    kNearestNeighFilter(const kNearestNeighFilter& other);

    /**
     * destructor
     */
    virtual ~kNearestNeighFilter();

    /**
     * returns the name of this type ("kNearestNeighFilter")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on the given %parameter.
     * @param srcdest channel8 with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(channel8& srcdest);

    /**
     * operates on a copy of the given %parameters.
     * @param src channel8 with the source data.
     * @param dest channel8 where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& src,channel8& dest) ;

    /**
     * operates on the given %parameter.
     * @param srcdest channel8 with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(imatrix& srcdest);

    /**
     * operates on a copy of the given %parameters.
     * @param src channel8 with the source data.
     * @param dest channel8 where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const imatrix& src,imatrix& dest) ;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    kNearestNeighFilter& copy(const kNearestNeighFilter& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    kNearestNeighFilter& operator=(const kNearestNeighFilter& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  protected:
    /**
     * the corrected kernel size (always odd number)
     */
    int sizeOfKernel;

    /**
     * size of the histogram (= max. labelnumber from src-image)
     */
    int histoSize;

    /**
     * return the most avaible label in the given histogram
     */
    int getMostLabel(const ivector& histogram,
		     const imatrix& src,
		     const int& row, const int& col) const;

    /**
     * returns the median of all (max)histogram entries
     */
    int getMedian(const ivector& histogram,
		  const int max,
                  const int numOfMax) const;

    /**
     * runs inside the image src
     */
    void histogramMethodMiddle(const imatrix& src,imatrix& dest,
                               ivector& histogram,
                               const int& row, int& col) const;

    /**
     * applies the histogramMethod for the type boundary Zero
     */
    bool histogramMethodZero(const imatrix& src,
                             imatrix& dest) const;

    /**
     * applies the histogramMethod for the type boundary Constant
     */
    bool histogramMethodConstant(const imatrix& src,
                                 imatrix& dest) const;

    /**
     * applies the histogramMethod for the type boundary Periodic
     */
    bool histogramMethodPeriodic(const imatrix& src,
                                 imatrix& dest) const;

    /**
     * applies the histogramMethod for the type boundary Mirror
     */
    bool histogramMethodMirror(const imatrix& src,
                               imatrix& dest) const;

    /**
     * applies the histogramMethod for the type boundary NoBoundary
     */
    bool histogramMethodNoBoundary(const imatrix& src,
                                   imatrix& dest) const;

  };

}

#endif

