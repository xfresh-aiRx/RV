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
 * file .......: ltiPolynomRoots.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 6.4.2003
 * revisions ..: $Id: ltiPolynomRoots.h,v 1.7 2005/06/16 12:37:35 doerfler Exp $
 */

#ifndef _LTI_POLYNOM_ROOTS_H_
#define _LTI_POLYNOM_ROOTS_H_

#include "ltiMath.h"
#include "ltiFunctor.h"
#include "ltiVector.h"
#include "ltiComplex.h"

namespace lti {
  /**
   * Find the roots of a polygon with real coefficients.
   *
   * The equation \c f(x)=0 with the polynom of n-th degree \c f(x) with
   * real coefficients has n solutions, where some of them can be complex.
   *
   * Complex roots for polynoms with real coefficients exist always 
   * as conjugate pairs.
   *
   * For an explanation of the algorithms used see:
   * W. Press et. at. "Numerical Recipes in C", 2nd edition, 1992.
   *
   */
  class polynomRoots : public functor {
  public:
    /**
     * the parameters for the class polynomRoots
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

    };

    /**
     * default constructor
     */
    polynomRoots();

    /**
     * Construct a functor using the given parameters
     */
    polynomRoots(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    polynomRoots(const polynomRoots& other);

    /**
     * destructor
     */
    virtual ~polynomRoots();

    /**
     * returns the name of this type ("polynomRoots")
     */
    virtual const char* getTypeName() const;

    /**
     * Solves the quadratic equation \f$ ax^2 + bx + c = 0 \f$,
     * for real \e a, \e b and \e c coefficients.
     *
     * @param a coefficient for the x^2 term
     * @param b coefficient for the x  term
     * @param c constant term
     *
     * @param rex1 real part of the first solution
     * @param imx1 imaginary part of the first solution
     * @param rex2 real part of the second solution
     * @param imx2 imaginary part of the second solution
     *
     * @return the number of real solutions.
     */
    int quadratic(const double& a, const double& b, const double& c,
                  double& rex1, double& imx1,
                  double& rex2, double& imx2) const;
    

    /**
     * Solves the quadratic equation \f$ ax^2 + bx + c = 0 \f$,
     * for real \e a, \e b and \e c coefficients.
     *
     * @param a coefficient for the x^2 term
     * @param b coefficient for the x  term
     * @param c constant term
     *
     * @param x1 first solution
     * @param x2 second solution
     *
     * @return the number of real solutions.
     */
    int quadratic(const double& a, const double& b, const double& c,
                  complex<double>& x1, complex<double>& x2) const;

    /**
     * Solves the quadratic equation \f$ x^2 + p*x + q = 0 \f$,
     * for real \e p and \e q coefficients.
     *
     * @param p coefficient for the x term
     * @param q constant term
     *
     * @param rex1 real part of the first solution
     * @param imx1 imaginary part of the first solution
     * @param rex2 real part of the second solution
     * @param imx2 imaginary part of the second solution
     *
     * @return the number of real solutions.
     */
    int quadratic(const double& p, const double& q,
                  double& rex1, double& imx1,
                  double& rex2, double& imx2) const;


    /**
     * Solves the quadratic equation \f$ x^2 + p*x + q = 0 \f$,
     * for real \e p and \e q coefficients.
     *
     * @param p coefficient for the x term
     * @param q constant term
     *
     * @param x1 first solution
     * @param x2 second solution
     *
     * @return the number of real solutions.
     */
    int quadratic(const double& p, const double& q,
                  complex<double>& x1, complex<double>& x2) const;


    /**
     * Solves the equation \f$ x^3 + a*x^2 + b*x + c= 0\f$, 
     * for real \c a, \c b and \c c coefficients.
     *
     * @param a coefficient for the x^2  term
     * @param b coefficient for the x term
     * @param c constant term
     *
     * @param rex1 real part of the first solution
     * @param imx1 imaginary part of the first solution
     * @param rex2 real part of the second solution
     * @param imx2 imaginary part of the second solution
     * @param rex3 real part of the third solution
     * @param imx3 imaginary part of the third solution
     *
     * @return the number of real solutions.
     */
    int cubic(const double& a, const double& b, const double& c,
              double& rex1, double& imx1,
              double& rex2, double& imx2,
              double& rex3, double& imx3) const;

    /**
     * Solves the equation \f$ x^3 + a*x^2 + b*x + c= 0\f$, 
     * for real \c a, \c b and \c c coefficients.
     *
     * @param a coefficient for the x^2  term
     * @param b coefficient for the x term
     * @param c constant term
     *
     * @param x1 first solution
     * @param x2 second solution
     * @param x3 third solution
     *
     * @return the number of real solutions.
     */
    int cubic(const double& a, const double& b, const double& c,
              complex<double>& x1,
              complex<double>& x2,
              complex<double>& x3) const;


    /**
     * General apply method.  The described polygon is
     * p[0]+p[1]*x + p[2]*x^2 + .. + p[n-1]*x^(n-1), with \a n the size of \a p
     *
     * @param p coefficients for the polynom
     * @param re real parts of the solutions
     * @param im imaginary parts of the solutions
     *
     * \warning at this point, only solutions for polynoms of first, second
     * and third degree have being implemented
     *
     * @return the number of real solutions or negative if an error occurred
     */
    virtual int apply(const vector<double>& p,
                      vector<double>& re,
                      vector<double>& im) const;

    /**
     * General apply method.  The described polygon is
     * p[0]+p[1]*x + p[2]*x^2 + .. + p[n-1]*x^(n-1), with \a n the size of \a p
     *
     * @param p coefficients for the polynom
     * @param roots the solutions
     *
     * \warning at this point, only solutions for polynoms of first, second
     * and third degree have being implemented
     *
     * @return the number of real solutions
     */
    virtual int apply(const vector<double>& p,
                      vector< complex<double> >& roots) const;

    /**
     * Solves the quadratic equation \f$ ax^2 + bx + c = 0 \f$,
     * for real \e a, \e b and \e c coefficients.
     *
     * @param a coefficient for the x^2 term
     * @param b coefficient for the x  term
     * @param c constant term
     *
     * @param rex1 real part of the first solution
     * @param imx1 imaginary part of the first solution
     * @param rex2 real part of the second solution
     * @param imx2 imaginary part of the second solution
     *
     * @return the number of real solutions.
     */
    inline int apply(const double& a, const double& b, const double& c,
                     double& rex1, double& imx1,
                     double& rex2, double& imx2) const {
      return quadratic(a,b,c,rex1,imx1,rex2,imx2);
    }
    

    /**
     * Solves the quadratic equation \f$ x^2 + p*x + q = 0 \f$,
     * for real \e p and \e q coefficients.
     *
     * @param p coefficient for the x term
     * @param q constant term
     *
     * @param rex1 real part of the first solution
     * @param imx1 imaginary part of the first solution
     * @param rex2 real part of the second solution
     * @param imx2 imaginary part of the second solution
     *
     * @return the number of real solutions.
     */
    inline int apply(const double& p, const double& q,
                     double& rex1, double& imx1,
                     double& rex2, double& imx2) const {
      return quadratic(p,q,rex1,imx1,rex2,imx2);
    };


    /**
     * Solves the equation \f$ x^3 + a*x^2 + b*x + c= 0\f$, 
     * for real \c a, \c b, \c c and \c d coefficients.
     *
     * @param a coefficient for the x^2  term
     * @param b coefficient for the x term
     * @param c constant term
     *
     * @param rex1 real part of the first solution
     * @param imx1 imaginary part of the first solution
     * @param rex2 real part of the second solution
     * @param imx2 imaginary part of the second solution
     * @param rex3 real part of the third solution
     * @param imx3 imaginary part of the third solution
     *
     * @return the number of real solutions.
     */
    int apply(const double& a, const double& b, const double& c,
              double& rex1, double& imx1,
              double& rex2, double& imx2,
              double& rex3, double& imx3) const {
      return cubic(a,b,c,rex1,imx1,rex2,imx2,rex3,imx3);
    }

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    polynomRoots& copy(const polynomRoots& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    polynomRoots& operator=(const polynomRoots& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */    
    const parameters& getParameters() const;

    /**
     * Laguerre's method to find the roots of a polynom of n-th degree.
     *
     * @param p polynom coefficients.  The polynom represented by this
     *          vector is p[0] + p[1]*x + p[2]*x^2 + ... + p[n]*x^n, which
     *          means the size of the vector is n+1.
     * @param degree number of elements of p to consider minus one (degree of 
     *          polynom)
     * @param root solution found.  The value given here is used as a
     *          first estimation, which will be improved until convergence
     * @return number of iterations required until convergence
     */
    int laguerre(const vector< complex<double> >& p,
                 const int degree,
                 complex<double>& root) const;

    /**
     * Search in succession for each root, through deflation.
     *
     * @param p polynom coefficients  The polynom represented by this
     *          vector is p[0] + p[1]*x + p[2]*x^2 + ... + p[n]*x^n, which
     *          means the size of the vector is n+1.
     * @param roots the n solutions found.
     * @param polish if true, the found solutions will be "polished", i.e.
     *          after finding all n solutions, they will be improved using
     *          again the laguerre method, without deflating the polynom
     * @return number of real roots.
     */
    int findRoots(const vector<double>& p ,
                  vector< complex<double> >& roots,
                  const bool polish=true) const;
    

  };

 
}

#endif

