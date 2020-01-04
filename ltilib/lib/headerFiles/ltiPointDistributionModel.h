/*
 * Copyright (C) 1998-2004
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
 * file .......: ltiPointDistributionModel.h
 * authors ....: Benjamin Winkler
 * organization: LTI, RWTH Aachen
 * creation ...: 12.6.2001
 * revisions ..: $Id: ltiPointDistributionModel.h,v 1.13 2005/06/16 12:35:43 doerfler Exp $
 */

#ifndef _LTI_POINT_DISTRIBUTION_MODEL_H_
#define _LTI_POINT_DISTRIBUTION_MODEL_H_


#include "ltiIoObject.h"
#include "ltiPointList.h"
#include "ltiVector.h"
#include "ltiMatrix.h"

namespace lti {
  /**
   * This class is the data structure for a Point Distribution Model (PDM).
   *
   * A PDM allows to represent a shape and its possible deformations, by a
   * mean shape and the eigenvectors and eigenvalues of the shape set. It is
   * estimated by the pdmGenerator and can be used to regularize the
   * deformations of an activeShapeModel (ASM).
   *
   * For a description of PDMs see: Sonka, "Image Processing, Analysis, and
   * Machine Vision", p380ff.
   *
   * Additional Remark:
   *
   * In ASMs a shape x is described as the sum of mean shape and the weighted
   * eigenvectors.
   * Note that in common a subset of all eigenvectors is chosen (i.e. the
   * eigenvectors with the first n largest eigenvalues) for data compression
   * reasons.
   *
   * <code>
   * x = xMean + eigenVectors*w
   * </code>
   *
   */
  class pointDistributionModel : public ioObject {
  public:

    /**
     * default constructor
     */
    pointDistributionModel();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    pointDistributionModel(const pointDistributionModel& other);

    /**
     * destructor
     */
    virtual ~pointDistributionModel();

    /**
     * copy data of "other" pointDistributionModel.
     * Please note that the status string will _NOT_ be copied!
     */
    pointDistributionModel& copy(const pointDistributionModel& other);

    /**
     * assigment operator (alias for copy(other)).
     * @param other the pointDistributionModel to be copied
     * @return a reference to the actual pointDistributionModel
     */
    pointDistributionModel& operator=(const pointDistributionModel& other);

    /**
     * returns the name of this type ("pointDistributionModel")
     */
    const char* getTypeName() const;


    /**
     * write the pointDistributionModel in the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool write(ioHandler& handler, const bool complete=true) const;

    /**
     * read the pointDistributionModel from the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool read(ioHandler& handler,const bool complete=true);

#   ifdef _LTI_MSC_6
    /**
     * this function is required by MSVC only, as a workaround for a
     * very awful bug, which exists since MSVC V.4.0, and still by
     * V.6.0 with all bugfixes (so called "service packs") remains
     * there...  This method is public due to another bug!, so please
     * NEVER EVER call this method directly
     */
    bool readMS(ioHandler& handler,const bool complete=true);

    /**
     * this function is required by MSVC only, as a workaround for a
     * very awful bug, which exists since MSVC V.4.0, and still by
     * V.6.0 with all bugfixes (so called "service packs") remains
     * there...  This method is public due to another bug!, so please
     * NEVER EVER call this method directly
     */
    bool writeMS(ioHandler& handler,const bool complete=true) const;
#   endif


    //
    // data
    //

    /**
     * Typedef for a single PDM shape. A shape is an ordered list of
     * landmark points of a 2D object. Generally this will be created
     * by sampling a fixed number of N points along the boundary.
     */
    typedef tpointList<float> shape;


    /**
     * The mean (average) shape of the model.
     */
    shape meanShape;

    /**
     * The matrix of all eigenvectors computed over a set of shapes.
     * Eigenvectors are sorted in descending order of magnitude of
     * their corresponding eigenvalues.
     */
    matrix<double> eigenVectorMatrix;

    /**
     * The eigenvalues, that belong to the eigenvectors, stored in a vector.
     * The eigenvalue is the variance of the set of shapes along the direction
     * of the eigenvector, that it belongs to.
     */
    vector<double> varianceVector;

  };


  /**
   * write a pointDistributionModel
   */
  bool write(ioHandler& handler, const pointDistributionModel& p,
             const bool complete=true);

  /**
   * read a pointDistributionModel
   */
  bool read(ioHandler& handler, pointDistributionModel& p,
            const bool complete=true);

}

#endif

