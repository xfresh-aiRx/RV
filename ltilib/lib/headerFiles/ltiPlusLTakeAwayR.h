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
 * file .......: ltiPlusLTakeAwayR.h
 * authors ....: Jens Paustenbach
 * organization: LTI, RWTH Aachen
 * creation ...: 8.7.2002
 * revisions ..: $Id: ltiPlusLTakeAwayR.h,v 1.7 2005/06/16 09:32:40 doerfler Exp $
 */

#ifndef _LTI_PLUS_L_TAKE_AWAY_R_H_
#define _LTI_PLUS_L_TAKE_AWAY_R_H_

#include "ltiFeatureSelector.h"

#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiCostFunction.h"

namespace lti {
  /**
   *  This class implements the plus l - take away r algorithm descripted in:
   *  P. Pudil, F.J. Ferri, J. Novovicova, J. Kittler. "Floating Search Methods
   *  for Feature Selection with nonmonotonic criterion functions" 
   *  Proceddings of the IEEE Intl. Conf. on Pattern Recognition, 
   *  279-283, 1994;
   *  In each iteration it adds the l best features and from the source data 
   *  to the destination data. Then the r least significant features are 
   *  deleted from this data set. This is done until the predetermined 
   *  number of features is reached
   *  As criterion function cross validation is used.
   */
  class plusLTakeAwayR : public featureSelector {
  public:
    /**
     * the parameters for the class plusLTakeAwayR
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
       * the number of features that are removed in each iteration
       */
      int r;

      /**
       * the number of features that are included in each iteration
       */
      int l;

      /** 
       * A pointer to the cost function that is used to evaluate the 
       * feature subset.
       */
      costFunction* usedCostFunction;

//       /**
//        * parameter object that is used if cross validation is used as
//        * cost function. In all other cases this parameter is not used.
//        */
//       crossValidator::parameters cVParameter;

//       /**
//        * the different choices for the costFunction
//        */
//       enum eCostFunctions {
//         BhattacharyyaDistance,
//         MahalanobisDistance,
//         CrossValidation
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
    plusLTakeAwayR();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    plusLTakeAwayR(const plusLTakeAwayR& other);

    /**
     * destructor
     */
    virtual ~plusLTakeAwayR();

    /**
     * returns the name of this type ("plusLTakeAwayR")
     */
    virtual const char* getTypeName() const;

    /**
     * extracts the k, that is a parameter, best features of src 
     * and copies them to dest
     * @param src dmatrix with the source data.
     * @param srcIds ivector with the ids corresponding to src
     * @param dest dmatrix where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const dmatrix& src,const ivector& srcIds, dmatrix& dest) const;
      
    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    plusLTakeAwayR& copy(const plusLTakeAwayR& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    plusLTakeAwayR& operator=(const plusLTakeAwayR& other);

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



