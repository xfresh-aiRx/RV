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
 * file .......: ltiProbabilityMap.h
 * authors ....: Benjamin Winkler
 * organization: LTI, RWTH Aachen
 * creation ...: 30.1.2001
 * revisions ..: $Id: ltiProbabilityMap.h,v 1.27 2005/06/16 12:35:43 doerfler Exp $
 */

#ifndef _LTI_PROBABILITY_MAP_H_
#define _LTI_PROBABILITY_MAP_H_


#include "ltiProbabilityMapBase.h"

namespace lti {
  /**
   * Probability Map based on 3D non-parametric (color) models.
   *
   * Creates a probability map given two color histogram, one
   * modelling the %object color and the other modeling the
   * non-object colors.
   *
   * The probability of a specified color \e rgb is calculated according to
   * the Bayes formula:
   *
   * \f[
   * p(obj|rgb) = \frac{p(rgb|obj) \cdot p(obj)}
   *             {(p(rgb|obj) \cdot p(obj) + p(rgb|nonobj) \cdot p(nonobj))}
   * \f]
   *
   * where p(obj) is the overall objectProbability,
   * p(nonobj) := 1 - p(obj).
   *
   * \f$p(rgb|obj)\f$ and \f$p(rgb|nonobj)\f$ are read from the given object
   * and non-object models.
   *
   * In case you provide only the object probability histogram, the non-object
   * histogram will be assumed to be uniform distributed, i.e. all colors can
   * be non-object with the same probability.
   */
  class probabilityMap : public probabilityMapBase {
  public:
    /**
     * The parameters for the class probabilityMap
     */
    class parameters : public probabilityMapBase::parameters {
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
       * returns name of this type
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
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const;

      /**
       * Check if the object color model is valid.
       *
       * Valid means that the model has already been set
       * (with setObjectColorModel) and that the dimensionality of the model
       * is 3.
       *
       * @return true if valid, false otherwise.
       */
      virtual bool isObjectColorModelValid() const;

      /**
       * Check if the non-object color model is valid
       *
       * Valid means that the model has already been set
       * (with setObjectColorModel) and that the dimensionality of the model
       * is 3.
       *
       * @return true if valid, false otherwise.
       */
      virtual bool isNonObjectColorModelValid() const;
    };

    /**
     * default constructor
     */
    probabilityMap();

    /**
     * constructor
     * @param theParams the parameters to be used
     */
    probabilityMap(const parameters& theParams);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    probabilityMap(const probabilityMap& other);

    /**
     * destructor
     */
    virtual ~probabilityMap();

    /**
     * returns the name of this type ("probabilityMap")
     */
    virtual const char* getTypeName() const;

    /**
     * Creates an object probability channel of an image (values range from 0.0
     * to 1.0).
     * @param src image with the source data.
     * @param dest channel where the result will be left.
     * @return true is successful, false otherwise
     */
    virtual bool apply(const image& src,channel& dest) const;


    /**
     * creates an object probability channel of an image (values range from 0.0
     * to 1.0).
     *
     * @param src image with the source data.
     * @param dest channel where the result will be left.
     * @param apriori By using this parameter it is possible to include a given
     *                position-dependent apriori object probability channel in
     *                the computation of the object probability channel. 
     *                A value of 0.5 at a point indicates equal probabilities
     *                for object and non-object values. 
     *                Any bigger value (< 1.0) indicates a higher probability
     *                for  object values, any smaller value (> 0) indicates a
     *                lower probability.
     *                The apriori-channel should have the same size as the 
     *                input image.            
     * @return true is successful, false otherwise
     */
    virtual bool apply(const image& src,channel& dest,
                       const channel &apriori) const;

    /**
     * Returns the object probability for an rgb color value (values range
     * from 0.0 to 1.0).
     *
     * This method uses the given vector to compute the index of the histogram.
     * It \b MUST be preinitialized with a size of 3, or the method will crash.
     * 
     * The idea is to externally provide an ivector in order to spare some
     * creation time.
     *
     * @param src rgbPixel with the source color.
     * @param theBin index vector computed by the method to the histogram 
     *               entry.
     *     
     * @return the object probability
     */
    virtual float apply(const rgbPixel &src,ivector& theBin) const;

    /**
     * Returns the object probability for an rgb color value (values range
     * from 0.0 to 1.0).
     *
     * This method is thread safe, but much slower than the other one.
     * 
     * @param src rgbPixel with the source color.
     * @return the object probability
     */
    virtual float apply(const rgbPixel &src) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    probabilityMap& copy(const probabilityMap& other);

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    probabilityMap& operator=(const probabilityMap& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * Returns used parameters
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
    virtual bool read(ioHandler& handler,const bool complete=true);


  protected:

    /**
     * Compute the second and up iterations of a probability map
     * using the given apriori probabilites per pixel.
     */
    void computeMap(const image& img,
                    channel& aPrioriDest) const;

  };
}

#endif

