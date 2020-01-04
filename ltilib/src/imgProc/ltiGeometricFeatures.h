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
 * file .......: ltiGeometricFeatures.h
 * authors ....: Ulle Canzler
 * organization: LTI, RWTH Aachen
 * creation ...: 11.1.2001
 * revisions ..: $Id: ltiGeometricFeatures.h,v 1.41 2005/06/16 12:35:10 doerfler Exp $
 */

#ifndef _LTI_GEOMETRIC_FEATURES_H_
#define _LTI_GEOMETRIC_FEATURES_H_

#include "ltiTypes.h"
#include "ltiContour.h"
#include "ltiMath.h"
#include <map>

// include parent class:
#include "ltiGlobalFeatureExtractor.h"

namespace lti {
  /**
   * Computes features, which describe some geometric properties of a 2D shape.
   * Results can be obtained as vector or map.
   *
   * The shape %object may be passed as areaPoints or
   * borderPoints. The Features are grouped. Group 0 will always be
   * calculated, the other groups only if indicated by the
   * corresponding %parameter.  The following features are
   * available:<br>
   *
   * (T,R,S,F indicate invariance against Translation, Rotation,
   * Scaling, Flipping)
   *
   * (The quoted "..." names are the key-names for accessing the
   * result map and at the same time the const-aliases for indices of
   * the result %vector.)
   *
   * Feature Group 0 (COG means "center of gravity").
   *
   * - feat[0] = "areasize"    = (T,F) number of enclosed pixels (area).
   * - feat[1] = "bordersize"  = (T,F) number of border pixels.
   * - feat[2] = "xcog"        = x-component of COG.
   * - feat[3] = "ycog"        = y-component of COG.
   * - feat[4] = "xmin"        = smallest x coordinate.
   * - feat[5] = "xmax"        = largest x coordinate.
   * - feat[6] = "ymin"        = smallest y coordinate.
   * - feat[7] = "ymax"        = largest y coordinate.
   * - feat[26]= "compactness" = (T,R,S,F)
   *    compactness=(4PI*area/(bordersize)^2), ranges from 0 to 1.
   *
   * Feature Group 1 (see Sonka, Hlavac, Boyle: Image Processing,
   * Analysis, and Machine Vision).
   *
   * - feat[8] = "m02"         = (T) central moment m02.
   * - feat[9] = "m03"         = (T) central moment m03.
   * - feat[10]= "m11"         = (T) central moment m11.
   * - feat[11]= "m12"         = (T) central moment m12.
   * - feat[12]= "m20"         = (T) central moment m20.
   * - feat[13]= "m21"         = (T) central moment m21.
   * - feat[14]= "m30"         = (T) central moment m30.
   *
   * - feat[22]= "j1"          = (T,R,F) inertia parallel to main axis.
   * - feat[23]= "j2"          = (T,R,F) inertia orthogonal to main axis.
   * - feat[24]= "orientation" = (T,S) orientation of main
   *             axis. range: -90 to 90 degrees.
   * - feat[25]= "eccentricity"= (T,R,S,F) eccentricity, ranges from 0
   *             (=circle) to 1 (=line).
   *
   * Feature Group 2. These are the first 7 moment invariants as
   * described in "Visual Pattern Recognition by Moment Invariants" by
   * Ming-Kuei Hu (IRE Transactions on Information Theory,
   * 1962). Since values can be very small, they have additionally
   * been scaled by the function sign(x)*log(abs(x))
   * - feat[15]= "hu1"         = (T,R,S,F) moment invariant 1.
   * - feat[16]= "hu2"         = (T,R,S,F) moment invariant 2.
   * - feat[17]= "hu3"         = (T,R,S,F) moment invariant 3.
   * - feat[18]= "hu4"         = (T,R,S,F) moment invariant 4.
   * - feat[19]= "hu5"         = (T,R,S,F) moment invariant 5.
   * - feat[20]= "hu6"         = (T,R,S,F) moment invariant 6.
   * - feat[21]= "hu7"         = (T,R,S) moment invariant 7.
   *
   * Feature Group 3. (note that "compactness" is now computed in feature
   * group 0 and j1, j2, orientation, eccentricity are now computed in
   * feature group 1 at no extra cost)
   *
   * Empty now, but keept for compatibility with group 4
   *
   * Feature Group 4. All the following distances are relative to the COG.
   * Calculation of these features is always performed using
   * BoundaryDefinition == CentralBoundary.
   *
   * - feat[27]= "rmin"        = (T,R,F) minimum distance to border.
   * - feat[28]= "rmax"        = (T,R,F) maximum  distance to border.
   * - feat[29]= "rmean"       = (T,R,F) mean distance to border.
   * - feat[30]= "dleft"       = (T,R -90°to+90°) leftmost distance to
   *                             main axis.
   * - feat[31]= "dright"      = (T,R -90°to+90°) rightmost distance to main
   *                             axis.
   * - feat[32]= "dfront"      = (T,R -90°to+90°) frontmost distance to COG,
   *                             along main axis.
   * - feat[33]= "drear" = (T,R -90°to+90°) rearmost distance to COG, along
   *                       main axis.
   */
  class geometricFeatures : public globalFeatureExtractor {
  public:
    /**
     * Labels for feature indices
     */
    enum {
      areasize    = 0,  /**< areasize   */   
      bordersize  = 1,  /**< bordersize */ 
      xcog        = 2,  /**< xcog */       
      ycog        = 3,  /**< ycog */
      xmin        = 4,  /**< xmin */
      xmax        = 5,  /**< xmax */
      ymin        = 6,  /**< ymin */
      ymax        = 7,  /**< ymax */
      m02         = 8,  /**< m02 */
      m03         = 9,  /**< m03 */
      m11         = 10, /**< m11 */
      m12         = 11, /**< m12 */
      m20         = 12, /**< m20 */
      m21         = 13, /**< m21 */
      m30         = 14, /**< m30 */
      hu1         = 15, /**< hu1 */
      hu2         = 16, /**< hu2 */
      hu3         = 17, /**< hu3 */
      hu4         = 18, /**< hu4 */
      hu5         = 19, /**< hu5 */
      hu6         = 20, /**< hu6 */
      hu7         = 21, /**< hu7 */
      j1          = 22, /**< j1  */
      j2          = 23, /**< j2  */
      orientation = 24, /**< orientation  */
      eccentricity= 25, /**< eccentricity */
      compactness = 26, /**< compactness  */
      rmin        = 27, /**< rmin   */
      rmax        = 28, /**< rmax   */
      rmean       = 29, /**< rmean  */
      dleft       = 30, /**< dleft  */
      dright      = 31, /**< dright */
      dfront      = 32, /**< dfront */
      drear       = 33, /**< drear  */ 
      numberOfFeatures = 34 /**< numberOfFeatures */ 
    };

    /**
     * the parameters for the class geometricFeatures
     */
    class parameters : public globalFeatureExtractor::parameters {
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
      virtual bool write(ioHandler& handler,
                         const bool complete=true) const;

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
#     endif

      /**
       * Toggle calculation of feature group 1.
       *
       * Default is true
       */
       bool calcFeatureGroup1;
      /**
       * Toggle calculation of feature group 2.
       *
       * Default is true
       */
       bool calcFeatureGroup2;
      /**
       * Toggle calculation of feature group 3.
       *
       * Default is true
       */
       bool calcFeatureGroup3;
      /**
       * Toggle calculation of feature group 4.
       *
       * Default is true
       */
       bool calcFeatureGroup4;

       /**
        * Boundary definition.
        *
        * The boundary definition to be used (default: CentralBoundary).
        */
       enum eBoundaryDefinition {
         /**
          * The central boundary is the line drawn from the center of
          * each borderPoint to the center of the following
          * borderPoint, and from the last one's center to the first
          * one's (with sub-pixel accuracy, meaning that the length is
          * the same as it were in continuous space).
          *
          * A single pixel has a central boundary length of 0 and an
          * area of 0.  A tiny rectangle with upper left corner at
          * (0,0) and bottom right corner at (1,1) has an outer
          * boundary length of 4 and an area of 1.
          *
          * Note: We define that objects with an area of 0 have the
          * same COG as by the OuterBoundary definition.
          *
          * \b Disadvantages: Very large objects may lead to
          * numerical problems in feature group 2 (and thus above).
          *
          * Formulas taken from: Carsten Steger: On the Calculation of
          * Arbitrary Moments of Polygons; Technical Report
          * FGBV-96-05, Forschungsgruppe Bildverstehen (FG BV),
          * Informatik IX, Technische Universität München, October
          * 1996
          *
          * http://www9.informatik.tu-muenchen.de/people/steger/publications.html
          */
         CentralBoundary,
         /**
          * The outer boundary delimits the borderPoints completely,
          * ie a single pixel has an outer boundary length of 4 and an
          * area of 1.  A tiny rectangle with upper left corner at
          * (0,0) and bottom right corner at (1,1) has an outer
          * boundary length of 8 and an area of 4. I.e. the boundary
          * is considered to be part of the object (completely).
          *
          * \b Disadvantages: By this definition, a square and a
          * circle with equal side length resp. diameter have the same
          * boundary length if the square is parallel to the
          * coordinate axes. This contradicts intuition and renders
          * features like compactness rather meaningless. Also, an
          * object's boundary length is very susceptible to rotation.
          */
         OuterBoundary,
         /**
          * "Approximation" uses CentralBoundary for the calculation
          * of feature group 4 and the bordersize, and OuterBoundary
          * for all others.
          *
          * <b>Disadvantages</b>: This definition is mathematically
          * inconsistent, e.g.  especially very small objects may have
          * a compactness greater than 1.
          */
         Approximation
       };

       /**
        * Type of boundary definition to use.
        *
        * Default value: CentralBoundary
        */
       eBoundaryDefinition boundaryDefinition;
    };

    /**
     * default constructor
     */
    geometricFeatures();

    /**
     * Create functor with the given parameters
     */
    geometricFeatures(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    geometricFeatures(const geometricFeatures& other);

    /**
     * destructor
     */
    virtual ~geometricFeatures();

    /**
     * returns the name of this type ("geometricFeatures")
     */
    virtual const char* getTypeName() const;

    /**
     * Compute features for the object described by the given areaPoints.
     * The boundaryDefinition set in the parameters will be considered.
     *
     * You can access a feature by its enum-name
     * (e.g. features[eFeatures::areasize]).
     * Please refer to Class Documentation for enum-names.
     *
     * @param ap list of areaPoints
     * @param features the Features as vector of doubles
     * @return true if successful, false otherwise.
     */
    bool apply(const lti::areaPoints& ap,dvector& features) const;

    /**
     * Compute features for the object described by the given borderPoints.
     * This methos always uses the boundaryDefinition "CentralBoundary" for
     * reasons of computational efficiency.
     *
     * You can access a feature by its enum-name
     * (e.g. features[eFeatures::areasize]).
     * Please refer to Class Documentation for enum-names.
     *
     * @param borderPointList list of borderPoints
     * @param features the Features as vector of doubles
     * @return true if successful, false otherwise.
     */
    bool apply(const lti::borderPoints& borderPointList,
               dvector& features) const;

    /**
     * Considers io Points for feature computation.
     *
     * The boundaryDefinition set in the parameters is currently not
     * considered; an approximation is returned instead.
     *
     * You can access a feature by its enum-name
     * (e.g. features[eFeatures::areasize]).  Please refer to Class
     * Documentation for enum-names.
     *
     * @param ioPointList list of ioPoints
     * @param features the Features as vector of doubles
     * @return true if successful, false otherwise.
     */
    bool apply(const lti::ioPoints& ioPointList,
               dvector& features) const;

    /**
     * Considers all area Points for feature computation.
     *
     * Warning! Working with a map is less efficient than with a
     * vector, but easier to use.
     *
     * You can access a feature by its key-name (e.g. features["areasize"]).
     * Please refer to Class Documentation for key-names.
     *
     * @param ap list of areaPoints
     * @param features a map with the extracted features
     * @return true if successful, false otherwise.
     */
    bool apply(const lti::areaPoints& ap,
               std::map<std::string,double>& features) const;

    /**
     * Considers border Points for feature computation.
     * Warning! Working with a map is less efficient than with a vector,
     * but easier to use.
     * You can access a feature by its key-name (e.g. features["areasize"]).
     * Please refer to Class Documentation for key-names.
     * @param borderPointList list of borderPoints
     * @param features a map with the extracted features
     * @return true if successful, false otherwise.
     */
    bool apply(const lti::borderPoints& borderPointList,
               std::map<std::string,double>& features) const;

    /**
     * Considers io Points for feature computation.
     * Warning! Working with a map is less efficient than with a vector,
     * but easier to use.
     * You can access a feature by its key-name (e.g. features["areasize"]).
     * Please refer to Class Documentation for key-names.
     * @param ioPointList list of ioPoints
     * @param features a map with the extracted features
     * @return true if successful, false otherwise.
     */
    bool apply(const lti::ioPoints& ioPointList,
               std::map<std::string,double>& features) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    geometricFeatures& copy(const geometricFeatures& other);

    /**
     * Alias for copy operator.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    geometricFeatures& operator=(const geometricFeatures& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Compute the length of the given border according to the
     * given boundary definition.
     */
    float getLength(const borderPoints& bp,
      const parameters::eBoundaryDefinition& boundaryDefinition) const;

  private:
    /**
     * This is an internal help function.
     * sign(x)*log(abs(x)) for scanning the channel reg. Objects
     * @param x value
     * @return sign(x)*log(abs(x))
     */
    inline double logn( double x ) const;

    /**
     * This is an internal help function for scanning the channel reg. Objects
     * @param numerator nominator
     * @param denominator denominator
     * @return fraction, in case of deviding by zero -> zero will be returned !
     */
    inline double divide( double numerator, double denominator ) const;

    /**
     * This is an internal function for mapping features to a dvector
     * @param theMap
     * @param theVector
     * @return true if successfull
     */
    bool mapFeatures(dvector& theVector, 
                     std::map<std::string,double>& theMap) const;

    /**
     * Compute the COG of a massless borderPoints object (according
     * to OuterBoundary definition) 
     */
    void computeMasslessCOG(const borderPoints& bp, 
                            double& xCog,
                            double& yCog) const;

public: // used by lti::borderPoints to compute their own length

    /**
     * helper array for chain codes, stores direction (e.g. North)
     * depending on x/y step. x/y step may be -1/0/1. Add 1 -> 0/1/2.
     * Use as index, e.g. direction[xStep+1][yStep+1] to obtain direction.
     * direction[1][1] means neither x nor y step -> Invalid.
     */
    static const int direction[3][3];

    /**
     * Helper array that stores the length of the outer boundary contributed
     * by two steps in the directions that serve as index. E.g. one step N,
     * then one step NW -> outerBoundaryLength[NW][N]. Invalid step sequences
     * are marked with a value of -1.
     *
     * For a definition of the outer boundary, see eBoundaryDefinition.
     */
    static const int outerBoundaryLength[8][8];

    /**
     * Helper array to compute the length of the central boundary. When
     * iterating through the borderPoints, a step can be defined as the
     * difference from the current point to the last point. Decomposing this
     * step into x and y step and incrementing each by 1, we have a tuple
     * (0-2,0-2) with (1,1) meaning no step (which is invalid). Using these
     * steps as indices to this array, the step's contribution to the central
     * boundary length is returned.
     *
     * For a definition of the central boundary, see eBoundaryDefinition.
     */
    static const double centralBoundaryLength[3][3];
  };
}

#endif

