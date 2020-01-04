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
 * file .......: ltiScene3DViewer.h
 * authors ....: Jens Paustenbach
 * organization: LTI, RWTH Aachen
 * creation ...: 17.1.2003
 * revisions ..: $Id: ltiScene3DViewer.h,v 1.10 2005/06/16 12:37:55 doerfler Exp $
 */

#ifndef _LTI_SCENE_3_D_VIEWER_H_
#define _LTI_SCENE_3_D_VIEWER_H_

#include "ltiConfig.h"

#ifdef HAVE_GTK

#include "ltiViewerBase3D.h"
#include "ltiScene3D.h"


namespace lti {

  /**
   * View a scene3D object.
   * This class provides the possiblity to show a scene3D object in a three
   * dimensional view.
   * This class only excepts colored scenes. This means that you can only
   * show object like scene3D<rgbPixel>.
   */
  class scene3DViewer : public viewerBase3D {
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
       * Informational parameter
       */
      //@{

      /** 
       * the ranges of grid, if grid is shown
       */
      dmatrix ranges;

      //@}

    };
  

    /**
     * default constructor
     */
    scene3DViewer(bool createDefaultParameters = true);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    scene3DViewer(const scene3DViewer& other);

    /**
     * constructor with windows title
     */
    scene3DViewer(const std::string& title);

    /**
     * constructor with windows title
     */
    scene3DViewer(const char* title);

    /**
     * destructor
     */
    virtual ~scene3DViewer();

    /**
     * returns the name of this type ("scene3DViewer")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    scene3DViewer& copy(const scene3DViewer& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    scene3DViewer& operator=(const scene3DViewer& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual viewerBase* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * shows a color image.
     * @param scene the object to be shown.
     * @return true if successful, false otherwise.
     */
     bool show(scene3D<rgbPixel>& scene);

  protected:

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
      void putData(scene3D<rgbPixel>& scene);

      /**
       * returns used parameters
       */
      const parameters& getParameters() const;
    protected:

      /**
       * a pointer to the scene that is shown
       */
      scene3D<rgbPixel>* ptScene;
      
      /**
       * draw an image of the scene
       */
      virtual void dataToImage();

      /**
       * prepares the parameters before the settings dialog is started
       */
      void prepareParameters(void);
      

    };    

    class configDialog : public viewerBase3D::configDialog {
    public:
      /**
       * create dialog
       */
      configDialog();

      /**
       * destroy the  dialog
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
      pageWidget buildRangePage();

      /**
       * gtkWidget which contains the ranges of axes
       */
      GtkWidget* table1;

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



      };
  };

}

#endif
#endif

