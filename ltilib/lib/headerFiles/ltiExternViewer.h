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
 * file .......: ltiExternViewer.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 21.07.99
 * revisions ..: $Id: ltiExternViewer.h,v 1.11 2005/06/16 12:37:55 doerfler Exp $
 */

#ifndef _LTI_EXTERNVIEWER_H_
#define _LTI_EXTERNVIEWER_H_

#include "ltiImage.h"
#include "ltiViewerBase.h"
#include <string>
#include <list>

namespace lti {
  /**
   * This object shows an image using an external application like
   * kview, xv, mspaint, etc.
   *
   * Use the externViewer::parameters to specify which application
   * should be used.
   *
   * A temporal image file will be created to transfer the image data
   * to the external application.  If you need a faster viewer, use
   * the lti::viewer instead!
   *
   * The default viewer used in Linux is the kview and in Windows the
   * mspaint.  Make sure that these programs are included in one of
   * the directories specified by the system variable PATH.
   */
  class externViewer : public viewerBase {

  public:

    /**
     * parameters for the externViewer-object
     */
    class parameters : public viewerBase::parameters {
    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       */
      parameters(const parameters& other);

      /**
       * destructor
       */
      virtual ~parameters();

      /**
       * copy member
       */
      parameters& copy(const parameters& other);

      /**
       * copy data of "other" parameters
       */
      parameters& operator=(const parameters& other);

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const;

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
      virtual bool write(ioHandler& handler,
                         const bool complete=true) const;

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

      // ------------------------
      // the parameters
      // ------------------------

      /**
       * directory name, which holds the temporary image files.
       *
       * Default directory: /tmp in Linux and C:\\TEMP in Windows
       */
      std::string tmpDirectory;

      /**
       * name of the external application.
       *
       * Default application: kview in Linux and MSPaint in Windows
       */
      std::string externViewerApp;
    };

    // --------------------------------------------------
    // externViewer
    // --------------------------------------------------

    /**
     * default constructor
     */
    externViewer();

    /**
     * default constructor
     */
    externViewer(const externViewer& other);

    /**
     * default destructor
     */
    ~externViewer();

    /**
     * returns the name of this type ("externViewer")
     */
    virtual const char* getTypeName() const;

    /**
     * shows an lti::image
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    bool show(const image& data);

    /**
     * shows a 8-bit channel
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const channel8& data);

    /**
     * shows a channel
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const channel& data);

    /**
     * shows a channel or matrix of float
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const matrix<float>& data);

    /**
     * shows a vector of double
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const vector<double>& data);

    /**
     * shows a vector of double
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const vector<float>& data);

    /**
     * shows a vector of double
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const vector<int>& data);

    /**
     * shows a matrix of doubles as a channel
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const matrix<double>& data);

    /**
     * shows a matrix of integers as a channel
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const matrix<int>& data);

    /**
     * hides the display window
     * @return true if successful, false otherwise.
     */
    virtual bool hide();

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual viewerBase* clone() const;

    /**
     * copy data of "other" functor.
     */
    externViewer& copy(const externViewer& other);

    /**
     * copy data of "other" functor.
     */
    externViewer& operator=(const externViewer& other);

    /**
     * returns the give parameters
     */
    const parameters& getParameters() const;

  protected:
    /**
     * how many times has 'show' been used?
     */
    static int numberShows;
  }; // class externViewer


} // namespace lti

#endif // _LTI_EXTERNVIEWER_H

