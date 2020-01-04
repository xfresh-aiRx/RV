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

#ifndef _LTI_EXAMPLES
#define _LTI_EXAMPLES

/** 

\page examples Examples

Loading and processing an image in a file 

\code
  // Example: Using the ltilib!

  #include <iostream>
  #include <cstdio>
  #include <list>
  #include <cmath>

  #include "ltiVector.h"
  #include "ltiMatrix.h"
  #include "ltiLogarithm.h"

  #include "ltiTimer.h"

  #include "ltiBMPFunctor.h"
  #include "ltiDownsampling.h"
  #include "ltiConvolution.h"
  #include "ltiGaussKernels.h"

  #include "ltiSplitImg.h"
  #include "ltiMergeChnl.h"
        
  using std::cout;
  using std::endl;

  ...

  void testfunction() {  
\endcode

  ------------------------
  simple matrix operations
  ------------------------

\code
  cout << "Testing matrix operations and constructor...";

  // data to be used by a matrix
  double mdata[15] = { 1,  2,  3,  4,  5,
                       6,  7,  8,  9, 10,
                      11, 12, 13, 14, 15 };

  // create a matrix "a" and copy the data "mdata" on it.
  matrix<double> a(3,5,mdata);

  // show the matrix
  cout << endl << "Matrix a: " << endl << a << endl;
    
  // add -7 to all elements
  a.add(-7.0);

  // apply the c-function fabs to all elements
  a.apply(fabs);

  // show the resulting matrix
  cout << "  After abs(x-7):" << endl;

  cout << a << endl;

  // add 0.5 to all elements
  a.add(0.5);
    
  // create an instance of the logarithm-iterating functor,
  // which apply the log function to all elements of a mathVector
  lti::logarithm<double> lg;
  lg.apply(a);
  
  // show the matrix after applying "log" to all of its elements
  cout << "  After log(x+0.5):" << endl << a  << endl;
\endcode

  ---------------------
  image procesing tests
  ---------------------

\code
  // create an instance of the image loading functor:
  lti::loadBMP loader;
        
  lti::image img,img2;

  // -- loading an image -- : shortcut
  
  // load the given image using the shortcut!
  loader.load("img/leo040801_00_019.bmp",img);

  // -- loading an image -- : the standard way
  
  // the "standard" way uses the parameters-class of the functor
  // the functor requires a parameter object:
  lti::loadBMP::parameters loaderParam;
  
  // give which file should be loaded
  loaderParam.filename = "img/leo040801_00_019.bmp";
  
  // tell the image-loader, which parameters should be used:
  loader.setParameters(loaderParam);
  
  // load the image
  loader.apply(img2);
  
  // -- extract a monochromatic channel of the image --
  
  // split image test
  
  lti::channel chnl,chnl2;
  
  // functor to split image in the chromaticity color space
  lti::splitImageTorgI splitter;
  
  // extract the intensity channel only!
  splitter.getIntensity(img,chnl);
  
  // -- Convolution and Filters --
  
  // 1D filter tests
  
  cout << "Vector filter...";
  
  lti::vector<float> vct1(65536,0.5);
  lti::vector<float> vct2(65536,0.0f);
  
  // create a gaussian kernel with 5 elements and a variance of 0.7
  lti::gaussKernel1D<float> gauss(5,0.7);
  
  // the convolution functor:
  lti::convolution convolver;
  
  // the parameters for the convolution functor
  lti::convolution::parameters param;
  
  // indicate which kernel should be used
  param.setKernel(gauss);
  
  // set the parameters
  convolver.setParameters(param);
  
  // convolve the vector vct1 with the gaussian kernel
  convolver.apply(vct1,vct2);
  
  // -- 2D separable filters --
  
  // create a (separable) column filter!
  
  cout << "Creating separable filter...";
  
  sepKernel<float> sepKern;
  sepKern.setNumberOfPairs(1);
  // in a column filter, the row filter has only 1 element, with value 1
  sepKern.getRowFilter(0).resize(0,0,1,false);
  // use the gaussian kernel as the column filter
  sepKern.getColFilter(0).copy(gauss);
  
  // indicate that now this column filter should be used
  param.setKernel(sepKern);
  convolver.setParameters(param);
  
  // filter the intensity channel with this column filter
  convolver.apply(chnl,chnl2);
  
  // -- 2D gaussian filter (separable)
  
  // change the kernel to a complete 2D gaussian filter
  sepKern.copy(gaussKernel2D<float>(k));
  
  // indicate the convolution functor to use this kernel    
  param.setKernel(sepKern);
  convolver.setParameters(param);
  
  // convolve the channel with the 2D gaussian kernel
  convolver.apply(chnl,chnl2);
  
  // -- 2D gaussian filter (matrix)
  
  // create a 2D (non-separable) kernel
  kernel2D<float> gauss2D;
  gauss2D.outerProduct(sepKern.getRowFilter(0),sepKern.getColFilter(0));
  
  // indicate the convolution functor to use this kernel
  param.setKernel(gauss2D);
  convolver.setParameters(param);
  
  // convolve the 2D kernel with the channel
  convolver.apply(chnl,chnl2);
  
  // -- downsampling test
  
  // create the downsampling functor (using the default parameters)
  lti::downsampling downsampler;
  
  downsampler.apply(chnl,chnl2);
 
  // create a viewer object to see the images
  lti::viewer view;
  
  view.show(chnl2);
  
  getchar(); // wait 'til the user hits a key

  }

\endcode

*/

#endif

