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
 * file .......: ltiSusan.cpp
 * authors ....: Stefan Syberichs(LTI-Lib port), Stephen Smith(original SUSAN)
 * organization: LTI, RWTH Aachen
 * creation ...: 11.03.2000
 * revisions ..: $Id: ltiSusan.cpp,v 1.11 2003/04/14 08:09:07 sltilib Exp $
 */

#include "ltiMath.h"
#include "ltiSusan.h"
#include "ltiImage.h"

namespace lti {


  susan::susan(){

    threshold = 20; // default

  };


  susan::~susan(){


  };


  void susan::setThreshold(int t) {

    if((t<=255) && (t>=1))
      threshold = t;
    else
      throw susanError("SUSAN edge detection threshold out of range!");
  }

  void susan::setOffOnValues(const ubyte offVal, const ubyte onVal) {
    off = offVal;
    on = onVal;
  }


  // ////////////////////////////////////////////////////////////
  // Susan Functions in ANSI-C style with pointer arithmetic
  // ////////////////////////////////////////////////////////////

  // Brightness Look-up Table
  // The actual computation that is needed to determine, whether point (x,y)
  // with brightness I is similar to the nucleus Io by the threshold t is:
  //
  //     c = 100*exp(-((I-Io)/t)^6)
  //
  // where 0 means no similarity in brightness and 100 maximum similarity
  // (i.e. identity) (See Figure 5 and equation (4))
  // 0 and 100 have been choosen to obtain a smooth gaussian membership
  // function while still using integer arithmetics for computational
  // efficiency
  // As the variety of possible brightness differences is limited,
  // a brightness LUT is set up in the manner, that
  // - the lutBase pointer points to the middle of the allocated memory
  //
  // value:  ........ 95 98 99 99 99 99 100 99 99 99 99 98 95 ...........
  //                                     |
  // index:  0                        lutBase                         515
  //
  // - the similarity (= membership value) of a pixel at image position p
  //   and  brightness *p to a kernel-centerpoint cp can be obtained by
  //   the following code:
  //
  //   cp=lutBase + image[row*xSize+col];
  //   similarity= *(cp-*p);
  //
  // This means: the brightness difference gives the distance from the LUT
  // center, the value in this LUT-position gives the similarity
  //
  // This code is not nice, but it's fast !
  void susan::setupBrightnessLUT(unsigned char **lutBase,int thresh,int form) {

    int   k;
    float temp;

    *lutBase = new ubyte[516];

    // move lutBase pointer to the middle of the allocated memory for
    // efficiency reasons
    // THE POINTER MUST BE MOVED BACK BEFORE DELETION !!!
    *lutBase=*lutBase+258;

    for(k=-256;k<257;k++)
    {
      temp=((float)k)/((float)thresh);
      temp=temp*temp;
      if (form==6)
        temp=temp*temp*temp;
      temp=100.0f*exp(-temp);
      *(*lutBase+k)= (unsigned char)temp;

    }

  }

  // susanEdges - Edgefinder
  // This method handles 2 things:
  // 1) find points that are candidates for an edge and calculate the USAN
  //    response
  // 2) compute the direction of the edge, i.e. horizontal, vertical or
  //    diagonal (with sign)
  // The direction is needed for the thinning procedure.
  // FINDING EDGE POINTS (1):
  // (see Dera Technical Report TR95SMS1c)
  // To find possible candidate pixels for an edge, the USAN response
  // (Univalue Segment Assimilating Nucleus) is computed for each
  // image-pixel in the following manner:
  // a) move a circular mask of size 37 pixels over the image:
  //
  //                                  ooo
  //                                 ooooo
  //                                ooooooo
  //    "+" indicates center        ooo+ooo
  //                                ooooooo
  //                                 ooooo
  //                                  ooo
  //
  // b) determine the (weighted) number of pixels that have similar
  //    brightness as the center pixel. The similarity is expressed
  //    within the brightness LUT described above.  Every pixel in the
  //    mask contributes a value between 100 (identical brightness) and 0
  //    (far beyond threshold) to a sum that is stored in the variable n
  //    in every mask loop.  (see equation (2) in paper).
  // c) a USAN response for every center pixel is calculated like
  //    response = maxNo - n (or 0, if n > maxNo, equation (3))
  //    where maxNo is a parameter that should be chosen like 2700 (=
  //    0.75 * maximum USAN sum) As a high value of n means "no edge",
  //    a high value of response means "strong edge" respectively. the
  //    response values are stored in the one-dimensional field
  //    response
  //
  // EDGE DIRECTIONS: There are two types of edges to be distinguished
  // - inter-pixel edges: pixel of an (almost) ideal step
  // edge. inter-pixel means: the center of gravity of the
  // corresponding USAN lies between two pixels.
  //
  // - intra-pixel edges: pixel of a band of brightness half the way
  // between the brightness of the two regions generating the
  // edge. The resulting center of gravity coincides with the mask
  // nucleus. (see Figure 6) intra-pixel edges are assumed, if the
  // USAN area is smaller than the mask diameter (= 6 pixels = value
  // 600 for the USAN sum n)
  //
  // the field edgeBuffer contains a "1" if the edge is of inter-pixel
  // type, and a "2" if the edge is of intra-pixel type, otherwise
  // "100" (i.e. the edgeBuffer must be initialized outside the
  // susanEdges() function, see public method findEdges()).
  void susan::susanEdges(const ubyte* in,int* response,ubyte* edgeBuffer,
			 ubyte* lutBase, int maxNo,
			 int xSize,int ySize) {
    float z;
    int   do_symmetry, row, col, m, n, a, b, x, y, w;
    unsigned char c,*p,*cp, *inTemp;

    inTemp = (unsigned char*)(in);

    memset (response,0,xSize * ySize * sizeof(int));

    for (row=3;row<ySize-3;row++)
      for (col=3;col<xSize-3;col++) {

	n=100;     // the center itself

	// p starts to run in the upper left pixel of the mask
	p=inTemp + (row-3)*xSize + col - 1;

	// cp holds brightness of the center pixel
        cp=lutBase + inTemp[row*xSize+col];


	// first row of mask
        n+=*(cp-*p++);
        n+=*(cp-*p++);
        n+=*(cp-*p);
        p+=xSize-3;

	// second row of mask
        n+=*(cp-*p++);
        n+=*(cp-*p++);
        n+=*(cp-*p++);
        n+=*(cp-*p++);
        n+=*(cp-*p);
        p+=xSize-5;

        // third row of mask
        n+=*(cp-*p++);
        n+=*(cp-*p++);
        n+=*(cp-*p++);
        n+=*(cp-*p++);
        n+=*(cp-*p++);
        n+=*(cp-*p++);
        n+=*(cp-*p);
        p+=xSize-6;

        // fourth row of mask
        n+=*(cp-*p++);
        n+=*(cp-*p++);
        n+=*(cp-*p);
        p+=2;  // skip center pixel
        n+=*(cp-*p++);
        n+=*(cp-*p++);
        n+=*(cp-*p);
        p+=xSize-6;

	// fifth row of mask
        n+=*(cp-*p++);
        n+=*(cp-*p++);
        n+=*(cp-*p++);
        n+=*(cp-*p++);
        n+=*(cp-*p++);
        n+=*(cp-*p++);
        n+=*(cp-*p);
        p+=xSize-5;

	// sixth row of mask
        n+=*(cp-*p++);
        n+=*(cp-*p++);
        n+=*(cp-*p++);
        n+=*(cp-*p++);
        n+=*(cp-*p);
        p+=xSize-3;

	// seventh row of mask
        n+=*(cp-*p++);
        n+=*(cp-*p++);
        n+=*(cp-*p);

	// the response equals: g-n (if n<g) or 0 (otherwise) (see
	// equation (3)) where g is a geometric threshold to maximize
	// the S/N ratio in average.
        // here: g = maxNo = approx 36*100*0.75
        if (n<=maxNo) {
          response[row*xSize+col] = maxNo - n;
	}
    }

    // we have the "response image"
    // now let's compute the edge directions.

    for (row=4;row<ySize-4;row++)
      for (col=4;col<xSize-4;col++) {

	if (response[row*xSize+col]>0) {

	  m=response[row*xSize+col];
          n=maxNo - m;
          cp=lutBase + in[row*xSize+col];

	  // n > 600 = inter-pixel edge
          if (n>600) {


	    // the position of the centre of gravity is
            // given as:
	    //            sum[r*similarity(r)]
            //   r_cg   = ---------------------
            //             sum[similarity(r)]
            //
            //  with r = pixel position in the mask
            //  (see equation (5))

	    // the vectors r are taken from the centerpixel and counted
            // like this:

	    //              -------------------------> x
            //             |
            //             |          0oo
            //             |         ooooo
            //             |        ooooooo
            //             |        ooo+ooo
            //             |        ooooooo
            //             |         ooooo       0 = first pixel of
            //             |          ooo            mask traversion
            //             |
            //             v
	    //             y

	    // i.e. the upper left most pixel has the spatial
	    // components (x=-1, y = -3) according to equation 5,
	    // these vectors in the numerator are weighted with
            // the corresponding similarity:

	    p=inTemp + (row-3)*xSize + col - 1;
            x=0;y=0;

            c=*(cp-*p++);x-=c;y-=3*c;
            c=*(cp-*p++);y-=3*c;
            c=*(cp-*p);x+=c;y-=3*c;
            p+=xSize-3;

	    c=*(cp-*p++);x-=2*c;y-=2*c;
            c=*(cp-*p++);x-=c;y-=2*c;
            c=*(cp-*p++);y-=2*c;
            c=*(cp-*p++);x+=c;y-=2*c;
            c=*(cp-*p);x+=2*c;y-=2*c;
            p+=xSize-5;

            c=*(cp-*p++);x-=3*c;y-=c;
            c=*(cp-*p++);x-=2*c;y-=c;
            c=*(cp-*p++);x-=c;y-=c;
            c=*(cp-*p++);y-=c;
            c=*(cp-*p++);x+=c;y-=c;
            c=*(cp-*p++);x+=2*c;y-=c;
            c=*(cp-*p);x+=3*c;y-=c;
            p+=xSize-6;
            c=*(cp-*p++);x-=3*c;
            c=*(cp-*p++);x-=2*c;
            c=*(cp-*p);x-=c;
            p+=2;
            c=*(cp-*p++);x+=c;
            c=*(cp-*p++);x+=2*c;
            c=*(cp-*p);x+=3*c;
            p+=xSize-6;

            c=*(cp-*p++);x-=3*c;y+=c;
            c=*(cp-*p++);x-=2*c;y+=c;
            c=*(cp-*p++);x-=c;y+=c;
            c=*(cp-*p++);y+=c;
            c=*(cp-*p++);x+=c;y+=c;
            c=*(cp-*p++);x+=2*c;y+=c;
            c=*(cp-*p);x+=3*c;y+=c;
            p+=xSize-5;

            c=*(cp-*p++);x-=2*c;y+=2*c;
            c=*(cp-*p++);x-=c;y+=2*c;
            c=*(cp-*p++);y+=2*c;
            c=*(cp-*p++);x+=c;y+=2*c;
            c=*(cp-*p);x+=2*c;y+=2*c;
            p+=xSize-3;

            c=*(cp-*p++);x-=c;y+=3*c;
            c=*(cp-*p++);y+=3*c;
            c=*(cp-*p);x+=c;y+=3*c;


	    // we have found the vector of the centre of gravity (x y) now.
            // the direction of the edge is perpendicular to the vector between
            // mask nucleus and centre of gravity:

            z = sqrt((float)((x*x) + (y*y)));

	    if (z > (0.9*(float)n)) { // 0.5

	      do_symmetry=0;
              if (x==0) {
                z=1000000.0;
	      }
	      else {
                z=((float)y) / ((float)x);
	      }

	      if (z < 0) {
	        z=-z;
		w=-1;
	      }
	      else {
		w=1;
	      }

	      if (z < 0.5) { /* vert_edge */
		a=0; b=1;
	      }
	      else {
		if (z > 2.0) { /* hor_edge */
		  a=1; b=0;
		}
	        else { /* diag_edge */
		  if (w>0) {
		    a=1; b=1;
		  }
                  else {
		    a=-1; b=1;
		  }
		}
	      }

	      if (    (m >  response[(row+a)*xSize+col+b])
		   && (m >= response[(row-a)*xSize+col-b])
		   && (m >  response[(row+(2*a))*xSize+col+(2*b)])
		   && (m >= response[(row-(2*a))*xSize+col-(2*b)])) {

	        edgeBuffer[row*xSize+col] = 1; // 1 for an inter-pixel
                                               // edge point

	      }
            }  // (z > 0.9*n)
            else
              do_symmetry=1;
          }
          else // (if n <= 600)
            do_symmetry=1;

          if (do_symmetry==1) {
            p=inTemp + (row-3)*xSize + col - 1;
            x=0; y=0; w=0;

            /*   |      \
                 y  -x-  w
                 |        \   */


	    // This is the intra-pixel case:
	    // according to equations (6), (7) and (8),
            // the ratio of
            //            ________   ________
            //            (y-yo)^2 / (x-xo)^2
	    //
	    // is calculated to determine the orientation of the edge,
            // and the sign of
	    //            ____________
	    //            (x-xo)(y-yo)
	    //
	    // the gradient sign of a diagonal edge


	    // the vetcor components are squared immediatley
            c=*(cp-*p++);x+=c;y+=9*c;w+=3*c;
            c=*(cp-*p++);y+=9*c;
            c=*(cp-*p);x+=c;y+=9*c;w-=3*c;
            p+=xSize-3;

            c=*(cp-*p++);x+=4*c;y+=4*c;w+=4*c;
            c=*(cp-*p++);x+=c;y+=4*c;w+=2*c;
            c=*(cp-*p++);y+=4*c;
            c=*(cp-*p++);x+=c;y+=4*c;w-=2*c;
            c=*(cp-*p);x+=4*c;y+=4*c;w-=4*c;
            p+=xSize-5;

            c=*(cp-*p++);x+=9*c;y+=c;w+=3*c;
            c=*(cp-*p++);x+=4*c;y+=c;w+=2*c;
            c=*(cp-*p++);x+=c;y+=c;w+=c;
            c=*(cp-*p++);y+=c;
            c=*(cp-*p++);x+=c;y+=c;w-=c;
            c=*(cp-*p++);x+=4*c;y+=c;w-=2*c;
            c=*(cp-*p);x+=9*c;y+=c;w-=3*c;
            p+=xSize-6;

            c=*(cp-*p++);x+=9*c;
            c=*(cp-*p++);x+=4*c;
            c=*(cp-*p);x+=c;
            p+=2;
            c=*(cp-*p++);x+=c;
            c=*(cp-*p++);x+=4*c;
            c=*(cp-*p);x+=9*c;
            p+=xSize-6;

            c=*(cp-*p++);x+=9*c;y+=c;w-=3*c;
            c=*(cp-*p++);x+=4*c;y+=c;w-=2*c;
            c=*(cp-*p++);x+=c;y+=c;w-=c;
            c=*(cp-*p++);y+=c;
            c=*(cp-*p++);x+=c;y+=c;w+=c;
            c=*(cp-*p++);x+=4*c;y+=c;w+=2*c;
            c=*(cp-*p);x+=9*c;y+=c;w+=3*c;
            p+=xSize-5;

            c=*(cp-*p++);x+=4*c;y+=4*c;w-=4*c;
            c=*(cp-*p++);x+=c;y+=4*c;w-=2*c;
            c=*(cp-*p++);y+=4*c;
            c=*(cp-*p++);x+=c;y+=4*c;w+=2*c;
            c=*(cp-*p);x+=4*c;y+=4*c;w+=4*c;
            p+=xSize-3;

            c=*(cp-*p++);x+=c;y+=9*c;w-=3*c;
            c=*(cp-*p++);y+=9*c;
            c=*(cp-*p);x+=c;y+=9*c;w+=3*c;

            if (y==0)
                z = 1000000.0;
            else
              z = ((float)x) / ((float)y);

            if (z < 0.5) { /* vertical */
	      a=0; b=1;
	    }
            else {
	      if (z > 2.0) { /* horizontal */
		a=1; b=0;
	      }
	    else { /* diagonal */
	      if (w>0) {
		  a=-1; b=1;
	      }
              else {
		a=1; b=1;
	      }
	    }
	  }
	  if (    (m >  response[(row+a)*xSize+col+b])
	       && (m >= response[(row-a)*xSize+col-b])
	       && (m >  response[(row+(2*a))*xSize+col+(2*b)])
	       && (m >= response[(row-(2*a))*xSize+col-(2*b)])) {

	    edgeBuffer[row*xSize+col] = 2; // 2 for an intra-pixel edge point
	  }
        }
      }
    }
  }

  // thin
  // privides a "thinning" of the raw edge image in the manner that
  // - single edge points without neigbours are completely removed
  // - broken edges are extended
  // - "shifted" points are moved back into line
  void susan::thin(int *response,ubyte* edgeBuffer,int x_size,int y_size){
    int   l[9], centre;
    int   b01, b12, b21, b10,
          p1, p2, p3, p4,
          b00, b02, b20, b22,
          m, n, a, b, x, y, i, j;
    ubyte *mp;

    for (i=4;i<y_size-4;i++)
      for (j=4;j<x_size-4;j++)

	if (edgeBuffer[i*x_size+j]<8) {
	  // 8 is the maxiumum possible signature of a pixel-edge.
	  // all other pixels are initialized to 100 by default

	  // get response
          centre = response[i*x_size+j];

	  // count the number of neihbours for the
	  // pixel

          mp=edgeBuffer + (i-1)*x_size + j-1;

          n = (*mp<8) +
              (*(mp+1)<8) +
              (*(mp+2)<8) +
              (*(mp+x_size)<8) +
              (*(mp+x_size+2)<8) +
              (*(mp+x_size+x_size)<8) +
              (*(mp+x_size+x_size+1)<8) +
              (*(mp+x_size+x_size+2)<8);

	  // if the pixel has no neighbours:
          // it is not an edge point, remove it!

        if (n==0)
          edgeBuffer[i*x_size+j]=100;


        // if n==1 -> extend the line if possible extension is only
        // allowed a few times - the value of edgeBuffer is used to
        // control this

        if ( (n==1) && (edgeBuffer[i*x_size+j]<6) )
        {
          // find maximum neighbour weighted in direction opposite the
          // neighbour already present. e.g.
          //  have: O O O  weight r by 0 2 3
          //        X X O              0 0 4
          //        O O O              0 2 3

          l[0]=response[(i-1)*x_size+j-1];
	  l[1]=response[(i-1)*x_size+j];
	  l[2]=response[(i-1)*x_size+j+1];
          l[3]=response[(i  )*x_size+j-1];
	  l[4]=0;
	  l[5]=response[(i  )*x_size+j+1];
          l[6]=response[(i+1)*x_size+j-1];
	  l[7]=response[(i+1)*x_size+j];
	  l[8]=response[(i+1)*x_size+j+1];

          if (edgeBuffer[(i-1)*x_size+j-1]<8) {
            l[0]=0; l[1]=0; l[3]=0; l[2]*=2;
            l[6]*=2; l[5]*=3; l[7]*=3; l[8]*=4;
          }
          else if (edgeBuffer[(i-1)*x_size+j]<8) {
            l[1]=0; l[0]=0; l[2]=0; l[3]*=2;
            l[5]*=2; l[6]*=3; l[8]*=3; l[7]*=4;
          }
          else if (edgeBuffer[(i-1)*x_size+j+1]<8) {
            l[2]=0; l[1]=0; l[5]=0; l[0]*=2;
            l[8]*=2; l[3]*=3; l[7]*=3; l[6]*=4;
          }
          else if (edgeBuffer[(i)*x_size+j-1]<8)   {
            l[3]=0; l[0]=0; l[6]=0; l[1]*=2;
            l[7]*=2; l[2]*=3; l[8]*=3; l[5]*=4;
          }
          else if (edgeBuffer[(i)*x_size+j+1]<8)   {
            l[5]=0; l[2]=0; l[8]=0; l[1]*=2;
            l[7]*=2; l[0]*=3; l[6]*=3; l[3]*=4; }
          else if (edgeBuffer[(i+1)*x_size+j-1]<8) {
            l[6]=0; l[3]=0; l[7]=0; l[0]*=2;
            l[8]*=2; l[1]*=3; l[5]*=3; l[2]*=4;
          }
          else if (edgeBuffer[(i+1)*x_size+j]<8)   {
            l[7]=0; l[6]=0; l[8]=0; l[3]*=2;
            l[5]*=2; l[0]*=3; l[2]*=3; l[1]*=4;
          }
          else if (edgeBuffer[(i+1)*x_size+j+1]<8) {
            l[8]=0; l[5]=0; l[7]=0; l[6]*=2;
            l[2]*=2; l[1]*=3; l[3]*=3; l[0]*=4;
          }

	  // find the highest point...

          m=0;
          a=0; b=0;
          for(y=0; y<3; y++)
            for(x=0; x<3; x++)
              if (l[y+y+y+x]>m) { m=l[y+y+y+x]; a=y; b=x; }

          if (m>0) {
            if (edgeBuffer[i*x_size+j]<4)
              edgeBuffer[(i+a-1)*x_size+j+b-1] = 4;
            else
              edgeBuffer[(i+a-1)*x_size+j+b-1] = edgeBuffer[i*x_size+j]+1;
            if ( (a+a+b) < 3 ) {// need to jump back in image !
              i+=a-1;
              j+=b-2;
              if (i<4) i=4;
              if (j<4) j=4;
	    }
	  }
        }

        // n==2 neighbours:

        if (n==2) {
          // put in a bit here to straighten edges
          b00 = edgeBuffer[(i-1)*x_size+j-1]<8; // corners of 3x3
          b02 = edgeBuffer[(i-1)*x_size+j+1]<8;
	  b20 = edgeBuffer[(i+1)*x_size+j-1]<8;
          b22 = edgeBuffer[(i+1)*x_size+j+1]<8;
          if ( ((b00+b02+b20+b22)==2) && ((b00|b22)&(b02|b20))) {

	    // case: move a point back into line.
	    // e.g. X O X  CAN  become X X X
	    //      O X O              O O O
	    //      O O O              O O O

            if (b00) {
              if (b02) { x=0; y=-1; }
              else     { x=-1; y=0; }
	    }
            else {
              if (b02) { x=1; y=0; }
              else     { x=0; y=1; }
	    }
            if (((float)response[(i+y)*x_size+j+x]/(float)centre) > 0.7) {
              if (((x==0) && (edgeBuffer[(i+(2*y))*x_size+j]>7)
		          && (edgeBuffer[(i+(2*y))*x_size+j-1]>7)
		          && (edgeBuffer[(i+(2*y))*x_size+j+1]>7) ) ||
                  ((y==0) && (edgeBuffer[(i)*x_size+j+(2*x)]>7)
		          && (edgeBuffer[(i+1)*x_size+j+(2*x)]>7)
		          && (edgeBuffer[(i-1)*x_size+j+(2*x)]>7))) {

		edgeBuffer[(i)*x_size+j]=100;
                edgeBuffer[(i+y)*x_size+j+x]=3;  // no jumping needed
	      }
	    }
	  }
          else {

	    b01 = edgeBuffer[(i-1)*x_size+j  ]<8;
            b12 = edgeBuffer[(i  )*x_size+j+1]<8;
            b21 = edgeBuffer[(i+1)*x_size+j  ]<8;
            b10 = edgeBuffer[(i  )*x_size+j-1]<8;

            if ( ((b01+b12+b21+b10)==2) && ((b10|b12)&(b01|b21)) &&
                 ((b01&((edgeBuffer[(i-2)*x_size+j-1]<8)
		  |(edgeBuffer[(i-2)*x_size+j+1]<8)))
                  |(b10&((edgeBuffer[(i-1)*x_size+j-2]<8)
                  |(edgeBuffer[(i+1)*x_size+j-2]<8)))
		  |(b12&((edgeBuffer[(i-1)*x_size+j+2]<8)
	          |(edgeBuffer[(i+1)*x_size+j+2]<8)))
		  |(b21&((edgeBuffer[(i+2)*x_size+j-1]<8)
	          |(edgeBuffer[(i+2)*x_size+j+1]<8))))) {

	      // case: clears odd right angles.
              // e.g.; O O O  becomes O O O
              //       X X O          X O O
              //       O X O          O X O
              edgeBuffer[(i)*x_size+j]=100;
              i--;
              j-=2;
              if (i<4) i=4;
              if (j<4) j=4;
	    }
	  }
	}


        // for n>2 the thinning is done here without breaking connectivity

        if (n>2) {
          b01 = edgeBuffer[(i-1)*x_size+j  ]<8;
          b12 = edgeBuffer[(i  )*x_size+j+1]<8;
          b21 = edgeBuffer[(i+1)*x_size+j  ]<8;
          b10 = edgeBuffer[(i  )*x_size+j-1]<8;
          if((b01+b12+b21+b10)>1) {
            b00 = edgeBuffer[(i-1)*x_size+j-1]<8;
            b02 = edgeBuffer[(i-1)*x_size+j+1]<8;
	    b20 = edgeBuffer[(i+1)*x_size+j-1]<8;
	    b22 = edgeBuffer[(i+1)*x_size+j+1]<8;
            p1 = b00 | b01;
            p2 = b02 | b12;
            p3 = b22 | b21;
            p4 = b20 | b10;

            if( ((p1 + p2 + p3 + p4) -
                 ((b01 & p2)+(b12 & p3)+(b21 & p4)+(b10 & p1))) < 2) {
              edgeBuffer[(i)*x_size+j]=100;
              i--;
              j-=2;
              if (i<4) i=4;
              if (j<4) j=4;
            }
          }
        }
      }
  }



  // edgeDraw
  // uses the edge image to mark all edge pixels in the original channel (!)
  // i.e. edge pixels are set to 255.
  void susan::edgeDraw(ubyte* image, ubyte* edgeBuffer, int xSize,int ySize){
    const ubyte* src;
    const ubyte* srcEnd;
    //    ubyte *dest;

    // now mark pixel at each edge point
    src=edgeBuffer;
    srcEnd = src+(xSize*ySize);
    //    dest=image;

    for (;src!=srcEnd;++src) {
      if (*src<8) {
        *(image + (src - edgeBuffer)) = 255; // statistically faster ?
        //*dest = 255; // don't forget ++dest
      }
    }
  }

  // binaryDraw
  // draw a binary edge image
  void susan::binaryDraw(channel8& img) {

    ubyte *edgeBuffer = &img.at(0,0);
    ubyte *midp;
    ubyte *midep;

    // now pixel at each edge point
    midp=edgeBuffer;
    midep=edgeBuffer+(img.rows()*img.columns());

    for (;midp!=midep;++midp) {
      if (*midp<8)
        *midp=on;
      else
	*midp=off;
    }
  }

  // binaryDraw
  // draw a binary edge image
  void susan::binaryDraw(const channel8& img,channel& dest) {
    const ubyte *edgeBuffer = &img.at(0,0);

    const ubyte *midp;
    const ubyte *midep;
    const float onValue = float(on)/255.0f;
    const float offValue = float(off)/255.0f;
    float* destp;

    if (dest.getMode() != channel::Connected) {
      dest.resize(0,0);
    }

    dest.resize(img.size(),0,false,false);

    // now pixel at each edge point
    midp=edgeBuffer;
    midep=edgeBuffer+(img.rows()*img.columns());

    for (destp=&dest.at(0,0);midp!=midep;++midp,++destp) {
      if (*midp<8)
        *destp=onValue;
      else
	*destp=offValue;
    }
  }

  void susan::findEdges(const channel8 &img, channel8& edges){

    unsigned char *lutBase = 0;
    int* response = 0;

    // allocate the edge buffer
    // initialize edge buffer with 100 (part of the algorithmn !!!)
    channel8 edgeBuffer(img.size(),ubyte(100));

    // a heuristic value for the maximum USAN value
    // an image point can achieve:
    int max_no_edges = 2650;

    // since 36 neighbour pixels * 100 = 3600
    // and 3600 * 0.736 = 2650 (roughly)
    // where 0.736 is the response expectation value for
    // a USAN with no edge in the presence of gaussian noise (see equation 25)

    // ensure that "edges" has the correct size and is connected:
    if (edges.getMode() != channel8::Connected) {
      edges.resize(0,0,0,false,false);
    }
    edges.copy(img);

    // allocate sequential memory for the response data
    // response is still needed for the thinning after edge finding
    //response   = (int *) malloc(img.columns() * img.rows() * sizeof(int));
    response = new int[img.columns()*img.rows()];

    // set up the brightness Look-up table, 6 is the sigmoid form factor
    setupBrightnessLUT(&lutBase,threshold,6);

    // find raw edges
    susanEdges(&(img.at(0,0)),response,&(edgeBuffer.at(0,0)),
               lutBase,max_no_edges,img.columns(),img.rows());

    // thin the edges using the response image
    thin(response,&(edgeBuffer.at(0,0)),img.columns(),img.rows());

    // mark edge points in original image
    edgeDraw(&(edges.at(0,0)),&(edgeBuffer.at(0,0)),img.columns(),img.rows());

    delete[] response;
    response=0;

    // it is important to move the lutBase pointer back to the
    // start of the allocated memory before deleting !
    // (see setupBrightnessLUT())
    lutBase=lutBase-258;
    delete[] lutBase;
    lutBase = 0;
  }

  void susan::binaryEdges(const channel8 &img, channel8& binEdges){

    ubyte  *lutBase = 0;
    int    *response = 0;

    // a heuristic value for the maximum USAN value
    // an image point can achieve:
    static const int max_no_edges = 2650;
    // since 36 neighbour pixels * 100 = 3600
    // and 3600 * 0.736 = 2650 (roughly)
    // where 0.736 is the response expectation value for
    // a USAN with no edge in the presence of gaussian noise (see equation 25)


    // allocate the edge buffer
    // ensure that binEdges is connected
    if (binEdges.getMode() != channel8::Connected) {
      binEdges.resize(0,0,0,false,false);
    }

    // initialize edge buffer with 100 (part of the algorithmn !!!)
    binEdges.resize(img.size(),100,false,true);

    // allocate sequential memory for the response data
    // response is still needed for the thinning after edge finding
    response   = new int[img.columns() * img.rows()];

    // set up the brightness Look-up table, 6 is the sigmoid form factor
    setupBrightnessLUT(&lutBase,threshold,6);

    // find raw edges
    susanEdges(&(img.at(0,0)),response,&(binEdges.at(0,0)),lutBase,
               max_no_edges,img.columns(),img.rows());

    // thin the edges using the response image
    thin(response,&(binEdges.at(0,0)),img.columns(),img.rows());

    // draw binary edge image
    binaryDraw(binEdges);

    //free(response);
    delete[] response;
    response=0;

    // it is important to move the lutBase pointer back to the
    // start of the allocated memory before deleting !
    // (see setupBrightnessLUT())
    lutBase=lutBase-258;
    delete[] lutBase;
    lutBase=0;

  }

  void susan::binaryEdges(const channel &imgFloat, channel& binEdgesFloat){

    ubyte  *lutBase = 0;
    int    *response = 0;
    // initialize edge buffer with 100 (part of the algorithmn !!!)
    channel8 binEdges(imgFloat.size(),ubyte(100));

    channel8 img;
    img.castFrom(imgFloat);

    // a heuristic value for the maximum USAN value
    // an image point can achieve:
    static const int max_no_edges = 2650;
    // since 36 neighbour pixels * 100 = 3600
    // and 3600 * 0.736 = 2650 (roughly)
    // where 0.736 is the response expectation value for
    // a USAN with no edge in the presence of gaussian noise (see equation 25)

    // allocate sequential memory for the response data
    // response is still needed for the thinning after edge finding
    response   = new int[img.columns() * img.rows()];

    // set up the brightness Look-up table, 6 is the sigmoid form factor
    setupBrightnessLUT(&lutBase,threshold,6);

    // find raw edges
    susanEdges(&(img.at(0,0)),response,&(binEdges.at(0,0)),lutBase,
               max_no_edges,img.columns(),img.rows());

    // thin the edges using the response image
    thin(response,&(binEdges.at(0,0)),img.columns(),img.rows());

    // draw binary edge image
    binaryDraw(binEdges,binEdgesFloat);

    //free(response);
    delete[] response;
    response=0;

    // it is important to move the lutBase pointer back to the
    // start of the allocated memory before deleting !
    // (see setupBrightnessLUT())
    lutBase=lutBase-258;
    delete[] lutBase;
    lutBase=0;
  }


}//namespace lti

