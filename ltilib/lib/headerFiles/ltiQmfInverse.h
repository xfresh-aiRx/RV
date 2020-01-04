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
 * file .......: ltiQmfInverse.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 23.1.2001
 * revisions ..: $Id: ltiQmfInverse.h,v 1.6 2004/05/03 12:18:39 alvarado Exp $
 */

#ifndef _LTI_QMF_INVERSE_H_
#define _LTI_QMF_INVERSE_H_

#include "ltiFilter.h"
#include "ltiQmf.h"
#include "ltiUpsampling.h"

namespace lti {
  /**
   * This class allows the reconstruction of channels which were wavelet-
   * transformed by the functor lti::qmf.
   *
   * The low-pass kernel specified in the parameters will be used to calculate
   * the wavelet coefficients required in the inverse wavelet transform.
   *
   * @see lti::qmfInverse::parameters
   */
  class qmfInverse : public filter {
  public:
    /**
     * the parameters for the class qmfInverse are the same than for the
     * qmf functor (see lti::qmf::parameters).  This way it is really easy
     * to set the parameters for the transform and its inverse:
     * \code
     * lti::qmf theTransform;
     * lti::qmfInverse theInvTransform;
     *
     * lti::qmf::parameters qmfParam;
     * qmfParam.setKernel(haarKernel()); // use the haar kernel
     * qmfParam.levels = 3;              // decompose in one low-pass band and
     *                                   // three high-pass bands
     * theTransform.setParameters(qmfParam);
     * theInvTransform.setParameters(qmfParam); // the same parameters can be
     *                                          // used!
     * \endcode
     */
    typedef lti::qmf::parameters parameters;

    /**
     * default constructor
     */
    qmfInverse();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    qmfInverse(const qmfInverse& other);

    /**
     * destructor
     */
    virtual ~qmfInverse();

    /**
     * returns the name of this type ("qmfInverse")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on the given %parameter.
     * @param srcdest channel with the source data.  The result
     *                 will be left here too.
     * @result a reference to the <code>srcdest</code>.
     */
    bool apply(channel& srcdest);

    /**
     * operates on the given %parameter.
     * @param srcdest vector<float> with the source data.  The result
     *                 will be left here too.
     * @result a reference to the <code>srcdest</code>.
     */
    bool apply(vector<float>& srcdest);

    /**
     * operates on a copy of the given %parameters.
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @result a reference to the <code>dest</code>.
     */
    bool apply(const channel& src,channel& dest);

    /**
     * operates on a copy of the given %parameters.
     * @param src vector<float> with the source data.
     * @param dest vector<float> where the result will be left.
     * @result a reference to the <code>dest</code>.
     */
    bool apply(const vector<float>& src,vector<float>& dest);

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    qmfInverse& copy(const qmfInverse& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  protected:
    /**
     * transform just the two bands between the given indices.  The
     * result will be left at the same place!
     * Do not forget to initialize the upsampling functors before calling
     * this member!
     */
    void mergeTwo(vector<float>& src,
                  const int& from,
                  const int& to) const;

    /**
     * transform just the two bands between the given indices for each
     * row.  The result will be left at the same place!
     * Do not forget to initialize the upsampling functors before calling
     * this member!
     */
    void mergeTwo(channel& src,
                  const int& fromRow,
                  const int& toRow,
                  const int& fromCol,
                  const int& toCol) const;

    /**
     * transform just the two bands between the given indices for each
     * column.  The result will be left at the same place!
     * Do not forget to initialize the upsampling functors before calling
     * this member!
     */
    void mergeTwoCol(channel& src,
                     const int& fromRow,
                     const int& toRow,
                     const int& fromCol,
                     const int& toCol) const;

    /**
     * upsampling functor for the low-frecuency band
     */
    upsampling lowUpsampler;

    /**
     * upsampling functor for the high-frecuency band
     */
    upsampling highUpsampler;

    /**
     * upsampling functor for the low-frecuency band of columns
     */
    upsampling lowColUpsampler;

    /**
     * upsampling functor for the high-frecuency band of columns
     */
    upsampling highColUpsampler;

    /**
     * initialize both upsampling functors, using the kernel filter specified
     * in the parameters.  If onlyRows is false, also the upsamplers for
     * columns of channels will be initialized.
     */
    void initializeUpsamplers(const bool onlyRows=true);
  };
}

#endif

