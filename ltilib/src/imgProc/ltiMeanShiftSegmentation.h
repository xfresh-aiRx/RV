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
 * file .......: ltiMeanShiftSegmentation.h
 * authors ....: Axel Berner
 * organization: LTI, RWTH Aachen
 * creation ...: 22.6.2001
 * revisions ..: $Id: ltiMeanShiftSegmentation.h,v 1.23 2005/06/16 12:35:42 doerfler Exp $
 */

#ifndef _LTI_MEAN_SHIFT_SEGMENTATION_H_
#define _LTI_MEAN_SHIFT_SEGMENTATION_H_

#include "ltiSegmentation.h"
#include "ltiImage.h"
#include "ltiTimer.h"

namespace lti {
  /**
   * This is the implementation of the mean shift segmentation algorithm.
   * It is described in
   *
   * D. Comaniciu, P. Meer,
   * "Mean Shift: A Robust Approach toward Feature Space Analysis"
   * appeared in IEEE Trans. Pattern Analysis Machine Intell. Vol. 24, No. 5,
   * 603-619, 2002
   * (original link <a href="http://www.caip.rutgers.edu/~comanici/">
   * here </a>)
   *
   * http://www.caip.rutgers.edu/~comanici/Papers/MsRobustApproach.pdf
   *
   * The code here is based on the code of the original authors, modified
   * extensively for the use with the LTI-Lib.  It has changed a lot since
   * then to provide more functionality and configuration options.
   *
   * There are two implementations available:
   *
   * The classic implementation is selected setting the parameter
   * parameters::classicAlgorithm to \c true.  It is based on an old code
   * example published by Comaniciu and colleagues a few years ago.  This
   * implementation was not as good as the newer version, but was already
   * available in the LTI-Lib.  Since several applications were adapted to use
   * that mean-shift version, the default behaviour of this functors has not
   * been changed.
   *
   * The new implementation is an adaption of Comaniciu and Meer's EDISON code
   * (http://www.caip.rutgers.edu/riul/research/robust.html).  It can be
   * selected setting parameters::classicAlgorithm to \c false.  Decisive for
   * the result of the new implementation is the choice of the parameters
   * parameters::sigmaR, parameters::sigmaS,
   * parameters::maxNeighbourColorDistance, and the parameters::speedup.
   *
   * \warning This functor will be completely reviewed soon.  Its interface
   *          may change a little bit to improve its usability.
   *          It has now the problem that the parameters for
   *          the classic version and the ones for the new algorithmic
   *          implementation are different and they shouldn't.
   *
   * \todo The interface for classic and new algorithms has to be unified and
   *       simplified.  There are also several efficiency aspects to be
   *       reviewed.
   */
  class meanShiftSegmentation : public segmentation {
  public:
    /**
     * the parameters for the class meanShiftSegmentation
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
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);

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
      virtual bool write(ioHandler& handler,const bool complete=true) const;

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
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const;
#     endif

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * @name Parameters for the old algorithm
       */
      //@{
      /**
       * Constants to specify degree of segmentation
       */
      enum {
        /**
         * Constant value to specify the quantization option.
         */
        Quantization = 0,

        /**
         * constant value to specify the oversegmentation option.
         */
        Oversegmentation = 1,

        /**
         * constant value to specify the undersegmentation option.
         */
        Undersegmentation = 2
      };

      /**
       * Option: 0 Quantization
       *         1 Oversegmentation
       *         2 Undersegmentation
       *
       * You should use the given constants instead of the magic numbers...
       *
       * Default: Undersegmentation
       */
      int option;

      /**
       * Number of rectangles, in which the segmenter looks for a color
       *
       * If the first rectangle is (0,0,0,0) then an \e auto-segmentation
       * mode is activated and colors are taken from the whole image.
       * Otherwise the colors will be taken from the given image region.
       *
       * The number of rectangles specifies the maximal possible number of
       * colors for the segmented/quantized image.
       *
       * If you give only one rectangle with (0,0,0,0) or an empty vector,
       * then 50 elements will be assumed.
       *
       * Default value: vector with 1 element (0,0,0,0)
       */
      std::vector<rectangle> rects;

      /**
       * Maximal number of trials for choosing a valid representing color.
       *
       * Default: 10
       */
      int maxTrial;

      /**
       * Number of trials for the representing color to converge
       *
       * Default: 15 trials
       */
      int trial2converge;

      /**
       * How many pixel in promille of the image must have a colorClass.
       *
       * Each element of the array corresponds to one of the possible options
       * (see <code>option</code> parameter)
       *
       * Default value: {2.5, 5.0, 10.0} for Quantization, Over- and
       *                Undersegmentation respectively.
       */
      double classThreshold[3];

      /**
       * Number of trials to pick up randomly a suitable color in the
       * image(rect).
       *
       * Default: 25
       */
      int maxTrialRandomColor;


      /**
       * Set the minSize (in Pixel) a region must have.
       * default: 15
       */
      int minRegionSize;

      /**
       * Multiplication factor for the colorRadius (variance in image)
       *
       * Only used in auto-segmentation mode (see rects).
       *
       * Each element of the array corresponds to one of the possible options
       * (see <code>option</code> parameter);
       *
       * Default value: {8.0, 6.0, 4.0} for Quantization, Over- and
       *                Undersegmentation respectively.
       */
      float rectRadius[3];

      /**
       * Multiplication factor for the colorRadius(option)
       *
       * Use in auto-segmentation mode (see rects).
       *
       * Increase value if too many colors are found, because of a low
       * variance(image)
       *
       * Each element of the array corresponds to one of the possible options
       * (see \c option parameter);
       *
       * Default value: {2.0, 3.0, 4.0} for Quantization, Over- and
       *                Undersegmentation respectively.
       */
      float autoRadius[3];

      /**
       * Set the lower limit of a multiplication factor \e v for the
       * colorRadius. max( minVar , \e v )
       *
       * The factor \e v = sqrt((var.l+var.u+var.v)/100)
       * depends on the variance of the image in luv.
       *
       * For images with an homogeneous background and a small object, this
       * value could be increased to get better results (for example with 1.0).
       *
       * Default value: 0.0 (no influence on factor v)
       */
      float minVar;
      //@}

      /**
       * @name Parameters for the new algorithm
       */
      //@{

      /*
       * Using the  multivariate normal kernel yields better results
       * but is more computational intensive
       *
       * Default: false
       * not yet implemented!
       */
      bool multivariateNormalKernel;

      /**
       * Three types of speed-up techniques
       */
      enum eSpeedUpType{
        NoSpeedup,     /**< Filters the image applying mean shift to each
                        *   point.
                        *
                        *   Advantage: most accurate
                        *
                        *   Disadvantage : time expensive
                        */
        MediumSpeedup, /**< Filters the image using previous mode information
                        *   to avoid re-applying mean shift to some data
                        *   points (is default)
                        *
                        *   Advantage: maintains high level of accuracy,
                        *              large speed up compared to
                        *              non-optimized version
                        *
                        *   Disadvantage: possibly not as accurate as
                        *                 non-optimized version
                        */
        HighSpeedup    /**< filters the image using previous mode information
                        *   and window traversals to avoid re-applying mean
                        *   shift to some data points
                        *
                        *   advantage: huge speed up  maintains accuracy good
                        *              enough for segmentation
                        *   disadvantage: not as accurate as previous filters
                        */
      };

      /**
       * Higher speedup level causes loss of acuracy
       *
       * Default: MediumSpeedup
       */
      eSpeedUpType speedup;

      /**
       * The spatial radius of the mean shift sphere.
       * (the radius in grid space)
       *
       * Higher values cause longer computation times, and smoother region
       * boundaries.
       *
       * Default value: 5
       */
      double sigmaS;

      /**
       * The range radius of the mean shift sphere.
       * (the radius in color space)
       *
       * Higher values result in less regions
       *
       * Default value: 5
       */
      double sigmaR;

      /**
       * Regions having a color difference less than this parameter are joined
       * together (by the method fuseRegions()).
       *
       * It should be smaller than sigmaR.
       *
       * Default value: 3
       */
      double maxNeighbourColorDistance;

      /**
       * If the magnitude of the mean-shift vector is under this
       * threshold, it is considered as converged.
       *
       * Default value: 0.1
       */
      double thresholdConverged;

      /**
       * Choose algorithm by this parameter.
       *
       * - \c true: the old algorithm (for compatibility reasons still
       *            provided)
       * - \c false: the new implementation, like in EDISON.
       *
       * The classic algorithm uses the following parameters:
       * - option
       * - rects
       * - maxTrial
       * - trial2converge
       * - classThreshold
       * - maxTrialRandomColor
       * - minRegionSize
       * - rectRadius
       * - autoRadius
       * - minVar
       *
       * The new algorithm considers the following parameters;
       * - maxTrial
       * - speedup
       * - sigmaS
       * - sigmaR
       * - maxNeighbourColorDistance
       * - thresholdConverged
       *
       * Default value: true (classic algorithm is used)
       */
      bool classicAlgorithm;

    };

    /**
     * default constructor
     */
    meanShiftSegmentation();

    /**
     * default constructor
     */
    meanShiftSegmentation(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    meanShiftSegmentation(const meanShiftSegmentation& other);

    /**
     * destructor
     */
    virtual ~meanShiftSegmentation();

    /**
     * returns the name of this type ("meanShiftSegmentation")
     */
    virtual const char* getTypeName() const;

    /**
     * Apply the mean shift segmentation algorithm to the given image.
     *
     * This image will be internally transformed into the Luv color space, as
     * proposed by Comaniciu et.al. in their original paper.
     *
     * @param src image with the source data.
     * @param dest image where the result (segmented image) will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const lti::image& src, lti::image& dest);

    /**
     * Apply the mean shift segmentation algorithm to the given image.
     *
     * This image will be internally transformed into the Luv color space, as
     * proposed by Comaniciu et.al. in their original paper.
     *
     * @param src image with the source data.
     * @param dest imatrix where the result (region-labels) will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const lti::image& src,
               lti::imatrix& dest);

    /**
     * Apply the mean shift segmentation algorithm to the given image.
     *
     * This image will be internally transformed into the Luv color space, as
     * proposed by Comaniciu et.al. in their original paper.
     *
     * @param src image with the source data.
     * @param labels imatrix where the result (region-labels) will be left.
     * @param colorMap the color palette used
     * @return true if apply successful or false otherwise.
     */
    bool apply(const lti::image& src,
               lti::imatrix& labels,
               lti::palette& colorMap);

    /**
     * Apply the mean shift segmentation algorithm to a color image, which
     * has been already split into three channels.  The algorithm will work
     * with these channels without converting them into the Luv color space.
     * This allows the use of the mean shift segmentation in other color
     * spaces than Luv.
     *
     * @param chnl1 first channel of the source color image.
     * @param chnl2 second channel of the source color image.
     * @param chnl3 third channel of the source color image.
     * @param dest imatrix where the result (region-labels) will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const lti::channel8& chnl1,
               const lti::channel8& chnl2,
               const lti::channel8& chnl3,
               lti::imatrix& dest);


    /**
     * Apply the mean shift segmentation algorithm to the given image.
     *
     * This image will be internally transformed into the Luv color space, as
     * proposed by Comaniciu et.al. in their original paper.
     *
     * @param src image with the source data.
     * @param destFiltered the meanshift filtered image (not
     *        available for classic algorithm)
     * @param destSegmented the segmented image
     * @param destLabels label associating each pixel to a
     *        mode and thus a color to each pixel
     * @param destColorMap the colors of the found modes
     * @return true if apply successful or false otherwise.
     */
    bool apply(const lti::image& src,
	       lti::image& destFiltered,
	       lti::image& destSegmented,
	       lti::imatrix& destLabels,
	       lti::palette& destColorMap);

    /**
     * filter the image with meanshift algorithm
     *
     * @param src the image to be filtered
     * @param dest the result will be left here
     * @return true if apply successful or false otherwise.
     */
    bool filter(const lti::image& src, lti::image& dest);

    /**
     * filter the image with meanshift algorithm
     *
     * @param srcdest the image to be filtered
     * @return true if apply successful or false otherwise.
     */
    bool filter(lti::image& srcdest);

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    meanShiftSegmentation& copy(const meanShiftSegmentation& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  protected:

    /**
     * like rgbPixel for RGB-ColorSpace,
     * is luvPixel  for LUV-ColorSpace
     */
    struct luvPixel {
      float l,u,v;
    };

  private:

    //-----------------------------------------------------------------------
    // members used for classic algorithm - not used in new implementation
    //-----------------------------------------------------------------------
    /**
     * @name Classic implementation
     */
    //@{

    /**
     * internal structure to administrate a region list.
     */
    struct region {
      int my_contor;
      int my_class;
      int my_label;
      int *my_region;
      region* next_region_str;
    };

    /**
     * def. number for nonColor in palette(colorMap)
     */
    static const int nonColor; // set to 255

    static const int FIRST_SIZE; // = 262144 = 2^18
    static const int SEC_SIZE;   // = 64     = 2^6

    /**
     * radius for searching of similar colors
     * depend from variance of image and parms(autoRadius,rectRadius)
     */
    std::vector<float> fixRadius;

    /**
     * color-search-radius
     */
    float RADIUS;
    float RADIUS2; // RADIUS^2

    /**
     *  actual number of image pixels that must have a color class
     */
    int act_threshold;

    /**
     * neighbour for images, which are stored in a vector (not matrix)
     * for whole image
     */
    int neigh[8];

    /**
     * neighbour for images, which are stored in a vector (not matrix)
     * for subImage
     */
    int neigh_sub[8];

    /**
     * image and subimage(rect) splitted in L,U,V - components
     */
    matrix<int> chL,chU,chV;
    matrix<int> chLcurr,chUcurr,chVcurr; // current subImage (def by rect)

    bool        auto_segm; // true for autosegmentation

    int numImgPxl;    //number of pixel of the original image
    int numSubimgPxl; //number of pixel of subimage
    int numRemPxl;    //number of remaining pixels
    int numOrgColors; //number of colors in the original image
    int numRemColors; //number of remaining colors


    int*_col0,*_col1,*_col2;   // LUV color representation of org image

    /**
     * map of assigned labels for segmented image
     * value = index in colorPalette luvPalSegm[](LUV) and colorMap[](RGB)
     */
    ubyte* mapAssignedLabels;

    /*
     * map of the pixels colors
     * value = index of color palette _col0[],_col1[],_col2[]
     * mapPxlToColor[0...numSubimgPxl]
     */
    int* mapPxlToColor;

    /**
     * map of remaining pixels
     * value = index of original pixel
     * mapRemPxl[0...numRemPxl]
     */
    int* mapRemPxl;

    /**
     * map of remaining colors
     * value = index of color palette _col0[], col1[], _col2[]
     * mapRemColors[0...numRemColors]
     */
    int* mapRemColors;

    /**
     * how many occurences of each remaining color
     * value = number of occurences of this color
     * howManyEachRemColor[0...numRemColors]
     */
    int* howManyEachRemColor;


    /*
     *
     */
    void setRadius(const double r);

    /*
     * For non auto segmentation
     * Test if the clusters have the same mean, that is return true
     * if found color T[rect] is too near to previously found color
     * from T[0...rect]
     */
    bool testSameCluster(int rect, luvPixel T[]);

    /*
     *
     */
    void luv2rgb( luvPixel& final_T, lti::rgbPixel& TI);

    /*
     *
     */
    void convert_RGB_LUV( const lti::image signal,
			  const int _col_RGB[], const int _col_misc[] );

    /**
     * Method that copies the three channels as Luv channels into
     * the internal structures.
     */
    void convert_RGB_LUV( const lti::channel8& chnl1,
                          const lti::channel8& chnl2,
                          const lti::channel8& chnl3,
			  const int _col_RGB[], const int _col_misc[] );
    /*
     * copy a rectangle from the original image into the subimage
     * by filling current channels chLcurr,chUcurr, chVcurr and
     * determine neighbourhood vector neigh_r
     */
    void cutRectangle(const rectangle& rect );

    /*
     *
     */
    void init_matr(int _m_colors[]);

    /*
     * compute the mean color of N points given by J[] and return it by T
     */
    void meanColor(const int N, int J[], luvPixel& T);

    /*
     * get the color of a pixel within the subimage which
     * is located in a  high density region in color space
     * by calling N times subsample() and keep the color
     * with the highest density
     */
    bool my_sampling(luvPixel& T, int N);

    /*
     * take randomly one of the remaining pixels and call meanColor()
     * to compute the mean color of a neighbourhood of 8 pixels
     */
    void subsample(luvPixel& Xmean);

    /*
     * check all remaining points which were not yet selected. If they
     * have enough neighbours(1,2,3) converging to T[rect] they are
     * also selected for getting T[rect] assigned
     * @param my_class[] true if point should get color assigned
     * @param selected[] indices of points that get color T[rect] assigned
     * @param numSelectedPxl total number of points changing color (directly
     *                  selected ones and as neighbours selected points)
     */
    void add_neigh(bool my_class[], int selected[], int& numSelectedPxl);

    /*
     * Update numRemPxl, mapRemPxl[], numRemColors, mapRemColors[]
     * and howManyEachRemColor[]
     */
    void updateTables(const bool my_class[]);

    /*
     *
     */
    void covariance_w(int _m_colors[], luvPixel& m, luvPixel& v);

    /*
     *
     */
    void find_other_neigh(int k, int *my_ptr_tab, region *current_region,
			  int conn_selected[], bool taken[]);

    /*
     * the actual meanshift algorithm: compute median color of the colors
     * within a sphere of RADIUS and center final_T in color space
     * @param final_T input: center of sphere output: median color
     * @param sel_col[] true if this color is inside the sphere
     */
    void new_auto_loop(luvPixel& final_T, bool sel_col[] );

    /*
     *
     */
    void nauto_loop(luvPixel& final_T, int selected[],
		    bool my_class[], int& numSelectedPxl);

    /*
     * call new_auto_loop() until color T is converged to local maximum
     * and return all points touched by moving sphere
     * @param selected[] indices of points that change color to T (all points
     *                   that have their color touched by the sphere)
     * @param my_class[] true if point should get new color T assigned
     * @param numSelectedPxl number of points that should get new color T
     *         assigned
     */
    void calcRepresColorAuto (int selected[], bool my_class[],
			      luvPixel& T, int& numSelectedPxl);

    /*
     *
     */
    void calcRepresColorNAuto(int selected[], bool my_class[],
			      luvPixel& T, int& numSelectedPxl);

    /*
     *
     */
    void getBestPalEntry(const luvPixel T[], const int n_rects);

    /*
     *
     */
    void try2getPalEntry(const luvPixel T[], const int n_rects);

    /*
     *
     */
    void newPalette(luvPixel T[], int n_rects);

    /*
     *
     */
    void buildHistogram(const lti::image& signal, int* _col_RGB[],
		      int* _m_colors[], int* _col_misc[]);

    /**
     * build the histogram for a split image.
     */
    void buildHistogram(const lti::channel8& chnl1,
                        const lti::channel8& chnl2,
                        const lti::channel8& chnl3,
                        int* _col_RGB[],
                        int* _m_colors[],
                        int* _col_misc[]);

    void updateGenclass( int& n_rects,
                         const genericVector<bool>& valid_class, luvPixel T[]);

    /*
     *
     */
    void eliminateSmallClasses( int& n_rects, luvPixel T[]);

    /*
     *
     */
    void eliminateRegions(const luvPixel T[], const int my_lim);

    /*
     *
     */
    void destroyRegionList(region *);

    /*
     *
     */
    region* createRegionList(int *my_max_region);

    /*
     *
     */
    void initializations(const lti::image& src, std::vector<rectangle>& rects);

    /**
     * initialization for the case when the user gives directly the
     * color space (giving three channels)
     */
    void initializations(const lti::channel8& chnl1,
                         const lti::channel8& chnl2,
                         const lti::channel8& chnl3,
                         std::vector<rectangle>& rects);

    //@}

    //-----------------------------------------------------------------------
    // members used for new imlementation - not used in classic algorithm
    //-----------------------------------------------------------------------

    /**
     * @name new implementation
     */
    //@{

    /// width and height of original image
    int width, height;

    ///number of pixel of original image
    int imageSize;

    ///The range dimension is the number of dimensions of color space
    int dimensionRange;

    ///The spatial dimension is the number of dimensions of the image lattice
    int dimensionSpace;

     ///The dimension of the feature space =  dimensionRange + dimensionSpace
    int dimensionFeatureSpace;

    ///represents the original image in luv space
    float* imageLuvOrgF;

    ///represents the meanshift filtered image in luv space
    float* imageLuvFilteredF;

    ///represents the meanshift filtered image in luv space (rounded integers)
    int* imageLuvFilteredI;

    /// contains for each pixel a weight factor
    float* weightMap;

    ///assigns a label to each data point associating it to a mode in modes
    ///e.g. a data point having label i has mode modes[i]
    int * labels;

    ///containes all found modes as luv values (l1,u1,v1,l2,u2,v2,l3,u3,v3,...)
    float *modes;

    ///counts the points associated to each mode
    vector<int> modePointCounts;

    ///number of found regions
    int regionCount;

    ///for debug and analysis information only: counts how many pixels converged in
    ///one step, two steps etc...
    vector<int> vecTrialsToConverge;

    /**
     * initialize members and allocate memory
     */
    void initialize();


    /**
     * filters the image applying mean shift to each point
     * advantage: most accurate
     * disadvantage : time expensive
     */
    void nonOptimizedFilter();


    /**
     * filters the image using previous mode information
     * to avoid re-applying mean shift to some data points
     * speedup depends on parameter speed
     */
    void optimizedFilter();


    /**
     * connect neighbouring pixels having the same color values to a region
     * by giving them the same label in labels[]
     * input: imageLuvFilteredI
     * output: labels, modes, regionCount, modePointCounts
     */
    void connect();


    /**
     * fuse regions with similar color (modes within sphere)
     * input: labels, modes, regionCount, modePointCounts
     * output: labels, modes, regionCount, modePointCounts
     */
    void fuseRegions();

    /**
     * prune regions that have less than minRegionSize pixels
     * input: labels, modes, regionCount, modePointCounts
     * output: labels, modes, regionCount, modePointCounts
     */
    void pruneRegions();

    /**
     * build an array of regionAdjacencyLists (raMatrix) containing all
     * neighbour regions for each region (used in fuseRegions())
     * input: labels, regionCount
     * output: raMatrix
     */
    void buildRegionAdjacencyMatrix();


    /**
     * convert image from rgb (src) to luv (imageLuvOrgF)
     */
    void rgbToLuv(const image& src);

    /**
     * convert image from luv (src) to rgb (dest)
     */
    void luvToRgb(float* src, image& dest);


    /**
     * convert one luv pixel(src) to one rgb pixel(dest)
     */
    void luvToRgb(luvPixel src, rgbPixel& dest);

     /**
     * free memory allocated by initialize()
     */
    void freeMemory();


    /**
     * define region adjacency list class
     * which has one entry for each neighbour region of this (one) region
     * the first entry represents the region itself and not a neighbour
     */
    class regionAdjacencyList {

    public:

      /**
       * the labels of this region
       */
      int label;

      /**
       * the next neighbour of the considered region
       */
      regionAdjacencyList* next;

      regionAdjacencyList();

      /** insert a region node into the region adjecency list
       * return true indicates that the node was inserted
       * return false indicates that the node was not inserted
       * because this neighbour region is already in the list
       */
      bool insert(regionAdjacencyList* entry);	

    private:

      // pointer to current and previous entry
      regionAdjacencyList *cur, *prev;
    };

    //used in fuseRegions()
    regionAdjacencyList* raMatrix, *raPool;

    //@}
  };
}

#endif


