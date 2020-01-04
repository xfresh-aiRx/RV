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
 * file .......: ltiCsPresegmentation.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 8.11.2001
 * revisions ..: $Id: ltiDocuSegmentation.h,v 1.4 2003/07/07 06:57:26 alvarado Exp $
 */

#ifndef _LTI_DOCU_SEGMENTATION
#define _LTI_DOCU_SEGMENTATION

/** 
  \page segmPage Segmentation Overview

  Segmentation is a complex task, and the number of functors in the LTI-lib
  that deal with segmentation grows...  A few extra-notes are required.

  There are two sorts of segmentation functors.  Traditional
  "complete" algorithms and "blocks", with which you can build your
  own segmentation approach.  There are also tools to convert the
  results from one representation to another.

  \section completeSeg Complete Algorithms

  The complete algorithms are:

  - Threshold segmentation.  Used to separate background from object 
    (two-classes segmentation).  You give a "window" for pixel values that
    should be recognized as object.  The rest will be background.  Some
    algorithms can choose automatically the threshold values.
    (see lti::thresholdSegmentation, lti::optimalThresholding)
  - Watershed segmentation.  This sort of algorithms are widely used.  Several 
    implementations can be chosen using the functor parameters
    (see lti::watershedSegmentation)
  - Comaniciu's Mean-Shift algorithm is also implemented.  With it you 
    can choose between quantization, oversegmentation and undersegmentation.
    (see lti::meanShiftSegmentation)
  - Region growing for a two-classes segmentation problem can also be found
    here (see lti::regionGrowing)

  \section blockSeg Functional Blocks
  
  There are several blocks, that can be used as part of a more complex
  segmentation approach.
  
  - A statistical background model per pixel can detect a continuous background
    from a moving object or objects.
    (see lti::backgroundModel)
  - A pre-segmentation approach is implemented, which finds regions
    using color quantization and marks as background the colors most
    used at the border in lti::csPresegmentation.
  - Similar to the pre-segmentation is the lti::kMeansSegmentation, but here
    no background is identified, only the color quantization and (optionally)
    the smoothing stages are done.
  - lti::whiteningSegmentation uses the kMeansSegmentation to segment the
    original image and a transformed one, and combines the results of both.
  - Given a list of area points, where each object (or region) is represented
    by an lti::areaPoints %object, it is possible to generate a so called
    similarity matrix using the lti::similarityMatrix functor.  Here a 
    similarity measure will be generated for all adjacent objects.
  - With a similarity matrix it is possible to merge similar regions using
    the lti::regionMerge %functor.
  
  \section toolsSeg Tools for segmentation

  Segmentation algorithms produce usually so called "masks",
  i.e. channels, where each pixel contains a label value.  These masks
  can be considered for two classes problems, where only values of
  zero and not-zero will be differentiated, and "labeled" masks, where
  each value is considered as a different object.  To extract the
  position of each single object in the image in an efficient way the
  functor \b lti::objectsFromMask should be used.  Sometimes the
  \b lti::fastRelabeling %functor provides all functionality required for
  this task.

  The lti::boundingBox functor can use one of the pointLists extracted with
  lti::objectsFromMask to produce a new image with only one object.  

  It is useful sometimes to generate a color %image, where each label
  found in the %segmentation is replaced by its mean color, producing
  an color quantized %image.  Here, the functors lti::usePalette and
  lti::computePalette are helpful.  
*/

#endif



