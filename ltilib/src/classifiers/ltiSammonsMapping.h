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
 * file .......: ltiSammonsMapping.h
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 9.10.2002
 * revisions ..: $Id: ltiSammonsMapping.h,v 1.10 2005/06/16 09:32:40 doerfler Exp $
 */

#ifndef _LTI_SAMMONS_MAPPING_H_
#define _LTI_SAMMONS_MAPPING_H_

#include "ltiFunctor.h"
#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiProgressInfo.h"

namespace lti {
  /**
   *  Performs Sammon's Mapping. The data from some high dimensional
   *  source space is mapped to a destination space of lower
   *  dimensionality. In the process Sammon's Mapping tries to keep
   *  distances between samples in the destination space as close to
   *  those in the source space. Typical uses are data analysis or
   *  visualization of SOFM networks. For details see:
   *
   *  John W. Sammon, Jr. <i>A nonlinear mapping for data structure
   *  analysis.</i> IEEE Transactions on Computers, C-18(5):401-409, May
   *  1969.
   *
   *  There are several parameters the algorithm can be tuned
   *  with. Initialization is usually performed using principal
   *  component analysis. It projects the data from the original space
   *  into the destination space using the eigenvectors with largest
   *  eingenvalues. The other option --- random initialization ---
   *  seems to be useful for demonstration purposes, only.
   *
   *  Three methods are available for minimizing the error or stress
   *  of the mapping: gradient descent, gradient descent with momentum
   *  and steepest descent. Of these, steepest descent usually
   *  requires the smallest number of iterations, but each iteration
   *  is more costly. Further, with many planes in the error surface,
   *  the computation of the second derivative might not be
   *  possible. In case of 0 this value is set to 1.E-4 as a
   *  work-around. The gradient descent methods are much more
   *  stable. Their convergence, however, is much slower. Using
   *  momentum can lead to faster convergence than regular gradient
   *  descent but also needs more computation per iteration. According
   *  to Sammon, errors should be below 1E-2 to be considered
   *  good. Usually, much smaller errors are achieved.
   */
  class sammonsMapping : public functor {
  public:
    /**
     * the parameters for the class sammonsMapping
     */
    class parameters : public functor::parameters {
    public:

      /**
       * Initialization for the lower dimensional image space. The
       * Random version uses initBox for the size of the hyper cube
       * the values are drawn from. The PCA version was suggested by
       * Sammon for real-world problems. It uses the projection of the
       * data points into the image space as initialization.
       */
      enum eInit {
        Random, /*!< choose initial values randomly from hyper cube initBox */
        PCA     /*!< initial values are projection of the data by PCA */
      };

      /**
       * Different methods for seeking the minimum of the error:
       * <p><ul>
       * <li><b>Steepest</b> uses steepest descent, i.e. the gradient
       *     is divided by the norm of the second derivative.</li>
       * <li><b>Gradient</b> uses regular gradient descent</li>
       * <li><b>Momentum</b> uses gradient descent with momentum. The momentum
       *      is set by the parameter mu.</li>
       * </ul>
       */
      enum eSearch {
        Steepest, /*!< steepest descent */
        Gradient, /*!< gradient descent */
        Momentum  /*!< gradient descent with momentum */
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
       * The number of dimensions of the output space. Since Sammon's
       * Mapping is mostly used for visualization the dafault is 2.
       */
      int dimensions;

      /**
       * Number of steps. Default is 200.
       */
      int steps;

      /**
       * Threshold for the error of the mapping. By default not used, thus 0.
       */
      double errorThresh;

      /**
       * A 'learn' rate or step size. According to Kohonen it should
       * be somewhere between 0.3 and 0.4. The default is 0.35
       */
      double alpha;

      /**
       * Sets the initType. Default is PCA.
       */
      eInit initType;

      /**
       * Hyper cube random init values are chosen from in case
       * initType is Random. Each dimension ranges from 0 to the given
       * value. Take care to choose the same dimesionality for initBox
       * as the value of dimensions. Default is a unit cube.
       */
      dvector initBox;

      /**
       * Sets the type of search method used to minimize the
       * error. Default is Steepest.
       */
      eSearch searchType;

      /**
       * The momentum. Used if searchType is set to Momentum. Default is 0.1
       */
      double mu;

    };

    /**
     * default constructor
     */
    sammonsMapping();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    sammonsMapping(const sammonsMapping& other);

    /**
     * destructor
     */
    virtual ~sammonsMapping();

    /**
     * returns the name of this type ("sammonsMapping")
     */
    virtual const char* getTypeName() const;

    //TODO: comment your apply methods!

    /**
     * operates on a copy of the given %parameters.
     * @param src dmatrix with the source data.
     * @param dest dmatrix where the result will be left.
     * @param error the error of the mapping
     * @return true if apply successful or false otherwise.
     */
    bool apply(const dmatrix& src,dmatrix& dest,double& error) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src dmatrix with the source data.
     * @param dest dmatrix where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const dmatrix& src,dmatrix& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    sammonsMapping& copy(const sammonsMapping& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    sammonsMapping& operator=(const sammonsMapping& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * set a progress %object
     *
     * A clone of the given %object will be generated.
     */
    void setProgressObject(const progressInfo& progBox);

    /**
     * remove the active progress %object
     */
    void removeProgressObject();

    /**
     * return true if a valid progressInfo %object has already been setted
     */
    bool validProgressObject() const;

    /**
     * get progress %object
     */
    progressInfo& getProgressObject();

    /**
     * get progress %object
     */
    const progressInfo& getProgressObject() const;

  protected:

    /**
     * current progress %object
     */
    mutable progressInfo* progressBox;

    /** random initialization */
    void initRandom(dmatrix& dest) const ;

    /** initialization using PCA */
    void initPca(const dmatrix& data, dmatrix& dest) const ;

  };
}

#endif

