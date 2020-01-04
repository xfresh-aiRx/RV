/*$T LabelReader.cpp GC 1.138 09/25/04 16:05:10 */

/*
 * LabelReader.cpp HAW - Hochschule für Angewandte Wissenschaften - Hamburg
 * Lehrgebiet: Technische Informatik Prof.Dr.-Ing. Andreas Meisel
 */
#include "ltiObject.h"

#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <list>
#include <stdio.h>

#include "LabelReader.h"
#include "ltiGeometricTransform.h"
#include "ltiBMPFunctor.h"
#include "ltiViewer.h"
#include "ltiSplitImg.h"
#include "ltiDraw.h"
#include "gtk.h"
#include "ltiGtkServer.h"
#include "ltiObjectsFromMask.h"
#include "ltiBoundingBox.h"
#include "ltiBoundingBox_template.h"
#include "ltiLocation.h"
#include "ltiOptimalThresholding.h"
#include "ltiMergeChnl.h"

#include "numbers.h"

using std:: cout;
using std:: endl;

namespace	lti
{
void LabelReader::operator	() (int argc, char *argv[])
{
	/*
	 * has always to be started (AMei) £
	 * if program is without gtk-Widgets
	 */
	gtkServer	server;
	server.start();

	/* Program Constants */
	const int		CHARMASK_X = 10;
	const int		CHARMASK_Y = 16;

	const int		NUMBER_OF_OUTPUT_CLASSES = 10;

	const int		thresholdLevel = 8;

	/* Variables */
	int				x, y, x_size, y_size;
	int				BoxSize_x, BoxSize_y;
	int				currentNumber;

	double			Max;
	int				MaxIndex;
	rectLocation	BoxLocation;
	tpoint<float>	ObjLocation;
	point			UL, LR;
	tpoint<float>	RescaleFactor;

	/*
	 * instantiation of used components £
	 * images, channels and lists RGB-Images
	 */
	image			img_OriginalScan, 
					img_OriginalScanInverted, 
					img_CurrentSeparatedObject, 
					img_CurrentObjectResized(CHARMASK_X, CHARMASK_Y, Black);

	/* Grayscale-Picture 8-bit */
	channel8		c8_OriginalScan, 
					c8_Segmented, 
					c8_CurrentObjectResized;

	std::list < areaPoints > ListOfSeparatedObjects;	/* list of handwritten chars */

	/* Loader, splitter and viewer */
	loadBMP										loader;
	splitImageToHSI								splitter;
	mergeHSIToImage								merger;

	viewer										viewOriginal("OriginalScan");
	viewer										viewResult("separated objects");
	viewer										viewResult2("segmented picture");
	viewer										viewResized("fitted handwritten character");

	/*
	 * Segmenter + parameters segment objects (handwritten characters) and suppress
	 * background
	 */
	optimalThresholding							segmenter;
	optimalThresholding::parameters				segmenter_param;

	/* Object-separator extracts objects from segmented images */
	objectsFromMask								ObjectSeparator;

	/* - Bounding-Box-component + parameters */
	boundingBox<image::value_type>				FitObjectIntoBoundingBox;
	boundingBox<image::value_type>::parameters	FitObjectIntoBoundingBoxParam;
	FitObjectIntoBoundingBoxParam.borderSize = 0;
	FitObjectIntoBoundingBoxParam.centerOfGravity = false;

	/*
	 * Resizer resize an image to desired format (affine transform. + bilinear
	 * interpol)
	 */
	geometricTransform::parameters	geoPar;
	geoPar.keepDimensions = true;

	/* Create geometric transformation object */
	geometricTransform	geoTrans(geoPar);

	/* Drawer */
	draw<rgbPixel>		drawer;
	drawer.use(img_OriginalScan);
	drawer.setColor(lti::Red);

	/*
	 * the program
	 * LOAD image, convert to channel 8 and invert
	 */
	loader.load("Numbers.bmp", img_OriginalScan);

	splitter.getIntensity(img_OriginalScan, c8_OriginalScan);

	invert(c8_OriginalScan);		/* reason: segmentation extraxts white objects */

	/* in front of a black background */
	img_OriginalScanInverted.castFrom(c8_OriginalScan);

	/* SEGMENTATION: create a picture where interesting areas are "NOT 0" */
	segmenter.apply(c8_OriginalScan, c8_Segmented);

	viewResult2.show(c8_Segmented); /* show segmented picture */
	viewOriginal.show(img_OriginalScan);

	/* SEPARATION: extract areaPoint-lists of all (outer) regions */
	ObjectSeparator.apply(c8_Segmented, ListOfSeparatedObjects);

	/* FIT OBJECTS INTO FIXED SIZE BOX: */
	std::list < areaPoints >::iterator it;

	/* foreach object ... */
	for(it = ListOfSeparatedObjects.begin(); it != ListOfSeparatedObjects.end(); it++)
	{
		/* get a new image with the current object */
		FitObjectIntoBoundingBox.apply
			(
				img_OriginalScanInverted,   /* source image */ 
				*it,					    /* curr.mask (AreaPoints) */
				img_CurrentSeparatedObject, /* picture with separated object */ 
				BoxLocation	
			);	/* center of object in source pic. */

		/*
		 * draw a red rect. around current object (handwritten chars) determine object
		 * position and size
		 */
		ObjLocation = BoxLocation.position;
		BoxSize_x = img_CurrentSeparatedObject.columns();
		BoxSize_y = img_CurrentSeparatedObject.rows();

		/* determine upper left(UL) and lower right (LR) corner of the object box */
		UL.x = ObjLocation.x - BoxSize_x / 2;
		UL.y = ObjLocation.y - BoxSize_y / 2;
		LR.x = ObjLocation.x + BoxSize_x / 2;
		LR.y = ObjLocation.y + BoxSize_y / 2;

		drawer.setColor(lti::Red);

		/* drawer.set(BoxLocation); */
		drawer.box(UL, LR);

		/* fit current object (handwr.char.) into fixed size pict. calculate scale factor */
		RescaleFactor.x = (float) CHARMASK_X / BoxSize_x;
		RescaleFactor.y = (float) CHARMASK_Y / BoxSize_y;

		/* affine transformation with bilinear interpolation */
		geoTrans.scale(img_CurrentSeparatedObject, img_CurrentObjectResized, RescaleFactor);

		/* resize image format to fixed size */
		img_CurrentObjectResized.resize(CHARMASK_Y+1, CHARMASK_X+0);

		/* binarize current object (handwritten character) */
		splitter.getIntensity(img_CurrentObjectResized, c8_CurrentObjectResized);

		/* binarize resized object (handwritten character) */
		binarize(c8_CurrentObjectResized, thresholdLevel);

		/* classify and print result class */
		printf("%d ", classify(c8_CurrentObjectResized, NUMBER_OF_OUTPUT_CLASSES));

		/* view results */
		viewResult.show(img_CurrentSeparatedObject);
		viewResized.show(c8_CurrentObjectResized);

		/*
		 * viewOriginal.show(img_OriginalScan); £
		 * getchar();
		 */
		drawer.setColor(lti::Blue);
		drawer.box(UL, LR);

		getchar();
	}
	viewOriginal.show(img_OriginalScan);
	getchar();
}





/*
 =======================================================================================================================
    private methods
 =======================================================================================================================
 */
void LabelReader::binarize(channel8 &pic, const int level)
{
	/*~~~~~~~~~~~~~*/
	int sx, sy, x, y;
	/*~~~~~~~~~~~~~*/

	/* determine picture size */
	sy = pic.rows();
	sx = pic.columns();

	/* binarize picture */
	for(y = 0; y < sy; y++)
		for(x = 0; x < sx; x++)
		{
			pic[y][x] = pic[y][x] > level ? 1 : 0;
		}

	return;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void LabelReader::invert(channel8 &pic)
{
	/*~~~~~~~~~~~~~*/
	int sx, sy, x, y;
	/*~~~~~~~~~~~~~*/

	/* determine picture size */
	sy = pic.rows();
	sx = pic.columns();

	/* invert picture */
	for(y = 0; y < sy; y++)
		for(x = 0; x < sx; x++)
		{
			pic[y][x] = 255 - pic[y][x];
		}

	return;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
int LabelReader::classify(channel8 &ImageMask, const int NumberOfOutputClasses)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	int		sx, sy, x, y, i;
	int		Mask_x, Mask_y;
	int		currentNumber, MaxIndex;
	double	*inMask, *outMask, Max;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	sy = ImageMask.rows();
	sx = ImageMask.columns();

	inMask = (double *) malloc(sx * sy * sizeof(double));
	if(inMask == NULL) return -1;
	outMask = (double *) malloc(NumberOfOutputClasses * sizeof(double));
	if(outMask == NULL) return -1;

	/* convert imagedata to neural network inputmask */
	for(Mask_y = 0; Mask_y < sy; Mask_y++)
	{
		for(Mask_x = 0; Mask_x < sx; Mask_x++)
		{
			inMask[Mask_y * sx + Mask_x] = ImageMask[Mask_y][Mask_x];
		}
	}

	/****************/
	/* classify now */
	/****************/
	currentNumber = numbers(inMask, outMask, 0);



	/* figure out the class index (= output-number with max. output-value) */
	Max = 0.0;
	MaxIndex = 0;
	for(i = 0; i < NumberOfOutputClasses; i++)
	{
		if(outMask[i] > Max)
		{
			MaxIndex = i;
			Max = outMask[i];
		}
	}

	/* free allocated memory */
	free((void *) outMask);
	free((void *) inMask);

	return MaxIndex;
}
};
/* end namespace */
