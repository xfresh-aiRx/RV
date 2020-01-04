/*
 * Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003
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
 * file .......: ltiLoadVectorImageList.h
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 28.11.2003
 * revisions ..: $Id: ltiLoadVectorImageList.h,v 1.5 2005/06/16 12:37:11 doerfler Exp $
 */

#ifndef _LTI_LOAD_VECTOR_IMAGE_LIST_H_
#define _LTI_LOAD_VECTOR_IMAGE_LIST_H_


#include "ltiImage.h"
#include "ltiVector.h"
#include "ltiDownsampling.h"
#include "ltiLoadImageList.h"

namespace lti {
  /**
   * This functor loads a list of images and transforms each into a
   * vector or all into a matrix where each row contains a vector
   * image. For convenience it is possible to perform downsampling on
   * the images while loading. Note that when getting a matrix of
   * vector images all images must have the same size (i.e. all vector
   * images must have the same length).
   *
   * Use this functor just like loadImageList.
   */
  class loadVectorImageList : public loadImageList {
  public:
    /**
     * The parameters for the class loadVectorImageList
     */
    class parameters : public loadImageList::parameters {
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

      //TODO: comment the parameters of your functor
      // If you add more parameters manually, do not forget to do following:
      // 1. indicate in the default constructor the default values
      // 2. make sure that the copy member also copy your new parameters
      // 3. make sure that the read and write members also read and
      //    write your parameters
      

      /**
       * If true each image is downsampled as specified in
       * downsamplingParam. Default false.
       */
      bool downsample;

      /**
       * Parameters used for downsampling when downsample is \a
       * true. Default are the default downsampling::parameters.
       */
      downsampling::parameters downsamplingParam;
    };

    /**
     * Default constructor
     */
    loadVectorImageList();

    /**
     * Construct a functor using the given parameters
     */
    loadVectorImageList(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    loadVectorImageList(const loadVectorImageList& other);

    /**
     * Sets one of two \a modes: parameters::ListFileName or
     * parameters::DirName and the corresponding \a name of the file
     * or directory in the parameters. Default is the filename of a textfile
     * containing image filenames.
     *
     * See also:
     * - loadVectorImageList(const std::list<std::string>&),
     * - use(const std::string&, parameters::eOperationMode),
     * - use(const std::list<std::string>&).
     *
     * @param name name of a file containing image filenames or of a directory
     * @param mode parameters::ListFileName or parameters::DirName.
     */
    loadVectorImageList(const std::string& name,
                        loadImageList::parameters::eOperationMode mode=
                        loadImageList::parameters::ListFileName);

    /**
     * Sets parameters::mode to parameters::NameList and uses the list
     * \a names for loading the files.
     *
     * See also:
     * - loadImageList(const std::string&, parameters::eOperationMode),
     * - use(const std::string&, parameters::eOperationMode),
     * - use(const std::list<std::string>&).
     *
     * @param names list of image filenames.
     */
    loadVectorImageList(const std::list<std::string>& names);

    /**
     * Destructor
     */
    virtual ~loadVectorImageList();

    /**
     * Returns the name of this type ("loadVectorImageList")
     */
    virtual const char* getTypeName() const;

    /**
     * Returns the current image from the list as a vector.
     * @param img the current image as vector
     * @return true if apply successful or false otherwise.
     */
    bool apply(fvector& img);

    /**
     * Returns the current image from the list as a vector.
     * @param img the current image as vector
     * @param rows number of rows of the image
     * @param columns number of columns of the image
     * @return true if apply successful or false otherwise.
     */
    bool apply(fvector& img, int& rows, int& columns);

    /**
     * Returns the current image from the list as a vector.
     * @param img the current image as vector
     * @return true if apply successful or false otherwise.
     */
    bool apply(dvector& img);

    /**
     * Returns the current image from the list as a vector.
     * @param img the current image as vector
     * @param rows number of rows of the image
     * @param columns number of columns of the image
     * @return true if apply successful or false otherwise.
     */
    bool apply(dvector& img, int& rows, int& columns);

    /**
     * Returns all images from the list as rows of the %matrix \a images.
     * @param images all images as rows of the matrix.
     * @return true if apply successful or false otherwise.
     */
    bool apply(fmatrix& images);

    /**
     * Returns all images from the list as rows of the %matrix \a images.
     * @param images all images as rows of the matrix.
     * @param rows number of rows of each image
     * @param columns number of columns of each image
     * @return true if apply successful or false otherwise.
     */
    bool apply(fmatrix& images, int& rows, int& columns);

    /**
     * Returns all images from the list as rows of the %matrix \a images.
     * @param images all images as rows of the matrix.
     * @return true if apply successful or false otherwise.
     */
    bool apply(dmatrix& images);

    /**
     * Returns all images from the list as rows of the %matrix \a images.
     * @param images all images as rows of the matrix.
     * @param rows number of rows of each image
     * @param columns number of columns of each image
     * @return true if apply successful or false otherwise.
     */
    bool apply(dmatrix& images, int& rows, int& columns);

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    loadVectorImageList& copy(const loadVectorImageList& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    loadVectorImageList& operator=(const loadVectorImageList& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * set functor's parameters.
     * This member makes a copy of <em>theParam</em>: the functor
     * will keep its own copy of the parameters!
     * @return true if successful, false otherwise
     */
    virtual bool updateParameters();

    /**
     * Read the functor from the given ioHandler. 
     *
     * The default implementation is to read just the parameters object.
     *
     * Since this virtual method needs to know the exact type of the
     * parameters to call the proper read method, it will just assume that the
     * current functor instance has a valid parameter set.  If this is not
     * the case, you need to reimplement the read method to set first a dummy
     * parameter object.
     *
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool read(ioHandler& handler,const bool complete=true);

  protected:

    downsampling down;

  };
}

#endif

