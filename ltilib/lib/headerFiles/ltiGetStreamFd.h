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
 * file .......: ltiStreamAccess.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 11.11.2002
 * revisions ..: $Id: ltiGetStreamFd.h,v 1.12 2005/06/16 12:37:49 doerfler Exp $
 */

#ifndef _LTI_GET_STREAM_FD_H_
#define _LTI_GET_STREAM_FD_H_

#include "ltiConfig.h"
#include "ltiFunctor.h"

#if defined(HAVE_STREAMBUF_FD) 
#include <iostream>
#include <fstream>
#else
#include <istream>
#include <ostream>
#endif

#include <cstdio>

namespace lti {
  /**
   * This functor can be used to access the file descriptor that is
   * used within a stream object. Its primary purpose is to
   * use it on machines or implementations of the iostreams that do 
   * not offer legal methods of obtaining it.
   *
   * This class is strongly system and compiler dependend
   */
  class getStreamFd : public functor {
  public:
    /**
     * the parameters for the class streamAccess
     */
    class parameters : public functor::parameters {
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
       * If this parameter is true, the file descriptor is duplicated
       * before it is returned. This is primarily relevant for creating
       * new ANSI C "FILE" structures.
       * Default value is false.
       */
      bool duplicateFd;

    };

    /**
     * default constructor
     */
    getStreamFd();

    /**
     * Construct a functor using the given parameters
     */
    getStreamFd(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    getStreamFd(const getStreamFd& other);

    /**
     * destructor
     */
    virtual ~getStreamFd();

    /**
     * returns the name of this type ("getStreamFd")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    getStreamFd& copy(const getStreamFd& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    getStreamFd& operator=(const getStreamFd& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Retrieve the file descriptor associated with the given stream.
     * @param ins ifstream whose file descriptor is to be returned.
     * @param fd will receive the file descriptor, or -1 if it could not
     * be determined.
     * @return true if the method succeeded, or false otherwise.
     */
    bool apply(std::ifstream& ins, int& fd) const;

    /**
     * Retrieve the file descriptor associated with the given stream.
     * @param ins ofstream whose file descriptor is to be returned.
     * @param fd will receive the file descriptor, or -1 if it could not
     * be determined.
     * @return true if the method succeeded, or false otherwise.
     */
    bool apply(std::ofstream& ins, int& fd) const;

    /**
     * Retrieve the file descriptor associated with the given stream.
     * @param ins fstream whose file descriptor is to be returned.
     * @param fd will receive the file descriptor, or -1 if it could not
     * be determined.
     * @return true if the method succeeded, or false otherwise.
     */
    bool apply(std::fstream& ins, int& fd) const;

    /**
     * Creates a new output stream that wraps around the given
     * file descriptor. If the duplicateFd parameter is true,
     * the file descriptor will be duplicated, and the duplicated
     * fd will be closed when the stream is closed. Otherwise,
     * the file descriptor will not be duplicated and not be closed.
     * @param fd the file descriptor
     * @param buffered a flag that tells if the stream should be
     *        buffered (default) or not.
     */
    std::ofstream* newOutputStream(int fd, bool buffered=true) const;

    /**
     * Creates a new input stream that wraps around the given
     * file descriptor. If the duplicateFd parameter is true,
     * the file descriptor will be duplicated, and the duplicated
     * fd will be closed when the stream is closed. Otherwise,
     * the file descriptor will not be duplicated and not be closed.
     * @param fd the file descriptor
     * @param buffered a flag that tells if the stream should be
     *        buffered (default) or not.
     */
    std::ifstream* newInputStream(int fd, bool buffered=true) const;

    /**
     * Create a new FILE structure from the given output stream.
     * WARNING: It is strongly advisable to use this method
     * only with the duplicateFd parameter set to true.
     * The returned FILE file must be closed with fclose.
     * @param ins ofstream for which a new FILE structure is to
     * be created.
     * @param f will receive the pointer to the newly created FILE
     * structure.
     * @return true if the file could be successfully opened,
     * false otherwise.
     */
    bool apply(std::ofstream& ins, FILE*& f) const;

    /**
     * Create a new FILE structure from the given input stream.
     * WARNING: It is strongly advisable to use this method
     * only with the duplicateFd parameter set to true.
     * The returned FILE file must be closed with fclose.
     * @param ins ofstream for which a new FILE structure is to
     * be created.
     * @param f will receive the pointer to the newly created FILE
     * structure.
     * @return true if the file could be successfully opened,
     * false otherwise.
     */
    bool apply(std::ifstream& ins, FILE*& f) const;

    /**
     * Retrieve the file descriptor associated with the given stream.
     * This method ignores the value of the duplicateFd parameter;
     * the file descriptor is never duplicated.
     * @param ins ifstream whose file descriptor is to be returned.
     * @return the file descriptor, or -1 if none could be determined.
     */
    int getfd(std::ifstream& ins) const;

    /**
     * Retrieve the file descriptor associated with the given stream.
     * This method ignores the value of the duplicateFd parameter;
     * the file descriptor is never duplicated.
     * @param ins ifstream whose file descriptor is to be returned.
     * @return the file descriptor, or -1 if none could be determined.
     */
    int getfd(std::ofstream& ins) const;

    /**
     * Retrieve the file descriptor associated with the given stream.
     * This method ignores the value of the duplicateFd parameter;
     * the file descriptor is never duplicated.
     * @param ins ifstream whose file descriptor is to be returned.
     * @return the file descriptor, or -1 if none could be determined.
     */
    int getfd(std::fstream& ins) const;


    /**
     * Sets the duplicateFd parameter according to the given value.
     */
    void setDuplicate(bool d=true);

  };
}

#endif

