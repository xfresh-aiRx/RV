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
 * file .......: ltiTimer.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 12.06.00
 * revisions ..: $Id: ltiTimer.h,v 1.10 2005/06/22 15:42:10 doerfler Exp $
 */

#ifndef _LTI_TIMER_H
#define _LTI_TIMER_H

#include "ltiObject.h"
#include <string>

namespace lti {

  /**
   * This global function is a replacement in the %lti namespace for
   * the sleep and Sleep functions of Windows and Unix.  To avoid
   * misunderstanding the name has been changed to passiveWait.  The
   * time must always be given in microseconds, although the real
   * precision will be determined by the operating system.  (Windows
   * uses milliseconds and Unixes a few microseconds, depending on the
   * implementation of usleep.
   *
   * @param usTime number of microseconds to wait
   */
  void passiveWait(const int& usTime);

  /** The timer allows to measure time with a precision of about 30us on
      Linux systems and ??? on windows systems.

      The maximum time that can be measured with this function is 1 day
      (86.4E+09 microseconds).  To measure longer time intervalls use the
      standard time() function.

      Note that this function returns the Wall-Clock time and not the CPU-Time.

      Example:

      \code

      lti::timer chron;

      chron.start();

      // do something

      chron.stop();

      std::cout << "something takes " << chron.getTime() << " microseconds\n";

      \endcode
  */
  class timer : public object {
  public:
    /// default constructor
    timer();

    /// start the timer
    void start();

    /// stop the timer
    void stop();

    /** get the elapsed time (in microsecond) between start() and stop() or the
        actual time (if stop() is not been called yet!)

        @return elapsed time
     */
    double getTime() const;

    /// name of this type
    virtual const char* getTypeName() const;

    static std::string getDateAndTime();

  protected:
    double startTime;
    double endTime;
    bool started;

    // get actual time
    double getActualTime() const;

    // only for MS VC++ version
# ifdef _LTI_WIN32
  private:
    // 2^32
    static const double max32bit;
    // frequency (Hz) of the performance counter
    double freq;
# endif

  };
}

#endif

