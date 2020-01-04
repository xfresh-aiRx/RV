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
 * file .......: ltiCubicSpline.h
 * authors ....: Ruediger Weiler
 * organization: LTI, RWTH Aachen
 * creation ...: 28.5.2001
 * revisions ..: $Id: ltiCubicSpline.h,v 1.8 2005/06/16 12:37:34 doerfler Exp $
 */

#ifndef _LTI_CUBICSPLINE_H_
#define _LTI_CUBICSPLINE_H_


#include "ltiPointList.h"
#include "ltiVariablySpacedSamplesInterpolator.h"

namespace lti {
  /**
   * This functor implements a cubic spline interpolation
   * for tabulated functions. The input parameters are some
   * sampling points in a tpointList<double>. The requirements
   * of the input points are that they must be a kind of function.
   * Therefore this rule x(n) < x(n+1) is required for each n
   * and the list must consist of at least three points.
   * Furthermore the parameters consist of the derivatives at the
   * beginning (first point) and end of the function (last point).
   * If the parameter useNaturalDerivatives is \a true the derivatives
   * from the parameters are ignored and the algorithm calculates
   * the optimal derivatives at the beginning and end.
   *
   * The template T type corresponds to the coordinate types.  Only the
   * float, double and int types are supported.
   */
  template <class T>
  class cubicSpline : public variablySpacedSamplesInterpolator {
  public:
    // --------------------------------------------------
    // cubicSpline::parameters
    // --------------------------------------------------

    /**
     * the parameters for the class cubicSpline
     */
    class parameters : public variablySpacedSamplesInterpolator::parameters {
    public:
      /**
       * default constructor
       */
      parameters()     : variablySpacedSamplesInterpolator::parameters() {
        //TODO: Initialize your parameter values!
        // If you add more parameters manually, do not forget to do following:
        // 1. indicate in the default constructor the default values
        // 2. make sure that the copy member also copy your new parameters
        // 3. make sure that the read and write members also read and
        //    write your parameters

        //use the natural Condition is the default
        useNaturalDerivatives = true;

        //the default derivative is 0
        derivativeAtLastPoint = double(0);
        derivativeAtFirstPoint = double(0);

        samplingPoints.clear();
      };

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other) 
        : variablySpacedSamplesInterpolator::parameters() {
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
      const char* getTypeName() const  {
        return "cubicSpline::parameters";
      };

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other) {
# ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        variablySpacedSamplesInterpolator::parameters::copy(other);
# else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        variablySpacedSamplesInterpolator::parameters& (variablySpacedSamplesInterpolator::parameters::* p_copy)
          (const variablySpacedSamplesInterpolator::parameters&) =
          variablySpacedSamplesInterpolator::parameters::copy;
        (this->*p_copy)(other);
# endif


        derivativeAtLastPoint = other.derivativeAtLastPoint;
        derivativeAtFirstPoint = other.derivativeAtFirstPoint;
        samplingPoints = other.samplingPoints;
        useNaturalDerivatives = other.useNaturalDerivatives;

        return *this;
      };

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other) {
        return copy(other);
      };

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const {
        return new parameters(*this);
      };

#   ifndef _LTI_MSC_6
      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was succeful
       */
      bool write(ioHandler& handler, const bool complete=true) const
#   else
      bool writeMS(ioHandler& handler, const bool complete=true) const
#   endif
      {
        bool b = true;
        if (complete) {
          b = handler.writeBegin();
        }

        if (b) {

          lti::write(handler,"useNaturalDerivatives",useNaturalDerivatives);
          lti::write(handler,"derivativeAtLastPoint",derivativeAtLastPoint);
          lti::write(handler,"derivativeAtFirstPoint",derivativeAtFirstPoint);
          lti::write(handler,"samplingPoints",samplingPoints);
        }

#     ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not able to
        // compile...
        b = b && variablySpacedSamplesInterpolator::parameters::write(handler,false);
#     else
        bool
          (variablySpacedSamplesInterpolator::parameters::* p_writeMS)(ioHandler&,const bool) const =
          variablySpacedSamplesInterpolator::parameters::writeMS;
        b = b && (this->*p_writeMS)(handler,false);
#     endif

        if (complete) {
          b = b && handler.writeEnd();
        }

        return b;
      }

#   ifdef _LTI_MSC_6
      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was succeful
       */
      bool write(ioHandler& handler, const bool complete=true) const {
        // ...we need this workaround to cope with another really
        // awful MSVC bug.
        return writeMS(handler,complete);
      }
#   endif

      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also read, otherwise only the data block will be read.
       * @return true if write was succeful
       */
#   ifndef _LTI_MSC_6
      bool read(ioHandler& handler, const bool complete=true)
#   else
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
      bool readMS(ioHandler& handler, const bool complete=true)
#   endif
      {
        bool b = true;
        if (complete) {
          b = handler.readBegin();
        }

        if (b) {

          lti::read(handler,"useNaturalDerivatives",useNaturalDerivatives);
          lti::read(handler,"derivativeAtLastPoint",derivativeAtLastPoint);
          lti::read(handler,"derivativeAtFirstPoint",derivativeAtFirstPoint);
          lti::read(handler,"samplingPoints",samplingPoints);
        }

#     ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not able to
        // compile...
        b = b && variablySpacedSamplesInterpolator::parameters::read(handler,false);
#     else
        bool (variablySpacedSamplesInterpolator::parameters::* p_readMS)(ioHandler&,const bool) =
          variablySpacedSamplesInterpolator::parameters::readMS;
        b = b && (this->*p_readMS)(handler,false);
#     endif

        if (complete) {
          b = b && handler.readEnd();
        }

        return b;
      }

#   ifdef _LTI_MSC_6
      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also read, otherwise only the data block will be read.
       * @return true if write was succeful
       */
      bool read(ioHandler& handler,  const bool complete) {
        // ...we need this workaround to cope with another really awful MSVC
        // bug.
        return readMS(handler,complete);
      }
#   endif

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
       * This boolean controls if the algorithm uses the derivative
       * values given within the parameters object (false), or if the functor
       * should calculate the optimal derivatives (true).
       * Default value: true
       */
      bool useNaturalDerivatives;

      /**
       * This parameter saves the derivative at the begin of the function.
       * If the spline should start horizontal the parameter
       * has to be zero. If the parameter useNaturalDerivatives
       * is true this value has no effect.
       * Default value: 0.0
       */
      double derivativeAtFirstPoint;

      /**
       * This parameter saves the derivative at the end of the function.
       * If the spline should end horizontal the parameter
       * has to be zero. If the parameter useNaturalDerivatives
       * is true this value has no effect.
       * Default value: 0.0
       */
      double derivativeAtLastPoint;

      /**
       * This is the list of the sampling points.
       * Every x-value must be unique in this tpointList<T>
       * and the list must consist of at least three points, or the
       * apply method will fail
       * Default value:  empty list!
       */
      tpointList<T> samplingPoints;
    };

    /**
     * default constructor
     */
    cubicSpline();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    cubicSpline(const cubicSpline& other);

    /**
     * destructor
     */
    virtual ~cubicSpline();

    /**
     * returns the name of this type ("cubicSpline")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on the given %parameter.
     * @param srcdest tpointList<double> with the source data in the
     * x part of the tpoints. The result for every x value is writen
     * in the y part of the same tpoint.
     * @return true if apply successful or false otherwise.
     */
    bool apply(tpointList<T>& srcdest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param x double with the source data.
     * @param fx double where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const T& x,T& fx) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    cubicSpline& copy(const cubicSpline& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * sets the functor's parameters.
     */
    virtual bool updateParameters();

  protected:
    /**
     * This vector saves the second derivatives at each point
     * of the sampling points.
     */
    std::vector<double> secondDerivatives;

    /**
     * This function calcualtes the second derivatives at each point of
     * the tpointList<double> &in.
     * @param in the point list with the source data
     * @param yp1 the first derivative an the first point of the tpointlist
     * @param ypn the first derivative an the last point of the tpointlist
     * @param natural if true constructs a natural spline
     */
    void spline(const tpointList<T>& in,
                const double& yp1,
                const double& ypn,
                const bool& natural);

    /**
     * This function evaluate the spline at one x value.
     * @param function list of points to be interpolated
     * @param x the value where the spline is evaluated
     * @param y the return value
     * @return true if successful, false otherwise
     */
    bool splint(const tpointList<T> &function,
                const T& x, T& y) const;
  };
}

#endif


