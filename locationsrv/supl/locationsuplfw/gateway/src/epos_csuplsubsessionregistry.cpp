/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Registry for subsessions and positioning module instances
*
*/



// INCLUDE FILES
#include <e32base.h>

#include "epos_csuplprotocolmanagerbase.h"
#include "epos_csuplsessionmanager.h"
#include "epos_csuplsubsessionregistry.h"
#include "epos_csuplipcsubsession.h"
#include "epos_csuplglobal.h"

//#ifdef _DEBUG
_LIT(KTraceFileName, "SUPL_GW::epos_csuplsubsessionregistry.cpp");
//#endif

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CSuplSubsessionRegistry::CSuplSubsessionRegistry()
    {
    }

// EPOC default constructor
void CSuplSubsessionRegistry::ConstructL()
    {
    DEBUG_TRACE("ConstructL", __LINE__)            
    iRegistryIndex = CObjectIx::NewL();
    iRegistryContainerIndex = CObjectConIx::NewL();
    iRegistryContainer = iRegistryContainerIndex->CreateL();
    }

// Two-phased constructor
CSuplSubsessionRegistry* CSuplSubsessionRegistry::NewL()
    {
    DEBUG_TRACE("NewL", __LINE__)            
    CSuplSubsessionRegistry* self = new (ELeave) CSuplSubsessionRegistry;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CSuplSubsessionRegistry::~CSuplSubsessionRegistry()
    {
    DEBUG_TRACE("~CSuplSubsessionRegistry", __LINE__)            
    delete iRegistryIndex;
    delete iRegistryContainerIndex;
    iRegistryContainer = NULL; // deleted by index
    }

// ---------------------------------------------------------
// CSuplSubsessionRegistry::SubSessionFromHandleL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CSuplIPCSubSession* CSuplSubsessionRegistry::SubSessionFromHandleL(TUint aHandle)
    {
    DEBUG_TRACE("SubSessionFromHandleL", __LINE__) 
    return static_cast<CSuplIPCSubSession*> (iRegistryIndex->At(aHandle));
    }

// ---------------------------------------------------------
// CSuplSubsessionRegistry::SubSessionFromIndexL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CSuplIPCSubSession* CSuplSubsessionRegistry::SubSessionFromIndex(TInt aIndex)
    {
    DEBUG_TRACE("SubSessionFromIndex", __LINE__) 
    return static_cast<CSuplIPCSubSession*> ((*iRegistryIndex)[aIndex]);
    }
// ---------------------------------------------------------
// CSuplSubsessionRegistry::Count
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CSuplSubsessionRegistry::Count()
    {
    DEBUG_TRACE("Count", __LINE__) 
    return iRegistryIndex->Count();
    }

// ---------------------------------------------------------
// CSuplSubsessionRegistry::CloseSubSession
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplSubsessionRegistry::CloseSubSession(TUint aHandle)
    {
    DEBUG_TRACE("CloseSubSession", __LINE__) 
    if (iRegistryIndex->At(aHandle))
        {
        // Remove on a CObjectIx does close on the CObject
        iRegistryIndex->Remove(aHandle);
        }
    }

// ---------------------------------------------------------
// CSuplSubsessionRegistry::AddInstanceL
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CSuplSubsessionRegistry::AddInstanceL(CSuplIPCSubSession* aSubSession)
    {
    DEBUG_TRACE("AddInstanceL", __LINE__) 
    iRegistryContainer->AddL(aSubSession);
    return iRegistryIndex->AddL(aSubSession);
    }

// End of File
