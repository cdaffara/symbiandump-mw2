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
* Description:  This header file declares the abstract factory class for
*                trigger store implementations
*
*/


#ifndef LBT_TRIGGERSTORE_FACTORY_H
#define LBT_TRIGGERSTORE_FACTORY_H

#include <e32cmn.h>

/* Interface that defines the services of a Trigger Store */
class MLbtTriggerStore;


/**
 *  This class provides an abstract factory for all the trigger store managers
 *  
 */
class LbtTriggerStoreFactory
    {
public:
    /**
     * This method returns the number of trigger stores that are supported.
     *
     * @return the number of stores currently available.
     */
    static TInt NumStores();

    /**
     * Create and obtain instances of all the trigger stores in the system.
     *
     * @return a pointer array containing an instance of each trigger store.
     */
    static void CreateTriggerStoresL(RPointerArray<MLbtTriggerStore>& aStoreArray);
    };

#endif // LBT_TRIGGERSTORE_FACTORY_H
