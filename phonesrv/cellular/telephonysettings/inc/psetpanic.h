/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Panic function for Phone Settings
*
*
*/


//Different panic causes.
enum TPSetPanic {
    ECBPanicNoBscSet, 
    ECBBasicServiceNotReturned,
    ECBUnknownRequest,
    ECBUnknownBarringSetting,
    ECBNoObserver,

    ECDNewDefaultNumberNull, 
    ECDPanicNoBscSet, 
    ECDRequestPanicNoObserver, 
    ECDRequestPanicIncorrectRequest,

    ECWPanicNoObserver,
    ECWPanicNoPhone,
    ECWBasicServiceNotReturned,
    ECWPanicInvalidRequestMode,
    ECWPanicNoBscSet,

    ECliParameter,
    ECliGeneral,
    ECliNoObserver,
    ECliOperationFailed,

    ENetSelectPanicNullPointer,
    ENetNoObserver,
    ENetUnknownRequest,
    ENetError,
    ENetMemberVariableAlreadyCreated,
    ENetNoNetworkObserver
    };

//  Local Function Prototypes  
static void Panic( const TDesC& aClassName, TPSetPanic aPanic );

//  LOCAL FUNCTIONS
static void Panic( const TDesC& aClassName, TPSetPanic aPanic )
    {
    User::Panic( aClassName, aPanic );
    }
// End of file
