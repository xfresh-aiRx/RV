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
 * file .......: ltiSelective2DConvolution.h
 * authors ....: Henning Luepschen
 * organization: LTI, RWTH Aachen
 * creation ...: 10.9.2003
 * revisions ..: $Id: ltiSelective2DConvolution.h,v 1.7 2005/06/16 12:35:44 doerfler Exp $
 */

#ifndef _LTI_SELECTIVE2_D_CONVOLUTION_H_
#define _LTI_SELECTIVE2_D_CONVOLUTION_H_


#include "ltiLinearKernels.h" 
#include "ltiFilter.h"
#include "ltiPointList.h"

namespace lti {
  
  /**
   *  This modifier is used to convolve individual points of
   *  a given channel with a arbitrary kernel2D<float> (e.g. a
   *  gaussian kernel like the one in "ltiGaussKernels.h").
   *  
   *  The points to be convolved may be given in a mask channel,
   *  as pointList, or as individual points.
   *
   *  TODO: The boundaryType is not checked yet!!! Border points 
   *  cannot and will not be convolved.
   *
   *  Example of determinating the GradX and GradY values
   *  for a given channel src using a mask channel:
   *  \code
   *
   *  #include "ltiGradientKernels.h"
   *  #include "ltiSelective2DConvolution.h"
   *  #include "ltiImage.h"
   *  
   *  lti::channel src, mask;
   *  .
   *  .
   *  .
   *  //Fill mask and src channel as necessary
   *  .
   *  .
   *  .
   *
   *  lti::gradientKernelX<float> myKernelX(5);   // Optimal Gradient 5x5 kernels
   *  lti::gradientKernelY<float> myKernelY(5);   // They both are separable.
   *  lti::kernel2D<float> myKernel2D;            // We need genuine 2D kernels, not separable ones.
   *
   *  myKernel2D.castFrom(myKernelX);             // So, we create one :-)
   *  lti::selective2DConvolution mySelConvX(myKernel2D);
   *
   *  myKernel2D.castFrom(myKernelY);
   *  lti::selective2DConvolution mySelConvY;
   *  mySelConvY.setKernel(myKernel2D);           // Another way of setting an arbitrary kernel.
   *
   *  lti::channel GradX,GradY;                   // Where to put the results
   *  
   *  mySelConvX.apply(src,GradX,mask);           // All points in the mask channel having a value
   *  mySelConvY.apply(src,GradY,mask);           // !=0.0f are being convolved.
   *  \endcode
   *  
   *  Now, we have the results in the GradX and GradY channel.
   *
   */
  class selective2DConvolution : public filter {
  public:
    /**
     * the parameters for the class selective2DConvolution
     */
    class parameters : public filter::parameters {
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
       * returns the kernel2D<float> in use.  If it is not set yet, an
       * lti::invalidParameters exception will be thrown
       * @return a const reference to the filter kernel.
       */
      const kernel2D<float>& getKernel2D() const;

      /**
       * sets the filter kernel2D<float> to be used.
       * A copy of the given %parameter will be made!
       * @param aKernel the filter kernel to be used
       */
      void setKernel2D(const kernel2D<float>& aKernel);



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
      
    protected:
      /**
       * pointer to the filter kernel2D copy
       */
      kernel2D<float>* kernel2D;

    };

    /**
     * default constructor
     */
    selective2DConvolution();

    /**
     * Construct a functor using the given parameters
     */
    selective2DConvolution(const parameters& par);

    /**
     * Construct a selective2DConvolution functor with a parameters set
     * which includes the given filter kernel.
     */
    selective2DConvolution(const kernel2D<float>& aKernel);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    selective2DConvolution(const selective2DConvolution& other);

    /**
     * destructor
     */
    virtual ~selective2DConvolution();

    /**
     * returns the name of this type ("selective2DConvolution")
     */
    virtual const char* getTypeName() const;
    

    /**
     * operates on a copy of the given %parameters.
     * @param src channel with the source data.
     * @param mask channel with the mask data. All points in the src channel
     * having a corresponding point in the mask channel
     * with values different from 0 are convolved with the given kernel.
     * @param dest channel where the result will be left.
     * @return true if apply successful or false otherwise.
     */ 
    bool apply(const channel& src, channel& dest, const channel& mask) const;
 
    /**
     * operates on a copy of the given %parameters. Convolves only
     * one specific point in the source channel with a given kernel. The
     * result can be found in the result variable.
     * @param src channel with the source data.
     * @param result The result of the convolution can be found here.
     * @param column Column of the convolved point.
     * @param row Row of the convolved point
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src, float& result, const int row, const int column) const;

    /**
     * operates on a copy of the given %parameters. Convolves only
     * one specific point in the source channel with a given kernel. The
     * result will be written into a destination channel.
     * @param src channel with the source data.
     * @param column Column of the convolved point.
     * @param row Row of the convolved point
     * @param dest channel where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src, channel& dest, const int row, const int column) const;

    /**
     * operates on a copy of the given %parameters. Convolves only
     * one specific point in the source channel with a given kernel.
     * @param src channel with the source data.
     * @param result The result of the convolution can be found here.
     * @param thePoint Convolved point.
     * @return true if apply successful or false otherwise.
     */ 
    bool apply(const channel& src, float& result, const point& thePoint) const;

    /**
     * operates on a copy of the given %parameters. Convolves only
     * one specific point in the source channel with a given kernel. The
     * result will be written into a destination channel.
     * @param src channel with the source data.
     * @param thePoint Convolved point.
     * @param dest channel where the result will be left.
     * @return true if apply successful or false otherwise.
     */ 
    bool apply(const channel& src, channel& dest, const point& thePoint) const;
    
    /**
     * operates on a copy of the given %parameters. Convolves all points
     * of a pointlist in the source channel with a given kernel. The result
     * will be written into a destination channel.
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @param thePointList Given pointlist. All points of the list are 
     *        convolved.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src, channel& dest, 
               const pointList& thePointList) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    selective2DConvolution& copy(const selective2DConvolution& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    selective2DConvolution& operator=(const selective2DConvolution& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * shortcut to set the filter kernel2D in the functor parameters.
     * The other parameters remain unchanged.
     */
    void setKernel2D(const kernel2D<float>& aKernel);


    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

  };
}

#endif

