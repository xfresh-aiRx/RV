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
 * file .......: ltiBlobEM.h
 * authors ....: Suat Akyol
 * organization: LTI, RWTH Aachen
 * creation ...: 27.5.2002
 * revisions ..: $Id: ltiBlobEM.h,v 1.10 2005/06/16 09:32:41 doerfler Exp $
 */

#ifndef _LTI_BLOB_EM_H_
#define _LTI_BLOB_EM_H_

// include only those files which are needed in this header!!
#include <vector>

#include "ltiImage.h"
#include "ltiVector.h"
#include "ltiMatrix.h"

// include to parent class:
#include "ltiFunctor.h"

namespace lti {

  /**
   * Estimates the position of M overlapping blobs by applying the
   * EM-algorithm and estimating the parameters of a gaussian mixture
   * model that fits the blobs. As input a distance transformed
   * image of the blobs or similar is expected and a vector of ellipses
   * for representing a gaussian distribution component.
   *
   * For a description of the EM-algorithm see Jeff Bilmes "A Gentle
   * Tutorial of the EM Algorithm and its Application to Parameter
   * Estimation for Gaussian Mixture and Hidden Markov Models",
   * Technical Report TR-97-021 by the International Computer
   * Science Institute, Berkeley, Ca.
   *
   * \htmlonly<a name="HINTS"></a>\endhtmlonly
   *
   * For usage of this functor please see the following examples and
   * hints:
   *
   * \li <a href="#ELLIPSES">Preparing the input ellipses</a>
   * \li <a href="#APPLYING">Applying this functor</a>
   * \li <a href="#ERRORS">Possible errors on usage</a>
   *
   * \par Example for preparing ellipses
   * \htmlonly
   * <a name="ELLIPSES"></a>
   * <a href="#HINTS">back</a>
   * \endhtmlonly
   *
   * \code
   *
   *   // Typedef for better readability
   *   typedef lti::blobEM::gaussEllipse TEllipse;
   *
   *   // Vector of ellipses
   *   std::vector<TEllipse>& allEllipses;
   *
   *   // Create one ellipse with center(10,10), eigenvalues 100 and 20
   *   // and 45 degrees orientation
   *   TEllipse oneEllipse(lti::tpoint<double>(10,10),100,20,lti::Pi/4);
   *
   *   // Constrain ellipse properties
   *   oneEllipse.constrainShape = true;
   *
   *   oneEllipse.constrainArea = true;
   *   oneEllipse.areaTolerance = 0.2;
   *
   *   oneEllipse.constrainAngle = true;
   *   oneEllipse.angleTolerance = lti::Pi/8;
   *
   *   // Create another ellipse with the same properties as the first
   *   // but center(50,50). This ellipse will be unconstrained (default)
   *   TEllipse anotherEllipse;
   *
   *   anotherEllipse.fromEllipse(oneEllipse); // is not copy operator!
   *                                           // copies only properties
   *                                           // angle, center, lambda1,2
   *   anotherEllipse.center = lti::tpoint<double>(50,50);
   *
   *   allEllipses.push_back(oneEllipse);
   *   allEllipses.push_back(anotherEllipse);
   *
   * \endcode
   *
   *
   * \par Example for using this functor
   * \htmlonly
   * <a name="APPLYING"></a>
   * <a href="#HINTS">back</a>
   * \endhtmlonly
   *
   * \code
   *
   * // prepare functor
   * lti::blobEM em;
   * lti::blobEM::parameters emParam;
   * emParam.maxIterations = 10;        // number of maximum allowed iterations
   * emParam.convergenceThreshold = -1; // force maximum iterations
   * em.setParameters(emParam);
   *
   * //////////////////////////////////////////////////////////////////////////
   * // Apply with allEllipses but make sure that initialization is good,
   * // since result stongly depends on initialization.
   * // Result will be delivered back in allEllipses. aChnl8 should be a
   * // distance transform of a binary image (see lti::distanceTransform)
   * em.apply(aChnl8,allEllipses);
   *
   * //////////////////////////////////////////////////////////////////////////
   * // Alternatively you can apply without knowing how to initialize by
   * // merely specifying the number of ellipses (here 2). These will be
   * // set randomly then.
   * // You must explicitly request the resulting ellipses afterwards
   * // by calling computeEllipses(...)
   * em.apply(aChnl8,2);
   * em.computeEllipses(allEllipses);
   *
   * //////////////////////////////////////////////////////////////////////////
   * // Another possibility is to control the iterations manually
   * em.initialize(aChnl8,allEllipses);
   * // OR
   * em.initialize(aChnl8,2);
   *
   * // perfom iterations
   * em.iterate();
   * em.iterate();
   * while (em.getQDiff()>0) {
   *   // the Q value is maximized by EM. QDiff will be > 0
   *   // As long as Q increases. To get valid QDiff, we have
   *   // to perform two iterations before this loop.
   *   em.iterate();
   * }
   *
   * em.computeEllipses(allEllipses);
   *
   * // If you are interested in the internal state of the functor use
   * // the getXXX(...) methods, which deliver const references (i.e.
   * // you don't have to copy them, if you only want to see them)
   *
   * std::vector< lti::matrix<double> >& covariances = em.getCovariances();
   * std::vector< lti::vector<double> >& centers     = em.getCenters();
   * std::vector< double >&              alphas      = em.getAlphas();
   *
   * // For a description of their meaning refer to the given bibliography
   *
   * \endcode
   *
   *
   * \par Beware of possible errors
   * \htmlonly
   * <a name="ERRORS"></a>
   * <a href="#HINTS">back</a>
   * \endhtmlonly
   *
   * \li <b>Constraints are relative to given ellipse</b>. If you use the
   *      returned ellipses for initializing another run of the functor,
   *      then please consider that their properties could have been changed
   *      by the first run.
   *      Example: You have an ellipse that is supposed to stay upright
   *      with a tolerance of 5°. You call apply once and the ellipse is indeed
   *      rotated 5°. You use the output for the next call of apply. And again
   *      the ellipse is rotated 5°, resulting in a total of 10° which is
   *      correct but possibly not what you expected.
   *
   */
  class blobEM : public functor {

  public:
    /**
     * the parameters for the class blobEM
     */
    class parameters : public functor::parameters {
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

      // If you add more parameters manually, do not forget to do following:
      // 1. indicate in the default constructor the default values
      // 2. make sure that the copy member also copy your new parameters
      // 3. make sure that the read and write members also read and
      //    write your parameters

      /** Maximum number of iterations. Default = 100
       */
      int maxIterations;

      /** If the difference of the Q value between two successive
       *  iterations (relative to the last Q value) falls below
       *  this threshold, then the iterations are aborted.
       *  A value < zero enforces the maximum number of iterations.
       *  Default 1e-6
       */
      double convergenceThreshold;

    };


    /**
     * An internal class of lti::blobEM for handling 2D gaussian ellipses.
     * The properties can be set manually.
     *
     * Another possibility is to obtain the properties from
     * a 2x2 covariance matrix (except center) or another
     * ellipse, either considering constraints or not.
     */
    class gaussEllipse {
      public:

        gaussEllipse();

        gaussEllipse(lti::tpoint<double> cen,
                     double l1,
                     double l2,
                     double ang);

        ~gaussEllipse();


        /** @name Ellipse properties
         */
        //@{

          /** Center of ellipse
           */
          lti::tpoint<double> center;

          /** Variance along main axis (eigenvalue 1).
           *  Ensure lambda1 to be > lambda2 for plausibility.
           *
           *  \b Note: This is not the standard deviation!
           *           For visualization of an ellipse use std dev.
           */
          double lambda1;

          /** Variance along second axis (eigenvalue 2)
           *
           *  \b Note: This is not the standard deviation!
           *           For visualization of an ellipse use std dev.
           */
          double lambda2;

          /** Angle of main axis (-Pi/2 to +Pi/2). Second axis is
           *  perpendicular to main axis.
           */
          double angle;

        //@}


        /** @name Ellipse constraints
         *    Use these to indicate constraints for the properties of an ellipse.
         *    \b NOTE: constraining properties can make it impossible
         *             for the EM-Algorithm to converge. Use with
         *             caution, preferably only for one component
         *             amongst several!
         */
        //@{

          /** keep center within centerTolerance (default = false)
           */
          bool constrainCenter;

          /** keep shape (lambda1/lambda2) within shapeTolerance
           *  (default = false)
           */
          bool constrainShape;

          /** keep area size (lambda1*lambda2) within areaTolerance (default = false)
           */
          bool constrainArea;

          /** keep orientation angle within angleTolerance (default = false)
           */
          bool constrainAngle;

        //@}


        /** @name Constraint tolerances
         *    Setting these to values > 0 (default) relaxes the
         *    constraint conditions.
         */
        //@{

          /** The center tolerance as maximum Euclidean distance in pixel.
           *  (default = 0)
           *
           *  \f$ dist(newCenter - center) < centerTolerance \f$
           */
          double centerTolerance;

          /** The relative shape tolerance (default = 0).
           *
           *  <b> NOT CONSIDERED YET!!! Development in Progress! </b>
           */
          double shapeTolerance;

          /** The relative size tolerance (default = 0), which means
           *  Ellipse area may change in range
           *
           *  \f$ area/a <= newArea <= area*a \f$ with \f$ a = (1+areaTolerance)\f$
           */
          double areaTolerance;

          /** The (+/-) angle tolerance (default = 0). Reasonable range
           *  is from 0 to Pi/2, since this is Pi/2 cyclic.
           *
           *  \f$ angle - angleTolerance <= newAngle <= angle + angleTolerance \f$
           */
          double angleTolerance;

        //@}


        /** Get Ellipse properties lambda and angle
         *  from a 2x2 covariance matrix considering
         *  shape, size, and/or angle constraints, if
         *  desired.
         *
         *  \b Note: center can NOT be obtained from
         *  covariance.
         *
         *  Returns false, if not possible
         */
        bool from2x2Covariance(const lti::matrix<double>& cov,
                               const bool& constraints = true);

        /** Get all ellipse properties (center, lambda, angle)
         *  from another ellipse considering center, shape, size,
         *  and/or angle constraints, if desired.
         *  THIS IS NOT A COPY METHOD!!!
         *
         *  Returns false, if not possible.
         */
        bool fromEllipse(const gaussEllipse& other,
                         const bool& constraints = true);

        /** Generate 2x2 covariance matrix with ellipse params
         *  lambda and angle. Returns false, if not possible
         */
        bool to2x2Covariance(lti::matrix<double>& cov) const;

      private:

        /** Perform pca on 2x2 covariance matrix.
         *  Lambdas are the eigenvalues and e are the
         *  eigenvectors (ordered by magnitude).
         *  Returns false, if not possible.
         */
        static bool pca2x2(const lti::matrix<double>& cov,
                           double& lambda1,
                           double& lambda2,
                           lti::vector<double>& e1,
                           lti::vector<double>& e2);
    };

    /**
     * default constructor
     */
    blobEM();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    blobEM(const blobEM& other);

    /**
     * destructor
     */
    virtual ~blobEM();

    /**
     * returns the name of this type ("blobEM")
     */
    virtual const char* getTypeName() const;

    /**
     * Calls initialize() once, then iterate() several times until
     * maximum number of iterations or abortion criterion reached
     * (percental increase of Q value below given threshold)
     * @param src channel8 with the source data. Should be distance
     *            transform of a binary image.
     * @param elem vector of blobEM::gaussEllipse with initial distribution
     *             parameters, represented by 2D-ellipses. Result of EM
     *             iteration is also left here.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& src, std::vector<gaussEllipse>& elem);

    /**
     * Calls initialize() once, then iterate() several times until
     * maximum number of iterations or abortion criterion reached
     * (percental increase of Q value below given threshold)
     * @param src channel8 with the source data. Should be distance
     *            transform of a binary image.
     * @param components number of mixture components. Will be initialized
     *            randomly. For deterministic initialization use other
     *            apply method.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& src, const int& components);

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    blobEM& copy(const blobEM& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    blobEM& operator=(const blobEM& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    /** @name Manual control
     *  These methods are required for the manual control of the EM
     */
    //@{

      /** Explicitly initialize this functor, for example if you intend to
       *  perform each iteration singly. Initializes with the given ellipses.
       */
      bool initialize(const channel8& src, const std::vector<gaussEllipse>& elem);

      /** Explicitly initialize this functor, for example if you intend to
       *  perform each iteration singly. Initializes with M random ellipses,
       *  with M = components
       */
      bool initialize(const channel8& src, const int& components);

      /** perform one single EM iteration. returns the total Q value, that has
       *  to be maximized by EM. \b Note: Call initialize() first !!!
       */
      double iterate();

      /** compute and return ellipses based on internal covariances and centers
       */
      bool computeEllipses(std::vector<gaussEllipse>& ellipses);

      /** reset this functors internal members and the working data
       */
      void reset();

    //@}

    /** @name State access
     *  Use these to access the internal state of this functor.
     */
    //@{

      /** Access the number of iterations so far
       */
      const int& getIterations() const;

      /** Access the number of components
       */
      const int& getM() const;

      /** Access the number of data elements considered
       */
      const int& getN() const;

      /** Access the vector of the M mixture component weights
       */
      const std::vector<double>& getAlphas() const;

      /** Access the centers of the M center components
       */
      const std::vector<tpoint<double> >& getCenters() const;

      /** Access the covariance matrices of the M center components
       */
      const std::vector<matrix<double> >& getCovariances() const;

      /** Access the Q value, that is maximized by EM
       */
      const double& getQ() const;

      /** Access the difference of current and last Q value
       */
      const double& getQDiff() const;

    //@}

  private:
    /** @name Internal state
     *  These are the internal state members.
     */
    //@{

      /** The weights (a-priori probs) of the M mixture components
       */
      std::vector<double> m_alphas;

      /** The centers of the M mixture components
       */
      std::vector< tpoint<double> > m_centers;

      /** The covariance matrices of the M center components
       */
      std::vector< matrix<double> > m_covariances;

    //@}

    /** @name Working data
     */
    //@{

      /** The number of components
       */
      int m_iterations;

      /** The number of components
       */
      int m_M;

      /** The number of data elements (sum of Ni)
       */
      int m_N;

      /** The x coordinates
       */
      std::vector<int> m_xi;

      /** The y coordinates
       */
      std::vector<int> m_yi;

      /** The frequency of the i-th x,y pair
       */
      std::vector<int> m_Ni;

      /** The ellipses used for initializing
       */
      std::vector<gaussEllipse> m_ellipses;

      /** The Q value, that is maximized by EM
       */
      double m_Q;

      /** The difference of current and last Q value
       */
      double m_QDiff;

    //@}


    /** Performs a single EM step to estimate the new parameters,
     *  and returns the likelihood
     */
    double singleEMStep();

    /**
     * An internal helper class for 2D gaussian PDF p(x,y).
     * Use only with 2x2 covariance matrices (symmetric)!
     */
    class gauss2DPDF {
      public:

        gauss2DPDF() {
          reset();
        }

        ~gauss2DPDF() {}

        /** evaluates the pdf with the given x,y pair and center and
         *  the internally stored covariance matrix.
         */
        double evaluate(int x, int y, const tpoint<double>& cen) const;

        /** sets the covariance matrix, computes its determinant,
         *  and its inverse. returns false and unsets covariance
         *  and inverse, on error.
         */
        bool set2x2CovarianceMatrix(const lti::matrix<double>& covar);

        /** resets the internal state
         */
        void reset();

      private:
        lti::matrix<double> m_covar;
        lti::matrix<double> m_covarInverse;
        double m_det;

        static const lti::point m_validSize;
    };

    // covariance regularization constant (see Ueda, Nakano et al.
    // "SMEM Algorithm for Mixture Models", resp. Ormoneit & Tresp
    // "Improved Gaussian Mixture Density Estimates Using Bayesian
    // Penalty Terms and Network Averaging".
    static const double LAMBDA;

    // regularization constant for alphas
    static const double ALPHAEPSILON;
  };

  /**
   * write a blobEM::gaussEllipse into the given stream handler
   * @param handler ioHandler where the ellipse should be written.
   * @param el ellipse
   * @param complete if true (default), begin and end tokens will be written
   *                 around the object.
   * @return true if successful, false otherwise
   */
  bool write(ioHandler& handler,
             const blobEM::gaussEllipse& el,
             const bool complete=true);

  /**
   * read a blobEM::gaussEllipse from the given stream handler
   * @param handler ioHandler where the ellipse should be read from.
   * @param el ellipse will be left here
   * @param complete if true (default), begin and end tokens will be read too.
   * @return true if successful, false otherwise
   */
  bool read(ioHandler& handler,
            blobEM::gaussEllipse& el,
            const bool complete=true);
}

#endif

