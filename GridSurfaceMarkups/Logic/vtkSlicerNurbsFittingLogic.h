/*==============================================================================

  Distributed under the OSI-approved BSD 3-Clause License.

  Copyright (c) Children's Hospital of Philadelphia. All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

  * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

  * Neither the name of Kitware, Inc. nor the names of Contributors
    may be used to endorse or promote products derived from this
    software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  This file was originally developed by Csaba Pinter (Pixel Medical /
  Ebatinca).

==============================================================================*/

#ifndef __vtkSlicerNurbsFittingLogic_h_
#define __vtkSlicerNurbsFittingLogic_h_

#include <vtkPolyDataAlgorithm.h>

#include "vtkSlicerGridSurfaceMarkupsModuleLogicExport.h"

class vtkDoubleArray;
class vtkIdList;

class VTK_SLICER_GRIDSURFACEMARKUPS_MODULE_LOGIC_EXPORT vtkSlicerNurbsFittingLogic : public vtkPolyDataAlgorithm
{
public:
  static vtkSlicerNurbsFittingLogic* New();
  vtkTypeMacro(vtkSlicerNurbsFittingLogic, vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /// TODO:
  void SetInputPoints(vtkPoints* points);
  /// TODO:
  vtkSmartPointer<vtkPoints> GetInputPoints() const { return this->InputPoints.GetPointer(); };

  /// TODO:
  vtkSetVector2Macro(InputResolution, unsigned int);
  /// TODO:
  vtkGetVector2Macro(InputResolution, unsigned int);
  /// TODO:
  vtkSetVector2Macro(InterpolationDegrees, unsigned int);
  /// TODO:
  vtkGetVector2Macro(InterpolationDegrees, unsigned int);
  /// TODO:
  vtkSetMacro(UseCentripetal, bool);
  vtkBooleanMacro(UseCentripetal, bool);
  /// TODO:
  vtkGetMacro(UseCentripetal, bool);

  /// Function computing the NURBS surface according to the pipeline architecture of VTK.
  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*);

protected:
  /// Compute NURBS surface poly data from the input points according to input resolution and degrees
  void UpdateNurbsPolyData(vtkPolyData* polyData);

  /// TODO:
  void ComputeParamsSurface(vtkDoubleArray* ukParams, vtkDoubleArray* vlParams);
  /// TODO:
  void ComputeParamsCurve(vtkIdList* pointIndexList, vtkDoubleArray* parametersArray);
  /// TODO:
  void ComputeKnotVector(int degree, int numOfPoints, vtkDoubleArray* params, vtkDoubleArray* outKnotVector);
  /// TODO:
  void BuildCoeffMatrix(int degree, vtkDoubleArray* knotVector, vtkDoubleArray* params, vtkPoints* points, double** outCoeffMatrix);

  /// TODO: helpers
  void BasisFunction(int degree, vtkDoubleArray* knotVector, int span, double knot, vtkDoubleArray* outBasisFunctions);
  /// TODO: helpers
  int FindSpanLinear(int degree, vtkDoubleArray* knotVector, int numControlPoints, double knot);

  /// TODO: linalg
  void LuSolve(double** coeffMatrix, int matrixSize, vtkPoints* points, vtkPoints* outControlPointsR);
  /// TODO: linalg
  void LuDecomposition(double** matrixA, double** matrixL, double** matrixU, int size);
  /// TODO: linalg
  void ForwardSubstitution(double** matrixL, double* b, int size, double* outY);
  /// TODO: linalg
  void BackwardSubstitution(double** matrixU, double* y, int size, double* outX);
   
  /// Convenience function to get point index from input point list with the two (u,v) indices
  unsigned int GetPointIndexUV(unsigned int u, unsigned int v);
  /// Convenience function to allocate NxN matrix
  /// \param m Number of rows
  /// \param n Number of columns. If omitted it is considered a square mxm matrix
  double** AllocateMatrix(int m, int n=0);
  /// Convenience function to delete NxN matrix
  /// \param m Number of rows
  /// \param n Number of columns. If omitted it is considered a square mxm matrix
  void DestructMatrix(double** matrix, int m, int n=0);

protected:
  /// Input control points. The number of points is u*v, and the strides contain the rows (0:[0,0], 1:[0,1], ...)
  vtkSmartPointer<vtkPoints> InputPoints;

  /// Number of data points along the u and v directions, respectively
  unsigned int InputResolution[2] = {4,4};
  /// Degree of the output surface for the u and v directions, respectively
  unsigned int InterpolationDegrees[2] = {3,3};

  /// Activate centripetal parametrization method. Default: false
  bool UseCentripetal = false;

protected:
  vtkSlicerNurbsFittingLogic();
  ~vtkSlicerNurbsFittingLogic() override;

private:
  vtkSlicerNurbsFittingLogic(const vtkSlicerNurbsFittingLogic&) = delete;
  void operator=(const vtkSlicerNurbsFittingLogic&) = delete;
};

#endif // __vtkSlicerNurbsFittingLogic_h_