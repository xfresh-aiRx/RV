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
 * file .......: ltiStudentDistribution.h
 * authors ....: Claudia Goenner
 * organization: LTI, RWTH Aachen
 * creation ...: 20.9.2004
 * revisions ..: $Id: ltiStudentDistribution.h,v 1.4 2005/07/21 12:58:07 doerfler Exp $
 */

#ifndef _LTI_STUDENT_DISTRIBUTION_H_
#define _LTI_STUDENT_DISTRIBUTION_H_

#include "ltiMathFunction.h"
#include <vector>
#include <cassert>

#undef _LTI_DEBUG
//#define _LTI_DEBUG 1
#include "ltiDebug.h"

#ifdef _LTI_DEBUG
using std::cout;
using std::endl;
using std::flush;
#endif

namespace lti {

  /**
   * The Student t-test compares the means of two normally distributed
   * samples of sizes n1 and n2, assuming that their unknown variances are
   * equal. If the test value is bigger than T(k,alpha), k=n1+n2-2,
   * then the equality hypothesis is rejected at significance level alpha.
   *
   * This class only computes T(k,alpha). Computing the test value is left
   * to the application. For efficiency, e.g. if testing many times in a row
   * with varying degrees of freedom the values T(k,alpha) may be precomputed
   * for a constant significance level alpha and stored in a LUT.
   */
  class studentDistribution : public mathFunction {
  public:
    /**
     * The parameters for the class studentDistribution
     */
    class parameters : public mathFunction::parameters {
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
       * If this is true a LUT is computed on the first call of accept and
       * used from there on. Please note that the LUT is only used by accept.
       * The apply method computes the confidence on every call.
       *
       * Default: false
       */
      bool useLUT;

      /**
       * The maximum degrees of freedom considered in the LUT. For some
       * common alpha, the an additional entry storing the limit is added.
       * Please note that computing the LUT is expensive.
       *
       * Default: 30.
       */
      int maxDOF;

      /**
       * If this is true, the significance level is interpreted to belong to a
       * one-sided test. Otherwise a two-sided test is performed.
       *
       * Default: false
       */
      bool oneSided;

      /**
       * The significance level.
       *
       * Default: 0.05
       */
      float alpha;
    };

    /**
     * Default constructor
     */
    studentDistribution();

    /**
     * Construct a functor using the given parameters
     */
    studentDistribution(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    studentDistribution(const studentDistribution& other);

    /**
     * Destructor
     */
    virtual ~studentDistribution();

    /**
     * Returns the name of this type ("studentDistribution")
     */
    virtual const char* getTypeName() const;

    /**
     * Computes Student's distribution probability function for a given
     * degree of freedom and t-value, which code the similarity of the
     * two sample means
     *.
     * @param n      degrees of freedom of the student distribution
     * @param tValue t-value computed by your aplication
     * @param confidence the result, i.e. the probability that the two
     *                   sample means are similar
     * @return true on success and false otherwise
     */
    bool apply (const int n, const float& tValue, float& confidence) const;

    /**
     * Compute the lowest t-value, for which the hypothesis is rejected.
     * @param n      degrees of freedom of the student distribution
     * @return the lowest t-value for which the hypothesis is rejected.
     */
    float bound (const int n) const;

    /**
     * Tests if the hypothesis of equal means is accepted at the
     * significance level given in the parameters. Depending on the
     * parameter settings this method uses the LUT or computes the
     * result. If the LUT is requested but not initialized properly,
     * it is done here.
     *
     * @param n      degrees of freedom of the student distribution
     * @param tValue t-value computed by your application
     * @return true if the hypothesis is accepted and false otherwise
     */
    bool accept (const int n, const float& tValue);

    /**
     * Tests if the hypothesis of equal means is accepted at the
     * significance level given in the parameters. The appropriate
     * bound of Student's t-distribution is taken from a pre-computed
     * LUT.
     *
     * CAUTION: This method does not check if the LUT is initialized,
     * nor does it check that the LUT has been computed for the current
     * significance level. To initialize it please call generateLUT().
     *
     * @param n      degrees of freedom of the student distribution
     * @param tValue t-value computed by your application
     * @return true if the hypothesis is accepted and false otherwise
     */
    inline bool acceptLUT (const int n, const float& tValue);

    /**
     * Tests if the hypothesis of equal means is accepted at the
     * significance level given in the parameters. The significance
     * for your t-value is computed and compared with the significance
     * level supplied in the parameters.
     *
     * @param n      degrees of freedom of the student distribution
     * @param tValue t-value computed by your application
     * @return true if the hypothesis is accepted and false otherwise
     */
    bool acceptCompute (const int n, const float& tValue) const;
  
    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    studentDistribution& copy(const studentDistribution& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    studentDistribution& operator=(const studentDistribution& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Generates the LUT.
     */
    bool generateLUT();
    
  protected:

    /**
     * Remember the significance level for which the LUT has been computed.
     * Normed to one-sided.
     */
    float alpha_;

    /**
     * The LUT. Due to computational expensive of the used algorithm
     * double does not make any sense here.
     */
    std::vector<float> lut_;

    /**
     * Compute the lowest t-value, for which the hypothesis is rejected.
     * @param n      degrees of freedom of the student distribution
     * @param tValue t-value computed by your application
     * @return the lowest t-value for which the hypothesis is rejected.
     */
    float computeBound(const int n, const float& alpha) const;

    /**
     * Compute the Student distribution probability function
     * for a given n (degrees of freedom) and t-value.
     * @param n      degrees of freedom of the student distribution
     * @param tValue t-value computed by your application
     * @return the two sided confidence
     */
    float distribution(const int n,
                       const float& tValue) const;
  };

  inline bool studentDistribution::acceptLUT (const int n,
                                              const float& tValue) {
    
    //make sure that lut in initialized in debug mode
    assert(lut_.size()>0);
    if ( n <= 0 ) {
      setStatusString("\napplyLUT: n (degrees of freedom) must be > 0\n");
      return false;
      
    } else if ( n >= static_cast<int>(lut_.size()) ) {
      //test with the value at infinity
      _lti_debug(" _bound "<< lut_.back());
      const float tmp ( tValue >= 0 ? tValue : -tValue );
      return tmp < lut_.back();

    } else {
      _lti_debug(" _bound " << lut_[n-1]);
      const float tmp ( tValue >= 0 ? tValue : -tValue );
      return tmp < lut_[n-1];
    }
  };

}
#include "ltiUndebug.h"

#endif

