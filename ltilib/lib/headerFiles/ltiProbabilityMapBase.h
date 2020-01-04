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


/*----------------------------------------------------------------
 * project ....: LTI Digital Image/Signal Processing Library
 * file .......: ltiProbabilityMapBase.h
 * authors ....: Benjamin Winkler, Florian Bley
 * organization: LTI, RWTH Aachen
 * creation ...: 30.1.2001
 * revisions ..: $Id: ltiProbabilityMapBase.h,v 1.9 2005/06/16 12:35:43 doerfler Exp $
 */

#ifndef _LTI_PROBABILITY_MAP_BASE_H_
#define _LTI_PROBABILITY_MAP_BASE_H_


#include "ltiTransform.h"
#include "ltiImage.h"
#include "ltiHistogram.h"

namespace lti {
  /**
   * base class for all probability map classes, e.g. using 2D or
   * 3D color histograms.
   */
  class probabilityMapBase : public transform {
  public:
    /**
     * the parameters for the class probabilityMapBase
     */
    class parameters : public transform::parameters {
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
       * copy the contents of a parameters object, except the histograms.
       *
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copyAllButHistograms(const parameters& other);

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was succeful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was succeful
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
       * overall object probability (default is 0.5)
       */
      float objectProbability;

      /**
       * set object color model.
       *
       * A copy of the object will be done.
       */
      void setObjectColorModel(const thistogram<double> &objModel);

      /**
       * returns a reference to the object color model
       */
      const thistogram<double> &getObjectColorModel() const;

      /**
       * check if the object color model is valid.
       *
       * Valid means that the model has already been set
       * (with setObjectColorModel) and that the dimensionality of the model
       * fits with the dimension which the derived class requires.
       *
       * @return true if valid, false otherwise.
       */
      virtual bool isObjectColorModelValid() const = 0;

      /**
       * set non-object color model
       *
       * A copy of the object will be done.
       */
      void setNonObjectColorModel(const thistogram<double> &nonObjModel);

      /**
       * returns a reference to the non-object color model
       */
      const thistogram<double> &getNonObjectColorModel() const;

      /**
       * check if the non-object color model is valid
       *
       * Valid means that the model has already been set
       * (with setObjectColorModel) and that the dimensionality of the model
       * fits with the dimension which the derived class requires.
       *
       * @return true if valid, false otherwise.
       */
      virtual bool isNonObjectColorModelValid() const = 0;

      /**
       * Number of Iterations.
       *
       * Default value: 1
       *
       * This member should be always greater than 1. If zero or negative
       * values are given, then 1 will be assumed.  At this time, the functor
       * requires BOTH models (object AND non-object) to compute more than
       * one iteration.
       *
       * The first iteration will always compute the probability map,
       * assuming that for all pixels in the color image the <em>a
       * priori</em> probability for it to be part of the object is given by
       * the <code>objectProbability</code>.
       *
       * Each following iteration will compute the <em>a priori</em> for each
       * pixel as some averaging of the probabilities in the neighbourhood of
       * pixel computed in the previous iteration.  The available averaging
       * operators are gaussian filters or square filters.  The size of the
       * neighbourhood will be specified by the <code>windowSize</code>
       * attribute.
       */
      int iterations;

      /**
       * If true, a gaussian averaging filter will be used.  If false, a
       * square filter will be used.
       *
       * Default value: false (use a square kernel)
       */
      bool gaussian;

      /**
       * Size for the gaussian kernel or the square kernel used to compute
       * the average probability to be used as <em>a priori</em> probability in
       * the next iteration.
       *
       * Default value: 5 (use a 5x5 kernel)
       */
      int windowSize;

      /**
       * In case the averaging operator is a gaussian kernel, this is the
       * variance to be used.
       *
       * Default value: -1 (meaning the lti::gaussKernel1D should compute a
       *                    default variance)
       */
      double variance;

    protected:
      /**
       * specify if the histograms are administrated by this class or not.
       */
      bool ownModels;

      /**
       * internal pointer to non-object color model
       */
      const thistogram<double>* nonObjectColorModel;

      /**
       * internal pointer to object color model
       */
      const thistogram<double>* objectColorModel;

    };

    /**
     * default constructor
     */
    probabilityMapBase();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    probabilityMapBase(const probabilityMapBase& other);

    /**
     * destructor
     */
    virtual ~probabilityMapBase();

    /**
     * returns the name of this type ("probabilityMapBase")
     */
    virtual const char* getTypeName() const;

    /**
     * updateParameters was overloaded to avoid reloading the
     * histograms every time apply is called.
     */
    virtual bool updateParameters();

    /**
     * This method is similar to setParameters, but the histograms are
     * leaved untouched.  It assumes that some parameters were already set. 
     * Otherwise it will return false.
     *
     * This means, all attributes in the given histograms will be copied, 
     * except the histograms themselves.
     *
     * This is necessary in time critical situations, where some of the
     * attributes have to be modified except the probabilistic models, which
     * are large and require some time to be copied.
     */
    virtual bool 
    setParametersKeepingHistograms(const parameters &theParams);

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    probabilityMapBase& copy(const probabilityMapBase& other);

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    probabilityMapBase& operator=(const probabilityMapBase& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const = 0;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Read the functor from the given ioHandler. 
     *
     * The default implementation is to read just the parameters object.
     *
     * Since this virtual method needs to know the exact type of the
     * parameters to call the proper read method, it will just assume that the
     * current functor instance has a valid parameter set.  If this is not
     * the case, you need to reimplement the read method to set first a dummy
     * parameter object.
     *
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool read(ioHandler& handler,const bool complete=true)=0;

  protected:

    /**
     * generate probability histogram.
     *
     * This is automatically called in the setParameters method.
     *
     * The generate method initialize the probabilityHistogram method which
     * contain the pre-computations based on the bayes-theorem to compute
     * a probability of a pixel to be an object or not.
     */
    virtual bool generate();

    /**
     * generate probability histogram from one histogram.
     */
    virtual bool generate(const thistogram<double> &objectModel);

    /**
     * generate probability histogram from two histograms.
     */
    virtual bool generate(const thistogram<double> &objectModel,
                          const thistogram<double> &nonObjectModel);

    /**
     * probability histogram
     *
     * This histogram contains already the computations for the first iteration
     * of the color map.
     */
    thistogram<double> probabilityHistogram;

    /**
     * generate lookup table for faster histogram element access
     */
    void generateLookupTable(const ivector &dimensions);

    /**
     * element lookup table
     */
    matrix<ubyte> lookupTable;
  };
}

#endif

