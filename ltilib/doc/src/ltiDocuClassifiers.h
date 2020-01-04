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
 * file .......: ltiDocuClassifiers.h
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 08.11.2002
 * revisions ..: $Id: ltiDocuClassifiers.h,v 1.6 2003/05/06 16:14:47 alvarado Exp $
 */

#ifndef _LTI_DOCUCLASSIFIERS_H_
#define _LTI_DOCUCLASSIFIERS_H_

/**
\page docuClassifiers How to use the Classifiers.

- \ref types
- \ref internalclasses
- \ref related
  - \ref viz
  - \ref classStat
  - \ref sammon
- \ref unsupervised
- \ref instance
- \ref sequence

\section types Types of Classifiers 

All classifiers follow the same principle. First, they are trained
using some kind of training data. The trained classifier can then be
used to classify new data, i.e. to assign the same label to the new
data as to the most similar training data.

Three different categories of classifiers exist in the LTI-Lib:
- unsupervised classifiers
- supervised sequence classifiers
- supervised instance classifiers

The first category includes all algorithms that are trained without
knowing the proper labels or results for each training
pattern. Clustering algorithms are included here, although most are
not designed for later classifying new data. Supervised classifiers
are trained knowing the expected result for each training
pattern. They are divided in two groups: sequence classifiers work on
time series whereas instance classifiers work on n-dimensional
vectors.

All classifiers have four essential properties:

- The \b parameters are static properties of the classifier. All
  classifiers have default settings for the parameters
  object. However, many classifiers will not work properly without
  adjustment of some of the parameters.
- The \b status contains error messages. If any of the methods,
  particularly train and classify, return false. Information of the
  nature of the failure can be obtained by calling getStatusString().
- The \b progress of the training can be monitored with a
  progressObject. See \ref progress.
- The \b outputTemplate contains information on how internal results
  are mapped to an ouputVector which is the result of a
  classification. This member should only be changed by experienced
  users. Usually, the outputTemplate is set by the classifier itself
  during the training.

The results of the classification are returned in an ouputVector. This
object contains labels and corresponding recognition values. These can
often be interpreted as probabilities. Section \ref outvec gives
details about this data structure.

\section internalclasses Helper Classes

\subsection progress Progress Objects

The progressInfo object gives information on how many steps an
algorithm will take until finished and how many have already been
accomplished. Also the name of the classifier is usually given. The
following progress infos exist:

- lti::streamProgressInfo writes ASCII into a stream. The default
  stream is the console output.

\subsection params Parameters

The parameters of the classifier class define an enumeration
eDistanceMeasure which is used to specify which distance is used in
the classifier. Options are the L1 and L2 distances.

See also lti::classifier::parameters.

\subsection outvec Output Vector

The output vector is the result of a classification, i.e. calling the
classify method of a classifier. It assigns values to the labels. In
the case of supervised classifiers these labels where supplied by the
user during training. In case of unsupervised classification the
classifier usually assigns labels from 0 to C-1 with C the number of
classes found. 

Output vectors can be the final result of a process. In this case they
are usually displayed by an application or used for statistical
analysis of the classification process. For the later the
classificationStatistics %functor can be used. It is also possible to
combine the results of several classifiers using the combination
%functor.

For further reading see the documentation of lti::classifier::outputVector.

\section related Related Topics


\subsection viz Visualization of Classifiers and Data

\subsubsection vizData Visualizing Data

lti::draw
lti::epsDraw
lti::draw3D
lti::draw2DDistribution

\subsubsection vizClass Visualizing Classification Results

lti::classifier2DVisualizer

\subsection classStat Classification Statistics

lti::classificationStatistics

\subsection sammon Sammon's Mapping

Sammon's Mapping transforms points in n-dimensional space to points in
m-dimensional space while trying to preserve all distances between the
points. Usually, m will be 2 or 3 so that the points can be displayed
using one of the lti::draw classes and an appropriate
lti::viewer. Sammon's mapping can be very useful to get an idea of the
distribution of higher dimensional data without losing as much
information as when using e.g. lti::principalComponents to reduce the
dimensionality.

However, the mapping is a very difficult task and might easily fail to
converge at a minimum. Check the error to get an idea about the performance. In case it is bad there are several options:
- try to reduce the amount of data (point). This can be done e.g. by
  using a lti::clustering algorithm with the number of clusters a
  fraction of the number of data points, say a third. Then use the
  cluster centers as data points. This is an easy quantization method.
- reduce the number of dimensions a little e.g. to 10 with
  lti::principalComponents or a feature selection method. Use the
  lower dimensional points for Sammon's mapping. Thus, at least you'll
  get the information from 10 dimensions.

For more information see lti::sammonsMapping.

*/

/**
\page unsupervised Unsupervised Classifiers

Unsupervised classifiers are methods that try to find 'natural' or
'sensible' structure in data. To this end each data point is usually
member of a newly found group. A distinct label is assigned to each
group. Some algorithms allow membership of each point to more then one
group. One example is the \ref fcm.

The most popular application of unsupervised classifiers is data
analysis and possibly visualization. However, some methods are also
used for data compression or quantization.

This page first deals with clustering techniques. These are usually
statistical methods for finding groups (clusters). The second section
deals with an artificial neural network, the Self-Organizing Feature
Maps, which perform unsupervised classification.

- \ref cluster
  - \ref kMeans
  - \ref fcm
  - \ref adapt
- \ref som
  - \ref som2
  - \ref somViz

Related classes:
  - lti::unsupervisedClassifier

\section cluster Clustering

- \ref kMeans
- \ref fcm
- \ref adapt

\subsection kMeans k-Means Clustering

lti::kMeansClustering

\subsection fcm Fuzzy-C-Means Clustering

lti::fuzzyCMeans

\subsection adapt Adaptive k-Means Clustering

lti::adaptiveKMeans

\section som Self Organizing Feature Maps

lti::SOFM

- \ref som2
- \ref somViz

\subsection som2 2D SOMs

lti::SOFM2D

\subsection somViz Visualization of SOMs

Still to be done.

*/

/**
\page instance Supervised Instance Classifiers

- \ref stat
  - \ref svm
  - \ref shClass
  - \ref trees
- \ref ann
  - \ref mlp
  - \ref lvq
  - \ref rbf

\section stat Statistical Classifiers

- \ref svm
- \ref shClass
- \ref trees

\subsection svm Support Vector Machine

lti::svm

\subsection shClass Sparse Histogram-based Classifier

lti::shClassifier

\subsection trees Decision Trees

lti::decisionTree

\section ann Artificial Neural Networks

- \ref mlp
- \ref lvq
- \ref rbf

\subsection mlp Multi Layer Perceptron

lti::MLP

\subsection lvq Learning Vector Quantization

lti::lvq

\subsection rbf Radial Basis Function Network

lti::rbf

*/

/**
\page sequence Supervised Sequence Classifiers

lti::hmmClassifier
*/


*/

#endif
