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
 * file .......: ltiBayerDemosaicing.h
 * authors ....: Arnd Hannemann 
 * organization: LTI, RWTH Aachen
 * creation ...: 28.10.2004
 * revisions ..: $Id: ltiBayerDemosaicing.h,v 1.2 2005/06/16 09:32:41 doerfler Exp $
 */

#ifndef _LTI_BAYER_DEMOSAICING_H_
#define _LTI_BAYER_DEMOSAICING_H_



#include "ltiMatrix.h"
#include "ltiVector.h"

#include "ltiTransform.h"

namespace lti {
  /**
   * This functor makes a color interpolation of common
   * BAYER Patterns used in digital cameras.
   */
  class bayerDemosaicing : public transform {
  public:
    
    /** enumeration of supported bayer patterns */
    enum eBayerPattern {
      RGGB
    };
    
    /** enumeration of supported interpolation methods */
    enum eBayerMethod {
      Simple,
      NearestNeighbor
    };


    /**
     * The parameters for the class bayerDemosaicing
     */
    class parameters : public transform::parameters {
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

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------
     

      /**
       * method used to interpolate unknown pixel values
       */
      eBayerMethod method;

      /**
       * pattern used in input data
       */
      eBayerPattern bayerPattern;

    };

    /**
     * Default constructor
     */
    bayerDemosaicing();

    /**
     * Construct a functor using the given parameters
     */
    bayerDemosaicing(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    bayerDemosaicing(const bayerDemosaicing& other);

    /**
     * Destructor
     */
    virtual ~bayerDemosaicing();

    /**
     * Returns the name of this type ("bayerDemosaicing")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on a copy of the given %parameters.
     * @param src matrix<ubyte> with the source data.
     * @param dest matrix<ubyte> where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<ubyte>& src,matrix<rgbPixel>& dest) const;


    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    bayerDemosaicing& copy(const bayerDemosaicing& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    bayerDemosaicing& operator=(const bayerDemosaicing& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

  protected:
    inline bool simpleDemosaicing(const matrix<ubyte>& src, 
                                  matrix<rgbPixel>& dest) const;

    inline bool nearestNeighborDemosaicing(const matrix<ubyte>& src,
                                           matrix<rgbPixel>& dest) const;


  };

  // global read and write functions for enumerations
  /**
   * read function for bayerDemosaicing::eBayerMethod
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler, bayerDemosaicing::eBayerMethod& data);

  /**
   * write function for bayerDemosaicing::eBayerMethod
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler, const bayerDemosaicing::eBayerMethod& data);

  /**
   * read function for bayerDemosaicing::eBayerPattern
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler, bayerDemosaicing::eBayerPattern& data);

  /**
   * write function for bayerDemosaicing::eBayerPattern
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler, const bayerDemosaicing::eBayerPattern& data);


}

#endif

