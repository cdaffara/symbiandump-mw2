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
#include <centralrepository.h>
#include "epos_csuplprotocolmanagerbase.h"

#include "epos_csuplsessionmanager.h"
#include "epos_csuplnetinitiatedregistry.h"
#include "epos_csuplnetinitiatedsession.h"
#include "epos_csuplglobal.h"


//#ifdef _DEBUG
_LIT(KTraceFileName, "SUPL_GW::epos_csuplnetinitiatedregistry.cpp");
//#endif

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CSuplNetInitiatedRegistry::CSuplNetInitiatedRegistry()
    {
    }

// EPOC default constructor
void CSuplNetInitiatedRegistry::ConstructL()
    {
    DEBUG_TRACE("ConstructL", __LINE__)            
    iRegistryIndex = CObjectIx::NewL();
    iRegistryContainerIndex = CObjectConIx::NewL();
    iRegistryContainer = iRegistryContainerIndex->CreateL();
    }

// Two-phased constructor
CSuplNetInitiatedRegistry* CSuplNetInitiatedRegistry::NewL()
    {
    DEBUG_TRACE("NewL", __LINE__)            
    CSuplNetInitiatedRegistry* self = new (ELeave) CSuplNetInitiatedRegistry;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CSuplNetInitiatedRegistry::~CSuplNetInitiatedRegistry()
    {
    DEBUG_TRACE("~CSuplNetInitiatedRegistry", __LINE__)            
    delete iRegistryIndex;
    delete iRegistryContainerIndex;
    iRegistryContainer = NULL; // deleted by index
    }

// ---------------------------------------------------------
// CSuplNetInitiatedRegistry::SubSessionFromHandleL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CSuplNetInitiatedSession* CSuplNetInitiatedRegistry::SubSessionFromHandleL(TUint aHandle)
    {
    DEBUG_TRACE("SubSessionFromHandleL", __LINE__) 
    return static_cast<CSuplNetInitiatedSession*> (iRegistryIndex->At(aHandle));
    }

// ---------------------------------------------------------
// CSuplNetInitiatedRegistry::SubSessionFromIndexL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CSuplNetInitiatedSession* CSuplNetInitiatedRegistry::SubSessionFromIndex(TInt aIndex)
    {
    DEBUG_TRACE("SubSessionFromIndex", __LINE__) 
    return static_cast<CSuplNetInitiatedSession*> ((*iRegistryIndex)[aIndex]);
    }
// ---------------------------------------------------------
// CSuplNetInitiatedRegistry::Count
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CSuplNetInitiatedRegistry::Count()
    {
    DEBUG_TRACE("Count", __LINE__) 
    return iRegistryIndex->Count();
    }

// ---------------------------------------------------------
// CSuplNetInitiatedRegistry::CloseSubSession
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplNetInitiatedRegistry::RemoveInstance(TUint aHandle)
    {
    DEBUG_TRACE("CloseSubSession", __LINE__) 
    if (iRegistryIndex->At(aHandle))
        {
        // Remove on a CObjectIx does close on the CObject
        iRegistryIndex->Remove(aHandle);
        }
    }

// ---------------------------------------------------------
// CSuplNetInitiatedRegistry::AddInstanceL
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CSuplNetInitiatedRegistry::AddInstanceL(CSuplNetInitiatedSession* aObj)
    {
    DEBUG_TRACE("AddInstanceL", __LINE__) 
    
    iRegistryContainer->AddL(aObj);
    return iRegistryIndex->AddL(aObj);
   
    
    }

// End of File
