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


/*--------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiClassifier2DVisualizer.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 15.5.2002
 * revisions ..: $Id: ltiClassifier2DVisualizer.h,v 1.15 2005/06/16 09:32:40 doerfler Exp $
 */

#ifndef _LTI_CLASSIFIER_2D_VISUALIZER_H_
#define _LTI_CLASSIFIER_2D_VISUALIZER_H_

#include "ltiTypes.h"
#include "ltiColors.h"
#include "ltiFunctor.h"
#include "ltiImage.h"
#include "ltiGenericVector.h"
#include "ltiSupervisedInstanceClassifier.h"

namespace lti {
  /**
   * This functor creates beautiful pictures. They additionally
   * have the property that they show the classification properties
   * of a supervisedInstanceClassifier for a 2D feature space.
   * Its application is rather simple: You have to create a
   * classifier for a 2D space, then you give it to one of the
   * apply methods, together with the target image.
   *
   * Example:
   * \code
    // number of training samples
   * const int N=150;
   * // number of classes
   * const int K=3;
   * const int width=400;
   * const int height=200;
   *
   * // fill data and label matrix
   * lti::dmatrix data(N,2);
   * lti::ivector labels(N);
   * int j=0;
   *
   * std::cerr << "Generating data...";
   * for (int k=0; k<K; k++) {
   *   // generate gaussian
   *   double sigma=double(k)/(K)*20.0+10;
   *   double mx=double(k)/K*(width-sigma)+sigma;
   *   double my=double(k)/K*(height-sigma)+sigma;
   *   lti::gaussianDistribution px(mx,sigma);
   *   lti::gaussianDistribution py(my,sigma);
   *   for (int i=0; i<N/K; i++) {
   *     data[j][0]=px.draw();
   *     data[j][1]=py.draw();
   *     labels[j]=k;
   *     j++;
   *   }
   * }
   *
   * std::cerr << "\nTraining MLP...";
   * // train MLP
   * lti::MLP s;
   * lti::streamProgressInfo inf(std::cerr,"MLP");
   * s.setProgressObject(inf);
   * lti::MLP::parameters p=s.getParameters();
   *
   * s.setParameters(p);
   * if (!s.train(data,labels)) {
   *   std::cerr << "Error: " << s.getStatusString() << "\n";
   *   //return;
   * }

   * std::cerr << "\nVisualizing...";
   * // NOW COMES THE INTERESTING PART:
   * // we need an image
   * lti::image panel;
   * lti::classifier2DVisualizer visual;
   * lti::classifier2DVisualizer::parameters vp;
   * // set the parameters
   * vp.showBoundaries=true;
   * vp.imgSize=point(width,height);
   * vp.upperRight=dpoint(width,height);
   * visual.setParameters(vp);
   * // create the image
   * if (!visual.apply(s,data,panel,labels)) {
   *   std::cerr << "Error: " << s.getStatusString() << "\n";
   *   return;
   * }
   * // enjoy!
   * \endcode
   * The visualization result of this example is shown in \image html svmvis.png
   */
  class classifier2DVisualizer : public functor {
  public:
    /**
     * the parameters for the class classifier2DVisualizer
     */
    class parameters : public functor::parameters {
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
       * read the parameters from the given ioHandler
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

      //TODO: comment the parameters of your functor
      // If you add more parameters manually, do not forget to do following:
      // 1. indicate in the default constructor the default values
      // 2. make sure that the copy member also copy your new parameters
      // 3. make sure that the read and write members also read and
      //    write your parameters


      /**
       * The lower left corner of the area that is to be plotted.
       * Default is (0,0).
       */
      dpoint lowerLeft;

      /**
       * The upper right corner of the area that is to be plotted.
       * Default is (1.333,1)
       */
      dpoint upperRight;

      /**
       * The size of the image that is generated. Default is
       * (1024,768).
       */
      point imgSize;

      /**
       * The size of each virtual pixel. A virtual pixel is a
       * rectangle whose center is classified. Default is 2.
       */
      int pixSize;

      /**
       * The size of a highlight. Default is 6.
       */
      int highlightSize;

      /**
       * If this is true, the visualizer will draw boundaries
       * between classes. A pixel is assigned to the class
       * with the highest classification output.
       *
       * Default value: false
       */
      bool showBoundaries;

      /**
       * The color of the drawn boundaries.
       *
       * Default is Grey75.
       */
      rgbColor boundaryColor;

      /**
       * The color of the drawn highlights.
       *
       * Default is White.
       */
      rgbColor highlightColor;

      /**
       * The color of the drawn highlights2.
       *
       * Default is Black.
       */
      rgbColor highlightColor2;

      /**
       * The offset for choosing the class color. If pixels are
       * colored, this value is added to the label to get
       * the index for the color table.
       *
       * Default is 0.
       */
      int colorOffset;

      /**
       * The colormap which is to be used for assigning colors
       * to labels. Note that the colors are assigned cyclically,
       * i.e. if there are more classes than labels,
       * there will be different classes with the same colors.
       *
       * Default value: a palette composed by the values Red, Green, Blue,
       *                Yellow, Magenta, Cyan, DarkOrange, Fusia, BrightGreen,
       *                LawnGreen, LightBlue, DarkViolet, Grey75, Grey50 and
       *                Grey25
       */
      palette colorMap;

      /**
       * Each color channel of the final image will usually have
       * values higher than the possible displayable values from 0.0
       * to 1.0.  To normalize the RGB channels, two options are
       * given.  If scaleRGB together is true all three channels will
       * be normalized by the same value, which will be the maximal
       * value between all three channels.  If false (default), each
       * channel will be normalized by its maximum.
       *
       * Default value: false
       */
      bool scaleRGBTogether;

      inline const rgbPixel& getColor(int i) const {
        assert(colorMap.size() > 0);
        return colorMap.at((i+colorOffset)%colorMap.size());
      }

      inline int getNumberOfColors() const {
        return colorMap.size();
      }

    protected:
      // class colors for visualization
      static const lti::rgbColor defaultColors[];
    };


    /**
     * default constructor
     */
    classifier2DVisualizer();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    classifier2DVisualizer(const classifier2DVisualizer& other);

    /**
     * destructor
     */
    virtual ~classifier2DVisualizer();

    /**
     * returns the name of this type ("classifier2DVisualizer")
     */
    virtual const char* getTypeName() const;

    //TODO: comment your apply methods!

    /**
     * Applies the given classifier to the area defined in the
     * parameters object. The image will contain the visualization
     * result.
     * @param cls supervisedInstanceClassifier with the classifier
     *            to be visualized. The classifier must be able to
     *            classify a 2D feature set.
     * @param result will receive the visualization result.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const supervisedInstanceClassifier& cls,
               image& result) const;

    /**
     * Applies the given classifier to the area defined in the
     * parameters object. The image will contain the visualization
     * result. It also highlights the vectors given in highlights.
     * @param cls supervisedInstanceClassifier with the classifier
     *            to be visualized. The classifier must be able to
     *            classify a 2D feature set.
     * @param highlights matrix with each row containing a vector
     *        that is to be highlighted.
     * @param result will receive the visualization result.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const supervisedInstanceClassifier& cls,
               const lti::dmatrix& highlights,
               image& result) const;

    /**
     * Applies the given classifier to the area defined in the
     * parameters object. The image will contain the visualization
     * result. It also highlights the vectors given in highlights.
     * The color of each highlight is determined by the corresponding
     * value in labels.
     * @param cls supervisedInstanceClassifier with the classifier
     *            to be visualized. The classifier must be able to
     *            classify a 2D feature set.
     * @param highlights matrix with each row containing a vector
     *        that is to be highlighted.
     * @param result will receive the visualization result.
     * @param labels is a vector with the class labels for each row of
     *        highlights.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const supervisedInstanceClassifier& cls,
               const lti::dmatrix& highlights,
               image& result,
               const ivector& labels) const;

    /**
     * Applies the given classifier to the area defined in the
     * parameters object. The image will contain the visualization
     * result. It also highlights the vectors given in highlights.
     * The color of each highlight is determined by the corresponding
     * value in labels. You can also distinguish between two classes
     * of labels. The label class is determined by the vector htype.
     * @param cls supervisedInstanceClassifier with the classifier
     *            to be visualized. The classifier must be able to
     *            classify a 2D feature set.
     * @param highlights matrix with each row containing a vector
     *        that is to be highlighted.
     * @param result will receive the visualization result.
     * @param labels is a vector with the class labels for each row of
     *        highlights.
     * @param htype is a vector with a flag denoting the highlight
     *        type.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const supervisedInstanceClassifier& cls,
               const lti::dmatrix& highlights,
               image& result,
               const ivector& labels,
               const genericVector<bool>& htype) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    classifier2DVisualizer& copy(const classifier2DVisualizer& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    classifier2DVisualizer& operator=(const classifier2DVisualizer& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;
  };
}

#endif

