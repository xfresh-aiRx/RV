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

#ifndef _LTI_NEW_FUNCTORS
#define _LTI_NEW_FUNCTORS

/** 

\page howtonew How to integrate new algorithms to the LTI-Lib.

The simplest way to integrate new algorithms to the LTI-Lib makes use
of the functor template files and the 
<a HREF="http://www.perl.org">PERL</a>-Script <code>ltiGenerator</code>
in directory <code>ltilib/tools/perl</code>.
  
Just edit the file <code>tools/perl/ltiTemplateValues.txt</code> and
set the values as described in that file.  Then call the
<code>ltiGenerator</code> script. If you use no parameters, the
resulting files will be created in the same directory as the PERL
script (<code>tools/perl/</code>).
  
In the resulting files, you can search for "TODO" to find out what is
still to be done!
  
And don't forget to write documentation in your code using the 
<A HREF="http://www.doxygen.org/index.html">doxygen</A>
style!
  
*/

#endif

