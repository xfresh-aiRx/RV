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
 * file .......: ltiGeometricFeaturesFromMask.h
 * authors ....: Dorothee Finck
 * organization: LTI, RWTH Aachen
 * creation ...: 11.6.2003
 * revisions ..: $Id: ltiGeometricFeaturesFromMask.h,v 1.11 2005/06/16 12:35:10 doerfler Exp $
 */

#ifndef _LTI_GEOMETRIC_FEATURES_FROM_MASK_H_
#define _LTI_GEOMETRIC_FEATURES_FROM_MASK_H_

#include "ltiFastRelabeling.h"
#include "ltiGeometricFeatureGroup.h"

namespace lti {

  /**
   * This functor takes a mask (labeled or not), where it is assumed that
   * a region must be connected or not and reassigned labels to them in
   * a way that each connected region gets its own label.
   *
   * Additionally the minimum bounding rectangle or geometric features of the
   * labeled objects are computed. Close objects are merged if requested. To
   * ensure fast merging two objects are close if their surrounding rectangles
   * are close.
   * CAVEAT: 
   * 1) Sorting of the merged objects by their size does not work if the circumscribing 
   *    rectangles are returned only.
   */
  class geometricFeaturesFromMask : public fastRelabeling {
  public:

    /**
     * the parameters for the class geometricFeaturesFromMask
     */
    class parameters : public fastRelabeling::parameters {
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
       * If this value is greater than 0 and sortSize is true, only the n biggest
       * objects will be processed and all other objects are suppressed.
       *
       * Default: 0
       */
      int nBest;

      /**
       * If the number of pixels of an object after merging
       * is less than the minimumMergedObjectSize, it will be
       * assigned to the background label (0). 
       * The inherited parameter minimumObjectSize applies to objects
       * before merging.
       *
       * Default value: 1 (i.e. consider all objects)
       */
      int minimumMergedObjectSize;

      /**
       * The minimum distance in x and y direction between labeled regions
       * that belong to the same object. The labeled regions are approximated
       * by rectangles and thus the distances are computed between those
       * rectangles along the x and y axis.
       *
       * Default: (0,0) (i.e. rectangles must overlap)
       */
      point minimumDistance;

      /**
       * The objects may be merged if close (true) or not (false).
       *
       * Default: false
       */
      bool merge;
    };

    /**
     * default constructor
     */
    geometricFeaturesFromMask();

    /**
     * Construct a functor using the given parameters
     */
    geometricFeaturesFromMask(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    geometricFeaturesFromMask(const geometricFeaturesFromMask& other);

    /**
     * destructor
     */
    virtual ~geometricFeaturesFromMask();

    /**
     * returns the name of this type ("geometricFeaturesFromMask")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    geometricFeaturesFromMask& copy(const geometricFeaturesFromMask& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    geometricFeaturesFromMask& operator=(const geometricFeaturesFromMask& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * apply-method to get the bounding boxes of the objects inside the image.
     * operates on the given %parameter.
     * @param src channel8 with the source data.
     * @param objects a vector with the smallest rectangles containing
     *                each labeled object
     * @return true if apply successful or false otherwise.
     */    
    bool apply(const channel8& src, std::vector<rectangle>& objects) const;
 

    /**
     * apply-method to get the bounding boxes of the objects inside
     * the image. Additionally to the bounding boxes a matrix with all
     * the labels is returned, where the label subtracted by one
     * corresponds to the index into the vector with the bounding
     * boxes.
     *
     * operates on the given %parameter.
     * @param src channel8 with the source data.
     * @param dest imatrix with the labeled objects.
     * @param objects a vector with the smallest rectangles containing
     *                each labeled object
     * @return true if apply successful or false otherwise.
     */    
    bool apply(const channel8& src, imatrix& dest,
               std::vector<rectangle>& objects) const;
    
    /**
     * apply-method to get simple geometric features (bounding box, area,
     * center of gravity) of the objects inside the image.
     * operates on the given %parameter.
     * @param src channel8 with the source data.
     * @param objects a vector with the geometric features of each
     *                labeled object
     * @return true if apply successful or false otherwise.
     */  
    bool apply(const channel8 &src, 
               std::vector<geometricFeatureGroup0>& objects) const;
    
    /**
     * apply-method to get simple geometric features (bounding box,
     * area, center of gravity) of the objects inside the
     * image. Additionally to the bounding boxes a matrix with all the
     * labels is returned, where the label subtracted by one
     * corresponds to the index of vector with the geometric features.
     * @param src channel8 with the source data.
     * @param dest imatrix with the labeled objects.
     * @param objects a vector with the geometric features of each
     *                labeled object
     * @return true if apply successful or false otherwise.
     */  
    bool apply(const channel8 &src, imatrix& dest,
	       std::vector<geometricFeatureGroup0>& objects) const;

  protected:
    /**
     * computes the vector of equivalent labels, i.e. suppresses
     * objects smaller than the minimum size and those which are not
     * among the n-best. Furthermore this suppression is more
     * efficient than using the parent class' methods because the
     * labeled mask is not updated yet.
     * @param src channel8 with the source data
     * @param dest matrix with the labels
     * @param objSize vector with the object sizes
     * @param equivLabels vector with equivalent labels
     * @param numObjects contains the number of detected objects
     * @return true on success or false otherwise.
     */

    bool computeEquivLabels(const channel8& src, imatrix& dest,
			    ivector& objSize, ivector& equivLabels,
			    int& numObjects) const;

    /**
     * initializes the vector of bounding rectangles with the
     * rectangle containing the entire image.
     * @param numLab the number of labels
     * @param size the size of the input channel
     * @param objects the vector to be initializes
     * @return true on success or false otherwise.
     */
    virtual bool initRectangleVector(const int numLab, const point& size,
				     std::vector<rectangle>& objects) const;

    /**
     * initializes the vector of geometric features from group 0
     * @param numLab the number of labels
     * @param size the size of the input channel
     * @param objects the vector to be initializes
     * @return true on success or false otherwise.
     */
    virtual bool 
      initGeomFeature0Vector(const int numLab, const point& size,
			     std::vector<geometricFeatureGroup0>& objects)
      const;
    
    /**
     * merge neighboring objects
     * @param src the vector with the rectangles to be merged
     * @return true on success or false otherwise.
     */
    bool merge(std::vector<rectangle>& src) const;
    
    /**
     * merge neighboring objects
     * @param src the vector with the geometric features to be merged
     * @return true on success or false otherwise.
     */
    bool merge(std::vector<geometricFeatureGroup0>& src) const;

    /** 
     * sort the merged objects
     * @param objects the vector with the geometric features to be sorted
     * @return true on success or false otherwise.
     */
    bool sortMerged(std::vector<geometricFeatureGroup0>& objects) const;
  };

}


#endif

