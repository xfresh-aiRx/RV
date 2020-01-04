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
 * file .......: ltiUciDataReader.h
 * authors ....: Peter Doerfler, Jens Paustenbach
 * organization: LTI, RWTH Aachen
 * creation ...: 18.3.2002
 * revisions ..: $Id: ltiUciDataReader.h,v 1.10 2005/06/16 12:37:11 doerfler Exp $
 */

#ifndef _LTI_UCI_DATA_READER_H_
#define _LTI_UCI_DATA_READER_H_

#include <fstream>

#include "ltiMatrix.h"
#include "ltiVector.h"

#include "ltiIOFunctor.h"

namespace lti {
  /**
   * Reads data in the form as it is often found in standard data sets:
   * Raw data either just a matrix of doubles with each row a feature
   * vector, or rows containing feature vectors and an id as last element
   * of the row.
   *
   * This %functor needs the number of dimensions, the existence of
   * ids and the filename as parameters. The according data from that file is
   * returned by the apply method.
   */
  class uciDataReader : public ioFunctor {
  public:
    /**
     * the parameters for the class uciDataReader
     */
    class parameters : public ioFunctor::parameters {
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
       * true if there is an id indicating the class of the feature vector
       * at the end of each line
       */
      bool hasIds;

      /**
       * the number of dimensions of each feature vector
       */
      int numberOfDimensions;

      /**
       * true if there is a header. The header has in its first line the
       * number of points, and in the second line the number of dimensions.
       */
      bool hasHeader;

    };

    /**
     * default constructor
     */
    uciDataReader();

    /**
     * sets the filename
     */
    uciDataReader(std::string filename);

    /**
     * sets the parameters filename, numberOfDimensions and hasIds
     */
    uciDataReader(std::string filename, int nbDimensions, bool bIds=true,
                  bool hasHeader=false);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    uciDataReader(const uciDataReader& other);

    /**
     * destructor
     */
    virtual ~uciDataReader();

    /**
     * returns the name of this type ("uciDataReader")
     */
    virtual const char* getTypeName() const;

    /**
     * Reads data from the file given in the parameters.
     * @param data the feature vectors
     * @return true if apply successful or false otherwise.
     */
    bool apply(dmatrix& data) const;

    /**
     * Reads data and ids from the file given in the parameters. If
     * hasIds is false <code>ids</code> is empty.
     * @param data the feature vectors
     * @param ids ids of the feature vectors
     * @return true if apply successful or false otherwise.
     */
    bool apply(dmatrix& data, ivector& ids) const;

    /**
     * Reads data from the file given in the parameters.
     * @param data the feature vectors
     * @return true if apply successful or false otherwise.
     */
    bool apply(fmatrix& data) const;

    /**
     * Reads data and ids from the file given in the parameters. If
     * hasIds is false <code>ids</code> is empty.
     * @param data the feature vectors
     * @param ids ids of the feature vectors
     * @return true if apply successful or false otherwise.
     */
    bool apply(fmatrix& data, ivector& ids) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    uciDataReader& copy(const uciDataReader& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    uciDataReader& operator=(const uciDataReader& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  };
}

#endif

