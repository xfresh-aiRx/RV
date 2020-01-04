/*
 * Copyright (C) 1998, 1999, 2000, 2001
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 *
 * This file can be used with the LTI-Computer Vision Library (LTI-Lib)
 * (please note this file is NOT part of the LTI-Lib)
 * This code is based on the original C code by S. Smith, which contains
 * following header:
 *
 **********************************************************************
 *
 * SUSAN Version 2l by Stephen Smith
 *
 * Oxford Centre for Functional Magnetic Resonance Imaging of the
 * Brain, Department of Clinical Neurology, Oxford University, Oxford,
 * UK (Previously in Computer Vision and Image Processing Group - now
 * Computer Vision and Electro Optics Group - DERA Chertsey, UK)
 *
 * Email:    steve@fmrib.ox.ac.uk
 * WWW:      http://www.fmrib.ox.ac.uk/~steve
 *
 * (C) Crown Copyright (1995-1999), Defence Evaluation and Research Agency,
 * Farnborough, Hampshire, GU14 6TD, UK
 * DERA WWW site:
 * http://www.dera.gov.uk/
 * DERA Computer Vision and Electro Optics Group WWW site:
 * http://www.dera.gov.uk/imageprocessing/dera/group_home.html
 * DERA Computer Vision and Electro Optics Group point of contact:
 * Dr. John Savage, jtsavage@dera.gov.uk, +44 1344 633203
 *
 * A UK patent has been granted: "Method for digitally processing
 * images to determine the position of edges and/or corners therein for
 * guidance of unmanned vehicle", UK Patent 2272285. Proprietor:
 * Secretary of State for Defence, UK. 15 January 1997
 *
 * This code is issued for research purposes only and remains the
 * property of the UK Secretary of State for Defence. This code must
 * not be passed on without this header information being kept
 * intact. This code must not be sold.
 **********************************************************************
 *
 *
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
 *
 */

/*----------------------------------------------------------------
 * project ....: LTI Digital Image/Signal Processing Library
 * file .......: ltiSusanDenoise.h
 * authors ....: Christian Harte
 * organization: LTI, RWTH Aachen
 * creation ...: 18.4.2000
 * revisions ..: ltiSusanDenoise.h,v $
 */

/**
 * \file ltiSusanDenoise.h Susan Denoiser: denoise a channel of a
 * picture using the susan denoise algorithm.  in most cases only the
 * intensity channel needs to be denoised. But if only one channel is
 * affected by the noise, that one can be processed on its own.
 */

#include "ltiConfig.h"

#ifndef HAVE_SUSAN
#ifdef _LTI_MSC_VER
#pragma message("Configuration Error:")
#pragma message("Change in src/basics/ltiWinConfig.h HAVE_SUSAN to 1")
#else
#warning "Configuration Error:"
#warning "Change in src/basics/config.h HAVE_SUSAN to 1"
#endif
#endif

#ifndef _LTI_SUSAN_DENOISE_H_
#define _LTI_SUSAN_DENOISE_H_

#include "ltiImage.h"
#include "ltiModifier.h"

namespace lti {
  /**
   * The susan denoiser enhances pictures by removing the noise from
   * any colour channel. Most times it gives good results in just
   * denoising the intensity channel. It might (in very few cases) be
   * better to denoise every channel if the noise strongly differs
   * between them.
   *
   * \code
   *   lti::susanDenoise denoiser;  // the susan denoiser
   *   lti::susanDenoise::parameters param;
   *
   *   param.kernelSize=9;
   *   param.threshold=12;
   *
   *   denoiser.setParameters(param);
   *
   *   denoiser.apply(chnl_in,chnl_out);
   * \endcode
   *
   * The parameter kernelSize accepts two values: 9 and 37. It
   * specifies the number of pixels around the center to use to
   * calculate a value for the new pixel.  The large value (37)
   * results in very good pictures and the small one (9) makes the
   * algorithm about 10 times faster. Giving another value will result
   * in kernelSize become 37.
   *
   * To denoise a single frame the denoiser must be told the maximum
   * value of the noise (i.e. +/- 9 gray values). The greater this
   * value the more blury the picture becomes. To determine the
   * optimal value it is recomended to examine a light area of a
   * picture: Adjust the threshold until the noise of this area
   * disappears and then don't raise it any more.
   *
   * There are four ways to call the denoiser:
   *
   * \code
   *   denoiser.apply(channel8 chnl,channel8 chnl2);
   *   denoiser.apply(channel8 chnl2);
   *   denoiser.apply(channel chnl,channel chnl2);
   *   denoiser.apply(channel chnl2);
   * \endcode
   *
   * The first call ist the fastest one. If it is possible use this method.
   *
   */
  class susanDenoise : public modifier {
  public:
    /**
     * the parameters for the class susanDenoise
     */
    class parameters : public modifier::parameters {
    public:
      /**
       *  default constructor
       */
      parameters();

      /**
       *  copy constructor
       */
      parameters(const parameters& other);

      /**
       *  destructor
       */
      ~parameters();

      /**
       *  returns name of this type
       */
      const char* getTypeName() const;

      /**
       *  copy member
       */
      parameters& copy(const parameters& other);

      /**
       *  returns a pointer to a clone of the parameters
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
       * the kernel size determines the quality of the noise reduction
       * and the speed of the algorithm.  Use 9 for fast filtering (but not
       * a very high quality) or 37 for better quality (about 10 slower).
       * The default value is 37
       */
      int kernelSize;

      /**
       * Default value 9
       * a value of 9.0 seems to be a good value not to smooth the
       * picture to much, but to eliminate all the noise of the
       * picture. This value has to be changed if a new image source is
       * used.  The noisier a picture is the larger this value must be.
       */
      double threshold;
    };

    /**
     *  default constructor
     */
    susanDenoise();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    susanDenoise(const susanDenoise& other);

    /**
     *  destructor
     */
    virtual ~susanDenoise();

    /**
     *  returns the name of this type ("susanDenoise")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on the given parameter.
     * @param srcdest channel8 with the source data.  The result
     *                will be left here too.
     * @return true if ok, false otherwise.
     */
    bool apply(channel8& srcdest) const;

    /**
     * operates on the given parameter.
     * @param srcdest channel with the source data.  The result
     * will be left here too.
     * @return true if ok, false otherwise.
     */
    bool apply(channel& srcdest) const;

    /**
     * operates on a copy of the given parameters.  This ist the
     * fastest implementation of the susan denoiser.  All other
     * apply-functions call this one. It is strongly recommended that
     * you use this "apply" for maximum speed!
     *
     * @param src channel8 with the source data.
     * @param dest channel8 where the result will be left.
     * @return true if ok, false otherwise.
     */
    bool apply(const channel8& src,channel8& dest) const;

    /**
     * operates on a copy of the given parameters.
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @return true if ok, false otherwise.
     */
    bool apply(const channel& src,channel& dest) const;

    /**
     *  copy data of "other" functor.
     */
    susanDenoise& copy(const susanDenoise& other);

    /**
     *  returns a pointer to a clone of the functor.
     */
    virtual functor* clone() const;

    /**
     *  returns used parameters
     */
    const parameters& getParameters() const;

  protected:
    //try which one produces best results (concerning quality and speed):
    typedef int TOTAL_TYPE;
    //typedef float TOTAL_TYPE;

    /**
     * This function is called by susan_smoothing. Don't call it
     * yourself.
     * @param bp pointer to the brightnes table
     * @param thresh brightnis threshold
     * @param form form of the evaluation function
     */
    static void setupBrightnessLUT(lti::ubyte* bp,int thresh,int form);

    /**
     * @param threeByThree flag for speed mode: if threeByThree is 1
     * the smaller kernel (size 9) ist taken, else this value must be 0.
     * @param in pointer to the ubyte array of the input image channel
     * @param out pointer to the ubyte array of the output image channel
     * @param dt distance threshold - only used if threeByThree is 0.
     *           It defines the size of the USAN.
     * @param xSize x-resolution (columns) of the frame
     * @param ySize y-resolution (rows) of the frame
     * @param bp pointer to the brightnes table generated by
     *           setupBrightnessLUT
     */
    static void susanSmoothing(int threeByThree,
                               lti::ubyte* in,
                               lti::ubyte* &out,
                               float dt,
                               int xSize,
                               int ySize,
                               lti::ubyte* bp);
    /**
     * create a border around the image data pointed by "in" using
     * tmpImage as temporary buffer.  The pointer "in" and the xSize and
     * ySize variables will be updated
     */
    static int enlarge(lti::ubyte*& in,
                       lti::ubyte* tmp_image,
                       int& xSize,int& ySize,
                       const int& border);

    static ubyte median(lti::ubyte* in,int i,int j,int xSize);



  };
}

#endif

