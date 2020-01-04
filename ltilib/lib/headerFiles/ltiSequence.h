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
 * file .......: ltiSequence.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 28.06.00
 * revisions ..: $Id: ltiSequence.h,v 1.10 2005/06/16 12:37:35 doerfler Exp $
 */

#ifndef _LTI_SEQUENCE_H_
#define _LTI_SEQUENCE_H_

#include "ltiObject.h"
#include <vector>
#include "ltiTypes.h"
#include "ltiIoObject.h"
#include "ltiMath.h"
#include "ltiException.h"
#include "ltiAssert.h"

namespace lti {
  /**
   * sequence container class.
   * The %lti::sequence class allows the representation of sequences of other
   * lti objects.  It can be used as a sort of vector of other objects.
   *
   * The elements of the sequence will be indexed between 0 an n-1.
   *
   * An example of a sequence of lti::vectors:
   *
   * \code
   *   // creates a sequence of 256 vectors
   *   lti::sequence< lti::vector<float> > mySeq(256)
   *
   * \endcode
   *
   * To access the vector elements use the access operators at() (or the
   * overloaded operator[]()).  For example:
   *
   * \code
   *   for (int i = 0; i < mySeq.size(); i++) {
   *     // each element of the sequence is a vector filled with i/2
   *     mySeq.at(i).resize(10,float(i)/2.0f);
   *   }
   * \endcode
   *
   * The sequence has following methods:
   *  - Constructors Constructors
   *    -  You can construct an empty sequence with the default constructor
   *       (sequence()).
   *    -  If you know the number of elements use
   *       sequence(const int& size)
   *    -  You can create the copy of another sequence with the copy
   *       constructor  (sequence(const sequence<T>& otherSequence))
   *  - Access members
   *    - at(), operator[]()
   *    - The size() member returns the number of elements of the vector.
   *    - The firstIdx() will return in a sequence always 0 and the lastIdx()
   *      will be always size()-1;
   *  - Fill and Copy members
   *    -  With the fill() members you can fill the sequence with a given
   *       constant value T or with values taken from other sequences.
   *    -  With the copy() member you can copy another sequence.
   *  - Iterators
   *    - It is possible to iterate within the sequence by making use of
   *      the sequence iterators.  (see begin() for more information)
   */
  template<class T>
  class sequence : public ioObject {
  public:
    typedef T value_type;

    /**
     * iterator type (allows read and write operations)
     * The use of the iterator classes is similar to the iterators of
     * the STL (Standard Template Library). See lti::sequence::begin()
     * for an example
     */
    typedef typename std::vector<T>::iterator iterator;

    /**
     * const iterator type (allows read-only operations)
     * The use of the iterator classes is similar to the iterators of
     * the STL (Standard Template Library). See lti::sequence::begin()
     * for an example.
     */
    typedef typename std::vector<T>::const_iterator const_iterator;

    /**
     * default constructor creates an empty sequence;
     */
    sequence();

    /**
     * create a sequence of the given size.
     *
     * The member will not be initialized
     * @param theSize number of elements of the sequence
     */
    sequence(const int& theSize);

    /**
     * create this sequence as a copy of another sequence
     * @param other the sequence to be copied.
     */
    sequence(const sequence<T>& other);

    /**
     * destructor
     */
    virtual ~sequence();

    /**
     * returns the name of this class: "sequence"
     */
    const char* getTypeName() const {return "sequence";};

    /**
     * returns the number of elements of the sequence
     */
    virtual int size() const;

    /**
     * returns first index (normally 0)
     */
    virtual int firstIdx() const;

    /**
     * returns first element as a const_iterator.
     * Note that you can not change the values of the sequence
     * elements when you use a const_iterator. See also begin()
     */
    virtual const_iterator begin() const;

    /**
     * returns an iterator pointing to the first element of the sequence
     * The use of the interators is similar to the iterators of the
     * Standard Template Library (STL).
     * If you need to iterate on all elements of the sequence, you can
     * use following code:
     * \code
     * int accu = 1;
     * lti::sequence<vector<int> > mySeq(10); // a sequence with 10 elements
     * lti::sequence<vector<int> >::iterator it;       // an iterator
     *
     * for (it=mySeq.begin();it!=mySeq.end();it++) {
     *   vector<int>& tmp = *it;     // tmp is a reference to the vector
     *                               // pointed by the iterator.
     *   accu++;
     *   tmp.resize(accu,accu);      // resize and initialize the vector
     * }
     * \endcode
     */
    virtual iterator begin();

    /**
     * returns last index (in a sequence this is always size()-1)
     */
    virtual int lastIdx() const;

    /**
     * returns last index as a const iterator.
     * For an example see begin()
     */
    virtual const_iterator end() const;

    /**
     * returns last index as an iterator
     * For an example see begin()
     */
    virtual iterator end();

    /**
     * append element to sequence.
     * @param theElement will be appended to the end of the sequence.
     *
     * Automatically resizes the vector. To remove Elements from end of sequence
     * use <code>resize()</code>
     */
    virtual void append(const T& theElement = T());

    /**
     * change dimension of the sequence.
     * @param newSize the new size of the sequence
     * @param iniValue the initialization value.
     * @param copyData if this parameter is true, the old data of the
     *                 sequence will be copied.  If it is false, the old data
     *                 will be lost.
     * @param initNew  if this parameter is true, then all new elements (if
     *                 they exist) will be initialized with
     *                 <code>iniValue</code>.
     *                 if <code>initNew</code> is false, then the new
     *                 elements are left uninitialized.
     *
     * For example:
     * \code
     *   lti::sequence<int> myVct;  // creates empty sequence
     *   myVct.resize(5,0);       // sequence with 5 elements initialized
     *                            // with 0
     *   myVct.resize(10,2);      // sequence has now 10 elements: the
     *                            // first five are still 0 and the
     *                            // rest have a 2
     *   myVct.resize(20,3,false,false); // now the sequence has 20
     *                                   // elements but their values
     *                                   // are unknown.
     *   myVct.resize(5,1,false,true); // the sequence has now 5
     *                                 // elements initialized with 1
     *
     *   // note that the last line could also be written:
     *
     *   myVct.resize(5,1,false);      // the sequence has now 5
     *                                 // elements initialized with 1
     *
     * \endcode
     *
     * If the resize is possible (see useExternData()), this %object
     * will always owns the data!
     */
    virtual void resize(const int& newSize,
                        const T& iniValue = T(),
                        const bool& copyData = true,
                        const bool& initNew = true);

    /**
     * fills the sequence elements with <code>iniValue</code> between
     * <code>from</code> and <code>to</code>.
     * @param iniValue the elements will be initialized with this
     *                 value.
     * @param from     first element index
     * @param to       last element index
     *
     * If <code>from</code> or <code>to</code> are out of bounds,
     * they will be (internaly) adjusted to to correct value.
     *
     * Example:
     * \code
     *   lti::sequence<double> myVct(10,0);  // sequence with 10 elements
     *                                       // with 0
     *   myVct.fill(9,1,3);                  // myVct=[0,9,9,9,0,0,0,0,0,0]
     * \endcode
     */
    virtual void fill(const T& iniValue,const int& from = 0,
                      const int& to = MaxInt32);

    /**
     * fills the sequence elements from <code>from</code> to
     * <code>to</code> with the elements of <code>vct</code> starting
     * at <code>startAt</code>.
     * @param vct sequence with the elements to be copied
     * @param from first element index of the actual sequence
     * @param to   last element index of the actual sequence
     * @param startAt start index of the source sequence <code>vct</code>.
     *
     * Example:  if a = [0 0 0 0 0] and b = [1 2 3], after a.fill(b,3,4,1)
     * results a = [0 0 0 2 3]
     */
    virtual void fill(const sequence<T>& vct,const int& from = 0,
                      const int& to = MaxInt32,
                      const int& startAt = 0);

    /**
     * access element x of the sequence
     * @param x index of the sequence element to be accessed. It should
     *          be between firstIdx() and lastIdx()
     */
    virtual T& at(const int& x);

    /**
     * access element x of the sequence in a read-only modus
     * @param x index of the sequence element to be accessed. It should
     *          be between firstIdx() and lastIdx()
     */
    virtual const T& at(const int& x) const;

    /**
     * access operator (alias for at(const int& x)).
     */
    virtual T& operator[](const int& x);

    /**
     * const access operator (alias for at(const int& x) const).
     */
    virtual const T& operator[](const int& x) const;

    /**
     * assigment operator.
     * copy the contents of <code>other</code> in this %object.
     * @param other the source sequence to be copied.
     */
    virtual sequence<T>& copy(const sequence<T>& other);

    /**
     * create a clone of this sequence
     * @return a pointer to a copy of this sequence
     */
    virtual object* clone() const;

    /**
     * compare this sequence with other
     * @param other the other sequence to be compared with
     * @return true if both sequences have the same elements and same size
     */
    virtual bool equals(const sequence<T>& other) const;

    /**
     * compare this sequence with other
     * @param other the other sequence to be compared with
     * @return true if both sequences have the same elements and same size
     */
    virtual bool operator==(const sequence<T>& other) const;

    /**
     * assigment operator (alias for copy(other)).
     * @param other the sequence to be copied
     * @return a reference to the actual sequence
     */
    virtual sequence<T>&
      operator=(const sequence<T>& other) {return copy(other);};

    /**
     * concatenate another sequence to this one
     * the elements of the other sequence will be concatenated
     * @param other the sequence to be concatenated at the end of this one
     * @return a reference to this sequence
     */
    virtual sequence<T>& concatenate(const sequence<T>& other);

    /**
     * write the sequence in the given ioHandler
     */
    virtual bool write(ioHandler& handler,const bool complete = true) const;

    /**
     * read the sequence from the given ioHandler
     */
    virtual bool read(ioHandler& handler,const bool complete = true);

  protected:

    /// this sequence class is implemented with a std::vector instance
    std::vector<T> theSequence;

  };

  /** @name Storable interface
   *  Members for the storable interface
   */

  //@{
  /**
   * read the sequence from the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be readed
   */
  template<class T>
  bool read(ioHandler& handler,sequence<T>& seq,const bool complete=true) {
      return seq.read(handler,complete);
  }

  /**
   * write the sequence in the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be written or not
   */
  template<class T>
  bool write(ioHandler& handler, const sequence<T>& seq,const bool complete=true) {
      return seq.write(handler,complete);
  }

  //@}

} // namespace lti

/*
// outputs the elements of the vector on a stream
template <class T>
std::ostream& operator<<(std::ostream& s,const lti::sequence<T>& v);
*/

#include "ltiSequence_template.h"

#endif

