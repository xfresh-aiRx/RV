/*
 * Copyright (C) 1998, 1999, 2000, 2001, 2002
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
 * file .......: ltiSparseHistogram.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 30.10.2001
 * revisions ..: $Id: ltiSparseHistogram.h,v 1.12 2005/06/16 12:37:35 doerfler Exp $
 */

#ifndef _LTI_SPARSE_HISTOGRAM_H_
#define _LTI_SPARSE_HISTOGRAM_H_

#include "ltiBoundsFunctor.h"
#include <map>

namespace lti {

  /**
   * Simple sparse multidimensional histogram. It can be indexed via
   * int and double vectors.  It can handle index spaces of
   * arbitrary dimensions.
   *
   * The maximum number of bins per dimension is limited to 64.
   *
   * The accumulated type is always float.
   *
   * @see sparseMatrix
   *
   * @ingroup gAggregate
   */
  class sparseHistogram: public mathObject {
  protected:

    /**
     * Comparator for C-strings (null-terminated char array)
     */
    struct cless {
      bool operator()(const char* x, const char* y) const {
        return strcmp(x,y) < 0;
      }
    };

    /**
     * index map type
     */
    typedef std::map<const char*,float,cless> mapType;


  public:

    typedef float value_type;

    class const_iterator;

    /**
     * Similar to the iterators of the sparse matrix, these iterators
     * allow to access the non-sparse cells of the histogram.  This way
     * all data of the histogram can be efficiently accessed without loosing
     * time with the sparse valued cells.
     *
     * Note that the access sequence is not defined, so this is usually
     * required when ALL the data of the histogram needs to be changed.
     */
    class iterator {
      friend class sparseHistogram;
#     ifdef _LTI_MSC_6
      friend class const_iterator;
#     else
      friend class sparseHistogram::const_iterator;
#     endif
    private:
      /**
       * actual vector index
       */
      mapType::iterator it;

    public:
      /**
       * Default constructor
       */
      iterator() : it() {};

      /**
       * copy constructor
       */
      iterator(const iterator& other) : it(other.it) {};

      /**
       * advance to next item
       */
      inline iterator& operator++() {++it; return *this;};   // prefix

      /**
       * advance to next item
       */
      inline iterator operator++(int) {
        iterator tmp(*this);
        it++; return tmp;
      }; // postfix

      /**
       * recede to previous item
       */
      inline iterator& operator--() {--it; return *this;};   // prefix

      /**
       * recede to previous item
       */
      inline iterator operator--(int) {
        iterator tmp(*this);
        it--; return tmp;
      }; // postfix

      /**
       * compare if both pointed positions are the same
       */
      inline bool operator==(const iterator& other) const {
        return (it == other.it);
      };

      /**
       * compare if both pointed positions are different
       */
      inline bool operator!=(const iterator& other) const {
        return (it != other.it);
      };

      /**
       * get pointed data
       */
      inline float& operator*() {return (*it).second;};

      /**
       * copy member
       */
      inline iterator& operator=(const iterator& other) {
        it = other.it;
        return *this;
      };

    protected:
      /**
       * protected constructor (for internal use only)
       */
      explicit iterator(const mapType::iterator& iter)
        : it(iter) {};

      /**
       *
       */
      const char* getKey() const {
        return (*it).first;
      }

    };

    /**
     * Similar to the const_iterators of the sparse matrix, these iterators
     * allow read-only access the non-sparse cells of the histogram.  This way
     * all data of the histogram can be efficiently accessed without loosing
     * time with the sparse valued cells.
     *
     * Note that the access sequence is not defined, so this is usually
     * required when ALL the data of the histogram needs to be changed.
     */
    class const_iterator {
      friend class sparseHistogram;
    private:
      /**
       * actual vector index
       */
      mapType::const_iterator it;

    public:
      /**
       * Default constructor
       */
      const_iterator() : it() {};

      /**
       * copy constructor
       */
      const_iterator(const const_iterator& other) : it(other.it) {};

      /**
       * advance to next item
       */
      inline const_iterator& operator++() {++it; return *this;};   // prefix

      /**
       * advance to next item
       */
      inline const_iterator operator++(int) {
        const_iterator tmp(*this);
        it++; return tmp;
      }; // postfix

      /**
       * recede to previous item
       */
      inline const_iterator& operator--() {--it; return *this;};   // prefix

      /**
       * recede to previous item
       */
      inline const_iterator operator--(int) {
        const_iterator tmp(*this);
        it--; return tmp;
      }; // postfix

      /**
       * compare if both pointed positions are the same
       */
      inline bool operator==(const const_iterator& other) const {
        return (it == other.it);
      };

      /**
       * compare if both pointed positions are different
       */
      inline bool operator!=(const const_iterator& other) const {
        return (it != other.it);
      };

      /**
       * get pointed data
       */
      inline const float& operator*() const {return (*it).second;};

      /**
       * copy member
       */
      inline const_iterator& operator=(const const_iterator& other) {
        it = other.it;
        return *this;
      };

    protected:
      /**
       * protected constructor (for internal use only)
       */
      explicit const_iterator(const mapType::const_iterator& iter)
        : it(iter) {};

      /**
       *
       */
      const char* getKey() const {
        return (*it).first;
      }
    };


    /**
     * Default constructor
     *
     * Creates an empty histogram.  You need to resize it before using it
     */
    sparseHistogram();

    /**
     * Constructor.
     *
     * Required parameters are the number of bins per dimension and
     * the number of dimensions.
     *
     * The maximum number of bins per dimension is limited to 64.
     *
     * @param dim number of dimensions
     * @param numberOfBins number of bins per dimension
     */
    sparseHistogram(const int dim, const int numberOfBins);

    /**
     * Constructor. The vector gives the number of bins for
     * each dimension.
     */
    sparseHistogram(const ivector& bins);

    /**
     * Constructor.
     *
     * Required parameters are the number of bins per dimension.
     * Furthermore, you must give the lower and upper bounds of the
     * hyperbox which is supposed to be occupied by the histogram.
     * This will be used to access the histogram by vectors with
     * double elements.
     *
     * The maximum number of bins per dimension is limited to 64.
     */
    sparseHistogram(const int numberOfBins,
                    const dvector& min,
                    const dvector& max);

    /**
     * Constructor.
     *
     * The first vector gives the number of bins for each dimension.
     * The second and third vector give the lower and upper bounds of
     * the hyperbox which is supposed to be occupied by the histogram.
     *
     * The maximum number of bins per dimension is limited to 64.
     */
    sparseHistogram(const ivector& numberOfBins,
                    const dvector& min,
                    const dvector& max);

    /**
     * copy constructor
     */
    sparseHistogram(const sparseHistogram& other);

    /**
     * destructor
     */
    virtual ~sparseHistogram();

    /**
     * copy data of "other" histogram.
     * @param other the clustering to be copied
     * @return a reference to this clustering object
     */
    sparseHistogram& copy(const sparseHistogram& other);

    /**
     * Clear the previous content of the histogram and resize it
     * to the given dimensions and bins per dimensions.
     *
     * The maximum number of bins per dimension is limited to 64.
     *
     * @param dim number of dimensions
     * @param numberOfBins number of bins per dimension
     */
    void resize(const int dim, const int numberOfBins);

    /**
     * Clear and resize the histogram to the number of dimensions equal to
     * the size of the vector bins, and having at each dimension the number
     * of bins given at each component of bins vector.
     *
     * The maximum number of bins per dimension is limited to 64.
     */
    void resize(const ivector& bins);

    /**
     * Clear and resize the histogram.
     *
     * The new histogram will have the given numbers of bins per dimension, for
     * a number of dimensions equal min.size() or max.size() (which must have
     * the same number of elements).
     * Furthermore, you must give the lower and upper bounds
     * of the hyperbox which is supposed to be occupied by the
     * histogram.
     * This will be used to access the histogram by vectors with
     * double elements.
     *
     * The maximum number of bins per dimension is limited to 64.
     */
    void resize(const int numberOfBins,
                const dvector& min,
                const dvector& max);

    /**
     * Clear and resize the histogram
     *
     * The first vector gives the number of bins for each dimension.
     * The second and third vector give the lower and upper bounds of
     * the hyperbox which is supposed to be occupied by the histogram.
     * This will be used to access the histogram by vectors with
     * double elements.
     *
     * The maximum number of bins per dimension is limited to 64.
     */
    void resize(const ivector& numberOfBins,
                const dvector& min,
                const dvector& max);

    /**
     * Returns the value stored at the given index.
     */
    float get(const ivector& index) const;

    /**
     * Sets the value at the given index.
     *
     * Note that put(index,0.0f) inserts a "non-sparse" cell with the
     * value zero.  If you really want to delete the cell, you need to
     * clear it explicitelly with the method clear(const ivector&).
     */
    void put(const ivector& index, float value=0.0f);

    /**
     * Set the entry value at the given index to the sparse value.
     * Note that the put(index,0.0f) inserts a "non-sparse" cell with the
     * value zero.  If you really want to delete the cell, you need to
     * clear it explicitelly.
     */
    void clear(const ivector& index);

    /**
     * Adds the value to the value at the given index.
     */
    void add(const ivector& index, float value=1.0);

    /**
     * Multiplies the value with the value at the given index.
     */
    void multiply(const ivector& index, const float& value);

    /**
     * Divides the value with the value at the given index.
     */
    void divide(const ivector& index, const float& value);

    /**
     * Divides all entries by the given value.
     */
    void divide(const float& sum);

    /**
     * returns the name of this class: "sparseHistogram"
     */
    const char* getTypeName() const {
      return "sparseHistogram";
    };

    /**
     * read-only access to the element x of the histogram
     * @param x index of the histogram element to be accessed.  It should
     *          be between getFirstCell() and getLastCell()
     * @return the number of entries in the given cell
     */
    const float& at(const ivector& x) const;

    /**
     * access element x of the histogram
     * @param x index of the histogram element to be accessed.  It should
     *          be between getFirstCell() and getLastCell()
     * @return the number of entries in the given cell
     */
    float& at(const ivector& x);

    /**
     * read-only access to the element x of the histogram
     * @param x index of the histogram element to be accessed.  It should
     *          be between getFirstCell() and getLastCell()
     * @return the number of entries in the given cell
     */
    inline const float& at(const dvector& x) const {
      return at(makeTmpIndex(x));
    }

    /**
     * access element x of the histogram
     * @param x index of the histogram element to be accessed.  It should
     *          be between getFirstCell() and getLastCell()
     * @return the number of entries in the given cell
     */
    inline float& at(const dvector& x) {
      return at(makeTmpIndex(x));
    }

    /**
     * returns the number of dimensions of this histogram
     */
    inline int dimensions() const {
      return bins.size();
    }

    /**
     * returns first element as a const_iterator.
     * Note that you can not change the values of the vector
     * elements when you use a const_iterator. See also begin()
     */
    inline const_iterator begin() const {
      return const_iterator(core.begin());
    };

    /**
     * returns first element as an iterator
     * The use of the interators is similar to the iterators of the
     * Standard Template Library (STL).
     * If you need to iterate on all non-sparse elements of the histogram
     * use following code:
     *
     * \code
     *   float tmp,accu;                   // temporal variables
     *   lti::sparseHistogram myHist(3,6); // a 3D Histogram with 6 bins
     *                                     // per dimension (216 cells)
     *   // fill the histogram
     *   ivector idx(3,1);
     *   myHist.put(idx,5); // at (1,1,1) set 5
     *   ...
     *
     *   lti::sparseHistogram::iterator it;  // an iterator
     *   accu = 0;
     *   for (it=myVct.begin();it!=myVct.end();it++) {
     *     tmp = *it;                    // tmp has value of element pointed
     *                                   // by the iterator.
     *     accu += tmp;
     *     (*it) *= 5                    // change the value in the vector.

     *    }
     * \endcode
     *
     * Please note that if you define <code>it</code> as a const_iterator,
     * you can not make something like <code>(*it)*=5</code>.
     */
    inline iterator begin() {
      return iterator(core.begin());
    };

    /**
     * returns last index as a const iterator.
     * For an example see begin()
     */
    inline const_iterator end() const {
      return const_iterator(core.end());
    }

    /**
     * returns last index as an iterator
     * For an example see begin()
     */
    inline iterator end() {
      return iterator(core.end());
    }

    /**
     * Returns the value stored at the given index.
     */
    inline float get(const dvector& index) const {
      return get(makeTmpIndex(index));
    };

    /**
     * Sets the value at the given index.
     */
    inline void put(const dvector& index, float value=0.0) {
      put(makeTmpIndex(index),value);
    };

    /**
     * Adds the value to the value at the given index.
     */
    inline void add(const dvector& index, float value=1.0) {
      add(makeTmpIndex(index),value);
    };

    /**
     * Multiplies the value with the value at the given index.
     */
    inline void multiply(const dvector& index, const float& value) {
      multiply(makeTmpIndex(index),value);
    };

    /**
     * Erases all elements from the histogram.
     */
    void clear();

    /**
     * Returns an identical copy of this histogram.
     */
    virtual mathObject* clone() const;


    /**
     * write the histogram in the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool write(ioHandler& handler,const bool complete=true) const;

    /**
     * read the histogram from the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool read(ioHandler& handler,const bool complete=true);


  private:

    /**
     * Creates a new char array key from an index vector.
     */
    inline const char* makeNewKey(const ivector& index) const {
      char* buf=new char[index.size()+1];
      char* c=buf;
      for (int i=0; i<index.size(); i++) {
        assert(index[i] < bins[i]);
        *c++=codeChars[index[i]];
      }
      *c='\0';
      return buf;
    };

    inline void makeKey(const ivector& index, char *buf) const {
      assert(index.size()==tmpBufSize);
      char* c=buf;
      for (int i=0; i<index.size(); i++) {
        assert(index[i] < bins[i]);
        *c++=codeChars[index[i]];
      }
      *c='\0';
    };

    /**
     * Creates a new string buffer to the generation of hash-keys.
     * This method ensures the deletion of the previous buffer
     */
    inline void checkTmpBuffer(const ivector& index) const;

    /**
     * Convert the given dvector into an ivector.  The attribute
     * tmpIndex is used for this task, and a reference to it is
     * returned.
     */
    inline const ivector& makeTmpIndex(const dvector& d) const;

    /**
     * conversion from the integer value to the corresponding string
     * entry for the hash-key creation.
     */
    static const char* codeChars;

    /**
     * Maximal number of bins per dimension is determined by the length of
     * the codeChar symbols.
     */
    static const int maxN;

    /**
     * number of bins per axis
     */
    ivector bins;

    /**
     * The data ist stored in this map.
     */
    mapType core;

    /**
     * transform for the index computation
     */
    dvector offset;

    /**
     * slope for the linear transformation from dvectors to the index ivectors
     */
    dvector scale;

    /**
     * lower bound of the bounding hyper-box of the histogram
     */
    ivector minIndex;

    /**
     * higher bound of the bounding hyper-box of the histogram
     */
    ivector maxIndex;

    /**
     *
     */
    boundsFunctor<int> indexClipper;

    /**
     * sparse valued returned when an element is not in the core data.
     */
    static const float zero;

    /**
     * temporary buffers for the conversion of double index
     * to integer index
     */
    mutable dvector tmp;

    /**
     * temporal index vector usually constructed through makeTmpIndex from
     * a double vector.
     */
    mutable ivector tmpIndex;

    /**
     * temporary buffer for the conversion of index vector
     * to index string
     */
    mutable char *tmpBuffer;

    /**
     * Size of the tmpBuffer string
     */
    mutable int tmpBufSize;
  };

  // inline methods implementation
  inline const ivector& sparseHistogram::makeTmpIndex(const dvector& d) const {
    tmp=d;
    tmp.subtract(offset);
    tmp.emultiply(scale);
    tmpIndex.castFrom(tmp);
    indexClipper.clip(tmpIndex,minIndex,maxIndex);
    return tmpIndex;
  }


}

#endif

