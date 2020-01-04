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
 * file .......: ltiFastRelabeling.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 16.5.2003
 * revisions ..: $Id: ltiFastRelabeling.h,v 1.9 2005/06/16 12:35:10 doerfler Exp $
 */

#ifndef _LTI_FAST_RELABELING_H_
#define _LTI_FAST_RELABELING_H_

#include "ltiModifier.h"
#include "ltiContour.h"

#include <vector>

namespace lti {
  /**
   * This functor takes a mask (labeled or not), where it is assumed that
   * a region must be connected or not and reassignes labels to them in
   * a way that each connected region gets its own label.
   *
   * Derived classes can compute for each of the found object additional
   * information like geometric features, bounding boxes, weighted
   * probabilities, etc.
   */
  class fastRelabeling : public modifier {
  public:
    /**
     * the parameters for the class fastRelabeling
     */
    class parameters : public modifier::parameters {
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
       * @name General Options
       */
      //@{

      /**
       * All values in the original mask above or equal this minimum threshold
       * will be considered as candidates to relabeling.
       *
       * Default value: 1
       */
      int minThreshold;

      /**
       * All values in the original mask below or equal this maximum threshold
       * will be considered as candidates to relabeling.
       *
       * Default value: std::numeric_limits<int>::max()
       */
      int maxThreshold;

      /**
       * If true, each integer value in the original image will be considered
       * as a new region label.  If false, only two "values" will be taken:
       * within the threshold interval or outside it.
       *
       * Default value: true
       */
      bool assumeLabeledMask;

      /**
       * If true, a 4-neighborhood will be used to detect the adjacent pixels,
       * otherwise an 8-neighborhood will be used.
       *
       * Due to the lesser number of required comparisons, the 4-neighborhood
       * allows a faster relabeling.  Sometimes however the 8-neighborhood
       * is required.
       *
       * Default value: true (i.e. 4-neighborhood)
       */
      bool fourNeighborhood;
      //@}

      /**
       * @name Filter objects
       */
      //@{
      /**
       * If sortSize is true, the object labels will be assigned according to
       * the number of pixels of each object.
       *
       * \warning There is an exception.  The label 0 has a special meaning
       * and it will always be assigned to the background (everything outside
       * the given interval between minThreshold and maxThreshold)
       * independently of its size.  This means the regions will be sorted
       * decreasingly but begining with the second element.  The first one is
       * always the background.  If you set minThreshold to zero, the first
       * element is always empty.
       *
       * Defalut value: false
       */
      bool sortSize;

      /**
       * If the number of pixels of an object is less than the
       * minimumObjectSize, it will be assigned to the background label (0).
       *
       * Default value: 1 (i.e. consider all objects)
       */
      int minimumObjectSize;
      //@}
      
    };

    /**
     * default constructor
     */
    fastRelabeling();

    /**
     * Construct a functor using the given parameters
     */
    fastRelabeling(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    fastRelabeling(const fastRelabeling& other);

    /**
     * destructor
     */
    virtual ~fastRelabeling();

    /**
     * returns the name of this type ("fastRelabeling")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on the given %parameter.
     * @param srcdest channel8 with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(channel8& srcdest) const;

    /**
     * operates on the given %parameter.
     * @param srcdest imatrix with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(imatrix& srcdest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src channel8 with the source data.
     * @param dest channel8 where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& src,channel8& dest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src channel8 with the source data.
     * @param dest channel8 where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& src,imatrix& dest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src imatrix with the source data.
     * @param dest imatrix where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const imatrix& src,imatrix& dest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src channel8 with the source data.
     * @param dest channel8 where the result will be left.
     * @param numPixels number of pixel per new object label
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& src,channel8& dest,ivector& numPixels) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src channel8 with the source data.
     * @param dest channel8 where the result will be left.
     * @param numPixels number of pixel per new object label
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& src,imatrix& dest,ivector& numPixels) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src imatrix with the source data.
     * @param dest imatrix where the result will be left.
     * @param numPixels number of pixel per new object label
     * @return true if apply successful or false otherwise.
     */
    bool apply(const imatrix& src,imatrix& dest,ivector& numPixels) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src channel8 with the source data.
     * @param dest channel8 where the result will be left.
     * @param numPixels number of pixel per new object label
     * @param objects for each label o, objects[o] contains all points 
     *                belonging to that object.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& src,channel8& dest,
               ivector& numPixels,std::vector<areaPoints>& objects) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src channel8 with the source data.
     * @param dest channel8 where the result will be left.
     * @param numPixels number of pixel per new object label
     * @param objects for each label o, objects[o] contains all points 
     *                belonging to that object.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& src,imatrix& dest,ivector& numPixels,
               std::vector<areaPoints>& objects) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src imatrix with the source data.
     * @param dest imatrix where the result will be left.
     * @param numPixels number of pixel per new object label
     * @param objects for each label o, objects[o] contains all points 
     *                belonging to that object.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const imatrix& src,imatrix& dest,ivector& numPixels,
               std::vector<areaPoints>& objects) const;

    /**
     * This apply method does not complete the relabeling process.  Instead of
     * that it returns an incomplete labeled mask and the label equivalences
     * vector, with which other functors can efficiently compute other
     * information about the found objects.
     *
     * @param src original labeled or unlabeled mask.
     * @param partMask partial labeled mask.
     * @param equivLabels vector with the equivalence labels
     * @param numObjects number of objects found in the src mask.
     * @return true if successful, false otherwise.
     *
     * The simplest way to get a consistend finished mask (which is directly
     * provided in other apply methods) is to assign to each pixel of partMask
     * the corresponding equivalent label, i.e. for each x and y, 
     * partMask.at(y,x) = equivLabels(partMask.at(y,x)).
     */
    bool apply(const channel8& src,
                     channel8& partMask,
                     ivector& equivLabels,
                     int& numObjects) const;

    /**
     * This apply method does not complete the relabeling process.  Instead of
     * that it returns an incomplete labeled mask and the label equivalences
     * vector, with which other functors can efficiently compute other
     * information about the found objects.
     *
     * @param src original labeled or unlabeled mask.
     * @param partMask partial labeled mask.
     * @param equivLabels vector with the equivalence labels
     * @param numObjects number of objects found in the src mask.
     * @return true if successful, false otherwise.
     *
     * The simplest way to get a consistend finished mask (which is directly
     * provided in other apply methods) is to assign to each pixel of partMask
     * the corresponding equivalent label, i.e. for each x and y, 
     * partMask.at(y,x) = equivLabels(partMask.at(y,x)).
     */
    bool apply(const channel8& src,
                     imatrix& partMask,
                     ivector& equivLabels,
                     int& numObjects) const;

    /**
     * This apply method does not complete the relabeling process.  Instead of
     * that it returns an incomplete labeled mask and the label equivalences
     * vector, with which other functors can efficiently compute other
     * information about the found objects.
     *
     * @param src original labeled or unlabeled mask.
     * @param partMask partial labeled mask.
     * @param equivLabels vector with the equivalence labels
     * @param numObjects number of objects found in the src mask.
     * @return true if successful, false otherwise.
     *
     * The simplest way to get a consistend finished mask (which is directly
     * provided in other apply methods) is to assign to each pixel of partMask
     * the corresponding equivalent label, i.e. for each x and y, 
     * partMask.at(y,x) = equivLabels(partMask.at(y,x)).
     */
    bool apply(const imatrix& src,
                     imatrix& partMask,
                     ivector& equivLabels,
                     int& numObjects) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    fastRelabeling& copy(const fastRelabeling& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    fastRelabeling& operator=(const fastRelabeling& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  protected:

    /**
     * Sort labels and eliminate those objects with sizes smaller than the
     * given threshold.
     *
     * @param minSize minimum number of pixels an object must have.
     * @param numPixels number of pixels per object.  The index of the vector
     *                  corresponds to the label in the given mask.
     *                  After calling this vector is also sorted.
     * @param reindex new indices
     */
    bool sortLabels(const int minSize, 
                          ivector& numPixels, 
                          ivector& reindex) const;

    /**
     * Just relabel the destination, but compute also the size of each object.
     */
    bool relabelWithArea(const ivector& equivLabels,
			 const int numObjects,
			 imatrix& dest,
			 ivector& numPixels) const;

    /**
     * Just relabel the destination, but compute also the size of each object.
     */
    bool relabelWithArea(const vector<ubyte>& equivLabels,
			 const int numObjects,
			 channel8& dest,
			 ivector& numPixels) const;

  };
}

#endif

