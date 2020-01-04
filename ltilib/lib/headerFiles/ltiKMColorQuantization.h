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
 * file .......: ltiKMColorQuantization.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 30.04.99
 * revisions ..: $Id: ltiKMColorQuantization.h,v 1.19 2005/06/16 12:35:11 doerfler Exp $
 */

#ifndef _LTI_KM_COLOR_QUANTIZATION_H_
#define _LTI_KM_COLOR_QUANTIZATION_H_

#include "ltiFunctor.h"
#include "ltiImage.h"
#include "ltiTypes.h"
#include "ltiVector.h"
#include "ltiColorQuantization.h"
#include "ltiUniformDist.h"
#include <map>

namespace lti {
  /**
   * k-Means based color quantization.
   *
   * This functor calculates (using k-Means) an optimal color subpalette for
   * the input image.  The maximal number of colors to be used is given
   * through the parameters (inherited from colorQuantization::parameters).
   *
   * If the real number of colors used in the image is less than the desired
   * number of quantized colors, no quantization is done and the output palette
   * will contain all image colors, i.e. it will be smaller that the expected
   * size of parameters::numberOfColors.
   */
  class kMColorQuantization : public colorQuantization {
  public:

    /**
     * the parameters for the class kMColorQuantization
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

      /**
       * maximal number of iteration for the k-Means algorithm
       * (Default value: 50)
       */
      int maximalNumberOfIterations;

      /**
       * if "Palette-Changing" by iteration < thresholdDeltaPalette
       * than stop iterating
       * sum(distanceSqr(palNew-palOld))
       * (Default value: 0.2)
       */
      float thresholdDeltaPalette;

    };

    /**
     * default constructor
     */
    kMColorQuantization();

    /**
     * default constructor
     */
    kMColorQuantization(const parameters& par);

    /**
     * copy constructor
     */
    kMColorQuantization(const kMColorQuantization& other);

    /**
     * destructor
     */
    virtual ~kMColorQuantization();

    /**
     * returns current parameters.
     */
    const parameters& getParameters() const;

    /**
     * copy data of "other" functor.
     */
    kMColorQuantization& copy(const kMColorQuantization& other);

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual functor* clone() const;

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const;

    /**
     * @param src original image with the true-color data
     * @param dest matrix<int> where the indexes of the calculated palette
     *             will be.
     * @param thePalette the color palette used by the matrix. If it is not
     *             empty, it will be taken as a InitPalette for
     *             the computation of the new one.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,matrix<int>& dest,palette& thePalette) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src original image with the true-color data
     * @param dest channel8 where the indexes of the also calculated palette
     *             will be.
     * @param thePalette the color palette used by the channel. If it is not
     *             empty, it will be taken as a InitPalette for
     *             the computation of the new one.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,channel8& dest,palette& thePalette) const;

    /**
     * operates on the given %parameter.
     * @param srcdest image with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(image& srcdest) const;

    /**
     * operates on the given %parameter.
     * @param src image with the source data.
     * @param dest image with only the number of colors specified in
     *             the parameters
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,image& dest) const;

  private:
    /**
     * functor to calculate the kMeans
     */
    class kMeanColor {
    public:
      /**
       * constructor
       */
      kMeanColor(const int& maxNumOfClasses,
		 const int& maxIterations,
		 const float& thresdDeltaPal);

      /**
       * destructor
       */
      virtual ~kMeanColor();

      /**
       * calculate palette and colorMap using k-Means.
       * img is the input image, and it will be modified to use
       * the quantized colors!
       */
      bool operator() (const image& src,matrix<int>& dest,palette& thePalette);

    protected:
      /**
       * an entry of the hash table contains the number of pixels in
       * the image that have the color corresponding to this entry (counter)
       * and the index of this color in the cetroid-vector;
       */
      struct hashEntry {
        hashEntry(const int& idx=0,const int& cnt=0)
          : index(idx),counter(cnt) {};
        int index;
        int counter;
      };

      /**
       * each entry of the hash has this type
       */
      typedef std::map<int,hashEntry> hashMapType;

      /**
       * hash table type (see theHash for more details).
       */
      typedef hashMapType* hashType;

      /**
       * the centroids
       */
      vector<trgbPixel<float> > centroids;

      /**
       * centroid elements
       */
      vector<int> centerElems;

      /**
       * The hash table.
       *
       * The hash table contains all relevant information of the image to be
       * quantized, necessary for the k-Mean algorithms.
       *
       * It is organized as an array of 4096 (12 bits) hashMapType elements.
       *
       * This array is then accessed with the lower 12 bits of the color
       * value.  The upper 12 bits of the color are used to index the
       * proper map, that returns a hashEntry structure, with the correponding
       * centroid index for this color and the number of pixels assigned
       * to the centroid until now.
       *
       * In real world images, there will be an average between 5 and 10
       * elements per map.
       *
       * To access theHash directly, you can use the method at(), or put()
       */
      hashType theHash;

      /**
       * the maximal number of classes
       */
      const int maxNumberOfClasses;

      /**
       * the number of classes really used in the image
       */
      int realNumberOfClasses;

      /**
       * maximum number of iterations for the k-Means
       */
      const int maxNumberOfIterations;

      /**
       * if "Palette-Changing" by iteration < thresholdDeltaPalette
       * than stop iterating
       * sum(distanceSqr(palNew-palOld))
       */
      const float thresholdDeltaPalette;

      /**
       * returns a reference to the hash entry for the given pixel
       */
      inline hashEntry& at(const rgbPixel& px);

      /**
       * put the given pixel in the hash table (increments the counter if
       * already exists).
       * @return true if the pixel is new added, false otherwise.
       */
      inline bool put(const rgbPixel& px);

      /**
       * create and initialize the hash table with the image values
       */
      void initialize(const image& src);

      /**
       * get initial palette from hash
       */
      void getInitialPalette(const lti::palette& thePalette);

      /**
       * iterate to find the clusters
       */
      void iterate();

      /**
       * get a random color from the color hash
       */
      rgbPixel getAnImageColor();

      /**
       * size of the maps array
       */
      static const int firstKeySize;

      /**
       * a counter to generate the random colors
       */
      int lastHashPosition;

      /**
       * Random number generator
       */
      uniformDistribution uni;
    };

  };

}

#endif
