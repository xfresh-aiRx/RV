/*
   RV02: Median und Sobel
   
   Autor: .....................
   HAW-University of Applied Sciences - Hamburg,Germany

 */ 

#include "ltiObject.h"

#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <list>
#include <stdio.h>

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

	/*---------------------*/
	/* images & channels   */
    /*---------------------*/
    image img;                              // normalized (color) image
	channel8  src;  // source picture       // 8-bit-image (source)
    channel8  dst;  // destination picture  // 8-bit-image (source) 


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
    const int columnSize = src.columns();


    // set destination size to source size 
    dst.resize(rowSize,columnSize,0,false,true);


    
    Median(src,dst,9,9);



	// view pictures
    view.show(src);
    viewTransformed.show(dst);

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
	const int PicSizeY = sPic.rows();
	const int PicSizeX = sPic.columns();

	int x,y,mx,my;

	for(y=0; y<PicSizeY; y++)
		for(x=0; x<PicSizeX; x++)
			dPic[y][x] = 256 - sPic[y][x];	

  }





};
