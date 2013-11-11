/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Contains utility functions for Phone Server.
*
*/


// INCLUDES
#include <e32base.h>
#include "phsrvutils.h" 


// CONSTANTS
_LIT( KPhoneServerPanicCategory, "PhSrvFault" );


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// PhoneServerUtils::Panic
// 
// Panic the server.
// -----------------------------------------------------------------------------
//
void PhoneServerUtils::Panic( TPhoneServerPanic aPanic ) 
    {
    User::Panic( KPhoneServerPanicCategory, aPanic );
    }


// -----------------------------------------------------------------------------
// PhoneServerUtils::PanicClient
// 
// Panic the client using server.
// -----------------------------------------------------------------------------
//
void PhoneServerUtils::PanicClient( 
    TPhoneServerPanic aPanic,
    const RMessage2& aMessage ) 
    {
    aMessage.Panic(
        KPhoneServerPanicCategory,
        aPanic );
    }

//  End of File
