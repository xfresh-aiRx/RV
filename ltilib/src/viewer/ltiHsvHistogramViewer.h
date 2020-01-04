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
 * file .......: ltiHsvHistogramViewer.h
 * authors ....: Joerg Zieren
 * organization: LTI, RWTH Aachen
 * creation ...: 19.7.2002
 * revisions ..: $Id: ltiHsvHistogramViewer.h,v 1.12 2005/06/16 12:37:55 doerfler Exp $
 */

#ifndef _LTI_HSV_HISTOGRAM_VIEWER_H_
#define _LTI_HSV_HISTOGRAM_VIEWER_H_

#include "ltiConfig.h"

#ifdef HAVE_GTK

#include "ltiViewer.h"
#include "ltiHistogram.h"
#include "ltiMergeHSVToImage.h"
#include "ltiFunctor.h"

namespace lti {
  /**
   * A simple HSV histogram viewer. It displays an HSV histogram as a 2D
   * image. Hue (H) corresponds to the y coordinate (ordinate), and saturation
   * (S) to the x coordinate (abscissa). Value (V) is ignored in the histogram
   * in one of the following two ways: 1) A histogram2D is given to the apply
   * method, containing H and S pairs (in this order). 2) A 3D histogram
   * containing H, S, V triples (in this order) is given, but is projected
   * into 2D (H,S)-space by summing up over all values (V).

   * In the display, V is always set to maximum, and the background is black.
   * Every histogram cell corresponds to a square with a fixed side length
   * defineable in the parameters. This square is filled with the corresponding
   * color depending on the cell's value (completely for maximum, empty for 0).
   */
  class hsvHistogramViewer : public functor {
  protected:
    /** A helper class for drawing. */
    class ray {
    public:
      ray(
        const point& start,
        const point& direction,
        const point& nextStartOffset,
        const int& steps);
      void getNextPoint(point& p);
    protected:
      point m_start;
      point m_cursor;
      point m_direction;
      point m_nextStartOffset;
      int m_steps;
      int m_stepsLeft;
    };
  public:
    /**
     * the parameters for the class hsvHistogramViewer
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
       * Scale factor: Side length (in pixels) of the square that
       * corresponds to one histogram cell. (Default: 10)
       */
      int scale;

    };

    /**
     * default constructor
     */
    hsvHistogramViewer();

    /**
     * Uses the given window title.
     */
    hsvHistogramViewer(const std::string& windowTitle);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    hsvHistogramViewer(const hsvHistogramViewer& other);

    /**
     * destructor
     */
    virtual ~hsvHistogramViewer();

    /**
     * returns the name of this type ("hsvHistogramViewer")
     */
    virtual const char* getTypeName() const;

    /**
     * Show the given HSV histogram. The histogram must be 3D
     * (H, S, V - in this order). It is projected into (H,S)-space.
     * @param hist thistogram<double> with the source data.
     * @return true if apply successful or false otherwise.
     */
    bool apply(thistogram<double>& hist) const;

    /**
     * Show the given HSV histogram. The histogram is 2D
     * (H, S - in this order).
     * @param hist histogram2D with the source data.
     * @return true if apply successful or false otherwise.
     */
    bool apply(histogram2D& hist) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    hsvHistogramViewer& copy(const hsvHistogramViewer& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    hsvHistogramViewer& operator=(const hsvHistogramViewer& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  protected:
    /** For converting HSV to RGB (for display) */
    mergeHSVToImage m_hsvToRgb;

    /** For actually showing the histogram. */
    mutable viewer m_viewer;

    /** Fill a square area in the given image with the given color.
      * The square has the the given side length, and its upper left
      * corder is at fromRow/fromCol. The cell is filled to the
      * given percentage. I hope it looks nice :-)
      */
    void fillCell(
      image& img,
      const rgbPixel& color,
      const int& fromRow,
      const int& fromCol,
      const int& sideLength,
      float percent) const;

    /** Convert an (H,S) pair to RGB. V=max is assumed. */
    rgbPixel getColor(
      const int&h, const int& hValues,
      const int&s, const int& sValues) const;
  };
}

#endif
#endif

