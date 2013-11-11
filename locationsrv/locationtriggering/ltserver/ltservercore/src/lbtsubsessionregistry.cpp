/*
* Copyright (c) 2006,2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Sub-session registry class for LBT sessions
*
*/


// INCLUDE FILES
#include <e32base.h>
#include <lbterrors.h>
#include "lbtsubsession.h"
#include "lbtsubsessionregistry.h"
#include "lbtlogger.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLbtSubSessionRegistry::NewL
// Symbian Two - phase constructor
// ---------------------------------------------------------------------------
//
CLbtSubSessionRegistry* CLbtSubSessionRegistry::NewL()
	{
	FUNC_ENTER("CLbtSubSessionRegistry::NewL");	
	CLbtSubSessionRegistry* self = new ( ELeave ) CLbtSubSessionRegistry;
    CleanupStack::PushL( self );
    self->ConstructL();    
    CleanupStack::Pop();
    return self;
	}

// ---------------------------------------------------------------------------
// CLbtSubSessionRegistry::CLbtSubSessionRegistry
// ---------------------------------------------------------------------------
//
CLbtSubSessionRegistry::CLbtSubSessionRegistry()
	{	
	}

// ---------------------------------------------------------------------------
// CLbtSubSessionRegistry::~CLbtSubSessionRegistry
// ---------------------------------------------------------------------------
//
CLbtSubSessionRegistry::~CLbtSubSessionRegistry()
	{
	FUNC_ENTER("CLbtSubSessionRegistry::~CLbtSubSessionRegistry");		
	delete iRegistryIndex;
    delete iRegistryContainerIndex;
	}


// ---------------------------------------------------------------------------
// CLbtSubSessionRegistry::ConstructL
// ---------------------------------------------------------------------------
//
void CLbtSubSessionRegistry::ConstructL()
	{
	FUNC_ENTER("CLbtSubSessionRegistry::ConstructL");		
	iRegistryIndex = CObjectIx::NewL();
    iRegistryContainerIndex = CObjectConIx::NewL();
    iRegistryContainer = iRegistryContainerIndex->CreateL();
	}

// ---------------------------------------------------------------------------
// CLbtSubSessionRegistry::AddSubSessionL
// ---------------------------------------------------------------------------
//
TInt CLbtSubSessionRegistry::AddSubSessionL(CLbtSubSession* aSubSession)
	{
	FUNC_ENTER("CLbtSubSessionRegistry::AddSubSessionL");		
	iRegistryContainer->AddL(aSubSession);
    return iRegistryIndex->AddL(aSubSession);
	}

// ---------------------------------------------------------------------------
// CLbtSubSessionRegistry::CloseSubSession
// ---------------------------------------------------------------------------
//
TInt CLbtSubSessionRegistry::CloseSubSession( TUint aHandle )
	{
	FUNC_ENTER("CLbtSubSessionRegistry::CloseSubSession");		
	if (iRegistryIndex->At(aHandle))
        {
        // Removes the sub-session object and closes
        // the handle i,e. destroys the object
        iRegistryIndex->Remove(aHandle);
        return KErrNone;
        }
    return ELbtServerBadHandle;
	}

// ---------------------------------------------------------------------------
// CLbtSubSessionRegistry::SubSessionFromHandle
// ---------------------------------------------------------------------------
//
CLbtSubSession* CLbtSubSessionRegistry::SubSessionFromHandle( TUint aHandle )
	{
	FUNC_ENTER("CLbtSubSessionRegistry::SubSessionFromHandle");		
	return static_cast<CLbtSubSession*> (iRegistryIndex->At(aHandle));
	}

// ---------------------------------------------------------------------------
// CLbtSubSessionRegistry::Count
// ---------------------------------------------------------------------------
//
TInt CLbtSubSessionRegistry::Count()
	{
	return iRegistryIndex->ActiveCount();
	}

// ---------------------------------------------------------------------------
// CLbtSubSessionRegistry::GetSubSessionHandle
// ---------------------------------------------------------------------------
//
TInt CLbtSubSessionRegistry::GetSubSessionHandle(TInt aIndex)
    {
    CObject* obj = (*iRegistryIndex)[aIndex];
    return iRegistryIndex->At(obj);
    }

// end of file
