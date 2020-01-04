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
 * file .......: ltiJPEGFunctor.h
 * authors ....: Benjamin Winkler
 * organization: LTI, RWTH Aachen
 * creation ...: 27.10.00
 * revisions ..: $Id: ltiJPEGFunctor.h,v 1.27 2005/06/16 12:37:11 doerfler Exp $
 */

#ifndef _LTI_JPEGFUNCTOR_H_
#define _LTI_JPEGFUNCTOR_H_

#include "ltiConfig.h"

#if defined HAVE_LIBJPEG || defined HAVE_LOCALJPEG

#include "ltiIOFunctor.h"

#include <fstream>
#include <vector>
#include <map>


#ifdef HAVE_LIBJPEG
#include <cstdio>
#endif

namespace lti {

  /**
   * Base class to functors which load and save images in JPEG format.
   *
   * There are two implementations of this functor.
   * - The first one uses the C++ JPEG interface from Miano (at
   *   Colosseum Builders C++ Image Library), for which several files
   *   are required.  These are published under another licence and
   *   therefore are not included in the default LTI-Lib.  You can use
   *   them in non-commercial applications and get them from
   *   ltilib.sourceforge.net.  See file ltilib/src/io/jpeg/00readme.txt
   *   for more information.
   *   If the file ltiJPEGFunctor.cpp and the directory jpeg/ are found
   *   in ltilib/src/io, and HAVE_LIBJPEG is not defined in basis/ltiConfig.h,
   *   this will be the implementation used.
   *
   * - The second implementation uses the standard JPEG Library, which is
   *   usually installed in all Linux distributions, but not for Windows.
   *   The implementation is included in the standard LTI-Lib, (file
   *   is called ltiJPEGLibFunctor.cpp).
   *   The autoconf script (./configure) will automatically detect if you
   *   have the libjpeg installed, and activate this implementation if
   *   possible. (see http://www.ijg.org/files/ and
   *   http://freealter.org/doc_distrib/libgr-2.0.13/jpeg/Libjpeg.html)
   *
   * We recomend to use the libjpeg, it is more robust against not 100%
   * correct JPEG-files, and it is faster.  Mianos' implementation is mainly
   * for windows users who do not want to install the jpeglib.
   *
   */
  class ioJPEG : public ioFunctor {
  public:
    /**
     * Parameter class of ioJPEG
     */
    class parameters : public ioFunctor::parameters {
      public:
      /**
       * Default constructor
       */
      parameters();

      /**
       * copy constructor
       */
      parameters(const parameters& other) : ioFunctor::parameters() {
        copy(other);
      };

      /**
       * copy member
       */
      parameters& copy(const parameters& other);

      /**
       * clone member
       */
      virtual functor::parameters* clone() const;

      /**
       * return the name of this type
       */
      virtual const char* getTypeName() const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * write the parameters in the given ioHandler
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
       * there...  This method is public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead!
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead!
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const;
#     endif

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * Determines the compression of the image when saved.
       * Valid values range from 1 to 100. Default value is 75.
       */
      int quality;

      /**
       * Save in progressive or sequential mode.
       * Default is sequential mode (false).
       */
      bool progressive;

      /**
       * Save a comment chunk in the file. Default is an empty string.
       */
      std::string comment;

      /**
       * Number of rows between restart markers (0 = no restart markers)
       * when saving.
       * Default is 0.
       */
      int rowsPerRestart;

      // SamplingFrequency ...
      // ScanAttributes ...

    };

    /**
     * default constructor
     */
    ioJPEG();

    /**
     * destructor
     */
    ~ioJPEG();

    /**
     * returns current parameters
     */
    const parameters& getParameters() const;

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const;
  };

  /**
   * Functor to read a JPEG file
   * It is NOT thread save, this means, the SAME instance can not be used
   * from different threads or processes at the same time.  If this occurs
   * an unpredictible behaviour must be expected!.
   * If this is required, just use a different instance of the functor
   * for each thread!
   * Example:
   *
   * \code
   *
   * lti::image anImg;                      // an image
   * ...
   * lti::loadJPEG myImg;                    // the load functor
   * lti::loadJPEG::parameters param;        // functor parameters
   * param.filename = "~/tmp/theFile.jpeg";  // name of the JPEG file
   * myImg.setParameters(param);            // set the functor parameters
   * myImg.apply(anImg);                    // load the file
   *
   * \endcode
   *
   * You can also use the more confortable shortcut:
   *
   * \code
   * lti::image anImg;                      // an image
   * ...
   * lti::loadJPEG loadImg;                  // the load functor
   * loadImg.load("~/tmp/theFile.jpeg",img); // load the image.
   * \endcode
   *
   * @exception BadJpegStream is thrown when the Stream is corrupted.
   * @see saveJPEG
   * @see image
   * @see loadJPEG::parameters
   *
   * For more information on implementation issues see ioJPEG
   */
  class loadJPEG : public ioJPEG {

  public:

    /**
     * default constructor
     */
    loadJPEG();

    /**
     * destructor
     */
    ~loadJPEG();

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName () const;

    /**
     * loads a JPEG image
     *
     * @param theImage variable where the image is to be stored
     */
    bool apply(image& theImage);

    /**
     * load JPEG image
     *
     * @param theImage variable where the image is to be stored
     * @param isGray returns true when the image is gray scale
     */
    bool apply(image& theImage, bool& isGray);

    /**
     * shortcut for loading a JPEG
     *
     * @param filename name of the file to read
     * @param theImage variable where the image is stored
     */
    bool load(const std::string& filename, image& theImage);

    /**
     * shortcut for loading a JPEG
     *
     * @param filename name of the file to read
     * @param theImage variable where the image is stored
     * @param isGray will be set with true if the image contains only gray
     *               values.
     */
    bool load(const std::string& filename, image& theImage, bool& isGray);

    /**
     * method for loading a JPEG image from an already opened
     * stream.
     */
    bool load(std::ifstream& file,image& theImage);

    /**
     * Returns the essential image information.
     * This function will search the file for all chunks with
     * essential picture information.  Scanning stops before the first
     * StartOfScan-Block. That is, all comments that follow the image
     * data are not read!
     *
     * @param filename name of the file to check
     * @param imageSize size of the image
     * @param comment file comments (author, ...)
     * @param isGray returns true, if grayscale jpeg
     *
     * @return true if file is ok
     */
    bool checkHeader (const std::string& filename, point& imageSize,
                      std::string& comment, bool& isGray);

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual functor* clone () const;

  private:
#ifdef HAVE_LIBJPEG
    /**
     * method for loading a JPEG image from an already opened
     * file.
     */
    bool load(FILE* file,image& theImage,bool& isGray);

#endif
  };

  /**
   * Functor for writing a lti::image to a JPEG file
   * To save a JPEG image just initialize the parameters of the
   * "saveJPEG" object and call the apply member.
   * Example:
   *
   * \code
   *
   * lti::image anImg;                       // an image
   * ...
   * lti::saveJPEG saveImg;                  // the save functor
   * lti::saveJPEG::parameters param;        // functor parameters
   * param.filename = "~/tmp/theFile.jpeg";  // name of the JPEG file
   * saveImg.setParameters(param);           // set the functor parameters
   * saveImg.apply(anImg);                   // save the file
   *
   * \endcode
   *
   * You can also use the more confortable shortcut:
   *
   * \code
   * lti::image anImg;                       // an image
   * ...
   * lti::saveJPEG saveImg;                  // the save functor
   * saveImg.save("~/tmp/theFile.jpeg",img); // save the image.
   * \endcode
   *
   * @see loadJPEG
   * @see image
   * @see saveJPEG::parameters
   */
  class saveJPEG : public ioJPEG {

  public:

    /**
     * default constructor
     */
    saveJPEG();

    /**
     * destructor
     */
    ~saveJPEG();

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName () const;

    /**
     * save JPEG image
     *
     * @param theImage the image to be saved
     */
    bool apply (const image& theImage);

    /**
     * save JPEG image
     *
     * @param theChannel the channel to be saved
     */
    bool apply (const channel& theChannel);

    /**
     * save JPEG image
     *
     * @param theChannel the channel to be saved
     */
    bool apply (const channel8& theChannel);

    /**
     * shortcut for load JPEG
     *
     * @param filename name of the file to save
     * @param theImage the image to be saved
     */
    bool save (const std::string& filename,const image &theImage);

    /**
     * shortcut for load JPEG
     *
     * @param filename name of the file to save
     * @param theChannel the channel to be saved
     */
    bool save (const std::string& filename,const channel& theChannel);

    /**
     * shortcut for load JPEG
     *
     * @param filename name of the file to save
     * @param theChannel the channel to be saved
     */
    bool save (const std::string& filename,const channel8& theChannel);

    /**
     * method for saving a JPEG image to an already opened
     * stream. The stream is left open in this method.
     */
    bool save(std::ofstream& file, const image& theImage);


    /**
     * returns a pointer to a clone of the functor.
     */
    virtual functor* clone () const;

  private:
#ifdef HAVE_LIBJPEG

    /**
     * method for saving a JPEG image to an already opened
     * file.
     */
    bool save(FILE* file,const image& theImage);

#endif

  };
}

#endif
#endif

