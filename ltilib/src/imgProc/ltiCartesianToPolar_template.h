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
 * file .......: ltiCartesianToPolar.cpp
 * authors ....: Bernd Mussmann
 * organization: LTI, RWTH Aachen
 * creation ...: 18.8.2000
 * revisions ..: $Id: ltiCartesianToPolar_template.h,v 1.14 2004/05/06 10:19:58 alvarado Exp $
 */


namespace lti {
  // --------------------------------------------------
  // cartesianToPolar::parameters
  // --------------------------------------------------

  // implementation in header file due to MS VC++ bug

  // --------------------------------------------------
  // cartesianToPolar
  // --------------------------------------------------

  // default constructor
  template<class T>
    cartesianToPolar<T>::cartesianToPolar()
    : transform(){

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the Default parameters
    setParameters(defaultParameters);
  }

  // constructs an instance with the given parameters
  template<class T>
    cartesianToPolar<T>::cartesianToPolar(const parameters& thePars)
      : transform(){
    // set the parameters
    setParameters(thePars);
  }

  // constructs an instance which uses the arctan lookup table
  template<class T>
    cartesianToPolar<T>::cartesianToPolar(bool useLUT)
      : transform() {
    parameters params;
    params.useLUT = useLUT;
    // set the parameters
    setParameters(params);
  }


  // copy constructor
  template<class T>
  cartesianToPolar<T>::cartesianToPolar(const cartesianToPolar& other)
    : transform() {
    copy(other);
  }

  // destructor
  template<class T>
  cartesianToPolar<T>::~cartesianToPolar() {
  }

  // returns the name of this type
  template<class T>
  const char* cartesianToPolar<T>::getTypeName() const {
    return "cartesianToPolar";
  }

  // copy member
  template<class T>
  cartesianToPolar<T>&
    cartesianToPolar<T>::copy(const cartesianToPolar& other) {
      transform::copy(other);
    return (*this);
  }

  // clone member
  template<class T>
  functor* cartesianToPolar<T>::clone() const {
    return new cartesianToPolar(*this);
  }

  // return parameters
  template<class T>
  const typename cartesianToPolar<T>::parameters&
  cartesianToPolar<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(getTypeName());
    }
    return *par;
  }

  // ---------------------------------------------------------------------
  // internal template class for doing all the work
  // all other apply methods are just facades
  // --------------------------------------------------------------------
  template <class T, class U>
  class ltiP2CWorker {
  public:

    // On copy apply for type matrix<T>!
    static bool apply( const matrix<T>& real,
                       const matrix<T>& imaginary,
                             matrix<T>& absvalue,
                             matrix<T>& phase) {

      if  (real.size() != imaginary.size()) {
        absvalue.clear();
        phase.clear();
        return false;
      }

      U myAtan;

      // creation and initialzation of new matrix
      absvalue.resize(real.size(),T(),false,false);
      phase.resize(real.size(),T(),false,false);

      typename lti::matrix<T>::eStoreMode Mode=real.getMode();
      typename lti::matrix<T>::eStoreMode Mode2=imaginary.getMode();
      //if one matrix mode not the connected mode then
      // the computation will be in the line mode
      if(Mode != Mode2) Mode = lti::matrix<T>::Line;

      // switch if the Matrix in the line or connected mode
      switch (Mode){
        //Matrix in the line mode
          case lti::matrix<T>::Line:{
            const int n=real.rows();
            //Iterators for line Mode
            typename vector<T>::const_iterator re,im,ere;
            typename matrix<T>::iterator ab,ph;

            ab = absvalue.begin();
            ph = phase.begin();
            //computing of the absolute value and the phase
            for (int y=0;y<n;++y) {
              for (re=real.getRow(y).begin(),
                     ere=real.getRow(y).end(),im=imaginary.getRow(y).begin();
                   re!=ere;
                   ++re,++im,++ab,++ph) {
                *ab = sqrt((*re)*(*re)+(*im)*(*im));
                *ph = myAtan((*im),(*re));
              }
            }
            break;
          }
            //Matrix in the connected mode
          case lti::matrix<T>::Connected: {
            //Iterators for connected Mode
            typename matrix<T>::const_iterator re,im,ere;
            typename matrix<T>::iterator ab,ph;
            
            ab = absvalue.begin();
            ph = phase.begin();
            //computing of the absolute value and the phase
            for (re=real.begin(),ere=real.end(),im=imaginary.begin();
                 re!=ere;
                 ++re,++im,++ab,++ph) {
              *ab = sqrt((*re)*(*re)+(*im)*(*im));
              *ph = myAtan((*im),(*re));
            }
            break;
          }
          default: {
            return false;
            break;
          }
      }
      return true;
    }


    // On copy apply for type vector<T>!
    static bool apply( const vector<T>& real,
                       const vector<T>& imaginary,
                             vector<T>& absvalue,
                             vector<T>& phase) {
      if  (real.size() != imaginary.size()) {
        absvalue.clear();
        phase.clear();
        return false;
      }

      // instanziate
      U myAtan;

      // creation and initialzation of new matrix
      absvalue.resize(real.size(),T(),false,false);
      phase.resize(real.size(),T(),false,false);

      typename vector<T>::const_iterator re,im,ere;
      typename vector<T>::iterator ab,ph;

      ab = absvalue.begin();
      ph = phase.begin();
      //computing of the absolute value and the phase
      for (re=real.begin(),ere=real.end(),im=imaginary.begin();
           re!=ere; ++re,++im,++ab,++ph) {
        *ab = sqrt((*re)*(*re)+(*im)*(*im));
        *ph = myAtan((*im),(*re));
      }
      return true;
    }


    static bool apply(const tpointList<T>& src, tpointList<double> &dest, const tpoint<double> origin) {
      dest.clear();

      U myAtan;

      tpoint<double> tmp;
      tpoint<double> zp;
      typename tpointList<T>::const_iterator itc;

      for(itc=src.begin(); itc != src.end(); itc++){
        zp.x = (*itc).x - origin.x;
        zp.y = (*itc).y - origin.y;
        tmp.x = sqrt(zp.x*zp.x + zp.y*zp.y);
        tmp.y = myAtan(zp.y,zp.x);
        dest.push_back(tmp);
      }
      return true;
    }

  }; // class ltiP2CWorker


  // ---------------------------------------------------------------------
  // internal wrapper class for the normal arctan function
  // ---------------------------------------------------------------------
  class ltiP2Catan {
  public:
    float operator() (float dy, float dx) {
      return atan2(dy,dx);
    }
    double operator() (double dy, double dx) {
      return atan2(dy,dx);
    }
  };


  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------


  // On place apply for type matrix<T>!
  template<class T>
  bool cartesianToPolar<T>::apply(matrix<T>& r_abs, matrix<T>& i_ph)  {

    lti::matrix<T> temp1,temp2;
    bool rc = apply(r_abs,i_ph,temp1,temp2);
    temp1.detach(r_abs);
    temp2.detach(i_ph);
    return rc;
  }

  // On place apply for type matrix<T>!
  template<class T>
  bool cartesianToPolar<T>::apply(vector<T>& r_abs, vector<T>& i_ph)  {

    lti::vector<T> temp1,temp2;
    bool rc = apply(r_abs,i_ph,temp1,temp2);
    temp1.detach(r_abs);
    temp2.detach(i_ph);
    return rc;
  }

  // On copy apply for type matrix<T>!
  template<class T>
  bool cartesianToPolar<T>::apply( const matrix<T>& real,
                                   const matrix<T>& imaginary,
                                   matrix<T>& absvalue,
                                   matrix<T>& phase) {
    // distinguish between two methods to calculate arctan
    if (getParameters().useLUT)
      return ltiP2CWorker<T, arctanLUT>::apply(real,imaginary,absvalue,phase);
    else 
      return ltiP2CWorker<T, ltiP2Catan>::apply(real,imaginary,absvalue,phase);
  }

  // On copy apply for type vector<T>!
  template<class T>
  bool cartesianToPolar<T>::apply( const vector<T>& real,
                                   const vector<T>& imaginary,
                                   vector<T>& absvalue,
                                   vector<T>& phase) {

    // distinguish between two methods to calculate arctan
    if (getParameters().useLUT)
      return ltiP2CWorker<T, arctanLUT>::apply(real,imaginary,absvalue,phase);
    else 
      return ltiP2CWorker<T, ltiP2Catan>::apply(real,imaginary,absvalue,phase);

  }
  

  // on copy apply for tpointlist<T> spezialized for double results
  template<class T>
  bool cartesianToPolar<T>::apply(const tpointList<T>& src, tpointList<double> &dest, const tpoint<double> origin)const{

    // this apply is only available with the normal atan function
    return ltiP2CWorker<T, ltiP2Catan>::apply(src,dest,origin);
  }

}


