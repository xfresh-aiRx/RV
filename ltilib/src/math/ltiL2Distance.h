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
 * file .......: ltiL2Distance.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 28.6.2000
 * revisions ..: $Id: ltiL2Distance.h,v 1.11 2004/05/03 12:20:01 alvarado Exp $
 */

#ifndef _LTI_L2_DISTANCE_H_
#define _LTI_L2_DISTANCE_H_

#include "ltiPoint.h"
#include "ltiRGBPixel.h"
#include <vector>
#include "ltiDistanceFunctor.h"
#include "ltiDistanceType.h"

namespace lti {

  /**
   * @name Square of Euclidian Distances Functions.
   *
   * Global functions to compute the distance between two n-dimensional
   * point representations
   *
   * @ingroup gLinearAlgebra
   */
  //@{

  /**
   * distanceSqr computes the square of the euclidian distance between 
   * the lti::vectors a and b.
   */
  template<class T>
  typename distanceType<T>::square_distance_type
  distanceSqr(const vector<T>& a, const vector<T>& b) {
    typename vector<T>::const_iterator ita,itb,ite;
    assert(a.size()==b.size());
    typename distanceType<T>::square_distance_type sum(0);
    T dif;
    for (ita=a.begin(),itb=b.begin(),ite=a.end();
         ita!=ite;
         ++ita,++itb) {
      dif = (*ita)-(*itb);
      sum+=static_cast<typename distanceType<T>::square_distance_type>
        (dif*dif);
    }
    return sum;
  }

  /**
   * distanceSqr computes the square of the euclidian distance between 
   * the lti::matrix a and b.
   */
  template<class T>
  typename distanceType<T>::square_distance_type
  distanceSqr(const matrix<T>& a, const matrix<T>& b) {
    typename matrix<T>::const_iterator ita,itb,ite;
    assert(a.size()==b.size());
    typename distanceType<T>::square_distance_type sum(0);
    T dif;
    for (ita=a.begin(),itb=b.begin(),ite=a.end();
         ita!=ite;
         ++ita,++itb) {
      dif = (*ita)-(*itb);
      sum+=static_cast<typename distanceType<T>::square_distance_type>
        (dif*dif);
    }
    return sum;
  }
  
  /**
   * distanceSqr computes the square of the euclidian distance between 
   * the std::vectors a and b.
   */
  template<class T>
  typename distanceType<T>::square_distance_type
  distanceSqr(const std::vector<T>& a, const std::vector<T>& b) {
    typename std::vector<T>::const_iterator ita,itb,ite;
    assert(a.size()==b.size());
    typename distanceType<T>::square_distance_type sum(0);
    T dif;
    for (ita=a.begin(),itb=b.begin(),ite=a.end();
         ita!=ite;
         ++ita,++itb) {
      dif = (*ita)-(*itb);
      sum+=static_cast<typename distanceType<T>::square_distance_type>
        (dif*dif);
    }
    return sum;
  }


  /**
   * distanceSqr computes the square of the euclidian distance between 
   * the points a and b.
   */
  template<class T>
  inline typename distanceType<T>::square_distance_type
  distanceSqr(const tpoint<T>& a, const tpoint<T>& b) {
    return static_cast<typename distanceType<T>::square_distance_type>
      (a.distanceSqr(b));
  }
  
  /**
   * distanceSqr computes the square of the euclidian distance between 
   * the points a and b.
   */
  template<class T>
  inline typename distanceType<T>::square_distance_type
  distanceSqr(const tpoint3D<T>& a, const tpoint3D<T>& b) {
    return static_cast<typename distanceType<T>::square_distance_type>
      (a.distanceSqr(b));
  }

  /**
   * distanceSqr computes the square of the euclidian distance between 
   * the RGB values a and b in the RGB color space.
   */
  template<class T>
  inline typename distanceType<T>::square_distance_type
  distanceSqr(const trgbPixel<T>& a, const trgbPixel<T>& b) {
    return static_cast<typename distanceType<T>::square_distance_type>
      (a.distanceSqr(b));
  }

  /**
   * distanceSqr computes the square of the euclidian distance between 
   * the RGB values a and b in the RGB color space.
   */
  inline distanceType<rgbPixel>::square_distance_type
  distanceSqr(const rgbPixel& a, const rgbPixel& b) {
    return static_cast<distanceType<rgbPixel>::square_distance_type>
      (a.distanceSqr(b));
  }
  //@}

  /**
   * Policy class used by several classifiers/trees to measure the square
   * of the euclidian distance between two points of type T.
   *   
   * The type T MUST define the \a value_type type, which exist for example
   * for vectors, (t)points and (t)rgbPixels.
   *
   * If you \e really want, you can use this class directly through the
   * operator().
   *
   * \code
   * l2SquareDistantor< vector<double> > myDist;
   * dvector a,b;
   * ... // fill vectors with data
   * double dist = myDist(a,b);
   * \endcode
   * 
   * But remember that there is also a global template function distanceSqr()
   * which is easier to use:
   *
   * \code
   * dvector a,b;
   * ... // fill vectors with data
   * double dist = distanceSqr(a,b);
   * \endcode
   *
   * You can of course also use the lti::l2Distance functor, as usual with its
   * apply() methods.
   */
#ifdef _LTI_MSC_6
  template <class T, class D=distanceType<T>::square_distance_type>
#else
  template <class T, class D=typename distanceType<T>::square_distance_type>
#endif
  class l2SquareDistantor {
  public:
    /**
     * type returned by the distantor
     */
    typedef D distance_type;

    /**
     * compute the distance between a and b
     */
    inline distance_type operator()(const T& a,const T& b) const {
      return 
        static_cast<distance_type>(distanceSqr(a,b));
    };

    /**
     * @name Special methods for Minkowski distances
     */
    //@{

    /**
     * This member accumulates in the given accumulator, the
     * given element.
     *
     * It can be used when the distance need to be computed manually, but
     * using a distantor to still allow the flexibility of changing distances.
     *
     * For the l2SquareDistantor this is just acc+=(elem*elem).
     *
     * @param element component of the difference between two points.
     * @param accumulator variable where the elements will be accumulated.
     */
    inline void accumulate(const distance_type element,
                           distance_type& accumulator) const {
      accumulator+=(element*element);
    }

    /**
     * This member accumulates in the given accumulator, the difference 
     * of the given elements.
     *
     * It can be used when the distance need to be computed manually, but
     * using a distantor to still allow the flexibility of changing distances.
     *
     * For the l2SquareDistantor this is just acc+=((elem1-elem2)^2).
     *
     * @param element1 component of the first point
     * @param element2 component of the second point
     * @param accumulator variable where the elements will be accumulated.
     */
    inline void accumulate(const typename T::value_type element1,
                           const typename T::value_type element2,
                           distance_type& accumulator) const {
      const distance_type tmp = static_cast<distance_type>(element2-element1);
      accumulator+=(tmp*tmp);
    }

    /**
     * Compute from the given accumulator the desired distance
     */
    inline distance_type 
    computeDistance(const distance_type& accumulator) const {
      return accumulator;
    }

    /**
     * return the distance between two components, which is in some way
     * a component of the total distance (that is the reason for the name).
     *
     * For this distantor it return (element2-element1)^2
     */
    inline distance_type 
    component(const typename T::value_type element1,
              const typename T::value_type element2) const {
      const distance_type tmp = static_cast<distance_type>(element2-element1);
      return tmp*tmp;
    }

    /**
     * Return true if the given partial computed from accumulator is less than
     * the given distance.
     *
     * Assume you have accumulated \a acc until now, and you want to check if
     * the partial distance derived from this accumulator is less than
     * the given distance.  So you check accLessThan(accumulator,distance)
     * 
     * For this norm it computes acc < dist
     */
    inline bool accLessThan(const distance_type acc,
                            const distance_type dist) const {
      return (acc < dist);
    }
    
    /**
     * Return true if the given partial computed from accumulator is greater
     * than the given distance.
     *
     * Assume you have accumulated \a acc until now, and you want to check if
     * the partial distance derived from this accumulator is greater than
     * the given distance.  So you check accLessThan(accumulator,distance)
     * 
     * For this norm it computes acc > dist
     */
    inline bool accGreaterThan(const distance_type acc,
                               const distance_type dist) const {
      return (acc > dist);
    }
    //@}
  };

  // specialization for rgbPixels
//   template <>
//   class l2SquareDistantor<rgbPixel> {
//   public:
//     /*
//      * type returned by the distantor
//      */
//     typedef int distance_type;

//     /*
//      * compute the distance between a and b
//      */
//     inline distance_type operator()(const rgbPixel& a,
//                                     const rgbPixel& b) const {
//       return distanceSqr(a,b);
//     } 

//     /**
//      * @name Special methods for Minkowski distances
//      */
//     //@{

//     /**
//      * This member accumulates in the given accumulator, the
//      * given element.
//      *
//      * It can be used when the distance need to be computed manually, but
//      * using a distantor to still allow the flexibility of changing distances.
//      *
//      * For the l2SquareDistantor this is just acc+=(elem*elem).
//      *
//      * @param element component of the difference between two points.
//      * @param accumulator variable where the elements will be accumulated.
//      */
//     inline void accumulate(const int element,
//                            distance_type& accumulator) const {
//       accumulator+=(element*element);
//     }

//     /**
//      * This member accumulates in the given accumulator, the difference 
//      * of the given elements.
//      *
//      * It can be used when the distance need to be computed manually, but
//      * using a distantor to still allow the flexibility of changing distances.
//      *
//      * For the l2SquareDistantor this is just acc+=((elem2-elem1)^2).
//      *
//      * @param element1 component of the first point
//      * @param element2 component of the second point
//      * @param accumulator variable where the elements will be accumulated.
//      */
//     inline void accumulate(const ubyte element1,
//                            const ubyte element2,
//                            distance_type& accumulator) const {
//       const int tmp = static_cast<int>(element2)-static_cast<int>(element1);
//       accumulator+=(tmp*tmp);
//     }

//     /**
//      * Compute from the given accumulator the desired distance
//      */
//     inline distance_type 
//     computeDistance(const distance_type& accumulator) const {
//       return accumulator;
//     }

//     /**
//      * return the distance between two components, which is in some way
//      * a component of the total distance (that is the reason for the name).
//      *
//      * For this distantor it return (element2-element1)^2
//      */
//     inline distance_type 
//     component(const ubyte element1,
//               const ubyte element2) const {
//       const distance_type tmp = 
//         static_cast<distance_type>(element2) - 
//         static_cast<distance_type>(element1);
//       return tmp*tmp;
//     }

//     /**
//      * Return true if the given partial computed from accumulator is less than
//      * the given distance.
//      *
//      * Assume you have accumulated \a acc until now, and you want to check if
//      * the partial distance derived from this accumulator is less than
//      * the given distance.  So you check accLessThan(accumulator,distance)
//      * 
//      * For this norm it computes acc < (dist*dist)
//      */
//     inline bool accLessThan(const distance_type acc,
//                             const distance_type dist) const {
//       return (acc < (dist*dist));
//     }
    
//     /**
//      * Return true if the given partial computed from accumulator is greater
//      * than the given distance.
//      *
//      * Assume you have accumulated \a acc until now, and you want to check if
//      * the partial distance derived from this accumulator is less than
//      * the given distance.  So you check accLessThan(accumulator,distance)
//      * 
//      * For this norm it computes acc > (dist*dist)
//      */
//     inline bool accGreaterThan(const distance_type acc,
//                                const distance_type dist) const {
//       return (acc > (dist*dist));
//     }
//     //@}
//   };

  /**
   * Policy class used by several classifiers/trees to measure the 
   * euclidian distance between two points of type T.
   *   
   * The type T MUST define the \a value_type type, which exist for example
   * for vectors, (t)points and (t)rgbPixels.
   *
   * If you \e really want, you can use this class directly through the
   * operator().
   *
   * \code
   * l2Distantor< vector<double> > myDist;
   * dvector a,b;
   * ... // fill vectors with data
   * double dist = myDist(a,b);
   * \endcode
   * 
   * But remember that there is also a global template function distanceSqr()
   * which can be more easily used:
   *
   * \code
   * dvector a,b;
   * ... // fill vectors with data
   * double dist = sqrt(distanceSqr(a,b));
   * \endcode
   *
   * You can of course also use the lti::l2Distance functor, as usual with its
   * apply() methods.
   */
#ifdef _LTI_MSC_6
  template <class T, class D=distanceType<T>::fp_distance_type>
#else
  template <class T, class D=typename distanceType<T>::fp_distance_type>
#endif
  class l2Distantor {
  public:
    /**
     * type returned by the distantor
     */
    typedef D distance_type;

    /**
     * compute the distance between a and b
     */
    inline distance_type operator()(const T& a,const T& b) const {
      return sqrt(static_cast<distance_type>(distanceSqr(a,b)));
    }

    /**
     * @name Special methods for Minkowski distances
     */
    //@{

    /**
     * This member accumulates in the given accumulator, the
     * given element.
     *
     * It can be used when the distance need to be computed manually, but
     * using a distantor to still allow the flexibility of changing distances.
     *
     * For the l2Distantor this is just acc+=(elem*elem).
     *
     * @param element component of the difference between two points.
     * @param accumulator variable where the elements will be accumulated.
     */
    inline void accumulate(const distance_type element,
                           distance_type& accumulator) const {
      accumulator+=(element*element);
    }

    /**
     * This member accumulates in the given accumulator, the difference 
     * of the given elements.
     *
     * It can be used when the distance need to be computed manually, but
     * using a distantor to still allow the flexibility of changing distances.
     *
     * For the l2Distantor this is just acc+=((elem2-elem1)^2).
     *
     * @param element1 component of the first point
     * @param element2 component of the second point
     * @param accumulator variable where the elements will be accumulated.
     */
    inline void accumulate(const typename T::value_type element1,
                           const typename T::value_type element2,
                           distance_type& accumulator) const {
      const distance_type tmp = static_cast<distance_type>(element2-element1);
      accumulator+=(tmp*tmp);
    }

    /**
     * Compute from the given accumulator the desired distance
     */
    inline distance_type 
    computeDistance(const distance_type& accumulator) const {
      return sqrt(accumulator);
    }
 
    /**
     * return the distance between two components, which is in some way
     * a component of the total distance (that is the reason for the name).
     *
     * For this distantor it return abs(element2-element1)
     */
    inline distance_type 
    component(const typename T::value_type element1,
              const typename T::value_type element2) const {
      return static_cast<distance_type>(abs(element2-element1));
    }

    /**
     * Return true if the given partial computed from accumulator is less than
     * the given distance.
     *
     * Assume you have accumulated \a acc until now, and you want to check if
     * the partial distance derived from this accumulator is less than
     * the given distance.  So you check accLessThan(accumulator,distance)
     * 
     * For this norm it computes acc < (dist*dist)
     */
    inline bool accLessThan(const distance_type acc,
                            const distance_type dist) const {
      return (acc < (dist*dist));
    }
    
    /**
     * Return true if the given partial computed from accumulator is greater
     * than the given distance.
     *
     * Assume you have accumulated \a acc until now, and you want to check if
     * the partial distance derived from this accumulator is less than
     * the given distance.  So you check accLessThan(accumulator,distance)
     * 
     * For this norm it computes acc > (dist*dist)
     */
    inline bool accGreaterThan(const distance_type acc,
                               const distance_type dist) const {
      return (acc > (dist*dist));
    }
    //@}
  };

//   template <>
//   class l2Distantor<rgbPixel> {
//   public:
//     /*
//      * type returned by the distantor
//      */
//     typedef int distance_type;

//     /*
//      * compute the distance between a and b
//      */
//     inline distance_type operator()(const rgbPixel& a,
//                                     const rgbPixel& b) const {
//       return sqrt(distanceSqr(a,b));
//     } 

//     /**
//      * @name Special methods for Minkowski distances
//      */
//     //@{

//     /**
//      * This member accumulates in the given accumulator, the
//      * given element.
//      *
//      * It can be used when the distance need to be computed manually, but
//      * using a distantor to still allow the flexibility of changing distances.
//      *
//      * For the l2Distantor this is just acc+=(elem*elem).
//      *
//      * @param element component of the difference between two points.
//      * @param accumulator variable where the elements will be accumulated.
//      */
//     inline void accumulate(const int element,
//                            distance_type& accumulator) const {
//       accumulator+=(element*element);
//     }

//     /**
//      * This member accumulates in the given accumulator, the difference 
//      * of the given elements.
//      *
//      * It can be used when the distance need to be computed manually, but
//      * using a distantor to still allow the flexibility of changing distances.
//      *
//      * For the l2Distantor this is just acc+=((elem2-elem1)^2).
//      *
//      * @param element1 component of the first point
//      * @param element2 component of the second point
//      * @param accumulator variable where the elements will be accumulated.
//      */
//     inline void accumulate(const ubyte element1,
//                            const ubyte element2,
//                            distance_type& accumulator) const {
//       const distance_type tmp = 
//         static_cast<int>(element2)-static_cast<int>(element1);
//       accumulator+=(tmp*tmp);
//     }

//     /**
//      * Compute from the given accumulator the desired distance
//      */
//     inline distance_type 
//     computeDistance(const distance_type& accumulator) const {
//       return sqrt(accumulator);
//     }

//     /**
//      * return the distance between two components, which is in some way
//      * a component of the total distance (that is the reason for the name).
//      *
//      * For this distantor it return abs(element2-element1)
//      */
//     inline distance_type 
//     component(const ubyte element1,
//               const ubyte element2) const {
//       return abs(static_cast<distance_type>(element2) - 
//                  static_cast<distance_type>(element1));
//     }

//     /**
//      * Return true if the given partial computed from accumulator is less than
//      * the given distance.
//      *
//      * Assume you have accumulated \a acc until now, and you want to check if
//      * the partial distance derived from this accumulator is less than
//      * the given distance.  So you check accLessThan(accumulator,distance)
//      * 
//      * For this norm it computes acc < (dist*dist)
//      */
//     inline bool accLessThan(const distance_type acc,
//                             const distance_type dist) const {
//       return (acc < (dist*dist));
//     }
    
//     /**
//      * Return true if the given partial computed from accumulator is greater
//      * than the given distance.
//      *
//      * Assume you have accumulated \a acc until now, and you want to check if
//      * the partial distance derived from this accumulator is less than
//      * the given distance.  So you check accLessThan(accumulator,distance)
//      * 
//      * For this norm it computes acc > (dist*dist)
//      */
//     inline bool accGreaterThan(const distance_type acc,
//                                const distance_type dist) const {
//       return (acc > (dist*dist));
//     }

//     //@}
//   };

  /**
   * This class computes the L2 distance between two vectors or matrices
   * or the L2 norm of a vector.
   *
   * For other aggregate classes like points and rgbPixels, you can use the
   * global functions lti::distanceSqr(), which return the square of the
   * euclidian distance (L2 distance) between the two n-dimensional point
   * representations.  That function exists also for vectors to provide a
   * unified interface.
   *
   * @ingroup gLinearAlgebra
   */
  template<class T>
  class l2Distance : public distanceFunctor<T> {
  public:
    /**
     * default constructor
     */
    l2Distance();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    l2Distance(const l2Distance& other);

    /**
     * destructor
     */
    virtual ~l2Distance();

    /**
     * returns the name of this type ("l2Distance")
     */
    virtual const char* getTypeName() const;

    /**
     * calculate the norm of vector v
     * @param v the vector<T>
     * @param norm the norm of the vectors
     * @return false on error
     */
    virtual bool apply(const vector<T>& v, T& norm) const;

    /**
     * calculate the norm of vector v
     * @param v the vector<T>
     * @return the norm of the vector
     */
    virtual T apply(const vector<T>& v) const;

    /**
     * calculate the norms of rows or columns of the matrix
     * @param m the matrix<T>
     * @param norms the norms of the rows/columns
     * @return false on error
     */
    virtual bool apply(const matrix<T>& m, vector<T>& norms) const;

    /**
     * calculate something like the norm of the matrix: the matrix
     * is seen as a vector.
     * @param m the matrix<T>
     * @param norm the 'norm' of the matrix
     * @return false on error
     */
    virtual bool apply(const matrix<T>& m, T& norm) const;

    /**
     * calculate something like the norm of the matrix: the matrix
     * is seen as a vector.
     * @param m the matrix<T>
     * @return the 'norm' of the matrix
     */
    virtual T apply(const matrix<T>& m) const;

    /**
     * calculate the distance between the vectors a and b
     * @param a the first vector<T>
     * @param b the second vector<T>
     * @param dist the distance between the vectors
     * @return false on error -> see status string
     */
    virtual bool apply(const vector<T>& a, const vector<T>& b,
                       T& dist) const;

    /**
     * calculates the L2 distance between the vectors a and b
     * If both vectors have different sizes, the returned value will be
     * negative!
     * @param a the first vector<T>
     * @param b the second vector<T>
     * @return the L2 distance between a and b
     */
    virtual T apply(const vector<T>& a, const vector<T>& b) const;

    /**
     * calculate the distances between the rows or columns of the
     * matrices a and b, determined by the parameters rowWise.
     * @param a the first vector<T>
     * @param b the second vector<T>
     * @param dists the distances between the matrices
     * @return false on error -> see status string
     */
    virtual bool apply(const matrix<T>& a, const matrix<T>& b,
                       vector<T>& dists) const;

    /**
     * calculate the L2 distance between the matrices a and b
     * If both matrices have different sizes, the returned value will be
     * negative!
     * @param a the first matrix<T>
     * @param b the second matrix<T>
     * @return the L2 distance between a and b
     */
    virtual T apply(const matrix<T>& a, const matrix<T>& b) const;

    /**
     * calculate the L2 distance between the matrices a and b
     * If both matrices have different sizes, the returned value will be
     * negative!
     * @param a the first matrix<T>
     * @param b the second matrix<T>
     * @param dist the L2 distance between a and b
     * @return false on error
     */
    virtual bool apply(const matrix<T>& a, const matrix<T>& b,
                       T& dist) const;

    /**
     * Calculate the distance between each row or column of m
     * depending on the value of rowWise and the vector v.
     * @param m the matrix<T>
     * @param v the vector to be compared with
     * @param dest the vector with the distances to the vector v
     * @return false on error
     */
    virtual bool apply(const matrix<T>& m, const vector<T>& v,
                       vector<T>& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    l2Distance& copy(const l2Distance& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    typedef typename distanceFunctor<T>::parameters parameters;

  };

}

#endif

