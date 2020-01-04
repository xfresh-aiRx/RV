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
 * file .......: ltiCentroidClustering.h
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 29.08.2001
 * revisions ..: $Id: ltiCentroidClustering.h,v 1.7 2005/06/16 09:32:39 doerfler Exp $
 */

#include "ltiObject.h"
#include "ltiClustering.h"


#ifndef _LTI_CENTROIDCLUSTERING_H_
#define _LTI_CENTROIDCLUSTERING_H_

namespace lti {


  /**
   * Base class for all clustering methods that use centroids or prototypes
   * for describing individual clusters. A classify(const dvector&) method
   * is implemented that should work for most subclasses.
   */
  class centroidClustering : public clustering {

  public:

    /**
     * default constructor
     */
    centroidClustering();

    /**
     * copy constructor
     * @param other the %object to be copied
     */
    centroidClustering(const centroidClustering& other);

    /**
     * destructor
     */
    virtual ~centroidClustering();

    /**
     * returns the name of this type ("centroidClustering")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor %object
     */
    centroidClustering& copy(const centroidClustering& other);

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * classifies a new data point. <p>
     * This method can be used by all centroid classifiers if they properly
     * initialize the classifier::output member. I.e. for each output unit
     * the list of probabilities may contain only one entry with probability
     * one and each id may be used only once. <p>
     * Since the classifier::output expects a probability vector as result
     * from the clustering method, the dissimilarity measure usually returned
     * must be converted to a similarity measure. Here, this is done by first
     * normalizing the vector of dissimilarities, then subtracting this vector
     * from a vector filled with ones and, finally, normalizing the result.
     * This operation yields similarity values between zero and one for
     * arbitrary scales of the feature space. Information about possible reject
     * cases is lost through the first normalizaton, though.
     *
     * @param feature vector to be classified
     * @param result result as described above
     * @return true if successful, false otherwise
     */
    virtual bool classify(const dvector& feature, outputVector& result) const;

    /**
     * Returns a const reference to the centroids of the clustering
     * @return const reference to the centroids
     */
    const dmatrix& getCentroids() const;

    /**
     * Declared so it wont be forgotten
     */
    virtual bool train(const dmatrix& input) =0;

    /**
     * Calls clustering::train(const dmatrix&, ivector&)
     */
    virtual bool train(const dmatrix& input,
                       ivector& ids);

    /**
     * write the rbf classifier in the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool write(ioHandler& handler,const bool complete=true) const;

    /**
     * read the rbf classifier from the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool read(ioHandler& handler,const bool complete=true);

  protected:

    /**
     * returns current parameters. (non const! -> protected)
     */
//  parameters& getParameters() {return *params;};

    /**
     * matrix containing the centroids formed by the clustering method
     */
    dmatrix centroids;


  private:

    /**
     * Used for calculating a similarity measure from the dissimilarities
     * return by the clustering: 1-d
     */
    static double probabilize(const double& d);

    };

}

#endif

