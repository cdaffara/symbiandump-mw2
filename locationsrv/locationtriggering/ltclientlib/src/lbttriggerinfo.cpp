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
* Description: Implementation of data class containing trigger
* information.
*
*/


#include "lbttriggerinfo.h"
#include "lbttriggerentry.h"
#include "lbttriggerdynamicinfo.h"
#include "lbtsessiontrigger.h"
#include "lbtstartuptrigger.h"



// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLbtTriggerInfo::CLbtTriggerInfo()
// Default constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtTriggerInfo::CLbtTriggerInfo()
    {
	
	}

// ---------------------------------------------------------------------------
// CLbtTriggerInfo::~CLbtTriggerInfo()
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtTriggerInfo::~CLbtTriggerInfo()
    {
    if( iEntry )
        {
        delete iEntry;
        iEntry = NULL;
        }
    if( iDynInfo )
        {
        delete iDynInfo;
        iDynInfo = NULL;
        }
    }


// ---------------------------------------------------------------------------
// CLbtTriggerInfo::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C CLbtTriggerInfo* CLbtTriggerInfo::NewL()
    {
	CLbtTriggerInfo* self = new( ELeave ) CLbtTriggerInfo;
    CleanupStack::PushL( self );
    self->ConstructL();
	CleanupStack::Pop( self );    
    return self;	
    }

// ---------------------------------------------------------------------------
// CLbtTriggerInfo::NewLC()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C CLbtTriggerInfo* CLbtTriggerInfo::NewLC()
    {
    CLbtTriggerInfo* self = new( ELeave ) CLbtTriggerInfo;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;    
    }

// ---------------------------------------------------------------------------
// CLbtTriggerInfo::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C CLbtTriggerInfo* CLbtTriggerInfo::NewL( 
            CLbtTriggerEntry* aEntry,
            TLbtTriggerDynamicInfo* aDynInfo )
    {
 	CLbtTriggerInfo* self = new( ELeave ) CLbtTriggerInfo;
    CleanupStack::PushL( self );
    self->ConstructL(aEntry,aDynInfo);
	CleanupStack::Pop( self );    
    return self;
 	}

// ---------------------------------------------------------------------------
// CLbtTriggerInfo::TriggerEntry()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//  
EXPORT_C CLbtTriggerEntry* CLbtTriggerInfo::TriggerEntry()
    {
    return iEntry;	
    }

// ---------------------------------------------------------------------------
// CLbtTriggerInfo::SetTriggerEntry()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//  
 
EXPORT_C void CLbtTriggerInfo::SetTriggerEntry( CLbtTriggerEntry* aEntry )
    {
    if( iEntry )
        {
        delete iEntry;
        iEntry = NULL;
        }
  	iEntry=aEntry;
    }

// ---------------------------------------------------------------------------
// CLbtTriggerInfo::DynInfo()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//  
EXPORT_C TLbtTriggerDynamicInfo* CLbtTriggerInfo::DynInfo()
    {
	return iDynInfo;	
    }

// ---------------------------------------------------------------------------
// CLbtTriggerInfo::SetDynInfo()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//  
EXPORT_C void CLbtTriggerInfo::SetDynInfo( TLbtTriggerDynamicInfo* aDynInfo )
    {
    if( iDynInfo )
        {
        delete iDynInfo;
        iDynInfo = NULL;
        }
	iDynInfo=aDynInfo;
    }

// ---------------------------------------------------------------------------
// CLbtGeoCircle::ConstructL()
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CLbtTriggerInfo::ConstructL()
    {
    iEntry = NULL;
    iDynInfo = NULL;
    }

// ---------------------------------------------------------------------------
// CLbtGeoCircle::ConstructL()
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CLbtTriggerInfo::ConstructL(CLbtTriggerEntry* aEntry,
            TLbtTriggerDynamicInfo* aDynInfo)
    {
	iEntry=aEntry;
	iDynInfo=aDynInfo;
    }

// ---------------------------------------------------------------------------
// CLbtTriggerInfo::SetTriggerEntry()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//  
EXPORT_C void CLbtTriggerInfo::InternalizeL( RReadStream& aStream )
    {	
	TInt trigType=aStream.ReadInt8L();
	if(trigType==1)	
	    {
		iEntry=CLbtSessionTrigger::NewL();
		iEntry->InternalizeL(aStream);
	    }
	else 
	    {
		iEntry=CLbtStartupTrigger::NewL();
		iEntry->InternalizeL(aStream);
	    }
	
    TInt trigValidity=aStream.ReadInt8L();
	
	if(iDynInfo !=NULL)
	    {
	    delete iDynInfo;
	    iDynInfo=NULL;
	    }
	
	iDynInfo=new TLbtTriggerDynamicInfo;
	
	switch(trigValidity)
	    {
		case 1: iDynInfo->iValidity=TLbtTriggerDynamicInfo::EInvalid;
						break;
		case 2: iDynInfo->iValidity=TLbtTriggerDynamicInfo::EValid;
						break;
		}
	
	iDynInfo->iDistanceToLatestLocation=aStream.ReadReal32L()	;
    }

// ---------------------------------------------------------------------------
// CLbtTriggerInfo::SetTriggerEntry()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//  
EXPORT_C void CLbtTriggerInfo::ExternalizeL( RWriteStream& aStream ) const
    {
	CLbtTriggerEntry::TType trigType=iEntry->Type();
	if(trigType==CLbtTriggerEntry::ETypeSession)
		aStream.WriteInt8L(1);
	
	else
		aStream.WriteInt8L(2);
	iEntry->ExternalizeL(aStream);
	
	TLbtTriggerDynamicInfo::TLbtTriggerValidity trigValidity=iDynInfo->iValidity;
	switch(trigValidity)
	    {
		case TLbtTriggerDynamicInfo::EInvalid:aStream.WriteInt8L(1);
				 break;
		case TLbtTriggerDynamicInfo::EValid: aStream.WriteInt8L(2);
					break;
		default:
			aStream.WriteInt8L(0);
			break;
	    }
	
	
	aStream.WriteReal32L(iDynInfo->iDistanceToLatestLocation);
    }

