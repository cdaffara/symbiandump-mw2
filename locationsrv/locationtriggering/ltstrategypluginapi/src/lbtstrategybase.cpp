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
* Description: Base class implementation of location acquisition 
* strategy plugins
*
*/



#include <ecom.h>
#include <badesca.h>
#include <implementationinformation.h>

#include "lbtstrategybase.h"


_LIT( KReserved, "Reserved" );


// ---------------------------------------------------------
// Resets & destroys array if leave occurs
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void ResetAndDestroy(TAny* aArray)
    {
    ( (RImplInfoPtrArray*)aArray )->ResetAndDestroy();
    }


// ============================ MEMBER FUNCTIONS ===============================


// ---------------------------------------------------------
// Factory method to create the instance of CLbtStrategyBase
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C CLbtStrategyBase* CLbtStrategyBase::NewL( 
							TAny* aConstructionParameters )
    {
    // Determine list of implementations and load the first available
	TUid implementationUID;
	RImplInfoPtrArray implInfoArray;
	
	CleanupStack::PushL( TCleanupItem( *ResetAndDestroy, &implInfoArray ) );
	REComSession::ListImplementationsL( KLocAcquisitionStrategyInterfaceUid, 
										implInfoArray );

	if( implInfoArray.Count() == 0 )
		{
		User::Leave( KErrNotFound );
		}
	const CImplementationInformation* pluginInformation = implInfoArray[0];
	implementationUID = pluginInformation->ImplementationUid();

	TAny* ptr = REComSession::CreateImplementationL(
								implementationUID, 
								_FOFF( CLbtStrategyBase, iDtorIdKey ),
								 aConstructionParameters ); 
	
	CleanupStack::PopAndDestroy( &implInfoArray );								
	return ( REINTERPRET_CAST( CLbtStrategyBase*, ptr ) );
    }    


// ---------------------------------------------------------
// Destructor
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C CLbtStrategyBase::~CLbtStrategyBase()
    {
    // Inform ECOM framework that this specific instance of the 
    // interface has been destroyed.
    REComSession::DestroyedImplementation( iDtorIdKey );
    }
    

// ---------------------------------------------------------
// Constuctor
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C CLbtStrategyBase::CLbtStrategyBase()
    {
    }


// ---------------------------------------------------------
// Creates the internals of strategy plugin
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CLbtStrategyBase::BaseConstructL( TAny* aConstructionParameters )
	{
	iStrategyObserver = 
		reinterpret_cast<MLbtTriggeringSupervisionObserver*>
			( aConstructionParameters );
	}


// ---------------------------------------------------------
// Inform Location Triggering Server about a trigger fire event 
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CLbtStrategyBase::TriggerFiredL( CLbtGeoAreaBase::TGeoAreaType aAreaType,TLbtTriggerId aId, 
        								const TPositionInfo& aPosInfo )
	{
	iStrategyObserver->TriggerFiredL( aAreaType,aId, aPosInfo );
	}


// ---------------------------------------------------------
// Report triggering supervision status to location triggering 
// server
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CLbtStrategyBase::SetTriggeringSupervisionDynamicInfo( 
        	const TLbtStrategySupervisionDynamicInfo& aStatus )
	{
	iStrategyObserver->SetTriggeringSupervisionDynamicInfo( aStatus );
	}


// ---------------------------------------------------------
// Retrieve triggering supervision settings from location 
// triggering server
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CLbtStrategyBase::GetTriggeringSupervisionSettings( 
        	TLbtStrategyTriggeringSupervisionSettings& aSettings )
	{
	iStrategyObserver->GetTriggeringSupervisionSettings( aSettings );
	}


// ---------------------------------------------------------
// Reserved for future expansion
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CLbtStrategyBase::CLbtStrategy_Reserved1()
	{
	User::Panic( KReserved, KErrNotSupported );
	}


// ---------------------------------------------------------
// Reserved for future expansion
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CLbtStrategyBase::CLbtStrategy_Reserved2()
	{
	User::Panic( KReserved, KErrNotSupported );
	}
