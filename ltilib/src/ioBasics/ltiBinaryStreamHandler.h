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
 * file .......: ltiBinaryStreamHandler.h
 * authors ....: Pablo Alvarado, Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 18.05.2001
 * revisions ..: $Id: ltiBinaryStreamHandler.h,v 1.7 2004/06/03 09:47:10 alvarado Exp $
 */

#ifndef _LTI_BINARY_STREAM_HANDLER_H_
#define _LTI_BINARY_STREAM_HANDLER_H_


#include <string>
#include <iostream>
#include <stack>
#include "ltiObject.h"
#include "ltiIoHandler.h"

namespace lti {
  /**
   * Binary stream handler.
   *
   * The binaryStreamHandler class offer an interface for the functor
   * parameters and other classes to read() and write() them in a
   * binary format.
   *
   * The advantage of the binary format is that the produced files are
   * smaller, which at the same time implies shorter times to read or write
   * the data.
   *
   * The disadvange is that the produced files cannot be easily edited.  If
   * you need this, you should still use the lti::lispStreamHandler.
   *
   * \warning Do not forget to open your streams in binary mode.  If you
   *          create them as ASCII streams, the data can be corrupted.
   *
   * Example:
   * \code
   * // the binary stream formatting object
   * lti::binaryStreamHandler bsh;
   *
   * // Write example:
   *
   * // open a stream in binary mode
   * std::ofstream out("testfile.bin",std::ios_base::binary);
   * 
   * // tell the binary stream handler to use the given stream
   * bsh.use(out);
   *
   * lti::write(bsh,"anInteger",5);
   * lti::write(bsh,"aString","hello world");
   *
   * out.close();
   * 
   * // Read example
   *
   * // Open a stream in binary mode
   * std::ifstream in("testfile.bin",std::ios_base::binary);
   *
   * bsh.use(in);
   * int i;
   * lti::read(bsh,"anInteger",i);
   *
   * std::string str;
   * lti::read(bsh,"aString",str);
   *
   * in.close();
   * \endcode
   *
   * @bug: An intrinsic bug for the binary stream handler was found
   * (2004-02-03).  There are some cases in which the tryXXX methods will
   * success (return true), even if the searched token is not present.  Some
   * rules need to be defined to avoid problems (indirectly the currently used
   * formats avoid these cases), but a more detailed revision of the whole
   * handler has to be done.
   *
   * @ingroup gStorable
   */
  class binaryStreamHandler : public ioHandler {
  public:
    /**
     * default constructor
     */
    binaryStreamHandler();

    /**
     * default constructor.
     * Please ensure that the given stream was opened with the
     * std::ios_base::binary flag!
     */
    binaryStreamHandler(std::ostream& aStream);

    /**
     * default constructor.
     * Please ensure that the given stream was opened with the
     * std::ios_base::binary flag!
     */
    binaryStreamHandler(std::istream& aStream);

    /**
     * default constructor.
     * This constructor uses the given string
     * as input.
     * Note that the given string is NOT a filename, but a string containing
     * the data "stream".
     * This is useful especially for the JNI.
     */
    binaryStreamHandler(const char *aStream);

    /**
     * copy constructor
     */
    binaryStreamHandler(const binaryStreamHandler& other);

    /**
     * destructor
     */
    virtual ~binaryStreamHandler();

    /**
     * indicate the output stream to be used.
     * Please ensure that the given stream was opened with the
     * std::ios_base::binary flag!
     */
    void use(std::ostream& aStream);

    /**
     * indicate the input stream to be used.
     * Please ensure that the given stream was opened with the
     * std::ios_base::binary flag!
     */
    void use(std::istream& aStream);

    /**
     * returns the name of this type ("binaryStreamHandler")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    binaryStreamHandler& copy(const binaryStreamHandler& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual ioHandler* clone() const;

    /**
     * the begin token or tokens: here a "(".
     *
     */
    virtual bool writeBegin();

    /**
     * the end token or tokens: here a ")".
     */
    virtual bool writeEnd();

    /**
     * the begin token or tokens: here a "("
     */
    virtual bool readBegin();

    /**
     * the end token or tokens: here a ")"
     */
    virtual bool readEnd();

    /** @name write members
     */
    //@{
    /**
     * write a std::string.
     * This method will write just the string if it does not contain spaces.
     * Otherwise the string will be enclosed by quotes.
     */
    virtual bool write(const std::string& data);

    /**
     * write a character string.
     * This method will write just the string if it does not contain spaces.
     * Otherwise the string will be enclosed by quotes.
     */
    virtual bool write(const char* data);

    /**
     * write a double value
     */
    virtual bool write(const double& data);

    /**
     * write a float value
     */
    virtual bool write(const float& data);

    /**
     * write an integer value
     */
    virtual bool write(const int& data);

    /**
     * write an unsigned integer value
     */
    virtual bool write(const unsigned int& data);

    /**
     * write a char
     */
    virtual bool write(const char& data);

    /**
     * write an 8-bit signed value
     */
    virtual bool write(const byte& data);

    /**
     * write a unsigned 8-bit value
     */
    virtual bool write(const ubyte& data);

    /**
     * write a boolean
     */
    virtual bool write(const bool& data);

    /**
     * write a long
     */
    virtual bool write(const long& data);

    /**
     * write an unsigned long
     */
    virtual bool write(const unsigned long& data);

    /**
     * write a short
     */
    virtual bool write(const short& data);

    /**
     * write an unsigned short
     */
    virtual bool write(const unsigned short& data);
    //@}

    /** @name read members
     */
    //@{
    /**
     * read a std::string
     */
    virtual bool read(std::string& data);

    /**
     * read a double value
     */
    virtual bool read(double& data);

    /**
     * read a float value
     */
    virtual bool read(float& data);

    /**
     * read an integer value
     */
    virtual bool read(int& data);

    /**
     * read an unsigned int value
     */
    virtual bool read(unsigned int& data);

    /**
     * read a char value
     */
    virtual bool read(char& data);

    /**
     * read a char value
     */
    virtual bool read(byte& data);

    /**
     * read an ubyte value
     */
    virtual bool read(ubyte& data);

    /**
     * read a boolan
     */
    virtual bool read(bool& data);

    /**
     * read a long
     */
    virtual bool read(long& data);

    /**
     * read an unsigned long
     */
    virtual bool read(unsigned long& data);

    /**
     * read a short
     */
    virtual bool read(short& data);

    /**
     * read an unsigned short
     */
    virtual bool read(unsigned short& data);
    //@}

    /**
     * write a std::string
     */
    virtual bool writeSymbol(const std::string& data);

    /**
     * read a symbol in the given std::string
     */
    virtual bool readSymbol(std::string& data);

    /**
     * try to read the given symbol from the handler.
     * If present, returns true and the token is removed from the
     * handler, if not present returns false and leaves the stream as
     * is...
     * @param data the symbol to be readed
     */
    virtual bool trySymbol(const std::string& data);

    /**
     * write comment writes the input data without any preprocessing,
     * just ensuring that the comment format is given
     */
    virtual bool writeComment(const std::string& data);

    /**
     * write comment writes the input data without any preprocessing,
     * just ensuring that the comment format is given
     */
    virtual bool writeComment(const char* data);

    /**
     * try to read the begin token from the handler.
     * If present, returns true and the token is removed from the
     * handler, if not present returns false and leaves the handle as
     * it was before calling this member...
     */
    virtual bool tryBegin();

    /**
     * try to read the end token from the handler.
     * If present, returns true and the token is removed from the
     * handler, if not present returns false and leaves the handle as
     * it was before calling this member...
     * This is usually used when reading lists of data, where the number of
     * elements is unknown.
     */
    virtual bool tryEnd();

    /**
     * write spaces (default value 1).
     * A space-token is a char with value 32.
     */
    virtual bool writeSpaces(const int& s=1);

    /**
     * write end of line
     */
    virtual bool writeEOL();

    /**
     * write key/value separator.
     * In this case the key/value separator is a space
     */
    virtual bool writeKeyValueSeparator();

    /**
     * write key/value separator.
     * In this case the data separator is a space
     */
    virtual bool writeDataSeparator();

    /**
     * write key/value separator
     * A space is expected
     */
    virtual bool readKeyValueSeparator();

    /**
     * write key/value separator
     * A space is expected
     */
    virtual bool readDataSeparator();

    /**
     * if the input stream is at the end of file return true, otherwise false
     * if the stream hasn't been set yet, this function also returns true.
     */
    virtual bool eof();

    /**
     * restore all the information in the handler taken in the actual
     * level.  If "complete" is true, the begin-token is also restored
     */
    virtual bool restoreLevel();

  protected:

    /**
     * pointer to the input stream
     */
    std::istream* inStream;

    /**
     * the input stream will be cached into this string
     */
    std::string inString;

    /**
     * actual reading position in the input string
     */
    std::string::size_type inStringPos;

    /**
     * pointer to the output stream
     */
    std::ostream* outStream;

    /**
     * flag to control spaces supression (for example, there is no need for
     * spaces between parenthesis)
     */
    bool supressSpaces;

    /**
     * flag to indicate if an EOL is needed
     */
    bool tryEOL;

    /**
     * Tokens
     *
     * This system requires a complete revision!
     */
    enum eTokenId {
      BeginToken,
      EndToken,
      SymbolToken,
      StringToken,
      ErrorToken
    };

    /**
     * read next token from the input string or from the input stream
     * @see getNextTokenFromString.
     */
    eTokenId getNextToken(const bool justTry = false);

    void putBackupToken();

    void checkBuffer(int newSize);

    eTokenId lastToken;

    inline eTokenId char2token(char x);

    std::stack<eTokenId> backupTokens;
    std::stack<const std::string*> backupStrings;

    std::string lastString;

    char *buffer;
    int bufSize;

  };

}

#endif

