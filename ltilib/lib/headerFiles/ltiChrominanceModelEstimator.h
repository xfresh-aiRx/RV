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
 * file .......: ltiChrominanceModelEstimator.h
 * authors ....: Claudia Goenner
 * organization: LTI, RWTH Aachen
 * creation ...: 8.2.2004
 * revisions ..: $Id: ltiChrominanceModelEstimator.h,v 1.5 2005/06/16 09:32:41 doerfler Exp $
 */

#ifndef _LTI_CHROMINANCE_MODEL_ESTIMATOR_H_
#define _LTI_CHROMINANCE_MODEL_ESTIMATOR_H_

#include "ltiMacroSymbols.h"

// only for compilers different than VC++ 6.0 available
#ifndef _LTI_MSC_6

#include "ltiImage.h"
#include "ltiHistogram.h"
#include "ltiOverlappingSets2D.h"
#include "cstdlib"

#include "ltiStatisticsFunctor.h"

namespace lti {
 
  /**
   * This class stores a 2d histogram containing all chrominances which
   * have been seen on a specific object. The class administers histograms
   * for >=1 objects. The histograms may be processed by other classes
   * to create probability maps or a look up table containing the most
   * likely object for each chrominance.
   *
   * The class supports a time out of the histogram entries. For robustness
   * and run-time efficiency a maximal chrominance set may be specified for
   * each object including the background. E.g. if an object is known to be
   * red it is advantageous to store only red chrominances inside it's
   * histogram. This way no false chrominances are stored if the
   * segmentation is errornous. Furthermore, if only red objects are processed
   * all other chrominances may be excluded from processing for run time
   * efficiency. We do not need to store these non-object chrominances in 
   * the background histogram neither.
   *
   * If using fully automatic re-calibration it is
   * possible to carve out chrominances that are well known to belong to an
   * object from the background. These chrominances will always be segmented
   * and the object cannot dissappear due to time-out and wrong maxima.
   *
   * If you change the maximal chrominance sets online you should implement
   * a method that deletes all histogram entries outside the region defined
   * by the maximal chrominance sets. This is not done yet.
   * 
   * @see overlappingSets2D, chrominanceMapEstimator, probabilityMap2D
   */
  class chrominanceModelEstimator : public statisticsFunctor {
  public:
    /**
     * The parameters for the class chrominanceModelEstimator
     */
    class parameters : public statisticsFunctor::parameters {
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
       * Check if a detected object's chrominance is contained in the maximimal
       * set of chrominances that are defined for each object a priori. If
       * false the histogram is build for all chrominances.
       *
       * Default: false.
       */
      bool checkIsObject;

      /**
       * Adapt the size of the table containing the maximal chrominances to
       * match the quantization of the histograms automatically.
       *
       * Default: true.
       */
      bool autoIsObjectSize;

      /**
       * Each color band usually contains 8bit resulting in a 256x256
       * histogram. For efficiency the histograms may be downsampled by
       * 2 power shrinkFactor.
       *
       * Default: 0.
       */
      int shrinkFactor;

      /**
       * The number of chrominance histograms which are build. Usually this
       * is the number of interesting objects plus the background.
       *
       * Default: 0.
       */
      int numberOfModels;

      /**
       * The interval after which a histogram is multiplied with the
       * decay factor.
       *
       * Default: 100.
       */
      int interval;
      
      /**
       * The factor by which the histogram are decay to erase old
       * entries.
       *
       * Default: .7.
       */
      float decay;

      /**
       * A histogram is not decayed if its maximum is below this
       * value.
       *
       * Default: 1000.
       */
      int minHits;

      /**
       * The maximal chrominance sets are coded as a single two dimensional
       * look up table.
       */
      overlappingSets2D checker;
    };

    /**
     * Default constructor
     */
    chrominanceModelEstimator();

    /**
     * Construct a functor using the given parameters
     */
    chrominanceModelEstimator(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    chrominanceModelEstimator(const chrominanceModelEstimator& other);

    /**
     * Destructor
     */
    virtual ~chrominanceModelEstimator();

    /**
     * Returns the name of this type ("chrominanceModelEstimator")
     */
    virtual const char* getTypeName() const;

    //TODO: comment your apply methods!
 
    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    chrominanceModelEstimator& copy(const chrominanceModelEstimator& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    chrominanceModelEstimator& 
      operator=(const chrominanceModelEstimator& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Set the parameters and ensure validity.
     */
    bool setParameters(const parameters& theParams);
    
    /**
     * Initialize the chrominance models.
     * @param theModels the histograms to be copied inside the internal state
     *   of the class.
     * @return true on success and false otherwise.
     */
    bool setChrominanceModels(const std::vector<histogram2D>& theModels);

    /**
     * Get the histograms that are build up by this class.
     */
    const std::vector<histogram2D>& getChrominanceModels() const;
    
    /**
     * Store all chrominance models to disk.
     * @param prefix a string containing the path or prefix to the files. If
     *   you specify a directory, please do not forget the /. It is not added
     *   automatically.
     * @param fileNames a vector with the filename for each histogram.
     * @return true on success and false otherwise.
     */
    bool writeChrominanceModels(const std::string& prefix, 
				const std::vector<std::string>& fileNames)
      const;
    
    /**
     * Initialize the chrominance models from disk.
     * @param prefix a string containing the path or prefix to the files. If
     *   you specify a directory, please do not forget the /. It is not added
     *   automatically.
     * @param fileNames a vector with the filename for each histogram.
     * @return true on success and false otherwise.
     */
    bool readChrominanceModels(const std::string& prefix, 
			       const std::vector<std::string>& fileNames);

    /**
     * Consider the chrominances in the supplied channel8. The mask specifies
     * which histogram is incremented. Usually the background has the
     * index 0. Any pixels that shall not be considered must have a value
     * above or equal to the number of models. All histograms will be
     * considered as updated, no matter whether they really did change.
     * @param uChan refers to the columns of the histogram.
     * @param vChan refers to the rows of the histogram.
     * @param mask specifies which models will be updated. Tip: If drawing the
     *   mask from object shape keep in mind that drawing rectangles is
     *   faster than drawing polygons.
     * @return true on success and false otherwise.
     */
    virtual bool consider (const channel8& uChan, const channel8& vChan,
			   const channel8& mask);

    /**
     * Consider the chrominances in the supplied channel8. The mask specifies
     * which histogram is incremented. Usually the background has the
     * index 0. Any pixels that shall not be considered must have a value
     * above or equal to the number of models.
     * @param uChan refers to the columns of the histogram.
     * @param vChan refers to the rows of the histogram.
     * @param mask specifies which models will be updated.
     * @param count specifies how the counters will be incremented. The 
     *  counter determine if a histogram is decayed prior to
     *  considering the new values.
     * @return true on success and false otherwise.
     */
    virtual bool consider (const channel8& uChan, const channel8& vChan,
			   const channel8& mask, const ivector& count);

  protected:
    /**
     * Returns used parameters
     */
    parameters& getWritableParameters();

    /**
     * Check if the histograms are valid, i.e. have the correct size
     * and accord to the number of models
     * @param theHistos the chrominance models.
     * @return true on success and false otherwise.
     */
    bool checkHistograms(const std::vector<histogram2D>& theHistos);

    /**
     * Decrease the histogram's entries.
     * @param theHisto a chrominance model.
     * @return true on success and false otherwise.
     */
    bool scaleValues(histogram2D& theHisto);
    
    /**
     * Rescale a histogram, i.e. change it's quantization.
     * @param theHisto a chrominance model. The result is left here, too.
     * @param cellsInDimension the new number of cells.
     * @return true on success and false otherwise.
     */ 
    bool rescale(histogram2D& theHisto, const int cellsInDimension); 
  
    /**
     * Rescale a histogram.
     * @param theHisto a chrominance model.
     * @param destHisto scaled chrominance model.
     * @param cellsInDimension the new number of cells.
     * @return true on success and false otherwise.
     */ 
    bool rescale(const histogram2D& theHisto, histogram2D& destHisto,
		 const int cellsInDimension); 
  
    /**
     * Upsample a histogram.
     * @param theHisto a chrominance model.
     * @param destHisto scaled chrominance model.
     * @param factor the upsampling factor.
     * @return true on success and false otherwise.
     */
    bool upsample(const histogram2D& theHisto, histogram2D& destHisto,
		  const int factor);
 
    /**
     * Downsample a histogram.
     * @param theHisto a chrominance model.
     * @param destHisto scaled chrominance model.
     * @param factor the downsampling factor.
     * @return true on success and false otherwise.
     */
    bool downsample(const histogram2D& theHisto, histogram2D& destHisto,
		    const int factor);

    /**
     * the 2D histograms of the chrominances
     */
    std::vector<histogram2D> m_chromHistos;

    /**
     * counts calls of consider
     */
    ivector m_count;
    
    /**
     * maximum number of entries in the histograms that make sense (max. ubyte)
     */
    int m_maxEntries;

    /**
     * the scaled number of entries
     */
    int m_numEntries;

    /**
     * Special care with setParameters during construction.
     * Historic and complicated code, but it works.
     */
    bool m_constructModel;
  };
 
}//namespace

#endif

#endif

