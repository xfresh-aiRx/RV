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
 * file .......: ltiMedianCut.h
 * authors ....: Norman Pfeil
 * organization: LTI, RWTH Aachen
 * creation ...: 18.5.2001
 * revisions ..: $Id: ltiMedianCut.h,v 1.16 2005/06/16 12:35:42 doerfler Exp $
 */

#ifndef _LTI_MEDIAN_CUT_H_
#define _LTI_MEDIAN_CUT_H_

#include "ltiHistogram.h"
#include "ltiTypes.h"
#include "ltiColorQuantization.h"
#include "ltiMath.h"

#include <list>

namespace lti {

  /**
   * This class does color quantization with median-cut-algorithm
   * (Heckbert, MIT 1980)
   *
   * Description of the algorithm:
   * http://www-lehre.informatik.uni-osnabrueck.de/~cg/2000/skript/10_7_Erzeugung_einer.html
   */
  class medianCut : public  colorQuantization {
  public:
    /**
     * the parameters for the class medianCut
     */
    class parameters : public colorQuantization::parameters {
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
      // If you add more parameters manually, do not forget to do following:
      // 1. indicate in the default constructor the default values
      // 2. make sure that the copy member also copy your new parameters
      // 3. make sure that the read and write members also read and
      //    write your parameters


      /**
       * preQuant: number of colors per dimension for pre-Quantization
       * for example: 32 means ==> 0-31 for r,g,b ==> 32^3 = 32768 colors
       *
       * Range: 1-256
       *
       * Default: 32
       *
       * It is recommended not to change this value.
       */
      int preQuant;

    };

    /**
     * default constructor
     */
    medianCut();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    medianCut(const medianCut& other);

    /**
     * destructor
     */
    virtual ~medianCut();

    /**
     * returns the name of this type ("medianCut")
     */
    virtual const char* getTypeName() const;

    /**
     * This apply method calculates a channel8, containing the indices of the
     * quantized colors. Will deliver empty channel8, if more than 256 colors!!
     *
     * @param src image with the source data.
     * @param dest channel8 where the result will be left.
     * @return true if successful, false otherwise
     */
    bool apply(const image &src, channel8 &dest) const;


    /**
     * This apply method calculates a channel8, containing the indices of the
     * quantized colors AND the palette. Will deliver empty channel8, if more
     * than 256 colors!! But palette will be O.K.
     *
     * @param src image with the source data.
     * @param dest channel8 where the result will be left.
     * @param thePalette palette appendant to dest.
     * @return true if successful, false otherwise
     */
    bool apply(const image &src,
               channel8 &dest,
               palette &thePalette) const;

    /**
     * This apply method calculates a quantized image (on place)
     *
     * @param srcdest image with the source data. The result will be left here too.
     * @return true if successful, false otherwise
     */
    bool apply(image& srcdest) const;

    /**
     * This apply method calculates a quantized image (on copy)
     *
     * @param src image with the source data.
     * @param dest image where the result will be left.
     * @return true if successful, false otherwise
     */
    bool apply(const image& src,image& dest) const;

    /**
     * This apply method calculates a a quantized image and respective palette.
     *
     * @param src image with the source data.
     * @param dest color quantized image
     * @param thePalette color palette used
     */
    bool apply(const image& src,
               image& dest,
               palette& thePalette) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    medianCut& copy(const medianCut& other);

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
     * The function, where quantization takes place!
     *
     * @param src image with the source data.
     * @param dest quantized image.
     * @param mask channel8 with labeled regions (will only be set if < 256 colors).
     * @param thePalette the quantized palette.
     * @return true if successful, false otherwise
     */
    bool performQuantization(const image& src,
                             image& dest,
                             channel8& mask,
                             palette &thePalette) const;

    /**
     * struct for holding box information
     */
    struct boxInfo {
      /**
       * constructor
       */
      boxInfo() {
        colorFrequency=0;
      }

      /**
       * give corners of box
       */
      boxInfo(const lti::rgbPixel& minVal,
              const lti::rgbPixel& maxVal) {
        min=minVal;
        max=maxVal;
      }

      /**
       * first corner with lowest coordinates in RGB
       */
      lti::rgbPixel min;

      /**
       * second corner with highest coordinates in RGB
       */
      lti::rgbPixel max;

      /**
       * how many pixels are in the box
       */
      long int colorFrequency;

      /**
       * mean value
       */
      double mean[3];
      /**
       * variance
       */
      double var[3];

      /**
       * number of colors (?)
       */
      int colors;
    };

    /**
     * boxInfo.min and .max must be specified, when using this function, then
     * missing information in boxInfo is computed (mean, var, colorFrequency,
     * colors) and box boundaries (min,max) are set to the smallest size, that
     * still encloses all entries in the specified range of the histogram.
     */
    void computeBoxInfo(const histogram& hist,
                              boxInfo& theBox) const;
  };


}

#endif

