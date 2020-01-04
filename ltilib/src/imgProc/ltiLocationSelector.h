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
 * file .......: ltiLocationSelector.h
 * authors ....: Axel Berner, Xinghan Yu, Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 28.2.2002
 * revisions ..: $Id: ltiLocationSelector.h,v 1.10 2005/06/16 12:35:11 doerfler Exp $
 */

#ifndef _LTI_LOCATION_SELECTOR_H_
#define _LTI_LOCATION_SELECTOR_H_

#include "ltiObject.h"
#include <list>
#include <vector>
#include "ltiContour.h"
#include "ltiLocation.h"
#include "ltiFeatureExtractor.h"

namespace lti {
  /**
   * This functor adjust the given list of locations by a given mask.
   *
   * There are two modes for this functor.  The first one will remove
   * all locations of a given list, that are marked to be removed in
   * a mask.
   *
   * The second mode will separate a list of locations into a set of
   * smaller lists, so that all locations in each sublist have the
   * same label.  This way, sublists of locations per object can be
   * generated.
   */
  class locationSelector : public functor {
  public:
    /**
     * the parameters for the class locationSelector
     */
    class parameters : public functor::parameters {
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
       * Defines the "outmask" value for a location in the given mask.
       * If the mask is at the position of a location equal to the eraseEntry,
       * the location will be removed.
       * default: 0
       */
      ubyte eraseEntry;
    };

    /**
     * default constructor
     */
    locationSelector();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    locationSelector(const locationSelector& other);

    /**
     * destructor
     */
    virtual ~locationSelector();

    /**
     * returns the name of this type ("locationSelector")
     */
    virtual const char* getTypeName() const;

    /**
     * Remove all locations from the given list, for which the corresponding
     * entry in the mask have the value getParameters().eraseEntry.
     *
     * @param mask the decision-mask for the locations
     * @param locs location list.  If for a location loc in the list holds
     *             mask.at(loc.position) == getParameters().eraseEntry, then
     *             the location will be removed from the list.
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& mask, std::list<location>& locs) const;

    /**
     * Separate the locations list "locs" into several smaller location
     * lists, each one having locations for one corresponding label in
     * the labeled mask.
     *
     * @param mask the label mask to select locations.
     * @param locs the complete location list.
     * @param filteredLocs a vector, indexed by the label number in
     *        the mask, containing list of locations.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<int>& mask,
               const std::list<location>& locs,
	       std::vector< std::list<location> >& filteredLocs) const;


    /**
     * Separate the locations list "locs" into several smaller locations
     * lists, each one having locations for one object in
     * the list of areaPoints.
     *
     * @param objs the list of objects.  Each object is represented by
     *             an areaPoints %object.
     * @param locs the complete location list.
     * @param filteredLocs a list, with a number of elements equal the
     * number of objects in objs plus one, containing list of
     * locations, one for each object.  The first element of the vector
     * will contain all locations assigned to the "background", ie. all
     * locations that do not correspond to any object in the objs list.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const std::list<areaPoints>& objs,
               const std::list<location>& locs,
	       std::vector< std::list<location> >& filteredLocs) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    locationSelector& copy(const locationSelector& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    locationSelector& operator=(const locationSelector& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

  };
}

#endif

