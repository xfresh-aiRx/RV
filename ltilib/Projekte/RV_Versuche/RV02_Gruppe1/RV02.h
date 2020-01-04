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

 
#ifndef _LTI_TESTER_H
#define _LTI_TESTER_H


#include "ltiImage.h"
#include <vector>

#define WITHOUT_GTK_WIDGETS 


namespace lti {

  /* use this object to test your code:  just reimplement the operator() code
     in the ltiTester.cpp file
     the main program will call tester()!! 
  */
  class RV02 {
  public:
    void operator()(int argc,char *argv[]);
	int RV02::normAngle(float val);
	void Median(const channel8& sPic, channel8& dPic, const int sizeX, const int sizeY);
	void RV02::sort (int, std::vector<int>);
	void RV02::Sobel (const channel8& sPic,channel8& GradientPic, channel8& DirectionPic);
  };
}
#endif
