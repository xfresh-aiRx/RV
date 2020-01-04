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
 * file .......: fundamentalMatrixSolverLMS.h
 * authors ....: Frederik Lange
 * organization: LTI, RWTH Aachen
 * creation ...: 7.1.2002
 * revisions ..: $Id: ltiFMatrixSolverLMS.h,v 1.8 2005/06/16 09:32:42 doerfler Exp $
 */

#ifndef _LTI_FUND_SOLVER_LMS_H_
#define _LTI_FUND_SOLVER_LMS_H_

//TODO: include only those files which are needed in this header!!

#include "ltiEigenSystem.h"
#include "ltiPointList.h"
#include "ltiImage.h"
#include "ltiTransform.h"

namespace lti {

  /**
   * This Algorithm determines the fundamental matrix from point
   * correnspondences in two images.
   *
   * It is the least median of squares method (LMS).
   *
   * Algorithm outline:
   *
   * For each iteration
   *
   * -# choose a subset of 8 points randomly
   *    the algorithm also works with more than 8 points, what could
   *    be interesting to increase the accuracy of the result
   *    when the used samples are all inliers
   *    (sub-pixel-accuracy)
   *
   * -# estimate the fundamental matrix for this subset
   *    using singular value decomposition
   *
   * -# calculate the residual for all samples
   *    (we have choosen to take the distance from the
   *    epipolar line in image 2)
   *
   * -# take the median of the resulting distances and compare it with
   *    the best previous median (and maybe swap)
   *    it would also be possible to count the number of
   *    conform samples and take the maximum of them
   *    this is called RANSAC
   *
   * This is repeated until a threshold for the median of the
   * residuals or the maximum number of iterations is reached
   *
   * enforce the rank 2 constraint of the resulting fmatrix
   */

  class fundamentalMatrixSolverLMS : public transform {
  public:
    /**
     * the parameters for the class fundamentalMatrixSolverLMS
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

      //TODO: comment the parameters of your functor
      // If you add more parameters manually, do not forget to do following:
      // 1. indicate in the default constructor the default values
      // 2. make sure that the copy member also copy your new parameters
      // 3. make sure that the read and write members also read and
      //    write your parameters


      /**
       * gives the maximum nuber of trials
       */
      int numTrials;

      /**
       * the threshold for the minimal median of the distances of the
       * points to their epipolar lines
       */
      double threshold;

    };

    /**
     * default constructor
     */
    fundamentalMatrixSolverLMS();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    fundamentalMatrixSolverLMS(const fundamentalMatrixSolverLMS& other);

    /**
     * destructor
     */
    virtual ~fundamentalMatrixSolverLMS();

    /**
     * returns the name of this type ("fundamentalMatrixSolverLMS")
     */
    virtual const char* getTypeName() const;

    //TODO: comment your apply methods!

    /**
     * Computes the fundamental matrix assuming the the corresponding
     * points occupies the same relative position in the lists l1 and l2.
     *
     * @param l1 first list of points.  It must have the same length t
     * @param l2 second list of points
     * @param fMatrix the resulting fundamental matrix.  It will be a
     *                a 3x3 matrix, the fundamental matrix
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const tpointList<double>& l1,
	       const tpointList<double>& l2,
	       matrix<double>& fMatrix) const;

    /**
     * operates on the given %parameter.
     * integer vesion (data is converted to floatingpoint)
     * @return true if apply successful or false otherwise.
     */
    bool apply(const pointList& l1,
	       const pointList& l2,
	       matrix<double>& fMatrix) const;


    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    fundamentalMatrixSolverLMS& copy(const fundamentalMatrixSolverLMS& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    fundamentalMatrixSolverLMS& operator=(const fundamentalMatrixSolverLMS& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.


    /**
     *  builds the pointMatrix from the point-correspondences
     *  needed for the calculation of the residual
     */
    void buildPointMatrix(const tpointList<double>& l1,
			  const tpointList<double>& l2,
			  matrix<double> &pointMatrix) const;

    /**
     *  calculates the residual for each point correspondence
     *  in our case the residual is the distance of the point
     *  to its corresponding epipolar line
     */
    double calcResidual(const matrix<double> &pointMatrix,
			const matrix<double> &fundMatrix,
			vector<double> &resid) const;


  protected:
    void buildMomentMatrix(const matrix<double> &pointMatrix,
			   matrix<double> &momentMatrix) const;

    void buildMomentMatrixRandom(const matrix<double> &pointMatrix,
				 const int numSamples,
				 matrix<double> &momentMatrix,
				 vector<int> &select) const;

    void calcFundMat(const matrix<double> &momentMatrix,
		     matrix<double> &fundMatrix) const;

  };
}

#endif

