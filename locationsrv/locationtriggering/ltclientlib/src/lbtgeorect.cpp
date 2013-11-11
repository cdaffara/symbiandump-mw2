/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: Implementation of class representing a geographical
* rectangular area.
*
*/


#include <lbtgeorect.h>
#include <s32strm.h>
#include "lbtgeoareabase.h"
#include "lbterrors.h"


// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLbtGeoRect::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C  CLbtGeoRect* CLbtGeoRect::NewL()
    {
   	CLbtGeoRect* self = new( ELeave ) CLbtGeoRect;
    CleanupStack::PushL( self );
    self->ConstructL();
	CleanupStack::Pop( self );  
    return self;
    }
    
    
// ---------------------------------------------------------------------------
// CLbtGeoRect::NewLC()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C  CLbtGeoRect* CLbtGeoRect::NewLC()
    {
   	CLbtGeoRect* self = new( ELeave ) CLbtGeoRect;
    CleanupStack::PushL( self );
    self->ConstructL();
	return self;
    }    
// ---------------------------------------------------------------------------
// CLbtGeoRect::NewL(const TReal&  aSouthLatitude,
//            const TReal&  aNorthLatitude,
//            const TReal&  aWestLongitude,
//            const TReal&  aEastLongitude)
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C  CLbtGeoRect*  CLbtGeoRect::NewL(  TReal  aSouthLatitude,
             TReal  aNorthLatitude,
             TReal  aWestLongitude,
             TReal  aEastLongitude )
    {
	CLbtGeoRect* self = new( ELeave ) CLbtGeoRect;
    CleanupStack::PushL( self );
    self->ConstructL(aSouthLatitude,
    aNorthLatitude,
    aWestLongitude,
    aEastLongitude );
	CleanupStack::Pop( self ); 
    return self; 
    }

// ---------------------------------------------------------------------------
// CLbtGeoRect::NewLC(const TReal&  aSouthLatitude,
//            const TReal&  aNorthLatitude,
//            const TReal&  aWestLongitude,
//            const TReal&  aEastLongitude)
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C  CLbtGeoRect*  CLbtGeoRect::NewLC(  TReal  aSouthLatitude,
             TReal  aNorthLatitude,
             TReal  aWestLongitude,
             TReal  aEastLongitude )
    {
	CLbtGeoRect* self = new( ELeave ) CLbtGeoRect;
    CleanupStack::PushL( self );
    self->ConstructL(aSouthLatitude,
    aNorthLatitude,
    aWestLongitude,
    aEastLongitude );
	return self;
    }


// ---------------------------------------------------------------------------
// CLbtGeoRect::ConstructL(const TReal&  aSouthLatitude,
//            const TReal&  aNorthLatitude,
//            const TReal&  aWestLongitude,
//            const TReal&  aEastLongitude )
//
// ---------------------------------------------------------------------------
//
void CLbtGeoRect::ConstructL( TReal  aSouthLatitude,
             TReal  aNorthLatitude,
             TReal  aWestLongitude,
             TReal  aEastLongitude )
    {
    if((- 90 <= aSouthLatitude)&&(aSouthLatitude  <= 90) && (- 90 <= aNorthLatitude)&&(aNorthLatitude <= 90) && (aSouthLatitude <= aNorthLatitude) )
		{
     	iSouthLatitude=aSouthLatitude;
     	iNorthLatitude=aNorthLatitude;
        }
	else
		User::Panic(KLbtClientPanicCategory,ELbtErrArgument);
	
    if((- 180 <= aWestLongitude)&&( aWestLongitude<= 180))
        {
        iWestLongitude=aWestLongitude;
        }
    else
		User::Panic(KLbtClientPanicCategory,ELbtErrArgument);
	
	
	if((- 180 <= aEastLongitude)&&( aEastLongitude<= 180))
	    {
	    iEastLongitude=aEastLongitude;
	    }
	else
		User::Panic(KLbtClientPanicCategory,ELbtErrArgument);
    }
// ---------------------------------------------------------------------------
// CLbtGeoRect::ConstructL()
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CLbtGeoRect::ConstructL()
    {
	
    }

// ---------------------------------------------------------------------------
// From class CLbtGeoAreaBase.
// 
// ---------------------------------------------------------------------------
//
CLbtGeoAreaBase::TGeoAreaType CLbtGeoRect::Type() const
    {
	return CLbtGeoAreaBase::ERectangular;
    }

// ---------------------------------------------------------------------------
// CLbtGeoRect::GetBorder(TReal&  aSouthLatitude,
//            TReal&  aNorthLatitude,
//            TReal&  aWestLongitude,
//            TReal&  aEastLongitude )
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtGeoRect::GetBorder( 
            TReal&  aSouthLatitude,
            TReal&  aNorthLatitude,
            TReal&  aWestLongitude,
            TReal&  aEastLongitude ) const
    {
    aSouthLatitude=iSouthLatitude;
	aNorthLatitude=iNorthLatitude;
	aWestLongitude=iWestLongitude;
	aEastLongitude=iEastLongitude;
    }
            
// ---------------------------------------------------------------------------
// CLbtGeoRect::SetBorder(TReal&  aSouthLatitude,
//            TReal&  aNorthLatitude,
//            TReal&  aWestLongitude,
//            TReal&  aEastLongitude )
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//

EXPORT_C void CLbtGeoRect::SetBorder( 
             TReal  aSouthLatitude,
             TReal  aNorthLatitude,
             TReal  aWestLongitude,
             TReal  aEastLongitude )
            
    {
    if((- 90 <= aSouthLatitude)&&(aSouthLatitude  <= 90) && (- 90 <= aNorthLatitude)&&(aNorthLatitude <= 90) && (aSouthLatitude <= aNorthLatitude) )
	    {
     	iSouthLatitude=aSouthLatitude;
     	iNorthLatitude=aNorthLatitude;
        }
	else
		User::Panic(KLbtClientPanicCategory,ELbtErrArgument);
	

   if((- 180 <= aWestLongitude)&&( aWestLongitude<= 180))
        {
        iWestLongitude=aWestLongitude;
        }
    else
		User::Panic(KLbtClientPanicCategory,ELbtErrArgument);
	
	if((- 180 <= aEastLongitude)&&(aEastLongitude <= 180))
	    {
	    iEastLongitude=aEastLongitude;
	    }
    else
		User::Panic(KLbtClientPanicCategory,ELbtErrArgument);
	}

// ---------------------------------------------------------------------------
// CLbtGeoRect::CLbtGeoRect()
// Default constructor
// ---------------------------------------------------------------------------
//  
 CLbtGeoRect::CLbtGeoRect()
     {
     	
     }
 // ---------------------------------------------------------------------------
// CLbtGeoRect::~CLbtGeoRect()
// Destructor
// ---------------------------------------------------------------------------
// 
 CLbtGeoRect::~CLbtGeoRect()
    {
 	
    }
// ---------------------------------------------------------------------------
// CLbtGeoRect::DoExternalizeL()
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//  
EXPORT_C void CLbtGeoRect::DoExternalizeL(RWriteStream& aStream) const 
    {
 	aStream.WriteReal32L(iSouthLatitude);
	aStream.WriteReal32L(iNorthLatitude);
	aStream.WriteReal32L(iWestLongitude);
	aStream.WriteReal32L(iEastLongitude);
    }

// ---------------------------------------------------------------------------
// CLbtGeoRect::DoInternalizeL()
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//   
EXPORT_C  void CLbtGeoRect::DoInternalizeL(RReadStream& aStream) 
    {
    iSouthLatitude=aStream.ReadReal32L();
 	iNorthLatitude=aStream.ReadReal32L();
 	iWestLongitude=aStream.ReadReal32L();
 	iEastLongitude=aStream.ReadReal32L();
	}



