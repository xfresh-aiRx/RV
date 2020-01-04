/*
   BME02: Median und Sobel
   
   Autor: .....................
   HAW-University of Applied Sciences - Hamburg,Germany

 */ 

#define _USE_MATH_DEFINES
#include "ltiObject.h"

#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <list>
#include <stdio.h>
#include <algorithm>

#include "RV02.h"
#include "ltiTimer.h"
#include "ltiBMPFunctor.h"
#include "ltiViewer.h"
#include "ltiSplitImg.h"
#include "gtk.h"
#include "ltiGtkServer.h"


using std::cout;
using std::endl;


namespace lti {

  void RV02::operator()(int argc,char *argv[]) {

	/*********************************************/
	/**** has always to be started (AMei)     ****/
    /**** if program is without gtk-Widgets   ****/
	/*********************************************/

	gtkServer server;
    server.start();

	/******************************************/
	/**** instantiation of used components ****/
	/******************************************/

	/*---------------------*/
	/* loaders and viewers */
    /*---------------------*/
    loadBMP loader;                         // object for loading .bmp-images

    viewer view("Original");                // object for visualizing images
	viewer viewTransformed("Filtered");
	viewer viewGradient("Gradient");
	viewer viewDirection("Direction");

	/*---------------------*/
	/* images & channels   */
    /*---------------------*/
    image img;										// normalized (color) image
	channel8  src;			// source picture       // 8-bit-image (source)
    channel8  dst;			// destination picture  // 8-bit-image (source) 
    channel8  dstGradient;  // destination gradient picture  // 8-bit-image (source) 
    channel8  dstDirection; // destination direction picture  // 8-bit-image (source) 


	/*-----------------------------*/
	/* Image processing components */
	/*-----------------------------*/

    // object to split image into hue, saturation and intensity
	// hue        = Farbton
	// saturation = Farbsättigung
	// intensity  = Intensität (Grauwert)
    splitImageToHSI splitter;



	/******************************************/
	/*    the program                         */
	/******************************************/

    // load the source image
    loader.load("shaft.bmp",img);
    
    // extract the intensity channel only
    splitter.getIntensity(img,src);

    // determine image size
    const int rowSize    = src.rows();
	std::cout << rowSize << std::endl;
    const int columnSize = src.columns();
	std::cout << columnSize << std::endl;


    // set destination size to source size 
    dst.resize(rowSize,columnSize,0,false,true);
    dstGradient.resize(rowSize,columnSize,0,false,true);
    dstDirection.resize(rowSize,columnSize,0,false,true);


     view.show(src);
    Median(src,dst,9,9);
    viewTransformed.show(dst);
    Sobel(dst,dstGradient,dstDirection);



	// view pictures
    viewGradient.show(dstGradient);
    viewDirection.show(dstDirection);

    getchar();

  }



  

  /***************************************************************************/
  /* Function definition: ----- Median-operator----                          */
  /***************************************************************************/
  void RV02::Median(  const	     channel8& sPic, 	// source picture 
								 channel8& dPic, 	// destination picture
	                   const int MaskSizeX,		    // mask size in x-direction
					   const int MaskSizeY		 	// mask size in y-direction
					   )
  {
	
	int x,y;
	int mx = MaskSizeX;
	int my = MaskSizeY;
	//Keine graden Werte erlaubt!
	if(mx % 2 == 0)mx++;
	if(my % 2 == 0)my++;
	const int PicSizeY = sPic.rows();
	const int PicSizeX = sPic.columns();
	const int bordery = my/2;
	const int borderx = mx/2;
	const int inTheMiddleOfArray = (my*mx+1)/2;
	
	int hist[256] = {0};
	int n = sizeof(hist)/sizeof(hist[0]);
	int m = inTheMiddleOfArray;
	int i = 0;

	//Bilddurchlauf
	for(y=bordery; y<PicSizeY-bordery; y++)
	{
		for(x=borderx; x<PicSizeX-borderx; x++)
		{
			//Maskendurchlauf
			for (int histY=y-bordery; histY <= y+bordery; histY++)
			{
				for (int histX=x-borderx; histX <= x+borderx; histX++)
				{
					//Histogramm wird erstellt
					hist[sPic[histY][histX]]++;
				}
			}
			
			//Medianberechnung
			m = inTheMiddleOfArray;
			for (i=0; i < 256; i++)
			{
				m = m - hist[i];
				if (m <= 0)
				{
					m = i;
					break;
				}
			}
			//Zuweisung des Medianwertes
			dPic[y][x] = i;
			std::fill(hist, hist+n, 0);
		}
	}

  }

  /***************************************************************************/
  /* Function definition: ----- Sobel-operator----                           */
  /***************************************************************************/
  void RV02::Sobel (const channel8& sPic,channel8& GradientPic,channel8& DirectionPic){
	
	//Faltungsmasken
	int maskX[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
	int maskY[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};

	float aSum = 0.0;
	int x,y, gY, gX, gSum;
	int mx = 3;
	int my = 3;
	const int PicSizeY = sPic.rows();
	const int PicSizeX = sPic.columns();
	const int bordery = my/2;
	const int borderx = mx/2;
	
	int hist[256] = {0};
	int i = 0;
	
	//Bilddurchlauf
	for(y=bordery; y<PicSizeY-bordery; y++)
	{
		for(x=borderx; x<PicSizeX-borderx; x++)
		{
			// Maskendurchlauf
			for (int histY=y-bordery; histY <= y+bordery; histY++)
			{
				for (int histX=x-borderx; histX <= x+borderx; histX++)

				{	//Berechnung der Richtungsableitungen Gx und Gy
					gX += sPic[histY][histX] * maskX[histY-y+1][histX-x+1];
					gY += sPic[histY][histX] * maskY[histY-y+1][histX-x+1];
				}
			}

			// Berechnung des Gradienten
			gSum = sqrt(pow((float)gX,2)+pow((float)gY,2));
			// Berechnung der Gradientenrichtung
			aSum = atan2((double)gY,(double)gX);

			aSum = aSum*180/M_PI*(-1);
			
			GradientPic[y][x] = gSum;
			//Normierung der BildPunkte
			DirectionPic[y][x] = normAngle(aSum);

			gSum = 0;
			aSum = 0;
			gX = 0;
			gY = 0;
		}
	}


  }
	
  /*
	Funktion zur Normierung von Winkeln
	@param val Winkel
	@return Normierter Wert
  */
  int RV02::normAngle(float val)
  {
	if (val <= -22.5)
	{
		val += 360;
	}

	if (-22.5 < val && val <= 22.5)
	{
		return 0;
	}
	else if (22.5 < val && val <= 67.5 )
	{
		return 1;
	}
	else if (67.5 < val && val <= 112.5 )
	{
		return 2;
	}
	else if (112.5 < val && val <= 157.5 )
	{
		return 3;
	}
	else if (157.5 < val && val <= 202.5 )
	{
		return 4;
	}
	else if (202.5 < val && val <= 247.5 )
	{
		return 5;
	}
	else if (247.5 < val && val <= 292.5 )
	{
		return 6;
	}
	else if (292.5 < val && val <= 337.5 )
	{
		return 7;
	}
	else
	{
		return 255;
	}
  }


};
