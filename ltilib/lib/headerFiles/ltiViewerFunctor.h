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


/*--------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiViewerFunctor.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 31.12.2002
 * revisions ..: $Id: ltiViewerFunctor.h,v 1.6 2004/05/03 12:20:35 alvarado Exp $
 */

#ifndef _LTI_VIEWER_FUNCTOR_H_
#define _LTI_VIEWER_FUNCTOR_H_

#include "ltiTransform.h"
#include "ltiViewer.h"
#include "ltiHistogram.h"

namespace lti {

  /**
   * This viewerFunctor has as task to prepare an image, that will be
   * visualized in the lti::viewer.  For this reason, it uses the same
   * parameters as the viewer, but it ignores those ones which control
   * widget specific stuff like window size and so on.
   *
   * This way, you can also generate images that you do with your viewer but
   * without showing them.
   *
   * Due to the fact, that viewer alter the "informational" part of the
   * parameters, this functor will also do this.  This means, after calling
   * an apply method, the parameters can contain (if specified to do so) new
   * information in the parameters corresponding to the statistics.
   */
  class viewerFunctor : public transform {
  public:
    /**
     * the parameters for the class viewerFunctor are the same parameters
     * of the lti::viewer
     */
    typedef viewer::parameters parameters;

    /**
     * default constructor
     */
    viewerFunctor();

    /**
     * Construct a functor using the given parameters
     */
    viewerFunctor(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    viewerFunctor(const viewerFunctor& other);

    /**
     * destructor
     */
    virtual ~viewerFunctor();

    /**
     * returns the name of this type ("viewerFunctor")
     */
    virtual const char* getTypeName() const;

    // apply methods!

    /**
     * create a visualization image for the the input data
     * @param src matrix<ubyte> with the source data.
     * @param dest matrix<ubyte> where the result will be left.
     * @param doStat if true, the statistics for the src will be computed and
     *                        leaved in the informational part of the
     *                        parameters object.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<ubyte>& src,image& dest,
               const bool doStat=true);

    /**
     * create a visualization image for the the input data
     * @param src matrix<int> with the source data.
     * @param dest matrix<int> where the result will be left.
     * @param doStat if true, the statistics for the src will be computed and
     *                        leaved in the informational part of the
     *                        parameters object.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<int>& src,image& dest,
               const bool doStat=true);

    /**
     * create a visualization image for the the input data
     * @param src matrix<float> with the source data.
     * @param dest matrix<float> where the result will be left.
     * @param doStat if true, the statistics for the src will be computed and
     *                        leaved in the informational part of the
     *                        parameters object.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<float>& src,image& dest,
               const bool doStat=true);

    /**
     * create a visualization image for the the input data
     * @param src matrix<double> with the source data.
     * @param dest matrix<double> where the result will be left.
     * @param doStat if true, the statistics for the src will be computed and
     *                        leaved in the informational part of the
     *                        parameters object.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<double>& src,image& dest,
               const bool doStat=true);


    /**
     * create a visualization image for the the input data
     * @param src image with the source data.
     * @param dest image where the result will be left.
     * @param doStat if true, the statistics for the src will be computed and
     *                        leaved in the informational part of the
     *                        parameters object.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,image& dest,
               const bool doStat=true);

    /**
     * create a visualization image for the the input data
     * @param src histogram1D with the source data.
     * @param dest image where the result will be left.
     * @param doStat if true, the statistics for the src will be computed and
     *                        leaved in the informational part of the
     *                        parameters object.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const histogram1D& src,image& dest,
               const bool doStat=true);

    /**
     * create a visualization image for the the input data
     * @param src histogram1D with the source data.
     * @param dest image where the result will be left.
     * @param doStat if true, the statistics for the src will be computed and
     *                        leaved in the informational part of the
     *                        parameters object.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const histogram2D& src,image& dest,
               const bool doStat=true);

    /**
     * create a visualization image for the the input data
     * @param src vector<double> with the source data.
     * @param dest image where the result will be left.
     * @param doStat if true, the statistics for the src will be computed and
     *                        leaved in the informational part of the
     *                        parameters object.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const dvector& src,image& dest,
               const bool doStat=true);

    /**
     * create a visualization image for the the input data
     * @param src vector<float> with the source data.
     * @param dest image where the result will be left.
     * @param doStat if true, the statistics for the src will be computed and
     *                        leaved in the informational part of the
     *                        parameters object.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const vector<float>& src,image& dest,
               const bool doStat=true);

    /**
     * create a visualization image for the the input data
     * @param src vector<float> with the source data.
     * @param dest image where the result will be left.
     * @param doStat if true, the statistics for the src will be computed and
     *                        leaved in the informational part of the
     *                        parameters object.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const vector<int>& src,image& dest,
               const bool doStat=true);

    /**
     * create a visualization image for the the input data
     * @param src vector<float> with the source data.
     * @param dest image where the result will be left.
     * @param doStat if true, the statistics for the src will be computed and
     *                        leaved in the informational part of the
     *                        parameters object.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const vector<ubyte>& src,image& dest,
               const bool doStat=true);

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    viewerFunctor& copy(const viewerFunctor& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    viewerFunctor& operator=(const viewerFunctor& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * returns a read/write-reference of the used parameters.
     *
     * This is required by the viewer, in order to write in the
     * parameters the options specified by the user in the Configuration
     * Dialogs and to write the statistics (informational data) into the
     * parameters object.
     */
    parameters& getParameters();

    /**
     * compute the values for contrast and brightness necessary to map
     * the minI and maxI attributes in the parameters to the
     * given minp and maxp values.
     * Note that must be minp <= maxp.
     */
    void computeContrastBrightness(const float minp,
                                   const float maxp,
                                   const float norm,
                                   float& c,
                                   float& b) const;
  protected:

    /**
     * The input channel is a scalar matrix
     */
    bool scalarToImage(const matrix<ubyte>& chnl,image& img,const bool doStat);

    /**
     * The input channel is a scalar matrix
     */
    bool scalarToImage(const matrix<int>& chnl,image& img,const bool doStat);

    /**
     * The input channel is a scalar matrix
     */
    bool scalarToImage(const matrix<float>& chnl,image& img,const bool doStat);

    /**
     * The input channel is a scalar matrix
     */
    bool scalarToImage(const matrix<double>& chnl,image& img,
                       const bool doStat);

    /**
     * The input channel is a scalar matrix
     */
    bool scalarToImage(const dvector& vct,image& img,
                       const bool doStat);

    /**
     * The input channel is a scalar matrix
     */
    bool imageToImage(const image& chnl,image& img,const bool doStat);

    /**
     * The input channel is a label map
     */
    bool labelsToImage(const matrix<ubyte>& chnl,image& img,const bool doStat);

    /**
     * The input channel is a label map
     */
    bool labelsToImage(const matrix<int>& chnl,image& img,const bool doStat);

    /**
     * Zoom the image
     */
    bool zoom(image& img);

    /**
     * statistics
     */
    bool statistics(const matrix<ubyte>& chnl,parameters& par) const;

    /**
     * statistics
     */
    bool statistics(const matrix<int>& chnl,parameters& par) const;

    /**
     * statistics
     */
    bool statistics(const matrix<float>& chnl,parameters& par) const;

    /**
     * statistics
     */
    bool statistics(const matrix<double>& chnl,parameters& par) const;

    /**
     * statistics
     */
    bool statistics(const image& img,parameters& par) const;

    /**
     * statistics
     */
    bool statistics(const dvector& img,parameters& par) const;

    /**
     * compute the static color palettes used to create the images
     */
    static void computePalettes();

    /**
     * the static color palettes
     */
    //@{

    /**
     * The gray palette
     */
    static palette grayPalette;

    /**
     * The gray palette
     */
    static palette redPalette;

    /**
     * The gray palette
     */
    static palette greenPalette;

    /**
     * The gray palette
     */
    static palette bluePalette;

    /**
     * The gray palette
     */
    static palette huePalette;
    //@}

    /**
     * compute, using the informational data in the parameters,
     * the required linear transformation to change the contrast of
     * an image intensity.
     * @param norm the norm used for the current type (255 or 1)
     * @param m slope
     * @param b offset
     */
    void computeLinearTrans(const float norm,float& m,float& b) const;

    /**
     * compute, for the contrast argument c and the
     * minI and maxI attributes in the parameters object,
     * the values to which this two extrema are mapped, if the brightness = 0;
     */
    void computeContrastExtrema(const float norm,
                                const float c,
                                float& minp,float& maxp)const;

  };
}

#endif

