/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Sub Session Factory.
*
*/




// INCLUDES
#include "phcltclientserver.h" 

#include "phsrvsubsessionfactory.h" 
#include "cphsrvsubsessionnotifier.h" 
#include "cphsrvsubsessionextcall.h" 
#include "cphsrvsubsessionussd.h" 
#include "cphsrvsubsessionemergencynum.h" 
#include "cphsrvsubsessioncommandhandler.h" 
#include "cphsrvsubsessionmessenger.h" 
#include "cphsrvsubsessionimagehandler.h" 


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// PhSrvSubSessionFactory::PhSrvSubSessionFactoryIsCreationFunction
// 
// A processing function that allows certain IPC op-codes
// to be intercepted and processed by the session rather than
// a subsession. This function returns ETrue if it is one of
// these special IPC op codes.
// -----------------------------------------------------------------------------
//
TBool PhSrvSubSessionFactory::PhSrvSubSessionFactoryIsCreationFunction( 
    TInt aFunction )
    {
    switch( aFunction )
        {
        case EPhoneServerExtCallSubSessionOpen:
        case EPhoneServerNotifySubSessionOpen:
        case EPhoneServerUSSDSubSessionOpen:
        case EPhoneServerEmergencyNumberSubSessionOpen:
        case EPhoneServerComHandSubSessionOpen:
        case EPhoneServerMessengerSubSessionOpen:
        case EPhoneServerImageHandlerSubSessionOpen:
            return ETrue;

        default:
            return EFalse;
        }
    }


// -----------------------------------------------------------------------------
// PhSrvSubSessionFactory::PhSrvSubSessionFactoryCreateLC
// 
// Creates a new instance of a subsession based upon the
// IPC op code specified.
// -----------------------------------------------------------------------------
//
CPhSrvSubSessionBase* PhSrvSubSessionFactory::PhSrvSubSessionFactoryCreateLC( 
    TInt aFunction, 
    CPhSrvSession& aSession )
    {
    CPhSrvSubSessionBase* subSession = NULL;

    switch( aFunction )
        {
        case EPhoneServerExtCallSubSessionOpen:
            subSession = new( ELeave ) CPhSrvSubSessionExtCall( aSession );
            break;
        case EPhoneServerNotifySubSessionOpen:
            subSession = new( ELeave ) CPhSrvSubSessionNotifier( aSession );
            break;
        case EPhoneServerUSSDSubSessionOpen:
            subSession = new( ELeave ) CPhSrvSubSessionUssd( aSession );
            break;
        case EPhoneServerEmergencyNumberSubSessionOpen:
            subSession = new( ELeave ) CPhSrvSubSessionEmergencyNum( aSession );
            break;
        case EPhoneServerComHandSubSessionOpen:
            subSession = 
                CPhSrvSubSessionCommandHandler::NewL( aSession );
            break;
        case EPhoneServerMessengerSubSessionOpen:
            subSession = 
                CPhSrvSubSessionMessenger::NewL( aSession );
            break;

        case EPhoneServerImageHandlerSubSessionOpen:
            subSession = CPhSrvSubSessionImageHandler::NewL( aSession );
            break;
        default:
            break;
        }

    // Complete construction
    if ( aFunction != EPhoneServerImageHandlerSubSessionOpen )
        {
        CleanupStack::PushL( subSession );        
        }
    subSession->ConstructL();
    return subSession;
    }

// -----------------------------------------------------------------------------
// PhSrvSubSessionFactory::PhSrvSubSessionFactoryIsCreationAllowed
// 
// This function returns ETrue if Creation a new instance of a subsession based upon 
// the IPC op code specified is allowed for the Secure ID of the process which 
// sent this message.
// -----------------------------------------------------------------------------
//
TBool PhSrvSubSessionFactory::PhSrvSubSessionFactoryIsCreationAllowed( 
    TInt aFunction,
    TUint32 aSID )
    {
    switch( aFunction )
        {
        case EPhoneServerNotifySubSessionOpen:
        case EPhoneServerComHandNotifySubSessionOpen:
            if ( aSID == KPhoneSecureId.iUid 
#if defined(__WINSCW__)
                            // To enable module testing using EUnit
                                || aSID == KEunitExeRunnerId.iUid 
#endif // __WINSCW__
                                )
                {       
                return ETrue;   
                }
            else
                {
                return EFalse;
                }
        default:
            return ETrue;
        }
    }

// End of File
