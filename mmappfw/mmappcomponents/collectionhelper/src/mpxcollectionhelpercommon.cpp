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
* Description:  Common helper functions for collection helper
*
*/


#include <mpxcollectionframeworkdefs.h>
#include <mpxcollectionplugin.hrh>
#include <mpxcollectionutility.h>
#include <f32file.h>
#include "mpxcollectionhelpercommon.h"

// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// Send initialize command to plugins
// ---------------------------------------------------------------------------
//
void InitializeCollectionPluginsL( MMPXCollectionUtility& aCollectionUtil )
    {
    RArray<TUid> uid;
    CleanupClosePushL( uid );
    
    // Initialize/Merge music collection databases
    TInt err = KErrNone;
    
    uid.AppendL( TUid::Uid(EMPXCollectionPluginMusic) );
    TUid collection = aCollectionUtil.CollectionIDL( uid.Array() );
    // Cannot guarantee that KLocalCollection plugin always exists, therefore,
    // ignore the case error is KErrNotSupported
    TRAP(err, aCollectionUtil.Collection().CommandL( EMcCmdCollectionInit, 
                                                     collection.iUid ) );
    if( err != KErrNone && err != KErrNotSupported)
        {
        User::Leave( err );
        }
    
    // Initialize/Merge podcast databases
    uid.Reset();
    uid.AppendL( TUid::Uid(EMPXCollectionPluginPodCast) );
    collection = aCollectionUtil.CollectionIDL( uid.Array() );
    // Cannot guarantee that KPodcastCollection plugin always exists, 
    // therefore, ignore the case error is KErrNotSupported
    TRAP(err, aCollectionUtil.Collection().CommandL( EMcCmdCollectionInit, 
                                                     collection.iUid ) );
    if( err != KErrNone && err != KErrNotSupported)
        {
        User::Leave( err );
        }
    CleanupStack::PopAndDestroy( &uid );
    }

