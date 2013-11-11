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
* Description:  Factory functions to create the update and delete utilities
*
*/


#include <e32base.h>
#include "mpxcollectionhelperimp.h"
#include "mpxcollectioncachedhelper.h"

#include "mpxcollectionuihelperimp.h"
#include "mpxcollectionhelperfactory.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Factory method to create the collection ui helper
// ---------------------------------------------------------------------------
// 
EXPORT_C MMPXCollectionUiHelper* CMPXCollectionHelperFactory::NewCollectionUiHelperL(const TUid& aModeId)
    {
    return CMPXCollectionUiHelperImp::NewL(aModeId);
    }

// ---------------------------------------------------------------------------
// Factory method to create the collection helper
// ---------------------------------------------------------------------------
//
EXPORT_C MMPXCollectionHelper* CMPXCollectionHelperFactory::NewCollectionHelperL()
    {
    return CMPXCollectionHelperImp::NewL();
    }

// ---------------------------------------------------------------------------
// Factory method to create the cached collection helper
// ---------------------------------------------------------------------------
//
EXPORT_C MMPXCollectionHelper* CMPXCollectionHelperFactory::NewCollectionCachedHelperL()
    {
    return CMPXCollectionCachedHelper::NewL();
    }
