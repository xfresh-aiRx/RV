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
 * file .......: ltiGtkWidget.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 21.4.2000
 * revisions ..: $Id: ltiGtkWidget.h,v 1.26 2003/12/15 07:01:41 alvarado Exp $
 */

#ifndef _LTI_GTK_WIDGET_H_
#define _LTI_GTK_WIDGET_H_

#include "ltiConfig.h"

#ifdef HAVE_GTK

#include <gtk/gtk.h>
#include <list>
#include <string>

#include "ltiMutex.h"
#include "ltiSemaphore.h"

#include "ltiMath.h"

#include "ltiViewerBase.h"
#include "ltiGtkColorDialog.h"

/*
 * Standard gettext macros.
 */
// #ifdef ENABLE_NLS
// #  include <libintl.h>
// #  undef _
// #  define _(String) dgettext (PACKAGE, String)
// #  ifdef gettext_noop
// #    define N_(String) gettext_noop (String)
// #  else
// #    define N_(String) (String)
// #  endif
// #else
// #  define textdomain(String) (String)
// #  define gettext(String) (String)
// #  define dgettext(Domain,Message) (Message)
// #  define dcgettext(Domain,Message,Type) (Message)
// #  define bindtextdomain(Domain,Directory) (Domain)
// #  define _(String) (String)
// #  define N_(String) (String)
// #endif
#  define _(String) (String)

namespace lti {

  class mainGTKWindow;

  /**
   * widget for the statistics dialog
   */
  class configGTKDialog : public object {
  public:

    /**
     * create dialog
     */
    configGTKDialog();

    /**
     * destroy the dialog
     */
    virtual ~configGTKDialog();

    /**
     * Build the Dialog's GTK+ Widget Tree.
     *
     * You won't need to reimplement this methode, but insertWidget() if
     * you inherit from this class to provide more options.
     */
    void buildDialog();

    /**
     * show the dialog
     */
    void show();

    /**
     * hide the dialog
     */
    void hide();

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
    virtual bool useParameters(viewerBase::parameters& theParam);

    /**
     * set the parent of this dialog to tell it when everything is done!
     */
    void whereToCallBack(mainGTKWindow* obj);

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
     * You will usually want to overload this member.  Here you
     * will usually want to call appendFrame(buildCommonFrame()) followed by
     * all methods where you build your pages.
     *
     * The default implementation will just add the common page, with
     * the image type and size.
     * You could call the parent class method to do this, but is usually
     * not done to avoid dealing with a MS VC++ bug.
     *
     * If you want to add some pages, you will need to create the
     * desired GTK Widget and append it to the current dialog frames
     * using appendFrame().
     *
     * The best way doing so is implementing methods buildABCFrame,
     * where ABC should be replaced with a meaningful name.  These
     * methods should return an GTKWidget pointer, that can be
     * appended to the pages list.
     * Following example illustrates this concept:
     *
     * \code
     * class myViewer : public viewerBase {
     *   // ... implementation of parameters, etc. ...
     *   class configDialog : public configGTKDialog {
     *   public:
     *     // default constructor
     *     configDialog() : configGTKDialog() {};
     *
     *     // build my page
     *     GTKWidget* buildMyFrame() {
     *       GtkWidget* w;        // this object will be returned
     *
     *       // create the GTK widget
     *       w = gtk_vbox_new (FALSE, 0);
     *       // ...
     *
     *       return w;
     *     };
     *
     *     // insert frames
     *     virtual void insertFrames() {
     *       // append first the common dialog (from the parent class)
     *       appendFrame(buildCommonFrame());
     *       // append my page
     *       appendFrame(buildMyFrame());
     *     }
     *   }
     * };
     * \endcode
     *
     * This method will insert the pages in an internal list, that will
     * be read in the buildDialog() method.
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

  protected:
    /**
     * Build a page in the main settings dialog.
     *
     * @return a GtkWidget for the common settings.
     */
    GtkWidget* buildCommonFrame();

    /**
     * copy src to dest and allocates memory for dest
     */
    void newStrCopy(char* &dest, const char* src);

    /**
     * convert a double in an ascii string
     */
    std::string toString(const double& value,const int& dec = 4);

    /**
     * especial case of toString from points
     */
    std::string toString(const point& pt);

    /**
     * especial case of toString from rgbPixel
     */
    std::string toString(const rgbPixel& px);

    /**
     * especial case of toString from rgbPixel
     */
    std::string toString(const trgbPixel<float>& px, const int dec=4);

    /**
     * convert an ascii string into a double value
     */
    double fromString(const char* value);

    /**
     * Pointer to the parent mainWindow
     */
    mainGTKWindow* theMainWindow;

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
    viewerBase::parameters* param;

    /**
     * append a page in the pages list.
     */
    void appendFrame(GtkWidget* pw);

    /**
     * copy the parameters to the mainWindow
     * and calls redraw method
     */
    virtual void redrawMainWindow();

    /**
     * @name GTK+ Callbacks
     * Callback functions created by Glade
     */
    //@{
    static void on_fileselector_ok(GtkButton* button,
                                   gpointer user_data);

    static void on_browse_clicked(GtkButton *button,
                                  gpointer user_data);

    static void on_save_button_clicked(GtkButton *button,
                                       gpointer user_data);

    static void on_ok_button_clicked(GtkButton *button,
                                     gpointer user_data);

    static void on_apply_button_clicked(GtkButton *button,
                                        gpointer user_data);

    static void on_cancel_button_clicked(GtkButton *button,
                                        gpointer user_data);

    /**
     * called when the user selects the "close window" button on the window
     */
    static gint delete_event(GtkWidget *widget,
                             GdkEvent  *event,
                             gpointer   data );

    //@}

  protected:

    /**
     * @name GTK+ Objects
     * GTK Widgets
     */
    //@{

    /**
     * The dialog widget
     */
    GtkWidget *configGTKWidget;

    /**
     * The box containing everything
     */
    GtkWidget *dialogBox;

    /**
     * The box with the boxes for configuration (pages will be inserted here
     */
    GtkWidget *ConfigFrame;

    /**
     * Box containing filename, browse and save buttons
     */
    GtkWidget *savingFrame;

    /**
     * Editbox with the filename
     */
    GtkWidget *filename;

    /**
     * Browse button
     */
    GtkWidget *browse;

    /**
     * Save button
     */
    GtkWidget *save_button;

    /**
     * Box containing all usual buttons (OK, Cancel, Apply)
     */
    GtkWidget *buttonsFrame;

    /**
     * Ok button
     */
    GtkWidget *ok_button;

    /**
     * Apply button
     */
    GtkWidget *apply_button;

    /**
     * Cancel button
     */
    GtkWidget *cancel_button;

    /**
     * Help messages (tool-tips)
     */
    GtkTooltips *tooltips;

    /**
     * file selector
     */
    GtkWidget* filew;

    /**
     * color selection dialog
     */
    colorDialog* colorSelector;

    //@}

  };

  /**
   * This class encapsulates all signals and calls of the GTK library
   */
  class mainGTKWindow : public object {
  public:
    /**
     * default constructor
     */
    mainGTKWindow(bool setConfigDlg=true);

    /**
     * copy constructor
     */
    mainGTKWindow(const mainGTKWindow& other);

    /**
     * destructor
     */
    virtual ~mainGTKWindow();

    /**
     * initialize values for the constructor
     */
    void init(void);

    /**
     * set the display parameters
     */
    virtual bool useParameters(viewerBase::parameters& par);

    /**
     * update geometry the next time
     */
    virtual void changeGeometryRequest(const bool request=true);

    /**
     * similar to drawData, but used by other widgets (like the
     * configuration dialog) to force new image display
     */
    virtual bool redraw();

    /**
     * generate an event which will show the data
     */
    virtual bool drawData();

    /**
     * generate an event which will hide the data
     */
    virtual void hideData();

    /**
     * creates new window
     */
    void createWindow();

    /**
     * set position of the window
     */
    void setPosition(const point& p,const bool withinGtkThread);

    /**
     * get position of the window
     */
    point getPosition() const;

    /**
     * set size of the window
     */
    void setSize(const point& p,const bool withinGtkThread);

    /**
     * get size of the window
     */
    point getSize() const;

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
    mainGTKWindow& copy(const mainGTKWindow& other);

    /**
     * clone this window
     */
    virtual mainGTKWindow* clone() const;

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
     * You just need to draw on the "theImage" attribute image.
     */
    virtual void dataToImage(image& img);

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
    virtual void prepareParameters();

    /**
     * Indicate if there are valid data to show.
     *
     * This method must check if the provided data is ready
     * to be shown, i.e. if the dataToImage() can be already
     * be used to draw the data.
     */
    virtual bool validData();

    /**
     * this method will be called to invalidate the data.
     *
     * Usually will set a few pointers to NULL to indicate
     * that the window has been hidden, and no valid data is
     * keeped in this class any more.
     */
    virtual void indicateDataHide();

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
     * this method is called when a mouse button is pressed and the mouse
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
     * this method is called when a mouse button is pressed.
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
     * this method is called when a key is released while the viewer window
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

    //@}


    /**
     * @name Tools and information flags
     */
    //@{
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

    /**
     * write the given string into the status bar
     */
    void setStatusBar(const char* str);

    //@}

    /**
     * the viewer parameters.  This is just a pointer to an external
     * instance.  This class will never delete or clone the pointed object
     */
    viewerBase::parameters* param;

    /**
     * the statistics and displaying options dialog
     */
    configGTKDialog* options;

    /**
     * extra width needed for the scrollbars
     */
    static const point borderWidth;

    /**
     * the data which is been actually displayed.
     */
    image theImage;

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
     * generate xbuffer from an image
     */
    void showXbuffer(GtkWidget *widget);

    /**
     * name of this window!
     */
    std::string winName;

    /**
     * size of the window
     */
    point winSize;

    /**
     * position of the window
     */
    point winPosition;

    /**
     * geometry change request from user
     */
    bool geometryChangeRq;

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
     * called by createWindow to create a window
     */
    void createWindowLocal();

    /**
     * creates new window timeout (this will be called when the time
     * has come to create the window).
     */
    static gint createWindowTimeout(gpointer data);

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
     * callback function called when the window size or position are
     * changed.
     */
    static gint geometryChanged(GtkWidget* widget,
                                GdkEventConfigure* event,
                                gpointer data);

    /**
     * method called by mouseMovedEvent() when the mouse has been moved.
     *
     * This will show on the status bar the mouse pointer position and
     * the value of the image at this position.
     */
    void mouseMovedEventLocal(GtkWidget* widget,
                              GdkEventMotion* event);

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
     * function called when any key has been pressed.
     */
    static gint keyPressedEvent(GtkWidget* widget,
                                GdkEventKey* event,
                                gpointer data);

    /**
     * function called when any key has been pressed.
     */
    virtual void keyPressedEventLocal(GtkWidget* widget,
                                      GdkEventKey* event);

    /**
     * function called when any key has been pressed.
     */
    static gint keyReleasedEvent(GtkWidget* widget,
                                GdkEventKey* event,
                                gpointer data);

    /**
     * function called when any key has been pressed.
     */
    virtual void keyReleasedEventLocal(GtkWidget* widget,
                                      GdkEventKey* event);

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
}

#endif

#endif

