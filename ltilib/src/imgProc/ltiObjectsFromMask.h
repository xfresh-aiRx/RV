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
 * file .......: ltiObjectsFromMask.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 5.10.2000
 * revisions ..: $Id: ltiObjectsFromMask.h,v 1.41 2005/06/16 12:35:42 doerfler Exp $
 */

#ifndef _LTI_OBJECTS_FROM_MASK_H_
#define _LTI_OBJECTS_FROM_MASK_H_

#include "ltiTypes.h"
#include "ltiImage.h"
#include "ltiContour.h"
#include "ltiMath.h"
#include "ltiGeometricFeatures.h"
#include "ltiSegmentation.h"
#include "ltiSTLIoInterface.h"
#include <list>
#include "ltiTree.h"

namespace lti {
  /**
   * This class works on channel8/imatrix to extract connected components
   * and describes them as the topological order of "objects" and
   * "holes".
   *
   * In the following example there are four objects, a triangle,
   * a rectangle, a circle and a line. There are also two holes.
   * The first one is inside the rectangle, the second is inside
   * the circle. The circle itself and the line are inside the
   * first hole.
   *
   * \code
   *  ---------------------------------
   *  ---*******-----------------------
   *  ----*****------------------------
   *  -----***----***************------
   *  ------*-----***************------
   *  ------------**-----------**------
   *  ------------**---***---*-**------
   *  ------------**--**-**--*-**------
   *  ------------**---***---*-**------
   *  ------------**---------*-**------
   *  ------------**-----------**------
   *  ------------***************------
   *  ------------***************------
   *  ---------------------------------
   *
   *  "-" means background
   *  "*" indicates objects
   * \endcode
   *
   * The topological structure of objects and holes can be
   * described as a tree, where odd numbered levels contain
   * objects and even numbered contain holes. (Level 0 is
   * an exception, because it represents the top-level, which
   * in general stands for the whole image.
   *
   * For the given example the tree looks like this:
   * \code
   * level 0             image
   *                       /\
   *                      /  \
   * level 1   triangle(*)    rectangle(*)
   *                               \
   *                                \
   * level 2                       hole(-)
   *                                /\
   *                               /  \
   * level 3              circle(*)    line(*)
   *                        /
   *                       /
   * level 4        hole(-)
   * \endcode
   *
   *
   * Each node of the tree contains borderPoints, areaPoints and
   * ioPoints. The tree can be obtained by using the
   * apply(...tree<objectStruct>...) method.
   *
   * If you are only interested in the objects and don't need the
   * holes, then you can use the other apply() methods, which deliver
   * a list of borderPoints, ioPoints or areaPoints for each object.
   * A labeled mask is also available.
   */

  class objectsFromMask : public segmentation {
  public:
    /**
     * the parameters for the class objectsFromMask
     */
    class parameters : public segmentation::parameters {
    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * destructor
       */
      ~parameters();

      /**
       * returns name of this type
       */
      const char* getTypeName() const;

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,
                         const bool complete=true) const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

#     ifdef _LTI_MSC_6
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is public due to another bug!, so please
       * NEVER EVER call this method directly
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is public due to another bug!, so please
       * NEVER EVER call this method directly
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const;
#     endif

      /**
       * A value >= this threshold is regarded as object (*), all
       * other values are background (-).
       *
       * Default is 1 (Only 0 (black) is background).
       *
       * Possible range is (1..255) */
      int threshold;

      /**
       * Regards input channel/matrix as a "labeled" mask if true.
       * Connected regions with identical label numbers are
       * extracted. Note: labels below threshold are regarded
       * as background!
       *
       * Default is false
       */
      bool assumeLabeledMask;

      /**
       * This value defines the minimum size that an object must have
       * in order to be detected. Please note that this refers to the size of
       * the data structure, <b>not</b> the actual object! I.e. if 
       * borderPoints are extracted, <tt>minSize</tt> refers to the minimum
       * number of border points (which, by the way, is not the border length!
       * see eBoundaryDefinition). So, a rough correspondence to the object's
       * area is only given if areaPoints are extracted. In other words, the
       * meaning of this parameter depends on which of the apply methods is
       * used.
       *
       * Default is 1
       */
      int minSize;

      /**
       * This value defines the maximum recursion depth for object and hole
       * extraction. The level number corresponds to the maximum depth of the
       * tree.
       *
       * I.e. level=0: search top-level for objects<br>
       *      level=1: find holes in top-level objects<br>
       *      level=2: search objects within holes of
       *               top-level objects<br>
       *      level=3: find their holes ... <br>
       *
       * Default is -1, which means the recursion goes as deep as possible
       */
      int level;

      /**
       * It is possible to close the holes within an object by setting
       * this value. This isn't necessary, if for example level==0, since
       * holes inside objects aren't considered then anyway.
       *
       * Default is false
       */
      bool meltHoles;

      /**
       * This %parameter lets the objects be sorted by the size of the
       * pointList type used.  This option will be ignored for the
       * tree-apply().
       *
       * Sorting by "size" means sorting considering the size of the
       * pointList used in each specific apply() method.  I.e. for areaPoints
       * this will be the number of area pixels, for border points,
       * the number of pixels at the border, and for ioPoints the number of
       * input-output points.
       *
       * Default is false
       */
      bool sortObjects;

      /**
       * Use objects' actual area size for sorting (true), or rather the size
       * of the data structure itself (false; default).
       */
      bool sortByArea;

      /**
       * Generally the whole input %channel8 / %imatrix is as top-level
       * for detection. Instead you may specify any region for search with
       * this parameter.
       * Default is an empty list, which means the whole %channel8 / %imatrix
       * will be used.
       */
      lti::ioPoints ioSearchAreaList;
    };

    /**
     * This node-structure for storing information about all
     * objects and holes in the tree.
     */
    class objectStruct : public ioObject {
    public:
      ioPoints      ioPointList;      /**< the ioPoints of the object or hole*/
      areaPoints    areaPointList;    /**< the areaPoints of the object or
                                           hole*/
      borderPoints  borderPointList;  /**< the borderPoints of the object or
                                           hole*/
      /**
       * Write structure to handler
       */ 
      virtual bool write(ioHandler& handler,const bool complete) const;

      /**
       * Read structure from handler
       */
      virtual bool read(ioHandler& handler,const bool complete);
    };

    /**
     * constructor
     */
    objectsFromMask();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    objectsFromMask(const objectsFromMask& other);

    /**
     * destructor
     */
    virtual ~objectsFromMask();

    /**
     * returns the name of this type ("objectsFromMask")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on the given %parameter to extract only objects.
     * @param src8 channel8 input-channel
     * @param lstIOPointLists std::list of ioPoints
     */
    bool apply(const channel8& src8,
               std::list<ioPoints>& lstIOPointLists);
    /**
     * operates on the given %parameter to extract only objects.
     * @param src matrix<int> input-channel
     * @param lstIOPointLists std::list of ioPoints
     */
    bool apply(const matrix<int>& src,
               std::list<ioPoints>& lstIOPointLists);

    /**
     * operates on the given %parameter to extract only objects.
     * @param src8 channel8 input-channel
     * @param lstBorderPointLists std::list of borderPoints
     */
    bool apply(const channel8& src8,
               std::list<borderPoints>& lstBorderPointLists);

    /**
     * operates on the given %parameter to extract only objects.
     * @param src matrix<int> input-channel
     * @param lstBorderPointLists std::list of borderPoints
     */
    bool apply(const imatrix& src,
               std::list<borderPoints>& lstBorderPointLists);

    /**
     * operates on the given %parameter to extract only objects.
     * @param src8 channel8 input-channel
     * @param lstAreaPointLists std::list of areaPoints.  The boundary of
     *                          each areaPoints object will be updated.
     */
    bool apply(const channel8& src8,
               std::list<areaPoints>& lstAreaPointLists);
    /**
     * operates on the given %parameter to extract only objects.
     * @param src matrix<int> input-channel
     * @param lstAreaPointLists std::list of areaPoints.  The boundary of
     *                          each areaPoints %object will be updated.
     */
    bool apply(const matrix<int>& src,
               std::list<areaPoints>& lstAreaPointLists);

    /**
     * operates on the given %parameter to extract only objects.
     * @param src8 channel8 input-channel
     * @param lstAreaPointLists list of area points.  The boundary of
     *                          each areaPoints %object will be updated.
     * @param labeledMask returns a matrix of integers, what represent the
     *        number of the segmented object
     */
    bool apply(const channel8& src8,
               std::list<areaPoints>& lstAreaPointLists,
               matrix<int>& labeledMask);

    /**
     * operates on the given %parameter to extract only objects.
     * @param src matrix<int> input-channel
     * @param lstAreaPointLists list of areaPoints.   The boundary of
     *                          each areaPoints object will be updated.
     * @param labeledMask returns a matrix of integers, what represent the
     *        number of the segmented object
     */
    bool apply(const matrix<int>& src,
               std::list<areaPoints>& lstAreaPointLists,
               matrix<int>& labeledMask);

    /**
     * operates on the given %parameter to extract objects AND holes in
     * their topological order.
     * @param src8 channel8 input-channel
     * @param objectTree tree, that contains io-, border- and areaPoints
     */
    bool apply(const channel8& src8,
               tree<objectStruct>& objectTree);
    /**
     * operates on the given %parameter to extract objects AND holes in
     * their topological order.
     * @param src matrix<int> input-channel
     * @param objectTree tree, that contains io-, border- and areaPoints
     */
    bool apply(const matrix<int>& src,
               tree<objectStruct>& objectTree);

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    objectsFromMask& copy(const objectsFromMask& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  private:
    objectStruct m_object;

    objectStruct* pObject;
    objectStruct* pObject2;

    channel8 chain; // temp channel for detected chaincodes
    int vres,hres; // resolution of image
    int vresm1,hresm1; // resulution of image minus 1
    int maxThresh; // = max value of variable-type (eg. channel8: 255)

    ioPoints::iterator iterIOPoint;
    ioPoints::iterator iterIOPointEnd;
    ioPoints::iterator iterIOPointTmp;

    static const int markarray[8][8];
    static const int areaarray[8][8];
    static const int xsteparray[8];
    static const int ysteparray[8];

    bool bCalcIoPoints,bCalcBorderPoints,bCalcAreaPoints;

    parameters gParams;

    tree<objectStruct>  objectTree;

    std::list<ioPoints>* plstIOPointLists;
    std::list<borderPoints>* plstBorderPointLists;
    std::list<areaPoints>* plstAreaPointLists;

    /**
     * This is an internal help function for scanning the matrix reg. Objects
     * @param pImg imatrix to be scanned
     * @param thresh threshold to be used for infront segmentation
     * @param numOfObjects number of objects to be detected
     * @param startPoint point for starting the scan
     * @param lstBorderPointLists borderPoint list for detected objects
     * @param lstIOPointLists ioPoint list for detected objects
     * @param calcIOPoints this flag decides calculating the ioPoint list
     * @return the number of detected objects
     */
    int detectObjects(const imatrix&    pImg,
                      const int&        thresh,
                      lti::ioPoints&    lstIOSearchArea,
                      tree<objectStruct>::node& mNode);

    /**
     * This is an internal help function for scanning the matrix reg. Objects
     * @param pImg imatrix to be scanned
     * @param thresh threshold to be used for infront segmentation
     * @param numOfObjects number of objects to be detected
     * @param startPoint point for starting the scan
     * @param lstBorderPointLists borderPoint list for detected objects
     * @param lstIOPointLists ioPoint list for detected objects
     * @param calcIOPoints this flag decides calculating the ioPoint list
     * @return the number of detected objects
     */
    int detectObjectsSimple(const imatrix&   pImg,
                            const int&        thresh,
                            lti::ioPoints&    lstIOSearchArea,
                            tree<objectStruct>::node& mNode);

    /**
     * This is an internal help function for scanning the matrix reg. Objects
     * @param pImg imatrix to be scanned
     * @param thresh threshold to be used for infront segmentation
     * @param numOfObjects number of objects to be detected
     * @param startPoint point for starting the scan
     * @param lstBorderPointLists borderPoint list for detected objects
     * @param lstIOPointLists ioPoint list for detected objects
     * @param calcIOPoints this flag decides calculating the ioPoint list
     * @return the number of detected objects
     */
    int detectObjectsLabeled(const imatrix&   pImg,
                             const int&        thresh,
                             lti::ioPoints&    lstIOSearchArea,
                             tree<objectStruct>::node& mNode);

    /**
     * This is an internal help function for scanning the matrix reg. Holes
     * @param pImg imatrix to be scanned
     * @param thresh threshold to be used for infront segmentation
     * @param numOfObjects number of objects to be detected
     * @param startPoint point for starting the scan
     * @param lstBorderPointLists borderPoint list for detected objects
     * @param lstIOPointLists ioPoint list for detected objects
     * @param calcIOPoints this flag decides calculating the ioPoint list
     * @return the number of detected objects
     */
    int detectHoles(const imatrix&   pImg,
                    const int&        thresh,
                    lti::ioPoints&    lstIOSearchArea,
                    tree<objectStruct>::node& mNode);

    /**
     * This is an internal help function for scanning the matrix reg. Holes
     * @param pImg imatrix to be scanned
     * @param thresh threshold to be used for infront segmentation
     * @param numOfObjects number of objects to be detected
     * @param startPoint point for starting the scan
     * @param lstBorderPointLists borderPoint list for detected objects
     * @param lstIOPointLists ioPoint list for detected objects
     * @param calcIOPoints this flag decides calculating the ioPoint list
     * @return the number of detected objects
     */
    int detectHolesSimple(const imatrix&   pImg,
                          const int&        thresh,
                          lti::ioPoints&    lstIOSearchArea,
                          tree<objectStruct>::node& mNode);

    /**
     * This is an internal help function for scanning the matrix reg. Holes
     * @param pImg imatrix to be scanned
     * @param thresh threshold to be used for infront segmentation
     * @param numOfObjects number of objects to be detected
     * @param startPoint point for starting the scan
     * @param lstBorderPointLists borderPoint list for detected objects
     * @param lstIOPointLists ioPoint list for detected objects
     * @param calcIOPoints this flag decides calculating the ioPoint list
     * @return the number of detected objects
     */
    int detectHolesLabeled(const imatrix&   pImg,
                           const int&        thresh,
                           lti::ioPoints&    lstIOSearchArea,
                           tree<objectStruct>::node& mNode);

    /**
     * This is an internal help function for building the tree recursively
     * @param src imatrix to be scanned
     * @param ioPointList ioPointList, within objects are be detected
     * @param mNode node under that the detected objects has to be inserted
     * @param level the level being detected/analysed
     */
    bool buildTree(const imatrix&   src,
                   ioPoints&         ioPointList,
                   tree<objectStruct>::node& mNode,
                   const int& level);

    /**
     * This is an internal help function for checking the parameters
     */
    bool checkParameters();

    /** A feature extractor required for sorting the objects by size. */
    lti::geometricFeatures featureExtractor;

    /** Used for sorting objects by size. */
    static double getArea(void* thisPtr, const areaPoints& ap);

    /** Used for sorting objects by size. */
    static double getArea(void* thisPtr, const borderPoints& bp);

    /** Used for sorting objects by size. */
    static double getArea(void* thisPtr, const ioPoints& iop);

    /**
     * Constants for the Canzler codes
     */
    enum {
      Nothing = 0, /**< constant for nothing */
      Top   = 1,   /**< constant for left border */
      Right = 2,   /**< constant for left border */
      Bottom= 4,   /**< constant for left border */
      Left  = 8,   /**< constant for left border */
      All   =15    /**< constant for all borders */
    };
  public:

    /**
     * Chain code constants (also used in geometricFeatures, hence
     * public)
     */
    enum {
      Invalid = -1, /**< Invalid */
      E  = 0,       /**< East or Right */
      SE = 1,       /**< South-East or Bottom Right */
      S  = 2,       /**< South or Bottom */
      SW = 3,       /**< South-West or Bottom Left */
      W  = 4,       /**< West or Left */
      NW = 5,       /**< North-West or Upper Left */
      N  = 6,       /**< North or Top */
      NE = 7        /**< North-East or Upper Right */
    };
  };
}

#endif

