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
*   ?description_line
*
*/


// INCLUDE FILES
#include <ImplementationProxy.h>
#include "FT_LandmarksDbManagerUids.hrh"
//#include "CDbManTestPlugin1.h"
#include "CDbManTestPluginBase.h"
//#include "CDbManTestPlugin2.h"
//#include "CDbManTestPlugin3.h"

// ==================== LOCAL FUNCTIONS ====================

// Define the interface UIDs
const TImplementationProxy KPosLmLocalImplTable[] = 
    {
    IMPLEMENTATION_PROXY_ENTRY(0x04440011, CDbManTestPluginBase::NewL),
    //IMPLEMENTATION_PROXY_ENTRY(0x04440012, CDbManTestPlugin2::NewL),
    //IMPLEMENTATION_PROXY_ENTRY(0x04440013, CDbManTestPlugin3::NewL)
    };

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(KPosLmLocalImplTable) / sizeof(TImplementationProxy);

    return KPosLmLocalImplTable;
    }

//  End of File
