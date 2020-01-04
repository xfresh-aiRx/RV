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
 * file .......: ltiMultiGeometricFeaturesFromMask.h
 * authors ....: Claudia Goenner
 * organization: LTI, RWTH Aachen
 * creation ...: 31.8.2003
 */

#ifndef _LTI_MULTI_GEOMETRIC_FEATURES_FROM_MASK_H_
#define _LTI_MULTI_GEOMETRIC_FEATURES_FROM_MASK_H_

#include "ltiGeometricFeaturesFromMask.h"
#include "ltiGenericVector.h"

namespace lti {

  /**
   * This functor takes a mask (labeled or not), where it is assumed that
   * a region must be connected or not and reassigns labels to them in
   * a way that each connected region gets its own label.
   *
   * Additionally the minimum bounding rectangle or geometric features of the
   * labeled objects are computed and stored in the vector corresponding to the
   * object's kind. Thereby it is assumed that each object kind is
   * assigned the same grey value in the input channel. All objects of the same
   * kind are stored in a single vector. These vectors again are returned in a
   * vector and ordered in the same way as in the parameter mask Values, which
   * specifies the grey values for which the geometric features are computed.
   * Any other grey values are still labeled as described in fastRelabeling, 
   * but excluded from the further processing performed by this class.
   *
   * Close objects are merged if requested. To ensure fast merging two objects
   * are close if their surrounding rectangles are close.
   *
   * CAVEAT: 
   * 1) Sorting of the merged objects by their size does not work if the
   *    circumscribing rectangles are returned only.
   */
  class multiGeometricFeaturesFromMask : public geometricFeaturesFromMask {
  public:
    /**
     * the parameters for the class multiGeometricFeaturesFromMask
     */
    class parameters : public geometricFeaturesFromMask::parameters {
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
       * Each object kind may be merged (true) or not (false). The inherited
       * parameter merge is ignored. The vectors mergeEntries and maskValues
       * must be of the same size.
       *
       * Default: empty
       */
      genericVector<bool> mergeEntries;

      /**
       * Specifies which mask values will be processed by this class. 
       * The vectors mergeEntries and maskValues must be of the same size.
       *
       * Default: empty
       */
      ivector maskValues;

      /**
       * Sometimes it is advantageous to perform the merge operation separately
       * from the apply, i.e. when labeling several roi's one after the other.
       * If manualPostProcess is true the apply won't compute the complete
       * result, but only perform the labeling and initialization of the
       * geometric feature vectors. Thus some geometric features are not
       * consistent after the apply in the manual case,
       * e.g. the center of gravity is not divided by the area yet.
       *
       * Default: false
       */
      bool manualPostProcess;

    };

    /**
     * default constructor
     */
    multiGeometricFeaturesFromMask();

    /**
     * Construct a functor using the given parameters
     */
    multiGeometricFeaturesFromMask(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    multiGeometricFeaturesFromMask(const multiGeometricFeaturesFromMask& other);

    /**
     * destructor
     */
    virtual ~multiGeometricFeaturesFromMask();

    /**
     * returns the name of this type ("multiGeometricFeaturesFromMask")
     */
    virtual const char* getTypeName() const;

    /**
     * Update the parameters content.
     * return true on success and false otherwise
     */
    bool updateParameters();

    /**
     * apply-method to get the bounding boxes of the objects inside the image.
     * operates on the given %parameter.
     * @param src     channel8 with the source data
     * @param objects a vector with the smallest rectangles containing each
     *                labeled object
     * @return true if apply successful or false otherwise.
     */    
    bool apply(const channel8& src, 
               std::vector<std::vector<rectangle> >& objects) const;
 
    /**
     * apply-method to get simple geometric features (bounding box, area,
     * center of gravity) of the objects inside the image. Additionally
     * to the bounding boxes a matrix with all the labels is returned, where
     * the label subtracted by one corresponds to the index into the vector
     * with the geometric features.
     * operates on the given %parameter.
     * @param src     channel8 with the source data
     * @param dest    a matrix with the labeled data
     * @param objects a vector with the geometric features of each labeled
     *                object
     * @return true if apply successful or false otherwise.
     */  
    bool apply(const channel8 &src, imatrix& dest,
	       std::vector<std::vector<rectangle> >& objects) const;

    /**
     * apply-method to get simple geometric features (bounding box, area,
     * center of gravity) of the objects inside the image.
     * operates on the given %parameter.
     * @param src     channel8 with the source data
     * @param objects a vector with the geometric features of each labeled object
     * @return true if apply successful or false otherwise.
     */  
    bool apply(const channel8 &src,
	       std::vector<std::vector<geometricFeatureGroup0> >& objects) const;
  
    /**
     * apply-method to get simple geometric features (bounding box, area,
     * center of gravity) of the objects inside the image. Additionally
     * to the bounding boxes a matrix with all the labels is returned, where
     * the label subtracted by one corresponds to the index into the vector
     * with the geometric features.
     * operates on the given %parameter.
     * @param src     channel8 with the source data
     * @param dest    matrix with the labeled data
     * @param objects a vector with the geometric features of each labeled
     *                object
     * @return true if apply successful or false otherwise.
     */  
    bool apply
	(const channel8 &src, imatrix& dest,
	 std::vector<std::vector<geometricFeatureGroup0> >&  objects) const;

    /**
     * Merge if requested and compute the center of gravity. This method need
     * only be called in the manual post processing mode.
     * @param objects the geometric features to be processed
     * @return true on success and false otherwise
     */
    bool postProcess
	(std::vector< std::vector<geometricFeatureGroup0> >& objects) const;

    /**
     * Merge if requested. This method need only be
     * called in the manual post processing mode.
     * @param objects the rectangles to be processed	
     * @return true on success and false otherwise
     */
    bool postProcess(std::vector< std::vector<rectangle> >& objects) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    multiGeometricFeaturesFromMask& copy
	(const multiGeometricFeaturesFromMask& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    multiGeometricFeaturesFromMask& operator=
	(const multiGeometricFeaturesFromMask& other);

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

