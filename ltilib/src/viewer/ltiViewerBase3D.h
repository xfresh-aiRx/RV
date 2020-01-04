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
 * file .......: ltiViewerBase3D.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 1.12.2001
 * revisions ..: $Id: ltiViewerBase3D.h,v 1.24 2005/06/16 12:37:55 doerfler Exp $
 */

#ifndef _LTI_VIEWER_BASE_3D_H_
#define _LTI_VIEWER_BASE_3D_H_

#include "ltiConfig.h"

#ifdef HAVE_GTK

#include "ltiImage.h"
#include "ltiViewerBase.h"
#include "ltiMutex.h"
#include "ltiSemaphore.h"
#include "ltiDraw3D.h"

#include <gtk/gtk.h>
#include "ltiGtkColorDialog.h"


namespace lti {

  /**
   * \page viewer3D Creating new viewers for 3D data
   *
   * Even if we have tried to create simple ways to inherit the 3D
   * manipulation functionality for 3D viewers, it is necessary to
   * explain the steps required when creating new viewer classes.
   *
   * All classes inherited from viewerBase3D contain three enclosed classes
   * (which can of course be inherited from parent classes):
   * viewerBase3D::parameters, viewerBase3D::configDialog and
   * viewerBase3D::mainWindow
   *
   * \section v3Dparams Classes parameters and configDialog
   *
   * Usually, each new viewer will require new parameters that specify
   * different display modalities (like show coordinate system,
   * display data with lines or points, etc.), and the user usually
   * wants to set them interactively using some dialogs. All new
   * parameters added should be encapsulated in an enclosed class with the
   * name "parameters" in your new %viewer class.  These %parameters classes
   * are similar to the lti::functor::parameters, but they include also
   * some special attributes used to tranfer some information about the
   * data being currently displayed to the configuration dialogs(see also
   * method lti::viewerBase3D::mainWindow::prepareParameters() ).  They will
   * implement the typical parameters interface: (read, write,
   * getTypeName, copy, clone, and so on).
   *
   * The dialog used to set this parameters is implemented also in an
   * enclosed class named "configDialog".  One of the most important
   * members in this class is
   * lti::viewerBase3D::configDialog::insertPages, which will insert
   * the a new settings page into the default dialog.  You will
   * usually want to add the common dialog with the line
   *
   * \code
   * appendPage(buildCommonPage());
   * \endcode
   *
   * For your own parameter configuration pages you can create methods like
   *
   * \code
   *   configDialog::pageWidget buildMyOwnPage() {
   *   pageWidget pw;        // this object will be returned
   *   pw.label = "my page"; // the name of the widget
   *
   *   // create the GTK widget
   *   GtkWidget* theWidget = gtk_vbox_new (FALSE, 0);
   *   // ...
   *
   *   pw.widget = theWidget;
   *   return pw;
   * };
   * \endcode
   *
   * The other two methods will copy the data from the parameters into
   * the dialog or otherwise.  They are called
   * lti::viewerBase3D::configDialog::getDialogData() and
   * lti::viewerBase3D::configDialog::setDialogData().
   *
   * Of course, you don't need to reimplement this two classes if you don't
   * have any new parameters in your own viewer.
   *
   * \section v3Dmainwnd mainWindow
   *
   * The main window administrates the real viewer, i.e. the window where
   * all data is drawn.  It also checks for all important events, like
   * right-mouse button pressed to show the configuration dialog, and
   * drag of mouse to change the  3D camera parameters.
   *
   * The method lti::viewerBase3D::mainWindow::dataToImage() will draw
   * the data using the internal lti::draw3D object
   * (lti::viewerBase3D::mainWindow::drawTool) into the proper image.
   * You can use here and in other methods the attributes
   * lti::viewerBase3D::mainWindow::mouseButtonPressed and
   * lti::viewerBase3D::mainWindow::lastMousePos in order to check if
   * you want to draw your data in a way or another, depending on how
   * the user interacts with the viewer.  For example, if the user is
   * dragging the camera position, zooming, etc.  He has the mouse
   * button pressed (mouseButtonPressed!=0), and you can draw a
   * preview of the data to allow a faster interation.
   *
   * When you request the configuration dialog (usually pressing the
   * right mouse button), you will need to transfer some informational data
   * from the main window to the configuration dialog.  This is done through
   * the informational attributes of the parameters-class, which can be filled
   * within the method lti::viewerBase3D::mainWindow::prepareParameters().
   *
   * \section v3Dmethods Important viewerBase3D methods
   *
   * The inheritance from viewerBase3D implies the implementation of following
   * methods:
   *
   * - lti::viewerBase3D::getNewMainWindow().  Here you will need to return
   *   a new instance of the proper mainWindow class, i.e. just type a
   *   <code>return new mainWindow()</code>.
   * - show(const T& yourData).  These methods should copy the data you
   *   want to display, into the mainWindow instance.  To ensure that
   *   the proper instance exist, you need to call first
   *   lti::viewerBase3D::ensureMainWindowInstance(), after that you
   *   can access your mainWindow through the attribute wnd.  At the
   *   end of your method you can call show() to start drawing the data.
   *   To protect the multiple access to the data in the mainWindow,
   *   you can also lock the access at the begin and end.
   *   A typical show method will look this way:
   *   \code
   *   bool myViewerClass::show(const myDataType& data) {
   *     if (ensureMainWindowInstance()) {
   *
   *       // cast wnd to the proper mainWindow class
   *      mainWindow* window = dynamic_cast<mainWindow*>(wnd);
   *
   *       if (notNull(window)) {
   *         window->dolock();
   *
   *         // transfer your data to wnd
   *         window->putData(data);  // assume your mainWindow class has a
   *                                 // method called putData
   *         window->unlock();
   *         return viewerBase3D::show();
   *       }
   *     }
   *     return false;
   *   }
   *   \endcode
   */

  /**
   * This is the base class for all %viewer classes which show 3D data.
   *
   * It supports the manipulation of the 3D camera parameters using
   * the mouse, and provides all necessary interfaces that simplify
   * subclasses accessing configuration dialogs.
   *
   * @see \ref viewer3D
   */
  class viewerBase3D : public viewerBase {
  public:
    /**
     * The parameters for the class viewerBase3D.
     *
     * The parameters for viewer classes have two functions:
     *
     * First, the allow the user to indicate display options, like if
     * he wants to show the system axes.  This is much like the %parameters
     * functionality for all functors in the ltilib.
     *
     * Second, the viewers have usually dialogs, that allow users to
     * specify the desired options interactivelly (while displaying
     * data).  It is often usefull to get in that dialog some
     * information about the image or data currently being displayed.  This
     * information is provided to the configuration dialog using some
     * attributes in the parameters class.  (this may be changed in the near
     * future)
     *
     * @see \ref viewer3D
     */
    class parameters : public viewerBase::parameters {
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
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      virtual parameters& operator=(const parameters& other);


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
       * init filename (with path) for saving
       *
       * Default: image.png
       */
      std::string saveName;

      /**
       * Color of the background.
       *
       * Default: lti::Black (lti::rgbPixel(0,0,0))
       */
      rgbPixel backgroundColor;

      /**
       * Color of the axis, if the corresponding <code>mode</code> is
       * specified for a fixed axis color.
       *
       * Default: lti::White (lti::rgbPixel(255,255,255))
       */
      rgbPixel axisColor;

      /**
       * Size of the viewer window.
       *
       * Default: point(256,256)
       */
      point windowSize;

      /**
       * If true, the given color for the axis will be used.  Otherwise
       * gradients for red, green and blue will be used.
       *
       * Default: true
       */
      bool axisColorFixed;

      /**
       * camera parameters for the 3D graphics.
       *
       * Defaut values: camPos = computed from azimuth, elevation, camRadius
       *                         and camTarget.
       *                azimuth = 45deg (45*Pi/180 radians)
       *                elevation = 60deg (60*Pi/180 radians)
       *                center = (0.5,0.5);
       *                perspective = 0.002;
       *                zoom = 0.5;
       */
      draw3D<rgbPixel>::parameters camParameters;

      /**
       * Point in the 3D space to which the camera always look at.
       *
       * Default value: (128,128,128)
       */
      dpoint3D camTarget;

      /**
       * Distance from the camera position to the camera origin
       *
       * Default value: 200
       */
      double camRadius;
    };

    /**
     * default constructor
     */
    viewerBase3D(bool createDefaultParams = true);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    viewerBase3D(const viewerBase3D& other);

    /**
     * constructor with windows title
     */
    viewerBase3D(const std::string& title);

    /**
     * destructor
     */
    virtual ~viewerBase3D();

    /**
     * returns the name of this type ("viewerBase3D")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    viewerBase3D& copy(const viewerBase3D& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    viewerBase3D& operator=(const viewerBase3D& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual viewerBase* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * returns used parameters
     */
    parameters& getParameters();

    /**
     * shows a color image.
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const image& data) {return false;};

    /**
     * shows a 8-bit channel
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const channel8& data) {return false;};

    /**
     * shows a channel or matrix of float
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const channel& data) {return false;};

    /**
     * shows a channel or matrix of float
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const matrix<float>& data) {return false;};

    /**
     * shows a vector of double
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const vector<double>& data) {return false;};

    /**
     * shows a vector of double
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const vector<float>& data) {return false;};

    /**
     * shows a vector of double
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const vector<int>& data) {return false;};

    /**
     * shows a matrix of doubles as a channel
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const matrix<double>& data) {return false;};

    /**
     * shows a matrix of integers as a channel
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const matrix<int>& data) {return false;};

    /**
     * hides the display window
     * @return true if successful, false otherwise.
     */
    virtual bool hide() {return false;};

    /**
     * take a snapshot of the current visualized image.
     *
     * The image being shown will be saved in a file with the given name.
     * All data types supported by lti::saveImage functor will be accepted.
     */
    virtual bool snapshot(const std::string& filename) const;

    /**
     * set the parameters to be used
     */
    virtual bool setParameters(const viewerBase::parameters& param);

  protected:
    /**
     * shows the data.
     *
     * The tasks done by this method are:
     * - ensure the existence of a mainWindow instance.
     * - indicate the mainWindow to draw the new data.
     * @return true if successful, false otherwise.
     */
    virtual bool show();

# ifdef _LTI_MSC_6
  public:
# endif

    class mainWindow;

    /**
     * GTK Widget for the configuration dialog of the histogram viewer.
     *
     * You will usually reimplement the methods insertPages(), setDialogData(),
     * getDialogData(), default constructor (which call the constructor of
     * the parent class) and destructor (which destroys the whole GTK stuff).
     *
     * @see \ref viewer3D
     */
    class configDialog : public object {
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

      /**
       * Build the Dialog's GTK+ Widget Tree.
       *
       * You won't need to reimplement this methode, but insertPages().
       */
      void buildDialog();

      /**
       * show the dialog
       */
      virtual void show();

      /**
       * hide the dialog
       */
      virtual void hide();

      /**
       * Indicate which parameters object should be used.
       *
       * This class will not administrate the memory management of the
       * object.  It will just keep a reference to it.  If deleted externally
       * an error will occur.
       *
       * You should also ensure that this method is called before showing
       * the dialog for the first time, or all data will be uninitialized!
       *
       * @return true if successful, false otherwise.
       */
      virtual bool useParameters(parameters& theParam);

      /**
       * set the parent of this dialog to tell it when everything is done!
       */
      void whereToCallBack(mainWindow* obj);

      /**
       * @name Virtual members to be reimplemented
       *
       * Following members should be reimplemented in your derived class
       * if you add new dialog options.
       */
      //@{

      /**
       * Inserts all pages in the main settings dialog.
       * @see \ref viewer3D
       *
       * You will usually want to overload this member.  Here you
       * will usually want to call appendPage(buildCommonPage()) followed by
       * all methods where you build your pages.
       *
       * The default implementation will just add the common page, with
       * the camera positioning, zoom, perspective and similar settings.
       * You could call the parent class method to do this, but is usually
       * not done to avoid dealing with a MS VC++ bug.
       *
       * If you want to add some pages, you will need to create the
       * desired GTK Widget, and give a name for it, assign both
       * values to a pageWidget structure and append it to the
       * current list of pages using appendPage().
       *
       * The best way doing so is implementing methods buildABCPage,
       * where ABC should be replaced with a meaningful name.  These
       * methods should return an object pageWidget, that can be
       * appended to the pages list.
       * Following example illustrate this concept:
       *
       * \code
       * class myViewer : public viewerBase3D {
       *   // ... implementation of parameters, etc. ...
       *   class configDialog : public viewerBase3D::configDialog {
       *   public:
       *     // default constructor
       *     configDialog() : viewerBase3D::configDialog() {};
       *
       *     // build my page
       *     pageWidget buildMyPage() {
       *       pageWidget pw;        // this object will be returned
       *       pw.label = "my page"; // the name of the widget
       *
       *       // create the GTK widget
       *       GtkWidget* theWidget = gtk_vbox_new (FALSE, 0);
       *       // ...
       *
       *       pw.widget = theWidget;
       *       return pw;
       *     };
       *
       *     // insert pages
       *     virtual void insertPages() {
       *       // append first the common dialog (from the parent class)
       *       appendPage(buildCommonPage());
       *       // append my page
       *       appendPage(buildMyPage());
       *     }
       *   }
       * };
       * \endcode
       *
       * This method will insert the pages in an internal list, that will
       * be read in the buildDialog() method.
       */
      virtual void insertPages();

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
      //@}

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
       * Structure used in the elements of the list of all
       * pages of the configuration dialog.
       */
      struct pageWidget {
        GtkWidget* widget;
        std::string label;
      };

    protected:

      /**
       * Build a page in the main settings dialog.
       *
       * @return a pageWidget for the common settings.
       */
      pageWidget buildCommonPage();

      /**
       * copy src to dest and allocates memory for dest
       */
      void newStrCopy(char* &dest, const char* src);

      /**
       * convert a double in an ascii string
       */
      std::string toString(const double& value,const int& dec = 4);

      /**
       * convert an ascii string into a double value
       */
      double fromString(const char* value);

      /**
       * Pointer to the parent mainWindow
       */
      mainWindow* theMainWindow;

      /**
       * pointer to the used viewer parameters.
       *
       * This class should NEVER delete or create this object,
       * because is just a reference to external data.
       *
       * The methods should check if it points to valid data, i.e.
       * if the pointer points to the right type (check it with
       * dynamic_cast), or if the pointer is null.
       */
      parameters* param;

      /**
       * userdef background color
       *
       * Default: lti::Black (lti::rgbPixel(0,0,0))
       */
      rgbPixel userdefBackcolor;

      /**
       * append a page in the pages list.
       */
      void appendPage(pageWidget pw);

      /**
       * holds the the widges and the names
       * for special pages in the settings dialog
       */
      std::list<pageWidget> pageList;

      /**
       * copy the parameters to the mainWindow
       * and calls redraw method
       */
      virtual void redrawMainWindow(void);

      /**
       * @name GTK+ Callbacks
       * Callback functions created by Glade
       */
      //@{

      //mainDialog
      static void on_ok_clicked(GtkButton *button,
                                gpointer user_data);

      static void on_cancel_clicked(GtkButton *button,
                                    gpointer user_data);

      static void on_apply_clicked(GtkButton *button,
				   gpointer user_data);

      static void on_chooseFilename_clicked(GtkButton *button,
					    gpointer user_data);

      static void chooseFilename_callback(GtkWidget *widget,
					  gpointer data);

      static void on_save_clicked(GtkButton *button,
                                  gpointer user_data);

/*        static gint on_close_clicked(GtkButton *button, */
/*  				   gpointer user_data); */

      // specialPage
      static void on_backBlack_toggled(GtkToggleButton *togglebutton,
				       gpointer user_data);

      static void on_backGrey_toggled(GtkToggleButton *togglebutton,
				      gpointer user_data);

      static void on_backWhite_toggled(GtkToggleButton *togglebutton,
				       gpointer user_data);

      static void on_backUserdef_toggled(GtkToggleButton *togglebutton,
					 gpointer user_data);

      static void on_backChoose_clicked(GtkButton *button,
					gpointer user_data);

      static void on_axisGradient_toggled(GtkToggleButton *togglebutton,
                                      gpointer user_data);

      static void on_axisFixed_toggled(GtkToggleButton *togglebutton,
                                   gpointer user_data);

      static void on_axisChoose_clicked(GtkButton *button,
                                     gpointer user_data);

      static void on_sizex_changed(GtkEditable *editable,
                                   gpointer user_data);

      static void on_sizey_changed(GtkEditable *editable,
                                   gpointer user_data);

      static void on_centerx_changed(GtkEditable *editable,
                                     gpointer user_data);

      static void on_centery_changed(GtkEditable *editable,
                                     gpointer user_data);

      static void on_camx_changed(GtkEditable *editable,
                                  gpointer user_data);

      static void on_camy_changed(GtkEditable *editable,
                                  gpointer user_data);

      static void on_camz_changed(GtkEditable *editable,
                                  gpointer user_data);

      static void on_elevation_changed(GtkEditable *editable,
                                       gpointer user_data);

      static void on_azimuth_changed(GtkEditable *editable,
                                     gpointer user_data);

      static void on_zoom_changed(GtkEditable *editable,
                                  gpointer user_data);

      static void on_perspective_changed(GtkEditable *editable,
                                         gpointer user_data);
      //@}

    protected:

      /**
       * @name GTK+ Objects
       * GTK Widgets
       */
      //@{

      /**
       * Tooltip: explains parameters
       */
      GtkTooltips *tooltips;

      /**
       * main dialog
       */
      GtkWidget *settings;

      /**
       * file selector
       */
      GtkWidget* filew;

      /**
       * color selection dialog
       */
      colorDialog* colorSelector;

      /**
       * combobox for save-file-names
       */
      GtkWidget* comboFilename;

      /**
       * pointerlist to strings, which saves all
       * save-file-names
       */
      GList* comboNamelist;

      /**
       * toggle buttons for backgroundcolor
       */
      GtkWidget* backBlack;
      GtkWidget* backGrey;
      GtkWidget* backWhite;
      GtkWidget* backUserdef;

      /**
       * x,y windows size
       */
      GtkWidget* sizex;
      GtkWidget* sizey;

      /**
       * kind(color) of coodinate axis
       */
      GtkWidget* axisGradient;
      GtkWidget* axisFixed;

      /**
       * view parameters
       */
      GtkWidget* elevation;
      GtkWidget* azimuth;
      GtkWidget* zoom;
      GtkWidget* perspective;

      /**
       * center position
       */
      GtkWidget* centerx;
      GtkWidget* centery;

      /**
       * camera position
       */
      GtkWidget* camx;
      GtkWidget* camy;
      GtkWidget* camz;

      //@}
    };

    /**
     * Widget for the main class of the histogram viewer.
     *
     * This class administrates the real viewer window.  It provides
     * the interface with which the data will be drawn.
     * For this task you will usually reimplement in your inherited class
     * the methods dataToImage() and prepareParameters().
     *
     * In your own classes, you should provide methods to set the
     * data required in dataToImage().  These methods will be used
     * in your %myviewer::show(const classtype& data) methods, to set
     * the data that must be displayed.
     *
     * @see \ref viewer3D
     */
    class mainWindow : public object {
    public:
      /**
       * default constructor.
       *
       * @param setConfigDlg if true (default) the configuration
       *                     dialog will be created and initialized.  Otherwise
       *                     it will be left empty
       */
      mainWindow(bool setConfigDlg = true);

      /**
       * copy constructor
       */
      mainWindow(const mainWindow& other);

      /**
       * destructor
       */
      ~mainWindow();

      /**
       * initialize values for the constructor
       */
      void init(void);

      /**
       * indicate which parameters instance should be used.
       */
      virtual bool useParameters(parameters& theParam);

      /**
       * similar to draw data, but used by other widgets to force image
       * display
       */
      virtual void redraw();

      /**
       * generate an event which will show the data
       */
      void drawData();

      /**
       * generate an event which will hide the data
       */
      void hideData();

      /**
       * creates new window
       */
      void createWindow();

      /**
       * destroy the window
       */
      void destroyWindow();

      /**
       * return a lti::image with the data being displayed.
       *
       * Used by the save image routines!
       */
      const image& getDisplayedData() const;

      /**
       * copy this window (only the actual data will be copied.
       * The display queue will not be copied!
       */
      mainWindow& copy(const mainWindow& other);

      /**
       * clone this window
       */
      mainWindow* clone() const;

      /**
       * lock internal data
       */
      void lock();

      /**
       * unlock internal data
       */
      void unlock();

    protected:
      /**
       * @name Members to be reimplemented
       */
      //@{
      /**
       * This method is the one where your data should be drawn.
       *
       * When called, you can assume that the <code>drawTool</code>
       * attribute of this class has already been initialized with the
       * proper parameters, the required canvas image has already been set
       * with the proper size and cleaned with the proper background color.
       *
       */
      virtual void dataToImage();

      /**
       * Prepare the parameters before the settings dialog is started.
       *
       * This member gets some information of the displayed data into
       * the informational attributes of the parameters instance.
       * It is called just before opening the settings dialog, and is
       * the proper place to update the mentioned informational data.
       */
      virtual void prepareParameters();

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

      //@}

      /**
       * @name Tools and information flags
       */
      //@{
      /**
       * draw the coordinate system and an auxiliary box of the given size
       * @param size axis size
       * @param auxBox if true, an auxiliary box will be drawn.
       */
      void draw3DAxis(const double size,const bool auxBox = true);

      /**
       * mouse being pressed.
       *
       * This attribute will contain always the status of the mouse
       * buttons.  The values will usually be:
       * \code
       * Nothing      : 0
       * Left button  : 1
       * Middle button: 2
       * Right button : 3
       * Wheel Up     : 4
       * Wheel Down   : 5
       * \endcode
       */
      int mouseButtonPressed;

      /**
       * last position of the mouse in the window.
       */
      point lastMousePos;
      //@}

      /**
       * the viewer parameters.  This is just a pointer to an external
       * instance.  This class will never delete or clone the pointed object
       */
      parameters* param;

      /**
       * the statistics and displaying options dialog
       */
      configDialog* options;

      /**
       * extra width needed for the scrollbars
       */
      static const point borderWidth;

      /**
       * @name Drawing Members.
       *
       * These members are used usually in your dataToImage method().
       */
      //@{
      /**
       * drawTool object
       */
      draw3D<rgbPixel> drawTool;

      /**
       * the data which is been actually displayed.  This is the
       * canvas used in the drawTool object.
       */
      image theImage;
      //@}

      /**
       * if a image is being shown, this attribute is true
       */
      bool showingBuffer;

      /**
       * if the viewer is computing the image to be shown, this
       * attribute is true
       */
      bool busy;

      /**
       * data lock.  Used to protect the data against parallel access
       */
      mutex theLock;

      /**
       * semaphore used to synchronize destruction of the widgets
       */
      semaphore syncEnd;

      /**
       * if the data has changed, this variable is true
       */
      bool dataChanged;

      /**
       * buffer of image being displayed!
       */
      guchar* xbuffer;

      /**
       * number of rows of the xbuffer
       */
      int xbuffer_rows;

      /**
       * number of columns of the xbuffer
       */
      int xbuffer_cols;

      /**
       * generate xbuffer from an image
       */
      void generateXbuffer(const image& img,
                           GtkWidget *widget,
                           GdkEventExpose *event);

      /**
       * name of this window!
       */
      std::string winName;

      /**
       * size of the window
       */
      point winSize;

      // GTK Internals

      /**
       * timer tag used in this viewer window
       */
      gint timerTag;

      /**
       * @name GTK+ Widgets
       */
      //@{

      /**
       * the main dialog window
       */
      GtkWidget *window;

      /**
       * the scrolled window
       */
      GtkWidget *scrolledWindow;

      /**
       * the status bar
       */
      GtkWidget *statusBar;

      /**
       * context id of the status bar
       */
      guint statusBarContextId;

      /**
       * drawing area that holds the image
       */
      GtkWidget *darea;

      /**
       * event box to capture mouse and button events
       */
      GtkWidget *eventBox;

      /**
       * convert the three color components red, green and blue in a guint32
       * used by the gtk-color maps.
       * @param r red component (between 0 and 255)
       * @param g green component (between 0 and 255)
       * @param b blue component (between 0 and 255)
       * @return an integer value used by gtk-color maps
       */
      guint32 rgbToInt(const int& r,const int& g,const int& b) const;
      //@}

      /**
       * unpack an gtk-integer into a rgb pixel
       */
      rgbPixel intToRgb(const guint32& rgb) const;

      /**
       * @name GTK+ callback functions
       */
      //@{

      /**
       * called when the user selects the "close window" button on the window
       */
      static gint delete_event(GtkWidget *widget,
                               GdkEvent  *event,
                               gpointer   data );


      /**
       * timeout gtk-callback function, called when the window must be
       * destroyed.
       * This static function just call the instance member destroyWindow().
       */
      static gint destroyWndTimeout(gpointer data);

      /**
       * call-back funtion to indicate the end of the whole gtk-application.
       * This function will end the gtk main-event-loop!
       */
      static void destroy(GtkWidget *widget,
                          gpointer   data );


      /**
       * timeout gtk-callback function, called when a new image must be
       * displayed!
       *
       * This function will stop the timeout automatically, if there are
       * no more images or objects to be displayed.
       */
      static gint show(gpointer   data);

      /**
       * timeout gtk-callback function, called when a new image must be
       * hide!
       */
      static gint hide(gpointer   data);

      /**
       * timeout gtk-callback function, called when the window must be
       * created.
       *
       * This function just calls createWindowLocal()
       */
      static gint createWindowTimeout(gpointer data);

      /**
       * called by createWindowTimeout to create a window
       */
      void createWindowLocal();

      /**
       * callback function called when the mouse is moved and a mouse
       * button has been pressed.
       *
       * This function just calls the mouseMovedEventLocal()
       */
      static gint mouseMovedEvent(GtkWidget* widget,
                                GdkEventMotion* event,
                                gpointer data);

      /**
       * function called by mouseMovedEvent() when the mouse has been moved.
       *
       * This will show on the status bar the mouse pointer position and
       * the value of the image at this position.
       */
      void mouseMovedEventLocal(GtkWidget* widget,
                                GdkEventMotion* event);

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


#   ifdef _LTI_MSC_6
    public:
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
    protected:
#   endif

      /**
       * function called when any mouse button has been pressed.
       */
      static gint buttonPressedEvent(GtkWidget* widget,
                                     GdkEventButton* event,
                                     gpointer data);

      /**
       * called by buttonPressedEvent()
       */
      virtual void buttonPressedEventLocal(GtkWidget* widget,
					   GdkEventButton* event);


      /**
       * callback function called when a key is pressed or released
       */
      static gint keyboardEvent(GtkWidget* widget,
                                GdkEventKey* event,
                                gpointer data);

      /**
       * show the data in the status bar
       */
      void showCameraPosition();

      /**
       * gtk-callback funtion to redraw the image been displayed.
       *
       * This function just calls the dareaExposeLocal()
       */
      static gboolean dareaExpose (GtkWidget      *widget,
                                   GdkEventExpose *event,
                                   gpointer        data);

      /**
       * This function redraws the image been displayed
       */
      virtual gboolean dareaExposeLocal  (GtkWidget      *widget,
					  GdkEventExpose *event);

      //@}
    };

# ifdef _LTI_MSC_6
  protected:
# endif


    /**
     * the main window.
     *
     * (have to defined after inhert the mainWindow class)
     */
    mainWindow* wnd;

    /**
     * return a new instance of the mainWindow class.
     *
     * This class must be reimplemented in order to get the
     * proper class instance.
     */
    virtual mainWindow* getNewMainWindow() const;

    /**
     * ensure that a mainWindow instance exist.
     *
     * You usually do not require to reimplement this method.
     *
     * @return true if everything is ok, false, if the mainWindow
     *         do not exist and it could not be created.
     */
    virtual bool ensureMainWindowInstance();

  };
}

#endif
#endif

