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
 * file .......: ltiPNGFunctor.cpp
 * authors ....: Stefan Syberichs, Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 25.10.99
 * revisions ..: $Id: ltiPNGFunctor.cpp,v 1.25 2005/06/16 12:37:11 doerfler Exp $
 */

#include "ltiConfig.h"

/*
 * Implementation without PNG-Lib
 * The libpng functor has a higher priority than the local one. 
 * If HAVE_LIBPNG is defined, then this should not be compiled.
 */


#if defined HAVE_LOCALPNG && !defined HAVE_LIBPNG

#include <fstream>
#include <vector>
#include <map>
#include "ltiPNGFunctor.h"

namespace lti {
  // -----------------------------------------------------------------------
  // ioPNG
  // -----------------------------------------------------------------------

  ioPNG::parameters::parameters() : ioFunctor::parameters(),
         bitsPerPixel(24),quantColors(0),useAlphaChannel(false)  {
  }

  ioPNG::parameters& ioPNG::parameters::copy(const parameters& other) {

#ifndef _LTI_MSC_6
    // MS Visual C++ 6 is not able to compile this...
    ioFunctor::parameters::copy(other);
#else
    // ...so we have to use this workaround.
    // Conditional on that copy may not be virtual.
    ioFunctor::parameters& (ioFunctor::parameters::* p_copy)
      (const ioFunctor::parameters&) = ioFunctor::parameters::copy;
    (this->*p_copy)(other);
#endif
    bitsPerPixel = other.bitsPerPixel;
    quantColors = other.quantColors;
    useAlphaChannel=other.useAlphaChannel;

    return (*this);
  }

  functor::parameters* ioPNG::parameters::clone() const  {
    return (new parameters(*this));
  }

  const char* ioPNG::parameters::getTypeName() const {
    return "ioPNG::parameters";
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was succeful
   */
# ifndef _LTI_MSC_6
  bool ioPNG::parameters::write(ioHandler& handler,
                                         const bool complete) const
# else
  bool ioPNG::parameters::writeMS(ioHandler& handler,
                                           const bool complete) const
# endif
  {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"bitsPerPixel",bitsPerPixel);
      lti::write(handler,"quantColors",quantColors);
      lti::write(handler,"useAlphaChannel",useAlphaChannel);
    }

# ifndef _LTI_MSC_6
    // This is the standard C++ code, which MS Visual C++ 6 is not able to
    // compile...
    b = b && ioFunctor::parameters::write(handler,false);
# else
    bool (ioFunctor::parameters::* p_writeMS)(ioHandler&,const bool) const =
      ioFunctor::parameters::writeMS;
    b = b && (this->*p_writeMS)(handler,false);
# endif

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

# ifdef _LTI_MSC_6
  bool ioPNG::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    // ...we need this workaround to cope with another really awful MSVC bug.
    return writeMS(handler,complete);
  }
# endif

  /*
   * read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if write was succeful
   */
# ifndef _LTI_MSC_6
  bool ioPNG::parameters::read(ioHandler& handler,
                                        const bool complete)
# else
  bool ioPNG::parameters::readMS(ioHandler& handler,
                                          const bool complete)
# endif
  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"bitsPerPixel",bitsPerPixel);
      lti::read(handler,"quantColors",quantColors);
      lti::read(handler,"useAlphaChannel",useAlphaChannel);
    }

# ifndef _LTI_MSC_6
    // This is the standard C++ code, which MS Visual C++ 6 is not able to
    // compile...
    b = b && ioFunctor::parameters::read(handler,false);
# else
    bool (ioFunctor::parameters::* p_readMS)(ioHandler&,const bool) =
      ioFunctor::parameters::readMS;
    b = b && (this->*p_readMS)(handler,false);
# endif

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

# ifdef _LTI_MSC_6
  bool ioPNG::parameters::read(ioHandler& handler,
                               const bool complete) {
    // ...we need this workaround to cope with another really awful MSVC
    // bug.
    return readMS(handler,complete);
  }
# endif

  // returns the current parameters
  const ioPNG::parameters& ioPNG::getParameters() const {
    const parameters* params =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(params==0) throw invalidParametersException(getTypeName());
    return *params;
  }


  // ----------------------------------------------------------------------
  // ioPNG
  // ----------------------------------------------------------------------

  // constructor
  ioPNG::ioPNG() {
    parameters param;
    setParameters(param);
  }

  // destructor
  ioPNG::~ioPNG() {
  }

  const char* ioPNG::getTypeName() const {
    return "ioPNG";
  }
  // ----------------------------------------------------------------------
  // loadPNG
  // ----------------------------------------------------------------------

  loadPNG::loadPNG() : ioPNG(){
  }

  loadPNG::~loadPNG() {
  }

  const char* loadPNG::getTypeName() const {
    return "loadPNG";
  }

  functor* loadPNG::clone() const {
    return (new loadPNG(*this));
  }

  // load RGB image

  bool loadPNG::apply(image& theImage) {
    // clear image:
    theImage.clear();

    if (!validParameters()) {
      setStatusString("Invalid parameters");
      return false;
    }

    // open file to read
    png::PngDecoder pngd;
    try {
      pngd.readImageFile(getParameters().filename, theImage);
    }
    catch (std::exception& exc) {
      setStatusString(exc.what());
      return false;
    }
    catch (...) {
      return false;
    }

    return !theImage.empty();
  }

  // load channel and palette

  bool loadPNG::apply(channel8& theChannel,lti::palette& colors) {
    // clear image:

    theChannel.clear();

    if (!validParameters()) {
      setStatusString("Invalid parameters");
      return false;
    }

    // open file to read
    png::PngDecoder pngd;
    try {
      pngd.readImageFile(getParameters().filename, theChannel,colors);
    }
    catch (std::exception& exc) {
      setStatusString(exc.what());
      return false;
    }

    return !theChannel.empty();
  }

  // shortcut for apply (load file with name "filename")
  bool loadPNG::load(const std::string& filename,image& theImage) {
    parameters param;
    if(validParameters())
      param.copy(getParameters());
    param.filename = filename;
    setParameters(param); // a copy of param will be made!
    return apply(theImage);
  }

  // (load file from stream)
  bool loadPNG::load(std::ifstream& file,image& theImage) {
    theImage.clear();

    if (!validParameters()) {
      setStatusString("Invalid parameters");
      return false;
    }

    // open file to read
    png::PngDecoder pngd;
    try {
      pngd.readImageFile(file, theImage);
    } catch (std::exception& exc) {
      setStatusString(exc.what());
      return false;
    }

    return !theImage.empty();
  }

  // shortcut for apply (load file with name "filename")
  bool loadPNG::load(const std::string& filename,
                     channel8& theChannel,
                     lti::palette& colors) {
    parameters param;
    if(validParameters())
      param.copy(getParameters());
    param.filename = filename;
    setParameters(param); // a copy of param will be made!
    return apply(theChannel,colors);
  }

  bool loadPNG::checkHeader(const std::string& filename,
                            point& imageSize,
                            int& bitsPerPixel,
                            int& imageType) {

    png::PngDecoder pngd;
    return pngd.checkHeader(filename,imageSize,bitsPerPixel,imageType);
  }


  // ------------------------------------------------------------------
  // saveBMP
  // ------------------------------------------------------------------

  savePNG::savePNG() : ioPNG(){
    parameters p;
    setParameters(p);
  }

  savePNG::~savePNG() {
    // do nothing
  }

  const char* savePNG::getTypeName() const {
    return "savePNG";
  }

  functor* savePNG::clone() const {
    return (new savePNG(*this));
  }

  bool savePNG::apply(const image& theImage) {
    if (!validParameters()) {
      setStatusString("Invalid parameters");
      return false;
    }

    // open file to write
    png::PngEncoder coder;

    // needed to engage compression!
    coder.setUseFilters(true);

    if (getParameters().useAlphaChannel) {
      coder.writeImageFile(getParameters().filename,
                           theImage,
                           png::PngEncoder::RGBalpha);
    } else {
      coder.writeImageFile(getParameters().filename,
                           theImage,
                           png::PngEncoder::RGB24);
    }
    return true;
  }


  // saves float-channel as an 8-bit gray image
  bool savePNG::apply(const channel& theChannel) {

    channel8 chnl;

    chnl.castFrom(theChannel);

    return apply(chnl);
  }

  /*
   * saves 8-bit channel into 24-bit RGB-BMP by assigning each
   * R, G and B the same grey-value...
   */
  bool savePNG::apply(const channel8& theChannel,
                      const lti::palette& colors) {

    if (!validParameters()) {
      setStatusString("Invalid parameters");
      return false;
    }

    // the color palette to be used!

    lti::palette thePalette(256);
    png::PngEncoder::pngColorID colorType;

    colorType = png::PngEncoder::Palette8;

    if (colors.size() == 0) {
      // generate a gray-valued palette
      int i;
      for (i=0;i<256;i++) {
        thePalette.at(i)=rgbPixel(i,i,i);
      }
    } else {
      // use the given palette
      thePalette.copy(colors);

      if (colors.size() <= 2) {
        colorType = png::PngEncoder::Palette1;
      } else if (colors.size() <= 4) {
        colorType = png::PngEncoder::Palette2;
      } else if (colors.size() <= 16) {
        colorType = png::PngEncoder::Palette4;
      }

    }

    if (getParameters().bitsPerPixel > 16) {
      colorType = png::PngEncoder::RGB24;
    }

    // open file to write
    png::PngEncoder coder;
    coder.writeImageFile(getParameters().filename,
                         theChannel,
                         thePalette,
                         colorType);

    return true;
  }

  /*
   * shortcut for save
   */
  bool savePNG::save(const std::string& filename,
                     const image& theImage) {
    parameters param;
    if (validParameters())
      param.copy(getParameters());
    param.filename = filename;
    setParameters(param); // a copy of param will be made!
    return apply(theImage);
  }

  /*
   * shortcut for save
   */
  bool savePNG::save(const std::string& filename,
                     const channel8& theChannel,
                     const lti::palette& colors) {
    parameters param;
    if (validParameters())
      param.copy(getParameters());
    param.filename = filename;
    setParameters(param); // a copy of param will be made!
    return apply(theChannel,colors);
  }

  /*
   * shortcut for save
   */
  bool savePNG::save(const std::string& filename,
                     const channel& theChannel) {

    parameters param;
    if (validParameters())
      param.copy(getParameters());
    param.filename = filename;
    setParameters(param);             // a copy of param will be made!
    return apply(theChannel);
  }
}

#endif

