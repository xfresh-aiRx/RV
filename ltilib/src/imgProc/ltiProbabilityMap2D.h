/*
 * Copyright (C) 1998, 1999, 2000, 2001,2002,2003
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
 * file .......: ltiProbabilityMap2D.h
 * authors ....: Florian Bley
 * organization: LTI, RWTH Aachen
 * creation ...: 26.3.2003
 * revisions ..: $Id: ltiProbabilityMap2D.h,v 1.6 2005/06/16 12:35:43 doerfler Exp $
 */

#ifndef _LTI_PROBABILITY_MAP_2D_H_
#define _LTI_PROBABILITY_MAP_2D_H_


#include "ltiProbabilityMapBase.h"

namespace lti {
  /**
   * Probability Map based on 2D non-parametric (color) models.
   *
   * Creates a probability map given two chromacity histogram, one
   * modelling the %object color and the other modeling the
   * non-object colors.
   *
   * The probability of a specified color rgb is calculated according to the
   * Bayes formula:
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
   */
  class probabilityMap2D : public probabilityMapBase {
  public:
   /**
     * the parameters for the class probabilityMap2D
     */
    class parameters : public probabilityMapBase::parameters {
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
       * check if the object color model is valid.
       *
       * Valid means that the model has already been set
       * (with setObjectColorModel) and that the dimensionality of the model
       * is 2.
       *
       * @return true if valid, false otherwise.
       */
      virtual bool isObjectColorModelValid() const;

      /**
       * check if the non-object color model is valid
       *
       * Valid means that the model has already been set
       * (with setObjectColorModel) and that the dimensionality of the model
       * is 2.
       *
       * @return true if valid, false otherwise.
       */
      virtual bool isNonObjectColorModelValid() const;
    };

    /**
     * default constructor
     */
    probabilityMap2D();

    /**
     * constructor
     * @param theParams the parameters to be used
     */
    probabilityMap2D(const parameters& theParams);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    probabilityMap2D(const probabilityMap2D& other);

    /**
     * destructor
     */
    virtual ~probabilityMap2D();

    /**
     * returns the name of this type ("probabilityMap2D")
     */
    virtual const char* getTypeName() const;

    /**
     * creates a object probability channel of an image (values range from 0.0
     * to 1.0).
     * @param src1 channel with the first chromacity band.
     * @param src2 channel with the second chromacity band.
     * @param dest channel where the result will be left.
     * @result true is successful, false otherwise
     */
    virtual bool apply(const channel8& src1, const channel8& src2, channel& dest) const;

   /**
     * returns the object probability for a chromacity color value (values range
     * from 0.0 to 1.0).
     *
     * This method uses the given vector to compute the index of the histogram.
     * It \b MUST be preinitialized with a size of 2, or the method will crash.
     * 
     * The idea is to externally provide an ivector in order to spare some
     * creation time.
     *
     * @param value1 pixel' value int the first chromacity band.
     * @param value2 pixel' value int the second chromacity band.
     * @param theBin index vector computed by the method to the histogram 
     *               entry.
     *     
     * @result the object probability
     */
    virtual float apply(const ubyte &value1, const ubyte &value2, ivector& theBin) const;

    /**
     * returns the object probability for a chromacity value (values range
     * from 0.0 to 1.0).
     *
     * This method is thread safe, but much slower than the other one.
     * 
     * @param value1 pixel' value int the first chromacity band.
     * @param value2 pixel' value int the second chromacity band.
     * @result the object probability
     */
    virtual float apply(const ubyte &value1, const ubyte &value2) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    probabilityMap2D& copy(const probabilityMap2D& other);

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    probabilityMap2D& operator=(const probabilityMap2D& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

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
    virtual bool read(ioHandler& handler,const bool complete=true);

    /**
     * compute the second and up iterations of a probability map
     * using the given aPriori probabilites per pixel.
     */
    bool computeMap(const channel8& src1, const channel8& src2,
                    channel& aPrioriDest) const;

 };
 
}
#endif

