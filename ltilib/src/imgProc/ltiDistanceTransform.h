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
 * file .......: ltiDistanceTransform.h
 * authors ....: Pablo Alvarado, Markus Radermacher
 * organization: LTI, RWTH Aachen
 * creation ...: 22.8.2001
 * revisions ..: $Id: ltiDistanceTransform.h,v 1.12 2005/06/16 09:32:42 doerfler Exp $
 */

#ifndef _LTI_DISTANCE_TRANSFORM_H_
#define _LTI_DISTANCE_TRANSFORM_H_

#include "ltiImage.h"
#include "ltiMorphology.h"

namespace lti {

  /**
   * This simple morphological operator assumes that the input data is
   * a binary image, i.e. there are values 0 and not 0.  It computes the
   * minimal distance of a pixel with not-zero value to a zero pixel.
   *
   * You can choose in the parameters between a 4-Neighborhood or an
   * 8-Neighborhood for the computations.  It is also possible to
   * compute the euclidean distance transform.
   *
   * For two pixels p and q with position p(p.x,p.y) and
   * q(q.x,q.y) following is valid:
   * - the distance in a 4-neighborhood is |p.x-q.x| + |p.y-q.y|
   * - the distance in an 8-neighborhood is max(|p.x-q.x|,|p.y-q.y|)
   *
   * The computation for the 4- and 8-neighborhood based distance
   * transform is very efficient and traverses the input channel just
   * twice: once from top to bottom and once on the opposite
   * direction.  To compute the euclidean distance transform the
   * algorithm described in
   *
   * Calvin R. Maurer Jr., Rensheng Qi, Vijay V. Raghavan: 
   * "A Linear Time Algorithm for Computing Exact Euclidean Distance Transforms
   *  of Binary Images in Arbitrary Dimensions". 
   * IEEE Transactions on Pattern Analysis and Machine Intelligence,
   * Vol.25, No. 2, 2003, pp. 265-270
   *
   * is used.
   *
   * @ingroup gMorphology
   */
  class distanceTransform : public morphology {
  public:
    /**
     * the parameters for the class distanceTransform
     */
    class parameters : public morphology::parameters {
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
       * Four distancetransformation computation types can be choosen:  
       * EightNeighbor, FourNeighbor, EuclideanSqr and Euclidean.
       *
       * For two pixels p and q with position p(p.x,p.y) and
       * q(q.x,q.y) following is valid:
       * - the distance in a 4-neighborhood is |p.x-q.x| + |p.y-q.y|
       * - the distance in an 8-neighborhood is max(|p.x-q.x|,|p.y-q.y|)
       * - the distance is euclidean square for (p.x - q.x)^2 + (p.y - q.y)^2 
       * - the distance is euclidean for ( (p.x - q.x)^2 + (p.y - q.y)^2 )^1/2
       */
      enum eDistanceType{
        EightNeighborhood, /**< eight neighborhood distance */
        FourNeighborhood,  /**< four neighborhood (city block) distance */
        EuclideanSqr,      /**< square of euclidean distance */
        Euclidean,         /**< euclidean distance */
        EightSED,          /**< eight point sequential euclidian
                            *   distance mapping 
                            */
        EightSEDSqr,       /**< square of the eight point sequential
                            *   euclidian distance mapping 
                            */
        FourSED,           /**< four point sequential euclidian
                            *   distance mapping 
                            */
        FourSEDSqr         /**< square of the four point sequential
                            *   euclidian distance mapping 
                            */
      };

      /**
       * Kind of distance transform to be computed.  See eDistanceType
       * for more information.
       *
       * Default value: Euclidean
       */
      eDistanceType distance;
    };

    /**
     * default constructor
     */
    distanceTransform();

    /**
     * default constructor
     */
    distanceTransform(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    distanceTransform(const distanceTransform& other);

    /**
     * destructor
     */
    virtual ~distanceTransform();

    /**
     * returns the name of this type ("distanceTransform")
     */
    virtual const char* getTypeName() const;

    /**
     * Compute the distance transform of the srcdest channel, i.e. the
     * minimal distance from a non-background pixel to a background pixel.
     * It will be assumed, that all background pixels have the value 0.0f.
     * (any non zero value will be taken as non-background pixel).
     *
     * The result in the channel will contain for each pixel the
     * distance value.  This means, the usual value range from 0.0f to 1.0f 
     * is ignored in this functor, providing the distances in "pixel" units.
     * 
     * @param srcdest channel with the source data.  The result
     *                 will be left here too.
     * @return true if successful, false otherwise.
     */
    virtual bool apply(channel& srcdest) const;

    /**
     * Compute the distance transform of the srcdest channel, i.e. the
     * minimal distance from a non-background pixel to a background pixel.
     * It will be assumed, that all background pixels have the value zero.
     * (any non zero value will be taken as non-background pixel).
     *
     * If the size of \a srcdest and form of its non-background
     * regions allow distances greater than 255 (the maximal possible
     * representable value with a ubyte), the content of the distance
     * transform will be invalid.
     * 
     * @param srcdest channel8 with the source data.  The result
     *                 will be left here too.
     * @return true if successful, false otherwise.
     */
    virtual bool apply(channel8& srcdest) const;

    /**
     * Compute the distance transform of the \a src channel and leave
     * the result in \a dest, i.e. calculate the minimal distance from
     * a non-background pixel to a background pixel.  It will be
     * assumed, that all background pixels have the value zero.  (any
     * non zero value will be taken as non-background pixel).
     *
     * The result in the channel will contain for each pixel the
     * distance value.  This means, the usual value range from 0.0f to 1.0f 
     * is ignored in this functor, providing the distances in "pixel" units.
     * 
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @return true if successful, false otherwise.
     */
    virtual bool apply(const channel& src,channel& dest) const;

    /**
     * Compute the distance transform of the \a src channel and leave
     * the result in \a dest, i.e. calculate the minimal distance from
     * a non-background pixel to a background pixel.  It will be
     * assumed, that all background pixels have the value zero.  (any
     * non zero value will be taken as non-background pixel).
     *
     * If the size of \a srcdest and form of its non-background
     * regions allow distances greater than 255 (the maximal possible
     * representable value with a ubyte), the content of the distance
     * transform will be invalid.
     * 
     * @param src channel8 with the source data.
     * @param dest channel8 where the result will be left.
     * @return true if successful, false otherwise.
     */
    virtual bool apply(const channel8& src,channel8& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    distanceTransform& copy(const distanceTransform& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    distanceTransform& operator=(const distanceTransform& other);

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
     * iteration for distance computation using an 8-Neighborhood
     */
    void iteration8(channel& chnl) const;

    /**
     * iteration for distance computation using an 4-Neighborhood
     */
    void iteration4(channel& chnl) const;

    /**
     * backwards iteration for distance computation using an 8-Neighborhood
     */
    void iteration8back(channel& chnl) const;

    /**
     * backwards iteration for distance computation using an 4-Neighborhood
     */
    void iteration4back(channel& chnl) const;
    
    /**
     * Method to compute ED for each column of the precomputed
     * distance image chnl. The chnl must be the output of a previos
     * call to EDT_1D.  Method uses the distance propagation of the
     * function voronoiEDT_2D.
     */
    inline void EDT_2D(channel& chnl) const;
        
    /**
     * queries some distance information for the EDT
     */
    inline bool removeEDT(const int du,
                          const int dv, 
                          const int dw,
                          const int u,
                          const int v,
                          const int w) const;
        
    /**
     * fast linear order computation of the accurade euclidean
     * distance with voronoi diagrams
     */
    void voronoiEDT_2D(channel& chnl, const int dim) const;
 
    /**
     * Method computes ED (euclidean distance) for each row pixel of
     * the given channel to the closest background pixel. It assumes
     * that a background pixel is set to 0.0f and a foreground pixel is
     * > 0.0f. All rows without any background pixel will be set to an
     * undefined distance (i.e. < 0.0f). The ED is computed with a fast
     * and easy forward-backward distance propagation.
     */
    void EDT_1D(channel& chnl) const;

    /**
     * Method computes ED (euclidean distance) for the given channel
     * with the 8SED or 4SED (8 or 4 point sequential euclidian
     * distance mapping) method.
     */
    void sedFiltering(channel &chnl, bool useEightSED)const;

    /**
     * Calculates the 4SED distance transform.
     */
    void fourSEDFiltering(channel &chnl, matrix<point> &dist)const;


    /**
     * Calculates the 8SED distance transform.
     */
    void eightSEDFiltering(channel &chnl, matrix<point> &dist)const;

  private:

    /**
     * Nested class for the SED_filtereing Method.
     */
    class sedMask;
  };

  
  class distanceTransform::sedMask{
  public:
    
    /**
     * inline Constructor
     * @param mask a list of points of the mask positions
     * @param size the number of points mask contains
     */
    sedMask(const point mask[], int size);

    /**
     *inline Destructor
     */
    ~sedMask();
  
    /**
     * filters the positoins in dist with the internal mask
     * @param pos the position in dist of the filter process
     */
    void filter(matrix<point> &dist, const point &pos) const;

  private:
  
    /**
     * After called shortest contains the value of the shorter
     * (euclidean) of the two given points. If any point contain
     * negative values its lenght is not calculated.
     */
    inline void query_distance(point &shortest, point &other) const;

    /**
     * List to points of the filtermask.
     *
     * Just a reference to extern data
     */
    const point *const mask_;

    /**
     * Number of point stored in mask
     */
    int size_;
  };

} //namespace lti

#endif

