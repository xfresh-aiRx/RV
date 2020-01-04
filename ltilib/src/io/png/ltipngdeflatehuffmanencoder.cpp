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
 * file .......: ltipngdeflatehuffmanencoder.cpp
 * authors ....: John M. Miano, miano@colosseumbuilders.com
 *               Stefan Syberichs, syberichs@techinfo.rwth-aachen.de
 * organization: LTI, RWTH Aachen
 * creation ...: 04.11.99
 * description.: HuffmanEncoder implementation
 * revisions ..: $Id: ltipngdeflatehuffmanencoder.cpp,v 1.5 2003/02/17 07:16:14 alvarado Exp $
 */


#include <string>
#include "ltipngdeflatehuffmanencoder.h"
#include "ltiAssert.h"
#include "ltiTypes.h"
#include "ltiVector.h"




namespace lti {
  namespace png {

const int MAXCODELENGTH = 15 ;

//
//  Description:
//
//    Default Class Constructor
//
DeflateHuffmanEncoder::DeflateHuffmanEncoder (unsigned int valuecount)
: frequencies (valuecount),
  ehufsi (valuecount),
  ehufco (valuecount),
  value_count (valuecount) {
  reset () ;
  return ;
}

//
//  Description:
//
//  After Huffman codes have been generated the object is in a state
//  where it cannot be used to create a new set of code. This function
//  places the object in a state where it can be reused to generate a
//  new set of Huffman Codes.
//
void DeflateHuffmanEncoder::reset () {

  for (unsigned int ii = 0 ; ii < value_count ; ++ ii) {
    ehufco [ii] = 0 ;
    ehufsi [ii] = 0 ;
    frequencies [ii] = 0 ;
  }

  return ;
}

//
//  Description:
//
//  Function to increment the frequency for a value.
//
//  Parameters:
//  value:  The value to increase the usage frequency of.
//
void DeflateHuffmanEncoder::incrementFrequency (unsigned int value) {
  ++ frequencies [value] ;
  return ;
}

//
//  Description:
//
//  This function generates the Huffman Codes using the frequency data.
//
//  The outputs from this function are the following member variables:
//
//  ehufsi [n] : The length of the Huffman Code for value "n"
//  ehufco [n] : The Huffman Code for value "n"
//
//  The first two arrays are used to encode Huffman values. The last two
//  are for writing the table to the output file.
//
//  The code generation process is:
//
//  1. Arrange the Huffman Values into a binary tree so that the most
//     frequently used codes are closest to the root of the tree. At the end
//     of this process the temporary array codesize [n] contains the length
//     of the pure Huffman code for the value "n"
//
//  2. Determine the number of Huffman Codes of for each code length. This
//     step places the number of codes of length "n+1" in huffbits[].
//
//  3. The Default places limites on the size of Huffman Codes. If
//     codes longer that the specified limits were generated in the
//     previous steps then we need to reduce the maximum depth of the
//     tree created in step 1. The input and output to this step is the
//     array huffbits[] created in the previous step.
//
//  4. Sort the Huffman values in code length order. codesize [n] is the
//     input to this step and huffvalues [n] is the output. At this point
//     all the information needed to write the Huffman Table to the output
//     stream has been found.
//
//  5. Determine the code size for each value. At the end of this step
//     the temporary array huffsizes [n] is the Huffman code length for
//     huffvalues [n].
//
//  6. Determine the Huffman code for each value. The temporary array
//     huffcodes [n] is the Huffman Code of length huffsizes [n] for
//     the value huffvalue [n].
//
//  7. Using huffsizes [] and huffcodes created in steps 6 and 7 create
//     the arrays ehufco [n] and ehufsi [n] giving the Huffman Code and
//     Code Size for n.
//
//  Parameters:
//   maxcodelength:  The maximum code length to generate
//
void DeflateHuffmanEncoder::buildTable (unsigned int maxcodelength) {

  bool codestoolong = false ;

  // The tmp array is used for validating the integrity of the Huffman code
  // table. We need a temporary array since frequencies [] gets trashed
  // during the code generation process.
  vector<unsigned int> tmpfrequencies (frequencies) ;

  // Build the Huffman Code Length Lists
  vector<int> others (int(value_count), int(-1)) ;
  vector<unsigned int> codesize (int(value_count), (unsigned int)0) ;

  while (true) {
    // Find the two smallest non-zero values
    int v1 = -1 ;
    int v2 = -1 ;
    for (unsigned int ii = 0 ; ii < value_count ; ++ ii)
    {
      if (frequencies [ii] != 0)
      {
        if (v1 < 0 || frequencies [ii] <= frequencies [v1])
        {
          v2 = v1 ;
          v1 = ii ;
        }
        else if (v2 < 0 || frequencies [ii] <= frequencies [v2])
        {
          v2 = ii ;
        }
      }
    }

    if (v2 < 0)
    {
      if (v1 < 0) {
        return ; // No codes defined
      }

      if (codesize [v1] == 0) {
        codesize [v1] = 1 ;  // Only one code defined
      }
      break ;
    } // if

    // Join the two tree nodes.
    frequencies [v1] = frequencies [v1] + frequencies [v2] ;
    frequencies [v2] = 0 ;

    for (++ codesize [v1] ; others [v1] >= 0 ; ++ codesize [v1]) {
      v1 = others [v1] ;
    }

    others [v1] = v2 ;

    for (++ codesize [v2] ; others [v2] >= 0 ; ++ codesize [v2]) {
      v2 = others [v2] ;
    }
  } // while

  // Determine the number of codes of length [n]
  vector<unsigned int> huffbits (maxcodelength * 2, (unsigned int)0) ; // 2x needed for encoding only.
  {
    for (unsigned int ii = 0 ; ii < value_count ; ++ ii)
    {
      if (codesize [ii] != 0)
      {
        ++ huffbits [codesize [ii] - 1] ;
      }
    }
  }

  {
    // Ensure that no code is longer than maxlength.
    for (unsigned int ii = 2 * maxcodelength -  1;ii >= maxcodelength ; -- ii) {
      while (huffbits [ii] != 0) {
        codestoolong = true ; // Remember that we had to reorder the tree

        unsigned int jj = ii - 1 ;
        do
        {
          -- jj ;
        }
        while (huffbits [jj] == 0) ;

        huffbits [ii] -= 2 ;
        ++ huffbits [ii - 1] ;
        huffbits [jj + 1] += 2 ;
        -- huffbits [jj] ;
      } // while
    } // for
  } // block end

  {
    // Make sure that the total number of symbols is correct.
    unsigned int count = 0 ;
    for (unsigned int ii = 0 ; ii < maxcodelength ; ++ ii)
      count += huffbits [ii] ;
    // We can't have more codes than values.
    assert(count <= value_count);
  }

  // Sort the values in order of code length.
  // What might not be clear is that codesize [n] is the length
  // of the Huffman code for n before it was shortened to maxcodelength.
  // That the values in codesize may be too large does not matter. The
  // ordering of the values by code size remains correct. As soon as this
  // step is complete, the codesize[] array is no longer used anyway.
  vector<unsigned int> huffvalues (value_count,(unsigned int)0);

  {
    for (unsigned int ii = 1, kk = 0 ; ii < 2 * maxcodelength ; ++ ii) {
      for (unsigned int jj = 0 ; jj < value_count ; ++ jj) {
        if (codesize [jj]  == ii) {
          huffvalues [kk] = jj ;
          ++ kk ;
        }
      }
    }
  }


  {
    for (unsigned int ii = 0, count = 0 ; ii < maxcodelength ; ++ ii) {
      count += huffbits [ii] ;
      assert(count <= value_count) ;
    }
  }

  // Convert the array "huffbits" containing the count of codes for each
  // length 1..maxcodelength into an array containing the length for each code.
  vector<unsigned int> huffsizes (value_count,(unsigned int)0) ;

  {
    for (unsigned int ii = 0, kk = 0 ; ii < maxcodelength && kk < value_count ; ++ ii) {
       for (unsigned int jj = 0 ; jj < huffbits [ii] ; ++ jj) {
          huffsizes [kk] = ii + 1 ;
          ++ kk ;
       }
    }
  }

  // Calculate the Huffman code for each Huffman value.
  unsigned int code = 0 ;
  vector<unsigned int> huffcodes (value_count,(unsigned int)0) ;

  {
    for (unsigned int kk = 0, si = huffsizes [0];kk < value_count && huffsizes [kk] != 0;++ si,code <<= 1) {
       for ( ; kk < value_count && huffsizes [kk] == si ; ++ code, ++ kk) {
          huffcodes [kk] = code ;
       }
    }
  }


  {
    for (unsigned int kk = 0 ; kk < value_count ; ++ kk) {
      if (huffsizes [kk] != 0)
      {
        unsigned int ii = huffvalues [kk] ;
        ehufco [ii] = huffcodes [kk] ;
        ehufsi [ii] = huffsizes [kk] ;
        assert(ehufsi [ii] <= maxcodelength); // Invalid Code Length
      }
    }
  }

  // If the pure Huffman code generation created codes longer than the
  // maximum the it is possible that the order got screwed up. Such a
  // situation could occur if the maximum code length is 15 and during the
  // pure process we the value 150 got assigned a length of 13, 100 a length
  // of 15 and 200 a length of 17. During the process of reducing the code
  // length for 200 it is possible that 150 would have its code length
  // increased to 14 and 100 would have its code length reduced to 14.
  // Unfortunately the Huffman codes would be assigned using the old
  // order so that 150 would get assigned a smaller Huffman code than
  // 100.  Here we fix that and ensure that if ehufsi [ii] == ehufsi [jj]
  //  and ii < jj then ehufco [ii] < ehufco [jj].
  if (codestoolong) {
    for (unsigned int ii = 0 ; ii < value_count - 1 ; ++ ii) {
      for (unsigned int jj = ii + 1 ; jj < value_count ; ++ jj) {
        if (ehufsi [ii] == ehufsi [jj] && ehufco [ii] > ehufco [jj]) {
          // The codes got out of order so switch them.
          unsigned int tmp = ehufco [jj] ;
          ehufco [jj] = ehufco [ii] ;
           ehufco [ii] = tmp ;
        }
      }
    }
  }

  // Validations
  // This remaining code is not necessary other than to ensure the
  // integrity of the Huffman table that is generated.

  // Make sure each value is used exactly once.
  {
    for (unsigned int ii = 0 ; ii < value_count ; ++ ii) {
      int count = 0 ;
      if (tmpfrequencies [ii] != 0) {
        assert(ehufsi [ii] != 0); // Missing Size

        for (unsigned int jj = 0 ; jj < value_count ; ++ jj) {
          if (ii == huffvalues [jj] && huffsizes [jj] != 0)
            ++ count ;
          assert(count <= 1); // Duplicate VAlue
        }
        assert(count != 0); // Missing Value
      }
    }
  }


  {
    // Ensure that each huffman code is used once annd that the values
    // are in range.
    for (unsigned int ii = 0 ; ii < value_count ; ++ ii) {
      if (ehufsi [ii] != 0) {
        assert(ehufsi [ii] <= maxcodelength); // Invalid Length

        for (unsigned int jj = ii + 1 ; jj < value_count ; ++ jj) {
          assert(ehufco [ii] != ehufco [jj] || ehufsi [jj] == 0); // Duplicate Code
        }
      }
    }
  }


  // If the decoder reads from the least significant bit to the most
  // significant bit, the codes need to be reversed.

  for (unsigned int ii = 0 ; ii < value_count ; ++ ii) {
    unsigned int value = 0 ;
    unsigned int code = ehufco [ii] ;
    unsigned int size = ehufsi [ii] ;

    for (unsigned int jj = 0 ; jj < ehufsi [ii] ; ++ jj) {
      unsigned int bit = (code & (1 << jj)) >> jj ;
      value |= bit << (size - jj - 1) ;
    }
    ehufco [ii] = value ;
  }

  return ;
}

//
//  Description:
//
//  This function returns the Huffman Code and Code Size for a given value.
//
//  Parameters:
//  value:  The value to encode
//  code:   The Huffman Code
//  size:   The Huffman Code Length
//
void DeflateHuffmanEncoder::encode (unsigned int value,unsigned int &code,unsigned int &size) const {
  code = ehufco [value] ;
  size = ehufsi [value] ;
  return ;
}


  } // ns png
} // End Namespace lti

