// This headerfile was generated automatically by Perl Script "collectHeaders.pl" on 22.10.2008 at 09:27:53
// It includes all headerfiles of the LTI-Lib.


#ifndef _LTI_HEADERS_H_
#define _LTI_HEADERS_H_

#include "ltiExternViewer.h"
#include "ltiFastViewer.h"
#include "ltiGtkColorDialog.h"
#include "ltiGtkServer.h"
#include "ltiGtkWidget.h"
#include "ltiHistogramViewer.h"
#include "ltiHsvHistogramViewer.h"
#include "ltiScene3DViewer.h"
#include "ltiViewer.h"
#include "ltiViewerBase.h"
#include "ltiViewerBase3D.h"
#include "ltiViewerFunctor.h"
#include "ltiGetStreamFd.h"
#include "ltiMutex.h"
#include "ltiProcessInfo.h"
#include "ltiReadWriteLock.h"
#include "ltiSemaphore.h"
#include "ltiSerial.h"
#include "ltiThread.h"
#include "ltiTimer.h"
#include "ltiURL.h"
#include "ltiAbsValue.h"
#include "ltiAddScalar.h"
#include "ltiAdjacencyGraph.h"
#include "ltiArctanLUT.h"
#include "ltiArray.h"
#include "ltiBackSubstitution.h"
#include "ltiBhattacharyyaDistance.h"
#include "ltiBinomialDistribution.h"
#include "ltiBoundaryType.h"
#include "ltiBoundsFunctor.h"
#include "ltiChiSquareFunctor.h"
#include "ltiCholeskyDecomposition.h"
#include "ltiComplex.h"
#include "ltiConstants.h"
#include "ltiContinuousRandDist.h"
#include "ltiCoordinateTransformation.h"
#include "ltiCosinusSimilarity.h"
#include "ltiCubicSpline.h"
#include "ltiCyclicTridiagonalEquationSystem.h"
#include "ltiDelaunayTriangulation.h"
#include "ltiDiscreteRandDist.h"
#include "ltiDistanceFromPixel.h"
#include "ltiDistanceFunctor.h"
#include "ltiDynamicMatrix.h"
#include "ltiEarthMoversDistance.h"
#include "ltiEigenSystem.h"
#include "ltiEntropyFunctor.h"
#include "ltiEquallySpacedSamplesInterpolator.h"
#include "ltiEquationSystem.h"
#include "ltiEuclideanSimilarity.h"
#include "ltiExpDist.h"
#include "ltiFastICA.h"
#include "ltiFastLine.h"
#include "ltiForwardSubstitution.h"
#include "ltiFunctionGenerator.h"
#include "ltiGammaDist.h"
#include "ltiGaussDist.h"
#include "ltiGaussian.h"
#include "ltiGaussianMixtureModel.h"
#include "ltiGeneralizedEigenSystem.h"
#include "ltiGenericMatrix.h"
#include "ltiGenericVector.h"
#include "ltiGeometry.h"
#include "ltiHiddenMarkovModel.h"
#include "ltiHistogram.h"
#include "ltiHmmTrainer.h"
#include "ltiHmmViterbiPathSearch.h"
#include "ltiInterpolator.h"
#include "ltiIteratingFunctor.h"
#include "ltiKdTree.h"
#include "ltiKPCA.h"
#include "ltiL1Distance.h"
#include "ltiL1L2Norm.h"
#include "ltiL2Distance.h"
#include "ltiLine.h"
#include "ltiLinearAlgebraFunctor.h"
#include "ltiLinearMapperFunctor.h"
#include "ltiLinearMixer.h"
#include "ltiLinearRegression.h"
#include "ltiLinearSatMapperFunctor.h"
#include "ltiLogarithm.h"
#include "ltiMahalanobisDistance.h"
#include "ltiMapperFunctor.h"
#include "ltiMaskFunctors.h"
#include "ltiMath.h"
#include "ltiMathFunction.h"
#include "ltiMathObject.h"
#include "ltiMatrix.h"
#include "ltiMatrixDecomposition.h"
#include "ltiMatrixInversion.h"
#include "ltiMeansFunctor.h"
#include "ltiMinimizeBasis.h"
#include "ltiMinimumSpanningTree.h"
#include "ltiMinimumSpanningTreeOfKeytype.h"
#include "ltiMinimumSpanningTreeOfKeyValuetype.h"
#include "ltiMultivariateGaussian.h"
#include "ltiMultScalar.h"
#include "ltiNoise.h"
#include "ltiPCA.h"
#include "ltiPoissonDist.h"
#include "ltiPolynomRoots.h"
#include "ltiPriorityQueue.h"
#include "ltiPseudoInverseMP.h"
#include "ltiQrDecomposition.h"
#include "ltiQuickMedian.h"
#include "ltiQuickPartialSort.h"
#include "ltiRandDist.h"
#include "ltiRegularizedPCA.h"
#include "ltiScalarFunctor.h"
#include "ltiScalarValuedInterpolation.h"
#include "ltiScramble.h"
#include "ltiSequence.h"
#include "ltiSerialPCA.h"
#include "ltiSerialStatsExtFunctor.h"
#include "ltiSerialStatsFunctor.h"
#include "ltiSerialVectorStats.h"
#include "ltiSimilarityFunctor.h"
#include "ltiSinCos.h"
#include "ltiSmallObjectList.h"
#include "ltiSort.h"
#include "ltiSortExpensive.h"
#include "ltiSparseHistogram.h"
#include "ltiSparseMatrix.h"
#include "ltiSqRoot.h"
#include "ltiSquare.h"
#include "ltiStatisticsFunctor.h"
#include "ltiStrassenMultiplication.h"
#include "ltiStudentDistribution.h"
#include "ltiSVD.h"
#include "ltiSymmetricMatrixInversion.h"
#include "ltiTensor.h"
#include "ltiTree.h"
#include "ltiTriangularMatrixType.h"
#include "ltiTridiagonalEquationSystem.h"
#include "ltiTriMatrix.h"
#include "ltiUnifiedGeneralizedEigenSystem.h"
#include "ltiUnifiedSVD.h"
#include "ltiUnifiedSymmetricEigenSystem.h"
#include "ltiUniformDist.h"
#include "ltiValidator.h"
#include "ltiVariablySpacedSamplesInterpolator.h"
#include "ltiVarianceFunctor.h"
#include "ltiVector.h"
#include "ltiWeightedGraph.h"
#include "ltiBinaryStreamHandler.h"
#include "ltiIoHandler.h"
#include "ltiIoObject.h"
#include "ltiLispStreamHandler.h"
#include "ltiSTLIoInterface.h"
#include "ltiALLFunctor.h"
#include "ltiAscii85Codec.h"
#include "ltiAsciiHexCodec.h"
#include "ltiBinarizationCodec.h"
#include "ltiBMPFunctor.h"
#include "ltiCamera.h"
#include "ltiCombinedCodec.h"
#include "ltiConfigFileHandler.h"
#include "ltiDataCodec.h"
#include "ltiDataTransformer.h"
#include "ltiDirectedPerceptionPTU.h"
#include "ltiFireWireDCAM.h"
#include "ltiFlateCodec.h"
#include "ltiFrameGrabber.h"
#include "ltiHardwareConfig.h"
#include "ltiIdentityCodec.h"
#include "ltiIOFunctor.h"
#include "ltiITIFrameGrabber.h"
#include "ltiJPEGFunctor.h"
#include "ltiLeutronFrameGrabber.h"
#include "ltiLncFeatureFile.h"
#include "ltiLoadImageList.h"
#include "ltiLoadVectorImageList.h"
#include "ltiLTIFunctor.h"
#include "ltiMicroEnablePulnix.h"
#include "ltiPanTiltUnit.h"
#include "ltiPNGFunctor.h"
#include "ltiQuickCam.h"
#include "ltiRunLengthCodec.h"
#include "ltiSonyEviD100P.h"
#include "ltiToUCam.h"
#include "ltiUciDataReader.h"
#include "ltiUciDataWriter.h"
#include "ltipngadler32.h"
#include "ltipngcrc32.h"
#include "ltipngdecoder.h"
#include "ltipngdeflateencoder.h"
#include "ltipngdeflatehuffmanencoder.h"
#include "ltipngdeflateoutputstream.h"
#include "ltipngdeflatepvt.h"
#include "ltipngencoder.h"
#include "ltipngendian.h"
#include "ltipngexcept.h"
#include "ltipnginflatedecoder.h"
#include "ltipnginflatehuffmandecoder.h"
#include "ltipnginflateinputstream.h"
#include "ltipnginputbytestream.h"
#include "ltipnginputstream.h"
#include "ltipngoutputbytestream.h"
#include "ltipngoutputstream.h"
#include "ltipngsystemspecific.h"
#include "ltijfif.h"
#include "ltijpdecobk.h"
#include "ltijpdecomp.h"
#include "ltijpdedu.h"
#include "ltijpdehuff.h"
#include "ltijpdequan.h"
#include "ltijpeg.h"
#include "ltijpegdeco.h"
#include "ltijpegenco.h"
#include "ltijpencobk.h"
#include "ltijpencomp.h"
#include "ltijpendu.h"
#include "ltijpenhuff.h"
#include "ltijpenquan.h"
#include "ltijpgexcep.h"
#include "ltijpmarker.h"
#include "ltiActiveShapeModel.h"
#include "ltiAxLocalRegions.h"
#include "ltiAxOGDFeature.h"
#include "ltiBackgroundModel.h"
#include "ltiBayerDemosaicing.h"
#include "ltiBicubicInterpolator.h"
#include "ltiBilinearInterpolator.h"
#include "ltiBinaryKernels.h"
#include "ltiBiquadraticInterpolator.h"
#include "ltiBlobEM.h"
#include "ltiBlueScreen.h"
#include "ltiBorderExtrema.h"
#include "ltiBorderSignature.h"
#include "ltiBoundingBox.h"
#include "ltiBrightRGB.h"
#include "ltiCamshiftTracker.h"
#include "ltiCannyEdges.h"
#include "ltiCartesianToPolar.h"
#include "ltiChannelStatistics.h"
#include "ltiChromaticityHistogram.h"
#include "ltiChrominanceMapEstimator.h"
#include "ltiChrominanceMapMasking.h"
#include "ltiChrominanceModelEstimator.h"
#include "ltiClassicEdgeDetector.h"
#include "ltiCoilBackgroundSegmentation.h"
#include "ltiColorACASegmentation.h"
#include "ltiColorContrastGradient.h"
#include "ltiColorEdgesGS.h"
#include "ltiColorModelEstimator.h"
#include "ltiColorModelSelector.h"
#include "ltiColorNormalization.h"
#include "ltiColorNormalizationBase.h"
#include "ltiColorQuantization.h"
#include "ltiComprehensiveColourNormalization.h"
#include "ltiComputePalette.h"
#include "ltiContour.h"
#include "ltiConvexHull.h"
#include "ltiConvolution.h"
#include "ltiConvolutionHelper.h"
#include "ltiCornerDetector.h"
#include "ltiCornerDetectorFactory.h"
#include "ltiCorrelation.h"
#include "ltiCrossCorrelationCoefficient.h"
#include "ltiCsPresegmentation.h"
#include "ltiCurvatureFeature.h"
#include "ltiCurvatureScaleSpace.h"
#include "ltiCWAGMSegmentation.h"
#include "ltiDecimation.h"
#include "ltiDilation.h"
#include "ltiDistanceTransform.h"
#include "ltiDownsampling.h"
#include "ltiDraw.h"
#include "ltiDraw2DDistribution.h"
#include "ltiDraw3D.h"
#include "ltiDraw3DDistribution.h"
#include "ltiDrawBase.h"
#include "ltiDrawFlowField.h"
#include "ltiEdgeDetector.h"
#include "ltiEdgeDetectorFactory.h"
#include "ltiEdgeSaliency.h"
#include "ltiEpsDraw.h"
#include "ltiErosion.h"
#include "ltiExpandVector.h"
#include "ltiFaceThreshold.h"
#include "ltiFastCircleExtraction.h"
#include "ltiFastEllipseExtraction.h"
#include "ltiFastLineExtraction.h"
#include "ltiFastObjectsFromMask.h"
#include "ltiFastRelabeling.h"
#include "ltiFeatureExtractor.h"
#include "ltiFeatureSaliencyAx.h"
#include "ltiFeatureSaliencyIK.h"
#include "ltiFFTinit.h"
#include "ltiFilledUpsampling.h"
#include "ltiFilter.h"
#include "ltiFlipImage.h"
#include "ltiFMatrixEstimator.h"
#include "ltiFMatrixEstimatorBase.h"
#include "ltiFMatrixSolverLMS.h"
#include "ltiFourierDescriptor.h"
#include "ltiFrankotChellapa.h"
#include "ltiGaborKernels.h"
#include "ltiGaborPyramid.h"
#include "ltiGaussianPyramid.h"
#include "ltiGaussKernels.h"
#include "ltiGenericInterpolator.h"
#include "ltiGeometricFeatureGroup.h"
#include "ltiGeometricFeatures.h"
#include "ltiGeometricFeaturesFromMask.h"
#include "ltiGeometricTransform.h"
#include "ltiGHoughTransform.h"
#include "ltiGlobalFeatureExtractor.h"
#include "ltiGradientASM.h"
#include "ltiGradientFunctor.h"
#include "ltiGradientKernels.h"
#include "ltiGraphicsPattern.h"
#include "ltiGrayWorldNormalization.h"
#include "ltiGuyMedioniSaliency.h"
#include "ltiHarrisCorners.h"
#include "ltiHessianFunctor.h"
#include "ltiHessianKernels.h"
#include "ltiHistogramEqualization.h"
#include "ltiHistograming1D.h"
#include "ltiHistogramRGBL.h"
#include "ltiHmmTrellisDiagram.h"
#include "ltiHomography8DofEstimator.h"
#include "ltiHomography9DofEstimator.h"
#include "ltiHomographyEstimatorBase.h"
#include "ltiHomographyEstimatorFactory.h"
#include "ltiHTypes.h"
#include "ltiHuMoments.h"
#include "ltiImage.h"
#include "ltiInterpolatorType.h"
#include "ltiIsotropicNormalization.h"
#include "ltiKalmanFilter.h"
#include "ltiKalmanTracker.h"
#include "ltiKernelType.h"
#include "ltiKMColorQuantization.h"
#include "ltiKMeansSegmentation.h"
#include "ltiKNearestNeighFilter.h"
#include "ltiKullbackContrast.h"
#include "ltiLabelAdjacencyMap.h"
#include "ltiLaplacianKernel.h"
#include "ltiLaplacianPyramid.h"
#include "ltiLinearFilter.h"
#include "ltiLinearKernels.h"
#include "ltiLinearRegressionTracking.h"
#include "ltiLkmColorQuantization.h"
#include "ltiLkTracker.h"
#include "ltiLocalColorFeature.h"
#include "ltiLocalFeatureExtractor.h"
#include "ltiLocalMaxima.h"
#include "ltiLocalMoments.h"
#include "ltiLocation.h"
#include "ltiLocationSelector.h"
#include "ltiLoweGradientFeature.h"
#include "ltiMaskImage.h"
#include "ltiMaximumFilter.h"
#include "ltiMeanShiftSegmentation.h"
#include "ltiMeanshiftTracker.h"
#include "ltiMedianCut.h"
#include "ltiMedianFilter.h"
#include "ltiMergeChnl.h"
#include "ltiMergeCIELuvToImage.h"
#include "ltiMergeHLSToImage.h"
#include "ltiMergeHSIToImage.h"
#include "ltiMergeHSVToImage.h"
#include "ltiMergeImage.h"
#include "ltiMergeOCPToImage.h"
#include "ltiMergeRGBToImage.h"
#include "ltiMergergIToImage.h"
#include "ltiMergexyYToImage.h"
#include "ltiMergeXYZToImage.h"
#include "ltiMergeYIQToImage.h"
#include "ltiMergeYUVToImage.h"
#include "ltiModifier.h"
#include "ltiMonteCarloEstimator.h"
#include "ltiMorphology.h"
#include "ltiMulticlassNormalizedCuts.h"
#include "ltiMultiGeometricFeaturesFromMask.h"
#include "ltiNearestNeighborInterpolator.h"
#include "ltiNonMaximaSuppression.h"
#include "ltiObjectsFromMask.h"
#include "ltiOgdFilter.h"
#include "ltiOgdKernels.h"
#include "ltiOpponentColor.h"
#include "ltiOpticalFlowHS.h"
#include "ltiOpticalFlowLK.h"
#include "ltiOptimalThresholding.h"
#include "ltiOrientationFeature.h"
#include "ltiOrientationMap.h"
#include "ltiOrientedHLTransform.h"
#include "ltiOverlappingSets2D.h"
#include "ltiPatternDraw.h"
#include "ltiPdmGenerator.h"
#include "ltiPointDistributionModel.h"
#include "ltiPointList.h"
#include "ltiPointSetNormalization.h"
#include "ltiPointSetNormalizationType.h"
#include "ltiPolarToCartesian.h"
#include "ltiPolygonApproximation.h"
#include "ltiPolygonPoints.h"
#include "ltiProbabilityMap.h"
#include "ltiProbabilityMap2D.h"
#include "ltiProbabilityMapBase.h"
#include "ltiPyramid.h"
#include "ltiPyramidLocationSearch.h"
#include "ltiQmf.h"
#include "ltiQmfEnergy.h"
#include "ltiQmfInverse.h"
#include "ltiQmfKernels.h"
#include "ltiQuadTreeSegmentation.h"
#include "ltiRansacEstimator.h"
#include "ltiRealFFT.h"
#include "ltiRealInvFFT.h"
#include "ltiRegionGraphFunctor.h"
#include "ltiRegionGraphMeans.h"
#include "ltiRegionGrowing.h"
#include "ltiRegionMerge.h"
#include "ltiRegionShapeFeatures.h"
#include "ltiRegionsPolygonizer.h"
#include "ltiRelativeThresholding.h"
#include "ltiRobustEstimator.h"
#include "ltiRobustEstimatorType.h"
#include "ltiRotation.h"
#include "ltiSaliency.h"
#include "ltiScalarValuedInterpolatorFactory.h"
#include "ltiScaleSpacePyramid.h"
#include "ltiScaling.h"
#include "ltiScene3D.h"
#include "ltiSchieleCrowley6DFeature.h"
#include "ltiSecondDerivativeKernels.h"
#include "ltiSegmentation.h"
#include "ltiSelective2DConvolution.h"
#include "ltiShapeFeatureExtractor.h"
#include "ltiShiftInvariance.h"
#include "ltiSimilarityMatrix.h"
#include "ltiSkeleton.h"
#include "ltiSkinASM.h"
#include "ltiSkinProbabilityMap.h"
#include "ltiSnake.h"
#include "ltiSplitImage.h"
#include "ltiSplitImageFactory.h"
#include "ltiSplitImageToCIELuv.h"
#include "ltiSplitImageToGSC.h"
#include "ltiSplitImageToGSL.h"
#include "ltiSplitImageToHLS.h"
#include "ltiSplitImageToHSI.h"
#include "ltiSplitImageToHSV.h"
#include "ltiSplitImageToOCP.h"
#include "ltiSplitImageToRGB.h"
#include "ltiSplitImageTorgI.h"
#include "ltiSplitImageToxyY.h"
#include "ltiSplitImageToXYZ.h"
#include "ltiSplitImageToYIQ.h"
#include "ltiSplitImageToYUV.h"
#include "ltiSplitImg.h"
#include "ltiSquareConvolution.h"
#include "ltiSusan.h"
#include "ltiSusanCorners.h"
#include "ltiSusanDenoise.h"
#include "ltiSusanEdges.h"
#include "ltiTemporalTemplate.h"
#include "ltiThresholding.h"
#include "ltiThresholdSegmentation.h"
#include "ltiTransform.h"
#include "ltiTransformEstimator.h"
#include "ltiTransformEstimatorFactory.h"
#include "ltiTransformEstimatorType.h"
#include "ltiTranslationScaleEstimator.h"
#include "ltiTriangularKernels.h"
#include "ltiUpsampling.h"
#include "ltiUsePalette.h"
#include "ltiWatershedSegmentation.h"
#include "ltiWhiteningSegmentation.h"
#include "ltiAdaptiveKMeans.h"
#include "ltiBagging.h"
#include "ltiBhattacharyyaDistOfSubset.h"
#include "ltiCentroidClustering.h"
#include "ltiClassificationStatistics.h"
#include "ltiClassifier.h"
#include "ltiClassifier2DVisualizer.h"
#include "ltiClustering.h"
#include "ltiClusteringValidity.h"
#include "ltiCombination.h"
#include "ltiCompetitiveAgglomeration.h"
#include "ltiCostFunction.h"
#include "ltiCrispDecisionTree.h"
#include "ltiCrossValidator.h"
#include "ltiDBScan.h"
#include "ltiDecisionTree.h"
#include "ltiFeatureSelector.h"
#include "ltiFuzzyCMeans.h"
#include "ltiHmmClassifier.h"
#include "ltiHmmOnlineClassifier.h"
#include "ltiKernelFunctor.h"
#include "ltiKMeansClustering.h"
#include "ltiKNNClassifier.h"
#include "ltiLinearKernel.h"
#include "ltiLvq.h"
#include "ltiMahalanobisDistOfSubset.h"
#include "ltiManualCrispDecisionTree.h"
#include "ltiMLP.h"
#include "ltiMSTClustering.h"
#include "ltiPlusLTakeAwayR.h"
#include "ltiPolynomialKernel.h"
#include "ltiProgressInfo.h"
#include "ltiRadialKernel.h"
#include "ltiRbf.h"
#include "ltiRecognitionRateOfSubset.h"
#include "ltiSammonsMapping.h"
#include "ltiSequentialBackwardSearch.h"
#include "ltiSequentialForwardSearch.h"
#include "ltiSffs.h"
#include "ltiSHClassifier.h"
#include "ltiSigmoidKernel.h"
#include "ltiSOFM.h"
#include "ltiSOFM2D.h"
#include "ltiSOFM2DVisualizer.h"
#include "ltiStdLayer.h"
#include "ltiSupervisedInstanceClassifier.h"
#include "ltiSupervisedInstanceClassifierFactory.h"
#include "ltiSupervisedSequenceClassifier.h"
#include "ltiSVM.h"
#include "ltiUnsupervisedClassifier.h"
#include "ltiAssert.h"
#include "ltiBox.h"
#include "ltiClassName.h"
#include "ltiCMYKColor.h"
#include "ltiColors.h"
#include "ltiConfig.h"
#include "ltiDebug.h"
#include "ltiDistanceType.h"
#include "ltiException.h"
#include "ltiFunctor.h"
#include "ltiMacroSymbols.h"
#include "ltiObject.h"
#include "ltiObjectFactory.h"
#include "ltiPerformanceConfig.h"
#include "ltiPoint.h"
#include "ltiRectangle.h"
#include "ltiRGBPixel.h"
#include "ltiStatus.h"
#include "ltiTypeInfo.h"
#include "ltiTypes.h"
#include "ltiUndebug.h"
#include "ltiWinConfig.h"
#include "ltiFastEigenSystem.h"
#include "ltiFastGeneralizedEigenSystem.h"
#include "ltiFastSVD.h"
#include "ltiGenEigVectors.h"
#include "ltiLapackInterface.h"
#include "ltiLDA.h"
#include "ltiSerialLDA.h"

#endif
