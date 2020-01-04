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
 * file .......: ltiClassificationStatistics.h
 * authors ....: Pablo Alvarado, Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 08.10.2000
 * revisions ..: $Id: ltiClassificationStatistics.h,v 1.23 2005/06/16 09:32:39 doerfler Exp $
 */

#ifndef _CLASSIFICATION_STATISTICS_H
#define _CLASSIFICATION_STATISTICS_H

#include "ltiIoObject.h"
#include "ltiDynamicMatrix.h"
#include "ltiClassifier.h"

#include <string>
#include <ctime>

namespace lti {

  /**
   * This class allows the generation of classification statistics.
   *
   * This object will generate some confusion matrices and log-files, which
   * can help to the selection of classifier-parameters.
   *
   * The results of the classification will be stored directly into the user
   * specified files.
   *
   * Following example shows how to use this object in the statistics for
   * classification results.
   *
   * \code
   *
   * lti::rbf theNet;     // a rbf-Network
   *
   * // Training data set:
   * // Some points in a two-dimensional feature-space will belong to one
   * // of two classes: circle or square
   * lti::dmatrix train(10,2);
   * double trainData[20] = { 0, 3, // circle
   *                          1, 3, // circle
   *                          2, 3, // circle
   *                          0, 2, // circle
   *                          1, 2, // circle
   *                          2, 1, // square
   *                          3, 1, // square
   *                          4, 1, // square
   *                          3, 0, // square
   *                          4, 0};// square
   * train.fill(trainData);
   *
   * // the ids for the patterns in the training set
   * lti::ivector trainIds(10);
   * int trainClasses[10] = {314,314,314,314,314,4,4,4,4,4};
   * trainIds.fill(trainClasses);
   * 
   * // just to see what's happening when training...
   * lti::streamProgressInfo progBox;
   * theNet.setProgressObject(progBox);
   * 
   * // train the classifier
   * theNet.train(train,trainIds);
   *    
   * // ------------- STATISTICS --------------
   * 
   * // prepare the statistics object
   * lti::rbf::outputVector nnoutput; // output of the rbf network
   * lti::classificationStatistics statistics;
   * lti::classificationStatistics::parameters statParam;
   * 
   * // all statistic files should be stored in the "tmp" directory
   * statParam.path = "/tmp/";
   * 
   * // and of course, indicate the statistics-object which parameters
   * // should be used:
   * statistics.setParameters(statParam);
   *
   * // assign some human-readable names to the object-Ids
   * std::map<int,std::string> names;
   * names[314] = "Circle";
   * names[4]   = "Square";
   * 
   * statistics.setNames(names);
   * 
   * // now test some points of the 2D-feature space, and generate
   * // some statistics about the classification:
   * 
   * double x,y; // the coordinates of a point in the feature space
   * int realID; // for the statistics is required the real id of
   * // the given feature, to check if the classification is
   * // correct or not!
   * 
   * char buffer[1024]; // a temporary string buffer
   * 
   * lti::dvector feature(2);// a feature-vector
   *
   * for (y=0;y<=4.0;y+=0.5) {
   *   for (x=0;x<=4.0;x+=0.5) {
   *     realID = (y>x) ? 314 : 4; // generate the real id!
   *  
   *     feature.at(0)=x;          // initialize the feature vector
   *     feature.at(1)=y;
   *
   *     theNet.classify(feature,nnoutput);// classify the vector!
   *
   *     // generate a feature-name for the feature
   *     sprintf(buffer,"x: %.2f, y: %.2f",x,y);
   *
   *     // consider the actual feature in the statistics
   *     statistics.consider(nnoutput,realID,buffer);
   *   }
   * }
   *
   * statistics.flush(); // save all the statistics on disk!
   *
   * \endcode
   *
   * Two files will be created: /tmp/logfile.txt with the list of the
   * wrong classifications and /tmp/confusion.mat with the confusion
   * matrices.
   *
   * The confusion matrix file has following information:
   *
   * - How many classes were considered in the classification.
   * - The confusion matrix itself, which show at each row how many
   *   percent of the presentations of the given object were recognized
   *   as the given objects.  For example, the previous code will produce
   *   following matrix:
   *
   * \code
   * ---     Circle  Square correctSingle correctMultip correctReject ...
   * Circle   1.000   0.000      1.000000      0.000000      0.000000 ...
   * Square   0.222   0.778      0.777778      0.000000      0.000000 ...
   * \endcode
   *
   * which means that the circle were allways correctly recognized, and the
   * square was 22% wrong and 78% correctly classified.
   * - Total statistics.  Here will be listed how many classifications were
   *   considered in the statistics, the correct and wrong classifications and
   *   the rejections.
   * - The n-Best statistic indicates that the correct object was at the n-th
   *   position
   * - The shared n-Best position indicates how many of the classifications
   *   were shared between different classes.
   * - The mininal number of positions indicates how many positions need to
   *   be taken into consideration to assure a correct classification.
   */
  class classificationStatistics : public ioObject {
  public:

    // --------------------------------------------------
    // classificationStatistics::parameters
    // --------------------------------------------------

    /**
     * the parameters for the class classificationStatistics
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
       * copy data of "other" parameters
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

      // -------------------------------------------------------------
      // the parameters
      // -------------------------------------------------------------

      /**
       * Use class names in file.
       *
       * If true, the ids of the objects are substituted by names
       * given in the file specified by parameter 'namesFile'
       * (Default value: false).
       *
       * \b Note: this must be set to false if you intend to
       * specify the idToNamesMap manually by calling setNames().
       */
      bool useNames;

      /**
       * Filename with class names.
       *
       * Name of the file where the names for the objects can
       * be found (Default value "objectNames.nms"). 
       *
       * This name is not expected to have a path, since it will always be
       * seached in the directory specified in 'path'.  Thus, the names file
       * will be expected at parameters::path + parameters::namesFile.
       *
       * These files consist of two columns. The first containing
       * the object ids, the second the object names in double quotes and
       * separated by whitespaces e.g.
       * \code
       * 314 "Circle"
       * 4   "Square"
       * \endcode
       *
       * \b Note: only considered, if parameters useNames==true. Make
       * sure that the parameter path is also specified!
       */
      std::string namesFile;

      /**
       * Names in file first
       *
       * The order of the columns in the names file can be inverted setting
       * this parameter to true.  
       *
       * Set to true if each line contains name of object followed by id.
       *
       * Set to false if each line contains the id followed by the object name.
       *
       * Default value: false
       */
      bool namesInFileFirst;

      /**
       * If the confusion matrix is going to be read by humans, it is
       * usually better if "-" are used instead of zeros.  
       * If you need other tools (like Matlab, Excel, etc.) to read the
       * confusion matrix files, you need the zeros.
       * 
       * Default value: true
       */
      bool suppressZeros;

      /**
       * if true, the confusion matrix will be saved automatically
       * in the file specified by parameter 'confusionMatrix'. Saving
       * intervals are defined by parameter 'saveStep' and on
       * destruction (Default value: true).
       */
      bool writeConfusionMatrix;

      /**
       * name of the file, where the confusion matrix will be saved
       * (Default value "confusion.mat"). Will be appended to
       * parameter 'path'.
       *
       * \b Note: only considered, if parameter writeConfusionMatrix==true.
       */
      std::string confusionMatrix;

      /**
       * specify the intervall of entries for automatic saving of
       * the confusion-matrix (Default value: 100, i.e. each 100 entries
       * the confusion-mtrix will be saved automatically). Alternatively
       * you may call flush() to force write at any moment.
       *
       * \b Note: only considered, if parameter writeConfusionMatrix==true.
       */
      int saveStep;

      /**
       * use a protocol file, where each entry will be registered.
       * (Default value: true)
       */
      bool useLogFile;

      /**
       * name for the protocol file (Default value "logfile.txt").
       * Will be appended to parameter 'path'.
       *
       * \b Note: only considered, if parameter useLogFile==true.
       */
      std::string logFile;

      /**
       * if true, all patterns will be registered in the log file, if false,
       * only the errors will be registered (Default value: false).
       *
       * \b Note: only considered, if parameter useLogFile==true.
       */
      bool logAllPatterns;

      /**
       * The "path" string will be concatenated before all filenames
       * (Default value: "", i.e. working directory).
       */
      std::string path;
    };

    /**
     * types of classification results
     */
    enum eStatistics {
      CorrectSingle  =-1, /*!< classifier chooses one class and it is the
                               correct one */
      CorrectMultiple=-2, /*!< classifier chooses many classes and one of
                               them is correct */
      CorrectReject  =-3, /*!< classifier rejects one class which is indeed
                               not the winner */
      WrongSingle    =-4, /*!< classifier chooses one class and it is wrong */
      WrongMultiple  =-5, /*!< classifier chooses many classes and all of
                               theme are wrong */
      WrongReject    =-6, /*!< classifier rejects a class which would have
                               been the correct winner */
      Error          =-7  /*!< there is an error on the data */
    };


    /**
     * default constructor
     */
    classificationStatistics();

    /**
     * default constructor with parameters
     */
    classificationStatistics(const parameters& par);

    /**
     * Destructor
     */
    ~classificationStatistics();

    /**
     * set the parameters
     */
    bool setParameters(const parameters& params);

    /**
     * returns a const reference to the parameters
     */
    const parameters& getParameters() const;

    /**
     * include a new classification result into the statistics.
     * The patternInfo is additional text that will appear in the
     * logFile. Returns the recognition state (see eStatistics).
     *
     * \b Note: the ID must be positive. negative indices are discarded.
     */
    eStatistics consider(const classifier::outputVector& result,
                         const int& realObjectID,
                         const std::string& patternInfo = "-?-");

    /**
     * reset clears the statistics
     */
    void reset();

    /**
     * save all statistics stored by now into the file specified in the
     * parameters. Note that the parameter writeConfusionMatrix must be
     * set to true.
     */
    void flush();

    /**
     * write all accumulated statistic data to the specified stream.
     * this can be used to manually save the statistics.
     */
    bool writeData(std::ostream& ostr);

    /**
     * load the file "filename", which contains the correspondences between
     * a class id number and a class name.
     *
     * \b Note: calling this does NOT set the internal idToNamesMap!!!
     *
     * The file must consist of two columns. The first containing
     * the object ids, the second the object names (optionally in double
     * quotes) and separated by whitespaces e.g.
     * \code
     * 314 "Circle"
     * 4   "Square"
     * \endcode
     *
     * @param filename the name of the file to be used
     * @param idToNamesMap the map where the correspondences will be written.
     *                     all previous data in the map will be kept.
     * @param namesInFileFirst invert the expected order of the data.
     */
    static bool loadNames(const std::string& filename,
                          std::map<int,std::string>& idToNamesMap,
                          const bool namesInFileFirst=false);


    /**
     * set the correspondences between a class id number and a class name.
     *
     * @param idMap the map where the correspondences are given.
     */
    bool setNames(const std::map<int,std::string>& idMap);


    /** @name Data access members
     *        Use these to access the internal data members
     */
    //@{

      /** returns a const reference to protected member "saved"
       */
      const bool& getSaved() const;

      /** returns a const reference to the confusion matrix
       */
      const dynamicMatrix<double>& getConfusionMatrix() const;

      /** returns a const reference to the idToNamesMap
       */
      const std::map<int,std::string>& getIdToNamesMap() const;

    //@}


    /** @name Statistics access members
     *        Use these to access the accumulated statistics
     */
    //@{

      /** returns a const reference to the number of results
       */
      const int& getNumberOfResults() const;

      /** number of correct singles
       */
      const int& getCorrectSingle() const;

      /** number of correct multiples
       */
      const int& getCorrectMultiple() const;

      /** number of correct rejects
       */
      const int& getCorrectReject() const;

      /** number of wrong singles
       */
      const int& getWrongSingle() const;

      /** number of wrong multiples
       */
      const int& getWrongMultiple() const;

      /** number of wrong rejects
       */
      const int& getWrongReject() const;

      /** nBest recognition results for single correct
      */
      const dynamicMatrix<double>& getNBestMatrix() const;

      /** nBest recognition results for multiple correct
      */
      const dynamicMatrix<double>& getNBestMultipleMatrix() const;

      /** minPlaces, i.e. minimum number of elements required in order to
      *  contain the proper class.
      */
      const dynamicMatrix<double>& getMinPlacesMatrix() const;

    //@}


  protected:

    /**
     * save all statistic data in the respective files
     */
    bool writeData();

    /**
     * write confusion matrix to file
     */
    bool writeConfusionMatrix(std::ostream& out,
                              const dynamicMatrix<double>& matrix);

    /**
     * write nBest-matrix
     */
    bool writeNBestMatrix(std::ostream& out,
                          const dynamicMatrix<double>& matrix,
                          const std::string& title,
                          const bool& normalize,
                          const std::string& labelSingular = "Best",
                          const std::string& labelPlural = "Best");

    /**
     * write classification to file
     */
    bool writeLogFileHeader(const std::string& filename);

    /**
     * write classification to file
     */
    bool writeLogFile(const classifier::outputVector& result,
                      const eStatistics& rState,
                      const int& realObjectID,
                      const std::string& patternInfo);

    /**
     * sort the object names in the matrices
     */
    void sortObjNames(const dynamicMatrix<double>& matrix,
                      std::vector<int>& seqToRealId);

    /**
     * workaround for std::ios bug in gcc when writing double numbers
     */
    void format(std::ostream& out,
                const int& width,
                const double& val) const;


    /**
     * workaround for std::ios bug in gcc when writing double numbers
     */
    void format(std::ostream& out,
                const int& width,
                const int& val) const;

    /**
     * use in the statistics an object with the given name and real id number
     */
    bool inscribeObject(const std::string& name, const int& id);


  protected:

    /**
     * store parameters
     */
    parameters param;

    /**
     * if true, all data has been saved.  If false, some consider() have been
     * called, and there are unsaved information.
     */
    bool saved;

    /**
     * the confusion matrix stores the classification results, each
     * row represents one class (row index = realObjectID) and the
     * classification results for this class
     */
    dynamicMatrix<double> confMatrix;


    /**
     * map an id number to the name of the object.
     */
    std::map<int,std::string> idToNamesMap;


    /** @name Statistics
     *        These are the internal members for storing the actual classification statistics
     */
    //@{

      /** number of "consider(...)" called so far. Divide any value, e.g. correctSingle,
       *  by this to obtain recognition rates.
       */
      int numberOfResults;

      /** number of correct singles
       */
      int correctSingle;

      /** number of correct multiples
       */
      int correctMultiple;

      /** number of correct rejects
       */
      int correctReject;

      /** number of wrong singles
       */
      int wrongSingle;

      /** number of wrong multiples
       */
      int wrongMultiple;

      /** number of wrong rejects
       */
      int wrongReject;

      // matrices with statistics per object, which tell how many n-best
      // classification have been done already

      /** nBest recognition results for single correct
       */
      dynamicMatrix<double> nBest;

      /** nBest recognition results for multiple correct
       */
      dynamicMatrix<double> nBestMultiple;

      /** minPlaces, i.e. minimum number of elements required in order to
       *  contain the proper class.
       */
      dynamicMatrix<double> minPlaces;

    //@}

    /**
     * formatting widths for the columns of the log file
     */
    static const int logWidths[];

    /**
     * formatting widths for the columns of the confusion matrix file
     */
    static const int confWidths[];

  };


};

#endif

