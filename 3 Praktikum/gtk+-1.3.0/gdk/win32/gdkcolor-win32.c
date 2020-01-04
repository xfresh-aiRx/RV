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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "gdkcolor.h"
#include "gdkwin32.h"

static gint
palette_size (HPALETTE hpal)
{
  WORD npal = 0;

  if (!GetObject (hpal, sizeof (npal), &npal))
    WIN32_GDI_FAILED ("GetObject (HPALETTE)");

  return npal;
}

#ifdef G_ENABLE_DEBUG

void
gdk_win32_print_paletteentries (const PALETTEENTRY *pep,
				const int           nentries)
{
  char buf[20];
  int i;

  for (i = 0; i < nentries; i++)
    g_print ("  %3d %02x:  %02x %02x %02x%s\n",
	     i, i,
	     pep[i].peRed, pep[i].peGreen, pep[i].peBlue,
	     (pep[i].peFlags == 0 ? "" :
	      (pep[i].peFlags == PC_EXPLICIT ? " PC_EXPLICIT" :
	       (pep[i].peFlags == PC_NOCOLLAPSE ? " PC_NOCOLLAPSE" :
		(pep[i].peFlags == PC_RESERVED ? " PC_RESERVED" :
		 (sprintf (buf, " %d", pep[i].peFlags), buf))))));
}

void
gdk_win32_print_system_palette (void)
{
  PALETTEENTRY *pe;
  int k;

  k = GetSystemPaletteEntries (gdk_DC, 0, 0, NULL);
  pe = g_new (PALETTEENTRY, k);
  k = GetSystemPaletteEntries (gdk_DC, 0, k, pe);

  if (!k)
    g_print ("GetSystemPaletteEntries failed: %s\n",
	     g_win32_error_message (GetLastError ()));
  else
    {
      g_print ("System palette: %d entries\n", k);
      gdk_win32_print_paletteentries (pe, k);
    }
  g_free (pe);
}

void
gdk_win32_print_hpalette (HPALETTE hpal)
{
  PALETTEENTRY *pe;
  gint n, npal;

  npal = palette_size (hpal);
  pe = g_new (PALETTEENTRY, npal);
  n = GetPaletteEntries (hpal, 0, npal, pe);

  if (!n)
    g_print ("HPALETTE %p: GetPaletteEntries failed: %s\n",
	     hpal, g_win32_error_message (GetLastError ()));
  else
    {
      g_print ("HPALETTE %p: %d (%d) entries\n", hpal, n, npal);
      gdk_win32_print_paletteentries (pe, n);
    }
  g_free (pe);
}

#endif

static gint
match_color (GdkColormap *cmap,
	     GdkColor    *color,
	     const gchar *available)
{
  GdkColor *colors;
  gint sum, min;
  gint rdiff, gdiff, bdiff;
  gint i, index;

  g_return_val_if_fail (cmap != NULL, 0);
  g_return_val_if_fail (color != NULL, 0);

  colors = cmap->colors;
  min = 3 * 65536;
  index = -1;

  for (i = 0; i < cmap->size; i++)
    {
      if ((!available) || (available && available[i]))
	{
	  rdiff = (color->red - colors[i].red);
	  gdiff = (color->green - colors[i].green);
	  bdiff = (color->blue - colors[i].blue);

	  sum = ABS (rdiff) + ABS (gdiff) + ABS (bdiff);

	  if (sum < min)
	    {
	      index = i;
	      min = sum;
	    }
	}
    }

  return index;
}

/* Mimics XAllocColorCells. Allocate read/write color cells. */

static gboolean
alloc_color_cells (GdkColormapPrivateWin32 *cmapp,
		   gboolean                 contig,
		   gulong                   plane_masks_return[],
		   guint                    nplanes,
		   gulong                   pixels_return[],
		   guint                    npixels)
{
  gint i, nfree, iret, start = 0;

  GDK_NOTE (COLORMAP, g_print ("alloc_color_cells: cmap=%p contig=%s npl=%d npix=%d",
			       cmapp, contig ? "TRUE" : "FALSE",
			       nplanes, npixels));

  switch (cmapp->base.visual->type)
    {
    case GDK_VISUAL_GRAYSCALE:
    case GDK_VISUAL_PSEUDO_COLOR:
      nfree = 0;
      for (i = 0; i < cmapp->base.colormap.size && nfree < npixels; i++)
	if (cmapp->use[i] == GDK_WIN32_PE_AVAILABLE &&
	    (!contig ||
	     (nfree == 0 || cmapp->use[i-1] == GDK_WIN32_PE_AVAILABLE)))
	  {
	    if (nfree == 0)
	      start = i;
	    nfree++;
	  }
	else if (contig)
	  nfree = 0;

      if (npixels > nfree)
	{
	  GDK_NOTE (COLORMAP, g_print ("...nope (%d > %d)\n", npixels, nfree));
	  return FALSE;
	}
      else
	GDK_NOTE (COLORMAP, g_print ("...ok\n"));

      iret = 0;
      for (i = start; i < cmapp->base.colormap.size && iret < npixels; i++)
	if (cmapp->use[i] == GDK_WIN32_PE_AVAILABLE)
	  {
	    cmapp->use[i] = GDK_WIN32_PE_INUSE;
	    pixels_return[iret] = i;
	    iret++;
	  }
      g_assert (iret == npixels);
      break;

    default:
      g_assert_not_reached ();
    }

  return TRUE;
}

/* The following functions are from Tk8.0, but heavily modified. Here
 * are Tk's licensing terms. I hope these terms don't conflict with
 * the GNU Library General Public License? They shouldn't, as they are
 * looser that the GLPL.
 */

/*
This software is copyrighted by the Regents of the University of
California, Sun Microsystems, Inc., and other parties.  The following
terms apply to all files associated with the software unless explicitly
disclaimed in individual files.

The authors hereby grant permission to use, copy, modify, distribute,
and license this software and its documentation for any purpose, provided
that existing copyright notices are retained in all copies and that this
notice is included verbatim in any distributions. No written agreement,
license, or royalty fee is required for any of the authorized uses.
Modifications to this software may be copyrighted by their authors
and need not follow the licensing terms described here, provided that
the new terms are clearly indicated on the first page of each file where
they apply.

IN NO EVENT SHALL THE AUTHORS OR DISTRIBUTORS BE LIABLE TO ANY PARTY
FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
ARISING OUT OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY
DERIVATIVES THEREOF, EVEN IF THE AUTHORS HAVE BEEN ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

THE AUTHORS AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWARE
IS PROVIDED ON AN "AS IS" BASIS, AND THE AUTHORS AND DISTRIBUTORS HAVE
NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
MODIFICATIONS.

GOVERNMENT USE: If you are acquiring this software on behalf of the
U.S. government, the Government shall have only "Restricted Rights"
in the software and related documentation as defined in the Federal 
Acquisition Regulations (FARs) in Clause 52.227.19 (c) (2).  If you
are acquiring the software on behalf of the Department of Defense, the
software shall be classified as "Commercial Computer Software" and the
Government shall have only "Restricted Rights" as defined in Clause
252.227-7013 (c) (1) of DFARs.  Notwithstanding the foregoing, the
authors grant the U.S. Government and others acting in its behalf
permission to use and distribute the software in accordance with the
terms specified in this license.
*/

/* Mimics XAllocColor. Allocate a read-only colormap entry. */

static gboolean
alloc_color (GdkColormapPrivateWin32 *cmapp,
	     PALETTEENTRY            *color,
	     gulong                  *pixelp)
{
  PALETTEENTRY entry, close_entry;
  COLORREF new_pixel;
  UINT index;
  gint i;
    
  entry = *color;
  entry.peFlags = 0;

  new_pixel = RGB (entry.peRed, entry.peGreen, entry.peBlue);

  switch (cmapp->base.visual->type)
    {
    case GDK_VISUAL_PSEUDO_COLOR:
      /* Find the nearest existing palette entry. */
      index = GetNearestPaletteIndex (cmapp->hpal, new_pixel);
      GetPaletteEntries (cmapp->hpal, index, 1, &close_entry);

      GDK_NOTE (COLORMAP,
		g_print ("alloc_color: new_pixel=%06lx index=%d=%02x close=%06lx\n",
			 new_pixel, index, index,
			 RGB (close_entry.peRed, close_entry.peGreen, close_entry.peBlue)));

      if (new_pixel != RGB (close_entry.peRed, close_entry.peGreen,
			    close_entry.peBlue))
	{
	  /* Not a perfect match. */
	  if (cmapp->use[index] == GDK_WIN32_PE_AVAILABLE)
	    {
	      /* It was a nonused entry anyway, so we can use it, and
	       * set it to the correct color.
	       */
	      GDK_NOTE (COLORMAP, g_print ("...was free\n"));
	      if (!SetPaletteEntries (cmapp->hpal, index, 1, &entry))
		WIN32_GDI_FAILED ("SetPaletteEntries");
	    }
	  else
	    {
	      /* The close entry found is in use, so search for a
	       * available slot.
	       */
	      gboolean done = FALSE;
	      for (i = 0; i < cmapp->base.colormap.size; i++)
		if (cmapp->use[i] == GDK_WIN32_PE_AVAILABLE)
		  {
		    /* An available slot, use it. */
		    GDK_NOTE (COLORMAP,
			      g_print ("...use free slot %d%s\n",
				       i, (i >= cmapp->current_size) ?
				       ", will resize palette" : ""));
		    if (i >= cmapp->current_size)
		      {
			if (!ResizePalette (cmapp->hpal, i + 1))
			  {
			    WIN32_GDI_FAILED ("ResizePalette");
			    break;
			  }
			cmapp->current_size = i + 1;
		      }
		    if (!SetPaletteEntries (cmapp->hpal, i, 1, &entry))
		      {
			WIN32_GDI_FAILED ("SetPaletteEntries");
			i = cmapp->base.colormap.size;
		      }
		    else
		      {
			done = TRUE;
			index = i;
		      }
		    break;
		  }
	      if (!done)
		{
		  /* No free slots available, or failed to resize
		   * palette or set palette entry.
		   */
		  GDK_NOTE (COLORMAP, g_print ("... failure\n"));
		  return FALSE;
		}
	    }
	}
      else
	{
	  /* We got a match, so use it. */
	}

      *pixelp = index;
      cmapp->use[index] = GDK_WIN32_PE_INUSE;
      GDK_NOTE (COLORMAP, g_print ("alloc_color: %p: "
				   "index=%3d=%02x for %02x %02x %02x: "
				   "%02x %02x %02x\n",
				   cmapp->hpal, index, index,
				   entry.peRed, entry.peGreen, entry.peBlue,
				   color->peRed, color->peGreen, color->peBlue));
      return TRUE;

    case GDK_VISUAL_STATIC_COLOR:
      /* Find the nearest existing palette entry. */
      index = GetNearestPaletteIndex (cmapp->hpal, new_pixel);
      GetPaletteEntries (cmapp->hpal, index, 1, &close_entry);
      *color = close_entry;
      *pixelp = index;
      GDK_NOTE (COLORMAP, g_print ("alloc_color %p: "
				   "index=%3d=%02x for %02x %02x %02x: "
				   "%02x %02x %02x\n",
				   cmapp->hpal, index, index,
				   entry.peRed, entry.peGreen, entry.peBlue,
				   color->peRed, color->peGreen, color->peBlue));
      return TRUE;

    case GDK_VISUAL_TRUE_COLOR:
      /* Determine what color will actually be used on non-colormap systems. */

      *pixelp = GetNearestColor (gdk_DC, new_pixel);
      color->peRed = GetRValue (*pixelp);
      color->peGreen = GetGValue (*pixelp);
      color->peBlue = GetBValue (*pixelp);
      return TRUE;

    default:
      g_assert_not_reached ();
      return FALSE;
    }
}

/* Mimics XFreeColors. */

static void
free_colors (GdkColormapPrivateWin32 *cmapp,
	     gulong                  *pixels,
	     gint                     npixels,
	     gulong                   planes)
{
  PALETTEENTRY pe;
  gint i;
#ifdef G_ENABLE_DEBUG
  gint set_explicit = 0;
#endif

  /* We don't have to do anything for non-palette devices. */
  
  switch (cmapp->base.visual->type)
    {
    case GDK_VISUAL_GRAYSCALE:
    case GDK_VISUAL_PSEUDO_COLOR:
      for (i = 0; i < npixels; i++)
	{
	  if (pixels[i] >= cmapp->base.colormap.size)
	    ; /* Nothing */
	  else if (cmapp->use[pixels[i]] == GDK_WIN32_PE_STATIC)
	    ; /* Nothing either*/
	  else
	    cmapp->use[pixels[i]] = GDK_WIN32_PE_AVAILABLE;
	}
      for (i = cmapp->current_size - 1; i >= 0; i--)
	if (cmapp->use[i] != GDK_WIN32_PE_AVAILABLE)
	  break;
      if (i < cmapp->current_size - 1)
	{
	  GDK_NOTE (COLORMAP, g_print ("free_colors: hpal=%p resize=%d\n",
				       cmapp->hpal, i + 1));
	  if (!ResizePalette (cmapp->hpal, i + 1))
	    WIN32_GDI_FAILED ("ResizePalette");
	  else
	    cmapp->current_size = i + 1;
	}
      for (i = 0; i < cmapp->current_size; i++)
	{
	  if (cmapp->use[i] == GDK_WIN32_PE_AVAILABLE)
	    {
	      *(WORD*)&pe = i;
	      pe.peFlags = PC_EXPLICIT;
	      if (!SetPaletteEntries (cmapp->hpal, i, 1, &pe))
		WIN32_GDI_FAILED ("SetPaletteEntries");
	      GDK_NOTE (COLORMAP, set_explicit++);
	    }
	}
#if 0
      GDK_NOTE (COLORMAP, gdk_win32_print_hpalette (cmapp->hpal));
#else
      GDK_NOTE (COLORMAP, (set_explicit > 0 ?
			   g_print ("free_colors: %d (%d) PC_EXPLICIT\n",
				    set_explicit, cmapp->current_size)
			   : (void) 0));
#endif
      break;

    default:
      g_assert_not_reached ();
    }
}

/* Mimics XCreateColormap. */

static void
create_colormap (GdkColormapPrivateWin32 *cmapp,
		 gboolean                 writeable)
{
  struct {
    LOGPALETTE pal;
    PALETTEENTRY pe[256-1];
  } lp;
  HPALETTE hpal;
  gint i;

  /* Allocate a starting palette with all the static colors. */
  hpal = GetStockObject (DEFAULT_PALETTE);
  lp.pal.palVersion = 0x300;
  lp.pal.palNumEntries = GetPaletteEntries (hpal, 0, 256, lp.pal.palPalEntry);

  if (cmapp->base.visual->type == GDK_VISUAL_STATIC_COLOR &&
      cmapp->base.visual->depth == 4)
    {
      /* Use only 16 colors */
      for (i = 8; i < 16; i++)
	lp.pal.palPalEntry[i] = lp.pal.palPalEntry[i+4];
      lp.pal.palNumEntries = 16;
    }

  for (i = 0; i < lp.pal.palNumEntries; i++)
    lp.pal.palPalEntry[i].peFlags = 0;
  GDK_NOTE (COLORMAP, (g_print ("Default palette %p: %d entries\n",
				hpal, lp.pal.palNumEntries),
		       gdk_win32_print_paletteentries (lp.pal.palPalEntry,
						       lp.pal.palNumEntries)));
  DeleteObject (hpal);
  
  /* For writeable colormaps, allow all 256 entries to be set. They won't
   * set all 256 system palette entries anyhow, of course, but we shouldn't
   * let the app see that, I think.
   */
  if (writeable)
    cmapp->current_size = 0;
  else
    cmapp->current_size = lp.pal.palNumEntries;

  cmapp->private_val = writeable;

  if (!(cmapp->hpal = CreatePalette (&lp.pal)))
    WIN32_GDI_FAILED ("CreatePalette");
  else
    GDK_NOTE (COLORMAP, g_print ("Created palette %p\n", cmapp->hpal));

  switch (cmapp->base.visual->type)
    {
    case GDK_VISUAL_PSEUDO_COLOR:
      cmapp->use = g_new (GdkWin32PalEntryState, cmapp->base.colormap.size);

      /* Mark static colors in use. */
      for (i = 0; i < cmapp->current_size; i++)
	{
	  cmapp->use[i] = GDK_WIN32_PE_STATIC;
	  cmapp->info[i].ref_count = G_MAXUINT/2;
	}
      /* Mark rest not in use */
      for (; i < cmapp->base.colormap.size; i++)
	cmapp->use[i] = GDK_WIN32_PE_AVAILABLE;
      break;

    default:
      cmapp->use = NULL;
      break;
    }
}

typedef struct {
    gchar *name;
    guchar red;
    guchar green;
    guchar blue;
} XColorEntry;

static XColorEntry xColors[] = {
    { "alice blue", 240, 248, 255 },
    { "AliceBlue", 240, 248, 255 },
    { "antique white", 250, 235, 215 },
    { "AntiqueWhite", 250, 235, 215 },
    { "AntiqueWhite1", 255, 239, 219 },
    { "AntiqueWhite2", 238, 223, 204 },
    { "AntiqueWhite3", 205, 192, 176 },
    { "AntiqueWhite4", 139, 131, 120 },
    { "aquamarine", 127, 255, 212 },
    { "aquamarine1", 127, 255, 212 },
    { "aquamarine2", 118, 238, 198 },
    { "aquamarine3", 102, 205, 170 },
    { "aquamarine4", 69, 139, 116 },
    { "azure", 240, 255, 255 },
    { "azure1", 240, 255, 255 },
    { "azure2", 224, 238, 238 },
    { "azure3", 193, 205, 205 },
    { "azure4", 131, 139, 139 },
    { "beige", 245, 245, 220 },
    { "bisque", 255, 228, 196 },
    { "bisque1", 255, 228, 196 },
    { "bisque2", 238, 213, 183 },
    { "bisque3", 205, 183, 158 },
    { "bisque4", 139, 125, 107 },
    { "black", 0, 0, 0 },
    { "blanched almond", 255, 235, 205 },
    { "BlanchedAlmond", 255, 235, 205 },
    { "blue", 0, 0, 255 },
    { "blue violet", 138, 43, 226 },
    { "blue1", 0, 0, 255 },
    { "blue2", 0, 0, 238 },
    { "blue3", 0, 0, 205 },
    { "blue4", 0, 0, 139 },
    { "BlueViolet", 138, 43, 226 },
    { "brown", 165, 42, 42 },
    { "brown1", 255, 64, 64 },
    { "brown2", 238, 59, 59 },
    { "brown3", 205, 51, 51 },
    { "brown4", 139, 35, 35 },
    { "burlywood", 222, 184, 135 },
    { "burlywood1", 255, 211, 155 },
    { "burlywood2", 238, 197, 145 },
    { "burlywood3", 205, 170, 125 },
    { "burlywood4", 139, 115, 85 },
    { "cadet blue", 95, 158, 160 },
    { "CadetBlue", 95, 158, 160 },
    { "CadetBlue1", 152, 245, 255 },
    { "CadetBlue2", 142, 229, 238 },
    { "CadetBlue3", 122, 197, 205 },
    { "CadetBlue4", 83, 134, 139 },
    { "chartreuse", 127, 255, 0 },
    { "chartreuse1", 127, 255, 0 },
    { "chartreuse2", 118, 238, 0 },
    { "chartreuse3", 102, 205, 0 },
    { "chartreuse4", 69, 139, 0 },
    { "chocolate", 210, 105, 30 },
    { "chocolate1", 255, 127, 36 },
    { "chocolate2", 238, 118, 33 },
    { "chocolate3", 205, 102, 29 },
    { "chocolate4", 139, 69, 19 },
    { "coral", 255, 127, 80 },
    { "coral1", 255, 114, 86 },
    { "coral2", 238, 106, 80 },
    { "coral3", 205, 91, 69 },
    { "coral4", 139, 62, 47 },
    { "cornflower blue", 100, 149, 237 },
    { "CornflowerBlue", 100, 149, 237 },
    { "cornsilk", 255, 248, 220 },
    { "cornsilk1", 255, 248, 220 },
    { "cornsilk2", 238, 232, 205 },
    { "cornsilk3", 205, 200, 177 },
    { "cornsilk4", 139, 136, 120 },
    { "cyan", 0, 255, 255 },
    { "cyan1", 0, 255, 255 },
    { "cyan2", 0, 238, 238 },
    { "cyan3", 0, 205, 205 },
    { "cyan4", 0, 139, 139 },
    { "dark blue", 0, 0, 139 },
    { "dark cyan", 0, 139, 139 },
    { "dark goldenrod", 184, 134, 11 },
    { "dark gray", 169, 169, 169 },
    { "dark green", 0, 100, 0 },
    { "dark grey", 169, 169, 169 },
    { "dark khaki", 189, 183, 107 },
    { "dark magenta", 139, 0, 139 },
    { "dark olive green", 85, 107, 47 },
    { "dark orange", 255, 140, 0 },
    { "dark orchid", 153, 50, 204 },
    { "dark red", 139, 0, 0 },
    { "dark salmon", 233, 150, 122 },
    { "dark sea green", 143, 188, 143 },
    { "dark slate blue", 72, 61, 139 },
    { "dark slate gray", 47, 79, 79 },
    { "dark slate grey", 47, 79, 79 },
    { "dark turquoise", 0, 206, 209 },
    { "dark violet", 148, 0, 211 },
    { "DarkBlue", 0, 0, 139 },
    { "DarkCyan", 0, 139, 139 },
    { "DarkGoldenrod", 184, 134, 11 },
    { "DarkGoldenrod1", 255, 185, 15 },
    { "DarkGoldenrod2", 238, 173, 14 },
    { "DarkGoldenrod3", 205, 149, 12 },
    { "DarkGoldenrod4", 139, 101, 8 },
    { "DarkGray", 169, 169, 169 },
    { "DarkGreen", 0, 100, 0 },
    { "DarkGrey", 169, 169, 169 },
    { "DarkKhaki", 189, 183, 107 },
    { "DarkMagenta", 139, 0, 139 },
    { "DarkOliveGreen", 85, 107, 47 },
    { "DarkOliveGreen1", 202, 255, 112 },
    { "DarkOliveGreen2", 188, 238, 104 },
    { "DarkOliveGreen3", 162, 205, 90 },
    { "DarkOliveGreen4", 110, 139, 61 },
    { "DarkOrange", 255, 140, 0 },
    { "DarkOrange1", 255, 127, 0 },
    { "DarkOrange2", 238, 118, 0 },
    { "DarkOrange3", 205, 102, 0 },
    { "DarkOrange4", 139, 69, 0 },
    { "DarkOrchid", 153, 50, 204 },
    { "DarkOrchid1", 191, 62, 255 },
    { "DarkOrchid2", 178, 58, 238 },
    { "DarkOrchid3", 154, 50, 205 },
    { "DarkOrchid4", 104, 34, 139 },
    { "DarkRed", 139, 0, 0 },
    { "DarkSalmon", 233, 150, 122 },
    { "DarkSeaGreen", 143, 188, 143 },
    { "DarkSeaGreen1", 193, 255, 193 },
    { "DarkSeaGreen2", 180, 238, 180 },
    { "DarkSeaGreen3", 155, 205, 155 },
    { "DarkSeaGreen4", 105, 139, 105 },
    { "DarkSlateBlue", 72, 61, 139 },
    { "DarkSlateGray", 47, 79, 79 },
    { "DarkSlateGray1", 151, 255, 255 },
    { "DarkSlateGray2", 141, 238, 238 },
    { "DarkSlateGray3", 121, 205, 205 },
    { "DarkSlateGray4", 82, 139, 139 },
    { "DarkSlateGrey", 47, 79, 79 },
    { "DarkTurquoise", 0, 206, 209 },
    { "DarkViolet", 148, 0, 211 },
    { "deep pink", 255, 20, 147 },
    { "deep sky blue", 0, 191, 255 },
    { "DeepPink", 255, 20, 147 },
    { "DeepPink1", 255, 20, 147 },
    { "DeepPink2", 238, 18, 137 },
    { "DeepPink3", 205, 16, 118 },
    { "DeepPink4", 139, 10, 80 },
    { "DeepSkyBlue", 0, 191, 255 },
    { "DeepSkyBlue1", 0, 191, 255 },
    { "DeepSkyBlue2", 0, 178, 238 },
    { "DeepSkyBlue3", 0, 154, 205 },
    { "DeepSkyBlue4", 0, 104, 139 },
    { "dim gray", 105, 105, 105 },
    { "dim grey", 105, 105, 105 },
    { "DimGray", 105, 105, 105 },
    { "DimGrey", 105, 105, 105 },
    { "dodger blue", 30, 144, 255 },
    { "DodgerBlue", 30, 144, 255 },
    { "DodgerBlue1", 30, 144, 255 },
    { "DodgerBlue2", 28, 134, 238 },
    { "DodgerBlue3", 24, 116, 205 },
    { "DodgerBlue4", 16, 78, 139 },
    { "firebrick", 178, 34, 34 },
    { "firebrick1", 255, 48, 48 },
    { "firebrick2", 238, 44, 44 },
    { "firebrick3", 205, 38, 38 },
    { "firebrick4", 139, 26, 26 },
    { "floral white", 255, 250, 240 },
    { "FloralWhite", 255, 250, 240 },
    { "forest green", 34, 139, 34 },
    { "ForestGreen", 34, 139, 34 },
    { "gainsboro", 220, 220, 220 },
    { "ghost white", 248, 248, 255 },
    { "GhostWhite", 248, 248, 255 },
    { "gold", 255, 215, 0 },
    { "gold1", 255, 215, 0 },
    { "gold2", 238, 201, 0 },
    { "gold3", 205, 173, 0 },
    { "gold4", 139, 117, 0 },
    { "goldenrod", 218, 165, 32 },
    { "goldenrod1", 255, 193, 37 },
    { "goldenrod2", 238, 180, 34 },
    { "goldenrod3", 205, 155, 29 },
    { "goldenrod4", 139, 105, 20 },
    { "gray", 190, 190, 190 },
    { "gray0", 0, 0, 0 },
    { "gray1", 3, 3, 3 },
    { "gray10", 26, 26, 26 },
    { "gray100", 255, 255, 255 },
    { "gray11", 28, 28, 28 },
    { "gray12", 31, 31, 31 },
    { "gray13", 33, 33, 33 },
    { "gray14", 36, 36, 36 },
    { "gray15", 38, 38, 38 },
    { "gray16", 41, 41, 41 },
    { "gray17", 43, 43, 43 },
    { "gray18", 46, 46, 46 },
    { "gray19", 48, 48, 48 },
    { "gray2", 5, 5, 5 },
    { "gray20", 51, 51, 51 },
    { "gray21", 54, 54, 54 },
    { "gray22", 56, 56, 56 },
    { "gray23", 59, 59, 59 },
    { "gray24", 61, 61, 61 },
    { "gray25", 64, 64, 64 },
    { "gray26", 66, 66, 66 },
    { "gray27", 69, 69, 69 },
    { "gray28", 71, 71, 71 },
    { "gray29", 74, 74, 74 },
    { "gray3", 8, 8, 8 },
    { "gray30", 77, 77, 77 },
    { "gray31", 79, 79, 79 },
    { "gray32", 82, 82, 82 },
    { "gray33", 84, 84, 84 },
    { "gray34", 87, 87, 87 },
    { "gray35", 89, 89, 89 },
    { "gray36", 92, 92, 92 },
    { "gray37", 94, 94, 94 },
    { "gray38", 97, 97, 97 },
    { "gray39", 99, 99, 99 },
    { "gray4", 10, 10, 10 },
    { "gray40", 102, 102, 102 },
    { "gray41", 105, 105, 105 },
    { "gray42", 107, 107, 107 },
    { "gray43", 110, 110, 110 },
    { "gray44", 112, 112, 112 },
    { "gray45", 115, 115, 115 },
    { "gray46", 117, 117, 117 },
    { "gray47", 120, 120, 120 },
    { "gray48", 122, 122, 122 },
    { "gray49", 125, 125, 125 },
    { "gray5", 13, 13, 13 },
    { "gray50", 127, 127, 127 },
    { "gray51", 130, 130, 130 },
    { "gray52", 133, 133, 133 },
    { "gray53", 135, 135, 135 },
    { "gray54", 138, 138, 138 },
    { "gray55", 140, 140, 140 },
    { "gray56", 143, 143, 143 },
    { "gray57", 145, 145, 145 },
    { "gray58", 148, 148, 148 },
    { "gray59", 150, 150, 150 },
    { "gray6", 15, 15, 15 },
    { "gray60", 153, 153, 153 },
    { "gray61", 156, 156, 156 },
    { "gray62", 158, 158, 158 },
    { "gray63", 161, 161, 161 },
    { "gray64", 163, 163, 163 },
    { "gray65", 166, 166, 166 },
    { "gray66", 168, 168, 168 },
    { "gray67", 171, 171, 171 },
    { "gray68", 173, 173, 173 },
    { "gray69", 176, 176, 176 },
    { "gray7", 18, 18, 18 },
    { "gray70", 179, 179, 179 },
    { "gray71", 181, 181, 181 },
    { "gray72", 184, 184, 184 },
    { "gray73", 186, 186, 186 },
    { "gray74", 189, 189, 189 },
    { "gray75", 191, 191, 191 },
    { "gray76", 194, 194, 194 },
    { "gray77", 196, 196, 196 },
    { "gray78", 199, 199, 199 },
    { "gray79", 201, 201, 201 },
    { "gray8", 20, 20, 20 },
    { "gray80", 204, 204, 204 },
    { "gray81", 207, 207, 207 },
    { "gray82", 209, 209, 209 },
    { "gray83", 212, 212, 212 },
    { "gray84", 214, 214, 214 },
    { "gray85", 217, 217, 217 },
    { "gray86", 219, 219, 219 },
    { "gray87", 222, 222, 222 },
    { "gray88", 224, 224, 224 },
    { "gray89", 227, 227, 227 },
    { "gray9", 23, 23, 23 },
    { "gray90", 229, 229, 229 },
    { "gray91", 232, 232, 232 },
    { "gray92", 235, 235, 235 },
    { "gray93", 237, 237, 237 },
    { "gray94", 240, 240, 240 },
    { "gray95", 242, 242, 242 },
    { "gray96", 245, 245, 245 },
    { "gray97", 247, 247, 247 },
    { "gray98", 250, 250, 250 },
    { "gray99", 252, 252, 252 },
    { "green", 0, 255, 0 },
    { "green yellow", 173, 255, 47 },
    { "green1", 0, 255, 0 },
    { "green2", 0, 238, 0 },
    { "green3", 0, 205, 0 },
    { "green4", 0, 139, 0 },
    { "GreenYellow", 173, 255, 47 },
    { "grey", 190, 190, 190 },
    { "grey0", 0, 0, 0 },
    { "grey1", 3, 3, 3 },
    { "grey10", 26, 26, 26 },
    { "grey100", 255, 255, 255 },
    { "grey11", 28, 28, 28 },
    { "grey12", 31, 31, 31 },
    { "grey13", 33, 33, 33 },
    { "grey14", 36, 36, 36 },
    { "grey15", 38, 38, 38 },
    { "grey16", 41, 41, 41 },
    { "grey17", 43, 43, 43 },
    { "grey18", 46, 46, 46 },
    { "grey19", 48, 48, 48 },
    { "grey2", 5, 5, 5 },
    { "grey20", 51, 51, 51 },
    { "grey21", 54, 54, 54 },
    { "grey22", 56, 56, 56 },
    { "grey23", 59, 59, 59 },
    { "grey24", 61, 61, 61 },
    { "grey25", 64, 64, 64 },
    { "grey26", 66, 66, 66 },
    { "grey27", 69, 69, 69 },
    { "grey28", 71, 71, 71 },
    { "grey29", 74, 74, 74 },
    { "grey3", 8, 8, 8 },
    { "grey30", 77, 77, 77 },
    { "grey31", 79, 79, 79 },
    { "grey32", 82, 82, 82 },
    { "grey33", 84, 84, 84 },
    { "grey34", 87, 87, 87 },
    { "grey35", 89, 89, 89 },
    { "grey36", 92, 92, 92 },
    { "grey37", 94, 94, 94 },
    { "grey38", 97, 97, 97 },
    { "grey39", 99, 99, 99 },
    { "grey4", 10, 10, 10 },
    { "grey40", 102, 102, 102 },
    { "grey41", 105, 105, 105 },
    { "grey42", 107, 107, 107 },
    { "grey43", 110, 110, 110 },
    { "grey44", 112, 112, 112 },
    { "grey45", 115, 115, 115 },
    { "grey46", 117, 117, 117 },
    { "grey47", 120, 120, 120 },
    { "grey48", 122, 122, 122 },
    { "grey49", 125, 125, 125 },
    { "grey5", 13, 13, 13 },
    { "grey50", 127, 127, 127 },
    { "grey51", 130, 130, 130 },
    { "grey52", 133, 133, 133 },
    { "grey53", 135, 135, 135 },
    { "grey54", 138, 138, 138 },
    { "grey55", 140, 140, 140 },
    { "grey56", 143, 143, 143 },
    { "grey57", 145, 145, 145 },
    { "grey58", 148, 148, 148 },
    { "grey59", 150, 150, 150 },
    { "grey6", 15, 15, 15 },
    { "grey60", 153, 153, 153 },
    { "grey61", 156, 156, 156 },
    { "grey62", 158, 158, 158 },
    { "grey63", 161, 161, 161 },
    { "grey64", 163, 163, 163 },
    { "grey65", 166, 166, 166 },
    { "grey66", 168, 168, 168 },
    { "grey67", 171, 171, 171 },
    { "grey68", 173, 173, 173 },
    { "grey69", 176, 176, 176 },
    { "grey7", 18, 18, 18 },
    { "grey70", 179, 179, 179 },
    { "grey71", 181, 181, 181 },
    { "grey72", 184, 184, 184 },
    { "grey73", 186, 186, 186 },
    { "grey74", 189, 189, 189 },
    { "grey75", 191, 191, 191 },
    { "grey76", 194, 194, 194 },
    { "grey77", 196, 196, 196 },
    { "grey78", 199, 199, 199 },
    { "grey79", 201, 201, 201 },
    { "grey8", 20, 20, 20 },
    { "grey80", 204, 204, 204 },
    { "grey81", 207, 207, 207 },
    { "grey82", 209, 209, 209 },
    { "grey83", 212, 212, 212 },
    { "grey84", 214, 214, 214 },
    { "grey85", 217, 217, 217 },
    { "grey86", 219, 219, 219 },
    { "grey87", 222, 222, 222 },
    { "grey88", 224, 224, 224 },
    { "grey89", 227, 227, 227 },
    { "grey9", 23, 23, 23 },
    { "grey90", 229, 229, 229 },
    { "grey91", 232, 232, 232 },
    { "grey92", 235, 235, 235 },
    { "grey93", 237, 237, 237 },
    { "grey94", 240, 240, 240 },
    { "grey95", 242, 242, 242 },
    { "grey96", 245, 245, 245 },
    { "grey97", 247, 247, 247 },
    { "grey98", 250, 250, 250 },
    { "grey99", 252, 252, 252 },
    { "honeydew", 240, 255, 240 },
    { "honeydew1", 240, 255, 240 },
    { "honeydew2", 224, 238, 224 },
    { "honeydew3", 193, 205, 193 },
    { "honeydew4", 131, 139, 131 },
    { "hot pink", 255, 105, 180 },
    { "HotPink", 255, 105, 180 },
    { "HotPink1", 255, 110, 180 },
    { "HotPink2", 238, 106, 167 },
    { "HotPink3", 205, 96, 144 },
    { "HotPink4", 139, 58, 98 },
    { "indian red", 205, 92, 92 },
    { "IndianRed", 205, 92, 92 },
    { "IndianRed1", 255, 106, 106 },
    { "IndianRed2", 238, 99, 99 },
    { "IndianRed3", 205, 85, 85 },
    { "IndianRed4", 139, 58, 58 },
    { "ivory", 255, 255, 240 },
    { "ivory1", 255, 255, 240 },
    { "ivory2", 238, 238, 224 },
    { "ivory3", 205, 205, 193 },
    { "ivory4", 139, 139, 131 },
    { "khaki", 240, 230, 140 },
    { "khaki1", 255, 246, 143 },
    { "khaki2", 238, 230, 133 },
    { "khaki3", 205, 198, 115 },
    { "khaki4", 139, 134, 78 },
    { "lavender", 230, 230, 250 },
    { "lavender blush", 255, 240, 245 },
    { "LavenderBlush", 255, 240, 245 },
    { "LavenderBlush1", 255, 240, 245 },
    { "LavenderBlush2", 238, 224, 229 },
    { "LavenderBlush3", 205, 193, 197 },
    { "LavenderBlush4", 139, 131, 134 },
    { "lawn green", 124, 252, 0 },
    { "LawnGreen", 124, 252, 0 },
    { "lemon chiffon", 255, 250, 205 },
    { "LemonChiffon", 255, 250, 205 },
    { "LemonChiffon1", 255, 250, 205 },
    { "LemonChiffon2", 238, 233, 191 },
    { "LemonChiffon3", 205, 201, 165 },
    { "LemonChiffon4", 139, 137, 112 },
    { "light blue", 173, 216, 230 },
    { "light coral", 240, 128, 128 },
    { "light cyan", 224, 255, 255 },
    { "light goldenrod", 238, 221, 130 },
    { "light goldenrod yellow", 250, 250, 210 },
    { "light gray", 211, 211, 211 },
    { "light green", 144, 238, 144 },
    { "light grey", 211, 211, 211 },
    { "light pink", 255, 182, 193 },
    { "light salmon", 255, 160, 122 },
    { "light sea green", 32, 178, 170 },
    { "light sky blue", 135, 206, 250 },
    { "light slate blue", 132, 112, 255 },
    { "light slate gray", 119, 136, 153 },
    { "light slate grey", 119, 136, 153 },
    { "light steel blue", 176, 196, 222 },
    { "light yellow", 255, 255, 224 },
    { "LightBlue", 173, 216, 230 },
    { "LightBlue1", 191, 239, 255 },
    { "LightBlue2", 178, 223, 238 },
    { "LightBlue3", 154, 192, 205 },
    { "LightBlue4", 104, 131, 139 },
    { "LightCoral", 240, 128, 128 },
    { "LightCyan", 224, 255, 255 },
    { "LightCyan1", 224, 255, 255 },
    { "LightCyan2", 209, 238, 238 },
    { "LightCyan3", 180, 205, 205 },
    { "LightCyan4", 122, 139, 139 },
    { "LightGoldenrod", 238, 221, 130 },
    { "LightGoldenrod1", 255, 236, 139 },
    { "LightGoldenrod2", 238, 220, 130 },
    { "LightGoldenrod3", 205, 190, 112 },
    { "LightGoldenrod4", 139, 129, 76 },
    { "LightGoldenrodYellow", 250, 250, 210 },
    { "LightGray", 211, 211, 211 },
    { "LightGreen", 144, 238, 144 },
    { "LightGrey", 211, 211, 211 },
    { "LightPink", 255, 182, 193 },
    { "LightPink1", 255, 174, 185 },
    { "LightPink2", 238, 162, 173 },
    { "LightPink3", 205, 140, 149 },
    { "LightPink4", 139, 95, 101 },
    { "LightSalmon", 255, 160, 122 },
    { "LightSalmon1", 255, 160, 122 },
    { "LightSalmon2", 238, 149, 114 },
    { "LightSalmon3", 205, 129, 98 },
    { "LightSalmon4", 139, 87, 66 },
    { "LightSeaGreen", 32, 178, 170 },
    { "LightSkyBlue", 135, 206, 250 },
    { "LightSkyBlue1", 176, 226, 255 },
    { "LightSkyBlue2", 164, 211, 238 },
    { "LightSkyBlue3", 141, 182, 205 },
    { "LightSkyBlue4", 96, 123, 139 },
    { "LightSlateBlue", 132, 112, 255 },
    { "LightSlateGray", 119, 136, 153 },
    { "LightSlateGrey", 119, 136, 153 },
    { "LightSteelBlue", 176, 196, 222 },
    { "LightSteelBlue1", 202, 225, 255 },
    { "LightSteelBlue2", 188, 210, 238 },
    { "LightSteelBlue3", 162, 181, 205 },
    { "LightSteelBlue4", 110, 123, 139 },
    { "LightYellow", 255, 255, 224 },
    { "LightYellow1", 255, 255, 224 },
    { "LightYellow2", 238, 238, 209 },
    { "LightYellow3", 205, 205, 180 },
    { "LightYellow4", 139, 139, 122 },
    { "lime green", 50, 205, 50 },
    { "LimeGreen", 50, 205, 50 },
    { "linen", 250, 240, 230 },
    { "magenta", 255, 0, 255 },
    { "magenta1", 255, 0, 255 },
    { "magenta2", 238, 0, 238 },
    { "magenta3", 205, 0, 205 },
    { "magenta4", 139, 0, 139 },
    { "maroon", 176, 48, 96 },
    { "maroon1", 255, 52, 179 },
    { "maroon2", 238, 48, 167 },
    { "maroon3", 205, 41, 144 },
    { "maroon4", 139, 28, 98 },
    { "medium aquamarine", 102, 205, 170 },
    { "medium blue", 0, 0, 205 },
    { "medium orchid", 186, 85, 211 },
    { "medium purple", 147, 112, 219 },
    { "medium sea green", 60, 179, 113 },
    { "medium slate blue", 123, 104, 238 },
    { "medium spring green", 0, 250, 154 },
    { "medium turquoise", 72, 209, 204 },
    { "medium violet red", 199, 21, 133 },
    { "MediumAquamarine", 102, 205, 170 },
    { "MediumBlue", 0, 0, 205 },
    { "MediumOrchid", 186, 85, 211 },
    { "MediumOrchid1", 224, 102, 255 },
    { "MediumOrchid2", 209, 95, 238 },
    { "MediumOrchid3", 180, 82, 205 },
    { "MediumOrchid4", 122, 55, 139 },
    { "MediumPurple", 147, 112, 219 },
    { "MediumPurple1", 171, 130, 255 },
    { "MediumPurple2", 159, 121, 238 },
    { "MediumPurple3", 137, 104, 205 },
    { "MediumPurple4", 93, 71, 139 },
    { "MediumSeaGreen", 60, 179, 113 },
    { "MediumSlateBlue", 123, 104, 238 },
    { "MediumSpringGreen", 0, 250, 154 },
    { "MediumTurquoise", 72, 209, 204 },
    { "MediumVioletRed", 199, 21, 133 },
    { "midnight blue", 25, 25, 112 },
    { "MidnightBlue", 25, 25, 112 },
    { "mint cream", 245, 255, 250 },
    { "MintCream", 245, 255, 250 },
    { "misty rose", 255, 228, 225 },
    { "MistyRose", 255, 228, 225 },
    { "MistyRose1", 255, 228, 225 },
    { "MistyRose2", 238, 213, 210 },
    { "MistyRose3", 205, 183, 181 },
    { "MistyRose4", 139, 125, 123 },
    { "moccasin", 255, 228, 181 },
    { "navajo white", 255, 222, 173 },
    { "NavajoWhite", 255, 222, 173 },
    { "NavajoWhite1", 255, 222, 173 },
    { "NavajoWhite2", 238, 207, 161 },
    { "NavajoWhite3", 205, 179, 139 },
    { "NavajoWhite4", 139, 121, 94 },
    { "navy", 0, 0, 128 },
    { "navy blue", 0, 0, 128 },
    { "NavyBlue", 0, 0, 128 },
    { "old lace", 253, 245, 230 },
    { "OldLace", 253, 245, 230 },
    { "olive drab", 107, 142, 35 },
    { "OliveDrab", 107, 142, 35 },
    { "OliveDrab1", 192, 255, 62 },
    { "OliveDrab2", 179, 238, 58 },
    { "OliveDrab3", 154, 205, 50 },
    { "OliveDrab4", 105, 139, 34 },
    { "orange", 255, 165, 0 },
    { "orange red", 255, 69, 0 },
    { "orange1", 255, 165, 0 },
    { "orange2", 238, 154, 0 },
    { "orange3", 205, 133, 0 },
    { "orange4", 139, 90, 0 },
    { "OrangeRed", 255, 69, 0 },
    { "OrangeRed1", 255, 69, 0 },
    { "OrangeRed2", 238, 64, 0 },
    { "OrangeRed3", 205, 55, 0 },
    { "OrangeRed4", 139, 37, 0 },
    { "orchid", 218, 112, 214 },
    { "orchid1", 255, 131, 250 },
    { "orchid2", 238, 122, 233 },
    { "orchid3", 205, 105, 201 },
    { "orchid4", 139, 71, 137 },
    { "pale goldenrod", 238, 232, 170 },
    { "pale green", 152, 251, 152 },
    { "pale turquoise", 175, 238, 238 },
    { "pale violet red", 219, 112, 147 },
    { "PaleGoldenrod", 238, 232, 170 },
    { "PaleGreen", 152, 251, 152 },
    { "PaleGreen1", 154, 255, 154 },
    { "PaleGreen2", 144, 238, 144 },
    { "PaleGreen3", 124, 205, 124 },
    { "PaleGreen4", 84, 139, 84 },
    { "PaleTurquoise", 175, 238, 238 },
    { "PaleTurquoise1", 187, 255, 255 },
    { "PaleTurquoise2", 174, 238, 238 },
    { "PaleTurquoise3", 150, 205, 205 },
    { "PaleTurquoise4", 102, 139, 139 },
    { "PaleVioletRed", 219, 112, 147 },
    { "PaleVioletRed1", 255, 130, 171 },
    { "PaleVioletRed2", 238, 121, 159 },
    { "PaleVioletRed3", 205, 104, 137 },
    { "PaleVioletRed4", 139, 71, 93 },
    { "papaya whip", 255, 239, 213 },
    { "PapayaWhip", 255, 239, 213 },
    { "peach puff", 255, 218, 185 },
    { "PeachPuff", 255, 218, 185 },
    { "PeachPuff1", 255, 218, 185 },
    { "PeachPuff2", 238, 203, 173 },
    { "PeachPuff3", 205, 175, 149 },
    { "PeachPuff4", 139, 119, 101 },
    { "peru", 205, 133, 63 },
    { "pink", 255, 192, 203 },
    { "pink1", 255, 181, 197 },
    { "pink2", 238, 169, 184 },
    { "pink3", 205, 145, 158 },
    { "pink4", 139, 99, 108 },
    { "plum", 221, 160, 221 },
    { "plum1", 255, 187, 255 },
    { "plum2", 238, 174, 238 },
    { "plum3", 205, 150, 205 },
    { "plum4", 139, 102, 139 },
    { "powder blue", 176, 224, 230 },
    { "PowderBlue", 176, 224, 230 },
    { "purple", 160, 32, 240 },
    { "purple1", 155, 48, 255 },
    { "purple2", 145, 44, 238 },
    { "purple3", 125, 38, 205 },
    { "purple4", 85, 26, 139 },
    { "red", 255, 0, 0 },
    { "red1", 255, 0, 0 },
    { "red2", 238, 0, 0 },
    { "red3", 205, 0, 0 },
    { "red4", 139, 0, 0 },
    { "rosy brown", 188, 143, 143 },
    { "RosyBrown", 188, 143, 143 },
    { "RosyBrown1", 255, 193, 193 },
    { "RosyBrown2", 238, 180, 180 },
    { "RosyBrown3", 205, 155, 155 },
    { "RosyBrown4", 139, 105, 105 },
    { "royal blue", 65, 105, 225 },
    { "RoyalBlue", 65, 105, 225 },
    { "RoyalBlue1", 72, 118, 255 },
    { "RoyalBlue2", 67, 110, 238 },
    { "RoyalBlue3", 58, 95, 205 },
    { "RoyalBlue4", 39, 64, 139 },
    { "saddle brown", 139, 69, 19 },
    { "SaddleBrown", 139, 69, 19 },
    { "salmon", 250, 128, 114 },
    { "salmon1", 255, 140, 105 },
    { "salmon2", 238, 130, 98 },
    { "salmon3", 205, 112, 84 },
    { "salmon4", 139, 76, 57 },
    { "sandy brown", 244, 164, 96 },
    { "SandyBrown", 244, 164, 96 },
    { "sea green", 46, 139, 87 },
    { "SeaGreen", 46, 139, 87 },
    { "SeaGreen1", 84, 255, 159 },
    { "SeaGreen2", 78, 238, 148 },
    { "SeaGreen3", 67, 205, 128 },
    { "SeaGreen4", 46, 139, 87 },
    { "seashell", 255, 245, 238 },
    { "seashell1", 255, 245, 238 },
    { "seashell2", 238, 229, 222 },
    { "seashell3", 205, 197, 191 },
    { "seashell4", 139, 134, 130 },
    { "sienna", 160, 82, 45 },
    { "sienna1", 255, 130, 71 },
    { "sienna2", 238, 121, 66 },
    { "sienna3", 205, 104, 57 },
    { "sienna4", 139, 71, 38 },
    { "sky blue", 135, 206, 235 },
    { "SkyBlue", 135, 206, 235 },
    { "SkyBlue1", 135, 206, 255 },
    { "SkyBlue2", 126, 192, 238 },
    { "SkyBlue3", 108, 166, 205 },
    { "SkyBlue4", 74, 112, 139 },
    { "slate blue", 106, 90, 205 },
    { "slate gray", 112, 128, 144 },
    { "slate grey", 112, 128, 144 },
    { "SlateBlue", 106, 90, 205 },
    { "SlateBlue1", 131, 111, 255 },
    { "SlateBlue2", 122, 103, 238 },
    { "SlateBlue3", 105, 89, 205 },
    { "SlateBlue4", 71, 60, 139 },
    { "SlateGray", 112, 128, 144 },
    { "SlateGray1", 198, 226, 255 },
    { "SlateGray2", 185, 211, 238 },
    { "SlateGray3", 159, 182, 205 },
    { "SlateGray4", 108, 123, 139 },
    { "SlateGrey", 112, 128, 144 },
    { "snow", 255, 250, 250 },
    { "snow1", 255, 250, 250 },
    { "snow2", 238, 233, 233 },
    { "snow3", 205, 201, 201 },
    { "snow4", 139, 137, 137 },
    { "spring green", 0, 255, 127 },
    { "SpringGreen", 0, 255, 127 },
    { "SpringGreen1", 0, 255, 127 },
    { "SpringGreen2", 0, 238, 118 },
    { "SpringGreen3", 0, 205, 102 },
    { "SpringGreen4", 0, 139, 69 },
    { "steel blue", 70, 130, 180 },
    { "SteelBlue", 70, 130, 180 },
    { "SteelBlue1", 99, 184, 255 },
    { "SteelBlue2", 92, 172, 238 },
    { "SteelBlue3", 79, 148, 205 },
    { "SteelBlue4", 54, 100, 139 },
    { "tan", 210, 180, 140 },
    { "tan1", 255, 165, 79 },
    { "tan2", 238, 154, 73 },
    { "tan3", 205, 133, 63 },
    { "tan4", 139, 90, 43 },
    { "thistle", 216, 191, 216 },
    { "thistle1", 255, 225, 255 },
    { "thistle2", 238, 210, 238 },
    { "thistle3", 205, 181, 205 },
    { "thistle4", 139, 123, 139 },
    { "tomato", 255, 99, 71 },
    { "tomato1", 255, 99, 71 },
    { "tomato2", 238, 92, 66 },
    { "tomato3", 205, 79, 57 },
    { "tomato4", 139, 54, 38 },
    { "turquoise", 64, 224, 208 },
    { "turquoise1", 0, 245, 255 },
    { "turquoise2", 0, 229, 238 },
    { "turquoise3", 0, 197, 205 },
    { "turquoise4", 0, 134, 139 },
    { "violet", 238, 130, 238 },
    { "violet red", 208, 32, 144 },
    { "VioletRed", 208, 32, 144 },
    { "VioletRed1", 255, 62, 150 },
    { "VioletRed2", 238, 58, 140 },
    { "VioletRed3", 205, 50, 120 },
    { "VioletRed4", 139, 34, 82 },
    { "wheat", 245, 222, 179 },
    { "wheat1", 255, 231, 186 },
    { "wheat2", 238, 216, 174 },
    { "wheat3", 205, 186, 150 },
    { "wheat4", 139, 126, 102 },
    { "white", 255, 255, 255 },
    { "white smoke", 245, 245, 245 },
    { "WhiteSmoke", 245, 245, 245 },
    { "yellow", 255, 255, 0 },
    { "yellow green", 154, 205, 50 },
    { "yellow1", 255, 255, 0 },
    { "yellow2", 238, 238, 0 },
    { "yellow3", 205, 205, 0 },
    { "yellow4", 139, 139, 0 },
    { "YellowGreen", 154, 205, 50 }
};
 
static int
compare_xcolor_entries (const void *a, const void *b)
{
  return g_strcasecmp ((const char *) a, ((const XColorEntry *) b)->name);
}

static gboolean
find_named_color (const gchar *name,
		  GdkColor    *colorPtr)
{
  XColorEntry *found;

  found = bsearch (name, xColors, G_N_ELEMENTS (xColors), sizeof (XColorEntry),
		   compare_xcolor_entries);
  if (found == NULL)
    return FALSE;
  
  colorPtr->red = (found->red * 65535) / 255;
  colorPtr->green = (found->green * 65535) / 255;
  colorPtr->blue = (found->blue * 65535) / 255;

  return TRUE;
}

gboolean
parse_color (const gchar *spec,
	     GdkColor    *colorPtr)
{
  if (spec[0] == '#')
    {
      char fmt[16];
      int i, red, green, blue;
      
      if ((i = strlen(spec+1))%3) 
	return FALSE;

      i /= 3;

      sprintf(fmt, "%%%dx%%%dx%%%dx", i, i, i);
      if (sscanf(spec+1, fmt, &red, &green, &blue) != 3) 
	return FALSE;

      if (i == 4)
	{
	  colorPtr->red = red;
	  colorPtr->green = green;
	  colorPtr->blue = blue;
	}
      else if (i == 1)
	{
	  colorPtr->red = (red * 65535) / 15;
	  colorPtr->green = (green * 65535) / 15;
	  colorPtr->blue = (blue * 65535) / 15;
	}
      else if (i == 2)
	{
	  colorPtr->red = (red * 65535) / 255;
	  colorPtr->green = (green * 65535) / 255;
	  colorPtr->blue = (blue * 65535) / 255;
	}
      else /* if (i == 3) */
	{
	  colorPtr->red = (red * 65535) / 4095;
	  colorPtr->green = (green * 65535) / 4095;
	  colorPtr->blue = (blue * 65535) / 4095;
	}
    }
  else if (!find_named_color (spec, colorPtr))
    return FALSE;

  return TRUE;
}

/* End of code from Tk8.0 */

GdkColormap*
gdk_colormap_new (GdkVisual *visual,
		  gint       private_cmap)
{
  GdkColormap *colormap;
  GdkColormapPrivateWin32 *cmapp;

  g_return_val_if_fail (visual != NULL, NULL);

  cmapp = g_new (GdkColormapPrivateWin32, 1);
  colormap = (GdkColormap*) cmapp;

  GDK_NOTE (COLORMAP, g_print ("gdk_colormap_new: visual=%p priv=%s: %p\n",
			       visual, private_cmap ? "TRUE" : "FALSE",
			       colormap));

  cmapp->base.visual = visual;
  cmapp->base.ref_count = 1;

  cmapp->hash = NULL;
  cmapp->info = NULL;
  
  colormap->size = visual->colormap_size;
  colormap->colors = NULL;

  switch (visual->type)
    {
    case GDK_VISUAL_GRAYSCALE:
    case GDK_VISUAL_PSEUDO_COLOR:
      cmapp->info = g_new0 (GdkColorInfo, colormap->size);
      colormap->colors = g_new (GdkColor, colormap->size);
      
      cmapp->hash = g_hash_table_new ((GHashFunc) gdk_color_hash,
					(GCompareFunc) gdk_color_equal);
      
      create_colormap (cmapp, private_cmap);

      if (private_cmap)
	{
	  PALETTEENTRY *pe;
	  gint i, npal;

	  npal = MIN (palette_size (cmapp->hpal), colormap->size);
	  pe = g_new (PALETTEENTRY, npal);
	  npal = GetPaletteEntries (cmapp->hpal, 0, npal, pe);
	  for (i = 0; i < colormap->size; i++)
	    {
	      colormap->colors[i].pixel = i;
	      if (i >= npal)
		{
		  colormap->colors[i].red =
		    colormap->colors[i].green =
		    colormap->colors[i].blue = 0;
		}
	      else
		{
		  colormap->colors[i].red = (pe[i].peRed * 65535) / 255;
		  colormap->colors[i].green = (pe[i].peGreen * 65525) / 255;
		  colormap->colors[i].blue = (pe[i].peBlue * 65535) / 255;
		}
	    }
	  gdk_colormap_change (colormap, colormap->size);
	  g_free (pe);
	}
      break;

    case GDK_VISUAL_STATIC_GRAY:
    case GDK_VISUAL_STATIC_COLOR:
      create_colormap (cmapp, FALSE);
      break;

    case GDK_VISUAL_TRUE_COLOR:
      break;

    default:
      g_assert_not_reached ();
    }

  return colormap;
}

void
_gdk_colormap_real_destroy (GdkColormap *colormap)
{
  GdkColormapPrivateWin32 *cmapp = (GdkColormapPrivateWin32 *) colormap;

  g_return_if_fail (colormap != NULL);
  g_return_if_fail (cmapp->base.ref_count == 0);

  GDK_NOTE (COLORMAP, g_print ("_gdk_colormap_real_destroy: cmap=%p hpal=%p\n",
			       colormap, cmapp->hpal));

  if (!DeleteObject (cmapp->hpal))
    WIN32_GDI_FAILED ("DeleteObject (palette) failed");

  if (cmapp->hash)
    g_hash_table_destroy (cmapp->hash);
  
  g_free (cmapp->info);
  g_free (colormap->colors);
  g_free (colormap);
}

GdkColormap*
gdk_colormap_get_system (void)
{
  static GdkColormap *colormap = NULL;
  GdkColormapPrivateWin32 *cmapp;
  PALETTEENTRY *pe;
  gint nlookup;
  gint i;

  if (!colormap)
    {
      colormap = gdk_colormap_new (gdk_visual_get_system (), FALSE);
      cmapp = (GdkColormapPrivateWin32 *) colormap;

      switch (cmapp->base.visual->type)
	{
	case GDK_VISUAL_GRAYSCALE:
	case GDK_VISUAL_PSEUDO_COLOR:
	  pe = g_new (PALETTEENTRY, colormap->size);
	  nlookup = GetPaletteEntries (cmapp->hpal, 0, colormap->size, pe);
	  
	  GDK_NOTE (COLORMAP, (g_print ("System colormap: %p hpal=%p: %d entries\n",
					cmapp, cmapp->hpal, nlookup),
			       gdk_win32_print_paletteentries (pe, nlookup)));
	  
	  for (i = 0; i < nlookup; i++)
	    {
	      colormap->colors[i].pixel = i;
	      colormap->colors[i].red = (pe[i].peRed * 65535) / 255;
	      colormap->colors[i].green = (pe[i].peGreen * 65535) / 255;
	      colormap->colors[i].blue = (pe[i].peBlue * 65535) / 255;
	    }
	  
	  for ( ; i < colormap->size; i++)
	    {
	      colormap->colors[i].pixel = i;
	      colormap->colors[i].red = 0;
	      colormap->colors[i].green = 0;
	      colormap->colors[i].blue = 0;
	    }
	  
	  g_free (pe);
	  break;

	default:
	  break;
	}
    }

  return colormap;
}

gint
gdk_colormap_get_system_size (void)
{
  return gdk_colormap_get_system ()->size;
}

void
gdk_colormap_change (GdkColormap *colormap,
		     gint         ncolors)
{
  GdkColormapPrivateWin32 *cmapp;
  PALETTEENTRY *pe;
  int i;

  g_return_if_fail (colormap != NULL);

  cmapp = (GdkColormapPrivateWin32 *) colormap;

  GDK_NOTE (COLORMAP, g_print ("gdk_colormap_change: hpal=%p ncolors=%d\n",
			       cmapp->hpal, ncolors));

  switch (cmapp->base.visual->type)
    {
    case GDK_VISUAL_GRAYSCALE:
    case GDK_VISUAL_PSEUDO_COLOR:
      pe = g_new (PALETTEENTRY, ncolors);

      for (i = 0; i < ncolors; i++)
	{
	  pe[i].peRed = (colormap->colors[i].red >> 8);
	  pe[i].peGreen = (colormap->colors[i].green >> 8);
	  pe[i].peBlue = (colormap->colors[i].blue >> 8);
	  pe[i].peFlags = 0;
	}

      if (!SetPaletteEntries (cmapp->hpal, 0, ncolors, pe))
	WIN32_GDI_FAILED ("SetPaletteEntries");
      g_free (pe);
      break;

    default:
      break;
    }
}

gboolean
gdk_colors_alloc (GdkColormap   *colormap,
		  gint           contiguous,
		  gulong        *planes,
		  gint           nplanes,
		  gulong        *pixels,
		  gint           npixels)
{
  GdkColormapPrivateWin32 *cmapp;
  gint return_val;
  gint i;

  g_return_val_if_fail (colormap != NULL, 0);

  cmapp = (GdkColormapPrivateWin32 *) colormap;

  GDK_NOTE (COLORMAP, g_print ("gdk_colors_alloc: cmap=%p npl=%d npix=%d\n",
			       colormap, nplanes, npixels));

  return_val = alloc_color_cells (cmapp, contiguous, planes, nplanes,
				  pixels, npixels);

  if (return_val)
    {
      for (i = 0; i < npixels; i++)
	{
	  cmapp->info[pixels[i]].ref_count++;
	  cmapp->info[pixels[i]].flags |= GDK_COLOR_WRITEABLE;
	}
    }

  return return_val != 0;
}

gboolean
gdk_color_parse (const gchar *spec,
		 GdkColor *color)
{
  g_return_val_if_fail (spec != NULL, FALSE);
  g_return_val_if_fail (color != NULL, FALSE);

  return parse_color (spec, color);
}

void
gdk_colors_free (GdkColormap *colormap,
		 gulong      *in_pixels,
		 gint         in_npixels,
		 gulong       planes)
{
  GdkColormapPrivateWin32 *cmapp;
  gulong *pixels;
  gint npixels = 0;
  gint i;

  g_return_if_fail (colormap != NULL);
  g_return_if_fail (in_pixels != NULL);

  cmapp = (GdkColormapPrivateWin32 *) colormap;

  if ((cmapp->base.visual->type != GDK_VISUAL_PSEUDO_COLOR) &&
      (cmapp->base.visual->type != GDK_VISUAL_GRAYSCALE))
    return;
  
  GDK_NOTE (COLORMAP, g_print ("gdk_colors_free: cmap=%p npix=%d\n",
			       colormap, in_npixels));

  pixels = g_new (gulong, in_npixels);

  for (i = 0; i < in_npixels; i++)
    {
      gulong pixel = in_pixels[i];
      
      if (cmapp->use[pixel] == GDK_WIN32_PE_STATIC)
	continue;

      if (cmapp->info[pixel].ref_count)
	{
	  cmapp->info[pixel].ref_count--;

	  if (cmapp->info[pixel].ref_count == 0)
	    {
	      pixels[npixels++] = pixel;
	      if (!(cmapp->info[pixel].flags & GDK_COLOR_WRITEABLE))
		g_hash_table_remove (cmapp->hash, &colormap->colors[pixel]);
	      cmapp->info[pixel].flags = 0;
	    }
	}
    }

  if (npixels)
    free_colors (cmapp, pixels, npixels, planes);

  g_free (pixels);
}

void
gdk_colormap_free_colors (GdkColormap *colormap,
			  GdkColor    *colors,
			  gint         ncolors)
{
  gulong *pixels;
  gint i;

  g_return_if_fail (colormap != NULL);
  g_return_if_fail (colors != NULL);

  if ((((GdkColormapPrivate *) colormap)->visual->type != GDK_VISUAL_PSEUDO_COLOR) &&
      (((GdkColormapPrivate *) colormap)->visual->type != GDK_VISUAL_GRAYSCALE))
    return;

  pixels = g_new (gulong, ncolors);

  for (i = 0; i < ncolors; i++)
    pixels[i] =  colors[i].pixel;

  gdk_colors_free (colormap, pixels, ncolors, 0);

  g_free (pixels);
}

/********************
 * Color allocation *
 ********************/

/* Try to allocate a single color using alloc_color. If it succeeds,
 * cache the result in our colormap, and store in ret.
 */
static gboolean 
alloc1 (GdkColormap *colormap,
	GdkColor    *color,
	GdkColor    *ret)
{
  GdkColormapPrivateWin32 *cmapp;
  PALETTEENTRY pe;

  cmapp = (GdkColormapPrivateWin32 *) colormap;

  pe.peRed = color->red >> 8;
  pe.peGreen = color->green >> 8;
  pe.peBlue = color->blue >> 8;

  if (alloc_color (cmapp, &pe, &ret->pixel))
    {
      ret->red = (pe.peRed * 65535) / 255;
      ret->green = (pe.peGreen * 65535) / 255;
      ret->blue = (pe.peBlue * 65535) / 255;
  
      if ((guint) ret->pixel < colormap->size)
	{
	  if (cmapp->info[ret->pixel].ref_count) /* got a duplicate */
	    {
	      GDK_NOTE (COLORMAP, g_print ("...duplicate %d\n",
					   cmapp->info[ret->pixel].ref_count));
	    }
	  else
	    {
	      colormap->colors[ret->pixel] = *color;
	      colormap->colors[ret->pixel].pixel = ret->pixel;
	      cmapp->info[ret->pixel].ref_count = 1;
	      
	      GDK_NOTE (COLORMAP, g_print ("...nonduplicate\n"));
	      
	      g_hash_table_insert (cmapp->hash,
				   &colormap->colors[ret->pixel],
				   &colormap->colors[ret->pixel]);
	    }
	}
      
      return TRUE;
    }

  return FALSE;
}

static gint
alloc_writeable (GdkColormap *colormap,
		 GdkColor    *colors,
		 gint         ncolors,
		 gboolean     writeable,
		 gboolean     best_match,
		 gboolean    *success)
{
  GdkColormapPrivateWin32 *cmapp;
  gulong *pixels;
  gboolean status;
  gint i, index;

  GDK_NOTE (COLORMAP, g_print ("alloc_writeable: "
			       "cmap=%p ncol=%d writ=%s bm=%s\n",
			       colormap, ncolors, writeable ? "TRUE" : "FALSE",
			       best_match ? "TRUE" : "FALSE"));

  cmapp = (GdkColormapPrivateWin32 *) colormap;

  if (cmapp->private_val)
    {
      index = 0;
      for (i = 0; i < ncolors; i++)
	{
	  while ((index < colormap->size) &&
		 (cmapp->info[index].ref_count != 0))
	    index++;
	  
	  if (index < colormap->size)
	    {
	      colors[i].pixel = index;
	      success[i] = TRUE;
	      cmapp->info[index].ref_count++;
	      cmapp->info[i].flags |= GDK_COLOR_WRITEABLE;
	    }
	  else
	    break;
	}
      return i;
    }
  else
    {
      pixels = g_new (gulong, ncolors);

      /* Allocation of a writeable color cells */
      status = alloc_color_cells (cmapp, FALSE, NULL, 0, pixels, ncolors);
      if (status)
	{
	  for (i = 0; i < ncolors; i++)
	    {
	      colors[i].pixel = pixels[i];
	      cmapp->info[pixels[i]].ref_count++;
	      cmapp->info[pixels[i]].flags |= GDK_COLOR_WRITEABLE;
	    }
	}
      
      g_free (pixels);

      return status ? ncolors : 0; 
    }
}

static gint
alloc_private (GdkColormap *colormap,
	       GdkColor    *colors,
	       gint         ncolors,
	       gboolean     writeable,
	       gboolean     best_match,
	       gboolean    *success)
{
  GdkColormapPrivateWin32 *cmapp;
  gint i, index;
  PALETTEENTRY pe;
  gint nremaining = 0;
  
  GDK_NOTE (COLORMAP, g_print ("alloc_private: "
			       "cmap=%p ncol=%d writ=%s bm=%s\n",
			       colormap, ncolors, writeable ? "TRUE" : "FALSE",
			       best_match ? "TRUE" : "FALSE"));

  cmapp = (GdkColormapPrivateWin32 *) colormap;
  index = -1;

  /* First, store the colors we have room for */

  index = 0;
  for (i = 0; i < ncolors; i++)
    {
      if (!success[i])
	{
	  GDK_NOTE (COLORMAP, g_print ("... %s\n",
				       gdk_win32_color_to_string (&colors[i])));
	  while ((index < colormap->size) &&
		 (cmapp->info[index].ref_count != 0))
	    index++;

	  if (index < colormap->size)
	    {
	      if (index >= cmapp->current_size)
		{
		  GDK_NOTE (COLORMAP, g_print ("... hpal=%p resize=%d\n",
					       cmapp->hpal, index + 1));
		  if (!ResizePalette (cmapp->hpal, index + 1))
		    {
		      WIN32_GDI_FAILED ("ResizePalette");
		      nremaining++;
		    }
		  else
		    cmapp->current_size = index + 1;
		}
	      if (index < cmapp->current_size)
		{
		  pe.peRed = colors[i].red >> 8;
		  pe.peGreen = colors[i].green >> 8;
		  pe.peBlue = colors[i].blue >> 8;
		  pe.peFlags = 0;
		  
		  GDK_NOTE (COLORMAP,
			    g_print ("... hpal=%p set ix=%d %02x %02x %02x\n",
				     cmapp->hpal, index,
				     pe.peRed, pe.peGreen, pe.peBlue));
		  if (!SetPaletteEntries (cmapp->hpal, index, 1, &pe))
		    {
		      WIN32_GDI_FAILED ("SetPaletteEntries");
		      nremaining++;
		    }
		  else
		    {
		      success[i] = TRUE;
		      
		      colors[i].pixel = index;
		      colormap->colors[index] = colors[i];
		      cmapp->info[index].ref_count++;
		    }
		}
	    }
	  else
	    nremaining++;
	}
    }
  
  if (nremaining > 0 && best_match)
    {
      /* Get best matches for remaining colors */

      gchar *available = g_new (gchar, colormap->size);
      for (i = 0; i < colormap->size; i++)
	available[i] = TRUE;

      for (i = 0; i < ncolors; i++)
	{
	  if (!success[i])
	    {
	      index = match_color (colormap, &colors[i], available);
	      if (index != -1)
		{
		  colors[i] = colormap->colors[index];
		  GDK_NOTE (COLORMAP,
			    g_print ("... best_match=%d %s\n",
				     index, gdk_win32_color_to_string (&colors[i])));
		  cmapp->info[index].ref_count++;

		  success[i] = TRUE;
		  nremaining--;
		}
	    }
	}
      g_free (available);
    }

  return (ncolors - nremaining);
}

static gint
alloc_shared (GdkColormap *colormap,
	      GdkColor    *colors,
	      gint         ncolors,
	      gboolean     writeable,
	      gboolean     best_match,
	      gboolean    *success)
{
  GdkColormapPrivateWin32 *cmapp;
  gint i, index;
  gint nremaining = 0;
  gint nfailed = 0;

  GDK_NOTE (COLORMAP, g_print ("alloc_shared: "
			       "cmap=%p ncol=%d writ=%s bm=%s\n",
			       colormap, ncolors, writeable ? "TRUE" : "FALSE",
			       best_match ? "TRUE" : "FALSE"));

  cmapp = (GdkColormapPrivateWin32 *) colormap;
  index = -1;

  for (i = 0; i < ncolors; i++)
    {
      if (!success[i])
	{
	  if (alloc1 (colormap, &colors[i], &colors[i]))
	    success[i] = TRUE;
	  else
	    nremaining++;
	}
    }


  if (nremaining > 0 && best_match)
    {
      gchar *available = g_new (gchar, colormap->size);
      for (i = 0; i < colormap->size; i++)
	available[i] = ((cmapp->info[i].ref_count == 0) ||
			!(cmapp->info[i].flags & GDK_COLOR_WRITEABLE));
      while (nremaining > 0)
	{
	  for (i = 0; i < ncolors; i++)
	    {
	      if (!success[i])
		{
		  index = match_color (colormap, &colors[i], available);
		  if (index != -1)
		    {
		      if (cmapp->info[index].ref_count)
			{
			  cmapp->info[index].ref_count++;
			  colors[i] = colormap->colors[index];
			  GDK_NOTE (COLORMAP,
				    g_print ("alloc_shared: best_match=%s\n",
					     gdk_win32_color_to_string (&colors[i])));
			  success[i] = TRUE;
			  nremaining--;
			}
		      else
			{
			  if (alloc1 (colormap, &colormap->colors[index],
				      &colors[i]))
			    {
			      success[i] = TRUE;
			      nremaining--;
			      break;
			    }
			  else
			    {
			      available[index] = FALSE;
			    }
			}
		    }
		  else
		    {
		      nfailed++;
		      nremaining--;
		      success[i] = 2; /* flag as permanent failure */
		    }
		}
	    }
	}
      g_free (available);
    }

  /* Change back the values we flagged as permanent failures */
  if (nfailed > 0)
    {
      for (i = 0; i < ncolors; i++)
	if (success[i] == 2)
	  success[i] = FALSE;
      nremaining = nfailed;
    }
  
  return (ncolors - nremaining);
}

static gint
alloc_pseudocolor (GdkColormap *colormap,
		   GdkColor    *colors,
		   gint         ncolors,
		   gboolean     writeable,
		   gboolean     best_match,
		   gboolean    *success)
{
  GdkColormapPrivateWin32 *cmapp;
  GdkColor *lookup_color;
  gint i;
  gint nremaining = 0;

  GDK_NOTE (COLORMAP, g_print ("alloc_pseudocolor: "
			       "cmap=%p ncol=%d writ=%s bm=%s\n",
			       colormap, ncolors, writeable ? "TRUE" : "FALSE",
			       best_match ? "TRUE" : "FALSE"));

  cmapp = (GdkColormapPrivateWin32 *) colormap;

  /* Check for an exact match among previously allocated colors */

  for (i = 0; i < ncolors; i++)
    {
      GDK_NOTE (COLORMAP, g_print ("...lookup %s:",
				   gdk_win32_color_to_string (&colors[i])));
      lookup_color = g_hash_table_lookup (cmapp->hash, &colors[i]);
      if (lookup_color)
	{
	  GDK_NOTE (COLORMAP,
		    g_print (" %s\n",
			     gdk_win32_color_to_string (lookup_color)));
	  cmapp->info[lookup_color->pixel].ref_count++;
	  colors[i].pixel = lookup_color->pixel;
	  success[i] = TRUE;
	}
      else
	{
	  GDK_NOTE (COLORMAP, g_print ("nope\n"));
	  nremaining++;
	}
    }

  /* If that failed, we try to allocate a new color, or approxmiate
   * with what we can get if best_match is TRUE.
   */
  if (nremaining > 0)
    {
      if (cmapp->private_val)
	return alloc_private (colormap, colors, ncolors,
			      writeable, best_match, success);
      else
	return alloc_shared (colormap, colors, ncolors,
			     writeable, best_match, success);
    }
  else
    return 0;
}

gint
gdk_colormap_alloc_colors (GdkColormap *colormap,
			   GdkColor    *colors,
			   gint         ncolors,
			   gboolean     writeable,
			   gboolean     best_match,
			   gboolean    *success)
{
  GdkColormapPrivateWin32 *cmapp;
  GdkVisual *visual;
  gint i;
  gint nremaining = 0;
  PALETTEENTRY pe;

  g_return_val_if_fail (colormap != NULL, FALSE);
  g_return_val_if_fail (colors != NULL, FALSE);

  cmapp = (GdkColormapPrivateWin32 *) colormap;

  GDK_NOTE (COLORMAP, g_print ("gdk_colormap_alloc_colors: cmap=%p "
			       "ncol=%d writ=%s bm=%s\n",
			       colormap, ncolors, writeable ? "TRUE" : "FALSE",
			       best_match ? "TRUE" : "FALSE"));

  for (i = 0; i < ncolors; i++)
    success[i] = FALSE;

  switch (cmapp->base.visual->type)
    {
    case GDK_VISUAL_PSEUDO_COLOR:
    case GDK_VISUAL_GRAYSCALE:
      if (writeable)
	return alloc_writeable (colormap, colors, ncolors, writeable, best_match, success);
      else
	return alloc_pseudocolor (colormap, colors, ncolors, writeable, best_match, success);
      break;

    case GDK_VISUAL_TRUE_COLOR:
      visual = cmapp->base.visual;

      for (i = 0; i < ncolors; i++)
	{
	  colors[i].pixel =
	    (((colors[i].red >> (16 - visual->red_prec)) << visual->red_shift) +
	     ((colors[i].green >> (16 - visual->green_prec)) << visual->green_shift) +
	     ((colors[i].blue >> (16 - visual->blue_prec)) << visual->blue_shift));
	  success[i] = TRUE;
	}
      break;

    case GDK_VISUAL_STATIC_GRAY:
    case GDK_VISUAL_STATIC_COLOR:
      for (i = 0; i < ncolors; i++)
	{
	  pe.peRed = colors[i].red >> 8;
	  pe.peGreen = colors[i].green >> 8;
	  pe.peBlue = colors[i].blue >> 8;
	  if (alloc_color (cmapp, &pe, &colors[i].pixel))
	    success[i] = TRUE;
	  else
	    nremaining++;
	}
      break;

    default:
      break;
    }
  return nremaining;
}

gboolean
gdk_color_change (GdkColormap *colormap,
		  GdkColor    *color)
{
  GdkColormapPrivateWin32 *cmapp;
  PALETTEENTRY pe;

  g_return_val_if_fail (colormap != NULL, FALSE);
  g_return_val_if_fail (color != NULL, FALSE);

  cmapp = (GdkColormapPrivateWin32 *) colormap;

  GDK_NOTE (COLORMAP, g_print ("gdk_color_change: cmap=%p %s\n",
			       colormap, gdk_win32_color_to_string (color)));

  if (color->pixel < 0 || color->pixel >= colormap->size)
    return FALSE;

  if (cmapp->use[color->pixel] == GDK_WIN32_PE_STATIC)
    return FALSE;

  pe.peRed = color->red >> 8;
  pe.peGreen = color->green >> 8;
  pe.peBlue = color->blue >> 8;
  pe.peFlags = 0;

  if (!SetPaletteEntries (cmapp->hpal, color->pixel, 1, &pe))
    WIN32_GDI_FAILED ("SetPaletteEntries");

  return TRUE;
}

#ifdef G_ENABLE_DEBUG

gchar *
gdk_win32_color_to_string (const GdkColor *color)
{
  static char buf[100];

  sprintf (buf, "(%.04x,%.04x,%.04x):%.06lx",
	   color->red, color->green, color->blue, color->pixel);

  return buf;
}

#endif
