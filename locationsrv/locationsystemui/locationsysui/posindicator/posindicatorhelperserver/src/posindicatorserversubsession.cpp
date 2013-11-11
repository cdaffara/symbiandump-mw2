/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Implementation of subsession class.
*
*/

// INCLUDES
#include "posindicatorserversubsession.h"
#include "posindicatorserversession.h"
#include "posindicatorservercore.h"
#include "posindicatorlocationrequestor.h"
#include "posindicatorserverconst.h"
#include "posindicatorlogger.h"

// ---------------------------------------------------------------------------
// CPosIndicatorServerSubsession::NewL
// ---------------------------------------------------------------------------
CPosIndicatorServerSubsession* CPosIndicatorServerSubsession::NewL( 
                                                CPosIndicatorLocationRequestor&
                                                aLocationRequestor )
    { 
    FUNC("CPosIndicatorServerSubsession::NewL");
    CPosIndicatorServerSubsession* self = new ( ELeave ) 
                                    CPosIndicatorServerSubsession( aLocationRequestor );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CPosIndicatorServerSubsession::CPosIndicatorServerSubsession
// ---------------------------------------------------------------------------
CPosIndicatorServerSubsession::CPosIndicatorServerSubsession(
                                                CPosIndicatorLocationRequestor&
                                                aLocationRequestor ):
                                                iLocationRequestor( aLocationRequestor )
    {
    FUNC("CPosIndicatorServerSubsession::CPosIndicatorServerSubsession");
    }

// ---------------------------------------------------------------------------
// CPosIndicatorServerSubsession::~CPosIndicatorServerSubsession
// ---------------------------------------------------------------------------
CPosIndicatorServerSubsession::~CPosIndicatorServerSubsession()
    {
    FUNC("CPosIndicatorServerSubsession::~CPosIndicatorServerSubsession");
    }

// ---------------------------------------------------------------------------
// CPosIndicatorServerSubsession::ConstructL
// ---------------------------------------------------------------------------
void CPosIndicatorServerSubsession::ConstructL()
    {
    FUNC("CPosIndicatorServerSubsession::ConstructL");
    }

// ---------------------------------------------------------------------------
// CPosIndicatorServerSubsession::ServiceL
// ---------------------------------------------------------------------------
void CPosIndicatorServerSubsession::ServiceL( const RMessage2 &aMessage )
    {
    FUNC("+ CPosIndicatorServerSubsession::ServiceL");
    switch( aMessage.Function() )
        {
        case EGetCurrentPosition:
            {
            iLocationRequestor.GetCurrentPosition( aMessage );
            break;
            }
        case EGetCurrentAddressInfoSize:
            {
            iLocationRequestor.GetCurrentAddressInfoSizeL( aMessage );
            break;
            }
        case EGetCurrentAddressInfo:
            {
            iLocationRequestor.GetCurrentAddressInfoL( aMessage );
            break;
            }
        case ECancelCurrentOperation:
            {
            iLocationRequestor.CancelRequest( aMessage );            
            break;
            }
        default:
            aMessage.Complete( KErrNotFound );
        }
    }

// end of file
