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
* circular area.
*
*/


#include <e32math.h>
#include "lbtgeocircle.h"
#include "lbtgeoareabase.h"
#include "lbterrors.h"





// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLbtGeoCircle::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C   CLbtGeoCircle* CLbtGeoCircle::NewL() 
    {
   	CLbtGeoCircle* self = new( ELeave ) CLbtGeoCircle;
    CleanupStack::PushL( self );
    self->ConstructL();
	CleanupStack::Pop( self );  
	return self;
    }
    
// ---------------------------------------------------------------------------
// CLbtGeoCircle::NewLC()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C   CLbtGeoCircle* CLbtGeoCircle::NewLC() 
    {
   	CLbtGeoCircle* self = new( ELeave ) CLbtGeoCircle;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }    
    
// ---------------------------------------------------------------------------
// CLbtGeoCircle::NewL(TCoordinate aCenter,
//            TReal aRadius)
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//

EXPORT_C  CLbtGeoCircle* CLbtGeoCircle::NewL( const TCoordinate& aCenter,
            TReal aRadiusInMeters )

    {
	CLbtGeoCircle* self = new( ELeave ) CLbtGeoCircle;
    CleanupStack::PushL( self );
    self->ConstructL(aCenter,aRadiusInMeters);
	CleanupStack::Pop( self );    
    return self;
    }

// ---------------------------------------------------------------------------
// CLbtGeoCircle::NewLC(TCoordinate aCenter,
//            TReal aRadius)
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//

EXPORT_C  CLbtGeoCircle* CLbtGeoCircle::NewLC( const TCoordinate& aCenter,
            TReal aRadiusInMeters )

    {
	CLbtGeoCircle* self = new( ELeave ) CLbtGeoCircle;
    CleanupStack::PushL( self );
    self->ConstructL(aCenter,aRadiusInMeters);
    return self;
    }


// ---------------------------------------------------------------------------
// CLbtGeoCircle::ConstructL()
// ---------------------------------------------------------------------------
//
void CLbtGeoCircle::ConstructL(TCoordinate aCenter,TReal aRadius)
    {
    iCenter=aCenter;
    if(aRadius<0)
		User::Panic(KLbtClientPanicCategory,ELbtErrArgument);
	iRadius=aRadius;
    } 
   
   
// ---------------------------------------------------------------------------
// CLbtGeoCircle::ConstructL()
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CLbtGeoCircle::ConstructL()
    {
	iRadius=0;
	}
// ---------------------------------------------------------------------------
// CLbtGeoCircle::Center()
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
TCoordinate CLbtGeoCircle::Center() const
    {
    // To Do : Check if the center has been set or not. If not then lat long values should be NaN
	return iCenter;
    }

// ---------------------------------------------------------------------------
// From class CLbtGeoAreaBase.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CLbtGeoAreaBase::TGeoAreaType CLbtGeoCircle::Type() const
    {
	return CLbtGeoAreaBase::ECircle;
    }

// ---------------------------------------------------------------------------
// CLbtGeoCircle::SetCenter()
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//  
EXPORT_C void CLbtGeoCircle::SetCenter(const TCoordinate& aCenter )
    {
    iCenter=aCenter;
    }

// ---------------------------------------------------------------------------
// CLbtGeoCircle::Radius()
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C TReal CLbtGeoCircle::Radius() const
    {
	return iRadius;
    }

// ---------------------------------------------------------------------------
// CLbtGeoCircle::SetRadius()
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C void CLbtGeoCircle::SetRadius( TReal aRadiusInMeters )
    {
	if(aRadiusInMeters<0)

		User::Panic(KLbtClientPanicCategory,ELbtErrArgument);
	
	iRadius=aRadiusInMeters;
    }

// ---------------------------------------------------------------------------
// CLbtGeoCircle::GetAdditionalGeoAreaInfo()
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C RPointerArray< CLbtGeoAreaBase >& CLbtGeoCircle::GetAdditionalGeoAreaInfo()
    {
    return iAdditionalGeoAreaInfo;
    }

// ---------------------------------------------------------------------------
// CLbtGeoCircle::SetAdditionalGeoAreaInfo()
// This method is currently not supported.
// ---------------------------------------------------------------------------
// 
EXPORT_C void CLbtGeoCircle::SetAdditionalGeoAreaInfo(CLbtGeoAreaBase* /*aGeoArea*/)
    {
    
    }

// ---------------------------------------------------------------------------
// CLbtGeoCircle::CLbtGeoCircle()
// Default constructor
// ---------------------------------------------------------------------------
// 
CLbtGeoCircle::CLbtGeoCircle()
    {
	
    }
    
// ---------------------------------------------------------------------------
// CLbtGeoCircle::~CLbtGeoCircle()
// Destructor
// ---------------------------------------------------------------------------
// 
CLbtGeoCircle::~CLbtGeoCircle()
    {
	
    }

// ---------------------------------------------------------------------------
// CLbtGeoCircle::DoInternalizeL()
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C void CLbtGeoCircle::DoInternalizeL(RReadStream& aStream) 
    {
	TReal64 Lat,Long;
	TReal Rad;
	TReal32 Alt;
	Lat=aStream.ReadReal64L();
	Long=aStream.ReadReal64L();
	
    Alt=aStream.ReadReal32L();
    Rad=aStream.ReadReal32L();
    iCenter.SetCoordinate(Lat,Long,Alt);
	iRadius=Rad;
	}
// ---------------------------------------------------------------------------
// CLbtGeoCircle::DoExternalizeL()
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C void CLbtGeoCircle::DoExternalizeL(RWriteStream& aStream) const 
    {
	aStream.WriteReal64L(iCenter.Latitude());
	aStream.WriteReal64L(iCenter.Longitude());
	aStream.WriteReal32L(iCenter.Altitude());

	aStream.WriteReal32L(iRadius);
	}

// ---------------------------------------------------------------------------
// CLbtGeoCircle::ValidateGeoCircleInfoL()
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
void CLbtGeoCircle::ValidateCircleInformationL()
    {
    if( Math::IsNaN(iCenter.Latitude()) ||
        Math::IsNaN(iCenter.Longitude())||
        iRadius == 0 )
        {
        User::Leave( KErrArgument );
        } 
    }
