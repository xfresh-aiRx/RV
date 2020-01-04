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
 * file .......: ltiFourierDescriptor.h
 * authors ....: Norman Pfeil
 * organization: LTI, RWTH Aachen
 * creation ...: 26.4.2001
 * revisions ..: $Id: ltiFourierDescriptor.h,v 1.12 2005/06/16 12:35:10 doerfler Exp $
 */

#ifndef _LTI_FOURIER_DESCRIPTOR_H_
#define _LTI_FOURIER_DESCRIPTOR_H_


#include "ltiTransform.h"
#include "ltiRealFFT.h"
#include "ltiRealInvFFT.h"
#include "ltiContour.h"
#include "ltiVector.h"
#include "ltiObjectsFromMask.h"
#include "ltiDraw.h"
#include "ltiMath.h"

namespace lti {
  /**
   *  This class generates fourier descriptors. The input is a list of border points.
   *  The output are four vectors of fourier descriptors:
   *  x value (Real,Imaginary) and y value (Real,Imaginary)
   *
   *  The algorithm: http://iul.cs.byu.edu/morse/550-F95/node29.html
   *  A demo: http://www.stud.ee.ethz.ch/~buc/brechbuehler/fourdem.html
   */

  class fourierDescriptor : public transform {
  public:
    /**
     * the parameters for the class fourierDescriptor
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
       * write the parameters in the given ioHandler
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
       * origin is an arbitrary two-dimensional point, default is (0,0).
       * It indicates the reference point for calculating the fourier descriptors.
       * E.g. the centre of gravity of an object creates translation invariant descriptors.
       */
      lti::tpoint<float> origin;

    };

    /**
     * default constructor
     */
    fourierDescriptor();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    fourierDescriptor(const fourierDescriptor& other);

    /**
     * destructor
     */
    virtual ~fourierDescriptor();

    /**
     * returns the name of this type ("fourierDescriptor")
     */
    virtual const char* getTypeName() const;

    /**
     * With the given borderpoints and the parameter "origin" (default: 0,0) the apply method calculates the
     * fourier descriptors (abscissa real and imaginary value + ordinate real and imaginary value).
  	 * @param input borderPoints with the input data.
  	 * @param xRealOutput vector<float> where the fourier descriptor of the abscissa will be left (real value).
  	 * @param xImagOutput vector<float> where the fourier descriptor of the abscissa will be left (imaginary value).
  	 * @param yRealOutput vector<float> where the fourier descriptor of the ordinate will be left (real value).
  	 * @param yImagOutput vector<float> where the fourier descriptor of the ordinate will be left (imaginary value).
     */
    bool apply( const borderPoints&  input,
                vector<float>& xRealOutput,
                vector<float>& xImagOutput,
                vector<float>& yRealOutput,
                vector<float>& yImagOutput) const;

    /**
     * Reconstructs borderpoints with given Input and the specified parameter "origin".
  	 * @param xRealInput vector<float> real abscissa value of fourier descriptor.
  	 * @param xImagInput vector<float> imaginary abscissa value of fourier descriptor.
  	 * @param yRealInput vector<float> real ordinate value of fourier descriptor.
  	 * @param yImagInput vector<float> imaginary ordinate value of fourier descriptor.
  	 * @param output borderPoints with the output data.
     */
    bool apply( const vector<float>& xRealInput,
                const vector<float>& xImagInput,
                const vector<float>& yRealInput,
                const vector<float>& yImagInput,
                borderPoints&  output) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    fourierDescriptor& copy(const fourierDescriptor& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;
  };
}


#endif

