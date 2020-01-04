/*
 * Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003
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
 * file .......: ltiKernelType.h
 * authors ....: Bodo Hoffmann
 * organization: LTI, RWTH Aachen
 * creation ...: 11.07.2004
 * revisions ..: $Id: ltiKernelType.h,v 1.1 2004/08/26 08:40:33 doerfler Exp $
 */

#ifndef _LTI_KERNELTYPE_H_
#define _LTI_KERNELTYPE_H_

#include "ltiIoHandler.h"

namespace lti {

  /**
   * kernel type.
   * This type indicates what kind of symmetry a kernel owns.
   *
   * For this type the methods read(ioHandler&, eKernelType&) and
   * write(ioHandler&, const eKernelType&) are implemented. Note
   * that template functions read(ioHandler&, const std::string&, T&)
   * and write(ioHandler&, const std::string&, const T&) exist. (see
   * Developer's Guide for further information).
   */
  enum eKernelType {
	  Default,		/**< There are no known symmetries found.*/
	  Asymmetric,	/**< Left part of kernel has negativ value of right part*/
	  Symmetric,	/**< Left part of kernel is equal to right part*/
  };
  
  /**
   * read function for eKernelType.
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,eKernelType& data);

  /**
   * write function for eKernelType.
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,const eKernelType& data);

  
}
#endif
