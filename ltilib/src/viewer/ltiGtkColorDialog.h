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
 * file .......: ltiGtkColorDialog.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 21.4.2000
 * revisions ..: $Id: ltiGtkColorDialog.h,v 1.4 2003/02/17 07:17:11 alvarado Exp $
 */

#ifndef _LTI_GTK_COLOR_DIALOG_H_
#define _LTI_GTK_COLOR_DIALOG_H_

#include "ltiConfig.h"

#ifdef HAVE_GTK

#include <gtk/gtk.h>
#include <list>
#include <string>
#include "ltiTypes.h"
#include "ltiRGBPixel.h"

namespace lti {

  /**
   * allows to choose a color in the HSI or RGB color space
   */
  class colorDialog : public object {
  public:
    /**
     * create the color dialog and specify the color to be edited
     */
    colorDialog();

    /**
     * destructor
     */
    virtual ~colorDialog();

    /**
     * show the dialog
     */
    void show();

    /**
     * hide the dialog
     */
    void hide();

    /**
     * specify the variable with the predifined color, and where the
     * user defined color will be left.
     */
    void useColor(rgbPixel& pixel);

  protected:
    rgbPixel color;
    rgbPixel* extColor;

    GtkWidget *colorselectiondialog;
    GtkWidget *ok_button1;
    GtkWidget *cancel_button1;
    GtkWidget *help_button1;

    static void colorChanged_callback(GtkColorSelection* widget,
                                      gpointer data);

    void colorChanged_local(GtkColorSelection* widget);

    static void ok_callback(GtkWidget *widget,
                            gpointer data);
    void ok_local(GtkWidget *widget);

    static void cancel_callback(GtkWidget *widget,
                                gpointer data);
    void cancel_local(GtkWidget *widget);
  };

}

#endif

#endif

