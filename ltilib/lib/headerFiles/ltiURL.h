/*
 * Copyright (C) 1998, 1999, 2000, 2001, 2002
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
 * file .......: ltiURL.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 08.07.02
 * revisions ..: $Id: ltiURL.h,v 1.15 2005/06/16 12:37:49 doerfler Exp $
 */

#ifndef _LTI_URL_H_
#define _LTI_URL_H_

#include "ltiConfig.h"

#include "ltiIoObject.h"
#include "ltiVector.h"
#include "ltiImage.h"
#include "ltiTypes.h"
#include <iostream>
#include <fstream>
#include <string>

#ifdef _LTI_WIN32
#include <winsock.h>
#endif


namespace lti {

  /**
   * Class for retrieving documents identified by an URL. You simply
   * give it an URL and call the appropriate get method (PUT's are not
   * yet supported. For instance:
   *
   * \code
   * lti::url doc("http://tintas/axiom/axicon.png");
   * if (doc.isOk()) {
   *   doc.getPNGImage(tmp);

   *   viewer view;
   *   view.show(tmp);
   *   getchar();
   * } else {
   *   std::cerr << doc.getStatusString() << std::endl;
   * }
   * \endcode
   *
   * Unfortunately, I have no idea how to implement http client code
   * for Windows. Therefore, this class currently only exists for
   * libs compiled for Linux.
   *
   * Ws2_32.lib is linked (you may need to specify its library in
   * Project..Settings..Link..Object/Library Modules.)
   */
  class url : public object {


  public:

    /**
     * parameters for the url-object
     */
    class parameters {
    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       */
      parameters(const parameters& other);

      /**
       * destructor
       */
      virtual ~parameters();

      /**
       * copy member
       */
      parameters& copy(const parameters& other);

      /**
       * copy data of "other" parameters
       */
      parameters& operator=(const parameters& other);

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual parameters* clone() const;

      /**
       * returns name of this type
       */
      const char* getTypeName() const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,
                         const bool complete=true) const;

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
       * there...  This method is public due to another bug!, so please
       * NEVER EVER call this method directly
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is public due to another bug!, so please
       * NEVER EVER call this method directly
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const;
#     endif

      // ------------------------
      // the parameters
      // ------------------------

      /**
       * directory name, which holds the temporary image files.
       * Default directory: C:\\TEMP
       * This parameter is only for Windows needed not Linux.0
       */
      std::string tmpDirectory;


    };

    /**
     * The type of service that is associated with the URL.
     */
    enum serviceType {
      UNKNOWN = -1,
      HTTP = 0,
      FTP = 1,
      FILE = 2,
      MAXSERVICE = 19
    };

    /**
     * This class is used to split a URL into its separate fields.
     * For example, if you call its constructor with
     * "http://ltilib.sourceforge.net:80/index.html", it would
     * split it into the service type (http), the hostname
     * (ltilib.sourceforge.net), the port (80), and the path
     * (/index.html).
     */
    class urlFields: public object {
    public:

      /**
       * Default constructor. Is equivalent to using
       * "http://localhost/index.html".
       */
      urlFields();

      /**
       * Constructor.
       */
      urlFields(const std::string& u);

      /**
       * Returns the port number for the URL. If no number is
       * specified, the port depends on the service type.
       * E.g. its 80 for HTTP, 21 for FTP.
       */
      inline int getPort() const { return port; };

      /**
       * Returns the hostname.
       */
      inline const std::string& getHostname() const { return hostname; };

      /**
       * Returns the path.
       */
      inline const std::string& getPath() const { return path; };

      /**
       * Returns the service type.
       */
      inline serviceType getService() const { return service; };

    protected:
      int port;
      serviceType service;
      std::string hostname;
      std::string path;
    };

    /**
     * This is an abstract base class for handling a kind of
     * data service. It contains methods for initiating a connection
     * given an url and get the most important meta data. A certain
     * service handler is created by subclassing this class.
     * At present, there is only a HTTP handler which is buried in
     * the implementation.
     */
    class serviceHandler: public object {
    protected:
      /**
       * Constructor. Subclasses must call this constructor in order
       * to set the URL for which this handler is used.
       */
      serviceHandler(url& u);

      /**
       * Returns the input stream for the URL which is handled by
       * this object.
       */
      std::ifstream* getIn() { return doc.in; };

      /**
       * Returns the output stream for the URL which is handled by
       * this object.
       */
      std::ofstream* getOut() { return doc.out; };

#ifdef _LTI_WIN32
      /**
       * Returns the socket used to send/receive bytes (only for windows)
       */

      SOCKET getSocket(){return doc.winsock;};
#endif

    public:
      /**
       * Gets the date of the server reply, may be empty.
       * @param date will receive the date in standard ASCII
       * notation
       * @return true if the date could be retrieved successfully,
       * false otherwise.
       */
      virtual bool getDate(std::string& date)=0;

      /**
       * Gets the MIME type description of the document which
       * is described by the URL.
       * @param type will receive the type description in standard MIME
       * notation
       * @return true if the type could be retrieved successfully,
       * false otherwise.
       */
      virtual bool getContentType(std::string& type)=0;

      /**
       * Gets the length of the document which is described by the URL.
       * @param size will receive the size of the document in bytes.
       * If this value is below zero, it means that the size is
       * unknown.
       * @return true if the size could be retrieved successfully,
       * false otherwise.
       */
      virtual bool getContentLength(int& size)=0;

      /**
       * Gets a pointer to an input stream which can be used
       * for retrieving the data.
       * @return true if the stream could be created successfully,
       * false otherwise.
       */
      virtual bool getDataStream(std::ifstream* &i)=0;

      /**
       * return the last message set with setStatusString().  This will
       * never return 0.  If no status-string has been set yet an empty string
       * (pointer to a string with only the char(0)) will be returned.
       */
      const char* getStatusString() const;

      /**
       * Returns true if the handler's state is in order and meta-data
       * can be inquired.
       */
      inline bool isOk() { return ok; };

    protected:
      url& doc;

      bool ok;

      /**
       * set a status string.
       *
       * @param msg the const string to be reported next time by
       * getStatusString(). The given string will be copied.
       * This message will be usually set within the apply methods to indicate
       * an error cause.
       * @param msg2 another message string, which is simply appended
       *        to the first one.
       */
      void setStatusString(const char* msg, const char *msg2=0);

      std::string status;
    };

    /**
     * The kind of data type that is expected for reading and
     * writing ioObjects.
     */
    enum dataType {
      RAW = 0,
      CONFIG = 1,
      LISP = 2,
      MAXDATA = 9
    };

    /**
     * Constructor. Creates a new connection to the URL given as
     * argument. If the connection could be created, the function
     * isOk returns true. Otherwise, it returns false. If the
     * connection failed, the cause can be inquired by calling
     * getStatusString.
     */
    url(const std::string &n);

    /**
     * Destructor. Closes the network connection and deletes all
     * allocated data structures.
     */
    ~url();

    /**
     * Returns true if so far all operations have been successful.
     */
    bool isOk();

    /**
     * set url's parameters.
     * This member makes a copy of <em>theParam</em>: the url
     * will keep its own copy of the parameters!
     * @return true if successful, false otherwise
     */
    virtual bool setParameters(const parameters& theParam);

    /**
     * returns the current parameters
     */
    const parameters& getParameters() const;

    /**
     * Retrieves the URL as text document. This only works if
     * the document is of MIME type "text/...".
     * @return true iff the document could be retrieved successfully.
     */
    bool get(std::string& data);

    /**
     * Retrieves the URL as binary Data written in data
     * @return true iff the document could be retrieved successfully.
     */
    bool get(vector<ubyte>& data);

    /**
     * not implemented yet
     */
    bool get(ioObject& data, dataType fmt);

#   if defined(HAVE_LIBPNG) || defined(HAVE_LOCALPNG)
    /**
     * Retrieves the URL as binary Data written in data
     * @return true iff the document could be retrieved successfully.
     */
    bool getPNGImage(image& data);
#   endif


#   if defined(HAVE_LIBJPEG) || defined(HAVE_LOCALJPEG)
    /**
     * Retrieves the URL as binary Data written in data
     * @return true iff the document could be retrieved successfully.
     */
    bool getJPGImage(image& data);
#   endif

    /**
     * Retrieves the URL as binary Data written in data
     * @return true iff the document could be retrieved successfully.
     */
    bool getBMPImage(image& data);

    /**
     * Not implemented yet.
     */
    bool put(const std::string& data);

    /**
     * Not implemented yet.
     */
    bool put(const vector<ubyte>& data);


    /**
     * Not implemented yet.
     */
    bool put(const ioObject& data, dataType fmt);

    /**
     * Not implemented yet.
     */
    bool putPNGImage(const image& data);

    /**
     * Returns the input stream that is used for reading the
     * document.(linux only)
     */
    std::istream& getInputStream();

    /**
     * Returns the output stream that is used for writing the
     * document. (linux only)
     */
    std::ostream& getOutputStream();

    /**
     * Returns the path component of this URL.
     */
    const std::string& getPath() const;

    /**
     * return the last message set with setStatusString().  This will
     * never return 0.  If no status-string has been set yet an empty string
     * (pointer to a string with only the char(0)) will be returned.
     */
    const char* getStatusString() const;

  protected:

    serviceHandler* handler;

    bool initConnect();
    bool openStreams();

    /**
     * set a status string.
     *
     * @param msg the const string to be reported next time by
     * getStatusString(). The given string will be copied.
     * This message will be usually set within the apply methods to indicate
     * an error cause.
     * @param msg2 another message string, which is simply appended
     *        to the first one.
     */
    void setStatusString(const char* msg, const char *msg2=0);

  private:

    /**
     * current parameters.
     */
    parameters* params;

    std::ifstream* in;
    std::ofstream* out;

    int sock;
    dataType dtype;
    urlFields fields;
    std::string status;
    std::string defaultType;
    friend class serviceHandler;

#ifdef _LTI_WIN32

    SOCKET winsock;

    /**
     * create unique name for temporary image file consisting
     * of path, pid, instance number and original name
     */
    bool getTempFilePath(std::string& filepath);

    /**
     * receive binary data by connected socket and write into out file
     * (httpHeader has already been read)
     */
    bool receiveAndWriteFile(std::ofstream& out);

    /**
     * used for debug only
     */
    int getFileSize(std::string fileName);

    /**
     * used to create unique temporary filename
     */
    static int instanceNumber;
#endif

  };
}

#endif

