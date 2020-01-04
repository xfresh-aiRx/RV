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
 * file .......: ltiColorModelSelector.h
 * authors ....: Axel Berner, Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 15.2.2002
 * revisions ..: $Id: ltiColorModelSelector.h,v 1.12 2005/06/16 09:32:41 doerfler Exp $
 */

#ifndef _LTI_COLOR_MODEL_SELECTOR_H_
#define _LTI_COLOR_MODEL_SELECTOR_H_

#include "ltiTransform.h"
#include "ltiHistogram.h"
#include "ltiProbabilityMap.h"
#include "ltiPCA.h"
#include <list>

namespace lti {
  /**
   * This functor selects one color model from a set, which elements
   * were given by addColorModel.  The selected model (a
   * lti::probabilityMap functor) will be the one which maximizes the
   * likelihood in the given image window, i.e. the model which achieves the
   * maximal average probability in the given window.
   *
   * The window size and position can be defined once in the parameters, or
   * can be be given directly to the required apply methods.
   *
   * It is also possible to get the lti::principalComponents %object that
   * will zoom into the model colors.  This is mainly used to zoom into
   * skin colors,  to allow the segmentation between skin-similar and skin
   * colors.  The computation of the PCA will be delayed until the
   * first apply requires it, or when the user explicitelly calls
   * initPCA.  This is done this way to avoid waiting times in those
   * cases where the color zoom is not required.
   */
  class colorModelSelector : public transform {
  public:
    /**
     * the parameters for the class colorModelSelector
     */
    class parameters : public transform::parameters {
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

      /**
       * rectangle that defines the window in the given image
       * for which the color probability is calculated
       * and finally the colorModel is selected by.
       * Some apply methods accept this parameter directly.  In these cases,
       * the value of <code>window</code> will be ignored.
       *
       * Default value: (0,0,10000,10000)
       */
      rectangle window;

      /**
       * filename of the default color model (with path).
       * @see lti::colorModelSelector::addColorModel
       *
       * Default value: "../skin/skin-32-32-32.hist"
       */
      std::string colorHistFile;

      /**
       * filename of the default non-color histogram (with path)
       * @see lti::colorModelSelector::addColorModel
       *
       * Default value: "../skin/nonskin-32-32-32.hist"
       */
      std::string nonColorHistFile;

      /**
       * default %parameters for the probabilityMap functors.  This will be
       * taken into account only for the models added after setting the
       * parameters object.
       *
       * Default values: see probabilityMap::parameters
       */
      probabilityMap::parameters mapParameters;

      /**
       * \b Deprecated
       *
       * To compute the PCA used for the color zooming, a specific
       * number of data points is required, generated from the color
       * model used.  The number of data points used can be specified here.
       *
       * This parameter is not required any more and will be ignore.
       * The covariance and mean values for the PCA functor are
       * computed directly from the histogram.
       *
       * Default value 500000
       */
      int pcaDataPoints;
    };

    /**
     * default constructor
     */
    colorModelSelector();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    colorModelSelector(const colorModelSelector& other);

    /**
     * destructor
     */
    virtual ~colorModelSelector();

    /**
     * returns the name of this type ("colorModelSelector")
     */
    virtual const char* getTypeName() const;

    /**
     * copy into colorMap and pca the internal functors for the color models
     * that maximize the likelihood for the image window specified in the
     * parameters %object.
     *
     * @param img image with the source data.
     * @param colorMap the internal model which maximizes the color
     *                probability for the given image window will be
     *                copied here.
     * @param pca functor with the whitening transform used to zoom
     *                the color model into the whole RGB color space.
     * @return true if apply successful or false otherwise.
     *
     * Note that this member will copy the models and pca transformation
     * functors, and this takes some time.  If you can ensure that
     * this object will exist while using the models and pca
     * functors, you better get just a pointer to the internal objects.
     */
    bool apply(const image& img,
	       probabilityMap& colorMap,
	       principalComponents<float>& pca);

    /**
     * get a pointer into the internal stored model and transformation
     * functors that maximize the likelihood for the image window
     * specified in the parameters %object.  Note that if this object
     * is destroyed, the pointers returned will be invalid.
     *
     * @param img image with the source data.
     * @param pColorMap pointer to the internal model which maximizes the color
     *                probability for the given image window.
     * @param pPca pointer to the internal PCA functor (whitening transform)
     *                computed from the colorMap to be used in color zooming.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& img,
	       const probabilityMap* &pColorMap,
	       const principalComponents<float>* &pPca);

    /**
     * copy into colorMap and pca the internal functors for the color models
     * that maximize the likelihood for the image window specified in the
     * %parameters.
     *
     * @param img image with the source data.
     * @param window the region of the image to be considered.
     * @param colorMap the internal model which maximizes the color
     *                probability for the given image window will be
     *                copied here.
     * @param pca functor with the whitening transform used to zoom
     *                the color model into the whole RGB color space.
     * @return true if apply successful or false otherwise.
     *
     * Note that this member will copy the models and pca transformation
     * functors, and this takes some time.  If you can ensure that
     * this object will exist while using the models and pca
     * functors, you better get just a pointer to the internal objects.
     */
    bool apply(const image& img,
               const rectangle& window,
	       probabilityMap& colorMap,
	       principalComponents<float>& pca);

    /**
     * get a pointer to the internal stored model and transformation
     * functors that maximize the likelihood for the image window
     * specified in the %parameters.  Note that if this object
     * is destroyed, the pointers returned will be invalid.
     *
     * @param img image with the source data.
     * @param window the region of the image to be considered.
     * @param pColorMap pointer to the internal model which maximizes the color
     *                probability for the given image window.
     * @param pPca pointer to the internal PCA functor (whitening transform)
     *                computed from the colorMap to be used in color zooming.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& img,
               const rectangle& window,
	       const probabilityMap* &pColorMap,
	       const principalComponents<float>* &pPca);

    /**
     * copy into colorMap the internal functor for the color model
     * that maximizes the likelihood for the image window specified in the
     * parameters %object.
     *
     * @param img image with the source data.
     * @param colorMap the internal model which maximizes the color
     *                probability for the given image window will be
     *                copied here.
     * @return true if apply successful or false otherwise.
     *
     * Note that this member will copy the color model functor, and
     * this takes some time.  If you can ensure that this object will
     * exist while using the model functor, you better get
     * just a pointer to the internal objects.
     */
    bool apply(const image& img,
               probabilityMap& colorMap) const;

    /**
     * get a pointer to the internal stored color model functor that
     * maximizes the likelihood for the image window specified in the
     * parameters %object.  Note that if this object is destroyed, the
     * pointers returned will be invalid.
     *
     * @param img image with the source data.
     * @param pColorMap pointer to the internal model which maximizes the color
     *                probability for the given image window.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& img,
               const probabilityMap* &pColorMap) const;

    /**
     * get a pointer to the internal stored color model functor that
     * maximizes the likelihood for the image window specified in the
     * parameters %object.  Note that if this object is destroyed, the
     * pointers returned will be invalid.
     *
     * @param img image with the source data.
     * @param pColorMap pointer to the internal model which maximizes the color
     *                probability for the given image window.
     * @param modelName the name of the model used
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& img,
               const probabilityMap* &pColorMap,
               std::string& modelName) const;

    /**
     * Get a pointer to the internal stored model and transformation functors
     * that maximize the likelihood for the image window specified in the
     * arguments.  Note that if this object is destroyed, the pointers
     * returned will be invalid.
     *
     * @param img image with the source data.
     * @param window the region of the image to be considered.
     * @param pColorMap pointer to the internal model which maximizes the color
     *                probability for the given image window.
     * @param pPca pointer to the internal PCA functor (whitening transform)
     *                computed from the colorMap to be used in color zooming.
     * @param modelName the name of the model used
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& img,
               const rectangle& window,
	       const probabilityMap* &pColorMap,
	       const principalComponents<float>* &pPca,
               std::string& modelName);

    /**
     * Copy into colorMap the internal functor for the color model
     * that maximizes the likelihood for the image window specified in the
     * parameters %object.
     *
     * @param img image with the source data.
     * @param window the region of the image to be considered.
     * @param colorMap the internal model which maximizes the color
     *                probability for the given image window will be
     *                copied here.
     * @return true if apply successful or false otherwise.
     *
     * Note that this member will copy the color model functor, and
     * this takes some time.  If you can ensure that this object will
     * exist while using the model functor, you better get
     * just a pointer to the internal objects.
     */
    bool apply(const image& img,
               const rectangle& window,
               probabilityMap& colorMap) const;

    /**
     * get a pointer to the internal stored color model functor that
     * maximizes the likelihood for the image window specified in the
     * parameters %object.  Note that if this object is destroyed, the
     * pointers returned will be invalid.
     *
     * @param img image with the source data.
     * @param window the region of the image to be considered.
     * @param pColorMap pointer to the internal model which maximizes the color
     *                probability for the given image window.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& img,
               const rectangle& window,
               const probabilityMap* &pColorMap) const;

    /**
     * get a pointer to the internal stored color model functor that
     * maximizes the likelihood for the image window specified in the
     * parameters %object.  Note that if this object is destroyed, the
     * pointers returned will be invalid.
     *
     * @param img image with the source data.
     * @param window the region of the image to be considered.
     * @param pColorMap pointer to the internal model which maximizes the color
     *                probability for the given image window.
     * @param modelName name of the model used
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& img,
               const rectangle& window,
               const probabilityMap* &pColorMap,
               std::string& modelName) const;

    /**
     * add a further color model to a intern list.
     *
     * The model is assumed to be stored in a file of a
     * thistogram<double> written with a lispStreamHandler.
     *
     * If you want to write the histograms by yourself do something like:
     * \code
     * thistogram<double> myHist;  // the histogram to be stored in a file
     *
     * std::ofstream out("myFile.hist"); // the file stream
     * lti::lispStreamHandler lsh(out);  // the lisp stream handler
     * myHist.write(lsh);                // write to the lisp stream handler
     * out.close();
     *
     * \endcode
     *
     * Please note that you can also add all histogram models directly, and
     * at the end you can write this functor with all its models (and pca) to
     * be readed later.  This can save lots of time, specially, because
     * the pca do not need to be computed again.
     *
     * @param name the name for this model
     * @param colorHistFileName complete file name (path+filename) for the
     *        thistogram<double> file with the color model
     * @param nonColorHistFileName complete file name (path+filename) for the
     *        thistogram<double> file with the non-color model.
     */
    bool addColorModel(const std::string& name,
                       const std::string& colorHistFileName,
                       const std::string& nonColorHistFileName);

    /**
     * add a further color model to a intern list
     *
     * @param name the name for this model
     * @param colorHist thistogram<double> with the color model
     * @param nonColorHist thistogram<double> with the non-color model.
     */
    bool addColorModel(const std::string& name,
                       const thistogram<double>& colorHist,
                       const thistogram<double>& nonColorHist);

    /**
     * add a further color model to a intern list
     *
     * @param name the name for this model
     * @param colorMap a probabilityMap functor with the color and non-color
     *                 models.
     */
    bool addColorModel(const std::string& name,
                       const probabilityMap& colorMap);

    /**
     * Update parameters for all models.
     *
     * The color models are considered here as state, and not as part of the 
     * probabilityMap::parameters.  I.e. all attributes except the histograms
     * themselves will be updated in each of the availabe functors.
     */
    bool updateModels(const probabilityMap::parameters& newAttribs);

    /**
     * call if you want to precompute the PCA (Whitening
     * Transformation) for the color models, before calling the
     * corresponding apply methods.  Otherwise, the pca will be
     * initialized on-the-fly (when required).
     */
    bool initPCA();

    /**
     * remove all color models
     */
    bool clearColorModels();

    /**
     * remove only the color models with the given name.
     */
    bool removeColorModel(const std::string& name);

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    colorModelSelector& copy(const colorModelSelector& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    colorModelSelector& operator=(const colorModelSelector& other);

    /**
     * read the colorModelSelector from the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    bool read(ioHandler& handler,const bool complete=true);

    /**
     * write the colorModelSelector in the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    bool write(ioHandler& handler,const bool complete=true) const;

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
     * Internal data type to facilitate the administration of
     * named color models.
     */
    class namedModel {
    public:
      /**
       * a free defineable name for a model
       */
      std::string name;

      /**
       * default contructor
       */
      namedModel();

      /**
       * read the namedModel from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      bool read (ioHandler& handler,const bool complete = true);

      /**
       * write the namedModel in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      bool write(ioHandler& handler,const bool complete = true) const;

      /**
       * copy the contents of a namedModel object
       * @param other the namedModel object to be copied
       * @return a reference to this parameters object
       */
      namedModel& copy(const namedModel& other);

      /**
       * copy the contents of a namedModel object
       * @param other the namedModel object to be copied
       * @return a reference to this parameters object
       */
      namedModel& operator=(const namedModel& other);

      /**
       * set the color map to be used.  The pca will be marked as uninitialized
       */
      void setColorMap(const probabilityMap& colMap);

      /**
       * get color map being used
       */
      const probabilityMap& getColorMap() const;

      /**
       * get color map being used
       */
      probabilityMap& getColorMap();

      /**
       * get color map being used
       */
      principalComponents<float>& getPCA();

      /**
       * returns true if the pca was already initialized
       */
      bool pcaInitialized() const;

      /**
       * compute the pca considering the histogram model given in the
       * colorMap
       * @return true if successful, false otherwise
       */
      bool computePCA();

    protected:
      /**
       * if true, the pca was already computed from the color model.
       * if false, it still needs to be computed
       */
      bool pcaComputed;

      /**
       * principalComponents class of the model
       */
      lti::principalComponents<float> pca;

      /**
       * probabilityMap class of the model
       */
      lti::probabilityMap colorMap;
    };

    /**
     * list with all color models
     */
    std::list<namedModel> modelList;
  };

}

#endif

