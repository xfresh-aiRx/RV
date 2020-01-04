/*
 * Copyright (C) 1998, 1999, 2000, 2001
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 *
 * This file can be used with the LTI-Computer Vision Library (LTI-Lib)
 * (please note this file is NOT part of the LTI-Lib)
 * This code is based on the original C code by S. Smith, which contains
 * following header:
 *
 **********************************************************************
 *
 * SUSAN Version 2l by Stephen Smith
 *
 * Oxford Centre for Functional Magnetic Resonance Imaging of the
 * Brain, Department of Clinical Neurology, Oxford University, Oxford,
 * UK (Previously in Computer Vision and Image Processing Group - now
 * Computer Vision and Electro Optics Group - DERA Chertsey, UK)
 *
 * Email:    steve@fmrib.ox.ac.uk
 * WWW:      http://www.fmrib.ox.ac.uk/~steve
 *
 * (C) Crown Copyright (1995-1999), Defence Evaluation and Research Agency,
 * Farnborough, Hampshire, GU14 6TD, UK
 * DERA WWW site:
 * http://www.dera.gov.uk/
 * DERA Computer Vision and Electro Optics Group WWW site:
 * http://www.dera.gov.uk/imageprocessing/dera/group_home.html
 * DERA Computer Vision and Electro Optics Group point of contact:
 * Dr. John Savage, jtsavage@dera.gov.uk, +44 1344 633203
 *
 * A UK patent has been granted: "Method for digitally processing
 * images to determine the position of edges and/or corners therein for
 * guidance of unmanned vehicle", UK Patent 2272285. Proprietor:
 * Secretary of State for Defence, UK. 15 January 1997
 *
 * This code is issued for research purposes only and remains the
 * property of the UK Secretary of State for Defence. This code must
 * not be passed on without this header information being kept
 * intact. This code must not be sold.
 **********************************************************************
 *
 *
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
 *
 */

/*----------------------------------------------------------------
 * project ....: LTI Digital Image/Signal Processing Library
 * file .......: ltiSusan.h
 * authors ....: Stefan Syberichs (LTI-Lib port),Stephen Smith (original SUSAN)
 * organization: LTI, RWTH Aachen
 * creation ...: 29.04.2000
 * revisions ..: $Id: ltiSusan.h,v 1.12 2005/07/22 08:19:05 libuda Exp $
 */

#ifndef _LTI_SUSAN_H_
#define _LTI_SUSAN_H_

#include "ltiObject.h"
#include "ltiException.h"
#include "ltiImage.h"

#ifndef HAVE_SUSAN
#ifdef _LTI_MSC_VER
#pragma message("Configuration Error:")
#pragma message("Change in src/basics/ltiWinConfig.h HAVE_SUSAN to 1")
#else
#warning "Configuration Error:"
#warning "Change in src/basics/config.h HAVE_SUSAN to 1"
#endif
#endif

namespace lti {

  ////////////////////////////////////////////////////////////
  //              Exceptions for Susan operator
  ///////////////////////////////////////////////////////////

  /**
   * A class for SUSAN image Processing.
   * The SUSAN principle implements a fast-low level approach for
   * finding binary edges and corners and reducing noise in grey
   * channel images. The basic idea is to move a circular mask of 37
   * pixels over the image and compare the brightness of all
   * peripheral pixels to the brightness of the center. The sum of all
   * pixels who's brightness is equal (or similar) to the center point
   * gives the USAN value (Univalue Segment Assimilating Nucleus),
   * which can be used to determine the presence of edges and corners
   * or to reduce noise. For more details see the <em>Technical Report
   * TR95SMS1c: SUSAN - A New Approach to Low Level Image
   * Processing</em> by
   * <a href="http://www.fmrib.ox.ac.uk/~steve/susan/"> Stephen M. Smith</a>,
   * or refer to the source comments.
   * <b> For the use in applications use the frontends
   * derived from the modifier or functor classes
   * (e.g. lti::susanEdges)!!! </b>
   */
  class susan: public object  {
  public:

    /// susan error exception
    class susanError : public exception {
    public:
      susanError (const std::string &msg) : exception (msg) {};
      const char* getTypeName() const {return "SusanError";};
    };

    /// default constructor
    susan();
    /// copy constructor

    /// destructor
    ~susan();

    /** find edges in img and visualize them in edges.
     *  According to the SUSAN Algorithmn, the binary edges
     *  in <em> img </em> are detected. A copy of <em> img </em> is made in
     *  <em>edges </em> and the edgepoints are highlighted by setting the
     *  brightness to 255. There is no need to cut <em>edges</em> to
     *  the right size, findedges() will do
     *  this (i.e. all data in <em> edges </em> is erased).
     *  findEdges() is notably slower than binaryEdges, because the whole
     *  image data is being copied (and it's just for demonstartion purpose)
     *  @param img the channel with the input image
     *  @param edges a channel8-buffer to store the result.
     */
    void findEdges(const channel8 &img, channel8& edges);


    /** draw binary edge image.
     *  According to the SUSAN Algorithmn, the binary edges in <em>
     *  img </em> are detected and stored in the buffer
     *  <em>binaryEdges</em>.  After that, <em> binaryEdges </em>
     *  contains a map of the image, where all edge points are white
     *  (255), and all other pixels are black(0).
     *  @param img the channel with the input image
     *  @param binEdges a channel8-buffer to store the result in black
     *                  and white.
     */
    void binaryEdges(const channel8 &img, channel8& binEdges);

    /** draw binary edge image.
     *  According to the SUSAN Algorithmn, the binary edges in <em>
     *  img </em> are detected and stored in the buffer
     *  <em>binaryEdges</em>.  After that, <em> binaryEdges </em>
     *  contains a map of the image, where all edge points are white
     *  (255), and all other pixels are black(0).
     *  @param img the channel with the input image
     *  @param binEdges a channel8-buffer to store the result in black
     *                  and white.
     */
    void binaryEdges(const channel &img, channel& binEdges);


    /**
     * set brightness threshold.
     * the brightness threshold is roughly the value to determine the
     * similarity of pixels (counted in grey values. i.e. threshold =
     * 20 declares all pixel similar to the center pixel, who's
     * brightness does not differ by more than roughly +/- 20
     * greylevels).  The default is 20. In effect, a lower threshold
     * finds more edges, a higher threshold finds less edges, since
     * the algorithmn of detecting an edge from an USAN is more subtle
     * than comparing brightness values !  In reality, the similarity
     * function is expressed in gaussian curve rather than a
     * rectangle, where the similarity (0..100, 100 is maximum
     * similarity) is given by <pre> c = 100*exp(-((I-Io)/t)^6) </pre>
     * with <em> Io </em>being the brightness of the center pixel to
     * be compared, and <em>I</em> the brightness of the considered
     * pixel. The exponent <em> 6 </em> is an arbitrary form factor
     * for the gaussian curve.
     *
     * @param t brightness threshold between 1 and 255, default is 20
     * @exception susanError is thrown if threshold is out of range.  */
    void setThreshold(int t);

    /**
     * set the values used to indicate a no-edge (off) and edge (on) pixels
     */
    void setOffOnValues(const ubyte off, const ubyte on);

    /**
     * returns the name of this type
     */
    const char* getTypeName() const {return "susan";};

  private:

    /**
     * marks the edge information from the buffer <em>edgeBuffer</em>
     * by highlighting all edge pixels in <em>in</em> to 255.
     */
    void edgeDraw(ubyte* in, ubyte* edgeBuffer, int xSize,int ySize);

    /**
     * binarise the raw edge information.
     * Binarisation is done by adjusting the raw edge data (values
     * between 1 and 100) to either 255 (edge point) or background
     * (0).
     */
    void binaryDraw(channel8& img);

    /**
     * binarise the raw edge information.
     * Binarisation is done by adjusting the raw edge data (values
     * between 1 and 100) to either 255 (edge point) or background
     * (0).
     */
    void binaryDraw(const channel8& img,channel& dest);


    /** thin the raw edge information.
      * The thinning provides an enhancement to the raw edge image by
      * <ul>
      * <li> deleting all edge points without neigbours (i.e. noise reduction)
      * <li> extending broken edges
      * <li> moving shifted points back into line
      * </ul>
      */
    void thin(int *response,ubyte* edgeBuffer,int xSize,int ySize);

    /** Set up  Brightness Look-up Table.
     * The actual computation that is needed to determine, whether point (x,y)
     * with brightness I is similar to the nucleus Io by the threshold t is:
     *
     *     c = 100*exp(-((I-Io)/t)^6)
     *
     * where 0 means no similarity in brightness and 100 maximum similarity
     * (i.e. identity) (See Figure 5 and equation (4))
     * 0 and 100 have been choosen to obtain a smooth gaussian membership
     * function while still using integer arithmetics for computational
     * efficiency
     * As the variety of possible brightness differences is limited,
     * a brightness LUT is set up in the manner, that
     * <ul>
     * <li> the lutBase pointer points to the middle of the allocated memory
     * <pre>
     * value:  ........ 95 98 99 99 99 99 100 99 99 99 99 98 95 ...........
     *                                     |
     * index:  0                        lutBase                         515
     * </pre>
     * <li> the similarity (= membership value) of a pixel at image position p
     *   and  brightness *p to a kernel-centerpoint cp can be obtained by
     *   the following code:
     * <pre>
     *   cp=lutBase + image[row*xSize+col];
     *   similarity= *(cp-*p);
     * </pre>
     * </ul>
     * This means: the brightness difference gives the distance from
     * the LUT center, the value in this LUT-position gives the
     * similarity
     */
    void setupBrightnessLUT(unsigned char **lutBase, int thresh,int form);

    /**
     * susanEdges - Edgefinder
     *
     * This method handles 2 things:
     *
     * -# find points that are candidates for an edge and calculate the USAN
     *    response
     * -# compute the direction of the edge, i.e. horizontal, vertical or
     *    diagonal (with sign)
     *
     * The direction is needed for the thinning procedure.
     * FINDING EDGE POINTS (1):
     * (see Dera Technical Report TR95SMS1c)
     *
     * To find possible candidate pixels for an edge, the USAN response
     * (Univalue Segment Assimilating Nucleus) is computed for each
     * image-pixel in the following manner:
     *
     * a) move a circular mask of size 37 pixels over the image:
     * \code
     *
     *                                  ooo
     *                                 ooooo
     *                                ooooooo
     *    "+" indicates center        ooo+ooo
     *                                ooooooo
     *                                 ooooo
     *                                  ooo
     * \endcode
     *
     * b) determine the (weighted) number of pixels that have similar
     *    brightness as the center pixel. The similarity is expressed
     *    within the brightness LUT described above.  Every pixel in the
     *    mask contributes a value between 100 (identical brightness) and 0
     *    (far beyond threshold) to a sum that is stored in the variable n
     *    in every mask loop.  (see equation (2) in paper).
     *
     * c) a USAN response for every center pixel is calculated like
     *    response = maxNo - n (or 0, if n > maxNo, equation (3))
     *    where maxNo is a parameter that should be chosen like 2700 (=
     *    0.75 * maximum USAN sum) As a high value of n means "no edge",
     *    a high value of response means "strong edge" respectively. the
     *    response values are stored in the one-dimensional field
     *    response
     *
     * EDGE DIRECTIONS: There are two types of edges to be distinguished
     * - inter-pixel edges: pixel of an (almost) ideal step
     * edge. inter-pixel means: the center of gravity of the
     * corresponding USAN lies between two pixels.
     *
     * - intra-pixel edges: pixel of a band of brightness half the way
     * between the brightness of the two regions generating the
     * edge. The resulting center of gravity coincides with the mask
     * nucleus. (see Figure 6) intra-pixel edges are assumed, if the
     * USAN area is smaller than the mask diameter (= 6 pixels = value
     * 600 for the USAN sum n)
     *
     * the field edgeBuffer contains a "1" if the edge is of inter-pixel
     * type, and a "2" if the edge is of intra-pixel type, otherwise
     * "100" (i.e. the edgeBuffer must be initialized outside the
     * susanEdges() function, see public method findEdges()).
     */
    void susanEdges(const ubyte* in,
		    int*   response,
		    ubyte* edgeBuffer, //mid
		    ubyte* lutBase,    //bp
		    int    maxNo,
		    int    xSize,
		    int    ySize);

    /**
     * Threshold for brightness comparison
     */
    int threshold;

    /**
     * value used for no-edges
     */
    ubyte off;

    /**
     * value used for edges
     */
    ubyte on;
  };

} // namespace lti

#endif

