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
 * file .......: ltiConfigFile.h
 * authors ....: Ruediger Weiler, Joerg Zieren
 * organization: LTI, RWTH Aachen
 * creation ...: 23.10.2001
 * revisions ..: $Id: ltiConfigFileHandler.h,v 1.24 2005/07/04 08:18:41 zieren Exp $
 */

#ifndef _LTI_CONFIG_FILE_HANDLER_H_
#define _LTI_CONFIG_FILE_HANDLER_H_

#include "ltiObject.h"
#include <list>
#include <map>
#include <string>
#include "ltiIOFunctor.h"

namespace lti {
  /**
   * Write and read config files, similar to windows ini files. These contain:
   * - sections
   * - keys
   * - values
   * - comments
   *
   * All entries will be written as specified, however
   * searching keys or values is case insensitive!
   *
   * Example config file:
   * \code
   *  # a global comment
   *
   *  *
   *  * comments in cvs style are alowed too
   *  *
   *
   *  [a section]
   *  # a section comment
   *  key = value ; a key comment
   *
   *  [another section] ;more comments
   *  key = value
   * \endcode
   *
   * Comments can be global, for sections or for single keys.
   * A section comment must be positioned before the first key
   * of this section. Comments for keys start with ';'
   *
   */
  class configFileHandler : public ioFunctor {
  public:
    /**
     * the parameters for the class configFileHandler
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
       * Should output be formatted for better readability? (default=true)
       */
      bool formatOutput;
    };

    /**
     * default constructor
     */
    configFileHandler();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    configFileHandler(const configFileHandler& other);

    /**
     * destructor
     */
    virtual ~configFileHandler();

    /**
     * returns the name of this type ("configFileHandler")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    configFileHandler& copy(const configFileHandler& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    configFileHandler& operator=(const configFileHandler& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * This is an alias for loadFile().
     */
    bool apply();

    /**
     * Loads the specified config file into the dictionary. If the file is not
     * valid, does not exist, or in case of parse errors 'false' will be returned
     * and the statusString will be set.
     */
    bool loadFile();

    /** Same as loadFile(), but given fileName overrides parameters. */
    bool loadFile(const std::string &fileName);

    /**
     * Saves the dictionary in the file specified in the parameters
     * (replaces existing). On error the status string is set and false is returned.
     */
    bool saveFile() const;

    /** Same as saveFile(), but given fileName overrides parameters. */
    bool saveFile(const std::string &fileName) const;

    // ------------------------------------------------------------------------
    /** @name Reading Values
     *  Use these methods to obtain a desired value.
     *  If request is not available "value" variable will be left
     *  unaltered and false will be returned
     */
    //@{

      /** string
       */
      bool getValue(const std::string &section, const std::string &key, std::string &value, std::string &comment) const;
      /** string
       */
      bool getValue(const std::string &section, const std::string &key, std::string &value) const{
        std::string tt;
        return getValue(section,key,value,tt);
      }

      /** bool
       */
      bool getValue(const std::string &section, const std::string &key, bool &value, std::string &comment) const;
      /** bool
       */
      bool getValue(const std::string &section, const std::string &key, bool &value) const{
        std::string tt;
        return getValue(section,key,value,tt);
      }

      /** double
       */
      bool getValue(const std::string &section, const std::string &key, double &value, std::string &comment) const;
      /** double
       */
      bool getValue(const std::string &section, const std::string &key, double &value) const{
        std::string tt;
        return getValue(section,key,value,tt);
      }

      /** float
       */
      bool getValue(const std::string &section, const std::string &key, float &value, std::string &comment) const;
      /** float
       */
      bool getValue(const std::string &section, const std::string &key, float &value) const{
        std::string tt;
        return getValue(section,key,value,tt);
      }

      /** int
       */
      bool getValue(const std::string &section, const std::string &key, int &value, std::string &comment) const;
      /** int
       */
      bool getValue(const std::string &section, const std::string &key, int &value) const{
        std::string tt;
        return getValue(section,key,value,tt);
      }

      /** unsigned int
       */
      bool getValue(const std::string &section, const std::string &key, unsigned int &value, std::string &comment) const;
      /** unsigned int
       */
      bool getValue(const std::string &section, const std::string &key, unsigned int &value) const{
        std::string tt;
        return getValue(section,key,value,tt);
      }

      /** Returns a list of all keys in the specified section. */
      std::list<std::string> getKeys(std::string section) const;

    //@}


    // ------------------------------------------------------------------------
    /** @name Setting Values
     * Use one of these methods to set entries (section,key,value,comment).
     * You can also use these to set comments:
     * - \b global if((section=="") && (key==""))
     * - \b section if((section!="") && (key==""))
     *
     * In case of errors the status string is set and false returned.
     */
    //@{

      /** string
       */
      bool setValue(const std::string &section, const std::string &key = "", const std::string &value = "", const std::string &comment = "");

      /** const char*
       */
      bool setValue(const std::string &section, const std::string &key = "", const char* value= "", const std::string &comment = ""){
        return setValue(section, key, std::string(value), std::string(comment));
      }

      /** bool
       */
      bool setValue(const std::string &section, const std::string &key = "", const bool &value = false, const std::string &comment = "");

      /** double
       */
      bool setValue(const std::string &section, const std::string &key = "", const double &value = 0.0, const std::string &comment = "");

      /** float
       */
      bool setValue(const std::string &section, const std::string &key = "", const float &value = 0.0f, const std::string &comment = "");

      /** int
       */
      bool setValue(const std::string &section, const std::string &key = "", const int &value = 0, const std::string &comment = "");

      /** unsigned int
       */
      bool setValue(const std::string &section, const std::string &key = "", const unsigned int &value = 0, const std::string &comment = "");

    //@}

    // ------------------------------------------------------------------------
    /** @name Manipulation
     *  Use these methods to manipulate the dictionary
     */
    //@{

      /**
       * Sort the sections of the dictionary. The sections are sorted in ascending order.
       * The order of the keys in a section in not changed!
       */
      void sortSections();

      /**
       * Remove alle elements of the section 'section'. If section is not set all
       * comments above the first section are removed. The return value is false if
       * no section of this name exists.
       */
      bool deleteSection(const std::string &section="");

      /**
       * Delete a key in an section. The return value is false if
       * no key of this name exists. If you set the name of the section
       * only you remove all comments of this section.
       */
      bool deleteKey(const std::string &section, const std::string &key="");


      /**
       * Delete every comment, section and key of the internal structure
       * of the config file. If you write after this to disk you get an empty file.
       */
      void clear();

    //@}


  protected:
    class configEntry {
    public:
      configEntry() {
        configEntry("","");
      }
      configEntry(std::string v,
                  std::string c) {
        value = v;
        comment = c;
      }
      ~configEntry() { };
      std::string value;
      std::string comment;
    };

    /**
     * This function splits a line into the tokens key, section...
     */
    std::string splitLine(const std::string& inputLine,
                          std::string& prolog,
                          std::string& section,
                          std::string& key,
                          std::string& value,
                          std::string& comment) const;

    /**
     * Creates a line of the configFile based on the given values
     */
    void composeLine(const std::string& prolog,
                     const std::string& section,
                     const std::string& key,
                     const std::string& value,
                     const std::string& comment,
                     bool secDef,
                     int keyLength,
                     std::string& outputLine) const;

    /**
     * Creates a map containing section|->maxKeyLength for formatted saving.
     * Additionally returns the overall maximum length of a key name
     */
    int getMaxKeyLengths(std::map<std::string,int>& maxKeyLengths) const;

    /** Map with section name as key, and map of section's keys as value */
    std::map<std::string, std::map<std::string, configEntry> > dictionary;

    /** Stores the order of the sections. */
    std::list<std::string> orderSections;

    /** Stores the order of the keys in each section. */
    std::map<std::string, std::list<std::string> > orderKeys;

    /** Stores pure comments without a key */
    std::map<std::string, std::list<std::string> > sectionComments;

    /** Stores capitalization of sections. */
    std::map<std::string, std::string> sectionOrig;
    
    /** Stores capitalization of keys. */
    std::map<std::string, std::map<std::string, std::string> > keyOrig;
  };
}

#endif

