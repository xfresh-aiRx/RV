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
 * creation ...: 21.9.2003
 * revisions ..: $Id: ltiChrominanceMapEstimator.h,v 1.6 2005/06/16 09:32:41 doerfler Exp $
 */

#ifndef _LTI_CHROMINANCE_MAP_ESTIMATOR_H_
#define _LTI_CHROMINANCE_MAP_ESTIMATOR_H_

#include "ltiMacroSymbols.h"

// only for compilers different than VC++ 6.0 available
#ifdef _LTI_MSC_6

#pragma message("Insufficient C++ Template Support for lti::regionGraphMeans.")
#pragma message("You need a newer compiler")

#else

#include "cstdlib"

#include "ltiImage.h"
#include "ltiChrominanceModelEstimator.h"

namespace lti {
  /**
   * This class computes a map between the chrominances and the most
   * likely object based on chrominance histograms and the Bayes Theorem.
   *
   * For details about building the colors models take a look at the
   * description of chrominanceModelEstimator.
   *
   * @see overlappingSets2D, chrominanceModelEstimator, probabilityMap2D,
   * chrominanceMapMasking
   */
  class chrominanceMapEstimator : public chrominanceModelEstimator {
  public:
    
    /**
     * the parameters for the class chrominanceModelEstimator
     */
    class parameters : public chrominanceModelEstimator::parameters {
    public:
      /**
       * default constructor
       */
      parameters(int dim = 0);

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

      /**
       * A probability that is returned for chrominances where no are very
       * few objects hit so far. At these chrominances the background,
       * referring to the first chrominance model with label 0, is
       * classified. To classify a foreground object its probability must
       * be above this threshold.
       *
       * Default: .0001.
       */
      float defaultProbability;

      /**
       * The object probabilities for each object. By default all objects
       * have the same probability.
       */
      genericVector<float> objectProbabilities;    
      
      /**
       * Automatically initialize the object probabilities with their
       * default values.
       *
       * Default: true.
       */
      bool autoObjectProbabilities;
      
      /**
       * Do not compute the probabilities for chrominances that contain
       * the background only. Turn on for speed up.
       *
       * Default: false.
       */
      bool excludeBackground;
    };

    /**
     * default constructor
     */
    chrominanceMapEstimator();

    /**
     * Construct a functor using the given parameters
     */
    chrominanceMapEstimator(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    chrominanceMapEstimator(const chrominanceMapEstimator& other);

    /**
     * destructor
     */
    virtual ~chrominanceMapEstimator();

    /**
     * returns the name of this type ("chrominanceModelEstimator")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    chrominanceMapEstimator& copy(const chrominanceMapEstimator& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    chrominanceMapEstimator& operator=(const chrominanceMapEstimator& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /** 
     * set the parameters and ensure validity 
     */
    bool setParameters(const parameters& theParams);
 
    /** 
     * For a given chrominance get the object label which is most likely.
     * For more sophisticated segmentation @see chrominanceMapMasking.
     * @param u refers to the columns of the chrominance map.
     * @param v refers to the rows of the chrominance map.
     * @param label the most likely object label.
     * @return true on success and false otherwise.
     */
    inline bool apply(ubyte u, ubyte v, int& label) const;

    /** 
     * For a given chrominance get the object label which is most likely
     * and its probability.
     * For more sophisticated segmentation @see chrominanceMapMasking.
     * @param u refers to the columns of the chrominance map.
     * @param v refers to the rows of the chrominance map.
     * @param label the most likely object label.
     * @param prob the probability of the most likely object.
     * @return true on success and false otherwise.
     */
    inline bool apply(ubyte u, ubyte v, int& label, float& prob) const;

    /** 
     * Recompute the chrominance map and probabilities from the chrominance
     * models. This method checks the parameters to determine if the entire
     * map is processed or only those parts that are covered by an object as
     * specified by the maximal chrominance sets.
     * @return true on success and false otherwise.
     */
    bool update();
    
    /** 
     * Recompute the chrominance map and probabilities from the chrominance
     * models. This method processes only those parts that are covered
     * by an object.
     * specified by the maximal chrominance sets.
     * @return true on success and false otherwise.
     */
    bool updateExclude();
 
    /** 
     * Recompute the chrominance map and probabilities from the chrominance
     * models. This method processes the entire map.
     * specified by the maximal chrominance sets.
     * @return true on success and false otherwise.
     */
    bool updateAll();

    /** 
     * Returns the chrominance map.
     */
    const channel8& getChrominanceMap() const;
    
    /** 
     * Returns the probabilities of the most likely object.
     */
    const channel& getProbabilityMap() const;

    /** 
     * Writes the chrominance map and probabilities to disk.
     * @param prefix a string containing the path or prefix to the files. If
     *   you specify a directory, please do not forget the /. It is not added
     *   automatically.
     * @param colors  color each object as specified by this vector.
     * @return true on success and false otherwise.
     */
    bool writeChrominanceMap(const std::string& prefix,
			     const vector<rgbPixel>& colors) const;

  protected:
    /**
     * Initializes stuff needed by the update methods.
     * @param histIts iterator pointing at the chrominance model
     * @param objNumEntries the number of entries in each histogram.
     */
    void initUpdate(std::vector<histogram2D::const_iterator>& histIts,
		    dvector& objNumEntries) const;
    /*
     * Get a pointer to the parameters.
     */
    parameters& getWritableParameters();

    /**
     * the color map
     */
    channel8 m_colorMap;

    /**
     * probabilities of the best object
     */
    channel m_probabilityMap;
    
    /**
     * Special care when error in setParameters during construction.
     * Historic and complicated code, but it works.
     */
    bool m_constructMap;

  };

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------
  
  inline bool chrominanceMapEstimator::apply(ubyte u, ubyte v,
					     int& label) const {
    
    label = m_colorMap.at(u,v);
    return true;
  };
  
  inline bool chrominanceMapEstimator::apply(ubyte u, ubyte v, 
					    int& label, float& prob) const {
    
    label = m_colorMap.at(u,v);
    prob  = m_probabilityMap.at(u,v);
    return true;
  };
}

#endif

#endif
