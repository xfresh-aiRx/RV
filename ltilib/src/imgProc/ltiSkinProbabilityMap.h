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
 * file .......: ltiSkinProbabilityMap.h
 * authors ....: Benjamin Winkler
 * organization: LTI, RWTH Aachen
 * creation ...: 30.1.2001
 * revisions ..: $Id: ltiSkinProbabilityMap.h,v 1.21 2005/06/16 12:35:44 doerfler Exp $
 */

#ifndef _LTI_SKIN_PROBABILITY_MAP_H_
#define _LTI_SKIN_PROBABILITY_MAP_H_


#include "ltiProbabilityMap.h"
#include "ltiLispStreamHandler.h"
#include <fstream>

namespace lti {
  /**
   * creates a skin probability map.
   *
   * the histograms were generated from 3077 pictures containing
   * masked skin regions and 6286 pictures not containing skin, which
   * were taken from the compaq cambridge research lab image-database.
   * Please cite "M. Jones, J. Rehg. Statistical Color Models with
   * Application to Skin Color Detection. Compaq Cambridge Research
   * Lab Technical Report CRL 98/11, 1998." when using in a publication.
   *
   * the color models are located in "skin-32-32-32.hist" and
   * "nonskin-32-32-32.hist", saved in lisp format and must reside in
   * the current directory, or the path must be specified in the constructor.
   *
   * This class just keeps the two color models (for skin and non-skin) and
   * is responsible to load the on time.  All computations are taken by the
   * parent class lti::probabilityMap
   */
  class skinProbabilityMap : public probabilityMap {
  public:
    /**
     * the parameters for the class skinProbabilityMap.
     */
    class parameters : public probabilityMap::parameters {
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
       * @return true if write was succeful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was succeful
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

      // If you add more parameters manually, do not forget to do following:
      // 1. indicate in the default constructor the default values
      // 2. make sure that the copy member also copy your new parameters
      // 3. make sure that the read and write members also read and
      //    write your parameters

      /**
       * assumed overall probability of any pixel being skin (default is 0.5)
       * This is an alias for the attribute of the parent class
       * "objectProbability".
       */
      float& skinProbability;

      /**
       * path where the skin probability histograms can be found.
       *
       * The two files expected here are: <code>skin-32-32-32.hist</code>
       * and <code>nonskin-32-32-32.hist</code>.  These will be readed as
       * soon as the parameters are set.
       *
       * These files can be generated writing the lti::histogram objets with
       * their own write method using an lti::lispStreamHandler object.
       *
       */
      std::string path;
    };

    /**
     * default constructor
     * This constructor will initialize the functor with the default
     * parameters.  If you require to set the parameters later, you can
     * give in the boolean <code>useDefaultParams</code> the value "false".
     * This way the histograms will be loaded when you set the parameters.
     */
    skinProbabilityMap(const bool& useDefaultParams = true);

    /**
     * constructor with path
     * @param path this parameter specify the path where
     *             the skin histogram files can be found, and loads them.
     */
    skinProbabilityMap(const std::string path);

    /**
     * constructor with path
     * @param path this parameter specify the path where
     *             the skin histogram files can be found, and loads them.
     */
    skinProbabilityMap(const char* path);

    /**
     * default constructor
     * @param param  The parameters contain the path where
     *               the skin histogram file can be found, which will be
     *               loaded here.  If you need to wait with the histograms
     *               initialization until the user specifies a path setting
     *               the parameters, you should use the default constructor.
     */
    skinProbabilityMap(const parameters& param);


    /**
     * copy constructor
     * @param other the object to be copied
     */
    skinProbabilityMap(const skinProbabilityMap& other);

    /**
     * destructor
     */
    virtual ~skinProbabilityMap();

    /**
     * returns the name of this type ("skinProbabilityMap")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    skinProbabilityMap& copy(const skinProbabilityMap& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * get a const reference to the skin color model
     */
    static const thistogram<double>& getSkinHistogram();

    /**
     * get a const reference to the non-skin color model
     */
    static const thistogram<double>& getNonSkinHistogram();

  private:

    /**
     * Generate skin probability histogram.
     *
     * This is automatically called in the setParameters() method.
     */
    virtual bool generate();

    /**
     * skin model
     */
    static const thistogram<double> theSkinModel;

    /**
     * non-skin model
     */
    static const thistogram<double> theNonSkinModel;

    /**
     * initialization flag
     */
    static bool initialized;

    /**
     * this member initialize (load) the histograms
     */
    void initialize();


  };
}

#endif

