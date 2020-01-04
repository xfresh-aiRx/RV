/*----------------------------------------------------------------
 * project ....: LTI Digital Image/Signal Processing Library
 * file .......: ltiPdmGenerator.h
 * authors ....: Benjamin Winkler
 * organization: LTI, RWTH Aachen
 * creation ...: 26.4.2001
 * revisions ..: $Id: ltiPdmGenerator.h,v 1.10 2005/06/16 12:35:43 doerfler Exp $
 */

#ifndef _LTI_PDM_GENERATOR_H_
#define _LTI_PDM_GENERATOR_H_


#include "ltiActiveShapeModel.h"
#include "ltiFunctor.h"
#include "ltiEigenSystem.h"
#include "ltiSTLIoInterface.h"


namespace lti {
  /**
   * This class creates a pointDistributionModel (PDM) given a set of shapes
   * of type pointDistributionModel::shape.
   *
   * All shapes must have an equal number of points. These are ordered and
   * should ideally have a fixed correspondence across different shapes.
   *
   *
   */
  class pdmGenerator : public functor {
  public:
    /**
     * the parameters for the class pdmGenerator
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
       * @return true if write was succeful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was succeful
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
       * This %parameter is an upper limit to the number of eigenvectors to consider.
       *
       * Eigenvectors are ordered by magnitude of eigenvalue, so that the ones
       * with high eigenvalues are chosen first. All others are discarded.
       *
       * Only the first 2*maxNumberOfWeights eigenVectors are taken into account
       * from a total of 2*N, where N is the number of points in one shape.
       * Reasonable values lie between 0 and N.
       *
       * If set to 0 (default), all eigenVectors will be used.
       */
      int maxNumberOfWeights;

      /**
       * This %parameter is a lower limit to the number of eigenvectors to consider.
       *
       * At least the first K eigenVectors are taken into account, for which holds:
       * (eigenValue[0] + ... + eigenValue[K-1]) / sum(eigenValue) >= maxRelativeWeights
       * reasonable values range from 0.0 (none) to 1.0 (all).
       */
      float maxRelativeWeights;

    };


    /**
     * default constructor
     */
    pdmGenerator();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    pdmGenerator(const pdmGenerator& other);

    /**
     * destructor
     */
    virtual ~pdmGenerator();

    /**
     * returns the name of this type ("pdmGenerator")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    pdmGenerator& copy(const pdmGenerator& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /** @name Training shape set
     *  These members are for managing the training shape set.
     */
      //@{

      /**
       * clear all considered shapes
       */
      void clear();

      /**
       * take shape into consideration
       */
      void consider(const pointDistributionModel::shape &theShape);

      /**
       * take list of shapes into consideration
       */
      void consider(const std::list<pointDistributionModel::shape> &theShapes);

      /**
       * return number of shapes considered so far (not the number of points!).
       */
      int getN() const;

      //@}

    /**
     * generate pointDistributionModel for the given dimension
     * @return a pointDistributionModel
     */
    pointDistributionModel apply();


  private:

    /**
     * generate model
     */
    void generate(pointDistributionModel &theModel);

    /**
     * calculate center
     */
//    tpoint<float> calculateShapeCenter(const pointDistributionModel::shape &theShape) const;

    /**
     * calculate size
     */
//    float calculateShapeSize(const pointDistributionModel::shape &theShape) const;

    /**
     * normalize shape (i.e. move to (0,0), scale to 1.0)
     */
    void normalizeShape(pointDistributionModel::shape &theShape) const;

    /**
     * - align all shapes to cog=(0,0), meanDistance=1.0
     * - create mean shape
     * - align all shapes iteratively to the mean shape
     * @return the mean shape
     */
    pointDistributionModel::shape alignShapes();

    /**
     * create mean shape
     */
    pointDistributionModel::shape createMeanShape() const;

    /**
     * sum of outerproducts of shape-vectors divided by number of shapes
     */
    matrix<double> calculateCovarianceMatrix(const pointDistributionModel::shape &meanShape) const;

    /**
     * create eigenmatrix and vector of eigenvalues
     */
    void createEigenSystem(const matrix<double> &covarMatrix,
                           matrix<double> &eigenVectorMatrix,
                           vector<double> &eigenValues) const;

    /**
     * create shape vector (x1, y1, x2, y2, ...)
     */
    vector<double> buildVector(const pointDistributionModel::shape &src) const;


    /**
     * list of considered shapes
     */
    std::list<pointDistributionModel::shape> shapes;

    /**
     * needed to align the given shapes
     */
    activeShapeModel theASM;

    /**
     * shape size
     */
    int shapeSize;
  };
}

#endif

