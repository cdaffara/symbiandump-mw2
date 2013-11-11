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
* Description: This class manages the collection of protocol-specific database manager
*  implementations.
*
*
*/


// INCLUDE FILES
#include <ecom/ecom.h>
#include <EPos_LandmarksDbManagerUids.hrh>
#include <EPos_LandmarksErrors.h>
#include <EPos_CPosLmDatabaseManagerPluginBase.h>
#include "EPos_CPosLmDbManPluginInfo.h"
#include "EPos_CPosLmDbManPluginStore.h"

// ================= LOCAL FUNCTIONS ========================

void CleanupImplInfoPointerArray(TAny* aArray)
    {
    (reinterpret_cast<RImplInfoPtrArray*>(aArray))->ResetAndDestroy();
    }



// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CPosLmDbManPluginStore::CPosLmDbManPluginStore
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmDbManPluginStore::CPosLmDbManPluginStore()
    {
    }

// -----------------------------------------------------------------------------
// CPosLmDbManPluginStore::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmDbManPluginStore::ConstructL(TAny* aConstructionParams)
    {

    // List implementations that support this protocol
    TUid interfaceUid;
    interfaceUid.iUid = KPosLmDbManagerBaseIfUid;

    RImplInfoPtrArray implInfoArray;
    CleanupStack::PushL(TCleanupItem(CleanupImplInfoPointerArray,
                                     &implInfoArray));

    REComSession::ListImplementationsL(interfaceUid, implInfoArray);
    TInt count = implInfoArray.Count();

    if (count == 0)
        {
        User::Leave(KErrNotSupported);
        }

    for (TInt i = 0; i < count; i++)
        {
        TUid implementationUid =
            reinterpret_cast<CImplementationInformation*>(implInfoArray[i])->
            ImplementationUid();

        CPosLmDbManPluginInfo* plugin =
            CPosLmDbManPluginInfo::NewLC(
                implementationUid, aConstructionParams);

        iPlugins.AppendL(plugin);
        CleanupStack::Pop(plugin);
        }

    CleanupStack::PopAndDestroy(&implInfoArray);

    }

// -----------------------------------------------------------------------------
// CPosLmDbManPluginStore::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPosLmDbManPluginStore* CPosLmDbManPluginStore::NewL(
    TAny* aConstructionParams)
    {
    CPosLmDbManPluginStore* self = new( ELeave ) CPosLmDbManPluginStore;
    CleanupStack::PushL(self);
    self->ConstructL(aConstructionParams);
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CPosLmDbManPluginStore::~CPosLmDbManPluginStore()
    {
    iPlugins.ResetAndDestroy();
    }


// ---------------------------------------------------------
// CPosLmDbManPluginStore::GetInstanceL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPosLmDatabaseManagerPluginBase* CPosLmDbManPluginStore::GetInstanceL(
    const TDesC& aProtocol)
    {
    TBool found = EFalse;
    TInt count = iPlugins.Count();
    TInt index;

    for (index = 0; index < count && !found; index++)
        {
        if (aProtocol == iPlugins[index]->Instance()->Protocol())
            {
            found = ETrue;
            }
        }
    if (!found)
        {
        User::Leave(KErrNotSupported);
        }
    return iPlugins[--index]->Instance();
    }

// ---------------------------------------------------------
// CPosLmDbManPluginStore::NumberOfInstances
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CPosLmDbManPluginStore::NumberOfInstances() const
    {
    return iPlugins.Count();
    }

// ---------------------------------------------------------
// CPosLmDbManPluginStore::GetInstanceAt
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPosLmDatabaseManagerPluginBase* CPosLmDbManPluginStore::GetInstanceAt(
    TInt aIndex)
    {
    return iPlugins[aIndex]->Instance();
    }


// End of File
