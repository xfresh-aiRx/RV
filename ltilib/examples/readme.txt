
This directory contains several examples for using the LTIlib from a
non-LTIlib program.
At present, only Makefiles are available for compiling them under
Linux. MSVC project files will be added soon.

List of examples

- bitmap2eps
  Convert bitmap files to eps using the epsDraw class.

- collage
  Create an image consisting in a collage of other images

- colorquant
  Simple example that quantizes an color images a shows the found
  clusters in the RGB color space
  
- distort
  Useful tool for generating test pictures for pattern recognition
  programs. It takes a list of input pictures, generates distorted
  copies and saves them on disk.

- fader 
  Another useful tool which maps a grayscale image to a color
  scale along any axis in RGB space

- ltiview
  A console application that expects a JPEG, PNG or BMP file name and 
  starts the lti::viewer to show it.

- secucam
  Small kind of security application, that gets images from a ToUCam
  camera and saves it only when some motion is detected.

- setgen
  Generator for 2D Training sets.  It gets the coordinates from some
  points in an image an constructs training matrices.

Templates

  The template directory contains an example makefile that you can use
  in your own examples/projects.

Building the examples

  Just type:

  make all

Other make targets:

  make clean      deletes all but the executables
  make clean-all  deletes everything except the sources and Makefiles
  make copymake   copy the Makefile in template/ into all examples
  
