/*
 * Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003
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
 * file .......: ltiChrominanceMapMasking.h
 * authors ....: Claudia Goenner
 * organization: LTI, RWTH Aachen
 * creation ...: 14.2.2004
 * revisions ..: $Id: ltiChrominanceMapMasking.h,v 1.3 2005/06/16 09:32:41 doerfler Exp $
 */

#ifndef _LTI_CHROMINANCE_MAP_MASKING_H_
#define _LTI_CHROMINANCE_MAP_MASKING_H_

#include "ltiImage.h"
#include "ltiModifier.h"
#include "cstdlib"

namespace lti {
  /**
   * This class performs color segmentation in the chrominance plane using
   * a chrominance map which may be drawn manually or computed by the
   * chrominance map estimator class.
   *
   * For achromatic objects a threshold may be specifies to distinguish
   * between black, grey and white.
   *
   * @see overlappingSets2D, chrominanceModelEstimator, chrominanceMapEstimator
   */
  class chrominanceMapMasking : public modifier {
  public:
    /**
     * The parameters for the class chrominanceMapMasking
     */
    class parameters : public modifier::parameters {
    public:
      /**
       * Default constructor
       */
      parameters();

      /**
       * Copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * Destructor
       */
      ~parameters();

      /**
       * Returns name of this type
       */
      const char* getTypeName() const;

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);


      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const;

      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * Read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

#     ifdef _LTI_MSC_6
      /**
       * This function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * This function is required by MSVC only, as a workaround for a
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
       * Achromatic objects which a luminance below this value are interpreted
       * as black.
       */
      ubyte blackLuminance;

      /**
       * Achromatic objects which a luminance above this value are interpreted
       * as white.
       */
      ubyte whiteLuminance;

      /**
       * The label of the achromatic region in the chrominance map.
       */
      int achromaticLabel;

      /**
       * The label of white objects in the destination.
       */
      int whiteLabel;

      /**
       * The label of grey objects in the destination.
       */
      int greyLabel;
      
      /**
       * The label of black objects in the destination.
       */
      int blackLabel;

      /**
       * Each color band usually contains 8bit resulting in a 256x256
       * histogram. For efficiency the histograms may be downsampled by
       * 2 power shrinkFactor.
       *
       * Default: 0.
       */
      int shrinkFactor;

    };

    /**
     * Default constructor
     */
    chrominanceMapMasking();

    /**
     * Construct a functor using the given parameters
     */
    chrominanceMapMasking(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    chrominanceMapMasking(const chrominanceMapMasking& other);

    /**
     * Destructor
     */
    virtual ~chrominanceMapMasking();

    /**
     * Returns the name of this type ("chrominanceMapMasking")
     */
    virtual const char* getTypeName() const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    chrominanceMapMasking& copy(const chrominanceMapMasking& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    chrominanceMapMasking& operator=(const chrominanceMapMasking& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * @name Apply methods for entire images or channel8s.
     */
    //@{

    /**
     * Color segmentation based on a chrominance map and thresholding on the
     * luminance for achromatic objects.
     * @param y channel8 with the luminance.
     * @param u channel8 with the 1st chrominance band.
     * @param v channel8 with the 2nd chrominance ban.
     * @param dest color segmentation.
     * @return true on success and false otherwise.
     */
    bool apply(const channel8& y, const channel8& u, const channel8& v,
	       channel8& dest);

    /**
     * Color segmentation based on a chrominance map and thresholding on the
     * luminance for achromatic objects.
     * @param y channel8 with the luminance.
     * @param u channel8 with the 1st chrominance band.
     * @param v channel8 with the 2nd chrominance ban.
     * @param dest color segmentation.
     * @param prob the probability of the segmented object.
     * @return true on success and false otherwise.
     */
    bool apply(const channel8& y, const channel8& u, const channel8& v,
	       channel8& dest, channel& prob);

    /**
     * Color segmentation based on a chrominance map only.
     * @param u channel8 with the 1st chrominance band.
     * @param v channel8 with the 2nd chrominance ban.
     * @param dest color segmentation.
     * @return true on success and false otherwise.
     */
    bool apply(const channel8& u, const channel8& v, channel8& dest);

    /**
     * Color segmentation based on a chrominance map only.
     * @param u channel8 with the 1st chrominance band.
     * @param v channel8 with the 2nd chrominance ban.
     * @param dest color segmentation.
     * @param prob the probability of the segmented object.
     * @return true on success and false otherwise.
     */
    bool apply(const channel8& u, const channel8& v,
	       channel8& dest, channel& prob);
    //@}  

    /**
     * Set a link to the chrominance map. Please ensure that the
     * chrominance map is not deleted while this class or pointer exists.
     * @param map the chrominance map.
     * @return true on success and false otherwise.
     */
    bool useExternalChrominanceMap(const channel8* map);

    /**
     * Set the chrominance map. The map is copied.
     * @param map the chrominance map.
     * @return true on success and false otherwise.
     */
    bool setChrominanceMap(const channel8& map);
    
    /**
     * Set a link to the probability map. Please ensure that the
     * probability is not deleted while this class or pointer exists.
     * @param map the probability map.
     * @return true on success and false otherwise.
     */
    bool useExternalProbabilityMap(const channel* map);

    /**
     * Set the probability map. The map is copied.
     * @param map the probability map.
     * @return true on success and false otherwise.
     */   
    bool setProbabilityMap(const channel& map);
    
    /**
     * Get the chrominance map.
     */   
    const channel8& getChrominanceMap() const;
    
    /**
     * Get the probability map for the most likely object.
     */   
    const channel& getProbabilityMap() const;
    
    /**
     * Is the chrominance map valid, e.g. is is set and does it have equal
     * dimensions?
     * @return true on success and false otherwise.
     */ 
    bool isChrominanceMapValid() const;
    
    /**
     * Is the probability map valid, e.g. is is set and does it have equal
     * dimensions?
     * @return true on success and false otherwise.
     */ 
    bool isProbabilityMapValid() const;
    
    /**
     * Are the chrominance map and probability map consistent, e.g. have
     * the same dimensions and are both valid?
     * @return true on success and false otherwise.
     */ 
    bool areMapsConsistent() const;
    
    /**
     * Read the chrominance map from disk. The chrominance map is interpreted
     * as a color image.
     * @param filename the name of the chrominance map file.
     * @param colors  conversion between the colored stored map and the labels
     *  used by vision processing.
     * @return true on success and false otherwise.
     */
    bool readChrominanceMap(const std::string& filename,
			    const std::vector<rgbPixel>& colors);
    
  protected:

    /**
     * Check if the input channel8 have the same dimension.
     * @param y channel8 with the luminance.
     * @param u channel8 with the 1st chrominance band.
     * @param v channel8 with the 2nd chrominance ban.
     * @return true on success and false otherwise.
     */
    bool checkInput(const channel8& y,
		    const channel8& u, const channel8& v);
    
    /**
     * Check if the input channel8 have the same dimension.
     * @param u channel8 with the 1st chrominance band.
     * @param v channel8 with the 2nd chrominance ban.
     * @return true on success and false otherwise.
     */
    bool checkInput(const channel8& u, const channel8& v);

    /**
     * Get the shift used because the chrominance map is downsampled.
     * @return the shift
     */
    int getShift() const;

    /**
     * Set the shift used because the chrominance map is downsampled.
     */
    void setShift();  

    /**
     * Remember if the probability map must be destroyed on destruction
     * or re-initialization.
     */
    bool m_destroyProbability;

    /**
     * Remember if the chrominance map must be destroyed on destruction
     * or re-initialization.
     */
    bool m_destroyChrominance;

    /**
     * the probability map
     */
    const channel *m_probabilityMap;
    
    /**
     * the chrominance map
     */
    const channel8 *m_chrominanceMap;
    
    /**
     * scaling between the size of the chrominance map and 8-bit
     */
    int m_shift;
  };
}

#endif

