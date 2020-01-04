/*
 * Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003, 2004
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
 * file .......: ltiHistogramRGBL.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 14.5.2001
 * revisions ..: $Id: ltiHistogramRGBL.h,v 1.10 2005/06/16 12:35:10 doerfler Exp $
 */

#ifndef _LTI_HISTOGRAM_R_G_B_L_H_
#define _LTI_HISTOGRAM_R_G_B_L_H_

#include "ltiImage.h"
#include "ltiVector.h"
#include "ltiLinearKernels.h"
#include "ltiGlobalFeatureExtractor.h"

namespace lti {
  /**
   * This simple feature consists on four independently calculated histograms,
   * one for each color component R, G and B and the luminance L, defined as
   * L = (min(R,G,B) + max(R,G,B))/2.  Each histogram contains the number of
   * cells given in the parameters, which means that the resulting
   * feature vector has a dimensionality of <code>4*cells</code>.
   */
  class histogramRGBL : public globalFeatureExtractor {
  public:
    /**
     * the parameters for the class histogramRGBL
     */
    class parameters : public globalFeatureExtractor::parameters {
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

      //TODO: comment the parameters of your functor
      // If you add more parameters manually, do not forget to do following:
      // 1. indicate in the default constructor the default values
      // 2. make sure that the copy member also copy your new parameters
      // 3. make sure that the read and write members also read and
      //    write your parameters


      /**
       * If true, the resulting feature vector will be smothed using the given
       * filter kernel.
       * Default value: true
       */
      bool smooth;

      /**
       * smoothing kernel.
       * Default: gaussian kernel of size 3 (default variance).
       */
      kernel1D<double> kernel;

      /**
       * If true, the area under the each single histogram will be normalized
       * to one; i.e. the total area will be four.
       * Default value: true
       */
      bool normalize;

      /**
       * Number of cells per single color histogram.
       * The dimensionality of the resulting feature vector will be four times
       * this value.
       * Default value: 32 (i.e. 128 dimensions for the final feature vector)
       */
      int cells;

      /**
       * if true, the ignoreValue will be ignored and all data values will
       * be considered in the histogram generation.  If false (the default)
       * all but the pixels with value <code>ignoreValue</code> will be
       * considered.
       * Default value: false
       */
      bool considerAllData;

      /**
       * this value will be ignored in the histogram.  It is usually used
       * to ignore the background.
       * Default value (0,0,0), i.e. black will be ignored.
       */
      rgbPixel ignoreValue;
    };

    /**
     * default constructor
     */
    histogramRGBL();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    histogramRGBL(const histogramRGBL& other);

    /**
     * destructor
     */
    virtual ~histogramRGBL();

    /**
     * returns the name of this type ("histogramRGBL")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on a copy of the given %parameters.
     * @param src image with the source data.
     * @param dest feature vector
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,vector<double>& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    histogramRGBL& copy(const histogramRGBL& other);

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

#endif

