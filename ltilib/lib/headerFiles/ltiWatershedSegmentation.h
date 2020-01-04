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
 * file .......: ltiWatershedSegmentation.h
 * authors ....: Benjamin Winkler, Axel Berner
 * organization: LTI, RWTH Aachen
 * creation ...: 11.1.2001
 * revisions ..: $Id: ltiWatershedSegmentation.h,v 1.17 2005/06/16 12:35:44 doerfler Exp $
 */

#ifndef _LTI_WATERSHED_SEGMENTATION_H_
#define _LTI_WATERSHED_SEGMENTATION_H_

#include "ltiSegmentation.h"
#include "ltiPointList.h"
#include "ltiImage.h"

#include <queue>
#include <vector>     // lti::vector doesn't work here
#include <list>

namespace lti {

  /**
   * Watershed segmentation of a channel8
   *
   * Algorithms: 
   *
   * Luc Vincent and Pierre Soille.
   * "Watersheds in Digital Spaces: An Efficient Algorithm Based on Immersion
   *  Simulations".
   * IEEE transactions on pattern analysis and machine intelligence,
   * vol. 13, No. 6, June 1991, pp. 583-598
   *
   * and
   * 
   * Patrick De Smet and Rui Luis V.P.M. Pires
   * "Implementation and analysis of an optimized rainfalling watershed
   *  algorithm".
   * IS\&T\/SPIE's 12th Annual Symposium Electronic Imaging 2000,
   * January 2000, pp. 759-766
   *
   * Watershed segmentation is a morphological operator used to segment gray
   * valued images, based on viewing the gray image as a topographical map.
   * Valleys will be flooded with water until the water surpasses the sheds
   * separating them.  At the contact lines the "watershed lines" are created
   * and constitute the limits between the image regions.
   * 
   * The topological map is usually a gradient map (the magnitude of the
   * gradient) that can be obtained with the lti::orientationMap functor or
   * with the lti::colorContrastGradient, or directly using the
   * lti::gradientKernelX and lti::gradientKernelY and the lti::convolution
   * operator.
   *
   * Two kinds of apply() methods are provided: 
   *
   * - Two methods returning a channel8 will compute only two values:
   *   watershed or the catchment basin.  The specific values for each class
   *   are indicated in the parameters object.
   * 
   * - The apply() method returning a matrix<int> return a labeled mask, where
   *   each catchment basin get its own id.  This method is more typical for a
   *   segmentation algorithm.
   */
  class watershedSegmentation : public segmentation {
  public:
    /**
     * the parameters for the class watershedSegmentation
     */
    class parameters : public segmentation::parameters {
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
       * defines the neighborhood of a pixel.
       *
       * if set to false (default), only the pixels horizontally and
       * vertically bordering are considered to be neighbors.
       *
       * if set to true, the four diagonally adjoining pixels are also taken
       * into account.
       */
      bool neighborhood8;

      /**
       * gray value to be used for watersheds in the resulting channel8
       */
      int watershedValue;

      /**
       * gray value to be used for basins in the resulting channel8
       */
      int basinValue;

      /**
       * Rainfalling Watersheds or Standard-Waterfall
       *
       * Default: true (Rainfall-Watersheds, they are much faster)
       */
      bool rainfall;

      /**
       * Threshold to eliminate noise in the src-image.
       *
       * Default value: 0
       *
       * To avoid oversegmentation, try higher values (for example 4).
       * Another usual measure to reduce oversegmentation is also to denoise
       * the input image, using median-filters, SUSAN denoiser or mean-shift
       * denoisers, among many other possibilities.
       */
      ubyte threshold;
    };

    /**
     * default constructor
     */
    watershedSegmentation();

    /**
     * constructor with parameters
     */
    watershedSegmentation(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    watershedSegmentation(const watershedSegmentation& other);

    /**
     * destructor
     */
    virtual ~watershedSegmentation();

    /**
     * returns the name of this type ("watershedSegmentation")
     */
    virtual const char* getTypeName() const;

    /**
     * creates a watershed mask on the given channel8
     * @param srcdest channel8 with the source data.  This is usually the
     * gradient of an intensity image or a color contrast gradient.  The
     * resulting watershed lines will be left here too.
     * @result true if successful, false otherwise
     */
    bool apply(channel8& srcdest);

    /**
     * saves a watershed mask of src in dest
     * @param src channel8 with the source data.  This is usually the gradient
     *            of an intensity image or a color contrast gradient.
     * @param dest channel8 where the watershed lines will be written.
     */
    bool apply(const channel8& src,channel8& dest);

    /**
     * creates a region mask on the given matrix
     * only exact watersheds are marked 0, regions are numbered 1,2,...
     * @param src channel8 with the source data.  This is usually the gradient
     *            of an intensity image or a color contrast gradient.
     * @param result matrix<int> with the resulting region information, with
     *               a label per basin.
     */
    bool apply(const channel8& src, matrix<int>& result);

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    watershedSegmentation& copy(const watershedSegmentation& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  private:

    // for both --------------------------------------------------------
    /**
     * fill neighborhood pointlist according to parameter neighborhood8.
     * if set to true, the full 8-neighborhood will be generated
     * otherwise, only the 4 horizontally and vertically neighboring pixels
     * will be taken into account.
     */
    void createNeighborhood(const int colms,const bool neigh8);

   /**
     * converts the resulting matrix to the given channel8
     * by using watershedValue and basinValue
     */
    void copyMatrixToChannel8(const matrix<int>& src,
                                    channel8& dest);
    /**
     * list of neighborhood points (relative).  Is an 4 or 8 length vector
     * containing the offsets to get the neighbor pixels from the current one.
     */
    vector<int> neigh;

    /**
     * stores the size of the image in pixels
     */
    int imgSize;

    /**
     * border LUT contains 0 for all non-border pixel and 255 for
     * all border pixels.  It is used to accelerate detection of a border
     * position.
     */
    channel8 borderLUT;

    /**
     * return true if the given point corresponds to a valid neighbor point
     * of the given current point
     */
    inline bool invalidNeighbor(const int currentPoint,
                                const int currentNeighbor) const {
      return (currentNeighbor<0 || currentNeighbor>=imgSize ||
              ((borderLUT.at(currentPoint)!=0) && 
               (abs((currentPoint%borderLUT.columns()) - 
                    (currentNeighbor%borderLUT.columns())) > 1)));
    }

    /**
     * return true if the given point corresponds to a valid neighbor point
     * of the given current point
     */
    inline bool validNeighbor(const int currentPoint,
                              const int currentNeighbor) const {
      return (currentNeighbor>=0 && currentNeighbor<imgSize &&
              ((borderLUT.at(currentPoint)==0) || 
               (abs((currentPoint%borderLUT.columns()) - 
                    (currentNeighbor%borderLUT.columns())) <= 1)));
    }


  private:
    // rainfalling-------------------------------------------------------
    /**
     * find for all points(if there is) a neigh, which have a lower level
     */
    void findLowerNeigh(const channel8& src,
      matrix<int>& downPos,
      channel8& thresSrc8);

    /**
     * number serialy all minimas (lakes,or only points),
     */
    void markMinimas(const matrix<int>& downPos,
         const channel8& src,
         matrix<int>& dest);

    /**
     * look in which minima a raindrop would flow
     */
    void letsRain(const matrix<int>& downPos, matrix<int>& dest);

  private:
    /**
     * following type can be replaced for a faster list for small objects
     * in the near future
     */
    typedef std::list<int> list_type;

    // standard-----------------------------------------------------------

    /**
     * Initialize a border LUT to save time detecting if a pixel is in
     * the border.
     */
    void initBorderLUT(const point& size,
                       channel8& borderLUT) const;

    /**
     * creates a kind of histogram and stores all points, belonging to
     * a grayvalue in a list.
     *
     * It assumes that \a sortedPoints is empty!
     */
    void sortPixels(const channel8& src,
                    std::vector<list_type>& sortedPoints) const;
    
    /**
     * set all new pixel (caused by waterlevel raising) MASK
     */
    void maskCurrLevelPoints  (const list_type& currentPointList,
                               vector<int>& distance,
                               std::queue<int>& fifoQueue,
                               matrix<int>& result) const;
    /**
     * find out to which minima(lake) the MASK-pixel belog to
     */
    void assignCurrLevelPoints(vector<int>& distance,
                               std::queue<int>& fifoQueue,
                               matrix<int>& result) const;

    /**
     * define all pixel, which are not assigned to a minima, as a new minima
     */
    void checkForMins          (const list_type& currentPointList,
                                vector<int>& distance,
                                std::queue<int>& fifoQueue,
                                matrix<int>& result,
                                int& currentLabel) const;
    /**
     * raise the waterlevel, and look what happen
     */
    void raiseWaterLevel(const std::vector<list_type>& sortedPoints,
                         matrix<int>& result) const;

  };

}

#endif

