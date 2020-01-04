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
 * file .......: ltiCubicSpline_template.h
 * authors ....: Ruediger Weiler
 * organization: LTI, RWTH Aachen
 * creation ...: 28.5.2001
 * revisions ..: $Id: ltiCubicSpline_template.h,v 1.5 2004/05/06 10:31:26 alvarado Exp $
 */

namespace lti {

  // --------------------------------------------------
  // cubicSpline
  // --------------------------------------------------

  // default constructor
  template <class T>
  cubicSpline<T>::cubicSpline()
    : variablySpacedSamplesInterpolator(){
  }

  // copy constructor
  template <class T>
  cubicSpline<T>::cubicSpline(const cubicSpline& other)
    : variablySpacedSamplesInterpolator() {
    copy(other);
  }

  // destructor
  template <class T>
  cubicSpline<T>::~cubicSpline() {
  }

  // returns the name of this type
  template <class T>
  const char* cubicSpline<T>::getTypeName() const {
    return "cubicSpline";
  }

  // copy member

  template <class T>
  cubicSpline<T>&
    cubicSpline<T>::copy(const cubicSpline<T>& other) {
      variablySpacedSamplesInterpolator::copy(other);
    return (*this);
  }

  // clone member
  template <class T>
  functor* cubicSpline<T>::clone() const {
    return new cubicSpline<T>(*this);
  }

  // return parameters
  template <class T>
  const typename cubicSpline<T>::parameters&
  cubicSpline<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(getTypeName());
    }
    return *par;
  }

  template <class T>
  bool
  cubicSpline<T>::updateParameters() {
    const parameters& par = getParameters();
    
    //check if the parameters are ok at least 3 point and so on
    if(par.samplingPoints.size() >=3){
      typename tpointList<T>::const_iterator it;
      T lastElement;
      it=par.samplingPoints.begin();
      lastElement = (*it).x;
      for (it++ ;it!=par.samplingPoints.end();it++) {
        //check if the pointlist displays a function -> xn < x(n+1)
        if((*it).x > lastElement)
          lastElement = (*it).x;
        else {
          setStatusString("Point list represents no function!");
          return false;
        }
      }
      
      //calculate the second derivatives
      spline(par.samplingPoints,
             par.derivativeAtFirstPoint,
             par.derivativeAtLastPoint,
             par.useNaturalDerivatives);
      //if all parameters are ok set the parameters
      return true;
    } else {
      // MSVC bug the compiler only take this in one line
      // std::string str;
      // str = "This functor needs at least 3 points.";
      setStatusString("This functor needs at least 3 points.");
    }

    return false;
  }

  template <class T>
  void cubicSpline<T>::spline(const tpointList<T>& in,
                              const double& yp1,
                              const double& ypn,
                              const bool &natural) {
    // Given arrays x[1..n] and y[1..n] containing a tabulated function,
    // i.e., y i = f(xi), with x1<x2< ... <xN , and given values yp1 and
    // ypn for the first derivative of the interpolating function at
    // points 1 and n, respectively, this routine returns an array
    // y2[1..n] that contains the second derivatives of the interpolating
    // function at the tabulated points xi. If yp1 and/or ypn are equal
    // to 1.10e30 or larger, the routine is signaled to set the
    // corresponding boundary condition for a natural spline, with zero
    // second derivative on that boundary.
    int i;
    int n = in.size(); // point list size
    double x1,x2,x3,y1,y2,y3; //3 double points
    double p,qn,sig,un;       //temporary doubles
    std::vector<double> u(n,0); //vector for temporary derivatives
    typename tpointList<T>::const_iterator it,itp,itn,it1El,
      it2El,itLast,itsecLast;

    secondDerivatives.resize(n,0); //the vector saves n second derivatives

    it1El = in.begin();             //iterator on the first element
    it2El = it1El; it2El++;         //iterator on the second element
    itLast = in.end();  itLast--;   //iterator on the last element
    itsecLast = itLast; itsecLast--;//iterator on the second last element

    if (natural == true){ // in NR this is made by the value of yp1 >
                          // value 0.99e30
      //The lower boundary condition is set either to be "natural"
      secondDerivatives[0]= 0.0;
      u[0]= 0.0;
    }
    else {
      // or else to have a specified first derivative.
      secondDerivatives[0]= -0.5;
      x1 = (*it1El).x;  y1 = (*it1El).y;
      x2 = (*it2El).x;  y2 = (*it2El).y;

      u[0] = (3.0/(x2-x1)) * (((y2-y1)/(x2-x1))-yp1);
    }

    i=1;
    for (it=it2El;it!=itLast;it++, i++) {
      //This is the decomposition loop of the tridiagonal al-
      //gorithm. secondDerivatives and u are used for temporary storage of
      //the decomposed factors.
      itp = it; itp--;    //iterator on the previous element
      itn = it;itn++;     //iterator on the next element

      x1 = (*itp).x;  y1 = (*itp).y; //the point before
      x2 = (*it).x;   y2 = (*it).y;  //the point
      x3 = (*itn).x;  y3 = (*itn).y; //the point after

      sig=(x2-x1)/(x3-x1);
      p=(sig*secondDerivatives[i-1])+2.0;

      secondDerivatives[i]= (sig-1.0)/p;
      u[i]= (y3-y2)/(x3-x2) - (y2-y1)/(x2-x1);
      u[i]= (6.0*u[i]/(x3-x1)-sig*u[i-1])/p;
    }

    if (natural == true) { //in NR this is made by the value of ypn >
                           //value 0.99e30
      //The upper boundary condition is set either to be "natural"
      qn=0.0;
      un=0.0;
    }
    else {
      //or else to have a specified first derivative.
      qn=0.5;
      x1 = (*itsecLast).x;
      y1 = (*itsecLast).y; //x1,y1 are the second last point
      x2 = (*itLast).x;
      y2 = (*itLast).y;    //x2,y2 are the last point
      un = (3.0/(x2-x1)) * (ypn-(y2-y1)/(x2-x1));
    }
    secondDerivatives[n-1]=
      (un-(qn*u[n-2]))/((qn*secondDerivatives[n-2])+1.0);
    for (i=n-2;i>=0;i--) {
      //This is the backsubstitution loop of the tridiagonal algorithm.
      secondDerivatives[i]=(secondDerivatives[i]*secondDerivatives[i+1]) +u[i];
    }
  }

  template <class T>
  bool cubicSpline<T>::splint(const tpointList<T> &function,
                              const T& x, T& y) const {
    // Given the arrays xa[1..n] and ya[1..n], which tabulate a
    // function (with the xai's in order), and given the array
    // secondDerivatives[1..n], which is the output from spline above,
    // and given a value of x, this routine returns a cubic-spline
    // interpolated value y.

    typename tpointList<T>::const_iterator itLo,itHi,itLast;
    double h,b,a;
    T x1,x2,y1,y2;
    int lo=0;
    int hi=0;
    itLast = function.end(); itLast--;

    for(itLo=function.begin();itLo!=itLast;itLo++, lo++){
      //go throuh the entire list
      itHi = itLo;
      itHi++;
      hi = lo+1;
      if(((*itHi).x >= x)&&((*itLo).x<=x)){
        break;
      }
    } //itLo and itHi now bracket the input value of x.

    if( (*itHi).x == (*itLo).x ) {
      //if the required interpolation point is matching one base point
      //return this base point
      y = (*itHi).y;
    } else {
      //if not make the interpolation
      x1 = (*itLo).x; y1 = (*itLo).y;
      x2 = (*itHi).x; y2 = (*itHi).y;

      h=x2-x1;
      if(h==0) h= 0.1e-100; //if h == 0, its impossible but i am paranoid

      a=(x2-x)/h;
      b=(x-x1)/h; //Cubic spline polynomial is now evaluated.
      y=static_cast<T>(a*y1+b*y2+((a*a*a-a)*secondDerivatives[lo]+
                                  (b*b*b-b)*secondDerivatives[hi])*(h*h)/6.0);
    }
    return true;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------


  // On place apply for type tpointList<double>!
  template <class T>
  bool cubicSpline<T>::apply(tpointList<T>& srcdest) const {

    T fx;
    bool success = false;
    const parameters pa = getParameters();
    typename tpointList<T>::iterator it;
    for(it=srcdest.begin();it!=srcdest.end();it++){
      //go along the pointlist and calculate for each point
      success = splint(pa.samplingPoints,(*it).x,fx);
      (*it).y = fx;
    }

    return success;
  };

  // On copy apply for type double!
  template <class T>
  bool cubicSpline<T>::apply(const T& x,T& fx) const {

    const parameters pa = getParameters();
    return splint(pa.samplingPoints,x,fx);
  };

}


