/*
 * Copyright (C) 1998, 1999, 2000, 2001
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 *
 * This file can be used with the LTI-Computer Vision Library (LTI-Lib)
 * (please note this file is NOT part of the LTI-Lib)
 *
 * This code is based on the original C++ code by John M. Miano
 * (Colosseum Builders, Inc.) which contains following licence: (the
 * original licence file can be found on 00readme.txt in the io/png
 * and io/jpeg directories):
 *
 ******************************************************************************
 *
 * Colosseum Builders C++ Image Library
 *
 * Copyright 1997, 1998, 1999
 * All Rights Reserved
 *
 * Permision is granted to use, modify this software provided these conditions
 * are followed:
 *
 * 1. You understand this software comes with no warranties of any kind
 *    whatsoever.
 * 2. You assume full responsibility for the use of this software and transmit
 *    no liability to the authors.
 * 3. You assume the responsility for obtaining licenses for from any patent
 *    holders.
 * 4. If you distribute this code in source form, you must include this file
 *    unmodified and document the changes you have made.
 * 5. If you incorporate this code in a product distributed without source code
 *    you state included in your documentation that it is based in part on
 *    software from Colosseum Builders, Inc.
 *
 *    THIS SOFTWARE IS PROVIDED AS IS. WE CAN'T TEST IT ON YOUR SYSTEM SO
 *    USE AT YOUR OWN RISK. YOU HAVE THE SOURCE SO YOU HAVE NO EXCUSES.
 *
 ******************************************************************************
 *
 * The modifications of the original code are mainly the adaptation
 * for the LTI-Lib types and the use of namespaces to create an own
 * scope for the original functions
 *
 */

/*----------------------------------------------------------------
 * project ....: LTI Digital Image/Signal Processing Library
 * file .......: jpegdeco.cpp
 * authors ....: John Miano, Benjamin Winkler
 * organization: LTI, RWTH Aachen
 * creation ...: 27.10.00
 * revisions ..: $Id: ltijpegdeco.cpp,v 1.8 2003/02/17 07:16:03 alvarado Exp $
 */

//
// JPEG Decoder Class Implementation
//
#include "ltijpegdeco.h"

using namespace std ;

namespace lti {
  namespace jpeg {
//
//  Description:
//
//    Class Default Constructor
//
JpegDecoder::JpegDecoder ()
{
  Initialize () ;
  return ;
}


//
//  Description:
//
//    Class Destructor
//
JpegDecoder::~JpegDecoder ()
{
  delete [] ac_tables ; ac_tables = NULL ;
  delete [] dc_tables ; dc_tables = NULL ;

  delete [] quantization_tables ; quantization_tables = NULL ;
  delete [] components ; components = NULL ;
  delete [] component_indices ; component_indices = NULL ;
  delete [] scan_components ; scan_components = NULL ;

  delete big_endian;

  return ;
}

//
//  Description:
//
//    Class initialization procudure for use by constructors.
//
void JpegDecoder::Initialize ()
{
  verbose_flag = false ;
  strict_jfif = false ;

  ac_tables = new JpegHuffmanDecoder [JpegMaxHuffmanTables] ;
  dc_tables = new JpegHuffmanDecoder [JpegMaxHuffmanTables] ;

  quantization_tables = new JpegDecoderQuantizationTable [JpegMaxQuantizationTables] ;
  components = new JpegDecoderComponent [JpegMaxComponentsPerFrame] ;
  component_indices = new int [JpegMaxComponentsPerFrame] ;

  scan_components = new JpegDecoderComponent * [JpegMaxComponentsPerScan] ;

  big_endian = new lti::endianness(lti::endianness::BigEndian);

  img_comment = "";

  return ;
}
//
//  Description:
//
//    This function returns the next byte from the input stream.
//
lti::ubyte JpegDecoder::ReadByte ()
{
  lti::ubyte value ;
  input_stream->read ((char *) &value, sizeof (value)) ;
  bit_position = 0 ;

  if (input_stream->eof ())
    throw JpegBadData ("Premature end of file") ;
  return value ;
}

//
//  Description:
//
//    This function reads the next 2-byte integer from the input stream
//    and returns the value in the correct format for the system.
//

lti::uint16 JpegDecoder::ReadWord ()
{
  bit_position = 0 ;
  lti::uint16 value ;
  big_endian->read((*input_stream), value);
  return value;
}

//
//  Description:
//
//    Section F.2.2.4
//
//    Extracts the next "count" bits from the input stream.
//
int JpegDecoder::Receive (int count)
{
  int result = 0 ;
  for (int ii = 0 ; ii < count ; ++ii)
  {
    result <<= 1 ;
    result |= NextBit () ;
  }
  return result ;
}

//
//  Description:
//
//    This function reads the Start of Image Marker and the JFIF APP0
//    marker that begin a JPEG file.
//
//    The JFIF standard states "The JPEG FIF APP0 marker is mandatory
//     right after the SOI marker."
//
//    I have come across some JPEG files that have a COM marker between
//    the SOI marker and APP0. This code will reject these non-conforming
//    files.
//

void JpegDecoder::ReadStreamHeader ()
{
  if (ReadByte () != SOB)
    throw JpegBadData ("Missing SOI Marker") ;
  if (ReadByte () != SOI)
    throw JpegBadData ("Missing SOI Marker") ;
  if (ReadByte () != SOB)
    throw JpegBadData ("Missing JFIF APP0 Marker") ;
  if (ReadByte () != APP0)
    throw JpegBadData ("Missing JFIF APP0 Marker") ;

  JfifHeader header ;
//!  input_stream->read ((char *) &header, sizeof (header)) ;
  header.read(*input_stream);
  if (memcmp ("JFIF", (char *) header.data.identifier, 4) != 0)
    throw JpegBadData ("Not a JFIF file") ;

  if (verbose_flag)
  {
  std::cout << "{ Start Of Image }" << endl ;
    std::cout << "{ JFIF APP0 Marker" << endl ;
//!    std::cout << "  Length: " << dec << BigEndianToSystem (header.length) << endl ;
    std::cout << "  Length: " << dec << header.data.length << endl ;
    std::cout << "  Version: " << dec << (int) header.data.version [0]
         << "." << (int) header.data.version [1] << endl ;
    // density unit = 0 => Only the aspect ratio is specified.
    // density unit = 1 => Density in pixels per inch.
    // density unit = 2 => Density in pixels per centimeter.
    std::cout << "  Density Unit: " ;
    switch (header.data.units)
    {
    case 0:
      std::cout << " (aspect ratio)" << endl ;
      break ;
    case 1:
      std::cout << " (pixels per inch)" << endl ;
      break ;
    case 2:
      std::cout << " (pixels/cm)" << endl ;
      break ;
    default:
      std::cout << " (?)" << endl ;
      break ;
    }
    std::cout << "  X Density: " << dec << header.data.xdensity << endl ;
    std::cout << "  Y Density: " << dec << header.data.xdensity << endl ;
    std::cout << "  Thumbnail Width: " << dec << (int) header.data.xthumbnail << endl ;
    std::cout << "  Thumbnail Height: " << dec << (int) header.data.xthumbnail << endl ;
    std::cout << "}" << endl ;
  }

  // Skip over any thumbnail data.
//! for (int ii = sizeof (header) ; ii < BigEndianToSystem (header.length) ; ++ ii)
  for (int ii = header.size(); ii < header.data.length ; ++ii)
    (void) ReadByte () ;

  return ;
}

//
//  Description:
//
//    Skip the following marker
//
void JpegDecoder::SkipMarker()
{
  lti::uint16 length = ReadWord();
  for (lti::uint16 ii = sizeof(lti::uint16); ii < length; ii++)
  (void) ReadByte();
}

//
//  Description:
//
//    This function reads the next marker in the input
//    stream. If the marker is followed by a data block
//    this function dispatches a routine to read the
//    data.
//
void JpegDecoder::ReadMarker ()
{
  while (! input_stream->eof ())
  {
    lti::ubyte type = ReadByte () ;
    switch (type)
    {
    case SOB:
      // According to E.1.2, 0xFF is allowed as fill when a
      // marker is expected.
      break ;
    case SOI:
      if (verbose_flag)
        std::cout << "{ Start Of Image }" << endl ;
      return ; // SOI has no data.
    case DQT:
      ReadQuantization () ;
      return ;
    case DHP:
      throw JpegBadData ("DHP marker not supported") ;

    // The only difference between a Sequential DCT Frame
    // (SOF0) and an extended Sequential DCT Frame (SOF1)
    // is that a baseline frame may only have 2 DC and 2 AC
    // Huffman tables per scan (F.1.2) and and extended
    // Sequential Frame may have up to 4 DC and 4 AC Huffman
    // tables per scan (F.1.3). Both are decoded identically
    // for 8-bit precision. Extended Sequential frames may
    // use 12-bit precision (F, Table B.2) which we do not
    // support.
    case SOF0:
    case SOF1:
    case SOF2:
      ReadStartOfFrame (type) ;
      return ;
    case SOF3:
      throw JpegBadData ("Lossless Huffman Coding Not Supported") ;
    case SOF5:
      throw JpegBadData (
        "Differential Sequential Huffman Coding Not Supported") ;
    case SOF6:
      throw JpegBadData (
        "Differential Progressive Huffman Coding Not Supported") ;
    case SOF7:
      throw JpegBadData (
        "Differential Lossless Huffman Coding Not Supported") ;

    // These are markers for frames using arithmetic coding.
    // Arithmetic coding is covered by patents so we ignore
    // this type.
    case SOF9:
    case SOFA:
    case SOFB:
    case SOFD:
    case SOFE:
    case SOFF:
      throw JpegBadData (
        "Cannot read image - Arithmetic Coding covered by patents") ;
    case DHT:
      ReadHuffmanTable () ;
      return ;
    case SOS:
      ReadStartOfScan () ;
      return ;
    case DRI:
      ReadRestartInterval () ;
      return ;
    case EOI:
      eoi_found = true ;
      if (verbose_flag)
        std::cout << "{ End Of Image }" << endl ;
      return ; // End of Image marker has no data
    case APP0:
    case APP1:
    case APP2:
    case APP3:
    case APP4:
    case APP5:
    case APP6:
    case APP7:
    case APP8:
    case APP9:
    case APPA:
    case APPB:
    case APPC:
    case APPD:
    case APPE:
    case APPF:
    case COM:
      ReadApplication (type) ;
      return ;
    default:
      // We call ReadByte to make sure the problem
      // is not a premature EOF.
      (void) ReadByte () ;
      throw JpegBadData (
      "Unknown, unsupported, or reserved marker encountered") ;
    }
  }
  throw JpegBadData ("Premature end of file") ;
}

//
//  Description:
//
//    This function reads the next marker in the input
//    stream.
//    It reads only the markers important for the image information
//    and does nothing concerning the image content and skips these
//    makers.
//
void JpegDecoder::ReadHeaderMarker ()
{
  while (! input_stream->eof ())
  {
    lti::ubyte type = ReadByte () ;
    switch (type)
    {
    case SOB:
      break ;
    case SOI:
      return ; // SOI has no data.
    case DQT:
    SkipMarker() ;
//      ReadQuantization () ;
      return ;
    case DHP:
      throw JpegBadData ("DHP marker not supported") ;

    // The only difference between a Sequential DCT Frame
    // (SOF0) and an extended Sequential DCT Frame (SOF1)
    // is that a baseline frame may only have 2 DC and 2 AC
    // Huffman tables per scan (F.1.2) and and extended
    // Sequential Frame may have up to 4 DC and 4 AC Huffman
    // tables per scan (F.1.3). Both are decoded identically
    // for 8-bit precision. Extended Sequential frames may
    // use 12-bit precision (F, Table B.2) which we do not
    // support.
    case SOF0:
    case SOF1:
    case SOF2:
      ReadStartOfFrame (type, true) ;   // rausspringen!!
      return ;
    case SOF3:
      throw JpegBadData ("Lossless Huffman Coding Not Supported") ;
    case SOF5:
      throw JpegBadData (
        "Differential Sequential Huffman Coding Not Supported") ;
    case SOF6:
      throw JpegBadData (
        "Differential Progressive Huffman Coding Not Supported") ;
    case SOF7:
      throw JpegBadData (
        "Differential Lossless Huffman Coding Not Supported") ;

    // These are markers for frames using arithmetic coding.
    // Arithmetic coding is covered by patents so we ignore
    // this type.
    case SOF9:
    case SOFA:
    case SOFB:
    case SOFD:
    case SOFE:
    case SOFF:
      throw JpegBadData (
        "Cannot read image - Arithmetic Coding covered by patents") ;
    case DHT:
    SkipMarker();
      return ;
    case SOS:
      eoi_found = true ;    // hier stimmt die länge nicht!!!
      return ;
    case DRI:
    SkipMarker();
      return ;
    case EOI:
      eoi_found = true ;
      if (verbose_flag)
        std::cout << "{ End Of Image }" << endl ;
      return ; // End of Image marker has no data
    case APP0:
    case APP1:
    case APP2:
    case APP3:
    case APP4:
    case APP5:
    case APP6:
    case APP7:
    case APP8:
    case APP9:
    case APPA:
    case APPB:
    case APPC:
    case APPD:
    case APPE:
    case APPF:
    case COM:
      ReadApplication (type) ;
      return ;
    default:
      // We call ReadByte to make sure the problem
      // is not a premature EOF.
      (void) ReadByte () ;
      throw JpegBadData (
      "Unknown, unsupported, or reserved marker encountered") ;
    }
  }
  throw JpegBadData ("Premature end of file") ;
}

//
//  Description:
//
//    This method reads an application or comment marker
//    from the input stream.
//
//  Parameters:
//    type:  The marker type
//
void JpegDecoder::ReadApplication (lti::ubyte type)
{
  lti::uint16 length = (lti::uint16)(ReadWord () - sizeof(lti::uint16));
//  char id [512] ;
  char id;
  int ii = 0 ;

  if (img_comment != "")
  img_comment+="\n";
  for (ii = 0; ii < length; ii++) {
  id = ReadByte ();
  if (id == 0)
    break;
    img_comment+=id;
  }


/*
  id [ii] = ReadByte () ;
  for (ii = 1 ; id [ii - 1] != '\000'
                && ii < (sizeof (id) - 1)
                && ii < (int)(length - sizeof (lti::uint16)) ; ++ ii)
  {
    id [ii] = ReadByte () ;
  }*/

/*  if (type == COM)
  img_comment = std::string(id);*/

  for ( ; ii < (int)(length - sizeof (lti::uint16)) ; ++ ii)
    (void) ReadByte () ;

  if (verbose_flag)
  {
    if (type == COM)
      std::cout << "( Comment Marker" << endl ;
    else
    std::cout << "{ APP" << hex << (lti::uint16) (type & 0x0F) << " Marker" << endl ;
    std::cout << "Length: " << dec << length << endl ;
    std::cout << "ID: " << img_comment << endl ;
    std::cout << "}" << endl ;
  }
  return ;
}

//
//  Description:
//
//    The function reads a Define Huffman Table marker from the input
//    stream.
//
void JpegDecoder::ReadHuffmanTable ()
{
  // Section B.2.4.2

  if (verbose_flag)
  std::cout << "{ Define Huffman Table" << endl ;

  lti::uint16 length = ReadWord () ;
  int remaining = length - sizeof (length) ;
  while (remaining > 0)
  {
    lti::ubyte data = ReadByte () ; -- remaining ;

    // Tc in standard 0=>DC, 1=>AC
    int tableclass = data >> 4 ;
    int id = data & 0x0F ; // Th in standard
    if (id > 3)
    {
      throw JpegBadData (
        "Huffman Table Index outside range [0..3]") ;
    }
    if (verbose_flag)
    {
      std::cout << "   Table Index " << (int) id << endl ;
      if (tableclass == 0)
        std::cout << "   Table Class: DC" << endl ;
      else
        std::cout << "   Table Class: AC" << endl ;
    }
    if (id > 3)
    {
      std::cout << "Bad index " << id << endl ;
      return ;
    }

    JpegHuffmanDecoder *table ;
    if (tableclass != 0)
      table = &ac_tables [id] ;
    else
       table = &dc_tables [id] ;

    // Read the table data into the table object
    remaining -= table->ReadTable (*this) ;

    if (verbose_flag)
    {
      table->Dump (std::cout) ;
    }
  }

  if (verbose_flag)
    std::cout << "}" << endl ;
  return ;
}

//
//  Description:
//
//    This function reads a DQT marker from the input stream.
//
void JpegDecoder::ReadQuantization ()
{
  // Defined in Section B.2.4.1

  lti::uint16 length = ReadWord () ;
  lti::ubyte data ;

  // Maintain a counter for the number of bytes remaining to be read in
  // the quantization table.
  int remaining = length - sizeof (length) ;

  if (verbose_flag)
  {
    std::cout << "{ Define Quantization Table" << endl ;
    std::cout << "  Length: " << length << endl ;
  }
  while (remaining > 0)
  {
    data = ReadByte () ; -- remaining ;
    int precision = data >> 4 ;    // Pq in standard
    int index = data & 0x0F ;      // Tq in standard

    if (index >= JpegMaxQuantizationTables)
      throw JpegBadData ("Quantization Table Index Too Large") ;

    if (verbose_flag)
    {
      std::cout << "  Table Index: " << dec << index << endl ;
      std::cout << "  Table Precision: " << dec << precision << endl ;
    }

    switch (precision)
    {
    case 1:
      remaining -= sizeof(lti::uint16) * JpegSampleSize ;
      break ;
    case 0:
      remaining -= sizeof (lti::ubyte) * JpegSampleSize ;
      break ;
    }

    // Read the table data into the table object
    quantization_tables [index].ReadTable (*this, precision) ;

    if (verbose_flag)
    {
      std::cout << "  Table Values: " ;
      quantization_tables [index].Print (std::cout) ;
      std::cout << endl << "}" << endl ;
    }
  }
  return ;
}

//
//  Description:
//
//    This function reads a define restart interval marker
//    from the input stream.
//
void JpegDecoder::ReadRestartInterval ()
{
  // Section B.2.4.4

  lti::uint16 length = ReadWord () ;
  restart_interval = ReadWord () ;
  if (verbose_flag)
  {
    std::cout << "{ Define Restart Interval" << endl ;
    std::cout << "  Length:  " << dec << length << endl ; // Should be 4
    std::cout << "  Interval: " << dec << restart_interval << endl ;
    std::cout << "}" << endl ;
   }
   return ;
}

//
//  Description:
//
//    The function reads a start of frame marker from the input stream.
//
//  Parameters:
//    type:  The marker type for the frame
//
void JpegDecoder::ReadStartOfFrame (lti::ubyte type, bool check_only)
{
  if (type == SOF2)
    progressive_frame = true ;
  else
    progressive_frame = false ;

  // Section B.2.2
  // Read in the image dimensions
  int length = ReadWord () ;
  int dataprecision = ReadByte () ;  // P in standard
  if (dataprecision != 8)
    throw JpegBadData ("Only 8-bit data supported") ;

  frame_height = ReadWord () ;            // Y in standard
  frame_width = ReadWord () ;             // X in standard
  component_count = ReadByte () ;   // Nf in standard

  // JFIF only allows 1 or 3 components.
  if (component_count != 1 && component_count != 3)
    throw JpegBadData ("JFIF only supports 1 and 3 component streams") ;

  frame_type = type ;

  if (verbose_flag)
  {
    std::cout << "{ Start Of Frame " << endl ;
    std::cout << "  Length: " << dec << length << endl ;
    std::cout << "  Precision: " << dec << dataprecision << endl ;
    std::cout << "  Height: " << dec << frame_height << endl ;
    std::cout << "  Width: " << dec << frame_width << endl ;
    std::cout << "  Component Count: " << component_count << endl ;
  }

  if (length != (component_count * 3 + 8))
    throw JpegBadData ("Invalid Frame Size") ;

  // Rread the component descriptions
  max_horizontal_frequency = 0 ;
  max_vertical_frequency = 0 ;
  for (int ii = 0 ; ii < component_count ; ++ ii)
  {
  int ID = ReadByte () ;  // Ci in standard
    int qtable ;

    // While JPEG does not put these restrictions on component IDs
    // the JFIF standard does.
    if (strict_jfif)
    {
    if (component_count == 1 && ID != 1)
      throw JpegBadData ("Component ID not 1") ;
    else if (ID != ii + 1)
      throw JpegBadData ("Invalid Component ID or ID out of order") ;
    }

    component_indices [ii] = ID ;

    lti::ubyte data = ReadByte () ;
    components [ID].HorizontalFrequency (data >> 4) ; // Hi in standard
    components [ID].VerticalFrequency (data & 0xF) ;  // Vi in standard
    qtable= ReadByte () ;  // Tqi in standard
    if (qtable >= JpegMaxQuantizationTables)
      throw JpegBadData ("Bad Quantization Table Index") ;
    components [ID].SetQuantizationTable (quantization_tables [qtable]) ;

    // Keep track of the largest values for horizontal and vertical
    // frequency.
    if (components [ID].HorizontalFrequency ()
        > max_horizontal_frequency)
    {
      max_horizontal_frequency =
        components [ID].HorizontalFrequency () ;
    }

    if (components [ID].VerticalFrequency ()
          > max_vertical_frequency)
    {
      max_vertical_frequency =
        components [ID].VerticalFrequency () ;
    }

    if (verbose_flag)
    {
      std::cout << "   Component " << ID << endl ;
      std::cout << "   Horizontal Frequency: "
           << components [ID].HorizontalFrequency () << endl ;
      std::cout << "   Vertical Frequency: "
           << components [ID].VerticalFrequency () << endl ;
      std::cout << "   Quantization Table: "
           << qtable << endl ;
    }
  }

  CalculateMcuDimensions () ;
  sof_found = true ;

  // genug gesehen für CheckHeader
  if (check_only)
  return;

  // Allocate storage for the image.
  current_image->resize (frame_height, frame_width) ;

  if (verbose_flag)
    std::cout << "}" << endl ;

  return ;
}

//
//  Description:
//
//    This function reads a start of scan marker and the scan data
//    following the marker.
//
void JpegDecoder::ReadStartOfScan ()
{
  int ii ;

  if (! sof_found)
    throw JpegBadData ("Scan found before frame defined") ;

  // Section B.2.3

  lti::uint16 length = ReadWord () ;
  if (verbose_flag)
  {
    std::cout << "{ Start Of Scan " << endl ;
    std::cout << "  Length:  " << dec << length << endl ;
  }

  scan_component_count = ReadByte () ;  // Ns in standard
  // Right now we can only handle up to three components.
  if (scan_component_count > 3 || scan_component_count < 1)
    throw JpegBadData ("Invalid component count in scan") ;

  for (ii = 0 ; ii < scan_component_count ; ++ ii)
  {
    lti::ubyte componentID = ReadByte () ;  // Csi in standard

    scan_components [ii] = &components [componentID] ;
    // If the horizontal frequency is zero then the component was not
    // defined in the SOFx marker.
    if (scan_components [ii]->HorizontalFrequency () == 0)
      throw JpegBadData ("Component Not Defined") ;

    lti::ubyte rb = ReadByte () ;
    int actable = rb & 0x0F ;
    int dctable = rb >> 4 ;

    scan_components [ii]->SetHuffmanTables (
                              dc_tables [dctable],
                              ac_tables [actable]) ;
    if (verbose_flag)
    {
      std::cout << "  Component ID: "
           << dec << componentID << endl ;
      std::cout << "  DC Entropy Table: "
           << dec << dctable << endl  ;
      std::cout << "  AC Entropy Table: "
           << dec << actable << endl  ;
    }
  }

  int spectralselectionstart = ReadByte () ; // Ss in standard
  int spectralselectionend = ReadByte ()  ;  // Se in standard

  lti::ubyte ssa = ReadByte () ;
  int successiveapproximationhigh = ssa >> 4 ;  // Ah in standard
  int successiveapproximationlow = ssa & 0x0F ; // Al in standard

  if (verbose_flag)
  {
    std::cout << " Spectral Selection Start: "
         << dec << spectralselectionstart << endl ;
    std::cout << " Spectral Selection End: "
         << dec << spectralselectionend << endl ;
    std::cout << " Successive Approximation High: "
         << dec << successiveapproximationhigh << endl ;
    std::cout << " Successive Approximation Low: "
         << dec << successiveapproximationlow << endl  ;
    std::cout << "}" << endl ;
  }

  for (ii = 0 ; ii < scan_component_count ; ++ ii)
  {
    if (progressive_frame)
    {
      scan_components [ii]->CheckQuantizationTable () ;
      if (spectralselectionstart == 0)
        scan_components [ii]->CheckDcTable () ;
      else
        scan_components [ii]->CheckAcTable () ;
    }
    else
    {
      scan_components [ii]->CheckQuantizationTable () ;
      scan_components [ii]->CheckDcTable () ;
      scan_components [ii]->CheckAcTable () ;
    }
    scan_components [ii]->AllocateComponentBuffers (*this) ;

  }

  ++ current_scan ;
  if (progressive_frame)
  {
    ReadProgressiveScanData (
                          spectralselectionstart,
                          spectralselectionend,
                          successiveapproximationhigh,
                          successiveapproximationlow) ;
  }
  else
  {
    ReadSequentialScanData () ;
  }

  return ;
}

//
//  Description:
//
//    This function determines for non-interlaced scans:
//
//     o The dimensions in pixels for an MCU
//     o The number of MCU rows and columns needed to encode the scan.
//
void JpegDecoder::CalculateMcuDimensions ()
{
  mcu_height = max_vertical_frequency * JpegSampleWidth ;
  mcu_width = max_horizontal_frequency * JpegSampleWidth ;
  mcu_rows = (frame_height + mcu_height - 1) / mcu_height ;
  mcu_cols = (frame_width + mcu_width - 1) / mcu_width ;
  return ;
}


//
//  Description:
//
//    This function reads the scan data for progressive scans.
//
//    All we do here is determine if we are processing a DC
//    scan (sss==sse==0) or AC scan and if we are processing
//    the first scan for the spectral selection (sah==0) or
//    subsequent scan.
//
//  Parameters:
//    sss: Spectral Selection Start (0..63)
//    sse: Spectral Selection End (sse..63)
//    sah: Successive Approximation High
//    sal: Successive Approximation Low
//
void JpegDecoder::ReadProgressiveScanData (int sss, int sse,
                                           int sah, int sal)
{
  if (sss == 0)
  {
    if (sse != 0)
      throw JpegBadData ("Progressive scan contains DC and AC data") ;

    if (sah == 0)
    {
      ReadDcFirst (sal) ;
    }
    else
    {
      ReadDcRefine (sal) ;
    }
  }
  else
  {
    if (sah == 0)
    {
      ReadAcFirst (sss, sse, sal) ;
    }
    else
    {
      ReadAcRefine (sss, sse, sal) ;
    }
  }
  return ;
}

//
//  Description:
//
//    This funtion reads the scan data for the first DC scan for
//    one or more components.
//
//  Parameters:
//
//    ssa:  Successive Approximation
//
void JpegDecoder::ReadDcFirst (int ssa)
{
  ResetDcDifferences () ;
  int restartcount = 0 ;

  if (ScanIsInterleaved ())
  {
    for (int mcurow = 0 ;
         mcurow < mcu_rows ;
         ++ mcurow)
    {
      for (int mcucol = 0 ;
           mcucol < mcu_cols ;
           ++ mcucol, ++restartcount)
      {
        if (restart_interval != 0
            && restart_interval == restartcount)
        {
          ResetDcDifferences () ;
          ProcessRestartMarker () ;
          restartcount = 0 ;
        }
        for (int cc = 0 ; cc < scan_component_count ; ++ cc)
        {
          for (int cy = 0 ;
               cy < scan_components [cc]->VerticalFrequency () ;
               ++ cy)
          {
            int durow = scan_components [cc]->VerticalFrequency ()
                                 * mcurow + cy ;
            for (int cx = 0 ;
                 cx < scan_components [cc]->HorizontalFrequency () ;
                 ++ cx)
            {
              int ducol = scan_components [cc]->HorizontalFrequency ()
                                   * mcucol + cx ;

              scan_components [cc]->DecodeDcFirst (*this, durow, ducol, ssa) ;
            }
          }
        }
      }
    }
  }
  else
  {
    for (int row = 0 ;
         row < scan_components [0]->NoninterleavedRows () ;
         ++ row)
    {
      for (int col = 0 ;
           col < scan_components [0]->NoninterleavedCols () ;
           ++ col, ++restartcount)
      {
        if (restart_interval != 0 && restart_interval == restartcount)
        {
          ResetDcDifferences () ;
          ProcessRestartMarker () ;
          restartcount = 0 ;
        }
        scan_components [0]->DecodeDcFirst (*this, row, col, ssa) ;
      }
    }
  }
  return ;
}

//
//  Description:
//
//    This function reads the scan data for a refining DC scan.
//
//  Parameters:
//    ssa:  The successive approximation value for this scan.
//
void JpegDecoder::ReadDcRefine (int ssa)
{
  ResetDcDifferences () ;
  int restartcount = 0 ;

  if (ScanIsInterleaved ())
  {
    for (int mcurow = 0 ; mcurow < mcu_rows ; ++ mcurow)
    {
      for (int mcucol = 0 ;
           mcucol < mcu_cols ;
           ++ mcucol, ++restartcount)
      {
        if (restart_interval != 0 && restart_interval == restartcount)
        {
          ResetDcDifferences () ;
          ProcessRestartMarker () ;
          restartcount = 0 ;
        }
        for (int cc = 0 ; cc < scan_component_count ; ++ cc)
        {
          for (int cy = 0 ;
               cy < scan_components [cc]->VerticalFrequency () ;
               ++ cy)
          {
            int durow = scan_components [cc]->VerticalFrequency ()
                                 * mcurow + cy ;
            for (int cx = 0 ;
                 cx < scan_components [cc]->HorizontalFrequency () ;
                 ++ cx)
            {
              int ducol = scan_components [cc]->HorizontalFrequency ()
                                   * mcucol + cx ;

              scan_components [cc]->DecodeDcRefine (*this, durow, ducol, ssa) ;
            }
          }
        }
      }
    }
  }
  else
  {
    for (int row = 0 ;
         row < scan_components [0]->NoninterleavedRows () ;
         ++ row)
    {
      for (int col = 0 ;
           col < scan_components [0]->NoninterleavedCols () ;
           ++ col, ++restartcount)
      {
        if (restart_interval != 0 && restart_interval == restartcount)
        {
          ResetDcDifferences () ;
          ProcessRestartMarker () ;
          restartcount = 0 ;
        }
        scan_components [0]->DecodeDcRefine (*this, row, col, ssa) ;
      }
    }
  }
  return ;
}

//
//  Description:
//
//    This function reads the scan data for the first AC scan for a
//    component. Progressive scans that read AC data cannot be
//    interleaved.
//
//  Parameters:
//    sss:  Spectral Selection Start
//    sse:  Spectral Selection End
//    ssa:  Spectral Selection
//

void JpegDecoder::ReadAcFirst (int sss,
                               int sse,
                               int ssa)
{
  ResetDcDifferences () ;

  int restartcount = 0 ;
  for (int row = 0 ;
       row < scan_components [0]->NoninterleavedRows () ;
       ++ row)
  {
    for (int col = 0 ;
         col < scan_components [0]->NoninterleavedCols () ;
         ++ col, ++restartcount)
    {
      if (restart_interval != 0 && restart_interval == restartcount)
      {
        ResetDcDifferences () ;
        ProcessRestartMarker () ;
        restartcount = 0 ;
      }
      scan_components [0]->DecodeAcFirst (*this,
                                          row, col,
                                          sss, sse,
                                          ssa) ;
    }
  }
  return ;
}

//
//  Description:
//
//    This function reads the scan data for a refining AC scan for a
//    component. Progressive scans that read AC data cannot be
//    interleaved.
//
//  Parameters:
//    sss:  Spectral Selection Start
//    sse:  Spectral Selection End
//    ssa:  Spectral Selection
//

void JpegDecoder::ReadAcRefine (int sss,
                                int sse,
                                int ssa)
{
  ResetDcDifferences () ;

  int restartcount = 0 ;
  for (int row = 0 ;
       row < scan_components [0]->NoninterleavedRows () ;
       ++ row)
  {
    for (int col = 0 ;
         col < scan_components [0]->NoninterleavedCols () ;
         ++ col, ++restartcount)
    {
      if (restart_interval != 0 && restart_interval == restartcount)
      {
        ResetDcDifferences () ;
        ProcessRestartMarker () ;
        restartcount = 0 ;
      }
      scan_components [0]->DecodeAcRefine (*this,
                                           row, col,
                                           sss, sse,
                                           ssa) ;
    }
  }
  return ;
}

//
//  Parameters:
//
//    The function reads the scan data for a sequential scan. All
//    we do here is determine whether or not we have an interleaved
//    or non-interleaved scan then call a function that handles
//    the scan type.

void JpegDecoder::ReadSequentialScanData ()
{
  expected_restart = 0 ;
  if (ScanIsInterleaved ())
  {
    ReadSequentialInterleavedScan () ;
  }
  else
  {
    ReadSequentialNonInterleavedScan () ;
  }
  return ;
}

//
//  Description:
//
//    This function reads the scan data for an interleaved scan.
//

void JpegDecoder::ReadSequentialInterleavedScan ()
{
  ResetDcDifferences () ;

  int restartcount = 0 ;
  for (int mcurow = 0 ; mcurow < mcu_rows ; ++ mcurow)
  {
    for (int mcucol = 0 ; mcucol < mcu_cols ;
         ++ mcucol, ++restartcount)
    {
      if (restart_interval != 0 && restart_interval == restartcount)
      {
        ProcessRestartMarker () ;
        restartcount = 0 ;
      }
      for (int cc = 0 ; cc < scan_component_count ; ++ cc)
      {
        for (int cy = 0 ;
             cy < scan_components [cc]->VerticalFrequency () ;
             ++ cy)
        {
          int durow = scan_components [cc]->VerticalFrequency ()
                               * mcurow + cy ;
          for (int cx = 0 ;
               cx < scan_components [cc]->HorizontalFrequency () ;
               ++ cx)
          {
            int ducol =
                   scan_components [cc]->HorizontalFrequency ()
                   * mcucol + cx ;

            scan_components [cc]->DecodeSequential (
                                      *this,
                                      durow,
                                      ducol) ;
          }
        }
      }
    }
  }
  return ;
}

//
//  Description:
//
//    This function reads the scan data for a non-interleaved scan.
//

void JpegDecoder::ReadSequentialNonInterleavedScan ()
{
  ResetDcDifferences () ;

  int restartcount = 0 ;
  for (int row = 0 ;
       row < scan_components [0]->NoninterleavedRows () ;
       ++ row)
  {
    for (int col = 0 ;
         col < scan_components [0]->NoninterleavedCols () ;
         ++ col, ++ restartcount)
    {
      if (restart_interval != 0
          && restart_interval == restartcount)
      {
        ProcessRestartMarker () ;
        restartcount = 0 ;
      }
      scan_components [0]->DecodeSequential (*this, row, col) ;
    }
  }
  return ;
}

//
//  Description:
//
//    This function resets the DC difference values for all components
//    for the current scan.
//
//    This function gets called before each scan is processed and
//    whenever a restart marker is read.
//

void JpegDecoder::ResetDcDifferences ()
{
  for (int ii = 0 ; ii < scan_component_count ; ++ ii)
    scan_components [ii]->ResetDcDifference () ;
  return ;
}

//
//  Description:
//
//    This function reads a restart marker from the input stream.
//    It gets called byte functions that read scan data whenever
//    a restart marker is expected. An exception is raise if the
//    correct restart marker is not next in the input stream.
//
void JpegDecoder::ProcessRestartMarker ()
{
  lti::ubyte data = ReadByte () ;
  if (data != 0xFF)
    throw JpegBadData ("Missing Restart Marker") ;
  // According to E.1.2 0xFF can be used as a fill character
  // before the marker.
  while (data == 0xFF)
    data = ReadByte () ;
  if (data < RST0 || data > RST7)
    throw JpegBadData ("Missing Restart Marker") ;

  // Restart markers RST0..RST7 should come in sequence.
  if ((0x0F & data) != expected_restart)
    throw JpegBadData ("Incorrect Restart Marker") ;

  // Move the counter to the next restart marker
  ++ expected_restart ;
  expected_restart %= 8 ;

  // Reset the DC coefficent differences to zero.
  ResetDcDifferences () ;
  return ;
}

//
//  Description:
//
//    This function reads an image from a JPEG stream and extracts
//    the image information
//
//  Parameters:
//    istrm: input stream
//    size: image size
//    comment: image comment
//
void JpegDecoder::CheckHeader(std::istream &istrm, lti::point &size, std::string &comment, bool &isGray)
{
  unsigned char data ;

  current_scan = 0 ;
  scan_count = 0 ;
  input_stream = &istrm ;

  eoi_found = false ;
  sof_found = false ;


  // Read the required SOI and APP0 markers at the start of the image.
  ReadStreamHeader () ;

  data = ReadByte () ;
  while (! input_stream->eof () && ! eoi_found)
  {
  if (data == SOB)
  {
      ReadHeaderMarker () ;
    if (eoi_found)
    break ;
  }
  data = ReadByte () ;
  if (input_stream->eof ())
    throw JpegBadData ("Premature end of file") ;
  }

  size = lti::point(frame_width, frame_height);
  comment = img_comment;
  isGray = (component_count == 1);
}


//! void JpegDecoder::ReadImage (std::istream &istrm, BitmapImage &image)
void JpegDecoder::ReadImage (std::istream &istrm, lti::image &image)
{
  unsigned char data ;

  current_scan = 0 ;
  scan_count = 0 ;
  input_stream = &istrm ;
  current_image = &image ;

  restart_interval = 0 ;  // Clear the restart interval ;
  try
  {
    current_image->clear () ;
    eoi_found = false ;
    sof_found = false ;

    // Read the required SOI and APP0 markers at the start of the image.
    ReadStreamHeader () ;

    data = ReadByte () ;
    while (! input_stream->eof () && ! eoi_found)
    {
      if (data == SOB)
      {
        ReadMarker () ;
        if (eoi_found)
          break ;
      }
      data = ReadByte () ;
      if (input_stream->eof ())
        throw JpegBadData ("Premature end of file") ;
    }
  if (! eoi_found)
  {
      throw JpegBadData("End of Image Marker Not Found") ;
    }

  }
  catch (JpegAbort)
  {
    FreeAllocatedResources () ;
    current_image = NULL ;
  }
  catch (...)
  {
    UpdateImage () ;
    FreeAllocatedResources () ;
    current_image = NULL ;
    throw ;
  }
  UpdateImage () ;

  if (! eoi_found)
  {
    throw JpegBadData("End of Image Marker Not Found") ;
  }
  // We do no want an exception so do not call ReadByte ()
  // Sometimes there can be trailing end of record markers.
  input_stream->read ((char *) &data, sizeof (data)) ;
  while ((data == '\r' || data == '\n') && ! input_stream->eof ())
    input_stream->read ((char *) &data, sizeof (data)) ;
  if (! input_stream->eof ())
  {
    throw JpegBadData ("Extra Data After End of Image Marker") ;
  }

  FreeAllocatedResources () ;
  current_image = NULL ;
  return ;
}

//
//  Description:
//
//    This function reads an image from a file.
//
//  Parameters:
//    filename: Input filename
//    image: The output image
void JpegDecoder::readImageFile(const std::string &filename, lti::image &image, bool &isGray) {

  std::ifstream input(filename.c_str(), std::ios::binary);

  ReadImage(input, image);

  isGray = (component_count == 1);

  input.close();
}


//
//  Description:
//
//    This function reads an image from a stream.
//
//  Parameters:
//    filename: Input filename
//    image: The output image
void JpegDecoder::readImageFile(std::ifstream &input,
                                lti::image &image, bool &isGray) {

  // input.flags(std::ios::in|std::ios::binary);

  ReadImage(input, image);
  isGray = (component_count == 1);
}

//
//  Description:
//
//    This function checks an image
//
//  Parameters:
//    filename: Input filename
//    points:
//
void JpegDecoder::checkImageFile(const std::string &filename, lti::point &size, std::string &comment, bool &isGray) {

  std::ifstream input(filename.c_str(), std::ios::binary);

  CheckHeader(input, size, comment, isGray);

  input.close();
}

//
//  Description:
//
//    This function scans a stream and counts the number of scans.  This
//    allows an exact count for a progress function.
//
void JpegDecoder::GetScanCount ()
{
  // Save the stream position so we can go back
  // when we are finished.
  long startpos = input_stream->tellg () ;

  // Count the number of SOS markers.
  scan_count = 0 ;
  while (! input_stream->eof ())
  {
    lti::ubyte data ;
    input_stream->read ((char *) &data, 1) ;
    if (data == SOB)
    {
      while (data == SOB)
      {
        input_stream->read ((char *) &data, 1) ;
      }
      if (data == SOS)
        ++ scan_count ;
      else if (data == EOI)
        break ;
    }
  }
  // Go back to where we were in the stream.
  input_stream->seekg (startpos) ;
  input_stream->clear () ;  // Clear the EOF flag.
  return ;
}

//
//  Description:
//
//    This function writes the image data that has been read so
//    far to the image. This function gets called after reading
//    the entire image stream.  The user can also call this function
//    from a progress function to display progressive images,
//    multi-scan sequential images, or just to display the image
//    as it is being read (slow).
//

void JpegDecoder::UpdateImage ()
{
  if (current_image == NULL)
    throw JpegError ("Not reading an image") ;

  if (progressive_frame)
  {
    for (int ii = 0 ; ii < component_count ; ++ ii)
    {
      components [component_indices [ii]].ProgressiveInverseDct () ;
      components [component_indices [ii]].Upsample () ;
    }
  }
  else
  {
    for (int ii = 0 ; ii < component_count ; ++ ii)
    {
      components [component_indices [ii]].Upsample () ;
    }
  }

  switch (component_count)
  {
  case 3:
    JpegDecoderComponent::RGBConvert (components [component_indices [0]],
                                      components [component_indices [1]],
                                      components [component_indices [2]],
                                      *current_image) ;
    break ;
  case 1:
    JpegDecoderComponent::GrayscaleConvert (
                             components [component_indices [0]],
                             *current_image) ;
    break ;
  }
  return ;
}

//
//  Description:
//
//    This function frees all the memory dynamically allocated
//    during the image decoding process.
//
void JpegDecoder::FreeAllocatedResources ()
{
  for (int ii = 0 ; ii < component_count ; ++ ii)
  {
    components [component_indices [ii]].FreeComponentBuffers () ;
  }
  return ;
}

//
//  Description:
//
//    This function is only used in progressive images. It refines
//    a non-zero AC coefficient. It is called at various times while
//    processing a progressive scan that refines AC coefficients.
//
//  Parameters:
//    value: (in/out) The value to refine
//    ssa: The succesive approximation for the scan.
//    decoder: The JPEG decoder
//

void JpegDecoder::RefineAcCoefficient (lti::int16 &value, int ssa)
{
  // Section G.1.2.3
  if (value > 0)
  {
    if (Receive (1) != 0)
    {
      value += (lti::int16)(1 << ssa) ;
    }
  }
  else if (value < 0)
  {
    if (Receive (1) != 0)
    {
      value += (lti::int16)(-1 << ssa) ;
    }
  }
  return ;
}

//
//  Description:
//
//    This function returns the next raw bit in the input stream.
//
//    Bits are read from high order to low.
//
//  Return Value:
//
//    The next bit (0, 1)
//

// This function returns the next bit in the input stream.
int JpegDecoder::NextBit ()
{
  // Section F.2.2.5 Figure F.18.
  // CNT is called bitposition
  // B is called bitdata
  if (bit_position == 0)
  {
    // We are out of data so read the next byte from the input stream.
    input_stream->read ((char *) &bit_data, sizeof (bit_data)) ;
    if (input_stream->eof ())
      throw JpegBadData ("Premature end of file") ;
    // Reset the bit read position starting with the highest order bit. We
    // read high to low.
    bit_position = CHAR_BIT * sizeof (bit_data) ;

    if (bit_data == 0xFF)
    {
      // 0xFF could start a marker. The sequence 0xFF, 0x00 is used to
      // to represent the value 0xFF. The only other marker that is legal
      // at this point is a DNL marker.
      lti::ubyte b2 ;
      input_stream->read ((char *) &b2, 1) ;
      if (input_stream->eof ())
        throw JpegBadData ("Premature end of file") ;
      if (b2 != 0)
      {
        if (b2 == DNL)
        {
          // DNL markers should not occur within the supported frame types.
          throw JpegBadData ("Unexpected Marker DNL") ;
        }
        else
        {
          throw JpegBadData ("Unexpected Marker") ;
        }
      }
    }
  }

  // Consume one bit of the input.
  -- bit_position ;
  // Shift the value to the low order bit position.
  lti::ubyte result = (lti::ubyte) ((bit_data >> bit_position) & 1) ;
  return result ;
}
}
}

