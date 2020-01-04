/*
 * Copyright (C) 1998 - 2004
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
 * file .......: ltiFMatrixEstimatorBase.h
 * authors ....: Stephanus Hendradji
 * organization: LTI, RWTH Aachen
 * creation ...: 15.7.2004
 * revisions ..: $Id: ltiFMatrixEstimatorBase.h,v 1.9 2005/06/16 09:32:42 doerfler Exp $
 */

#ifndef _LTI_F_MATRIX_ESTIMATOR_BASE_H_
#define _LTI_F_MATRIX_ESTIMATOR_BASE_H_

#include "ltiTransformEstimator.h"
#include "ltiUnifiedSVD.h"
#include <limits>
#include "ltiMath.h"

#ifdef _LTI_MSC_VER
#pragma warning(disable:4800)
#endif

#undef _LTI_DEBUG
//#define _LTI_DEBUG 1
#ifdef _LTI_DEBUG
using std::endl;
#endif

#include "ltiDebug.h"

namespace lti {

  /**
   * A base class for estimating the fundamental matrix with a least squares
   * approach. This class is here because a simultanous estimator for the
   * fundamental matrix and a radial distortion is planned.
   *
   * A note for developers of derived fmatrix classes: The header of this class
   * contains an hidden template base class for template helpers. This template
   * base helper must not be used by any classes other than least squares
   * fundamental matrix estimators, though. Therefore it is not included in the
   * doxygen documentation.
   */
  class fMatrixEstimatorBase : public transformEstimator {
  public:
    /**
     * The parameters for the class fMatrixEstimatorBase
     */
    class parameters : public transformEstimator::parameters {
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
       * The rank 2 constraint may be enforced by the frobenius norm or
       * T-parameterization.
       */
      enum eRank2Type { Frobenius, TParameterization };
      
      /**
       * Several ways exist to enforce the rank 2 constraint.
       * E.g. the fundamental matrix may be replaced by the closest fundamental
       * matrix under the frobenius norm. A different approach is to replace
       * one row and one column by a linear combination of the other two. This
       * is called T-parameterization. The frobenius norm is computational more
       * efficient whereas T-parameterization is reported to produce more 
       * accurate results, e.g. when combined with a maximum likelyhood
       * estimator. But with an inaccurate fmatrix estimate the
       * Frobenius norm works better.
       * Default: Frobenius.
       */
      eRank2Type rank2Enforcement;

      /**
       * The distance measure to be used when computing the residual: Sampson
       * or epipolar distance.
       */
      enum eDistanceType { Sampson, Epipolar };

      /**
       * The distance measure to be used when computing the residual. The
       * Sampson distance is reported to be superior when iteratively refining
       * the fundamental matrix (e.g. Hartley, Zisserman: 3D Computer Vision),
       * whereas the epipolar distance is equivalent to the residual.
       * Default: Epipolar.
       */
      eDistanceType distanceMeasure;

    };

    /**
     * Default constructor
     */
    fMatrixEstimatorBase();

    /**
     * Construct a functor using the given parameters
     */
    fMatrixEstimatorBase(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    fMatrixEstimatorBase(const fMatrixEstimatorBase& other);

    /**
     * Destructor
     */
    virtual ~fMatrixEstimatorBase();

    /**
     * Returns the name of this type ("fMatrixEstimatorBase")
     */
    virtual const char* getTypeName() const;

 
    /**
     * Estimates a transform from the supplied point sets, where all
     * points are considered. Please implement efficient code using
     * iterators here. Not all robust estimators use a random sampling
     * approach. Some estimators consider all input points and weight them
     * according to their deviation from the transform computed at the prior
     * iteration.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src  matrix<ipoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param dest fvector the estimated transform.
     * @return true if the transform could be estimated and false otherwise.
     */
    virtual bool apply(const matrix<ipoint>& src,
                       fvector& dest) const = 0;

    /**
     * Estimates a transform from the supplied point sets, where all
     * points are considered. Usually this method calls the apply without
     * the residual first, and then computes the residual.
     * @param src  matrix<ipoint> with the point sets. All points of the same
     *             image stand in a row. The correspondences in another image
     *             stand in the according columns.  All points of one point
     *             set give a matrix row, whereas all elements of a specific
     *             correspondence stand in a matrix column.
     * @param dest fvector the estimated transform.
     * @param error fvector containing the deviation of each point from the
     *              estimated transform. Usually this is the residual or
     *              elementwise squared residual.
     * @return true if the transform could be estimated and false otherwise.
     */
    virtual bool apply(const matrix<ipoint>& src,
                       fvector& dest, fvector& error) const = 0;

    /**
     * Estimates a transform from the supplied point sets, whereby only
     * the points specified in the index vector are considered. This method
     * is used by robost estimators using a monte carlo approach.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src  matrix<ipoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param dest fvector the estimated transform.
     * @param indices ivector with the indices of the relevant points.
     * @param numCorrespondences the first numCorrespondences indices are
     *                            considered.
     * @return true if the transform could be estimated and false otherwise.
     */
    virtual bool apply(const matrix<ipoint>& src,
                       fvector& dest,
                       const ivector& indices,
                       int numCorrespondences) const = 0;

    /**
     * Estimates a transform from the supplied point sets, whereby only
     * the points specified in the index vector are considered. Usually this
     * method calls the apply without the residual first, and then computes
     * the residual.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src  matrix<ipoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param dest fvector the estimated transform.
     * @param error fvector containing the deviation of each point from the
     *              estimated transform. Usually this is the residual or
     *              elementwise squared residual. All correspondences are
     *              considered, discarding the valid indices.
     * @param indices ivector with the indices of the relevant points.
     * @param numCorrespondences the first numCorrespondences indices are
     *                            considered.
     * @return true if the transform could be estimated and false otherwise.
     */
    virtual bool apply(const matrix<ipoint>& src,
                       fvector& dest, fvector& error,
                       const ivector& indices,
                       int numCorrespondences) const = 0;

    /**
     * Estimates a transform from the supplied point sets, where all
     * points are considered. Please implement efficient code using
     * iterators here. Not all robust estimators use a random sampling
     * approach. Some estimators consider all input points and weight them
     * according to their deviation from the transform computed at the prior
     * iteration.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src  matrix<fpoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param dest fvector the estimated transform.
     * @return true if the transform could be estimated and false otherwise.
     */
    virtual bool apply(const matrix<fpoint>& src,
                       fvector& dest) const = 0;

    /**
     * Estimates a transform from the supplied point sets, where all
     * points are considered. Usually this method calls the apply without
     * the residual first, and then computes the residual.
     * @param src  matrix<fpoint> with the point sets. All points of the same
     *             image stand in a row. The correspondences in another image 
     *             stand in the according columns.
     *             All points of one point set give a matrix row, whereas all
     *             elements of a specific correspondence stand in a matrix
     *             column.
     * @param dest fvector the estimated transform.
     * @param error fvector containing the deviation of each point from the
     *              estimated transform. Usually this is the residual or
     *              elementwise squared residual.
     * @return true if the transform could be estimated and false otherwise.
     */
    virtual bool apply(const matrix<fpoint>& src,
                       fvector& dest, fvector& error) const = 0;

    /**
     * Estimates a transform from the supplied point sets, whereby only
     * the points specified in the index vector are considered. This method
     * is used by robost estimators using a monte carlo approach.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src  matrix<dpoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param dest fvector the estimated transform.
     * @param indices ivector with the indices of the relevant points.
     * @param numCorrespondences the first numCorrespondences indices are
     *                            considered.
     * @return true if the transform could be estimated and false otherwise.
     */
    virtual bool apply(const matrix<fpoint>& src,
                       fvector& dest,
                       const ivector& indices,
                       int numCorrespondences) const = 0;

    /**
     * Estimates a transform from the supplied point sets, whereby only
     * the points specified in the index vector are considered. Usually this
     * method calls the apply without the residual first, and then computes
     * the residual.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src  matrix<fpoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param dest fvector the estimated transform.
     * @param error fvector containing the deviation of each point from the
     *              estimated transform. Usually this is the residual or
     *              elementwise squared residual. All correspondences are
     *              considered, discarding the valid indices.
     * @param indices ivector with the indices of the relevant points.
     * @param numCorrespondences the first numCorrespondences indices are
     *                            considered.
     * @return true if the transform could be estimated and false otherwise.
     */
    virtual bool apply(const matrix<fpoint>& src,
                       fvector& dest, fvector& error,
                       const ivector& indices,
                       int numCorrespondences) const = 0;

    /**
     * Estimates a transform from the supplied point sets, where all
     * points are considered. Please implement efficient code using
     * iterators here. Not all robust estimators use a random sampling
     * approach. Some estimators consider all input points and weight them
     * according to their deviation from the transform computed at the prior
     * iteration.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src  matrix<dpoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param dest dvector the estimated transform.
     * @return true if the transform could be estimated and false otherwise.
     */
    virtual bool apply(const matrix<dpoint>& src,
                       dvector& dest) const = 0;

    /**
     * Estimates a transform from the supplied point sets, where all
     * points are considered. Usually this method calls the apply without
     * the residual first, and then computes the residual.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src  matrix<dpoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param dest dvector the estimated transform.
     * @param error dvector containing the deviation of each point from the
     *              estimated transform. Usually this is the residual or
     *              elementwise squared residual.
     * @return true if the transform could be estimated and false otherwise.
     */
    virtual bool apply(const matrix<dpoint>& src,
                       dvector& dest, dvector& error) const = 0;

    /**
     * Estimates a transform from the supplied point sets, whereby only
     * the points specified in the index vector are considered. This method
     * is used by robost estimators using a monte carlo approach.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src  matrix<dpoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param dest dvector the estimated transform.
     * @param indices which rows of the src matrix have to be considered.
     * @param numCorrespondences the first numCorrespondences indices are
     *                            considered.
     * @return true if the transform could be estimated and false otherwise.
     */
    virtual bool apply(const matrix<dpoint>& src,
                       dvector& dest,
                       const ivector& indices,
                       int numCorrespondences) const = 0;

    /**
     * Estimates a transform from the supplied point sets, whereby only
     * the points specified in the index vector are considered. Usually this
     * method calls the apply without the residual first, and then computes
     * the residual.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src  matrix<dpoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param dest dvector the estimated transform.
     * @param error dvector containing the deviation of each point from the
     *              estimated transform. Usually this is the residual or
     *              elementwise squared residual.
     * @param indices ivector with the indices of the relevant points.
     * @param numCorrespondences the first numCorrespondences indices are
     *                            considered.
     * @return true if the transform could be estimated and false otherwise.
     */
    virtual bool apply(const matrix<dpoint>& src,
                       dvector& dest, dvector& error,
                       const ivector& indices,
                       int numCorrespondences) const = 0;

    /**
     * Compute the residual for the given correspondences and transformation
     * vector.
     * @param src matrix<fpoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param transform fvector with the transformation
     * @param dest      fvector with the residual
     * @return true on success and false otherwise.
     */
    virtual bool computeResidual(const matrix<fpoint >& src,
                                    const fvector& transform,
                                    fvector& dest) const = 0;

    /**
     * Compute the residual for the given correspondences and transformation
     * vector.
     * @param src matrix<dpoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param transform dvector with the transformation
     * @param dest      dvector with the residual
     * @return true on success and false otherwise.
     */
    virtual bool computeResidual(const matrix<dpoint >& src,
                                    const dvector& transform,
                                    dvector& dest) const = 0;
    
    /**
     * Returns the minimum number of correspondences required to estimate
     * the transform.
     */
    virtual int minNumberCorrespondences() const = 0;

    /**
     * Returns the mininum dimension of a correspondence,
     * e.g. the minimum dimension of a correspondence pair is 2.
     * Each derived transform estimator only works on correspondences of
     * priori defined dimensions.
     */
    virtual int minCorrespondenceDimension() const = 0;

    /**
     * Returns the maximum dimension of a correspondence,
     * e.g. the maximum dimension of a correspondence pair is 2, whereas
     * transformEstimator running on n-tuples may allow an infinite number.
     * Each derived transform estimator only works on correspondences of
     * priori defined dimensions.
     */
    virtual int maxCorrespondenceDimension() const = 0;

    /**
     * A transform estimated on normalized data usually differs from
     * the transform of the original data. Considering the normalization
     * performed this methods computes the transform which applies to the
     * original data.
     * @param srcdest the normalized transform. The result will be left here
     *     too.
     * @param scale a vector containing the scale applied to each point set.
     * @param shift a vector containing the shift of each scaled point set.
     */
    virtual bool denormalize(fvector& srcdest,
                             const vector<fpoint>& scale,
                             const vector<fpoint>& shift) const = 0;
    
    /**
     * A transform estimated on normalized data usually differs from
     * the transform of the original data. Considering the normalization
     * performed this methods computes the transform which applies to the
     * original data.
     * @param srcdest the normalized transform. The result will be left here
     *     too.
     * @param scale a vector containing the scale applied to each point set.
     * @param shift a vector containing the shift of each scaled point set.
     */
    virtual bool denormalize(dvector& srcdest,
                             const vector<dpoint>& scale,
                             const vector<dpoint>& shift) const = 0;  

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    fMatrixEstimatorBase& copy(const fMatrixEstimatorBase& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    fMatrixEstimatorBase& operator=(const fMatrixEstimatorBase& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const = 0;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

  };

  /*
   *  A small template class that provides methods to enforce the
   *  singularaty of the fundamental matrix and other things common to
   *  fundamental matrix esimators.
   *
   *  Actually this class is at the level of a helper class,
   *  but it is required by several helper templates of fundamental estimators.
   *  Therefore this class is here and documented properly.
   */
  template<class T, class U>
  class fMatBaseHelper {
  public:
    fMatBaseHelper(const fMatrixEstimatorBase::parameters par) :
      computeSqError_   ( par.computeSqError ),
      rank2Enforcement_ ( par.rank2Enforcement),
      distanceMeasure_ ( par.distanceMeasure ) {
    };

    ~fMatBaseHelper() {
    };

    /*
     * To remember error messages.
     */
    std::string statusString;

  protected:

    /*
     * Compute a squared error for efficiency?
     */
    bool computeSqError_;

    /*
     * How shall we enforce singularity?
     */
    fMatrixEstimatorBase::parameters::eRank2Type rank2Enforcement_;

    /*
     * Which error measure is used?
     */
    bool distanceMeasure_;

    /*
     * Initialize the design matrix A for fundamental matrix computation.
     * As only A_transposed * A is processed further on, it is computed
     * directly from the point correspondences, without setting up A first.
     * CAUTION: returns transposed At_A for efficiency
     */
    void initialize(const matrix<tpoint<U> >& src, matrix<T>& At_A) const;

    /*
     * Initialize A_transposed * A as described above. But this time only
     * a subset of the correspondences are used. This subset is specified
     * by the first n entries of the index vector.
     * CAUTION: returns transposed At_A for efficiency
     */
    void initialize(const matrix<tpoint<U> >& src, const ivector& indices,
                    const int numCorrespondences, matrix<T>& At_A) const;

    /*
     * Enforce singularity, i.e. rank 2. How this will be done is specified
     * in the parameters.
     * CAUTION: input is transposed fmatrix, output is fmatrix
     */
    bool enforceSingularity(vector<T>& src);

    /*
     * Enforces singularity, i.e. rank 2, by frobenius norm.
     * CAUTION: input is transposed fmatrix, output is fmatrix
     */
    bool frobenius(vector<T>& src) const;

    /*
     * Enforces rank 2 by T-parameterization.
     * CAUTION: input is transposed fmatrix, output is fmatrix
     */
    bool tParameterization(vector<T>& src);

  private:
    /*
     * Compute the tParameterization for one permutation of the fmatrix.
     */
    bool oneTParameterization(matrix<T>&  src,
                              vector<T>& lambda, vector<T>& mu, T& norm);

    //todo: the epipole methods should be moved into a fmatrix-class,
    //      which provides useful methods when working with fmatrices.
    // e.g. compute epipoles, compute epipolar lines, extract homography,
    // extract trivial set of camera projection matrices.

    /*
     * Compute the epipoles by solving F*e = 0 with 3 degrees of freedom
     * and setting the 3rd coordinate equal 1 afterwards.
     */
    bool computeEpipoles3Dof(const matrix<T>& src, 
                         vector<T>& epipole1, vector<T>& epipole2);

    /*
     * Normalize the epipole (3rd coordinate equals 1)
     */
    bool normalizeEpipole(vector<T>& srcdest) const;

    /*
     * Compute the epipoles by solving [F1 F2] *e = [F3], i.e. with
     * 2 degrees of freedom. This method is more accurate for inaccurate
     * fundamental matrices (todo: verify on more data).
     */
    bool computeEpipoles2Dof(const matrix<T>& src, 
                     vector<T>& epipole1, vector<T>& epipole2);

    /*
     * Solve the equation system A * x = b by singular value decomposition,
     * i.e. x = v * w_inverse * u_transposed * b.
     */
    bool solveAXEqualsB(const matrix<T>& src, vector<T>& b, vector<T>& dest);
  };

  //------------------------
  // singularity enforcement
  //------------------------

  template<class T, class U>
  inline bool fMatBaseHelper<T,U>::enforceSingularity(vector<T>& src) {

    if ( rank2Enforcement_ == fMatrixEstimatorBase::parameters::Frobenius ) {
      return frobenius(src);
    } else {
      return tParameterization(src);
    }
  }

  template<class T, class U>
  inline bool fMatBaseHelper<T,U>::frobenius(vector<T>& src) const {

    matrix<T> fmat;
    fmat.useExternData(3,3,&src.at(0));
    _lti_debug(" old fmatrix:\n" << fmat << std::endl);

    matrix<T> u;    //eigenvectors
    matrix<T> v;    //left hand singular vectors
    vector<T> w;    //eigenvalues
    typename unifiedSVD<T>::parameters svdPara;
    svdPara.sort = true;
    svdPara.transposeU = true;  //faster; Ut is returned
    unifiedSVD<T> svd ( svdPara );
    if (!svd.apply(fmat,u,w,v)) {
      return false;
    }

    _lti_debug(" frobenius variances are:\n" << w
               << "\n ...setting 3rd value to 0 now\n");

    //set the 3rd variance to zero and new fmatrix is u*diag(w)*v
    T& s1 = w.at(0);
    T& s2 = w.at(1);

    //because u is transposed due to efficiency
    typename matrix<T>::iterator it ( u.begin() );
    *it     *= s1;  *(++it) *= s1;  *(++it) *= s1;
    *(++it) *= s2;  *(++it) *= s2,  *(++it) *= s2;
    *(++it) *= T(0);  *(++it) *= T(0);  *(++it) *= T(0);
    v.multiply(u);

    //copy back into src; OK because fmat not used anymore
    //necessary because v is local
    //v.transpose();
    v.detach(src);
  
    _lti_debug(" new fmatrix:\n" << src << std::endl);

    return true;
  }

  //===================
  // T-parameterization
  //===================

  template<class T, class U>
  inline bool fMatBaseHelper<T,U>::tParameterization(vector<T>& src) {
 
    //maximize over all possible T-parameterizations
    //CAUTION: the input fmatrix is transposed

    //the implementation could be optimized if computing the epipoles
    // by computeEpipole3Dof. In this case they need only be computed
    // once.

    //possible optimization for computeEpipole2Dof:
    //compute the one epipole for the 3 permutions of the rows and the
    //other for the 3 permutations of the columns
    
    //_lti_debug(" *** tpara *** old fmatrix " << endl << src << endl);
    T winningNorm ( T(0) );
    matrix<T> fmat ( false, 3, 3 );
    for ( int row (0); row<3; ++row ) {
      for ( int col (0); col<3; ++col ) {
        _lti_debug(" row " << row << " col " << col << endl);
        //shuffel the fundamental matrix
        // such that the last row and column are parameterized
        //caution: the input fundamenal matrix is tranposed !!!
        matrix<T> shuffelF (false,3,3);
        for ( int r ( 0 ); r < 3; ++r ) {
          const int usedCol ( (row + r + 1) % 3 );
          typename vector<T>::iterator it ( shuffelF.getRow(r).begin() );
          for ( int c ( 0 ); c < 3; ++c, ++it ) {
            const int usedRow ( ((col + c + 1) % 3) * 3 );
            *it = src.at(usedCol + usedRow );
          }//for c
        }//for r
        //_lti_debug("shuffeled at " << row << " " << col << endl
        //   << shuffelF << endl);

        T norm;
        vector<T> lambda, mu;
        if ( oneTParameterization(shuffelF,lambda, mu, norm) &&
             norm > winningNorm ) {
          //remember the new fundamental matrix - maximize norm
          winningNorm = norm;
          //init. parameterized row
          vector<T>& parRowVect = fmat.getRow(row);//parameterized row
          typename vector<T>::iterator fit ( parRowVect.begin() );
          *fit = T(0); *(++fit) = T(0); *(++fit) = T(0);
          for ( int r ( 0 ); r < 2; ++r ) {
            int usedRow ( ( row + r + 1) % 3 );
            vector<T>& newVect = fmat.getRow(usedRow);
            newVect.at(col) = T(0);
            typename vector<T>::iterator it ( shuffelF.getRow(r).begin() );
            //parameterized column
            for ( int c ( 0 ); c < 2; ++c, ++it ) {
              const int usedCol ( (col + c + 1) % 3 );
              newVect.at(usedCol) = *it;
              newVect.at(col) -= lambda.at(c) * *it;
              parRowVect.at(usedCol) -= mu.at(r) * *it;
            }//for c 
            //parametrized by row and column
            parRowVect.at(col) -=  mu.at(r) * newVect.at(col);
          }//for r
          //_lti_debug(" updated fmatrix " << row << " " << col 
          //     << " val " << norm << endl << fmat << endl);
        }
      }
    }

    //move the new fmatrix into the src (without copying)
    fmat.detach(src);

    return true;
  }

  template<class T, class U>
  inline bool fMatBaseHelper<T,U>
    ::oneTParameterization(matrix<T>&  src,
                           vector<T>& lambda, vector<T>& mu, T& norm) {

    const T epsilon ( std::numeric_limits<T>::epsilon() );

    //compute the row- and columnwise parameterizations
    //todo: which is better? with an initial FMatrix: 2Dof is more accurate;
    //      both result in poorer epipolar lines than frobenius
    //if ( !computeEpipoles2Dof(src, mu, lambda) ) {
    if ( !computeEpipoles3Dof(src, mu, lambda) ) {
      statusString += "\nCould not compute epipoles";
      return false;
    }

    //normalize by setting the largest value 1
    //thus the conditioning below will be (approx.) scale invariant
    typename matrix<T>::iterator it ( src.begin() );
    T maxEntry ( abs(*it) );
    maxEntry = max ( maxEntry, abs(*(++it)) );
    ++it; //igonre 3rd column
    maxEntry = max ( maxEntry, abs(*(++it)) );
    maxEntry = max ( maxEntry, abs(*(++it)) );
    //_lti_debug(" maxEntry " << maxEntry << " src " << endl << src << endl);

    if ( maxEntry > -epsilon && maxEntry < epsilon ) {
      statusString += "\nAll 4 FMatrix values are 0";
      return false;
    }
    //normalization is performed parallel to conditioning
    //choose best conditioned Jacobian matrix,
    //i.e. compute norm of the vector with determinants of the 8x8 submatrices
    it = src.begin();
    *it /= maxEntry;
    const T& f1 = *it;
    *(++it) /= maxEntry;
    const T& f2 = *it;
    ++it; //igonre 3rd column
    *(++it) /= maxEntry;
    norm = f2 * *(it);
    *(++it) /= maxEntry;
    norm += f1 * *(it);
    norm *= norm;

//     typename vector<T>::const_iterator lit ( lambda.begin() );
//     typename vector<T>::const_iterator mit ( mu.begin() );
//     norm += sqrt( sqr(*lit) + sqr(*(++lit)) + 1)
//       * sqrt( sqr(*mit) + sqr(*(++mit)) + 1);

    const T l0=lambda.at(0);
    const T l1=lambda.at(1);
    const T m0=mu.at(0);
    const T m1=mu.at(1);
    norm += sqrt(l0*l0 + l1*l1 + 1) * sqrt(m0*m0 + m1*m1 + 1);

    _lti_debug(" lambda " << lambda << " mu " << mu << endl);
    //_lti_debug(" norm and fmat (7-point) " << norm << endl << src << endl);

    return true;
  }

  //todo: which is epipole1 and epipole2
  //for compatibility with the method below this would be inversed ???
  template<class T, class U>
    inline bool fMatBaseHelper<T,U>::computeEpipoles2Dof(const matrix<T>& src, 
                                                         vector<T>& epipole1,
                                                         vector<T>& epipole2) {

    //seperate fmatrix into first two and third column
    matrix<T> a ( false, 3, 2);
    vector<T> b ( false, 3);
    typename matrix<T>::iterator aend ( a.end() );
    typename matrix<T>::iterator ait ( a.begin() );
    typename vector<T>::iterator bit ( b.begin() );
    typename matrix<T>::const_iterator fit ( src.begin() );
    for ( ; ait!=aend; ++ait,++bit,++fit ) {
      *ait = *fit;
      *(++ait) = *(++fit);
      *bit = *(++fit);
    }
    if ( !solveAXEqualsB(a,b,epipole2) ) {
      return false;
    }

    //seperate fmatrix into first two and third row
    a.copy(src, 0, 1, 0, 2);
    a.transpose();
    b.copy(src.getRow(2));
    if ( !solveAXEqualsB(a,b,epipole1) ) {
      return false;
    }

    return true;
  }

  template<class T, class U>
  inline bool fMatBaseHelper<T,U>
    ::solveAXEqualsB(const matrix<T>& src, vector<T>& b, vector<T>& dest) {
 
    matrix<T> u;    //eigenvectors
    matrix<T> v;    //left hand singular vectors
    vector<T> w;    //eigenvalues
    typename unifiedSVD<T>::parameters svdPara;
    svdPara.sort = true;
    svdPara.transposeU = true;

    unifiedSVD<T> svd ( svdPara );
    if (!svd.apply(src,u,w,v)) {
      statusString += svd.getStatusString();
      return false;
    }
    //invert w
    typename vector<T>::iterator it ( w.begin() );
    typename vector<T>::iterator end ( w.end() );
    for ( ; it!=end; ++it ) {
      const T epsilon ( std::numeric_limits<T>::epsilon() );
      if ( *it > -epsilon && *it < epsilon ) {
        *it = T(0);
      } else {
        *it = - T(1) / *it; //change sign because we solve for -b
      }
    }
    u.multiply(b);  //leaves the result in b
    w.emultiply(b); //elementwise multiply
    v.multiply(w);  //leaves the result in w
    w.detach(dest);

    return true;
  }

  template<class T, class U>
  inline bool fMatBaseHelper<T,U>
    ::computeEpipoles3Dof(const matrix<T>& src, 
                          vector<T>& epipole1, vector<T>& epipole2) {
    
    matrix<T> u;    //eigenvectors
    matrix<T> v;    //left hand singular vectors
    vector<T> w;    //eigenvalues
    typename unifiedSVD<T>::parameters svdPara;
    svdPara.sort = true;
    svdPara.transposeU = true;  //faster; Ut is returned
    unifiedSVD<T> svd ( svdPara );
    if (!svd.apply(src,u,w,v)) {
      statusString += svd.getStatusString();
      return false;
    }

    // epipole2_t * F * epipole1 = 0

    //epipole1 is the left eigenvector with least eigenvalue
    //the u-matrix is transposed, so use the last row
    epipole1.copy(u.getRow(u.lastRow()));

    //epipole2 is the right eigenvector with the least eigenvalue
    //get the last column
    epipole2.resize( 3, T(0), false, false );
    typename vector<T>::iterator it  ( epipole2.begin() );
    typename matrix<T>::iterator vit ( v.begin() );
    vit += 2; //move to last column
    *it = *vit; vit += 3; //move to next row
    *(++it) = *vit; vit += 3; //move to next row
    *(++it) = *vit;
  
    //_lti_debug(" u " << endl << u << endl << " v " << endl << v << endl);
    _lti_debug(" epipole1 " << epipole1 <<
               " epipole2 " << epipole2 << endl);
    
    //we do not normalize the epipoles if they are the plane of inifinity
    return normalizeEpipole(epipole1) && normalizeEpipole(epipole2);
  }

  template<class T, class U>
    inline bool fMatBaseHelper<T,U>
    ::normalizeEpipole(vector<T>& srcdest) const {
  
    const T epsilon ( std::numeric_limits<T>::epsilon() );
    const T& tmp = srcdest.at(2);
    if ( tmp < -epsilon || tmp > epsilon ) {
      typename vector<T>::iterator it ( srcdest.begin() );
      *it /= tmp;
      *(++it) /= tmp;
      *(++it ) = T(1);
      return true;
    } 

    //if e[3] is 0 then the columns/rows corresponding to e[0] and e[1] are
    //lineary dependent -> epipole at plane of infinity
    return false;
  }
  
  //------------------
  //the initialization
  //------------------

  template<class T, class U>
  inline void fMatBaseHelper<T,U>::initialize(const matrix<tpoint<U> >& src,
                                              matrix<T>& AtA) const {

    //initialize A_transponed * A
    //AtA is composed of 6 symmetric submatrices
    //        | m11  m21  m31 |
    // AtA =  | m21  m22  m32 |
    //        | m31  m32  m33 |

    //---------------------------------------------------
    //references for fast access of the vector's elements
    //---------------------------------------------------
    AtA.resize(9,9, T(0), false, true);  //initialize all with 0
    typename matrix<T>::iterator it ( AtA.begin() );
    //1st row
    T& m11_0 = *it;
    T& m11_1 = *(++it);
    T& m11_2 = *(++it);
    T& m21_0 = *(++it);
    T& m21_1 = *(++it);
    T& m21_2 = *(++it);
    T& m31_0 = *(++it);
    T& m31_1 = *(++it);
    T& m31_2 = *(++it);

    //2nd row
    T& a21   = *(++it);       //duplicate due to symmetry
    T& m11_3 = *(++it);
    T& m11_4 = *(++it);
    T& a24   = *(++it);       //duplicate due to symmetry
    T& m21_3 = *(++it);
    T& m21_4 = *(++it);
    T& a27   = *(++it);       //duplicate due to symmetry
    T& m31_3 = *(++it);
    T& m31_4 = *(++it);

    //3rd row
    T& a31   = *(++it);      //duplicate due to symmetry
    T& a32   = *(++it);      //duplicate due to symmetry
    T& m11_5 = *(++it);
    T& a34   = *(++it);      //duplicate due to symmetry
    T& a35   = *(++it);      //duplicate due to symmetry
    T& m21_5 = *(++it);
    T& a37   = *(++it);      //duplicate due to symmetry
    T& a38   = *(++it);      //duplicate due to symmetry
    T& m31_5 = *(++it);
    
    //4th row
    T& a41   = *(++it);      //duplicate due to symmetry
    T& a42   = *(++it);      //duplicate due to symmetry
    T& a43   = *(++it);      //duplicate due to symmetry
    T& m22_0 = *(++it);
    T& m22_1 = *(++it);
    T& m22_2 = *(++it);    
    T& m32_0 = *(++it);
    T& m32_1 = *(++it);
    T& m32_2 = *(++it);  

    //5th row
    T& a51   = *(++it);      //duplicate due to symmetry
    T& a52   = *(++it);      //duplicate due to symmetry
    T& a53   = *(++it);      //duplicate due to symmetry
    T& a54   = *(++it);      //duplicate due to symmetry
    T& m22_3 = *(++it);
    T& m22_4 = *(++it);    
    T& a57   = *(++it);      //duplicate due to symmetry
    T& m32_3 = *(++it);
    T& m32_4 = *(++it);

    //6th row
    T& a61   = *(++it);      //duplicate due to symmetry
    T& a62   = *(++it);      //duplicate due to symmetry
    T& a63   = *(++it);      //duplicate due to symmetry
    T& a64   = *(++it);      //duplicate due to symmetry
    T& a65   = *(++it);      //duplicate due to symmetry
    T& m22_5 = *(++it);
    T& a67   = *(++it);      //duplicate due to symmetry
    T& a68   = *(++it);      //duplicate due to symmetry
    T& m32_5 = *(++it);

    //7th row
    T& a71   = *(++it);      //duplicate due to symmetry
    T& a72   = *(++it);      //duplicate due to symmetry
    T& a73   = *(++it);      //duplicate due to symmetry
    T& a74   = *(++it);      //duplicate due to symmetry
    T& a75   = *(++it);      //duplicate due to symmetry
    T& a76   = *(++it);      //duplicate due to symmetry
    T& m33_0 = *(++it);
    T& m33_1 = *(++it);
    T& m33_2 = *(++it);

    //8th row
    T& a81   = *(++it);      //duplicate due to symmetry
    T& a82   = *(++it);      //duplicate due to symmetry
    T& a83   = *(++it);      //duplicate due to symmetry
    T& a84   = *(++it);      //duplicate due to symmetry
    T& a85   = *(++it);      //duplicate due to symmetry
    T& a86   = *(++it);      //duplicate due to symmetry
    T& a87   = *(++it);      //duplicate due to symmetry
    T& m33_3 = *(++it);
    T& m33_4 = *(++it);

    //9th row
    T& a91   = *(++it);      //duplicate due to symmetry
    T& a92   = *(++it);      //duplicate due to symmetry
    T& a93   = *(++it);      //duplicate due to symmetry
    T& a94   = *(++it);      //duplicate due to symmetry
    T& a95   = *(++it);      //duplicate due to symmetry
    T& a96   = *(++it);      //duplicate due to symmetry
    T& a97   = *(++it);      //duplicate due to symmetry
    T& a98   = *(++it);      //duplicate due to symmetry
    *(++it)  = static_cast<T>(src.columns());

    //--------------------
    //init all submatrices
    //--------------------

    const vector<tpoint<U> >& pts1 = src.getRow(0);
    const vector<tpoint<U> >& pts2 = src.getRow(1);
    typename vector<tpoint<U> >::const_iterator end ( pts1.end()   );
    typename vector<tpoint<U> >::const_iterator it1 ( pts1.begin() );
    typename vector<tpoint<U> >::const_iterator it2 ( pts2.begin() );
    
    for (; it1!=end; ++it1, ++it2) {
      //indices 1 and 2 are crossed because we compute a transposed fmatrix
      //due to efficiency of frobenius singularity enforcement
      const T x1 ( static_cast<T>((*it2).x) );
      const T y1 ( static_cast<T>((*it2).y) );
      const T sqX1 ( x1*x1 );
      const T sqY1 ( y1*y1 );
      const T x1Y1 ( x1*y1 );

      const T x2 ( static_cast<T>((*it1).x) );
      const T y2 ( static_cast<T>((*it1).y) );
      const T sqX2 ( x2*x2 );
      const T sqY2 ( y2*y2 );
      const T x2Y2 ( x2*y2 );

      //design matrix after Zisserman (if it1 and it2 were not crossed)
      m11_0 += sqX1 * sqX2;
      m11_1 += x1Y1 * sqX2;
      m11_2 += x1   * sqX2;
      m11_3 += sqY1 * sqX2;
      m11_4 += y1   * sqX2;
      m11_5 += sqX2;
 
      m21_0 += sqX1 * x2Y2;
      m21_1 += x1Y1 * x2Y2;
      m21_2 += x1   * x2Y2;
      m21_3 += sqY1 * x2Y2;
      m21_4 += y1   * x2Y2;
      m21_5 += x2Y2;

      m31_0 += sqX1 * x2;
      m31_1 += x1Y1 * x2;
      m31_2 += x1   * x2;
      m31_3 += sqY1 * x2;
      m31_4 += y1   * x2;
      m31_5 += x2;
  
      m22_0 += sqX1 * sqY2;
      m22_1 += x1Y1 * sqY2;
      m22_2 += x1   * sqY2;
      m22_3 += sqY1 * sqY2;
      m22_4 += y1   * sqY2;
      m22_5 += sqY2;

      m32_0 += sqX1 * y2;
      m32_1 += x1Y1 * y2;
      m32_2 += x1   * y2;
      m32_3 += sqY1 * y2;
      m32_4 += y1   * y2;
      m32_5 += y2;

      m33_0 += sqX1;
      m33_1 += x1Y1;
      m33_2 += x1;
      m33_3 += sqY1;
      m33_4 += y1;
    }

    //-------------------------------------------------
    //initialize symmetric elements of A_transponed * A
    //-------------------------------------------------
    //2nd row
    a21 = m11_1;
    a24 = m21_1;
    a27 = m31_1;
    //3rd row
    a31 = m11_2; a32 = m11_4;
    a34 = m21_2; a35 = m21_4;
    a37 = m31_2; a38 = m31_4;
    
    //4th row
    a41 = m21_0; a42 = m21_1; a43 = m21_2;
    //5th row
    a51 = m21_1;   a52 = m21_3; a53 = m21_4;
    a54 = m22_1;
    a57 = m32_1;
    //6th row
    a61 = m21_2; a62 = m21_4;   a63 = m21_5;
    a64 = m22_2; a65 = m22_4;
    a67 = m32_2; a68 = m32_4;

    //7th row
    a71 = m31_0; a72 = m31_1; a73 = m31_2;
    a74 = m32_0; a75 = m32_1; a76 = m32_2;
    //8th row 
    a81 = m31_1; a82 = m31_3; a83 = m31_4;
    a84 = m32_1; a85 = m32_3; a86 = m32_4;
    a87 = m33_1;
    //9th row
    a91 = m31_2; a92 = m31_4; a93 = m31_5;
    a94 = m32_2; a95 = m32_4; a96 = m32_5;
    a97 = m33_2; a98 = m33_4;

#if defined(_LTI_DEBUG) && (_LTI_DEBUG > 3)
    cout << " At_A " << AtA << endl;
#endif
  }

  template<class T, class U>
  inline void fMatBaseHelper<T,U>
  ::initialize(const matrix<tpoint<U> >& src, const ivector& indices,
               const int numCorrespondences, matrix<T>& AtA) const {
    
    //re-implemented due to efficiency

    //initialize A_transponed * A
    //AtA is composed of 6 symmetric submatrices
    //        | m11  m21  m31 |
    // AtA =  | m21  m22  m32 |
    //        | m31  m32  m33 |

    //---------------------------------------------------
    //references for fast access of the vector's elements
    //---------------------------------------------------
    AtA.resize(9,9, T(0), false, true);  //initialize all with 0
    typename matrix<T>::iterator it ( AtA.begin() );
    //1st row
    T& m11_0 = *it;
    T& m11_1 = *(++it);
    T& m11_2 = *(++it);
    T& m21_0 = *(++it);
    T& m21_1 = *(++it);
    T& m21_2 = *(++it);
    T& m31_0 = *(++it);
    T& m31_1 = *(++it);
    T& m31_2 = *(++it);

    //2nd row
    T& a21   = *(++it);       //duplicate due to symmetry
    T& m11_3 = *(++it);
    T& m11_4 = *(++it);
    T& a24   = *(++it);       //duplicate due to symmetry
    T& m21_3 = *(++it);
    T& m21_4 = *(++it);
    T& a27   = *(++it);       //duplicate due to symmetry
    T& m31_3 = *(++it);
    T& m31_4 = *(++it);

    //3rd row
    T& a31   = *(++it);      //duplicate due to symmetry
    T& a32   = *(++it);      //duplicate due to symmetry
    T& m11_5 = *(++it);
    T& a34   = *(++it);      //duplicate due to symmetry
    T& a35   = *(++it);      //duplicate due to symmetry
    T& m21_5 = *(++it);
    T& a37   = *(++it);      //duplicate due to symmetry
    T& a38   = *(++it);      //duplicate due to symmetry
    T& m31_5 = *(++it);
    
    //4th row
    T& a41   = *(++it);      //duplicate due to symmetry
    T& a42   = *(++it);      //duplicate due to symmetry
    T& a43   = *(++it);      //duplicate due to symmetry
    T& m22_0 = *(++it);
    T& m22_1 = *(++it);
    T& m22_2 = *(++it);    
    T& m32_0 = *(++it);
    T& m32_1 = *(++it);
    T& m32_2 = *(++it);  

    //5th row
    T& a51   = *(++it);      //duplicate due to symmetry
    T& a52   = *(++it);      //duplicate due to symmetry
    T& a53   = *(++it);      //duplicate due to symmetry
    T& a54   = *(++it);      //duplicate due to symmetry
    T& m22_3 = *(++it);
    T& m22_4 = *(++it);    
    T& a57   = *(++it);      //duplicate due to symmetry
    T& m32_3 = *(++it);
    T& m32_4 = *(++it);

    //6th row
    T& a61   = *(++it);      //duplicate due to symmetry
    T& a62   = *(++it);      //duplicate due to symmetry
    T& a63   = *(++it);      //duplicate due to symmetry
    T& a64   = *(++it);      //duplicate due to symmetry
    T& a65   = *(++it);      //duplicate due to symmetry
    T& m22_5 = *(++it);
    T& a67   = *(++it);      //duplicate due to symmetry
    T& a68   = *(++it);      //duplicate due to symmetry
    T& m32_5 = *(++it);

    //7th row
    T& a71   = *(++it);      //duplicate due to symmetry
    T& a72   = *(++it);      //duplicate due to symmetry
    T& a73   = *(++it);      //duplicate due to symmetry
    T& a74   = *(++it);      //duplicate due to symmetry
    T& a75   = *(++it);      //duplicate due to symmetry
    T& a76   = *(++it);      //duplicate due to symmetry
    T& m33_0 = *(++it);
    T& m33_1 = *(++it);
    T& m33_2 = *(++it);

    //8th row
    T& a81   = *(++it);      //duplicate due to symmetry
    T& a82   = *(++it);      //duplicate due to symmetry
    T& a83   = *(++it);      //duplicate due to symmetry
    T& a84   = *(++it);      //duplicate due to symmetry
    T& a85   = *(++it);      //duplicate due to symmetry
    T& a86   = *(++it);      //duplicate due to symmetry
    T& a87   = *(++it);      //duplicate due to symmetry
    T& m33_3 = *(++it);
    T& m33_4 = *(++it);

    //9th row
    T& a91   = *(++it);      //duplicate due to symmetry
    T& a92   = *(++it);      //duplicate due to symmetry
    T& a93   = *(++it);      //duplicate due to symmetry
    T& a94   = *(++it);      //duplicate due to symmetry
    T& a95   = *(++it);      //duplicate due to symmetry
    T& a96   = *(++it);      //duplicate due to symmetry
    T& a97   = *(++it);      //duplicate due to symmetry
    T& a98   = *(++it);      //duplicate due to symmetry
    *(++it)  = static_cast<T>(numCorrespondences);

    //--------------------
    //init all submatrices
    //--------------------

    const vector<tpoint<U> >& pts1 = src.getRow(0);
    const vector<tpoint<U> >& pts2 = src.getRow(1);
    typename ivector::const_iterator iit  ( indices.begin() );
    typename ivector::const_iterator iend ( iit + numCorrespondences );
    for (; iit!=iend; ++iit) {
      //indices 1 and 2 are crossed because we compute a transposed fmatrix
      //due to efficiency of frobenius singularity enforcement
      const tpoint<U>& pt1 = pts2.at(*iit);
      const T x1 ( static_cast<T>(pt1.x) );
      const T y1 ( static_cast<T>(pt1.y) );
      const T sqX1 ( x1*x1 );
      const T sqY1 ( y1*y1 );
      const T x1Y1 ( x1*y1 );

      const tpoint<U>& pt2 = pts1.at(*iit);
      const T x2 ( static_cast<T>(pt2.x) );
      const T y2 ( static_cast<T>(pt2.y) );
      const T sqX2 ( x2*x2 );
      const T sqY2 ( y2*y2 );
      const T x2Y2 ( x2*y2 );

     //design matrix after Zisserman (if it1 and it2 were not crossed)
      m11_0 += sqX1 * sqX2;
      m11_1 += x1Y1 * sqX2;
      m11_2 += x1   * sqX2;
      m11_3 += sqY1 * sqX2;
      m11_4 += y1   * sqX2;
      m11_5 += sqX2;
 
      m21_0 += sqX1 * x2Y2;
      m21_1 += x1Y1 * x2Y2;
      m21_2 += x1   * x2Y2;
      m21_3 += sqY1 * x2Y2;
      m21_4 += y1   * x2Y2;
      m21_5 += x2Y2;

      m31_0 += sqX1 * x2;
      m31_1 += x1Y1 * x2;
      m31_2 += x1   * x2;
      m31_3 += sqY1 * x2;
      m31_4 += y1   * x2;
      m31_5 += x2;
  
      m22_0 += sqX1 * sqY2;
      m22_1 += x1Y1 * sqY2;
      m22_2 += x1   * sqY2;
      m22_3 += sqY1 * sqY2;
      m22_4 += y1   * sqY2;
      m22_5 += sqY2;

      m32_0 += sqX1 * y2;
      m32_1 += x1Y1 * y2;
      m32_2 += x1   * y2;
      m32_3 += sqY1 * y2;
      m32_4 += y1   * y2;
      m32_5 += y2;

      m33_0 += sqX1;
      m33_1 += x1Y1;
      m33_2 += x1;
      m33_3 += sqY1;
      m33_4 += y1;
    }

    //-------------------------------------------------
    //initialize symmetric elements of A_transponed * A
    //-------------------------------------------------
    //2nd row
    a21 = m11_1;
    a24 = m21_1;
    a27 = m31_1;
    //3rd row
    a31 = m11_2; a32 = m11_4;
    a34 = m21_2; a35 = m21_4;
    a37 = m31_2; a38 = m31_4;
    
    //4th row
    a41 = m21_0; a42 = m21_1; a43 = m21_2;
    //5th row
    a51 = m21_1;   a52 = m21_3; a53 = m21_4;
    a54 = m22_1;
    a57 = m32_1;
    //6th row
    a61 = m21_2; a62 = m21_4;   a63 = m21_5;
    a64 = m22_2; a65 = m22_4;
    a67 = m32_2; a68 = m32_4;

    //7th row
    a71 = m31_0; a72 = m31_1; a73 = m31_2;
    a74 = m32_0; a75 = m32_1; a76 = m32_2;
    //8th row 
    a81 = m31_1; a82 = m31_3; a83 = m31_4;
    a84 = m32_1; a85 = m32_3; a86 = m32_4;
    a87 = m33_1;
    //9th row
    a91 = m31_2; a92 = m31_4; a93 = m31_5;
    a94 = m32_2; a95 = m32_4; a96 = m32_5;
    a97 = m33_2; a98 = m33_4;

#if defined(_LTI_DEBUG) && (_LTI_DEBUG > 3)
    cout << " At_A " << AtA << endl;
#endif
  }

}

#include "ltiUndebug.h"
#endif

