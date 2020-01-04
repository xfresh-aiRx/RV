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
 * file .......: ltiSffs.h
 * authors ....: Jens Paustenbach
 * organization: LTI, RWTH Aachen
 * creation ...: 11.7.2002
 * revisions ..: $Id: ltiSffs.h,v 1.7 2005/06/16 09:32:40 doerfler Exp $
 */

#ifndef _LTI_SFFS_H_
#define _LTI_SFFS_H_


#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiFeatureSelector.h"
#include "ltiCostFunction.h"
// #include "ltiSupervisedInstanceClassifier.h"

namespace lti {
  /**
   *  Implemantation of the sequential floating forward search algorithm to
   *  select the best features from a data set. 
   *  This algorithm is implented from: P. Pudil, F.J. Ferri, J. Novovicova, 
   *  J. Kittler: "Floating Search Methods for Feature Selection with 
   *  nonmonotonic criterion Functions" Procedings of the IEEE Intl. Conf.
   *  on Pattern Recognition, 279-283, 1994;
   *  The original SFFS-Algorithm is discriped in P.Pudil,J.Novovicova,Kittler
   *  "Floating search methods in feature selection" Pattern Recogniton 
   *  Letters 15, pages 1119-1125
   */
  class sffs : public featureSelector {
  public:
    /**
     * the parameters for the class sffs
     */
    class parameters : public featureSelector::parameters {
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
       * the cost function that is used to decide which features are the best
       * in the context of the cost function 
       */
      costFunction* usedCostFunction;

//       /**
//        * the classifier used in the cross validator
//        */
//       supervisedInstanceClassifier* classifier;

//       enum eCostFunctions {
//         bhattacharyyaDistance,
//         mahalanobisDistance,
//         crossValidation
//       };

//       /** 
//        * The cost function that is used to decide if a feature is better than 
//        * an other feature.
//        */
//       eCostFunctions costFunction;


    };

    /**
     * default constructor
     */
    sffs();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    sffs(const sffs& other);

    /**
     * destructor
     */
    virtual ~sffs();

    /**
     * returns the name of this type ("sffs")
     */
    virtual const char* getTypeName() const;
  
    //TODO: comment your apply methods!
    
//      /**
//       * operates on the given %parameter.
//       * @param srcdest dmatrix with the source data.  The result
//       *                 will be left here too.
//       * @return true if apply successful or false otherwise.
//       */
//      bool apply(dmatrix& srcdest) const;

//     /**
//      * operates on a copy of the given %parameters.
//      * @param src dmatrix with the source data.
//      * @param dest dmatrix where the result will be left.
//      * @return true if apply successful or false otherwise.
//      */
//     bool apply(const dmatrix& src,const ivector& srcIds, dmatrix& dest) const;

    /**
     * extracts the most significant features from a source data set
     * @param src the src data
     * @param srcIds the cluster ids corresponding to the data points in src
     * @param dest the extracted features
     * @return true if apply successful or false otherwise.
     */
    bool apply(const dmatrix& src,const ivector& srcIds,dmatrix& dest) const;
      
    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    sffs& copy(const sffs& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    sffs& operator=(const sffs& other);

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



