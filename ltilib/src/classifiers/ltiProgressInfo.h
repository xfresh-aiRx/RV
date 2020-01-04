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
 * file .......: ltiProgressInfo.h
 * authors ....: Pablo Alvarado, Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 10.8.2000
 * revisions ..: $Id: ltiProgressInfo.h,v 1.7 2004/02/24 07:23:32 alvarado Exp $
 */

#ifndef _LTI_PROGRESSINFO_H_
#define _LTI_PROGRESSINFO_H_

#include <string>
#include <iostream>

#include "ltiObject.h"

namespace lti {

  /**
   * This class can be overloaded to display the training progress of a
   * classifier.
   * The default implementation uses the standard output (cout)
   */
  class progressInfo : public object {
  public:
    /**
     * Default constructor
     * @param title the name of the progressInfo object
     * @param maximumSteps the maximum number of steps of the process
     */
    progressInfo(const std::string& title = "",
                 const int& maximumSteps = 100);

    /**
     * Copy constructor
     */
    progressInfo(const progressInfo& other);

    /**
     * Destructor
     */
    virtual ~progressInfo();

    /**
     * Set the title of the progress info block
     */
    virtual void setTitle(const std::string& theTitle)=0;

    /**
     * Maximal number of steps
     */
    virtual void setMaxSteps(const int& maximalSteps)=0;

    /**
     * Detail level of substep information
     */
    virtual void setDetailLevel(const int level);

    /**
     * Return the used detail level of substep information
     */
    virtual int getDetailLevel() const;

    /**
     * Report one step done
     * @param progressInfo string with some text information for the step
     */
    virtual void step(const std::string& progressInfo)=0;

    /**
     * Report additional information for a step, with the given detail
     * level.
     *
     * The given information will be displayed only if the current detail level
     * is higher or equal than the detail specified in this method.
     *
     * The default implementation is void.
     */
    virtual void substep(const int detail,
                         const std::string& info);

    /**
     * Reset progress information
     */
    virtual void reset()=0;

    /**
     * Returns true if someone wants the caller of this progress info
     * object to terminate.
     */
    virtual bool breakRequested() const=0;

    /**
     * The copy member
     */
    progressInfo& copy(const progressInfo& other);

    /**
     * The clone member
     */
    virtual progressInfo* clone() const=0;

  protected:
    /**
     * Detail level used for the substeps
     */
    int detailLevel;
  };


  /**
   * This class is a default implementation for the progress info
   * interface. It dumps the progress information to a std::ostream,
   * or std::cout if you do not give one.
   */
  class streamProgressInfo : public progressInfo {
  public:
    /**
     * Default constructor
     * @param title the name of the progressInfo object
     * @param maximumSteps the maximum number of steps of the process
     */
    streamProgressInfo(const std::string& title = "",
                       const int& maximumSteps = 100);

    /**
     * Default constructor
     * @param outStream output stream, where the progress information will
     *                  be written.
     * @param title the name of the progressInfo object
     * @param maximumSteps the maximum number of steps of the process
     */
    streamProgressInfo(std::ostream& outStream,
                       const std::string& title = "",
                       const int& maximumSteps = 100);


    /**
     * Copy constructor
     */
    streamProgressInfo(const streamProgressInfo& other);

    /**
     * Destructor
     */
    virtual ~streamProgressInfo();

    /**
     * Set the title of the progress info block
     */
    virtual void setTitle(const std::string& theTitle);

    /**
     * Maximal number of steps
     */
    virtual void setMaxSteps(const int& maximalSteps);

    /**
     * Report one step done
     * @param progressInfo string with some text information for the step
     */
    virtual void step(const std::string& progressInfo);

    /**
     * Report additional information for a step, with the given detail
     * level.
     *
     * The given information will be displayed only if the current detail level
     * is higher or equal than the detail specified in this method.
     */
    virtual void substep(const int detail,
                         const std::string& info);

    /**
     * Reset progress information
     */
    virtual void reset();

    /**
     * The copy member
     */
    virtual streamProgressInfo& copy(const streamProgressInfo& other);

    /**
     * The clone member
     */
    virtual progressInfo* clone() const;

    virtual bool breakRequested() const;

    virtual void useLineFeed(bool uself);

  protected:
    /**
     * Maximum number of steps expected
     */
    int maxSteps;

    /**
     * Last processed step
     */
    int lastStep;

    /**
     * Text specified for the last step
     */
    std::string lastStepText;

    /**
     * Title for this progress info
     */
    std::string title;

    /**
     * Stream being used
     */
    std::ostream *out;

    /**
     * The sequence that is used for end-of-line when emitting
     * steps.
     */
    std::string endline;
  };

}

#endif

