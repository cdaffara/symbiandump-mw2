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
* Description: Class to handle registration and unregistration of subsessions.
*
*
*/


#include "EPos_CPosLmSubsessionRegistry.h"
#include "EPos_CPosLmSubsessionBase.h"

// -----------------------------------------------------------------------------
// CPosLmSubsessionRegistry::CPosLmSubsessionRegistry
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmSubsessionRegistry::CPosLmSubsessionRegistry()
    {
    }

// -----------------------------------------------------------------------------
// CPosLmSubsessionRegistry::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmSubsessionRegistry::ConstructL()
    {
    iRegistryIndex = CObjectIx::NewL();
    iRegistryContainerIndex = CObjectConIx::NewL();
    iRegistryContainer = iRegistryContainerIndex->CreateL();
    }

// -----------------------------------------------------------------------------
// CPosLmSubsessionRegistry::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPosLmSubsessionRegistry* CPosLmSubsessionRegistry::NewL()
    {
    CPosLmSubsessionRegistry* self = new (ELeave) CPosLmSubsessionRegistry;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CPosLmSubsessionRegistry::~CPosLmSubsessionRegistry()
    {
    delete iRegistryIndex;
    delete iRegistryContainerIndex;
    iRegistryContainer = NULL; // deleted by index
    }

// -----------------------------------------------------------------------------
// CPosLmSubsessionRegistry::SubsessionFromHandleL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPosLmSubsessionBase* CPosLmSubsessionRegistry::SubsessionFromHandleL(
    TUint aHandle)
    {
    return static_cast<CPosLmSubsessionBase*>(iRegistryIndex->At(aHandle));
    }

// -----------------------------------------------------------------------------
// CPosLmSubsessionRegistry::SubsessionFromIndex
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPosLmSubsessionBase* CPosLmSubsessionRegistry::SubsessionFromIndex(
    TInt aIndex)
    {
    return static_cast<CPosLmSubsessionBase*>((*iRegistryIndex)[aIndex]);
    }

// -----------------------------------------------------------------------------
// CPosLmSubsessionRegistry::Count
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPosLmSubsessionRegistry::Count()
    {
    return iRegistryContainer->Count();
    }

// -----------------------------------------------------------------------------
// CPosLmSubsessionRegistry::CloseSubsession
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPosLmSubsessionRegistry::CloseSubsession(
    TUint aHandle)
    {
    if (iRegistryIndex->At(aHandle))
        {
        // Remove on a CObjectIx does close on the CObject
        iRegistryIndex->Remove(aHandle);
        return KErrNone;
        }
    return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// CPosLmSubsessionRegistry::AddInstanceL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPosLmSubsessionRegistry::AddInstanceL(
    CPosLmSubsessionBase* aSubsession)
    {
    iRegistryContainer->AddL(aSubsession);
    return iRegistryIndex->AddL(aSubsession);
    }

//  End of File
