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


/*----------------------------------------------------------------
 * project ....: LTI Digital Image/Signal Processing Library
 * file .......: ltiMinimizeBasis_template.h
 * authors ....: Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 09.06.99
 * revisions ..: $Id: ltiMinimizeBasis_template.h,v 1.7 2003/05/05 06:42:08 alvarado Exp $
 */

#include "ltiLinearAlgebraFunctor.h"
#include "ltiEquationSystem.h"
#include "ltiEigenSystem.h"
// #include <fstream>

namespace lti {
  // construction
  template<class T>
  minimizeBasis<T>::minimizeBasis(const parameters& theParams) {
    setParameters(theParams);
  }

  // returns the current parameters
  template<class T>
  const typename minimizeBasis<T>::parameters&
  minimizeBasis<T>::getParameters() const {
    const parameters* params =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(params)) {
      throw invalidParametersException(getTypeName());
    }
    return *params;
  }

  // onCopy version of apply
  template<class T>
  void minimizeBasis<T>::apply(const matrix<T>& testVectors,
                               matrix<T>& newBasis,
                               matrix<T>& factors) {

    //    std::ofstream mat;
    //    mat.open("mat.txt",ios::out/*|ios::app*/);
    //    mat << "======================================\n";

    const parameters& tmpParam = getParameters();
    double norm=0,maxDev;
    typename matrix<T>::const_iterator iter;

    // compute l2MatrixNorm of the set of given vectors for normalization
    for(iter=testVectors.begin();iter!=testVectors.end();iter++) {
      norm += (*iter)*(*iter);
    }
    norm = sqrt(norm);

    // unnormalize maxDeviation
    maxDev = tmpParam.maxDeviation * norm;

    if(tmpParam.method==parameters::Fast) { // *** fast method
      int r,c,i,j;
      // int basisVectors=0;
      double maxL2VectorDev,l2SqMatrixDev=0;
      bool isNullvector;
      qrSolution<T> leastSquares;
      typename qrSolution<T>::parameters lsParams;

      lsParams.computeResiduum=true;

      // search for first basis vector
      r=0;
      isNullvector=true;
      while(isNullvector && (r<testVectors.rows())) {
        c=0;
        while(isNullvector && (c<testVectors.columns())) {
          if(testVectors[r][c] != 0) {
            isNullvector=false;
          }
          c++;
        }
        r++;
      }

      // nullmatrix?
      if(isNullvector) {
        newBasis.resize(0,0);
        factors.resize(0,0);
        return;
      }

      // deviation: maximal l2VectorNorm
      if(tmpParam.deviationType==parameters::Element) {
        maxL2VectorDev = maxDev * sqrt(testVectors.columns());
      } else { // deviation type is Vector or Matrix
        maxL2VectorDev = maxDev;
      }

      //      mat << "norm: " << norm << std::endl;
      //      mat << "maxDev: " << maxDev << std::endl;
      //      mat << "maxL2VectorDev: " << maxL2VectorDev << std::endl;

      // use first given vector (!=nullvector) as first basis vector
      // systemMatrix is the temporary output matrix
      lsParams.systemMatrix.resize(testVectors.columns(),1);
      lsParams.systemMatrix.setColumn(0,testVectors[r-1]);
      leastSquares.setParameters(lsParams);
      // adjust transformation matrix
      factors.resize(testVectors.rows(),1,0,false,true);
      factors[r-1][0]=1;

      // for all given vectors...
      for(i=r;i<testVectors.rows();i++) {
        vector<T> newFactors(lsParams.systemMatrix.columns());

        //mat << "\n\ni = " << i << std::endl;
        //mat << "systemMatrix:\n" << leastSquares.getParameters().systemMatrix;
        //mat << "\n\ntestVectors\n" << testVectors;
        //mat << "\nfactors\n" << factors;
        double dev;
        bool devPermit;
        // ... is the given vector representable by the current basis
        // vectors within the maxDeviation?
        // dev: distance between the actual vector and the actual
        // subbasis (l2-norm)
        if((dev=leastSquares.apply(testVectors[i],newFactors))
           <=maxL2VectorDev) {
          //mat << "\nexpected l2SqMatrixDev: "
          //    << l2SqMatrixDev + dev*dev << std::endl;
          devPermit=true;
          if(tmpParam.deviationType==parameters::Element) {
            // compute difference vector
            vector<T> diff(testVectors[i]),tmp;

            for(j=0;j<newFactors.size();j++) {
              lsParams.systemMatrix.getColumnCopy(j,tmp);
              diff -= tmp.multiply(newFactors[j]);
            }
            //mat << "\nDifferencevector:\n" << diff;

            // check deviation
            for(j=0;(j<diff.size())&&devPermit;j++) {
              if(fabs(diff[j])>maxDev) {
                devPermit = false;
              }
            }
          }
          else if((tmpParam.deviationType==parameters::Matrix) &&
                  (sqrt(l2SqMatrixDev + dev*dev)>maxDev)) {
            devPermit = false;
          }
        } else {
          devPermit=false;
        }
        // if((dev=leastSquares.apply(testVectors[i],newFactors))
        //       <=tmpParam.maxDeviation)
        if(devPermit) {
          // yes: copy linear combination factors
          //mat << "\nyes!";
          factors[i]=newFactors;
          l2SqMatrixDev += dev*dev;
        } else {
          // no: use vector as new basis vector
          //mat << "\nno!";
          lsParams.systemMatrix.resize(testVectors.columns(),
                                       lsParams.systemMatrix.columns()+1);
          lsParams.systemMatrix.setColumn(lsParams.systemMatrix.lastColumn(),
                                          testVectors[i]);
          leastSquares.setParameters(lsParams);
          factors.resize(testVectors.rows(),lsParams.systemMatrix.columns(),
                         0,true,true);
          factors[i][lsParams.systemMatrix.lastColumn()]=1;
        }
        //mat << "\n\nnorm. l2-Fehler: " << dev/norm << std::endl;
        //mat << "\nl2SqMatrixDev: " << l2SqMatrixDev << std::endl;
      }

      lsParams.systemMatrix.detach(newBasis);
    } else { // *** Exact Method

      matrix<double> dNewBasis,dFactors;

      matrix<double> moment;
      matrix<double> subBasis;
      vector<double> eigenvalues;
      jacobi<double> pca;
      jacobi<double>::parameters pcaParam;
      luSolution<double> eqs;
      luSolution<double>::parameters eqsParam;
      int i,j,k;
      int bestApprox;
      double sum,tempSum,temp;
      double minDeviation=0,totalDeviation,maxL2MatrixDev;
      matrix<T> newBasisSav,factorsSav;

      // deviation: maximal l2MatrixDev
      if(tmpParam.deviationType==parameters::Element) {
        maxL2MatrixDev = maxDev *
          sqrt(testVectors.columns()*testVectors.rows());
      } else if(tmpParam.deviationType==parameters::Vector) {
        maxL2MatrixDev = maxDev * sqrt(testVectors.rows());
      } else { // Matrix
        maxL2MatrixDev = maxDev;
      }

      // initialization
      dFactors.castFrom(testVectors);

      pcaParam.sort = false;
      pca.setParameters(pcaParam);

      totalDeviation = 0.0;

      dNewBasis.resize(dFactors.columns(),dFactors.columns(),0,false,true);
      for(i=0;i<dFactors.columns();i++) {
        dNewBasis.at(i,i) = 1.0;
      }

      do {
        // construct symmetric moment matrix (ignore below diagonal elements)
        moment.resize(dFactors.columns(),dFactors.columns());
        for(i=0;i<dFactors.columns();i++) {
          for(j=i;j<dFactors.columns();j++) {
            sum = 0.0;
            for(k=0;k<dFactors.rows();k++) {
              sum += dFactors[k][i]*dFactors[k][j];
            }
            moment[i][j] = sum;
          }
        }

        // principle components analysis
        pca.apply(moment,eigenvalues,subBasis);

        //mat << "\n***DIMENSION: " << dFactors.columns();
        //mat << "\nfactors:\n" << dFactors << std::endl;
        // find subspace with smallest deviation
        bestApprox = -1;
        for(i=0;i<subBasis.columns();i++) {
          vector<double> tempVec;
          subBasis.getColumnCopy(i,tempVec);
          //mat << "\n----\nEigenwert: " << eigenvalues[i] << std::endl;
          //mat << "\nEigenvector:\n" << tempVec << std::endl;

          sum = 0.0;
          for(j=0;j<dFactors.rows();j++) {
            tempSum = 0.0;
            for(k=0;k<dFactors.columns();k++) {
              tempSum += dFactors[j][k] * subBasis[k][i];
            }
            // tempSum: distance between the actual testVector and the
            // actual subbasis (l2-norm)
            sum += tempSum*tempSum;
          }

          //mat << "\nFehler: " << sum << std::endl;

          if((sum<minDeviation)||(bestApprox==-1)) {
            minDeviation = sum;
            bestApprox = i;
          }
        }

        totalDeviation += minDeviation;

        //mat << "\n***minDeviation = " << minDeviation;
        //mat << "\n***bestApprox = " << bestApprox;
        //mat << "\n***totalDeviation = " << sqrt(totalDeviation)/norm;

        // Deviation too large -> ABORT
        if(sqrt(totalDeviation)>maxL2MatrixDev)
          break;

        // save matrices for later deviation check
        if((tmpParam.deviationType==parameters::Element) ||
           (tmpParam.deviationType==parameters::Vector)) {
          newBasisSav.castFrom(dNewBasis);
          factorsSav.castFrom(dFactors);
        }

        // new subspace's normal is last vector of new basis
        for(i=0;i<subBasis.rows();i++) {
          temp = subBasis[i][subBasis.lastColumn()];
          subBasis[i][subBasis.lastColumn()] = subBasis[i][bestApprox];
          subBasis[i][bestApprox] = temp;
        }

        /*
          // new basis does not include subspace's normal vector
          if(bestApprox<(subBasis.lastColumn()))
          {
          for(i=0;i<subBasis.rows();i++)
          {
          subBasis[i][bestApprox] = subBasis[i][subBasis.lastColumn()];
          }
          }
          subBasis.resize(subBasis.rows(),subBasis.lastColumn());
        */
        //mat << "\nsubBasis:\n" << subBasis;

        // transform test vectors to new basis
        eqsParam.systemMatrix = subBasis;
        eqs.setParameters(eqsParam);
        for(i=0;i<dFactors.rows();i++) {
          eqs.apply(dFactors[i]);
        }
        subBasis.resize(subBasis.rows(),subBasis.lastColumn());

        // compute backtransformation matrix
        dNewBasis.multiply(subBasis);
        //mat << "\nnewBasis:\n" << dNewBasis;

        //mat << "\nTest Vectors:\n" << dFactors;

        // resize test vectors
        dFactors.resize(dFactors.rows(),dFactors.lastColumn());
        // mat << "\n...resize:\n" << dFactors;

        // convert to type T (from double!)
        newBasis.castFrom(dNewBasis);
        factors.castFrom(dFactors);

        // deviation check
        if((tmpParam.deviationType==parameters::Element) ||
           (tmpParam.deviationType==parameters::Vector)) {
          int i,j,k;
          matrix<T> app(testVectors);

          for(i=0;i<newBasis.rows();i++) {
            for(j=0;j<factors.rows();j++) {
              for(k=0;k<newBasis.columns();k++) {
                app[j][i] -= newBasis[i][k] * factors[j][k];
              }
            }
          }

          //mat << "Differenz:\n";
          //mat << app;

          if(tmpParam.deviationType==parameters::Element) {
            for(i=0;i<app.rows();i++) {
              for(j=0;j<app.columns();j++) {
                if(fabs(app[i][j])>maxDev) {
                  //mat << "Ausstieg bei Element = "
                  //    << fabs(app[i][j])/norm << std::endl;
                  newBasisSav.detach(newBasis);
                  factorsSav.detach(factors);
                  return;
                }
              }
            }
          } else { // deviationType==vector
            double l2Vec;
            for(i=0;i<app.rows();i++) {
              l2Vec = 0;
              for(j=0;j<app.columns();j++) {
                l2Vec += app[i][j]*app[i][j];
              }
              if(sqrt(l2Vec)>maxDev) {
                //mat << "Ausstieg bei l2Vec = "
                //    << sqrt(l2Vec)/norm << std::endl;
                newBasisSav.detach(newBasis);
                factorsSav.detach(factors);
                return;
              }
            }
          }
        }
      } while(factors.columns()>1);
    }

    // Test
    /*
      mat << "\n-----------\n";
      mat << "Approximation:\n";
      int i,j,k;
      double l2mat,max;
      matrix<double> app,diff,dTestVectors;
      vector<double> l2vec;

      dTestVectors.castFrom(testVectors);

      app.resize(factors.rows(),newBasis.rows(),0,false,true);

      for(i=0;i<newBasis.rows();i++) {
      for(j=0;j<factors.rows();j++) {
      for(k=0;k<newBasis.columns();k++) {
      app[j][i] += newBasis[i][k] * factors[j][k];
      }
      }
      }
      mat << app << std::endl;

      mat << "Differenz:\n";
      app.multiply(-1);
      app.add(dTestVectors);
      mat << app;

      max = l2mat = 0;
      l2vec.resize(app.rows(),0,false,true);
      for(i=0;i<app.rows();i++) {
      for(j=0;j<app.columns();j++) {
      l2mat += app[i][j]*app[i][j];
      l2vec[i] += app[i][j]*app[i][j];
      if(fabs(app[i][j])>max)
      max = fabs(app[i][j]);
      }
      }
      mat << "\n>> Matrix-Norm (L2^2): " << l2mat << "   "
      << l2mat/norm << "   sqrt: " << sqrt(l2mat)/norm << std::endl;
      vector<double> tv(l2vec);
      for(i=0;i<tv.size();i++)
      tv[i]=sqrt(tv[i])/norm;
      mat << ">> Vector-Norm (L2� zeilenweise):\n" << l2vec << std::endl
      << tv << "  (sqrt)" << std::endl;
      mat << ">> Maximum: " << max << "   " << max/norm << std::endl;
    */
  }
}

