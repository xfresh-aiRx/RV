/*
 * Copyright (C) 1998 - 2004
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
 * file .......: ltiMathFunction.h
 * authors ....: Lars Libuda
 * organization: LTI, RWTH Aachen
 * creation ...: 2.11.2004
 * revisions ..: $Id: ltiFunctionGenerator.h,v 1.5 2005/06/16 12:37:34 doerfler Exp $
 */

#ifndef _LTI_FUNCTION_GENERATOR_H_
#define _LTI_FUNCTION_GENERATOR_H_

#include "ltiMathFunction.h"
#include "ltiMath.h"
#include <string>

namespace lti {
  /**
   *  functionGenerator parses input strings representing mathematical functions
   *  of the type \f$f(x)\f$ and \f$f(x,y)\f$. These strings are converted into a tree
   *  structure which allows the retrieval of the functions' result for passed arguments.
   *  The calculation is performed in the apply-methods.
   *  The string is set in the parameters of this class like in this example:
   *  \code
   *  #include "ltiFunctionGenerator.h"
   *
   *  lti::functionGenerator             function;
   *  lti::functionGenerator::parameters params;
   *  params.function = "(x-1)*cos(y)/x^2+1.234";
   *  if (!function.setParameters(params)) {
   *    // something went wrong during parsing, get the error message
   *    std::cout << function.getStatusString();
   *  }
   *  else {
   *    // calculate the result for two arguments
   *    double result;
   *    if (!function.apply(2.5,3.1,result)) {
   *      std::cout << "Function is not defined for given arguments";
   *    }
   *  }
   *  \endcode
   *
   *  Conventions for string input:
   *  -# The string is case insensitive
   *  -# Numerical constants consists of decimal digits (0-9) and the period (.)
   *  -# Two numerical constants are defined as literals: 'pi' and 'e'
   *  -# The names for the variables holding the arguments are 'x' and 'y' by default.
   *     These names can be changed in the parameters.
   *  -# Parenthesis are allowed in any desired depth
   *  -# Allowed operators: '+','-','*','/','^'
   *  -# Allowed functions: 
   *         - sin()    -> sine 
   *         - cos()    -> cosine 
   *         - tan()    -> tangent
   *         - cot()    -> cotangent
   *         - sec()    -> secant
   *         - csc()    -> cosecant
   *         - arcsin() -> arc sine 
   *         - arccos() -> arc cosine 
   *         - arctan() -> arc tangent
   *         - arccot() -> arc cotangent
   *         - sinh()   -> hyperbolic sine
   *         - cosh()   -> hyperbolic cosine
   *         - tanh()   -> hyperbolic tangent
   *         - coth()   -> hyperbolic cotangent
   *         - arsinh() -> hyperbolic area sine
   *         - arcosh() -> hyperbolic area cosine 
   *         - artanh() -> hyperbolic area tangent
   *         - arcoth() -> hyperbolic area cotangent
   *         - ln()     -> logarithm base e
   *         - lg()     -> logarithm base 10
   *         - ld()     -> logarithm base 2
   *         - sqrt()   -> Square root
   *         - abs()    -> absolute value
   *         - sgn()    -> sign function
   *         - int()    -> greatest integer smaller the argument (Gaussian parenthesis)
   */
  class functionGenerator : public mathFunction {
  public:
    /**
     * The parameters for the class functionGenerator
     */
    class parameters : public mathFunction::parameters {
    public:
      /**
       * Default constructor
       */
      parameters();

      /**
       * Copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * Destructor
       */
      ~parameters();

      /**
       * Returns name of this type
       */
      const char* getTypeName() const;

      /**
       * Copy the contents of a parameters object
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
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * Read the parameters from the given ioHandler
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
       * string containing the function to evaluate
       */
      std::string function;
      /**
       * Name of the first argument. Default is 'x'.
       * Do not change if it is not necessary. If a name is chosen which
       * is already reserved for function names, operators,or literals the
       * behaviour is unpredictable.
       */
      std::string nameVarX;
      /**
       * Name of the second argument. Default is 'y'.
       * Do not change if it is not necessary. If a name is chosen which
       * is already reserved for function names, operators,or literals the
       * behaviour is unpredictable.
       */
      std::string nameVarY;
    };

    /**
     * Default constructor
     */
    functionGenerator();

    /**
     * Construct a functor using the given parameters
     */
    functionGenerator(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    functionGenerator(const functionGenerator& other);

    /**
     * Destructor
     */
    virtual ~functionGenerator();

    /**
     * Returns the name of this type ("functionGenerator")
     */
    virtual const char* getTypeName() const;

    /**
     * Updates the internal state after parameter change. Here, the function string
     * is parsed and the tree is build.
     * @return true, if the tree was successfully build.
     */
    virtual bool updateParameters();
   
    /**
     * operates on a copy of the given %parameters.
     * @param x first argument
     * @param y second argument
     * @param result result of evaluation. 1.0 in case of errors
     * @return true if the function is defined for the given arguments
     */
    bool apply(const double& x,const double& y,double& result) const;

    /**
     * operates on a copy of the given %parameters.
     * @param x argument
     * @param result result of evaluation. 1.0 in case of errors
     * @return true if the function is defined for the given argument
     */
    bool apply(const double& x,double& result) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    functionGenerator& copy(const functionGenerator& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    functionGenerator& operator=(const functionGenerator& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

# ifdef _LTI_MSC_6
  public:
# else            
  private:
# endif      
    class node {
      protected:
        node *pRight, *pLeft;
        static bool bMathError;

      public:
        node(node *pl = NULL, node *pr = NULL);

        virtual ~node();

        node *getRightChild() {
          return pRight;
        }

        node *getLeftChild() {
          return pLeft;
        }

        void setLeftChild(node *p = NULL) {
          pLeft = p;
        }

        void setRightChild(node *p = NULL) {
          pRight = p;
        }

        int isLeaf() {
          return (pLeft == NULL && pRight == NULL);
        }

        virtual double calculate(const double& x, const double& y) = 0;

        static bool error();

    };

    class nodeAdd : public node {
      public:
        nodeAdd(node *l,node *r) : node(l,r) {
        }

        virtual double calculate(const double& x, const double& y) {
          return getLeftChild()->calculate(x,y) + getRightChild()->calculate(x,y);
        }
    };

    class nodeSub : public node {
      public:
        nodeSub(node *l,node *r) : node(l,r) {
        }

        virtual double calculate(const double& x, const double& y) {
          return getLeftChild()->calculate(x,y) - getRightChild()->calculate(x,y);
        }
    };

    class nodeMul : public node {
      public:
        nodeMul(node *l,node *r) : node(l,r) {
        }

        virtual double calculate(const double& x, const double& y) {
          return getLeftChild()->calculate(x,y) * getRightChild()->calculate(x,y);
        }
    };

    class nodeDiv : public node {
      public:
        nodeDiv(node *l,node *r);
        virtual double calculate(const double& x, const double& y);
    };

    class nodePow : public node {
      public:
        nodePow(node *l, node *r);
        virtual double calculate(const double& x, const double& y);
    };

    class nodeConst : public node {
      private:
        double dValue;

      public:
        nodeConst(double dConst) : node(NULL,NULL),dValue(dConst) {
        }

        virtual double calculate(const double& x, const double& y) {
          return dValue;
        }
    };

    class nodeVarX : public node {
      public:
        nodeVarX() : node(NULL,NULL) {
        }

        virtual double calculate(const double& x, const double& y) {
          return x;
        }
    };

    class nodeVarY : public node {
      public:
        nodeVarY() : node(NULL,NULL) {
        }

        virtual double calculate(const double& x, const double& y) {
          return y;
        }
    };

    class nodeFunc : public node {
# ifdef _LTI_MSC_6
      public:
# else            
      private:
# endif      
        typedef double (*tMathFunc) (double);

        static tMathFunc pFuncs[]; 
        int iFunc;

      public:
        nodeFunc(node *pNode, int iFuncNr);
        virtual double calculate(const double& x, const double& y);
        static double Pow(double x,double y);

      private:
        static double Sin(double x);
        static double Cos(double x);
        static double Tan(double x);
        static double Cot(double x);
        static double Sec(double x);
        static double Cosec(double x);
        static double Arcsin(double x);
        static double Arccos(double x);
        static double Arctan(double x);
        static double Arccot(double x);
        static double Sinh(double x);
        static double Cosh(double x);
        static double Tanh(double x);
        static double Coth(double x);
        static double Arsinh(double x);
        static double Arcosh(double x);
        static double Artanh(double x);
        static double Arcoth(double x);
        static double Ln(double x);
        static double Lg(double x);
        static double Ld(double x);
        static double Sqrt(double x);
        static double Abs(double x);
        static double Sgn(double x);
        static double Intfunc(double x);
    };

  private:
    /**
     * these methods and attributes are used during parsing
     */
    char* strLower(char* s);
    int   skipBrackets(char* s,int& i);
    char* deleteBrackets(char* s);
    char* skipWhiteSpace(char* s);
    node* compile1(char* s);
    node* compile2(char* s);
    node* compile3(char* s);
    node* compile4(char* s);

    const static char* szFuncs[];
    const static double pi;      
    const static double e;       

    int   iError;
    char* szErrorPos;     
    bool  bHasTwoVars;    
    node* pRootNode;      
  };
}

#endif

