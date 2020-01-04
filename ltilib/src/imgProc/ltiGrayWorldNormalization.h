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
 * file .......: ltiGrayWorldNormalization.h
 * authors ....: Suat Akyol,Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 8.11.2000
 * revisions ..: $Id: ltiGrayWorldNormalization.h,v 1.10 2005/06/16 12:35:10 doerfler Exp $
 */

#ifndef _LTI_GRAY_WORLD_NORMALIZATION_H_
#define _LTI_GRAY_WORLD_NORMALIZATION_H_

#include "ltiImage.h"
#include "ltiColorNormalizationBase.h"

namespace lti {
  /**
   * Performs a color normalization on an lti::image using a gray
   * world approach, in order to eliminate effects of illumination
   * colour.
   *
   * Gray world methods and white world methods assume that some low order
   * statistics on the colors of a real world scene lie on the gray line of
   * a color space.  The most simple approach uses only the mean color and
   * maps it linearly into one specific point on the gray line, assuming
   * that a diagonal transform (a simple channel scaling) is valid.  
   *
   * Normalization is done under the assumptiomn of mondrian worlds,
   * constant point light sources and delta function camera sensors.
   *
   * For each color channel \f$c\in{r,g,b}\f$, the normalization will be
   * \f[
   * c_n \rightarrow \frac{c}
   * {(1-\gamma) + \gamma(\nu \cdot c_\mu + \kappa \cdot c_\sigma)}
   *\f]
   *
   * With:
   * - \f$\gamma\f$: normalization strength between 0 (no normalization) and
   *                 1 (complete normalization).
   * - \f$c_\mu\f$:  mean value of the \e c channel.
   * - \f$c_\sigma\f$:  standard deviation of the \e c channel.
   * - \f$\nu\f$:    mean value scaling factor.
   * - \f$\kappa\f$: standard deviation scaling factor.
   *
   * Initial theory: Diploma Thesis, Dipl.-Ing. Jose Pablo Alvarado Moya, 1998
   * Dept. of Technical Computer Science, RWTH Aachen, Germany.
   *
   * \ingroup gColor
   */
  class grayWorldNormalization : public colorNormalizationBase {
  public:
    /**
     * The parameters for the class grayWorldNormalization.
     * 
     * This class provides mainly the parameters to implement the
     * normalization equation:
     *
     * \f[
     * c_n \rightarrow \frac{c}
     * {(1-\gamma) + \gamma(\nu \cdot c_\mu + \kappa \cdot c_\sigma)}
     *\f]
     *
     * With:
     * - \f$c_\mu\f$:  mean value of the \e c channel.
     * - \f$c_\sigma\f$:  standard deviation of the \e c channel.
     * - \f$\gamma\f$: normalization strength between 0 (no normalization) and
     *                 1 (complete normalization) (gamma)
     * - \f$\nu\f$:    mean value scaling factor  (nu).
     * - \f$\kappa\f$: standard deviation scaling factor (kappa).
     */
    class parameters : public colorNormalizationBase::parameters {
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
       * @name Computation of normalization constant.
       */
      //@{

      /**
       * Scaling factor for the standard deviation.
       *
       * Default = 3.0
       */
      float kappa;

      /**
       * Scaling factor for the channel mean.
       *
       * Default value: 1
       */
      float nu;

      /**
       * Some times a "total" normalization will damage the color information
       * in a way, that this cannot be used in recognition processes.  In order
       * to avoid to strong normalizations, this gamma factor can be used.
       * Its value MUST be between 0 and 1.  0 means no normalization at all
       * and 1 the best normalization to be achieved.
       *
       * Default value: 1
       */
      float gamma;

      //@}

      /**
       * @name General options
       */
      //@{
      /**
       * Flag to indicate if a specific color should be ignored (true)
       * or if the whole image should be considered to determine the
       * normalization factors (false).  Please note that this flag
       * will have effect if and only if the localNormalization is
       * deactivated!
       *
       * Default value: false
       */
      bool ignoreColorFlag;

      /**
       * This color will be ignored in the computations of the norm, if
       * the ignoreColorFlag is true.  This way, the background can be ignored
       * if it was "markt" with a specific color (usually black).
       *
       * Default value: rgbPixel(0,0,0) (Black)
       */
      rgbPixel ignoreColor;
      
      /**
       * default is false. Set this to true, if you want to allow
       * overflow values in the normalized channels. Otherwise over-
       * and underflows will be set to the allowed max resp. min
       * values.
       */
      bool allowOverflow;
      //@}
      
      /**
       * @name Local normalization
       */
      //@{
      /**
       * if true, the normalization constants are computed for each pixel
       * using the given window size.  If false, only one normalization
       * constant is used for the whole image.
       *
       * Default: false
       */
      bool localNormalization;

      /**
       * Window size specify the neighbourhood to be considered in the
       * computations of the normalization factors.  This element has
       * effect only if <code>localInvariance</code> is true.
       *
       * Default value: 64;
       */
      int windowSize;
      //@}

    };

    /**
     * default constructor
     */
    grayWorldNormalization();

    /**
     * default constructor
     */
    grayWorldNormalization(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    grayWorldNormalization(const grayWorldNormalization& other);

    /**
     * destructor
     */
    virtual ~grayWorldNormalization();

    /**
     * returns the name of this type ("grayWorldNormalization")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on the given %parameter.
     * @param srcdest image with the source data.
     * The result will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(image& srcdest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src image with the source data.
     * @param dest image where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const image& src,image& dest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param srcR red channel of the source data
     * @param srcG green channel of the source data
     * @param srcB blue channel of the source data
     * @param dest image where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const channel& srcR,
               const channel& srcG,
               const channel& srcB,
               image& dest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src image with the source data.
     * @param chR red component of the normalized input image.
     * @param chG green component of the normalized input image.
     * @param chB blue component of the normalized input image.
     * @return true if successful, false otherwise.
     */
    bool apply(const image& src,
               channel& chR,
               channel& chG,
               channel& chB) const;

    /**
     * operates on a copy of the given %parameters.
     * @param srcR red channel of the source data
     * @param srcG green channel of the source data
     * @param srcB blue channel of the source data
     * @param chR red component of the normalized input image.
     * @param chG green component of the normalized input image.
     * @param chB blue component of the normalized input image.
     * @return true if successful, false otherwise.
     */
    bool apply(const channel& srcR,
               const channel& srcG,
               const channel& srcB,
               channel& chR,
               channel& chG,
               channel& chB) const;

    /**
     * operates on the given %parameter.
     * @param srcdestR red channel of the source and destination.
     * @param srcdestG green channel of the source and destination.
     * @param srcdestB blue channel of the source and destination.
     * @return true if successful, false otherwise.
     */
    bool apply(channel& srcdestR,
               channel& srcdestG,
               channel& srcdestB) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    grayWorldNormalization& copy(const grayWorldNormalization& other);

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
     * ensure that the value x lies between a and b
     */
    inline ubyte inChannel8Range(const float& x) const {
      return (x<=0.0f) ? 0: ((x>=255.0f) ? 255 : static_cast<ubyte>(x));
    }

    /**
     * ensure that the value x lies between a and b
     */
    inline float inChannelRange(const float& x) const {
      return (x<0.0f) ? 0.0f : ((x>1.0f) ? 1.0f : x);
    }

    /**
     * for the global method, compute the norm
     */
    void computeNorm(const channel& red,
                     const channel& green,
                     const channel& blue,
                     trgbPixel<float>& norm) const;

    /**
     * for the global method, compute the norm
     */
    void computeNorm(const image& img,
                     trgbPixel<float>& norm) const;

    /**
     * for the local method, compute the norm
     */
    void computeNorm(const channel& red,
                     const channel& green,
                     const channel& blue,
                     channel& normRed,
                     channel& normGreen,
                     channel& normBlue) const;

    /**
     * compute the norm for one channel
     */
    void computeNorm(const channel& chnl,
                           channel& norm) const;

    /**
     * normalize avoiding overflows
     */
    void normalizeWithoutOverflow(const channel& input,
                                  const float& norm,
                                  channel& output) const;

    /**
     * normalize image
     */
    void normalize(image& img, const trgbPixel<float>& norm) const;

    /**
     * normalize image
     */
    void normalize(const image& img,
                   const trgbPixel<float>& norm,
                         image& dest) const;


  };
}

#endif

