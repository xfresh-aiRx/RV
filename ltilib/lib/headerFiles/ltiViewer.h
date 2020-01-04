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
 * file .......: ltiViewer.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 21.4.2000
 * revisions ..: $Id: ltiViewer.h,v 1.41 2005/06/16 12:37:55 doerfler Exp $
 */

#ifndef _LTI_VIEWER_H_
#define _LTI_VIEWER_H_

#include "ltiConfig.h"

#include "ltiViewerBase.h"
#include "ltiHistogram.h"
#include "ltiImage.h"
#include "ltiException.h"

#ifdef HAVE_GTK
#include "ltiGtkWidget.h"
#endif

namespace lti {

  // this can be implemented with any gui toolkit...
  class viewerWindow;

  // this class draws everything
  class viewerFunctor;

  /**
   * Use objects of this class to display your data in a window.
   *
   * The GTK-GUI Toolkit is required to compile this object.
   *
   * The use of the lti::viewer is very simple.  Each viewer
   * administrates one window, and this window will exist as long as
   * the viewer object exists.  This means, if you destroy the
   * viewer-object, the window will be automatically destroyed.
   *
   * To use the viewer see following example:
   *
   * \code
   *
   *  #include "ltiImage.h"
   *  #include "ltiSplitImage.h"
   *  #include "ltiViewer.h"
   *  #include "ltiBMPFunctor.h"
   *
   *  ...
   *
   *  lti::image img;                // an image
   *  lti::channel chnl;             // a channel
   *  lti::splitImageTorgI splitter; // a functor to get the intensity channel
   *
   *  lti::loadBMP loader;
   *
   *  loader.load("yourImage.bmp",img); // load the image in the given file
   *  splitter.getIntensity(img,chnl);  // get the intensity channel of the img
   *
   *  lti::viewer view1,view2;
   *
   *  view1.show(img);                  // open a window and show the image
   *  view2.show(chnl);                 // open a new window and show the
   *                                    // intensity channel.
   *  ...
   *
   *  // until the end of the execution block the images will be displayed.
   *  // When view1 and view2 are destroyed, their windows will disappear!
   *
   * \endcode
   *
   * If you press the left-button of the mouse on a pixel, the value
   * of this pixel will be displayed on the status bar.
   *
   * You can configure some display-options by using the viewer::parameters.
   *
   * If you press the right-mouse button an options-dialog will be displayed.
   * You can control directly some display parameters (brightness, contrast,
   * zoom, etc.)  You can also save exactly the
   * image you are viewing pressing the button "Save" on the options dialog.
   * See lti::viewer::parameters for more information
   *
   * The statistic values computed will correspond always to intensity
   * values, even for color images.  Remember that intensity is
   * defined as (R+G+B)/3.
   *
   * The method waitButtonPressed() allows you to block the execution of your
   * program until the user clicks somewhere on the viewer window.  The
   * position of the pixel clicked will be returned.
   *
   * <b>Known Bugs</b>
   *
   * The multithreaded nature of the viewer functor has shown a few bugs.
   * Until a proper fix is provided, the following workaround have to be done:
   * 
   * 1. A crash at the very end of the application can be expected
   *    since the destruction of window handlers is done by two 
   *    threads simultaneously.  The secondary GTK+ thread is the only one
   *    that should do this, but the main thread destroys everything open at
   *    the end of the application, which also include stuff that secondary
   *    thread should do.  To avoid this conflict you can force termination of
   *    the secondary "GTK-Server" thread explicitely:
   *
   * \code
   *   #include "ltiGtkServer.h"
   *   #include "ltiTimer.h"
   *
   *   int main() {
   *
   *     // ... your code here
   *     // ... try to hide all viewers that aren't used any more
   *     //
   *
   *     lti::passiveWait(50000);     // wait 50ms that all viewers end its 
   *                                  // event handling
   *     lti::gtkServer::shutdown();  // stop the server (secondary thread)
   *     lti::passiveWait(50000);     // wait 50ms that all viewers end its 
   *                                  // event handling
   *     return 1;
   *   }
   *   
   * \endcode
   *    
   *    Please note that the shutdown() method is called at the very end of
   *    the main method.  This must be done this way, because after removing
   *    the GTK Server, there is no way to start it again in order to create
   *    or display the data on viewers.  The last passiveWait forces to wait
   *    until everything that has to do with viewers is removed.
   *
   * 2. The second bug has to do with a race condition when two
   *    viewers try to display data at almost the same time.  Even if the
   *    LTI-Lib calls the locking mechanisms of GTK, they don't seem to
   *    work and the application ends in a dead-lock.  The workaround is
   *    to avoid calling show() methods successively, separating them by
   *    small pauses.  This has to be done only once, before the windows are
   *    created:
   *
   * \code
   *    lti::viewer v1,v2; // two viewers
   *    v1.show(img1);     // show an image
   *    lti::passiveWait(50000) // wait 50ms (WORKAROUND FOR BUG)
   *    v2.show(img2);     // show another image
   *
   *    for (int i=0;i<10;i++) {
   *      v1.show(imgs[i]); // show images, since the viewer was already
   *                        // displayed, the problem does not occur anymore.
   *    }
   * \endcode
   *
   * A new viewer generation is being developed to fix this awful bugs.
   */
  class viewer : public viewerBase {
  public:
    /**
     * The parameters for the class viewer.
     *
     * These parameters control the way an image or channel is
     * displayed.  You can also use them to generate with the
     * lti::viewerFunctor images as shown by the lti::viewer.
     */
    class parameters : public viewerBase::parameters {
    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       */
      parameters(const parameters& other);

      /**
       * destructor
       */
      virtual ~parameters();

      /**
       * copy member
       */
      parameters& copy(const parameters& other);

      /**
       * copy data of "other" parameters
       */
      parameters& operator=(const parameters& other);

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const;

      /**
       * returns name of this type
       */
      const char* getTypeName() const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,
                         const bool complete=true) const;

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
       * there...  This method is public due to another bug!, so please
       * NEVER EVER call this method directly
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is public due to another bug!, so please
       * NEVER EVER call this method directly
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const;
#     endif

      // ------------------------
      // the parameters
      // ------------------------

      /**
       * Predefined color palettes for the channel and channel8 display
       */
      enum eDefaultPalette {
        Gray,       /**< Gray Palette  */
        Hue,        /**< Hue Palette   */
        Red,        /**< Red Palette   */
        Green,      /**< Green Palette */
        Blue,       /**< Blue Palette  */
        UserDefined /**< User Defined Palette */
      };

      /**
       * @name General parameters
       */
      //@{
      /**
       * color of the overflow values.
       *
       * For vectors and 1D histograms, this is used to draw the mean value of 
       * the elements and the mean index value.
       *
       * The default is Yellow
       */
      rgbPixel overflowColor;

      /**
       * color of the underflow values.
       *
       * For vectors and 1D histograms, this is used as the axis color.
       *
       * The default is Blue
       */
      rgbPixel underflowColor;

      /**
       * zoom factor of the window.  The scale factor is 2^zoomFactor.
       * Only values between -4 and 4 are valid.
       *
       * Default value: 0
       */
      int zoomFactor;

      /**
       * Parameter to control the constrast of the displayed image.
       * - +2.0 Use all the dynamic range.  This is the maximum possible
       *        contrast, without losing any information of the normal
       *        intensity value range (due to underflow or overflow).
       * - +1.0 means keep the original image contrast.
       * -  0.0 Zero contrast (a gray value)
       * - -1.0 produce the negative of the image using the original value
       *        range.
       * - -2.0 Use all the dynamic range to generate the negative of the
       *        shown image.
       *
       * Default value: 1.0
       */
      float contrast;

      /**
       * Add the given value to the intensity channel.  Independent of
       * the type of image or channel used, 1.0 means the complete
       * intensity range.
       *
       * Default value: 0.0
       */
      float brightness;

      /**
       * if showValue is true, then the color or channel value will be
       * displayed in a status bar.
       *
       * Default value: true
       */
      bool showValue;

      /**
       * if showHex is true, then the RGB value will be displayed in hex.
       *
       * Default value: false
       */
      bool showHex;

      //@}

      /**
       * @name Options for monochromatic channels
       */
      //@{
      /**
       * Which palette should be used?
       *
       * Default value: Gray
       */
      eDefaultPalette whichPalette;

      /**
       * if the data to be show is a channel or channel8, then the color
       * palette specified here will be used if <code>whichPalette</code> is
       * UserDefined.
       *
       * The default is a grey values palette.
       */
      palette colors;

      /**
       * for integer images, consider them as a label-mask and use
       * only a few colors to appreciate the different labeled regions in
       * a better way.  @see labelAdjacencyMap
       *
       * Default value: false
       */
      bool labelAdjacencyMap;

      /**
       * If labelAdjacencyMap is true, indicate if an 8-Neighborhood (true) or
       * a 4-Neighborhood (false) between the regions exist.
       *
       * Default value: 4-Neighborhood (i.e. false);
       */
      bool neighborhood8;

      /**
       * If labelAdjacencyMap is true, indicate if all colors in the user
       * defined palette should be used or only the first ones.
       *
       * Default value: false (i.e. use all colors in the palette if possible)
       */
      bool useFewColors;
      //@}


      /**
       * @name Options for vectors and histograms
       */
      //@{
      /**
       * Number of pixels used at the y-coordinate of the vector or 
       * histogram1D representation
       *
       * Default value: 256
       */
      int vectorHeight;

      /**
       * Number of pixels per element or per histogram bin used
       * 
       * Default value: 1 pixel
       */
      int pixelsPerElement;

      /**
       * Used to specify if the elements of the vector or bins of histogram1D s
       * should be represented with boxes or with points.
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
       * show the lines between the coordinate system and the points should
       * be drawn.
       *
       * If true, (and with \c useBoxes=false) the result will be like the one
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
       * Default value: false
       */
      bool useLines;

      /**
       * Background color
       *
       * @see lineColor, underflowColor, overflowColor
       *
       * Default value: Black
       */
      rgbPixel backgroundColor;

      /**
       * Line color
       *
       * @see backgroundColor, underflowColor, overflowColor
       *
       * Default value: White
       */
      rgbPixel lineColor;

      /**
       * Show axis.
       *
       * If true, the horizontal axis (value 0) will be drawn using the color
       * given in underflowColor.
       *
       * Default value: true
       */
      bool drawHorizontalAxis;
      
      /**
       * Show mean values
       *
       * If true, the mean value of the elements and the mean value of
       * the indices will be drawn using the color given by
       * overflowColor
       *
       * Default value: false
       */
      bool drawMeanValues;
      //@}

      /**
       * @name Informational Parameters
       */
      //@{

      /**
       * Size of the matrix, image or vector.
       */
      point size;

      /**
       * minimum intensity value
       */
      float minI;

      /**
       * maximum intensity value
       */
      float maxI;

      /**
       * intensity average
       */
      float averageI;

      /**
       * minimum RGB components
       */
      rgbPixel minRGB;

      /**
       * maximum RGB components
       */
      rgbPixel maxRGB;

      /**
       * RGB average
       */
      trgbPixel<float> averageRGB;

      /**
       * standard deviation
       */
      float stdDeviation;

      /**
       * square root of the diagonal of covariance matrix
       */
      trgbPixel<float> diagCovariance;

      //@}
    };

    /**
     * construct a viewer with the given title, contrast and brightness
     *
     * @param title Window title.  Default value "lti::viewer"
     * @param contrast contrast for the image.  Default value 1.0f
     *                 (see viewer::parameters::contrast)
     * @param brightness brightness for the image. Default value 0.0f
     *                   (see viewer::parameters::brightness)
     * @param zoom zoom factor.  Default value 0. 
     *                  (see viewer::parameters::zoomFactor)
     * @param labelMap if true, the label adjacency map modus will be turned on
     */
    viewer(const std::string& title = "lti::viewer", 
           const float contrast = 1.0f,
           const float brightness = 0.0f,
           const int zoom=0,
           const bool labelMap=false);

    /**
     * constructor with given parameters
     */ 
    viewer(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    viewer(const viewer& other);

    /**
     * destructor
     */
    virtual ~viewer();

    /**
     * returns the name of this type ("viewer")
     */
    virtual const char* getTypeName() const;

    /**
     * shows a color image.
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const image& data);

    /**
     * shows a channel
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const matrix<float>& data);

    /**
     * shows a matrix of doubles as a channel
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const matrix<double>& data);

    /**
     * shows a channel
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const matrix<int>& data);
    /**
     * shows a channel
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const matrix<ubyte>& data);

    /**
     * shows histogram as vector of double
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const histogram1D& data);

    /**
     * shows histogram as vector of double
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const histogram2D& data);

    /**
     * shows a vector of double
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const vector<double>& data);

    /**
     * shows a vector of double
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const vector<float>& data);

    /**
     * shows a vector of double
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const vector<int>& data);

    /**
     * shows a vector of double
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const vector<ubyte>& data);

    /**
     * hides the display window
     */
    bool hide();

    /**
     * copy data of "other" functor.
     */
    viewer& copy(const viewer& other);

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual viewerBase* clone() const;

    /**
     * @name Parameters
     */
    //@{

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * set the parameters to be used
     */
    virtual bool setParameters(const viewerBase::parameters& param);

    /**
     * set position of the window
     */
    void setPosition(const point& p);

    /**
     * get position of the window
     */
    point getPosition() const;

    /**
     * set size of the window
     */
    void setSize(const point& p);

    /**
     * get size of the window
     */
    point getSize() const;
    //@}

    /**
     * Get the last clicked position and write the result at the given
     * parameter.  If this position lies within the displayed image, the
     * returned value is true, otherwise false.
     *
     * This method does not block the program execution.  It just returns
     * the last clicked position (which can be seen in the status bar)
     *
     * Note that the clicked position can be out of range.
     *
     * @see waitButtonPressed()
     *
     * @param pos the last clicked position will be written here
     * @return true, if the position lies within the displayed image,
     *         false otherwise.
     */
    bool lastClickedPosition(point& pos) const;

    /**
     * Wait until the user clicks somewhere on the image/vector
     * and returns the clicked position.
     *
     * Note that the clicked position can be out of range, unless you
     * specify with the parameter, that you expect only in-image points.
     *
     * @see lastClickedPosition()
     */
    point waitButtonPressed(const bool reportOnlyValidPos = false) const;


    /**
     * return the last pressed key
     *
     * This method does not block the program execution.  It just returns
     * the value of the last key pressed.
     *
     * @see waitKey()
     */
    int lastKey() const;

    /**
     * Wait until the user press some key while the viewer window is active
     * and return the code of the key pressed.
     *
     * @return the last key pressed or a negative number if the window
     *         was destroyed.
     *
     * @see lastKey()
     */
    int waitKey() const;

    /**
     * Wait until the user presses a key or a mouse button.
     *
     * @param key will be set to true if the user presses a key, otherwise
     *            will be set to false.
     * @param button will be set to true if the user presses a mouse button,
     *               otherwise will be set to false.
     * @return the key code at the x attribute of the point if the user
     *         presses a key or the clicked position if the user pressed a 
     *         mouse button.
     */
    point waitKeyOrButton(bool& key,bool& button) const;

  protected:

#ifdef HAVE_GTK
    /**
     * Parent class for all types of configuration dialogs for the normal
     * viewer.
     */
    class configDialog : public configGTKDialog {
    public:

      /**
       * Default constructor
       */
      configDialog();

      /**
       * @name Virtual members to be reimplemented
       *
       * Following members should be reimplemented in your derived class
       * if you add new dialog options.
       */
      //@{

      /**
       * Inserts all pages in the main settings dialog.
       *
       */
      virtual void insertFrames();

      /**
       * copy the contents of the dialog in the parameters object.
       *
       * This method should read all data from the GTK-Widget and write them
       * in the parameter object.  Do not forget to check first if the
       * parameter pointer is valid or not.
       *
       * @return true if successful, false otherwise.
       */
      virtual bool getDialogData();

      /**
       * copy the contents of the parameters object into the dialog entries
       *
       * This method should write all data given in the parameters
       * into the GTK-Widget.
       * Do not forget to check first if the parameter pointer is
       * valid or not.
       *
       * @return true if successful, false otherwise.
       */
      virtual bool setDialogData();

      /**
       * create a new instance of this configuration dialog
       */
      virtual configGTKDialog* newInstance() const;
      //@}

      /**
       * set the drawing functor being used
       */
      void useFunctor(viewerFunctor& fct);

    protected:
      /**
       * Non virtual function to set all common dialog data
       */
      bool setCommonDialogData();

      /**
       * Set all widgets as NULL
       */
      void clearAllWidgets();

      /**
       * Build and Append the intensity control frame
       */
      void buildIntensityControlFrame();

      /**
       * Build and Append the intensity control frame
       */
      void buildZoomFrame();

      /**
       * Build and Append the statistics frame
       */
      void buildStatisticsFrame();

      /**
       * Build and Append the Palette frame
       */
      void buildPaletteFrame();

      /**
       * Build and Append the Palette frame with label control
       */
      void buildPaletteAndLabelFrame();

      /**
       * Build hexadecimal button
       */
      void buildHexadecimalDisplayFrame();

      /**
       * Build vector options frame
       */
      void buildVectorFrame();

      /**
       * The painting functor
       */
      viewerFunctor* master;

      /**
       * GTK Widgets
       */
      //@{
      // Statistics Frame
      GtkWidget *StatisticsFrame;
      GtkWidget *StatisticsBox;
      GtkWidget *StatisticsSubBox;
      GtkWidget *MinLabel;
      GtkWidget *MaxLabel;
      GtkWidget *AverageLabel;
      GtkWidget *StdDevLabel;
      GtkWidget *SizeEntry;
      GtkWidget *MinEntry;
      GtkWidget *MaxEntry;
      GtkWidget *AverageEntry;
      GtkWidget *StdDevEntry;
      GtkWidget *ScaleMinimumButton;
      GtkWidget *ScaleMaximumButton;
      GtkWidget *ScaleMinMaxButton;
      GtkWidget *Size;
      GtkWidget *AvrgStdDevScalingBox;
      GtkWidget *ScaleStdDevButton;
      GtkWidget *ScalingFactorEntry;
      GtkWidget *ColorsBox;
      GtkWidget *UnderflowColorLabel;
      GtkWidget *UnderflowColorButton;
      GtkWidget *OverflowColorButton;
      GtkWidget *OverflowColorLabel;

      // Intensity Control Frame
      GtkWidget *IntensityControlFrame;
      GtkWidget *table1;
      GtkWidget *ZoomLabel;
      GtkWidget *ContrastLabel;
      GtkWidget *BrightnessLabel;
      GtkWidget *ZoomScale;
      GtkObject *ZoomAdjustment;
      GtkObject *BrightnessAdjustment;
      GtkObject *ContrastAdjustment;
      GtkWidget *BrightnessEntry;
      GtkWidget *ContrastEntry;
      GtkWidget *ZoomEntry;
      GtkWidget *ContrastScale;
      GtkWidget *BrightnessScale;

      // Palette Control Frame
      GtkWidget *PaletteControlFrame;
      GtkWidget *PaletteOptionsBox;
      GtkWidget *UsualPalettes;
      GSList *_1_group;
      GtkWidget *GrayButton;
      GtkWidget *RedButton;
      GtkWidget *GreenButton;
      GtkWidget *BlueButton;
      GtkWidget *HueButton;
      GtkWidget *UserDefinedButton;

      // Label Adjacency
      GtkWidget *LabelAdjacencyButton;
      GtkWidget *hbox2;
      GSList *_2_group;
      GtkWidget *Neigh4Button;
      GtkWidget *Neigh8;
      GtkWidget *MinColorsButton;

      // Hexadecimal Display
      GtkWidget *hexDisplay;

      // Vector Options Frame
      GtkWidget *VectorOptionsFrame;
      GtkWidget *vctSubframe;
      GtkWidget *vctbox1;
      GtkWidget *vctSizes;
      GtkWidget *vctHeightLabel;
      GtkWidget *vctHeight;
      GtkWidget *vctPixPerElemLabel;
      GtkWidget *vctPixPerElem;
      GtkWidget *vctBools;
      GtkWidget *useBoxes;
      GtkWidget *useLines;
      GtkWidget *horizontalAxis;
      GtkWidget *drawMeanValues;
      GtkWidget *vctColors;
      GtkWidget *backgroundColor;
      GtkWidget *lineColor;
      //@}

      /**
       * Callback functions
       */
      //@{
      static void on_ScaleMinimumButton_clicked(GtkButton *button,
                                                gpointer user_data);

      static void on_ScaleMaximumButton_clicked(GtkButton *button,
                                                gpointer user_data);

      static void on_ScaleMinMaxButton_clicked(GtkButton *button,
                                               gpointer user_data);

      static void on_ScaleStdDevButton_clicked(GtkButton *button,
                                               gpointer user_data);

      static void on_UnderflowColorButton_clicked(GtkButton *button,
                                                  gpointer user_data);

      static void on_OverflowColorButton_clicked(GtkButton *button,
                                                 gpointer user_data);

      static void on_zoom_value_changed(GtkAdjustment* widget,
                                        gpointer value);

      static void on_contrast_value_changed(GtkAdjustment* widget,
                                            gpointer value);

      static void on_brightness_value_changed(GtkAdjustment* widget,
                                              gpointer value);

      static void on_BrightnessEntry_changed(GtkEditable *editable,
                                             gpointer user_data);

      static void on_ContrastEntry_changed(GtkEditable *editable,
                                           gpointer user_data);

      static void on_ZoomEntry_changed(GtkEditable *editable,
                                       gpointer user_data);

      static void on_palette_toggled(GtkToggleButton *togglebutton,
                                     gpointer user_data);

      static void on_LabelAdjacencyButton_toggled(GtkToggleButton *togglebutton,
                                                  gpointer user_data);

      static void on_neighborhood_toggled(GtkToggleButton *togglebutton,
                                          gpointer user_data);

      static void on_MinColorsButton_toggled(GtkToggleButton *togglebutton,
                                             gpointer user_data);

      static void on_hexDisplay_toggled(GtkToggleButton *togglebutton,
                                        gpointer user_data);


      static void on_vctHeight_changed(GtkEditable *editable,
                                       gpointer user_data);
      
      static void on_vctPixPerElem_changed(GtkEditable *editable,
                                           gpointer user_data);
      
      static void on_useBoxes_toggled(GtkToggleButton *togglebutton,
                                      gpointer user_data);
      
      static void on_useLines_toggled(GtkToggleButton *togglebutton,
                                      gpointer user_data);
      
      static void on_horizontalAxis_toggled(GtkToggleButton *togglebutton,
                                            gpointer user_data);
      
      static void on_drawMeanValues_toggled(GtkToggleButton *togglebutton,
                                            gpointer user_data);
      
      static void on_backgroundColor_clicked(GtkButton *button,
                                             gpointer user_data);
      
      static void on_lineColor_clicked(GtkButton *button,
                                       gpointer user_data);
      


      /**
       * compute, using the min and max values in the parameters object,
       * which contrast and brightness are required to have an interval
       * between 0 and max
       */
      void minScaling(float& contr,float& bright);

      /**
       * compute, using the min and max values in the parameters object,
       * which contrast and brightness are required to have an interval
       * between min and 1
       */
      void maxScaling(float& contr,float& bright);

      /**
       * compute, using the min and max values in the parameters object,
       * which contrast and brightness are required to have an interval
       * between avrg-factor*stddev and avrg+factor*stddev
       */
      void deltaScaling(float& contr,float& bright);

      /**
       * Value used as 1.0 in the given image type (usually 1.0 or 255);
       */
      float norm;

      /**
       * Temporal parameters object.  The real parameters are taken only
       * if the user does an ok!
       */
      parameters tmpParam;

      //@}
    };

    /**
     * Configuration Dialog for viewer of images of fixed point scalar
     * values, like channel8 or matrix<int>
     */
    class configChannelFixed : public configDialog {
    public:
      /**
       * constructor
       */
      configChannelFixed();

      /**
       * Inserts all pages in the main settings dialog.
       *
       */
      virtual void insertFrames();

      /**
       * copy the contents of the parameters object into the dialog entries
       *
       * This method should write all data given in the parameters
       * into the GTK-Widget.
       * Do not forget to check first if the parameter pointer is
       * valid or not.
       *
       * @return true if successful, false otherwise.
       */
      virtual bool setDialogData();

      /**
       * create a new instance of this configuration dialog
       */
      virtual configGTKDialog* newInstance() const;
    };

    /**
     * Configuration Dialog for viewer of images of floating point scalar
     * values, like channel or matrix<double>
     */
    class configChannelFloat : public configDialog {
    public:
      /**
       * constructor
       */
      configChannelFloat();

      /**
       * Inserts all pages in the main settings dialog.
       *
       */
      virtual void insertFrames();

      /**
       * copy the contents of the parameters object into the dialog entries
       *
       * This method should write all data given in the parameters
       * into the GTK-Widget.
       * Do not forget to check first if the parameter pointer is
       * valid or not.
       *
       * @return true if successful, false otherwise.
       */
      virtual bool setDialogData();

      /**
       * create a new instance of this configuration dialog
       */
      virtual configGTKDialog* newInstance() const;
    };

    /**
     * Configuration Dialog for viewer of images of rgbPixel elements.
     */
    class configImage : public configDialog {
    public:
      /**
       * constructor
       */
      configImage();

      /**
       * Inserts all pages in the main settings dialog.
       *
       */
      virtual void insertFrames();

      /**
       * copy the contents of the parameters object into the dialog entries
       *
       * This method should write all data given in the parameters
       * into the GTK-Widget.
       * Do not forget to check first if the parameter pointer is
       * valid or not.
       *
       * @return true if successful, false otherwise.
       */
      virtual bool setDialogData();

      /**
       * create a new instance of this configuration dialog
       */
      virtual configGTKDialog* newInstance() const;
    };

    /**
     * Configuration Dialog for viewer of vectors
     */
    class configVector : public configDialog {
    public:
      /**
       * constructor
       */
      configVector();

      /**
       * Inserts all pages in the main settings dialog.
       *
       */
      virtual void insertFrames();

      /**
       * copy the contents of the parameters object into the dialog entries
       *
       * This method should write all data given in the parameters
       * into the GTK-Widget.
       * Do not forget to check first if the parameter pointer is
       * valid or not.
       *
       * @return true if successful, false otherwise.
       */
      virtual bool setDialogData();

      /**
       * create a new instance of this configuration dialog
       */
      virtual configGTKDialog* newInstance() const;
    };

    /**
     * Abstract class for all main windows for the different data types
     */
    class mainWindow : public mainGTKWindow {
    public:
      /**
       * Constructor
       */
      mainWindow();

      /**
       * Copy constructor
       */
      mainWindow(const mainWindow& other);

      /**
       * Destructor
       */
      virtual ~mainWindow();

      /**
       * clone this window
       */
      virtual mainGTKWindow* clone() const = 0;

      /**
       * Types accepted by this viewer (and its subclasses)
       */
      enum eDataType {
        Invalid,
        MatrixUByte,
        MatrixInt,
        MatrixFloat,
        MatrixDouble,
        Image,
        VectorUByte,
        VectorInt,
        VectorFloat,
        VectorDouble
      };

      /**
       * Set the data to be displayed (matrix<ubyte> and inherited classes)
       */
      void setData(const matrix<ubyte>& data);

      /**
       * Set the data to be displayed
       */
      void setData(const matrix<int>& data);

      /**
       * Set the data to be displayed (matrix<float> and inherited classes)
       */
      void setData(const matrix<float>& data);

      /**
       * Set the data to be displayed (matrix<float> and inherited classes)
       */
      void setData(const matrix<double>& data);

      /**
       * Set the data to be displayed
       */
      void setData(const matrix<rgbPixel>& data);

      /**
       * Set the data to be displayed
       */
      void setData(const vector<ubyte>& data);

      /**
       * Set the data to be displayed
       */
      void setData(const vector<int>& data);

      /**
       * Set the data to be displayed
       */
      void setData(const vector<float>& data);

      /**
       * Set the data to be displayed
       */
      void setData(const vector<double>& data);

      /**
       * generate an event which will show the data
       */
      virtual bool drawData();

      /**
       * Indicate which functor should be used for painting
       */
      void useFunctor(viewerFunctor& fct);

      /**
       * Get the last clicked position and write the result at the given
       * parameter.  If this position lies within the displayed image, the
       * returned value is true, otherwise false.
       *
       * This method does not block the program execution.  It just returns
       * the last clicked position (which can be seen in the status bar)
       *
       * Note that the clicked position can be out of range.
       *
       * @see waitButtonPressed()
       *
       * @param pos the last clicked position will be written here
       * @return true, if the position lies within the displayed image,
       *         false otherwise.
       */
      bool lastClickedPosition(point& pos) const;

      /**
       * Wait until the user clicks somewhere on the image/vector
       * and returns the clicked position.
       *
       * Note that the clicked position can be out of range.
       *
       * @see lastClickedPosition()
       */
      point waitButtonPressed(const bool onlyValidPos) const;

      /**
       * return the last pressed key
       *
       * This method does not block the program execution.  It just returns
       * the value of the last key pressed.
       *
       * @see waitKey()
       */
      int lastKey() const;

      /**
       * Wait until the user press some key while the viewer window is active
       * and return the code of the key pressed.
       *
       * @see lastKey()
       */
      int waitKey() const;

      /**
       * Wait until the user presses a key or a mouse button.
       *
       * @param key will be set to true if the user presses a key, otherwise
       *            will be set to false.
       * @param button will be set to true if the user presses a mouse button,
       *               otherwise will be set to false.
       * @return the key code if the user presses a key or the mouse button 
       *         number.
       */
      point waitKeyOrButton(bool& key,bool& button) const;
      
    protected:
      /**
       * Type of the data contained in this class
       */
      eDataType dataType;

      /**
       * copy of the data wished to be visualized
       */
      const mathObject* theData;

      /**
       * The painting functor
       */
      viewerFunctor* master;

      /**
       * Value used as 1.0 in the given image type (usually 1.0 or 255);
       */
      float norm;

      /**
       * true if the data has been set and no statistics have been
       * computed for it yet.
       */
      bool newDataFlag;

      /**
       * mutex to protect the data attribute
       */
      mutable mutex dataLock;

      /**
       * drawing available
       */
      semaphore drawRequestSem;

      /**
       * @name Members to be reimplemented
       */
      //@{
      /**
       * This method is the one where your data should be drawn.
       * You just need to draw on the "theImage" attribute image.
       */
      virtual void dataToImage(image& img) = 0;

      /**
       * This method is called as soon as all data has been drawn.  You can
       * use it to send a message to another widget or object, in order to
       * allow further actions.  The default implementation does nothing.
       */
      virtual void drawReady();

      /**
       * Prepare the parameters before the configuration dialog is started.
       *
       * This member gets some information of the displayed data into
       * the informational attributes of the parameters instance.
       * It is called just before opening the settings dialog, and is
       * the proper place to update the mentioned informational data.
       */
      virtual void prepareParameters() = 0;

      /**
       * Indicate if there are valid data to show.
       *
       * This method must check if the provided data is ready
       * to be shown, i.e. if the dataToImage() can be already
       * be used to draw the data.
       */
      virtual bool validData(void);

      /**
       * this method will be called to invalidate the data.
       *
       * Usually will set a few pointers to NULL to indicate
       * that the window has been hidden, and no valid data is
       * keeped in this class any more.
       */
      virtual void indicateDataHide(void);

      /**
       * window destruction handler.
       *
       * This method is called when the window is destroyed.  You don't
       * need to call GTK hide methods or stuff like that, because the
       * enclosing application will take care on these matters.  Just
       * finalize tasks you started that should be active as long as the
       * widget window does.  
       *
       * A typical use for this method is un-locking keyboard or mouse
       * mutexes that wait for the user.  If he closes the window
       * without giving any information, these mutexes will remain locked, 
       * blocking some other threads.  This method is the proper place to
       * un-lock them.
       */
      virtual void windowDestructionHandler();


      /**
       * this function is called when a mouse button is pressed and the mouse
       * moved.
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

      /**
       * this function is called when a mouse button is pressed.
       * @param button which button is pressed (0 means none,
       *               1 left button, 3 right button)
       * @param pos actual position of mouse pointer
       * @param shift true if the shift-key has been pressed while moving the
       *              mouse.
       * @param ctrl true if the control-key has been pressed while moving the
       *              mouse.
       * @param pressed true if the button was pressed, false if the button was
       *              released.
       */
      virtual void mouseButtonHandler(const int& button,
                                      const point& pos,
                                      const bool shift,
                                      const bool ctrl,
                                      const bool pressed);

      /**
       * this method is called when a key is pressed while the viewer window
       * is active.
       * @param shift true if the shift-key has been pressed while moving the
       *              mouse.
       * @param ctrl true if the control-key has been pressed while moving the
       *              mouse.
       * @param key integer value containing the key code.  You can find
       *            symbol definitions for the values used here in the file
       *            \<gdk/gdkkeysym.h\>
       */
      virtual void keyPressedHandler(const bool shift,
                                     const bool ctrl,
                                     const int key);

      /**
       * this method is called when a key is pressed while the viewer window
       * is active.
       * @param shift true if the shift-key has been pressed while moving the
       *              mouse.
       * @param ctrl true if the control-key has been pressed while moving the
       *              mouse.
       * @param key integer value containing the key code.  You can find
       *            symbol definitions for the values used here in the file
       *            \<gdk/gdkkeysym.h\>
       */
      virtual void keyReleasedHandler(const bool shift,
                                      const bool ctrl,
                                      const int key);

      /**
       * called when the status bar information needs to be updated
       */
      virtual void updateStatusBar(const point& pos)=0;
      //@}

      /**
       * last position in the image where the mouse was clicked
       */
      point lastPosition;

      /**
       * This flag indicates, if the lastPosition contains a valid
       * value, i.e. if the point lies within the displayed image
       */
      bool validLastPosition;

      /**
       * semaphore to wait until the user clicks on the window
       */
      mutable semaphore waitInteraction;

      /**
       * set to true, when the user waits for a mouse click
       */
      mutable bool waitingForClick;

      /**
       * last position in the image where the mouse was clicked
       */
      mutable int lastKeyCode;

      /**
       * set to true, when the user waits for a mouse click
       */
      mutable bool waitingForKey;
    };

    /**
     * Main Window for channels of fixed point values
     */
    class mainWndChannelFixed : public mainWindow {
    public:
      /**
       * Constructor
       */
      mainWndChannelFixed();

      /**
       * Copy constructor
       */
      mainWndChannelFixed(const mainWndChannelFixed& other);

      /**
       * Destructor
       */
      virtual ~mainWndChannelFixed();

      /**
       * clone this window
       */
      virtual mainGTKWindow* clone() const;

    protected:
      /**
       * @name Members to be reimplemented
       */
      //@{
      /**
       * This method is the one where your data should be drawn.
       * You just need to draw on the "theImage" attribute image.
       */
      virtual void dataToImage(image& img);

      /**
       * normal grey channel to image
       */
      void channelToImage();

      /**
       * labeled mask to image
       */
      void labelsToImage();

      /**
       * Prepare the parameters before the configuration dialog is started.
       *
       * This member gets some information of the displayed data into
       * the informational attributes of the parameters instance.
       * It is called just before opening the settings dialog, and is
       * the proper place to update the mentioned informational data.
       */
      virtual void prepareParameters();

      /**
       * called when the status bar information needs to be updated
       */
      virtual void updateStatusBar(const point& pos);
      //@}

    private:
      /**
       * this pointer equals always the theData pointer, and is
       * used to make unnecessary casting theData to the proper type each time
       */
      const matrix<ubyte>* chnl8;

      /**
       * this pointer equals always the theData pointer, and is
       * used to make unnecessary casting theData to the proper type each time
       */
      const matrix<int>* chnl32;
    };

    /**
     * Main Window for channels of floating point values
     */
    class mainWndChannelFloat : public mainWindow {
    public:
      /**
       * Constructor
       */
      mainWndChannelFloat();

      /**
       * Copy constructor
       */
      mainWndChannelFloat(const mainWndChannelFloat& other);

      /**
       * Destructor
       */
      virtual ~mainWndChannelFloat();

      /**
       * clone this window
       */
      virtual mainGTKWindow* clone() const;

    protected:
      /**
       * This method is the one where your data should be drawn.
       * You just need to draw on the "theImage" attribute image.
       */
      virtual void dataToImage(image& img);

      /**
       * Prepare the parameters before the configuration dialog is started.
       *
       * This member gets some information of the displayed data into
       * the informational attributes of the parameters instance.
       * It is called just before opening the settings dialog, and is
       * the proper place to update the mentioned informational data.
       */
      virtual void prepareParameters();

      /**
       * called when the status bar information needs to be updated
       */
      virtual void updateStatusBar(const point& pos);

    private:
      /**
       * Casted alias for theData
       */
      const matrix<float>* chnl;

      /**
       * Casted alias for theData
       */
      const matrix<double>* dchnl;
    };

    /**
     * Main Window for vectors
     */
    class mainWndVector : public mainWindow {
    public:
      /**
       * Constructor
       */
      mainWndVector();

      /**
       * Copy constructor
       */
      mainWndVector(const mainWndVector& other);

      /**
       * Destructor
       */
      virtual ~mainWndVector();

      /**
       * clone this window
       */
      virtual mainGTKWindow* clone() const;

    protected:
      /**
       * This method is the one where your data should be drawn.
       * You just need to draw on the "theImage" attribute image.
       */
      virtual void dataToImage(image& img);

      /**
       * Prepare the parameters before the configuration dialog is started.
       *
       * This member gets some information of the displayed data into
       * the informational attributes of the parameters instance.
       * It is called just before opening the settings dialog, and is
       * the proper place to update the mentioned informational data.
       */
      virtual void prepareParameters();

      /**
       * called when the status bar information needs to be updated
       */
      virtual void updateStatusBar(const point& pos);

    private:
      /**
       * casted alias
       */
      const vector<double>* dvct;

      /**
       * casted alias
       */
      const vector<float>* fvct;

      /**
       * casted alias
       */
      const vector<int>* ivct;

      /**
       * casted alias
       */
      const vector<ubyte>* uvct;
    };

    /**
     * Main Window for color images
     */
    class mainWndImage : public mainWindow {
    public:
      /**
       * Constructor
       */
      mainWndImage();

      /**
       * Copy constructor
       */
      mainWndImage(const mainWndImage& other);

      /**
       * Destructor
       */
      virtual ~mainWndImage();

      /**
       * clone this window
       */
      virtual mainGTKWindow* clone() const;
    protected:
      /**
       * This method is the one where your data should be drawn.
       * You just need to draw on the "theImage" attribute image.
       */
      virtual void dataToImage(image& img);

      /**
       * Prepare the parameters before the configuration dialog is started.
       *
       * This member gets some information of the displayed data into
       * the informational attributes of the parameters instance.
       * It is called just before opening the settings dialog, and is
       * the proper place to update the mentioned informational data.
       */
      virtual void prepareParameters();

      /**
       * called when the status bar information needs to be updated
       */
      virtual void updateStatusBar(const point& pos);

    private:
      /**
       * Casted alias for theData
       */
      const image* chnl;

    };

    /**
     * Ensure that the wnds array contains a valid window with the
     * actual parameters at each given position
     */
    void ensureWindow(const int newWnd);

    /**
     * The current used window (the index in the wnds vector
     */
    int wnd;

    /**
     * Number of different window types
     */
    static const int winTypes;

    /**
     * Window instance for channels of fixed point arithmetic
     */
    mainWindow* wnds[4]; // 4 == winTypes

    /**
     * returns used parameters as a read/write reference
     */
    parameters& getParameters();

    /**
     * This is the master who paints everything nice
     */
    viewerFunctor* master;
#endif

  };
}

#endif

