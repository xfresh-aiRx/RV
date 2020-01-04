Colosseum Builders C++ Image Library

Copyright 1997, 1998, 1999
All Rights Reserved

Permision is granted to use, modify this software provided these conditions
are followed:

1. You understand this software comes with no warranties of any kind 
   whatsoever.
2. You assume full responsibility for the use of this software and transmit
   no liability to the authors.
3. You assume the responsility for obtaining licenses for from any patent
   holders.
4. If you distribute this code in source form, you must include this file
   unmodified and document the changes you have made.
5. If you incorporate this code in a product distributed without source code
   you state included in your documentation that it is based in part on
   software from Colosseum Builders, Inc.

THIS SOFTWARE IS PROVIDED AS IS. WE CAN'T TEST IT ON YOUR SYSTEM SO
USE AT YOUR OWN RISK. YOU HAVE THE SOURCE SO YOU HAVE NO EXCUSES.

Intro:

This archive contains the source code for the Colosseum Builders image
file library. This library is based upon the book

"Compressed Image File Formats: JPEG, PNG, GIF, XBM and BMP"
by John Miano
Addison-Wesley-Longman 1999

This library is an enhancement of the sample library that comes with the
book. While their structures are similar, this library sacrifices clarity
in some places to increase performance.

Changes:

The BitmapImage class now always uses 32-bits to represent a pixel. This
allows us to have an Alpha channel and, in the future, four-color component
colorspaces (eg CMYK). Color quantization was removed. Support for
alpha-merging images was added.

The BmpDecoder class can now read 16 and 32-bit images.

The BmpEncoder no longer creates image with fewer than 16-bits per pixel. The
setBitsPerPixel member function can set the number of bits used to 16, 24, or
32.  24-bit images use the widely used BITMAPINFOHEADER. The other sizes
use the newer BITMAPV4HEADER that are supported on Win95 and later.

Note that in typical Microsoft fashion, support for the newer bitmap format
is inconsistent. While the Win32 API supports the new bitmap header format
most Microsoft image applications appear not to. So you can view images
with the new format in your own applications that use the API, you may
have a hard time finding other applications that can view them.

The PngEncoder has the setUseAlphaChannel function that enables or disables
the creation of an Alpha channel.

The PngDecoder class now stores the Alpha channel in the BitmapImage object.

Building:

The directories BC and MSVC contain procedures for building the library using 
Borland C++Builder and MSVC++ respectively. 

For other compilers you will have to create your own procedure and you will 
probably need to make changes to the source code as well.

If you are using Borland C++ V5 (Not Borland C++Builder), the problem you will 
encounter is that standard I/O is not included in the namespace std. You will 
have to edit the code to remove "std::" qualifiers from these declarations.

The changes required for GNU vary among systems an implementations. The main
problems deal with include files and standard classes (notably exceptions).

If you build an application using the libimage.lib object library you need
to ensure that you compile the library using the same compiler options for
both the application and library. Borland C++Bulder V5 introduced many
problems because of differences between the command line and IDE compiler
options.

We have attempted to move the library closer to the ANSI C++ standard.
Unfortunately Microsoft apparently sees no need to conform to the standard.
Getting something to work with any other compiler and Visual C++ is difficult.
The MICROSOFT subdirectory contains replacement for files that VC++ chokes on
but shouldn't.

Send Questions or comments to

info@colosseumbuilders.com

Send Bug Reports to:

imagebugs@colosseumbuilders.com


Changes:

May 31, 1999 - BitmapImage.cpp Fixed incorrect test in findColor ().
July 14, 1999 - BmpDecoder.cpp Fixed incorrect color count
Nov 31, 1999 - Major Revision
Jan 3, 2000 - Added terminator to JFIF header. ANSI C++ enhancements.
