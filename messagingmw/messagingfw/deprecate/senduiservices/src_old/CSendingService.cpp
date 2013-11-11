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
* Description:   CSendingService implementation.
*
*/



// INCLUDE FILES
#include <e32Base.h>
#include <ECom.h>
#include <CSendingService.h>
#include <CMessageData.h>


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSendingService::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CSendingService* CSendingService::NewL(
    const TUid& aImplementationUid,
    CCoeEnv& aCoeEnv,
    CSendUiSingleton& aSingleton )
    {
    // Get the instantiation
    TSendingServiceParams params( aCoeEnv, aSingleton );
    CSendingService* sendingService = 
        REINTERPRET_CAST(
           CSendingService*, REComSession::CreateImplementationL(
               aImplementationUid,
               _FOFF(CSendingService, iDtor_ID_Key), 
               &params )
               );
    
    return sendingService;
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CSendingService::~CSendingService()
    {
    REComSession::DestroyedImplementation(iDtor_ID_Key);
    }

// -----------------------------------------------------------------------------
// C++ constructor
// -----------------------------------------------------------------------------
//
EXPORT_C CSendingService::CSendingService( CCoeEnv& aCoeEnv, CSendUiSingleton& aSingleton )
    : iCoeEnv( aCoeEnv ),
    iSingleton( aSingleton )
	{
	}

//  End of File
