/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFuzzyConnectednessImageFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 2001 Insight Consortium
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * The name of the Insight Consortium, nor the names of any consortium members,
   nor of any contributors, may be used to endorse or promote products derived
   from this software without specific prior written permission.

  * Modified source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
#ifndef __itkFuzzyConnectednessImageFilter_h
#define __itkFuzzyConnectednessImageFilter_h

#include "itkImage.h"
#include "itkImageToImageFilter.h"
#include "itkSimpleFuzzyConnectednessImageFilterBase.h"

#include <queue>

namespace itk{

/** /class FuzzyConnectednessRGBImageFilter
 * 
 * Perform the segmentation for a single channel (Grayscale) image 
 * via thresholding of a fuzzy connectedness scene.
 * Used as a node of the segmentation toolkit.
 * Fuzzy affinity is defined between two neighboor pixels, to reflect
 * their similarity and assign a probability that these two pixels belong to the
 * same object.  A "path" between two pixels is a list of pixels that connect
 * them, the strength of a particular path is defined as the weakest affinity
 * between the neighboor pixels that form the path. The fuzzy connectedness
 * between two pixels is defined as the strongest path strength between these
 * two pixels.  The segmentation based on fuzzy connectedness assumes that
 * the fuzzy connectedness between any two pixels from a single object 
 * is significantly higher than those for pixels belonging to different objects.  
 * A fuzzy connectedness scene is first computed for a set of input seed
 * points selected inside the object of interest.  A threshold is then
 * applied to the fuzzy scene to extract the binary segmented object.
 * The fuzzy affinity here was defined as a gaussian function of the pixel difference 
 * and the difference of the estimated object mean and the mean of the two input 
 * pixels. 
 * 
 * Input Parameters are:
 * (1) Input image in the form itkImage
 * (2) Seed points
 * (3) Threshold value.
 * 
 * Usage:
 * 1. use SetInput to import the input image object
 * 2. use SetParameter, SetSeed, SetThreshold to set the parameters
 * 3. run GenerateData() to perform the segmenation
 * 4. threshold can be set using UpdateThreshold after the segmentation, and no computation
 *    will be redo. no need to run GenerateData. But if SetThreshold was used. MakeSegmentObject()
 *    should be called to get the updated result.
 * 5. use GetOutput to obtain the resulted binary image Object.
 * 6. GetFuzzyScene gives the pointer of Image<unsigned short> for the fuzzy scene.
 *
 * Detailed information about this algorithm can be found in:
 *  "Fuzzy Connectedness and Object Definition: Theory, Algorithms,
 *    and Applications in Image Segmentation", J. Udupa and S. Samarasekera
 *  Graphical Models and Image Processing, Vol.58, No.3. pp 246-261, 1996.
 *
 * 
 * \ingroup FuzzyConnectednessSegmentation */

template <class TInputImage, class TOutputImage>
class FuzzyConnectednessImageFilter:
  public SimpleFuzzyConnectednessImageFilterBase<TInputImage,TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef FuzzyConnectednessImageFilter       Self;
  typedef SimpleFuzzyConnectednessImageFilterBase<TInputImage,TOutputImage>   
                                              Superclass;
  typedef SmartPointer <Self>  Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods).  */
  itkTypeMacro(FuzzyConnectednessImageFilter,
               SimpleFuzzyConnectednessImageFilterBase);

  /** Region, size, and pixel types. */
  typedef typename Superclass::IndexType IndexType;
  typedef typename Superclass::SizeType SizeType;
  typedef typename Superclass::PixelType PixelType;

  /** Set the Estimation of the mean difference between neighbor pixels for
   *  the object. */
  itkSetMacro(Diff_Mean, double);

  /** Get the Estimation of the mean difference between neighbor pixels for
   *  the object. */
  itkGetMacro(Diff_Mean, double);

  /** Set the Estimation of the variance of the difference between pixels for
   *  the object. */
  itkSetMacro(Diff_Var, double);

  /** Get the Estimation of the variance of the difference between pixels for
   *  the object. */
  itkGetMacro(Diff_Var, double);
  
  /** Set the Estimation of the mean difference between neighbor pixels for
   *  the object. */
  itkSetMacro(Mean, double);

  /** Get the Estimation of the mean difference between neighbor pixels for
   *  the object. */
  itkGetMacro(Mean, double);

  /** Set the Estimation of the variance of the difference between pixels for
   *  the object. */
  itkSetMacro(Var, double);

  /** Get the Estimation of the variance of the difference between pixels for
   *  the object. */
  itkGetMacro(Var, double);

  /** Setting the parameters for segmentation. */
  void SetParameters(const double inmean,const double invar, 
    const double indifmean,const double indifvar,const double inweight);
  
protected:
  FuzzyConnectednessImageFilter();
  ~FuzzyConnectednessImageFilter();

private:
  FuzzyConnectednessImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  double m_Mean; 
  double m_Var; //estimation of the Variance.
  double m_Diff_Mean;
  double m_Diff_Var;
  IndexType m_Seed;
  SizeType m_Size;

  virtual double FuzzyAffinity(const PixelType f1, const PixelType f2);
};


} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkFuzzyConnectednessImageFilter.txx"
#endif

#endif
