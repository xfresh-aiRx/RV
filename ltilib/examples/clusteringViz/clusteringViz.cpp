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

/*
 * This is a tool to visualize the clustering performance of various
 * clustering algorithms on 2D or 3D data. The data can be given
 * either in the format of the setgen tool of that example or in UCI
 * format (each row contains one data sample, space separated, the
 * first row of the file states the number of points, the second the
 * dimensionality).
 *
 * To stop the programm left click on the 2D viewer or enter any
 * character in the console for the 3D viewer.
 * 
 * Usage: clusteringViz [-c <clustering>] [-p <paramFile>] 
 *                      [-n <nbClusters>] [-v <vizParam>]
 *                      [-w <width>] [-h <height>]
 *                      {-U, -S} <filename>
 *        clusteringViz -d
 *        clusteringViz --help
 *
 * --help print help
 *
 * -d
 * writes all possible parameters files with filenames "def<TYPE>.par". All
 * other parameters including the filename are ignored when this option is 
 * given.
 *
 * -c 
 * <clustering> is a lti::clustering that takes sets of dvectors as training
 *              data. Currently the following clustring methods are available
 *              in the LTI-Lib:
 *   KM         k-Means
 *   FCM        Fuzzy-C-Means
 *   AKM        Adaptive-k-Means
 *   DBScan     DBScan
 *   MST        Minimum Spanning Tree Clustering
 * The default is KM.
 *
 * -p
 * <paramFile> is a parameters file of the clustering type given 
 *             by <clustering>.
 *
 * -n
 * <nbClusters> The first two clustering methods need a given number of
 *              clusters. Default is 3.
 *
 * -v
 * <vizParam> A parameters file for the drawing method used. If the data is 2D
 *            it must be a draw2DDistribution::parameters file. If the data is
 *            3D it must be a draw3DDistribution::parameters file.
 *
 * -w
 * <width> Width of the image to be used for visualization of 2D data. 
 *         Default 400.
 *
 * -h
 * <height> Height of the image to be used for visualization of 2D data. 
 *          Default 400.
 *
 * -U
 * -S       It is mandatory to specify whether UCI data (-U), or 
 *          setgen data (-S) is used.
 *
 * filename Either a setgen or UCI file. In the first case it is an ASCII file
 *          that contains a double data matrix with the label 
 *          "inputs" and ids with the label "ids" which are ignored. Such files
 *          can be created manually by 
 *          lti::write(handler,"inputs",data);
 *          lti::write(handler,"ids",ids);
 *          where handler is a file handle, data and ids as expected.
 *          UCI files contain one data point per row, each value separated by
 *          a space. There is a two line header. First line has an integer with
 *          the number of points, the second line the dimensionality.
 */

#include "ltiRGBPixel.h"
#include "ltiClustering.h"
#include "ltiKMeansClustering.h"
#include "ltiFuzzyCMeans.h"
#include "ltiAdaptiveKMeans.h"
#include "ltiDBScan.h"
#include "ltiMSTClustering.h"

#include "ltiImage.h"
#include "ltiViewer.h"
#include "ltiScene3DViewer.h"
#include "ltiDraw.h"
#include "ltiScene3D.h"
#include "ltiDraw2DDistribution.h"
#include "ltiDraw3DDistribution.h"

#include "ltiL2Distance.h"
#include "ltiGtkServer.h"
#include "ltiTimer.h"
#include "ltiLispStreamHandler.h"
#include "ltiUciDataReader.h"

#include <fstream>
#include <string>
#include <iostream>
#include <cstring>

using std::cerr;
using std::cout;
using std::endl;

enum eClusteringType {
  KM, FCM, AKM, DBScan, MST};

enum eFileType {
  None, SG, UCI};

static lti::draw<lti::rgbPixel> _my_draw;
static lti::scene3D<lti::rgbPixel> _my_scene;
static lti::draw2DDistribution<lti::rgbPixel> _my_draw2D;
static lti::draw3DDistribution _my_draw3D;
static lti::viewer _my_viewer2D;
static lti::scene3DViewer _my_viewer3D;

void printHelp() {

  cout
 <<" This is a tool to visualize the clustering performance of various\n"
 <<" clustering algorithms on 2D or 3D data. The data can be given\n"
 <<" either in the format of the setgen tool of that example or in UCI\n"
 <<" format (each row contains one data sample, space separated, the\n"
 <<" first row of the file states the number of points, the second the\n"
 <<" dimensionality).\n"
 <<"\n"
 <<" To stop the programm left click on the 2D viewer or enter any\n"
 <<" character in the console for the 3D viewer.\n"
 <<" \n"
 <<" Usage: clusteringViz [-c <clustering>] [-p <paramFile>] \n"
 <<"                      [-n <nbClusters>] [-v <vizParam>]\n"
 <<"                      [-w <width>] [-h <height>]\n"
 <<"                      {-U, -S} <filename>\n"
 <<"        clusteringViz -d\n"
 <<"        clusteringViz --help\n"
 <<"\n"
 <<" --help print help\n"
 <<"\n"
 <<" -d\n"
 <<" writes all possible parameters files with filenames \"def<TYPE>.par\". All\n"
 <<" other parameters including the filename are ignored when this option is \n"
 <<" given.\n"
 <<"\n"
 <<" -c \n"
 <<" <clustering> is a lti::clustering that takes sets of dvectors as training\n"
 <<"              data. Currently the following clustring methods are available\n"
 <<"              in the LTI-Lib:\n"
 <<"   KM         k-Means\n"
 <<"   FCM        Fuzzy-C-Means\n"
 <<"   AKM        Adaptive-k-Means\n"
 <<"   DBScan     DBScan\n"
 <<"   MST        Minimum Spanning Tree Clustering\n"
 <<" The default is KM.\n"
 <<"\n"
 <<" -p\n"
 <<" <paramFile> is a parameters file of the clustering type given \n"
 <<"             by <clustering>.\n"
 <<"\n"
 <<" -n\n"
 <<" <nbClusters> The first two clustering methods need a given number of\n"
 <<"              clusters. Default is 3.\n"
 <<"\n"
 <<" -v\n"
 <<" <vizParam> A parameters file for the drawing method used. If the data is 2D\n"
 <<"            it must be a draw2DDistribution::parameters file. If the data is\n"
 <<"            3D it must be a draw3DDistribution::parameters file.\n"
 <<"\n"
 <<" -w\n"
 <<" <width> Width of the image to be used for visualization of 2D data. \n"
 <<"         Default 400.\n"
 <<"\n"
 <<" -h\n"
 <<" <height> Height of the image to be used for visualization of 2D data. \n"
 <<"          Default 400.\n"
 <<"\n"
 <<" -U\n"
 <<" -S       It is mandatory to specify whether UCI data (-U), or \n"
 <<"          setgen data (-S) is used.\n"
 <<"\n"
 <<" filename Either a setgen or UCI file. In the first case it is an ASCII file\n"
 <<"          that contains a double data matrix with the label \n"
 <<"          \"inputs\" and ids with the label \"ids\" which are ignored. Such files\n"
 <<"          can be created manually by \n"
 <<"          lti::write(handler,\"inputs\",data);\n"
 <<"          lti::write(handler,\"ids\",ids);\n"
 <<"          where handler is a file handle, data and ids as expected.\n"
 <<"          UCI files contain one data point per row, each value separated by\n"
 <<"          a space. There is a two line header. First line has an integer with\n"
 <<"          the number of points, the second line the dimensionality.\n";
  
}

void dumpParameters() {

  lti::kMeansClustering::parameters kmp;
  lti::fuzzyCMeans::parameters fcmp;
  lti::adaptiveKMeans::parameters akmp;
  lti::DBScan< lti::l2SquareDistantor<lti::dvector> >::parameters dbp;
  lti::MSTClustering< lti::l2SquareDistantor<lti::dvector> >::parameters mstp;
  lti::draw2DDistribution<lti::rgbPixel>::parameters d2p;
  lti::draw3DDistribution::parameters d3p;

  std::ofstream of;
  lti::lispStreamHandler lsh;

  of.open("defaultKM.par");
  if (of.good() && of.is_open()) {
    lsh.use(of);
    kmp.write(lsh);
  } else {
    cerr << "Could not write default parmeters file\n";
  }
  of.close();

  of.open("defaultFCM.par");
  if (of.good() && of.is_open()) {
    lsh.use(of);
    fcmp.write(lsh);
  } else {
    cerr << "Could not write default parmeters file\n";
  }
  of.close();

  of.open("defaultAKM.par");
  if (of.good() && of.is_open()) {
    lsh.use(of);
    akmp.write(lsh);
  } else {
    cerr << "Could not write default parmeters file\n";
  }
  of.close();

  of.open("defaultDBScan.par");
  if (of.good() && of.is_open()) {
    lsh.use(of);
    dbp.write(lsh);
  } else {
    cerr << "Could not write default parmeters file\n";
  }
  of.close();

  of.open("defaultMST.par");
  if (of.good() && of.is_open()) {
    lsh.use(of);
    mstp.write(lsh);
  } else {
    cerr << "Could not write default parmeters file\n";
  }
  of.close();

  of.open("defaultViz2D.par");
  if (of.good() && of.is_open()) {
    lsh.use(of);
    d2p.write(lsh);
  } else {
    cerr << "Could not write default parmeters file\n";
  }
  of.close();

  of.open("defaultViz3D.par");
  if (of.good() && of.is_open()) {
    lsh.use(of);
    d3p.write(lsh);
  } else {
    cerr << "Could not write default parmeters file\n";
  }
  of.close();


}

bool initClustering(lti::clustering*& clustering, 
                    const eClusteringType& cType,
                    const std::string& cParam,
                    const int& nbClusters) {

  lti::clustering::parameters* param;
  clustering=0;

  switch (cType) {
    case KM:
      clustering=new lti::kMeansClustering();
      param=new lti::kMeansClustering::parameters();
      break;
    case FCM:
      clustering=new lti::fuzzyCMeans();
      param=new lti::fuzzyCMeans::parameters();
      break;
    case AKM:
      clustering=new lti::adaptiveKMeans();
      param=new lti::adaptiveKMeans::parameters();
      break;
    case DBScan:
      clustering=
        new lti::DBScan< lti::l2SquareDistantor<lti::dvector> >();
      param=
        new lti::DBScan< lti::l2SquareDistantor<lti::dvector> >::parameters();
      break;
    case MST:
      clustering=
        new lti::MSTClustering< lti::l2SquareDistantor<lti::dvector> >();
      param=
        new lti::MSTClustering< lti::l2SquareDistantor<lti::dvector> >::parameters();
      break;
    default:
      cerr << "Unknown clustering type\n";
      return false;
  }

  if (clustering==0) {
    cerr << "Could not instantiate clustering\n";
    return false;
  }
    
  if (cParam.length() != 0) {
    std::ifstream ifs(cParam.c_str());
    if (ifs.good() && ifs.is_open()) {
      lti::lispStreamHandler lsh;
      lsh.use(ifs);
      if (!param->read(lsh)) {
        std::string msg = "Could not read " + std::string(param->getTypeName())
          + " from file " + cParam;
        cerr << msg << endl;
      }
      clustering->setParameters(*param);
      ifs.close();
      delete param;
      param=0;
    } else {
      std::string msg = "Could not open clustering parameters file" + cParam;
      cerr << msg << std::endl;
      return false;
    }
  }

  if (nbClusters>0) {
    switch (cType) {
      case KM: {
        lti::kMeansClustering* km=
          dynamic_cast<lti::kMeansClustering*>(clustering);
        lti::kMeansClustering::parameters par=km->getParameters();
        par.numberOfClusters=nbClusters;
        km->setParameters(par);
        break;
      }
      case FCM: {
        lti::fuzzyCMeans* fcm=dynamic_cast<lti::fuzzyCMeans*>(clustering);
        lti::fuzzyCMeans::parameters par=fcm->getParameters();
        par.nbOfClusters=nbClusters;
        fcm->setParameters(par);
        break;
      }
      default:
        break;
    }
  }
  
  return true;

}

bool getSData(const std::string& filename, lti::dmatrix& data) {

  //dummy
  lti::ivector ids;
  
  std::ifstream ifs(filename.c_str());
  if (ifs.good() && ifs.is_open()) {
    lti::lispStreamHandler lsh;
    lsh.use(ifs);
    if(!lti::read(lsh,"inputs",data)) {
      std::string msg="Error reading data from "+filename;
      cerr << msg << std::endl;
      return false;
    }
    if(!lti::read(lsh,"ids",ids)) {
      std::string msg="Error reading ids from "+filename;
      cerr << msg << std::endl;
      return false;
    }
    ifs.close();
  } else {
    std::string msg="Error opening file "+filename;
    cerr << msg << std::endl;
    return false;
  }

  return true;
}

bool getUData(const std::string& filename, lti::dmatrix& data) {

  lti::uciDataReader uread;
  lti::uciDataReader::parameters up;
  up.hasHeader=true;
  up.hasIds=false;
  up.filename=filename;
  uread.setParameters(up);

  if (!uread.apply(data)) {
    cerr << uread.getStatusString() << endl;
    return false;
  }
  
  return true;
}

bool init2DVisualizerParam(lti::draw2DDistribution<lti::rgbPixel>::parameters& vizParam,
                           const std::string& vParam) {

  if (vParam.length()!=0) {
    std::ifstream ifs(vParam.c_str());
    if (ifs.good() && ifs.is_open()) {
      lti::lispStreamHandler lsh;
      lsh.use(ifs);
      if(!vizParam.read(lsh)) {
        std::string msg="Could not read visualizer parameters from file"
          +vParam;
        cerr << msg << std::endl;
        return false;
      }
      ifs.close();
    } else {
      std::string msg="Could not open file"+vParam;
      cerr << msg << std::endl;
      return false;
    }
  }
    
  return true;
}

bool init3DVisualizerParam(lti::draw3DDistribution::parameters& vizParam,
                           const std::string& vParam) {

  if (vParam.length()!=0) {
    std::ifstream ifs(vParam.c_str());
    if (ifs.good() && ifs.is_open()) {
      lti::lispStreamHandler lsh;
      lsh.use(ifs);
      if(!vizParam.read(lsh)) {
        std::string msg="Could not read visualizer parameters from file"
          +vParam;
        cerr << msg << std::endl;
        return false;
      }
      ifs.close();
    } else {
      std::string msg="Could not open file"+vParam;
      cerr << msg << std::endl;
      return false;
    }
  }
    
  return true;
}

int main(int argc, char* argv[]) {

// parses the command line arguments
  eFileType fType=None;
  std::string filename="";
  eClusteringType cType=KM;
  std::string cParam="";
  int nbClusters;
  std::string vParam="";
  int width=400;
  int height=400;

  for (int i=1; i<argc; i++) {
    if (*argv[i] == '-') {
      switch (argv[i][1]) {
        // dump parametes?
        case 'd':
          dumpParameters();
          lti::passiveWait(10000);
          lti::gtkServer::shutdown();
          exit(0);
          break;
        // print help
        case '-':
          if (strcasecmp(&argv[i][2],"help")==0) {
            printHelp();
            exit(0);
          } else {
            cerr << "Unknown option "<<argv[i]<<std::endl;
          }
          break;
        // clustering
        case 'c':
          if (i < argc-1) {
            ++i;
            if (strcasecmp(argv[i],"km") == 0) {
              cType=KM;
            } else if (strcasecmp(argv[i],"fcm") == 0) {
              cType=FCM;
            } else if (strcasecmp(argv[i],"akm") == 0) {
              cType=AKM;
            } else if (strcasecmp(argv[i],"dbscan") == 0) {
              cType=DBScan;
            } else if (strcasecmp(argv[i],"mst") == 0) {
              cType=MST;
            } else {
              cerr << "Warning: Clustering " << argv[i] 
                        << " is not supported using KM\n";
            }
          } else {
            cerr << "Missing argument to -c\n";
          }
          break;
        // clustering parameters
        case 'p':
          if (i < argc-1) {
            cParam=argv[++i];
          } else {
            cerr << "Missing argument to -p\n";
          }
          break;
        // number of clusters
        case 'n':
          if (i < argc-1) {
            nbClusters=atoi(argv[++i]);
          } else {
            cerr << "Missing argument to -n\n";
          }
          break;
        // visualizer parameters
        case 'v':
          if (i < argc-1) {
            vParam=argv[++i];
          } else {
            cerr << "Missing argument to -v\n";
          }
          break;
        // image width
        case 'w':
          if (i < argc-1) {
            width=atoi(argv[++i]);
          } else {
            cerr << "Missing argument to -w\n";
          }
          break;
        // image height
        case 'h':
          if (i < argc-1) {
            height=atoi(argv[++i]);
          } else {
            cerr << "Missing argument to -h\n";
          }
          break;
        // setgen file
        case 'S':
          if (fType == None) {
            fType=SG;
          } else {
            cerr << "Only setgen OR uci file allowed (-S or -U)" << endl;
            exit(1);
          }
          break;
        // UCI file
        case 'U':
          if (fType == None) {
            fType=UCI;
          } else {
            cerr << "Only setgen OR uci file allowed (-S or -U)" << endl;
            exit(1);
          }
          break;
        // unknown option
        default:
          cerr << "Warning: Unknown option -" << argv[i][1] << std::endl;
          break;
      }
    } else {
      filename=argv[i];
      if (i<argc-1) {
        cerr << "filename must be last argument, ignoring the rest.\n";
        break;
      }
    }
  }
  if (filename.length() == 0) {
    cerr << "You need to give a file name.\n";
    printHelp();
    exit(1);
  }
  if (fType == None) {
    cerr << "You must specify the file type: -U for UCI file or -S for setgen style file\n";
    exit(1);
  }

  lti::clustering* clustering=0;

  if (!initClustering(clustering, cType, cParam, nbClusters)) {
    cerr << "Could not initialize clustering\n";
    exit(1);
  }

  if (clustering==0) {
    cerr << "Clustering not initialized\n";
    exit(1);
  }

  lti::dmatrix data;
  lti::ivector ids;

  switch (fType) {
    case SG:
      if (!getSData(filename,data)) {
        std::string msg="Could not read data from file " + filename;
        cerr << msg << std::endl;
        exit(1);
      }
      break;
    case UCI:
      if (!getUData(filename,data)) {
        std::string msg="Could not read data from file " + filename;
        cerr << msg << std::endl;
        exit(1);
      }
      break;
    case None:
    default:
      cerr << "internal error\n";
      exit(1);
      break;
  }

  if (!clustering->train(data,ids)) {
    cerr << "Could not train clustering on given data\n";
    cerr << clustering->getStatusString() << std::endl;
    exit(1);
  }

  int dim=data.columns();

  switch(dim) {
    case 2: {
      lti::draw2DDistribution<lti::rgbPixel>::parameters vizParam;
      if (!init2DVisualizerParam(vizParam, vParam)) {
        cerr << "Could not initialize visualizer\n";
        exit(1);
      }
      _my_draw2D.setParameters(vizParam);
      lti::image img(width,height);
      _my_draw.use(img);
      _my_draw2D.apply(_my_draw,data,ids);
      _my_viewer2D.show(img);

      _my_viewer2D.waitButtonPressed();
      _my_viewer2D.hide();
      lti::passiveWait(10000);

      break; 
      }
    case 3: {
      lti::draw3DDistribution::parameters vizParam;
      if (!init3DVisualizerParam(vizParam, vParam)) {
        cerr << "Could not initialize visualizer\n";
        exit(1);
      }
      _my_draw3D.setParameters(vizParam);
      _my_scene.reset();
      _my_draw3D.apply(_my_scene,data,ids);
      _my_viewer3D.show(_my_scene);

      char c;
      std::cin >> c; 

      break; 
      }
    default:
      cerr << "Only data with 2 or 3 dimensions allowed, but "
                << dim << " dimensional data supplied.\n";
      exit(1);
      break;
  }

  delete clustering;
  clustering=0;

  lti::gtkServer::shutdown();
  return 0;
}

