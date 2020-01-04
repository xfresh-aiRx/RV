This is a tool to visualize the classification performance of
various classifiers on 2D data.

Usage: classifierViz [-c <classifier>] [-p <paramFile>]
                     [-k <kernelType>] [-s <kernelParam>]
                     [-v <vizParam>]
                     [-w <width>] [-h <height>]
                     [-r <[x1 x2 y1 y2]>] <filename>
       classifierViz -d
       classifierViz --help

--help print help

-d
writes all possible parameters files with filenames "def<TYPE>.par". All
other parameters including the filename are ignored when this option is
given.

-c
<classifier> is a lti::supervisedInstanceClassifier that takes sets
             of dvectors and integer ids as training data. Currently
             the following classifiers are availabele in the LTI-Lib:
  MLP        Multi-Layer Perceptron.
  RBF        Radial Basis Function network
  SVM        Support Vector Machine
The default is RBF.

-p
<paramFile> is a parameters file of the classifier type given
            by <classifier>.

-k
<kernelType> if <classifier> is SVM this parameter sets the kernel type.
             It overrides the kernel given in a parameters file.
             The following kernels are available:
  Linear     Linear kernel.
  Polynomial Polynomial kernel.
  Radial     Radial (RBF) kernel.
  Sigmoid    Sigmoid kernel.
The default is Linear.

-s
<kernelParam> is a scalar parameter for the SVM kernel. If a Radial kernel
              is used it is the variance. For Polynomial its the degree.
              This value is interpreted only if the <kernelType> is set
              manually. Defaults are 0.5 and 2 for the above kernels.

-v
<vizParam> A parameters file of the lti::classifier2DVisualization functor.

-w
<width> Width of the image to be used for visualization. Overrides value
        in the vizParam file. Default 400.

-h
<height> Height of the image to be used for visualization. Overrides value
        in the vizParam file. Default 400.

-r
<[x1 y1 x2 y2]> Range the x and y values in the data file can take.
                Overrides value in the vizParam file. Default [-1 -1 1 1].

filename An ASCII file that contains a double data matrix with the label
         inputs and ids with the label ids. The example setgen creates
         such files. They are easily created with
         lti::write(handler,"inputs",data);
         lti::write(handler,"ids",ids);
         where handler is a file handle, data and ids as expected.
