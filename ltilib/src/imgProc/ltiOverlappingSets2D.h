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
 * file .......: ltiOverlappingSets2D.h
 * authors ....: Claudia Goenner
 * organization: LTI, RWTH Aachen
 * creation ...: 8.2.2004
 * revisions ..: $Id: ltiOverlappingSets2D.h,v 1.3 2005/06/16 12:35:42 doerfler Exp $
 */

#ifndef _LTI_OVERLAPPING_SETS_2D_H_
#define _LTI_OVERLAPPING_SETS_2D_H_

#include "ltiMacroSymbols.h"

// only for compilers different than VC++ 6.0 available
#ifdef _LTI_MSC_6

#pragma message("Insufficient C++ Template Support for lti::regionGraphMeans.")
#pragma message("You need a newer compiler")

#else

#include "ltiImage.h"
#include "cstdlib"
#include "list"

#include "ltiFunctor.h"

#undef _LTI_DEBUG
//#define _LTI_DEBUG 2
#include "ltiDebug.h"
#if defined(_LTI_DEBUG) && (_LTI_DEBUG>1)
#include "ltiViewer.h"
#include <iostream>
using std::cout;
using std::endl;
using std::flush;
#endif

namespace lti {

  /**
   * Considering 2d sets, which may overlap, one might ask in which sets a
   * particular 2d point is contained. This class creates a single
   * 2d look up table (LUT), which stores the ids of all sets that cover
   * a particular position. This is useful to exclude any other set
   * from further processing. For example an image contains several objects
   * and each object has certain 2d features, which are known a priori.
   * If a specific feature is extracted, an application would only process
   * the objects which contain the feature.
   * The classes for estimating chrominance models use this framework for
   * robustness and run time efficiency. If it is known that an object is
   * red then only red chrominances are processed.
   *
   * The LUT entry is coded in an additive manner. E.g. for a total of
   * three sets the LUT will contain values from 0 to 7. If a position is
   * covered by no set, the LUT contains a 0. If a position is contained
   * in all sets the LUT's entry is 7. If a position is contained in the
   * first and third set the LUT returns 3. If a position is contained in
   * the third set only the LUT contains 4.
   *
   * The maximum number of sets is limited by the capacity of the data
   * type int. Caution, this class is only efficient if a few sets overlap.
   *
   * @see chrominanceMapEstimator, chrominanceModelEstimator
   */
  class overlappingSets2D : public functor {
  public:
    /**
     * The parameters for the class overlappingSets2D
     */
    class parameters : public functor::parameters {
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

      /**
       * All ids below this threshold will be marked as discarded.
       * This is useful to blend out objects ore sets from processing
       * which relate to "default" actions only. Internally ids start
       * with 1.
       *
       * Default: 0 (disabled).
       */
      ubyte discardThreshold;

      /**
       * The size of the internal table storing the contained sets.
       * If this value is modified the internal table will be scaled
       * to match the new size using nearest neighbor interpolation.
       *
       * Default: 256.
       */
      ipoint size;
  
#if defined(_LTI_DEBUG) && (_LTI_DEBUG>0)
      //good for debugging
      bool getAllContainedSets(int value, std::list<int>& dest);
#endif

    };

    // -------------------------------------------------------------------

    /**
     * Default constructor
     */
    overlappingSets2D();

    /**
     * Construct a functor using the given parameters
     */
    overlappingSets2D(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    overlappingSets2D(const overlappingSets2D& other);

    /**
     * Destructor
     */
    virtual ~overlappingSets2D();

    /**
     * Returns the name of this type ("overlappingSets2D")
     */
    virtual const char* getTypeName() const;
 
    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    overlappingSets2D& copy(const overlappingSets2D& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    overlappingSets2D& operator=(const overlappingSets2D& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Set used parameters
     */
    bool setParameters(const parameters& theParams);

    /**
     * Initialize the LUT with the overlapping sets from disk.
     * Each set is interpreted as an image and loaded accordingly. Regions
     * that are contained in a set must have intensities above 0.
     * @param prefix a string containing the path or prefix to the files. If
     *   you specify a directory, please do not forget the /. It is not added
     *   automatically.
     * @param names  a vector with the filename of each image or set. The
     *   internal set ids correspond with the order of this vector,
     *   but start from 1.
     * @returns true on success and false otherwise.
     */
    bool readObjectTable(const std::string& prefix,
			 const std::vector<std::string>& names);
    
    /**
     * Initialize the LUT with overlapping sets coded as channel8. Regions
     * that are contained in a set must have values above 0.
     * @param src  a vector containing one channel8 for each set. The
     *   internal set ids correspond with the order of this vector,
     *   but start from 1.
     * @returns true on success and false otherwise.
     */
    bool setObjectTable(const std::vector<channel8>& src);
    
    /**
     * Initialize the LUT with overlapping sets coded as channel8. Regions
     * that are contained in a set must have intensities above 0.
     * @param src  a vector containing one image for each set. The
     *   internal set ids correspond with the order of this vector,
     *   but start from 1.
     * @returns true on success and false otherwise.
     */
    bool setObjectTable(const std::vector<image>& src);

    /**
     * Returns the LUT with the overlapping sets.
     */
    const imatrix& getObjectTable() const;
    
    /**
     * Returns true if position (u,v) is contained in the set associated
     * with the provided label. Fast access which does not check if the 
     * LUT is initialized.
     * @param label the label of the set, which starts from 0. Please note
     * that the label and the internal id are not identical.
     * @param u the column of the LUT.
     * @param v the row of the LUT.
     * @returns true if position (u,v) is contained.
     */
    inline bool isContained(int label, int u, int v) const;
      
    /**
     * Returns true if position (u,v) is not contained
     * in any set, whose internal id is above the discard threshold.
     * Fast access which does not check if the LUT is initialized.
     * @param u the column of the LUT.
     * @param v the row of the LUT.
     * @returns true if position (u,v) is not contained in any set.
     */
    inline bool doDiscard(int u, int v) const;
        
    /**
     * Checks if the LUT for the overlapping sets is initialized.
     * @returns true on success and false otherwise.
     */
    bool isInitialized() const;

    /**
     * Checks if position (u,v) is contained in the set associated
     * with the provided label.
     * @param label the label of the set, which starts from 0. Please note
     * that the label and the internal id are not identical.
     * @param u the column of the LUT.
     * @param v the row of the LUT.
     * @param isContained true if (u,v) is contained in the set and false
     *   otherwise.
     * @returns true on success and false otherwise.
     */
    bool apply(int label, int u, int v, bool& isContained) const;

    /**
     * Checks if position (u,v) is not contained
     * in any set, whose internal id is above the discard threshold.
     * @param u the column of the LUT.
     * @param v the row of the LUT.
     * @param doDiscard true if (u,v) is not contained in any set and false
     *   otherwise.
     * @returns true on success and false otherwise.
     */
    bool apply(int u, int v, bool& doDiscard) const;

  protected:

    /**
     * Initialize the LUT with overlapping sets coded as channel8. Regions
     * that are contained in a set must have values above 0.
     * @param src  a vector containing one channel8 for each set. The
     *   internal set ids correspond with the order of this vector,
     *   but start from 1.
     * @returns true on success and false otherwise.
     */
    bool initObjectTable(const std::vector<channel8>& src);
    
    /**
     * Scale a channel8 to match the LUT size specified in the parameters.
     * @param src original channel8.
     * @param dest scaled channel8.
     * @returns true on success and false otherwise.
     */
    bool scale(const channel8& src, channel8& dest) const;
    
    /**
     * Scale a channel8 to match the LUT size specified in the parameters.
     * @param src channel8 to be scaled. The result is left here too.
     * @returns true on success and false otherwise.
     */
    bool scale(channel8& src) const;
    
    /**
     * Adjust the size of the internal LUT to fit the parameters. The LUT
     * will be scaled using nearest neighbor interpolation.
     */
    bool resize();
 
    /**
     * The LUT with the overlapping sets.
     */
    imatrix m_objectTable;
    
    /**
     * A table which stores the id which is associated with each label,
     * defined by id = 2 power label.
     */
    std::vector<int> m_id;

    /**
     * A copy of the discardThreshold in the parameters. This is here
     * for efficiency.
     */
    int m_discardThreshold;
  };
  
  // -------------------------------------------------------------------

  inline bool overlappingSets2D::doDiscard(int u, int v ) const {

    return ( m_objectTable.at(v,u) <= m_discardThreshold );
  }

  inline bool overlappingSets2D::isContained(int label, int u, int v) const {

    int value ( m_objectTable.at(v,u) );

    //this is here for speedup
    const int test ( m_id[label] );

    //the 3 most frequent cases
    if ( test > value ) {
      return false;
    }
    if ( test == value-1 ) { //only this object and background in set
      return true;
    }
    if ( test == value ) { //only this object in set
      return true;
    }

    //check all other possible overlaps

    //get the largest multiple of 2 that is smaller than value
    int id ( 1 );
    while ( id < value ) {
      id *= 2;
    }
    if ( id > value ) {
      id = id>>1;
    }

    while ( value > 0 ) {
      value -= id;
      if ( test == id ) {
	return true;
      }
      //get the largest multiple of 2 that is smaller than value
      id = 1;
      while ( id < value ) {
	id *= 2;
      }
      if ( id > value )  {
	id = id>>1;
      }
    }

    return false;
  }
}

#include "ltiUndebug.h"

#endif
#endif
