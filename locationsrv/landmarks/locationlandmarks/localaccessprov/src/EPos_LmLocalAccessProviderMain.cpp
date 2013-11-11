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
* Description: Ecom method implementation
*
*
*/


#include <ecom/implementationproxy.h>
#include <EPos_LandmarksUids.hrh>
#include "epos_cposlmlocaldatabase.h"
#include "epos_cposlmlocalcategorymanager.h"
#include "EPos_LmLocalAccessUids.hrh"

// ==================== LOCAL FUNCTIONS ====================

// Define the interface UIDs
const TImplementationProxy KPosLmLocalImplTable[] =
    {
    IMPLEMENTATION_PROXY_ENTRY(
        KPosLmLocalDatabaseImplUid, CPosLmLocalDatabase::OpenL),
    IMPLEMENTATION_PROXY_ENTRY(
        KPosLmLocalCatManagerImplUid, CPosLmLocalCategoryManager::NewL),
    IMPLEMENTATION_PROXY_ENTRY(
        KPosLmLocalDatabaseExtImplUid, CPosLmLocalDatabase::OpenL),
    };

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(KPosLmLocalImplTable) / sizeof(TImplementationProxy);

    return KPosLmLocalImplTable;
    }

