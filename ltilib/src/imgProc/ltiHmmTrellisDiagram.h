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
 * file .......: ltiHmmTrellisDiagram.h
 * authors ....: Suat Akyol
 * organization: LTI, RWTH Aachen
 * creation ...: 16.5.2002
 * revisions ..: $Id: ltiHmmTrellisDiagram.h,v 1.8 2005/06/16 12:35:10 doerfler Exp $
 */

#ifndef _LTI_HMM_TRELLIS_DIAGRAM_H_
#define _LTI_HMM_TRELLIS_DIAGRAM_H_

// include only those files which are needed in this header!!

#include "ltiDraw.h"
#include "ltiHiddenMarkovModel.h"
#include "ltiHmmViterbiPathSearch.h"
// include to parent class:
#include "ltiFunctor.h"

namespace lti {
  // comment your class
  /**
   *  Draws the trellis diagram for visualizing the viterbi path selection
   *  given by lti::hmmViterbiPathSearch for a lti::hiddenMarkovModel!
   */
  class hmmTrellisDiagram : public functor {
  public:
    /**
     * the parameters for the class hmmTrellisDiagram
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

      // comment the parameters of your functor
      // If you add more parameters manually, do not forget to do following:
      // 1. indicate in the default constructor the default values
      // 2. make sure that the copy member also copy your new parameters
      // 3. make sure that the read and write members also read and
      //    write your parameters

      /**
       * proportional scaling factor. Should be larger than 1.
       *
       * default 3
       */
      int scaleFactor;

      /**
       * visualize relative emission contibutions of states as
       * grey value encoding of state color along winner path.
       *
       * default = false
       */
      bool emissions;

      /**
       * show density selection along winner path as textual
       * info next to states.
       *
       * default = false;
       */
      bool densities;

      /**
       * draw looser paths into diagram.
       *
       * deafult = false
       */
      bool loosers;

      /**
       * the background color. default 0 (black)
       */
      ubyte bgcol;

      /**
       * the text color. default 255 (white)
       */
      ubyte textcol;

      /**
       * the color for the states in the state-time grid. Will also be
       * used for looser paths. default 64 (dark grey)
       */
      ubyte statecol;

      /**
       * the color of the winner path and the winner states.
       * \b Note that states of winner path will not have
       * this color, if parameter "emissions" is true.
       *
       * default 255 (white)
       */
      ubyte winnercol;

    };

    /**
     * default constructor
     */
    hmmTrellisDiagram();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    hmmTrellisDiagram(const hmmTrellisDiagram& other);

    /**
     * destructor
     */
    virtual ~hmmTrellisDiagram();

    /**
     * returns the name of this type ("hmmTrellisDiagram")
     */
    virtual const char* getTypeName() const;

    // comment your apply methods!

    /**
     * operates on the given %parameter.
     * @param seq the input observation sequence
     * @param hmm the hidden markov model to score the observation sequence
     * @param result the graphical output
     * @param text additional user defined text line to appear in output
     * @return true if apply successful or false otherwise.
     */
    bool apply(const lti::sequence<lti::dvector>& seq,
               const lti::hiddenMarkovModel& hmm,
               lti::channel8& result,
               const std::string& text = "") const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    hmmTrellisDiagram& copy(const hmmTrellisDiagram& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    hmmTrellisDiagram& operator=(const hmmTrellisDiagram& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    // comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

  };
}

#endif

