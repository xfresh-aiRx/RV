/*
RV01: Affine Transformation

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

#include "RV01.h"
#include "ltiTimer.h"
#include "ltiBMPFunctor.h"
#include "ltiViewer.h"
#include "ltiSplitImg.h"
#include "gtk.h"
#include "ltiGtkServer.h"
#include "RV01.h"

using std::cout;
using std::endl;


namespace lti {

	void RV01::operator()(int argc,char *argv[]) {

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
		viewer viewTransformed("Transformed");
		viewer pBilin("4 Punkt Transformation Bilinear");
		viewer afBilin("Affine Transformation Bilinear");
		viewer afNn("Affine Transformation Rundung");
		viewer pNn("4 Punkt Transformation Rundung");

		/*---------------------*/
		/* images & channels   */
		/*---------------------*/
		image img;                              // normalized (color) image
		channel8  src;  // source picture       // 8-bit-image (source)
		channel8  dst1;  // destination picture  // 8-bit-image (source)
		channel8  dst2;  // destination picture  // 8-bit-image (source) 
		channel8  dst3;  // destination picture  // 8-bit-image (source) 
		channel8  dst4;  // destination picture  // 8-bit-image (source) 


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
		loader.load("Kalib.bmp",img);

		// extract the intensity channel only
		splitter.getIntensity(img,src);

		// determine image size
		const int rowSize    = src.rows();
		const int columnSize = src.columns();


		// set destination size to source size 
		dst1.resize(rowSize,columnSize,0,false,true);
		dst2.resize(rowSize,columnSize,0,false,true);
		dst3.resize(rowSize,columnSize,0,false,true);
		dst4.resize(rowSize,columnSize,0,false,true);


		// copy source pixels to destination pixels
		
		/*for (int y = 0; y < rowSize; y++)
			for (int x = 0; x < columnSize; x++)
				dst[y][x] = src[y][x];*/
		
		matrix mat;
		mat.A0 = (3859.0/24);
		mat.B0 = (26249.0/120);
		mat.A1 = (53.0/120);
		mat.B1 = (1.0/600);
		mat.A2 = (-13.0/100);
		mat.B2 = (197.0/400);
		
		PointMatrix points;
		points.X[3] = 100;
		points.Y[3] = 457;
		points.X[2] = 666;
		points.Y[2] = 471;
		points.X[1] = 584;
		points.Y[1] = 95;
		points.X[0] = 197;
		points.Y[0] = 100;

		interpol = &lti::RV01::bilinInterpol;
		RV01::pointTransformation(points,src,dst1);
		RV01::affineTransformation(mat,src,dst2);

		
		interpol = &lti::RV01::nearestNeighbour;
		RV01::pointTransformation(points,src,dst3);
		RV01::affineTransformation(mat,src,dst4);
		
		


		// view pictures
		view.show(src);
		pBilin.show(dst1);
		afBilin.show(dst2);
		pNn.show(dst3);
		afNn.show(dst4);
		getchar();

	}


	int RV01::bilinInterpol( const double dX, const double dY, const channel8& img){
		int x1,x2,y1,y2;
		x1 = (int)dX;
		x2 = (int)(dX+1);
		y1 = (int)dY;
		y2 = (int)(dY+1);

		if (RV01::checkBorder(x1,img.columns())
			&& RV01::checkBorder(y1,img.rows())
			&& RV01::checkBorder(x2,img.columns())
			&& RV01::checkBorder(y2,img.rows()))
		{	
			int values[2][2] = {0};
			values[0][0] = img[y1][x1];
			values[0][1] = img[y1][x2];
			values[1][0] = img[y2][x1];
			values[1][1] = img[y2][x2];

			int x_bilin[2] = {0};
			x_bilin[0] = (int) (values[0][0] + (dX-x1)*(values[0][1]-values[0][0]));
			x_bilin[1] = (int) (values[1][0] + (dX-x1)*(values[1][1]-values[1][0]));

			int y_bilin;
			y_bilin = (int) (x_bilin[0] + (dY-y1)*(x_bilin[1]-x_bilin[0]));

			return y_bilin;
		}
		else
		{
			return 0;
		}

	}

	int RV01::nearestNeighbour( const double dX, const double dY, const channel8& img){
		int y = round(dY);
		int x = round(dX);

		
		//std::cout << "[" << y << " , " << x << "]" << " --------- [" << dY << " , " << dX << "]" << std::endl;
		
		if (RV01::checkBorder(x,img.columns()) && RV01::checkBorder(y,img.rows()))
		{
			return img[y][x];
		}
		else
		{
			return 0;
		}

	}

	int RV01::affineTransformation( const struct matrix mat ,const channel8& src, channel8& dst){	
		const int PicSizeY = dst.rows();
		const int PicSizeX = dst.columns();

		double A0 = mat.A0;
		double B0 = mat.B0;
		double A1 = mat.A1;
		double B1 = mat.B1;
		double A2 = mat.A2;
		double B2 = mat.B2;

		int x,y,i = 0;
		double xSrc = 0;
		double ySrc = 0;

		//Bilddurchlauf
		for(y=0; y<PicSizeY; y++)
		{
			for(x=0; x<PicSizeX; x++)
			{
				xSrc = A0 + A1 * x + A2 * y;
				ySrc = B0 + B1 * x + B2 * y;
				
				// hier funktionspointer
				i = (this->*interpol)(xSrc, ySrc, src);
				dst[y][x] = i;
			}
		}
		return 0;

	}

	int RV01::pointTransformation( const struct PointMatrix points, const channel8& src, channel8& dst){
		
		const int PicSizeY = dst.rows();
		const int PicSizeX = dst.columns();

		int i= 0,j = 0;
		double ySrc = 0, xSrc = 0;
		double x  = 0 ,y = 0;
		double normY ,normX = 0;

		//Bilddurchlauf
		for(y=0; y<PicSizeY; y++)
		{
			for(x=0; x<PicSizeX; x++)
			{
				for (j = 0; j < 4; j++)
				{
					normX = x/PicSizeX;
					normY = y/PicSizeY;
					xSrc += phi(j,normX, normY)* points.X[j];
					ySrc += phi(j,normX, normY)* points.Y[j];
				}
				
				// hier funktionspointer
				//std::cout << "x source:" << xSrc << ";" << "y source"  << ySrc<< std::endl;
				i = (this->*interpol)(xSrc, ySrc, src);
				//std::cout << "Calculated Value:" << i << std::endl;
				dst[y][x] = i;
				//std::cout << "Pixel Value:" << dst[y][x] << std::endl;
				xSrc = 0;
				ySrc = 0;
			}
		}
		
		return 0;

	}
	bool RV01::checkBorder(int coordinate, int maxsize){

		bool resu = false;

		if(0 <= coordinate && coordinate < maxsize-1)
			resu = true;

		return resu;
	}

	int RV01::round(double i)
	{
		double kommawert;
		int gerade;
		int out;
		gerade = (int)i;
		kommawert = i-gerade;
		if (kommawert>=0.5)
		{
			out = gerade+1;
		}
		else
		{
			out = gerade;
		}
		return out;
	}

	double RV01::phi(int num, double x, double y)
	{	
		double resu = 0;
		switch (num)
		{
			case 0 : resu = (1-x)*(1-y); break;
			case 1 : resu = x*(1-y); break;
			case 2 : resu = x*y; break;
			case 3 : resu = (1-x)*y; break;
			default : resu = 0; break;
		}
		return resu;
	}
};