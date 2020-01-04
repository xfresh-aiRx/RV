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
 * file .......: ltiMultivariateGaussian.h
 * authors ....: Jens Paustenbach
 * organization: LTI, RWTH Aachen
 * creation ...: 14.5.2002
 * revisions ..: $Id: ltiMultivariateGaussian.h,v 1.9 2005/06/16 12:37:35 doerfler Exp $
 */

#ifndef _LTI_MULTIVARIATE_GAUSSIAN_H_
#define _LTI_MULTIVARIATE_GAUSSIAN_H_


#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiGaussDist.h"
#include "ltiEigenSystem.h"
#include "ltiMatrixDecomposition.h"
#include "list"

namespace lti {
  /**
   *  this class generates either one point from a given covarianceMatrix
   *  and centre or it generates a given number of points according to
   *  these parameters.
   *  The points are drawn from a gaussian distribution
   *  <p> The apply methods expect the %parameters directly, the %parameters
   *  for the draw method must be set in the parameter class
   *  <p> Only draw(const int& number) is implemented.
   *  draw() is only reimplemented to make sure that the function of the
   *  parent class isn't used. This function return always 0.
   *  To draw only one point use draw(const int& number) with number=1
   */
  class multivariateGaussian : public continuousRandomDistribution {
  public:
    /**
     * the parameters for the class multivariateGaussian
     */
    class parameters : public continuousRandomDistribution::parameters {
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
       * the centre point of the distribution
       */
      dvector centre;

      /**
       * the covariance for the distribution
       */
      dmatrix covarianceMatrix;

    };

    /**
     * default constructor
     */
    multivariateGaussian();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    multivariateGaussian(const multivariateGaussian& other);

    /**
     * destructor
     */
    virtual ~multivariateGaussian();

    /**
     * returns the name of this type ("multivariateGaussian")
     */
    virtual const char* getTypeName() const;


    /**
     * computes one point from a gaussian distribution with the given
     * parameters. The resulting point is left in dest
     * operates on the given %parameter.
     * @param mean dvector with the centre.
     * @param covarianceMatrix dmatrix with the covariances
     * @param dest dvector with the resulting point
     * @return true if apply successful or false otherwise.
     */
    bool apply(const dvector& mean, const dmatrix& covarianceMatrix,
               dvector& dest) const;

    /**
     * creates either one cluster of a set of clusters with the given
     * number of points the covarianceMatrix .
     * @param mean dvector with the centre.
     * @param covarianceMatrix dmatrix with the covariances
     * @param number int with the number of points to create
     * @param dest dmatrix with the resulting points in its rows
     * @return true if apply successful or false otherwise.
     */
    bool apply(const dvector& mean, const dmatrix& covarianceMatrix,
               const int& number, dmatrix& dest) const;

    /**
     * @param mean list of dvectors with the centres.
     * @param covarianceMatrix list of dmatrix with the covariances
     * @param numbers list of int with the number of points to create in
     * each cluster
     * @param dest list of dmatrix with the resulting points in its rows
     * a one dmatrix for each cluster
     */
    bool apply(std::list<dvector>& mean, std::list<dmatrix>& covarianceMatrix,
               std::list<int>& numbers, std::list<dmatrix>& dest) const;

    /**
     * @param mean list of dvector with the centres.
     * @param covarianceMatrix list of dmatrix with the covariances
     * @param numbers list of int with the number of points to create
     * in each cluster
     * @param dest dmatrix with the resulting points in its rows
     */
    bool apply(std::list<dvector>& mean, std::list<dmatrix>& covarianceMatrix,
               std::list<int>& numbers, dmatrix& dest) const;



   /**
    * in this context this method doesn't make any sense, so it
    * returns 0.Don't use. Use draw(const int& number) instead
    * and set number equal to one; this returns also only one point
    * in a Matrix with only one row
    * This function was reimplented, because the function is implemented in
    * the parent class, and this function can't be used at this place.
    */
   double draw() const;

    /**
     * computes a number of points with the %parameters set in the
     * parameters object
     * @param number int with the number of points to create
     * @returns return a dmatrix with the points in its rows.
     */
    dmatrix draw(const int& number) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    multivariateGaussian& copy(const multivariateGaussian& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    multivariateGaussian& operator=(const multivariateGaussian& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  protected:
      /**
       * random functor used for generation of points
       */
      gaussianDistribution *gaussFunc;

      /**
       *  get the eigenvalues and eigenvectors
       */
      jacobi<double> *eigenFunc;

      /**
       *  compute determinate
       */
      luDecomposition<double> *detFunc;


  };
}

#endif

