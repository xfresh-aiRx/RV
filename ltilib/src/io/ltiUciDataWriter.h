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
 * file .......: ltiUciDataWriter.h
 * authors ....: Jens Paustenbach
 * organization: LTI, RWTH Aachen
 * creation ...: 21.5.2002
 * revisions ..: $Id: ltiUciDataWriter.h,v 1.11 2005/06/16 12:37:11 doerfler Exp $
 */

#ifndef _LTI_UCI_DATA_WRITER_H_
#define _LTI_UCI_DATA_WRITER_H_

#include <fstream>
#include <list>

#include "ltiMatrix.h"
#include "ltiVector.h"
#include "ltiIOFunctor.h"


namespace lti {
  /**
   * Writes data in the form as it is often found in standard data sets:
   * Raw data either just a matrix of doubles with each row a feature
   * vector, or rows containing feature vectors and an id as last element
   * of the row, or raw data with or without ids and a header with the number
   * of points and the dimensionality. With %parameter append it can be choosen
   * if the data if appended to an existing file or if the file should be new
   * created.
   * <p> If the filename exists and append is false, your data will be lost.
   *
   * This %functor needs the filename and the existence of a header as
   * %parameters. With the third parameter you can choose if the data
   * should be appended of if a new file will be created.
   */
  class uciDataWriter : public ioFunctor {
  public:
    /**
     * the parameters for the class uciDataWriter
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
       * if true there is an id indicating the class of the feature vector
       * inserted
       */
      bool hasHeader;

      /**
       * if true the data will be appended to the given file
       */
      bool append;
    };

    /**
     * default constructor
     */
    uciDataWriter();

    /**
     * sets the filename
     */
    uciDataWriter(std::string filename);

    /**
     * sets the parameters filename and hasHeader
     */
    uciDataWriter(std::string filename, bool bHeader);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    uciDataWriter(const uciDataWriter& other);

    /**
     * destructor
     */
    virtual ~uciDataWriter();

    /**
     * returns the name of this type ("uciDataWriter")
     */
    virtual const char* getTypeName() const;

    /**
     * Writes data to the file given in the parameters.
     * @param data the feature vectors
     * @return true if apply successful or false otherwise.
     */
    bool apply(dmatrix& data) const;

    /**
     * Writes data and ids to the file given in the parameters.
     * @param data the feature vectors
     * @param ids ids of the feature vectors
     * @return true if apply successful or false otherwise.
     */
    bool apply(dmatrix& data, ivector& ids) const;

    /**
     * Writes data to the file given in the parameters.
     * @param data the feature vectors
     * @return true if apply successful or false otherwise.
     */
    bool apply(fmatrix& data) const;

    /**
     * Writes data and ids to the file given in the parameters.
     * @param data the feature vectors
     * @param ids ids of the feature vectors
     * @return true if apply successful or false otherwise.
     */
    bool apply(fmatrix& data, ivector& ids) const;

    /**
     * Writes data and ids to the file given in the parameters.
     * It is suggested that each element of the list is one Cluster
     * @param data the feature vectors
     * @return true if apply successful or false otherwise.
     */
    bool apply(std::list<dmatrix>& data) const;

    /**
     * Writes data and ids to the file given in the parameters.
     * The i-th element of ids is the number to which the points in the i-th
     * matrix in data belong to
     * @param data the feature vectors
     * @param ids IDs for the elements in data.
     * @return true if apply successful or false otherwise.
     */
    bool apply(std::list<dmatrix>& data,ivector& ids) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    uciDataWriter& copy(const uciDataWriter& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    uciDataWriter& operator=(const uciDataWriter& other);

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

