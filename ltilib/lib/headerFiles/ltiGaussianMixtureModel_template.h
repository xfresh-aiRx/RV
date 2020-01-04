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


/* -----------------------------------------------------------------------
 * project ....: LTI Digitale Bild/Signal Verarbeitungsbibliothek
 * file .......: ltiGaussianMixtureModel.cpp
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 25.9.2000
 * revisions ..: $Id: ltiGaussianMixtureModel_template.h,v 1.20 2004/05/03 12:20:01 alvarado Exp $
 */

#include <limits>
#include "ltiBoundsFunctor.h"
#include "ltiSort.h"

//#define _SHOW_ALL

#undef _LTI_DEBUG
//#define _LTI_DEBUG 1
#include "ltiDebug.h"

// ---------------------------------------------------------------------------
// DEBUG CODE
// ---------------------------------------------------------------------------

#ifdef _LTI_DEBUG
#ifdef _SHOW_ALL
#include "ltiViewer.h"
#include "ltiFastViewer.h"
#include "ltiDraw.h"
#include "ltiEpsDraw.h"
#include "ltiEigenSystem.h"
#include "ltiColors.h"
#include <sstream>

namespace lti {

  namespace observer {

    static const int width=400;
    static const int height=400;
    static const double fromX = -3;
    static const double toX   = +3;
    static const double fromY = -3;
    static const double toY   = +3;

    static const int zeroX = width/2;
    static const int zeroY = height/2;

    static const double scaleX = width/(toX-fromX);
    static const double scaleY = height/(toY-fromY);

    template <class T> inline int MAPX(T x) { return iround(zeroX + (x)*scaleX); };
    template <class T> inline int MAPY(T y) { return iround(zeroY - (y)*scaleY); };

    static int imgCounter=0;

    /**
     * Returns the alpha quantile of the \chi_2 distribution
     */
    template <class T>
    inline T getChiQuant(const T& alpha) {
      return -2*lti::log(1-alpha);
    }
    
    // this is the correct method of determining the radius
    static const double radius=lti::sqrt(getChiQuant(0.95));

    
    template <class T>
    void show3(const matrix<T>& x, const std::vector< vector<T> >& mu,
               const std::vector < matrix<T> >& sigma,
               const int i) {
      
      /*
        rgbPixel colors[mu.size()];
        for ( int j=0; j<int(mu.size()); j++) {
        if (j == m1) {
        colors[j]=Red;
        } else if (j == s1 || j == s2) {
        colors[j]=Green;
        } else {
        colors[j]=Blue;
        }
        }
      */

      // drawing stuff
      char filename[80];
      sprintf(filename,"gmm-%03d-3.eps",imgCounter++);
      std::ofstream outs(filename);
      epsDraw<rgbPixel> drawer(outs,point(width,height)); //

      std::ostringstream cmts;
      cmts << "Data plotted in this figure:\n";
      for (unsigned int i=0; i<mu.size(); i++) {
        cmts << "Component " << i << "\n";
        cmts << "mu = " << mu[i] << "\n";
        cmts << "sigma = " << sigma[i] << "\n";
      }

      drawer.addComment(cmts.str());
          
      // use this for Bitmaps:
      //image panel;
      //static image* canvas=new image(width,height,White);
      //if (i == 0) {

      //drawer.use(*canvas);
      drawer.setColor(Grey50);

      // draw axes
      drawer.line(MAPX(fromX),MAPY(0),MAPX(toX),MAPY(0));
      drawer.line(MAPX(0),MAPY(fromY),MAPX(0),MAPY(toY));

      // draw points

      drawer.setMarkerType(drawBase<rgbPixel>::Xmark);
      drawer.setMarkerSize(4);
      drawer.setColor(rgbPixel(0,0,0));
      for (int k=0;k<x.rows();++k) {
//         box.setCenter(point(MAPX(x.at(k,0)),MAPY(x.at(k,1))));
//         drawer.rectangle(box);
        drawer.marker(point(MAPX(x.at(k,0)),MAPY(x.at(k,1))));
      }

      //}
      //panel.copy(*canvas);
      //drawer.use(panel);
 
      // draw circles

      lti::jacobi<T> eigen;
      typename lti::jacobi<T>::parameters jp;
      jp.sort=false;
      eigen.setParameters(jp);

      for (unsigned int k=0;k<mu.size();++k) {
        const vector<T>& muk(mu[k]);
        const matrix<T>& sigmak(sigma[k]);

        drawer.setColor(Blue);

        vector<T> eigValues;
        matrix<T> eigVectors;

        eigen.apply(sigmak,eigValues,eigVectors);

        std::ostringstream cmts;
        cmts << "Component " << k << "\n";
        cmts << "eigValues = " << eigValues << "\n";
        cmts << "eigVectors = " << eigVectors << "\n";

        drawer.addComment(cmts.str());

        // the eigenvalues are the axes scales, the eigVectors should be
        // a rotation matrix
        float angle=acos(eigVectors.at(1,1));
        if (sin(angle)*eigVectors.at(1,1) < 0) {
          // we need to take the negative angle
          angle=-angle;
        }

        drawer.ellipse(point(MAPX(muk.at(0)),MAPY(muk.at(1))),
                       iround(scaleX*radius*sqrt(eigValues.at(0))),
                       iround(scaleY*radius*sqrt(eigValues.at(1))),-angle);
      }

      //static viewer* view=new viewer("Debug Viewer");
      //view->show(panel);
      drawer.close();
    }

    template <class T>
    void show4(const matrix<T>& x, const std::vector< vector<T> >& mu,
               const std::vector < matrix<T> >& sigma,
               const int i, const int m1, const int s1, const int s2,
               const std::vector< vector<T> >& oldmu,
               const std::vector < matrix<T> >& oldsigma) {

      rgbPixel colors[mu.size()];
      rgbPixel oldcolors[oldmu.size()];
      for ( int j=0; j<int(mu.size()); j++) {
        if (j == m1) {
          colors[j]=Red;
        } else if (j == s1 || j == s2) {
          colors[j]=Green;
        } else {
          colors[j]=Blue;
        }
      }
      oldcolors[0]=darker(Red);
      oldcolors[1]=darker(Red);
      oldcolors[2]=darker(Green);

      // ltilib stuff
      char filename[80];
      sprintf(filename,"gmm-%03d-4.eps",imgCounter++);
      std::ofstream outs(filename);
      epsDraw<rgbPixel> drawer(outs,point(width,height)); //
      //image panel;
      //static image* canvas=new image(width,height,White);
      //if (i == 0) {

      //drawer.use(*canvas);
      drawer.setColor(Grey50);

      // draw axes
      drawer.line(MAPX(fromX),MAPY(0),MAPX(toX),MAPY(0));
      drawer.line(MAPX(0),MAPY(fromY),MAPX(0),MAPY(toY));

      // draw points

      drawer.setColor(rgbPixel(0,0,0));
      drawer.setMarkerType(drawBase<rgbPixel>::Xmark);
      drawer.setMarkerSize(4);
      for (int k=0;k<x.rows();++k) {
//         box.setCenter(point(MAPX(x.at(k,0)),MAPY(x.at(k,1))));
//         drawer.rectangle(box);
        drawer.marker(point(MAPX(x.at(k,0)),MAPY(x.at(k,1))));
      }
      //}
      //panel.copy(*canvas);
      //drawer.use(panel);

      // draw circles

      lti::jacobi<T> eigen;
      typename lti::jacobi<T>::parameters jp;
      jp.sort=false;
      eigen.setParameters(jp);

      for (unsigned int k=0;k<mu.size();++k) {
        const vector<T>& muk(mu[k]);
        const matrix<T>& sigmak(sigma[k]);

        drawer.setColor(colors[k]);

        vector<T> eigValues;
        matrix<T> eigVectors;

        eigen.apply(sigmak,eigValues,eigVectors);
        // the eigenvalues are the axes scales, the eigVectors should be
        // a rotation matrix
        float angle=acos(eigVectors.at(1,1));
        if (sin(angle)*eigVectors.at(1,2) < 0) {
          // we need to take the negative angle
          angle=-angle;
        }

        drawer.ellipse(point(MAPX(muk.at(0)),MAPY(muk.at(1))),
                       iround(scaleX*radius*sqrt(eigValues.at(0))),
                       iround(scaleY*radius*sqrt(eigValues.at(1))),-angle);
      }
      for (unsigned int k=0; k<oldmu.size();++k) {
        const vector<T>& muk(oldmu[k]);
        const matrix<T>& sigmak(oldsigma[k]);

        drawer.setColor(oldcolors[k]);

        vector<T> eigValues;
        matrix<T> eigVectors;

        eigen.apply(sigmak,eigValues,eigVectors);
        // the eigenvalues are the axes scales, the eigVectors should be
        // a rotation matrix
        float angle=acos(eigVectors.at(1,1));
        if (sin(angle)*eigVectors.at(1,2) < 0) {
          // we need to take the negative angle
          angle=-angle;
        }

        drawer.ellipse(point(MAPX(muk.at(0)),MAPY(muk.at(1))),
                       iround(scaleX*radius*sqrt(eigValues.at(0))),
                       iround(scaleY*radius*sqrt(eigValues.at(1))),-angle);
      }

      //static viewer* view=new viewer("Debug Viewer 2");
      //view->show(panel);
      drawer.close();

    }
  }
}
#else

namespace lti {

  namespace observer {

    template <class T>
    void show3(const matrix<T>& x, const std::vector< vector<T> >& mu,
               const std::vector < matrix<T> >& sigma,
               const int i) {
    }
    template <class T>
    void show4(const matrix<T>& x, const std::vector< vector<T> >& mu,
               const std::vector < matrix<T> >& sigma,
               const int i, const int m1, const int s1, const int s2,
               const std::vector<  vector<T> >& oldmu,
               const std::vector < matrix<T> >& oldsigma) {
    }
  }
}
#endif
#endif

// ---------------------------------------------------------------------------
// END OF DEBUG CODE
// ---------------------------------------------------------------------------


namespace lti {


  // --------------------------------------------------
  // gaussianMixtureModel::parameters
  // --------------------------------------------------

  // implementation in .h file due to a MS-VC bug

  // --------------------------------------------------
  // gaussianMixtureModel
  // --------------------------------------------------

  // default constructor
  template <class T>
  gaussianMixtureModel<T>::gaussianMixtureModel()
    : statisticsFunctor(){
    parameters tmp;
    reporter=0;
    setParameters(tmp);
  }

  // copy constructor
  template <class T>
  gaussianMixtureModel<T>::gaussianMixtureModel(const gaussianMixtureModel& other)
    : statisticsFunctor() {
    copy(other);
  }

  // destructor
  template <class T>
  gaussianMixtureModel<T>::~gaussianMixtureModel() {
  }

  // returns the name of this type
  template <class T> const char* gaussianMixtureModel<T>::getTypeName() const {
    return "gaussianMixtureModel";
  }

  // copy member
  template <class T>
  gaussianMixtureModel<T>&
  gaussianMixtureModel<T>::copy(const gaussianMixtureModel<T>& other) {
    statisticsFunctor::copy(other);
    //TODO: copy all our own stuff!
    reporter=other.reporter;
    return (*this);
  }

  // clone member
  template <class T>
  functor* gaussianMixtureModel<T>::clone() const {
    return new gaussianMixtureModel<T>(*this);
  }

  // return parameters
  template <class T>
  const typename gaussianMixtureModel<T>::parameters&
  gaussianMixtureModel<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(getTypeName());
    }
    return *par;
  }


  template <class T>
  void gaussianMixtureModel<T>::setProgressInfo(progressInfo& pi) {
    reporter=getParameters().reportProgress ? &pi : 0;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------


  // On copy apply for type matrix<T>!
  template <class T>
  bool gaussianMixtureModel<T>::estimate(const matrix<T>& src) {
    const parameters& par=getParameters();

    data=&src;
    M=par.numberOfComponents;
    N=src.rows();
    mus.clear();
    sigmas.clear();
    sigmaInvs.clear();
    matrix<T> sproto(M,M);
    vector<T> mproto(M);
    alpha.resize(M);
    dets.resize(M);

    regularizer.resize(data->columns(),data->columns());
    regularizer.setIdentity(par.lambda);

    for (int i=0; i<M; i++) {
      mus.push_back(mproto);
      sigmas.push_back(sproto);
      sigmaInvs.push_back(sproto);
    }

    if (par.useSM) {
      return smemEstimate();
    } else {
      return plainEstimate();
    }
  }


  template <class T>
  bool gaussianMixtureModel<T>::smemEstimate() {
    // perform initialization by a normal EM algorithm
    plainEstimate();
    const parameters& par=getParameters();
    const int dim=data->columns();

    // now, compute the split and merge criteria
    matrix<T> jmerge(M,M);
    vector<T> jsplit(M);

    //fullEMStep();

    T oldQ,q;

    do {
#ifdef _LTI_DEBUG
      observer::show3(*data,mus,sigmas,0);
#endif
      // compute the a-posteriori probabilities
      computeP();

      // compute the merge criteria
      for (int k=0; k<M; k++) {
        const vector<T>& rowi=p.getRow(k);
        T sum=rowi.sumOfElements();
        for (int j=k+1; j<M; j++) {
          jmerge[k][j]=rowi.dot(p.getRow(j));
          //jmerge[j][k]=jmerge[k][j];
        }
        T s2=T(0);
        for (int i=0; i<N; i++) {
          T likeki=getPartialLikelihood(data->getRow(i),k);
          // log(p[k][i]/(likeki*sum))
          // == log(p[k][i])-log(likeki*sum)
          // == log(p[k][i])-log(likeki)-log(sum)
          s2+=p[k][i]*lti::log(p[k][i]/(likeki*sum));
        }
        if (!isnan(s2)) {
          jsplit[k]=s2/sum;
        } else {
          jsplit[k]=-std::numeric_limits<T>::max();
        }
      }
      _lti_debug("Computed split and merge criteria\n");
      // now, sort the merge criteria
      matrix<lti::point> mergeIndex(M,M);
      ivector splitIndex(M);
      int i;
      for (i=0; i<M; i++) {
        for (int j=i+1; j<M; j++) {
          mergeIndex[i][j]=point(j,i);
        }
        splitIndex[i]=i;
      }

      // mergeIndex is an upper triangular matrix. Each entry
      // is the index of the corresponding element of jmerge
      // When we sort that matrix, we get a matrix
      // whose M*(M-1) first elements from (0,0) are
      // valid
      matrix<T> jmergeCopy(jmerge);
      vector<T> jsplitCopy(jsplit);
      lti::sort2<T,point> psorter(true);
      lti::sort2<T,int> isorter(true);
      psorter.apply(jmergeCopy,mergeIndex);
      isorter.apply(jsplitCopy,splitIndex);
      std::vector<point3D> index(M*(M-1)*(M-2)/2);
      //std::vector<point3D>::iterator ixi=index.begin();
      matrix<lti::point>::const_iterator mxi=mergeIndex.begin();
      // build the index of component triplets
      // the first two are the merge candidates, the last one is the split
      // candidate
      int ip=0;
      while (mxi != mergeIndex.end() && ip < int(index.size())) {
        // i and j are the merge candidates
        const point& p=*mxi;
        int i=p.y;
        int j=p.x;
        ivector::iterator sxi=splitIndex.begin();
        while (sxi != splitIndex.end()) {
          // *sxi is the split candidate
          if (*sxi != i && *sxi != j) {
            index[ip++]=point3D(j,i,*sxi);
          }
          sxi++;
        }
        mxi++;
      }

      _lti_debug("Sorted split and merge criteria\n");
      matrix<T> unity(dim,dim);
      for (i=0; i<dim; i++) {
        unity[i][i]=T(1);
      }

      oldQ=computeQ();
      if (isnan(oldQ)) {
        setStatusString("Q has ceased being a number in SM step.");
        return false;
      }
      q=oldQ;

      std::vector< vector<T> > muBackup=mus;
      std::vector< matrix<T> > sigmaBackup=sigmas;
      vector<T> alphaBackup=alpha;

      if (reporter != 0) {
        reporter->reset();
        reporter->setMaxSteps(par.cMax*(par.partialIterations+par.iterations));
      }
      // now we can do the partial split-and-merge steps
      for (int c=0; c<par.cMax && q <= oldQ; c++) {
        oldQ=q;
        point3D& p=index[c];
        int i=p.x;
        int j=p.y;
        int k=p.z;

        const int ijk[] = {i, j, k};

        // we need to re-estimate the a-posterioris of i,j,k
        // so, first, update the distribution parameters
#ifdef _LTI_DEBUG
        std::vector< vector<T> > oldmu(3);
        oldmu[0]=mus[i];
        oldmu[1]=mus[j];
        oldmu[2]=mus[k];
        std::vector< matrix<T> > oldsigma(3);
        oldsigma[0]=sigmas[i];
        oldsigma[1]=sigmas[j];
        oldsigma[2]=sigmas[k];
#endif
        // combine components i and j
        vector<T> tmpMean(mus[i]);
        matrix<T> tmpSigma(sigmas[i]);
        // compute new mean vector
        tmpMean.multiply(alpha[i]);
        tmpMean.addScaled(alpha[j],mus[j]);
        tmpMean.divide(alpha[i]+alpha[j]);
        // compute new covariance matrix
        tmpSigma.multiply(alpha[i]);
        tmpSigma.addScaled(alpha[j],sigmas[j]);
        tmpSigma.divide(alpha[i]+alpha[j]);
        mus[i]=tmpMean;
        sigmas[i]=tmpSigma;
        // compute new alpha
        alpha[i]=alpha[i]+alpha[j];

        // now split distribution k
        // first, compute new alphas
        alpha[j]=alpha[k]/2;
        alpha[k]=alpha[k]/2;
        // now compute new mean vectors
        vector<T> murange(mus[k]);
        vector<T> dmu1(dim), dmu2(dim);
        murange.apply(lti::abs);

        mus[j]=mus[k];
        for (int jj=0; jj<dim; jj++) {
          dmu1[jj]=(frand(murange[jj])-murange[jj]/T(2))*par.epsilon;
          dmu2[jj]=(frand(murange[jj])-murange[jj]/T(2))*par.epsilon;
        }
        mus[j].add(dmu1);
        mus[k].add(dmu2);

        // now compute new covariance matrices
        T det=lti::pow(luDecomposer.det(sigmas[k]),T(1)/T(dim));
        sigmas[j]=unity;
        sigmas[j].multiply(det);
        sigmas[k]=sigmas[j];

#ifdef _LTI_DEBUG
        //observer::show4(*data,mus,sigmas,c,i,j,k,oldmu,oldsigma);
        observer::show3(*data,mus,sigmas,c);
#endif
        T partialQ=-std::numeric_limits<T>::max()/2;
        //        T locOldQ=partialQ;
        // now, update the p matrix (partial EM step)
        _lti_debug("Partial step for c = " << c << "\n");
        T locOldQ=-std::numeric_limits<T>::max();
        int iter;
        for (iter=0; iter < par.partialIterations && partialQ > locOldQ; iter++) {
          if (reporter != 0) {
            reporter->step("partial step");
          }
          _lti_debug("Iteration " << iter << "\t");
          locOldQ=partialQ;
          partialEMStep(ijk,true,partialQ);
          _lti_debug("Qp is " << partialQ << std::endl);
#ifdef _LTI_DEBUG
          //observer::show4(*data,mus,sigmas,1,i,j,k,oldmu,oldsigma);
          observer::show3(*data,mus,sigmas,1);
#endif
        }
        //getchar();

        // END of partial EM step
        // now we have to iterate this step until it converges (really?)

        // then, we perform full EM, again until convergance.
        // then we evaluate Q and repeat all if it is not improved.
        // otherwise, we use the new parameters and continue with the
        // sorting step
        // otherwise, after all C are examined, halt.

        locOldQ=-std::numeric_limits<T>::max();
        for (iter=0; iter < par.iterations && q > locOldQ; iter++) {
          _lti_debug("Iteration " << iter << "\t");
          if (iter > 0) {
            // do never stop after one iteration
            locOldQ=q;
          }
          if (reporter != 0) {
            reporter->step("full step");
          }
          fullEMStep(true,q);
          _lti_debug("Q1 is " << q << std::endl);
#ifdef _LTI_DEBUG
          //observer::show4(*data,mus,sigmas,1,i,j,k,oldmu,oldsigma);
          observer::show3(*data,mus,sigmas,1);
#endif
        }
        if (q <= oldQ) {
          _lti_debug("Q is worse (best is " << oldQ << "!\n");
          mus=muBackup;
          sigmas=sigmaBackup;
          alpha=alphaBackup;
          q=oldQ;
        }
      }

    } while (q > oldQ);

    return true;
  }

  /*
   * This computes the a-posteriori probability matrix p with
   * p[l][i]=P(l|xi);
   * Therefore, the sum of a row l is the a-posteriori probability of model
   * l.
   */
  template <class T>
  void gaussianMixtureModel<T>::updateP(const int ijk[]) {

    vector<T> pold[3];

    int k,i;
    for (k=0; k<3; k++) {
      //
      pold[k]=p.getRow(ijk[k]);
    }
    for (k=0; k<3; k++) {
      // compute the inverse of each covariance matrix
      int l=ijk[k];
      dets[l]=luDecomposer.det(sigmas[l]);
      if (dets[l] < std::numeric_limits<T>::epsilon()) {
        dets[l]=rescueMatrix(sigmas[l]);
      }
      inverter.apply(sigmas[l],sigmaInvs[l]);
    }


    // we need to compute p(l|x) for each l from 1 to M:
    for (i=0; i<N; i++) {
      vector<T> pl(3);
      T sum(0);
      T sum2(0);
      for (k=0; k<3; k++) {
        // compute the inverse of each covariance matrix
        int l=ijk[k];
        pl[k]=getPartialLikelihood(data->getRow(i),l);
        sum+=alpha[l]*pl[k];
        sum2+=pold[k][i];
      }
      for (k=0; k<3; k++) {
        int l=ijk[k];
        p[l][i]=alpha[l]*pl[k]*sum2/sum;
      }
    }
  }



  /*
   * This computes the a-posteriori probability matrix p with
   * p[l][i]=P(l|xi);
   * Therefore, the sum of a row l is the a-posteriori probability of model
   * l.
   */
  template <class T>
  void gaussianMixtureModel<T>::computeP() {

    int i,l;

    _lti_debug("COMPUTING P\n");

    // compute the inverse of each covariance matrix
    for (l=0; l<M; l++) {
      dets[l]=luDecomposer.det(sigmas[l]);
      _lti_debug("dets[" << l << "] = " << dets[l] << "\n");
      if (dets[l] < std::numeric_limits<T>::epsilon()) {
        dets[l]=rescueMatrix(sigmas[l]);
      }
      inverter.apply(sigmas[l],sigmaInvs[l]);
      _lti_debug("sigmaInv[" << l << "] = " << sigmaInvs[l] << "\n");
    }

    // resize matrix
    p.resize(M,N,T(0),false,false);

    // we need to compute p(l|x) for each l from 1 to M:
    for (i=0; i<N; i++) {
      vector<T> pl(M);
      T sum(0);
      for (l=0; l<M; l++) {
        pl[l]=getPartialLikelihood(data->getRow(i),l);
        sum+=alpha[l]*pl[l];
      }
      for (l=0; l<M; l++) {
        p[l][i]=alpha[l]*pl[l]/sum;
      }
    }
  }


  template <class T>
  bool gaussianMixtureModel<T>::fullEMStep(bool evalQ, T& q) {
    // here comes a full EM step, copy from plainEstimate
    // the data-point index (0..N-1)
    int i;
    // the component index (0..M-1)
    int l;

    int dim=data->columns();

    vector<T> norm(M);
    vector<T> pl(M);

    computeP();

    // now we have the P's, do the update steps
    for (l=0; l<M; l++) {
      // we can do the update step sequentially: first update alpha[l],
      // then mu[l], then sigma[l], then proceed to next l
      T sum(0);
      for (i=0; i<N; i++) {
        sum+=p[l][i];
      }
      norm[l]=sum;
      sum/=T(N);
      //alphadist+=(alpha[l]-sum)*(alpha[l]-sum);
      alpha[l]=sum;
      _lti_debug("alpha[" << l << "] = " << alpha[l] << "\n");

      // compute new mean vectors
      vector<T> sumA(dim);
      for (i=0; i<N; i++) {
        sumA.addScaled(p[l][i],data->getRow(i));
      }
      sumA.divide(norm[l]);
      //mudist+=dist.apply(mus[l],sumA);
      mus[l].copy(sumA);
      _lti_debug("mu[" << l << "] = " << mus[l] << "\n");

      // now, compute new covariance matrices
      matrix<T> prod(dim,dim);
      matrix<T> cov(dim,dim);
      for (i=0; i<N; i++) {
        vector<T> tmp(data->getRow(i));
        tmp.subtract(mus[l]);
        tmp.multiply(lti::sqrt(p[l][i]));
        prod.outerProduct(tmp,tmp);
        cov.add(prod);
      }
      // this is for bayesian regularization, without it, this would be
      // just: cov.divide(norm[l]);
      cov.add(regularizer);
      cov.divide(norm[l]+T(1));
      //sigmadist+=dist.apply(cov,sigmas[l]);
      sigmas[l].copy(cov);
      _lti_debug("Sigma[" << l << "] =\n" << sigmas[l] << "\n");
    }
    if (evalQ) {
      q=computeQ();
      if (isnan(q)) {
        setStatusString("Q has ceased being a number in full step.");
        return false;
      }
    }
    return true;
  }


  template <class T>
  T gaussianMixtureModel<T>::computeQ() {
    T q(0);
    for (int l=0; l<M; l++) {
      for (int i=0; i<N; i++) {
        q+=p[l][i]*(lti::log(alpha[l])+getPartialLogLikelihood(data->getRow(i),l));
        if (isnan(q)) {
          return q;
          /*
          cerr << "Q is NaN!\n -> l = " << l << ", i = " << i
               << "\n -> p[l,i] = " << p[l][i]
               << "\n -> alpha = " << alpha
               << "\n -> logP(l|i) = " << getDBGPartialLogLikelihood(data->getRow(i),l)
               << "\n -> dets = " << dets
               << "\n";
          getchar();
          */
        }
      }
    }
    //_lti_debug("Q3 is " << q << endl);
    return q;
  }

  template <class T>
  T gaussianMixtureModel<T>::updateQ(const int ijk[]) {
    T q(0);
    for (int k=0; k<3; k++) {
      int l=ijk[k];
      for (int i=0; i<N; i++) {
        q+=p[l][i]*(lti::log(alpha[l])+getPartialLogLikelihood(data->getRow(i),l));
        if (isnan(q)) {
          return q;
          /*
          cerr << "Qp is NaN!\n -> l = " << l << ", i = " << i
               << "\n -> p[l,i] = " << p[l][i]
               << "\n -> alpha = " << alpha
               << "\n -> logP(l|i) = " << getDBGPartialLogLikelihood(data->getRow(i),l)
               << "\n -> dets = " << dets
               << "\n";
          getchar();
          */
        }
      }
    }
    //_lti_debug("Q3 is " << q << endl);
    return q;
  }

  template <class T>
  bool gaussianMixtureModel<T>::partialEMStep(const int ijk[], bool evalQ, T& q) {
    // here comes a full EM step, copy from smemEstimate
    updateP(ijk);

    int dim=data->columns();

    // now we have the P's, do the update steps
    for (int k=0; k<3; k++) {
      int l=ijk[k];
      int i;
      // we can do the update step sequentially: first update alpha[l],
      // then mu[l], then sigma[l], then proceed to next l
      T sum=T(0);
      dvector norm(3);
      for (i=0; i<N; i++) {
        sum+=p[l][i];
      }
      norm[k]=sum;
      sum/=T(N);
      //alphadist+=(alpha[l]-sum)*(alpha[l]-sum);
      alpha[l]=sum;
      //_lti_debug("alpha[" << l << "] = " << alpha[l] << "\n");

      // compute new mean vectors
      vector<T> sumA(dim);
      for (i=0; i<N; i++) {
        sumA.addScaled(p[l][i],data->getRow(i));
      }
      sumA.divide(static_cast<T>(norm[k]));
      //mudist+=dist.apply(means[l],sumA);
      mus[l].copy(sumA);
      //_lti_debug("mu[" << l << "] = " << mus[l] << "\n");

      // now, compute new covariance matrices
      matrix<T> prod(dim,dim);
      matrix<T> cov(dim,dim);
      for (i=0; i<N; i++) {
        vector<T> tmp(data->getRow(i));
        tmp.subtract(mus[l]);
        tmp.multiply(lti::sqrt(p[l][i]));
        prod.outerProduct(tmp,tmp);
        cov.add(prod);
      }
      // this is for bayesian regularization; without it, this would
      // be just: cov.divide(norm[k]);
      cov.add(regularizer);
      cov.divide(static_cast<T>(norm[k])+static_cast<T>(1));
      //sigmadist+=dist.apply(cov,sigmas[l]);
      sigmas[l].copy(cov);
      //_lti_debug("Sigma[" << l << "] =\n" << sigmas[l] << "\n");
    }
    if (evalQ) {
      q=updateQ(ijk);
      if (isnan(q)) {
        setStatusString("Q has ceased being a number after partial EM step.");
        return false;
      }
    }
    return true;
  }


  template <class T>
  bool gaussianMixtureModel<T>::plainEstimate() {

    const parameters& par=getParameters();
    const int dim=data->columns();

    if (reporter != 0) {
      reporter->reset();
      reporter->setMaxSteps(par.iterations);
    }
    // initialize the mean vectors and covariance matrices:
    // divide the matrix into M parts, estimate mu and sigma for each
    // part
    int psize=N/M;
    vector<T> datamin,datamax,datarange;
    boundsFunctor<T> bounds;
    bounds.apply(*data,datamin,datamax);
    datarange=datamax;
    datarange.subtract(datamin);

    _lti_debug("Datamin = " << datamin << "\n");
    _lti_debug("Datamax = " << datamax << "\n");

    for (int i=0; i<M; i++) {
      int begin=i*psize;
      int end=(i < M-1 ? (i+1)*psize-1 : data->rows());
      const matrix<T> pmat(false,const_cast<matrix<T>&>(*data),begin,end,0,dim);

      mean.meanOfRows(pmat,mus[i]);
      vector<T> offset(dim);
      for (int j=0; j<dim; j++) {
        offset[j]=frand(datarange[j])-datarange[j]/T(2);
      }
      mus[i].add(offset);
      bounds.clip(mus[i],datamin,datamax);
      var.covarianceMatrixOfRows(pmat,sigmas[i]);
      sigmas[i].divide(static_cast<T>(M));
      alpha[i]=T(1)/T(M);
      _lti_debug("alpha[" << i << "] = " << alpha[i] << "\n");
      _lti_debug("mu[" << i << "] = " << mus[i] << "\n");
      _lti_debug("Sigma[" << i << "] =\n" << sigmas[i] << "\n");
    }
    // now we are set to start the iteration
#ifdef _LTI_DEBUG
    observer::show3(*data,mus,sigmas,0);
#endif

    std::vector< matrix<T> > sigmaInv(M);
    vector<T> dets(M);

    luDecomposition<T> luDecomposer;
    l2Distance<T> dist;

    int iter=0;

    T q=-std::numeric_limits<T>::max();
    T oldQ=q;
    while (iter < par.iterations && (q >= oldQ || par.forceIterations))  {
      iter++;
      oldQ=q;
      if (reporter != 0) {
        reporter->step("iterations");
      }
      _lti_debug("Iteration " << iter << "\t");
      //cerr << "  Iteration " << iter<< "\r";
      fullEMStep(true,q);
      _lti_debug("Q2 is " << q << std::endl);
#ifdef _LTI_DEBUG
      observer::show3(*data,mus,sigmas,iter);
#endif
    }
    return true;
  };


  /*
    template <class T>
    T gaussianMixtureModel<T>::getLikelihood(const vector<T>& x) {
    vector<T> tmp(x);
    vector<T> tmp2;
    tmp.subtract(mu);
    sigmaInv.leftMultiply(tmp,tmp2);
    T e=lti::exp(-tmp.dot(tmp2)/T(2));
    e/=lti::sqrt(lti::pow(T(2)*T(lti::Pi),x.size())*det);
    return e;
    }
  */


  template <class T>
  T gaussianMixtureModel<T>::getPartialLikelihood(const vector<T>& x,
                                                  const int k) {
    vector<T> tmp(x);
    vector<T> tmp2;
    tmp.subtract(mus[k]);
    sigmaInvs[k].leftMultiply(tmp,tmp2);
    T e=lti::exp(-tmp.dot(tmp2)/T(2));
    e/=lti::sqrt(lti::pow(T(2)*T(lti::Pi),x.size())*dets[k]);
    return e;
  }

#ifdef _LTI_DEBUG
  template <class T>
    T gaussianMixtureModel<T>::getDBGPartialLikelihood(const vector<T>& x,
                                                       const int k) {
    _lti_debug("x = " << x << std::endl);
    vector<T> tmp(x);
    vector<T> tmp2;
    tmp.subtract(mus[k]);
    _lti_debug("x-mu = " << tmp << std::endl);
    sigmaInvs[k].leftMultiply(tmp,tmp2);
    _lti_debug("sigmaInv = " << sigmaInvs[k] << std::endl);
    _lti_debug("tmp2 = " << tmp2 << std::endl);
    T e=lti::exp(-tmp.dot(tmp2)/T(2));
    _lti_debug("e = " << e << std::endl);
    _lti_debug("norm = " << lti::pow(T(2)*T(lti::Pi),x.size()) << std::endl);
    e/=lti::sqrt(lti::pow(T(2)*T(lti::Pi),x.size())*dets[k]);
    return e;
  }
#else
  template <class T>
  T gaussianMixtureModel<T>::getDBGPartialLikelihood(const vector<T>& x,
                                                     const int k) {
    return getPartialLikelihood(x,k);
  }
#endif


  template <class T>
  T gaussianMixtureModel<T>::getPartialLogLikelihood(const vector<T>& x,
                                                     const int k) {
    vector<T> tmp(x);
    vector<T> tmp2;
    tmp.subtract(mus[k]);
    sigmaInvs[k].leftMultiply(tmp,tmp2);
    T e=-tmp.dot(tmp2)/T(2);
    return e-lti::log(lti::sqrt(lti::pow(T(2)*T(lti::Pi),x.size())*dets[k]));
  }

#ifdef _LTI_DEBUG
  template <class T>
  T gaussianMixtureModel<T>::getDBGPartialLogLikelihood(const vector<T>& x,
                                                        const int k) {
    _lti_debug("x = " << x << std::endl);
    vector<T> tmp(x);
    vector<T> tmp2;
    tmp.subtract(mus[k]);
    _lti_debug("x-mu = " << tmp << std::endl);
    sigmaInvs[k].leftMultiply(tmp,tmp2);
    _lti_debug("sigmaInv = " << sigmaInvs[k] << std::endl);
    _lti_debug("tmp2 = " << tmp2 << std::endl);
    T e=-tmp.dot(tmp2)/T(2);
    _lti_debug("e = " << e << std::endl);
    _lti_debug("norm = " << lti::sqrt(lti::pow(T(2)*T(lti::Pi),x.size())) << std::endl);
    return e-lti::log(lti::sqrt(lti::pow(T(2)*T(lti::Pi),x.size())*dets[k]));
  }
#else
  template <class T>
  T gaussianMixtureModel<T>::getDBGPartialLogLikelihood(const vector<T>& x,
                                                        const int k) {
    return getPartialLogLikelihood(x,k);
  }
#endif

  template <class T>
  T gaussianMixtureModel<T>::rescueMatrix(matrix<T>& victim) {
    vector<T> d=victim.getDiagonal();
    const int n=d.size();
    T lambda=getParameters().emergencyLambda;
    T f=lambda+T(1);
    T det=0;
    victim.multiply(T(1)/f);
    f*=f;
    _lti_debug("rescueing matrix\n");
    while (det < std::numeric_limits<T>::epsilon()) {
      // emphasize diagonal until it seems reasonable to quit
      for (int i=0; i<n; i++) {
        victim[i][i]*=f;
      }
      T oldDet=det;
      det=luDecomposer.det(victim);
      if (det == oldDet) {
        // this means that the loop will not terminate, set it to unity
        for (int i=0; i<n; i++) {
          victim[i][i]=lambda;
        }
        det=lti::pow(lambda,n);
      }
      _lti_debug("emphasized det is " << det << "\n");
    }
    return det;
  }


}

#include "ltiUndebug.h"
