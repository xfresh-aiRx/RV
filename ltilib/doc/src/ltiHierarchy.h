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

#ifndef _LTI_HIERARCHY
#define _LTI_HIERARCHY

/** 

\page classHierarchy Conceptual Class Hierarchy in the LTI-Lib

We can group the functionality of the LTI-Lib into several categories:

- \ref datastruc
  - \ref basics 
  - \ref geom   
  - \ref pixel  
  - \ref aggregates
    - \ref matvec
    - \ref chnimg
    - \ref kernels
    - \ref contours
    - \ref othconts
- \ref globalfunc
- \ref functors
  - \ref mathOp
    - \ref linalg
    - \ref stats
    - \ref random
    - \ref sorting
    - \ref interpol
    - \ref mathother
  - \ref imgproc
    - \ref split
    - \ref colquant
    - \ref illum
    - \ref masks
    - \ref edges
    - \ref contproc
    - \ref segm
    - \ref trackers
    - \ref filters
    - \ref morpho
    - \ref enhanc
    - \ref featExt
    - \ref featproc
    - \ref reconstr
  - \ref inout
    - \ref imgfiles
    - \ref descfiles
    - \ref grabbers
    - \ref coding
    - \ref otherio
- \ref classifiers
  - \ref superv
  - \ref unsuperv
  - \ref clastools
- \ref viewers
  - \ref draw
  - \ref view
- \ref system
- \ref genother

<hr> 

\section datastruc Data Structures

The LTI-Lib provides data structures necessary to store different
kinds of data required in computer vision and mathematical
applications.

There are different levels of data structures

- \ref basics 
- \ref geom   
- \ref pixel  
- \ref aggregates

\subsection basics Basic Types

Due to the fact that the C++ standard does not define the exact size
of the internal types, we need to specify some basic types, which a
standardized size, independent of the system or processor where you
compile the library. 

These types are:
- lti::ubyte a one-byte unsigned integer type (values from 0 to 255)
- lti::byte  a one-byte signed integer type (values from -128 to 127)
- lti::uint16 a two-byte unsigned integer type 
- lti::int16  a two-byte signed integer type
- lti::uint32 a four-byte unsigned integer type
- lti::int32  a four-byte signed integer type
- lti::sreal  a single precision floating point type
- lti::dreal  a double precision floating point type 

\subsection geom Geometric Primitives

Typical geometric elements in image processing application are
implemented in the LTI-Lib.  These are
- lti::tpoint     a two-dimensional structure with coordinates x and y. Is 
                  implemented as template, so that the precision of the
                  coordinates can be specified by the user.
- lti::point      alias for lti::tpoint<int>.
- lti::dpoint     alias for lti::tpoint<double>.
- lti::tpoint3D   a three-dimensional structure with coordinates x, y and z.
                  Is implemented as template, so that the precision of the 
                  coordinates can be specified by the user.
- lti::point3D    alias for lti::tpoint3D<int>.
- lti::dpoint3D   alias for lti::tpoint3D<double>.
- lti::trectangle represents a simple rectangle, always vertically or
                  horizontally aligned.  Is also a template class, where
                  the given type is the type for the lti::tpoints used for the 
                  upper-left and bottom-right corners of the rectangle.
- lti::rectangle  alias for lti::trectangle<int>.
- lti::location   used to indicate a place in an image or channel.  It gives
                  the position of a middle point, an orientation and an angle.
- lti::rectLocation gives the position of a rectangular region in an image or
                  channel by the position of a middle point, an orientation
                  angle and two lengths.
- lti::hPoint2D and lti::hPoint3D are template types that define homogeneous
                  points.  These are used in projective geometry tasks.
- lti::chainCode  is an element of a chain code representation.

\subsection pixel Pixel Primitives

With the LTI-Lib you can manipulate \e channels of images extracted
from different color spaces.  The acquisition and display of color
images is however always done in the RGB color space.  Two types are
provided to work in this space:

- lti::rgbPixel  is the basic RGB color type in the LTI-Lib.  It is
                 exactly 4 bytes long, and code the red, green and blue
                 values (1 byte each) together with a dummy (or maybe alpha) 
                 value that "aligns" each pixel to 4 byte structure.
- lti::trgbPixel this is a template type, with the three color components
                 red, green and blue.

More information on extracting different color channels from the
pixels or merging them into a color image can be found in the functors
lti::splitImage and lti::mergeImage.

Many colors are defined as constant instances of lti::rgbPixel.
Always defined are the values lti::Black, lti::White, lti::Red,
lti::Green, lti::Blue, lti::Cyan, lti::Magenta and lti::Yellow.  Many
other definitions can be found in the file "ltiColors.h".

\subsection aggregates Aggregate Types

- \ref matvec
- \ref chnimg
- \ref kernels
- \ref contours
- \ref othconts

\subsubsection matvec Matrices and Vectors

There are many aggregate types in the LTI-Lib, but two of them are
very important:

- lti::vector   is a template class to store and manipulate
                n-dimensional vectors.  Is a template class, so that you
                can specify which type the elements should have.  They
                provide many simple methods for typical vector operations,
                iterators to allow a fast access.  The design of this class
                considered many efficiency aspects and constrains the possible
                types to static ones.  This means, this class is not a
                replacement for std::vector, which is also frequently used
                in the LTI-Lib to contain dynamic types like lti::vectors or
                lti::matrices.  Aliases for different vector types are
                - lti::ivector for lti::vector<int>
                - lti::dvector for lti::vector<double>
                - lti::palette for lti::vector<lti::rgbPixel>
- lti::matrix   is a template class to store and manipulate matrices of
                any size.  Similar to vectors, the allowed types are restricted
                to static ones.  Many useful simple matrix operations are
                provided as methods of the class.  Iterators are also
                provided to allow faster operations.  
                Aliases for different matrix types are:
                - lti::imatrix for lti::matrix<int>
                - lti::dmatrix for lti::matrix<double>

You can cast between matrices (or vectors) of different types using the 
\c castFrom() methods.

Other vector and matrices types exist, to solve specific storage problems:

- lti::array         is a subclass of lti::vector that allows an indication
                     of the range of indices that can be used to access 
                     the elements.  
- lti::dynamicMatrix is used for matrices, where you do not know which
                     indices are going to be used and in which range.  They
                     grow dynamically as you use it.  They are really slow
                     and useful to when building statistics in tabular forms.
- lti::sparseMatrix  is used for huge matrices, where the dimensions are
                     already known, but the most elements contain the same
                     value (usually zero).  It optimizes the amount of memory
                     required, but there are slow.  With help of the iterators
                     you can visit very fast the elements that contain
                     a non-zero value.
- lti::triMatrix small class used to store triangular matrices.
- lti::hMatrix       this class is parent of lti::hMatrix2D and lti::hMatrix3D,
                     which implements homogeneous matrix transformations.

\subsubsection chnimg Channels and Images

The representation of images and channels are classes that inherit
from lti::matrix.  This means, in the LTI-Lib the images and channels
are matrices of special types:

- lti::channel8 is a class that inherits from lti::matrix<ubyte> and implements
                gray-valued images with elements ranging between 0 and 255.
- lti::channel  is a class that inherits from lti::matrix<float> and
                implements gray-valued image with elements usually ranging 
                between 0.0f and 1.0f.  
- lti::image    is a class that inherits from lti::matrix<rgbPixel> and is
                used to represent a color image.

These three classes are widely used in all image processing algorithms
of the LTI-Lib.  For some special cases there is also a
lti::channel32, that inherits from lti::matrix<int>.

You can cast between the different channels (gray valued images) using the 
\c castFrom() methods.

\subsubsection kernels Filter kernels

The filter kernels are small vector or matrix similar types allowing 
an access with integer indices (not only positive values, but also
negative ones).

- lti::kernel1D  one dimensional kernels to filter vector.  You can find
                 specializations for gaussian filtering ( lti::gaussKernel1D ),
                 triangular kernels ( lti::triangularKernel1D ) and several
                 quadrature mirror filters used in wavelet transformations:
                 ( lti::battleLemarieKernel, lti::daubechies16Kernel, 
                   lti::haarKernel and lti::tap9Symmetric )
- lti::kernel2D  two dimensional kernels.  The specialization cover a Gabor
                 filter kernel to extract information with a specific frequency
                 range an orientation ( lti::gaborKernel ) and several kernels
                 used by morphological operators:
                 ( lti::cityBlockKernel, lti::euclideanKernel,
                   lti::octagonalKernel )
- lti::sepKernel separable kernels to filter channels.  In some special cases
                 the 2D kernels can be generated as an outer product of
                 1D ones.  Here is much more efficient to apply each 1D kernel
                 separately.  This kernels store the 1D kernels as row or
                 column filter.  With the method \c separate you can try to
                 separate a 2D kernel into its 1D components.                
                 As specializations you will find the gaussian kernel
                 ( lti::gaussKernel2D ), a separable Gabor kernel
                 ( lti::gaborKernelSep ), kernels to approximate the gradient
                 of a channel ( lti::gradientKernelX and 
                 lti::gradientKernelY ), kernels for the oriented gaussian
                 derivatives ( lti::ogd1Kernel and lti::ogd2Kernel ) and a
                 simple square formed kernel ( lti::chessBoardKernel ) used 
                 in some morphological operators.


\subsubsection contours Contours, Polygons and Regions

The image representation in form of matrices is not appropriate for
all possible applications.  Representations for contours and regions are also
necessary.  The types used in the LTI-Lib are:

- lti::tpointList   a simple list of lti::tpoints, without any specific
                    semantical meaning.  An important alias for list of
                    integer points is lti::pointList.

Four classes inherit from the list of integer points:

- lti::borderPoints is a list of adjacent points representing a contour
                    within an image.  All algorithms ensure that two adjacent
                    points in the list are neighbor points in the image.
- lti::ioPoints     the input/output point representation describes also
                    a region indicating alternatively when a "beam" that
                    sweeps the images from top to bottom, from left to right
                    gets IN the region and when gets OUT.
- lti::areaPoints   is a list of all points enclosed in an region (border
                    inclusive).
- lti::polygonPoints are the representation of a polygon by its vertices.
                    It contains methods to approximate an arbitrary contour
                    (represented by a lti::borderPoints object) by a polygon.
                    You also can get the convex hull of a list of
                    points (lti::pointList) using the respective methods.
           
You can cast between the last classes using the respective \c castFrom()
methods.

Another important and more complex representations for shapes and
their variations are the point distribution models
( lti::pointDistributionModel ).  They can be constructed using 
lti::pdmGenerator.

\subsubsection othconts Other aggregate types

Other aggregate types are:

- lti::tree       represents rooted ordered trees.
- lti::sequence   are used to represent sequence of objects (like images).
- lti::thistogram is the parent class for several representations of
                  multidimensional histograms.
                  The most important ones are the specialization 
                  for one and two dimensional histograms ( lti::histogram1D
                  and lti::histogram2D ).  You can also use lti::mapperFunctor
                  instances to map any value ranges into the integer ranges
                  accepted by the histograms (from 0 to mi-1, with mi the 
                  number of elements of the i-th dimension).
- lti::sparseHistograms allow the representation of multidimensional histograms
                  where it is expected, that the number of occupied cells is
                  relatively small compared with the total number of cells of
                  the histogram.  The number of bins per dimension is limited
                  to a maximal of 64.
- lti::tensor     template class to create multidimensional tables.
- lti::pyramid    A hierarchical set of images or channels.  Typical cases
                  for multiresolutional analysis in image processing are
                  implemented in the LTI-Lib:
                  lti::gaussianPyramid, lti::laplacianPyramid and 
                  lti::gaborPyramid.

<hr>

\section globalfunc Global Functions and Constants

There are several global functions necessary to extend the old C-typed math
functions.  Many of them are defined for several types, so that you do not
need to explicitely cast.  The most usual ones are:
- lti::min and lti::max with two and three arguments
- lti::round, lti::iround and lti::lround to round a floating point value.
  The iround returns and integer and the lround a long integer.  
- Three typical tasks related with the absolute value are lti::abs
  absolute value, lti::absdiff the absolute value of the difference
  between two values and lti::rectify, which returns zero for all negative
  values.
- lti::signum and lti::signum0 return the sign of a number
- lti::sqr returns the square of a number and lti::sqrt the square root.
- lti::sin, lti::cos, lti::tanh and atan2 for float and double types.
- A special case for atan2 for integer arguments using a Look-Up Table 
  accelerates the computation of many algorithms.  Take a look at
  lti::arctanLUT
- lti::log, lti::exp for float and double types
- lti::pow for float, double and integer types
- lti::sigmoid the simple sigmoid function 1/(1+exp(-x)) for float, double and
  integer types.

Several geometry related global functions compute among other things the
 intersection between lines and the minimal distances between points and lines:
- lti::minDistance2()
- lti::intersection
- lti::clockwiseTurn

Usual constants are 

- lti::Pi a double constant for 3.1415926535897932 
- lti::NaN for Not a Number
- lti::Inf for Infinity

Also global are the functions to read and write into lti::ioHandler objects.
These functions are lti::read and lti::write

The global function lti::passiveWait is a wrapper function for the system
dependent functions usleep and Sleep.

<hr>

\section functors Functor

The most algorithms to process and analyze images or matrices are implemented 
as functors (see also \ref arcFunctor).

We have following functor categories:

- \ref mathOp
  - \ref linalg
  - \ref stats
  - \ref random
  - \ref sorting
  - \ref interpol
  - \ref mathother
- \ref imgproc
  - \ref split
  - \ref colquant
  - \ref illum
  - \ref subpix
  - \ref masks
  - \ref edges
  - \ref contproc
  - \ref segm
  - \ref trackers
  - \ref filters
  - \ref morpho
  - \ref enhanc
  - \ref featExt
  - \ref featproc
  - \ref reconstr
- \ref inout
  - \ref imgfiles
  - \ref descfiles
  - \ref grabbers
  - \ref coding
  - \ref otherio

\subsection mathOp  Mathematical Operations

Following groups of mathematical functors exist in the LTI-Lib:

- \ref linalg
- \ref stats
- \ref random
- \ref sorting
- \ref interpol
- \ref mathother

\subsubsection linalg Linear Algebra

- The parent class to functor that solve systems of linear equations
  is lti::linearEquationSystemSolutionMethod.  Two decomposition
  methods exist at this time: lti::luSolution and lti::qrSolution
- Usefull while solving linear equation systems are the functors 
  lti::backSubstitution and lti::forwardSubstitution.
- The lti::luDecomposition computes the LU decomposition of a matrix.
- The Singular Value Decomposition, useful to solve under-determined or
  over-determined linear equations systems is implemented in 
  lti::singularValueDecomp
- To find the inverse of a matrix use lti::matrixInversion.  Remember
  that if you want to solve a system of linear equations (something
  with the form Ax=b with a matrix A and two vectors x and b), the
  lti::linearEquationSystemSolutionMethod functors provide more efficient 
  and stable ways to find the values of x, without inverting the matrix A. 
- The Moore-Penrose pseudo inverse can be computed by the lti::pseudoInverseMP 
  functor.
- lti::minimizeBasis generates a minimum number of basis vectors to
  approximate another set of vectors within a given deviation.
- To extract the eigenvalues of a matrix you can use any class that
  inherits from lti::eigenSystem.  At this time only the lti::jacobi functor
  is implemented for this task.  If you have the CLAPACK installed, you can
  configure the LTI-Lib to also provide other functors present in 
  <code>ltilib/misc/lamath</code>.
- lti::distanceFunctor is the parent class for all functors that
  compute distances between vectors and matrices
  (e.g. lti::l1Distance, lti::l2Distance).  
- A related class to the previous distance functors are the
  lti::similarityFunctor classes,  that currently offer
  lti::cosinus2Similarity and lti::euclidianSimilarity.
- To do a principal components analysis you can use the
  lti::principalComponents functor or the more sophisticated lti::kernelPCA.
  The lti::serialPCA computes sequentially the principal components of an
  always increasing number of data.
- A linear combination of different vectors and matrices can be done with
  the lti::linearMixer functor.
- To multiply huge matrices you can use the lti::strassenMultiplication.
- To access n-dimensional containers like histograms or sparseHistograms you
  require ivectors as indices.  Is usually helpful the clip the values in these
  vectors to be in a valid hyperbox.  For this task you can use the
  lti::boundsFunctor or one of the from lti::mapperFunctor inherited classes.

Many other basic linear algebra functions are directly implemented in
the matrix classes.  You can multiply, add, transpose, etc. vector and
matrices.  The dot product between two vectors of the same type is also build
in.  If you need the dot product of two vectors of different types you can
use the lti::scalarProduct functor.

If you have compiled the LTI-lib with CLAPACK support enabled, there are
some more linear algebra functors available:

- lti::generalEigenVectors to extract the eigenvalues and eigenvectors of
  a matrix.
- lti::linearDiscriminantAnalysis to search in a set of vectors the axes
  that can best discriminate between them (LDA).

\subsubsection stats Statistics

- lti::boundsFunctor is used to find the minimum or maximum row or column
  vectors of a matrix.
- lti::quickMedian search using a "partial" quick-sort algorithm the
  median of a vector o matrix.
- lti::varianceFunctor and lti::meansFunctor compute simple statistics for
  the rows and columns of matrices or for vectors.
- lti::serialStatsFunctor is a very useful small functor, that helps computing
  the variance and mean values of data stream, when you do not know exactly
  how many (one-dimensional) samples you will have.  
  The lti::serialStatsExtFunctor is very similar to the previous one, but
  a tiny little bit slower due to the additional computation of the minimum and
  maximum values of the data.
- lti::serialVectorStats is similar to the previous functor but for 
  n-dimensional samples.
- lti::entropyFunctor  assumes that the rows (or columns) of a matrix contain
  probability distributions, i.e. the sum of the rows (column) elements must
  be 1.0. The entropy for the row will be defined as the sum of p(x)*ln(p(x))
  for all x, where p(x) are the elements of the vector, row or column. 
- lti::chiSquareFunctor calculates the chiSquareValue indicating the 
  likelihood, that the considered elements are drawn from a gaussian normal 
  distribution.
- lti::bhattacharyyaDistance and lti::bhattacharyyaDistOfSubset compute the
  well known comparison method for normal distributions.

\subsubsection random Random numbers
 
There are several random number generators that follow different discrete or
continuous probability distributions:

Discrete random distributions:

- lti::binomialDistribution

Continuous random distributions:

- lti::gaussianDistribution     one dimensional gaussian distribution
- lti::multivariateGaussian     n-dimensional gaussian distribution
- lti::exponentialDistribution  one dimensional gaussian distribution
- lti::gammaDistribution        one dimensional gamma distribution
- lti::poissonDistribution      one dimensional Poisson distribution
- lti::uniformDistribution      one dimensional uniform distribution


With lti::noise you can add noise with a given distribution to
matrices or vectors

\subsubsection sorting Sorting

- lti::sort to sort the elements in vectors and matrices
- lti::sort2 to sort a vector/matrix using another vector/matrix as key
- lti::scramble to shuffle the elements in vectors/matrices
- lti::quickMedian search using a "partial" quick-sort algorithm the
  median of a vector o matrix.
- lti::sortExpensive sorts the elements in a std::list that are
  computationally expensive to compare.

\subsubsection interpol Interpolation

The base class of all interpolators is lti::interpolator.

Two basic classes of interpolation are distinguished for equal and 
variable distances between tabulated points, respectively:
- lti::equallySpacedSamplesInterpolator 
- lti::variablySpacedSamplesInterpolator

These have the following subclasses:
- lti::scalarValuedInterpolation uses equally spaced samples and returns
  a scalar given a set of points with 1D or 2D coordinates. It is usually
  used for interpolating pixel values in images. Its subclasses are:
  - lti::nearestNeighborInterpolator
  - lti::bilinearInterpolator 
  - lti::bicubicInterpolator

- lti::cubicSpline variable sample distance cubic spline interpolation
- lti::multidimensionalCubicSpline as cubicSpline but the curve is in 
  n-dimensional space and thus returns a vector

\subsubsection mathother Other Mathematical Functors

- lti::validator checks if a matrix of floating points types contains
  invalid numbers (i.e. lti::NaN or lti::Inf ).
- lti::sammonsMapping maps data from a high dimensional to a lower
  dimensional space while trying to preserve the inter-point
  distances.

\subsection imgproc Image Processing

There are many different algorithms for image processing in the LTI-Lib:
- \ref split
- \ref colquant
- \ref illum
- \ref subpix
- \ref masks
- \ref edges
- \ref contproc
- \ref segm
- \ref trackers
- \ref filters
- \ref morpho
- \ref enhanc
- \ref featExt
- \ref featproc
- \ref reconstr

\subsubsection split  Color Spaces

In the LTI-Lib there are only RGB images, but you can split it into
the channels of different color spaces.  You can also take three channels
in a specific color space and merge them into a lti::image.

The classes to split a color image inherit from lti::splitImage and to merge
channels you can use functors derived from lti::mergeImage.  You can
create your own linear color spaces transformations with lti::linearMixer.

The color spaces currently supported are:

- RGB (Red, Green and Blue)        lti::splitImageToRGB, lti::mergeRGBToImage
- HSV (Hue, Saturation, Value)     lti::splitImageToHSV, lti::mergeHSVToImage
- HSI (Hue, Saturation, Intensity) lti::splitImageToHSI, lti::mergeHSIToImage
- HLS (Hue, Luminance, Saturation) lti::splitImageToHLS, lti::mergeHLSToImage
- rgI (chromaticity,  Intensity)   lti::splitImageTorgI, lti::mergergIToImage
- XYZ (CIE XYZ color space)        lti::splitImageToXYZ, lti::mergeXYZToImage
- xyY (Chromaticity CIE  space)    lti::splitImageToxyY, lti::mergexyYToImage
- YIQ (Luminance,Inphase,Quadrature) lti::splitImageToYIQ, lti::mergeYIQToImage
- OCP (Opponent Color Channels)    lti::splitImageToOCP, lti::mergeOCPToImage
- CIE Luv (L* u* v* channels) lti::splitImageToCIELuv, lti::mergeCIELuvToImage

Other functors for color analysis:

- lti::opponentColor generates an "opponent color" channel from the given two
  channels, one representing the center, the other representing the surround.

\subsubsection colquant Color Quantization and Related Topics

An usual condition to many algorithms is to reduce the number of colors used
in the color images.

- lti::kMColorQuantization uses the well known k-Means clustering algorithm
  to find the best k colors.
- lti::lkmColorQuantization or local k-Means quantization use a SOM (Self 
  Organizing Map)-like approach to fine the k best colors.
- lti::medianCut classical median cut algorithm for color quantization.
- lti::meanShiftSegmentation has a color quantization modus that considers not
  only the color distribution in the color space, but their locations in the
  image.

This other functors are also useful:

- lti::computePalette computes some statistics for the colors used in an image 
  using the indices (or labels) mask generated by a color quantization or
  segmentation functor.
- lti::usePalette is used to replace the values of in an index image 
  (usually a channel8) with the values given in an specified palette.

\subsubsection illum Color and Intensity Analysis Techniques

To eliminate the influences of the illumination you can use functors derived 
from the class lti::colorNormalizationBase:
- lti::grayWorldNormalization is a simple robust method that computes second
  order statistics of each color channel (RGB) to produce canonical images
  that are independent of the illumination color.
- lti::comprehensiveColourNormalization is a method proposed by Finlayson, 
  Schiele and Crowley to elliminate also dependencies on the geometry of the
  illuminants.
- You can use lti::whiteningSegmentation to do a color zooming of an image, 
  which provides also sort of illumination invariancy.

You can create \b statistical \b color \b models for one or more images in form
of 3D histograms with lti::colorModelEstimator.  You can employ these models to
decide if a pixel belongs to the class it describes ( lti::probabilityMap ).
The functor lti::colorModelSelector decides using a Maximum Likelihood approach
which model in a set describes more appropriately the colors found in an
image region.

Other functors for color analysis:

- lti::opponentColor generates an "opponent color" channel from the given two
  channels, one representing the center, the other representing the surround.

\subsubsection subpix Subpixel interpolation

You can interpolate the values between pixels or elements in a vector using
lti::scalarValuedInterpolation instances:

- lti::nearestNeighborInterpolator does not really interpolate, it just get
  the next point available.
- lti::bilinearInterpolator is the simplest and fastest method.  For vectors
  it does a linear interpolation.
- lti::bicubicInterpolator is a more precise but slower interpolator.

These classes are derived from lti::interpolator. See \ref interpol

\subsubsection masks Mask Operations

Typical boolean and arithmetical operations to manipulate binary masks
(images with only two values) can be found in the classes derived from
lti::maskFunctor.  The usual ones are:

- lti::maskNot          boolean NOT operator
- lti::maskAnd          boolean AND operator
- lti::maskOr           boolean OR operator
- lti::maskInvert       sort of boolean NOT operator: (1-x) for x in [0,1]
- lti::maskAlgebraicSum usual OR-equivalent function used in fuzzy systems.
- lti::maskMultiply     usual AND-equivalent function used in fuzzy systems

Other usual operations with this kind of masks are \ref morpho.

\subsubsection edges Edge and Corner Detectors

The edge detectors in the LTI-Lib inherit from lti::edgeDetector.  You can use
the lti::edgeDetectorFactory to create instances of all edge detectors existent
in the LTI-Lib:

- lti::susanEdges is NOT part of the LTI-Lib, but you can use it if you 
  accept the original conditions of use of the SUSAN algorithm.
- lti::cannyEdges is the standard edge detection algorithm.


All corner detectors inherit from lti::cornerDetector.  You can use the
lti::cornerDetectorFactory to create instances of all corner detectors existent
in the LTI-Lib:

- lti::susanCorners is NOT part of the LTI-Lib, but you can use it if you
  accept the original conditions of use of the SUSAN algorithm.
- lti::harrisCorners a standard corner detector.


\subsubsection contproc Points, Contours and Shape Manipulation

Some basic geometrical operations are provided by the types described in 
\ref contours.  For example, you can get the convex hull of a set of points,
approximate a contour with a polygon or get the boundary of an area or
the area points enclosed by a contour using the casting methods of the
from lti::tpointList<T> inherited classes.

- lti::borderExtrema extracts minimum and maximum positions along a border in
  terms of distance to a given center point.
- lti::cubicSpline interpolates between the points in a list using cubic
  splines.
- lti::boundingBox extracts part of an image under consideration of
  a contour representation of the object of interest.  It suppresses the
  irrelevant background.
- lti::polygonApproximation approximates a contour represented by a
  lti::borderPoints object with a polygon (see also lti::tpolygonPoints).
- lti::convexHull computes the convex hull of a set of points. (see also
  lti::tpolygonPoints).

\subsubsection segm Segmentation and Localization 

Segmentation algorithms found in the LTI-Lib are (see also \ref segmPage):

- lti::thresholdSegmentation is the simplest algorithm.
- lti::watershedSegmentation is a widespread method to partition an image
  into small regions.
- lti::snake is a very primitive (but fast) version of an active contour
  based on a region growing approach.
- lti::regionGrowing a simple segmentation approach that expands some
  regions starting at some given seeds.
- lti::meanShiftSegmentation is based on the work of Comaniciu et.al.  
  Produces very good results in the most cases.  You can configure it
  to quantize, or to produce over or under-segmentations of your images.
- lti::kMeansSegmentation is based on a combination of color quantization and
  edge preserving filtering.
- lti::whiteningSegmentation tries to augment a color region in the color
  space to separate objects which are similar in their color.
- lti::csPresegmentation is a simple functor that suppresses a homogeneous
  background from other objects.  It works under the assumption that the
  borders of the image contain mainly background pixels.

Other tools related with segmentation tasks are:

- lti::regionMerge uses a similarity matrix (see lti::similarityMatrix) and
  a threshold value given in the parameters to decide if two objects in a
  mask (also returned by lti::similarityMatrix or by lti::objectsFromMask)
  should be merged or not.
- lti::boundingBox extracts part of an image under consideration of
  a contour representation of the object of interest.  It suppresses the
  irrelevant background.
- Classes for region/object detection: lti::objectsFromMask and 
  lti::fastRelabeling.

The functor \b lti::objectsFromMask deserves special attention.  Usually the 
segmentation algorithms produce a the end a "labeled" mask, that contains
an assignment for each pixel in an image to a specific object.  This functor
allows you to extract from this mask all found objects in a very efficient
way.  If this functor is too slow for your needs, and you do not require so
much information as it provides, you can also try \b lti::fastRelabeling.  It 
cannot detect which regions are within others, but this kind of information is 
not always required.

Algorithms used in the localization of specific image regions:

- lti::activeShapeModel is the base class for functors that work on point
  distribution models ( lti::pointDistributionModel ).  It forces them 
  to fit different shapes in an image.  (See lti::gradientASM and lti::skinASM)
- lti::compaqObjectFinderTrainer, lti::compaqObjectFinderModel and 
  lti::compaqObjectFinder are based on the work of Viola and Jones.  They allow
  among other things to find faces in images.
- lti::blobEM estimates the position of M overlapping blobs by
  applying the EM-algorithm and estimating the parameters of a
  gaussian mixture model that fits the blobs.
- You can of course use lti::correlation in image localization tasks.
- Assuming you have image sequences with a relative stable background, you
  can use the lti::backgroundModel to detect which objects move.

- lti::axLocalRegions detects relevant small regions in an image, that
  can be used to extract local descriptors.  
- lti::locationSelector splits a list of locations into several smaller lists
  depending on the values of a given decision mask.

Saliency functors are used to detect parts of an image that are perceptually
interesting:

- lti::edgeSaliency implements an older algorithm of Shashua and Ullman that 
  extracts salient information out of edge images.
- lti::featureSaliencyIK is inspired in an algorithm of Itti and Koch.  It
  detects relevant regions in color images.
- lti::featureSaliencyAx similar to the previous functor, but use other
  somehow equivalent tools to speed up the generation of the saliency map.


\subsubsection trackers Tracking and Optical Flow

Several mechanisms to track objects in images are provided in the LTI-Lib:

- lti::kalmanFilter and lti::kalmanTracker use the Kalman Filter prediction
  mechanisms to track parts of images or just points.
- lti::lkTracker is a pyramidal implementation of the Lukas-Kanade point
  tracker.
- lti::camshiftTracker tracks a rectangular search window in a channel (gray
  valued).
- lti::meanshiftTracker tracks a rectangular search window (the target) in an
  image by its color distribution

Optical flow functors:

- lti::opticalFlowHS implements the Horn-Schunks gradient based method.
- lti::opticalFlowLK implements the Lucas-Kanade gradient based method.
- lti::temporalTemplate is not exactly an optical flow functor, but something
  similar.  It extracts motion history images from a sequence of channels.

\subsubsection filters Filters and Transformations

Classical filter operators are:

- lti::convolution is the classical filter operation.  It convolves a given
  kernel with a given channel.  Depending on the kernel type you use,
  you can do almost everything (see also \ref kernels).
- lti::squareFilter is an optimized version of the convolution with a
  rectangular kernel.
- Filtering an image with the oriented gaussian derivatives (OGD) can be
  efficiently achieved using the lti::ogdFilter functor.
- lti::correlation is used to correlate small regions in bigger images.  In 
  its classical form it is close related to the convolution, but this functor
  offers other operation modes.
- Sampling down an image can be done using a lti::downsampling functor, which 
  convolves a kernel (in a very efficient way) with an image before it extracts
  the desired pixel subset.   If you need only some pixels in a regular grid of
  an image (ignoring the Nyquist theorem) you can use lti::decimation.
- Sampling up an image can be done using the lti::upsampling.  If you want to
  use "squares" for each upsampled pixel instead of filtering the image
  appropriately, you can do it efficiently with the lti::filledUpsampling 
  functor.

Image Transformations:

- lti::realFFT computes the Fourier transformation of a channel with real
  values.  The inverse transformation is done by lti::realInvFFT.
- lti::qmf used with the appropriate 1D kernels produces the wavelet transform 
  of images or vectors.  With lti::qmfInverse you can reconstruct the original
  data.
- Some times, the value of each pixel in an channel represents one coordinate
  value in some given coordinate system.  Usual conversions between polar
  and Cartesian coordinates can be done with lti::cartesianToPolar and
  lti::polarToCartesian.
- lti::geometricTransform allows flexible rotation, shift and scaling 
  transformation of two dimensional images.
- Two variants of the hough transform are provided: a very fast implementation
  of the line detection algorithm in lti::orientedHLTransform and a much slower
  general form detection algorithm in lti::gHoughTransform.
- lti::orientationMap extracts the orientation of each pixel in an image
  and a relevance or "degree of truth" for each pixel based on gradient
  information, or optionally on the OGD filtering of the image.


Two edge preserving filters are implemented:

- lti::medianFilter classical median filter with an efficient histogram-based
  implementation for lti::channel8 objects.
- lti::kNearestNeighFilter is based on the statistical k nearest neighbors 
  classification approach.  Each pixel is assigned the most frequent pixel
  in its neighborhood.

\subsubsection morpho Morphological Operators

Some classical and some unconventional morphological operations are
already implemented:

- lti::dilation and lti::erosion implement the basic morphological operators
- lti::distanceTransform computes in a binary mask the shortest distance from
  a region pixel to a background pixel.
- lti::skeleton constructs an homotopy preserving skeleton.
- lti::maximumFilter assigns to each pixel the maximum value found in a given
  neighborhood.  A related functor extracts from an image all found 
  local maxima ( lti::localMaxima ).

\subsubsection enhanc  Image Enhancement

- lti::histogramEqualization is classical intensity histogram equalization 
  method to improve the contrast in an image.
- lti::susanDenoise is NOT part of the LTI-Lib, but you can use it if you 
  accept the original conditions of use of the SUSAN algorithm.  It uses
  the SUSAN principles to remove noise in a gray-valued image.

\subsubsection featExt Feature Extraction

With feature extraction functors are meant objects that extract some
descriptors from images or shapes.  

Color feature extractors:

- lti::brightRGB very simple color feature
- lti::channelStatistics simple statistics (like min, max, average,
  etc.) of the channels of a color image in an arbitrary color space.
- lti::chromaticityHistogram a classical illumination invariant color feature.
- lti::histogramming1D is a very simple functor that constructs a 1D histogram
  of gray-valued channel.
- lti::histogramRGBL extracts four 1D histograms for the color channels R, G, B
  and the luminance channel L, and concatenates them.

Texture feature extractors:

- lti::axOGDFeature is based on steerable filters (OGD).
- lti::qmfEnergy computes the energy in each band of a wavelet decomposition
  of an image.

Shape feature extractors:

- lti::fourierDescritor is a classical shape descriptor.  It works with border
  point representation of the contour.
- lti::curvatureFeature is a simple rotation invariant shape feature.  It
  works on gray valued images.
- lti::orientationFeature is a weighted histogram of the orientation of
  the pixels in a gray valued image.
- lti::regionShapeFeatures compute the coefficients of a basis
  function set for a given binary mask.  One of the modes used comes
  from the MPEG-7 standard.
- lti::geometricFeatures computes many shape statistics, including the
  classical Hu moments.  It works on contours.
- lti::huMoments computes the Hu Moments for regions of gray valued images 
  instead of contours.
- lti::borderSignature extracts some features for a boundary points
  representation of a contours, given a reference point.
- lti::curvatureScaleSpace extracts the CSS representation of a contour.  It
  can use this representation to generate a few shape feature vectors.

Most functors listed above compute a descriptor for the whole image.  This 
kind of descriptors are usually known as "global descriptors".  
There are other kinds of them to describe only small regions of an image.
To extract these "local features" you usually need to compute first interesting
locations.  You can achieve this with lti::axLocalRegions
(see also lti::locationSelector).  Following functors can be used to extract
the local features:
- lti::localColorFeature compute the mean color for a given number of slices
  of all locations given in a list.
- lti::axOGDFeature is based on steerable filters (OGD).
- lti::schieleCrowley6DFeature is used to extract local descriptors.
 
\subsubsection featproc Feature Processing

- lti::shiftInvariace computes a shift-normalized vector.
- lti::principalComponents Principal Components Analysis extracts from
  a set of feature vector the principal components, where the higher
  variances are found.
- lti::kernelPCA does a PCA analysis in a higher dimensional space, where
  the mapping of the points to the higher dimensional space is done using
  a lti::kernelFunctor.
- lti::serialPCA computes sequentially the principal components of continously
  arriving data.

\subsubsection reconstr 3D Reconstruction

- lti::fundamentalMatrixSolverLMS computes the fundamental matrix given a
  few pairs of corresponding points in two images taken at different
  perspectives.
- lti::frankotChellapa tries to extract a depth image from an intensity image.
- lti::sfsBichselPentland is also a shape from shading algorithm.

\subsection inout Input and Output

- \ref imgfiles
- \ref descfiles
- \ref grabbers
- \ref coding
- \ref otherio

\subsubsection imgfiles Reading and Writing Image Files

- lti::loadImage, lti::saveImage are the functors you will usually use.  They
  understand PNG, JPEG and BMP file formats. 
- lti::loadImageList can be used if you want to read several images files.
  The filenames of the images can be specified in a text file, a list of 
  filenames or all images in a directory. The images are loaded sequentially.

If you need for some reason to limit the file format you want to read or
write to just one, you can use:

- lti::loadBMP, lti::saveBMP for the Windows Bitmap Format BMP
- lti::loadPNG, lti::savePNG for the Portable Network Graphics format PNG
- lti::loadJPEG, lti::saveJPEG for the JPEG format.

For the last two file formats there are two implementations that can
be used with the LTI-Lib.  We recommend the use of the JPEG-Lib and
PNG-Lib implementations, which are part of the LTI-Lib.  The required
libraries are usually installed in all Linux distributions.  It is
faster, more robust and stable against incorrect files, and 
they are open source too.

The second implementation is NOT part of the LTI-Lib due to License problems.
It is based on Mianos' code (Colosseum Builders C++ Image Library) and used
per default in the Windows version of the LTI-Lib.  You can get them as extra
functors from the download pages and you can use them only if you
agree with their conditions of use.

\subsubsection descfiles Reading and Writing Features

Some tests require to save and load files containing tons of feature
vectors.  This way you can separate the process of feature extraction
from the training and test.  At this time following functors are provided:
- lti::loadLnc and lti::saveLnc is a very primitive but easy to use file format
  that stores the feature vectors together with some elementary information.
- lti::uciDataReader and lti::uciDataWriter read and write data using the
  format of the UCI standard data sets.

\subsubsection grabbers Getting Images from Frame Grabbers

A few functors to get images from frame grabbers are already implemented
in the LTI-Lib.  They all inherit from lti::frameGrabber.

- lti::quickCam (for Linux only)
- lti::toUCam Philips WebCam (for Linux only)
- lti::itiITIFrameGrabber (for Windows only)
- lti::microEnablePulnix with Pulnix TMC6700 (for Windows and Linux)

Remember to "activate" your frame grabbers in the
<code>ltilib/src/io/ltiHardwareConfig.h</code> file or to define the
appropriate preprocessor macros while compiling!

\subsubsection coding Coding
Coding and Decoding functors between different formats inherit from 
lti::dataTransformer, (for example lti::asciiHexCodec or lti::runLengthCodec).

\subsubsection otherio Other Input and Output Objects

- lti::configFileHandler reads and writes files using the syntax common
  in the Windows and KDE configuration files.
- lti::url allows you to easily retrieve information from the Internet
  just specifying the URL where your data is located.
- lti::serial allows import and export of data through the serial port.

If you need to read/write Standard Template Library (STL) containers
include the header "ltiSTLIoInterface.h". It includes read and write
methods for the following data structures:
- std::list<T>
- std::vector<T>
- std::map<T,U>

These methods only work if the types T,U implement the operator=().

<hr>

\section classifiers Classifiers

Classifiers are used in pattern recognition applications.  They can be usually
be trained with some training data, and later they can be used to test some
learned properties, for example, to which class could belong a given point.
A detailed description for these objects can be found in \ref docuClassifiers.

- \ref superv
- \ref unsuperv
- \ref clastools

\subsection superv Supervised Classifiers

- lti::decisionTree 
- lti::MLP Multi-Layer Perceptrons
- lti::rbf Radial-Basis-Function Networks
- lti::shClassifier is suggested by Schiele and Crowley to classify a huge
  number of local descriptors.
- Support Vector Machines lti::svm require also kernel functors
  lti::kernelFunctor to define its operation mode.
- Hidden Markov Models are implemented in lti::hiddenMarkovModel.  These can
  be trained with lti::hmmClassifier, which uses lti::hmmTrainer instances.

\subsection unsuperv Unsupervised Classifiers

- lti::SOFM Self-Organizing Feature Maps
- There are several clustering algorithms implemented, like 
  lti::adaptiveKMeans, lti::fuzzyCMeans and lti::kMeansClustering.


\subsection clastools Classification Tools


- lti::combination allows to combine different classification results.
- lti::sammonsMapping maps data from a high dimensional to a lower
  dimensional space while trying to preserve the inter-point
  distances.
- lti::classificationStatistics produces several statistics for a
  classification test, including confusion matrices, n-th best recognition,
  recognition rates and so on.
- lti::progressInfo is used to report the user the progress of a long
  computation.

<hr>

\section viewers Visualization and Drawing Tools

- \ref draw
- \ref view

\subsection draw Drawing Tools

- lti::draw is the basic object to draw simple geometric primitives on images.
- lti::draw3D allows the projection of simple 3D geometric primitives on
  an image or channel.
- lti::scene3D is a specialization of lti::draw3D, which remembers all steps 
  used while drawing.  This allows you to redraw a scene using new camera
  parameters.
- lti::epsDraw allows you to draw simple geometric primitives on an 
  Encapsulated Postscript file.

Visualization of Classification and Statistical Data
- lti::classifier2DVisualizer generates colorful images to analyze how
  a classifier distinguish between different classes in a 2D space.
- lti::sammonsMapping helps in the visualization of SOFM networks.
- lti::draw2DDistribution plots sets of 2D points using different point
  representations and colors.
- lti::hmmTrellisDiagram is helpful when working with Hidden Markov Models.
- lti::hsvHistogramViewer visualizes 2D histograms of hue and saturation.
- lti::labelAdjacencyMap analyzes a labeled-map (an integer valued channel8
  or matrix of integers) and creates a color image where the contrast of
  different adjacent regions is easier to be seen.

Other tools used for visualization

- lti::drawFlowField allows easy representation of optical flow results.
- lti::expandVector 

\subsection view Viewers

- lti::externViewer is used to invoke in a very easy way an external
  application that is supposed to show the image.
- lti::viewer is a <A HREF="http://www.gtk.org">GTK</A>-based object,
  which runs in an own thread, to allow the user a very easy
  way to visualize images, almost as easy as using the std::cout stream!
- lti::fastViewer is still available only for Linux/X-Windows
  (for Microsoft Windows it is just an alias for the lti::viewer).
  It displays an image without refreshing or updating the window content or
  allowing any kind interaction.  It is used to debug iterative algorithms,
  where a view into the "evolution" of the algorithm might be required (for 
  example, with snakes).  It requires a X-Windows Server with 32-bits per pixel   color depth.
- lti::histogramViewer displays 3D plots of the RGB color space for an image or
  a histogram.
- lti::scene3DViewer is a simple way to view 3D scenes, allowing you to change
  with the mouse the camera position, zoom and other parameters.  See also
  lti::scene3D.
<hr>

\section system Operating System and Hardware related classes

- lti::mutex and lti::semaphore are wrapper classes for the mutex and
  semaphores process synchronization concepts of the respective operating
  systems.
- lti::thread is a wrapper class for the system threads.  Every object
  you want to be executed in a separate thread must inherit from this
  object.
- lti::processInfo can be used to check for different system and process
  properties like free memory, processor frequency, processor load, etc.
- lti::timer is used to measure time with micro-second resolution.
- lti::passiveWait is a wrapper function for usleep or Sleep and expects
  a value in microseconds.  It sends the actual thread to the background for
  the given interval of time (passive wait).
- lti::serial is a wrapper class to access the serial ports.

<hr>

\section genother Other Objects

- lti::objectFactory is used to generate instances of classes.  It expects 
  the name of the required class in a string.
- lti::className  provides methods for getting the class names of lti::objects
- lti::typeInfo is a simple template class to ask for type norms and the
  floating point nature of a type.

*/

#endif

