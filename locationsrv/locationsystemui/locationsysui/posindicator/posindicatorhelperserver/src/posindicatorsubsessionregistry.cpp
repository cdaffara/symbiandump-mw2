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
* Description: Implementation of subsession registry class.
*
*/


// INCLUDE FILES

#include "posindicatorsubsessionregistry.h"
#include "posindicatorserversubsession.h"
#include "posindicatorlogger.h"
#include <e32base.h>

// ---------------------------------------------------------------------------
// CPosIndicatorSubSessionRegistry::NewL
// ---------------------------------------------------------------------------
CPosIndicatorSubSessionRegistry* CPosIndicatorSubSessionRegistry::NewL()
    {
    FUNC("CPosIndicatorSubSessionRegistry::NewL");
    CPosIndicatorSubSessionRegistry* self = new ( ELeave ) CPosIndicatorSubSessionRegistry;
    CleanupStack::PushL( self );
    self->ConstructL();    
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------------------------
// CPosIndicatorSubSessionRegistry::CPosIndicatorSubSessionRegistry
// ---------------------------------------------------------------------------
CPosIndicatorSubSessionRegistry::CPosIndicatorSubSessionRegistry()
    {   
    FUNC("CPosIndicatorSubSessionRegistry::CPosIndicatorSubSessionRegistry");
    }

// ---------------------------------------------------------------------------
// CPosIndicatorSubSessionRegistry::~CPosIndicatorSubSessionRegistry
// ---------------------------------------------------------------------------
CPosIndicatorSubSessionRegistry::~CPosIndicatorSubSessionRegistry()
    {
    FUNC("CPosIndicatorSubSessionRegistry::~CPosIndicatorSubSessionRegistry");
    delete iRegistryIndex;
    delete iRegistryContainerIndex;
    }


// ---------------------------------------------------------------------------
// CPosIndicatorSubSessionRegistry::ConstructL
// ---------------------------------------------------------------------------
void CPosIndicatorSubSessionRegistry::ConstructL()
    {
    FUNC("CPosIndicatorSubSessionRegistry::ConstructL");
    iRegistryIndex = CObjectIx::NewL();
    iRegistryContainerIndex = CObjectConIx::NewL();
    iRegistryContainer = iRegistryContainerIndex->CreateL();
    }

// ---------------------------------------------------------------------------
// CPosIndicatorSubSessionRegistry::AddSubSessionL
// ---------------------------------------------------------------------------
TInt CPosIndicatorSubSessionRegistry::AddSubSessionL(
                                    CPosIndicatorServerSubsession* aSubSession)
    {
    FUNC("CPosIndicatorSubSessionRegistry::AddSubSessionL");
    iRegistryContainer->AddL( aSubSession );
    return iRegistryIndex->AddL( aSubSession );
    }

// ---------------------------------------------------------------------------
// CPosIndicatorSubSessionRegistry::CloseSubSession
// ---------------------------------------------------------------------------
TInt CPosIndicatorSubSessionRegistry::CloseSubSession( TUint aHandle )
    { 
    FUNC("CPosIndicatorSubSessionRegistry::CloseSubSession");
    if ( iRegistryIndex->At( aHandle ) )
        {
        // Removes the sub-session object and closes
        // the handle i,e. destroys the object
        iRegistryIndex->Remove( aHandle );
        return KErrNone;
        }
    return KErrBadHandle;
    }

// ---------------------------------------------------------------------------
// CPosIndicatorSubSessionRegistry::SubSessionFromHandle
// ---------------------------------------------------------------------------
CPosIndicatorServerSubsession* CPosIndicatorSubSessionRegistry::
                                            SubSessionFromHandle( TUint aHandle )
    {
    FUNC("CPosIndicatorSubSessionRegistry::SubSessionFromHandle");
    return static_cast<CPosIndicatorServerSubsession*> (iRegistryIndex->At(aHandle));
    }

// end of file
