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
 * file .......: ltiLncFeatureFile.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 19.04.99
 * revisions ..: $Id: ltiLncFeatureFile.h,v 1.18 2005/06/16 12:37:11 doerfler Exp $
 */

#ifndef _LTI_LNC_FEATURE_FILE_H_
#define _LTI_LNC_FEATURE_FILE_H_

#include <string>
#include <fstream>
#include <set>
#include <map>
#include "ltiVector.h"
#include "ltiIOFunctor.h"

namespace lti {

  /**
   * parent class for the functors which deal with the ASCII file format
   * to store feature vectors.  This is an abstract class an cannot be
   * instantiated.  @see loadLnc, saveLnc
   */
  class lncFeatureFile : public ioFunctor {
  public:

    /**
     * header of a lnc feature file
     *
     * The header of the feature file must begin with something like
     *
     * \code
     * SNNS pattern definition file V3.2
     * generated at Thu Apr  6 17:12:10 2000
     * \endcode
     *
     * The following lines will be ignore except for those which
     * follow following squeme:
     *
     * \code
     * [itemName]:[itemValue]
     * \endcode
     *
     * where <code>[itemName]</code> is a string with no colon (':') and value
     * is a valid integer number.
     *
     * The last line of the header MUST be
     * \code
     * # FeatureType : xyz
     * \endcode
     *
     * where xyz represents any string
     *
     * There are five item-pairs, which will be directly parsed and
     *  stored in * the correspondent header attributes:
     *   - Objects: the number of objects or classes in the file.
     *   - No. of patterns: the number of patterns (or feature vector in the
     *     file
     *   - No. of input units: the dimension of the feature vectors
     *   - No. of output units: the number of classes, to which the pattern
     *                          belongs.  Usually 1
     *   - FeatureType: the name of the feature stored in the file
     *
     * This five pair will NOT be included in the items map when
     * reading the file, and should NOT be present in the items map when
     * storing the file.
     *
     */
    class header : public object {
    public:
      /**
       * default constructor
       */
      header();

      /**
       * copy constructor
       */
      header(const header& other);

      /**
       * copy member
       */
      header& copy(const header& other);

      /**
       * total number of object/classes in the file
       */
      int numberOfObjects;

      /**
       * number of patters (or feature vectors) in this file
       */
      int numberOfPatterns;

      /**
       * dimension of the feature vector
       */
      int numberOfInputUnits;

      /**
       * number of classes, to which each pattern belongs (usually 1)
       */
      int numberOfOutputUnits;

      /**
       * other items-pairs will be stored here
       */
      std::map<std::string,int> items;

      /**
       * name of the feature type
       */
      std::string featureType;

      /**
       * save this header on the given stream
       */
      bool save(std::ostream& dest);

      /**
       * load this header from the given stream
       */
      bool load(std::istream& source);

    };

    /**
     * The ID of a class is determined with three attributes:
     *
     *  - a numerical id number
     *  - the name of the class, which may not contain spaces!
     *  - a set of attributes.  (here they will be stored in a string)
     */
    struct patternId {
    public:
      /**
       * the numerical ID of the object or class, to which the pattern belongs
       */
      int objectId;

      /**
       * name of the object or class
       */
      std::string name;

      /**
       * attributes
       */
      std::string attributes;
    };


    // ----------------------------------------------------
    // Parameters-class
    // ----------------------------------------------------

    /**
     * lncFeatureFile parameters class
     */
    class parameters : public ioFunctor::parameters {
    public:
      /**
       * default constructor
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
       * returns a pointer to a clone of the parameters.
       */
      virtual functor::parameters* clone() const;

      /**
       * returns name of this type
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
       * specify if a class-name-file (cnc) should be used
       */
      bool useCnc;

      /**
       * name of the class-names-file (cnc file).  This name MUST be
       * specified if "useCnc" is true!
       */
      std::string cncFilename;

      /**
       * cnc file type
       *
       * The cnc files contain one object per line.
       * This parameter specifies if the id is expected as first column
       * or in the second one.
       *
       * Default mode: true (i.e. id first column)
       */
      bool idFirst;
    };

    // ---------------------------------
  public:

    /**
     * destructor
     */
    virtual ~lncFeatureFile();

    /**
     * check the consistency of the file
     */
    bool checkConsistency();

    /**
     * copy member
     */
    lncFeatureFile& copy(const lncFeatureFile& other);

    /**
     * set the header information
     */
    void setHeader(const header& other) {
      theHeader = other;
    };

    /**
     * get the header information
     */
    header& getHeader() {
      return theHeader;
    };

    /**
     * get the header information
     */
    const header& getHeader() const {
      return theHeader;
    };

    /**
     * get the used parameters
     */
    const parameters& getParameters() const;

  protected:
    /**
     * default constructor
     */
    lncFeatureFile();

    /**
     * copy constructor
     */
    lncFeatureFile(const lncFeatureFile& other) 
      : ioFunctor() {
      copy(other);
    };

    /**
     * use the parameter values to start a reading/writing session
     */
    bool initialize(const bool deleteFile = false);

    /**
     * load next vector in the feature file
     * @return true if everything is ok, false if there is no more data
     *              available on the file.
     */
    bool loadNext(dvector& theVct,patternId& id);

    /**
     * save next vector at the end of the feature file
     * @return true if everything is ok, if not an exception will be
     *              thrown
     *
     */
    bool saveNext(const dvector& theVct, const patternId& id);

    /**
     * check if the input stream is ready to be readed
     */
    bool isReadyToRead() const {
      return (!input.eof() && input.good());
    };

    /**
     * shortcut to the the number of patterns of the file
     */
    int getNumberOfPatterns() const {return theHeader.numberOfPatterns;};

    /**
     * shortcut to the the dimension of the vectors on the file
     */
    int getNumberOfInputUnits() const {return theHeader.numberOfInputUnits;};

    /**
     * the header of the file actually being used
     */
    header theHeader;

    /**
     * read the cnc file
     */
    bool loadCncFile();

    /**
     * write the whole name-id map to the given cnc file
     * (the contents of the file will be destroyed if the file already
     *  exists)
     */
    bool writeCncFile(const std::string& filename);

    /**
     * append a class name to the cnc-file (class-name-file)
     */
    bool writeToCncFile(const std::string& name);

    /**
     * specify if the file already existed
     */
    bool mayCheckConsistence;

    /**
     * the first time the file is readed
     */
    bool initialized;

    /**
     * the input stream when reading a file
     */
    std::ifstream input;

    /**
     * the output stream when writing a file
     */
    std::ofstream output;

    /**
     * the table with the correspondence between class name and id
     */
    std::map<std::string,int> nameToIdMap;

    /**
     * a set with the ids actually being used
     */
    std::set<int> usedObjects;

    /**
     * read from the given stream a pattern (class name, data and class id)
     */
    bool loadPattern(std::istream& source,dvector& theVct,patternId& id);

    /**
     * write in the given stream a pattern (class name, data and class id)
     */
    bool savePattern(std::ostream& dest,
                     const dvector& theVct,
                     const patternId& id) const;

  };

  /**
   * class to load lnc-files (ascii format to store feature vectors)
   * @see saveLnc
   */
  class loadLnc : public lncFeatureFile {
  public:

    typedef lncFeatureFile::parameters parameters;

    /**
     * Default constructor
     */
    loadLnc();

    /**
     * Constructor with parameters
     */
    loadLnc(const parameters& par);

    /**
     * load the next vector in the file.
     * @param vct the vector object, where the readed data will be written
     * @param id the id of the class the vector belongs to.
     * @return true if everything is ok, false if there
     *         is no more data left on the file. If the file is
     *         corrupted, a lti::exception will be thrown.
     */
    bool apply(dvector& vct,patternId& id);

    /**
     * load all the vectors in the file and leave the results in the given
     * matrix.  The i-th row of the matrix will be the i-th vector on the file.
     * @param vcts the matrix where the vectors will be written
     * @param ids a vector the class ids for each vector in the matrix.  The
     *            i-th element of the vector contains the id of the i-th row
     *            of the matrix.
     * @return true if everything is ok or false if the file is corrupted or
     *              not found.
     */
    bool apply(dmatrix& vcts,ivector& ids);

    /**
     * load all the vectors in the file and leave the results in the given
     * matrix.  The i-th row of the matrix will be the i-th vector on the file.
     * @param vcts the matrix where the vectors will be written
     * @param ids a vector the class ids for each vector in the matrix.  The
     *            i-th element of the vector contains the id of the i-th row
     *            of the matrix.
     * @param names a table with the corresponding names for each object id
     * @return true if everything is ok or false if the file is corrupted or
     *              not found.
     */
    bool apply(dmatrix& vcts,ivector& ids,std::map<int,std::string>& names);

    /**
     * load all the vectors in the file and leave the results in the given
     * matrix.  The i-th row of the matrix will be the i-th vector on the file.
     * @param vcts the matrix where the vectors will be written
     * @param ids a vector the class ids for each vector in the matrix.  The
     *            i-th element of the vector contains the id of the i-th row
     *            of the matrix.
     * @param attribs a vector of std::string with the attributes for each
     *                pattern in the file
     * @param names a table with the corresponding names for each object id
     * @return true if everything is ok or false if the file is corrupted or
     *              not found.
     */
    bool apply(dmatrix& vcts,
               ivector& ids,
               std::vector<std::string>& attribs,
               std::map<int,std::string>& names);

    /**
     * load all the vectors in the file and leave the results in the given
     * matrix.  The i-th row of the matrix will be the i-th vector on the file.
     * @param vcts the matrix where the vectors will be written
     * @param ids a vector, the class ids for each vector in the matrix.  The
     *            i-th element of the vector contains the id of the i-th row
     *            of the matrix.
     * @param attribs a vector of std::string with the attributes for each
     *                pattern in the file
     * @return true if everything is ok or false if the file is corrupted or
     *              not found.
     */
    bool apply(dmatrix& vcts,
               ivector& ids,
               std::vector<std::string>& attribs);

    /**
     * shortcut to read the given lnc file, ignoring the filenames
     * @param filename name of the file to be loaded
     * @param vcts the matrix where the vectors will be written
     * @param ids a vector the class ids for each vector in the matrix.  The
     *            i-th element of the vector contains the id of the i-th row
     *            of the matrix.
     * @param useCnc use a class name file (extension cnc) to get the object
     *              names and corresponding ids.
     * @return true if everything is ok or false if the file is corrupted or
     *              not found.
     */
    bool load(const std::string& filename,
              dmatrix& vcts,
              ivector& ids,
              const bool useCnc=false);

    /**
     * shortcut to read the given lnc file, ignoring the filenames
     * @param filename name of the file to be loaded
     * @param vcts the matrix where the vectors will be written
     * @param ids a vector the class ids for each vector in the matrix.  The
     *            i-th element of the vector contains the id of the i-th row
     *            of the matrix.
     * @param attribs a vector of std::string with the attributes for each
     *                pattern in the file
     * @param useCnc use a class name file (extension cnc) to get the object
     *              names and corresponding ids.
     * @return true  if everything is ok or false if the file is corrupted or
     *              not found.
     */
    bool load(const std::string& filename,
              dmatrix& vcts,
              ivector& ids,
              std::vector<std::string>& attribs,
              const bool useCnc=false);

    /**
     * shortcut to read the given lnc file, ignoring the filenames
     * @param filename name of the file to be loaded
     * @param vcts the matrix where the vectors will be written
     * @param ids a vector the class ids for each vector in the matrix.  The
     *            i-th element of the vector contains the id of the i-th row
     *            of the matrix.
     * @param names a table with the corresponding names for each object id
     * @param useCnc use a class name file (extension cnc) to get the object
     *              names and corresponding ids.
     * @return true  if everything is ok or false if the file is corrupted or
     *              not found.
     */
    bool load(const std::string& filename,
              dmatrix& vcts,
              ivector& ids,
              std::map<int,std::string>& names,
              const bool useCnc=false);

    /**
     * shortcut to read the given lnc file, ignoring the filenames
     * @param filename name of the file to be loaded
     * @param vcts the matrix where the vectors will be written
     * @param ids a vector the class ids for each vector in the matrix.  The
     *            i-th element of the vector contains the id of the i-th row
     *            of the matrix.
     * @param attribs a vector of std::string with the attributes for each
     *                pattern in the file
     * @param names a table with the corresponding names for each object id
     * @param useCnc use a class name file (extension cnc) to get the object
     *              names and corresponding ids.
     * @return true  if everything is ok or false if the file is corrupted or
     *              not found.
     */
    bool load(const std::string& filename,
              dmatrix& vcts,
              ivector& ids,
              std::vector<std::string>& attribs,
              std::map<int,std::string>& names,
              const bool useCnc=false);

    /**
     * returns a map with the names of the classes for each object id.
     */
    void getClassNames(std::map<int,std::string>& names) const;

    /**
     * restart reading the file (use only when a file need to be readed again)
     */
    void reset();

    /**
     * clone member
     */
    virtual functor* clone() const {
      return new loadLnc(*this);
    }
  };

  /**
   * class to save lnc-files (ascii format to store feature vectors)
   * @see loadLnc
   */
  class saveLnc : public lncFeatureFile {
  public:
    /**
     * lncFeatureFile parameters class
     */
    class parameters : public lncFeatureFile::parameters {
    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       */
      parameters(const parameters& other) 
        : lncFeatureFile::parameters() {copy(other);};

      /**
       * copy member
       */
      parameters& copy(const parameters& other);

      /**
       * returns a pointer to a clone of the parameters.
       */
      virtual functor::parameters* clone() const;

      /**
       * returns name of this type
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
       * if true, then the file deleted first, if it already exists,
       * otherwise, the new patterns will be appended at the end of the file.
       *
       * Default value: true
       */
      bool deleteIfExists;
    };

    /**
     * Default constructor
     */
    saveLnc();

    /**
     * Constructor with parameters
     */
    saveLnc(const parameters& par);

    /**
     * get the used parameters
     */
    const parameters& getParameters() const;

    /**
     * clone member
     */
    virtual functor* clone() const {
      return new saveLnc(*this);
    }

    /**
     * save the vector at the end of the file.
     * @param vct the vector object to be saved
     * @param id the id of the class the vector belongs to.
     * @return true if everything is ok, otherwise  a lti::exception will
     *              be thrown.
     */
    bool apply(const dvector& vct,const patternId& id);

    /**
     * restart writing the file
     */
    void reset();

    /**
     * write the whole name-id map to the given cnc file
     * (the contents of the file will be destroyed if the file already
     *  exists)
     */
    bool writeCncFile(const std::string& filename);

    /**
     * set the current name to object id table
     */
    void setNameToIdMap(const std::map<std::string,int>& newMap);

  };

}

#endif


