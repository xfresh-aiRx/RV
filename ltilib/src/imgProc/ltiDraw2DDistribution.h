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
 * file .......: ltiDraw2DDistribution.h
 * authors ....: Jens Paustenbach
 * organization: LTI, RWTH Aachen
 * creation ...: 16.9.2002
 * revisions ..: $Id: ltiDraw2DDistribution.h,v 1.18 2005/06/16 12:40:23 doerfler Exp $
 */

#ifndef _LTI_DRAW2_D_DISTRIBUTION_H_
#define _LTI_DRAW2_D_DISTRIBUTION_H_


#include "ltiFunctor.h"
#include "ltiMatrix.h"
#include "ltiDrawBase.h"
#include <map>
#include <vector>
#include "ltiBoundsFunctor.h"
#include "ltiSTLIoInterface.h"
#include "ltiTypeInfo.h"

namespace lti {
  /**
   *  This class draws a 2-D distribution into an image or a channel.
   *
   *  <B>If you want to draw the distribution into an eps picture it
   *  is mandatory to specify a size for the eps picture.</B> For example:
   *  \code
   *  epsdraw<cmykColor> drawer(std::out,point(800,800));
   *  \endcode
   *
   *  The apply methods draws each point from the given matrices into the
   *  given image.
   *  It is expected that each row of the matrix represents one point
   *  If there is also a vector with ids corresponding to each point in
   *  the matrices each id is drawn in an other color or style.
   *  First all colors are permuted. (In channels this is only white).
   *  After this, if there are more ids than colors available, the style
   *  of the points is permuted. If there is no id-vector you can assign
   *  a color an the style that will be used.
   *  According to the parameter hold the old points that are drawn before
   *  are saved. If an instance of this class is used later the old points
   *  that were saved will appear to in the image.
   */
  template<class T>
  class draw2DDistribution : public functor {
  public:

    /**
     * the parameters for the class draw2DDistribution
     */
    class parameters : public functor::parameters {
    public:
      /**
       * default constructor
       */
      parameters() 
        : functor::parameters() {
        coSystem = false;
        scaleEqual = true;
        //border=40;
        background=getDefaultBackground();
        axisColor=getDefaultAxisColor();

        scale=tpoint<float>(20.0,20.0);
        offset=tpoint<float>(400.0,400.0);
        autoscale=true;

        initPalette();
        initMarker();
      };

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other) 
        : functor::parameters() {
        copy(other);
      };

      /**
       * destructor
       */
      ~parameters() {};

      /**
       * returns name of this type
       */
      const char* getTypeName() const {
        return "draw2DDistribution::parameters";
      };

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other){
# ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        functor::parameters::copy(other);
# else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        functor::parameters& (functor::parameters::* p_copy)
          (const functor::parameters&) =
          functor::parameters::copy;
        (this->*p_copy)(other);
# endif


        coSystem = other.coSystem;
        scaleEqual = other.scaleEqual;
        background = other.background;
        axisColor = other.axisColor;
        marker = other.marker;
        palette = other.palette;
        scale = other.scale;
        offset = other.offset;
        autoscale = other.autoscale;

        return *this;
      };

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other) {
        return copy(other);
      };


      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const {
        return new parameters(*this);
      };

# ifndef _LTI_MSC_6
      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      bool write(ioHandler& handler,const bool complete=true) const
# else
      bool writeMS(ioHandler& handler,const bool complete=true) const
# endif
      {
        bool b = true;
        if (complete) {
          b = handler.writeBegin();
        }

        if (b) {

          lti::write(handler,"coSystem",coSystem);
          lti::write(handler,"scaleEqual",scaleEqual);
          lti::write(handler,"background",background);
          lti::write(handler,"axisColor",axisColor);
          lti::write(handler,"marker",marker);
          lti::write(handler,"palette",palette);

          lti::write(handler,"scale",scale);
          lti::write(handler,"offset",offset);
          lti::write(handler,"autoscale",autoscale);
        }

# ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not able to
        // compile...
        b = b && functor::parameters::write(handler,false);
# else
        bool (functor::parameters::* p_writeMS)(ioHandler&,const bool) const =
          functor::parameters::writeMS;
        b = b && (this->*p_writeMS)(handler,false);
# endif

        if (complete) {
          b = b && handler.writeEnd();
        }

        return b;
      }

# ifdef _LTI_MSC_6
      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      bool write(ioHandler& handler,const bool complete) const {
        // ...we need this workaround to cope with another really awful MSVC bug.
        return writeMS(handler,complete);
      }
# endif

      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
# ifndef _LTI_MSC_6
      bool read(ioHandler& handler,const bool complete=true)
# else
      bool readMS(ioHandler& handler,const bool complete=true)
# endif
      {
        bool b = true;
        if (complete) {
          b = handler.readBegin();
        }

        if (b) {
          lti::read(handler,"coSystem",coSystem);
          lti::read(handler,"scaleEqual",scaleEqual);
          lti::read(handler,"background",background);
          lti::read(handler,"axisColor",axisColor);

          lti::read(handler,"marker",marker);
          lti::read(handler,"palette",palette);

          lti::read(handler,"scale",scale);
          lti::read(handler,"offset",offset);
          lti::read(handler,"autoscale",autoscale);
        }

# ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not able to
        // compile...
        b = b && functor::parameters::read(handler,false);
# else
        bool (functor::parameters::* p_readMS)(ioHandler&,const bool) =
          functor::parameters::readMS;
        b = b && (this->*p_readMS)(handler,false);
# endif

        if (complete) {
          b = b && handler.readEnd();
        }

        return b;
      }

# ifdef _LTI_MSC_6
      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      bool read(ioHandler& handler,const bool complete) {
        // ...we need this workaround to cope with another really awful MSVC
        // bug.
        return readMS(handler,complete);
      }
# endif

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------


      /**
       * if true a coordiate system is drawn. Default false.
       */
      bool coSystem;


      /**
       * if true, the axis will be scaled equal, otherwise the space is used
       * as well as possible. Default true.
       */
      bool scaleEqual;

      /**
       * the scaling factor. Default (20,20)
       */
      tpoint<float> scale;

      /**
       * All pixels are move by this offset. If you set both components of
       * offset to the half of the picture size, the drawn pixels are centred.
       * Default (400,400).
       */
      tpoint<float> offset;

      /**
       * If true, the optimal scaling factor is computed, so that all points
       * of the distribution are in the picture. Default true.
       */
      bool autoscale;

      /**
       * the backgroundColor of the picture. Default black.
       */
      T background;

      /**
       * the drawing color of the coordinate system. Default white.
       */
      T axisColor;

      /**
       * the color palette for drawing
       */
      std::vector<T> palette;

      /**
       * the markers for the different ids
       */
      std::vector<char> marker;

    private:
      // this needs to be specialized
      T getDefaultAxisColor() {
        return T(0);
      };

      T getDefaultBackground() {
        return T(0);
      };

      /**
       * Initialise the default color Palette
       */
      void initPalette()

// Visual C++ .NET 2003 defines _LTI_MSC_6 with 1310
// and it uses also ANSI code
# ifdef _LTI_MSC_6
      {
        // insert complemenatry color to background
        palette.push_back(getDefaultAxisColor());
		palette.push_back(typeInfo<T>::suggestedNorm()/3);
		palette.push_back(typeInfo<T>::suggestedNorm()*2/3);
      }
#endif
      ;

      /**
       * Initialise the default markers
       */
      void initMarker() {
        marker.push_back('o');
        marker.push_back('s');
        marker.push_back('d');
        marker.push_back('^');
        marker.push_back('v');
        marker.push_back('<');
        marker.push_back('>');
      }

    };


    // ------------------------------------------------
    // draw2DDistribution
    // ------------------------------------------------

    /**
     * default constructor
     */
    draw2DDistribution();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    draw2DDistribution(const draw2DDistribution& other);

    /**
     * destructor
     */
    virtual ~draw2DDistribution();

    /**
     * returns the name of this type ("draw2DDistribution")
     */
    virtual const char* getTypeName() const;

    /**
     * Draws the points in dist with the given style into an image
     * @param drawer the drawering object that is used.
     * @param dist dmatrix with the source data.
     * @param hold if true the old image is not deleted
     * @param style the style with which the points are drawn
     * @return true if apply successful or false otherwise.
     */
    bool apply(drawBase<T>& drawer, const dmatrix& dist, const bool& hold=false,
               const char* style="ow0") const;

    /**
     * Draws two dimensions of dist.
     * @see apply(drawer,dist,hold,style)
     * @param drawer the drawer object used.  Usually it is an object
     *        of a class inherited from drawBase<T>, like draw<T>.
     * @param dist dmatrix with the source data.
     * @param first first dimension taken
     * @param second second dimension
     * @param hold if true the old image is not deleted
     * @param style the style with which the points are drawn
     * @return true if apply successful or false otherwise.
     */
    bool apply(drawBase<T>& drawer,
               const dmatrix& dist,
               const int first,
               const int second,
               const bool hold=false,
               const char* style="ow") const;

    /**
     * Draws the point in the two vectors, taking the first as abscissa
     * and the second as ordinate.
     */
    bool apply(drawBase<T>& drawer,const dvector& x,const dvector& y,
               const bool hold=false,const char* style="ow") const;

    /**
     * Draws the points in dist into an image. For each new ID a new style
     * is used.
     * @param drawer that drawing object that is used
     * @param dist dmatrix with the source data.
     * @param ids the ID's accoding to the points in dist
     * @param hold if true the old image is not deleted
     * @return true if apply successful or false otherwise.
     */
    bool apply(drawBase<T>& drawer, const dmatrix& dist,const ivector& ids,
               const bool& hold=false) const;


    /**
     * Draws the points in dist1 and dist2 into an image. In ids1 and ids2
     * the ids according to the points in dist1 and dist2 are expected.
     * For each new ID a new style is used. If points in dist1 and dist2
     * have equal ids, the pionts will be drawn with the same symbol and color,
     * but the symbol will be filled.
     * @param drawer drawing object that is used.
     * @param dist1 dmatrix with the source data.
     * @param ids1 the ID's accoding to the points in dist
     * @param dist2 dmatrix with the source data.
     * @param ids2 the ID's accoding to the points in dist
     * @param hold if true the old image is not deleted
     * @return true if apply successful or false otherwise.
     */
    bool apply(drawBase<T>& drawer, const dmatrix& dist1,const ivector& ids1,
               const dmatrix& dist2,const ivector& ids2,
               const bool& hold=false) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    draw2DDistribution<T>& copy(const draw2DDistribution<T>& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    draw2DDistribution<T>& operator=(const draw2DDistribution<T>& other);

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
     * the distribution with the test points
     */
    dmatrix *testDist;

    /**
     * the ids of the points in testDist
     */
    ivector *testIds;

    /**
     * the distribution with the test points
     */
    dmatrix *trainDist;

    /**
     * the ids of the points in testDist
     */
    ivector *trainIds;

    /**
     * map with the styles according to the ids
     */
    std::map<int,char* > *styles;

    /**
     * map with the color according to the ids
     */
    std::map<int,T > *colorMap;


  };

}

#endif


