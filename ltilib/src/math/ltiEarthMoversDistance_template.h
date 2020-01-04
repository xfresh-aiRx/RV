/*
 * Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003
 * Yossi Rubner, Computer Science Department, Stanford University
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


/* -------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiEarthMoversDistance.cpp
 * authors ....: Yossi Rubner, Peter Doerfler
 * organization: CS, Stanford University; LTI, RWTH Aachen
 * creation ...: 29.10.2003
 * revisions ..: $Id: ltiEarthMoversDistance_template.h,v 1.6 2004/06/21 11:28:26 doerfler Exp $
 */

#include "ltiMath.h"
#include "ltiGenericVector.h"

#undef _LTI_DEBUG
//#define _LTI_DEBUG 2
#include "ltiDebug.h"

namespace lti {

  // --------------------------------------------------
  // earthMoversDistance::parameters
  // --------------------------------------------------

  // implementation on header file due to MS VC++ bug

  // --------------------------------------------------
  // earthMoversDistance
  // --------------------------------------------------

  // default constructor
  template<class W, class C, class D>
  earthMoversDistance<W,C,D>::earthMoversDistance()
    : distanceFunctor<W>() {

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);


  }

  // default constructor
  template<class W, class C, class D>
  earthMoversDistance<W,C,D>::earthMoversDistance(const parameters& par)
    : distanceFunctor<W>() {

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  template<class W, class C, class D>
  earthMoversDistance<W,C,D>::earthMoversDistance(const earthMoversDistance<W,C,D>& other)
    : distanceFunctor<W>() {
    copy(other);
  }

  // destructor
  template<class W, class C, class D>
  earthMoversDistance<W,C,D>::~earthMoversDistance() {
    delete basicVars;
    delete distr1Basic;
    delete distr2Basic;
  }

  // returns the name of this type
  template<class W, class C, class D>
  const char* earthMoversDistance<W,C,D>::getTypeName() const {
    return "earthMoversDistance<W,C,D>";
  }

  // copy member
  template<class W, class C, class D>
  earthMoversDistance<W,C,D>&
  earthMoversDistance<W,C,D>::copy(const earthMoversDistance<W,C,D>& other) {
    distanceFunctor<W>::copy(other);

    return (*this);
  }

  // alias for copy member
  template<class W, class C, class D>
  earthMoversDistance<W,C,D>&
  earthMoversDistance<W,C,D>::operator=(const earthMoversDistance<W,C,D>& other) {
    return (copy(other));
  }


  // clone member
  template<class W, class C, class D>
  functor* earthMoversDistance<W,C,D>::clone() const {
    return new earthMoversDistance<W,C,D>(*this);
  }

  // return parameters
  template<class W, class C, class D>
  const typename earthMoversDistance<W,C,D>::parameters&
  earthMoversDistance<W,C,D>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(getTypeName());
    }
    return *par;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  /**
   * calculate the distance between the vectors a and b
   * @param a the first vector<W>
   * @param b the second vector<W>
   * @param dist the distance between the vectors
   * @return false on error -> see status string
   */
  template<class W, class C, class D>
  bool earthMoversDistance<W,C,D>::apply(const vector<W>& a,
                                         const vector<W>& b,
                                         W& dist) const {
    bool ok=true;
    int i;

    int szA=a.size();
    int szB=b.size();

    vector<W> supplyW(a);
    vector<W> demandW(b);
    
    _lti_debug2("calling init()\n");
    init(supplyW,demandW);

    ivector idxA, idxB; 
    if (szA==szB) {
      idxA.resize(szA,0,false,false);
      for (i=0; i<szA; ++i) {
        idxA.at(i)=i;
      }
      idxB.useExternData(szA,&idxA.at(0));
    } else {
      idxA.resize(szA,0,false,false);
      idxB.resize(szB,0,false,false);
      for (i=0; i<min(szA,szB); ++i) {
        idxA.at(i)=i;
        idxB.at(i)=i;
      }
      if (szA>szB) {
        for (i=szB; i<szA; ++i) {
          idxA.at(i)=i;
        }
      } else {
        for (i=szA; i<szB; ++i) {
          idxB.at(i)=i;
        }
      }
    }

    _lti_debug2("calculating cost matrix\n");
    ok = ok && calcCost(idxA,idxB);
    
    _lti_debug2("cost matrix:\n" << costMatrix);
    
    ok = ok && emd(supplyW, demandW, szA, szB, dist);

    return ok;
  }

  /**
   * calculate something like the distance between the matrices a and b:
   * both matrices are seen as vectors.
   * @param a the first matrix<W>
   * @param b the second matrix<W>
   * @param dist the 'distance' between the matrices
   * @return false on error -> see status string
   */
  template<class W, class C, class D>
  bool earthMoversDistance<W,C,D>::apply(const matrix<W>& a,
                                         const matrix<W>& b,
                                         W& dist) const {
    
    bool ok=true;
    int i,j;

    int rowsA=a.rows();
    int colsA=a.columns();
    int rowsB=b.rows();
    int colsB=b.columns();

    int szA=rowsA*colsA;
    int szB=rowsB*colsB;

    vector<W> supplyW(szA,&a.at(0,0));
    vector<W> demandW(szB,&b.at(0,0));
    

    init(supplyW,demandW);

    vector<point> idxA, idxB; 
    if (szA==szB) {
      idxA.resize(szA,point(0,0),false,false);
      for (i=0; i<rowsA; ++i) {
        for (j=0; j<colsA; ++j) {
          idxA.at(i*colsA+j)=point(i,j);
        }
      }
      idxB.useExternData(szA,&idxA.at(0));
    } else {
      idxA.resize(szA,point(0,0),false,false);
      idxB.resize(szB,point(0,0),false,false);
      for (i=0; i<rowsA; ++i) {
        for (j=0; j<colsA; ++j) {
          idxA.at(i*colsA+j)=point(i,j);
        }
      }
      for (i=0; i<rowsB; ++i) {
        for (j=0; j<colsB; ++j) {
          idxB.at(i*colsB+j)=point(i,j);
        }
      }
    }

    _lti_debug2("idxA:" << idxA << std::endl);
    _lti_debug2("idxB:" << idxB << std::endl);

    ok = ok && calcCost(idxA,idxB);
    
    _lti_debug2("cost matrix:\n" << costMatrix << std::endl);
    
    ok = ok && emd(supplyW, demandW, szA, szB, dist);

    return ok;
  }


  /**
   * calculate the distances between the rows or columns of the
   * matrices a and b, determined by the parameters rowWise.
   * @param a the first vector<W>
   * @param b the second vector<W>
   * @param dists the distances between the matrices
   * @return false on error -> see status string
   */
  template<class W, class C, class D>
  bool earthMoversDistance<W,C,D>::apply(const matrix<W>& a,
                                       const matrix<W>& b,
                                       vector<W>& dists) const {
    assert(a.size()==b.size());
    int r=a.rows();
    dists.resize(r,W(0),false,false);
    bool ok =true;
    for (int i=0; i<r; i++) {
      ok = ok && apply(a.getRow(i), b.getRow(i), dists.at(i));
    }
    return ok;
  }

  /**
   * Calculate the distance between each row or column of m
   * depending on the value of rowWise and the vector v.
   * @param m the matrix<W>
   * @param v the vector to be compared with
   * @param dest the vector with the distances to the vector v
   * @return false on error
   */
  template<class W, class C, class D>
  bool earthMoversDistance<W,C,D>::apply(const matrix<W>& m,
                                       const vector<W>& v,
                                       vector<W>& dest) const {

    assert(m.columns()==v.size());
    int r=m.rows();
    dest.resize(r,W(0),false,false);
    bool ok=true;
    for (int i=0; i<r; i++) {
      ok = ok && apply(m.getRow(i), v, dest.at(i));
    }
    return ok;
  }

  template<class W, class C, class D>
  bool earthMoversDistance<W,C,D>::apply(const std::vector<C>& clusters1,
                                         const std::vector<C>& clusters2,
                                         const vector<W>& weights1,
                                         const vector<W>& weights2,
                                         W& dist) const {

    assert(clusters1.size()==static_cast<unsigned int>(weights1.size()));
    assert(clusters2.size()==static_cast<unsigned int>(weights2.size()));

    bool ok=true;

    int szA=weights1.size();
    int szB=weights2.size();
    
    vector<W> supplyW(weights1);
    vector<W> demandW(weights2);
    
    init(supplyW,demandW);
    
    ok = ok && calcCost(clusters1, clusters2);
    
    _lti_debug2("cost matrix:\n" << costMatrix << std::endl);
    
    ok = ok && emd(supplyW, demandW, szA, szB, dist);
    
    return ok;
  }

  template<class W, class C, class D>
  bool 
  earthMoversDistance<W,C,D>::apply(const matrix<cluster_value_type>& clusters1,
                                    const matrix<cluster_value_type>& clusters2,
                                    const vector<W>& weights1,
                                    const vector<W>& weights2,
                                    W& dist) const {

    assert(clusters1.rows()==weights1.size());
    assert(clusters2.rows()==weights2.size());

    bool ok=true;

    int szA=weights1.size();
    int szB=weights2.size();

    vector<W> supplyW(weights1);
    vector<W> demandW(weights2);
    
    init(supplyW,demandW);

    ok = ok && calcCost(clusters1, clusters2);
    
    _lti_debug2("cost matrix:\n" << costMatrix << std::endl);
    
    ok = ok && emd(supplyW, demandW, szA, szB, dist);

    return ok;
  }
  
//************************************************************************
//**********  Helper Methods  ********************************************
//************************************************************************

  /**
   * inits data structures. returns true if the sum of weights (earth) is
   * significantly different in the two distributions 
   */
  template<class W, class C, class D>
  bool earthMoversDistance<W,C,D>::init(vector<W>& weights1,
                                        vector<W>& weights2) const {

    sz1=weights1.size();
    sz2=weights2.size();

    int i,j;
    bool b;

    const parameters& param=getParameters();

    minWeight=weights1.sumOfElements();
    maxWeight=weights2.sumOfElements();

    W diff=minWeight-maxWeight;

    // if the amount of earth to move is not equal the cost matrix needs
    // an extra row or column
    if (lti::abs(diff)>=param.epsilon*minWeight) {
      if (diff<W(0)) {
        sz1++;
        weights1.resize(sz1,-diff,true,true);
      } else {
        exchange(minWeight,maxWeight);
        sz2++;
        weights2.resize(sz2,diff,true,true);
      }
      b = true;
    } else {
      b = false;
    }

    int szsz=sz1+sz2;

    basicVars = new double_node[szsz];
    distr1Basic = new double_node*[sz1];
    distr2Basic = new double_node*[sz2];
    isBasicVar.resize(sz1,sz2,false);
    endBasicVars = basicVars;

    for (i=0; i<sz1; i++) {
      distr1Basic[i]=NULL;
    }

    for (j=0; j<sz2; j++) {
      distr2Basic[j]=NULL;
    }

    return b;

  }

  template<class W, class C, class D>
  bool earthMoversDistance<W,C,D>::emd(vector<W>& weights1,
                                       vector<W>& weights2,
                                       const int& slen,const int& dlen,
                                       W& totalFlow) const {

    bool ok=true;
    int step;
    single_node *sup, *dem;
    double_node* itBV;
    sup = new single_node[sz1];
    dem = new single_node[sz2];

    const parameters& param=getParameters();

    _lti_debug2("--- Before russel\n");
    _lti_debug2("supply weights\n" << weights1 << std::endl);
    _lti_debug2("demand weights\n" << weights2 << std::endl << std::endl);


    ok = ok && russel(weights1, weights2);
    enterBasicVars=endBasicVars++;

    
#ifdef _LTI_DEBUG
    dumpVariables();
#endif

    if (sz1>1 && sz2>1) {
      
      for (step=0; step<param.maxSteps; step++) {

        findBasicVariables(sup, dem);

        if (isOptimal(sup, dem)) {
          break;
        }

        newSolution();

      }

      _lti_debug2("optimization steps: " << step << "\n");

    }

#ifdef _LTI_DEBUG
    dumpVariables();
#endif

    totalFlow=W(0);
    for (itBV=basicVars; itBV<endBasicVars; ++itBV) {
      // empty slot
      if (itBV==enterBasicVars) continue;
      // dummy row or column
      if ((itBV->i==slen)||(itBV->j==dlen)) continue;
      // zero flow
      if (itBV->val==W(0)) continue;
      
      totalFlow += itBV->val * costMatrix.at(itBV->i, itBV->j);
    }
    totalFlow/=minWeight;

    delete[] sup;
    delete[] dem;

    return ok;
  }

  template<class W, class C, class D>
  bool earthMoversDistance<W,C,D>::russel(vector<W>& weights1,
                                          vector<W>& weights2) const {
    
    bool ok=true;

    W eps=getParameters().epsilon;
    int i, j, found;
    int minI=0;
    int minJ=0;
    W deltaMin, oldVal, diff;
    matrix<W> delta(sz1,sz2);
    single_node *Ur, *Vr;
    single_node uHead, *curU, *prevU;
    single_node vHead, *curV, *prevV;
    single_node *Remember, *prevUMinI=NULL;
    single_node *prevVMinJ=NULL;
    Ur = new single_node[sz1];
    Vr = new single_node[sz2];

    /* INITIALIZE THE ROWS LIST (Ur), AND THE COLUMNS LIST (Vr) */
    uHead.next = curU = Ur;
    for (i=0; i < sz1; i++) {
      curU->i = i;
      curU->val = -Inf;;
      curU->next = curU+1;
      curU++;
    }
    (--curU)->next = NULL;
  
    vHead.next = curV = Vr;
    for (j=0; j < sz2; j++) {
      curV->i = j;
      curV->val = -Inf;
      curV->next = curV+1;
      curV++;
    }
    (--curV)->next = NULL;
  
    /* FIND THE MAXIMUM ROW AND COLUMN VALUES (Ur[i] AND Vr[j]) */
    for(i=0; i < sz1 ; i++) {
      for(j=0; j < sz2 ; j++) {
	W v;
	v = costMatrix.at(i,j);
	if (Ur[i].val <= v)
	  Ur[i].val = v;
	if (Vr[j].val <= v)
	  Vr[j].val = v;
      }
    }
  
    /* COMPUTE THE delta MATRIX */
    for(i=0; i < sz1 ; i++) {
      for(j=0; j < sz2 ; j++) {
        delta.at(i,j) = costMatrix.at(i,j) - Ur[i].val - Vr[j].val;
      }
    }

    /* FIND THE BASIC VARIABLES */
    do {
// #if DEBUG_LEVEL > 3
//       printf("Ur=");
//       for(curU = uHead.next; curU != NULL; curU = curU->next)
// 	printf("[%d]",curU-Ur);
//       printf("\n");
//       printf("Vr=");
//       for(curV = vHead.next; curV != NULL; curV = curV->next)
// 	printf("[%d]",curV-Vr);
//       printf("\n");
//       printf("\n\n");
// #endif
 
      /* FIND THE SMALLEST delta[i][j] */
      found = 0; 
      deltaMin = Inf;      
      prevU = &uHead;
      for (curU=uHead.next; curU != NULL; curU=curU->next) {
        int i;
        i = curU->i;
        prevV = &vHead;
        for (curV=vHead.next; curV != NULL; curV=curV->next) {
          int j;
          j = curV->i;
          if (deltaMin > delta[i][j]) {
            deltaMin = delta[i][j];
            minI = i;
            minJ = j;
            prevUMinI = prevU;
            prevVMinJ = prevV;
            found = 1;
          }
          prevV = curV;
        }
        prevU = curU;
      }
      
      if (! found) {
	break;
      }

      /* ADD X[minI][minJ] TO THE BASIS, AND ADJUST SUPPLIES AND COST */
      Remember = prevUMinI->next;
      addBasicVariable(minI, minJ, weights1, weights2, 
                       prevUMinI, prevVMinJ, &uHead);

      /* UPDATE THE NECESSARY delta[][] */
      if (Remember == prevUMinI->next) {  /* LINE minI WAS DELETED */
        for (curV=vHead.next; curV != NULL; curV=curV->next) {
          int j;
          j = curV->i;
          if (curV->val == costMatrix.at(minI,j)) { /* COLUMN j NEEDS UPDATING */
            /* FIND THE NEW MAXIMUM VALUE IN THE COLUMN */
            oldVal = curV->val;
            curV->val = -Inf;
            for (curU=uHead.next; curU != NULL; curU=curU->next) {
              int i;
              i = curU->i;
              if (curV->val <= costMatrix.at(i,j))
                curV->val = costMatrix.at(i,j);
            }
		  
            /* IF NEEDED, ADJUST THE RELEVANT delta[*][j] */
            diff = oldVal - curV->val;
            if (abs(diff) < eps * maxCost) {
              for (curU=uHead.next; curU != NULL; curU=curU->next) {
                delta.at(curU->i,j) += diff;
              }
            }
          }
        }
      } else { /* COLUMN minJ WAS DELETED */
        for (curU=uHead.next; curU != NULL; curU=curU->next) {
          int i;
          i = curU->i;
          if (curU->val == costMatrix.at(i,minJ)) { /* ROW i NEEDS UPDATING */
            /* FIND THE NEW MAXIMUM VALUE IN THE ROW */
            oldVal = curU->val;
            curU->val = -Inf;
            for (curV=vHead.next; curV != NULL; curV=curV->next) {
              int j;
              j = curV->i;
              if(curU->val <= costMatrix.at(i,j)) {
                curU->val = costMatrix.at(i,j);
              }
            }
		  
            /* If NEEDED, ADJUST THE RELEVANT delta[i][*] */
            diff = oldVal - curU->val;
            if (abs(diff) < eps * maxCost) {
              for (curV=vHead.next; curV != NULL; curV=curV->next) {
                delta[i][curV->i] += diff;
              }
            }
          }
        }
      }
    } while (uHead.next != NULL || vHead.next != NULL);

    delete[] Ur;
    delete[] Vr;

    return ok;

  }

  template <class W, class C, class D>
  void earthMoversDistance<W,C,D>::addBasicVariable(int minI, int minJ, 
                        vector<W>& weights1, vector<W>& weights2,
                        single_node *prevUMinI, single_node *prevVMinJ,
                        single_node *UHead) const {
    W tmp;
    const W& eps=getParameters().epsilon;
    
    /* DEGENERATE CASE */
    if (abs(weights1.at(minI)-weights2.at(minJ)) <= eps * maxWeight)  
    {
      tmp = weights1.at(minI);
      weights1.at(minI) = 0;
      weights2.at(minJ) -= tmp; 
    }
    else if (weights1.at(minI) < weights2.at(minJ))  /* SUPPLY EXHAUSTED */
    {
      tmp = weights1.at(minI);
      weights1.at(minI) = 0;
      weights2.at(minJ) -= tmp; 
    }
    else  /* DEMAND EXHAUSTED */
    {
      tmp = weights2.at(minJ);
      weights2.at(minJ) = 0; 
      weights1.at(minI) -= tmp; 
    }

    /* X(minI,minJ) IS A BASIC VARIABLE */
    isBasicVar.at(minI,minJ) = true; 

    endBasicVars->val = tmp;
    endBasicVars->i = minI;
    endBasicVars->j = minJ;
    endBasicVars->nextC = distr1Basic[minI];
    endBasicVars->nextR = distr2Basic[minJ];
    distr1Basic[minI] = endBasicVars;
    distr2Basic[minJ] = endBasicVars;
    endBasicVars++;
  
    /* DELETE SUPPLY ROW ONLY IF THE EMPTY, AND IF NOT LAST ROW */
    if (weights1.at(minI) == 0 && UHead->next->next != NULL) {
      prevUMinI->next = prevUMinI->next->next;  /* REMOVE ROW FROM LIST */
    } else {
      prevVMinJ->next = prevVMinJ->next->next;  /* REMOVE COLUMN FROM LIST */
    }
  }

  template <class W, class C, class D>
  void earthMoversDistance<W,C,D>::findBasicVariables(single_node *U,
                                                      single_node *V) const {
    
    int i, j, found;
    int UfoundNum, VfoundNum;
    single_node u0Head, u1Head, *curU, *prevU;
    single_node v0Head, v1Head, *curV, *prevV;
    
    /* INITIALIZE THE ROWS LIST (U) AND THE COLUMNS LIST (V) */
    u0Head.next = curU = U;
    for (i=0; i < sz1; i++) {
      curU->i = i;
      curU->next = curU+1;
      curU++;
    }
    (--curU)->next = NULL;
    u1Head.next = NULL;

    curV = V+1;
    v0Head.next = sz2 > 1 ? V+1 : NULL;
    for (j=1; j < sz2; j++) {
      curV->i = j;
      curV->next = curV+1;
      curV++;
    }
    (--curV)->next = NULL;
    v1Head.next = NULL;
    
    /* THERE ARE sz1+sz2 VARIABLES BUT ONLY sz1+sz2-1 INDEPENDENT EQUATIONS,
       SO SET V[0]=0 */
    V[0].i = 0;
    V[0].val = 0;
    v1Head.next = V;
    v1Head.next->next = NULL;

    /* LOOP UNTIL ALL VARIABLES ARE FOUND */
    UfoundNum=VfoundNum=0;
    while (UfoundNum < sz1 || VfoundNum < sz2) {
      
// #if DEBUG_LEVEL > 3
//       printf("UfoundNum=%d/%d,VfoundNum=%d/%d\n",UfoundNum,sz1,VfoundNum,sz2);
//       printf("U0=");
//       for(curU = u0Head.next; curU != NULL; curU = curU->next)
// 	printf("[%d]",curU-U);
//       printf("\n");
//       printf("U1=");
//       for(curU = u1Head.next; curU != NULL; curU = curU->next)
// 	printf("[%d]",curU-U);
//       printf("\n");
//       printf("V0=");
//       for(curV = v0Head.next; curV != NULL; curV = curV->next)
// 	printf("[%d]",curV-V);
//       printf("\n");
//       printf("V1=");
//       for(curV = v1Head.next; curV != NULL; curV = curV->next)
// 	printf("[%d]",curV-V);
//       printf("\n\n");
// #endif
      
      found = 0;
      if (VfoundNum < sz2) {
        /* LOOP OVER ALL MARKED COLUMNS */
        prevV = &v1Head;
        for (curV=v1Head.next; curV != NULL; curV=curV->next) {
          j = curV->i;
          /* FIND THE VARIABLES IN COLUMN j */
          prevU = &u0Head;
          for (curU=u0Head.next; curU != NULL; curU=curU->next) {
            i = curU->i;
            if (isBasicVar.at(i,j)) {
              /* COMPUTE U[i] */
              curU->val = costMatrix.at(i,j) - curV->val;
              /* ...AND ADD IT TO THE MARKED LIST */
              prevU->next = curU->next;
              curU->next = u1Head.next != NULL ? u1Head.next : NULL;
              u1Head.next = curU;
              curU = prevU;
            } else {
              prevU = curU;
            }
          }
          prevV->next = curV->next;
          VfoundNum++;
          found = 1;
        }
      }
      if (UfoundNum < sz1) {
        /* LOOP OVER ALL MARKED ROWS */
        prevU = &u1Head;
        for (curU=u1Head.next; curU != NULL; curU=curU->next) {
          i = curU->i;
          /* FIND THE VARIABLES IN ROWS i */
          prevV = &v0Head;
          for (curV=v0Head.next; curV != NULL; curV=curV->next) {
            j = curV->i;
            if (isBasicVar.at(i,j)) {
              /* COMPUTE V[j] */
              curV->val = costMatrix.at(i,j) - curU->val;
              /* ...AND ADD IT TO THE MARKED LIST */
              prevV->next = curV->next;
              curV->next = v1Head.next != NULL ? v1Head.next: NULL;
              v1Head.next = curV;
              curV = prevV;
            } else {
              prevV = curV;
            }
          }
          prevU->next = curU->next;
          UfoundNum++;
          found = 1;
        }
      }
      if (! found) {
        fprintf(stderr, "emd: Unexpected error in findBasicVariables!\n");
        fprintf(stderr, "This typically happens when the EPSILON defined in\n");
        fprintf(stderr, "emd.h is not right for the scale of the problem.\n");
        exit(1);
      }
    }
  }




/**********************
    isOptimal
 **********************/
  template <class W, class C, class D>
  int earthMoversDistance<W,C,D>::isOptimal(single_node *U, 
                                            single_node *V) const { 
    double delta, deltaMin;
    int i, j;
    int minI=0;
    int minJ=0;
    
    /* FIND THE MINIMAL Cij-Ui-Vj OVER ALL i,j */
    deltaMin = Inf;
    for(i=0; i < sz1; i++) {
      for(j=0; j < sz2; j++) {
        if (! isBasicVar.at(i,j)) {
	  delta = costMatrix.at(i,j) - U[i].val - V[j].val;
	  if (deltaMin > delta) {
            deltaMin = delta;
            minI = i;
            minJ = j;
          }
	}
      }
    }

    _lti_debug3("deltaMin="<<deltaMin);

    if (deltaMin == Inf) {
      fprintf(stderr, "emd: Unexpected error in isOptimal.\n");
      exit(0);
    }
   
    enterBasicVars->i = minI;
    enterBasicVars->j = minJ;
    
    /* IF NO NEGATIVE deltaMin, WE FOUND THE OPTIMAL SOLUTION */
    return deltaMin >= -getParameters().epsilon * maxCost;
    
  }
  

/**********************
    newSolution
**********************/
  template <class W, class C, class D>
  void earthMoversDistance<W,C,D>::newSolution() const {
    
    int i, j, k;
    double xMin;
    int steps;
    double_node **loop, *curX, *leaveX=NULL;
    loop = new double_node*[sz1+sz2];
 
    _lti_debug3("enterBasicVar = (" 
                << enterBasicVars->i << ", " << enterBasicVars->j << "\n");

    /* ENTER THE NEW BASIC VARIABLE */
    i = enterBasicVars->i;
    j = enterBasicVars->j;
    isBasicVar.at(i,j) = true;
    enterBasicVars->nextC = distr1Basic[i];
    enterBasicVars->nextR = distr2Basic[j];
    enterBasicVars->val = 0;
    distr1Basic[i] = enterBasicVars;
    distr2Basic[j] = enterBasicVars;

    /* FIND A CHAIN REACTION */
    steps = findLoop(loop);

    /* FIND THE LARGEST VALUE IN THE LOOP */
    xMin = Inf;
    for (k=1; k < steps; k+=2) {
      if (loop[k]->val < xMin) {
        leaveX = loop[k];
        xMin = loop[k]->val;
      }
    }

    /* UPDATE THE LOOP */
    for (k=0; k < steps; k+=2){
      loop[k]->val += xMin;
      loop[k+1]->val -= xMin;
    }

// #if DEBUG_LEVEL > 3
//     printf("leaveX = (%d,%d)\n", leaveX->i, leaveX->j);
// #endif

    /* REMOVE THE LEAVING BASIC VARIABLE */
    i = leaveX->i;
    j = leaveX->j;
    isBasicVar.at(i,j) = false;
    if (distr1Basic[i] == leaveX) {
      distr1Basic[i] = leaveX->nextC;
    } else {
      for (curX=distr1Basic[i]; curX != NULL; curX = curX->nextC) {
	if (curX->nextC == leaveX) {
          curX->nextC = curX->nextC->nextC;
          break;
        }
      }
    }
    if (distr2Basic[j] == leaveX) {
      distr2Basic[j] = leaveX->nextR;
    } else {
      for (curX=distr2Basic[j]; curX != NULL; curX = curX->nextR) {
	if (curX->nextR == leaveX) {
          curX->nextR = curX->nextR->nextR;
          break;
        }
      }
    }
    /* SET enterBasicVars TO BE THE NEW EMPTY SLOT */
    enterBasicVars = leaveX;

    delete[] loop;
  }



/**********************
    findLoop
**********************/
  template <class W, class C, class D>
  int earthMoversDistance<W,C,D>::findLoop(double_node **loop) const {

    int steps;
    double_node **curX, *NewX;
    genericVector<bool> isUsed(sz1+sz2,false); 
 
    curX = loop;
    NewX = *curX = enterBasicVars;
    isUsed[enterBasicVars-basicVars] = 1;
    steps = 1;

    do {
      if (steps%2 == 1) {
        /* FIND AN UNUSED X IN THE ROW */
        NewX = distr1Basic[NewX->i];
        while (NewX != NULL && isUsed[NewX-basicVars])
          NewX = NewX->nextC;
      } else {
        /* FIND AN UNUSED X IN THE COLUMN, OR THE ENTERING X */
        NewX = distr2Basic[NewX->j];
        while (NewX != NULL && isUsed[NewX-basicVars] && NewX != enterBasicVars) {
          NewX = NewX->nextR;
        }
        if (NewX == enterBasicVars) {
          break;
        }
      }

      if (NewX != NULL) {  /* FOUND THE NEXT X */
        /* ADD X TO THE LOOP */
        *++curX = NewX;
        isUsed[NewX-basicVars] = 1;
        steps++;
// #if DEBUG_LEVEL > 3
//         printf("steps=%d, NewX=(%d,%d)\n", steps, NewX->i, NewX->j);    
// #endif
      } else {  /* DIDN'T FIND THE NEXT X */
        /* BACKTRACK */
        do {
          NewX = *curX;
          do {
            if (steps%2 == 1) {
              NewX = NewX->nextR;
            } else {
              NewX = NewX->nextC;
            }
          } while (NewX != NULL && isUsed[NewX-basicVars]);
	     
          if (NewX == NULL) {
            isUsed[*curX-basicVars] = 0;
            curX--;
            steps--;
          }
        } while (NewX == NULL && curX >= loop);
	 
// #if DEBUG_LEVEL > 3
//         printf("BACKTRACKING TO: steps=%d, NewX=(%d,%d)\n",
//                steps, NewX->i, NewX->j);    
// #endif
        isUsed[*curX-basicVars] = 0;
        *curX = NewX;
        isUsed[NewX-basicVars] = 1;
      }     
    } while(curX >= loop);
  
    if (curX == loop) {
      fprintf(stderr, "emd: Unexpected error in findLoop!\n");
      exit(1);
    }
// #if DEBUG_LEVEL > 3
//     printf("FOUND LOOP:\n");
//     for (i=0; i < steps; i++)
//       printf("%d: (%d,%d)\n", i, loop[i]->i, loop[i]->j);
// #endif

    return steps;
  }

  /**
   * Finds the costMatrix for the given supply and demand. 
   */
  template <class W, class C, class D>
  bool earthMoversDistance<W,C,D>::calcCost(const ivector& supply,
                                            const ivector& demand) const {


    _lti_debug2(sz1 << " supply clusters:\n" << supply << std::endl);
    _lti_debug2(sz2 << " demand clusters:\n" << demand << std::endl);

    const parameters& param=getParameters();

    // actual sizes of demand and supply
    int slen=supply.size();
    int dlen=demand.size();
    int i,j;

    // an extra column or row needs to be added since the
    // total weights of supply and demand are unequal
//      bool addOne=((slen!=sz1)||(dlen!=sz2));
    // the costMatrix is already defined in the parameters
    bool costDefined=!param.costMatrix.empty();

    // if costMatrix is defined in the parameters
    if (costDefined) {
      _lti_debug2("Cost Matrix defined in parameters\n");
      _lti_debug2("slen= " << slen << "  sz1=" << sz1 << "\n");
      _lti_debug2("dlen= " << dlen << "  sz2=" << sz2 << "\n");
      // check dimensions
      assert (param.costMatrix.rows()==slen &&
              param.costMatrix.columns()==dlen);
      // copy the data
      costMatrix.resize(sz1,sz2,W(0),false,false);
      costMatrix.fill(param.costMatrix);
      if (slen<sz1) {
        costMatrix.getRow(slen).fill(W(0));
      } else if (dlen<sz2) {
        for (i=0; i<sz1; i++) {
          costMatrix.at(i,dlen)=W(0);
        }
      }

    } else {   // calculate the costMatrix
      _lti_debug2("Cost Matrix NOT defined in parameters\n");
      
      // bring to correct size
      costMatrix.resize(sz1,sz2,W(0),false,false);

      for (i=0; i<sz1; i++) {
        // extra row needed?
        if (i>=slen) {
          _lti_debug4("extra row\n");
          costMatrix.getRow(i).fill(W(0));
        } else {
          for (j=0; j<sz2; j++) {
            if (j>=dlen) {
              // extra column needed?
              _lti_debug4("extra col\n");
              costMatrix.at(i,j)=W(0);
            } else {
              _lti_debug4("i,j: " << i << " " << j << std::endl);
              costMatrix.at(i,j)
                =static_cast<W>(abs(supply.at(i)-demand.at(j)));
            }
          }
        }
      }
    }
    maxCost=costMatrix.maximum();
    return true;
  }

  /**
   * Finds the costMatrix for the given supply and demand.
   */
  template <class W, class C, class D>
  bool earthMoversDistance<W,C,D>::calcCost(const vector<point>& supply,
                                            const vector<point>& demand) const {


    _lti_debug2(sz1 << " supply clusters:\n" << supply << std::endl);
    _lti_debug2(sz2 << " demand clusters:\n" << demand << std::endl);

    const parameters& param=getParameters();

    // actual sizes of demand and supply
    int slen=supply.size();
    int dlen=demand.size();
    int i,j;

    // an extra column or row needs to be added since the
    // total weights of supply and demand are unequal
//      bool addOne=((slen!=sz1)||(dlen!=sz2));
    // the costMatrix is already defined in the parameters
    bool costDefined=!param.costMatrix.empty();

    // if costMatrix is defined in the parameters
    if (costDefined) {
      // check dimensions
      assert (param.costMatrix.rows()==slen &&
              param.costMatrix.columns()==dlen);
      // copy the data
      costMatrix.resize(sz1,sz2,W(0),false,false);
      costMatrix.fill(param.costMatrix);
      if (slen<sz1) {
        costMatrix.getRow(slen).fill(W(0));
      } else if (dlen<sz2) {
        for (i=0; i<sz1; i++) {
          costMatrix.at(i,dlen)=W(0);
        }
      }

    } else {   // calculate the costMatrix
      
      // bring to correct size
      costMatrix.resize(sz1,sz2,W(0),false,false);

      for (i=0; i<sz1; i++) {
        // extra row needed?
        if (i>=slen) {
          _lti_debug4("extra row\n");
          costMatrix.getRow(i).fill(W(0));
        } else {
          for (j=0; j<sz2; j++) {
            if (j>=dlen) {
              // extra column needed?
              _lti_debug4("extra col\n");
              costMatrix.at(i,j)=W(0);
            } else {
              _lti_debug4("i,j: " << i << " " << j << std::endl);
              costMatrix.at(i,j)
                =static_cast<W>(sqrt(distanceSqr(supply.at(i),
                                                 demand.at(j))));
            }
          }
        }
      }
    }
    maxCost=costMatrix.maximum();
    return true;
  }

  /**
   * Finds the costMatrix for the given supply and demand. 
   */
  template <class W, class C, class D>
  bool earthMoversDistance<W,C,D>::calcCost(const std::vector<C>& supply,
                                            const std::vector<C>& demand) const {
      
      
//       _lti_debug2(sz1 << " supply clusters:\n" << supply << std::endl);
//       _lti_debug2(sz2 << " demand clusters:\n" << demand << std::endl);

    const parameters& param=getParameters();

    // actual sizes of demand and supply
    int slen=supply.size();
    int dlen=demand.size();
    int i,j;

    // an extra column or row needs to be added since the
    // total weights of supply and demand are unequal
//      bool addOne=((slen!=sz1)||(dlen!=sz2));
    // the costMatrix is already defined in the parameters
    bool costDefined=!param.costMatrix.empty();

    // if costMatrix is defined in the parameters
    if (costDefined) {
      // check dimensions
      assert (param.costMatrix.rows()==slen &&
              param.costMatrix.columns()==dlen);
      // copy the data
      costMatrix.resize(sz1,sz2,W(0),false,false);
      costMatrix.fill(param.costMatrix);
      if (slen<sz1) {
        costMatrix.getRow(slen).fill(W(0));
      } else if (dlen<sz2) {
        for (i=0; i<sz1; i++) {
          costMatrix.at(i,dlen)=W(0);
        }
      }

    } else {   // calculate the costMatrix
      
      // bring to correct size
      costMatrix.resize(sz1,sz2,W(0),false,false);

      D distFunc;

      for (i=0; i<sz1; i++) {
        // extra row needed?
        if (i>=slen) {
          _lti_debug4("extra row\n");
          costMatrix.getRow(i).fill(W(0));
        } else {
          for (j=0; j<sz2; j++) {
            if (j>=dlen) {
              // extra column needed?
              _lti_debug4("extra col\n");
              costMatrix.at(i,j)=W(0);
            } else {
              _lti_debug4("i,j: " << i << " " << j << std::endl);
              costMatrix.at(i,j)
                =static_cast<W>(distFunc(supply[i], demand[j]));
            }
          }
        }
      }
    }
    maxCost=costMatrix.maximum();
    return true;
  }

  /**
   * Finds the costMatrix for the given supply and demand.
   */
  template <class W, class C, class D>
  bool earthMoversDistance<W,C,D>::calcCost(
    const matrix<cluster_value_type>& supply, 
    const matrix<cluster_value_type>& demand) const {
      
    
//       _lti_debug2(sz1 << " supply clusters:\n" << supply << std::endl);
//       _lti_debug2(sz2 << " demand clusters:\n" << demand << std::endl);

    const parameters& param=getParameters();

    // actual sizes of demand and supply
    int slen=supply.rows();
    int dlen=demand.rows();
    int i,j,k;

    // an extra column or row needs to be added since the
    // total weights of supply and demand are unequal
//      bool addOne=((slen!=sz1)||(dlen!=sz2));
    // the costMatrix is already defined in the parameters
    bool costDefined=!param.costMatrix.empty();

    // if costMatrix is defined in the parameters
    if (costDefined) {
      // check dimensions
      assert (param.costMatrix.rows()==slen &&
              param.costMatrix.columns()==dlen);
      // copy the data
      costMatrix.resize(sz1,sz2,W(0),false,false);
      costMatrix.fill(param.costMatrix);
      if (slen<sz1) {
        costMatrix.getRow(slen).fill(W(0));
      } else if (dlen<sz2) {
        for (i=0; i<sz1; i++) {
          costMatrix.at(i,dlen)=W(0);
        }
      }

    } else {   // calculate the costMatrix
      
      // bring to correct size
      costMatrix.resize(sz1,sz2,W(0));

      D distFunc;
      typename D::distance_type tmp;
      const typename D::distance_type zeroDist
        =static_cast<typename D::distance_type>(0);
      int dim=supply.columns();

      for (i=0; i<sz1; i++) {
        // extra row needed?
        if (i>=slen) {
          _lti_debug4("extra row\n");
          costMatrix.getRow(i).fill(W(0));
        } else {
          for (j=0; j<sz2; j++) {
            if (j>=dlen) {
              // extra column needed?
              _lti_debug4("extra col\n");
              costMatrix.at(i,j)=W(0);
            } else {
              _lti_debug4("i,j: " << i << " " << j << std::endl);
              tmp=zeroDist;
              for (k=0; k<dim; k++) {
                distFunc.accumulate(supply.at(i,j),demand.at(i,j),tmp);
              }
              costMatrix.at(i,j)=static_cast<W>(distFunc.computeDistance(tmp));
            }
          }
        }
      }
    }
    maxCost=costMatrix.maximum();
    return true;
  }

  template <class W, class C, class D>
  bool earthMoversDistance<W,C,D>::findFlow(const int& slen, const int& dlen,
                                            matrix<W>& flow) const {
    flow.resize(slen, dlen, W(0));
    
    double_node* itBV;
    
    for (itBV=basicVars; itBV<endBasicVars; ++itBV) {
      // empty slot
      if (itBV==enterBasicVars) continue;
      // dummy row or column
      if ((itBV->i==slen)||(itBV->j==dlen)) continue;
      // zero flow
      if (itBV->val==W(0)) continue;
      
      flow.at(itBV->i,itBV->j)=itBV->val;
    }
    return true;
  }


//========================================================================
//===============  Debugging stuff  ======================================
//========================================================================

#ifdef _LTI_DEBUG
  template <class W, class C, class D>
  void earthMoversDistance<W,C,D>::dumpVariables() const {
    int i;
    int szsz=sz1+sz2;
    
    _lti_debug("isBasicVar:\n"<<isBasicVar<<"\n");

    _lti_debug("The basic Variables:\n");
    for (i=0; i<szsz; i++) {
      _lti_debug("(i="<<basicVars[i].i<<" j="<<basicVars[i].j
                 <<" val="<<basicVars[i].val<<")  ");
    }
    _lti_debug("\n");
    
    _lti_debug("Supply links:\n");
    for (i=0; i<sz1; i++) {
      if (distr1Basic[i]!=NULL) {
        _lti_debug("(i="<<distr1Basic[i]->i<<" j="<<distr1Basic[i]->j
                   <<" val="<<distr1Basic[i]->val<<") ");
      } else {
        _lti_debug("(NULL) ");
      }
    }
    _lti_debug("\n");
    _lti_debug("Demand links:\n");
    for (i=0; i<sz2; i++) {
      if (distr2Basic[i]!=NULL) {
        _lti_debug("(i="<<distr2Basic[i]->i<<" j="<<distr2Basic[i]->j
                   <<" val="<<distr2Basic[i]->val<<") ");
      } else {
        _lti_debug("(NULL) ");
      }
    }
    _lti_debug("\n");

  }


#endif
#include "ltiUndebug.h"
}
