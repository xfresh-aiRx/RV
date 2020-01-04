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
 * file .......: ltiSOFM2D.h
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 20.8.2002
 * revisions ..: $Id: ltiSOFM2D.h,v 1.10 2005/06/16 09:32:40 doerfler Exp $
 */

#ifndef _LTI_S_O_F_M2_D_H_
#define _LTI_S_O_F_M2_D_H_

#include "ltiSOFM.h"
#include "ltiLinearKernels.h"
//#include "ltiImage.h"

namespace lti {
  /**
   *  This is a Self-Organizing Feature Map with a 2-D grid of neurons.
   */
  class SOFM2D : public SOFM {
  public:
    /**
     * the parameters for the class SOFM2D
     */
    class parameters : public SOFM::parameters {
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
      virtual ~parameters();

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
      virtual classifier::parameters* clone() const;

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
       * The dimensions of the grid can be given explicitely
       * (calculateSize=false) by setting sizeX and sizeY or
       * calculated from the distribution of the training points. In
       * the latter case, the two highest eigenvalues are calculated
       * and multiplied by factor that after rounding the error to the
       * given area is as small as possible. This method yields better
       * unfolding of the grid at the cost of more calculations and
       * possibly failure due to singularity of the covariance matrix
       * of the training samples.<p>
       * The default is false.
       */
      bool calculateSize;

      /**
       * size of the grid of neurons in the first dimension. Default 0.
       */
      int sizeX;

      /**
       * size of the grid of neurons in the second dimension. Default 0.
       */
      int sizeY;

      /**
       * approximate area of the grid when the dimension's sizes are
       * calculated from the distribution of training points. Default 0.
       */
      int area;

    };

    /**
     * default constructor
     */
    SOFM2D();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    SOFM2D(const SOFM2D& other);

    /**
     * destructor
     */
    virtual ~SOFM2D();

    /**
     * returns the name of this type ("SOFM2D")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" classifier.
     * @param other the classifier to be copied
     * @return a reference to this classifier object
     */
    SOFM2D& copy(const SOFM2D& other);

    /**
     * alias for copy member
     * @param other the classifier to be copied
     * @return a reference to this classifier object
     */
    SOFM2D& operator=(const SOFM2D& other);

    /**
     * returns a pointer to a clone of this classifier.
     */
    virtual classifier* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Unsupervised training.  The vectors in the <code>input</code>
     * matrix will be put into groups according to the training
     * algorithm.  Additionally, an integer indicating the class each
     * point belongs to is returned. <p> By default this method uses
     * the other train method train(const dmatrix&) and then
     * calls classify(const dvector&) to get the ids for each
     * trainvector. These ids are then returned.
     * @param input the matrix with the input vectors (each row is a training
     *              vector)
     * @param ids vector of class ids for each input point
     * @return true if successful, false otherwise. (if false you can check
     *              the error message with getStatusString())
     */
    virtual bool train(const dmatrix& input,
                       ivector& ids);

    /**
     * Unsupervised training.
     * The row vectors in the <code>input</code> matrix
     * are used for training of the classifier.
     * @param input the matrix with the input vectors (each row is a training
     *              vector)
     * @return true if successful, false otherwise. (if false you can check
     *              the error message with getStatusString())
     */
    virtual bool train(const dmatrix& input);

    //TODO Check whether you really need a new classify method.
    // In some cases the superclasses method will suffice. Then just
    // delete the declaration and its implementation stump.

    /**
     * Classification.
     * Classifies the feature and returns the outputVector with
     * the classification result.
     * @param feature the %vector to be classified
     * @param result the result of the classification
     * @return false if an error occurred during classification else true
     */
    virtual bool
      classify(const dvector& feature, outputVector& result) const;

    /**
     * returns the size (x,y) of the feature map.
     */
    inline point size() const {
      point p(sizeX,sizeY);
      return p;
    };

    /**
     * Returns the size of the map in x-direction (number of columns)
     */
    inline int xSize() const {return sizeX;};

    /**
     * Returns the size of the map in y-direction (number of rows)
     */
    inline int ySize() const {return sizeY;};

    /**
     * write the SOFM2D in the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool write(ioHandler& handler,const bool complete=true) const;

    /**
     * read the SOFM2D from the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool read(ioHandler& handler,const bool complete=true);


  protected:

    /** size of the first dimension */
    int sizeX;
    /** size of the second dimension */
    int sizeY;

    /** the highest eigenvalue of the train data */
    double eva1;
    /** the second highest eigenvalue of the train data */
    double eva2;
    /** the the eigenvector corresponding to eva1 */
    dvector eve1;
    /** the the eigenvector corresponding to eva2 */
    dvector eve2;

    /** calculate sizeX and sizeY in case of param.calulateSize is true */
    bool calcSize(const dmatrix& data);

    /** initializes the grid according to parameters.initType */
    bool initGrid(const dmatrix& data);

    /** calculate the neigborhood kernel */
    void getNeighborhoodKernel(const int& maxN, kernel2D<double>& facN);

    /** train the SOFM using L1- or L2-Norm */
    bool trainDist(const dmatrix& data);

    /** train the SOFM using dot product */
    bool trainDot(const dmatrix& data);
  };
}

#endif

