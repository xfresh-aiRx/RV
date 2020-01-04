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
 * file .......: ltiCoordinateTransformation.h
 * authors ....: Christoph Meyer, Florian Bley
 * organization: LTI, RWTH Aachen
 * creation ...: 1.10.2004
 * revisions ..: $Id: ltiCoordinateTransformation.h,v 1.2 2005/06/16 12:37:34 doerfler Exp $
 */

#ifndef _LTI_COORDINATE_TRANSFORMATION_H_
#define _LTI_COORDINATE_TRANSFORMATION_H_


#include "ltiMatrix.h"
#include "ltiVector.h"
#include "ltiLinearAlgebraFunctor.h"           // parentclass
#include "ltiConstants.h"                      // for Pi

using namespace std;

namespace lti {
  /**
   * This class transforms the coordinates of a 3D point between two coordinate systems.
   * These two coordinate systems are both of cartesian type, but they are rotated
   * and/or displaced to each other.
   *
   * Inner- vs. Outer-coordinate-system:<br>
   * The two systems will be called outer- and the inner-coordinate-
   * system. The inner system is rotated and/or displaced to the outer one. Let's take a
   * look at a common example. We have a room which is our global reference frame. A 
   * mobile robot can be navigated in this room. On top of the robot, a stereo-camera is 
   * placed on a pan-tilt unit.<br>
   * Our goal is to transform a point from the cameras coordinate system to the global 
   * reference frame. To do this we need two instances of this class.<br> 
   * 1. the camera and the robot: the camera is the inner system and the robot is the 
   *    outer system. The inner system has a fixed displacement and a variable
   *    orientation, which can be changed by the pan-tilt unit.<br>
   * 2. the robot and the room: here the robot is the inner system and the room is the
   *    outer one.
   *
   * To perform a transformation, we need the following parameters (all <b> float </b>):
   *   - The position of the origin (0,0,0) of the inner system in respect to the outer 
   *     coordinate system as a 3D lti::vector. Short: Displacement from outer to inner.
   *   - The three angles in degree of axis-rotations, which are defined as follows:
   *       - alpha: degree to rotate the inner system around the inner x-axis
   *       - beta:  degree to rotate the inner system around the inner y-axis
   *       - gamma: degree to rotate the inner system around the inner z-axis
   *
   * It is important, that you rotate your inner coordinate system axis in the 
   * right-hand-screw-rule in chronological order!<br>
   * That means:<br>
   * Take your right hand (difficult job) and orientate your thumb in the
   * positive direction of the inner-axis around which you want to rotate. When
   * your remaining fingers are not racked, they will point around this axis and point
   * in direction of positive angles.<br>    
   * Attention:<br> 
   * Rotating in chronological order means to rotate the axis in the order x, y, z.
   * Otherwise you will get wrong results.
   * If you rotate around all three axis, beta and gamma can not be seen directly, 
   * so you definitely have to calculate the angles one after the other.
   *
   * Here is how the transformation is performed:<br>
   * For each angle / axis-rotation, a rotation matrix will be calculated. Matrix A 
   * contains the rotation around the x axis, B around y and C around z.
   * The transformed vector \c vT arises by multiplying the untransformed vector \c v with these   
   * matrices:    <tt>vT = C*B*A*v + displacement.</tt><br>
   * In order to save calculation time for subsequent transformations, the three matrices 
   * will be multiplied to D=C*B*A, hence only the following will be computed when you 
   * call the apply-method:     <tt>vT = D*v + displacement.</tt>
   *
   * Here is how to set/initialize the parameters:
   * After creating an object of the parameter class you have to call the function
   * coordinateTransformation::parameters::initParameters to set/initialize the parameters.. In this method, the rotation matrix D will be
   * calculated. Take a look at the code example below.<br> 
   * Alternatives:<br>
   *    - You can directly set the 3x3 total-rotation-matrix D called 
   *      coordinateTransformation::parameters::mRotateAroundXYZAxis
   *      and the displacement-vector called coordinateTransformation::parameters::vDisplace, 
   *      which are both public members. Just pay no attention to the function 
   *      coordinateTransformation::parameters::initParameters and to the grim face 
   *      of the C++ guru standing behind you.
   *    - Or you can also set the displacement-vector, call the method
   *      coordinateTransformation::parameters::defineTransformationMatrices to set the 
   *      three matrices A, B and C and call the method 
   *      coordinateTransformation::parameters::calculateTotalRotationMatrix to 
   *      calculate the matrix D.
   *
   *
   * Example:<br>
   * We have a robot with a stereo camera on it, which is standing in a room (our global
   * reference frame).
   * 1. The camera is placed 0.3 meters right and 0.25 meters in front of the origin of
   *    our global reference frame.
   * 2. The robot points in z-direction, the y-axis of our global frame points to the
   *    ceiling, so the x-axis points to the left.
   * 3. The camera looks 15 degrees to the right and 45 degrees to the ground.
   *
   * Our displacement-vector is: (-0.3, 0, 0.25).
   * To get the rotation-angles we first have to rotate the camera around the x-axis:<br>
   * => alpha = -45 degrees.<br>
   * Then we have to rotate around the y-axis:<br>
   * => beta = +15 degrees.<br>
   * We do not have a rotation around z.<br>
   *
   * So here is the code:
   * \code
   *    lti::coordinateTransformation coordTransform;
   *    lti::coordinateTransformation::parameters transformParam;
   *    float fSystemsDisplacement[3] =  {-0.3, 0, 0.25};
   *    lti::vector<float>  vDisplace(3,fSystemsDisplacement);
   *
   *    transformParam.initParameters(-45, 15, 0, vDisplace);
   *    coordTransform.setParameters(transformParam);
   *
   *    float fPoint[3] = {u, v, w};
   *    lti::vector<float>  vPoint(3,fPoint);
   *    std::cout << "The point: " << vPoint << " in the inner system:" << std::endl;
   *    coordTransform.apply(vPoint);
   *    std::cout << "has the position :" << vPoint << " in the outer system." << std::endl;
   * \endcode
   */

  class coordinateTransformation : public linearAlgebraFunctor {
  public:

    /**
     * ------------------------------------------------------------------ start of parameters
     * The parameters for the class coordinateTransformation
     */
    class parameters : public linearAlgebraFunctor::parameters {
    public:
      // -------------------------------------------------------------
      // Constructors & Destructor
      // ------------------------------------------------
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


      // ------------------------------------------------
      // methods of parameters
      // ------------------------------------------------
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
      bool writeMS(ioHandler& hamRotateAroundXYZAxisndler,const bool complete=true) const;
#     endif


    public:
      /**
       * This method has to be called by the user to initialize the parameters.
       * 
       * The parameters:
       *  - fAngleXAxisRotation, fAngleYAxisRotation and fAngleZAxisRotation
       *    represent the three angles (in degree) of the rotation of the inner coordinate system
       *    in respect to the outer system (explanation is given above).
       *  - vDisplacement represents the displacement between the two coordinate systems
       *    as a vector (x,y,z) from the origin of the outer system to the
       *    origin of the inner system.
       *
       * A note for the interested reader:
       * Internally, the parameter-class calculates three matrices <b>A, B and C</b> from 
       * the three angles and a the total-rotation-matrix
       * <b>D=C*B*A</b>(see introduction), in order to reduce calculation time for the
       * following point transformations. This way, the apply method doesn't have to 
       * calculate this matrix each time a point has to be transformed.
       */
      virtual bool initParameters(float fAngleXAxisRotation,
                                  float fAngleYAxisRotation,
                                  float fAngleZAxisRotation,
                                  vector<float> vDisplacement);

      /**
       * This method is called by initParameters
       * and calculates the three rotation-matrices <b>A, B and C</b>.
       * <b>A</b> contains the rotation around x, <b>B</b> around y and
       * <b>C</b> around z. 
       */
      virtual bool defineTransformationMatrices(float falpha,
                                                float fbeta,
                                                float fgamma);

      /**
       * This method is called by initParameters,
       * and calculates the total-rotation-matrix by multiplying the three
       * rotation-matrices: D=C*B*A
       */
      virtual bool calculateTotalRotationMatrix(void);



      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------
    public:
      /**
       * The three rotation-matrices (3x3) <b>A, B and C</b>
       * for the rotation around each axis.
       */
      typedef matrix<float> transformMatrix;
      transformMatrix mRotateAroundXAxis,
                      mRotateAroundYAxis,
                      mRotateAroundZAxis;

    public:
      /**
       * The total-rotation-matrix(3x3) <b>D=C*B*A</b>(see introduction)
       * which rotates a vector around all three axes. This matrix is
       * calculated when you initialize the parameters with the method
       * initParameters. You also can set this 3x3 matrix directly.
       */
      transformMatrix mRotateAroundXYZAxis;

      /**
       * The displacement between the two coordinate systems, described by
       * a vector (x,y,z) from the origin (0,0,0) of the outer system to the
       * origin of the inner system. You can set this
       * parameter when you initialize the parameters with the method
       * initParameters, or you can set it directly.
       */
      vector<float> vDisplace;

    public:


    };/**
       * ------------------------------------------------------------------ End of Parameters
       */


    
    // ------------------------------------------------
    // Constructors & Destructor
    // ------------------------------------------------
    /**
     * Default constructor
     */
    coordinateTransformation();

    /**
     * Construct a functor using the given parameters
     */
    coordinateTransformation(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    coordinateTransformation(const coordinateTransformation& other);

    /**
     * Destructor
     */
    virtual ~coordinateTransformation();

    
    // ------------------------------------------------
    // methods of class coordinateTransformation
    // ------------------------------------------------
    /**
     * Returns the name of this type ("coordinateTransformation")
     */
    virtual const char* getTypeName() const;

    /**
     * Transforms a given point ( as vector \e srcdest ) in the inner
     * coordinate system (e.g. camera) to a point (also \e srcdest) in the outer
     * coordinate system (e.g. robot).
     * @param srcdest vector with the source data. The result will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(vector<float>& srcdest) const;

    /**
     * Transforms a given point ( as vector \e src ) in the inner
     * coordinate system (e.g. camera) to a point (also \e dest) in the outer
     * coordinate system (e.g. robot).
     * Operates on a copy of the given %parameters.
     * @param src vector with the source data.
     * @param dest vector where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const vector<float>& src, vector<float>& dest) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    coordinateTransformation& copy(const coordinateTransformation& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    coordinateTransformation& operator=(const coordinateTransformation& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * Returns used parameters.
     */
    const parameters& getParameters() const;

  };
}

#endif

