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
 * file .......: ltiJPEGFunctor.cpp
 * authors ....: John Miano, Benjamin Winkler
 * organization: LTI, RWTH Aachen
 * creation ...: 27.10.00
 * revisions ..: $Id: ltiJPEGFunctor.cpp,v 1.23 2005/06/16 12:37:11 doerfler Exp $
 */

#include "ltiJPEGFunctor.h"

// the libjpeg functor has a higher priority than the local one. 
// If HAVE_LIBJPEG is defined, then this should not be compiled.

#if defined HAVE_LOCALJPEG && !defined HAVE_LIBJPEG

#include "ltijpegdeco.h"  // Jpeg-Decoder
#include "ltijpegenco.h"  // Jpeg-Encoder

#include "ltiMergeRGBToImage.h"

namespace lti {
  // -----------------------------------------------------------------------
  // ioJPEG::parameters
  // -----------------------------------------------------------------------

  ioJPEG::parameters::parameters()
    : ioFunctor::parameters(), quality(75),progressive(false),
                 comment(""), rowsPerRestart(0) {
  }

  ioJPEG::parameters&
  ioJPEG::parameters::copy(const ioJPEG::parameters& other) {

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
    // copy additional information
    quality = other.quality;
    progressive = other.progressive;
    comment = other.comment;
    rowsPerRestart = other.rowsPerRestart;

    return (*this);
  }


  functor::parameters* ioJPEG::parameters::clone() const  {
    return (new parameters(*this));
  }


  const char* ioJPEG::parameters::getTypeName() const {
    return "ioJPEG::parameters";
  }


  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was succeful
   */
# ifndef _LTI_MSC_6
  bool ioJPEG::parameters::write(ioHandler& handler,
                                         const bool complete) const
# else
  bool ioJPEG::parameters::writeMS(ioHandler& handler,
                                           const bool complete) const
# endif
  {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"quality",quality);
      lti::write(handler,"progressive",progressive);
      lti::write(handler,"comment",comment);
      lti::write(handler,"rowsPerRestart",rowsPerRestart);
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
  bool ioJPEG::parameters::write(ioHandler& handler,
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
  bool ioJPEG::parameters::read(ioHandler& handler,
                                const bool complete)
# else
  bool ioJPEG::parameters::readMS(ioHandler& handler,
                                  const bool complete)
# endif
  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"quality",quality);
      lti::read(handler,"progressive",progressive);
      lti::read(handler,"comment",comment);
      lti::read(handler,"rowsPerRestart",rowsPerRestart);
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
  bool ioJPEG::parameters::read(ioHandler& handler,
                               const bool complete) {
    // ...we need this workaround to cope with another really awful MSVC
    // bug.
    return readMS(handler,complete);
  }
# endif

  // -----------------------------------------------------------------------
  // ioJPEG
  // -----------------------------------------------------------------------

  // constructor
  ioJPEG::ioJPEG() {
    parameters defPar;
    setParameters(defPar);
  }

  // destructor
  ioJPEG::~ioJPEG() {
  }

  // returns the current parameters
  const ioJPEG::parameters& ioJPEG::getParameters() const {
    const parameters* params =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(params==0) throw invalidParametersException(getTypeName());
    return *params;
  }

  const char* ioJPEG::getTypeName() const {
    return "ioJPEG";
  }

  // ----------------------------------------------------------------------
  // loadJPEG
  // ----------------------------------------------------------------------

  // constructor
  loadJPEG::loadJPEG() : ioJPEG() {
  }

  // destructor
  loadJPEG::~loadJPEG() {
  }

  const char* loadJPEG::getTypeName() const {
    return "loadJPEG";
  }

  functor* loadJPEG::clone() const {
    return (new loadJPEG(*this));
  }

  // load the picture defined in parameters, determine whether a gray
  // scale image was loaded
  bool loadJPEG::apply(image& theImage, bool& isGray) {

    if (!validParameters()) {
      theImage.clear();
      setStatusString("Invalid parameters");
      return false;
    }

    jpeg::JpegDecoder jpegd;

    // read in file
    // PLEASE NOTE: This method throws exceptions, but
    // exceptions are not widely used in the LTIlib, which uses return
    // values instead. So we have to "convert" exceptions to return
    // values here. If you decide to redo this, please document all
    // thrown exceptions!
    try {
      jpegd.readImageFile(getParameters().filename, theImage, isGray);
    }
    catch (std::exception& exc) { // anything means failure
      setStatusString(exc.what());
      return false;
    }
    catch (...) {
      return false;
    }

    return !theImage.empty();
  }

  // load the picture defined in parameters
  bool loadJPEG::apply(image& theImage) {
    bool gray;
    return apply(theImage, gray);
  }

  // shortcut for apply (load file named "filename")
  bool loadJPEG::load(const std::string& filename,image& theImage) {

    parameters param;
    if(validParameters())
      param.copy(getParameters());
    param.filename = filename;
    setParameters(param); // a copy of param will be made!
    return apply(theImage);
  }

  // shortcut for apply (load file named "filename")
  bool loadJPEG::load(const std::string& filename,
                      image& theImage,
                      bool& isGray) {

    parameters param;
    if(validParameters())
      param.copy(getParameters());
    param.filename = filename;
    setParameters(param); // a copy of param will be made!
    return apply(theImage,isGray);
  }

  // shortcut for apply (load from stream)
  bool loadJPEG::load(std::ifstream& file,image& theImage) {

    jpeg::JpegDecoder jpegd;

    bool isGray;

    // read in file
    jpegd.readImageFile(file, theImage, isGray);

    return !theImage.empty();
  }


  // scans all chunks before the first SoS-chunk (!) for image information.
  bool loadJPEG::checkHeader(const std::string& filename,
                             point &imageSize,
                             std::string &comment,
                             bool &isGray) {
    parameters param;
    if(validParameters())
      param.copy(getParameters());
    param.filename = filename;
    setParameters(param);
    jpeg::JpegDecoder jpegd;
    jpegd.checkImageFile(getParameters().filename, imageSize,
                         comment, isGray);
    return true;
  }

  // ------------------------------------------------------------------
  // saveJPEG
  // ------------------------------------------------------------------
  // constructor
  saveJPEG::saveJPEG() : ioJPEG(){
  }

  saveJPEG::~saveJPEG() {
  }

  const char* saveJPEG::getTypeName() const {
    return "saveJPEG";
  }

  functor* saveJPEG::clone() const {
    return (new saveJPEG(*this));
  }

  bool saveJPEG::apply(const image& theImage) {
    if (!validParameters()) {
      setStatusString("Invalid parameters");
      return false;
    }

    // open file to write
    jpeg::JpegEncoder coder;
    coder.SetQuality(getParameters().quality);
    coder.SetProgressive(getParameters().progressive);
    coder.SetComment(getParameters().comment);
    coder.SetRowsPerRestart(getParameters().rowsPerRestart);
    coder.writeImageFile(getParameters().filename, theImage);

    return true;
  }

  bool saveJPEG::apply(const channel& theChannel) {
    channel8 theChannel8;
    theChannel8.castFrom(theChannel);
    return apply(theChannel8);
  };

  bool saveJPEG::apply(const channel8& theChannel) {
    if (!validParameters()) {
      setStatusString("Invalid parameters");
      return false;
    }

    image theImage;
    mergeRGBToImage merger;
    merger.apply(theChannel,theChannel,theChannel,theImage);

    //open file to write
    jpeg::JpegEncoder coder;
    coder.SetQuality(getParameters().quality);
    coder.SetProgressive(getParameters().progressive);
    coder.SetComment(getParameters().comment);
    coder.SetRowsPerRestart(getParameters().rowsPerRestart);
    coder.writeImageFile(getParameters().filename, theImage);
//todo  coder.setGrayscale(true);
//      coder.writeImageFile(getParameters().filename,theImage);
    setStatusString("Saving grey images - not implemented yet");
    return true;
  };

  bool saveJPEG::save (std::ofstream& s, const image& theImage) {
    if (!validParameters()) {
      setStatusString("Invalid parameters");
      return false;
    }

    // open file to write
    jpeg::JpegEncoder coder;
    coder.SetQuality(getParameters().quality);
    coder.SetProgressive(getParameters().progressive);
    coder.SetComment(getParameters().comment);
    coder.SetRowsPerRestart(getParameters().rowsPerRestart);
    coder.WriteImage(s, theImage);

    return true;
  };


  bool saveJPEG::save (const std::string& filename,const image& theImage) {
    parameters param;
    if(validParameters())
      param.copy(getParameters());
    param.filename = filename;
    setParameters(param); // a copy of param will be made!
    return apply(theImage);
  };

  bool saveJPEG::save (const std::string& filename,const channel& theChannel) {
    parameters param;
    if(validParameters())
      param.copy(getParameters());
    param.filename = filename;
    setParameters(param); // a copy of param will be made!
    return apply(theChannel);
  };

  bool saveJPEG::save (const std::string& filename,
                       const channel8& theChannel) {
    parameters param;
    if(validParameters())
      param.copy(getParameters());
    param.filename = filename;
    setParameters(param); // a copy of param will be made!
    return apply(theChannel);
  };

}

#endif

