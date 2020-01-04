/*
 * Copyright (C) 1998 - 2004
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
 * file .......: ltiDistanceFromPixel.h
 * authors ....: Peter Hosten, Florian Bley
 * organization: LTI, RWTH Aachen
 * creation ...: 30.9.2004
 * revisions ..: $Id: ltiDistanceFromPixel.h,v 1.2 2005/06/16 12:37:34 doerfler Exp $
 */

#ifndef _LTI_DISTANCE_FROM_PIXEL_H_
#define _LTI_DISTANCE_FROM_PIXEL_H_

#include "ltiImage.h"
#include "ltiVector.h"
#include "ltiMath.h"
#include "ltiFunctor.h"

namespace lti {
  /**
   * This class derives the 3D position of an object from a 2D image point.  
   * Obviously we have to make some assumptions at first to get this to work.<br>
   * First of all we are calculating the distance between a camera positioned at
   * an arbitrary height and an object standing on a flat floor.
   * We also assume that the optical axis of the camera is parallel to the floor.
   * 
   * Different parameters of the camera have to be known:
   * - the picture size,
   * - the focal length d, 
   * - the height of the camera and 
   * - the camera constants kx/ky, which are necessary to calculate the equivalent 
   *   height in cm on the CCD chip if the pixel height is known.<br>
   * Note: As standard unit, this documentation uses cm. Of course you can substitute
   *       this with any other metric or non-metric unit as long as you subsitute them all.
   *
   * So here is some example code. The camera axis is not directly aligned
   * along the axis on which the distance should be calculated. It is rotated a 
   * little bit down to the floor. Thus, we also have to use the class
   * lti::coordinateTransformation:
   * \code
   * lti::coordinateTransformation coordTransform;
   * lti::coordinateTransformation::parameters transformParam;
   * float b;
   * float fDataDisplacement[3] =  { 0, 0, -7};
   * lti::vector<float> vDisplace(3,fDataDisplacement);
   * 
   * std::cout << "y angle of cam (beta):" << std::endl; 
   * std::cin >> b;
   * transformParam.initParameters(0,b,0,vDisplace);
   * coordTransform.setParameters(transformParam);
   *    
   * lti::DistanceFromPixel distance;
   * lti::DistanceFromPixel::parameters distanceParam;      
   * lti::vector<int> imgCoordinate(2,0); 
   * lti::vector<float> spaceCoordinate(3,0);
   * 
   * // parameters for the Sony EVI D100P
   * distanceParam.fHeight=41.0f;
   * distanceParam.iPictureSizeX=379;   // x
   * distanceParam.iPictureSizeY=262;   // y
   * distanceParam.fD=19.07f;           // focal length in cm
   * distanceParam.fKx=0.0555f;         // camera const in x-direction cm/pixel
   * distanceParam.fKy=0.0555f;         // camera const in y-direction cm/pixel
   * 
   * std::cout << "x coordinate of object on floor in pixel (integer) :";
   * std::cin  >> imgCoordinate[0];
   * std::cout << std::endl;
   * std::cout << "y coordinate of object on floor in pixel (integer) :";
   * std::cin  >> imgCoordinate[1];
   * std::cout << std::endl;
   *
   * distance.setParameters(distanceParam);
   * distance.apply(imgCoordinate,spaceCoordinate);
   * std::cout << "3D coordinates of object in camera system: " << spaceCoordinate << std::endl;
   *
   * float eucliddist=0;
   * distance.apply(imgCoordinate,eucliddist);
   * std::cout << "Euclidian distance between object and camera: " << eucliddist << std::endl;
   *
   * std::cout << "Point " << spaceCoordinate << " in the camera coordinate system" << std::endl;
   * coordTransform.apply(spaceCoordinate);
   * std::cout << "has position " << spaceCoordinate << " in the global coordinate system." << std::endl;
   * \endcode
   */
   
   
  class DistanceFromPixel : public functor {
  public:
    /**
     * The parameters for the class DistanceFromPixel
     */
    class parameters : public functor::parameters {
    public:
      /**
       * Default constructor
       */
      parameters();

      /**
       * Copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * Destructor
       */
      ~parameters();

      /**
       * Returns name of this type
       */
      const char* getTypeName() const;

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);


      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const;

      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * Read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

#     ifdef _LTI_MSC_6
      /**
       * This function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * This function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const;
#     endif
      
      // 
     
      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------
    

      /**
       * Height of camera over the ground (cm)
       */
      float fHeight;
      /**
       * Focal length (cm)
       */
      
      float fD;       
      /**
       * Camera const in x-direction (cm/pixel)
       */
      float fKx;      
      /**
       * Camera const in y-direction (cm/pixel)
       */
      float fKy;
      /**
       * Size of the picture in x (pixel)
       */
      int iPictureSizeX;//      
      /**
       * Size of the picture in y (pixel)
       */
      int iPictureSizeY;     
    };

    /**
     * Default constructor
     */
    DistanceFromPixel();

    /**
     * Construct a functor using the given parameters
     */
    DistanceFromPixel(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    DistanceFromPixel(const DistanceFromPixel& other);

    /**
     * Destructor
     */
    virtual ~DistanceFromPixel();

    /**
     * Returns the name of this type ("DistanceFromPixel")
     */
    virtual const char* getTypeName() const;

    
    /**
     * This method calculates the corresponding 3D space point from an 2D image point 
     * The requirements needed for this to work 
     * are described above in the detailed description of this class.  
     * @param src  lti::vector<int> src(2,0)
     * @param dest lti::vector<int> dest(3,0)
     * @return true if apply successful or false otherwise.
     */
    bool apply(const vector<int>& src,vector<float>& dest) const;

    /**
     * This method calculates the euclidian distance between the 3D space position point 
     * of an image point and the camera. The requirements needed for this to work 
     * are described above in the detailed description of this class.
     * @param src  lti::vector<int> src(2,0)
     * @param dest float dest
     * @return true if apply successful or false otherwise.
     */
    bool apply(const vector<int>& src,float& dest) const;


    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    DistanceFromPixel& copy(const DistanceFromPixel& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    DistanceFromPixel& operator=(const DistanceFromPixel& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;
 
 private:
   
   float DeltaY(const int SizeY,const int PointY, const float Ky) const; // Returns relative pixeldistance between the 
   float DeltaX(const int SizeX,const int PointX,const float Kx) const; // middle of the picture and the object's picturepoint   
   float CalcZDistance(const float relPointY,const float Height, const float D) const  ;
   float CalcXDistance(const float relPointX,const float ZDistance, const float D) const;
   
   
   float EuclidDistance(const float XDistance,const float Height,const float ZDistance) const ;
  };
}

#endif

