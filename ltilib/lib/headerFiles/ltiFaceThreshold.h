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
 * file .......: ltiFaceThreshold.h
 * authors ....: Thorsten Dick
 * organization: LTI, RWTH Aachen
 * creation ...: 17.12.2003
 * revisions ..: $Id: ltiFaceThreshold.h,v 1.11 2005/06/16 09:32:42 doerfler Exp $
 */

#ifndef _LTI_FACE_THRESHOLD_H_
#define _LTI_FACE_THRESHOLD_H_

#include "ltiImage.h"
#include "ltiConvolution.h"
#include "ltiGaussKernels.h"
#include "ltiPolygonPoints.h"
#include "ltiGeometricFeatures.h"
#include "ltiFastObjectsFromMask.h"

#include <iostream>
#include "ltiFunctor.h"

namespace lti {

 /** 
  * The ltiFaceThreshold-class calculates for a given skin probability mask of
  * an image an extraction-threshold that allows to 'cut-out' the face (blob) 
  * of the shown person.
  * Depending on the apply()-method that is being called, the resulting 
  * face-blob is also returned. <BR>
  * In order to do so, a blob-ranking is performed, which can be done in two
  * ways:
  * <ul>
  *   <li> linear mode: every n-th threshold is tested
  *   <li> time-optimized mode: binary search like threshold testing
  * </ul>
  * For every regarded threshold, all blobs (with size greater than a 
  * specified minimum) are extracted and then ranked considering the 
  * following criteria:
  * <ol>
  *   <li> the compactness
  *   <li> the main axis orientation
  *   <li> the relation of the the bounding box's edges' lengths
  *   <li> the horizontal position in the image
  *   <li> the vertical position in the image
  *   <li> the size (relative to image size)
  *   <li> the skin-probability
  * </ol>
  * The result is the threshold that causes the blob with the highest score. 
  * For a detailed description of the functions that benchmark the above 
  * mentioned criteria see the individual parameters 
  * (lti::faceThreshold::parameters) that determine computation.
  */

  class faceThreshold : public functor {
  public:

    // ############################### parameters ############################

    /**
     * The parameters for the class faceThreshold. <BR>
     * Note: for reasons of simplicity, in the blob ranking parameters 
     * formulas, all parameters (i.e.not the arguments) are written without 
     * the method-related prefix that makes them unique in the code. For 
     * example, both 'compactnessExponent' and 'mainAxisOrientationExponent'
     * are just named 'exponent'.(Since, after all, one can conclude the full
     * name by the methods's name...)
     */
    class parameters : public functor::parameters {
    public:

      /**
       * Constants for the possible values of 
       * lti::faceThreshold::parameters::computationMode
       * <ul>
       *   <li> linearMode = 0
       *   <li> timeOptimizedMode = 1
       * </ul>
       */
      enum OperationMode { 
        linearMode = 0, 
        timeOptimizedMode = 1,
      };
      
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
       * @return true if read was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

# ifdef _LTI_MSC_6     
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
# endif

      // ---------------------------------------------------------------------
      // the parameters: computation mode parameters
      // ---------------------------------------------------------------------

      /**
       * Specification of computation mode:
       * <ul>
       *   <li> 0 (alias linearMode) <BR>
       *        Linear threshold search mode, i.e.every 
       *        lti::faceThreshold::parameters::thresholdStep-th threshold 
       *        will be tested.<BR>
       *        Note: the parameter 
       *        lti::faceThreshold::parameters::optModePrecision has no effect
       *        in this mode.
       *   <li> 1 (alias timeOptimizedMode) <BR>
       *        Binary-search like mode, i.e.in a first iteration every 16-th
       *        threshold is  tested. The two best ones determine the region 
       *        for the next iteration and so on. The iteration is aborted, if
       *        the difference between the two best thresholds is less than 
       *        lti::faceThreshold::parameters::optModePrecision. <BR>
       *        Note: the parameter 
       *        lti::faceThreshold::parameters::thresholdStep has no effect
       *        in this mode.
       * </ul>
       * See lti::faceThreshold::parameters::OperationMode for a list of 
       * predefined mode-constants. <BR>
       * Default: 0
       */
      int computationMode;

      /** 
       * Except for time-optimized mode (see 
       * lti::faceThreshold::parameters::computationMode), this parameter 
       * specifies the distance between two tested thresholds for the linear 
       * threshold search. <BR>
       * Thus, a greater value results in a faster but more inexact blob 
       * ranking. <BR>
       * Range: [1..127], Default: 5
       */
      int thresholdStep;

      /** 
       * Besides interpolation (see 
       * lti::faceThreshold::parameters::interpolationStep), smoothing 
       * provides better results for a blob's compactness value, since the 
       * blob's 'fringes' are reduced. Possible values of this parameter:
       * <ul>
       *   <li> <= 1  : smoothing disabled
       *   <li> 2 <= n: smoothing of the skin probability mask with kernel of
       *                size n
       * </ul>
       * Note: in case the input image has not yet been smoothed, a kernel
       * size of 19 has shown good results.
       * <BR>
       * Default: 0
       */
      int gaussKernelSize;

      /**
       * To reduce a blob's 'fringes', its border points can be interpolated,
       * where this parameter determines the distance between two unchanged 
       * border points.
       * Possible values:
       * <ul>
       *   <li> <= 0: interpolation disabled
       *   <li> 0 < n < 100: interpolation with the distance between two 
       *                     unchanged border points being n\% of the maximum
       *                     of the image's height and width
       * </ul>
       * Thus, with increasing value, the roughness of the resulting polygon
       * blob's outline increases, too. <BR>
       * Default: 1.0
       */
      float interpolationStep;

      /**
       * If the time-optimized threshold search mode is used (see 
       * lti::faceThreshold::parameters::computationMode), this
       * parameter specifies, at which precision to stop the threshold search
       * (where a greater value results in a faster computation but more 
       * inexact threshold). <BR>
       * Range: [1..15], Default: 2
       */
      int optModePrecision;

      // ---------------------------------------------------------------------
      // the parameters: blob ranking parameters
      // ---------------------------------------------------------------------

      /** 
       * Blob ranking parameter: score for a blob's compactness. 
       * <BR>
       * Compactness is a measure for a blob's area-to-circumference ratio and
       * ranges from 0 to 1, where 1 is the compactness of a circle. Thus, 
       * higher compactness values result in higher scores. The corresponding
       * score formula is:
       * \f[
       *    \text{compactness} (c) := 100 - 100 \cdot (1-c)^{\text{exponent}}
       * \f]
       * Effect of this parameter: the greater, the more scores are achieved 
       * with relative small compactness values. 
       * <BR>
       * Range: >=0, Default: 2, Disable: 0
       */
      int compactnessExponent;

      /** 
       * Blob ranking parameter: score for a blob's main axis orientation.
       * <BR>
       * The more vertical a blob's main axis is, the more score is achieved.
       * The corresponding score formula is:
       * \f[
       *    \text{axisOrientation} (a) := 100 \cdot \left\arrowvert
       *    \dfrac{a}{90} \right\arrowvert^{\text{exponent}}
       * \f]
       * Effect of this parameter: the greater, the stricter the ranking. 
       * <BR>
       * Range: >=0, Default: 2, Disable: 0
       */
      int mainAxisOrientationExponent;

      /** 
       * Blob ranking parameter: score for a blob's ratio of border points 
       * distances relative to the main axis. 
       * <BR>
       * The sum of the leftmost and rightmost blob points distances to the
       * main axis (i.e.orthogonal to the main axis) is set in relation to
       * the sum of the frontmost and rearmost blob points distances to the 
       * blob's center of gravity (i.e.along the main axis). This ratio is
       * compared to the specified desired ratio 
       * 1 : lti::faceThreshold::parameters::dRatioOptRatio. The closer these
       * two values are, the more score is achieved. This results in the
       * rating of a main axis dependent bounding box's sides proportion.
       * The corresponding formula is:
       * \f[
       *    \text{dRatio} (\Sigma_{\,ortho},\Sigma_{\,along}) := 100 - 100 
       *     \cdot \left( \text{width} \cdot
       *    \left\arrowvert \dfrac{\Sigma_{\,ortho}}{\Sigma_{\,along}} - 
       *    \dfrac{1}{\text{optRatio}} \right\arrowvert
       *    \right)^{\text{exponent}}
       * \f]
       * Effect of this parameter: the greater, the more binary is the ranking
       * (i.e.in extreme cases almost equal scores for tolerable blobs and a
       * sharp switch to penalty scores), but without changing the null 
       * (i.e.the sign itself of the score remains unchanged). The tolerable
       * deviation is determined by 
       * lti::faceThreshold::parameters::dRatioWidth. 
       * <BR>
       * Range: >=0, Default: 2, Disable: 0
       */
      int dRatioExponent;

      /** 
       * Blob ranking parameter: score for a blob's ratio of border points
       * distances relative to the main axis (i.e.a kind of main axis
       * dependent bounding box rating). For formula and more detailed
       * description see lti::faceThreshold::parameters::dRatioExponent. 
       * <BR>
       * Effect of this parameter: specification of the desired bounding box
       * edges relation, e.g. a value of 1 would specify a quadrat and a
       * value of 2 a rectangle with edges parallel to main axis twice as
       * long as the edges orthogonal to it. 
       * <BR>
       * Range: >=1, Default: 1.5 (to disable this criterion, set 
       * lti::faceThreshold::parameters::dRatioExponent to 0)
       */
      float dRatioOptRatio;

      /** 
       * Blob ranking parameter: score for a blob's ratio of border points
       * distances relative to the main axis (i.e.a kind of main axis
       * dependent bounding box rating). For formula and more detailed
       * description see lti::faceThreshold::parameters::dRatioExponent . 
       * <BR>
       * Effect of this parameter: determines the width of the region for
       * positive scores around the ratio that reaches maximum score. Note
       * that a smaller value results in a wider region, i.e.a more generous
       * ranking. 
       * <BR>
       * Range: >=1, Default: 2 (to disable this criterion, set 
       * lti::faceThreshold::parameters::dRatioExponent to 0)
       */
      int dRatioWidth;

      /** 
       * Blob ranking parameter: score for a blob's horizontal position.<BR>
       * The closer a blob's center of gravity is to a specified horizontal
       * position (see lti::faceThreshold::parameters::hDistExpectedPosition),
       * the more score is reached. The corresponding formula is:
       * \f[
       *    \text{hDist} (x, cols) :=  100 - 100 \cdot \left( \text{width}
       *    \cdot \dfrac { | x - \frac{cols\cdot\text{expectedPosition}}{100}
       *    | } {\frac{cols}{2}} \right)^{\text{exponent}}
       * \f]
       * Effect of this parameter: the greater, the more generous the ranking
       * (without changing null). <BR>
       * Range: >=0, Default: 2, Disable: 0
       */
      int hDistExponent;

      /** 
       * Blob ranking parameter: score for a blob's horizontal position. For
       * corresponding formula see 
       * lti::faceThreshold::parameters::hDistExponent. 
       * <BR>
       * Effect of this parameter: determines where the face has to be
       * expected relative to image width, i.e.50 (\%) means in the middle. 
       * <BR>
       * Range: [0..100], Default: 50 (to disable this criterion, set 
       * lti::faceThreshold::parameters::hDistExponent to 0)
       */
      int hDistExpectedPosition;

      /** 
       * Blob ranking parameter: score for a blob's horizontal position. For
       * corresponding formula see 
       * lti::faceThreshold::parameters::hDistExponent. 
       * <BR>
       * Effect of this parameter: the greater, the smaller the region for
       * positive scores
       * <BR>
       * Range: >=1, Default: 1 (to disable this criterion, set 
       * lti::faceThreshold::parameters::hDistExponent to 0)
       */
      int hDistWidth;

      /** 
       * Blob ranking parameter: score for a blob's vertical position. 
       * <BR>
       * The closer a blob's center of gravity is to a specified vertical
       * position (see lti::faceThreshold::parameters::vDistExpectedPosition),
       * the more score is reached. 
       * The corresponding formula is:
       * \f[
       *    \text{vDist} (y, rows) := 100 - 100 \cdot \left( \text{width}
       *    \cdot \dfrac {| \frac{y-rows\cdot\text{expectedPosition}}{100} |}
       *    {\frac{rows}{2}} \right)^{\text{exponent}}
       * \f]
       * Effect of this parameter: the greater, the more generous the ranking
       * (without changing null). <BR>
       * Range: >=0, Default: 3, Disable: 0
       */
      int vDistExponent;

      /** 
       * Blob ranking parameter: score for a blob's vertical position. For
       * corresponding formula see 
       * lti::faceThreshold::parameters::vDistExponent. 
       * <BR>
       * Effect of this parameter: determines where the face has to be
       * expected relative to image height, i.e.50(\%) means in the middle.
       * <BR>
       * Range: [0..100], Default: 30 (to disable this criterion, set 
       * lti::faceThreshold::parameters::vDistExponent to 0)
       */
      int vDistExpectedPosition;

      /** 
       * Blob ranking parameter: score for a blob's vertical position. For
       * corresponding formula see 
       * lti::faceThreshold::parameters::vDistExponent. 
       * <BR>
       * Effect of this parameter: the greater, the smaller the region for
       * positive scores 
       * <BR>
       * Range: >=1, Default: 3 (to disable this criterion, set 
       * lti::faceThreshold::parameters::vDistExponent to 0)
       */
      int vDistWidth;

      /** 
       * Blob ranking parameter: score for a blob's size. <BR>
       * Blobs with an area size between the minimal an maximal size (both
       * specified relative to image size by 
       * lti::faceThreshold::parameters::sizeMin and 
       * lti::faceThreshold::parameters::sizeMax) gain positive score,
       * favouring the middel. The penalty score increases for both smaller
       * and bigger blobs. The corresponding forumla is:
       * \f[
       *    \text{blobSize} (A_{\,blob}, A_{\,image}) := 100 - 100 \cdot
       *    \left( \dfrac {| 2 \cdot A_{\,blob} - \frac{\text{min}\cdot
       *    A_{\,image}}{100} - \frac{\text{max}\cdot A_{\,image}}{100} | }
       *    {\frac{\text{max}\cdot A_{\,image}}{100} - \frac{\text{min}\cdot
       *    A_{\,image}}{100}} \right)^{exponent}
       * \f]
       * Effect of this parameter: The greater, the more generous the ranking
       * (i.e.higher score for blobs with size within range but close to min
       * or max).
       * <BR>
       * Range: >= 0, Default: 3, Disable: 0
       */
      int sizeExponent;

      /** 
       * Blob ranking parameter: score for a blob's size. For corresponding 
       * formula see lti::faceThreshold::parameters::sizeExponent. 
       * <BR>
       * Effect of this parameter: specification of the minimal blob size 
       * relative to image size.
       * <BR>
       * Range: [0..100] , Default: 0.5 (to disable this criterion, set 
       * lti::faceThreshold::parameters::sizeExponent to 0)
       */
      float sizeMin;

      /**
       * Blob ranking parameter: score for a blob's size. For corresponding 
       * formula see lti::faceThreshold::parameters::sizeExponent. 
       * <BR>
       * Effect of this parameter: specification of the maximal blob size 
       * relative to image size.
       * <BR>
       * Range: [0..100] , Default: 5.5 (to disable this criterion, set 
       * lti::faceThreshold::parameters::sizeExponent to 0)
       */
      float sizeMax;

      /** 
       * Blob ranking parameter: Since a higher treshold means that at least
       * the outer points of a blob are skin with equivalent higher
       * probability, higher thresholds are favoured. Furthermore a threshold
       * increase in lower regions is rewarded more than in higher regions.
       * The corresponding formula is:
       * \f[
       *    \text{thresholdProbability}(t) := 100 \cdot 
       *    \left( \dfrac{t}{255} \right)^
       *    {\frac{1}{\text{exponent}}}
       * \f]
       * Effect of this parameter: The greater, the greater the score for
       * increase in low threshold regions.
       * <BR>
       * Range: >= 0, Default: 2, Disable: 0
       */
      int thresholdProbabilityExponent;

    };
    // #################### faceThreshold (public) ###########################

    /**
     * default constructor
     */
    faceThreshold();

    /**
     * Construct a functor using the given parameters
     */
    faceThreshold(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    faceThreshold(const faceThreshold& other);

    /**
     * destructor
     */
    virtual ~faceThreshold();

    /**
     * returns the name of this type ("faceThreshold")
     */
    virtual const char* getTypeName() const;
  
    /**
     * @param input channel with the source data
     * @param threshold resulting threshold
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& input, float& threshold) const;

    /**
     * @param input channel with the source data
     * @param threshold resulting threshold
     * @param faceBlob resulting face blob
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& input, 
               float& threshold, 
               borderPoints& faceBlob) const;

    /**
     * @param input channel8 with the source data
     * @param threshold resulting threshold
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& input, int& threshold) const;

    /**
     * @param input channel8 with the source data
     * @param threshold resulting threshold
     * @param faceBlob resulting face blob
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& input,
               int& threshold, 
               borderPoints& faceBlob) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    faceThreshold& copy(const faceThreshold& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    faceThreshold& operator=(const faceThreshold& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const faceThreshold::parameters& getParameters() const;

  // ###################### faceThreshold (protected) ########################

  protected:
    // protected, internal constant
    enum {
      numOfCriteria = 8
    };

    // protected method, is called by apply()
    bool linearThresholdSearch (const channel8 &mask, 
                                int &faceThreshold, 
                                borderPoints &faceBlob) const;

    // protected method, is called by apply()
    bool optimizedThresholdSearch (const channel8 &mask, 
                                   int &faceThreshold, 
                                   borderPoints &faceBlob) const;
    
    // protected method, is called by linearThresholdSearch() and 
    // optimizedThresholdSearch()
    void getScoreForBlob (std::map<std::string, double> &featureMap, 
                          const int &cols, 
                          const int &rows, 
                          const int &threshold,
                          int &score) const;

    // protected method, is called by linearThresholdSearch() and
    // optimizedThresholdSearch()
    void interpolate (borderPoints &blob, 
                      const int &cols, 
                      const int &rows) const;
  };
}

#endif
