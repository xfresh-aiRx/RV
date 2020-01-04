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
 * file .......: ltiHistogramViewer.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 21.6.2002
 * revisions ..: $Id: ltiHistogramViewer.h,v 1.18 2005/06/16 12:37:55 doerfler Exp $
 */

#ifndef _LTI_HISTOGRAM_VIEWER_H_
#define _LTI_HISTOGRAM_VIEWER_H_

#include "ltiConfig.h"

#ifdef HAVE_GTK

#include "ltiViewerBase3D.h"
#include "ltiHistogram.h"

namespace lti {

  /**
   * This class display the histograms of images or channels.  Histograms
   * can also be given directly (1D, 2D or 3D).
   */
  class histogramViewer : public viewerBase3D {
  public:
    /**
     * the parameters for the class viewerBase3D
     */
    class parameters : public viewerBase3D::parameters {
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
      virtual parameters& copy(const parameters& other);

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
       * Number of cells (or bins) per dimension for the histograms created.
       * For the one dimensional case, only the \a x attribute will be
       * considered.  In the two dimesional case \a x and \a y are
       * important, and in the three dimensional case all attributes
       * are relevant.
       *
       * Note that this parameter is ignored for the show method that
       * directly receives a histogram, where it intrinsically defines the
       * number of bins per dimension.
       *
       * Default: tpoint3D<int>(32,32,32)
       */
      tpoint3D<int> cells;

      /**
       * Used to specify if the cells should be represented with boxes
       * or with points.
       *
       * If false, points will represent each cell:
       * \code
       *  |
       *  |           .
       *  |           |
       *  |        .  |           .
       *  |        |  |     .     |
       *  |  .     |  |  .  |     |
       *  |  |  .  |  |  |  |  .  |
       *  .  |  |  |  |  |  |  |  |
       *  |__|__|__|__|__|__|__|__|____
       * \endcode
       *
       * If true, boxes will be used:
       * \code
       *  |
       *  |           .__
       *  |           |  |
       *  |        .__|  |        .__
       *  |        |  |  |  .__   |  |
       *  |  .__   |  |  .__|  |  |  |
       *  |  |  .__|  |  |  |  .__|  |
       *  .__|  |  |  |  |  |  |  |  |
       *  |__|__|__|__|__|__|__|__|__|_
       * \endcode
       *
       * Default value: true (i.e. boxes will be displayed)
       */
      bool useBoxes;


      /**
       * Flag to specify if the "color" of the point/box should represent
       * the entry value in the corresponding histogram cell.
       *
       * If true, a grey value will be used to represent the value.  White will
       * be the highest value in the histogram and Black will represent the
       * cells with zero entries.  (see also binThreshold)
       *
       * If false, the color for the cell will be determined with the
       * indices of each cell, assuming that the first element of the index
       * correponds with red, the second with green and the third with blue.
       * This is used to display the "color clouds" of images.
       *
       * Default value: false
       */
      bool greyEntries;

      /**
       * show the lines in case of 2D and 1D histograms to denote the
       * boundaries of the cells:
       *
       * If true, (and with boxes=false) the result will be like the one
       * on the left side.
       *
       * \code
       *  |                               |
       *  |           .                   |           .
       *  |           |                   |
       *  |        .  |           .       |        .              .
       *  |        |  |     .     |       |                 .
       *  |  .     |  |  .  |     |       |  .           .
       *  |  |  .  |  |  |  |  .  |       |     .              .
       *  .  |  |  |  |  |  |  |  |       .
       *  |__|__|__|__|__|__|__|__|____   |____________________________
       * \endcode
       *
       * Default value: false;
       */
      bool useLines;

      /**
       * an entry in the histogram must have a value greater than this
       * threshold multiplied with the maximum entry value in order to
       * be displayed.  Valid values are between 0 and 1.
       *
       * Default: 0.0
       */
      double binThreshold;

      /**
       * \name Info fields
       * The following attributes are not viewer parameters, but
       * are required to give some information to the configuration
       * dialog.
       * This can change in the feature (enclosed in a new class, for example).
       * The values you write here will be ignored.
       */
      //@{

      /**
       * maximal entry value of histogram being displayed
       */
      double infoMaxEntry;

      /**
       * number of entries in the histogram
       */
      double infoNumEntries;

      /**
       * dimensions and bins of the histogram
       */
      ivector infoCells;

      //@}
    };

    /**
     * default constructor
     */
    histogramViewer(bool createDefaultParameters = true);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    histogramViewer(const histogramViewer& other);

    /**
     * constructor with windows title
     */
    histogramViewer(const std::string& title);

    /**
     * constructor with windows title
     */
    histogramViewer(const char* title);

    /**
     * destructor
     */
    virtual ~histogramViewer();

    /**
     * returns the name of this type ("histogramViewer")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    histogramViewer& copy(const histogramViewer& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    histogramViewer& operator=(const histogramViewer& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual viewerBase* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * compute a 3D histogram for the image, where the numbers of 
     * bins per dimension are taken from the parameters' attribute \a cells.
     *
     * Each dimension will cover the entire R,G, or B color component from
     * 0 to 255 divided by the corresponding number of cells. (x for red, y
     * for green and z for blue).
     *
     * The value in the cell will be displayed as a gray level, with
     * white assigned to the highest value in the histogram and black
     * to the lowest, or, in a color cloud where each cell-box will have
     * the corresponding color of the color space, and will be display if
     * its value is greater that a threshold given through the parameter 
     * \c binThreshold
     *
     * @param data image, which histogram will be displayed.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const image& data);

    /**
     * compute a 1D histogram for the channel8, where the number of 
     * bins is taken from the parameters' attribute \a cells.
     *
     * The value-axis covers the entire ubyte value range from 0 to 255
     * divided by the corresponding number of cells. (x component).
     *
     * @param data channel8, which histogram will be displayed.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const channel8& data);

    /**
     * compute a 1D histogram for the channel, where the number of 
     * bins per dimension is taken from the parameters' attribute \a cells.
     *
     * The value-axis covers the entire value range found in the channel
     * divided by the corresponding number of cells. (x component).
     *
     * @param data channel, which histogram will be displayed.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const channel& data);

    /**
     * compute a 1D histogram for the matrix, where the number of 
     * bins per dimension is taken from the parameters' attribute \a cells.
     *
     * The value-axis covers the entire value range found in the matrix
     * divided by the corresponding number of cells. (x component).
     *
     * @param data matrix, which histogram will be displayed.
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const matrix<float>& data);

    /**
     * compute a 1D histogram for the matrix, where the number of 
     * bins per dimension is taken from the parameters' attribute \a cells.
     *
     * The value-axis covers the entire value range found in the matrix
     * divided by the corresponding number of cells. (x component).
     *
     * @param data matrix, which histogram will be displayed.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const matrix<double>& data);

    /**
     * compute a 1D histogram for the matrix, where the number of 
     * bins per dimension is taken from the parameters' attribute \a cells.
     *
     * The value-axis covers the entire value range found in the matrix
     * divided by the corresponding number of cells. (x component).
     *
     * @param data matrix, which histogram will be displayed.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const matrix<int>& data);

    /**
     * compute a 1D histogram for the values in the vector, where the
     * number of bins per dimension is taken from the parameters'
     * attribute \a cells.
     *
     * The value-axis covers the entire value range found in the channel
     * divided by the corresponding number of cells. (x component).
     *
     * Please note that the lti::viewer is faster displaying vectors.
     *
     * shows a vector of double
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const vector<double>& data);

    /**
     * compute a 1D histogram for the values in the vector, where the
     * number of bins per dimension is taken from the parameters'
     * attribute \a cells.
     *
     * The value-axis covers the entire value range found in the channel
     * divided by the corresponding number of cells. (x component).
     *
     * Please note that the lti::viewer is faster displaying vectors.
     *
     * shows a vector of double
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const vector<float>& data);

    /**
     * compute a 1D histogram for the values in the vector, where the
     * number of bins per dimension is taken from the parameters'
     * attribute \a cells.
     *
     * The value-axis covers the entire value range found in the channel
     * divided by the corresponding number of cells. (x component).
     *
     * Please note that the lti::viewer is faster displaying vectors.
     *
     * shows a vector of double
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const vector<int>& data);

    /**
     * shows the given histogram.
     *
     * Please note that the lti::viewer is faster displaying one-dimensional
     * histograms.
     *
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const thistogram<double>& data);

  protected:
    /**
     * the histogram being displayed
     */
    thistogram<double> theHistogram;

    /**
     * return a new instance of the mainWindow class.
     *
     * This class must be reimplemented in order to get the
     * proper class instance.
     */
    virtual viewerBase3D::mainWindow* getNewMainWindow() const;

# ifdef _LTI_MSC_6
  public:
# endif
    /**
     * GTK Widget for the configuration dialog of the histogram viewer
     */
    class configDialog : public viewerBase3D::configDialog {
    public:
      /**
       * create dialog and tell it in which parameter block
       * the user data should be stored
       */
      configDialog();

      /**
       * destroy the dialog
       */
      virtual ~configDialog();

    protected:
      /**
       * Inserts all Pages in the main settings dialog
       */
      virtual void insertPages();

      /**
       * Build a page in the main settings dialog
       */
      pageWidget buildHistoPage();

      /**
       * copy the contents of the dialog in the parameters object
       */
      virtual bool getDialogData();

      /**
       * copy the contents of the parameters object into the dialog entries
       */
      virtual bool setDialogData();

#   ifdef _LTI_MSC_6
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use mouseMovedHandler instead
       */
      bool getDialogDataMS();

      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use mouseMovedHandler instead
       */
      bool setDialogDataMS();
#   endif

      /**
       * @name GTK+ Callbacks
       * Callback functions created by Glade
       */
      //@{
      static void on_greycell_toggled(GtkToggleButton *togglebutton,
                                      gpointer user_data);

      static void on_rgbCell_toggled(GtkToggleButton *togglebutton,
                                     gpointer user_data);

      static void on_points_toggled(GtkToggleButton *togglebutton,
                                    gpointer user_data);

      static void on_boxes_toggled(GtkToggleButton *togglebutton,
                                   gpointer user_data);

      static void on_celllines_toggled(GtkToggleButton *togglebutton,
                                       gpointer user_data);

      static void on_threshold_changed(GtkEditable *editable,
                                       gpointer user_data);

      static void on_binr_changed(GtkEditable *editable,
                                  gpointer user_data);

      static void on_bing_changed(GtkEditable *editable,
                                  gpointer user_data);

      static void on_binb_changed(GtkEditable *editable,
                                  gpointer user_data);
      //@}

      /**
       * @name GTK+ Widgets
       * Callback functions created by Glade
       */
      //@{
      // gtkWidget, which needs to be controled
      GtkWidget* cellcolor;
      GtkWidget* cellcolorGrey;
      GtkWidget* cellcolorRGB;

      GtkWidget* cellformPoint;
      GtkWidget* cellformBox;
      GtkWidget* cellformLines;

      GtkWidget* cellThreshold;

      GtkWidget* binr;
      GtkWidget* bing;
      GtkWidget* binb;

      GtkWidget* maxcellvalue;
      GtkWidget* numofentry;
      GtkWidget* bins;
      //@}
    };

    /**
     * GTK Widget for the main class of the histogram viewer
     */
    class mainWindow : public viewerBase3D::mainWindow {
    public:
      /**
       * default constructor
       */
      mainWindow();

      /**
       * copy constructor
       */
      mainWindow(const mainWindow& other);

      /**
       * destructor
       */
      ~mainWindow();

      /**
       * indicate which data should be used when drawing.
       */
      void putData(const thistogram<double>& hist);

      /**
       * returns used parameters
       */
      const parameters& getParameters() const;

    protected:
      /**
       * indicate if there are valid data to show
       */
      virtual bool validData(void);

      /**
       * set the data pointer to NULL to indicate
       * that the window is hidden
       */
      virtual void indicateDataHide(void);

      /**
       * pointer to external histogram with the data to be
       * converted
       */
      const thistogram<double>* theHistogram;

      /**
       * maximum of theHistogram
       */
      double histMaximum;

      /**
       * convert the histogram into a color image
       */
      virtual void dataToImage();

      /**
       * convert the histogram into a color image
       */
      void his3DToImage();

      /**
       * convert the histogram into a color image
       */
      void his2DToImage();

      /**
       * convert the histogram into a color image
       */
      void his1DToImage();

      /**
       * prepares the parameters before the settings dialog is started
       */
      virtual void prepareParameters(void);

      /**
       * this function is called when the right button is pressed and moved.
       * @param button which button is pressed (0 means none,
       *               1 left button, 3 right button)
       * @param pos actual position of mouse pointer
       * @param shift true if the shift-key has been pressed while moving the
       *              mouse.
       * @param ctrl true if the control-key has been pressed while moving the
       *              mouse.
       */
      virtual void mouseMovedHandler(const int& button,
                                     const point& pos,
                                     const bool shift,
                                     const bool ctrl);

#     ifdef _LTI_MSC_6
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use mouseMovedHandler instead
       */
      void mouseMovedHandlerMS(const int& button,
                               const point& pos,
                               const bool shift,
                               const bool ctrl);
#     endif
    };
  };

}

#endif
#endif

