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
 * file .......: ltiIoObject.h
 * authors ....: Benjamin Winkler
 * organization: LTI, RWTH Aachen
 * creation ...: 14.12.01
 * revisions ..: $Id: ltiIoObject.h,v 1.9 2005/06/16 12:37:12 doerfler Exp $
 */

#ifndef _LTI_IO_OBJECT_H_
#define _LTI_IO_OBJECT_H_

#include "ltiObject.h"
#include "ltiIoHandler.h"

namespace lti {

  /**
   * basic input/output class for data objects
   *
   * @ingroup gStorable
   */
  class ioObject : public object {
  public:

    /**
     * default constructor
     */
    ioObject();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    ioObject(const ioObject& other);

    /**
     * destructor
     */
    virtual ~ioObject();

    /**
     * copy data of "other" ioObject.
     * Please note that the status string will _NOT_ be copied!
     */
    ioObject& copy(const ioObject& other);

    /**
     * assigment operator (alias for copy(other)).
     * @param other the ioObject to be copied
     * @return a reference to the actual ioObject
     */
    ioObject& operator=(const ioObject& other);

    /**
     * returns name of this type
     */
    const char* getTypeName() const;

    /**
     * write the parameters in the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool write(ioHandler& handler, const bool complete=true) const {
      return true;
    };

    /**
     * read the parameters from the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool read(ioHandler& handler,const bool complete=true) {
      return true;
    };

#   ifdef _LTI_MSC_6
    /**
     * this function is required by MSVC only, as a workaround for a
     * very awful bug, which exists since MSVC V.4.0, and still by
     * V.6.0 with all bugfixes (so called "service packs") remains
     * there...  This method is public due to another bug!, so please
     * NEVER EVER call this method directly
     */
    bool readMS(ioHandler& handler,const bool complete=true) {
      return true;
    };

    /**
     * this function is required by MSVC only, as a workaround for a
     * very awful bug, which exists since MSVC V.4.0, and still by
     * V.6.0 with all bugfixes (so called "service packs") remains
     * there...  This method is public due to another bug!, so please
     * NEVER EVER call this method directly
     */
    bool writeMS(ioHandler& handler,const bool complete=true) const {
      return true;
    };
#   endif
  };


  /**
   * write
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler, const ioObject& p, const bool complete=true);

  /**
   * read
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler, ioObject& p, const bool complete=true);
}


#endif

