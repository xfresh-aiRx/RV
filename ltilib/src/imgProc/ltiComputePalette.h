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
 * file .......: ltiComputePalette.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 11.3.2002
 * revisions ..: $Id: ltiComputePalette.h,v 1.15 2005/06/16 09:32:42 doerfler Exp $
 */

#ifndef _LTI_COMPUTE_PALETTE_H_
#define _LTI_COMPUTE_PALETTE_H_

#include "ltiImage.h"
#include "ltiFunctor.h"

namespace lti {
  /**
   * This class is used to compute a palette given a color image and an
   * index mask.  Both objects must have the same size.
   * The size of the resulting palette will be the maximal value
   * in the index mask plus one.  The colors in the palette will be the average
   * colors in the mask.
   *
   * @see usePalette
   */
  class computePalette : public functor {
  public:
    /**
     * the parameters for the class computePalette
     */
    class parameters : public functor::parameters {
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

    };

    /**
     * default constructor
     */
    computePalette();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    computePalette(const computePalette& other);

    /**
     * destructor
     */
    virtual ~computePalette();

    /**
     * returns the name of this type ("computePalette")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on a copy of the given %parameters.
     * @param src image with the source data.
     * @param mask mask with palette entry indices.
     * @param dest computed palette.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,const channel8& mask, palette& dest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src image with the source data.
     * @param mask mask with palette entry indices.
     * @param dest computed palette.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,const matrix<int>& mask, palette& dest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src image with the source data.
     * @param mask mask with palette entry indices.
     * @param dest computed palette (mean value of all elements with the
     *                               same index).
     * @param palStdDev standard deviation for each palette entry.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,
               const channel8& mask,
               palette& dest,
               vector<trgbPixel<float> >& palStdDev) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src image with the source data.
     * @param mask mask with palette entry indices.
     * @param dest computed palette (mean value of all elements with the
     *                               same index).
     * @param palStdDev standard deviation for each palette entry.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,
               const matrix<int>& mask,
               palette& dest,
               vector<trgbPixel<float> >& palStdDev) const;

    /**
     * computes the mean values for each label present in the image
     * @param src image with the source data.
     * @param mask mask with palette entry indices.
     * @param means mean value of all elements with the same label.
     * @param n number of elements for each label.
     * @param numLabels this optional parameter can be used to save some time
     *                  if you give the number of labels in the mask, or to
     *                  be more specific the greatest label in mask + 1.  If
     *                  ommited, the mask.maximum() will be called to find out
     *                  how many elements the vectors n and means must have,
     *                  which costs one iteration on the whole mask.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,
               const matrix<int>& mask,
               vector<trgbPixel<float> >& means,
               ivector& n,
               const int numLabels = -1) const;


    /**
     * operates on a copy of the given %parameters.
     * @param src image with the source data.
     * @param mask mask with palette entry indices.
     * @param dest computed palette (mean value of all elements with the
     *                               same index).
     * @param palStdDev standard deviation for each palette entry.
     * @param n number of pixels found for each entry
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,
               const channel8& mask,
               palette& dest,
               vector<trgbPixel<float> >& palStdDev,
               vector<int>& n) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src image with the source data.
     * @param mask mask with palette entry indices.
     * @param dest computed palette (mean value of all elements with the
     *                               same index).
     * @param palStdDev standard deviation for each palette entry.
     * @param n number of pixels found for each entry
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,
               const matrix<int>& mask,
               palette& dest,
               vector<trgbPixel<float> >& palStdDev,
               vector<int>& n) const;

    /**
     * this apply will generate multivariate statistics for each
     * entry in the palette, at it takes more time than the other
     * ones.
     * @param src image with the source data.
     * @param mask mask with palette entry indices.
     * @param dest computed palette (mean value of all elements with the
     *                               same index).  Note that this apply
     *                               will compute with floats)
     * @param palCovar covariance matrices for each palette entry.
     * @param n number of pixels found for each entry
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,
               const matrix<int>& mask,
               vector<trgbPixel<float> >& dest,
               std::vector<matrix<float> >& palCovar,
               vector<int>& n) const;

    /**
     * this apply will generate multivariate statistics for each
     * entry in the palette.  It takes more time than the other
     * ones.  The image will be assumed to be composed by the given three
     * channels.
     * @param ch1 first channel of the image
     * @param ch2 second channel of the image
     * @param ch3 third channel of the image
     * @param mask mask with palette entry indices.
     * @param dest computed palette (mean value of all elements with the
     *                               same index).  Note that this apply
     *                               will compute with floats)
     * @param palCovar covariance matrices for each palette entry.
     * @param n number of pixels found for each entry
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& ch1,
               const channel& ch2,
               const channel& ch3,
               const matrix<int>& mask,
               vector<trgbPixel<float> >& dest,
               std::vector<matrix<float> >& palCovar,
               vector<int>& n) const;

    /**
     * this apply will generate multivariate statistics for each
     * entry in the palette.  It takes more time than the other
     * ones.  The image will be assumed to be composed by the given three
     * channels.
     * @param ch1 first channel of the image
     * @param ch2 second channel of the image
     * @param ch3 third channel of the image
     * @param mask mask with palette entry indices.
     * @param dest computed palette (mean value of all elements with the
     *                               same index).  Note that this apply
     *                               will compute with floats)
     * @param n number of pixels found for each entry
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& ch1,
               const channel& ch2,
               const channel& ch3,
               const matrix<int>& mask,
               vector<trgbPixel<float> >& dest,
               vector<int>& n) const;

    /**
     * This apply method computes the mean values and variances of the
     * regions indicated by the mask for the given channel.
     *
     * @param ch channel for which the region statistics will be computed
     * @param mask labeled mask indicating the regions
     * @param means mean values for each region
     * @param vars variances for each region
     * @param n number of pixels per region
     * @return true if successful, false otherwise.
     */
    bool apply(const channel& ch,
               const matrix<int>& mask,
               vector<float>& means,
               vector<float>& vars,
               vector<int>& n) const;


    /**
     * compute the mean value and covariance matrix for just one
     * label in the mask.
     *
     * @param src image with the source data.
     * @param mask mask with palette entry indices.
     * @param label label to be analyzed.
     * @param mean mean color of the given label.
     * @param covar covariance matrix for the given label.
     * @param n number of pixels found with the given label.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,
               const matrix<int>& mask,
               const int label,
               trgbPixel<float>& mean,
               matrix<float>& covar,
               int& n) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    computePalette& copy(const computePalette& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    computePalette& operator=(const computePalette& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  private:
    /**
     * outer product of two colors
     */
    void outer(const trgbPixel<float>& a,
               const trgbPixel<float>& b,
                     matrix<double>& mat) const;
  };
}

#endif

