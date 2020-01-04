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
 * file .......: ltiChiSquareFunctor.h
 * authors ....: Birgit Gehrke
 * organization: LTI, RWTH Aachen
 * creation ...: 3.6.2002
 * revisions ..: $Id: ltiChiSquareFunctor.h,v 1.11 2005/06/16 12:37:34 doerfler Exp $
 */

#ifndef _LTI_CHI_SQUARE_FUNCTOR_H_
#define _LTI_CHI_SQUARE_FUNCTOR_H_


// Check this include to parent class:
#include <ltiFunctor.h>
#include "ltiVector.h"
#include <list>
#include <vector>
#include <string>
#include "ltiSerialStatsFunctor.h"

namespace lti {

  /**
   *  chiSquareFunctor calculates the chiSquareValue indicating the
   *  likelihood, that the considerd elements are drawn from a
   *  gaussian normal distribution.
   *
   *  All results are written to the file chiSquareInfo.txt (the use may
   *  specify another name for this file)
   *
   *  the default parameter values are optimal for integer data
   *  Mean and variance of the distribution are drawn from the data
   *  if the user has not specified other values.
   *
   *  For a similar algorithm of the Chi-Square Test and numerical
   *  integration see "Numerical Recipes in C (Cambridge University
   *  Press)"
   *
   *  example:
   *  \code
   *
   *   double data[] = {2,5,6,7.5,8,9,7,6,5,4,3};
   *   lti::vector<double> vct(11,data);
   *
   *   double chiSquare;
   *   lti::chiSquareFunctor csf;
   *
   *   lti::chiSquareFunctor::parameters param;  // set a parameter
   *   param.minimalWidth = 0.5;
   *   csf.setParameters(param);
   *
   *   csf.consider(vct);
   *   csf.apply(chiSquare);
   *
   *   std::cout << "chiSquare: " << chiSquare << std::endl;
   *   std::cout << "numberOfIntervalls: "
   *             << csf.getNumberOfIntervalls() << std::endl;
   *   std::cout << csf.getStatusString() << std::endl;// gives information
   *                                                   // when an error ocurred
   *
   *  \endcode
   */
  class chiSquareFunctor : public functor {
  public:
    /**
     * the parameters for the class chiSquareFunctor
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
      * the accuracy for the integration of the gauss function.
      * small values lead to a good approximation
      *
      * default is 1.0e-5, reasonable values are 1.0e-10...infinite
      * if the user sets a value smaller than 1.0e-10, accuracy is set to 1.0e-10
      * small values cause very long calculations
      */
     double accuracy;

     /**
      * must allways be "true" if you have discrete data,
      * otherwise the Chi-Square Test won't work properly.
      *
      * default is true, because it does no harm except that it
      * constrains the intervall size to be multiple of minimum data distance
      */
     bool discrete;

      /**
      * true means that all intervalls will have the same size
      *
      * equidistant is not good for data with small and large distances
      * between the elements, because there may be intervalls which contain
      * no elements
      *
      * default is false
      */
     bool equidistant;

     /**
      * data is splitted in maxNumofIntervalls, than the number of intervalls
      * is reduced untill constraints are met
      *
      * maxNumberOfIntervalls must not be smaller than 3
      * default is 100;
      */
     int maxNumOfIntervalls;

     /**
      * the maximum number of steps for the integration;
      * integration is stopped after maxSteps even if accuracy is not reached
      *
      * default is 10, reasonable values are 5...15
      *
      * if the user sets a value smaller than 5, maxSteps is set to 5
      * if the user sets a vlaue larger than 15, maxSteps is set to 15
      *
      * be careful with large values, because it may take very long time
      * as long as accuracy is not reached, the integration intervall is
      * devided into more and more intervalls up to 2^maxSteps intevalls
      */
     int maxSteps;

     /**
      * the user may give the estimated mean of the data
      *
      * if no value is specified, mean of data is calculated
      * by lti::SerialStatsFunctor and this value is used for
      * the calculation of the chiSquare test
      *
      * default is numeric_limit<double>
      */
     double mean;

     /**
      * the minimal width of an intervall
      *
      * if the parameter minimalWidth is not explicitly set (i.e.
      * left at default value < 0), minimalWidth is set to the
      * smallest distance of the given data
      */
     double minimalWidth;

      /**
      * file contains information about the parameters, status and errors
      * default name: chiSquareInfo.txt
      */
     std::string nameOfInfoFile;

      /**
      * should the infoFile be saved?
      * default is true
      */
     bool saveInfoFile;

     /**
      * if useBetterMeanAndVar is true
      * mean and variance are calculated again after the intervalls were set
      * if all constraints are still met, this values will be used
      * if not an error is reported and the old values will be used
      *
      * default is true;
      */
     bool useBetterMeanAndVar;

     /**
      * the user may give the estimated variance of the data
      *
      * variance must be positive, if negative value is given absolute
      * of this value is used
      *
      * if no value is specified, variance of data is calculated
      * by lti::SerialStatsFunctor and this value is used for
      * the calculation of the chiSquare test
      *
      * default is numeric_limit<double>
      */
     double variance;

    };

    /**
     * default constructor
     */
    chiSquareFunctor();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    chiSquareFunctor(const chiSquareFunctor& other);

    /**
     * destructor
     */
    virtual ~chiSquareFunctor();

    /**
     * returns the name of this type ("chiSquareFunctor")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    chiSquareFunctor& copy(const chiSquareFunctor& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    chiSquareFunctor& operator=(const chiSquareFunctor& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    //TODO comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.


    /**
     * considers one Element (adds it to intern element list)
     */
    void consider(const double& element);

    /**
     * considers one Element freq times (adds it to intern element list)
     * returns false if freq equals zero or has a negative value
     */
    bool consider(const double&  element, const int freq);

    /**
     * considers each element of the vector (adds it to intern element list)
     * returns false if vector is empty
     */
    bool consider(const lti::vector<double> element);

    /**
     * considers the vector freq times (adds it to intern element list)
     * returns false if freq equals zero or has a negative value
     * or if vector is empty
    */
    bool consider(const vector<double> element, const int freq);

    /**
     * if apply function has not been called jet,
     * or maximal number of Intervalls was not specified: returns 100
     */
    int getNumberOfIntervalls() const;

    /**
     * result of Chi-Square test will be stored in chiSquare
     */
    bool apply(double& chiSquare);

    /**
     * returns the chiSquareValue, after the apply function has been called
     * if apply has not been called, getChiSqureValue returns negative -1.0;
     */
    double getChiSquareValue();

    /**
     * to check the inhold of the elementList
     * elements are printed to standard output
     */
    void showElementList();

    /**
     * deletes all elements from m_ElementList, intervallList
     * and intervallBorders
     */
    void resetElements();

    /**
     * returns the variance of the given data
     * returns 0 if no data has been given before call of getVariance
     * does NOT return the variance the user specified
     * variance is calculated with lti::serialStatsFunctor
     */
    double getVarianceOfData();

    /**
     * returns the mean of the given data
     * returns 0 if no data has been given before call of getMean
     * does NOT return the mean the user specified
     * mean is calculated with lti::serialStatsFunctor
     */
    double getMeanOfData();


  private:
    void writeOutputFile();

    /**
     * is called by setIntervalls
     * returns false if constraints could not be met
     */
    bool    combineIntervalls(int index, bool equidistant);

    /**
     * calculates how many elements from the element list belong to
     * an intervall of the borderList
     * and stores result in intervallList
     */
    void    setIntervallVec();

    /**
     * calls integration function untill accuracy or maxSteps is reached
     * returns false if the given accuracy could not be reached in maxSteps
     */
    bool    integrationAccuracy(double lowBorder, double highBorder);

    /**
     * returns the result of the gauss function
     */
    double  gaussFunc(double x);

    /**
     * low and high border contain the intervall borders, integration
     * will stop when accuracy or maxSteps is reached;
     * depth increases with each call of the integration function
     * until maxSteps is reached;
     * the integration intervall is devided into 2^depth intervalls
     */
    double  integration(double lowBorder, double highBorder, int depth);

    /**
     * calculates the chiSquare value and stores it in private variable
     * chiSquareValue
     */
    void    chiSquareCalc();

    /**
     * returns false if there is not enough data
     */
    bool    setIntervalls(bool equidistant);

    /**
     * checks if string is allready in m_StringList, if not string is added
     */
    void    statusPushback(std::string status);

    //------------------------------------------------------
    // Variables
    //------------------------------------------------------

    /**
     * first it's maxNumOfInts, than it's reduced untill it meets the
     * constraints
     */
    int    m_NumberOfIntervalls;

    /// value of the element with the m_Biggest value
    double m_Biggest;

    /// value of the element with the m_Smallest value
    double m_Smallest;

    /// m_Mean calculated with the serialStatsFunctor or set by user
    double m_Mean;

    /// variance calculated with the serialStatsFunctor or set by user
    double m_Variance;

    /// result of chiSquareCalc()
    double m_ChiSquareValue;

    /// result of integrationAccuracy()
    double m_GaussIntegral;

    /**
     * result of integration over all intervalls, to normalize value
     * of intervall to 1
     */
    double m_Norm;

    /// number of all elements which were considered
    double m_NumberOfElements;

    /// calculated after intervalls were set
    double m_MeanIntervalls;

    /// calculated after intervalls were set
    double m_VarianceIntervalls;

    /// is true if it is possible to use better Mean and Var
    bool   m_UseBetterMeanAndVar;

    /// all results are written to this file
    std::string m_NameOfInfoFile;

    /// for calculation of mean and variance
    lti::serialStatsFunctor<double> m_Stats;

    /// changes during intervall setting
    std::vector<double>     m_BordersVec;

    /// contains the number of elements in each intervall
    std::vector<int>        m_IntervallVec;

    /// expected number of elements in an intervall
    std::vector<double>     m_ExpectedVec;

    /// contains all elements which were considered
    std::list<double>       m_ElementList;

    /**
     * all StatusStrings that were set, are stored here and later
     * written in infoFile
     */
    std::list<std::string>  m_StatusList;
  };
}

#endif

