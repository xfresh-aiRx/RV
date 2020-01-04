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
 * file .......: ltiSnake.h
 * authors ....: Pablo Alvarado, Peter Gerber, Axel Berner
 * organization: LTI, RWTH Aachen
 * creation ...: 15.04.99
 * revisions ..: $Id: ltiSnake.h,v 1.21 2005/06/16 12:35:44 doerfler Exp $
 */

#ifndef _LTI_SNAKE_H
#define _LTI_SNAKE_H

#include "ltiContour.h"
#include "ltiImage.h"
#include "ltiTypes.h"
#include "ltiSegmentation.h"
#include "ltiGenericMatrix.h"

namespace lti {

  /**
   * Snake segmentation.
   *
   * This is a very old class that do not implement the "real" snakes
   * but a form of connected region growing algorithms (a very simple
   * case of geometrical snakes).
   *
   * If you have implemented a real snake functor, we would be glad to
   * replace this one with it.
   *
   * \deprecated This is not a real snake functor.
   */
  class snake : public segmentation {
  public:
    /**
     * Parameters for the snakes
     */
    class parameters: public segmentation::parameters {
    public:
     /**
       * default constructor
       */
      parameters();
      /**
       * destructor
       */
      virtual ~parameters();
      /**
       * creates a clone of this object
       */
      virtual functor::parameters* clone() const;
      /**
       * become the other object
       */
      parameters& copy(const parameters& other);
      /**
       * name of this type
       */
      virtual const char* getTypeName() const;

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
       * write the parameters in the given ioHandler
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

      // -----------------------
      //     the parameters
      // -----------------------

      /**
       * placement of the reference color patch.  This is a rectangular
       * region that will be ignored by the initialization of the snake
       *
       * Default value: rectangle(180,0,255,30)
       */
      rectangle patchPlace;

      /**
       * dimension of the linear filter used to identify the object
       * (in pixel) This is the number of pixels (on a line
       * perpendicular to the actual snake) that will be considered to
       * evaluate if the active contour should be moved or not.
       *
       * Default value: 5
       */
      int lfilter;

      /**
       * average
       *
       * These are the threshold values used to determine if a new pixel belongs
       * to the outer region or not.  The values for each color channel should be
       * between 0.0f and 255.0f
       *
       * Default value: frgbPixel(10.f,10.f,10.f)
       */
      frgbPixel average;

      /**
       * variance
       *
       * These are the threshold values used to determine if a new pixel belongs
       * to the outer region or not.  The values for each color channel should be
       * between 0.0f and 255.0f
       *
       * Default value: frgbPixel(10.f,10.f,10.f)
       */
      frgbPixel variance;

      /**
       * Allow the snake to split into two independent contours.
       *
       * Default value: false
       */
      bool splitting;


    };

    /**
     * default constructor
     */
    snake();

    /**
     * copy constructor
     */
    snake(const snake& other)
      : segmentation() {copy(other);}

    /**
     * destructor
     */
    ~snake();

    /**
     * copy data of "other" functor
     */
    snake& copy(const snake& other);

    /**
     * clone member
     */
    virtual functor* clone() const;

    /**
     * OnCopy apply
     */
    bool apply(const image& imgIn,areaPoints& newAreaPoints);

    /**
     * get parameters
     */
    const parameters& getParameters() const;

    /**
     * get parameters
     */
    parameters& getParameters();

    /**
     * Structure for a snake element
     */
    class element : public ioObject {
    public:
      /**
       * chain code for the next element
       */
      chainCode next;

      /**
       * chain code for the previous element
       */
      chainCode prev;

      /**
       * flag to indicate if the element might be moved or not
       */
      bool fixed;

      /**
       * default constructor makes all elements moveable
       */
      element() : fixed(false) {};

      /**
       * default constructor makes all elements moveable
       */
      element(const int& i) : fixed(false) {};

      // just required to instantiate an lti::matrix
      bool operator==(const element& other) const {
        return this == &other;
      }

      operator int() {
        return static_cast<int>(next.getDirection());
      }

      operator ubyte() {
        return static_cast<ubyte>(next.getDirection());
      }

      operator const int() const {
        return static_cast<int>(next.getDirection());
      }

      operator const ubyte() const {
        return static_cast<ubyte>(next.getDirection());
      }

      /** @name Storable interface
       *  Members for the storable interface
       */
      //@{
      /**
       * read the matrix from the given ioHandler.  The complete flag indicates
       * if the enclosing begin and end should be also be readed
       */
      virtual bool read(ioHandler& handler,const bool complete=true);
      
      /**
       * write the matrix in the given ioHandler.  The complete flag indicates
       * if the enclosing begin and end should be also be written or not
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;
      //@}

    };

  protected:
    /**
     * set a snake
     * The firstSnake must contain a valid borderPoints list!!
     */
    void setSnake(const borderPoints& aSnake);

    /** uses two chainCodes in the order prev,next to check if
      	there is an object left or right of the element */
    inline bool objectRight(const chainCode& prev,
			    const chainCode& next) const;

    /** returns the snake as areaPoints */
    bool getContour(areaPoints& newAreaPoints);

    // here is valid to use a lti::vector, because no dynamic memory
    // allocation is done by "element"!
    genericMatrix<element> snakeField; // Snake

    /**
     * image rows and columns
     */
    point size;

    /**
     * pointer to the image
     */
    const image* img;

    /**
     *
     */
    pointList anchorPoint;

  private:
    /**
     * Set reference snake
     */
    void initReference(borderPoints& colorReference);

    /**
     * Set object snake without the possible reference-color-patch
     */
    void initObject(borderPoints& firstSnake);

    /**
     * Let the snake find an object
     */
    void findObjects();

    /**
     * Move one snake element.
     */
    bool move(point pos);

    /**
     * Check if the element at 'p' is free, pos is the asking element
     */
    bool freeAndAllowed(const point pos, const point p);

    /**
     * Delete snake if it consists of two elements
     */
    bool AreLastTwo(point& pos, pointList::iterator& actAnchor);

    /**
     * split snake if it has contact to itself
     */
    bool TryToSplit(point& pos, pointList::iterator& actAnchor);

  };

}
#endif

