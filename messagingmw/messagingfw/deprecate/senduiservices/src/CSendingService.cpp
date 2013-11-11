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
#include <e32base.h>
#include <ecom.h>
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
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CSendingService::~CSendingService()
    {
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
