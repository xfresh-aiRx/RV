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
 * file .......: ltiGaussianMixtureModel.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 25.9.2000
 * revisions ..: $Id: ltiGaussianMixtureModel.h,v 1.20 2005/06/22 15:42:10 doerfler Exp $
 */

#ifndef _LTI_GAUSSIAN_MIXTURE_MODEL_H_
#define _LTI_GAUSSIAN_MIXTURE_MODEL_H_

#include "ltiConfig.h"
#include "ltiStatisticsFunctor.h"
#include "ltiMeansFunctor.h"
#include "ltiVarianceFunctor.h"
#include "ltiMatrixInversion.h"
#include "ltiL2Distance.h"
#include "ltiProgressInfo.h"
#include "ltiMatrixDecomposition.h"


#ifdef _LTI_WIN32
#define random rand
#endif


namespace lti {
  /**
   * Functor which estimates a gaussian mixture model of a given data set.
   * See documentation of the apply() methods for details.
   * Please note that, even though this is a template class, it does not
   * make much sense to use it with anything but floating-point types.
   * The higher the precision, the better. So, if you use
   * gaussianMixtureModel<float> and get crappy results, you should not
   * wonder. If you get reasonable results, you're just lucky.
   * Other template arguments than float or double probably won't even
   * compile.
   */
  template <class T> class gaussianMixtureModel : public statisticsFunctor {
  public:
    /**
     * The parameters for the class gaussianMixtureModel
     */
    class parameters : public statisticsFunctor::parameters {
    public:
      /**
       * Default constructor
       */
      parameters()
        : statisticsFunctor::parameters() {
        partialIterations=50;
        iterations=100;
        forceIterations=false;
        numberOfComponents=3;
        useSM=false;
        lambda=T(0.1);
        emergencyLambda=T(0.1);
        cMax=5;
        epsilon=T(0.2);
        reportProgress=false;
      };

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other) 
        : statisticsFunctor::parameters() {
        copy(other);
      };

      /**
       * destructor
       */
      ~parameters() {
      };

      /**
       * returns name of this type
       */
      const char* getTypeName() const {
        return "gaussianMixtureModel::parameters";
      };

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other) {
#       ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        statisticsFunctor::parameters::copy(other);
#       else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        statisticsFunctor::parameters&
          (statisticsFunctor::parameters::* p_copy)
          (const statisticsFunctor::parameters&) =
          statisticsFunctor::parameters::copy;
        (this->*p_copy)(other);
#      endif

        numberOfComponents=other.numberOfComponents;
        partialIterations=other.partialIterations;
        iterations=other.iterations;
        forceIterations=other.forceIterations;
        useSM=other.useSM;
        lambda=other.lambda;
        emergencyLambda=other.emergencyLambda;
        cMax=other.cMax;
        epsilon=other.epsilon;
        reportProgress=other.reportProgress;
        return *this;
      };

#     ifndef _LTI_MSC_6
        /**
         * write the parameters in the given ioHandler
         * @param handler the ioHandler to be used
         * @param complete if true (the default) the enclosing begin/end will
         *        be also written, otherwise only the data block will be
         *        written.
         * @return true if write was successful
         */
        virtual bool write(ioHandler& handler,const bool complete=true) const
#     else
        /**
         * workaround for MS-VC bug.  Never use this method directly,
         * use write() instead
         */
        bool writeMS(ioHandler& handler,const bool complete=true) const
#     endif
        {
          bool b = true;
          if (complete) {
            b = handler.writeBegin();
          }

          if (b) {
            lti::write(handler,"numberOfComponents",numberOfComponents);
            lti::write(handler,"iterations",iterations);
            lti::write(handler,"forceIterations",iterations);
            lti::write(handler,"partialIterations",partialIterations);
            lti::write(handler, "useSM", useSM);
            lti::write(handler, "lambda", lambda);
            lti::write(handler, "emergencyLambda", emergencyLambda);
            lti::write(handler, "Cmax", cMax);
            lti::write(handler, "epsilon", epsilon);
            lti::write(handler, "reportProgress", reportProgress);
          }

#       ifndef _LTI_MSC_6
          // This is the standard C++ code, which MS Visual C++ 6 is not
          // able to compile...
          b = b && statisticsFunctor::parameters::write(handler,false);
#       else
          bool (functor::parameters::* p_writeMS)(ioHandler&,
                                                   const bool) const =
            functor::parameters::writeMS;
          b = b && (this->*p_writeMS)(handler,false);
#       endif

          if (complete) {
            b = b && handler.writeEnd();
          }

          return b;
        }

#     ifdef _LTI_MSC_6
        /**
         * write the parameters in the given ioHandler
         * @param handler the ioHandler to be used
         * @param complete if true (the default) the enclosing begin/end will
         *        be also written, otherwise only the data block will be
         *        written.
         * @return true if write was successful
         */
        virtual bool write(ioHandler& handler,
                           const bool complete = true) const {
           // ...we need this workaround to cope with another really
           // awful MSVC bug.
           return writeMS(handler,complete);
        }
#     endif

#     ifndef _LTI_MSC_6
        /**
         * read the parameters from the given ioHandler
         * @param handler the ioHandler to be used
         * @param complete if true (the default) the enclosing begin/end will
         *        be also read, otherwise only the data block will be read.
         * @return true if write was successful
         */
        virtual bool read(ioHandler& handler,const bool complete = true)
#     else
        /**
         * workaround for MS-VC bug.  Never use this method directly,
         * use read() instead
         */
        bool readMS(ioHandler& handler,const bool complete=true)
#     endif
        {
          bool b = true;
          if (complete) {
            b = handler.readBegin();
          }

          if (b) {
            lti::read(handler,"numberOfComponents",numberOfComponents);
            lti::read(handler,"iterations",iterations);
            lti::read(handler,"forceIterations",iterations);
            lti::read(handler,"partialIterations",partialIterations);
            lti::read(handler, "useSM", useSM);
            lti::read(handler, "lambda", lambda);
            lti::read(handler, "emergencyLambda", emergencyLambda);
            lti::read(handler, "Cmax", cMax);
            lti::read(handler, "epsilon", epsilon);
            lti::read(handler, "reportProgress", reportProgress);
          }

#     ifndef _LTI_MSC_6
          // This is the standard C++ code, which MS Visual C++ 6 is not
          // able to compile...
          b = b && statisticsFunctor::parameters::read(handler,false);
#     else
          bool (functor::parameters::* p_readMS)(ioHandler&,const bool) =
          functor::parameters::readMS;
          b = b && (this->*p_readMS)(handler,false);
#     endif



          if (complete) {
            b = b && handler.readEnd();
          }

          return b;
        }

#     ifdef _LTI_MSC_6
        /**
         * read the parameters from the given ioHandler
         * @param handler the ioHandler to be used
         * @param complete if true (the default) the enclosing begin/end will
         *        be also read, otherwise only the data block will be read.
         * @return true if write was successful
         */
        virtual bool read(ioHandler& handler,const bool complete=true) {
          // ...we need this workaround to cope with another really awful MSVC
          // bug.
          return readMS(handler,complete);
        }
#      endif


      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const {
        return new parameters(*this);
      };

      /**
       * This is the number of components of the mixture.
       */
      int numberOfComponents;

      /**
       * This is the maximum number of iterations that the EM
       * algorithm is supposed to be run.
       */
      int iterations;

      /**
       * If this is true, the EM always executes the maximum
       * number of iterations, no matter if it converges earlier
       * or not.
       */
      bool forceIterations;

      /**
       * This is the maximum number of partial iterations that the SMEM
       * algorithm is supposed to be run.
       */
      int partialIterations;

      /**
       * This flag is true, if the estimator should use the
       * <a href="http://citeseer.nj.nec.com/ueda99smem.html">Split-and-Merge EM</a>
       * algorithm by <a href="http://www.kecl.ntt.co.jp/icl/as/members/ueda/">Ueda</a>,
       * <a href="http://www-nkn.ics.nitech.ac.jp/nakano/">Nakano</a>,
       * <a href="http://www.db.toronto.edu/~zoubin/">Ghahramani</a>, and
       * <a href="http://www.cs.utoronto.ca/~hinton/">Hinton</a>
       * (<a href="http://mitpress.mit.edu/NECO/">Neural Computation</a> 12(9), pp. 2109-2128).
       */
      bool useSM;


      /**
       * If You use the split-and-merge version, this parameter is used as
       * the regularization constant. The larger this constant, the more
       * a covariance matrix will be drawn to be the identity matrix.
       */
      T lambda;

      /**
       * This is a factor for regularizing the covariance matrices in
       * emergencies. An emergency occurs when the determinant of a
       * covariance matrix is almost zero (which often occurs due to
       * numerical problems). In that case, the diagonal of the
       * matrix will be emphasized by this factor, until the
       * determinant becomes reasonable.
       */
      T emergencyLambda;

      /**
       * This is the maximum number of split-and-merge trials
       * that are done in each SM step.
       */
      int cMax;

      /**
       * The magnitude of the random perturbation of the split
       * algorithm.
       */
      T epsilon;

      /**
       * Is true if the functor should report its result to a progress
       * info object
       */
      bool reportProgress;
    };

    /**
     * default constructor */
    gaussianMixtureModel();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    gaussianMixtureModel(const gaussianMixtureModel& other);

    /**
     * destructor
     */
    virtual ~gaussianMixtureModel();

    /**
     * returns the name of this type ("gaussianMixtureModel")
     */
    virtual const char* getTypeName() const;


    /**
     * Performs estimation of a gaussian mixture model for the given
     * data matrix. The matrix is interpreted as having a data point
     * in each row.
     * The method uses the Expectation Maximization method; for a really
     * nice explanation of the algorithm, see the paper
     * <a href="http://ssli.ee.washington.edu/people/bilmes/mypapers/em.ps.gz">
     * A Gentle Tutorial of the EM Algorithm and its Application to
     * Parameter Estimation for Gaussian Mixture and Hidden Markov Models</a>
     * by <a href="http://ssli.ee.washington.edu/people/bilmes/">Jeff Bilmes</a>.
     * (published as Technical Report TR-97-021 by the
     * <a href="http://www.icsi.berkeley.edu/">International Computer
     * Science Institute</a>).
     * If the useSM parameter is true, the split-and-merge algorithm by
     * <a href="http://citeseer.nj.nec.com/ueda99smem.html">Split-and-Merge EM</a>
     * algorithm by <a href="http://www.kecl.ntt.co.jp/icl/as/members/ueda/">Ueda</a>,
     * <a href="http://www-nkn.ics.nitech.ac.jp/nakano/">Nakano</a>,
     * <a href="http://www.db.toronto.edu/~zoubin/">Ghahramani</a>, and
     * <a href="http://www.cs.utoronto.ca/~hinton/">Hinton</a>
     * (<a href="http://mitpress.mit.edu/NECO/">Neural Computation</a> 12(9), pp. 2109-2128)
     * is used instead.
     *
     * @param src matrix<T> with the source data, each row is
     *            considered a data point.
     * @result true if the estimation was successfull.
     */
    inline bool apply(const matrix<T>& src) {
      return estimate(src);
    }

    /**
     * Equivalent to apply.
     * @param src matrix<T> with the source data, each row is
     *            considered a data point.
     * @result true if the estimation was successfull.
     */
    bool estimate(const matrix<T>& src);

    /**
     * Returns a std::vector of covariance matrices of the
     * components. Each vector element is the covariance matrix
     * of a model component.
     * @result a reference to the component covariance matrices
     *         of the last estimation
     */
    inline const std::vector< matrix<T> >& getComponentSigmas() const {
      return sigmas;
    };

    /**
     * Returns a std::vector of mean vectors of the
     * components. Each vector element is the mean vector of
     * a model component.
     * @result a reference to the component means
     *         of the last estimation
     */
    const std::vector< vector<T> >& getComponentMus() const {
      return mus;
    };

    /**
     * Returns the mixing coefficient vector. Each element
     * is the mixing coefficient of a model component. It
     * can be viewed as a
     * @result a reference to the mixing coefficients
     *         of the last estimation
     */
    const vector<T>& getAlpha() const {
      return alpha;
    };

    /**
     * Copies a std::vector of covariance matrices of the
     * components to the given parameter.
     * Each vector element is the covariance matrix
     * of a model component.
     * @param sigs will receive the component covariance matrices
     *         of the last estimation
     */
    void getComponentSigmas(std::vector< matrix<T> >& sigs) const {
      sigs=sigmas;
    };

    /**
     * Copies a std::vector of mean vectors of the
     * components to the given parameter.
     * Each vector element is the mean vector
     * of a model component.
     * @param ms will receive the component means
     *         of the last estimation
     */
    void getComponentMus(std::vector< vector<T> >& ms) const {
      ms=mus;
    };

    /**
     * Copies the mixing coefficients to the given parameter.
     * Each vector element is the a-priori probability
     * of a model component.
     * @param a will receive the component mixture coefficients
     *         of the last estimation
     */
    void getAlpha(vector<T>& a) const {
      a=alpha;
    };


    /*
     * This is exactly as the other apply method, however, with this
     * one you pass pointers to already-allocated objects within the
     * std::vector. The function only modifies the pointed-to vectors
     * and matrices, but not the pointers themselves, and not the
     * list.
     *
     bool apply(const matrix<T>& src,
               const std::vector< vector<T>* >& means,
               const std::vector< matrix<T>* >& sigmas,
               vector<T>& alphas) const;
    */

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    gaussianMixtureModel& copy(const gaussianMixtureModel& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Sets the progress info object.
     */
    void setProgressInfo(progressInfo& pi);

    /*
     * This computes the likelihood of a vector x given this model.
     *
    T getLikelihood(const vector<T>& x);
    */
  protected:
    // estimates a mixture model by plain EM
    bool plainEstimate();
    // estimates a mixture model by SMEM
    bool smemEstimate();

    // returns the likelihood of x being generated by component k
    T getPartialLikelihood(const vector<T>& x, const int k);
    // returns the likelihood of x being generated by component k,
    // debug version
    T getDBGPartialLikelihood(const vector<T>& x, const int k);
    // returns the log of the likelihood of x being generated by component k
    T getPartialLogLikelihood(const vector<T>& x, const int k);
    // returns the log of the likelihood of x being generated by component k,
    // debug version
    T getDBGPartialLogLikelihood(const vector<T>& x, const int k);

    // performs a full EM step over all components
    // if evalQ is true, the quality measure value will be returned in q
    bool fullEMStep(bool evalQ, T& q);

    // performs a full EM step over all components
    inline bool fullEMStep() {
      T q;
      return fullEMStep(false,q);
    };

    // returns the quality measure value for the mixture; this is the
    // objective function which is maximized
    // if the result is below zero, all is in order. If it is above
    // zero, then you have most probably a crappy estimation
    T computeQ();

    // returns the quality measure value for the mixture of
    // the components whose indices are given in index.
    // This array MUST CONTAIN EXACTLY 3 elements (needed for SMEM)
    T updateQ(const int index[]);

    // Performs a partial EM step, considering only the components
    // whose indices are given in index.
    // This array MUST CONTAIN EXACTLY 3 elements (needed for SMEM)
    bool partialEMStep(const int index[], bool evalQ, T& q);

    // Performs a partial EM step, considering only the components
    // whose indices are given in index.
    // This array MUST CONTAIN EXACTLY 3 elements (needed for SMEM)
    inline bool partialEMStep(const int index[]) {
      T q;
      return partialEMStep(index,false,q);
    };

  private:

    // tries to rescue a covariance matrix from singularity
    // returns the determinant of the rescued matrix
    T rescueMatrix(matrix<T>& victim);

    /**
     * Returns a uniformly distributed random number between 0 and a.
     */
    inline T frand(T a) const {
      return (T(random())/T(RAND_MAX))*a;
    }

    /**
     * This computes the a-posteriori probability matrix p with
     * p[l][i]=P(l|xi);
     * Therefore, the sum of a row l is the a-posteriori probability of model
     * l.
     */
    void computeP();

    /**
     * This computes the a-posteriori probability matrix p with
     * p[l][i]=P(l|xi);
     * The values of l are given in the parameter. The array must have
     * exactly 3 elements.
     * Therefore, the sum of a row l is the a-posteriori probability of model
     * l.
     */
    void updateP(const int ijk[]);

    // functors
    meansFunctor<T> mean;
    matrixInversion<T> inverter;
    varianceFunctor<T> var;
    luDecomposition<T> luDecomposer;
    l2Distance<T> dist;

    // the data for which the model is generated
    const matrix<T>* data;

    // the regularizer matrix
    matrix<T> regularizer;

    // the mean vectors of the components
    std::vector< vector<T> > mus;
    // the covariance matrices of the components
    std::vector< matrix<T> > sigmas;
    // the inverses of the covariance matrices of the components
    std::vector< matrix<T> > sigmaInvs;
    // the determinants of the covariance matrices of the components
    vector<T> dets;
    // the a-priori probabilities of the components
    vector<T> alpha;
    // the a-posteriori probabilities of each data point
    // being generated by each component
    matrix<T> p;
    // number of components and number of data vectors.
    int M,N;

    progressInfo* reporter;
  };
}

#endif


