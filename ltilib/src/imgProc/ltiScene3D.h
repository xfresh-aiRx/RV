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
 * file .......: ltiScene3D.h
 * authors ....: Jens Paustenbach
 * organization: LTI, RWTH Aachen
 * creation ...: 14.1.2003
 * revisions ..: $Id: ltiScene3D.h,v 1.4 2003/02/17 11:43:48 paustenbach Exp $
 */

#ifndef _LTI_SCENE3_D_H_
#define _LTI_SCENE3_D_H_

#include "ltiDraw3D.h"

namespace lti {
  /**
   *  Tool for creating three dimensional scenes.
   *  A scene can be created in the same way as a scene in drawn with draw3D.
   *  To view a scene, you have first to set up the camera parameters with
   *  the setCamera and setZoom methods. Then the scene can be drawn into an
   *  image with the flush() methods.
   */
  template<class T>
  class scene3D : public draw3D<T> {
  public:

    /**
     * default constructor
     */
    scene3D();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    scene3D(const scene3D& other);

    /**
     * destructor
     */
    virtual ~scene3D();

    /**
     * returns the name of this type ("scene3D")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    scene3D& copy(const scene3D& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    scene3D& operator=(const scene3D& other);

    /**
     * Specifies color to be used. If the template type is rgbPixel this means
     * an actual color. For other template types, this sets a grey-level.
     */
    void setColor(const T& px);

    /**
     * Specifies color to be used. If the template type is rgbPixel this means
     * an actual color. For other template types, this sets a grey-level.
     */
    void setStyle(const char* style); 

    void text(const std::string& str,const dpoint3D& p);

      void number(const int& i, const dpoint3D& p);


    /**
     * sets point with selected color
     */
    void set3D(const dpoint3D& p);

    /**
     * sets 2D point with the selected color if it is in front of
     * all other points with the same projection coordinates, considering
     * the z-buffering (if enabled) and the given z coordinate.
     *
     * If the z-buffering is disabled, the pixel at (x,y) will be set.
     * @param x position x
     * @param y position y
     * @param z deep information
     */
    void set2D(const int& x,const int& y,const double& z);

    /**
     * sets marker with selected color and the given marker type
     */
    void marker3D(const double& x,const double& y,const double& z,
                  char* marker);

    /**
     * sets point with selected color and marker type
     */
    void marker3D(const double& x,const double& y,const double& z);

    /**
     * sets point with selected color
     */
    void set3D(const double& x,const double& y,const double& z);

    /**
     * sets point with selected color
     */
    template<class U>
    void set3D(const hPoint3D<U>& p) {
      set3D(p.x/p.h,p.y/p.h,p.z/p.h);
    };

    /**
     * draw a line between the last used point and (x,y,z)
     */
    void line3DTo(const double& x,const double& y,const double& z);

    /**
     * draw a line between the last used point and (x,y,z)
     */
    void line3DTo(const dpoint3D& p);

    /**
     * draw a line between a and b
     */
    void line3D(const dpoint3D& a,const dpoint3D& b);

    /**
     * draw a line between (x,y,z) and (x2,y2,z2)
     */
    void line3D(const double& x, const double& y, const double& z,
                const double& x2,const double& y2,const double& z2);

    /**
     * draw a box with the opposite corners a and b
     */
    void box(const dpoint3D& a,
             const dpoint3D& b, const bool filled=true);

    /**
     * draw a filled box of the actual color with the opposite corners a and b,
     * and with the line color given
     */
    void box(const dpoint3D& a,
             const dpoint3D& b,
             const T lineColor);


    /**
     * Draw an ellipsoid surface defined through a constant mahalanobis
     * distance between the mean vector and the points of the surface, i.e.
     * the surface contains all points that satisfy the equation
     * (x-mean)^T sigma^(-1)  (x-mean) = cst
     *
     * The color of the ellipsoid is given with the setColor() method, and
     * the lighting conditions with the parameter "lighting".
     *
     * @param mean point at the center of the ellipsoid.
     * @param sigma covariance matrix
     * @param cst mahalanobis distace from the points at the surface and the
     *            mean value
     */
    void ellipsoid(const vector<double> &mean,
                   const matrix<double> &sigma,
                   const double &cst);

    /**
     * Draw the given channel as a 3D structure, where the height of the
     * pixel is given by its intensity.  The actual color will be shaded
     * to produce the 3D effect.  The y-axis will be inverted, to convert the
     * image left coordinate system to the 3D right system.
     *
     * @param chnl the channel with the 3D information
     * @param height the height of the value 1.0 in the channel
     * @param gridSize the number of pixels used for a "cell" in the grid.
     *                 Use for example point(5,5)
     * @param sampleRate determines which pixels in the image are going
     *                   to be used in the grid points.  point(1,1) uses
     *                   all pixels, point(2,2) each second point and so on.
     * @param onlyPoints if true, only the points will be drawn, if false,
     *                   boxes or grids will be drawn.
     * @param useBoxes if true, boxes will be used to display each pixel,
     *                 otherwise a triangular mesh will be used.
     * @param heightColor if true, the channel intensity will define the grey
     *                    tone for the pixel, otherwise the actual color will
     *                    be used.
     * @param drawLines if true, the vertical lines at each grid point will
     *                  be drawn.
     * @param lineColor color for the vertical lines
     * @param drawContour if true, the contour of each triangle in the mesh
     *                    will be redrawn using the countour color given.
     * @param contourColor color used to draw the contour of the triangles.
     *                     Only used if drawContour is true
     */
    void set3D(const channel& chnl,
               const float& height,
               const point& gridSize = point(5,5),
               const point& sampleRate = point(1,1),
               const bool onlyPoints = false,
               const bool useBoxes = false,
               const bool heightColor = false,
               const bool drawLines = false,
               const T& lineColor = T(),
               const bool drawContour = false,
               const T& contourColor = T());


    /**
     * draw axis with dimension "size"
     */
    void axis(const double size);

    /**
     * draw axis with dimension "size" and using the given color for each
     * axis
     */
    void axis(const double size,
              const T& colorX,
              const T& colorY,
              const T& colorZ);

    /**
     * draw a grid in all three dimensions from -size/2 to size/2, 
     * with nbSteps lines between
     */
    void grid(const double size,const int nbSteps);

    /** 
     * set new image and draw scene to this image with the actual camera 
     * parameters.
     */
    bool flush(matrix<T>& image);

    /**
     * set new camera parameters and draw all saved commands into the actual
     * image specified by the use() method
     */
    bool flush(const typename draw3D<T>::parameters& camera);

    /**
     * set new camera parameters and draw all saved commands
     * into the given image.
     */
    bool flush(const typename draw3D<T>::parameters& camera, matrix<T>& image);

    /**
     * draw all saved commands into the image specified by the use method
     * with the actual camera position and direction
     */
    bool flush();

    /**
     * Clears the scene.
     */
    void reset();

    /** 
     * sets the ranges of the axes shown in the configuration dialog
     */
    void setRange(const dmatrix& ranges);

    /** 
     * Returns the ranges of the axes shown in the configuration dialog
     */
    dmatrix getRange() const;

  protected:

    /**
     * these are the code that are used for the instructions that are saved,
     * to generate an image of the scene.
     * The letters after the function name symbolize the argument type:
     * i=int, d=double, dp=dpoint3D, T=T, dv=dvector, dm=dmatrix, ch=channel
     * This is necessary, because some functions exist more than once with
     * different arguments but the names in the enumeration had to be unique.
     */
    enum Instructions {
      setCol,
      setSty,
      text_s_p,
      number_i_dp,
      set2D_i_i_d,
      marker3D_d_d_d_c,
      marker3D_d_d_d,
      set3D_d,
      set3D_ch,
      line3DTo_d,
      line3D_d,
      line_i_i,
      box_dp_dp,
      box_dp_dp_T,
      ellips,
      axis_d,
      axis_d_T_T_T,
      grid_d
    };

    /**
     * List with the codes of the saved instructions.
     * All instructions are coded with an integer value and there parameters
     * are stored in the lists. When a picture is drawn the instructions
     * are extracted from the list in the same order as they were saved.
     * So the parameters can also be extracted from the lists in the same
     * order.
     */
    std::list<Instructions> instructionList;

    /** 
     * parameters for the Instructions
     */
    std::list<double> doubles;

    /** 
     * parameters for the Instructions
     */
    std::list<int> ints;

    /** 
     * parameters for the Instructions
     */
    std::list<T> colors;

    /** 
     * parameters for the Instructions
     */
    std::list<bool> bools;

    /** 
     * parameters for the Instructions
     */
    std::list<dvector> dvectors;

    /** 
     * parameters for the Instructions
     */
    std::list<dmatrix> dmatrices;

    /** 
     * parameters for the Instructions
     */
    std::list<channel> channels;

    /** 
     * parameters for the Instructions
     */
    std::list<std::string> styles;

  private: 
    /** 
     * Set by draw3DDistribution with the ranges of the axes for each dimension
     */
    dmatrix *ranges;

  };
}

#endif

