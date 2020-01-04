/*
 * Copyright (C) 1998 - 2005
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
 * file .......: //@filename
 * authors ....: //@author
 * organization: LTI, RWTH Aachen
 * creation ...: //@date
 * //@rev
 */

#ifndef //@_LTI_FILENAME_
#define //@_LTI_FILENAME_

//@TODO: include only those files which are needed in this header!!
//@includes

namespace lti {
  //@TODO: comment your class
  /**
   *  /*@author*/ should document this!
   */
  template<class T>
  class /*@classname*/ : public /*@parentclass*/ {
  public:
    /**
     * The parameters for the class /*@classname*/
     */
    class parameters : public /*@parentclass*/::parameters {
    public:
      /**
       * Default constructor
       */
      parameters() : /*@parentclass*/::parameters() {
        //@TODO: Initialize your parameter values!
        // If you add more parameters manually, do not forget to do following:
        // 1. indicate in the default constructor the default values
        // 2. make sure that the copy member also copy your new parameters
        // 3. make sure that the read and write members also read and
        //    write your parameters
        /*@parametersConstructor*/
      };

      /**
       * Copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other) : /*@parentclass*/::parameters() {
        copy(other);
      }
      
      /**
       * Destructor
       */
      ~parameters() {
      };

      /**
       * Returns name of this type
       */
      const char* getTypeName() const {
        return "/*@classname*/::parameters";
      };

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other) {
#     ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        /*@parentclass*/::parameters::copy(other);
#     else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        /*@parentclass*/::parameters& (/*@parentclass*/::parameters::* p_copy)
                            (const /*@parentclass*/::parameters&) =
                            /*@parentclass*/::parameters::copy;
        (this->*p_copy)(other);
#     endif

        //@copyParameters
        
        return *this;
      };

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other) {
        return copy(other);
      };

      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const {
        return new parameters(*this);
      };

#     ifndef _LTI_MSC_6
      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const
#     else
      /**
       * This function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const
#     endif
      {
        bool b = true;
        if (complete) {
          b = handler.writeBegin();
        }
        
        if (b) {
          //@writeparams
        }

#     ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not able to
        // compile...
        b = b && /*@parentclass*/::parameters::write(handler,false);
#     else
        bool (/*@parentclass*/::parameters::* p_writeMS)(ioHandler&,
                                                         const bool) const =
          /*@parentclass*/::parameters::writeMS;
        b = b && (this->*p_writeMS)(handler,false);
#     endif

        if (complete) {
          b = b && handler.writeEnd();
        }

        return b;
      }

#     ifdef _LTI_MSC_6
      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      bool write(ioHandler& handler,
                 const bool complete=true) const {
        // ...we need this workaround to cope with another really
        // awful MSVC bug.
        return writeMS(handler,complete);
      }
#     endif


#     ifndef _LTI_MSC_6
      /**
       * Read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if read was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true)
#     else
      /**
       * This function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
      bool readMS(ioHandler& handler,const bool complete=true)
#     endif
      {
        bool b = true;
        if (complete) {
          b = handler.readBegin();
        }
        
        if (b) {
          //@readparams
        }

#       ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not
        // able to compile...
        b = b && /*@parentclass*/::parameters::read(handler,false);
#       else
        bool (/*@parentclass*/::parameters::* p_readMS)(ioHandler&,
                                                        const bool) =
          /*@parentclass*/::parameters::readMS;
        b = b && (this->*p_readMS)(handler,false);
#       endif

        if (complete) {
          b = b && handler.readEnd();
        }
        
        return b;
      }

#     ifdef _LTI_MSC_6
      /**
       * Read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if read was successful
       */
      bool read(ioHandler& handler,const bool complete=true) {
        // ...we need this workaround to cope with another really awful MSVC
        // bug.
        return readMS(handler,complete);
      }
#     endif

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      //@TODO: comment the parameters of your functor
      // If you add more parameters manually, do not forget to do following:
      // 1. indicate in the default constructor the default values
      // 2. make sure that the copy member also copy your new parameters
      // 3. make sure that the read and write members also read and
      //    write your parameters
      /*@parameters*/

    };

    /**
     * Default constructor
     */
    /*@classname*/();

    /**
     * Construct a functor using the given parameters
     */
    /*@classname*/(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    /*@classname*/(const /*@classname*/& other);

    /**
     * Destructor
     */
    virtual ~/*@classname*/();

    /**
     * Returns the name of this type ("/*@classname*/")
     */
    virtual const char* getTypeName() const;

    //@TODO: comment your apply methods!
    //@apply_members

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    /*@classname*/& copy(const /*@classname*/& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    /*@classname*/& operator=(const /*@classname*/& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    //@TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

  };
}

#endif
