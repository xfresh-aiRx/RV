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


/* -----------------------------------------------------------------------
 * project ....: LTI Digitale Bild/Signal Verarbeitungsbibliothek
 * file .......: ltiDraw.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 28.4.2002
 * revisions ..: $Id: ltiDraw3D.h,v 1.8 2005/06/16 09:32:42 doerfler Exp $
 */
#ifndef _LTI_DRAW_3D_H
#define _LTI_DRAW_3D_H

#include "ltiDraw.h"
#include "ltiIoObject.h"

namespace lti {
  template <class T>
  class rayTraceObject;

  /**
   * Tool for drawing 3D points and lines in a lti::matrix.  The
   * template type T specifies the type of elements of the matrix.  To
   * draw in a color image you will need for example a
   * lti::draw3D<rgbPixel> object.  Use the parameters to set the
   * camera position and direction.  You can activate/deactivate a
   * z-Buffering using the method resetDeepCtrl(bool), where the
   * boolean should be true if you want to activate the z-Buffering, or false
   * otherwise.  The default state for the z-Buffering is off.
   *
   * Example:
   *
   * \code
   *
   * double a;
   *
   * lti::image canvas;                 // the image to draw on
   * lti::draw3D<lti::rgbPixel> drawer; // our draw object
   * lti::viewer view;                  // a viewer to see what we do
   *
   * canvas.resize(256,256,lti::Black); // 256x256 are enough
   * drawer.use(canvas);
   * drawer.setZoom(50);
   *
   *
   * // take pictures all around... (at 1/2 degree steps...)
   * for (a=0;a<5*2.0*lti::Pi;a+=0.5*lti::Pi/180.0) {
   *   canvas.fill(lti::Black);
   *   drawer.resetDeepCtrl(true); // activate the z-Buffering
   *   drawer.setCamera(1.5*sin(a),1.5*cos(a),1,0,0,0);

   *   // draw a cube
   *   drawer.setColor(lti::White);

   *   // draw the lower plate
   *   drawer.set3D(1,1,-1);
   *   drawer.line3DTo(-1,1,-1);
   *   drawer.line3DTo(-1,-1,-1);
   *   drawer.line3DTo(1,-1,-1);
   *   drawer.line3DTo(1,1,-1);

   *   // draw the upper plate
   *   drawer.set3D(1,1,1);
   *   drawer.line3DTo(-1,1,1);
   *   drawer.line3DTo(-1,-1,1);
   *   drawer.line3DTo(1,-1,1);
   *   drawer.line3DTo(1,1,1);
   *
   *   // and the edges
   *   drawer.setColor(lti::Red);
   *   drawer.line3D(-1, 1,1,-1, 1,-1);
   *   drawer.setColor(lti::Green);
   *   drawer.line3D(-1,-1,1,-1,-1,-1);
   *   drawer.setColor(lti::Blue);
   *   drawer.line3D( 1,-1,1, 1,-1,-1);
   *   drawer.setColor(lti::Yellow);
   *   drawer.line3D( 1, 1,1, 1, 1,-1);

   *   view.show(canvas);
   * }
   * \endcode
   *
   * See also draw3D<T>::parameters for more information about the camera
   * parameters.
   */
  template<class T>
  class draw3D : public draw<T> {
  public:

    /**
     * return a grey value for the proper type T, where the input
     * is always between 0 (black) and 1 (white).
     */
    static T getGray(const float& val);

    /**
     * homogene matrix
     *
     * Due to a MSVC bug the implementation of the class must be here.
     */
    class hmatrix {
    public:
      /**
       * default constructor
       */
      hmatrix() {
        clear();
      };

      /**
       * copy constructor
       */
      hmatrix(const hmatrix& other) {
        copy(other);
      }

      /**
       * destructor
       */
      ~hmatrix() {
      };

      /**
       * clean matrix
       */
      void clear() {
        for (int i=0;i<3;i++)
          for (int j=0;j<4;j++)
            at(i,j) = 0.0;
      };

      /**
       * multiply with a point
       */
      dpoint3D operator*(const dpoint3D& p) const {
        dpoint3D temp;

        const hmatrix &m = (*this);

        temp.x = m.at(0,0)*p.x + m.at(0,1)*p.y + m.at(0,2)*p.z + m.at(0,3);
        temp.y = m.at(1,0)*p.x + m.at(1,1)*p.y + m.at(1,2)*p.z + m.at(1,3);
        temp.z = m.at(2,0)*p.x + m.at(2,1)*p.y + m.at(2,2)*p.z + m.at(2,3);

        return temp;
      };

      /**
       * return value at row m and column n
       */
      double &at(const int m,const int n) {return mem[m][n];};

      /**
       * return value a row m and column n
       */
      const double &at(const int m,const int n) const {return mem[m][n];};

      /**
       * copy member
       */
      hmatrix& copy(const hmatrix& other) {
        int j,i;
        for (j=0;j<3;++j) {
          for (i=0;i<4;++i) {
            mem[j][i] = other.mem[j][i];
          }
        }
        return *this;
      }

      /**
       * alias for copy member
       */
      hmatrix& operator=(const hmatrix& other) {
        return copy(other);
      }

    protected:
      double mem[3][4];
    };


    /**
     * parameters for draw3D.
     *
     * Most of the parameters in this class can be accessed through access
     * methods in the class draw3D directly.  This simplifies changing
     * the parameters and giving them to the drawer object again.
     *
     * The camera position is given by the <code>cam1</code> point.
     * The direction of the camera is given through the second point
     * <code>cam2</code>.  The projective algorithm used will ensure that
     * this second point cam2 will be mapped exactly at the position given
     * by <code>center</code>.
     */
    class parameters : public ioObject {
    public:
      /**
       * default constructor
       */
      parameters()
        : zoom(1.0),perspective(0.0),center(0.5,0.5),
          camPos(0.0,0.0,1.0),elevation(0.0),azimuth(0.0),
          lightPosition(0,0,1000000),
          diffuseColor(draw3D<T>::getGray(223.0f/255.0f)),
          ambientColor(draw3D<T>::getGray(32.0f/255.0f)) {
      };

      /**
       * copy constructor
       */
      parameters(const parameters& other) 
        : ioObject() {
        copy(other);
      }

      /**
       * returns the name of this type
       */
      virtual const char* getTypeName() const {
        return "draw3D::parameters";
      };

      /**
       * copy member
       */
      parameters& copy(const parameters& other) {
        camPos = other.camPos;
        elevation = other.elevation;
        azimuth = other.azimuth;
        zoom = other.zoom;
        perspective = other.perspective;
        center = other.center;
        lightPosition = other.lightPosition;
        diffuseColor = other.diffuseColor;
        ambientColor = other.ambientColor;
        return *this;
      };

      /**
       * alias for copy member
       */
      parameters& operator=(const parameters& other) {
        return copy(other);
      }

      /**
       * returns a pointer to a clone of the parameters.
       */
      virtual parameters* clone() const {
        return new parameters(*this);
      };

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const {
        bool b=true;

        if (complete) {
          b=handler.writeBegin();
        }

        if (b) {
          lti::write(handler,"camPos",camPos);
          lti::write(handler,"elevation",elevation);
          lti::write(handler,"azimuth",azimuth);
          lti::write(handler,"zoom",zoom);
          lti::write(handler,"perspective",perspective);
          lti::write(handler,"center",center);
          lti::write(handler,"lightPosition",lightPosition);
          lti::write(handler,"diffuseColor",diffuseColor);
          lti::write(handler,"ambientColor",ambientColor);
        }

        if (complete) {
          b=b && handler.writeEnd();
        }

        return b;
      };

      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true) {
        bool b=true;

        if (complete) {
          b=handler.readBegin();
        }

        if (b) {
          lti::read(handler,"camPos",camPos);
          lti::read(handler,"elevation",elevation);
          lti::read(handler,"azimuth",azimuth);
          lti::read(handler,"zoom",zoom);
          lti::read(handler,"perspective",perspective);
          lti::read(handler,"center",center);
          lti::read(handler,"lightPosition",lightPosition);
          lti::read(handler,"diffuseColor",diffuseColor);
          lti::read(handler,"ambientColor",ambientColor);
        }

        if (complete) {
          b=b && handler.readEnd();
        }

        return b;
      };

      /**
       * set position and target position of the camera
       */
      void setCamera(const double& x1,const double& y1,const double& z1,
                     const double& x2,const double& y2,const double& z2) {


        double dx,dy,dz;
        double dx2,dy2,dz2;
        double alpha,beta,cosalpha,cosbeta;

        dx = x1-x2;
        dy = y1-y2;
        dz = z1-z2;

        dx2 = dx*dx;
        dy2 = dy*dy;
        dz2 = dz*dz;

        if ((dx2+dy2) != 0) {
          cosalpha = dx/(sqrt(dx2+dy2));
          alpha = abs(acos(cosalpha));
          if (dy<0) {
            alpha *= -1.0;
          }
        }
        else {
          cosalpha = 1.0;
          alpha = 0;
        }

        if ((dx2+dy2+dz2) != 0) {
          cosbeta = sqrt((dx2+dy2)/(dx2+dy2+dz2));
          beta  = abs(acos(cosbeta));
          if (dz<0) {
            beta *= -1.0;
          }
        }
        else {
          cosbeta = 1.0;
          beta  = 0;
        }

        setCamera(x2,y2,z2,(Pi/2.0) - beta,alpha);
      }

      /**
       * set position and direction of the camera
       */
      void setCamera(const double& x1,const double& y1,const double& z1,
                     const double& theElevation,const double& theAzimuth) {
        camPos.copy(dpoint3D(x1,y1,z1));
        elevation = theElevation;
        azimuth = theAzimuth;
      }

      /**
       * set position and target position of the camera
       */
      void setCamera(const dpoint3D& x1,
                     const dpoint3D& x2) {
        setCamera(x1.x,x1.y,x1.z,x2.x,x2.y,x2.z);
      }

      /**
       * set position and direction of the camera
       */
      void setCamera(const dpoint3D& x1,
                     const double& theElevation,const double& theAzimuth) {
        camPos.copy(x1);
        elevation = theElevation;
        azimuth = theAzimuth;
      }

      // ------------------------------------------------------
      // the parameters
      // ------------------------------------------------------

      /**
       * camera zoom factor (default value: 1.0)
       */
      double zoom;

      /**
       * camera perspective factor.  Set to zero for an orthographic
       * projection or greater zero for perspective effects.  You
       * can consider this value as zoom/f with f the focal distance.
       */
      double perspective;

      /**
       * view plane center in relative coordinates (default (0.5,0.5), i.e.
       * in the center of the used image)
       */
      dpoint center;

      /**
       * camera position (default value: (0,0,1))
       */
      dpoint3D camPos;

      /**
       * elevation is the angle (in radians) between the z-axis and
       * the camera axis.
       *
       * Default value: 0.0
       */
      double elevation;

      /**
       * azimuth is the angle (in radians) between the x-axis and the
       * projection of the camera axis on the xy plane.
       *
       * Default value: 0.0
       */
      double azimuth;

      /**
       * position of the light source.
       */
      dpoint3D lightPosition;

      /**
       * Color (or intensity) of the light source.
       *
       * To avoid saturation ensure that diffuseColor+ambientColor is smaller
       * than the maximum allowed value for T (i.e. 255 for ubyte,
       * rgbPixel(255,255,255) or 1.0 for float)
       */
      T diffuseColor;

      /**
       * Color (or intensity) of the ambient light
       *
       * To avoid saturation ensure that diffuseColor+ambientColor is smaller
       * than the maximum allowed value for T (i.e. 255 for ubyte,
       * rgbPixel(255,255,255) or 1.0 for float)
       */
      T ambientColor;
    };


  public:
    /**
     * default constructor
     */
    draw3D();
    /**
     * destructor
     */
    ~draw3D();

    /**
     * return the name of the this object
     */
    virtual const char* getTypeName() const;

    /**
     * Indicates in which image will be drawn
     */
    void use(matrix<T>& img);

    /**
     * set parameters
     */
    bool setParameters(const parameters& opt);

    /**
     * get parameters
     */
    const parameters& getParameters() const {return param;};

    /**
     * get parameters
     */
    parameters& getParameters() {return param;};

    /**
     * set position and target position of the camera
     */
    void setCamera(const double& x1,const double& y1,const double& z1,
                   const double& x2,const double& y2,const double& z2);

    /**
     * set position and direction of the camera
     */
    void setCamera(const double& x1,const double& y1,const double& z1,
                   const double& elevation,const double& azimuth);

    /**
     * set position and target position of the camera
     */
    void setCamera(const dpoint3D& x1,
                   const dpoint3D& x2);

    /**
     * set position and direction of the camera
     */
    void setCamera(const dpoint3D& x1,
                   const double& elevation,const double& azimuth);

    /**
     * set perspective.  Set to zero for an orthographic projection or
     * greater zero for perspective effects.  You can consider this value
     * as -1/f with f the focal distance.  So, to get good results try always
     * positive values
     */
    void setPerspective(const double& p);

    /**
     * set camera plane center.
     *
     * These are relative coordinates, i.e. if you want to map the
     * target camera point <code>cam2</code> (see parameters) exactly
     * at the center of your image, just setCenter(0.5,0.5).
     */
    void setCenter(const double& x,const double& y);

    /**
     * set camera plane center
     *
     * These are relative coordinates, i.e. if you want to map the
     * target camera point <code>cam2</code> (see parameters) exactly
     * at the center of your image, just setCenter(0.5,0.5).
     */
    void setCenter(const dpoint& x);

    /**
     * set zoom factor
     */
    void setZoom(const double& z);

    /**
     * get a read-only reference to the coordinate point in the current image,
     * where the origin of the world coordinates will be projected.
     */
    dpoint getOriginPosition() const;

    /**
     * get a read-only reference of the transformation matrix being used.
     */
    const hmatrix& getCameraMatrix() const;

    /**
     * reset z-Buffer.  With this method you can activate/deactivate
     * the z-Buffering.  This allows the correct display of your
     * drawings.  Note that if you resize the used image (see use()),
     * this object will not be able to detect it automatically (this
     * would take too much time!) and your program could crash!  To
     * avoid problems, call again one of <code>use</code> or
     * <code>resetDeepCtrl</code>methods after resizing your image.
     *
     * @param deep if true, the z-Buffering will be activated,
     * otherwise the z-Buffering will be deactivated.
     */
    void resetDeepCtrl(const bool deep=true);

    /**
     * project 3D point into camera plane
     */
    void p3Dto2D(const double& x,const double& y,const double& z,
                 int &x2,int &y2,double &z2) const;

    /**
     * project 3D point into camera plane
     */
    void p3Dto2D(const dpoint3D& p3,point& p2,double &z2) const;

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
     * sets marker with selected style
     */
    void marker3D(const double& x,const double& y,const double& z);

    /**
     * sets marker with given style
     */
    void marker3D(const double& x,
                  const double& y,
                  const double& z,
                  const char* style);

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
     * draw a grid with dimension "size" using the actual drawnig color.
     * The grid lies in all three dimensions at size/2. 
     */
    void grid(const double size,const int nbSteps);

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


  protected:

    /**
     * Triangle is the base for all other polygons.
     *
     * It is assumed, that the points are given in a "right system", to define
     * the normal of the triangle.  The color of the hidden face of the
     * triangle can be specified at construct time.
     */
    class triangle {
    public:
      /**
       * default constructor
       */
      triangle() {
      }

      /**
       * Access operator to the three points
       */
      dpoint3D& operator[](const int idx) {
        assert(idx < 3);
        return pts[idx];
      }

      /**
       * Access operator to the three points
       */
      const dpoint3D& operator[](const int idx) const {
        assert(idx < 3);
        return pts[idx];
      }

      /**
       * compute the normal of the triangle.  It will NOT be normalized
       */
      dpoint3D getNormal() const {
        dpoint3D a,b,n;
        a.subtract(pts[0],pts[2]);
        b.subtract(pts[1],pts[2]);
        n.x = a.y*b.z - a.z*b.y;
        n.y = a.z*b.x - a.x*b.z;
        n.z = a.x*b.y - a.y*b.x;

        return n;
      }

      /**
       * returns true if the triangle "positive" face is visible for the
       * given camera configuration
       */
      bool visible(const hmatrix& cam) const {
        dpoint3D n(getNormal());
        return ((cam.at(0,0)*n.x + cam.at(0,1)*n.y + cam.at(0,2)*n.z) >= 0);
      }

    protected:
      /**
       * data
       */
      dpoint3D pts[3];
    };

    void drawSymbol(double x, double y,double z);

    /**
     * parameters
     */
    parameters param;

    /**
     * true: use a Z-Buffer
     */
    bool deepCtrl;

    /**
     * Z-Buffer
     */
    matrix<float> deepImg;

    /**
     * actual (last used) 3D point coordinates
     */
    dpoint3D act;

    /**
     * camera homogene transformation matrix
     */
    hmatrix cam;

    /**
     * camera target point
     */
    dpoint3D camTarget;

    /**
     * camera plane center
     */
    dpoint center;

    /**
     * camera plane center in normalized coordinates (0..1)
     */
    dpoint precenter;

    /**
     * checks and corrects 3D line to be within the image
     */
    bool correctLine (int &x,int &y,double &z,
                      const int& x2, const int& y2, const double& z2);

    /**
     * compute the shading color from "color", assuming that the
     * light source is at z-axis in infinity, and the normal vector to the
     * surface is given.
     */
    T shadingColor(const dpoint3D& nrm,const T& color) const;

    /**
     * sets a triangle
     */
    void set3D(const triangle& t,const bool filled = true);

    /**
     * help function for triangle.  Draws a line as io codes in the
     * given lists.
     * This function expects p1.y <= p2.y
     */
    void ioLine(const point& p1,const double& z1,
                const point& p2,const double& z2,
                ivector& in, ivector& out,
                dvector& zIn, dvector& zOut,
                const int firstY) const;

    /**
     * copysign return abs(a) with the sign of b
     */
    inline double copysign(const double& a,const double& b);

    /**
     * returns 1 if a >= 0.0; -1 otherwise
     */
    inline int sign(const double& a) {return (a >= 0.0) ? 1 : -1;};

  private:
    /**
     * some internal variables needed for the 3D channel drawing
     */
    //@{
    const channel* mChannel;
    float mHeight;
    point mGridSize;
    point mSampleRate;
    bool mOnlyPoints;
    bool mUseBoxes;
    bool mHeightColor;
    bool mDrawLines;
    T mLineColor;
    bool mDrawContour;
    T mContourColor;
    //@}

    /**
     * draw a cell
     */
    void drawCell(const int x,const int y);

  };

  template<class T>
  inline double draw3D<T>::copysign(const double& a,const double& b) {
    return (sign(a)*sign(b))*a;
  }

  // --------------------------------------------------------------------------
  // Ray Tracing Objects
  // --------------------------------------------------------------------------

  // These are not internal classes of draw3D<> to avoid some bugs of
  // MS VC++ with template inner classes.

  /**
   * abstract class parent for all objects that can be ray-traced.
   *
   * All objects derived from this class are used internally in
   * lti::draw3D objects.
   */
  template <class T>
  class rayTraceObject : public object {
  public:
    /**
     * The constructor must receive the drawer object, where this
     * object will be draw.
     */
    rayTraceObject(draw3D<T>& drawer);

    /**
     * Compute the color value of the pixel at x,y
     * @param x x coordinate in the image
     * @param y y coordinate in the image
     * @param z2 z coordinate of the surface point belonging to the given
     *           pixel (required for the z-buffering).
     * @param val computed color (or gray) value for the given (x,y) pixel
     * @return true if the position corresponds to a pixel on the surface
     *              of the object.
     */
    virtual bool rayTrace(const int x,const int y,
                          double& z2,T& val) = 0;

    /**
     * render the whole object
     * @return true if something was drawn, of false otherwise.
     */
    virtual bool render() = 0;

    /**
     * return the name of the this object
     */
    virtual const char* getTypeName() const;

  protected:
    /**
     * Drawer object
     */
    draw3D<T>& drawer;

    /**
     * parameters of the drawer
     */
    const typename draw3D<T>::parameters& param;

    /**
     * camera homogene transformation matrix
     */
    typename draw3D<T>::hmatrix cam;

    /**
     * inverse camera rotation matrix of the drawer object
     */
    typename draw3D<T>::hmatrix camInv;

    /**
     * position of the projected origin of the world coordinates
     */
    dpoint center;

    /**
     * multiply ((a/norm) * (b/norm))*norm
     */
    T multiplyNormed(const T& a,const T& b) const;

    /**
     * add with clipping
     */
    T addSaturated(const T& a,const T& b) const;

    /**
     * compute the vector in world coordinates parallel to the ray
     * going through the focal point and the point (x,y) in the image plane.
     *
     * @param x image coordinate x
     * @param y image coordinate y
     * @param imgp position of the image point (x,y) (on the image plane) in
     *             the world coordinate system
     * @param ray  direction of the ray going through the point (x,y) and
     *             the focal point.
     */
    void p2Dto3D(const int& x,
                 const int& y,
                 dpoint3D& imgp,
                 dpoint3D& ray) const;

    /**
     * compute the inverse of the camera transformation matrix
     * (only the rotation part is created.  The result is left
     * in the attribute member camInv
     */
    void buildInverseCamera();

  };

  /**
   * Ray tracing object for ellipsoids
   */
  template<class T>
  class rayTraceEllipsoid : public rayTraceObject<T> {
  public:
    /**
     * The constructor must receive the drawer object, where this
     * object will be draw.
     */
    rayTraceEllipsoid(draw3D<T>& drawer);

    /**
     * The constructor must receive the drawer object, where this
     * object will be draw.
     *
     * @param drawer the drawer object where the ellipsoid will be drawn.
     * @param theMean mean point of the ellipsoid
     * @param covar covariance matrix used in the definition of the
     *              ellipsoid.  This should be a 3x3 symmetric positive
     *              definite matrix.
     * @param theMahDist mahalanobis distance using the given covariance
     *                   matrix between the mean point and the surface of
     *                   the ellipsoid.
     */
    rayTraceEllipsoid(draw3D<T>& drawer,
                      const dpoint3D& theMean,
                      const dmatrix& covar,
                      const double& theMahDist = 1);

    /**
     * set the parameters of the ellipsoid.
     *
     * @param theMean mean point of the ellipsoid
     * @param covar covariance matrix used in the definition of the
     *              ellipsoid. This should be a 3x3 symmetric positive
     *              definite matrix.
     * @param theMahDist mahalanobis distance using the given covariance
     *                   matrix between the mean point and the surface of
     *                   the ellipsoid.
     */
    void setEllipsoid(const dpoint3D& theMean,
                      const dmatrix& covar,
                      const double& theMahDist = 1);
    /**
     * Compute the value of the pixel at x,y
     * @param x x coordinate in the image
     * @param y y coordinate in the image
     * @param z2 z coordinate of the surface point belonging to the given
     *           pixel (required for the z-buffering).
     * @param val computed color (or gray) value for the given (x,y) pixel
     * @return true if the position corresponds to a pixel on the surface
     *              of the object.
     */
    virtual bool rayTrace(const int x,const int y,double& z2,T& val);

    /**
     * render the whole object.
     * @return true if something was drawn, of false otherwise
     */
    virtual bool render();

    /**
     * return the name of the this object
     */
    virtual const char* getTypeName() const;

  protected:
    /**
     * Given a point of the image plane in world coordinates and the
     * direction of a ray passing through the focal point and the
     * image point, compute the intersection of the ray and the
     * ellipsoid formed using a constant mahalanobis distance from
     * the mean value to the surface of the ellipsoid under
     * consideration of the given covariance matrix.
     *
     * @param ip point of the image plane in world coordinates
     *               (see p2Dto3D())
     * @param ray point describing the direction of a ray passing through the
     *            image point and the focal point
     * @param mu  mean value of the ellipsoid
     * @param sigma covariance matrix
     * @param mah mahalanobis distance between the ellipsoid surface and
     *            the mean value mu
     * @param lambda imgPnt + lambda*ray is the point on the ellipsoid
     *        surface to be considered
     * @param sp point at the surface of the ellipsoid
     * @return true if point was found, or false if the image point
     *              does not belong to the ellipsoid
     */
    bool intersectRayEllipsoid(const dpoint3D &ip,
                               const dpoint3D &ray,
                               const dpoint3D &mu,
                               const matrix<double> &sigma,
                               const double &mah,
                               double &lambda,
                               dpoint3D &sp);

    /**
     * compute the normal vector of an ellipsoid surface at a specific point
     */
    void calcNormal(const dpoint3D &intersectionPoint,
                    const dpoint3D &mu,
                    const matrix<double> &sigma,
                    dpoint3D &normal) const;

    /**
     * mean point of the ellipsoid
     */
    dpoint3D mean;

    /**
     * covariance matrix of the ellipsoid
     */
    dmatrix sigma;

    /**
     * inverse matrix of the covariance matrix
     */
    dmatrix invMat;

    /**
     * mahalanobis distance of the ellipsoid surface to the mean point
     * using the given covariance matrix
     */
    double mahDist;

    /**
     * current color of the ellipsoid
     */
    T storedColor;

  private:
    /** @name temporary variables
     */
    //@{
    /**
     * temporary variables declared as attributes to save some time
     * rendering the ellipsoid. (the code for which this variables
     * are required was generated automatically, that's the reason
     * why the names are so descriptive...)
     */
    double t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t16,t17,t18,t21,
      t22,t23,t24,t26,t27,t29,t30,t31,t32,t35,t46,t47,t48,t49,t50,t52,t54,
      t56,t57,t58,t61,t63,t65,t66,t69,t71,t73,t74,t76,t77,t81,t83,t85,t88,
      t91,t93,t96,t98,t99,t102,t104,t106,t108,t110,t114,t117,t119,t125,t127,
      t130,t138,t139,t141,t142,t144,t146,t158,t161,t168,t169,t179,t183,t205,
      t226;

    /**
     * image point
     */
    dpoint3D ip;

    /**
     * surface point
     */
    dpoint3D sp;

    /**
     * ray direction between focal point and surface point
     */
    dpoint3D ray;

    /**
     * normal vector to the ellipsoid surface
     */
    dpoint3D normal;

    /**
     * direction between light source and surface point
     */
    dpoint3D lDir;

    /**
     * temporary factors to find intersections
     */
    double lambda,cosL;
    //@}
  };

  // -------------------------------------------------------------------------

  /**
   * write the draw3D<>::parameters in the given ioHandler.
   * The complete flag indicates
   * if the enclosing begin and end should be also be written or not
   */
  inline bool write(ioHandler& handler,const draw3D<rgbPixel>::parameters& p,
             const bool complete = true) {
    return p.write(handler,complete);
  }

  inline bool write(ioHandler& handler,const draw3D<float>::parameters& p,
             const bool complete = true) {
    return p.write(handler,complete);
  }

  inline bool write(ioHandler& handler,const draw3D<ubyte>::parameters& p,
             const bool complete = true) {
    return p.write(handler,complete);
  }

  inline bool write(ioHandler& handler,const draw3D<double>::parameters& p,
             const bool complete = true) {
    return p.write(handler,complete);
  }


  /**
   * read the draw3D<>::parameters from the given ioHandler.
   * The complete flag indicates
   * if the enclosing begin and end should be also be written or not
   */
  inline bool read(ioHandler& handler,draw3D<rgbPixel>::parameters& p,
            const bool complete = true) {
    return p.read(handler,complete);
  }

  inline bool read(ioHandler& handler,draw3D<ubyte>::parameters& p,
            const bool complete = true) {
    return p.read(handler,complete);
  }

  inline bool read(ioHandler& handler,draw3D<float>::parameters& p,
            const bool complete = true) {
    return p.read(handler,complete);
  }

  inline bool read(ioHandler& handler,draw3D<double>::parameters& p,
            const bool complete = true) {
    return p.read(handler,complete);
  }



}

#endif


