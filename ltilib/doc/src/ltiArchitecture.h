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

#ifndef _LTI_ARCHITECTURE
#define _LTI_ARCHITECTURE

/** 

\page architecture Architecture


The LTI-Lib is easy to use due to the specification of a well-defined
programming interface for all classes.  The preservation of its
consistency is partially achieved through the use of the
PERL-script \c ltiGenerator.  Based on a few rudimentary
data provided by the programmer (like class name and parent class)
this script builds some template files, containing all standard
definitions (see \ref howtonew).  After that, only the functionality
needs to be implemented.  This chapter explains all basic concepts
required to understand the meaning of these classes.

\section arcFunctor Functors, parameters and states

Most algorithms require \e parameters, i.e. user defined values that
modify its behavior.  For example, the file name is a parameter of an
image loader class, or the size of a filter kernel is a parameter of a
convolution algorithm.

All algorithms in the LTI-Lib are encapsulated in so-called \e functor classes.
They always enclose a class called \c parameters, that
can be explicitely declared or just inherited from the parent class.

This means, when you use the LTI-Lib you do not call some functions or
class methods with lots of confusing arguments, some meaning input
data, others the output data, and additionally a (maybe too long) list
of parameters for the algorithm.  A default \c parameters object is
usually set in the functor class, and the methods to call the
functionality expect only the input data and the output objects where
the results are going to be written.  You can of course change the
used parameters as you wish, in order to fit the functor's
functionality to your own needs.

Example:

\code
lti::cannyEdges canny;           // an ltilib object to extract the edges
                                 // of a channel
canny.apply(myChannel,myEdges);  // extract the edges from myChannel, 
                                 // and leave them in myEdges using a
                                 // canny functor.  (default parameters used).

// now we want to do a similar task, but changing Canny's default parameters

lti::cannyEdges::parameters cannyParams;  // our parameters object
cannyParams.variance = 5;        // change the variance used
canny.setParameters(cannyParams);// and set the new parameters
canny.apply(myChannel,myEdges);  // now extract the new edges from myChannel, 
                                 // and leave them in myEdges again.
\endcode

The parameters of a functor have to be distinguished from its state, which
consists of all those attributes of the class that are computed 
during the execution the algorithm, but are not directly required by the user.
For the Motion History Images (lti::temporalTemplate) for
example, the last presented image must be kept in order to compute the next
iteration.  This image is not a parameter, but a part of the functor's state.
These concepts are shown following image:

\image html functor.png 

\n

The user can change the behavior of the functor through the
parameters.  The functor can also have a state, that eventually (like
the parameters) can also be saved.

There are several reasons for an independent lti::functor::parameters
class.  You can create several instances with different value sets and
change at once the functionality of your functor with a simple \c
setParameters().  You can load and save your parameters object in a
file, or can give it to a graphical user interface where the user can
choose between several values.  The parameters-classes can also provide
methods to find special configurations of parameters, or to check if the
given values fulfill several conditions imposed by the algorithms.
The parameters contain values directly specified by the user and they
are never modified by the algorithms themselves.

An usual question is: why do I need to call the method \c getParameters()
to get the parameters instance? would it not be faster if each functor-class
had its own parameters instance that could be used directly?.

\image html paramhier.png

\n

The answer relies partially on memory management issues.  It would be very
expensive if all classes in the functor hierarchy would have an own instance
of parameters, because all inherited parameter attributes would be present
several times.  With the functor hierarchy shown on the left side of
the previous figure an instance of the functor
lti::optimalThresholding would have two parameter objects: the one of
its own with five attributes (\c precision  and the four attributes of the
parent class) and the parameters-instance of lti::thresholdSegmentation
with its four attributes.  In other words, the four attributes of the parent
class are present twice!

To avoid this problem, there is only one instance of the parameters in
the lti::functor class.  Each class casts this instance to the proper
parameters type using the overloaded method \c getParameters().

Another important reason for the use of just one parameters-instance
in the functor class appears when the inherited class calls methods
of the parent classes, the later ones could not see the proper
parameters-instance but only the own one, which could contain other values
than those specified by the user.

The functionality of a functor is always accessed by the methods
\c apply().  They expect input data, (usually \c const references to
objects like matrices of images), and references to output objects
(references to containers where the result is written).  Other
functor methods (the shortcuts) provide comfortable ways to access specific
functionality.  To load an image file, for example, image loaders
provide the shortcut \c load that expects a file name and the image
where the read image result should be written.  Otherwise, you would require to
create a parameters object, set there the file name, give this
parameters-instance to the functor, and at last call the apply
method:

\code
  // an image
  lti::image img;

  // functor to load images in Windows BMP format:
  lti::loadBMP loader;

  // parameters for the loader
  lti::loadBMP::parameters loaderParam;

  // the file to load
  loaderParam.filename = "testimage.bmp";

  // load the image into img
  loader.setParameters(loaderParam);
  loader.apply(img); // load the image!
\endcode

It is much easier and comfortable to employ following shortcut:

\code
   // an image
  lti::image img;

  // functor to load images with Windows BMP format:
  lti::loadBMP loader;

  // load an image
  loader.load("testimage.bmp",img);
\endcode

All functors nevertheless provide an interface based on a
\c parameters -object and \c apply -methods, in order to warrant more complex
applications a uniform way to access the functionality of the functor.

There exist just one \c getParameters() -method per functor and it returns a
\e constant reference to the parameters-instance, this due to the fact
that the attributes of the parameters instance should not be changed by the 
functor itself.  

Besides the parameters, a functor may have a state, where it stores
information irrelevant for the user but necessary for later computations.
An example for a functor with separated state and parameters is the
lti::principalComponents (PCA) object.  Here you find a parameter
\c autoDim which indicates that another parameter
\c resultDim  should be detected automatically.  In the \c apply method
the value of \c resultDim stored in the internal parameters-instance remains 
unchanged, to respect the wishes of the users.
The PCA computes a transformation matrix that is part of
the functor's state.  It is used later to linearly transform other vectors.  
This matrix is not something that the user usually gives directly, but after
being computed, it can be saved and loaded together with other parts of the 
functor's state (this is done when you load or save the whole functor).
In general, all functors with a state
relevant for later computations can be saved and loaded, i.e. they overload
the methods \c read and \c write.

\section arcIo Input and Output in the LTI-lib

Serializable objects in the LTI-Lib (i.e. objects that can be written
or read from disk) never directly use \c std::fstream objects.  The
main reason is that we need to provide a way to support different file
formats at the same time.  The desired file format is determined
through a so called lti::ioHandler.  At this time there are two
file formats.  A Lisp-like one (lti::lispStreamHandler) writes or reads ASCII 
strings in or from a given stream, where different scopes are delimited with
parenthesis.  A binary format (lti::binaryHandler) produces shorter files 
and is faster to be read or written, but can not be edited by hand.

A uniform way to load or save LTI-Lib-objects and internal types (\c int ,
\c float , \c double , \c std::string, etc.) is provided through
four global functions that passes them properly to a given lti::ioHandler.
These are:

\code
  bool lti::write(ioHandler& handler, 
                  const T& data);

  bool lti::read(ioHandler& handler,
                 T& data);


  bool lti::write(ioHandler& handler, 
                  const std::string& name,
                  const T& data,
                  const bool complete=true);

  bool lti::read(ioHandler& handler, 
                 const std::string& name,
                 T& data,
                 const bool complete=true);
\endcode

The first two functions write or read the contents of an object of type
\c T  in or from the given \c ioHandler .  The third and fourth methods
write the data together with a name that identifies this object.  To read
the data, the given name must match the one used when the data was saved.

With a handler of type lti::lispStreamHandler following lines
\code
  lti::write(handler,"a",5);
  lti::write(handler,"b",9);
\endcode

produce the following output in the output stream associated with the handler:
\code
    (a 5)
    (b 9)
\endcode

The parenthesis around each line can be left out if the fourth parameter of
the functions (\c complete ) is set to \c false .  Note that the default
value for this parameter is \c true.

The lti::lispStreamHandler can find an object using its name:
\code
  int x,y;
  lti::read(handler,"a",x);
  lti::read(handler,"b",y);
\endcode

After these lines it applies \c x==5 and \c y==9.  Some 
\c ioHandler  (for example lti::binaryStreamHandler ) require that the
read order for different data matches the one used when writing.  If this is
not true, the read methods will return \c false .  Other \c ioHandler 
(like lti::lispStreamHandler ) search for the data using the given name as
key, so that you can use a different reading order.  Following lines would
also result in  \c x==5 and \c y==9:

\code
  int x,y;
  lti::read(handler,"b",y);
  lti::read(handler,"a",x);
\endcode

The \c ioHandler  concept makes it possible to define new file formats
\e without requiring to reimplement all read and write methods of the
LTI-Lib classes.  Due to the fact that the read and write methods use a
relative rigorous syntax, it is also relative simple to parse the files.

Please note that the variables used in the previous examples could also have
any other type defined in the LTI-Lib.   All numerical standard types
(\c int , \c double, etc.), the Standard Template Library (STL) types 
\c std::vector, \c std::list and \c std::map (if you include the
file "\c ltiSTLIoInterface.h") and the most LTI-Lib functors,
parameters and data structures can be serialized.

\subsection ioExample Example

How can I save and load some parameters in my program?

\code
  // ltilib functors and their parameters
  lti::csPresegmentation segmentor;
  lti::csPresegmentation::parameters segParam;

  lti::orientationFeature orientor;
  lti::orientationFeature::parameters orientParam;

  // ... initialize the parameters ...

  // how can we write the parameters in a file named "param.txt"?
  lti::lispStreamHandler handler;  // the stream handler
  std::ofstream out("param.txt");  // the std::fstream used

  // if the output stream is ok, write the data
  if (out) {
    // the handler have to write the data using the stream "out":
    handler.use(out);
    
    // write the parameters:
    lti::write(handler,"orientParam",orientParam); 
    lti::write(handler,"segmentParam",segParam);
    lti::write(handler,"anInteger",5);
  }
\endcode

And how can we read the data from "param.txt"?

\code
  std::ifstream in("param.txt");

  if (in) {
    int x;

    handler.use(in);
    
    // read the data
    lti::read(handler,"orientParam",orientParam); 
    lti::read(handler,"segmentParam",segParam);
    lti::read(handler,"anInteger",x);
  }
\endcode

You can find a hierarchical list of many functors in the LTI-Lib in the
section \ref functors.

\section arcViewdraw Visualization Classes

Not everything in an image processing or computer vision library can be
considered as functor.  Examples for this are the so called drawing and
visualization objects.

Drawing objects does not execute one algorithm.  They provide different tools
to draw simple geometric constructs on images or other output media.  To use a
drawing object you need to provide it with your \e canvas, i.e. you need to
specify the image where you want to draw.  This is done with the method
\c use().  After that, you can choose the color you want to use with the
method \c setColor().  All lines, circles or points you draw after this,
will be painted using the given color.

Following example draws a circle and a line on a color image:

\code
  lti::image img(256,256);  // our canvas

  lti::draw<rgbPixel> drawing;  // drawing tool
  
  drawing.use(img);                         // where should "drawing" paint on?
  drawing.setColor(lti::Blue);                  // Blue color
  drawing.circle(lti::point(128,128),20,true)); // filled circle, radius 20
  drawing.setColor(lti::Red);                   // Red color
  drawing.line(10,10,128,128);                  // A red line
\endcode

Viewer objects do not modify any data, but provide simple ways to visualize
them.  The presentation of the data persists as long as the viewer object
exists.

You can show the previously drawn image with following code:

\code
  lti::viewer viewer("This is art");  // our viewer object
  viewer.show(canvas);                // show our master piece
  getchar();                          // just wait 
\endcode

Available viewers, drawing objects and other visualization tools in the LTI-Lib
are summarized in the section \ref viewers

\section arcClassifiers Classifiers

Other important objects that do not fit into the functor paradigm
are the classifiers.  They provide methods to learn from data, and to use the
learned information to analyze new data.  There are different interfaces
for the supervised and unsupervised classifiers.  Both types can be categorized
into instance classifiers that learn from single vectors (like traditional
neural networks) and sequence classifiers that also considered time aspects
(like Hidden Markov Models).

In the LTI-Lib all classifiers deliver the results using the same data
structures lti::classifier::outputVector, so that the processing of
their results does not depend on the specific classifier used.

More information about the classifier classes can be found in the sections
\ref docuClassifiers and \ref classifiers.

*/

#endif

