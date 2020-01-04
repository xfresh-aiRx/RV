/*
 * Copyright (C) 1998 - 2004
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
 * file .......: ltiRegionShapeFeatures.h
 * authors ....: Axel Berner, Pablo Alvarado, Jens Rietzschel
 * organization: LTI, RWTH Aachen
 * creation ...: 19.6.2002
 * revisions ..: $Id: ltiRegionShapeFeatures.h,v 1.15 2005/06/16 12:35:43 doerfler Exp $
 */

#ifndef _LTI_REGION_SHAPE_FEATURES_H_
#define _LTI_REGION_SHAPE_FEATURES_H_

#include "ltiGlobalFeatureExtractor.h"

namespace lti {
  /**
   * This functor computes descriptors for shapes, decomposing a
   * \b binary mask as a sum of some basis functions.
   *
   * Several basis function sets have been implemented:
   *
   * - MPEG-7: the region shape descriptor of MPEG-7 uses the ART
   *   transformation, which projects the shape into the
   *   set \f$V_{nm}(\rho,\theta)\f$ of non-orthogonal basis functions, defined
   *   as
   *   \f$V_{nm}(\rho,\theta) = A_m(\theta) R_n(\rho)\f$, where
   *   \f[A_m(\theta) = \frac{1}{2\pi} \exp(jm\theta)\f]
   *   \f[R_n(\rho) =
   *      \begin{cases} 1 & n=0\\ 2\cos(\pi n \rho) & n \neq 0 \end{cases}\f]
   *   More information about this descriptor can be found in
   *   Miroslaw Bober, "MPEG-7 Visual Shape Descriptors", IEEE Transactions on
   *   Circuits and Systems for Video Technology, Vol. 11, No. 6, June 2001.
   *   or http://www.ctr.columbia.edu/~sfchang/course/vis/REF/bober-01.pdf
   *
   * - Radial Orthogonal Basis Functions:  Similar to the MPEG-7 descriptors,
   *   but with an orthogonal radial component:
   *   \f[R_n(\rho) =
   *      \begin{cases} 1 & n=0\\ 2\cos(2\pi n \rho) & n \neq 0 \end{cases}\f]
   *
   * - Zernike Moments:  The theory about them can be found:
   *   A. Khotanzad and Y.H. Hong. "Invariant Image Recognition by Zernike
   *   Moments".  IEEE Trans. on Pattern Analysis and Machine Intelligence,
   *   Vol. 12, No. 5, May 1990, pp. 489-497
   *
   * Tschebichef Moments need still to be implemented.
   */
  class regionShapeFeatures : public globalFeatureExtractor {
  public:
    /**
     * the parameters for the class regionShapeFeatures
     */
    class parameters : public globalFeatureExtractor::parameters {
    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * destructor
       */
      ~parameters();

      /**
       * returns name of this type
       */
      const char* getTypeName() const;

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

#     ifdef _LTI_MSC_6
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const;
#     endif

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * size of the descriptor matrix.  The x component will
       * correspond to the number of angular frequencies to be used,
       * and the y component to the number of radial frequencies.
       *
       * The feature vectors will have size.x*size.y elements.
       *
       * For Zernike moments the matrix is symmetric with (size.y,size.y)
       *
       * Default value: (12 , 3)
       */
      point size;

      /**
       * In order to achieve scale invariance, the given mask will always be
       * first scaled to fit into square mask of the given size at one side.
       *
       * Default size: 65
       */
      int maskSize;

      /**
       * Specify if the computed coefficients should be in cartesic form
       * (false) (i.e. real and imaginary parts) or polar form (true)
       * (i.e. magnitude and phase).
       *
       * Default value: true (i.e. polar form: magnitude and phase)
       */
      bool polar;

      /**
       * Basis functions that can be used
       */
      enum basisFunctionsType {
        MPEG7,       /**< Use MPEG-7 non-orthogonal basis functions */
        OrthoRadial, /**< Similar to the region-shape descriptor of MPEG-7, but
                          with an orthogonal radial basis function  */
        Zernike      /**< use Zernike moments                       */
      };

      /**
       * Which basis function set should be used.
       *
       * Default:  MPEG7
       */
      basisFunctionsType basisFunctionSet;
    };

    /**
     * default constructor
     */
    regionShapeFeatures();

    /**
     * default constructor with parameters
     */
    regionShapeFeatures(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    regionShapeFeatures(const regionShapeFeatures& other);

    /**
     * destructor
     */
    virtual ~regionShapeFeatures();

    /**
     * returns the name of this type ("regionShapeFeatures")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on the given %parameter.
     * @param src channel8 with the input binary mask.  All not-object pixels
     *            must be zero.  The object pixels must have a value 255.
     * @param dest channel with coefficients for the base functions.  (real
     *             part or magnitude, depending on parameters::polar value)
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& src,channel& dest) const;

    /**
     * operates on the given %parameter.
     * @param src channel8 with the input binary mask.  All not-object pixels
     *            must be zero.  The object pixels must have a value 255.
     * @param dest feature vector with coefficients for the base functions.
     *             (real part or magnitude, depending on
     *              parameters::polar value)
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& src,dvector& dest) const;

    /**
     * operates on the given %parameter.
     * @param src channel8 with the input binary mask.  All not-object pixels
     *            must be zero.  The object pixels must have a value 255.
     * @param dest1 channel with real part or magnitude of the coefficients,
     *              depending on the value of parameters::polar.
     * @param dest2 channel with imaginary part or phase of the coefficients,
     *              depending on the value of parameters::polar.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& src,channel& dest1,channel& dest2) const;

    /**
     * operates on the given %parameter.
     * @param src channel8 with the input binary mask.  All not-object pixels
     *            must be zero.  The object pixels must have a value 255.
     * @param dest1 feature vector with real part or magnitude of the
     *              coefficients, depending on the value of parameters::polar.
     * @param dest2 feature vector with imaginary part or phase of the
     *              coefficients, depending on the value of parameters::polar.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& src,dvector& dest1,dvector& dest2) const;

    /**
     * This method is for debugging purposes only.  It provides a tentative
     * reconstruction algorithm for the original shape using the given
     * coefficients.  Depending on the parameters::polar the output will
     * be in cartesic or in polar form.
     *
     * @param coef1 real part (or magnitude) of the coefficients.
     * @param coef2 imaginary part (or phase) of the coefficients.
     * @param shape1 reconstructed real part (or magnitude)
     * @param shape2 reconstructed imaginary part (or phase)
     * @return true if successful, false otherwise.
     */
    bool reconstruct(const channel& coef1,
                     const channel& coef2,
                     channel& shape1,
                     channel& shape2) const;

    /**
     * get one of the basis functions currently being used.
     * The indices must be in the valid range (specified in the parameters),
     * otherwise the function will return false.
     * @param n radial component (row)
     * @param m angular component (column)
     * @param real real part of the basis function
     * @param imag imaginary part of the basis function
     * @return true if successful, false otherwise
     */
    bool getBasisFunction(const int n, const int m,
                          channel& real,
                          channel& imag) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    regionShapeFeatures& copy(const regionShapeFeatures& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    regionShapeFeatures& operator=(const regionShapeFeatures& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * set functor's parameters.
     * This member makes a copy of <em>theParam</em>: the functor
     * will keep its own copy of the parameters!
     * @return true if successful, false otherwise
     */
    virtual bool updateParameters();

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  protected:
    /**
     * dot product between two matrices of the same size
     */
    void dot(const channel& reA, const channel& imA,
             const channel& reB, const channel& imB,
             float& real,float& imag) const;

    /**
     * calculates the look up tables (LUT) for the transformation
     * (suitable to the given parameters::maskSize)
     * @return true if successful
     */
    bool calcLUT();

    /**
     * Test the orthogonality of the base functions
     * The comparition matrix will be a square matrix with the size.x*sizey
     * elements at one size.
     */
    bool testOrthogonality(channel& ortho) const;

    /**
     * crop the object in the mask and resizes it to the parameters::maskSize
     * (here: the center of mass of the object is
     *        in the center of the new image)
     */
    void resizeMask(const channel8& src,
		    channel& ssrc) const;

    /**
     * real part of the basis functions (LUT)
     */
    std::vector< std::vector<channel> > lutR;

    /**
     * imaginary part of the basis functions (LUT)
     */
    std::vector< std::vector<channel> > lutI;

    /**
     * radius matrix.  It will contain the distance of a pixel to the
     * center of the image:
     * f(x,y) = radius ; radius = ((x-xcenter)^2+(y-ycenter)^2)^0.5
     */
    channel radLut;

    /**
     * static function used to binarize a channel.
     * All values under 0.5 will be set to 0 and all values above 0.5 will
     * be set to 1.0
     */
    static float binarize(const float& x);

    /**
     * flag to remember the last basis function set
     */
    parameters::basisFunctionsType lastBasisFunctionSet;


    /**
     * used to decide whether zernike moment (n,m) exists
     */
    std::vector<std::vector<bool> > vecNM;

    /**
     * compute factorial of arg
     */
    double fac(int arg);

  };
}

#endif

