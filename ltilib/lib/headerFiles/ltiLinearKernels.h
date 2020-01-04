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
 * file .......: ltiLinearKernels.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 28.04.00
 * revisions ..: $Id: ltiLinearKernels.h,v 1.26 2005/06/16 12:35:11 doerfler Exp $
 */

#ifndef _LTI_LINEARKERNELS_H_
#define _LTI_LINEARKERNELS_H_

#include <vector>
#include "ltiArray.h"
#include "ltiMatrix.h"
#include "ltiTypes.h"
#include "ltiTypeInfo.h"

namespace lti {

  /**
   * one-dimensional filter kernel
   *
   * The template type of this class should coincide with the template
   * class of the vector be convolved with.  For example, if you want
   * to convolve a kernel1D with a vector<double>, you will need a
   * kernel1D<double> (@see lti::convolution).
   *
   * If you instantiate a kernel1D of a fixed point type, like
   * kernel1D<int> or kernel1D<ubyte>, you also need to consider the
   * "norm" of the kernel (see lti::kernel1D<T>::getNorm() and
   * lti::kernel1D<T>::setNorm(const T&)). This "norm" allows the
   * representation of numbers less than 1.0.  You can see this norm as the
   * value to be consider as 1.0, when operating with the kernel.
   * For example, if you have a kernel1D<ubyte> with the values [64,128,64]
   * and norm=255, the the interpreted values during convolution will be
   * [0.25,0.5,0.25].  With floating-point types, the norm will be always
   * assumed to be 1.0.  For other types the default norms are the following:
   *
   * For int:              65536
   * For ubyte:              255
   * Otherwise:                1.0
   *
   * @see lti::convolution
   */
  template<class T>
  class kernel1D : public array<T> {
  public:
    /**
     * default constructor
     */
    kernel1D();
    /** construct a filter kernel indexed from <em>from</em> to <em>to</em> and
	initialized with the value <em>init</em> */
    kernel1D(const int& from,
	     const int& to,
	     const T& init=T());
    /** construct a kernel from a one dimensional vector
        @param other the source vector
        @param theOffset this is the index in the vector that corresponds to
                         the index '0' in the filter kernel
    */
    kernel1D(const vector<T>& other,const int& theOffset);

    /**
     * If \a init is true this constructor is equivalent to calling
     * kernel1D(const int& from, const int& to), and thus initializing
     * all elements with T(). However, in some cases the elements need
     * not be initialized during construction, since complex
     * initializion is required. Especially for large kernel1Ds, the
     * unnecessary constructor initialization is very time consuming.
     *
     * If \a init is false, memory is allocated but no initialization
     * takes place.
     *
     * @param init initialize matrix or not
     * @param from first index
     * @param to last index
     */
    kernel1D(const bool& init, const int& from, const int& to);

    /** copy constructor
        @param other the one dimensional kernel to be copied
    */
    kernel1D(const kernel1D& other);

    /**
     * destructor
     */
    virtual ~kernel1D();

    /**
     * returns name of this type ("kernel1D")
     */
    const char* getTypeName() const {return "kernel1D";};

    /**
     * copy member
     * @param other the one dimensional kernel to be copied
     * @return a reference to this instance
     */
    kernel1D& copy(const kernel1D& other);

    /**
     * clone member
     * @return a pointer to a copy of this object
     */
    virtual mathObject* clone() const {
      return (new kernel1D<T>(*this));
    }

    /**
     * copy from kernel of different type
     * @param other a one dimensional kernel of another type
     * @return a reference to this instance
     */
    template<class U>
    kernel1D& castFrom(const kernel1D<U>& other) {
      array<T>::castFrom(other);
      norm = T(other.getNorm());
      return (*this);
    };

    /**
     * copy the content of the other vector in this kernel and assign
     * the index (firstElement) to the first element of the vector.
     * For example if <code>other</code> is a 3 dimensional vector, then
     * <code>castFrom(other,-1)</code> is a 3-elements-kernel which indices
     * lay inside [-1,1].
     * @param other the vector with the data to be copied
     * @param firstElement index for the first element of the vector
     * @return a reference to this instance
     */
    kernel1D<T>& castFrom(const vector<T>& other,
                          const int& firstElement = 0);

    /**
     * get offset position of element 0
     * @return the position of the element with index 0 in the memory array.
     *         This value is the same as -firstIdx()
     */
    int getOffset() const;

    /**
     * set offset position of element 0
     * @return the position of the element with index 0 in the memory array.
     *         This value is the same as -firstIdx()
     */
    void setOffset(const int& theOffset);


    /**
     * get normalization factor
     *
     * The normalization factor is used by the fixed point types as a
     * representation of the value 1.  For example, the norm for a
     * kernel1D<ubyte> can be 255, if the filter kernel don't need values
     * greater than 1.0.
     */
    inline const T& getNorm() const {
      return norm;
    }

    /**
     * set normalization factor
     * @see getNorm()
     */
    inline void setNorm(const T& n) {
      norm=n;
    };

    /**
     * denormalize divide all elements by norm and set the norm to 1!
     */
    void denormalize();

    /**
     * Mirror the other kernel and leave the result here, i.e.
     * at(x) = other.at(-x);
     * @param other the kernel to be copied and then mirrored
     * @return a reference to this instance
     */
    kernel1D<T>& mirror(const kernel1D<T>& other);

    /**
     * Mirror this kernel, i.e.
     * at(y,x) = at(-y,-x);
     * @return a reference to this instance
     */
    kernel1D<T>& mirror();

    /**
     * write the object in the given ioHandler
     */
    virtual bool write(ioHandler& handler,const bool complete = true) const;

    /**
     * read the object from the given ioHandler
     */
    virtual bool read(ioHandler& handler,const bool complete = true);

  protected:
    /**
     * Normalisation factor.
     *
     * This value will be ignored for floating point formats.
     * For fixed point formats, this value corresponds to 1.0
     */
    T norm;
  };

  template <class T>
    class sepKernel;

  /**
   * two-dimensional filter kernel
   *
   * The template type of this class should coincide with the template
   * class of the matrix or channel to be convolved with.  For
   * example, if you want to convolve a kernel2D with a channel8, you
   * will need a kernel2D<channel8::value_type> or
   * kernel2D<ubyte>. The offset of the kernel is at
   * int(columns()/2),int(rows()/2). This means that for kernels of
   * odd width and height, the origin (0,0) is exactly in the center,
   * while for kernels with even width and/or height, it is shifted
   * towards the upper left corner.
   *
   * If you instantiate a kernel2D of a fixed point type, like
   * kernel2D<int> or kernel2D<ubyte>, you also need to consider the
   * "norm" of the kernel (see lti::kernel2D<T>::getNorm() and
   * lti::kernel2D<T>::setNorm(const T&)). This "norm" allows the
   * representation of numbers less than 1.0.  You can see this norm as the
   * value to be consider as 1.0, when operating with the kernel.
   * For example, if you have a kernel2D<ubyte> with the values [64,128,64]
   * and norm=255, the the interpreted values during convolution will be
   * [0.25,0.5,0.25].  With floating-point types, the norm will be always
   * assumed to be 1.0.  For other types the default norms are the following:
   *
   * For int:              65536
   * For ubyte:              255
   * Otherwise:                1.0
   *
   * You can generate 2D kernels from separable ones using the
   * castFrom() method.  For example:
   *
   *  \code
   *  lti::gaussKernel2D<float> gauss(5); // a sepKernel 5x5 
   *  lti::kernel2D<float>      kern;     // a 2D kernel;
   *  
   *  kern.castFrom(gauss);
   *  \endcode   
   *
   * You can also use the outerProduct member to construct a 2D kernel from
   * two 1D ones:
   *
   *  \code
   *  lti::gaussKernel1D<float> colKern(3); // a 1D kernel
   *  lti::gaussKernel1D<float> rowKern(5); // another 1D kernel
   *
   *  lti::kernel2D<float>      kern;     // a 2D kernel;
   *  
   *  kern.outerProd(colKern,rowKern);    // outer product of both kernels
   *  \endcode   
   *
   * @see lti::convolution
   */
  template<class T>
  class kernel2D : public matrix<T> {
  public:
    /**
     * default constructor
     */
    kernel2D();

    /**
     * copy constructor
     * @param other the kernel to be copied
     */
    kernel2D(const kernel2D& other);

    /**
     * Create a new kernel of indicated size and initial value. The offset is
     * kept at zero.
     *
     * @param rows number of rows
     * @param columns number of columns
     * @param iniValue initial value of all elements
     */
    kernel2D(const int& rows, const int& columns, const T& iniValue = T());

    /**
     * Create a new kernel with indicated indices.
     *
     * @param fromRow index of the lower row of the kernel
     * @param fromCol index of the lower column of the kernel
     * @param toRow index of the higher row of the kernel
     * @param toCol index of the higher column of the kernel
     * @param iniValue the initial value for the elements of the kernel
     *
     * You can initialize a 5x3 kernel to filter a lti::channel with
     * following code:
     *
     * \code
     * lti::kernel2D<channel::value_type> aKernel(-1,-2,1,2,0);
     * aKernel.at(-1,-2)=0.2;
     * aKernel.at(-1,-1)=0.2;
     * aKernel.at(0,0)=0.2;
     * aKernel.at(1,1)=0.2;
     * aKernel.at(1,2)=0.2;
     * \endcode
     */
    kernel2D(const int& fromRow,const int& fromCol,
             const int& toRow,const int& toCol,
             const T& iniValue = T());

    /**
     * If \a init is true this constructor is equivalent to calling
     * kernel2D(const int& rows, const int& columns), and thus initializing
     * all elements with T(). However, in some cases the elements need
     * not be initialized during construction, since complex
     * initializion is required. Especially for large kernel2Ds, the
     * unnecessary constructor initialization is very time consuming.
     *
     * If \a init is false, memory is allocated but no initialization
     * takes place.
     *
     * @param init initialize matrix or not
     * @param rows number of rows
     * @param columns number of columns
     */
    kernel2D(const bool& init, const int& rows, const int& columns);

    /**
     * If \a init is true this constructor is equivalent to calling
     * kernel2D(const int& fromRow,const int& fromCol, const int&
     * toRow,const int& toCol), and thus initializing all elements
     * with T(). However, in some cases the elements need not be
     * initialized during construction, since complex initializion is
     * required. Especially for large kernel2Ds, the unnecessary
     * constructor initialization is very time consuming.
     *
     * If \a init is false, memory is allocated but no initialization
     * takes place.
     *
     * @param init initialize matrix or not
     * @param fromRow index of the lower row of the kernel
     * @param fromCol index of the lower column of the kernel
     * @param toRow index of the higher row of the kernel
     * @param toCol index of the higher column of the kernel
     */
    kernel2D(const bool& init, const int& fromRow,const int& fromCol,
             const int& toRow,const int& toCol);

    /**
     * destructor
     */
    virtual ~kernel2D();

    /**
     * copy member
     * @param other the kernel to be copied
     * @return a reference to this instance
     */
    kernel2D<T>& copy(const kernel2D<T>& other);

    /**
     * returns name of this type ("kernel2D")
     */
    const char* getTypeName() const {return "kernel2D";};

    /**
     * clone member
     * @return a pointer to a copy of this object
     */
    virtual mathObject* clone() const {
      return (new kernel2D<T>(*this));
    };

    /**
     * copy from kernel of different type
     * @param other a one dimensional kernel of another type
     * @return a reference to this instance
     */
    template<class U>
    kernel2D<T>& castFrom(const kernel2D<U>& other) {
      // implementation needs to be here due to bug in VC++
      matrix<T>::castFrom(other);
      offset = other.getOffset();
      norm = static_cast<T>(other.getNorm());
      return (*this);
    };

    /**
     * copy the content of the other matrix in this kernel and assign
     * the index (firstRow,firstColumn) to the first element of the matrix.
     * For example if <code>other</code> is a 3x3 matrix, then
     * <code>castFrom(other,-1,-1)</code> is a 3x3 kernel which indices
     * lay inside [-1,1].
     * @param other the matrix with the data to be copied
     * @param firstRow index for the first row of the matrix
     * @param firstColumn index for the first column of the matrix
     * @return a reference to this instance
     */
    kernel2D<T>& castFrom(const matrix<T>& other,
                          const int& firstRow,
                          const int& firstColumn);

    /**
     * copy the content of the other matrix in this kernel and assign
     * the index (firstRow,firstColumn) to the first element of the matrix.
     * For example if <code>other</code> is a 3x3 matrix, then
     * <code>castFrom(other,-1,-1)</code> is a 3x3 kernel which indices
     * lay inside [-1,1].
     * @param other the matrix with the data to be copied
     * @param firstElement index for the first element of the matrix
     * @return a reference to this instance
     */
    kernel2D<T>& castFrom(const matrix<T>& other,
                          const point& firstElement = point(0,0));


    /**
     * copy the contents of the other separable kernel in this
     * two-dimensional kernel
     */
    kernel2D<T>& castFrom(const sepKernel<T>& other);

    /**
     * resize kernel with indicated indexes
     *
     * @param fromRow index of the lower row of the kernel
     * @param fromCol index of the lower column of the kernel
     * @param toRow index of the higher row of the kernel
     * @param toCol index of the higher column of the kernel
     * @param iniValue the initial value for the elements of the kernel
     * @param copyData if true, keep data (see lti::matrix::resize())
     * @param initNew initialize new elements with the <code>iniValue</code>.
     *
     * You can initialize a 5x3 kernel to filter a lti::channel with
     * following code:
     *
     * \code
     * lti::kernel2D<channel::value_type> aKernel;
     * aKernel.resize(-1,-2,1,2,0,false);
     * aKernel.at(-1,-2)=0.2;
     * aKernel.at(-1,-1)=0.2;
     * aKernel.at(0,0)=0.2;
     * aKernel.at(1,1)=0.2;
     * aKernel.at(1,2)=0.2;
     * \endcode
     */
    void resize(const int& fromRow,const int& fromCol,
                const int& toRow,const int& toCol,
		const T& iniValue = T(),
		const bool& copyData=true,
		const bool& initNew=true);

    /**
     * fills this kernel between the "from's" and "to's" with the
     * contents of the matrix <code>mat</code> starting at
     * <code>startAtRow</code> and <code>startAtCol</code>
     *
     * @param mat      the matrix with the data to be copied
     * @param fromRow  first row of the submatrix to be filled
     * @param fromCol  first column of the submatrix to be filled
     * @param toRow    last row of the submatrix to be filled
     * @param toCol    last column of the submatrix to be filled
     * @param startAtRow starting row of <code>mat</code> where the data is
     *                   located.
     * @param startAtCol starting column of <code>mat</code> where the data
     *                   is located.
     */
    void fill(const matrix<T>& mat,
              const int& fromRow= MinInt24,
              const int& fromCol= MinInt24,
              const int& toRow=   MaxInt24,
              const int& toCol=   MaxInt24,
              const int& startAtRow=0,
              const int& startAtCol=0);

    /**
     * fills this kernel between the "from's" and "to's" with the
     * contents of the matrix <code>mat</code> starting at
     * <code>startAtRow</code> and <code>startAtCol</code>
     *
     * @param data     the data that should fill this kernel
     * @param fromRow  first row of the submatrix to be filled
     * @param fromCol  first column of the submatrix to be filled
     * @param toRow    last row of the submatrix to be filled
     * @param toCol    last column of the submatrix to be filled
     */
    void fill(const T* data,
              const int& fromRow= MinInt24,
              const int& fromCol= MinInt24,
              const int& toRow=   MaxInt24,
              const int& toCol=   MaxInt24);


    /**
     * get normalization factor
     *
     * The normalization factor is used by the fixed point types as a
     * representation of the value 1.  For example, the norm for a
     * kernel1D<ubyte> can be 255, if the filter kernel don't need
     * values greater than 1.0.
     */
    inline const T& getNorm() const {
      return norm;
    };

    /**
     * set normalization factor
     * @see getNorm()
     */
    inline void setNorm(const T& n) {
      norm=n;
    }

    /**
     * returns offset, i.e. center of the filter-kernel
     * @return the position of the element with index 0 in the memory array.
     *         This value is the same as -firstIdx()
     */
    const point& getOffset() const;

    /**
     *  set kernel offset, i.e. center of the filter-kernel
     * @param p the position of the element with index (0,0) in the memory
     *          array.
     *          This value is the same as point(-firstRow(),-firstColumns()).
     */
    void setOffset(const point& p);

    /**
     * first row index
     */
    inline int firstRow() const {
      return -offset.y;
    };

    /**
     * last row index
     */
    inline int lastRow() const {
      return this->rows()-this->offset.y-1;
    }

    /**
     * first column index
     */
    inline int firstColumn() const {
      return -this->offset.x;
    }

    /**
     * last column index
     */
    inline int lastColumn() const {
      return this->columns()-this->offset.x-1;
    };

    /**
     * access operator with offset
     */
    inline T& at(const point& p) {
      return matrix<T>::at(p+this->offset);
    };

    /**
     * access operator with offset
     */
    inline const T& at(const point& p) const {
      return matrix<T>::at(p+offset);
    }

    /**
     * access operator with offset
     */
    inline T& at(const int& y,const int& x) {
      return matrix<T>::at(y+offset.y,x+offset.x);
    }
 
    /**
     * access operator with offset
     */
    inline const T& at(const int& y,const int& x) const {
      return matrix<T>::at(y+offset.y,x+offset.x);
    }

    /**
     * outer product of two kernel1D.
     * The first kernel is a column kernel and the second kernel is a
     * row kernel.
     *
     * @param colKernel column kernel
     * @param rowKernel row kernel
     * @return a reference to this kernel
     *
     * If you want to convert a separable kernel (lti::sepKernel<T>) into a
     * 2D kernel, see also the methods castFrom().
     *
     * The final kernel will contain colKernel.size() rows and rowKernel.size()
     * columns.  Each row will be equal to the rowKernel multiplied by the
     * respective colKernel element.
     */
    kernel2D<T>& outerProduct(const kernel1D<T>& colKernel,
			      const kernel1D<T>& rowKernel);

    /**
     * denormalize divide all elements by norm and set the norm to 1!
     */
    void denormalize();

    /**
     * Mirror the other kernel and leave the result here, i.e.
     * at(y,x) = other.at(-y,-x);
     * @param other the kernel to be copied and then mirrored
     * @return a reference to this instance
     */
    kernel2D<T>& mirror(const kernel2D<T>& other);

    /**
     * Mirror this kernel, i.e.
     * at(y,x) = at(-y,-x);
     * @return a reference to this instance
     */
    kernel2D<T>& mirror();

    /**
     * write the object in the given ioHandler
     */
    virtual bool write(ioHandler& handler,const bool complete = true) const;

    /**
     * read the object from the given ioHandler
     */
    virtual bool read(ioHandler& handler,const bool complete = true);

  protected:
    /**
     * absolute coordinates of the kernel point (0,0)
     */
    point offset;

    /** Normalisation factor.
        This value will be ignored for floating point formats.
        For fixed point formats, this value corresponds to 1.0 */
    T norm;
  };

  /**
   * Separable Kernel.
   *
   * A separable kernel is a %vector of one dimensional kernels.
   * If a two dimensional kernel can be separated, the convolution can be
   * applied in a very efficient way.
   *
   * A filter kernel K is called separable "in one pair", if the matrix
   * representation of K can be produced as an outer product of two
   * one-dimensional kernels Kx and Ky.
   *
   * The template type of this class should coincide with the template
   * class of the matrix or channel to be convolved with.
   * For example, if you want to convolve a sepKernel with a lti::channel, you
   * will need a sepKernel<channel::value_type> or sepKernel<float>.
   *
   * If you instantiate a sepKernel of a fixed point type, like
   * sepKernel<int> or sepKernel<ubyte>, you also need to consider the
   * "norm" of the kernel (see lti::sepKernel<T>::getNorm() and
   * lti::sepKernel<T>::setNorm(const T&)). This "norm" allows the
   * representation of numbers less than 1.0.  You can see this norm as the
   * value to be consider as 1.0, when operating with the kernel.
   * For example, if you have a sepKernel<ubyte> with the values [64,128,64]
   * and norm=255, the the interpreted values during convolution will be
   * [0.25,0.5,0.25].  With floating-point types, the norm will be always
   * assumed to be 1.0.  For other types the default norms are the following:
   * (see lti::typeInfo)
   *
   * - For int:              65536
   * - For ubyte:              255
   * - Otherwise:              1.0
   *
   * @see convolution
   *
   */
  template<class T>
  class sepKernel : public mathObject {
  public:
    /**
     * default constructor
     */
    sepKernel();

    /**
     * copy constructor
     * @param other the kernel to be copied.
     */
    sepKernel(const sepKernel& other);

    /**
     * construct a separable kernel with one filter pair, all elements
     * of the subfilters initialized with the given value.
     * @param from first index of the one dimensional filter kernel
     * @param to   last index of the one dimensional filter kernel
     * @param iniValue initial value for the kernel elements
     */
    sepKernel(const int& from,const int& to,const T& iniValue=T());

    /**
     * construct a symmetrical separable kernel
     *
     * The resulting separable kernel will have just one filter pair, where
     * the row and column filters are identical.
     *
     * @param subkernel the one-dimensional kernel to be used as row and
     *                  column filter.
     */
    sepKernel(const kernel1D<T>& subkernel);

    /**
     * destructor
     */
    virtual ~sepKernel();

    /**
     * copy member
     * @param other the kernel to be copied.
     * @return a reference to this instance
     */
    sepKernel& copy(const sepKernel& other);

    /**
     * returns name of this type ("sepKernel")
     */
    const char* getTypeName() const {return "sepKernel";};

    /**
     * clone member
     * @return a pointer to a copy of this instance
     */
    virtual mathObject* clone() const {
      return (new sepKernel<T>(*this));
    };

    /**
     * copy from kernel of another type
     * @param other a separable kernel of another type
     * @return a reference to this instance
     */
    template<class U>
    sepKernel& castFrom(const sepKernel<U>& other) {
      // implementation needs to be here due to VC++ bug
      rowKernels.resize(other.getNumberOfPairs());
      colKernels.resize(other.getNumberOfPairs());
      for (int i=0;i<other.getNumberOfPairs();i++) {
	rowKernels[i].castFrom(other.getRowFilter(i));
	colKernels[i].castFrom(other.getColFilter(i));
      }
      return (*this);
    }

    /**
     * separate a 2D kernel into 1D kernels
     *
     * Try to separate the two dimensional kernel <code>k</code>.  Stop the
     * separation if the error between original and separated kernel is less
     * than <code>maxDev</code>.
     *
     * @param k the two dimensional filter to be separated
     * @param maxDev the maximal deviation per element to be achieved
     * @return true if the separation succeeded or false otherwise.
     */
    bool separate(const kernel2D<T>& k,const double &maxDev=0.01);

    /**
     * number of filter pairs
     */
    int getNumberOfPairs() const;

    /**
     * set the number of column/row 1D-filters
     */
    void setNumberOfPairs(const int& numPairs);

    /**
     * return a row-kernel
     * @param i the index of the row filter.  This value must be between
     *        0 and getNumberOfPairs()
     */
    inline kernel1D<T>& getRowFilter(const int& i) {
      // check for limits
      assert(i<static_cast<int>(rowKernels.size()));
      return rowKernels[i];
    };

    /**
     * return a column-kernel
     * @param i the index of the column filter.  This value must be between
     *        0 and getNumberOfPairs()
     */
    inline kernel1D<T>& getColFilter(const int& i) {
      // check for limits
      assert(i<static_cast<int>(colKernels.size()));
      return colKernels[i];      
    };

    /**
     * return an unmodifiable row kernel
     * @param i the index of the row filter.  This value must be between
     *        0 and getNumberOfPairs()
     */
    inline const kernel1D<T>& getRowFilter(const int& i) const {
      // check for limits
      assert(i<static_cast<int>(rowKernels.size()));
      return rowKernels[i];
    }

    /**
     * return an unmodifiable column kernel
     * @param i the index of the column filter.  This value must be between
     *        0 and getNumberOfPairs()
     */
    inline const kernel1D<T>& getColFilter(const int& i) const {
      // check for limits
      assert(i<static_cast<int>(colKernels.size()));
      return colKernels[i];
    };

    /**
     * denormalize divide all elements by norm and set the norm to 1!
     */
    void denormalize();

    /**
     * multiply each 1D kernel with a constant value
     * @param value the value to be multiplied with
     * @return a reference to this object
     */
    sepKernel<T>& multiply(const T& value);

    /**
     * set the norm of each individual 1D kernel to the given value
     * @param newNorm the value to be used as norm
     * @return a reference to this object
     */
    void setNorm(const T& newNorm);

    /**
     * returns the sum of the elements of the resulting 2D kernel
     */
    T sumOfElements() const;

    /**
     * Mirror the other kernel and leave the result here, i.e.
     * at(y,x) = other.at(-y,-x);
     * @param other the kernel to be copied and then mirrored
     * @return a reference to this instance
     */
    sepKernel<T>& mirror(const sepKernel<T>& other);

    /**
     * Mirror this kernel, i.e.
     * at(y,x) = at(-y,-x);
     * @return a reference to this instance
     */
    sepKernel<T>& mirror();

    /**
     * write the object in the given ioHandler
     */
    virtual bool write(ioHandler& handler,const bool complete = true) const;

    /**
     * read the object from the given ioHandler
     */
    virtual bool read(ioHandler& handler,const bool complete = true);

    /**
     * @name Apply Methods
     */
    //@{

    /**
     * applies a C-function to each element of the kernel.
     * 
     * In the following example, kernel \a kernel is initialized with
     * 4.0. After applying \a sqrt(), all elements of \a kernel are 2.0.
     * \code
     * sepKernel<float> kern(-2,2,4.0);
     * kern.apply(sqrt);
     * \endcode
     * @param function a pointer to a C-function
     * @return a reference to the actual kernel
     */
    sepKernel<T>& apply(T (*function)(T));

    /**
     * applies a C-function to each element of the kernel.
     * @param function a pointer to a C-function
     * @return a reference to the actual kernel
     */
    sepKernel<T>& apply(T (*function)(const T&));

    /**
     * applies a C-function to each element of the other kernel and leaves
     * the result here.
     * @param other the source kernel
     * @param function a pointer to a C-function
     * @return a reference to the actual kernel
     */
    sepKernel<T>& apply(const sepKernel<T>& other,T (*function)(T));


    /**
     * applies a C-function to each element the other kernel and
     * leaves the result here.
     * @param other the kernel with the source data
     * @param function a pointer to a C-function
     * @return a reference to the actual kernel
     */
    sepKernel<T>& apply(const sepKernel<T>& other,T (*function)(const T&));

    //@}

  protected:
    /**
     * list of one-dimensional row kernels
     */
    std::vector< kernel1D<T> > rowKernels;

    /**
     * list of one-dimensional column kernels
     */
    std::vector< kernel1D<T> > colKernels;

  };

  // ----------------------------------------------------------
  //   Typical used types
  // ----------------------------------------------------------

  /**
   * one dimensional kernel of integers
   */
  typedef kernel1D<int>    ikernel1D;

  /**
   * one dimensional kernel of floats
   */
  typedef kernel1D<float>  fkernel1D;

  /**
   * one dimensional kernel of doubles
   */
  typedef kernel1D<double> dkernel1D;

  /**
   * one dimensional kernel of unsigned bytes
   */
  typedef kernel1D<ubyte>  bkernel1D;

  /**
   * two dimensional kernel of integers
   */
  typedef kernel2D<int>    ikernel2D;

  /**
   * two dimensional kernel of floats
   */
  typedef kernel2D<float>  fkernel2D;

  /**
   * two dimensional kernel of doubles
   */
  typedef kernel2D<double> dkernel2D;

  /**
   * two dimensional kernel of bkernel
   */
  typedef kernel2D<ubyte>  bkernel2D;

  /**
   * separable kernel of integers
   */
  typedef sepKernel<int>    isepKernel;

  /**
   * separable kernel of floats
   */
  typedef sepKernel<float>  fsepKernel;

  /**
   * separable kernel of doubles
   */
  typedef sepKernel<double> dsepKernel;

  /**
   * separable kernel of unsigned bytes
   */
  typedef sepKernel<ubyte>  bsepKernel;


}

#endif

