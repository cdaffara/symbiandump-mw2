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
* Description:  This file defines the abstract factory class for
*                trigger store implementations
*
*/


#include "lbttriggerstorerepository.h"
#include "lbtdbtriggersmanager.h"
#include "lbtramtriggersmanager.h"


// ---------------------------------------------------------------------------
// This method returns the number of trigger stores supported in the container
// ---------------------------------------------------------------------------
//
TInt LbtTriggerStoreFactory::NumStores()
    {
    return 2;
    }
    

// ---------------------------------------------------------------------------
// Creates and returns all the available trigger stores.
// ---------------------------------------------------------------------------
//
void LbtTriggerStoreFactory::CreateTriggerStoresL(RPointerArray<MLbtTriggerStore>& aStoreArray)
    {
    CleanupClosePushL(aStoreArray);
    CLbtDbTriggersManager* dbTriggerStore = CLbtDbTriggersManager::NewL();
    CleanupStack::PushL( dbTriggerStore );
    
    aStoreArray.AppendL( dbTriggerStore );
    
    CLbtRamTriggersManager* ramTriggerStore = CLbtRamTriggersManager::NewL();
    CleanupStack::PushL( ramTriggerStore );
    
    aStoreArray.AppendL( ramTriggerStore );
    
    CleanupStack::Pop( 3, &aStoreArray );
    }
