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
 * file .......: ltiSOFM.h
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 12.8.2002
 * revisions ..: $Id: ltiSOFM.h,v 1.10 2005/06/16 09:32:40 doerfler Exp $
 */

#ifndef _LTI_S_O_F_M_H_
#define _LTI_S_O_F_M_H_

//TODO: include only those files which are needed in this header!!

// TODO: Check this include to parent class:
#include "ltiUnsupervisedClassifier.h"

namespace lti {
  /**
   * This is the base class for all Self Organizing Feature Maps. They
   * are modelled after Kohonen's book 'Self-Organizing Maps'
   * published by Springer in 1995.
   * <p> SOFMs have a two phase training: In the first phase the
   * ordering of the neurons is established. The neighborhood should
   * be rather large during this phase. In the second phase the
   * neurons are more finely tuned to the underlying probability
   * distributions with a small neighborhood. This phase is called
   * convergence phase.

   *
   */
  class SOFM : public unsupervisedClassifier {
  public:
    /**
     * the parameters for the class SOFM
     */
    class parameters : public unsupervisedClassifier::parameters {

    public:

      /**
       * Possible types of initialization for the grid of neurons:<p>
       * <dl>
       * <dt>Random</dt>
       * <dd>Grid points are selected randomly from the data.</dd>
       * <dt>Linear</dt>
       * <dd>The mean as well as the two maximum eigenvalues and
       * corresponding eigenvectors of the data are calculted. Then
       * the neurons in the grid are uniformly spread over the
       * rectangle spanned by the eigenvectors and of the dimensions
       * of the eigenvalues. Like this the grid is already ordered and
       * neurons should be close to their final destination. Here, it
       * is useful to set the parameter calculateSize to true </dd>
       * </dl>
       */
      enum eInit {
        Random = 1, /*!< choose initial values randomly from data */
        Linear = 2  /*!< take max eigenvalues and mean and distribute points */
      };

      /**
       * Different metrics can be used to determine the distance
       * between an input vector and each neuron.
       */
      enum eMetric {
        L1 = 1, /*!< use L1 norm */
        L2, /*!< use L2 norm */
        Dot /*!< use dot product */
      };

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

      //TODO: comment the parameters of your classifier
      // If you add more parameters manually, do not forget to do following:
      // 1. indicate in the default constructor the default values
      // 2. make sure that the copy member also copy your new parameters
      // 3. make sure that the read and write members also read and
      //    write your parameters


      /**
       * type of initialization method used. See eInit.
       */
      eInit initType;

      /**
       * type of metric used. See eMetric.
       */
      eMetric metricType;

      /**
       * Number of epochs used for ordering in the training
       */
      int stepsOrdering;

      /**
       * Maximum value of learn-rate during ordering
       */
      double orderLearnMax;

      /**
       * Minimum value of learn-rate during ordering
       */
      double orderLearnMin;

      /**
       * Maximum distance to the winner neuron during ordering. The
       * unit is in grid steps. E.g. a value of 1 would include
       * (theoretically) the 8 neighbors, since a square window is
       * assumed. You can change this by setting orderNeighborThresh
       * to an appropriate value.
       */
      double orderNeighborMax;

      /**
       * Minimum distance to the winner neuron during ordering. The
       * neighborhood stays like this for the convergence phase. This
       * value should be rather low, typically 1. For values below 1
       * no neighborhood is created which will not work!
       */
      double orderNeighborMin;

      /**
       * The minimum value the neighborhood function assumes (cut-off
       * value). This should be rather low. Since a Gaussian kernel is
       * used this value steers the size of the kernel to be
       * used. I.e. the bigger the value, the faster the training and
       * the less precise. E.g. if for the current neighborhood size
       * the Gaussian value is smaller than the threshold for the roof
       * of the neighborhood value, the kernel size will be lower than
       * the roof of the neighborhood size.
       */
      double orderNeighborThresh;



      /**
       * Number of epochs used for convergence in the training
       */
      int stepsConvergence;

      /**
       * Maximum value of learn-rate during convergence
       */
      double convergenceLearnMax;

      /**
       * Minimum value of learn-rate during convergence
       */
      double convergenceLearnMin;


    };

    /**
     * default constructor
     */
    SOFM();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    SOFM(const SOFM& other);

    /**
     * destructor
     */
    virtual ~SOFM();

    /**
     * returns the name of this type ("SOFM")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" classifier.
     * @param other the classifier to be copied
     * @return a reference to this classifier object
     */
    SOFM& copy(const SOFM& other);

    /**
     * alias for copy member
     * @param other the classifier to be copied
     * @return a reference to this classifier object
     */
    SOFM& operator=(const SOFM& other);

    /**
     * returns a pointer to a clone of this classifier.
     */
    virtual classifier* clone() const=0;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Unsupervised training.  The vectors in the <code>input</code>
     * matrix will be put into groups according to the training
     * algorithm.  Additionally, an integer indicating the class each
     * point belongs to is returned.
     *
     * By default this method uses the other train method train(const dmatrix&)
     * and then calls classify(const dvector&) to get the ids for each
     * trainvector. These ids are then returned.
     *
     * @param input the matrix with the input vectors (each row is a training
     *              vector)
     * @param ids vector of class ids for each input point
     * @return true if successful, false otherwise. (if false you can check
     *              the error message with getStatusString())
     */
    virtual bool train(const dmatrix& input,
                       ivector& ids);

    /**
     * Unsupervised training.
     * The row vectors in the <code>input</code> matrix
     * are used for training of the classifier.
     * @param input the matrix with the input vectors (each row is a training
     *              vector)
     * @return true if successful, false otherwise. (if false you can check
     *              the error message with getStatusString())
     */
    virtual bool train(const dmatrix& input)=0;

    //TODO Check whether you really need a new classify method.
    // In some cases the superclasses method will suffice. Then just
    // delete the declaration and its implementation stump.

    /**
     * Classification.
     * Classifies the feature and returns the outputVector with
     * the classification result.
     * @param feature the %vector to be classified
     * @param result the result of the classification
     * @return false if an error occurred during classification else true
     */
    virtual bool
    classify(const dvector& feature, outputVector& result) const=0;

    /**
     * write the SOFM in the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool write(ioHandler& handler,const bool complete=true) const;

    /**
     * read the SOFM from the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool read(ioHandler& handler,const bool complete=true);

    /**
     * returns the prototypes of the SOFM. Each row of the matrix
     * contains one prototype. Depending on the dimensionality these
     * are ordered in dimensions. For two dimensions it is Y,X giving
     * first the prototype at (0,0) than at (0,1) and so on. In three
     * dimensions Z has the highest order (Z,Y,X):
     * (0,0,0),(0,0,1),...,(0,0,n),(0,1,0),...
     */
    inline const dmatrix& getPrototypes() const {return grid;};

  protected:

    /** weights for each neuron in the grid */
    dmatrix grid;

    /** learn-rate for first phase */
    double lrOrder;
    /** learn-rate delta for first phase */
    double lrOrderDelta;
    /** current sigma for neighborhood */
    double sigma;
    /** delta for sigma */
    double sigmaDelta;
    /** parameter A for lr = A / (t+B) during convergence */
    double lrConvergeA;
    /** parameter B for lr = A / (t+B) during convergence */
    double lrConvergeB;


    /** calculate learn-rates and deltas from the parameters. */
    virtual bool setLearnRates(const int& nbData);

  };
}

#endif

