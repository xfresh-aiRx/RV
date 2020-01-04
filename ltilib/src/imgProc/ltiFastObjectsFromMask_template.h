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
 * file .......: ltiFastObjectsFromMask_template.h
 * authors ....: Thorsten Dick
 * organization: LTI, RWTH Aachen
 * creation ...: 01.02.2004
 * revisions ..: $Id: ltiFastObjectsFromMask_template.h,v 1.12 2005/06/16 12:35:09 doerfler Exp $
 */

#ifndef _LTI_FAST_OBJECTS_FROM_MASK_TEMPLATE_H_
#define _LTI_FAST_OBJECTS_FROM_MASK_TEMPLATE_H_

#include "ltiContour.h"
#include "ltiMatrix.h"
#include <list>

#include <iostream>
#include "ltiFunctor.h"

namespace lti {

  /** 
   * The ltiFastObjectsFromMask-class is a simple and therefore faster version
   * of ltiObjectsFromMask. You can only specify:
   * <ol>
   *   <li> the threshold that determines what is to be regarded as background
   *        (if pixel value is < threshold)
   *   <li> the minimum number of border points that a blob must have in order
   *        to be extracted
   * </ol>
   * This class has been implemented as a template, so you can use it for
   * every image type T you like, as long as you:
   * <ol>
   *   <li> provide a member-variable "value_type" (to specify the type of a
   *        single pixel) along with the image type T
   *   <li> provide a < Operator for "value_type" of T
   * </ol>
   * Fortunately, for most lti classes (like channel, channel8, image) both
   * was already done.
   */

  template <class T> 
  class fastObjectsFromMask : public functor {
  public:

    // ########################## parameters - definitions ###################

    /**
     * The parameters of the ltiFastObjectsFromMask-class:
     * <ol>
     *   <li> the threshold that determines what is to be regarded as
     *        background (if pixel value is < threshold)
     *   <li> the minimum number of border points that a blob must have in
     *        order to be extracted
     * </ol>
     * Remark: all parameter methods are inline due to some MSVC++ 6.0 bug
     * (nested classes inside templates)
     */
    class parameters : public functor::parameters {
    public:
      
      /**
       * Value Type of the template type used.
       */
      typedef typename T::value_type value_type;

      /**
       * default constructor <BR>
       * Note that no default value for threshold can be guaranteed, therefore
       * always set it manually before first usage. (Which you would do 
       * anyway, I suppose... ;-)
       */
      // inline
      parameters() : functor::parameters() { 
        threshold       = value_type(); // call default constructor
        minBorderPoints = int (0);
      }

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      // inline
      parameters(const parameters& other) {
        copy(other); 
      }

      /**
       * destructor
       */
      // inline
      ~parameters() { };

      /**
       * returns name of this type
       */
      // inline
      const char* getTypeName() const {
        return "fastObjectsFromMask<T>::parameters";
      };

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      // inline
      parameters& copy(const parameters& other) {
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
        threshold       = other.threshold;
        minBorderPoints = other.minBorderPoints;
        return *this;
      };
   
      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      // inline
      parameters& operator=(const parameters& other) {
        return copy(other);
      }

      /**
       * returns a pointer to a clone of the parameters
       */
      // inline
      virtual functor::parameters* clone() const {
        return new parameters(*this);
      }

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was succeful
       */

# ifndef _LTI_MSC_6
      virtual bool write (ioHandler& handler, const bool complete=true) const
# else
        bool writeMS (ioHandler& handler, const bool complete=true) const 
# endif
      {
        bool b = true;
        if (complete) {
          b = handler.writeBegin();
        }
        if (b) {
          lti::write(handler,"threshold", threshold);
          lti::write(handler,"minBorderPoints", minBorderPoints);
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
      virtual bool write (ioHandler& handler, const bool complete=true) const{
      // ...we need this workaround to cope with another really awful MSVC bug.
        return writeMS (handler,complete);
      }
# endif
      
      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also read, otherwise only the data block will be read.
       * @return true if read was succeful
       */

# ifndef _LTI_MSC_6
      virtual bool read(ioHandler& handler, const bool complete=true) 
# else
        bool readMS(ioHandler& handler, const bool complete=true)
# endif
      {
        bool b = true;
        if (complete) {
          b = handler.readBegin();
        }
        if (b) {
          lti::read(handler,"threshold", threshold);
          lti::read(handler,"minBorderPoints", minBorderPoints);
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
      virtual bool read (ioHandler& handler, const bool complete=true) {
        // ...we need this workaround to cope with another really awful MSVC
        // bug.
        return readMS (handler,complete);
      }
# endif

      // ------------------------the parameters ------------------------------

      /**
       * A pixel with value < this threshold is not regarded as a blob's
       * border. Remember to provide a "<"-operator as well as "value_type"
       * for the type of the pixels if you use exotic image types.
       */
      value_type threshold;

      /**
       * The minimum number of border points for all blobs to be extracted.
       */
      int minBorderPoints;

    };

    // ################## fastObjectsFromMask - public definitions ###########

    /**
     * default constructor
     */
    fastObjectsFromMask();

    /**
     * Construct a functor using the given parameters
     */
    fastObjectsFromMask(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    fastObjectsFromMask(const fastObjectsFromMask& other);

    /**
     * destructor
     */
    virtual ~fastObjectsFromMask();

    /**
     * returns the name of this type ("fastObjectsFromMask")
     */
    virtual const char* getTypeName() const;
  
    /**
     * @param input image with the source data
     * @param blobList list of extracted blobs (will be cleared before usage)
     * @return true if apply was successful
     */
    bool apply(const T &input, std::list<borderPoints> &blobList); 

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    fastObjectsFromMask& copy(const fastObjectsFromMask& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    fastObjectsFromMask& operator=(const fastObjectsFromMask& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const typename fastObjectsFromMask::parameters& getParameters() const;

    // ############### fastObjectsFromMask - protected definitions ###########
    
  protected:
    // ------------ variables -------------
    point        pixel;
    borderPoints pixelList;
    matrix<int>  helperMatrix;
    bool insideBlob;
    int  borderCounter;
    int  matrixRows;
    int  matrixCols;
    int  row;
    int  col;
    int  currRow;
    int  currCol;
    int  nextRow;
    int  nextCol;
    int  lastDirection;
    int  nextDirection;
  
    // ------------ methods ---------------
    // protected method (with side-effects)
    bool setAndTestNext ();

    // ------------ data ------------------

    // protected, look-up-table
    static int lookUpTableDirection[8];

    // protected, look-up-table
    static int lookUpTableInOut[8][8];

    // protected, look-up-table
    static int lookUpTableBorderTypeUpdate[6][6];

    // protected, look-up-table
    static int lookUpTableIncModulo[8];

    // protected, internal constants
    enum eBorderTypes { 
      eERR=1, eB, eIN, eOUT, eIO };
    
    // protected, internal constants
    enum eDirections { 
      eNW=0, eN, eNE, eE, eSE, eS, eSW, eW };
  };

  // #########################################################################
  // Implementation (in header file because this is a template)
  // #########################################################################

  // ################### fastObjectsFromMask - implementation ################

  // default constructor
  template <class T>
  fastObjectsFromMask<T>::fastObjectsFromMask()
    : functor() {
    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  template <class T> 
  fastObjectsFromMask<T>::fastObjectsFromMask(const parameters& par)
    : functor() {
    // set the given parameters
    setParameters(par);
  }

  // copy constructor
  template <class T>
  fastObjectsFromMask<T>::fastObjectsFromMask(const fastObjectsFromMask<T>& 
                                                    other) {
    copy(other);
  }

  // destructor
  template <class T> 
  fastObjectsFromMask<T>::~fastObjectsFromMask() {
  }

  // returns the name of this type
  template <class T> 
  const char* fastObjectsFromMask<T>::getTypeName() const {
    return "fastObjectsFromMask";
  }

  // copy member
  template <class T> 
  fastObjectsFromMask<T>&
  fastObjectsFromMask<T>::copy(const fastObjectsFromMask<T>& other) {
    functor::copy(other);
    return (*this);
  }

  // alias for copy member
  template <class T> 
  fastObjectsFromMask<T>&
  fastObjectsFromMask<T>::operator=(const fastObjectsFromMask<T>& other) {
    return (copy(other));
  }

  // clone member
  template <class T>
  functor* fastObjectsFromMask<T>::clone() const {
    return new fastObjectsFromMask<T>(*this);
  }

  // return parameters
  template <class T> 
  const typename fastObjectsFromMask<T>::parameters&
  fastObjectsFromMask<T>::getParameters() const {
    const parameters* par = 
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException();
    }
    return *par;
  }

  // ------------------------------ apply ------------------------------------
  
  template <class T> 
  bool fastObjectsFromMask<T>::apply(const T &input, 
                                     std::list<borderPoints> &blobList) {
    // initialize
    const parameters& params = getParameters ();
    blobList.clear ();
    matrixRows = input.rows ();
    matrixCols = input.columns ();
    helperMatrix = matrix<int> (matrixRows, matrixCols, 0);

    // loop over all pixels
    for (row= 0; row< matrixRows; ++row) {
      insideBlob= false;
      for (col= 0; col< matrixCols; ++col) {
        // if inside an already detected blob: check if end is reached
        if (insideBlob) {
          if (helperMatrix.at(row, col) >= eOUT)
            insideBlob = false;
          continue;
        }
        // check if the current pixel is not to be extracted
        if (input.at(row, col) < params.threshold)
          continue;
        // check if an already detected blob is reached
        if (helperMatrix.at(row, col) == eIN) {
          insideBlob = true;
          continue;
        }
        // check if an already detected blob is touched
        if (helperMatrix.at(row, col) == eIO)
          continue;
    
        /////////////////////////////////////////////////////////////////
        // extract the blob that belongs to the current pixel (row,col)
        // 1) check if current pixel is alone
        currRow= row;
        currCol= col;
        for (nextDirection= eE; nextDirection< eW; ++nextDirection) {
          if (setAndTestNext()) // set (nextRow, nextCol)
            if (! (input.at(nextRow, nextCol) < params.threshold))
              break;
        }
        // 1) (i) current pixel is alone
        if (nextDirection == eW) { 
          helperMatrix.at (row, col) = eIO;
          if ( params.minBorderPoints <= 1 ) {
            pixel.y = row;
            pixel.x = col;
            pixelList.clear ();
            pixelList.push_back (pixel);
            blobList.push_back (pixelList);
          }
          continue;
        }
        // 1) (ii) current pixel is not alone: (nextRow, nextCol)
        if (nextDirection == eE) {
          helperMatrix.at (row, col) = eIN;
          insideBlob = true;
        } else 
          helperMatrix.at (row, col) = eIO;
        pixel.y = row;
        pixel.x = col;
        pixelList.clear ();
        pixelList.push_back (pixel);
        borderCounter = 1;
        // 2) detect remaining border points
        while (!(nextRow==row && nextCol==col)) {
          currRow= nextRow;
          currCol= nextCol;
          lastDirection= nextDirection;
          // find next border point from (currRow,currCol) on
          for (nextDirection  = lookUpTableDirection[lastDirection]; ;
               nextDirection  = lookUpTableIncModulo[nextDirection]) {
            if (setAndTestNext()) // set (nextRow, nextCol)
              if (! (input.at(nextRow, nextCol) < params.threshold) )
                break;
          }
          // (nextRow,nextCol) is next border point
          ++borderCounter;
          // set border type for (currRow,currCol) with knowledge of
          // pre- and post-pixel
          helperMatrix.at (currRow, currCol) = 
            lookUpTableBorderTypeUpdate[helperMatrix.at(currRow, currCol)]
            [lookUpTableInOut[lastDirection][nextDirection]];
          pixel.y = currRow;
          pixel.x = currCol;
          pixelList.push_back (pixel);
        } // end of while-loop
        // 3) evaluate detected blob
        if (borderCounter >= params.minBorderPoints) 
          blobList.push_back (pixelList);
        /////////////////////////////////////////////////////////////////
      }
    }
    return true;
  }
  
  // ----------------------------- protected ---------------------------------

  // initialize static look-up-tables:

  // The eight directions are numbered clockwise from 0 for NW to 7 for W.
  // This look-up-table says at position i, in which direction to start 
  // searching (clockwise around the current border point) for the next 
  // neighbour of the current border point, if the direction from 
  // where it was reached is i.
  template <class T>
  int fastObjectsFromMask<T>::lookUpTableDirection[8] = { 
    6, 0, 0, 2, 2, 4, 4, 6 };

  // This look-up-table stores at position (i,j) the border point type of a
  // pixel that was reachead from direction i and leads to the next border
  // point in direction j.
  // Remember: eERR = 1 (error), 
  //           eB   = 2 (border point with neighbours in W and E)
  //           eIN  = 3 (border point that leads into the blob if passed from
  //                     W to E)
  //           eOUT = 4 (border point that leads out of the blob if passed
  //                     from W to E)
  //           eIO  = 5 (border point that leads in and out (touching the
  //                     blob) if passed from W to E)
  template <class T>
  int fastObjectsFromMask<T>::lookUpTableInOut[8][8] = {
    { 3, 3, 3, 3, 5, 1, 2, 2 },
    { 3, 3, 3, 3, 5, 5, 1, 1 },
    { 3, 3, 3, 3, 5, 5, 5, 1 },
    { 1, 1, 2, 2, 4, 4, 4, 4 },
    { 5, 1, 2, 2, 4, 4, 4, 4 },
    { 5, 5, 1, 1, 4, 4, 4, 4 },
    { 5, 5, 5, 1, 4, 4, 4, 4 },
    { 3, 3, 3, 3, 1, 1, 3, 2 } };
    
  // This look-up-table stores at position (i,j) the value that has to be
  // reminded in the helper matrix at position (x,y) for the currently
  // handled border point (x,y) (i.e. the border point type, cf.
  // lookUpTableInOut) with:
  // - i corresponds to the current border point type of (x,y) (according to
  //     lookUpTableInOut, zero if not yet visited) without consideration of
  //     the just found predecessor/successor
  //     neighbours
  // - j corresponds to the border point type according to the just found
  //     predecessor/successor neighbours
  template <class T>
  int fastObjectsFromMask<T>::lookUpTableBorderTypeUpdate[6][6] = {
    { 1, 1, 2, 3, 4, 5 },
    { 1, 1, 1, 1, 1, 1 },
    { 1, 1, 2, 3, 4, 5 },
    { 1, 1, 3, 3, 5, 5 },
    { 1, 1, 4, 5, 4, 5 },
    { 1, 1, 5, 5, 5, 5 } };
  
  // Just a modulo 8 look-up-table
  template <class T>
  int fastObjectsFromMask<T>::lookUpTableIncModulo[8] = { 
    1, 2, 3, 4, 5, 6, 7, 0 };
    
  // For reasons of performance this protected method has a SIDE-EFFECT:
  // Set (nextRow,nextCol) to be the pixel in direction nextDirection
  // from current pixel (currRow, currCol)
  template <class T>
  bool fastObjectsFromMask<T>::setAndTestNext () {
    switch (nextDirection) {
      case eNW: 
        nextRow= currRow-1; 
        nextCol= currCol-1;
        if (nextRow< 0 || nextCol< 0) return false; 
        break;
      case eN : 
        nextRow= currRow-1; 
        nextCol= currCol;
        if (nextRow< 0) return false; 
        break;
      case eNE: 
        nextRow= currRow-1; 
        nextCol= currCol+1;
        if (nextRow< 0 || nextCol>= matrixCols) return false; 
        break;
      case eE : 
        nextRow= currRow; 
        nextCol= currCol+1;
        if (nextCol>= matrixCols) return false; 
        break;
      case eSE: 
        nextRow= currRow+1; 
        nextCol= currCol+1;
        if (nextRow>= matrixRows || nextCol>= matrixCols) return false;
        break;
      case eS : 
        nextRow= currRow+1;
        nextCol= currCol;
        if (nextRow>= matrixRows) return false;
        break;
      case eSW: 
        nextRow= currRow+1;
        nextCol= currCol-1;
        if (nextRow>= matrixRows || nextCol< 0) return false;
        break;
      case eW :
        nextRow= currRow;
        nextCol= currCol-1;
        if (nextCol< 0) return false;
        break;
      default:
        return false;
        break;
    }
    return true;
  }

}

#endif
