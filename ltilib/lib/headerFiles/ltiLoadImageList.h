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
 * file .......: ltiLoadImageList.h
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 22.11.2002
 * revisions ..: $Id: ltiLoadImageList.h,v 1.14 2005/06/16 12:37:11 doerfler Exp $
 */

#ifndef _LTI_LOAD_IMAGE_LIST_H_
#define _LTI_LOAD_IMAGE_LIST_H_


#include "ltiImage.h"
#include "ltiALLFunctor.h"
#include <list>


namespace lti {
  /**
   *  This functor loads images given either in a std::list of
   *  strings, a special file or a whole directory: Each apply call
   *  loads the next image from the list.
   *
   *  The source of the list of image filenames can be chosen via the
   *  parameters parameters::mode. All images which are of types PNG,
   *  BMP or JPEG are loaded. The default is a textfile with a list of
   *  filenames: parameters::listFileName contains the name of the
   *  file (must be set) and parameters::mode is parameters::ListFileName.
   *  \b Note: see parameters::mode for corresponding parameter.
   *
   *  Setting the parameter parameters::skipErrors to true makes the
   *  functor error tolerant. I.e. if an image in the list can not be
   *  loaded, false is returned and the image name is in the status
   *  string. However, the next valid image in the list is
   *  returned. If the parameter is false, false is returned and no
   *  image is loaded. Default is true.
   */
  class loadImageList : public ioFunctor {
  public:
    /**
     * the parameters for the class loadImageList
     */
    class parameters : public ioFunctor::parameters {
    public:

      enum eOperationMode {
        ListFileName,  /*!< read all images given in a textfile, see listFileName*/
        DirName,       /*!< read all image in a directory, see dirName */
        NameList       /*!< read all images in the list, see nameList */
      };

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
       * Determines where the functor expects to find the filenames of
       * the images to load. There are three possibilities:
       * - ListFileName: The string listFileName contains the name of
       *   a textfile which contains the filenames of images to be
       *   selected for later loading. See listFileName for the format
       *   of the textfile
       * - DirName: The string dirName must contain the the path of a
       *   directory which contains image files. All files ending in
       *   '.jpg', '.jpeg', '.png' or '.bmp' are selected for later
       *   loading no matter whether upper or lower case.
       * - NameList: nameList is a list of strings, where each string
       *   is a filename of an image.
       */
      eOperationMode mode;

      /**
       * Name of a directory containing images, e.g. "/home/fooman/myImages/"
       */
      std::string dirName;

      /**
       * Filename of a text file containing filenames of images. Each
       * row contains one filename only. The comment character is the
       * semi-colon ';'. All text behind a semi-colon is regarded as
       * comment. Such a file could look like this:
       *
       * \code
       * ; some images to read
       * /home/fooman/pics1/image1.png
       * ;/home/fooman/pics1/image2.png
       * /home/fooman/pics2/2.BMP
       * /home/fooman/pics1/image1.jpg; another image
       * \endcode
       *
       * The functor would read the images:
       * - /home/fooman/pics1/image1.png
       * - /home/fooman/pics2/2.BMP
       * - /home/fooman/pics1/image1.jpg
       */
      std::string listFileName;

      /**
       * Each entry in the list contains the full filename of an image.
       */
      std::list<std::string> nameList;

      /**
       * If reading an image fails and \a skipErrors is:
       * - \a true: The next available filename is tried. The apply
       * method returns false and sets the status string.
       * - \a false: The image is empty, the apply method returns
       *    false and the status string is set.
       *
       * The default is \a false;
       */
      bool skipErrors;
    };

    /**
     * default constructor
     */
    loadImageList();

    /**
     * Construct a functor using the given parameters
     */
    loadImageList(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    loadImageList(const loadImageList& other);

    /**
     * Sets one of two \a modes: parameters::ListFileName or
     * parameters::DirName and the corresponding \a name of the file
     * or directory in the parameters. Default is the filename of a textfile
     * containing image filenames.
     *
     * See also loadImageList(const std::list<std::string>&),
     * use(const std::string&, parameters::eOperationMode),
     * use(const std::list<std::string>&).
     *
     * @param name name of a file containing image filenames or of a directory
     * @param mode parameters::ListFileName or parameters::DirName.
     */
    loadImageList(const std::string& name,
                  parameters::eOperationMode mode=parameters::ListFileName);

    /**
     * Sets parameters::mode to parameters::NameList and uses the list
     * \a names for loading the files.
     *
     * See also loadImageList(const std::string&, parameters::eOperationMode),
     * use(const std::string&, parameters::eOperationMode),
     * use(const std::list<std::string>&).
     *
     * @param names list of image filenames.
     */
    loadImageList(const std::list<std::string>& names);

    /**
     * destructor
     */
    virtual ~loadImageList();

    /**
     * returns the name of this type ("loadImageList")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    loadImageList& copy(const loadImageList& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    loadImageList& operator=(const loadImageList& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;


    /**
     * Set the parameters and loads the list of filenames given in the
     * parameters into the functor. Returns \a false if cloning of
     * parameters failed or the source of the image files could not be
     * found. The functor is reset by setting new parameters.
     */
    virtual bool updateParameters();

    /**
     * Returns true if more images are available for loading.
     */
    bool hasNext() const;

    /**
     * Returns the filename of the image which will be loaded by the
     * next call to an apply method.
     */
    const std::string& getNextFileName() const;

    /**
     * Starts over from the beginning of the list of image
     * filenames. However, the information is not reread from the
     * parameters. This only happens when setParameters(), use() or a
     * constructor are called.
     */
    void rewind();

    /**
     * Returns the number of image filenames the functor knows. Note
     * that this is not necessarily the number of images that will be
     * loaded, since loading might fail for some images.
     */
    int size() const;

    /**
     * Sets one of two \a modes: parameters::ListFileName or
     * parameters::DirName and the corresponding \a name of the file
     * or directory in the parameters. Default is the filename of a
     * textfile containing image filenames. If \a false is returned,
     * there was an error loading the given file.
     *
     * See also loadImageList(const std::list<std::string>&),
     * loadImageList(const std::string&, parameters::eOperationMode),
     * use(const std::list<std::string>&).
     *
     * @param name name of a file containing image filenames or of a directory
     * @param mode parameters::ListFileName or parameters::DirName.
     * @return false on error
     */
    bool use(const std::string& name,
             parameters::eOperationMode mode=parameters::ListFileName);

    /**
     * Sets parameters::mode to parameters::NameList and uses the list
     * \a names for loading the files. If \a false is returned an
     * error occured.
     *
     * See also loadImageList(const std::list<std::string>&),
     * loadImageList(const std::string&, parameters::eOperationMode),
     * use(const std::string&, parameters::eOperationMode)
     *
     * @param names list of image filenames.
     * @return false on error
     */
    bool use(const std::list<std::string>& names);

    /**
     * load the next image from the list given in the parameters, via
     * the constructor or by calling use().
     *
     * @param theImage the file specified in the parameters will be loaded
     *                 in this image.  Note that independently of the sort
     *                 of image in the file, this will always be converted to
     *                 a color lti::image.
     * @return false on error
     */
    bool apply(image& theImage);

    /**
     * load the next image from the list given in the parameters, via
     * the constructor or by calling use().
     *
     * Use this method if you know that the file contains a gray
     * valued image.  If you try to load a 24-bit image with this
     * method, some quantization algorithms will be used to reduce the
     * number of colors to 256.
     *
     * @param theChannel the image on the file will be loaded here
     * @param colors theChannel contains just indexes to the pixel values
     *               in this vector.
     * @return true if successful, false otherwise.
     */
    bool apply(channel8& theChannel,lti::palette& colors);

    /**
     * load the next image from the list given in the parameters, via
     * the constructor or by calling use().
     *
     * Use this method if you know that the file contains a gray
     * valued image.  If you try to load a 24-bit image with this
     * method, the intensity channel is extracted.
     *
     * @param theChannel the image on the file will be loaded here
     * @return true if successful, false otherwise.
     */
    bool apply(channel& theChannel);

    /**
     * load all images from the list given in the parameters, via
     * the constructor or by calling use().
     *
     * @param theImages all files specified in the parameters will be
     * loaded.  Note that independently of the kind of images in the
     * files, they are always converted to a color lti::image.
     * @return false on error
     */
    bool apply(std::vector<image>& theImages);

    /**
     * load all images from the list given in the parameters, via
     * the constructor or by calling use().
     *
     * Use this method if you know that the file contains a gray
     * valued image.  If you try to load a 24-bit image with this
     * method, some quantization algorithms will be used to reduce the
     * number of colors to 256.
     *
     * @param theChannels all images specified in the parameters
     * @param colors theChannels contains just indexes to the pixel values
     *               in this vector.
     * @return false on error
     */
    bool apply(std::vector<channel8>& theChannels, 
               std::vector<lti::palette>& colors);

    /**
     * load all images from the list given in the parameters, via
     * the constructor or by calling use().
     *
     * Use this method if you know that the file contains a gray
     * valued image.  If you try to load a 24-bit image with this
     * method, the intensity channel is extracted.
     *
     * @param theChannels all images specified in the parameters
     * @return true if successful, false otherwise.
     */
    bool apply(std::vector<channel>& theChannels);

  protected:

    /**
     * internal list of filenames to be loaded
     */
    std::list<std::string> fileNames;

    /** iterator for fileNames */
    std::list<std::string>::const_iterator iter;

    /** empty dummy string */
    const std::string emptyDummy;

    /** functor used for loading images */
    loadImage iLoader;

    /**
     * Analyses the parameters and loads the filenames for the images
     * from the appropriate source. If anything goes wrong, the
     * statusString is set and false is returned.
     */
    bool getImageNames();

    /**
     * Reads the textfile \a filename which contains filenames of images.
     * Returns false if the file could not be opened.
     */
    bool readListFile(const std::string& filename);

    /**
     * Selects all image filenames in the given directory for
     * reading. Returns false if the directory does not exist.
     */
    bool readDirectory(const std::string& dirName);

    /** return true if the filename has an image extension */
    bool hasImageExtension(std::string& filename);
  };
}

#endif

