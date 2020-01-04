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
 * file .......: ltiDataTransformer.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 5.11.2002
 * revisions ..: $Id: ltiDataTransformer.h,v 1.6 2004/05/03 12:19:24 alvarado Exp $
 */

#ifndef _LTI_DATA_TRANSFORMER_H_
#define _LTI_DATA_TRANSFORMER_H_

#include "ltiFunctor.h"
#include "ltiTypes.h"
#include "ltiVector.h"

namespace lti {
  /**
   * This is the low-level base class for data transformers. Most notable
   * examples for such transformers are coding or encryption functors.
   * This an abstract class.
   */
  class dataTransformer : public functor {
  public:

    enum {
      notEnoughSpace=-1
    };

    /**
     * Type for the elements in a buffer must be restricted to
     * unsigned bytes.
     */
    typedef ubyte bufferElement;

    /**
     * The buffers with the data to be transformed (or the transformed data)
     * are manipulated in container instances of the buffer type.
     */
    class buffer : public vector<bufferElement> {
    public:
      buffer(const int& n) : vector<bufferElement>(n) {};
      buffer(const int& n, bufferElement data[], bool constRef)
        : vector<bufferElement>(n,data,constRef) {};

      /**
       * Save this buffer as a raw data block to the given file.
       */
      bool saveRaw(const std::string& name) const;

      /**
       * Load this buffer as a raw data block from the given file.
       * The buffer is resized to fit the size of the file.
       */
      bool loadRaw(const std::string& name);
    };

    /**
     * default constructor
     */
    dataTransformer();

    /**
     * Construct a functor using the given parameters
     */
    dataTransformer(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    dataTransformer(const dataTransformer& other);

    /**
     * destructor
     */
    virtual ~dataTransformer();

    /**
     * returns the name of this type ("dataTransformer")
     */
    virtual const char* getTypeName() const;

    /**
     * Operates on the given arguments.
     * @param srcdest pointer to an array of bufferElement. The input
     *        array must be allocated and have enough space for
     *        input as well as output data.
     * @param nsrc the number of input bytes that should be transformed.
     * @param ndest The caller must set this to the number of bytes
     *        of the buffer that can be used for the result.
     *        After returning, it will contain the number of bytes
     *        that are actually used.
     *        If buffer was too small, it will contain notEnoughSpace.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(bufferElement* srcdest, int nsrc, int& ndest) const=0;

    /**
     * Operates on the given argument.
     * @param srcdest vector with the input data which will
     *        also receive the transformed data. The
     *        vector will be resized to fit the transformed data.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(buffer& srcdest) const=0;

    /**
     * Operates on a copy of the given arguments.
     * @param src pointer to an array of bufferElement which contains the
     *            source data
     * @param nsrc the number of input bytes that should be transformed.
     * @param dest pointer to an array of bufferElement which will receive
     *             the transformed data. The array must be allocated
     *             by the caller.
     * @param ndest When called, must contain the size of the output
     *              buffer. When the function returns, it contains the
     *              number of bytes actually used, or -1 if the buffer
     *              was too small.
     *
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const bufferElement* src, int nsrc, bufferElement* dest, int& ndest) const=0;

    /**
     * Operates on the given argument.
     * @param src vector with the input data. All elements of the vector
     *        are transformed.
     * @param dest vector which will receive the transformed data. The
     *        vector is resized so that it fits the data.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const buffer& src, buffer& dest) const=0;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    dataTransformer& copy(const dataTransformer& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    dataTransformer& operator=(const dataTransformer& other);

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

  protected:
    static const char* notEnoughSpaceMsg;

  };
}

#endif

