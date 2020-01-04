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
 * file .......: ltiClusteringValidity.h
 * authors ....: Jens Paustenbach
 * organization: LTI, RWTH Aachen
 * creation ...: 4.4.2002
 * revisions ..: $Id: ltiClusteringValidity.h,v 1.8 2005/11/24 15:01:45 doerfler Exp $
 */

#ifndef _LTI_CLUSTERING_VALIDITY_H_
#define _LTI_CLUSTERING_VALIDITY_H_

#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiL2Distance.h"
#include "ltiFunctor.h"
#include <list>

namespace lti {
  /**
   *  Parent class for all clustering validity measures.
   *  Clustering validity measures are used to evaluate the quality of a 
   *  clustering. The measure can e.a. be used to find the best possible
   *  number of clusters in a data set.
   *  It provides some distance and diameter measure for clusters.
   *  These measures are descriped in IEEE Transaction on systems,man and
   *  cybernetics - part B: cybernetics, Vol 28, No.3, June 1998, 301-315
   */
  // ------------------------------------------------------------------
  // clustering Validity
  // ------------------------------------------------------------------
  class clusteringValidity : public functor {
  public:
    /**
     * default constructor
     */
    clusteringValidity();

    /**
     * default constructor
     */
    clusteringValidity(const clusteringValidity& other);

    /**
     * destructor
     */
    virtual ~clusteringValidity();

    /**
     * returns the name of this type ("clusteringValidity")
     */
    virtual const char* getTypeName() const;

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    clusteringValidity& operator=(const clusteringValidity& other);

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    clusteringValidity& copy(const clusteringValidity& other);
    
    /**
     * abstract parant class
     * operates on the given %parameter.
     * @param clusteredData std::vector<dmatrix> with the source data.
     * @param index the clustering validity result
     * @param centroids dmatrix with each row representing a centroid
     *        of the corresponding distribution in clusteredData
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const std::vector<dmatrix>& clusteredData, 
                       double& index, const dmatrix& centroids) const =0;

  public: //AMei protected
    /**
     * calculates the minimum distances of the points in the given matrices
     * and returns it.
     */
    double getMinimumDistance(const dmatrix& m1,const dmatrix& m2) const;

    /**
     * calculates the maximum distance of all the points in the given
     * matrices and returns this distance
     */
    double getMaximumDistance(const dmatrix& m1,const dmatrix& m2) const;

    /** 
     * return the distance of the centroids of m1 and m2
     */
    double getCentroidDistance(const dmatrix& m1,const dmatrix& m2) const;

    /** 
     * return the average distance of all the point in m1 and m2
     */
    double getAverageDistance(const dmatrix& m1,const dmatrix& m2) const;

    /** 
     * accumulates the the distances of all points each matrix to matrix
     * and divides the sum of these distance through the sum of all points
     */
    double getAverageInterpointDistance(const dmatrix& m1,
                                        const dmatrix& m2) const ;

    /**
     * return the maximum distance of all points in m1
     */
    double getStandardDiameter(const dmatrix& m1) const;

    /** 
     * the average distance of all points in m1
     */
    double getAverageDiameter(const dmatrix& m1) const;

    /** 
     * average distance between each point and the centroid of m1
     */
    double getAverageToCentroidDiameter(const dmatrix& m1) const;
      

  };

  ////////////////////////////////////////////////////////////////////////
  //////////////////////// D U N N   I N D E X ///////////////////////////
  ////////////////////////////////////////////////////////////////////////
  /** 
   * computes the Dunn Index and its generalisations described in 
   * Bezdek, J.C.,Pal,N.R., 1998. "Some new indexes of cluster validity."
   * IEEE Transaction on Systems,Man, and cybernetics (Part B) 28, 301-315
   * the distance and diameter functions are implemented in the parent class
   * so they can be used for other computations of validity indexes.
   * <p>To find the best possible clustering look for the maximum value of
   * this indices
   * <p>Valid values for the measure of the diameters are:
   * standard: the largest distance between to points in a cluster
   * average: the average distance between all points in the cluster
   * centroid: twice the average distance between each point and the centroid
   * <p> valid values for the distance measure are
   * minimum: the minimal distance between the points in the two clusters
   * maximum: the maximum distance between the points in the two clusters
   * mean: the average distance between all points in the clusters
   * centroids: the distance between the centroids of the clusters
   * interpoint: combination of mean and centroids
   */
  class dunnIndex : public clusteringValidity {
  public:
      
    class parameters : public clusteringValidity::parameters {
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
       * be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;
            
      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *      be also written, otherwise only the data block will be written.
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
       * Diameter Measure Types
       */
      enum eDiameterMeasure {
        Standard,   /* use standard diameter */
        Average,    /* use average distance of all points as diameter */
        Centroid    /* use the average distance of each point to 
                       the centroid */
      };

      /**
       * Distance Measure Types
       */
      enum eDistanceMeasure {
        Minimum=0,  /* use minimum distance between clusters */
        Maximum,    /* use maximum distance between clusters */
        Mean,       /* use average distance between clusters */
        Centroids,  /* use distance of the centroids */
        Interpoint  /* use average of all interpoint distances */
      };

      /**
       * Diameter measure
       *
       * Default value: Standard
       */
      eDiameterMeasure diameterMeasure;

      /**
       * Distance measure
       *
       * Default value: Minimum
       */
      eDistanceMeasure distanceMeasure;
    };

    // ----------------------------------------------------------
    // Dunn Index 
    // ----------------------------------------------------------

    /**
     * standard contructor
     */
    dunnIndex();

    /**
     * standard contructor
     */
    dunnIndex(const dunnIndex& other);

    /**
     * destructor
     */
    virtual ~dunnIndex();

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const;

    /**
     * calculates the Dunn Index of the clustered Data
     * @param clusteredData std::vector<dmatrix> with the source data.
     * @param index the clustering validity result
     * @param centroids dmatrix with each row representing a centroid
     *        of the corresponding distribution in clusteredData
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const std::vector<dmatrix>& clusteredData,
                       double& index, const dmatrix& centroids) const;
    
    
    const parameters& getParameters() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    dunnIndex& copy(const dunnIndex& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    dunnIndex& operator=(const dunnIndex& other);
    
  };

  ////////////////////////////////////////////////////////////////////////
  /////////////// Modified Hubert Statistic //////////////////////////////
  ////////////////////////////////////////////////////////////////////////
  /** 
   * Calculates the modified Hubert statistic of the given clustering.
   *
   * When the same data set is tested with different parameters, e.a. with
   * different number of clusters, the best possible clustering is shown 
   * by a sharp knee in this index.
   * 
   * <em> This validity measure is not defined for one cluster or if
   * the number of clusters is equal the the number of points </em>
   */
  class modHubertStat : public clusteringValidity {
  public:

    /**
     * standard contructor
     */
    modHubertStat();

    /**
     * standard contructor
     */
    modHubertStat(const modHubertStat& other);

    /**
     * destructor
     */
    virtual ~modHubertStat();

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    modHubertStat& copy(const modHubertStat& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    modHubertStat& operator=(const modHubertStat& other);

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const;

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * calculates the Dunn Index of the clustered Data
     * @param clusteredData std::vector<dmatrix> with the source data.
     * @param index the clustering validity result
     * @param centroids dmatrix with each row representing a centroid
     *        of the corresponding distribution in clusteredData
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const std::vector<dmatrix>& clusteredData,
                       double& index, const dmatrix& centroids) const;
  };

  ////////////////////////////////////////////////////////////////////////
  ////////// normalized Modified Hubert Statistic ////////////////////////
  ////////////////////////////////////////////////////////////////////////
  /** 
   * Calculates the normalized version of the modified Hubert statistic 
   * The index is between -1 and 1. A value near 1 shows compact well 
   * separated clusters.
   * When the same data set is tested with different parameters, e.a. with
   * different number of clusters, the best possible clustering is shown 
   * by a sharp knee in this index.
   * <em> This validity measure is not defined, if the number of cluster is
   * 1 or equal to the number of points </em>
   */
  class normModHubertStat : public clusteringValidity {
  public:

    /**
     * standard contructor
     */
    normModHubertStat();

    /**
     * standard contructor
     */
    normModHubertStat(const normModHubertStat& other);

    /**
     * destructor
     */
    virtual ~normModHubertStat();

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    normModHubertStat& copy(const normModHubertStat& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    normModHubertStat& operator=(const normModHubertStat& other);

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const;

    /**#
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * calculates the normalized modified Hubert Statistics 
     * of the clustered data.
     * @param clusteredData std::vector<dmatrix> with the source data.
     * @param index the clustering validity result
     * @param centroids dmatrix with each row representing a centroid
     *        of the corresponding distribution in clusteredData
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const std::vector<dmatrix>& clusteredData,
                       double& index, const dmatrix& centroids) const;
  };

  ////////////////////////////////////////////////////////////////////////
  //////////////////////// Davies Bouldin Index //////////////////////////
  ////////////////////////////////////////////////////////////////////////

  /** 
   * Calculates the Davies Bouldin Index of the given clustering.
   * If different clustering, e.a. with different number of clusters, 
   * of the same data set are tested, the best
   * will be the clustering with the minimal Davies Bouldin Index.
   */
  class daviesBouldinIndex : public clusteringValidity {
  public:

    /**
     * standard contructor
     */
    daviesBouldinIndex();

    /**
     * standard contructor
     */
    daviesBouldinIndex(const daviesBouldinIndex& other);

    /**
     * destructor
     */
    virtual ~daviesBouldinIndex();

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    daviesBouldinIndex& copy(const daviesBouldinIndex& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    daviesBouldinIndex& operator=(const daviesBouldinIndex& other);

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const;

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * calculates the Davies Bouldin Index of the clustered data.
     * @param clusteredData std::vector<dmatrix> with the source data.
     * @param index the clustering validity result
     * @param centroids dmatrix with each row representing a centroid
     *        of the corresponding distribution in clusteredData
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const std::vector<dmatrix>& clusteredData,
                       double& index, const dmatrix& centroids) const;
  };


}

#endif











