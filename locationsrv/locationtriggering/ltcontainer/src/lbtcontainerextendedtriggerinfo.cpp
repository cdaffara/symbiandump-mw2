/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  class containing extended information about a trigger
*
*/


#include "lbtcontainerextendedtriggerinfo.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLbtExtendedTriggerInfo::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtExtendedTriggerInfo* CLbtExtendedTriggerInfo::NewL()
    {
	CLbtExtendedTriggerInfo* self=new( ELeave ) CLbtExtendedTriggerInfo;
	CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self ); 
    return self;
    }

EXPORT_C CLbtExtendedTriggerInfo* CLbtExtendedTriggerInfo::NewL(
    TLbtTriggerRectArea aArea,
    TReal aHystRadius,
    TBool aFired,
    const TDesC8& aStrategyData,
    TSecureId aOwnerSid,
    TBool aFireOnCreation)
    {
	CLbtExtendedTriggerInfo* self=new( ELeave ) CLbtExtendedTriggerInfo;
	CleanupStack::PushL( self );
    self->ConstructL( aArea,
                      aHystRadius,
                      aFired, 
                      aStrategyData,
                      aOwnerSid,
                      aFireOnCreation );
    CleanupStack::Pop( self ); 
    return self;
    }

EXPORT_C CLbtExtendedTriggerInfo::~CLbtExtendedTriggerInfo()
    {
    delete iStrategyData;
    }

EXPORT_C const CLbtExtendedTriggerInfo::TLbtTriggerRectArea 
    CLbtExtendedTriggerInfo::TriggerReactangleArea() const
    {
    return iArea;
    }

EXPORT_C void CLbtExtendedTriggerInfo::SetTriggerRectangleArea(
    const CLbtExtendedTriggerInfo::TLbtTriggerRectArea& aArea)
    {
    iArea = aArea;
    }

EXPORT_C const TReal& CLbtExtendedTriggerInfo::HysteresisRadius() const
    {
    return iHystRadius;
    }
    
EXPORT_C void CLbtExtendedTriggerInfo::SetHysteresisRadius(
    const TReal& aHystRadius)
    {
    iHystRadius = aHystRadius;
    }

EXPORT_C TBool CLbtExtendedTriggerInfo::IsTriggerFired() const
    {
    return iFired;
    }
    
EXPORT_C void CLbtExtendedTriggerInfo::SetTriggerFiredState(TBool aFired)
    {
    iFired = aFired;
    }

EXPORT_C const TDesC8& CLbtExtendedTriggerInfo::StategyData() const
    {
    return *iStrategyData;
    }
    
EXPORT_C TLbtTriggerFireInfo CLbtExtendedTriggerInfo::GetFiredInfo() const
    {
    return iFiredInfo;
    }
    
EXPORT_C void CLbtExtendedTriggerInfo::SetFiredInfo(TLbtTriggerFireInfo aFiredInfo)
    {
    iFiredInfo = aFiredInfo;
    }
    
EXPORT_C void CLbtExtendedTriggerInfo::SetStrategyDataL(
    const TDesC8& aStrategyData)
    {
    // Pre-allocate the memory. If the memory allocation fails atleast the
    // old data is still valid in this case.
    HBufC8 *tmp = HBufC8::NewL( aStrategyData.Length() );
    tmp->Des().Copy( aStrategyData );
    if ( iStrategyData )
        {
        delete iStrategyData;
        iStrategyData = NULL;
        }
    iStrategyData = tmp;
    }

EXPORT_C const TSecureId& CLbtExtendedTriggerInfo::OwnerSid() const
    {
    return iOwnerSid;
    }
    
EXPORT_C void CLbtExtendedTriggerInfo::SetOwnerSid(const TSecureId& aOwnerSid)
    {
    iOwnerSid = aOwnerSid;
    }

EXPORT_C TBool CLbtExtendedTriggerInfo::IsTriggerFireOnCreation() const
	{
	return iFireOnCreation;	
	}

EXPORT_C void CLbtExtendedTriggerInfo::SetTriggerFireOnCreation(TBool aFireOnCreation)
	{
	iFireOnCreation = aFireOnCreation;	
	}

CLbtExtendedTriggerInfo::CLbtExtendedTriggerInfo()
    {
    }

void CLbtExtendedTriggerInfo::ConstructL()
    {
    // Fill iStrategyData with KNullDesC.
    // This is required because the method StategyData()
    // does not return a pointer. Instead it returns a TDesC&.
    // If this HBufC is null then accessing the Ptr will result
    // in a KERN-EXEC 3.
    SetStrategyDataL( KNullDesC8() );
    }

void CLbtExtendedTriggerInfo::ConstructL(
    TLbtTriggerRectArea aArea,
    TReal aHystRadius,
    TBool aFired,
    const TDesC8& aStrategyData,
    TSecureId aOwnerSid,
    TBool aFireOnCreation)
    {
    SetTriggerRectangleArea( aArea );
    SetHysteresisRadius( aHystRadius );
    SetTriggerFiredState( aFired );
    SetStrategyDataL( aStrategyData );
    SetOwnerSid( aOwnerSid );
    SetTriggerFireOnCreation(aFireOnCreation);
    }

