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

#ifndef _LTI_GROUPS
#define _LTI_GROUPS

/**
 * @defgroup gDataStruct Data Structures
 *
 * Data Structures in the LTI-Lib
 * @see \ref classHierarchy
 */

/** 
 * @defgroup gMath Math
 *
 * Classes and Functions related with mathematical operations
 * @see \ref classHierarchy
 */

/**
 * @defgroup gGeomData Geometry Data Structures
 *
 * Data primitives for geometric algorithms
 * @ingroup gDataStruct
 * @ingroup gGeometry
 * @see \ref classHierarchy
 */

/**
 * @defgroup gAggregate Aggregate Data Types
 *
 * This group contains aggregate classes, which besides some data also provide 
 * some functionality to manipulate it.
 *
 * @ingroup gDataStruct
 * @see \ref classHierarchy
 */

/**
 * @defgroup gImageProcessing Algorithms and Classes for Image Processing
 *
 * This group contains all groups of functors and other classes related
 * with image processing.
 * @see \ref classHierarchy
 */

/**
 * @defgroup gLinearFilters Linear filtering
 *
 * Linear filtering kernels and functors
 * @ingroup gImageProcessing
 * @see \ref classHierarchy
 */

/**
 * @defgroup gEdgeCorner Edge and Corner Detectors
 *
 * Edge and Corner Detectors
 * @ingroup gImageProcessing
 * @see \ref classHierarchy
 */

/**
 * @defgroup gSegmentation Segmentation Algorithms and related tools
 *
 * Segmentation algorithms and related tools
 * @ingroup gImageProcessing
 * @see \ref classHierarchy
 */

/**
 * @defgroup gRegionAnalysis Region analysis
 *
 * Algorithms used to generate and used labeled masks denoting regions.
 * @ingroup gSegmentation
 * @see \ref classHierarchy
 */

/**
 * @defgroup gShape Shape Analysis
 *
 * @ingroup gImageProcessing
 * @see \ref classHierarchy
 */

/**
 * @defgroup gGeometry Simple geometry classes and functions
 *
 * @ingroup gMath
 * @ingroup gImageProcessing
 * @see \ref classHierarchy
 */

/**
 * @defgroup gMorphology Morphology operators
 *
 * @ingroup gImageProcessing
 * @see \ref classHierarchy
 */

/**
 * @defgroup gColor Color Analysis
 *
 * Classes and Data Structures used for color analysis or color representation
 *
 * @ingroup gImageProcessing
 * @see \ref classHierarchy
 */

/**
 * @defgroup gFeatureExtr Feature Extraction
 *
 * Classes and Data Structures used to extract descriptors from images
 * or image regions.
 *
 * @ingroup gImageProcessing
 * @see \ref classHierarchy
 */

/**
 * @defgroup gLinearAlgebra Linear Algebra 
 * 
 * @ingroup gMath
 * @see \ref classHierarchy
 */

/**
 * @defgroup gBasicMathFunc Basic Math Functions
 *
 * @ingroup gMath
 * @see \ref classHierarchy
 */

/**
 * @defgroup gStats Basic Statistics
 *
 * @ingroup gMath
 * @see \ref classHierarchy
 */

/**
 * @defgroup gIO Input and Output
 *
 * Classes and Functions used to read and write data.
 * @see \ref classHierarchy
 */

/**
 * @defgroup gStorable Storable Interface
 *
 * Methods using lti::ioHandler objects to read and write data.
 *
 * @ingroup gIO
 * @see \ref classHierarchy
 */

/**
 * @defgroup gVisualization Visualization Tools
 *
 * In this group belong not only the viewer and drawing classes, but all
 * functors used to "paint" data structures on images, in order to simplify
 * their interpretation and analysis.
 * @see \ref classHierarchy
 */

#endif
