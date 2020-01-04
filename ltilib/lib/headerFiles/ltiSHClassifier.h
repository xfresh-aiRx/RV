/*
 * Copyright (C) 1998, 1999, 2000, 2001, 2002
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
 * file .......: ltiSHClassifier.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 30.10.2001
 * revisions ..: $Id: ltiSHClassifier.h,v 1.9 2005/06/16 09:32:40 doerfler Exp $
 */

#ifndef _LTI_SHCLASSIFIER_H_
#define _LTI_SHCLASSIFIER_H_

#include "ltiSupervisedInstanceClassifier.h"
#include "ltiSparseHistogram.h"

namespace lti {
  /**
   * Implements a sparse histogram-based classifier, such as the one
   * described in
   * B. Schiele, J. L. Crowley: Recognition without Correspondence
   * using Multidimensional Receptive Field Histograms.
   */
  class shClassifier : public supervisedInstanceClassifier {
  public:

    /**
     * the parameters for the class shClassifier
     */
    class parameters : public supervisedInstanceClassifier::parameters {
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
      virtual ~parameters();

      /**
       * returns name of this type.
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
      virtual classifier::parameters* clone() const;

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
       * The number of bins per axis in the classifier.
       */
      int numberOfBins;

      /**
       * The number of bins per axis in the classifier. The classifier
       * checks in training, if the size of this vector is greater
       * than zero. Otherwise, numberOfBins is used.
       */
      ivector binVector;

      /**
       * The minimum element to be classified.
       */
      dvector minimum;

      /**
       * The maximum element to be classified.
       */
      dvector maximum;

      /**
       * Should bounds be determined automatically?
       */
      bool autoBounds;

    };

    /**
     * default constructor
     */
    shClassifier();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    shClassifier(const shClassifier& other);

    /**
     * destructor
     */
    virtual ~shClassifier();

    /**
     * returns the name of this type ("shClassifier")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" clustering.
     * @param other the clustering to be copied
     * @return a reference to this clustering object
     */
    shClassifier& copy(const shClassifier& other);

    /**
     * alias for copy member
     * @param other the clustering to be copied
     * @return a reference to this clustering object
     */
    shClassifier& operator=(const shClassifier& other);

    /**
     * returns a pointer to a clone of this clustering.
     */
    virtual classifier* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Supervised training.  The vectors in the <code>input</code> matrix
     * are arranged row-wise, i.e. each row contains one data vector.
     * The <code>ids</code> vector contains the class label for each row.
     * This functor implements the
     * <a href="http://www.research.microsoft.com/~jplatt/smo.html">SMO algorithm</a> by
     * <a href="http://www.research.microsoft.com/~jplatt">J.C. Platt</a>.
     * All classes are modeled using the same kernel function, given
     * in the parameters object.
     *
     * @param input the matrix with the input vectors (each row is a training
     *              vector)
     * @param ids vector of class ids for each input point
     * @return true if successful, false otherwise. (if false you can check
     *              the error message with getStatusString())
     */
    virtual bool train(const dmatrix& input,
                       const ivector& ids);

    /**
     * Adds an object to this classifier. The id is given automatically
     * and returned in the parameter.
     */
    virtual bool trainObject(const dmatrix& input, int& id);


    //TODO Check whether you really need a new classify method.
    // In some cases the superclasses method will suffice. Then just
    // delete the declaration and its implementation stump.

    /*
     * Classification.
     * Classifies the feature and returns the output %object with
     * the classification result.
     * @param feature pattern to be classified
     * @return result of the classifications as a classifier::output
     */
/*      virtual const output& classify(const dvector& feature); */


    /**
     * Classification.
     * Classifies the feature and returns the outputVector %object with
     * the classification result.
     * <p><b>NOTE:</b> This method is NOT really const. Although the main
     * members of the shClassifier are not changed some state variables used for
     * efficiency are. Thus, it is not save to use the same instance of the
     * shClassifier in two different threads.
     * @param feature pattern to be classified
     * @param result of the classifications as a classifier::outputVector
     * @return true if the classification has been successful
     */
    virtual bool classify(const dvector& feature, outputVector& result) const;

    //TODO: comment the attributes of your clustering
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.


    /**
     * write the classifier in the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool write(ioHandler& handler,const bool complete=true) const;

    /**
     * read the classifier from the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool read(ioHandler& handler,const bool complete=true);

  protected:
    void buildIdMaps(const ivector& ids);
    void defineOutputTemplate();

    // map from external id to internal id, used for training
    std::map<int,int> idMap;
    // map from internal to external id, used for training
    std::map<int,int> rIdMap;

    int nClasses;

    std::vector<sparseHistogram*> models;
  };

}

#endif

