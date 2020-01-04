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
 * file .......: ltiCWAGMSegmentationEvaluation.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 25.11.2003
 * revisions ..: $Id: ltiCWAGMSegmentationEvaluation.h,v 1.2 2004/06/02 09:52:12 sltilib Exp $
 */

#ifndef _LTI_CWAGM_SEGMENTATION_EVALUATION_H_
#define _LTI_CWAGM_SEGMENTATION_EVALUATION_H_

#include "ltiCWAGMSegmentation.h"
#include "ltiSegmentationEvaluation.h"
#include "ltiImage.h"

namespace lti {

  /**
   * Pareto evaluation of the lti::cwagmSegmentation functor.
   * 
   * See lti::segmentationEvaluation for more information
   */
  class cwagmSegmentationEvaluation : public segmentationEvaluation {
  public:

    /**
     * The parameters for the class cwagmSegmentationEvaluation
     */
    class parameters : public segmentationEvaluation::parameters {
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
      virtual bool write(ioHandler& handler,const bool& complete=true) const;

      /**
       * Read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool& complete=true);

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------
      /**
       * Mininum values for the parameterization of the algorithm
       *
       * The sequence for the enum/string types are:
       * 
       * - colorSplitter: RGB \< XYZ \< xyY \< Luv \< rgI \< YUV \< YIQ \< OCP
       * - gradientType : Optimal \< OGD \< Difference \< Roberts \< Sobel 
       *                  \< Prewitt \< Kirsch
       * - contrastType : MDD \< Contrast \< Maximum
       * - mergeType    : Fast \< Optimal
       */
      cwagmSegmentation::parameters minValues;

      /**
       * Maxinum values for the parameterization of the algorithm
       *
       * The sequence for the enum/string types are:
       * 
       * - colorSplitter: RGB \< XYZ \< xyY \< Luv \< rgI \< YUV \< YIQ \< OCP
       * - gradientType : Optimal \< OGD \< Difference \< Roberts \< Sobel 
       *                  \< Prewitt \< Kirsch
       * - contrastFormat : MDD \< Contrast \< Maximum
       * - mergeType    : Fast \< Optimal
       */
      cwagmSegmentation::parameters maxValues;

    };

    /**
     * Default constructor
     */
    cwagmSegmentationEvaluation();

    /**
     * Construct a functor using the given parameters
     */
    cwagmSegmentationEvaluation(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    cwagmSegmentationEvaluation(const cwagmSegmentationEvaluation& other);

    /**
     * Destructor
     */
    virtual ~cwagmSegmentationEvaluation();

    /**
     * Returns the name of this type ("cwagmSegmentationEvaluation")
     */
    virtual const char* getTypeName() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * @name Public methods reimplemented
     */
    //@{

    /**
     * Convert a binary-chain representation of a chromosome to a valid
     * parameter object.
     */
    virtual bool chromosomeToPhenotype(const chromosome& genotype,
                                       functor::parameters& phenotype) const;

    /**
     * Convert a valid parameters object (phenotype) into binary-chain
     * representation of a chromosome.
     */
    virtual bool phenotypeToChromosome(const functor::parameters& phenotype,
                                       chromosome& genotype) const;

    /**
     * Return the length in bits for a chromosome.
     *
     * This method needs to be reimplemented, in order to get some 
     * default implementations to work.
     */
    virtual int getChromosomeSize() const;

  protected:
    /**
     * Struct containing how many bits are required for each parameter.
     */
    struct bits;

    /**
     * @name The segmentation evaluation methods
     */
    //@{
    /**
     * Initialize the segmentation functor
     */
    virtual bool init(const functor::parameters& param);

    /**
     * Segmentate an image using the parameterization set with init()
     */
    virtual bool segment(const image& img,
                         const imatrix& prevMask,
                         imatrix& mask,
                         channel& certainty);

    //@}

    /**
     * Return a new object with the correct parameters' type
     */
    virtual functor::parameters* getInstanceOfParam() const;

    /**
     * Functor for segmentation
     */
    cwagmSegmentation segmenter;

  };
}

#endif

