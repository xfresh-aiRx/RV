/*$T LabelReader.h GC 1.138 09/25/04 15:08:40 */

/*
 * Copyright (C) 1998, 1999, 2000, 2001 Lehrstuhl fuer Technische Informatik,
 * RWTH-Aachen, Germany This file is part of the LTI-Computer Vision Library
 * (LTI-Lib) The LTI-Lib is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License (LGPL) as published
 * by the Free Software Foundation; either version 2.1 of the License, or (at your
 * option) any later version. The LTI-Lib is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General
 * Public License for more details. You should have received a copy of the GNU
 * Lesser General Public License along with the LTI-Lib; see the file LICENSE. If
 * not, write to the Free Software Foundation, Inc., 59 Temple Place Suite 330,
 * Boston, MA 02111-1307, USA.
 */
#ifndef _LTI_TESTER_H
#define _LTI_TESTER_H

#include "ltiImage.h"

#define WITHOUT_GTK_WIDGETS

namespace	lti
{



class	LabelReader
{


public:
	void operator () (int argc, char *argv[]);


private:
	void	binarize(channel8 &pic, const int level);

	void	invert(channel8 &pic);

	int     LabelReader::classify(channel8 &ImageMask, const int NumberOfOutputClasses);
};




}
#endif
