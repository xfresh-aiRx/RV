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
 * file .......: ltiSerial.h
 * authors ....: Guy Wafo
 * organization: LTI, RWTH Aachen
 * creation ...: 11.4.2001
 * revisions ..: $Id: ltiSerial.h,v 1.20 2005/11/24 14:24:03 doerfler Exp $
 */

#ifndef _LTI_SERIAL_H_
#define _LTI_SERIAL_H_

#include "ltiObject.h"
#include "ltiIoObject.h"
#include "ltiIoHandler.h"
#include "ltiTypes.h"
#include "ltiVector.h"

#ifdef _LTI_WIN32
// headers required in visual (or perhaps windows?)
#include <windows.h>
#include <winbase.h>

#else

// for posix systems
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <cstdio>

#endif

#ifdef _LTI_MACOSX
// these don't get defined on MacOSX 
// (sys/termios.h excludes them if _POSIX_C_SOURCE is defined)
#  define B57600 57600
#  define B115200 115200
#endif


#include <cstdlib>
#include <string>

namespace lti {
  /**
   * This serial class provides an unified interface to access serial
   * ports in posix and windows operating systems.  At this time only
   * Linux and Windows are supported.
   *
   * You just need to set the parameters if you do not want the default ones,
   * and send or receive data with the send() and receive() methods.
   *
   * If you use a Linux system, do not forget to make the users who
   * needs access to the serial ports to be members of the group
   * <code>uucp</code> or the corresponding group of the
   * <code>/dev/ttyS*</code> devices.  */
  class serial : public object {
  public:
    /**
     * the parameters for the class serial
     */
    class parameters : public ioObject {
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
      virtual ~parameters();

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
      virtual parameters* clone() const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * write the parameters in the given ioHandler
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
       * Type to specificy the port to be used
       */
      enum ePortType {
        Com1=0, /**< first port usually called COM1 in Windows or
                     /dev/ttyS0 in Linux */
        Com2,   /**< first port usually called COM2 in Windows or
                     /dev/ttyS1 in Linux */
        Com3,   /**< first port usually called COM3 in Windows or
                     /dev/ttyS2 in Linux */
        Com4,   /**< first port usually called COM4 in Windows or
                     /dev/ttyS3 in Linux */
        Com5,   /**< first port usually called COM5 in Windows or
                     /dev/ttyS4 in Linux */
        Com6,   /**< first port usually called COM6 in Windows or
                     /dev/ttyS5 in Linux */
        Com7,   /**< first port usually called COM7 in Windows or
                     /dev/ttyS6 in Linux */
        Com8    /**< first port usually called COM8 in Windows or
                     /dev/ttyS7 in Linux */
      };

      /**
       * Type to specify the baud rate.
       *
       * Baud rate is a measure of the number of times per second a
       * signal in a communications channel varies, or makes a
       * transition between states (states being frequencies, voltage
       * levels, or phase angles). One baud is one such change. Thus,
       * a 1200-baud modem's signal changes state 1200 times each
       * second.  The number of bits transmitted in a second (baudrate
       * in bits-per-second (bps) ) depends on the modulation
       * technique being used.  A 2400 baud modem can transmit 9400
       * bps if there are 4 bits/baud.
       */
      enum eBaudRateType {
        Baud0,      /**<     0 baud */
        Baud300,    /**<   300 baud */
        Baud600,    /**<   600 baud */
        Baud1200,   /**<  1200 baud */
        Baud1800,   /**<  1800 baud */
        Baud2400,   /**<  2400 baud */
        Baud4800,   /**<  4800 baud */
        Baud9600,   /**<  9600 baud */
        Baud19200,  /**< 19200 baud */
        Baud38400,  /**< 38400 baud */
        Baud57600,  /**< 57600 baud */
        Baud76800,  /**< 76800 baud */
        Baud115200  /**<115200 baud */
      };

      /**
       * Type to specify the number of bits of a character.
       * The first element Cs4 means for example 4 bits/character.
       */
      enum eCharBitSizeType {
        Cs4=4, /**< 4 bits per character */
        Cs5,   /**< 5 bits per character */
        Cs6,   /**< 6 bits per character */
        Cs7,   /**< 7 bits per character */
        Cs8    /**< 8 bits per character */
      };

      /**
       * definiton of the stop bit
       * This is also called the remaining bits.  Stop bits serve to
       * synchronise the receiving computer to the incoming characters.
       */
      enum eStopBitType {
        One,     /**< one stop bit             */
        OneFive, /**< one and a half stop bits */
        Two      /**< two stop bits            */
      };

      /**
       * definition of the parity bit
       * the optional parity bit is a simple checksum of the data bits
       * indicating whether or not the data contains an even or odd
       * number of "1" bits.
       */
      enum eParityType {
        No,    /**< no parity   */
        Even,  /**< even parity */
        Odd,   /**< odd parity  */
        Space, /**< space       */
        Mark   /**< mark        */
      };

      /**
       * Baud rate.
       * Default value: Baud9600
       */
      eBaudRateType baudRate;

      /**
       * Which serial port.
       * Default value: Com1
       */
      ePortType port;

      /**
       * Character size.
       * Default value: Cs8
       */
      eCharBitSizeType characterSize;

      /**
       * Parity.
       * Default value: No
       */
      eParityType parity;

      /**
       * Stop bits
       * Default: One
       */
      eStopBitType stopBits;

      /**
       * Receive timeout in milliseconds.
       *
       * If you specify zero, then no timeout will exist.  The receive method
       * will check if there is something in the buffer.  If so, then it
       * will normally return (reading the first character in the serial FIFO).
       * If there is nothing in the buffer, the receive() method will return
       * inmediatly a "false" value.
       *
       * You should consider that for Linux systems the expected timeout
       * precision is in deciseconds.  The given value will be rounded
       * to the best decisecond precision.  For example 1 ms will be rounded
       * to 0 ms, 80ms will be rounded to 0.1s.
       *
       * Windows OS accepts the ms value.
       *
       * Default value: 5000 (i.e. 5 seconds)
       */
      int receiveTimeout;
    };


    /**
     * default constructor
     */
    serial();

    /**
     * construct a serial instance with the given parameters
     */
    serial(const parameters& serialPar);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    serial(const serial& other);

    /**
     * destructor
     */
    virtual ~serial();

    /**
     * returns the name of this type ("serial")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    serial& copy(const serial& other);

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * set the parameters for the serial object
     */
    bool setParameters(const parameters& theParameters);

    /**
     * sends a character c through the port specified in the
     * parameters.
     *
     * @return true if the character could be send successfully, or
     *              false otherwise.
     */
    bool send(const char c);

    /**
     * sends an unsigned character c through the port specified in the
     * parameters.
     *
     * @return true if the character could be send successfully, or
     *              false otherwise.
     */
    bool send(const ubyte c);

    /**
     * sends a string through the port specified in the parameters.
     *
     * @return true if all the string characters are send
     * successfully, or false otherwise.
     */
    bool send(const std::string& theString);

    /**
     * sends all ubytes contained in the vector through the port
     * specified in the parameters.  The vector is more appropriate if
     * the data to be send has a more "binary" nature than an
     * ASCII string.
     *
     * @return true if all the string characters are send
     * successfully, or false otherwise.  
     */
    bool send(const vector<ubyte>& theVector);

    /**
     * wait for a character to be received in the port specified in
     * the parameters.
     *
     * @return true if the character has been received, or false if a
     * problem occured.  In this case you can read the status string
     * to check the source of the problem.  Note that false will also
     * be returned if a timeout occurs.
     */
    bool receive(char& c);

    /**
     * wait for an unsigned character to be received in the port specified in
     * the parameters.
     *
     * @return true if the character has been received, or false if a
     * problem occured.  In this case you can read the status string
     * to check the source of the problem.  Note that false will also
     * be returned if a timeout occurs.
     */
    bool receive(ubyte& c);

    /**
     * read all characters until the next new line or zero character, and
     * store them in the given string.
     */
    bool receive(std::string& theString);

    /**
     * read exactly theVector.size() characters from the port specified in
     * the parameters.
     *
     * @return true if the character has been received, or false if a
     * problem occured.  In this case you can read the status string
     * to check the source of the problem.  Note that false will also
     * be returned if a timeout occurs.
     */
    bool receive(vector<ubyte>& theVector);

    /**
     * return the last message set with setStatusString().  This will
     * never return 0.  If no status-string has been set yet an empty string
     * (pointer to a string with only the char(0)) will be returned.
     */
    const char* getStatusString() const;

    /**
     * set a status string.
     *
     * @param msg the const string to be reported next time by
     * getStatusString().
     *
     * This message will be usually set within the send/receive
     * methods to indicate an error cause.
     *
     * Note that the change of the status string is not considered as
     * a change in the class status (this method is const).
     */
    void setStatusString(const char* msg) const;

  protected:

    /**
     * open the port using the parameters
     */
    bool openPort();

    /**
     * the parameters in use
     */
    parameters param;

  private:

#ifdef _LTI_WIN32
    /**
     * handler used in windows
     */
    HANDLE hHandle;

    /**
     * sets and query the time-out parameters for a communications device
     */
    COMMTIMEOUTS timeOut;
#else

    /**
     * File descriptor for the port in POSIX.
     * get the value -1 in case of error,while opening the port
     */
    int hHandle;

#endif

    /**
     * contains true if the port is opened, else false
     */
    bool isPortOpened;

    /**
     * close the port
     */
    void closePort();

    /**
     * the empty string returned if the statusString is empty
     */
    static const char *const emptyString;

    /**
     * the status string written with setStatusString
     */
    mutable char* statusString;
  };
}

#endif

