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
 * file .......: ltiProcessInfo.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 30.10.00
 * revisions ..: $Id: ltiProcessInfo.h,v 1.18 2005/06/22 15:42:10 doerfler Exp $
 */

#ifndef _LTI_PROCESSINFO_H_
#define _LTI_PROCESSINFO_H_

#include "ltiObject.h"
#include <string>

#ifndef _LTI_WIN32
#else

/*
 * In case of WinNT queriing system registry using performance-
 * datahelper Pdh (you may need to specify its library in
 * Project..Settings..Link..Object/Library Modules.)
 * Pdh.lib and pdh.dll are needed.
 *
 * Microsoft didn't include PDH.DLL in some releases of Windows NT.
 * You can download the PDH file for winnt from
 * http://support.microsoft.com/default.aspx?scid=kb;en-us;Q284996,
 * or search the Microsoft Knowledge Base for article "Q284996".
 *
 * NOTE: For Windows 2000-based systems, use the system-supplied Pdh.dll file.
 * Do not install a new version of Pdh.dll over the system-supplied version.
 * This will fail because of Windows File Protection. For Windows NT 4.0,
 * install Pdh.dll into the private directory of the application ltilib\bin
 * (not into the system directory).
 * You have the following nonexclusive, royalty-free rights subject to the
 * Distribution Requirements: You may distribute PDH.dll on Windows NT 4.0.
 */

struct PROCESS_MEMORY_COUNTERS;

#endif

namespace lti {

  /**
   * LTI-Lib process info class. An object of this class can be used
   * among other things to get the amount of memory used by the process.
   *
   * For MS Windows NT you will require the library Pdh.lib/pdh.dll
   * available under: http://support.microsoft.com/default.aspx?scid=kb;en-us;Q284996
   * or search the Microsoft Knowledge Base for article "Q284996"
   */
  class processInfo : public object {
#ifndef _LTI_WIN32
    /**
     * buffer to construct a device path
     */
    char procPath[80];
#endif
  protected:
    /**
     * system depended structure
     */
    struct procStatus {
    public:
#ifdef _LTI_LINUX
      /**
       * default constructor
       * set default values and init array for cpu clockrate storage
       */
      procStatus();

      /**
       * destructor
       * delete memory allocated in constructor
       */
      ~procStatus();

      /**
       * number of processors in this system (available for Linux only)
       */
      static const int MAX_SYSTEM_CPUS;

      /**
       * @name Linux attributes
       */
      //@{

      /**
       * The process id
       */
      int pid;

      /**
       * The filename of the executable
       */
      char *comm;

      /**
       * State of process, in Linux: one of [RSDZT]
       */
      char state;

      /**
       * parent process id
       */
      int ppid;

      /**
       * group id
       */
      int pgrp;

      /**
       * number of page faults so far
       */
      unsigned int majflt;

      /**
       * number of page faults of process and its children
       */
      unsigned int cmajflt;

      /**
       * priority, i.e. nice level
       */
      int priority;

      /**
       * virtual memory size
       */
      int vsize;

      /**
       * resident set size
       */
      int rss;

      /**
       * total core memory
       */
      int coremem;

      /**
       * total swap memory
       */
      int swapmem;

      /**
       * free core memory
       */
      int freecoremem;

      /**
       *  buffers actual used
       */
      int buffers;

      /**
       *  cached memory used
       */
      int cached;

      /**
       * free swap memo
       */
      int freeswapmem;

      /**
       * cpu number
       */
      int numberofcpus;

      /**
       * clockrate
       */
      int* clockrate;

      /**
       * processorload
       */
      float processorload;
      //@}
#endif

#ifdef _LTI_WIN32
      /**
       * @name Windows Attributes
       */
      //@{
          /**
       * default constructor
       * set default values and init array for cpu clockrate storage
       */
      procStatus();

      /**
       * destructor
       * delete memory allocated in constructor
       */
      ~procStatus();
          /**
       * number of processors in this system
       */
      static const int MAX_SYSTEM_CPUS;
      /**
       * process id
       */
      void* pid;
      /**
       * working set size
       */
      unsigned int wss;
      /**
       * page file usage
       */
      unsigned int pfu;
      /**
       * clockrate
       */
      int* clockrate;
      /**
       * number of CPUs
       */
      int numberOfCPUs;
      //@}
#endif
    };

  public:
    /**
     * default constructor
     */
    processInfo();

    /**
     * destructor.
     */
    virtual ~processInfo();

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const;

    /**
     * Returns the amount of virtual memory used by this process in kBytes.
     * The actual meaning of this value depends on the operating system:
     * In Unix-like OSs it really means the total amount of virtual memory
     * allocated to this process. In WinNT, it means the size of the page
     * file fragment that has been reserved to the process.
     */
    int getUsedVirtualMemory();

    /**
     * Returns the amount of core memory. What this means, differs
     * depending on the OS. In Unix, it really is the amount of physical
     * RAM that is occupied by this process. In WinNT, it is the size of
     * the working set, which may be larger than the allocated pagefile
     * segment. It is not quite clear what the term "working set" in
     * windows actually means. One document states that it is the amount
     * of physical RAM the process occupies. Another document states that it
     * is the average amount of memory the process accesses without causing
     * a page fault. So, in WinNT, the result of this function is best
     * interpreted as a guess.
     */
    int getUsedCoreMemory();

    /**
     * Returns the amount of total available virtual memory in kBytes.
     * In case of an error the function returns (-1)
     */
    int getVirtualMemory();

    /**
     * Returns the amount of total available core memory in kBytes.
     * In case of an error the function returns (-1)
     */
    int getCoreMemory();

    /**
     * Returns the amount of free core memory in kBytes.
     * In case of an error the function returns (-1)
     */
    int getFreeCoreMemory();

    /**
     * Returns the amount of used buffers in kBytes.
     * In case of an error and in WinNT the function returns (-1)
     */
    int getBuffers();

    /**
     * Returns the amount of memory cached in kBytes.
     * In case of an error and in WinNT the function returns (-1)
     */
    int getCachedMemory();

    /**
     * Returns the amount of free swap memory in kBytes.
     * In case of WinNT the function returns the free disk space
         * available to the PageFile.
         * In case of an error the function returns (-1)
     */
    int getFreeSwapMemory();

    /**
     * Returns the amount of free virtual system memory in kBytes.
     * In case of an error the function returns (-1)
     */
    int getFreeVirtualMemory();

    /**
     * Returns the number of CPUs in the measured system.
     * Uses the /proc/cpuinfo file in linux systems.
     * In case of an error the function returns (0).
     */
    int getNumberOfCPUs();

    /**
     * Returns an array of the processor clock rates.
     * Uses the /proc/cpuinfo file in linux systems.
     * In WinNT the function returns clock rate
         * only for systems with one processor
     */
    int* getProcessorClockRate();

    /**
     * Returns the ProcessorLoad of the last minute.
     * Uses the /proc/loadavg file in linux systems.
     * In case of an error the function returns (-1).
         * In case of WinNT calling Win32 API routine,
         * you may need to specify its library in
         * Project..Settings..Link..Object/Library Modules.
         * here: Pdh.lib and you also need Pdh.dll
     */
    float getProcessorLoad();

    /**
     * Return the actual meaning of "k" (1000 or 1024)
     */
    inline int getSizeOfK() const { return sizeOfK; };

    /**
     * Set the meaning of "k" as 1024 (2^10)
     */
    inline void kEquals1024() { sizeOfK=1024; };

    /**
     * Set the meaning of "k" as 10^3
     */
    inline void kEquals1000() { sizeOfK=1000; };

    /**
     * Returns the name of the host this process is running on.
     */
    std::string getHostname() const;

    /**
     * Returns the user ID of the user that has started the process.
     */
    std::string getUsername() const;

    /**
     * Returns the process ID of the current process/thread.
     */
    long getPid() const;

  private:
    procStatus state;
    int sizeOfK;
#ifndef _LTI_WIN32
    char *commBuffer;
    int bufLength;
    void getCurrentCPUStatus();
    void getCurrentStatus();
    void getCurrentSystemStatus();
    void readBuffer();
#else
    PROCESS_MEMORY_COUNTERS *commBuffer;
        void getCurrentStatus();
        long ReadRDTSCTimer();
        void getCpuStatus();
#endif
  };

}

#endif

