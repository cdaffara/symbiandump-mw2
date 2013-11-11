/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Table of plugins for ECom
*
*/





// INCLUDE FILES
#include <implementationproxy.h>

#include "xdmdmadapter.h"
#include "xdmdmuid.h"


// -----------------------------------------------------------------------------
// TImplementationProxy ImplementationTable[]
// Needed because of ECOM architecture
// -----------------------------------------------------------------------------
const TImplementationProxy ImplementationTable[] = 
    {
#ifdef __EABI__    
    IMPLEMENTATION_PROXY_ENTRY( KXdmDmImplUid, CXdmDMAdapter::NewL )
#else
    { { KXdmDmImplUid }, CXdmDMAdapter::NewL },
#endif
    };

// -----------------------------------------------------------------------------
// ImplementationGroupProxy()
// Needed because of ECOM architecture
// -----------------------------------------------------------------------------
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& 
                                                               aTableCount )
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
#ifdef _DEBUG
    CXdmDMAdapter::WriteToLog(_L8( "CXdmDMAdapter:ImplementationGroupProxy") );
#endif
    return ImplementationTable;
    }
