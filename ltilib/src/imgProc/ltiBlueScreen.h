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
 * file .......: ltiBlueScreen.h
 * authors ....: Marius Wolf  
 * organization: LTI, RWTH Aachen
 * creation ...: 9.4.2003
 * revisions ..: $Id: ltiBlueScreen.h,v 1.10 2005/06/16 09:32:41 doerfler Exp $
 */

#ifndef _LTI_BLUE_SCREEN_H_
#define _LTI_BLUE_SCREEN_H_


#include "ltiTypes.h"
#include "ltiConvolution.h"
#include <ltiRGBPixel.h> 

#include "ltiModifier.h"

namespace lti {
  
  /**
   * This functor replaces a specified background color of an image
   * with another color or another image ("Bluescreen Technique").
   *
   * The background color is indicated through a spherical or cubic window
   * in the RGB color space.
   */
  class blueScreen : public modifier {
  public:       

    /**
     * the parameters for the class blueScreen
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
       * Type to define the color space window used to define
       * the homogeneous background color that must be replaced by
       * another one.
       */
      enum eRegionMode { 
        Cube, /**< Cube */
        Sphere /**< Sphere */
      };

      /**
       * The original background color; default is black
       */
      rgbPixel screenColor;

      /**
       * Tolerance for the background color.
       * Size is half cube edge length, and sphere radius; default is 70.    
       */
      int size;
    
      /**
       * Compute the tolerance in rgb-color-space based on a cube or a sphere
       *
       * default is "Cube"       
       */
      eRegionMode regionMode;

      /**
       * Enable gauss filterin (slower but the edges in the result
       * look smoother)
       *
       * default is "true"
       */
      bool gaussFilter;

      /**
       * Kernel size for the gauss filter; default is 4
       */
      int gaussKernelSize;

       /**
       * The threshold value for gauss-filtered mask; default is 150
       */
      int gaussMaskThreshold;        

      
    };

    /**
     * default constructor
     */
    blueScreen();

    /**
     * Construct a functor using the given parameters
     */
    blueScreen(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    blueScreen(const blueScreen& other);

    /**
     * destructor
     */
    virtual ~blueScreen();

    /**
     * returns the name of this type ("blueScreen")
     */
    virtual const char* getTypeName() const;
  
    /**
     * operates on the given %parameter.
     * @param srcdest image with the source data.  The result
     *                 will be left here too.
     * @param background: new background image
     * @return true if apply successful or false otherwise.
     */
    bool apply(image& srcdest,image& background) const;

    /**
     * operates on the given %parameter.
     * @param srcdest image with the source data.  The result
     *                 will be left here too.
     * @param background: new background color
     * @return true if apply successful or false otherwise.
     */
    bool apply(image& srcdest,rgbPixel& background) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src image with the source data.
     * @param dest image where the result will be left.
     * @param background: new background image
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,
               image& dest,
               image& background) const;
    /**
     * operates on a copy of the given %parameters.
     * @param src image with the source data.
     * @param dest image where the result will be left.
     * @param background: new background color
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,
               image& dest,
               rgbPixel& background) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    blueScreen& copy(const blueScreen& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    blueScreen& operator=(const blueScreen& other);

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
     * Checks if a given rgbPixel is in the defined cube-shaped area
     * in the rgb-color-space
     */
    bool inCube(rgbPixel check) const;
  };
}

#endif



