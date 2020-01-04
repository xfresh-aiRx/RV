/* GDK - The GIMP Drawing Kit
 * Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 * Copyright (C) 1998-2002 Tor Lillqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/*
 * Modified by the GTK+ Team and others 1997-1999.  See the AUTHORS
 * file for a list of people on the GTK+ Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GTK+ at ftp://ftp.gtk.org/pub/gtk/. 
 */

#include "config.h"

#include <stdlib.h>
#include <stdio.h>

#include "gdk.h"
#include "gdkprivate-win32.h"
#include "gdkinputprivate.h"
#include "gdkwin32.h"

static gboolean gdk_window_gravity_works (void);
static void     gdk_window_set_static_win_gravity (GdkWindow *window, 
						   gboolean   on);

/* The Win API function AdjustWindowRect may return negative values
 * resulting in obscured title bars. This helper function is coreccting it.
 */
BOOL
SafeAdjustWindowRectEx (RECT* lpRect, 
			DWORD dwStyle, 
			BOOL bMenu, 
			DWORD dwExStyle)
{
  if (!AdjustWindowRectEx(lpRect, dwStyle, bMenu, dwExStyle))
    {
      WIN32_API_FAILED ("AdjustWindowRectEx");
      return FALSE;
    }
  if (lpRect->left < 0)
    {
      lpRect->right -= lpRect->left;
      lpRect->left = 0;
    }
  if (lpRect->top < 0)
    {
      lpRect->bottom -= lpRect->top;
      lpRect->top = 0;
    }
  return TRUE;
}

static void
gdk_win32_window_destroy (GdkDrawable *drawable)
{
  GDK_NOTE (MISC, g_print ("gdk_win32_window_destroy: %p\n",
			   GDK_DRAWABLE_XID (drawable)));

  if (!GDK_DRAWABLE_DESTROYED (drawable))
    {
      if (GDK_DRAWABLE_TYPE (drawable) == GDK_WINDOW_FOREIGN)
	gdk_xid_table_remove (GDK_DRAWABLE_XID (drawable));
      else
	g_warning ("losing last reference to undestroyed window\n");
    }

  if (GDK_WINDOW_WIN32DATA (drawable)->bg_type == GDK_WIN32_BG_PIXMAP
      && GDK_WINDOW_WIN32DATA (drawable)->bg_pixmap != NULL)
    gdk_drawable_unref (GDK_WINDOW_WIN32DATA (drawable)->bg_pixmap);

  if (GDK_WINDOW_WIN32DATA (drawable)->xcursor != NULL)
    {
      if (GetCursor () == GDK_WINDOW_WIN32DATA (drawable)->xcursor)
	SetCursor (NULL);
      if (!DestroyCursor (GDK_WINDOW_WIN32DATA (drawable)->xcursor))
	WIN32_GDI_FAILED ("DestroyCursor");
    }

  g_free (GDK_DRAWABLE_WIN32DATA (drawable));
}

static GdkWindow *
gdk_win32_window_alloc (void)
{
  GdkWindow *window;
  GdkWindowPrivate *private;
  
  static GdkDrawableClass klass;
  static gboolean initialized = FALSE;

  if (!initialized)
    {
      initialized = TRUE;
      
      klass = _gdk_win32_drawable_class;
      klass.destroy = gdk_win32_window_destroy;
    }

  window = _gdk_window_alloc ();
  private = (GdkWindowPrivate *) window;

  private->drawable.klass = &klass;
  private->drawable.klass_data = g_new (GdkWindowWin32Data, 1);

  GDK_WINDOW_WIN32DATA (window)->owner_thread_id = GetCurrentThreadId ();
  GDK_WINDOW_WIN32DATA (window)->event_mask = 0;
  GDK_WINDOW_WIN32DATA (window)->bg_type = GDK_WIN32_BG_NORMAL;
  GDK_WINDOW_WIN32DATA (window)->xcursor = NULL;
  GDK_WINDOW_WIN32DATA (window)->hint_flags = 0;
  GDK_WINDOW_WIN32DATA (window)->extension_events_selected = FALSE;
  
  GDK_WINDOW_WIN32DATA (window)->input_locale = GetKeyboardLayout (0);
  TranslateCharsetInfo ((DWORD FAR *) GetACP (),
			&GDK_WINDOW_WIN32DATA (window)->charset_info,
			TCI_SRCCODEPAGE);

  return window;
}

void
gdk_window_init (void)
{
  GdkWindowPrivate *private;
  RECT r;
  guint width;
  guint height;

  SystemParametersInfo (SPI_GETWORKAREA, 0, &r, 0);
  width  = r.right - r.left;
  height = r.bottom - r.top;

  gdk_parent_root = gdk_win32_window_alloc ();
  private = (GdkWindowPrivate *) gdk_parent_root;
  
  GDK_DRAWABLE_WIN32DATA (gdk_parent_root)->xid = gdk_root_window;
  private->drawable.window_type = GDK_WINDOW_ROOT;
  private->drawable.width = width;
  private->drawable.height = height;

  gdk_xid_table_insert (&gdk_root_window, gdk_parent_root);
}

/* RegisterGdkClass
 *   is a wrapper function for RegisterWindowClassEx.
 *   It creates at least one unique class for every 
 *   GdkWindowType. If support for single window-specific icons
 *   is ever needed (e.g Dialog specific), every such window should
 *   get its own class
 */
ATOM
RegisterGdkClass (GdkDrawableType wtype)
{
  static ATOM klassTOPLEVEL = 0;
  static ATOM klassDIALOG   = 0;
  static ATOM klassCHILD    = 0;
  static ATOM klassTEMP     = 0;
  static HICON hAppIcon = NULL;
  static WNDCLASSEX wcl; 
  ATOM klass = 0;

  wcl.cbSize = sizeof (WNDCLASSEX);
  wcl.style = 0; /* DON'T set CS_<H,V>REDRAW. It causes total redraw
                  * on WM_SIZE and WM_MOVE. Flicker, Performance!
                  */
  wcl.lpfnWndProc = gdk_WindowProc;
  wcl.cbClsExtra = 0;
  wcl.cbWndExtra = 0;
  wcl.hInstance = gdk_ProgInstance;
  wcl.hIcon = 0;
  /* initialize once! */
  if (0 == hAppIcon)
    {
      gchar sLoc [_MAX_PATH+1];

      if (0 != GetModuleFileName(gdk_ProgInstance, sLoc, _MAX_PATH))
	{
	  hAppIcon = ExtractIcon(gdk_ProgInstance, sLoc, 0);
	  if (0 == hAppIcon)
	    {
	      if (0 != GetModuleFileName (gdk_DLLInstance, sLoc, _MAX_PATH))
		hAppIcon = ExtractIcon (gdk_DLLInstance, sLoc, 0);
	    }
	  
	  if (0 == hAppIcon) 
	    hAppIcon = LoadIcon (NULL, IDI_APPLICATION);
	}
    }

  wcl.lpszMenuName = NULL;
  wcl.hIconSm = 0;

  /* initialize once per class */
#define ONCE_PER_CLASS() \
  wcl.hIcon = CopyIcon (hAppIcon); \
  wcl.hIconSm = CopyIcon (hAppIcon); \
  wcl.hbrBackground = NULL; \
  wcl.hCursor = LoadCursor (NULL, IDC_ARROW); 
  
  switch (wtype)
    {
    case GDK_WINDOW_TOPLEVEL:
      if (0 == klassTOPLEVEL)
	{
	  wcl.lpszClassName = "gdkWindowToplevel";
	  
	  ONCE_PER_CLASS();
	  klassTOPLEVEL = RegisterClassEx(&wcl);
	}
      klass = klassTOPLEVEL;
      break;
    case GDK_WINDOW_CHILD:
      if (0 == klassCHILD)
	{
	  wcl.lpszClassName = "gdkWindowChild";
	  
	  wcl.style |= CS_PARENTDC; /* MSDN: ... enhances system performance. */
	  ONCE_PER_CLASS();
	  klassCHILD = RegisterClassEx(&wcl);
	}
      klass = klassCHILD;
      break;
    case GDK_WINDOW_DIALOG:
      if (0 == klassDIALOG)
	{
	  wcl.lpszClassName = "gdkWindowDialog";
	  wcl.style |= CS_SAVEBITS;
	  ONCE_PER_CLASS();
	  klassDIALOG = RegisterClassEx(&wcl);
	}
      klass = klassDIALOG;
      break;
    case GDK_WINDOW_TEMP:
      if (0 == klassTEMP)
	{
	  wcl.lpszClassName = "gdkWindowTemp";
	  wcl.style |= CS_SAVEBITS;
	  ONCE_PER_CLASS();
	  klassTEMP = RegisterClassEx(&wcl);
	}
      klass = klassTEMP;
      break;
    case GDK_WINDOW_ROOT:
      g_error ("cannot make windows of type GDK_WINDOW_ROOT");
      break;
    case GDK_DRAWABLE_PIXMAP:
      g_error ("cannot make windows of type GDK_DRAWABLE_PIXMAP (use gdk_pixmap_new)");
      break;
    default:
      g_assert_not_reached ();
    }
  
  if (klass == 0)
    {
      WIN32_API_FAILED ("RegisterClassEx");
      g_error ("That is a fatal error");
    }
  return klass;
}

GdkWindow*
gdk_window_new (GdkWindow     *parent,
		GdkWindowAttr *attributes,
		gint           attributes_mask)
{
  GdkWindow *window;
  GdkWindowPrivate *private;
  GdkWindowPrivate *parent_private;
  GdkVisual *visual;
  HANDLE xparent;
  ATOM klass = 0;
  DWORD dwStyle, dwExStyle;
  RECT rect;
  int width, height;
  int x, y;
  char *title;
  gint titlelen;
  wchar_t *wctitle;
  gint wlen;
  char *mbtitle;

  g_return_val_if_fail (attributes != NULL, NULL);

  if (!parent)
    parent = gdk_parent_root;

  parent_private = (GdkWindowPrivate*) parent;
  if (GDK_DRAWABLE_DESTROYED (parent))
    return NULL;

  xparent = GDK_DRAWABLE_XID (parent);

  window = gdk_win32_window_alloc ();
  private = (GdkWindowPrivate *)window;

  private->parent = parent;

  private->x = (attributes_mask & GDK_WA_X) ? attributes->x : 0;
  private->y = (attributes_mask & GDK_WA_Y) ? attributes->y : 0;

  private->drawable.width = (attributes->width > 1) ? (attributes->width) : (1);
  private->drawable.height = (attributes->height > 1) ? (attributes->height) : (1);
  private->drawable.window_type = attributes->window_type;
  GDK_WINDOW_WIN32DATA (window)->extension_events_selected = FALSE;

  if (attributes_mask & GDK_WA_VISUAL)
    visual = attributes->visual;
  else
    visual = gdk_visual_get_system ();

  if (attributes_mask & GDK_WA_TITLE)
    title = attributes->title;
  else
    title = g_get_prgname ();
  if (!title)
    title = "GDK client window";

  GDK_WINDOW_WIN32DATA (window)->event_mask = GDK_STRUCTURE_MASK | attributes->event_mask;
      
  if (parent_private && parent_private->guffaw_gravity)
    {
      /* XXX ??? */
    }

  if (attributes->wclass == GDK_INPUT_OUTPUT)
    {
      dwExStyle = 0;
      if (attributes_mask & GDK_WA_COLORMAP)
	private->drawable.colormap = attributes->colormap;
      else
	private->drawable.colormap = gdk_colormap_get_system ();
    }
  else
    {
      dwExStyle = WS_EX_TRANSPARENT;
      private->drawable.colormap = NULL;
      GDK_WINDOW_WIN32DATA (window)->bg_type = GDK_WIN32_BG_TRANSPARENT;
      GDK_WINDOW_WIN32DATA (window)->bg_pixmap = NULL;
    }

  if (attributes_mask & GDK_WA_X)
    x = attributes->x;
  else
    x = CW_USEDEFAULT;

  if (attributes_mask & GDK_WA_Y)
    y = attributes->y;
  else if (attributes_mask & GDK_WA_X)
    y = 100;			/* ??? We must put it somewhere... */
  else
    y = 500;			/* x is CW_USEDEFAULT, y doesn't matter then */

  if (parent_private)
    parent_private->children = g_list_prepend (parent_private->children, window);

  switch (private->drawable.window_type)
    {
    case GDK_WINDOW_TOPLEVEL:
      dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN;
      xparent = gdk_root_window;
      break;
    case GDK_WINDOW_CHILD:
      dwStyle = WS_CHILDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
      break;
    case GDK_WINDOW_DIALOG:
      dwStyle = WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION | WS_THICKFRAME | WS_CLIPCHILDREN;
#if 0
      dwExStyle |= WS_EX_TOPMOST; /* //HB: want this? */
#endif
      xparent = gdk_root_window;
      break;
    case GDK_WINDOW_TEMP:
      dwStyle = WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
      dwExStyle |= WS_EX_TOOLWINDOW;
      break;
    case GDK_WINDOW_ROOT:
      g_error ("cannot make windows of type GDK_WINDOW_ROOT");
      break;
    case GDK_DRAWABLE_PIXMAP:
      g_error ("cannot make windows of type GDK_DRAWABLE_PIXMAP (use gdk_pixmap_new)");
      break;
    }

  klass = RegisterGdkClass (private->drawable.window_type);

  if (private->drawable.window_type != GDK_WINDOW_CHILD)
    {
      if (x == CW_USEDEFAULT)
	{
	  rect.left = 100;
	  rect.top = 100;
	}
      else
	{
	  rect.left = x;
	  rect.top = y;
	}

      rect.right = rect.left + private->drawable.width;
      rect.bottom = rect.top + private->drawable.height;

      SafeAdjustWindowRectEx (&rect, dwStyle, FALSE, dwExStyle);

      if (x != CW_USEDEFAULT)
	{
	  x = rect.left;
	  y = rect.top;
	}
      width = rect.right - rect.left;
      height = rect.bottom - rect.top;
    }
  else
    {
      width = private->drawable.width;
      height = private->drawable.height;
    }

  titlelen = strlen (title);
  wctitle = g_new (wchar_t, titlelen + 1);
  mbtitle = g_new (char, 3*titlelen + 1);
  wlen = gdk_nmbstowchar_ts (wctitle, title, titlelen, titlelen);
  wctitle[wlen] = 0;
  WideCharToMultiByte (GetACP (), 0, wctitle, -1,
		       mbtitle, 3*titlelen, NULL, NULL);
  
  GDK_DRAWABLE_WIN32DATA (window)->xid =
    CreateWindowEx (dwExStyle,
		    MAKEINTRESOURCE(klass),
		    mbtitle,
		    dwStyle,
		    x, y, 
		    width, height,
		    xparent,
		    NULL,
		    gdk_ProgInstance,
		    NULL);

  GDK_NOTE (MISC,
	    g_print ("gdk_window_new: %s %s %dx%d@+%d+%d %p = %p\n"
		     "...locale %#x codepage %d\n",
		     gdk_win32_drawable_type_to_string (private->drawable.window_type),
		     mbtitle,
		     width, height, (x == CW_USEDEFAULT ? -9999 : x), y, 
		     xparent,
		     GDK_DRAWABLE_XID (window),
		     (guint) GDK_WINDOW_WIN32DATA (window)->input_locale,
		     GDK_WINDOW_WIN32DATA (window)->charset_info.ciACP));

  g_free (mbtitle);
  g_free (wctitle);

  if (GDK_DRAWABLE_XID (window) == NULL)
    {
      WIN32_API_FAILED ("CreateWindowEx");
      g_free (GDK_DRAWABLE_WIN32DATA (window));
      g_free (private);
      return NULL;
    }

  gdk_drawable_ref (window);
  gdk_xid_table_insert (&GDK_DRAWABLE_XID (window), window);

  if (private->drawable.colormap)
    gdk_colormap_ref (private->drawable.colormap);

  gdk_window_set_cursor (window, ((attributes_mask & GDK_WA_CURSOR) ?
				  (attributes->cursor) :
				  NULL));

  return window;
}

GdkWindow *
gdk_window_foreign_new (guint32 anid)
{
  GdkWindow *window;
  GdkWindowPrivate *private;
  GdkWindowPrivate *parent_private;
  HANDLE parent;
  RECT rect;
  POINT point;

  window = gdk_win32_window_alloc ();
  private = (GdkWindowPrivate *)window;

  parent = GetParent ((HWND) anid);
  private->parent = gdk_xid_table_lookup (parent);

  parent_private = (GdkWindowPrivate *)private->parent;
  
  if (parent_private)
    parent_private->children = g_list_prepend (parent_private->children, window);

  GDK_DRAWABLE_WIN32DATA (window)->xid = (HWND) anid;
  GetClientRect ((HWND) anid, &rect);
  point.x = rect.left;
  point.y = rect.right;
  ClientToScreen ((HWND) anid, &point);
  if (parent != GetDesktopWindow ())
    ScreenToClient (parent, &point);
  private->x = point.x;
  private->y = point.y;
  private->drawable.width = rect.right - rect.left;
  private->drawable.height = rect.bottom - rect.top;
  private->drawable.window_type = GDK_WINDOW_FOREIGN;
  private->drawable.destroyed = FALSE;
  private->mapped = IsWindowVisible (GDK_DRAWABLE_XID (window));

  private->drawable.colormap = NULL;

  gdk_drawable_ref (window);
  gdk_xid_table_insert (&GDK_DRAWABLE_XID (window), window);

  return window;
}

/* Call this function when you want a window and all its children to
 * disappear.  When xdestroy is true, a request to destroy the window
 * is sent out.  When it is false, it is assumed that the window has
 * been or will be destroyed by destroying some ancestor of this
 * window.
 */
static void
gdk_window_internal_destroy (GdkWindow *window,
			     gboolean   xdestroy,
			     gboolean   our_destroy)
{
  GdkWindowPrivate *private;
  GdkWindowPrivate *temp_private;
  GdkWindow *temp_window;
  GList *children;
  GList *tmp;

  g_return_if_fail (window != NULL);

  private = (GdkWindowPrivate *) window;

  GDK_NOTE (MISC, g_print ("gdk_window_internal_destroy %p\n",
			   GDK_DRAWABLE_XID (window)));

  switch (GDK_DRAWABLE_TYPE (window))
    {
    case GDK_WINDOW_TOPLEVEL:
    case GDK_WINDOW_CHILD:
    case GDK_WINDOW_DIALOG:
    case GDK_WINDOW_TEMP:
    case GDK_WINDOW_FOREIGN:
      if (!GDK_DRAWABLE_DESTROYED (window))
	{
          gdk_win32_clear_hdc_cache_for_hwnd (GDK_DRAWABLE_XID (window));

	  if (private->parent)
	    {
	      GdkWindowPrivate *parent_private = (GdkWindowPrivate *)private->parent;
	      if (parent_private->children)
		parent_private->children = g_list_remove (parent_private->children, window);
	    }

	  if (GDK_DRAWABLE_TYPE (window) != GDK_WINDOW_FOREIGN)
	    {
	      children = tmp = private->children;
	      private->children = NULL;

	      while (tmp)
		{
		  temp_window = tmp->data;
		  tmp = tmp->next;
		  
		  temp_private = (GdkWindowPrivate*) temp_window;
		  if (temp_private)
		    gdk_window_internal_destroy (temp_window, FALSE,
						 our_destroy);
		}

	      g_list_free (children);
	    }

	  if (private->extension_events != 0)
	    gdk_input_window_destroy (window);

	  if (private->filters)
	    {
	      tmp = private->filters;

	      while (tmp)
		{
		  g_free (tmp->data);
		  tmp = tmp->next;
		}

	      g_list_free (private->filters);
	      private->filters = NULL;
	    }
	  
	  if (private->drawable.window_type == GDK_WINDOW_FOREIGN)
	    {
	      if (our_destroy && (private->parent != NULL))
		{
		  /* It's somebody elses window, but in our hierarchy,
		   * so reparent it to the root window, and then send
		   * it a delete event, as if we were a WM
		   */
		  gdk_window_hide (window);
		  gdk_window_reparent (window, NULL, 0, 0);
		  
		  /* Is this too drastic? Many (most?) applications
		   * quit if any window receives WM_QUIT I think.
		   * OTOH, I don't think foreign windows are much
		   * used, so the question is maybe academic.
		   */
		  PostMessage (GDK_DRAWABLE_XID (window), WM_QUIT, 0, 0);
		}
	    }
	  else 
	    {
	      private->drawable.destroyed = TRUE;
	      if (xdestroy)
		{
		  /* Calls gdk_WindowProc */
		  DestroyWindow (GDK_DRAWABLE_XID (window));
		}
	    }

	  if (private->drawable.colormap)
	    gdk_colormap_unref (private->drawable.colormap);

	  private->mapped = FALSE;
	}
      break;

    case GDK_WINDOW_ROOT:
      g_error ("attempted to destroy root window");
      break;

    case GDK_DRAWABLE_PIXMAP:
      g_error ("called gdk_window_destroy on a pixmap (use gdk_drawable_unref)");
      break;
    }
}

/* Like internal_destroy, but also destroys the reference created by
   gdk_window_new. */

void
gdk_window_destroy (GdkWindow *window)
{
  GDK_NOTE (MISC, g_print ("gdk_window_destroy: %p\n",
			   GDK_DRAWABLE_XID (window)));
  gdk_window_internal_destroy (window, TRUE, TRUE);
  gdk_drawable_unref (window);
}

/* This function is called when the window really gone.  */

void
gdk_window_destroy_notify (GdkWindow *window)
{
  g_return_if_fail (window != NULL);

  GDK_NOTE (EVENTS,
	    g_print ("gdk_window_destroy_notify: %p  %s\n",
		     GDK_DRAWABLE_XID (window),
		     (GDK_DRAWABLE_DESTROYED (window) ? "yes" : "no")));

  if (!GDK_DRAWABLE_DESTROYED (window))
    {
      if (GDK_DRAWABLE_TYPE(window) != GDK_WINDOW_FOREIGN)
	g_warning ("window %p unexpectedly destroyed",
		   GDK_DRAWABLE_XID (window));

      gdk_window_internal_destroy (window, FALSE, FALSE);
    }
  
  gdk_xid_table_remove (GDK_DRAWABLE_XID (window));
  gdk_drawable_unref (window);
}

void
gdk_window_show (GdkWindow *window)
{
  g_return_if_fail (window != NULL);

  if (!GDK_DRAWABLE_DESTROYED (window))
    {
      GDK_NOTE (MISC, g_print ("gdk_window_show: %p\n",
			       GDK_DRAWABLE_XID (window)));

      ((GdkWindowPrivate *) window)->mapped = TRUE;
      if (GDK_DRAWABLE_TYPE (window) == GDK_WINDOW_TEMP)
	{
	  ShowWindow (GDK_DRAWABLE_XID (window), SW_SHOWNOACTIVATE);
	  SetWindowPos (GDK_DRAWABLE_XID (window), HWND_TOPMOST, 0, 0, 0, 0,
			SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
#if 0
	  /* Don't put on toolbar */
	  ShowWindow (GDK_DRAWABLE_XID (window), SW_HIDE);
#endif
	}
      else
	{
          if (GetWindowLong (GDK_DRAWABLE_XID (window), GWL_EXSTYLE) & WS_EX_TRANSPARENT)
	    {
	      SetWindowPos(GDK_DRAWABLE_XID (window), HWND_TOP, 0, 0, 0, 0,
			   SWP_SHOWWINDOW | SWP_NOREDRAW | SWP_NOMOVE | SWP_NOSIZE);
	    }
          else
            {
	      GdkWindow *parent = ((GdkWindowPrivate *)window)->parent;

              ShowWindow (GDK_DRAWABLE_XID (window), SW_SHOWNORMAL);
              ShowWindow (GDK_DRAWABLE_XID (window), SW_RESTORE);
              if (parent == gdk_parent_root)
                SetForegroundWindow (GDK_DRAWABLE_XID (window));
              BringWindowToTop (GDK_DRAWABLE_XID (window));
#if 0
	      ShowOwnedPopups (GDK_DRAWABLE_XID (window), TRUE);
#endif
	    }
	}
    }
}

void
gdk_window_hide (GdkWindow *window)
{
  g_return_if_fail (window != NULL);

  if (!GDK_DRAWABLE_DESTROYED (window))
    {
      GDK_NOTE (MISC, g_print ("gdk_window_hide: %p\n",
			       GDK_DRAWABLE_XID (window)));

      ((GdkWindowPrivate *) window)->mapped = FALSE;
      if (GDK_DRAWABLE_TYPE (window) == GDK_WINDOW_TOPLEVEL)
	ShowOwnedPopups (GDK_DRAWABLE_XID (window), FALSE);
      if (GetWindowLong (GDK_DRAWABLE_XID (window), GWL_EXSTYLE) & WS_EX_TRANSPARENT)
	{
	  SetWindowPos(GDK_DRAWABLE_XID (window), HWND_BOTTOM, 0, 0, 0, 0,
		       SWP_HIDEWINDOW | SWP_NOREDRAW | SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE);
	}
      else
	{
	  ShowWindow (GDK_DRAWABLE_XID (window), SW_HIDE);
	}
    }
}

void
gdk_window_withdraw (GdkWindow *window)
{
  g_return_if_fail (window != NULL);

  if (!GDK_DRAWABLE_DESTROYED (window))
    {
      GDK_NOTE (MISC, g_print ("gdk_window_withdraw: %p\n",
			       GDK_DRAWABLE_XID (window)));

      gdk_window_hide (window);	/* XXX */
    }
}

void
gdk_window_move (GdkWindow *window,
		 gint       x,
		 gint       y)
{
  GdkWindowPrivate *private;

  g_return_if_fail (window != NULL);

  if (!GDK_DRAWABLE_DESTROYED (window))
    {
      RECT rect;

      GDK_NOTE (MISC, g_print ("gdk_window_move: %p +%d+%d\n",
			       GDK_DRAWABLE_XID (window), x, y));

      private = (GdkWindowPrivate *) window;
      GetClientRect (GDK_DRAWABLE_XID (window), &rect);

      if (GDK_DRAWABLE_TYPE (window) != GDK_WINDOW_CHILD)
	{
	  POINT ptTL, ptBR;
	  DWORD dwStyle;
	  DWORD dwExStyle;

	  ptTL.x = 0;
	  ptTL.y = 0; 
	  ClientToScreen (GDK_DRAWABLE_XID (window), &ptTL);
	  rect.left = x;
	  rect.top = y;

	  ptBR.x = rect.right;
	  ptBR.y = rect.bottom;
	  ClientToScreen (GDK_DRAWABLE_XID (window), &ptBR);
	  rect.right = x + ptBR.x - ptTL.x;
	  rect.bottom = y + ptBR.y - ptTL.y;

	  dwStyle = GetWindowLong (GDK_DRAWABLE_XID (window), GWL_STYLE);
	  dwExStyle = GetWindowLong (GDK_DRAWABLE_XID (window), GWL_EXSTYLE);
	  SafeAdjustWindowRectEx (&rect, dwStyle, FALSE, dwExStyle);

	  x = rect.left;
	  y = rect.top;
	}
      else
	{
	  private->x = x;
	  private->y = y;
	}
      GDK_NOTE (MISC, g_print ("...MoveWindow(%p,%ldx%ld@+%d+%d)\n",
			       GDK_DRAWABLE_XID (window),
			       rect.right - rect.left, rect.bottom - rect.top,
			       x, y));
      if (!MoveWindow (GDK_DRAWABLE_XID (window),
		       x, y, rect.right - rect.left, rect.bottom - rect.top,
		       TRUE))
	WIN32_API_FAILED ("MoveWindow");
    }
}

void
gdk_window_resize (GdkWindow *window,
		   gint       width,
		   gint       height)
{
  GdkWindowPrivate *private;

  g_return_if_fail (window != NULL);

  if ((gint16) width < 1)
    width = 1;
  if ((gint16) height < 1)
    height = 1;

  private = (GdkWindowPrivate*) window;

  if (!private->drawable.destroyed &&
      ((private->resize_count > 0) ||
       (private->drawable.width != (guint16) width) ||
       (private->drawable.height != (guint16) height)))
    {
      int x, y;

      GDK_NOTE (MISC, g_print ("gdk_window_resize: %p %dx%d\n",
			       GDK_DRAWABLE_XID (window), width, height));
      
      if (private->drawable.window_type != GDK_WINDOW_CHILD)
	{
	  POINT pt;
	  RECT rect;
	  DWORD dwStyle;
	  DWORD dwExStyle;

	  pt.x = 0;
	  pt.y = 0; 
	  ClientToScreen (GDK_DRAWABLE_XID (window), &pt);
	  rect.left = pt.x;
	  rect.top = pt.y;
	  rect.right = pt.x + width;
	  rect.bottom = pt.y + height;

	  dwStyle = GetWindowLong (GDK_DRAWABLE_XID (window), GWL_STYLE);
	  dwExStyle = GetWindowLong (GDK_DRAWABLE_XID (window), GWL_EXSTYLE);
	  if (!AdjustWindowRectEx (&rect, dwStyle, FALSE, dwExStyle))
	    WIN32_API_FAILED ("AdjustWindowRectEx");

	  x = rect.left;
	  y = rect.top;
	  width = rect.right - rect.left;
	  height = rect.bottom - rect.top;
	}
      else
	{
	  x = private->x;
	  y = private->y;
	  private->drawable.width = width;
	  private->drawable.height = height;
	}

      private->resize_count += 1;

      GDK_NOTE (MISC,
		g_print ("...MoveWindow(%p,%dx%d@+%d+%d)\n",
			 GDK_DRAWABLE_XID (window), width, height, x, y));
      if (!MoveWindow (GDK_DRAWABLE_XID (window),
		       x, y, width, height,
		       TRUE))
	WIN32_API_FAILED ("MoveWindow");
    }
}

void
gdk_window_move_resize (GdkWindow *window,
			gint       x,
			gint       y,
			gint       width,
			gint       height)
{
  GdkWindowPrivate *private;

  g_return_if_fail (window != NULL);

  if ((gint16) width < 1)
    width = 1;
  if ((gint16) height < 1)
    height = 1;

  if (!GDK_DRAWABLE_DESTROYED (window))
    {
      RECT rect;
      DWORD dwStyle;
      DWORD dwExStyle;

      GDK_NOTE (MISC, g_print ("gdk_window_move_resize: %p %dx%d@+%d+%d\n",
			       GDK_DRAWABLE_XID (window), width, height, x, y));
      
      private = (GdkWindowPrivate*) window;
      rect.left = x;
      rect.top = y;
      rect.right = x + width;
      rect.bottom = y + height;

      dwStyle = GetWindowLong (GDK_DRAWABLE_XID (window), GWL_STYLE);
      dwExStyle = GetWindowLong (GDK_DRAWABLE_XID (window), GWL_EXSTYLE);
      if (!AdjustWindowRectEx (&rect, dwStyle, FALSE, dwExStyle))
	WIN32_API_FAILED ("AdjustWindowRectEx");

      if (private->drawable.window_type == GDK_WINDOW_CHILD)
	{
	  private->x = x;
	  private->y = y;
	  private->drawable.width = width;
	  private->drawable.height = height;
	}
      GDK_NOTE (MISC, g_print ("...MoveWindow(%p,%ldx%ld@+%ld+%ld)\n",
			       GDK_DRAWABLE_XID (window),
			       rect.right - rect.left, rect.bottom - rect.top,
			       rect.left, rect.top));
      if (!MoveWindow (GDK_DRAWABLE_XID (window),
		       rect.left, rect.top,
		       rect.right - rect.left, rect.bottom - rect.top,
		       TRUE))
	WIN32_API_FAILED ("MoveWindow");

      if (private->guffaw_gravity)
	{
	  GList *tmp_list = private->children;
	  while (tmp_list)
	    {
	      GdkWindowPrivate *child_private = tmp_list->data;
	      
	      child_private->x -= x - private->x;
	      child_private->y -= y - private->y;
	      
	      tmp_list = tmp_list->next;
	    }
	}
      
    }
}

void
gdk_window_reparent (GdkWindow *window,
		     GdkWindow *new_parent,
		     gint       x,
		     gint       y)
{
  GdkWindowPrivate *window_private;
  GdkWindowPrivate *parent_private;
  GdkWindowPrivate *old_parent_private;

  g_return_if_fail (window != NULL);

  if (!new_parent)
    new_parent = gdk_parent_root;

  window_private = (GdkWindowPrivate*) window;
  old_parent_private = (GdkWindowPrivate *) window_private->parent;
  parent_private = (GdkWindowPrivate*) new_parent;

  if (!GDK_DRAWABLE_DESTROYED (window)
      && !GDK_DRAWABLE_DESTROYED (new_parent))
    {
      GDK_NOTE (MISC, g_print ("gdk_window_reparent: %p %p\n",
			       GDK_DRAWABLE_XID (window),
			       GDK_DRAWABLE_XID (new_parent)));
      if (!SetParent (GDK_DRAWABLE_XID (window),
		      GDK_DRAWABLE_XID (new_parent)))
	WIN32_API_FAILED ("SetParent");

      if (!MoveWindow (GDK_DRAWABLE_XID (window),
		       x, y,
		       window_private->drawable.width,
		       window_private->drawable.height,
		       TRUE))
	WIN32_API_FAILED ("MoveWindow");
    }
  
  window_private->parent = new_parent;

  if (old_parent_private)
    old_parent_private->children =
      g_list_remove (old_parent_private->children, window);

  if ((old_parent_private &&
       (!old_parent_private->guffaw_gravity != !parent_private->guffaw_gravity)) ||
      (!old_parent_private && parent_private->guffaw_gravity))
    gdk_window_set_static_win_gravity (window, parent_private->guffaw_gravity);
  
  parent_private->children = g_list_prepend (parent_private->children, window);
}

void
gdk_window_clear (GdkWindow *window)
{
  g_return_if_fail (window != NULL);
  g_return_if_fail (GDK_IS_WINDOW (window));

  if (!GDK_DRAWABLE_DESTROYED (window))
    gdk_window_clear_area (window, 0, 0, 0, 0);
}


void
gdk_window_clear_area (GdkWindow *window,
		       gint       x,
		       gint       y,
		       gint       width,
		       gint       height)
{
  g_return_if_fail (window != NULL);
  g_return_if_fail (GDK_IS_WINDOW (window));
  
  if (!GDK_DRAWABLE_DESTROYED (window))
    {
      HDC hdc;

      if (width == 0)
	width = ((GdkDrawablePrivate *) window)->width - x;
      if (height == 0)
	height = ((GdkDrawablePrivate *) window)->height - y;
      GDK_NOTE (MISC, g_print ("gdk_window_clear_area: %p %dx%d@+%d+%d\n",
			       GDK_DRAWABLE_XID (window),
			       width, height, x, y));
      hdc = GetDC (GDK_DRAWABLE_XID (window));
      IntersectClipRect (hdc, x, y, x + width, y + height);
      gdk_WindowProc (GDK_DRAWABLE_XID (window), WM_ERASEBKGND, (WPARAM) hdc, 0);
      if (!ReleaseDC (GDK_DRAWABLE_XID (window), hdc))
	WIN32_GDI_FAILED ("ReleaseDC");
    }
}

void
gdk_window_clear_area_e (GdkWindow *window,
		         gint       x,
		         gint       y,
		         gint       width,
		         gint       height)
{
  g_return_if_fail (window != NULL);
  g_return_if_fail (GDK_IS_WINDOW (window));
  
  if (!GDK_DRAWABLE_DESTROYED (window))
    {
      RECT rect;

      GDK_NOTE (MISC, g_print ("gdk_window_clear_area_e: %p %dx%d@+%d+%d\n",
			       GDK_DRAWABLE_XID (window),
			       width, height, x, y));

      rect.left = x;
      rect.right = x + width;
      rect.top = y;
      rect.bottom = y + height;
      if (!InvalidateRect (GDK_DRAWABLE_XID (window), &rect, TRUE))
	WIN32_GDI_FAILED ("InvalidateRect");
      UpdateWindow (GDK_DRAWABLE_XID (window));
    }
}

void
gdk_window_raise (GdkWindow *window)
{
  g_return_if_fail (window != NULL);
  g_return_if_fail (GDK_IS_WINDOW (window));
  
  if (!GDK_DRAWABLE_DESTROYED (window))
    {
      GDK_NOTE (MISC, g_print ("gdk_window_raise: %p\n",
			       GDK_DRAWABLE_XID (window)));

      if (!BringWindowToTop (GDK_DRAWABLE_XID (window)))
	WIN32_API_FAILED ("BringWindowToTop");
    }
}

void
gdk_window_lower (GdkWindow *window)
{
  g_return_if_fail (window != NULL);
  g_return_if_fail (GDK_IS_WINDOW (window));
  
  if (!GDK_DRAWABLE_DESTROYED (window))
    {
      GDK_NOTE (MISC, g_print ("gdk_window_lower: %p\n",
			       GDK_DRAWABLE_XID (window)));

      if (!SetWindowPos (GDK_DRAWABLE_XID (window), HWND_BOTTOM, 0, 0, 0, 0,
			 SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE))
	WIN32_API_FAILED ("SetWindowPos");
    }
}

void
gdk_window_set_hints (GdkWindow *window,
		      gint       x,
		      gint       y,
		      gint       min_width,
		      gint       min_height,
		      gint       max_width,
		      gint       max_height,
		      gint       flags)
{
  WINDOWPLACEMENT size_hints;
  RECT rect;
  DWORD dwStyle;
  DWORD dwExStyle;
  int diff;
  
  g_return_if_fail (window != NULL);
  g_return_if_fail (GDK_IS_WINDOW (window));
  
  if (GDK_DRAWABLE_DESTROYED (window))
    return;
  
  GDK_NOTE (MISC, g_print ("gdk_window_set_hints: %p %dx%d..%dx%d @+%d+%d\n",
			   GDK_DRAWABLE_XID (window),
			   min_width, min_height, max_width, max_height,
			   x, y));

  GDK_WINDOW_WIN32DATA (window)->hint_flags = flags;
  size_hints.length = sizeof (size_hints);

  if (flags)
    {
      if (flags & GDK_HINT_POS)
	{
	  if (!GetWindowPlacement (GDK_DRAWABLE_XID (window), &size_hints))
	    WIN32_API_FAILED ("GetWindowPlacement");
	  else
	    {
	      GDK_NOTE (MISC, g_print ("...rcNormalPosition:"
				       " (%ld,%ld)--(%ld,%ld)\n",
				       size_hints.rcNormalPosition.left,
				       size_hints.rcNormalPosition.top,
				       size_hints.rcNormalPosition.right,
				       size_hints.rcNormalPosition.bottom));
	      /* What are the corresponding window coordinates for client
	       * area coordinates x, y
	       */
	      rect.left = x;
	      rect.top = y;
	      rect.right = rect.left + 200;	/* dummy */
	      rect.bottom = rect.top + 200;
	      dwStyle = GetWindowLong (GDK_DRAWABLE_XID (window), GWL_STYLE);
	      dwExStyle = GetWindowLong (GDK_DRAWABLE_XID (window), GWL_EXSTYLE);
	      AdjustWindowRectEx (&rect, dwStyle, FALSE, dwExStyle);
	      size_hints.flags = 0;
	      size_hints.showCmd = SW_SHOWNA;
	      
	      /* Set the normal position hint to that location, with unchanged
	       * width and height.
	       */
	      diff = size_hints.rcNormalPosition.left - rect.left;
	      size_hints.rcNormalPosition.left = rect.left;
	      size_hints.rcNormalPosition.right -= diff;
	      diff = size_hints.rcNormalPosition.top - rect.top;
	      size_hints.rcNormalPosition.top = rect.top;
	      size_hints.rcNormalPosition.bottom -= diff;
	      GDK_NOTE (MISC, g_print ("...setting: (%ld,%ld)--(%ld,%ld)\n",
				       size_hints.rcNormalPosition.left,
				       size_hints.rcNormalPosition.top,
				       size_hints.rcNormalPosition.right,
				       size_hints.rcNormalPosition.bottom));
	      if (!SetWindowPlacement (GDK_DRAWABLE_XID (window), &size_hints))
		WIN32_API_FAILED ("SetWindowPlacement");
	      GDK_WINDOW_WIN32DATA (window)->hint_x = rect.left;
	      GDK_WINDOW_WIN32DATA (window)->hint_y = rect.top;
	    }
	}

      if (flags & GDK_HINT_MIN_SIZE)
	{
	  rect.left = 0;
	  rect.top = 0;
	  rect.right = min_width;
	  rect.bottom = min_height;
	  dwStyle = GetWindowLong (GDK_DRAWABLE_XID (window), GWL_STYLE);
	  dwExStyle = GetWindowLong (GDK_DRAWABLE_XID (window), GWL_EXSTYLE);
	  AdjustWindowRectEx (&rect, dwStyle, FALSE, dwExStyle);
	  GDK_WINDOW_WIN32DATA (window)->hint_min_width =
	    rect.right - rect.left;
	  GDK_WINDOW_WIN32DATA (window)->hint_min_height =
	    rect.bottom - rect.top;

	  /* Also chek if he current size of the window is in bounds. */
	  GetClientRect (GDK_DRAWABLE_XID (window), &rect);
	  if (rect.right < min_width && rect.bottom < min_height)
	    gdk_window_resize (window, min_width, min_height);
	  else if (rect.right < min_width)
	    gdk_window_resize (window, min_width, rect.bottom);
	  else if (rect.bottom < min_height)
	    gdk_window_resize (window, rect.right, min_height);
	}

      if (flags & GDK_HINT_MAX_SIZE)
	{
	  rect.left = 0;
	  rect.top = 0;
	  rect.right = max_width;
	  rect.bottom = max_height;
	  dwStyle = GetWindowLong (GDK_DRAWABLE_XID (window), GWL_STYLE);
	  dwExStyle = GetWindowLong (GDK_DRAWABLE_XID (window), GWL_EXSTYLE);
	  AdjustWindowRectEx (&rect, dwStyle, FALSE, dwExStyle);
	  GDK_WINDOW_WIN32DATA (window)->hint_max_width =
	    rect.right - rect.left;
	  GDK_WINDOW_WIN32DATA (window)->hint_max_height =
	    rect.bottom - rect.top;
	  /* Again, check if the window is too large currently. */
	  GetClientRect (GDK_DRAWABLE_XID (window), &rect);
	  if (rect.right > max_width && rect.bottom > max_height)
	    gdk_window_resize (window, max_width, max_height);
	  else if (rect.right > max_width)
	    gdk_window_resize (window, max_width, rect.bottom);
	  else if (rect.bottom > max_height)
	    gdk_window_resize (window, rect.right, max_height);
	}
    }
}

void 
gdk_window_set_geometry_hints (GdkWindow      *window,
			       GdkGeometry    *geometry,
			       GdkWindowHints  geom_mask)
{
  WINDOWPLACEMENT size_hints;
  RECT rect;
  DWORD dwStyle;
  DWORD dwExStyle;
  
  g_return_if_fail (window != NULL);
  g_return_if_fail (GDK_IS_WINDOW (window));
  
  if (GDK_DRAWABLE_DESTROYED (window))
    return;

  size_hints.length = sizeof (size_hints);

  GDK_WINDOW_WIN32DATA (window)->hint_flags = geom_mask;

  if (geom_mask & GDK_HINT_POS)
    ; /* XXX */

  if (geom_mask & GDK_HINT_MIN_SIZE)
    {
      rect.left = 0;
      rect.top = 0;
      rect.right = geometry->min_width;
      rect.bottom = geometry->min_height;
      dwStyle = GetWindowLong (GDK_DRAWABLE_XID (window), GWL_STYLE);
      dwExStyle = GetWindowLong (GDK_DRAWABLE_XID (window), GWL_EXSTYLE);
      AdjustWindowRectEx (&rect, dwStyle, FALSE, dwExStyle);
      GDK_WINDOW_WIN32DATA (window)->hint_min_width = rect.right - rect.left;
      GDK_WINDOW_WIN32DATA (window)->hint_min_height = rect.bottom - rect.top;

      /* Also check if he current size of the window is in bounds */
      GetClientRect (GDK_DRAWABLE_XID (window), &rect);
      if (rect.right < geometry->min_width
	  && rect.bottom < geometry->min_height)
	gdk_window_resize (window, geometry->min_width, geometry->min_height);
      else if (rect.right < geometry->min_width)
	gdk_window_resize (window, geometry->min_width, rect.bottom);
      else if (rect.bottom < geometry->min_height)
	gdk_window_resize (window, rect.right, geometry->min_height);
    }
  
  if (geom_mask & GDK_HINT_MAX_SIZE)
    {
      rect.left = 0;
      rect.top = 0;
      rect.right = geometry->max_width;
      rect.bottom = geometry->max_height;
      dwStyle = GetWindowLong (GDK_DRAWABLE_XID (window), GWL_STYLE);
      dwExStyle = GetWindowLong (GDK_DRAWABLE_XID (window), GWL_EXSTYLE);
      AdjustWindowRectEx (&rect, dwStyle, FALSE, dwExStyle);
      GDK_WINDOW_WIN32DATA (window)->hint_max_width = rect.right - rect.left;
      GDK_WINDOW_WIN32DATA (window)->hint_max_height = rect.bottom - rect.top;

      /* Again, check if the window is too large currently. */
      GetClientRect (GDK_DRAWABLE_XID (window), &rect);
      if (rect.right > geometry->max_width
	  && rect.bottom > geometry->max_height)
	gdk_window_resize (window, geometry->max_width, geometry->max_height);
      else if (rect.right > geometry->max_width)
	gdk_window_resize (window, geometry->max_width, rect.bottom);
      else if (rect.bottom > geometry->max_height)
	gdk_window_resize (window, rect.right, geometry->max_height);
    }
  
  /* I don't know what to do when called with zero base_width and height. */
  if (geom_mask & GDK_HINT_BASE_SIZE
      && geometry->base_width > 0
      && geometry->base_height > 0)
    {
      if (!GetWindowPlacement (GDK_DRAWABLE_XID (window), &size_hints))
	WIN32_API_FAILED ("GetWindowPlacement");
      else
	{
	  GDK_NOTE (MISC, g_print ("gdk_window_set_geometry_hints:"
				   " rcNormalPosition: (%ld,%ld)--(%ld,%ld)\n",
				   size_hints.rcNormalPosition.left,
				   size_hints.rcNormalPosition.top,
				   size_hints.rcNormalPosition.right,
				   size_hints.rcNormalPosition.bottom));
	  size_hints.rcNormalPosition.right =
	    size_hints.rcNormalPosition.left + geometry->base_width;
	  size_hints.rcNormalPosition.bottom =
	    size_hints.rcNormalPosition.top + geometry->base_height;
	  GDK_NOTE (MISC, g_print ("...setting: rcNormal: (%ld,%ld)--(%ld,%ld)\n",
				   size_hints.rcNormalPosition.left,
				   size_hints.rcNormalPosition.top,
				   size_hints.rcNormalPosition.right,
				   size_hints.rcNormalPosition.bottom));
	  if (!SetWindowPlacement (GDK_DRAWABLE_XID (window), &size_hints))
	    WIN32_API_FAILED ("SetWindowPlacement");
	}
    }
  
  if (geom_mask & GDK_HINT_RESIZE_INC)
    {
      /* XXX */
    }
  
  if (geom_mask & GDK_HINT_ASPECT)
    {
      /* XXX */
    }
}

void
gdk_window_set_title (GdkWindow   *window,
		      const gchar *title)
{
  gint titlelen;
  wchar_t *wcstr;
  gint wlen;
  char *mbstr;

  g_return_if_fail (window != NULL);
  g_return_if_fail (GDK_IS_WINDOW (window));
  
  GDK_NOTE (MISC, g_print ("gdk_window_set_title: %p %s\n",
			   GDK_DRAWABLE_XID (window), title));
  if (!GDK_DRAWABLE_DESTROYED (window))
    {
      /* As the title is in UTF-8 we must translate it
       * to the system codepage.
       */
      titlelen = strlen (title);
      wcstr = g_new (wchar_t, titlelen + 1);
      mbstr = g_new (char, 3*titlelen + 1);
      wlen = gdk_nmbstowchar_ts (wcstr, title, titlelen, titlelen);
      wcstr[wlen] = 0;
      WideCharToMultiByte (GetACP (), 0, wcstr, -1,
			   mbstr, 3*titlelen, NULL, NULL);

      if (!SetWindowText (GDK_DRAWABLE_XID (window), mbstr))
	WIN32_API_FAILED ("SetWindowText");

      g_free (mbstr);
      g_free (wcstr);
    }
}

void          
gdk_window_set_role (GdkWindow   *window,
		     const gchar *role)
{
  g_return_if_fail (window != NULL);
  g_return_if_fail (GDK_IS_WINDOW (window));
  
  GDK_NOTE (MISC, g_print ("gdk_window_set_role: %p %s\n",
			   GDK_DRAWABLE_XID (window), (role ? role : "NULL")));
  /* XXX */
}

void          
gdk_window_set_transient_for (GdkWindow *window, 
			      GdkWindow *parent)
{
  g_return_if_fail (window != NULL);
  g_return_if_fail (GDK_IS_WINDOW (window));
  
  GDK_NOTE (MISC, g_print ("gdk_window_set_transient_for: %p %p\n",
			   GDK_DRAWABLE_XID (window),
			   GDK_DRAWABLE_XID (parent)));
  /* XXX */
}

void
gdk_window_set_background (GdkWindow *window,
			   GdkColor  *color)
{
  g_return_if_fail (window != NULL);
  g_return_if_fail (GDK_IS_WINDOW (window));
  
  if (!GDK_DRAWABLE_DESTROYED (window))
    {
      GDK_NOTE (MISC, g_print ("gdk_window_set_background: %p %s\n",
			       GDK_DRAWABLE_XID (window), 
			       gdk_win32_color_to_string (color)));

      if (GDK_WINDOW_WIN32DATA (window)->bg_type == GDK_WIN32_BG_PIXMAP)
	{
	  if (GDK_WINDOW_WIN32DATA (window)->bg_pixmap != NULL)
	    {
	      gdk_drawable_unref (GDK_WINDOW_WIN32DATA (window)->bg_pixmap);
	      GDK_WINDOW_WIN32DATA (window)->bg_pixmap = NULL;
	    }
	  GDK_WINDOW_WIN32DATA (window)->bg_type = GDK_WIN32_BG_NORMAL;
	}
      GDK_WINDOW_WIN32DATA (window)->bg_type = GDK_WIN32_BG_PIXEL;
      GDK_WINDOW_WIN32DATA (window)->bg_pixel = color->pixel;
    }
}

void
gdk_window_set_back_pixmap (GdkWindow *window,
			    GdkPixmap *pixmap,
			    gint       parent_relative)
{
  g_return_if_fail (window != NULL);
  g_return_if_fail (GDK_IS_WINDOW (window));
  
  if (!GDK_DRAWABLE_DESTROYED (window))
    {
      if (GDK_WINDOW_WIN32DATA (window)->bg_type == GDK_WIN32_BG_PIXMAP)
	{
	  if (GDK_WINDOW_WIN32DATA (window)->bg_pixmap != NULL)
	    {
	      gdk_drawable_unref (GDK_WINDOW_WIN32DATA (window)->bg_pixmap);
	      GDK_WINDOW_WIN32DATA (window)->bg_pixmap = NULL;
	    }
	  GDK_WINDOW_WIN32DATA (window)->bg_type = GDK_WIN32_BG_NORMAL;
	}
      if (parent_relative)
	{
	  GDK_WINDOW_WIN32DATA (window)->bg_type =
	    GDK_WIN32_BG_PARENT_RELATIVE;
	}
      else if (!pixmap)
	{
          /* Is this right ?? */
          GDK_WINDOW_WIN32DATA (window)->bg_type = GDK_WIN32_BG_TRANSPARENT;
	}
      else
	{
	  /* We must cache the pixmap in the GdkWindowWin32Data and
	   * paint it each time we get WM_ERASEBKGND
	   */
	  GDK_WINDOW_WIN32DATA (window)->bg_type = GDK_WIN32_BG_PIXMAP;
	  GDK_WINDOW_WIN32DATA (window)->bg_pixmap = pixmap;
	  gdk_drawable_ref (pixmap);
	}
    }
}

void
gdk_window_set_cursor (GdkWindow *window,
		       GdkCursor *cursor)
{
  GdkCursorPrivate *cursor_private;
  HCURSOR xcursor;
  HCURSOR prev_xcursor;
  
  g_return_if_fail (window != NULL);
  g_return_if_fail (GDK_IS_WINDOW (window));
  
  cursor_private = (GdkCursorPrivate*) cursor;
  
  if (GDK_DRAWABLE_DESTROYED (window))
    return;

  if (!cursor)
    xcursor = NULL;
  else
    xcursor = cursor_private->xcursor;

  GDK_NOTE (MISC, g_print ("gdk_window_set_cursor: %p %p\n",
			   GDK_DRAWABLE_XID (window), xcursor));
  /* First get the old cursor, if any (we wait to free the old one
   * since it may be the current cursor set in the Win32 API right
   * now).
   */
  prev_xcursor = GDK_WINDOW_WIN32DATA (window)->xcursor;

  if (xcursor == NULL)
    GDK_WINDOW_WIN32DATA (window)->xcursor = NULL;
  else
    {
      /* We must copy the cursor as it is OK to destroy the GdkCursor
       * while still in use for some window. See for instance
       * gimp_change_win_cursor() which calls
       * gdk_window_set_cursor (win, cursor), and immediately
       * afterwards gdk_cursor_destroy (cursor).
       */
      if ((GDK_WINDOW_WIN32DATA (window)->xcursor = CopyCursor (xcursor)) == NULL)
	WIN32_API_FAILED ("CopyCursor");
      GDK_NOTE (MISC, g_print ("...CopyCursor (%p) = %p\n",
			       xcursor,
			       GDK_WINDOW_WIN32DATA (window)->xcursor));
    }

   /* Set new cursor in all cases if we're over our window */
  if (gdk_window_get_pointer(window, NULL, NULL, NULL) == window)
    SetCursor (GDK_WINDOW_WIN32DATA (window)->xcursor);

  /* Destroy the previous cursor: Need to make sure it's no longer in
   * use before we destroy it, in case we're not over our window but
   * the cursor is still set to our old one.
   */
  if (prev_xcursor != NULL)
    {
      if (GetCursor() == prev_xcursor)
 	SetCursor (NULL);

      GDK_NOTE (MISC, g_print ("...DestroyCursor (%p)\n",
			       prev_xcursor));
      if (!DestroyCursor (prev_xcursor))
	WIN32_API_FAILED ("DestroyCursor");
    }
}

void
gdk_window_get_geometry (GdkWindow *window,
			 gint      *x,
			 gint      *y,
			 gint      *width,
			 gint      *height,
			 gint      *depth)
{
  g_return_if_fail (window == NULL || GDK_IS_WINDOW (window));
  
  if (!window)
    window = gdk_parent_root;
  
  if (!GDK_DRAWABLE_DESTROYED (window))
    {
      RECT rect;

      if (!GetClientRect (GDK_DRAWABLE_XID (window), &rect))
	WIN32_API_FAILED ("GetClientRect");

      if (window != gdk_parent_root)
	{
	  POINT pt;
	  GdkWindow *parent = gdk_window_get_parent (window);

	  pt.x = rect.left;
	  pt.y = rect.top;
	  ClientToScreen (GDK_DRAWABLE_XID (window), &pt);
	  ScreenToClient (GDK_DRAWABLE_XID (parent), &pt);
	  rect.left = pt.x;
	  rect.top = pt.y;

	  pt.x = rect.right;
	  pt.y = rect.bottom;
	  ClientToScreen (GDK_DRAWABLE_XID (window), &pt);
	  ScreenToClient (GDK_DRAWABLE_XID (parent), &pt);
	  rect.right = pt.x;
	  rect.bottom = pt.y;
	}

      if (x)
	*x = rect.left;
      if (y)
	*y = rect.top;
      if (width)
	*width = rect.right - rect.left;
      if (height)
	*height = rect.bottom - rect.top;
      if (depth)
	*depth = gdk_drawable_get_visual (window)->depth;
    }
}

gint
gdk_window_get_origin (GdkWindow *window,
		       gint      *x,
		       gint      *y)
{
  gint return_val;
  gint tx = 0;
  gint ty = 0;

  g_return_val_if_fail (window != NULL, 0);

  if (!GDK_DRAWABLE_DESTROYED (window))
    {
      POINT pt;

      pt.x = 0;
      pt.y = 0;
      ClientToScreen (GDK_DRAWABLE_XID (window), &pt);
      tx = pt.x;
      ty = pt.y;
      return_val = 1;
    }
  else
    return_val = 0;
  
  if (x)
    *x = tx;
  if (y)
    *y = ty;

  GDK_NOTE (MISC, g_print ("gdk_window_get_origin: %p: +%d+%d\n",
			   GDK_DRAWABLE_XID (window), tx, ty));
  return return_val;
}

gboolean
gdk_window_get_deskrelative_origin (GdkWindow *window,
				    gint      *x,
				    gint      *y)
{
  return gdk_window_get_origin (window, x, y);
}

void
gdk_window_get_root_origin (GdkWindow *window,
			    gint      *x,
			    gint      *y)
{
  GdkWindowPrivate *rover;
  POINT pt;

  g_return_if_fail (window != NULL);
  g_return_if_fail (GDK_IS_WINDOW (window));

  rover = (GdkWindowPrivate*) window;
  if (x)
    *x = 0;
  if (y)
    *y = 0;
  if (GDK_DRAWABLE_DESTROYED (window))
    return;
      
  while (rover->parent && ((GdkWindowPrivate*) rover->parent)->parent)
    rover = (GdkWindowPrivate *) rover->parent;
  if (rover->drawable.destroyed)
    return;

  pt.x = 0;
  pt.y = 0;
  ClientToScreen (GDK_DRAWABLE_XID (rover), &pt);
  if (x)
    *x = pt.x;
  if (y)
    *y = pt.y;

  GDK_NOTE (MISC, g_print ("gdk_window_get_root_origin: %p: (%p) +%ld+%ld\n",
			   GDK_DRAWABLE_XID (window),
			   GDK_DRAWABLE_XID (rover), pt.x, pt.y));
}

GdkWindow*
gdk_window_get_pointer (GdkWindow       *window,
			gint            *x,
			gint            *y,
			GdkModifierType *mask)
{
  GdkWindow *return_val;
  POINT screen_point, point;
  HWND hwnd, hwndc;

  g_return_val_if_fail (window == NULL || GDK_IS_WINDOW (window), NULL);
  
  if (!window)
    window = gdk_parent_root;

  return_val = NULL;
  GetCursorPos (&screen_point);
  point = screen_point;
  ScreenToClient (GDK_DRAWABLE_XID (window), &point);

  if (x)
    *x = point.x;
  if (y)
    *y = point.y;

  hwnd = WindowFromPoint (screen_point);
  if (hwnd != NULL)
    {
      gboolean done = FALSE;
      
      while (!done)
	{
	  point = screen_point;
	  ScreenToClient (hwnd, &point);
	  hwndc = ChildWindowFromPoint (hwnd, point);
	  if (hwndc == NULL)
	    done = TRUE;
	  else if (hwndc == hwnd)
	    done = TRUE;
	  else
	    hwnd = hwndc;
	}
      
      return_val = gdk_window_lookup (hwnd);
    }
  else
    {
      return_val = NULL;
    }

  if (mask)
    {
      BYTE kbd[256];

      GetKeyboardState (kbd);
      *mask = 0;
      if (kbd[VK_SHIFT] & 0x80)
	*mask |= GDK_SHIFT_MASK;
      if (kbd[VK_CAPITAL] & 0x80)
	*mask |= GDK_LOCK_MASK;
      if (kbd[VK_CONTROL] & 0x80)
	*mask |= GDK_CONTROL_MASK;
      if (kbd[VK_MENU] & 0x80)
	*mask |= GDK_MOD1_MASK;
      if (kbd[VK_LBUTTON] & 0x80)
	*mask |= GDK_BUTTON1_MASK;
      if (kbd[VK_MBUTTON] & 0x80)
	*mask |= GDK_BUTTON2_MASK;
      if (kbd[VK_RBUTTON] & 0x80)
	*mask |= GDK_BUTTON3_MASK;
    }
  
  return return_val;
}

GdkWindow*
gdk_window_at_pointer (gint *win_x,
		       gint *win_y)
{
  GdkWindow *window;
  POINT point, pointc;
  HWND hwnd, hwndc;
  RECT rect;

  GetCursorPos (&pointc);
  point = pointc;
  hwnd = WindowFromPoint (point);

  if (hwnd == NULL)
    {
      window = gdk_parent_root;
      if (win_x)
	*win_x = pointc.x;
      if (win_y)
	*win_y = pointc.y;
      return window;
    }
      
  ScreenToClient (hwnd, &point);

  do {
    hwndc = ChildWindowFromPoint (hwnd, point);
    ClientToScreen (hwnd, &point);
    ScreenToClient (hwndc, &point);
  } while (hwndc != hwnd && (hwnd = hwndc, 1));

  window = gdk_window_lookup (hwnd);

  if (window && (win_x || win_y))
    {
      GetClientRect (hwnd, &rect);
      if (win_x)
	*win_x = point.x - rect.left;
      if (win_y)
	*win_y = point.y - rect.top;
    }

  GDK_NOTE (MISC, g_print ("gdk_window_at_pointer: +%ld+%ld %p%s\n",
			   point.x, point.y, hwnd,
			   (window == NULL ? " NULL" : "")));

  return window;
}

static BOOL CALLBACK
get_children_enum_proc (HWND   hwnd,
			LPARAM lparam)
{
  GdkWindow *w = gdk_window_lookup (hwnd);
  GList **children = (GList **) lparam;

  if (w != NULL)
    *children = g_list_prepend (*children, w);

  return TRUE;
}

GList*
gdk_window_get_children (GdkWindow *window)
{
  GList *children = NULL;

  g_return_val_if_fail (window != NULL, NULL);
  g_return_val_if_fail (GDK_IS_WINDOW (window), NULL);

  if (GDK_DRAWABLE_DESTROYED (window) ||
      GDK_DRAWABLE_XID (window) == NULL)
    return NULL;

  EnumChildWindows (GDK_DRAWABLE_XID (window),
		    get_children_enum_proc,
		    (LPARAM) &children);

  return children;
}

GdkEventMask  
gdk_window_get_events (GdkWindow *window)
{
  g_return_val_if_fail (window != NULL, 0);
  g_return_val_if_fail (GDK_IS_WINDOW (window), 0);

  if (GDK_DRAWABLE_DESTROYED (window))
    return 0;

  return GDK_WINDOW_WIN32DATA (window)->event_mask;
}

void          
gdk_window_set_events (GdkWindow   *window,
		       GdkEventMask event_mask)
{
  g_return_if_fail (window != NULL);
  g_return_if_fail (GDK_IS_WINDOW (window));

  if (GDK_DRAWABLE_DESTROYED (window))
    return;

  GDK_WINDOW_WIN32DATA (window)->event_mask = event_mask;
}

void
gdk_window_add_colormap_windows (GdkWindow *window)
{
  static gboolean already_warned = FALSE;

  if (!already_warned)
    {
      g_warning ("gdk_window_add_colormap_windows not implemented");
      already_warned = TRUE;
    }
}

void
gdk_window_shape_combine_mask (GdkWindow *window,
			       GdkBitmap *mask,
			       gint x, gint y)
{
  g_return_if_fail (window != NULL);
  g_return_if_fail (GDK_IS_WINDOW (window));

  if (!mask)
    {
      GDK_NOTE (MISC, g_print ("gdk_window_shape_combine_mask: %p none\n",
			       GDK_DRAWABLE_XID (window)));
      SetWindowRgn (GDK_DRAWABLE_XID (window), NULL, TRUE);
    }
  else
    {
      HRGN hrgn;
      DWORD dwStyle;
      DWORD dwExStyle;
      RECT rect;

      /* Convert mask bitmap to region */
      hrgn = gdk_win32_bitmap_to_region (mask);

      GDK_NOTE (MISC, g_print ("gdk_window_shape_combine_mask: %p %p\n",
			       GDK_DRAWABLE_XID (window),
			       GDK_DRAWABLE_XID (mask)));

      /* SetWindowRgn wants window (not client) coordinates */ 
      dwStyle = GetWindowLong (GDK_DRAWABLE_XID (window), GWL_STYLE);
      dwExStyle = GetWindowLong (GDK_DRAWABLE_XID (window), GWL_EXSTYLE);
      GetClientRect (GDK_DRAWABLE_XID (window), &rect);
      AdjustWindowRectEx (&rect, dwStyle, FALSE, dwExStyle);
      OffsetRgn (hrgn, -rect.left, -rect.top);

      OffsetRgn (hrgn, x, y);

      /* If this is a top-level window, add the title bar to the region */
      if (GDK_DRAWABLE_TYPE (window) == GDK_WINDOW_TOPLEVEL)
	{
	  HRGN tmp = CreateRectRgn (0, 0, rect.right - rect.left, -rect.top);
	  CombineRgn (hrgn, hrgn, tmp, RGN_OR);
	  DeleteObject (tmp);
	}
      
      SetWindowRgn (GDK_DRAWABLE_XID (window), hrgn, TRUE);
    }
}

void
gdk_window_set_override_redirect (GdkWindow *window,
				  gboolean   override_redirect)
{
  g_return_if_fail (window != NULL);
  g_return_if_fail (GDK_IS_WINDOW (window));

  g_warning ("gdk_window_set_override_redirect not implemented");
}

void          
gdk_window_set_icon (GdkWindow *window, 
		     GdkWindow *icon_window,
		     GdkPixmap *pixmap,
		     GdkBitmap *mask)
{
  g_return_if_fail (window != NULL);
  g_return_if_fail (GDK_IS_WINDOW (window));

  /* Nothing to do, really. As we share window classes between windows
   * we can't have window-specific icons, sorry. Don't print any warning
   * either.
   */
}

void
gdk_window_set_icon_name (GdkWindow   *window, 
			  const gchar *name)
{
  g_return_if_fail (window != NULL);
  g_return_if_fail (GDK_IS_WINDOW (window));

  if (GDK_DRAWABLE_DESTROYED (window))
    return;
  
  if (!SetWindowText (GDK_DRAWABLE_XID (window), name))
    WIN32_API_FAILED ("SetWindowText");
}

void          
gdk_window_set_group (GdkWindow *window, 
		      GdkWindow *leader)
{
  g_return_if_fail (window != NULL);
  g_return_if_fail (GDK_IS_WINDOW (window));
  g_return_if_fail (leader != NULL);
  g_return_if_fail (GDK_IS_WINDOW (leader));

  if (GDK_DRAWABLE_DESTROYED (window) || GDK_DRAWABLE_DESTROYED (leader))
    return;
  
  g_warning ("gdk_window_set_group not implemented");
}

void
gdk_window_set_decorations (GdkWindow      *window,
			    GdkWMDecoration decorations)
{
  LONG style, bits;
  const LONG settable_bits = WS_BORDER|WS_THICKFRAME|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX|WS_MAXIMIZEBOX;

  g_return_if_fail (window != NULL);
  g_return_if_fail (GDK_IS_WINDOW (window));
  
  style = GetWindowLong (GDK_DRAWABLE_XID (window), GWL_STYLE);

  bits = 0;

  if (decorations & GDK_DECOR_BORDER)
    bits |= WS_BORDER;
  if (decorations & GDK_DECOR_RESIZEH)
    bits |= WS_THICKFRAME;
  if (decorations & GDK_DECOR_TITLE)
    bits |= WS_CAPTION;
  if (decorations & GDK_DECOR_MENU)
    bits |= WS_SYSMENU;
  if (decorations & GDK_DECOR_MINIMIZE)
    bits |= WS_MINIMIZEBOX;
  if (decorations & GDK_DECOR_MAXIMIZE)
    bits |= WS_MAXIMIZEBOX;

  if (decorations & GDK_DECOR_ALL)
    style |= settable_bits, style &= ~bits;
  else
    style &= ~settable_bits, style |= bits;

  SetWindowLong (GDK_DRAWABLE_XID (window), GWL_STYLE, style);

  SetWindowPos (GDK_DRAWABLE_XID (window), NULL, 0, 0, 0, 0,
		SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOMOVE |
		SWP_NOREPOSITION | SWP_NOSIZE | SWP_NOZORDER);
}

void
gdk_window_set_functions (GdkWindow    *window,
			  GdkWMFunction functions)
{
  LONG style, bits;
  const LONG settable_bits = (WS_THICKFRAME|WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_SYSMENU);

  g_return_if_fail (window != NULL);
  g_return_if_fail (GDK_IS_WINDOW (window));
  
  style = GetWindowLong (GDK_DRAWABLE_XID (window), GWL_STYLE);

  bits = 0;

  if (functions & GDK_FUNC_RESIZE)
    bits |= WS_THICKFRAME;
  if (functions & GDK_FUNC_MOVE)
    bits |= (WS_THICKFRAME|WS_SYSMENU);
  if (functions & GDK_FUNC_MINIMIZE)
    bits |= WS_MINIMIZEBOX;
  if (functions & GDK_FUNC_MAXIMIZE)
    bits |= WS_MAXIMIZEBOX;
  if (functions & GDK_FUNC_CLOSE)
    bits |= WS_SYSMENU;
  
  if (functions & GDK_FUNC_ALL)
    style |= settable_bits, style &= ~bits;
  else
    style &= ~settable_bits, style |= bits;

  SetWindowLong (GDK_DRAWABLE_XID (window), GWL_STYLE, style);

  SetWindowPos (GDK_DRAWABLE_XID (window), NULL, 0, 0, 0, 0,
		SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOMOVE |
		SWP_NOREPOSITION | SWP_NOSIZE | SWP_NOZORDER);
}

/* 
 * propagate the shapes from all child windows of a GDK window to the parent 
 * window. Shamelessly ripped from Enlightenment's code
 * 
 * - Raster
 */

static void
QueryTree (HWND hwnd,
	   HWND **children,
	   gint *nchildren)
{
  guint i, n;
  HWND child;

  n = 0;
  do {
    if (n == 0)
      child = GetWindow (hwnd, GW_CHILD);
    else
      child = GetWindow (child, GW_HWNDNEXT);
    if (child != NULL)
      n++;
  } while (child != NULL);

  if (n > 0)
    {
      *children = g_new (HWND, n);
      for (i = 0; i < n; i++)
	{
	  if (i == 0)
	    child = GetWindow (hwnd, GW_CHILD);
	  else
	    child = GetWindow (child, GW_HWNDNEXT);
	  *children[i] = child;
	}
    }
}

static void
gdk_propagate_shapes (HANDLE   win,
		      gboolean merge)
{
   RECT emptyRect;
   HRGN region, childRegion;
   HWND *list = NULL;
   gint i, num;

   SetRectEmpty (&emptyRect);
   region = CreateRectRgnIndirect (&emptyRect);
   if (merge)
     GetWindowRgn (win, region);
   
   QueryTree (win, &list, &num);
   if (list != NULL)
     {
       WINDOWPLACEMENT placement;

       placement.length = sizeof (WINDOWPLACEMENT);
       /* go through all child windows and combine regions */
       for (i = 0; i < num; i++)
	 {
	   GetWindowPlacement (list[i], &placement);
	   if (placement.showCmd == SW_SHOWNORMAL)
	     {
	       childRegion = CreateRectRgnIndirect (&emptyRect);
	       GetWindowRgn (list[i], childRegion);
	       CombineRgn (region, region, childRegion, RGN_OR);
	       DeleteObject (childRegion);
	     }
	  }
       SetWindowRgn (win, region, TRUE);
     }
   else
     DeleteObject (region);
}

void
gdk_window_set_child_shapes (GdkWindow *window)
{
  g_return_if_fail (window != NULL);
  g_return_if_fail (GDK_IS_WINDOW (window));
   
  if (GDK_DRAWABLE_DESTROYED (window))
    return;

  gdk_propagate_shapes (GDK_DRAWABLE_XID (window), FALSE);
}

void
gdk_window_merge_child_shapes (GdkWindow *window)
{
  g_return_if_fail (window != NULL);
  g_return_if_fail (GDK_IS_WINDOW (window));
  
  if (GDK_DRAWABLE_DESTROYED (window))
    return;

  gdk_propagate_shapes (GDK_DRAWABLE_XID (window), TRUE);
}

/* Support for windows that can be guffaw-scrolled
 * (See http://www.gtk.org/~otaylor/whitepapers/guffaw-scrolling.txt)
 */

static gboolean
gdk_window_gravity_works (void)
{
  enum { UNKNOWN, NO, YES };
  static gint gravity_works = UNKNOWN;
  
  if (gravity_works == UNKNOWN)
    {
      GdkWindowAttr attr;
      GdkWindow *parent;
      GdkWindow *child;
      gint y;
      
      attr.window_type = GDK_WINDOW_TEMP;
      attr.wclass = GDK_INPUT_OUTPUT;
      attr.x = 0;
      attr.y = 0;
      attr.width = 100;
      attr.height = 100;
      attr.event_mask = 0;
      
      parent = gdk_window_new (NULL, &attr, GDK_WA_X | GDK_WA_Y);
      
      attr.window_type = GDK_WINDOW_CHILD;
      child = gdk_window_new (parent, &attr, GDK_WA_X | GDK_WA_Y);
      
      gdk_window_set_static_win_gravity (child, TRUE);
      
      gdk_window_resize (parent, 100, 110);
      gdk_window_move (parent, 0, -10);
      gdk_window_move_resize (parent, 0, 0, 100, 100);
      
      gdk_window_resize (parent, 100, 110);
      gdk_window_move (parent, 0, -10);
      gdk_window_move_resize (parent, 0, 0, 100, 100);
      
      gdk_window_get_geometry (child, NULL, &y, NULL, NULL, NULL);
      
      gdk_window_destroy (parent);
      gdk_window_destroy (child);
      
      gravity_works = ((y == -20) ? YES : NO);
    }
  
  return (gravity_works == YES);
}

static void
gdk_window_set_static_bit_gravity (GdkWindow *window, gboolean on)
{
  g_return_if_fail (window != NULL);

  GDK_NOTE (MISC, g_print ("gdk_window_set_static_bit_gravity: Not implemented\n"));
}

static void
gdk_window_set_static_win_gravity (GdkWindow *window, gboolean on)
{
  g_return_if_fail (window != NULL);

  GDK_NOTE (MISC,
	    g_print ("gdk_window_set_static_win_gravity: Not implemented\n"));
}

/*************************************************************
 * gdk_window_set_static_gravities:
 *     Set the bit gravity of the given window to static,
 *     and flag it so all children get static subwindow
 *     gravity.
 *   arguments:
 *     window: window for which to set static gravity
 *     use_static: Whether to turn static gravity on or off.
 *   results:
 *     Does the XServer support static gravity?
 *************************************************************/

gboolean 
gdk_window_set_static_gravities (GdkWindow *window,
				 gboolean   use_static)
{
  GdkWindowPrivate *private = (GdkWindowPrivate *)window;
  GList *tmp_list;
  
  g_return_val_if_fail (window != NULL, FALSE);
  g_return_val_if_fail (GDK_IS_WINDOW (window), FALSE);

  if (!use_static == !private->guffaw_gravity)
    return TRUE;
  
  if (use_static && !gdk_window_gravity_works ())
    return FALSE;
  
  private->guffaw_gravity = use_static;
  
  if (!GDK_DRAWABLE_DESTROYED (window))
    {
      gdk_window_set_static_bit_gravity (window, use_static);
      
      tmp_list = private->children;
      while (tmp_list)
	{
	  gdk_window_set_static_win_gravity (window, use_static);
	  
	  tmp_list = tmp_list->next;
	}
    }
  
  return TRUE;
}