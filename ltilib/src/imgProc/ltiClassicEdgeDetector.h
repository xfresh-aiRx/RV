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
 * file .......: ltiClassicEdgeDetector.h
 * authors ....: Arnd Hannemann 
 * organization: LTI, RWTH Aachen
 * creation ...: 2003-11-27
 * 
 */

// include only once
#ifndef _LTI_SOBEL_EDGES_H_
#define _LTI_SOBEL_EDGES_H_

#include "ltiEdgeDetector.h"
#include "ltiNonMaximaSuppression.h"
#include "ltiThresholding.h"
#include "ltiConvolution.h"
#include "ltiGradientFunctor.h"


namespace lti {
  /**
  * This class implements simple standard edgedectors like Sobel.
  * It uses a gradient functor to create a gradient magnitude.
  * Furthermore it uses nonMaximaSuppression or thresholding 
  * to extract the edges.
  *
  * \code
  * #include ltiClassicEdgeDetector.h
  *
  * channel src;
  * channel8 edges;
  *
  * classicEdgeDetector::parameters cedPar;
  * // using thresholding instead of standard nonMaximaSuppression
  * thresholding::parameters tPar;
  * tPar.highThreshold = 0.8;
  * thresholding t(tPar);
  *
  * cedPar.setMaximaSearchMethod(t);
  * cedPar.gradientParameters.kernelType = gradientFunctor::parameters::Prewitt;
  *
  * // This has NO effect, because setMaximaSearchMethod was used
  * // so the thresholding functor was copied
  * tPar.highTreshold = 0.5;
  * t.setParameters(tPar);
  *
  * classicEdgeDetector ced(cedPar);
  *
  * ced.apply(src,edges);
  *
  * \endcode
  *
  * @ingroup gEdgeCorner
  */
  class classicEdgeDetector : public edgeDetector {
  public:

    /** 
     * The parameters for the class classicEdgeDetector.
     */
    class parameters : public edgeDetector::parameters {
    // there are protected attributes which are used by classicEdgeDetector
      friend class classicEdgeDetector;
    public:


      /**
       * enum type for maxima search method
       */
      enum eMaximaSearchMethod {
        NonMaximaSuppression, /**< use an instance of nonMaximaSuprression to extract edges */
        Thresholding /**< use an instance of thresholding to extract edges */
      };

      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       * @param other the parametrs object to copied
       */
      parameters(const parameters& other);

      /**
       * destructor
       */
      ~parameters();

      /** 
       * returns name of this type 
       * @return string "ltiClassicEdgeDetector::parameters"
       */
      const char* getTypeName() const;

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);


      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const;

      /**
       * Write the parameters in the given ioHandler.
       * \b Note: The parameters of the used maxima search method will 
       * \b NOT be saved!
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * Read the parameters from the given ioHandler
       * \b Note: The parameters of the used maxima search method will 
       * \b NOT be read!
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

#     ifdef _LTI_MSC_6
      /**
       * This function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * This function is required by MSVC only, as a workaround for a
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
       * Parameters of gradientFunctor which is used to compute the Gradients.
       *
       * Default value: parameters with KernelType = Sobel.
       */
      gradientFunctor::parameters gradientParameters;

      /** 
       * Sets a new maxima search method. A copy of the functor will be
       * done (so it is useless to change the parameters of the given
       * functor instance, because the internal functor will never notice
       * the changes done to its "parent").
       *
       * Attention:
       * some parameters of the functor will be overwritten 
       * in classicEdgeDetector::setParameters(const functor::parameters& theParam);
       */
      void setMaximaSearchMethod(const thresholding& thresholder);

      /** 
       * Sets a new maxima search method. A copy of the functor will be
       * done (so it is useless to change the parameters of the given 
       * functor instance, because the internal functor will never notice
       * the changes done to its "parent").
       *       
       * Attention:
       * some parameters of the functor will be overwritten 
       * in classicEdgeDetector::setParameters(const functor::parameters& theParam);
       */
      void setMaximaSearchMethod(const nonMaximaSuppression& suppressor);

      /**
       * Sets a new maxima search method. The functor which is passed here as
       * an argument must have been allocated with new; it must not be 
       * a local variable. On destruction of the parameters object, the
       * instance of the given functor will be deleted, i.e. this parameters
       * instance will be responsible for the memory managment of the object.
       *
       * \b Attention:
       * some parameters of the functor will be overwritten 
       * in classicEdgeDetector::setParameters();
       */
      void attachMaximaSearchMethod(thresholding* thresholder);

      /**
       * Sets a new maxima search method. The functor which is passed here as
       * an argument must have been allocated with new; it must not be 
       * a local variable. On destruction of the parameters object, the
       * instance of the given functor will be deleted, i.e. this parameters
       * instance will be responsible for the memory managment of the object.
       *
       * \b Attention:
       * some parameters of the functor will be overwritten 
       * in classicEdgeDetector::setParameters();
       */
      void attachMaximaSearchMethod(nonMaximaSuppression* suppressor);


      /**
       * Sets a new maxima search method. The functor which is passed here as
       * an argument is not deleted by the parameters object, the caller
       * must ensure that threre are no memory leaks.
       *   
       * \b Attention:
       * some parameters of the functor will be overwritten 
       * in classicEdgeDetector::setParameters();
       */
      void useExternalMaximaSearchMethod(thresholding* thresholder);

      /**
       * Sets a new maxima search method. The functor which is passed here as
       * an argument is not deleted by the parameters object, the caller
       * must ensure that threre are no memory leaks.
       *
       * \b Attention:
       * some parameters of the functor will be overwritten 
       * in classicEdgeDetector::setParameters();
       */
      void useExternalMaximaSearchMethod(nonMaximaSuppression* suppressor);

    protected:

      /** 
       * Method which is used to get the edges out of the gradient.
       * Default value: nonMaximaSuppression
       */
      eMaximaSearchMethod maximaSearchMethod;

      /**
       * non maxima suppression functor.
       * 
       * Default value: default nonMaximaSuppression
       */
      nonMaximaSuppression* suppressor;

      /**
       * thresholding functor
       *
       * Default value: 0
       */
      thresholding* thresholder;

      /**
       * Flag used to indicate if the local maxima search method functor
       * must be deleted by this object or not (just pointing to some
       * external instance...)
       */
      bool destroyFunctor;



    }; // parameters

    /** 
     * default constructor
     */
    classicEdgeDetector();

   /**
     * Construct a functor using the given parameters
     */
    classicEdgeDetector(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    classicEdgeDetector(const classicEdgeDetector& other);

    /**
     * Destructor
     */
    virtual ~classicEdgeDetector();


    /** 
     * returns the name of this type ("classicEdgeDetector")
     */
    virtual const char* getTypeName() const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    classicEdgeDetector& copy(const classicEdgeDetector& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    classicEdgeDetector& operator=(const classicEdgeDetector& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /** 
     * operates on the given %parameters
     * @param src channel with the Source data.
     * @param edges channel where the result will be left.
     * @return true if apply successful or false otherwise.   
     */

    virtual bool apply(const channel& src, channel8& edges) const;


    /**
     * operates on the given %parameters
     * @param srcdest channel with the source data. The result will be left 
     *                here too.
     * @return true if apply successful or false otherwise.   
     */
    virtual bool apply(channel8& srcdest) const;


    /**
     * update functor's parameters.
     * This member initializes some internal data depending on the 
     * parameter set.
     *
     * if using Thresholding as maxima search method:
     * - thresholding::parameters::inRegionValue will be overwritten
     *   by edgeDetector::parameters::edgeValue (divided by 255) 
     * - thresholding::parameters::outRegionValue will be overwritten
     *   by edgeDetector::parameters::noEdgeValue (divided by 255)
     * - thresholding::parameters::keepInRegion will always be set
     *   to false
     * - thresholding::parameters::keepOutRegion will always be set
     *   to false
     *
     * if using nonMaximaSuppression as maxima search method:
     * - nonMaximaSuppression::parameters.edgeValue will be
     *   overwritten by edgeDetector::parameters::edgeValue
     * - nonMaximaSuppression::parameters.background will be
     *   overwritten by edgeDetector::parameters::noEdgeValue
     * - nonMaximaSuppression::parameters.checkAngles will always 
     *   be set to false
     *
     * @param theParam the parameters which should be set
     * @return true if successful, false otherwise
     */
    virtual bool updateParameters(const parameters& theParam);


  protected:
    /** functor which is used to compute the gradient */
    gradientFunctor gradienter;
 
  };
}

#endif
