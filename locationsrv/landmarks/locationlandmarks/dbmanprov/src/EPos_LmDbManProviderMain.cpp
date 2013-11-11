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
* Description: 
*                Provider.
*
*/


// INCLUDE FILES
#include <ecom/implementationproxy.h>
#include <EPos_LandmarksDbManagerUids.hrh>
#include "EPos_CPosLmDatabaseManagerImpl.h"

// ==================== LOCAL FUNCTIONS ====================

// Define the interface UIDs
const TImplementationProxy KPosLmLocalImplTable[] =
    {
    IMPLEMENTATION_PROXY_ENTRY(KPosLmDbManagerImplUid,
        CPosLmDatabaseManagerImpl::NewL)
    };

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(KPosLmLocalImplTable) / sizeof(TImplementationProxy);

    return KPosLmLocalImplTable;
    }

//  End of File
