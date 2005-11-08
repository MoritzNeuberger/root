// @(#)root/gl:$Name:  $:$Id: TGLOrthoCamera.h,v 1.6 2005/07/08 15:39:29 brun Exp $
// Author:  Richard Maunder  25/05/2005
// Parts taken from original by Timur Pocheptsov

/*************************************************************************
 * Copyright (C) 1995-2004, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TGLOrthoCamera
#define ROOT_TGLOrthoCamera

#ifndef ROOT_TGLCamera
#include "TGLCamera.h"
#endif

/*************************************************************************
 * TGLOrthoCamera - TODO
 *
 *
 *
 *************************************************************************/
class TGLOrthoCamera : public TGLCamera {
public:
   enum EType { kXOY, kXOZ, kZOY  };
private:
   // Fields
   EType          fType;      //!

   // Limits - set in Setup()
   Double_t       fZoomMin;      //!
   Double_t       fZoomDefault;  //!
   Double_t       fZoomMax;      //!
   TGLBoundingBox fVolume;       //!

	// Current interaction
   Double_t       fZoom;      //!
   TGLVector3     fTruck;     //!
   TGLMatrix      fMatrix;    //!

   static   UInt_t   fgZoomDeltaSens;

   // Methods
   void Init();

public:

   TGLOrthoCamera(EType type);
   virtual ~TGLOrthoCamera();

   virtual void   Setup(const TGLBoundingBox & box);
   virtual void   Reset();
   virtual Bool_t Dolly(Int_t delta, Bool_t mod1, Bool_t mod2);
   virtual Bool_t Zoom (Int_t delta, Bool_t mod1, Bool_t mod2);
   virtual Bool_t Truck(Int_t x, Int_t y, Int_t xDelta, Int_t yDelta);
   virtual Bool_t Rotate(Int_t xDelta, Int_t yDelta);
   virtual void   Apply(const TGLBoundingBox & sceneBox, const TGLRect * pickRect = 0);

   ClassDef(TGLOrthoCamera,0) // an orthogonal view camera
};

#endif // ROOT_TGLOrthoCamera
