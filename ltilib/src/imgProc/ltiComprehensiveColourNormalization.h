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
 * file .......: lticomprehensiveColourNormalization.h
 * authors ....: Marius Wolf	
 * organization: LTI, RWTH Aachen
 * creation ...: 19.3.2003
 * revisions ..: $Id: ltiComprehensiveColourNormalization.h,v 1.11 2005/06/24 12:19:42 doerfler Exp $
 */

#ifndef _LTI_COMPREHENSIVECOLOURNORMALIZATION_H_
#define _LTI_COMPREHENSIVECOLOURNORMALIZATION_H_

#include "ltiColorNormalizationBase.h"

namespace lti {

  /**
   * This class implements the algorithm described in the paper
   * "Comprehensive Colour Image Normalization" by Finlayson, Schiele
   * and Crowley.
   *
   * The original paper can be found here:
   * http://web.media.mit.edu/~bernt/Pubs/eccv98.ps.gz
   * 
   * The algorithm can eliminate dependencies from lighting geometry and
   * illumination colour, normalizing both chromaticities and intensities of
   * each channel.
   */
  class comprehensiveColourNormalization : public colorNormalizationBase {
  public:
    /**
     * the parameters for the class comprehensiveColourNormalization
     */
    class parameters : public colorNormalizationBase::parameters {
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
       * Maximum number of iterations done for an image.
       *
       * Default is 5.
       */
      int maxSteps;

      /**
       * Maximum square distance between the pixels of i-th and (i+1)-th image.
       *
       * When the maximum square euclidian distance between two pixels
       * of consecutive iterations falls below this value the process
       * is stopped.
       *
       * Possible values are between 0.0 and 3.0. These extreme values
       * are not very useful, however.
       *
       * Default: 1.0
       */
      float maxDist;

    };

    /**
     * default constructor
     */
    comprehensiveColourNormalization();

    /**
     * Construct a functor using the given parameters
     */
    comprehensiveColourNormalization(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    comprehensiveColourNormalization(const comprehensiveColourNormalization& other);

    /**
     * destructor
     */
    virtual ~comprehensiveColourNormalization();

    /**
     * returns the name of this type ("comprehensiveColourNormalization")
     */
    virtual const char* getTypeName() const;
  
    /**
     * operates on the given %parameter.
     * @param srcdest image with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(image& srcdest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src image with the source data.
     * @param dest image where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const image& src,image& dest) const;
      
    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    comprehensiveColourNormalization& copy(const comprehensiveColourNormalization& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    comprehensiveColourNormalization& operator=(const comprehensiveColourNormalization& other);

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
     * One iteration of the comprehensive normalization from src to dest
     *
     * This method assumes that all channels are Connected 
     * (see matrix::getMode())
     * 
     * The square of the maximum distance between the src and the dest channels
     * will be left on \a dist
     */
    bool step(const channel& srcred,
              const channel& srcgreen,
              const channel& srcblue,
              channel& destred,
              channel& destgreen,
              channel& destblue,
              float& dist) const;

  };
}

#endif



