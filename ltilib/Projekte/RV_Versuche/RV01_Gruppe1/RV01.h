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

#define WITHOUT_GTK_WIDGETS 


namespace lti {

  /* use this object to test your code:  just reimplement the operator() code
     in the ltiTester.cpp file
     the main program will call tester()!! 
  */
  class RV01 {
	struct matrix
	{
		double A0;
		double B0;
		double A1;
		double B1;
		double A2;
		double B2;
	};
	
	struct PointMatrix
	{
		int X[4];
		int Y[4];
	};
  public:
    void operator()(int argc,char *argv[]);
	bool checkBorder(int coordinate, int maxsize);
	int pointTransformation( 
		const struct PointMatrix points,
		const channel8& src,
		channel8& dst
		);
	int affineTransformation(
		const struct matrix,
		const channel8& src, 
		channel8& dst
		);
  private:
	int (lti::RV01::*interpol)(const double dX, const double dY, const channel8& img);
	int bilinInterpol(const double dX, const double dY, const channel8& img);
	int nearestNeighbour( const double dX, const double dY, const channel8& img);
	int round(double i);
	double phi(int num, double x, double y);

  };
}

#endif