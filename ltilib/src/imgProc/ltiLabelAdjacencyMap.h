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
 * file .......: ltiLabelAdjacencyMap.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 18.11.2002
 * revisions ..: $Id: ltiLabelAdjacencyMap.h,v 1.8 2005/06/16 12:35:11 doerfler Exp $
 */

#ifndef _LTI_LABEL_ADJACENCY_MAP_H_
#define _LTI_LABEL_ADJACENCY_MAP_H_

#include "ltiFunctor.h"
#include "ltiImage.h"
#include "ltiVector.h"
#include "ltiSparseMatrix.h"

namespace lti {
  /**
   * Visualize a label mask in a color image
   *
   * This class draws a color image using as input a labeled mask.  The
   * colors used for each label are chosen based on the adjacency, so that
   * two neighbor labels never get the same color.
   *
   * You can choose the kind of neighborhood used (4 or 8 neighborhood) and
   * the number of colors you want to use.
   *
   * @ingroup gVisualization
   */
  class labelAdjacencyMap : public functor {
  public:
    /**
     * the parameters for the class labelAdjacencyMap
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
       * If true, the mininum number of colors will be used, which will depend
       * on the neighborhood used.  (a max of 4 colors is required for a
       * 4 neighborhood, and a max of 8 color for a 8 neighborhood).
       *
       * If false, all colors in the palette might be used.
       *
       * Default: false
       */
      bool minColors;

      /**
       * The colors used to denote the labels.  Note that the assigment is not
       * 1 to 1, but will be done depending on the adjacency of the labels.
       *
       * Default: {lti::Black,lti::Red,lti::Green,lti::Blue,lti::Yellow,
       *           lti::Cyan,lti::Magenta,lti::DarkOrange,lti::Lemon,
       *           lti::Violet}
       *
       * This default palette can be access anytime as
       * lti::labelAdjacencyMap::parameters::defaultPalette
       */
      palette thePalette;

      /**
       * Neighborhood used.
       *
       * Valid values are 4 and 8.  Other values will be considered as
       * 8-neighborhood.
       *
       * Default value: 8
       */
      int neighborhood;

      /**
       * Default color palette
       *
       * {lti::Black,  lti::Red,  lti::Green,   lti::Blue,
       *  lti::Yellow, lti::Cyan, lti::Magenta, lti::DarkOrange,
       *  lti::Lemon,  lti::Violet}
       */
      static const palette defaultPalette;

    };

    /**
     * default constructor
     */
    labelAdjacencyMap();

    /**
     * Construct a functor using the given parameters
     */
    labelAdjacencyMap(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    labelAdjacencyMap(const labelAdjacencyMap& other);

    /**
     * destructor
     */
    virtual ~labelAdjacencyMap();

    /**
     * returns the name of this type ("labelAdjacencyMap")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on a copy of the given %parameters.
     * @param src channel8 with the source data.
     * @param dest image where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<ubyte>& src,image& dest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src matrix<int> with the source data.
     * @param dest image where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<int>& src,image& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    labelAdjacencyMap& copy(const labelAdjacencyMap& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    labelAdjacencyMap& operator=(const labelAdjacencyMap& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

// protected:

    /**
     * compute the adjacency of the labels of a given label mask
     */
    bool adjacency(const matrix<ubyte>& mask,sparseMatrix<int>& dest) const;

    /**
     * compute the adjacency of the labels of a given label mask
     */
    bool adjacency(const matrix<int>& mask,sparseMatrix<int>& dest) const;

    /**
     * compute the minimum palette required
     */
    bool computeMinPalette(const sparseMatrix<int>& adj,
                           palette& pal) const;


    /**
     * compute the palette required, using all colors in the parameters if
     * possible
     */
    bool computeMaxPalette(const sparseMatrix<int>& adj,
                           palette& pal) const;

  };
}

#endif

