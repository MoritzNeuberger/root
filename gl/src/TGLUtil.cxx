// @(#)root/gl:$Name:  $:$Id: TGLUtil.cxx,v 1.13 2005/11/04 20:13:08 pcanal Exp $
// Author:  Richard Maunder  25/05/2005

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

// TODO: Function descriptions
// TODO: Class def - same as header!!!

#include "TGLUtil.h"
#include "TGLIncludes.h"
#include "TGLQuadric.h"
#include "TError.h"
#include "TMath.h"
#include "Riostream.h"

ClassImp(TGLVertex3)

//______________________________________________________________________________
TGLVertex3::TGLVertex3()
{
   Fill(0.0);
}

//______________________________________________________________________________
TGLVertex3::TGLVertex3(Double_t x, Double_t y, Double_t z)
{
   Set(x,y,z);
}

//______________________________________________________________________________
TGLVertex3::TGLVertex3(const TGLVertex3 & other)
{
   Set(other);
}

//______________________________________________________________________________
TGLVertex3::~TGLVertex3()
{
}

//______________________________________________________________________________
void TGLVertex3::Shift(TGLVector3 & shift)
{
   fVals[0] += shift[0];
   fVals[1] += shift[1];
   fVals[2] += shift[2];
}

//______________________________________________________________________________
void TGLVertex3::Shift(Double_t xDelta, Double_t yDelta, Double_t zDelta)
{
   fVals[0] += xDelta;
   fVals[1] += yDelta;
   fVals[2] += zDelta;
}

//______________________________________________________________________________
void TGLVertex3::Dump() const
{
   std::cout << "(" << fVals[0] << "," << fVals[1] << "," << fVals[2] << ")" << std::endl;
}

ClassImp(TGLVector3)

//______________________________________________________________________________
TGLVector3::TGLVector3() :
   TGLVertex3()
{
}

//______________________________________________________________________________
TGLVector3::TGLVector3(Double_t x, Double_t y, Double_t z) :
   TGLVertex3(x, y, z)
{
}

//______________________________________________________________________________
TGLVector3::TGLVector3(const TGLVector3 & other) :
   TGLVertex3(other.fVals[0], other.fVals[1], other.fVals[2])
{
}

//______________________________________________________________________________
TGLVector3::~TGLVector3()
{
}

ClassImp(TGLLine3)

//______________________________________________________________________________
TGLLine3::TGLLine3(const TGLVertex3 & vert1, const TGLVertex3 & vert2) :
   fVertex(vert1), fVector(vert2 - vert1)
{
}

//______________________________________________________________________________
TGLLine3::TGLLine3(const TGLVertex3 & vert, const TGLVector3 & vect) :
   fVertex(vert), fVector(vect)
{
}

//______________________________________________________________________________
TGLLine3::~TGLLine3()
{
}

//______________________________________________________________________________
void TGLLine3::Set(const TGLVertex3 & vert, const TGLVertex3 & end)
{
   fVertex = vert;
   fVector = end - fVertex;
}

//______________________________________________________________________________
void TGLLine3::Set(const TGLVertex3 & vert, const TGLVector3 & vect)
{
   fVertex = vert;
   fVector = vect;
}

//______________________________________________________________________________
void TGLLine3::Draw() const
{
   glBegin(GL_LINE_LOOP);
   glVertex3dv(fVertex.CArr());
   glVertex3dv(End().CArr());
   glEnd();
}

ClassImp(TGLRect)

//______________________________________________________________________________
TGLRect::TGLRect() :
      fX(0), fY(0), fWidth(0), fHeight(0)
{
}

//______________________________________________________________________________
TGLRect::TGLRect(Int_t x, Int_t y, UInt_t width, UInt_t height) :
      fX(x), fY(y), fWidth(width), fHeight(height)
{
}

//______________________________________________________________________________
TGLRect::~TGLRect()
{
}

//______________________________________________________________________________
void TGLRect::Expand(Int_t x, Int_t y)
{
   Int_t delX = x - fX;
   Int_t delY = y - fY;

   if (delX>static_cast<Int_t>(fWidth)) {
      fWidth = delX;
   }
   if (delY>static_cast<Int_t>(fHeight)) {
      fHeight = delY;
   }

   if (delX<0) {
      fX = x;
      fWidth += -delX;
   }
   if (delY<0) {
      fY = y;
      fHeight += -delY;
   }
}

//______________________________________________________________________________
Double_t TGLRect::Aspect() const
{
   if (fHeight == 0) {
      return 0.0;
   } else {
      return static_cast<Double_t>(fWidth) / static_cast<Double_t>(fHeight);
   }
}

//______________________________________________________________________________
EOverlap TGLRect::Overlap(const TGLRect & other) const
{
   if ((fX <= other.fX) && (fX + fWidth >= other.fX + other.fWidth) &&
        (fY <= other.fY) && (fY +fHeight >= other.fY + other.fHeight)) {
      return kInside;
   }
   else if ((fX >= other.fX + static_cast<Int_t>(other.fWidth)) ||
            (fX + static_cast<Int_t>(fWidth) <= other.fX) ||
            (fY >= other.fY + static_cast<Int_t>(other.fHeight)) ||
            (fY + static_cast<Int_t>(fHeight) <= other.fY)) {
      return kOutside;
   } else {
      return kPartial;
   }
}

ClassImp(TGLPlane)

//______________________________________________________________________________
TGLPlane::TGLPlane()
{
   // Construct a default plane of x + y + z = 0
   Set(1.0, 1.0, 1.0, 0.0);
}

//______________________________________________________________________________
TGLPlane::TGLPlane(const TGLPlane & other)
{
   Set(other);
}

//______________________________________________________________________________
TGLPlane::TGLPlane(Double_t a, Double_t b, Double_t c, Double_t d)
{
   // Construct plane with equation a.x + b.y + c.z + d = 0
   // with optional normalisation
   Set(a, b, c, d);
}

//______________________________________________________________________________
TGLPlane::TGLPlane(Double_t eq[4])
{
   // Construct plane with equation eq[0].x + eq[1].y + eq[2].z + eq[3] = 0
   // with optional normalisation
   Set(eq);
}

//______________________________________________________________________________
TGLPlane::TGLPlane(const TGLVertex3 & p1, const TGLVertex3 & p2, 
                   const TGLVertex3 & p3)
{
   // Construct plane passing through 3 supplied points
   // with optional normalisation
   Set(p1, p2, p3);
}

//______________________________________________________________________________
TGLPlane::TGLPlane(const TGLVector3 & v, const TGLVertex3 & p)
{
   // Construct plane with supplied normal vector, passing through point
   // with optional normalisation
   Set(v, p);
}

//______________________________________________________________________________
TGLPlane::~TGLPlane()
{
}

//______________________________________________________________________________
void TGLPlane::Dump() const
{
   std::cout.precision(6);
 	std::cout << "Plane : " << fVals[0] << "x + " << fVals[1] << "y + " << fVals[2] << "z + " << fVals[3] <<
	std::endl;
 }

ClassImp(TGLMatrix)

//______________________________________________________________________________
TGLMatrix::TGLMatrix()
{
   SetIdentity();
}

//______________________________________________________________________________
TGLMatrix::TGLMatrix(Double_t x, Double_t y, Double_t z)
{
   SetIdentity();
   SetTranslation(x, y, z);
}

//______________________________________________________________________________
TGLMatrix::TGLMatrix(const TGLVertex3 & translation)
{
   SetIdentity();
   SetTranslation(translation);
}

//______________________________________________________________________________
TGLMatrix::TGLMatrix(const TGLVertex3 & origin, const TGLVector3 & zAxis)
{
   SetIdentity();
   Set(origin, zAxis);
}

//______________________________________________________________________________
TGLMatrix::TGLMatrix(const Double_t vals[16])
{
   Set(vals);
}

//______________________________________________________________________________
TGLMatrix::TGLMatrix(const TGLMatrix & other)
{
   *this = other;
}

//______________________________________________________________________________
TGLMatrix::~TGLMatrix()
{
}

//______________________________________________________________________________
void TGLMatrix::Set(const TGLVertex3 & origin, const TGLVector3 & z)
{
   // Establish matrix transformation which when applied puts local origin at 
   // passed 'origin' and the local Z axis in direction 'z'. Both
   // 'origin' and 'zAxisVec' are expressed in the parent frame
   TGLVector3 zAxis(z);
   zAxis.Normalise();
   TGLVector3 axis; 
   if (TMath::Abs(zAxis.X()) <= TMath::Abs(zAxis.Y()) && TMath::Abs(zAxis.X()) <= TMath::Abs(zAxis.Z())) {
      axis.Set(1, 0, 0); 
   } else if (TMath::Abs(zAxis.Y()) <= TMath::Abs(zAxis.X()) && TMath::Abs(zAxis.Y()) <= TMath::Abs(zAxis.Z())) {
      axis.Set(0, 1, 0); 
   } else { 
      axis.Set(0, 0, 1);
   }

   TGLVector3 xAxis = Cross(zAxis, axis);
   xAxis.Normalise();
   TGLVector3 yAxis = Cross(zAxis, xAxis);

   fVals[0] = xAxis.X(); fVals[4] = yAxis.X(); fVals[8 ] = zAxis.X(); fVals[12] = origin.X();
   fVals[1] = xAxis.Y(); fVals[5] = yAxis.Y(); fVals[9 ] = zAxis.Y(); fVals[13] = origin.Y();
   fVals[2] = xAxis.Z(); fVals[6] = yAxis.Z(); fVals[10] = zAxis.Z(); fVals[14] = origin.Z();
   fVals[3] = 0.0;       fVals[7] = 0.0;       fVals[11] = 0.0;       fVals[15] = 1.0;
}

//______________________________________________________________________________
void TGLMatrix::Set(const Double_t vals[16])
{
   for (UInt_t i=0; i < 16; i++) {
      fVals[i] = vals[i];
   }
}

//______________________________________________________________________________
void TGLMatrix::SetIdentity()
{
   fVals[0] = 1.0; fVals[4] = 0.0; fVals[8 ] = 0.0; fVals[12] = 0.0;
   fVals[1] = 0.0; fVals[5] = 1.0; fVals[9 ] = 0.0; fVals[13] = 0.0;
   fVals[2] = 0.0; fVals[6] = 0.0; fVals[10] = 1.0; fVals[14] = 0.0;
   fVals[3] = 0.0; fVals[7] = 0.0; fVals[11] = 0.0; fVals[15] = 1.0;
}

//______________________________________________________________________________
void TGLMatrix::SetTranslation(Double_t x, Double_t y, Double_t z)
{
   SetTranslation(TGLVertex3(x,y,z));
}

//______________________________________________________________________________
void TGLMatrix::SetTranslation(const TGLVertex3 & translation)
{
   // Set the translation component of matrix
   fVals[12] = translation[0];
   fVals[13] = translation[1];
   fVals[14] = translation[2];
}

//______________________________________________________________________________
TGLVertex3 TGLMatrix::GetTranslation() const
{
   return TGLVertex3(fVals[12], fVals[13], fVals[14]);
}

//______________________________________________________________________________
void TGLMatrix::Translate(const TGLVector3 & vect)
{
   fVals[12] += vect[0];
   fVals[13] += vect[1];
   fVals[14] += vect[2];
}

//______________________________________________________________________________
void TGLMatrix::Scale(const TGLVector3 & scale)
{

   // Set local axis scaling factors
   TGLVector3 currentScale = GetScale();
   
   // x
   if (currentScale[0] != 0.0) {
      fVals[0] *= scale[0]/currentScale[0];
      fVals[1] *= scale[0]/currentScale[0];
      fVals[2] *= scale[0]/currentScale[0];
   } else {
      Error("TGLMatrix::Scale()", "zero scale div by zero");
   }
   // y
   if (currentScale[1] != 0.0) {
      fVals[4] *= scale[1]/currentScale[1];
      fVals[5] *= scale[1]/currentScale[1];
      fVals[6] *= scale[1]/currentScale[1];
   } else {
      Error("TGLMatrix::Scale()", "zero scale div by zero");
   }
   // z
   if (currentScale[2] != 0.0) {
      fVals[8] *= scale[2]/currentScale[2];
      fVals[9] *= scale[2]/currentScale[2];
      fVals[10] *= scale[2]/currentScale[2];
   } else {
      Error("TGLMatrix::Scale()", "zero scale div by zero");
   }
}

//______________________________________________________________________________
void TGLMatrix::Rotate(const TGLVertex3 & pivot, const TGLVector3 & axis, Double_t angle)
{
   TGLVector3 nAxis = axis;
   nAxis.Normalise();
   Double_t x = nAxis.X();
   Double_t y = nAxis.Y();
   Double_t z = nAxis.Z();
   Double_t c = TMath::Cos(angle);
   Double_t s = TMath::Sin(angle);

   // Calculate local rotation, with pre-translation to local pivot origin
   TGLMatrix rotMat;
   rotMat[ 0] = x*x*(1-c) + c;   rotMat[ 4] = x*y*(1-c) - z*s; rotMat[ 8] = x*z*(1-c) + y*s; rotMat[12] = pivot[0];
   rotMat[ 1] = y*x*(1-c) + z*s; rotMat[ 5] = y*y*(1-c) + c;   rotMat[ 9] = y*z*(1-c) - x*s; rotMat[13] = pivot[1];
   rotMat[ 2] = x*z*(1-c) - y*s; rotMat[ 6] = y*z*(1-c) + x*s; rotMat[10] = z*z*(1-c) + c;   rotMat[14] = pivot[2];
   rotMat[ 3] = 0.0;             rotMat[ 7] = 0.0;             rotMat[11] = 0.0;             rotMat[15] = 1.0;
   TGLMatrix localToWorld(-pivot);

   // TODO: Ugly - should use quaternions to avoid compound rounding errors and 
   // triple multiplication
   *this = rotMat * localToWorld * (*this);
}

//______________________________________________________________________________
void TGLMatrix::TransformVertex(TGLVertex3 & vertex) const
{
   TGLVertex3 orig = vertex;
   for (UInt_t i = 0; i < 3; i++) {
      vertex[i] = orig[0] * fVals[0+i] + orig[1] * fVals[4+i] +
                  orig[2] * fVals[8+i] + fVals[12+i];
   }
}

//______________________________________________________________________________
void TGLMatrix::Transpose3x3()
{
   // Transpose the top left 3x3 matrix component along major diagonal

   // TODO: Move this fix to the TBuffer3D filling side and remove

   // 0  4  8 12
   // 1  5  9 13
   // 2  6 10 14
   // 3  7 11 15

   Double_t temp = fVals[4];
	fVals[4] = fVals[1];
	fVals[1] = temp;
	temp = fVals[8];
	fVals[8] = fVals[2];
	fVals[2] = temp;
	temp = fVals[9];
	fVals[9] = fVals[6];
	fVals[6] = temp;
}

//______________________________________________________________________________
TGLVector3 TGLMatrix::GetScale() const
{
   // Get local axis scaling factors
   TGLVector3 x(fVals[0], fVals[1], fVals[2]);
   TGLVector3 y(fVals[4], fVals[5], fVals[6]);
   TGLVector3 z(fVals[8], fVals[9], fVals[10]);
   return TGLVector3(x.Mag(), y.Mag(), z.Mag());
}

//______________________________________________________________________________
void TGLMatrix::Dump() const
{
   std::cout.precision(6);
   for (Int_t x = 0; x < 4; x++) {
      std::cout << "[ ";
      for (Int_t y = 0; y < 4; y++) {
         std::cout << fVals[y*4 + x] << " ";
      }
      std::cout << "]" << std::endl;
   }
}

ClassImp(TGLUtil)

//______________________________________________________________________________
void TGLUtil::CheckError()
{
   GLenum errCode;
   const GLubyte *errString;

   if ((errCode = glGetError()) != GL_NO_ERROR) {
      errString = gluErrorString(errCode);
      Error("TGLUtil::CheckError", (const char *)errString);
   }
}

//______________________________________________________________________________
void TGLUtil::DrawSphere(const TGLVertex3 & position, Double_t radius, const Float_t rgba[4])
{
   static TGLQuadric quad;
   glPushMatrix();
   glTranslated(position.X(), position.Y(), position.Z());

   // Set diffuse material and default color - cover cases lighting on/off
   glMaterialfv(GL_FRONT, GL_DIFFUSE, rgba);
   glColor3d(rgba[0], rgba[1], rgba[2]);
   gluSphere(quad.Get(), radius, 40, 40);
   glPopMatrix();
}


