/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Definition of Movement Detection API
*
*/


#include <ecom.h>
#include "lbtmovementdetector.h"

// ======== LOCAL FUNCTIONS =========
// static cleanup function
static void RImpleInfoPtrArrayCleanup( TAny* aArray )
	{
	static_cast<RImplInfoPtrArray*>( aArray )->ResetAndDestroy();
	}


// Class methods

//------------------------------------------------------------
// CLbtMovementDetector::NewL
// Symbian Two - phase constructor
//------------------------------------------------------------
//
EXPORT_C CLbtMovementDetector* CLbtMovementDetector::NewL()
	{
	RImplInfoPtrArray implInfoPtrArray;
	TUid implementationUID;
	
	REComSession::ListImplementationsL( TUid::Uid ( KMovementDetectionPlugInInterfaceId ),
										implInfoPtrArray );
	
	if( implInfoPtrArray.Count() == 0 )
		{
		implInfoPtrArray.ResetAndDestroy();
		User::Leave( KErrNotFound );
		}
	TCleanupItem arrayCleanup( RImpleInfoPtrArrayCleanup, &implInfoPtrArray );
	CleanupStack::PushL(arrayCleanup);
	const CImplementationInformation* pluginInformation = implInfoPtrArray[0];
	implementationUID = pluginInformation->ImplementationUid();
											
	TAny* ptr = REComSession::CreateImplementationL( implementationUID,
											         _FOFF(CLbtMovementDetector,iDtor_ID_Key) );
	CleanupStack::PopAndDestroy(); //implInfoPtrArray

    return reinterpret_cast<CLbtMovementDetector*>(ptr);
	}

//------------------------------------------------------------
// CLbtMovementDetector::~CLbtMovementDetector
// Destructor
//------------------------------------------------------------
//
EXPORT_C CLbtMovementDetector::~CLbtMovementDetector()
	{
	REComSession::DestroyedImplementation(iDtor_ID_Key);
	}

// End of file.
