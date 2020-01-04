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
 * file .......: ltiActiveShapeModel.h
 * authors ....: Benjamin Winkler, Holger Fillbrandt
 * organization: LTI, RWTH Aachen
 * creation ...: 1.3.2002
 * revisions ..: $Id: ltiActiveShapeModel.h,v 1.13 2005/06/16 09:32:41 doerfler Exp $
 */

#ifndef _LTI_ACTIVE_SHAPE_MODEL_H_
#define _LTI_ACTIVE_SHAPE_MODEL_H_


#include "ltiPointDistributionModel.h"
#include "ltiGeometricTransform.h"
#include "ltiFunctor.h"

namespace lti {
  /**
   * This is the base class for active shape models (ASM). ASMs are deformable
   * 2D-templates, that can align themselves to image features according
   * to defined search strategies (see derived classes). The deformation is
   * regularized (i.e. constrained) by an underlying PDM (pointDistributionModel).
   *
   * This class provides the methods for aligning PDM shapes (pointDistributionModel::shape)
   * and to correct invalid shapes to satisfy the restrictions of a given PDM.
   * Furthermore, methods to obtain information about the position of a shape are provided.
   * Shapes with weighted points (point reliabilities) are also supported.
   */
  class activeShapeModel : public functor {
  public:
    /**
     * the parameters for the class activeShapeModel
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

      /**
       * The PDM (pointDistributionModel), that regularizes the deformation of
       * an ASM.
       */
      pointDistributionModel model;

      /**
       * Defines the limit for valid shapes in eigenspace.
       * The dimensions, that exceede the limit given by
       * varianceCoefficient*eigenvalue , where eigenvalue is
       * the corresponding variance, are set to 0.0 .
       *
       * Default value is 3.0
       */
      double varianceCoefficient;

      /**
       * Weight points by point reliabilities before correcting the shape.
       * I.e. unreliable points are moved towards the mean shape. Thus if
       * true, then use
       *
       * <code>
       *   w = (eigenVectorMatrix)^T * ((shape - meanShape) * reliabilities)
       * </code>
       *
       * instead of
       *
       * <code>
       *   w = (eigenVectorMatrix)^T * (shape - meanShape)
       * </code>
       *
       * By default this is set to false.
       * Reliability values are then only considered for the geometric
       * transformations (shift, rotate, scale).
       */
      bool reliabilityAsWeightingFactor;

    };

    /**
     * default constructor
     */
    activeShapeModel();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    activeShapeModel(const activeShapeModel& other);

    /**
     * destructor
     */
    virtual ~activeShapeModel();

    /**
     * returns the name of this type ("activeShapeModel")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    activeShapeModel& copy(const activeShapeModel& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    activeShapeModel& operator=(const activeShapeModel& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * @name Transformation
     * Methods for transforming a shape from shape space to eigenspace and
     * vice versa.
     *
     * Eigenspace (w) -> shape space (x):
     *               x = eigenVectorMatrix * weightsVector  +  meanShape <br>
     * Shape space (x) -> eigenspace (w): 
     *               w = (eigenVectorMatrix)^T * (shape - meanShape) <br>
     */
    //@{

    /**
     * convert a shape to a weights vector, i.e. project a shape into the pdm
     * eigenspace
     *
     * Formula: w = (eigenVectorMatrix)^T * (shape - meanShape)
     *
     * Note: the shape is assumed to be aligned to position, size and rotation
     * of meanShape!
     *
     * @param src shape with the source data.
     * @param dest vector where the result will be left.
     */
    void convertShapeToWeightsVector(const pointDistributionModel::shape &src,
                                     vector<double> &dest) const;

    /**
     * convert a reliability-weighted shape to a weights vector,
     * i.e. project a shape into the pdm eigenspace
     * Formula:
     * w = (eigenVectorMatrix)^T * ((shape - meanShape) * reliabilities)
     * Note: the shape is assumed to be aligned to position, size and rotation
     * of meanShape!
     * @param src shape with the source data.
     * @param dest vector where the result will be left.
     * @param pointReliability vector containing point reliabilities.
     */
    void convertShapeToWeightsVector(const pointDistributionModel::shape &src,
                                     vector<double> &dest,
                                     const fvector &pointReliability) const;

    /**
     * convert a weights vector into a shape (i.e. shape space)
     * Formula: x = eigenVectorMatrix * weightsVector + meanShape
     * @param src vector with the source data.
     * @param dest shape where the result will be left.
     */
    void 
    convertWeightsVectorToShape(const vector<double> &src, 
                                pointDistributionModel::shape &dest) const;
    //@}

    /** 
     * @name Comparison
     * Methods for comparing and aligning two shapes.
     */
    //@{
    
    /**
     * align a given shape to another.
     *
     * note that the return geometricTransform::parameters describe the
     * transformation from the referenceShape to the unaligned shape! this
     * is done to align a given shape to a normalized shape and be able to
     * easily project this normalized shape (e.g. meanShape) on the original
     * shape.
     *
     * @param shape shape to be aligned. the transformed shape will also be
     *              saved here.
     * @param referenceShape fixed shape to be aligned to
     * @return geometric parameters that are used to re-transform shape
     */
    geometricTransform::parameters 
    alignShape(pointDistributionModel::shape &shape,
               const pointDistributionModel::shape &referenceShape) const;
    
    /**
     * align a given weighted shape to another.
     *
     * @param shape shape to be aligned. the transformed shape will also be
     *              saved here.
     * @param referenceShape fixed shape to be aligned to
     * @param pointReliability vector containing reliability values between
     *                         0 (unreliable) and 1 (reliable) for all points
     * @return geometric parameters that are used to re-transform shape
     */
    geometricTransform::parameters
    alignShape(pointDistributionModel::shape &shape,
               const pointDistributionModel::shape &referenceShape,
               const fvector &pointReliability) const;
    
    /**
     * calculate mean difference of two shapes, i.e. the sum of squared
     * distances of all corresponding points divided by the number of points
     */
    float 
    calculateShapeDifference(const pointDistributionModel::shape &firstShape,
                             const pointDistributionModel::shape &secondShape
                             ) const;
    
    /**
     * calculate difference of two weighted shapes
     */
    float
    calculateShapeDifference(const pointDistributionModel::shape &firstShape,
                             const pointDistributionModel::shape &secondShape,
                             const fvector &pointReliability) const;
    //@}
    
    
    /**
     * @name Properties
     * Methods for computing properties of one shape.
     */
    //@{
    
    /**
     * calculate the center of a given shape.
     * all points of the shape are summed and the results divided by the
     * number of points.
     * @return center of the given shape
     */
    tpoint<float> 
    calculateShapeCenter(const pointDistributionModel::shape &shape) const;

    /**
     * calculate the center of a given shape.
     * all points of the shape are multiplied with their reliability factor,
     * then summed, and the results divided by the sum of reliabilities.
     */
    tpoint<float>
    calculateShapeCenter(const pointDistributionModel::shape &shape,
                         const fvector &pointReliability) const;

    /**
     * calculate the mean distance of a given point to the points of a shape
     * @return average distance
     */
    float 
    calculateShapeDistance(const pointDistributionModel::shape &shape,
                           const tpoint<float> &thePoint =
                           tpoint<float>(0.0f, 0.0f)) const;
    
    /**
     * calculate the mean distance of a given point to the
     * reliability-weighted points of a shape
     */
    float calculateShapeDistance(const pointDistributionModel::shape &shape,
                                 const fvector &pointReliability,
                                 const tpoint<float> &thePoint =
                                 tpoint<float>(0.0f, 0.0f)) const;
    
    /**
     * calculate the relative orientation difference of two shapes,
     * with regard to a given rotation center
     * @return angle between referenceShape and shape
     */
    double 
    calculateShapeOrientation(const pointDistributionModel::shape &shape,
                           const pointDistributionModel::shape &referenceShape,
                              const tpoint<float> &theCenter = 
                              tpoint<float>(0.0f, 0.0f)) const;
    
    /**
     * calculate the relative orientation difference of two shapes
     * with regard to a given rotation center and given point reliabilities
     */
    double 
    calculateShapeOrientation(const pointDistributionModel::shape &shape,
                           const pointDistributionModel::shape &referenceShape,
                              const fvector &pointReliability,
                              const tpoint<float> &theCenter =
                              tpoint<float>(0.0, 0.0)) const;
    //@}

  protected:

    /**
     * calculate reliability-weighted scaling factor
     */
    float
    calculateRelativeShapeScale(const pointDistributionModel::shape &shape,
                           const pointDistributionModel::shape &referenceShape,
                                const fvector &pointReliability,
                                const tpoint<float> &shapeCenter =
                                tpoint<float>(0.0, 0.0),
                                const tpoint<float> &referenceShapeCenter =
                                tpoint<float>(0.0, 0.0)) const;

    /**
     * convert shape to weights vector, correct if invalid and convert back to
     * shape space.
     * @param srcdest shape with the source data.  The result will be left
     * here too.
     */
    void trimShape(pointDistributionModel::shape& srcdest) const;

    /**
     * convert reliability-weighted shape to weights vector, correct if
     * invalid and convert back to shape space.
     * @param srcdest shape with the source data.  The result will be left
     * here too.
     * @param pointReliability the reliabilities of each point of the shape as
     * value between 0.0 and 1.0
     */
    void trimShape(pointDistributionModel::shape& srcdest, 
                   const fvector &pointReliability) const;

    /**
     * Correct weights vector according to varianceCoefficient.
     *
     * all vector entries that are not in between -c*v and c*v, where
     * c is the given varianceCoefficient and v is the variance of the
     * corresponding dimension (eigenvector), are set to 0.0
     *
     * @param srcdest vector<double> with the source data.  The result
     *                 will be left here too.
     */
    void trimWeightsVector(vector<double>& srcdest) const;

    /*
     * calculate normal direction of a point between two line segments
     * n = normalized( normalized(v1) - normalized(v2) );
     */
    tpoint<float> calculateNormal(const tpoint<float> &seg1,
                                  const tpoint<float> &seg2) const;

    /**
     * calculate angle between a point and the x-axis
     */
    double calculateAngle(const tpoint<float> &thePoint) const;

    /**
     * convert the shape [(x1,y1),...] to a vector [x1, y1, x2, y2, ...]
     */
    vector<double> buildVector(const pointDistributionModel::shape &src) const;

    /**
     * convert a vector [x1, y1, x2, y2, ...] into a shape [(x1,y1),...]
     */
    pointDistributionModel::shape buildShape(const vector<double> &src) const;

    /*
     * return normalized vector
     */
    tpoint<float> getNormalizedVector(const tpoint<float> &vec) const;

    /**
     * create normals for each point
     */
    void createListOfNormals(const pointDistributionModel::shape &theShape,
                             tpointList<float> &listOfNormals) const;

  };
}

#endif

