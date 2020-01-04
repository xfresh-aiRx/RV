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
 * file .......: ltiImage.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 09.04.99
 * revisions ..: $Id: ltiImage.h,v 1.19 2004/01/11 10:33:07 alvarado Exp $
 */

#ifndef _LTI_IMAGE_H_
#define _LTI_IMAGE_H_

#include "ltiMatrix.h"
#include "ltiTypes.h"
#include "ltiRGBPixel.h"

namespace lti {
  class channel8;
  class channel;

  /**
   * The one and only RGB-image format.
   *
   * This class is an specialization of a matrix of lti::rgbPixel.
   *
   * The concept for color images and gray valued images in the LTI-Lib is
   * simple:  they are specializations of the class lti::matrix.
   *
   * Several aspects must however be clarified.  The rows of the matrix will
   * represent horizontal lines in the image, and the columns vertical ones.
   * The row with index zero will be the row at the top of the image.  The
   * column with row zero is the one at the left of the image.  This means
   * that the used coordinate system for the position of a pixel is
   * "left-handed":  the origin is situated at the top-left corner of the
   * image, the x-coordinate gives the position in the horizontal axis and
   * the y-coordinate the position in the vertical axis.  With other words,
   * the y coordinate give the row and x the column of the matrix.
   * This fact is important to remember when accessing the image elements:
   *
   * \code
   * image img; // our image
   * if (img.at(y,x) == img[y][x] == img.at(point(x,y))) {
   *   cout << "This is always true!";
   * } else {
   *   cout << "ERROR: it's imposible to come here";
   *   exit 1;
   * }
   * \endcode
   *
   * The gray valued channels lti::channel and lti::channel8 differ on
   * the type and valid value ranges of their elements.  The former
   * accepts floating point values, with a default value range from
   * 0.0 to 1.0.  Many algorithms produce other values with specific
   * meanings like angles or gradients, but using the default range you can
   * assume 0.0 as a representation for black and 1.0 for white.
   *
   * The lti::channel8 is a much smaller representation but allows
   * only integer values between 0 and 255, fact that can be advantageous
   * in many algorithms.  Here 0 usually means black and 255 white.
   *
   * The lti::image as lti::matrix<lti::rgbPixel> allows the representation of
   * true-color images, i.e. images with pixels that can be chosen from a
   * palette of more than 16 million colors.
   *
   * @see lti::matrix for a reference to all inherited methods.
   *
   * @ingroup gAggregate
   * @ingroup gColor
   * @ingroup gImageProcessing
   */
  class image : public matrix<rgbPixel> {
  public:
    /**
     * default constructor creates an empty image
     */
    image();

    /**
     * this constructor creates a connected <code>rows x cols</code> image
     * and initializes all elements with <code>iniValue</code>
     * @param rows number of rows of the image
     * @param cols number of columns of the image
     * @param iniValue all elements will be initialized with this value
     */
    image(const int& rows,const int& cols,
          const rgbPixel& iniValue = rgbPixel());

    /**
     * this constructor creates a connected <code>rows x cols</code> image
     * and initializes all elements with the data pointed by
     * <code>data</code>.  The first <code>cols</code>-elements of the data
     * will be copied on the first row, the next ones on the second row and
     * so on.
     * @param rows number of rows of the image
     * @param cols number of columns of the image
     * @param data pointer to the memory block with the data to be initialized
     * with.
     */
    image(const int& rows,const int& cols,const rgbPixel data[]);

    /**
     * this constructor creates a connected <code>size.y x size.x</code>
     * image and initializes all elements with <code>iniValue</code>
     * @param size lti::point with the size of the image
     *            (size.x is the number of columns and
     *             size.y the number of rows)
     * @param iniValue all elements will be initialized with this value
     */
    image(const ipoint& size,const rgbPixel& iniValue = rgbPixel());

    /**
     * copy constructor.
     *
     * create this image as a connected copy of another image
     * for this const version, the data will be always copied!
     * It is also possible to create a copy of a subimage of another image.
     *
     * @param other   the image to be copied.
     * @param fromRow initial row of the other image to be copied
     * @param toRow   last row to be copied of the other image
     * @param fromCol initial column of the other image to be copied
     * @param toCol   last column to be copied of the other image
     *
     * Example:
     * \code
     * lti::image m(4,6,0); // image with 24 elements
     * // ...
     * // initialize image with:
     * //        0  1  2  3  4  5
     * //        2  1  5  4  0  3
     * //        1  2  1  2  3  2
     * //        3  3  2  1  2  3
     *
     * lti::image sm(m,1,3,0,2)  // this line will lead to the
     * //                             following contents for sm:
     * //        1  2  3
     * //        1  5  4
     * //        2  1  2
     * \endcode
     *
     */
    image(const image& other,
          const int& fromRow=0,const int& toRow=MaxInt32,
          const int& fromCol=0,const int& toCol=MaxInt32);

    /**
     * copy constructor (reference to a subimage).
     *
     * creates subimage of another image.
     *
     * if <code>copyData == true</code>, the new object has its own data
     * (equivalent to previous copy constructor).
     *
     * if <code>copyData == false</code>, the new object has references to
     * the other image, which means that the data is not necessarily
     * consecutive.  (This will not be a connected but a lined image)
     *
     * Those algorithms which use direct access to the image memory block
     * should check first if the memory lies in a consecutive block!
     * (see getMode())
     *
     * @param copyData should the data of the other image be copied or not
     * @param other   the image with the original data
     * @param fromRow initial row of the other image to be copied
     * @param toRow   last row to be copied of the other image
     * @param fromCol initial column of the other image to be copied
     * @param toCol   last column to be copied of the other image
     */
    image(const bool& copyData, image& other,
          const int& fromRow=0,const int& toRow=MaxInt32,
          const int& fromCol=0,const int& toCol=MaxInt32);

    /**
     * If \a init is true this constructor is equivalent to calling
     * image(const int& rows, const int& cols), and thus initializing
     * all elements with T(). However, in some cases the elements need
     * not be initialized during construction, since complex
     * initializion is required. Especially for large matrices, the
     * unnecessary constructor initialization is very time consuming.
     *
     * If \a init is false, memory is allocated but no initialization
     * takes place. Thus the following is equivalent:
     * \code
     * image a(false,100,100);
     *
     * image a;
     * a.resize(100,100,0,false,false);
     * \endcode
     *
     * @param init initialize image or not
     * @param rows number of rows of the image
     * @param cols number of columns of the image
     */
    image(const bool& init, const int& rows, const int& cols);

    /**
     * If \a init is true this constructor is equivalent to calling
     * image(const point& size), and thus initializing
     * all elements with T(). However, in some cases the elements need
     * not be initialized during construction, since complex
     * initializion is required. Especially for large matrices, the
     * unnecessary constructor initialization is very time consuming.
     *
     * If \a init is false, memory is allocated but no initialization
     * takes place. Thus the following is equivalent:
     * \code
     * image a(false,point(100,100));
     *
     * image a;
     * a.resize(100,100,0,false,false);
     * \endcode
     *
     * @param init initialize image or not
     * @param size new size
     */
    image(const bool& init, const ipoint& size);

    /**
     * create a clone of this image
     * @return a pointer to a copy of this image
     */
    virtual mathObject* clone() const;

    /**
     * return the name of this type
     */
    virtual const char* getTypeName() const;


    /**
     * cast from the <code>other</code> channel8.
     * For the transformation it assumes the channel8 as a gray valued
     * channel where 0 means black and 255 means white.
     *
     * @param other the channel8 to be casted
     * @return a reference to this image
     * Example:
     * \code
     *   lti::channel8 matA(10,10,255); // a channel8
     *   lti::image  matB;              // an image
     *
     *   matB.castFrom(matA);         // this will copy matA in matB!!
     *                                // and all elements will have
     *                                // rgbPixel(255,255,255)
     * \endcode
     */
    image& castFrom(const channel8& other);

    /**
     * cast from the <code>other</code> channel.
     * For the transformation it assumes the channel as a gray valued
     * channel where 0 means black and 1.0f means white.  All other
     * values will be clipped (less than zero to zero and more than 1.0 to 1.0)
     *
     * @param other the channel8 to be casted
     * @param minToBlack if minToBlack is true, a linear gray-valued
     * tranformation will be applied, which maps the minimal value in
     * the channel to (0,0,0).  If false, the value zero will be mapped
     * to zero.
     * @param maxToWhite if maxToWhite is true, a linear gray-valued
     * transformation will be applied, which maps the maximal value in
     * the channel to (255,255,255).  If false, the value 1.0f will be
     * mapped to 255.
     * @return a reference to this image
     * Example:
     * \code
     *   lti::channel matA(10,10,1.0f); // a channel
     *   lti::image  matB;             // an image
     *
     *   matB.castFrom(matA);         // this will copy matA in matB!!
     *                                // and all elements will have
     *                                // rgbPixel(255,255,255)
     * \endcode
     */
    image& castFrom(const channel& other,
                    const bool minToBlack = false,
                    const bool maxToWhite = false);

  };

  /**
   * a format for float channels.
   *
   * This class is identical to a matrix of floats except for the method
   * castFrom(channel8).
   *
   * The typical value range is between 0.0f and 1.0f (see lti::image for more
   * information).
   *
   * @see lti::image, lti::channel8
   *
   * @ingroup gAggregate
   * @ingroup gImageProcessing
   */
  class channel : public matrix<float> {
  public:
    /**
     * default constructor creates an empty channel
     */
    channel();

    /**
     * this constructor creates a connected <code>rows x cols</code> channel
     * and initializes all elements with <code>iniValue</code>
     * @param rows number of rows of the channel
     * @param cols number of columns of the channel
     * @param iniValue all elements will be initialized with this value
     */
    channel(const int& rows,const int& cols,const float& iniValue = float());

    /**
     * this constructor creates a connected <code>rows x cols</code> channel
     * and initializes all elements with the data pointed by
     * <code>data</code>.  The first <code>cols</code>-elements of the data
     * will be copied on the first row, the next ones on the second row and
     * so on.
     * @param rows number of rows of the channel
     * @param cols number of columns of the channel
     * @param data pointer to the memory block with the data to be initialized
     * with.
     */
    channel(const int& rows,const int& cols,const float data[]);

    /**
     * this constructor creates a connected <code>size.y x size.x</code>
     * channel and initializes all elements with <code>iniValue</code>
     * @param size lti::point with the size of the channel
     *             (size.x is the number of columns and
     *              size.y the number of rows)
     * @param iniValue all elements will be initialized with this value
     */
    channel(const ipoint& size,const float& iniValue = float());

    /**
     * copy constructor.
     *
     * create this channel as a connected copy of another channel
     * for this const version, the data will be always copied!
     * It is also possible to create a copy of a subchannel of another channel.
     *
     * @param other   the channel to be copied.
     * @param fromRow initial row of the other channel to be copied
     * @param toRow   last row to be copied of the other channel
     * @param fromCol initial column of the other channel to be copied
     * @param toCol   last column to be copied of the other channel
     *
     * Example:
     * \code
     * lti::channel m(4,6,0); // channel with 24 elements
     * // ...
     * // initialize channel with:
     * //        0  1  2  3  4  5
     * //        2  1  5  4  0  3
     * //        1  2  1  2  3  2
     * //        3  3  2  1  2  3
     *
     * lti::channel sm(m,1,3,0,2)  // this line will lead to the
     * //                             following contents for sm:
     * //        1  2  3
     * //        1  5  4
     * //        2  1  2
     * \endcode
     *
     */
    channel(const channel& other,
            const int& fromRow=0,const int& toRow=MaxInt32,
            const int& fromCol=0,const int& toCol=MaxInt32);

    /**
     * copy constructor (reference to a subchannel).
     *
     * creates subchannel of another channel.
     *
     * if <code>copyData == true</code>, the new object has its own data
     * (equivalent to previous copy constructor).
     *
     * if <code>copyData == false</code>, the new object has references to
     * the other channel, which means that the data is not necessarily
     * consecutive.  (This will not be a connected but a lined channel)
     *
     * Those algorithms which use direct access to the channel memory block
     * should check first if the memory lies in a consecutive block!
     * (see getMode())
     *
     * @param copyData should the data of the other channel be copied or not
     * @param other the channel with the original data
     * @param fromRow initial row of the other channel to be copied
     * @param toRow   last row to be copied of the other channel
     * @param fromCol initial column of the other channel to be copied
     * @param toCol   last column to be copied of the other channel
     */
    channel(const bool& copyData, channel& other,
            const int& fromRow=0,const int& toRow=MaxInt32,
            const int& fromCol=0,const int& toCol=MaxInt32);

    /**
     * If \a init is true this constructor is equivalent to calling
     * channel(const int& rows, const int& cols), and thus initializing
     * all elements with T(). However, in some cases the elements need
     * not be initialized during construction, since complex
     * initializion is required. Especially for large matrices, the
     * unnecessary constructor initialization is very time consuming.
     *
     * If \a init is false, memory is allocated but no initialization
     * takes place. Thus the following is equivalent:
     * \code
     * channel a(false,100,100);
     *
     * channel a;
     * a.resize(100,100,0,false,false);
     * \endcode
     *
     * @param init initialize channel or not
     * @param rows number of rows of the channel
     * @param cols number of columns of the channel
     */
    channel(const bool& init, const int& rows, const int& cols);

    /**
     * If \a init is true this constructor is equivalent to calling
     * channel(const int& rows, const int& cols), and thus initializing
     * all elements with T(). However, in some cases the elements need
     * not be initialized during construction, since complex
     * initializion is required. Especially for large matrices, the
     * unnecessary constructor initialization is very time consuming.
     *
     * If \a init is false, memory is allocated but no initialization
     * takes place. Thus the following is equivalent:
     * \code
     * channel a(false,point(100,100));
     *
     * channel a;
     * a.resize(100,100,0,false,false);
     * \endcode
     *
     * @param init initialize channel or not
     * @param size new size for the matrix (size.x columns, size.y rows)
     */
    channel(const bool& init, const ipoint& size);

    /**
     * create a clone of this channel
     * @return a pointer to a copy of this matrix
     */
    virtual mathObject* clone() const;

    /**
     * return the name of this type
     */
    virtual const char* getTypeName() const;

    /**
     * copy the <code>other</code> channel8 by casting each of its elements.
     *
     * The elements of the channel8 will be also multiplied by 1/255.
     *
     * @param other the channel8 to be casted
     * @return a reference to this channel
     * Example:
     * \code
     *   lti::channel8 matA(10,10,255); // a channel8
     *   lti::channel  matB;            // a channel
     *
     *   matB.castFrom(matA);         // this will copy matA in matB!!
     *                                // and all elements will have 1.0f
     * \endcode
     */
    channel& castFrom(const channel8& other);

    /**
     * cast the image to an channel.
     * It extracts the intensity channel of the image, defined as
     * (R+G+B)/3, where R, G, and B are the red, green and blue components
     * of the pixel.
     *
     * The elements of the resulting channel will be between 0.0f (black) and
     * 1.0f (white).
     *
     * @param other the image to be casted
     * @return a reference to this channel
     */
    channel& castFrom(const image& other);

    /**
     * Apply a gray valued transformation which maps the given intervall to
     * [0.0,1.0] (default) or the explicitly given "destination" interval
     * @param minVal the lower limit of the original data interval
     * @param maxVal the higher limit of the original data interval
     * @param minDest the lower limit of the mapped interval (default 0.0f)
     * @param maxDest the higher limit of the mapped interval (default 1.0f)
     * @return a reference to this object
     *
     * For example, if you want to map the interval [-1.0f,2.0f] to
     * the "usual" interval [0.0,1.0] just use one of following methods:
     *
     * \code
     * lti::channel chnl;
     * // ...
     * chnl.mapLinear(-1.0f,2.0f,0.0,1.0); // map [-1,2] to  [0,1]
     * // this is equivalent to (due to default "destination" interval)
     * chnl.mapLinear(-1.0f,2.0f);
     * \endcode
     *
     * Not that you can use this method to "invert" your gray values with
     * \code
     * chnl.mapLinear(0.0f,1.0f,1,0f,0.0f); // map [0,1] to  [1,0]
     * // this is equivalent to (due to default "destination" interval)
     * chnl.mapLinear(1.0f,0.0f);
     * \endcode
     *
     */
    channel& mapLinear(const float& minVal, const float& maxVal,
                       const float& minDest=0.0f, const float& maxDest=1.0f);


    /**
     * Apply a gray valued transformation which maps the given
     * intervall of the other channel into [0.0,1.0] (default) or the
     * explicitly given "destination" interval in this channel.
     *
     * @param other the other channel which values are to be mapped into
     *              the new interval
     * @param minVal the lower limit of the original data interval
     * @param maxVal the higher limit of the original data interval
     * @param minDest the lower limit of the mapped interval (default 0.0f)
     * @param maxDest the higher limit of the mapped interval (default 1.0f)
     * @return a reference to this object
     *
     * For example, if you want to map the interval [-1.0f,2.0f] to
     * the "usual" interval [0.0,1.0] just use one of following methods:
     *
     * \code
     * lti::channel chnl;
     * // ...
     * chnl.mapLinear(-1.0f,2.0f,0.0,1.0); // map [-1,2] to  [0,1]
     * // this is equivalent to (due to default "destination" interval)
     * chnl.mapLinear(-1.0f,2.0f);
     * \endcode
     *
     * Not that you can use this method to "invert" your gray values with
     * \code
     * chnl.mapLinear(0.0f,1.0f,1,0f,0.0f); // map [0,1] to  [1,0]
     * // this is equivalent to (due to default "destination" interval)
     * chnl.mapLinear(1.0f,0.0f);
     * \endcode
     * */
    channel& mapLinear(const channel& other,
                       const float& minVal, const float& maxVal,
                       const float& minDest=0.0f, const float& maxDest=1.0f);

    /**
     * copy the <code>other</code> matrix by casting each of its elements
     * @param other The matrix to be casted
     * @return a reference to this channel
     */
    template<class U>
      channel& castFrom(const matrix<U>& other) {
      matrix<value_type>::castFrom(other);
      return *this;
    };
  };

  /**
   * a format for 8-bit-channels.
   *
   * This class is identical to a matrix of bytes except for the method
   * castFrom(channel)
   *
   * The typical value range is between 0 and 255 (see lti::image for more
   * information).
   *
   * @see lti::image, lti::channel
   *
   * @ingroup gAggregate
   * @ingroup gImageProcessing
   */
  class channel8 : public matrix<ubyte> {
  public:
    /**
     * default constructor creates an empty channel8
     */
    channel8();

    /**
     * this constructor creates a connected <code>rows x cols</code> Channel8
     * and initializes all elements with <code>iniValue</code>
     * @param rows number of rows of the channel8
     * @param cols number of columns of the channel8
     * @param iniValue all elements will be initialized with this value
     */
    channel8(const int& rows,const int& cols,const ubyte& iniValue = ubyte());

    /**
     * this constructor creates a connected <code>rows x cols</code> Channel8
     * and initializes all elements with the data pointed by
     * <code>data</code>.  The first <code>cols</code>-elements of the data
     * will be copied on the first row, the next ones on the second row and
     * so on.
     *
     * @param rows number of rows of the channel8
     * @param cols number of columns of the channel8
     * @param data pointer to the memory block with the data to be initialized
     *             with.
     */
    channel8(const int& rows,const int& cols,const ubyte data[]);

    /**
     * this constructor creates a connected <code>size.y x size.x</code>
     * Channel8 and initializes all elements with <code>iniValue</code>
     * @param size lti::point with the size of the channel8
     *             (size.x is the number of columns and
     *              size.y the number of rows)
     * @param iniValue all elements will be initialized with this value
     */
    channel8(const ipoint& size,const ubyte& iniValue = ubyte());

    /**
     * copy constructor.
     *
     * create this channel8 as a connected copy of another channel8
     * for this const version, the data will be always copied!
     * It is also possible to create a copy of a subchannel of another
     * channel.
     *
     * @param other the channel8 to be copied.
     * @param fromRow initial row of the other channel8 to be copied
     * @param toRow   last row to be copied of the other channel8
     * @param fromCol initial column of the other channel8 to be copied
     * @param toCol   last column to be copied of the other channel8
     *
     * Example:
     * \code
     * lti::channel8 m(4,6,0); // channel8 with 24 elements
     * // ...
     * // initialize channel8 with:
     * //        0  1  2  3  4  5
     * //        2  1  5  4  0  3
     * //        1  2  1  2  3  2
     * //        3  3  2  1  2  3
     *
     * lti::channel8 sm(m,1,3,0,2)  // last line will leat to
     * //                              following contents in sm:
     * //        1  2  3
     * //        1  5  4
     * //        2  1  2
     * \endcode
     *
     */
    channel8(const channel8& other,
             const int& fromRow=0,const int& toRow=MaxInt32,
             const int& fromCol=0,const int& toCol=MaxInt32);

    /**
     * copy constructor (reference to a subchannel8).
     *
     * creates subchannel8 of another channel8.
     *
     * if <code>copyData == true</code>, the new object has its own data
     * (equivalent to previous copy constructor).
     *
     * if <code>copyData == false</code>, the new object has references to
     * the other channel8, which means that the data is not necessarily
     * consecutive.  (This will not be a connected but a lined channel8)
     *
     * Those algorithms which use direct access to the channel8 memory block
     * should check first if the memory lies in a consecutive block!
     * (see getMode())
     *
     * @param copyData should the data of the other channel8 be copied or not
     * @param other   the channel with the original data
     * @param fromRow initial row of the other channel8 to be copied
     * @param toRow   last row to be copied of the other channel8
     * @param fromCol initial column of the other channel8 to be copied
     * @param toCol   last column to be copied of the other channel8
     */
    channel8(const bool& copyData, channel8& other,
             const int& fromRow=0,const int& toRow=MaxInt32,
             const int& fromCol=0,const int& toCol=MaxInt32);

    /**
     * If \a init is true this constructor is equivalent to calling
     * channel8(const int& rows, const int& cols), and thus initializing
     * all elements with T(). However, in some cases the elements need
     * not be initialized during construction, since complex
     * initializion is required. Especially for large matrices, the
     * unnecessary constructor initialization is very time consuming.
     *
     * If \a init is false, memory is allocated but no initialization
     * takes place. Thus the following is equivalent:
     * \code
     * channel8 a(false,100,100);
     *
     * channel8 a;
     * a.resize(100,100,0,false,false);
     * \endcode
     *
     * @param init initialize channel8 or not
     * @param rows number of rows of the channel8
     * @param cols number of columns of the channel8
     */
    channel8(const bool& init, const int& rows, const int& cols);

    /**
     * If \a init is true this constructor is equivalent to calling
     * channel(const int& rows, const int& cols), and thus initializing
     * all elements with T(). However, in some cases the elements need
     * not be initialized during construction, since complex
     * initializion is required. Especially for large matrices, the
     * unnecessary constructor initialization is very time consuming.
     *
     * If \a init is false, memory is allocated but no initialization
     * takes place. Thus the following is equivalent:
     * \code
     * channel a(false,point(100,100));
     *
     * channel a;
     * a.resize(100,100,0,false,false);
     * \endcode
     *
     * @param init initialize channel or not
     * @param size new size for the matrix (size.x columns, size.y rows)
     */
    channel8(const bool& init, const ipoint& size);

    /**
     * create a clone of this channel
     * @return a pointer to a copy of this matrix
     */
    virtual mathObject* clone() const;

    /**
     * return the name of this type
     */
    virtual const char* getTypeName() const;

    int sumOfElements() const;

    /**
     * copy the <code>other</code> channel by casting each of its elements.
     *
     * The elements of the channel will be multiplied by 255 if no
     * other %parameter but the channel is given..
     *
     * @param other the channel to be casted from
     *
     * @param minToBlack if minToBlack is true, a linear gray-valued
     * tranformation will be applied, which maps the minimal value in
     * the channel to zero.  If false, the value zero will be mapped
     * to zero.
     *
     * @param maxToWhite if maxToWhite is true, a linear gray-valued
     * transformation will be applied, which maps the maximal value in
     * the channel to 255.  If false, the value 1.0f will be mapped to
     * 255.
     *
     * @return a reference to this channel
     * Example:
     * \code
     *   lti::channel matA(10,10,1); // a channel
     *   lti::channel8  matB;          // a channel8
     *
     *   matB.castFrom(matA);         // this will copy matA in matB!!
     *                                // and all elements will have 255
     * \endcode */
    channel8& castFrom(const channel& other,
                       const bool minToBlack = false,
                       const bool maxToWhite = false);

    /**
     * cast the image to an channel8.
     * It extracts the intensity channel of the image, defined as
     * (R+G+B)/3, where R, G, and B are the red, green and blue components
     * of the pixel.
     *
     * The elements of the resulting channel will be between 0 (black) and
     * 255 (white).
     *
     * @param other the image to be casted
     * @return a reference to this channel
     */
    channel8& castFrom(const image& other);

    /**
     * copy the <code>other</code> matrix by casting each of its elements
     * @param other The matrix to be casted
     * @return a reference to this channel
     */
    template<class U>
      channel8& castFrom(const matrix<U>& other) {
      matrix<value_type>::castFrom(other);
      return *this;
    };

  };

  /**
   * a format for signed 32-bit-channels.
   *
   * This class is identical to a matrix of integers except for the method
   * castFrom(channel)
   *
   * @see lti::image
   *
   * @ingroup gAggregate
   */
  class channel32 : public matrix<int32> {
  public:
    /**
     * default constructor creates an empty channel32
     */
    channel32();

    /**
     * this constructor creates a connected <code>rows x cols</code> Channel32
     * and initializes all elements with <code>iniValue</code>
     * @param rows number of rows of the channel32
     * @param cols number of columns of the channel32
     * @param iniValue all elements will be initialized with this value
     */
    channel32(const int& rows,const int& cols,const int& iniValue = int());

    /**
     * this constructor creates a connected <code>rows x cols</code> Channel32
     * and initializes all elements with the data pointed by
     * <code>data</code>.  The first <code>cols</code>-elements of the data
     * will be copied on the first row, the next ones on the second row and
     * so on.
     * @param rows number of rows of the channel32
     * @param cols number of columns of the channel32
     * @param data pointer to the memory block with the data to be initialized
     * with.
     */
    channel32(const int& rows,const int& cols,const int data[]);

    /**
     * this constructor creates a connected <code>size.y x size.x</code>
     * Channel32 and initializes all elements with <code>iniValue</code>
     * @param size lti::point with the size of the channel32
     *             (size.x is the number of columns and
     *              size.y the number of rows)
     * @param iniValue all elements will be initialized with this value
     */
    channel32(const ipoint& size,const int& iniValue = int());

    /**
     * copy constructor.
     *
     * create this channel32 as a connected copy of another channel32
     * for this const version, the data will be always copied!
     * It is also possible to create a copy of a subchannel of another
     * channel.
     *
     * @param other the channel32 to be copied.
     * @param fromRow initial row of the other channel32 to be copied
     * @param toRow   last row to be copied of the other channel32
     * @param fromCol initial column of the other channel32 to be copied
     * @param toCol   last column to be copied of the other channel32
     *
     * Example:
     * \code
     * lti::channel32 m(4,6,0); // integer channel32 with 25 elements
     * // ...
     * // initialize Channel32 with:
     * //        0  1  2  3  4  5
     * //        2  1  5  4  0  3
     * //        1  2  1  2  3  2
     * //        3  3  2  1  2  3
     *
     * lti::channel32<int> sm(m,1,3,0,2)  // last line will leat to
     * //                                 following contents in sm:
     * //        1  2  3
     * //        1  5  4
     * //        2  1  2
     * \endcode
     *
     */
    channel32(const channel32& other,
              const int& fromRow=0,const int& toRow=MaxInt32,
              const int& fromCol=0,const int& toCol=MaxInt32);

    /**
     * copy constructor (reference to a subchannel32).
     *
     * creates subchannel32 of another channel32.
     *
     * if <code>copyData == true</code>, the new object has its own data
     * (equivalent to previous copy constructor).
     *
     * if <code>copyData == false</code>, the new object has references to
     * the other channel32, which means that the data is not necessarily
     * consecutive.  (This will not be a connected but a lined channel32)
     *
     * Those algorithms which use direct access to the channel32 memory block
     * should check first if the memory lies in a consecutive block!
     * (see getMode())
     *
     * @param copyData should the data of the other channel32 be copied or not
     * @param other the channel with the original data
     * @param fromRow initial row of the other channel32 to be copied
     * @param toRow   last row to be copied of the other channel32
     * @param fromCol initial column of the other channel32 to be copied
     * @param toCol   last column to be copied of the other channel32
     */
    channel32(const bool& copyData, channel32& other,
             const int& fromRow=0,const int& toRow=MaxInt32,
             const int& fromCol=0,const int& toCol=MaxInt32);

    /**
     * If \a init is true this constructor is equivalent to calling
     * channel8(const int& rows, const int& cols), and thus initializing
     * all elements with T(). However, in some cases the elements need
     * not be initialized during construction, since complex
     * initializion is required. Especially for large matrices, the
     * unnecessary constructor initialization is very time consuming.
     *
     * If \a init is false, memory is allocated but no initialization
     * takes place. Thus the following is equivalent:
     * \code
     * channel8 a(false,100,100);
     *
     * channel8 a;
     * a.resize(100,100,0,false,false);
     * \endcode
     *
     * @param init initialize channel8 or not
     * @param rows number of rows of the channel8
     * @param cols number of columns of the channel8
     */
    channel32(const bool& init, const int& rows, const int& cols);

    /**
     * If \a init is true this constructor is equivalent to calling
     * channel(const int& rows, const int& cols), and thus initializing
     * all elements with T(). However, in some cases the elements need
     * not be initialized during construction, since complex
     * initializion is required. Especially for large matrices, the
     * unnecessary constructor initialization is very time consuming.
     *
     * If \a init is false, memory is allocated but no initialization
     * takes place. Thus the following is equivalent:
     * \code
     * channel a(false,point(100,100));
     *
     * channel a;
     * a.resize(100,100,0,false,false);
     * \endcode
     *
     * @param init initialize channel or not
     * @param size new size for the matrix (size.x columns, size.y rows)
     */
    channel32(const bool& init, const ipoint& size);

    /**
     * create a clone of this channel
     * @return a pointer to a copy of this matrix
     */
    virtual mathObject* clone() const;

    /**
     * return the name of this type
     */
    virtual const char* getTypeName() const;

    /**
     * copy the <code>other</code> matrix by casting each of its elements
     * @param other The matrix to be casted
     * @return a reference to this channel
     */
    template<class U>
      channel32& castFrom(const matrix<U>& other) {
      matrix<value_type>::castFrom(other);
      return *this;
    };
  };

  // =========================================
  // Define a palette type for indexed images
  // =========================================

  /**
   *  Vector of rgbPixel:  used as a color palette
   */
  typedef vector<rgbPixel> palette;

  /**
   *  An empty vector used to denote an empty palette
   */
  const palette emptyPalette;

}


#endif

