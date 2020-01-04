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
 * file .......: ltiLinearRegressionTracking.h
 * authors ....: Holger Fillbrandt
 * organization: LTI, RWTH Aachen
 * creation ...: 4.7.2003
 * revisions ..: $Id: ltiLinearRegressionTracking.h,v 1.6 2005/06/16 12:35:11 doerfler Exp $
 */

#ifndef _LTI_LINEAR_REGRESSION_TRACKING_H_
#define _LTI_LINEAR_REGRESSION_TRACKING_H_

#include "ltiImage.h"
#include "ltiLinearRegression.h"

namespace lti {

  /**
   * With this tracking algorithm it is possible to track the movement
   * of an object together with scaling and rotation in the image plane.
   * It uses the difference between the reference image of the object and
   * the image values at the current position to estimate the correction
   * of the geometrical parameters. This relation is calculated in a training
   * phase using linear regression. To achieve stable tracking results, the
   * image values in the background should be similar to those in the
   * training image. It is also required that the movement of the object
   * between subsequent frames is not too big, there should be still an
   * overlap of the object areas at the two positions.
   */
  class linearRegressionTracking : public linearRegression<double> {
  public:
    /**
     * the parameters for the class linearRegressionTracking
     */
    class parameters : public linearRegression<double>::parameters {
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
       * maximum displacement in x-direction to be learned (in pixels)
       * default: 10
       */
      int maxXDisplace; 

      /**
       * maximum displacement in y-direction to be learned (in pixels)
       * default: 10
       */
      int maxYDisplace; 

      /**
       * maximum rotation angle to be learned (in rad)
       * default: 0.2
       */
      float maxAngleDiff;
      
      /**
       * maximum scaling difference to be learned
       * default: 0.1
       */
      float maxScaleDiff;

      /**
       * size of training set (number of random displacements)
       * default: 100
       */
      int trainingSize;
      

    };

    /**
     * default constructor
     */
    linearRegressionTracking();

    /**
     * Construct a functor using the given parameters
     */
    linearRegressionTracking(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    linearRegressionTracking(const linearRegressionTracking& other);

    /**
     * destructor
     */
    virtual ~linearRegressionTracking();

    /**
     * returns the name of this type ("linearRegressionTracking")
     */
    virtual const char* getTypeName() const;

    /**
     * training of x- and y-movement in the image plane using channel values
     * @param referenceImage reference channel to be used for training
     * @param imageSection rectangle that surrounds the object in the given image
     */
    bool learnDisplacement(channel& referenceImage, rectangle imageSection);

    /**
     * training of x- and y-movement in the image plane using RGB values
     * @param referenceImage reference image to be used for training
     * @param imageSection rectangle that surrounds the object in the given image
     */
    bool learnDisplacementRGB(image& referenceImage, rectangle imageSection);

    /**
     * estimates the object displacement from the difference of current and reference
     * channel values
     * @param theImage current frame channel
     * @param xpos current x-position of left upper edge of object
     * @param ypos current y-position of left upper edge of object
     * @param dx estimated displacement in x-direction
     * @param dy estimated displacement in y-direction
     */
    void getDisplacement(channel& theImage, int xpos, int ypos, double &dx, double &dy) const;

    /**
     * estimates the object displacement from the difference of current and reference
     * RGB values
     * @param theImage current frame channel
     * @param xpos current x-position of left upper edge of object
     * @param ypos current y-position of left upper edge of object
     * @param dx estimated displacement in x-direction
     * @param dy estimated displacement in y-direction
     */
    void getDisplacementRGB(image& theImage, int xpos, int ypos, double &dx, double &dy) const;   

    /**
     * training of x- and y-movement as well as scaling and rotation in the image plane 
     * using channel values
     * @param referenceImage reference channel to be used for training
     * @param imageSection rectangle that surrounds the object in the given image
     */
    bool learnTracking(channel& referenceImage, rectangle imageSection);

    /**
     * training of x- and y-movement as well as scaling and rotation in the image plane 
     * using RGB values
     * @param referenceImage reference channel to be used for training
     * @param imageSection rectangle that surrounds the object in the given image
     */
    bool learnTrackingRGB(image& referenceImage, rectangle imageSection);

    /**
     * estimates the object displacement, scaling and rotation from the difference of 
     * current and reference channel values. Use more than one iteration per frame
     * for best results.
     * @param theImage current frame channel
     * @param xpos current x-position of object center
     * @param ypos current y-position of object center
     * @param alpha current object angle
     * @param scale current object scale
     * @param dx estimated displacement in x-direction
     * @param dy estimated displacement in y-direction
     * @param dAlpha estimated angle difference
     * @param dScale estimated scale difference
     */
    void getDispRotScale(channel& theImage, double xpos, double ypos, double alpha, double scale, double &dx, double &dy, double &dAlpha, double &dScale) const;

    /**
     * estimates the object displacement, scaling and rotation from the difference of 
     * current and reference RGB values. Use more than one iteration per frame
     * for best results.
     * @param theImage current frame image
     * @param xpos current x-position of object center
     * @param ypos current y-position of object center
     * @param alpha current object angle
     * @param scale current object scale
     * @param dx estimated displacement in x-direction
     * @param dy estimated displacement in y-direction
     * @param dAlpha estimated angle difference
     * @param dScale estimated scale difference
     */
    void getDispRotScaleRGB(image& theImage,
                            double xpos,
                            double ypos,
                            double alpha,
                            double scale,
                            double &dx,
                            double &dy,
                            double &dAlpha,
                            double &dScale) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    linearRegressionTracking& copy(const linearRegressionTracking& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    linearRegressionTracking& operator=(const linearRegressionTracking& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * reads this functor from the given handler.
     */
    virtual bool read(ioHandler &handler, const bool complete=true);

    /**
     * writes this functor to the given handler.
     */
    virtual bool write(ioHandler &handler, const bool complete=true) const;

    /**
     * gets the values of the reference image section used for calculating the error vector
     * @param referenceSection contains either the grey-scale or RGB-values in a vector
     * @param w width of image section
     * @param h height of image section
     */
    void getReferenceSection(vector<double> &referenceSection, int &w, int &h) const;

    /**
     * sets the values of the reference image section used for calculating the error vector
     * @param referenceSection contains either the grey-scale or RGB-values in a vector
     * @param w width of image section
     * @param h height of image section
     */
    void setReferenceSection(vector<double> &referenceSection, int w, int h);


    // member variables

    protected:
    /**
     * reference image values (grey level or RGB-values subsequently) 
     * of the image region which shall be tracked
     */
    vector<double> referenceValues;

    /**
     * width and height of the image region
     */
    int width, height;


  };
}

#endif

